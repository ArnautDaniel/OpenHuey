\ story/rooms/chaos-forest-12.fs - the event scripts of room chaos-forest-12 ($10A; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-12
USING: room-names story.words story.shared story-names fiona.anims ;

\ room 0x10A: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 1 a frame (room_nudge).
: chaos-forest-12.cmd00 ( b0 -- )  1e room-nudge ;
\ room 0x10A: a noise of loudness 0x20 or more was made in this room last frame (the progress'
\ noise requests kept at +0x10D4).
: chaos-forest-12.cond00? ( -- flag )  loud-here? ;

: chaos-forest-12.enter ( -- )   \ 00419730
    room-sounds
    $19 1.0 0 bgm
    cf-12-flag1 story-flag? not if
        $E 5 $FF char-load
        0 1 track 0 action
    then
    $1F 0 pvar? if
        deal-things
    then
    in-panic? if
        1 ebit-set
    then
    cf-12-camomile story-flag? not if
        0 -253.83 5.54 -217.28 flicker-sprite
    then
;

: chaos-forest-12.char-enter ( -- )   \ 00419770
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
    then then
    3 0 0 area-camera
;

: chaos-forest-12.phase1 ( -- )   \ 00419870
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
    fiona-who 2 char-in-area? fiona-who char-busy? not and if
        found-file-247 story-flag? cf-13-flag1 story-flag? not and if
            $17 3 2 char-load
        then
        2 exit-check
    then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 6 char-entered-area? or fiona-who 8 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        found-file-247 story-flag? cf-13-flag1 story-flag? not and if
            $17 3 2 char-load
        then
        2 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 7 char-left-area? if
        found-file-247 story-flag? cf-13-flag1 story-flag? not and if
            3 0 char-remove
        then
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
    2 ebit? cf-12-flag1 story-flag? not and if
        chaos-forest-12.cond00? if
            cf-12-flag1 story-flag-set
            1 track action-end
            0 1 track 2 action
        then
    then
;

: chaos-forest-12.phase2 ( -- )   \ 00419950
    cf-12-camomile story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            1 calm offer-action
        then
    then
;

: chaos-forest-12.act00 ( -- )   \ 00419970
    5 char-start
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
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cf-12-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
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
chaos-forest-12 register-scripts   \ (its phases and actions, by their names)
