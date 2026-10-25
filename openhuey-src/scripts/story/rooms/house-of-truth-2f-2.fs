\ story/rooms/house-of-truth-2f-2.fs - the event scripts of room house-of-truth-2f-2 ($83; House of Truth: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-2f-2
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant books-remarked              \ (bit 0) its first remark made: "Stacks of books have been arranged neatly on the shelf. Some"

: house-of-truth-2f-2.enter ( -- )   \ 00431DE0
    0 part-shown
    0 1 object-hidden
    1 1 object-hidden
    2 1 object-hidden
    got-mystery-sketch story-flag? not if
        1 part-shown
    then
    ht2f-2-viride-medallion-found story-flag? ht2f-2-viride-medallion story-flag? not and if
        0 37.0 1.0 84.0 flicker-sprite
    then
;

: house-of-truth-2f-2.char-enter ( -- )   \ 00431E20
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $81 0 char-to-tri-facing
            $82 1 -1 hewie-to-room
            world-held state-flag-set
            0 fiona-who 0 action
        then
    then
;

: house-of-truth-2f-2.phase1 ( -- )   \ 00431E80
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    ht2f-2-viride-medallion-found story-flag? not if
        0 37.0 0.0 84.0 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var ht2f-2-viride-medallion-found var-set
                taken-flag-var ht2f-2-viride-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 37000 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 84000 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: house-of-truth-2f-2.phase2 ( -- )   \ 00431EF0
    got-mystery-sketch story-flag? not if
        fiona-who 3 char-in-area? fiona-who 52 $32 char-heading? and if
            1 calm offer-action
        then
    then
    fiona-who 4 char-in-area? fiona-who -34 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 5 char-in-area? fiona-who 32 $32 char-heading? and if
        4 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who -3 $32 char-heading? and if
        5 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 46 $32 char-heading? and if
        6 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who -35 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who -41 $32 char-heading? and if
        8 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who -24 $32 char-heading? and if
        9 unhurried offer-action
    then
    ht2f-2-viride-medallion-found story-flag? ht2f-2-viride-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then
    fiona-who $B char-in-area? fiona-who 12 $32 char-heading? and if
        $B unhurried offer-action
    then
;

: house-of-truth-2f-2.phase3 ( -- )   \ 00431F90
    40.0 20.0 -9.0 17.0 20.0 -9.0 40.0 0.0 -9.0 23.2 0.0 -9.0 lights-doorway
    -20.0 16.0 40.0 3.0 16.0 40.0 -20.0 0.0 40.0 3.0 0.0 40.0 lights-doorway
    30.0 16.0 32.4 45.0 16.0 32.4 30.0 0.0 32.4 45.0 0.0 32.4 lights-doorway
;

: house-of-truth-2f-2.act00 ( -- )   \ 00432030
    1 self-scripted
    stalker-who action-end
    stalker-who char-done
    $10 3 $FF char-load
    $B partner-load
    0 part-hidden
    self-wait-done
    fiona-who $28 5 char-sound
    3 char-start
    2 char-start
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $A action
    panic-eased
    0 message-prepare   \ "Who's there? Poor Fiona... Riccardo put you through quite a bit, didn't he? Unl..."
    1 result? if
        scene-play
    then
    3 0 char-remove
    stalker-who action-end
    stalker-who char-done
    0 self-scripted
    over-music-d story-flag-set
    fiona-who 0 char-no-shadow
    $80 exit-check
    $E2 door-open-clear
    $E2 door-lock
    $43 resident-flag-set
    music-stage-end
    3 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    self-idle-or-end
;

: house-of-truth-2f-2.act01 ( -- )   \ 00432100
    self-wait-done
    40.0 54.0 self-turn-to-xz
    self-wait-done
    reach-out-anim play-out
    4 wait-frames
    got-mystery-sketch story-flag-set
    1 part-hidden
    $27 message-param-room
    $27 1 item-give-count
    0 $27 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: house-of-truth-2f-2.act02 ( -- )   \ 00432150
    self-wait-done
    $9F 5.3 80.92 -68 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 8.0 0.0 0.0 4.5 event-camera
    1 say   \ "'MAGNUS' Little Box' Is this the painting's title?"
    8 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-2.act03 ( -- )   \ 004321A0
    self-wait-done
    37.0 84.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht2f-2-viride-medallion story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-2f-2.act04 ( -- )   \ 0047AE00
    self-wait-done
    2 say   \ "This closet seems strangely warped at the sides."
    self-idle-or-end
;

: house-of-truth-2f-2.act05 ( -- )   \ 004321F8
    self-wait-done
    look-close-anim $A self-anim-blend
    3 say   \ "Various bobbles used for experimental purposes. Some notebooks have been stacked here."
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-2f-2.act06 ( -- )   \ 0047AE08
    self-wait-done
    4 say   \ "Stacks of books have been arranged neatly on the shelf. Some of the books are in Englis..."
    self-idle-or-end
;

: house-of-truth-2f-2.act07 ( -- )   \ 0047AE10
    self-wait-done
    4 say   \ "Stacks of books have been arranged neatly on the shelf. Some of the books are in Englis..."
    self-idle-or-end
;

: house-of-truth-2f-2.act08 ( -- )   \ 00432210
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    books-remarked ebit? not if
        4 say   \ "Stacks of books have been arranged neatly on the shelf. Some of the books are in En..."
        books-remarked ebit-set
    else
        look-close-anim self-anim
        9 say   \ "One book stands out from the others. It has no title."
        self-wait-anim
        $F 6 fade
        wait-fade
        $A say   \ "Great earthen one, Born in the image of man, Granted an ever so transient life SAL..."
        $F 7 fade
        wait-fade
        hand-to-mouth-anim self-anim
        $B say   \ "Not that it should surprise me, but this book makes no sense."
        self-wait-anim
        books-remarked ebit-clear
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-2.act09 ( -- )   \ 00432250
    self-wait-done
    ht2f-2-rubrum-medallion story-flag? not if
        5 say   \ "What's this? These something wedged in between these two books..."
        reach-out-anim play-out
        4 wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht2f-2-rubrum-medallion story-flag-set
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    else
        4 say   \ "Stacks of books have been arranged neatly on the shelf. Some of the books are in En..."
    then
    self-idle-or-end
;

: house-of-truth-2f-2.act0A ( -- )   \ 004322B0
    [: 2 cutscene-shot? ;] wait-until
    fiona-who 1 char-no-shadow
    [: 3 cutscene-shot? ;] wait-until
    fiona-who 0 char-no-shadow
    self-idle-or-end
;

: house-of-truth-2f-2.act0B ( -- )   \ 004322D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    25 self-turn-angle
    self-wait-done
    6 say   \ "Something is written here..."
    $F 6 fade
    wait-fade
    7 say   \ "Fortis Strengthens body for a limited amount of time."
    $F 7 fade
    wait-fade
    $4B subscreen-bit? not if
        hand-to-mouth-anim self-anim
        8 say   \ "Looks like the name of some medicine... The medicine's effects are explained as well."
        self-wait-anim
        $4B subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-2.act0C ( -- )   \ 00432310
    self-wait-done
    $B 3 $FF char-load
    3 char-start
    $10 4 $FF char-load
    4 char-start
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $A action
    panic-eased
    0 message-prepare   \ "Who's there? Poor Fiona... Riccardo put you through quite a bit, didn't he? Unl..."
    1 result? if
        scene-play
    then
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-2f-2 register-scripts   \ (its phases and actions, by their names)
