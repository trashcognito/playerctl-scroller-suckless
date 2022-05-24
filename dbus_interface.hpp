#pragma once

#include <string>
#include <giomm.h>
#include <glibmm.h>

class DBusInterface {
    public:
    enum class PlaybackStatus {
        Playing,
        Paused,
        Offline
    };
    DBusInterface(Glib::RefPtr<Gio::DBus::Connection>);
    std::string get_prefix();
    PlaybackStatus get_status();
    void update_cscroller();
    void update_buttons();
    private:
    Glib::RefPtr<Gio::DBus::Connection> dbus_conn;
    PlaybackStatus button_status;
};