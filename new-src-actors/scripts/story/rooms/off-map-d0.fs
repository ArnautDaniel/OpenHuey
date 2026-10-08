\ story/rooms/off-map-d0.fs - the event scripts of room off-map-d0 ($D0; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d0
USING: room-names story.words story.shared ;

\ RoomD0_Cmd00
: off-map-d0.cmd00 ( -- )  s" off-map-d0.cmd00" stub-step ;
\ RoomD0_Cmd01
: off-map-d0.cmd01 ( -- )  s" off-map-d0.cmd01" stub-step ;
\ RoomD0_Cmd02
: off-map-d0.cmd02 ( -- )  s" off-map-d0.cmd02" stub-step ;
\ RoomD0_Cond00
: off-map-d0.cond00? ( -- flag )  s" off-map-d0.cond00?" stub-flag ;

: off-map-d0.enter ( -- )   \ 004469C0
    0 $16F 8 nav-tri-flags
    0 $174 8 nav-tri-flags
    $19 1.0 0 bgm
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: off-map-d0.char-enter ( -- )   \ 00446A00
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    0 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
;

: off-map-d0.phase1 ( -- )   \ 00446A80
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    2 0 0 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 1 1 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 4 4 1 chars-area-camera
    7 5 5 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 5 -1 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 5 5 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 5 -1 1 chars-area-camera
    1 2 char-entered-area? if
        1 exit-prepare
    then
    1 ebit? not if
        0 $10 char-in-area? if
            1 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 0 action-force
            0 1 1 action-force
        then
    then
;

: off-map-d0.phase3 ( -- )   \ 00446B00
    off-map-d0.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-d0.cmd02
    then then
;

: off-map-d0.phase2 ( -- )   \ 0047B030
;

: off-map-d0.act00 ( -- )   \ 00446B20
    $E state-flag-set
    $18 state-flag-set
    1 self-scripted
    off-map-d0.cmd01
    $FF 1.0 0 bgm
    begin
        1 adx? not while
        yield
    repeat
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
        4 state-flag? while
        yield
    repeat
    9 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $FF 1.0 0 bgm
    $F $50 fade
    wait-fade
    off-map-d0.cmd00
    $37 0 -1 hewie-to-room
    $10C door-open-clear
    2 0 char-remove
    $18 state-flag-clear
    $81 exit-check
    self-idle-or-end
;

: off-map-d0.act01 ( -- )   \ 0047B038
    1 self-scripted
    self-wait-done
    begin
        yield
    again
;

\ ---- registered ----
' off-map-d0.enter off-map-d0 0 room-script!
' off-map-d0.char-enter off-map-d0 6 room-script!
' off-map-d0.phase1 off-map-d0 1 room-script!
' off-map-d0.phase3 off-map-d0 3 room-script!
' off-map-d0.phase2 off-map-d0 2 room-script!
' off-map-d0.act00 off-map-d0 $00 action-script!
' off-map-d0.act01 off-map-d0 $01 action-script!
