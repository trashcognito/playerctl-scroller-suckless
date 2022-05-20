#include "dbus_interface.hpp"
#include "dbus_interface.hpp"

#include <cstdlib>
#include <regex>
#include "config.hpp"
#include "thread_helper.hpp"
#include "dbus_interface.hpp"
#include "cscroll.hpp"

DBusInterface::DBusInterface() {
    button_status = PlaybackStatus::Playing;
};

std::string DBusInterface::get_prefix() {
    char *temp = get_stdout(COMMAND_INSTANCE);
    std::string instance(temp);
    free(temp);

    if (std::regex_match(instance, std::regex("/chrom.*/"))) {
        return std::string("");
    }
    if (instance == "firefox") {
        return std::string("");
    }
    if (std::regex_match(instance, std::regex("/spotify.*/"))) {
        return std::string("");
    }
    return std::string("");
}

DBusInterface::PlaybackStatus DBusInterface::get_status() {
    char *dest = get_stdout(COMMAND_GET_DEST);
    if (strlen(dest) == 0) {
        free(dest);
        return PlaybackStatus::Offline;
    }
    std::string get_status_command;

    get_status_command.append(COMMAND_STATUS_PRE);
    get_status_command.append(dest);
    get_status_command.append(COMMAND_STATUS_POST);
    free(dest);

    char *temp = get_stdout(get_status_command.c_str());
    std::string status(temp);
    free(temp);
    
    if (status.length() == 0) {
        return PlaybackStatus::Offline;
    }
    
    if (status.find("Playing") != std::string::npos) {
        return PlaybackStatus::Playing;
    }
    if (status.find("Paused") != std::string::npos) {
        return PlaybackStatus::Paused;
    }

    return PlaybackStatus::Offline;
}

void DBusInterface::update_cscroller() {
    char *dest = get_stdout(COMMAND_GET_DEST);
    if (strlen(dest) == 0) {
        free(dest);
        return;
    }
    std::string get_metadata_command;

    get_metadata_command.append(COMMAND_GET_METADATA_PRE);
    get_metadata_command.append(dest);
    get_metadata_command.append(COMMAND_GET_METADATA_POST);
    free(dest);

    char *temp = get_stdout_multiline(get_metadata_command.c_str());
    std::string metadata(temp);
    free(temp);

    temp = funnel_command(COMMAND_FUNNEL_TITLE_ONE, metadata.c_str());
    std::string title(temp);
    free(temp);
    title = title.substr(0, title.find("\n"));

    if (title.length() == 0) {
        temp = funnel_command(COMMAND_FUNNEL_TITLE_TWO, metadata.c_str());
        title = std::string(temp);
        free(temp);
    }

    temp = funnel_command(COMMAND_FUNNEL_ARTIST, metadata.c_str());
    std::string artist(temp);
    free(temp);
    artist = artist.substr(0, artist.find("\n"));

    std::string prompt;
    if (artist.length() != 0 && artist != std::string("7\U00000004")) {
        prompt.append(artist);
        prompt.append(MIDDLE);
    }
    prompt.append(title);

    #ifdef SEPARATOR

    #ifndef FORCE_ROTATE
        if (prompt.length() > len) {
    #endif
    
            prompt.append(SEPARATOR);
    
    #ifndef FORCE_ROTATE
        }
    #endif

    #endif
    if (strcmp(full, prompt.c_str()) != 0) {
        free(full);
        full = (char *)malloc(prompt.length() + 1);
        memcpy(full, prompt.c_str(), prompt.length());
        full[prompt.length()] = 0x00;
        offset = 0;
    }
}

void DBusInterface::update_buttons() { 
    PlaybackStatus current = get_status();
    if (current == button_status) {
        return;
    }
    std::string cmd_base(COMMAND_PID_PREFIX);
    char *pid = get_stdout(PID_COMMAND);
    cmd_base.append(pid);
    free(pid);
    if (current == PlaybackStatus::Offline) {
        std::string hide_prev(cmd_base);
        std::string hide_play_pause(cmd_base);
        std::string hide_next(cmd_base);

        hide_prev.append(COMMAND_HIDE_PREV_POST);
        hide_play_pause.append(COMMAND_HIDE_PLAY_PAUSE_POST);
        hide_next.append(COMMAND_HIDE_NEXT_POST);

        system(hide_prev.c_str());
        system(hide_play_pause.c_str());
        system(hide_next.c_str());  
    } else {
        if (button_status == PlaybackStatus::Offline) {
            std::string show_prev(cmd_base);
            std::string show_next(cmd_base);

            show_prev.append(COMMAND_SHOW_PREV_POST);
            show_next.append(COMMAND_SHOW_NEXT_POST);

            system(show_prev.c_str());
            system(show_next.c_str());
        }
        if (current == PlaybackStatus::Playing) {
            std::string open_play_pause(cmd_base);

            open_play_pause.append(COMMAND_OPEN_PLAY_PAUSE_POST);

            system(open_play_pause.c_str());
        } else {
            // current == PlaybackStatus::Paused
            std::string close_play_pause(cmd_base);

            close_play_pause.append(COMMAND_PAUSE_PLAY_PAUSE_POST);

            system(close_play_pause.c_str());

        }
    }
    button_status = current;
}