\ story/rooms/castle-b1-7.fs - the event scripts of room castle-b1-7 ($1D; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-7
USING: room-names story.words story.shared ;

\ room 0x1D (Room1D_Cmd00_ptmf): script variable 0 = 2 .. 5 at random
: castle-b1-7.cmd00 ( -- )  s" castle-b1-7.cmd00" stub-step ;
\ room 0x1D (Room1D_Cmd01_ptmf): door 0's +0x74 (0, or -0.08 by byte 3)
: castle-b1-7.cmd01 ( b0 -- )  drop s" castle-b1-7.cmd01" stub-step ;
\ Fiona in move 5, room 0x1D's flag 0 not set and its exit 0's door shut
: castle-b1-7.cond00? ( -- flag )  s" castle-b1-7.cond00?" stub-flag ;

: castle-b1-7.act04 ( -- )   \ 003FD720
    $2D state-flag-clear
    1 char-here? 1 hewie-side? and if
        $1E 0 -1 hewie-to-room
        1800 2 hewie-anim
    then
    exit
;

: castle-b1-7.enter ( -- )   \ 003FD370
    room-sounds
    1 $1E char-in-room? 119 hewie-action? and if
        $2D state-flag-set
        $1D 1 252 hewie-to-room
        8 story-flag? not if
            0 1 2 action
        then
    else
        castle-b1-7.act04
    then
    0 30.0 -27.65 -9.8 0 effect-86
    8 story-flag? not if
        1 -12.0 -40.0 -25.0 flicker-sprite
    then
    $201 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $22D story-flag? not if
            2 31.0 -40.0 13.0 flicker-sprite
        then
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    $252 story-flag? $253 story-flag? not and if
        3 29.0 1.0 21.0 flicker-sprite
    then
    0 0 0 $1ED $245 obstacle-place
;

: castle-b1-7.char-enter ( -- )   \ 003FD420
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
    0 self-is? if
        0 exit-taken? if
            2 map-page
        then
    then
;

: castle-b1-7.phase1 ( -- )   \ 003FD470
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    3 1 0 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    5 3 1 1 chars-area-camera
    0 3 char-entered-area? if
        2 map-page
    then
    0 4 char-entered-area? if
        1 map-page
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    0 $1ED obstacle-on? if
        $26 door-locked? if
            $26 door-unlock
        then
    then
    -2147483644 scene-request? not castle-b1-7.cond00? and if
        $26 door-locked? not if
            $26 door-lock
        then
    then
    1 ebit? not if
        $26 door-locked? if
            $FE $1C char-in-room? if
                0 $F2 9 action
            then
        then
    else $26 door-locked? not if
        $F2 action-end
        0 castle-b1-7.cmd01
        $FE 1 stalker-mode
        $18 state-flag-clear
        1 ebit-clear
    then then
    $252 story-flag? not if
        1 29.0 0.0 21.0 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 594 var-set
                $1A 595 var-set
                $1B 3 var-set
                $1C 29000 var-set
                $1D 1000 var-set
                $1E 21000 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
;

: castle-b1-7.phase2 ( -- )   \ 003FD580
    $201 story-flag? not if
        0 31.0 -41.0 13.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $201 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            31.0 -41.0 13.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 31.0 -41.0 13.0 0 0 sound
            $40 $1B0 noise
            2 31.0 -40.0 13.0 flicker-sprite
        then
    else $22D story-flag? not if
        0 2 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then then
    0 7 $3C char-faces-area? if
        5 1 0 scene-change
    then
    1 char-here? not 1 hewie-side? not or 8 story-flag? not and 0 6 char-in-area? and 0 -12 -25 $32 char-faces-xz? and if
        5 5 0 scene-change
    then
    -2147483646 scene-request? if
        5 8 1 scene-change
    then
    0 8 char-in-area? 0 0 $32 char-heading? and if
        5 $B 0 scene-change
    then
    $252 story-flag? $253 story-flag? not and if
        1 3 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
;

: castle-b1-7.phase3 ( -- )   \ 003FD660
    camera-setup-changed? if
        1 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
;

: castle-b1-7.act00 ( -- )   \ 003FD680
    1 self-scripted
    self-wait-done
    2 wait-counter
    8 story-flag-set
    1 effect-remove
    10 hewie-trust
    3 message-param-room
    3 1 item-give-count
    0 3 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    40 hewie-trust
    $827C item-give
    counter-inc
    0 self-scripted
    self-idle-or-end
;

: castle-b1-7.act01 ( -- )   \ 0047AA88
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: castle-b1-7.act03 ( -- )   \ 003FD700
    1 wait-counter
    hewie-bark
    self-wait-done
    $92 -17.0 -30.0 45 $FFFF 5 self-move-to
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    counter-inc
    3 wait-counter
    self-idle-or-end
;

: castle-b1-7.act02 ( -- )   \ 003FD6C0
    self-wait-done
    1 $FC 0 char-to-tri-facing
    1 self-anim
    self-wait-anim
    0 self-look-at
    yield
    begin
        35 fiona-started? 44 fiona-started? or 2 game-mode? not and 0 6 char-in-area? and 0 -12 -25 $32 char-faces-xz? and if
            0 counter-set
            0 $F1 6 action
            ['] castle-b1-7.act03 goto
        then
        yield
    again
;

: castle-b1-7.act05 ( -- )   \ 003FD740
    self-wait-done
    $A01 $A self-anim-blend
    self-wait-anim
    0 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $FF 4 -1 char-camera
    self-frames-reset
    8 self-wait-frames
    1 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    hewie-controlled? not if
        0 3 1 char-camera
        0 camera-follow
    else
        1 3 1 char-camera
        1 camera-follow
    then
    camera-restart
    self-idle-or-end
;

: castle-b1-7.act06 ( -- )   \ 003FD780
    begin
        fiona-free? not while
        yield
    repeat
    counter-inc
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 0 action-force
    self-idle-or-end
;

: castle-b1-7.act07 ( -- )   \ 003FD7A0
    self-wait-done
    31.0 13.0 self-turn-to-xz
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
            $22D story-flag-set
            2 effect-remove
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

: castle-b1-7.act08 ( -- )   \ 003FD7F8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: castle-b1-7.act09 ( -- )   \ 003FD810
    1 ebit-set
    $18 state-flag-set
    self-frames-reset
    $3C self-wait-frames
    castle-b1-7.cmd00
    begin
        1 6 -40.0 0.0 -20.0 0 0 sound
        1 castle-b1-7.cmd01
        yield
        0 castle-b1-7.cmd01
        $19 chance? if
            self-frames-reset
            self-wait-16
        then
        self-frames-reset
        self-wait-16
        1 6 -40.0 0.0 -20.0 0 0 sound
        1 castle-b1-7.cmd01
        yield
        0 castle-b1-7.cmd01
        $32 chance? if
            self-frames-reset
            self-wait-16
            self-frames-reset
            self-wait-16
        then
        self-frames-reset
        $3C self-wait-frames
        0 0 var? not while
        0 var-dec
        yield
    repeat
    $26 door-locked? if
        $31A story-flag? not if
            $31A story-flag-set
        else $4B chance? if
            $31B story-flag-set
        then then
        $FE action-end
        $FE char-done
    else
        $FE 1 stalker-mode
    then
    $18 state-flag-clear
    1 ebit-clear
    self-idle-or-end
;

: castle-b1-7.act0A ( -- )   \ 003FD8A0
    self-wait-done
    29.0 21.0 self-turn-to-xz
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
            $253 story-flag-set
            3 effect-remove
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

: castle-b1-7.act0B ( -- )   \ 003FD900
    self-wait-done
    $2FD story-flag? not if
        0 self-turn-angle
        self-wait-done
        4 message
        wait-message
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $2FD story-flag-set
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: castle-b1-7.phase5 ( -- )   \ 0047AA80
    castle-b1-7.act04
;

\ ---- registered ----
' castle-b1-7.enter castle-b1-7 0 room-script!
' castle-b1-7.char-enter castle-b1-7 6 room-script!
' castle-b1-7.phase1 castle-b1-7 1 room-script!
' castle-b1-7.phase2 castle-b1-7 2 room-script!
' castle-b1-7.phase3 castle-b1-7 3 room-script!
' castle-b1-7.act00 castle-b1-7 $00 action-script!
' castle-b1-7.act01 castle-b1-7 $01 action-script!
' castle-b1-7.act02 castle-b1-7 $02 action-script!
' castle-b1-7.act03 castle-b1-7 $03 action-script!
' castle-b1-7.act04 castle-b1-7 $04 action-script!
' castle-b1-7.act05 castle-b1-7 $05 action-script!
' castle-b1-7.act06 castle-b1-7 $06 action-script!
' castle-b1-7.act07 castle-b1-7 $07 action-script!
' castle-b1-7.act08 castle-b1-7 $08 action-script!
' castle-b1-7.act09 castle-b1-7 $09 action-script!
' castle-b1-7.act0A castle-b1-7 $0A action-script!
' castle-b1-7.act0B castle-b1-7 $0B action-script!
' castle-b1-7.phase5 castle-b1-7 5 room-script!
