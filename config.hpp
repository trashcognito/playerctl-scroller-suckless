#pragma once

#define BAR "bottom"
#define DELAY 0.2
#define INTERVAL_TIME 1
#define MODULE_PLAY_PAUSE "music-play-pause"
#define MODULE_NEXT "music-next"
#define MODULE_PREV "music-prev"
#define PLAYER "playerctl"
//#define FORCE_ROTATE
#define LENGTH 30
#define SEPARATOR "  "
#define MIDDLE " — "
#define ICONS

//#define I3 1


#define PID_COMMAND "pgrep -a \"polybar\" | grep \"" BAR "\" | cut -d\" \" -f1"
#define COMMAND_PID_PREFIX "polybar-msg -p "
#define COMMAND_HIDE_PLAY_PAUSE_POST " action \"#" MODULE_PLAY_PAUSE ".hook.3\" > /dev/null"
#define COMMAND_OPEN_PLAY_PAUSE_POST " action \"#" MODULE_PLAY_PAUSE ".hook.1\" > /dev/null"
#define COMMAND_PAUSE_PLAY_PAUSE_POST " action \"#" MODULE_PLAY_PAUSE ".hook.2\" > /dev/null"
#define COMMAND_HIDE_PREV_POST " action \"#" MODULE_PREV ".hook.2\" > /dev/null"
#define COMMAND_SHOW_PREV_POST " action \"#" MODULE_PREV ".hook.1\" > /dev/null"
#define COMMAND_HIDE_NEXT_POST " action \"#" MODULE_NEXT ".hook.2\" > /dev/null"
#define COMMAND_SHOW_NEXT_POST " action \"#" MODULE_NEXT ".hook.1\" > /dev/null"
#define INTERVAL (int)((float)INTERVAL_TIME / DELAY)
#define COMMAND_GET_DEST "dbus-send --print-reply --dest=org.freedesktop.DBus /org/freedesktop/DBus org.freedesktop.DBus.ListNames 2> /dev/null | grep mpris | grep " PLAYER " | sed 's/.*string \"//g;s/.$//g'"
#define COMMAND_STATUS_PRE "dbus-send --print-reply --dest="
#define COMMAND_STATUS_POST " /org/mpris/MediaPlayer2 org.freedesktop.DBus.Properties.Get string:org.mpris.MediaPlayer2.Player string:PlaybackStatus 2> /dev/null | grep variant | sed 's/.*string \"//g;s/.$//g'"
#define COMMAND_INSTANCE "dbus-send --print-reply --dest=org.mpris.MediaPlayer2.playerctld /org/mpris/MediaPlayer2 org.freedesktop.DBus.Properties.Get string:\"com.github.altdesktop.playerctld\" string:\"PlayerNames\" 2> /dev/null | grep string | head -1 | sed 's/.*Player2\\.//g;s/.$//g;s/\\..*//g'"
#define COMMAND_GET_METADATA_PRE "dbus-send --print-reply --dest="
#define COMMAND_GET_METADATA_POST " /org/mpris/MediaPlayer2 org.freedesktop.DBus.Properties.Get string:\"org.mpris.MediaPlayer2.Player\" string:\"Metadata\" 2> /dev/null"
#define COMMAND_FUNNEL_ARTIST "grep -A 2 'artist' | tail -1 | sed 's/.*string \"//g;s/.$//g'"
#define COMMAND_FUNNEL_TITLE_ONE "grep -A 1 'title' | tail -1 | sed 's/.*string \"//g;s/.$//g'"
#define COMMAND_FUNNEL_TITLE_TWO "grep -A 1 'url' | tail -1 | sed 's/.*string \"//g;s/.$//g' | rev | cut -d/ -f1 | rev | sed 's@+@ @g;s@%@\\x@g' | xargs -0 printf %b | sed 's/\"/\\\\\"/g'"
#ifdef I3
#define COMMAND_GET_I3_WORKSPACES "i3-msg -t get_workspaces 2> /dev/null | grep -o \"num\" | wc -l"
#endif
#define COMMAND_SETUP_PLAYERCTL "dbus-send --print-reply --dest=org.mpris.MediaPlayer2.playerctld /org/mpris/MediaPlayer2 org.freedesktop.DBus.Properties.GetAll string:\"org.mpris.MediaPlayer2.Player\" > /dev/null 2>/dev/null"