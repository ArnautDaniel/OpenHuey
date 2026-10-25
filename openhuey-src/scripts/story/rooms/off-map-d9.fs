\ story/rooms/off-map-d9.fs - the event scripts of room off-map-d9 ($D9; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d9
USING: room-names facts story.words story.shared flag-names story.clock ;

\ ---- this room's own event bits and script variables ----
0 constant started                     \ (bit 0) the run begun (the world let go; Hewie played)
1 constant cond-was-off                \ (bit 1) the room's condition was false last frame: its hook waits a frame once it holds

\ room 0xD9: starts the countdown clock (clock_start: progress +0x1FBEC1 on, Hewie restarted,
\ the camera director +0x40 14, the time zeroed).
: off-map-d9.cmd00 ( -- )  clock-start ;
\ room 0xD9: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d9.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ (as Room48_Cmd04) room 0x48 (Room48_Cmd04_ptmf): the player's Character_ChooseExit(0)
: off-map-d9.cmd02 ( -- )  fiona-who 0 char-to-exit ;   \ (Fiona to exit 0: Character_ChooseExit)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d9.cond00? ( -- flag )  true ;

: off-map-d9.enter ( -- )   \ 00447E80
    $19 1.0 0 bgm
    hewie-controlled? if
        started ebit-set
    then
;

: off-map-d9.char-enter ( -- )   \ 00447E90
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? if
            $80 exit-taken? if
                0 exit-prepare
                off-map-d9.cmd00
                $2F 0 pvar-set
                fiona-who char-activate
                fiona-who $23 -2.79 -21.69 81 char-to-xz
                fiona-who 0 0 char-camera
                hewie-who char-activate
                $D9 0 119 hewie-to-room
                hewie-who $77 -14.67 -14.87 120 char-to-xz
                0 0 player-camera
                stalker-who char-activate
                $FE $D8 123 2 stalker-to-room
                summoner-own-cooldown
                $2E 0 pvar? if
                    0 music-stage
                else
                    3 music-stage
                then
                2 0 0 music
                4 0 0 music
                off-map-d9.cmd02
                0 hewie-who 0 action
            then
        then
    then
;

: off-map-d9.phase1 ( -- )   \ 00447F40
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
;

: off-map-d9.phase3 ( -- )   \ 00447F60
    off-map-d9.cond00? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-d9.cmd01
    then then
;

: off-map-d9.act00 ( -- )   \ 00447F78
    self-wait-done
    camera-restart
    world-held state-flag-clear
    $F 1 fade
    wait-fade
    started ebit-set
    self-idle-or-end
;

\ ---- registered ----
off-map-d9 register-scripts   \ (its phases and actions, by their names)
