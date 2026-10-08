\ story/rooms/castle-b1-2.fs - the event scripts of room castle-b1-2 ($14; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-2
USING: room-names story.words story.shared flag-names ;

\ room 0x14: the curtain ("Cartain") animated by event variable 0 (var0_obj_anim: byte 3 picks
\ the frame range and direction).
: castle-b1-2.cmd00 ( b0 -- )  drop s" castle-b1-2.cmd00" stub-step ;

: castle-b1-2.enter ( -- )   \ 003F99E0
    room-sounds
    $225 story-flag? not if
        0 78.0 6.5 20.0 flicker-sprite
    then
    $246 story-flag? $247 story-flag? not and if
        1 -44.9 -7.0 -12.9 flicker-sprite
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 castle-b1-2.cmd00
    1 $3FFF sound-volume
;

: castle-b1-2.act07 ( -- )   \ 003F9EB0
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 1 char-file-load
    then
    $FE camera-follow
    3 ebit-clear
    $F 0 pvar? if
        0 chance? if
            3 ebit-set
        then
    else $F 1 pvar? if
        0 chance? if
            3 ebit-set
        then
    else $F 2 pvar? if
        $19 chance? if
            3 ebit-set
        then
    else $F 3 pvar? if
        $19 chance? if
            3 ebit-set
        then
    then then then then
    2 creature-action? if
        3 ebit-set
    then
    4 stalker-alert? if
        3 ebit-clear
    then
    3 ebit? if
        0 $FE 8 action
    else
        $78 1 item-cooldown
    then
    $F pvar-inc
    exit
;

: castle-b1-2.char-enter ( -- )   \ 003F9A30
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
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    1 2 -1 area-camera
    $FE self-is? if
        fiona-hidden state-flag? if
            5 ebit-set
            castle-b1-2.act07
        else
            5 ebit-clear
        then
    then
;

: castle-b1-2.phase1 ( -- )   \ 003F9AC0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    0 2 char-left-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $246 story-flag? not if
        2 -44.9 -8.0 -12.9 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 582 var-set
                $1A 583 var-set
                $1B 1 var-set
                $1C -44900 var-set
                $1D -7000 var-set
                $1E -12900 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
;

: castle-b1-2.phase2 ( -- )   \ 003F9B60
    0 $A char-in-area? 0 90 $2D char-heading? and if
        5 0 0 scene-change
    then
    0 $B char-in-area? 0 90 $2D char-heading? and if
        5 1 0 scene-change
    then
    -2147483646 scene-request? if
        5 3 1 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 5 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $225 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
    $246 story-flag? $247 story-flag? not and if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
;

: castle-b1-2.act00 ( -- )   \ 0047AA20
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: castle-b1-2.act01 ( -- )   \ 003F9BD0
    0 ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $89 -9.659 4.88 180 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        1 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        0 $84 5 char-sound
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        2 message
        wait-message
    then
    self-idle-or-end
;

: castle-b1-2.act02 ( -- )   \ 003F9C90
    self-wait-done
    78.0 20.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $225 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: castle-b1-2.act03 ( -- )   \ 003F9CF0
    self-wait-done
    1 ebit? not 2 game-mode? or $FF panic-stage? or if
        1 self-through-exit
        self-wait-done
        $FF panic-stage? 2 game-mode? or if
            $60A self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $608 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            3 message
            wait-message
        then
        1 ebit-set
    else
        $95 -58.5 9.5 -105 $FFFF 5 self-move-to
        self-wait-done
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        4 message
        wait-message
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    then
    self-idle-or-end
;

: castle-b1-2.act04 ( -- )   \ 003F9D50
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A self-through-door
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 2 6 char-sound
    7 message-param-room
    7 item-use
    $A door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-2.act06 ( -- )   \ 003F9E60
    2 avoid-prompt
    1 self-scripted
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 0 var-set
    4 ebit? if
        counter-inc
    then
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        2 castle-b1-2.cmd00
        yield
        0 28 var? not while
        0 var-inc
    repeat
    3 castle-b1-2.cmd00
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    self-wait-anim
    0 self-noclip
    4 ebit? if
        2 wait-counter
    then
    0 self-scripted
    self-idle-or-end
;

: castle-b1-2.act09 ( -- )   \ 003F9F80
    2 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 0 var-set
    $8003 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        5 castle-b1-2.cmd00
        yield
        0 18 var? not while
        0 var-inc
    repeat
    3 castle-b1-2.cmd00
    self-wait-anim
    0 self-noclip
    2 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: castle-b1-2.act05 ( -- )   \ 003F9D90
    stalkers-stay state-flag-set
    5 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    1 self-noclip
    1 self-scripted
    $15 60.918 -14.76 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $FF self-look-at
    yield
    0 0 var-set
    $8000 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        0 castle-b1-2.cmd00
        yield
        0 54 var? not while
        0 var-inc
    repeat
    self-wait-anim
    4 castle-b1-2.cmd00
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    2 ebit-clear
    4 ebit-clear
    0 counter-set
    0 avoid-prompt
    begin
        4 ebit? if
            ['] castle-b1-2.act06 goto
        else 0 2 pad? not if
            $FF panic-stage? 2 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-b1-2.act09 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? $FE char-here? not and if
                    5 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                $FE action-end
                ['] castle-b1-2.act09 goto
            then
            0 camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            0 0 var-set
            $8001 0 self-anim-9
            begin
                1 castle-b1-2.cmd00
                yield
                0 21 var? not while
                0 var-inc
            repeat
            3 castle-b1-2.cmd00
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then then
    again
;

: castle-b1-2.act08 ( -- )   \ 003F9F10
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $FA $FFFF 6 self-move-tri
    self-wait-done
    $10 58.883 -8.841 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        4 ebit-set
        1 self-scripted
        yield
        $FE char-file-use
        1 wait-counter
        $8000 5 self-anim-9
        self-wait-anim
        counter-inc
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        2 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-b1-2.act0A ( -- )   \ 003F9FD0
    self-wait-done
    -44.9 -12.9 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $247 story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-2.enter castle-b1-2 0 room-script!
' castle-b1-2.char-enter castle-b1-2 6 room-script!
' castle-b1-2.phase1 castle-b1-2 1 room-script!
' castle-b1-2.phase2 castle-b1-2 2 room-script!
' castle-b1-2.act00 castle-b1-2 $00 action-script!
' castle-b1-2.act01 castle-b1-2 $01 action-script!
' castle-b1-2.act02 castle-b1-2 $02 action-script!
' castle-b1-2.act03 castle-b1-2 $03 action-script!
' castle-b1-2.act04 castle-b1-2 $04 action-script!
' castle-b1-2.act05 castle-b1-2 $05 action-script!
' castle-b1-2.act06 castle-b1-2 $06 action-script!
' castle-b1-2.act07 castle-b1-2 $07 action-script!
' castle-b1-2.act08 castle-b1-2 $08 action-script!
' castle-b1-2.act09 castle-b1-2 $09 action-script!
' castle-b1-2.act0A castle-b1-2 $0A action-script!
