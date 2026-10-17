\ story/rooms/house-of-truth-b1-7.fs - the event scripts of room house-of-truth-b1-7 ($95; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-7
USING: room-names story.words story.shared flag-names story-names floor-flags ;

\ room 0x95: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-7.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-7.enter ( -- )   \ 004444E0
    htb1-7-flag1 story-flag-set
    shared-flag4 story-flag? if
        $FE $95 124 2 stalker-to-room
        stalker-who 0 stalker-command
        stalker-who 1 1 char-camera
    then
    htb1-7-beef-jerky story-flag? not if
        0 -119.22 1.0 -96.82 flicker-sprite
    then
    $31 1.0 0 bgm
    0 keeps-hewie-off keeps-stalkers-off or floor-on
    stalker-who $95 1 room-doors-state
    hewie-who $95 1 room-doors-state
;

: house-of-truth-b1-7.char-enter ( -- )   \ 00444530
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $129 129.0 0.0 -90 char-to-xz
            0 0 player-camera
            $95 0 235 hewie-to-room
            hewie-who $EB -90 char-to-tri-facing
            0 fiona-who 2 action
        then
    then
;

: house-of-truth-b1-7.phase1 ( -- )   \ 00444570
    0 ebit? not if
        fiona-who 0 char-in-area? if
            0 ebit-set
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
        fiona-who 1 char-in-area? if
            0 ebit-set
            fiona-who 0 char-action? if
                0 fiona-who 1 action-force
            else
                1 fiona-who 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        htb1-6-flag1 story-flag? not if
            $94 room-preload
        else
            $87 room-preload
        then
    then
    fiona-who 3 char-entered-area? if
        htb1-8-flag1 story-flag? not if
            $96 room-preload
        else
            $87 room-preload
        then
    then
    shared.act98
    house-of-truth-b1-7.cmd00
;

: house-of-truth-b1-7.phase2 ( -- )   \ 004445D0
    fiona-who 6 char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
    htb1-7-beef-jerky story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            4 calm offer-action
        then
    then
;

: house-of-truth-b1-7.act00 ( -- )   \ 00444600
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act01 ( -- )   \ 00444610
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act02 ( -- )   \ 00444620
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act03 ( -- )   \ 00444630
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    3 message   \ "The colors, like the unique memories of the mind, may only come once in a lifetime."
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act04 ( -- )   \ 00444650
    self-wait-done
    -119.22 -96.82 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            htb1-7-beef-jerky story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: house-of-truth-b1-7.phase5 ( -- )   \ 0047AFE8
    scene-locked state-flag-clear
;

\ ---- registered ----
house-of-truth-b1-7 register-scripts   \ (its phases and actions, by their names)
