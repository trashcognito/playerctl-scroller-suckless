#pragma once

#include <string>

class DBusInterface {
    public:
    enum class PlaybackStatus {
        Playing,
        Paused,
        Offline
    };
    DBusInterface();
    std::string get_prefix();
    PlaybackStatus get_status();
    void update_cscroller();
    void update_buttons();
    private:
    PlaybackStatus button_status;
};