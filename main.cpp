#include <clocale>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <regex>
#include <unistd.h>
#include "thread_helper.hpp"
#include "config.hpp"
#include "cscroll.hpp"
#include "dbus_interface.hpp"

#ifdef I3

void update_i3() {
    char *workspaces = get_stdout(COMMAND_GET_I3_WORKSPACES);
    int ws_count = atoi(workspaces);
    free(workspaces);
    if (ws_count > 0) {
        len = LENGTH - ws_count * I3;
    } else {
        len = LENGTH;
    }
}

#endif

int main(int, char**) {
    setlocale(LC_ALL, "");
    if (strstr(PLAYER, "playerctl") != NULL) {
        system(COMMAND_SETUP_PLAYERCTL);
    }
    full = (char*) calloc(maxLength, sizeof(char));
    int time = 0;

    DBusInterface interface;

    while (true) {
        interface.update_buttons();
        DBusInterface::PlaybackStatus status = interface.get_status();
        if (status == DBusInterface::PlaybackStatus::Offline) {
            std::cout << std::endl << std::flush;
            usleep(1000000*DELAY);
            continue;
        }

        if (time == 0 || (INTERVAL > 0 && time % INTERVAL == 0)) {
            interface.update_cscroller();
        }

        #ifdef ICONS
            std::cout << interface.get_prefix();
        #endif

        #ifndef FORCE_ROTATE
        if (strlen(full) > len) {
        #endif
            if (status == DBusInterface::PlaybackStatus::Playing) {
                rotateText(2);
                offset++;
                offset %= ((strlen(full) > len) ? len : strlen(full));
            } else {
                rotateText(0);
            }
        #ifndef FORCE_ROTATE
        } else {
            rotateText(1);
        }
        #endif
        #ifdef I3
        update_i3();
        #endif

        std::cout << std::endl << std::flush;

        usleep(1000000*DELAY);
        time++;
    }
}

