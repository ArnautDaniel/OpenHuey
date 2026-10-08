\ story/rooms/chaos-forest-9.fs - the event scripts of room chaos-forest-9 ($107; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-9
USING: room-names story.words story.shared ;

\ room 0x107: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 2 a frame (room_nudge).
: chaos-forest-9.cmd00 ( b0 -- )  drop s" chaos-forest-9.cmd00" stub-step ;
\ room 0x107: starts the slowly turning backdrop model (Effect7A3D0: model 0x30 far off at
\ (-225, -64, -1125)).
: chaos-forest-9.cmd01 ( -- )  s" chaos-forest-9.cmd01" stub-step ;
\ room 0x107: a noise of loudness 0x20 or more was made in this room last frame (the progress'
\ noise requests kept at +0x10D4).
: chaos-forest-9.cond00? ( -- flag )  s" chaos-forest-9.cond00?" stub-flag ;

: chaos-forest-9.enter ( -- )   \ 00418AF0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $21C story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2CB story-flag? not if
            0 -298.4 72.6 -365.4 flicker-sprite
        then
    then
    0 $B 0.812 0.687 0.187 0.312 zone-rect
    $35D story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
    chaos-forest-9.cmd01
;

: chaos-forest-9.char-enter ( -- )   \ 00418B60
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
;

: chaos-forest-9.phase1 ( -- )   \ 00418BA0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 ebit? $35D story-flag? not and if
        chaos-forest-9.cond00? if
            $35D story-flag-set
            $F1 action-end
            0 $F1 2 action
        then
    then
;

: chaos-forest-9.phase2 ( -- )   \ 00418BF0
    $21C story-flag? not if
        0 -298.4 71.6 -365.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $21C story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -298.4 71.6 -365.4 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -298.4 71.6 -365.4 0 0 sound
            $40 $BA noise
            0 -298.4 72.6 -365.4 flicker-sprite
        then
    else $2CB story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
    0 1 char-in-area? 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
;

: chaos-forest-9.act00 ( -- )   \ 00418C90
    5 char-unload
    2 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -339.86 125.95 -391.22 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: chaos-forest-9.act01 ( -- )   \ 00418CD0
    self-wait-done
    -298.4 -365.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $2CB story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: chaos-forest-9.act02 ( -- )   \ 00418D30
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 chaos-forest-9.cmd00
    1 chaos-forest-9.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

: chaos-forest-9.act03 ( -- )   \ 00418D50
    self-wait-done
    3 ebit? not if
        $A02 5 self-anim-blend
        self-wait-anim
        0 message
        wait-message
        3 ebit-set
    else
        $1D02 5 self-anim-blend
        self-wait-anim
        1 message
        wait-message
    then
    self-idle-or-end
;

: chaos-forest-9.phase5 ( -- )   \ 0047AC78
    $E action-end
    $E char-done
;

\ ---- registered ----
' chaos-forest-9.enter chaos-forest-9 0 room-script!
' chaos-forest-9.char-enter chaos-forest-9 6 room-script!
' chaos-forest-9.phase1 chaos-forest-9 1 room-script!
' chaos-forest-9.phase2 chaos-forest-9 2 room-script!
' chaos-forest-9.act00 chaos-forest-9 $00 action-script!
' chaos-forest-9.act01 chaos-forest-9 $01 action-script!
' chaos-forest-9.act02 chaos-forest-9 $02 action-script!
' chaos-forest-9.act03 chaos-forest-9 $03 action-script!
' chaos-forest-9.phase5 chaos-forest-9 5 room-script!
