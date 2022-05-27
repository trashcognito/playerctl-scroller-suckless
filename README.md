An updated version of https://github.com/jan146/playerctl-scroller . Now without the callbacks to the script file.

Configure the program through config.hpp .

See https://github.com/jan146/playerctl-scroller/blob/main/README.md for more details.

This repository is intended for use with the following polybar config modules:

```ini
[module/music]
type = custom/script
tail = true
format = <label>
exec = playerctl-scroller-suckless

[module/music-play-pause]
type = custom/ipc
hook-0 = echo "  "
hook-1 = echo "  "
hook-2 = ""
initial = 1
click-left = playerctl play-pause

[module/music-prev]
type = custom/ipc
hook-0 = echo "      "
hook-1 = ":"
initial = 1
click-left = playerctl previous

[module/music-next]
type = custom/ipc
hook-0 = echo  "      "
hook-1 = ""
initial = 1
click-left = playerctl next
```

Note: This project doesn't actually have anything to do with suckless.org . I just renamed it that way because suckless also typically uses this source-based configuration philosophy.
