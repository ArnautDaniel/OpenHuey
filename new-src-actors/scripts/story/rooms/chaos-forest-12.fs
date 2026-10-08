\ story/rooms/chaos-forest-12.fs - the event scripts of room chaos-forest-12 ($10A; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-12
USING: room-names story.words story.shared ;

\ room 0x10A: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 1 a frame (room_nudge).
: chaos-forest-12.cmd00 ( b0 -- )  drop s" chaos-forest-12.cmd00" stub-step ;
\ room 0x10A: a noise of loudness 0x20 or more was made in this room last frame (the progress'
\ noise requests kept at +0x10D4).
: chaos-forest-12.cond00? ( -- flag )  s" chaos-forest-12.cond00?" stub-flag ;

: chaos-forest-12.enter ( -- )   \ 00419730
    room-sounds
    $19 1.0 0 bgm
    $344 story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $2C7 story-flag? not if
        0 -253.83 5.54 -217.28 flicker-sprite
    then
;

: chaos-forest-12.char-enter ( -- )   \ 00419770
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
;

: chaos-forest-12.phase1 ( -- )   \ 00419870
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    0 2 char-in-area? 0 char-busy? not and if
        $86 story-flag? $88 story-flag? not and if
            $17 3 2 char-load
        then
        2 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? 0 6 char-entered-area? or 0 8 char-entered-area? or if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        $86 story-flag? $88 story-flag? not and if
            $17 3 2 char-load
        then
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 7 char-left-area? if
        $86 story-flag? $88 story-flag? not and if
            3 0 char-remove
        then
    then
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
    2 ebit? $344 story-flag? not and if
        chaos-forest-12.cond00? if
            $344 story-flag-set
            $F1 action-end
            0 $F1 2 action
        then
    then
;

: chaos-forest-12.phase2 ( -- )   \ 00419950
    $2C7 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: chaos-forest-12.act00 ( -- )   \ 00419970
    5 char-unload
    2 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -25.61 38.13 -136.96 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: chaos-forest-12.act01 ( -- )   \ 004199B0
    self-wait-done
    -253.83 -217.28 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C7 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: chaos-forest-12.act02 ( -- )   \ 00419A10
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 chaos-forest-12.cmd00
    1 chaos-forest-12.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

: chaos-forest-12.phase5 ( -- )   \ 0047AC80
    $E action-end
    $E char-done
;

\ ---- registered ----
' chaos-forest-12.enter chaos-forest-12 0 room-script!
' chaos-forest-12.char-enter chaos-forest-12 6 room-script!
' chaos-forest-12.phase1 chaos-forest-12 1 room-script!
' chaos-forest-12.phase2 chaos-forest-12 2 room-script!
' chaos-forest-12.act00 chaos-forest-12 $00 action-script!
' chaos-forest-12.act01 chaos-forest-12 $01 action-script!
' chaos-forest-12.act02 chaos-forest-12 $02 action-script!
' chaos-forest-12.phase5 chaos-forest-12 5 room-script!
