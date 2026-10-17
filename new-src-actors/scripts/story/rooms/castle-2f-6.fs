\ story/rooms/castle-2f-6.fs - the event scripts of room castle-2f-6 ($22; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-6
USING: room-names story.words story.shared flag-names story-names fiona.anims ;

: castle-2f-6.enter ( -- )   \ 00400C60
    got-lunar-refractor story-flag? not if
        room-sounds
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    c2f-6-lavender story-flag? not if
        0 -22.26 8.5 -41.95 flicker-sprite
    then
;

: castle-2f-6.char-enter ( -- )   \ 00400C90
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 -1 player-camera
    then
    0 1 -1 area-camera
;

: castle-2f-6.phase1 ( -- )   \ 00400CD0
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    got-lunar-refractor story-flag? not if
        0 3.5 0.0 33.0 $1E 20 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            hewie-zone-var 0 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    1 3.07 0.0 30.96 $D 35 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 35.0 hewie-look-zone
            then
        then
    then
    2 -28.72 0.0 -20.26 $D 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-6.phase2 ( -- )   \ 00400D90
    c2f-6-lavender story-flag? not if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            7 calm offer-action
        then
    then
    got-lunar-refractor story-flag? not if
        fiona-who 5 char-in-area? fiona-who 0 $2D char-heading? and if
            3 unhurried offer-action
        then
    then
    fiona-who 6 char-in-area? fiona-who -45 $2D char-heading? and if
        1 unhurried offer-action
    then
    fiona-who $F char-in-area? fiona-who -67 $32 char-heading? and if
        6 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 9 char-in-area? or fiona-who -45 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who $B char-in-area? or fiona-who 0 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who $D char-in-area? or fiona-who 45 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 90 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $E $32 char-faces-area? if
        4 unhurried offer-action
    then
;

: castle-2f-6.phase3 ( -- )   \ 00400E30
    camera-setup-changed? if
        0 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
;

: castle-2f-6.act00 ( -- )   \ 00400E50
    2 0 char-remove
    7 partner-load
    reach-out-anim play-out
    fiona-who 3 6 char-sound
    reach-back-anim play-out
    3 message-param-room
    $8019 say   \ "Used ."
    fiona-who $55 3.622 32.279 0 char-to-xz
    1 -17.0 -10.0 30.0 0.0 event-camera
    8 wait-frames
    0 0 object-anim
    0 room-motion $A self-anim-blend
    $14 wait-frames
    fiona-who 0 6 char-sound
    self-wait-anim
    16 wait-frames
    2 char-start
    got-lunar-refractor story-flag-set
    3 item-use
    4 message-param-room
    4 1 item-give-count
    0 4 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    1 0 0 music
    fiona-who 1 6 char-sound
    0 1 object-anim
    1 room-motion 0 blend-out
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    summoner-on state-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-6.act01 ( -- )   \ 00400F10
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C say   \ "There is something scribbled on the board..."
    $F 6 fade
    wait-fade
    $F say   \ "'Luminessants' Luminessants are tiny creatures that respond to the ethereal energies g..."
    $F 7 fade
    wait-fade
    got-doc-luminessants story-flag? not if
        hand-to-mouth-anim $A self-anim-blend
        $D say   \ "'Luminessants'...'Azoth'... No clue what any of this means."
        self-wait-anim
    else
        $E say   \ "Luminessants... judging by the ring of that, I'll bet these are those lightning bu..."
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-6.act02 ( -- )   \ 00400F50
    self-wait-done
    1 ebit? not if
        8 say   \ "Old scholarly masterpieces fill up the shelves. It's almost as if someone has been..."
        1 ebit-set
    else
        9 say   \ "Not to state the obvious, but this is clearly the castle study."
    then
    self-idle-or-end
;

: castle-2f-6.act03 ( -- )   \ 00400F70
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    fiona-who 1 char-file-load
    $55 3.622 32.279 0 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    2 room-motion $A self-anim-blend
    $16 wait-frames
    fiona-who 2 6 char-sound
    $B wait-frames
    fiona-who 2 6 char-sound
    8 wait-frames
    fiona-who 2 6 char-sound
    self-wait-anim
    3 say   \ "The desk is locked."
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-6.act04 ( -- )   \ 00400FC0
    self-wait-done
    0 ebit? not if
        $A say   \ "A massive book stands stalwart. It looks quite old. Perhaps it's an antique?"
        0 ebit-set
    else
        look-low-anim $A self-anim-blend
        $B say   \ "The script looks almost Latin in nature... But unfortunately, it's all Greek to me."
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-6.act05 ( -- )   \ 00400FE0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    fiona-who 1 char-file-load
    $55 3.622 31.0 0 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    ['] castle-2f-6.act00 goto
;

: castle-2f-6.act06 ( -- )   \ 00401000
    self-wait-done
    -135 self-turn-angle
    self-wait-done
    $10 say   \ "Various formulae and computations have been scribbled on the desktop. This goes way b..."
    self-idle-or-end
;

: castle-2f-6.act07 ( -- )   \ 00401010
    self-wait-done
    -22.26 -41.95 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c2f-6-lavender story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
castle-2f-6 register-scripts   \ (its phases and actions, by their names)
