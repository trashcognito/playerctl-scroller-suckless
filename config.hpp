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
#define COMMAND_HIDE_PLAY_PAUSE_POST " action \"#" MODULE_PLAY_PAUSE ".hook.2\" > /dev/null"
#define COMMAND_OPEN_PLAY_PAUSE_POST " action \"#" MODULE_PLAY_PAUSE ".hook.0\" > /dev/null"
#define COMMAND_PAUSE_PLAY_PAUSE_POST " action \"#" MODULE_PLAY_PAUSE ".hook.1\" > /dev/null"
#define COMMAND_HIDE_PREV_POST " action \"#" MODULE_PREV ".hook.1\" > /dev/null"
#define COMMAND_SHOW_PREV_POST " action \"#" MODULE_PREV ".hook.0\" > /dev/null"
#define COMMAND_HIDE_NEXT_POST " action \"#" MODULE_NEXT ".hook.1\" > /dev/null"
#define COMMAND_SHOW_NEXT_POST " action \"#" MODULE_NEXT ".hook.0\" > /dev/null"
#define INTERVAL (int)((float)INTERVAL_TIME / DELAY)
#ifdef I3
#define COMMAND_GET_I3_WORKSPACES "i3-msg -t get_workspaces 2> /dev/null | grep -o \"num\" | wc -l"
#endif