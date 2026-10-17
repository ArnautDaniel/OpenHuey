\ story/rooms/off-map-e9.fs - the event scripts of room off-map-e9 ($E9; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e9
USING: room-names story.words story.shared flag-names story.clock floor-flags ;

\ ---- this room's own event bits and script variables ----
0 constant cond-was-off                \ (bit 0) the room's condition was false last frame: its hook waits a frame once it holds
1 constant area16-reacted              \ (bit 1) her and Hewie's reaction in area $10 made

\ room 0xE9: stops the countdown clock (clock_stop: progress +0x1FBEC1 off, the camera director
\ +0x40 -1).
: off-map-e9.cmd00 ( -- )  clock-stop ;
\ room 0xE9: saves the countdown clock's time in script variables 0..2 (clock_save).
: off-map-e9.cmd01 ( -- )  clock-save ;
\ (as Room00_Cmd00) the same four spots for bytes 3..6
: off-map-e9.cmd02 ( b0 b1 -- )  3 grey-glow ;   \ (as front-garden-1's: the four spots for bytes 3..6)
\ room 0xE9: draws the countdown clock, frozen at the saved time while event flag 1 is set
\ (clock_draw_saved; a frame hook).
: off-map-e9.cmd03 ( -- )  clock-draw-saved ;   \ (each frame: the clock, or the time kept)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e9.cond00? ( -- flag )  true ;

: off-map-e9.enter ( -- )   \ 0044A2C0
    $2E 1 pvar? if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $80C0F2AA 0 1 screen-blend
        3 part-shown
        4 part-shown
        5 part-shown
        6 part-shown
        7 part-shown
        8 part-shown
        3 0 off-map-e9.cmd02
        4 0 off-map-e9.cmd02
        5 0 off-map-e9.cmd02
        6 0 off-map-e9.cmd02
        $10 -368.0 100.0 55.0 $A $80 $80 $80 $40 specks
        $A -332.0 100.8 -12.0 $E $80 $80 $80 $40 specks
        $10 -332.0 100.8 165.0 $E $80 $80 $80 $40 specks
        $10 -368.0 100.8 165.0 $C $80 $80 $80 $40 specks
        $A -327.5 82.0 236.3 $E $80 $80 $80 $50 specks
        $A -372.5 82.0 236.3 $E $80 $80 $80 $50 specks
    then
    0 part-hidden
;

: off-map-e9.char-enter ( -- )   \ 0044A380
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
;

: off-map-e9.phase1 ( -- )   \ 0044A3C0
    0 exit-usable? if
        0 exit-check
    then
    6 1 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 2 0 1 chars-area-camera
    9 0 -1 1 chars-area-camera
    $2E 3 pvar? if
        fiona-who $C char-entered-area? if
            $1E chance? if
                0 8 track 0 action
            then
        then
        fiona-who $C char-left-area? if
            $1E chance? if
                0 8 track 0 action
            then
        then
        fiona-who $D char-entered-area? if
            $1E chance? if
                0 7 track 0 action
            then
        then
        fiona-who $D char-left-area? if
            $1E chance? if
                0 7 track 0 action
            then
        then
        fiona-who $E char-entered-area? if
            $1E chance? if
                0 6 track 0 action
            then
        then
        fiona-who $E char-left-area? if
            $1E chance? if
                0 5 track 0 action
            then
        then
        $14 chance? if
            1 chance? if
                0 8 track 0 action
            else 1 chance? if
                0 7 track 0 action
            else 1 chance? if
                0 6 track 0 action
            else 1 chance? if
                0 5 track 0 action
            then then then then
        then
    then
    area16-reacted ebit? not if
        fiona-who $10 char-in-area? if
            area16-reacted ebit-set
            panic-eased
            0 fiona-who 1 action-force
            0 hewie-who 2 action-force
        then
    then
;

: off-map-e9.phase3 ( -- )   \ 0044A470
    camera-setup-changed? if
        2 camera-mode? if
            1.0 1.0 100.0 200.0 depth-range
        else
            depth-range-off
        then
    then
    -370.0 76.0 178.0 -370.0 76.0 109.0 -370.0 20.0 178.0 -370.0 20.0 109.0 lights-doorway
    2 camera-mode? if
        -380.0 74.0 250.0 -368.0 74.0 175.0 -380.0 22.0 250.0 -368.0 22.0 175.0 lights-doorway
    then
    -370.0 76.0 110.0 -370.0 76.0 41.0 -370.0 20.0 110.0 -370.0 20.0 41.0 lights-doorway
    -370.0 76.0 38.0 -370.0 76.0 -8.0 -370.0 20.0 39.0 -370.0 20.0 -8.0 lights-doorway
    off-map-e9.cond00? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-e9.cmd03
    then then
;

: off-map-e9.act00 ( -- )   \ 0044A570
    8 track self-is? if
        3 part-hidden
        3 1 off-map-e9.cmd02
        yield
        3 part-shown
        3 0 off-map-e9.cmd02
        yield
        3 part-hidden
        3 1 off-map-e9.cmd02
        4 wait-frames
        $32 chance? if
            3 part-shown
            3 0 off-map-e9.cmd02
            2 wait-frames
            3 part-hidden
            3 1 off-map-e9.cmd02
            yield
            3 part-shown
            3 0 off-map-e9.cmd02
            yield
            3 part-hidden
            3 1 off-map-e9.cmd02
            8 wait-frames
        then
        $19 chance? if
            3 part-shown
            3 0 off-map-e9.cmd02
            2 wait-frames
            3 part-hidden
            3 1 off-map-e9.cmd02
            yield
            3 part-shown
            3 0 off-map-e9.cmd02
            yield
            3 part-hidden
            3 1 off-map-e9.cmd02
            16 wait-frames
        then
        5 chance? if
            3 part-shown
            3 0 off-map-e9.cmd02
            yield
            3 part-hidden
            3 1 off-map-e9.cmd02
            yield
            3 part-shown
            3 0 off-map-e9.cmd02
            yield
            3 part-hidden
            3 1 off-map-e9.cmd02
            $20 wait-frames
        then
        3 part-shown
        3 0 off-map-e9.cmd02
        yield
        3 part-hidden
        3 1 off-map-e9.cmd02
        yield
        3 part-shown
        3 0 off-map-e9.cmd02
    else 7 track self-is? if
        4 part-hidden
        4 1 off-map-e9.cmd02
        yield
        4 part-shown
        4 0 off-map-e9.cmd02
        yield
        4 part-hidden
        4 1 off-map-e9.cmd02
        4 wait-frames
        $32 chance? if
            4 part-shown
            4 0 off-map-e9.cmd02
            2 wait-frames
            4 part-hidden
            4 1 off-map-e9.cmd02
            yield
            4 part-shown
            4 0 off-map-e9.cmd02
            yield
            4 part-hidden
            4 1 off-map-e9.cmd02
            8 wait-frames
        then
        $19 chance? if
            4 part-shown
            4 0 off-map-e9.cmd02
            2 wait-frames
            4 part-hidden
            4 1 off-map-e9.cmd02
            yield
            4 part-shown
            4 0 off-map-e9.cmd02
            yield
            4 part-hidden
            4 1 off-map-e9.cmd02
            16 wait-frames
        then
        5 chance? if
            4 part-shown
            4 0 off-map-e9.cmd02
            yield
            4 part-hidden
            4 1 off-map-e9.cmd02
            yield
            4 part-shown
            4 0 off-map-e9.cmd02
            yield
            4 part-hidden
            4 1 off-map-e9.cmd02
            $20 wait-frames
        then
        4 part-shown
        4 0 off-map-e9.cmd02
        yield
        4 part-hidden
        4 1 off-map-e9.cmd02
        yield
        4 part-shown
        4 0 off-map-e9.cmd02
    else 6 track self-is? if
        5 part-hidden
        5 1 off-map-e9.cmd02
        yield
        5 part-shown
        5 0 off-map-e9.cmd02
        yield
        5 part-hidden
        5 1 off-map-e9.cmd02
        4 wait-frames
        $32 chance? if
            5 part-shown
            5 0 off-map-e9.cmd02
            2 wait-frames
            5 part-hidden
            5 1 off-map-e9.cmd02
            yield
            5 part-shown
            5 0 off-map-e9.cmd02
            yield
            5 part-hidden
            5 1 off-map-e9.cmd02
            8 wait-frames
        then
        $19 chance? if
            5 part-shown
            5 0 off-map-e9.cmd02
            2 wait-frames
            5 part-hidden
            5 1 off-map-e9.cmd02
            yield
            5 part-shown
            5 0 off-map-e9.cmd02
            yield
            5 part-hidden
            5 1 off-map-e9.cmd02
            16 wait-frames
        then
        5 chance? if
            5 part-shown
            5 0 off-map-e9.cmd02
            yield
            5 part-hidden
            5 1 off-map-e9.cmd02
            yield
            5 part-shown
            5 0 off-map-e9.cmd02
            yield
            5 part-hidden
            5 1 off-map-e9.cmd02
            $20 wait-frames
        then
        5 part-shown
        5 0 off-map-e9.cmd02
        yield
        5 part-hidden
        5 1 off-map-e9.cmd02
        yield
        5 part-shown
        5 0 off-map-e9.cmd02
    else 5 track self-is? if
        6 part-hidden
        6 1 off-map-e9.cmd02
        yield
        6 part-shown
        6 0 off-map-e9.cmd02
        yield
        6 part-hidden
        6 1 off-map-e9.cmd02
        4 wait-frames
        $32 chance? if
            6 part-shown
            6 0 off-map-e9.cmd02
            2 wait-frames
            6 part-hidden
            6 1 off-map-e9.cmd02
            yield
            6 part-shown
            6 0 off-map-e9.cmd02
            yield
            6 part-hidden
            6 1 off-map-e9.cmd02
            8 wait-frames
        then
        $19 chance? if
            6 part-shown
            6 0 off-map-e9.cmd02
            2 wait-frames
            6 part-hidden
            6 1 off-map-e9.cmd02
            yield
            6 part-shown
            6 0 off-map-e9.cmd02
            yield
            6 part-hidden
            6 1 off-map-e9.cmd02
            16 wait-frames
        then
        5 chance? if
            6 part-shown
            6 0 off-map-e9.cmd02
            yield
            6 part-hidden
            6 1 off-map-e9.cmd02
            yield
            6 part-shown
            6 0 off-map-e9.cmd02
            yield
            6 part-hidden
            6 1 off-map-e9.cmd02
            $20 wait-frames
        then
        6 part-shown
        6 0 off-map-e9.cmd02
        yield
        6 part-hidden
        6 1 off-map-e9.cmd02
        yield
        6 part-shown
        6 0 off-map-e9.cmd02
    then then then then
    self-idle-or-end
;

: off-map-e9.act01 ( -- )   \ 0044A8C0
    stalkers-stay state-flag-set
    stalkers-blind state-flag-set
    1 self-scripted
    off-map-e9.cmd01
    $34 1.0 1 bgm
    0 $1E 0 music
    $1E wait-frames
    self-wait-done
    [: 0 adx? ;] wait-until
    0 0.0 $FF bgm
    [: 1 adx? ;] wait-until
    $37 room-preload
    $C subscreen-open
    [: subscreen-wanted state-flag? ;] wait-while
    9 subscreen-open
    [: subscreen-wanted state-flag? ;] wait-while
    $F $50 fade
    wait-fade
    off-map-e9.cmd00
    $37 0 -1 hewie-to-room
    $10C door-open-clear
    2 0 char-remove
    stalkers-stay state-flag-clear
    $81 exit-check
    self-idle-or-end
;

: off-map-e9.act02 ( -- )   \ 0047B138
    1 self-scripted
    self-wait-done
    begin
        yield
    again
;

: off-map-e9.phase2 ( -- )   \ 0047B134
;

\ ---- registered ----
off-map-e9 register-scripts   \ (its phases and actions, by their names)
