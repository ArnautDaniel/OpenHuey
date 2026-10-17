\ story/rooms/off-map-d0.fs - the event scripts of room off-map-d0 ($D0; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d0
USING: room-names story.words story.shared flag-names story.clock floor-flags ;

\ RoomD0_Cmd00
: off-map-d0.cmd00 ( -- )  clock-stop ;
\ RoomD0_Cmd01
: off-map-d0.cmd01 ( -- )  clock-save ;
\ RoomD0_Cmd02
: off-map-d0.cmd02 ( -- )  clock-draw-saved ;   \ (each frame: the clock, or the time kept)
\ RoomD0_Cond00
: off-map-d0.cond00? ( -- flag )  true ;   \ (the progress object's +0x7C: ProgressBase_Slot7C, always 1)

: off-map-d0.enter ( -- )   \ 004469C0
    $16F solid-floor tri-floor-off
    $174 solid-floor tri-floor-off
    $19 1.0 0 bgm
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: off-map-d0.char-enter ( -- )   \ 00446A00
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        3 3 player-camera
    then
    0 3 3 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
;

: off-map-d0.phase1 ( -- )   \ 00446A80
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
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
    hewie-who 2 char-entered-area? if
        1 exit-prepare
    then
    1 ebit? not if
        fiona-who $10 char-in-area? if
            1 ebit-set
            panic-eased
            0 fiona-who 0 action-force
            0 hewie-who 1 action-force
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
    stalkers-blind state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    off-map-d0.cmd01
    $FF 1.0 0 bgm
    [: 1 adx? ;] wait-until
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
    $FF 1.0 0 bgm
    $F $50 fade
    wait-fade
    off-map-d0.cmd00
    $37 0 -1 hewie-to-room
    $10C door-open-clear
    2 0 char-remove
    stalkers-stay state-flag-clear
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
off-map-d0 register-scripts   \ (its phases and actions, by their names)
