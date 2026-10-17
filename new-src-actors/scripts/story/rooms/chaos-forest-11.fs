\ story/rooms/chaos-forest-11.fs - the event scripts of room chaos-forest-11 ($109; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-11
USING: room-names story.words story.shared story-names ;

\ character kind 0x1A: byte 3 0 starts Kind26_MoveTo(2, -6, 257); else waits (2) until
\ Kind26_MoveDone says done
: chaos-forest-11.cmd00 ( b0 -- )
    if  $1A char-wait-done  else  $1A 2 -6e 257e char-animal-go  then ;
\ room 0x109: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 1 a frame (room_nudge).
: chaos-forest-11.cmd01 ( b0 -- )  1e room-nudge ;

: chaos-forest-11.enter ( -- )   \ 004193D0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    in-panic? if
        1 ebit-set
    then
    $1E chance? if
        $1A 4 $FF char-load
        0 $F1 0 action
    then
    cf-11-flag1 story-flag? not if
        $E 5 $FF char-load
        0 $F2 1 action
    then
;

: chaos-forest-11.char-enter ( -- )   \ 00419400
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        3 3 player-camera
    then then
    1 3 3 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        2 2 player-camera
    then then
    3 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 4 exit-taken? and if
            fiona-who 4 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 4 exit-taken? and if
        hewie-who 4 char-to-exit
        2 2 player-camera
    then then
    4 2 2 area-camera
;

: chaos-forest-11.phase1 ( -- )   \ 00419530
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    hewie-controlled? not if
        fiona-who 4 char-in-area? fiona-who char-busy? not and if
            4 exit-check
        then
    else hewie-who 4 char-in-area? hewie-who char-busy? not and if
        4 exit-check
    then then
    $A 3 3 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 3 3 1 chars-area-camera
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        4 exit-prepare
    then
    0 ebit? not if
        1 ebit? not in-panic? and if
            0 ebit-set
            deal-things
        then
    then
    in-panic? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 ebit? cf-11-flag1 story-flag? not and if
        0 -24.0 8.31 -132.0 $19 10 0 zone
        fiona-who 0 2 char-zone-bits? hewie-who 0 2 char-zone-bits? or if
            cf-11-flag1 story-flag-set
            $F2 action-end
            0 $F2 2 action
        then
    then
;

: chaos-forest-11.phase5 ( -- )   \ 00419638
    $1A action-end
    $1A char-done
    $E action-end
    $E char-done
;

: chaos-forest-11.act00 ( -- )   \ 00419650
    4 char-start
    $1A char-activate
    $1A 1 3.0 235.0 180 char-to-xz
    $1A $9000 1 0 char-anim-hold
    0 chaos-forest-11.cmd00
    1 chaos-forest-11.cmd00
    $1A action-end
    $1A char-done
    self-idle-or-end
;

: chaos-forest-11.act01 ( -- )   \ 00419680
    5 char-start
    2 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -21.17 31.84 -137.07 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: chaos-forest-11.act02 ( -- )   \ 004196C0
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 chaos-forest-11.cmd01
    1 chaos-forest-11.cmd01
    $E action-end
    $E char-done
    self-idle-or-end
;

\ ---- registered ----
chaos-forest-11 register-scripts   \ (its phases and actions, by their names)
