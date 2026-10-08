\ story/rooms/castle-1f-17.fs - the event scripts of room castle-1f-17 ($1B; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-17
USING: room-names story.words story.shared ;

\ five pendulums (Room1B_ObjectNames[byte 3]) of their own periods and swings: byte 4 0 still at
\ a phase offset (+0x34) 60 x the index, 1 swinging on (+0x30, +0x14)
: castle-1f-17.cmd00 ( b0 b1 -- )  drop drop s" castle-1f-17.cmd00" stub-step ;

: castle-1f-17.enter ( -- )   \ 003FC6A0
    $14 1.0 0 bgm
    $227 story-flag? not if
        0 -6.0 8.5 -5.0 flicker-sprite
    then
    0 0 castle-1f-17.cmd00
    1 0 castle-1f-17.cmd00
    3 0 castle-1f-17.cmd00
    4 0 castle-1f-17.cmd00
    0 1 castle-1f-17.cmd00
    1 1 castle-1f-17.cmd00
    3 1 castle-1f-17.cmd00
    4 1 castle-1f-17.cmd00
    0 $F1 6 action
;

: castle-1f-17.char-enter ( -- )   \ 003FC6F0
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
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
;

: castle-1f-17.phase1 ( -- )   \ 003FC770
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    1 20.5 0.0 49.99 $14 17 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 17.0 hewie-look-zone
            then
        then
    then
    2 20.64 0.0 11.01 9 10 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    2 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    3 -22.87 0.0 38.05 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    4 28.49 0.0 -29.82 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    5 1.39 0.0 -30.06 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    6 -29.89 0.0 -30.85 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    7 -29.03 0.0 -7.64 $F 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 7 9 char-zone-bits? if
                $1F 7 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: castle-1f-17.phase2 ( -- )   \ 003FC8F0
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    0 6 char-in-area? if
        5 1 0 scene-change
    then
    0 7 char-in-area? 0 0 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 90 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 $A char-in-area? 0 90 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 $B char-in-area? 0 45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $227 story-flag? not if
        0 0 8 4 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    0 $C $32 char-faces-area? if
        5 4 0 scene-change
    then
;

: castle-1f-17.act00 ( -- )   \ 003FC970
    self-wait-done
    $FE self-touching? not if
        $19 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $19 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $19 door-unlock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: castle-1f-17.act01 ( -- )   \ 003FC9B0
    self-wait-done
    $A01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-17.act02 ( -- )   \ 003FC9C0
    self-wait-done
    0 self-turn-angle
    self-wait-done
    1 40.0 20.0 0.0 0.0 event-camera
    $A00 $A self-anim-blend
    1 message
    wait-message
    self-wait-anim
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-17.act03 ( -- )   \ 003FCA00
    self-wait-done
    0 ebit? not if
        2 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-17.act04 ( -- )   \ 003FCA20
    self-wait-done
    1 ebit? not if
        4 message
        wait-message
        1 self-anim
        self-wait-anim
        -1 self-move-16
        6 message
        self-frames-reset
        7 self-wait-frames
        wait-message
        1 ebit-set
    else
        5 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: castle-1f-17.act05 ( -- )   \ 003FCA50
    self-wait-done
    -6.0 -5.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $227 story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-17.act06 ( -- )   \ 003FCAB0
    begin
        yield
        0 1 castle-1f-17.cmd00
        1 1 castle-1f-17.cmd00
        3 1 castle-1f-17.cmd00
        4 1 castle-1f-17.cmd00
    again
;

\ ---- registered ----
' castle-1f-17.enter castle-1f-17 0 room-script!
' castle-1f-17.char-enter castle-1f-17 6 room-script!
' castle-1f-17.phase1 castle-1f-17 1 room-script!
' castle-1f-17.phase2 castle-1f-17 2 room-script!
' castle-1f-17.act00 castle-1f-17 $00 action-script!
' castle-1f-17.act01 castle-1f-17 $01 action-script!
' castle-1f-17.act02 castle-1f-17 $02 action-script!
' castle-1f-17.act03 castle-1f-17 $03 action-script!
' castle-1f-17.act04 castle-1f-17 $04 action-script!
' castle-1f-17.act05 castle-1f-17 $05 action-script!
' castle-1f-17.act06 castle-1f-17 $06 action-script!
