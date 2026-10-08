\ story/rooms/off-map-5a.fs - the event scripts of room off-map-5a ($5A; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-5a
USING: room-names story.words story.shared ;

\ the three dials (Room5A_ObjectNames, progress vars gSndProgressVars: 0..3, 90 degrees each),
\ event var 0 the one picked: byte 3 0 set (the original sets the picked one three times), 1 up
\ / down picks one (event var 0), left / right turns it (event +0x5C 3), cancel leaves (+0x60
\ 2); 2 turning to it 4 degrees a step, and there: solved at 1 / 0 / 2 (+0x60 2, +0x5C 4), else
\ +0x60 3; 3 wait
: off-map-5a.cmd00 ( b0 -- )  drop s" off-map-5a.cmd00" stub-step ;

: off-map-5a.char-enter ( -- )   \ 00410BE0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 6 -1 char-camera
                0 camera-follow
            else
                1 6 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 6 -1 char-camera
            0 camera-follow
        else
            1 6 -1 char-camera
            1 camera-follow
        then
    then then
    0 6 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    1 3 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    2 3 2 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
    then then
    3 2 1 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
            0 0 1 action
            1 $59 char-in-room? if
            then
        then
    then
;

: off-map-5a.phase1 ( -- )   \ 00410D00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $A 5 4 1 chars-area-camera
    $B 6 -1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 5 4 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 1 -1 1 chars-area-camera
    $10 1 -1 1 chars-area-camera
    $11 4 3 1 chars-area-camera
    $12 4 3 1 chars-area-camera
    $13 3 2 1 chars-area-camera
    $14 2 1 1 chars-area-camera
    $15 3 2 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? 0 6 char-entered-area? or if
        1 exit-prepare
    then
    0 7 char-entered-area? 0 8 char-entered-area? or if
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
;

: off-map-5a.phase2 ( -- )   \ 00410D90
    0 $16 $32 char-faces-area? if
        5 2 0 scene-change
    then
    1 exit-door-open? not 0 ebit? and if
        0 1 char-group-bit4? if
            0 scene-ending
        then
    then
    0 $17 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: off-map-5a.phase5 ( -- )   \ 00410DC0
    0 ebit? $FE char-here? not and $FE 2 char-C4? and if
        $FE action-end
        2 summon-take
    then
;

: off-map-5a.act00 ( -- )   \ 00410DD0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.act01 ( -- )   \ 00410E10
    0 ebit-set
    $F $41 fade
    0 $28 5 char-sound
    self-wait-done
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.act02 ( -- )   \ 00410E30
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 message
    wait-message
    0 answer? if
        0 $D2 293.136 -95.0 180 char-to-xz
        $17 state-flag-set
        1 5.0 0.0 0.0 2.0 event-camera
        2 message
        2 ebit-set
        0 0 var-set
        begin
            3 ebit? not if
                1 off-map-5a.cmd00
            else
                2 off-map-5a.cmd00
            then
            2 ebit? while
            yield
        repeat
        2 message-close
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
        4 ebit? if
            self-frames-reset
            self-wait-16
            0 $72 5 char-sound
            318.0 10.0 80.0 self-look-at-point
            yield
            self-frames-reset
            $3C self-wait-frames
            $FF self-look-at
            yield
        then
    then
    \ (never runs in the original: an else outside any block)
    \   7 message
    \   wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.act03 ( -- )   \ 00410ED0
    self-wait-done
    -115.0 -75.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: off-map-5a.act04 ( -- )   \ 00410EF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -115.0 -75.0 self-turn-to-xz
    self-wait-done
    5 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.enter ( -- )   \ 0047ABE0
    0 off-map-5a.cmd00
;

\ ---- registered ----
' off-map-5a.char-enter off-map-5a 6 room-script!
' off-map-5a.phase1 off-map-5a 1 room-script!
' off-map-5a.phase2 off-map-5a 2 room-script!
' off-map-5a.phase5 off-map-5a 5 room-script!
' off-map-5a.act00 off-map-5a $00 action-script!
' off-map-5a.act01 off-map-5a $01 action-script!
' off-map-5a.act02 off-map-5a $02 action-script!
' off-map-5a.act03 off-map-5a $03 action-script!
' off-map-5a.act04 off-map-5a $04 action-script!
' off-map-5a.enter off-map-5a 0 room-script!
