\ story/rooms/off-map-d9.fs - the event scripts of room off-map-d9 ($D9; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d9
USING: room-names story.words story.shared ;

\ room 0xD9: starts the countdown clock (clock_start: progress +0x1FBEC1 on, Hewie restarted,
\ the camera director +0x40 14, the time zeroed).
: off-map-d9.cmd00 ( -- )  s" off-map-d9.cmd00" stub-step ;
\ room 0xD9: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d9.cmd01 ( -- )  s" off-map-d9.cmd01" stub-step ;
\ (as Room48_Cmd04) room 0x48 (Room48_Cmd04_ptmf): the player's Character_ChooseExit(0)
: off-map-d9.cmd02 ( -- )  s" off-map-d9.cmd02" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-d9.cond00? ( -- flag )  s" off-map-d9.cond00?" stub-flag ;

: off-map-d9.enter ( -- )   \ 00447E80
    $19 1.0 0 bgm
    hewie-controlled? if
        0 ebit-set
    then
;

: off-map-d9.char-enter ( -- )   \ 00447E90
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        0 self-is? if
            $80 exit-taken? if
                0 exit-prepare
                off-map-d9.cmd00
                $2F 0 pvar-set
                0 char-activate
                0 $23 -2.79 -21.69 81 char-to-xz
                0 0 0 char-camera
                1 char-activate
                $D9 0 119 hewie-to-room
                1 $77 -14.67 -14.87 120 char-to-xz
                hewie-controlled? not if
                    0 0 0 char-camera
                    0 camera-follow
                else
                    1 0 0 char-camera
                    1 camera-follow
                then
                $FE char-activate
                $FE $D8 123 2 stalker-to-room
                stalker-item-cooldown
                $2E 0 pvar? if
                    0 music-stage
                else
                    3 music-stage
                then
                2 0 0 music
                4 0 0 music
                off-map-d9.cmd02
                0 1 0 action
            then
        then
    then
;

: off-map-d9.phase1 ( -- )   \ 00447F40
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
;

: off-map-d9.phase3 ( -- )   \ 00447F60
    off-map-d9.cond00? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
    else
        off-map-d9.cmd01
    then then
;

: off-map-d9.act00 ( -- )   \ 00447F78
    self-wait-done
    camera-restart
    8 state-flag-clear
    $F 1 fade
    wait-fade
    0 ebit-set
    self-idle-or-end
;

\ ---- registered ----
' off-map-d9.enter off-map-d9 0 room-script!
' off-map-d9.char-enter off-map-d9 6 room-script!
' off-map-d9.phase1 off-map-d9 1 room-script!
' off-map-d9.phase3 off-map-d9 3 room-script!
' off-map-d9.act00 off-map-d9 $00 action-script!
