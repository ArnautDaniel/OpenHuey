\ story/rooms/house-of-truth-b1-5.fs - the event scripts of room house-of-truth-b1-5 ($93; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-5
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant all-four-done               \ (bit 0) all four rooms' flags set as she entered area 3 (the way on loaded)
1 constant area0-reacted               \ (bit 1) her reaction in area 0 made (once a visit)

\ Room93_Cmd00
: house-of-truth-b1-5.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-5.enter ( -- )   \ 00444170
    htb1-5-flag1 story-flag-set
    shared-flag4 story-flag? if
        $FE $93 124 2 stalker-to-room
        stalker-who 0 stalker-command
        stalker-who 1 1 char-camera
    then
    htb1-5-camomile story-flag? not if
        0 -117.05 1.0 101.81 flicker-sprite
    then
    $31 1.0 0 bgm
    0 keeps-hewie-off keeps-stalkers-off or floor-on
    stalker-who $93 1 room-doors-state
    hewie-who $93 1 room-doors-state
;

: house-of-truth-b1-5.char-enter ( -- )   \ 004441C0
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $129 129.0 0.0 -90 char-to-xz
            0 0 player-camera
            $93 0 235 hewie-to-room
            hewie-who $EB -90 char-to-tri-facing
            0 fiona-who 2 action
        then
    then
;

: house-of-truth-b1-5.phase1 ( -- )   \ 00444200
    area0-reacted ebit? not if
        fiona-who 0 char-in-area? if
            area0-reacted ebit-set
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
        fiona-who 1 char-in-area? if
            area0-reacted ebit-set
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
        htb1-7-flag1 story-flag? not if
            $95 room-preload
        else
            $87 room-preload
        then
    then
    fiona-who 3 char-entered-area? if
        htb1-5-flag1 story-flag? htb1-6-flag1 story-flag? and htb1-7-flag1 story-flag? and htb1-8-flag1 story-flag? and if
            all-four-done ebit-set
            $88 room-preload
        else
            all-four-done ebit-clear
            $87 room-preload
        then
    then
    shared.act98
    house-of-truth-b1-5.cmd00
;

: house-of-truth-b1-5.phase2 ( -- )   \ 00444270
    fiona-who 6 char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
    htb1-5-camomile story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            4 calm offer-action
        then
    then
;

: house-of-truth-b1-5.act00 ( -- )   \ 004442A0
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act01 ( -- )   \ 004442B0
    1 self-scripted
    all-four-done ebit? if
        $FF 1.0 0 bgm
        keep-room-sounds state-flag-clear
    then
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act02 ( -- )   \ 004442D0
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act03 ( -- )   \ 004442E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    1 say   \ "For the next step, be it red or blue, the answers will make themselves known one by one."
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act04 ( -- )   \ 00444300
    self-wait-done
    -117.05 101.81 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            htb1-5-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-b1-5.phase5 ( -- )   \ 0047AFD8
    scene-locked state-flag-clear
;

\ ---- registered ----
house-of-truth-b1-5 register-scripts   \ (its phases and actions, by their names)
