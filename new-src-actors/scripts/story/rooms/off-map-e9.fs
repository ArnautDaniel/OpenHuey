\ story/rooms/off-map-e9.fs - the event scripts of room off-map-e9 ($E9; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e9
USING: room-names story.words story.shared flag-names ;

\ room 0xE9: stops the countdown clock (clock_stop: progress +0x1FBEC1 off, the camera director
\ +0x40 -1).
: off-map-e9.cmd00 ( -- )  s" off-map-e9.cmd00" stub-step ;
\ room 0xE9: saves the countdown clock's time in script variables 0..2 (clock_save).
: off-map-e9.cmd01 ( -- )  s" off-map-e9.cmd01" stub-step ;
\ (as Room00_Cmd00) the same four spots for bytes 3..6
: off-map-e9.cmd02 ( b0 b1 -- )  drop drop s" off-map-e9.cmd02" stub-step ;
\ room 0xE9: draws the countdown clock, frozen at the saved time while event flag 1 is set
\ (clock_draw_saved; a frame hook).
: off-map-e9.cmd03 ( -- )  s" off-map-e9.cmd03" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e9.cond00? ( -- flag )  s" off-map-e9.cond00?" stub-flag ;

: off-map-e9.enter ( -- )   \ 0044A2C0
    $2E 1 pvar? if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $80C0F2AA 0 1 screen-blend
        3 1 $14 door-bits
        4 1 $14 door-bits
        5 1 $14 door-bits
        6 1 $14 door-bits
        7 1 $14 door-bits
        8 1 $14 door-bits
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
    0 0 $14 door-bits
;

: off-map-e9.char-enter ( -- )   \ 0044A380
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
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
        0 $C char-entered-area? if
            $1E chance? if
                0 $F8 0 action
            then
        then
        0 $C char-left-area? if
            $1E chance? if
                0 $F8 0 action
            then
        then
        0 $D char-entered-area? if
            $1E chance? if
                0 $F7 0 action
            then
        then
        0 $D char-left-area? if
            $1E chance? if
                0 $F7 0 action
            then
        then
        0 $E char-entered-area? if
            $1E chance? if
                0 $F6 0 action
            then
        then
        0 $E char-left-area? if
            $1E chance? if
                0 $F5 0 action
            then
        then
        $14 chance? if
            1 chance? if
                0 $F8 0 action
            else 1 chance? if
                0 $F7 0 action
            else 1 chance? if
                0 $F6 0 action
            else 1 chance? if
                0 $F5 0 action
            then then then then
        then
    then
    1 ebit? not if
        0 $10 char-in-area? if
            1 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 1 action-force
            0 1 2 action-force
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
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-e9.cmd03
    then then
;

: off-map-e9.act00 ( -- )   \ 0044A570
    $F8 self-is? if
        3 0 $14 door-bits
        3 1 off-map-e9.cmd02
        yield
        3 1 $14 door-bits
        3 0 off-map-e9.cmd02
        yield
        3 0 $14 door-bits
        3 1 off-map-e9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            3 1 $14 door-bits
            3 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            3 1 off-map-e9.cmd02
            yield
            3 1 $14 door-bits
            3 0 off-map-e9.cmd02
            yield
            3 0 $14 door-bits
            3 1 off-map-e9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            3 1 $14 door-bits
            3 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            3 1 off-map-e9.cmd02
            yield
            3 1 $14 door-bits
            3 0 off-map-e9.cmd02
            yield
            3 0 $14 door-bits
            3 1 off-map-e9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            3 1 $14 door-bits
            3 0 off-map-e9.cmd02
            yield
            3 0 $14 door-bits
            3 1 off-map-e9.cmd02
            yield
            3 1 $14 door-bits
            3 0 off-map-e9.cmd02
            yield
            3 0 $14 door-bits
            3 1 off-map-e9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        3 1 $14 door-bits
        3 0 off-map-e9.cmd02
        yield
        3 0 $14 door-bits
        3 1 off-map-e9.cmd02
        yield
        3 1 $14 door-bits
        3 0 off-map-e9.cmd02
    else $F7 self-is? if
        4 0 $14 door-bits
        4 1 off-map-e9.cmd02
        yield
        4 1 $14 door-bits
        4 0 off-map-e9.cmd02
        yield
        4 0 $14 door-bits
        4 1 off-map-e9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            4 1 $14 door-bits
            4 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            4 1 off-map-e9.cmd02
            yield
            4 1 $14 door-bits
            4 0 off-map-e9.cmd02
            yield
            4 0 $14 door-bits
            4 1 off-map-e9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            4 1 $14 door-bits
            4 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            4 1 off-map-e9.cmd02
            yield
            4 1 $14 door-bits
            4 0 off-map-e9.cmd02
            yield
            4 0 $14 door-bits
            4 1 off-map-e9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            4 1 $14 door-bits
            4 0 off-map-e9.cmd02
            yield
            4 0 $14 door-bits
            4 1 off-map-e9.cmd02
            yield
            4 1 $14 door-bits
            4 0 off-map-e9.cmd02
            yield
            4 0 $14 door-bits
            4 1 off-map-e9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        4 1 $14 door-bits
        4 0 off-map-e9.cmd02
        yield
        4 0 $14 door-bits
        4 1 off-map-e9.cmd02
        yield
        4 1 $14 door-bits
        4 0 off-map-e9.cmd02
    else $F6 self-is? if
        5 0 $14 door-bits
        5 1 off-map-e9.cmd02
        yield
        5 1 $14 door-bits
        5 0 off-map-e9.cmd02
        yield
        5 0 $14 door-bits
        5 1 off-map-e9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            5 1 $14 door-bits
            5 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            5 1 off-map-e9.cmd02
            yield
            5 1 $14 door-bits
            5 0 off-map-e9.cmd02
            yield
            5 0 $14 door-bits
            5 1 off-map-e9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            5 1 $14 door-bits
            5 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            5 1 off-map-e9.cmd02
            yield
            5 1 $14 door-bits
            5 0 off-map-e9.cmd02
            yield
            5 0 $14 door-bits
            5 1 off-map-e9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            5 1 $14 door-bits
            5 0 off-map-e9.cmd02
            yield
            5 0 $14 door-bits
            5 1 off-map-e9.cmd02
            yield
            5 1 $14 door-bits
            5 0 off-map-e9.cmd02
            yield
            5 0 $14 door-bits
            5 1 off-map-e9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        5 1 $14 door-bits
        5 0 off-map-e9.cmd02
        yield
        5 0 $14 door-bits
        5 1 off-map-e9.cmd02
        yield
        5 1 $14 door-bits
        5 0 off-map-e9.cmd02
    else $F5 self-is? if
        6 0 $14 door-bits
        6 1 off-map-e9.cmd02
        yield
        6 1 $14 door-bits
        6 0 off-map-e9.cmd02
        yield
        6 0 $14 door-bits
        6 1 off-map-e9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            6 1 $14 door-bits
            6 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            6 1 off-map-e9.cmd02
            yield
            6 1 $14 door-bits
            6 0 off-map-e9.cmd02
            yield
            6 0 $14 door-bits
            6 1 off-map-e9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            6 1 $14 door-bits
            6 0 off-map-e9.cmd02
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            6 1 off-map-e9.cmd02
            yield
            6 1 $14 door-bits
            6 0 off-map-e9.cmd02
            yield
            6 0 $14 door-bits
            6 1 off-map-e9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            6 1 $14 door-bits
            6 0 off-map-e9.cmd02
            yield
            6 0 $14 door-bits
            6 1 off-map-e9.cmd02
            yield
            6 1 $14 door-bits
            6 0 off-map-e9.cmd02
            yield
            6 0 $14 door-bits
            6 1 off-map-e9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        6 1 $14 door-bits
        6 0 off-map-e9.cmd02
        yield
        6 0 $14 door-bits
        6 1 off-map-e9.cmd02
        yield
        6 1 $14 door-bits
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
    self-frames-reset
    $1E self-wait-frames
    self-wait-done
    begin
        0 adx? not while
        yield
    repeat
    0 0.0 $FF bgm
    begin
        1 adx? not while
        yield
    repeat
    $37 room-preload
    $C subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    9 subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
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
' off-map-e9.enter off-map-e9 0 room-script!
' off-map-e9.char-enter off-map-e9 6 room-script!
' off-map-e9.phase1 off-map-e9 1 room-script!
' off-map-e9.phase3 off-map-e9 3 room-script!
' off-map-e9.act00 off-map-e9 $00 action-script!
' off-map-e9.act01 off-map-e9 $01 action-script!
' off-map-e9.act02 off-map-e9 $02 action-script!
' off-map-e9.phase2 off-map-e9 2 room-script!
