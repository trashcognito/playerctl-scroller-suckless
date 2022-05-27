#include "dbus_interface.hpp"
#include "dbus_interface.hpp"

#include <array>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "config.hpp"
#include "giomm/dbusconnection.h"
#include "giomm/dbusmessage.h"
#include "giomm/dbusproxy.h"
#include "glibmm/unicode.h"
#include "glibmm/ustring.h"
#include "glibmm/value.h"
#include "glibmm/variant.h"
#include "glibmm/variantdict.h"
#include "glibmm/variantiter.h"
#include "glibmm/varianttype.h"
#include "sigc++/reference_wrapper.h"
#include "thread_helper.hpp"
#include "dbus_interface.hpp"
#include "cscroll.hpp"

DBusInterface::DBusInterface(Glib::RefPtr<Gio::DBus::Connection> conn) {
    button_status = PlaybackStatus::Playing;
    dbus_conn = conn;
    if (strstr(PLAYER, "playerctl") != NULL) {
        try {
            conn->call_sync("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "GetAll", Glib::Variant<std::tuple<Glib::VariantBase>>::create_tuple(std::vector<Glib::VariantBase>({Glib::Variant<Glib::ustring>::create("org.mpris.MediaPlayer2.Player")})), "org.mpris.MediaPlayer2.playerctld");
        } catch (Glib::Error e) {
            //pass
        }
    }
};

std::string DBusInterface::get_prefix() {
    
    auto reply = dbus_conn->call_sync("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get", Glib::Variant<std::tuple<Glib::VariantBase>>::create_tuple(std::vector<Glib::VariantBase>({Glib::Variant<Glib::ustring>::create("com.github.altdesktop.playerctld"), Glib::Variant<Glib::ustring>::create("PlayerNames")})), "org.mpris.MediaPlayer2.playerctld");
    
    GVariant *output;
    g_variant_get_child(reply.gobj(), 0, "v", &output);
    GVariant *mystring = g_variant_get_child_value(output, 0);
    auto res = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(Glib::wrap(mystring)).get();

    if (res.find("chrom") != Glib::ustring::npos) {
        return std::string("");
    }
    if (res.find("firefox") != Glib::ustring::npos) {
        return std::string("");
    }
    if (res.find("spotify") != Glib::ustring::npos) {
        return std::string("");
    }
    return std::string("");
}

Glib::ustring DBusInterface::get_dest() {
    auto reply = dbus_conn->call_sync("/org/freedesktop/DBus", "org.freedesktop.DBus", "ListNames", Glib::Variant<std::tuple<Glib::VariantBase>>::create_tuple(std::vector<Glib::VariantBase>()), "org.freedesktop.DBus");
    auto output = Glib::wrap(g_variant_get_child_value(reply.gobj(), 0));
    gsize len;
    auto my_list = g_variant_get_strv(output.gobj(), &len);
    Glib::ustring retval("");
    for (int i=0;i<len;i++) {
        Glib::ustring temp(my_list[i]);
        if (temp.find("mpris") != Glib::ustring::npos) {
            if (temp.find(PLAYER) != Glib::ustring::npos) {
                retval = temp;
                break;
            }
        }
    }
    g_free(my_list);
    return retval;
}

DBusInterface::PlaybackStatus DBusInterface::get_status() {
    Glib::ustring dest = this->get_dest();
    Glib::VariantContainerBase reply;
    try {
        reply = dbus_conn->call_sync("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get", Glib::Variant<std::tuple<Glib::VariantBase>>::create_tuple(std::vector<Glib::VariantBase>({Glib::Variant<Glib::ustring>::create("org.mpris.MediaPlayer2.Player"), Glib::Variant<Glib::ustring>::create("PlaybackStatus")})), dest);
    } catch (Gio::Error e) {
        return PlaybackStatus::Offline;
    }
    GVariant *output;
    g_variant_get_child(reply.gobj(), 0, "v", &output);
    auto res = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(Glib::wrap(output)).get();
    if (res.length() == 0) {
        return PlaybackStatus::Offline;
    }
    
    if (res.find("Playing") != Glib::ustring::npos) {
        return PlaybackStatus::Playing;
    }
    if (res.find("Paused") != Glib::ustring::npos) {
        return PlaybackStatus::Paused;
    }

    return PlaybackStatus::Offline;
}

void DBusInterface::update_cscroller() {
    Glib::ustring dest = this->get_dest();

    auto reply = dbus_conn->call_sync("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get", Glib::Variant<std::tuple<Glib::VariantBase>>::create_tuple(std::vector<Glib::VariantBase>({Glib::Variant<Glib::ustring>::create("org.mpris.MediaPlayer2.Player"), Glib::Variant<Glib::ustring>::create("Metadata")})), dest);
    GVariant *output;
    g_variant_get_child(reply.gobj(), 0, "v", &output);
    auto wrapped = Glib::wrap(output);
    auto res = Glib::VariantDict::create(wrapped);
    
    Glib::ustring title;
    res->lookup_value("xesam:title", title);
    
    Glib::ustring artist;
    Glib::VariantBase artist_container;
    if (res->lookup_value_variant("xesam:artist", Glib::VARIANT_TYPE_ARRAY, artist_container)) {
        auto artist_obj = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(Glib::wrap(g_variant_get_child_value(artist_container.gobj(), 0)));
        artist = artist_obj.get();
    }

    if (title.length() == 0) {
        Glib::ustring uri_r;
        res->lookup_value("xesam:url", uri_r);
        Glib::ustring uri = uri_r.substr(uri_r.find_last_of('/')+1);
        
        //escape uri
        std::stringstream output;
        for (int i = 0; i < uri.length(); i++) {
            if (uri[i] == '%') {
                if (i + 2 < uri.length()) {
                    output << (char)((Glib::Unicode::xdigit_value(uri[i+1]) << 4) | (Glib::Unicode::xdigit_value(uri[i+2])));
                    i += 2;
                }

            } else if (uri[i] == '+') {
                output << ' ';
            } else {
                output << (char)uri[i];
            }
        }
        title = output.str();
    }


    std::string prompt;
    if (artist.length() != 0) {
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