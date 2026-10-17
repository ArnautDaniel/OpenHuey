\ story/rooms/castle-2f-6.fs - the event scripts of room castle-2f-6 ($22; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-6
USING: room-names story.words story.shared flag-names story-names ;

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
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 -1 player-camera
    then then
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
                            $1F 0 var-set
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
                $1F 1 var-set
                $1F 35.0 hewie-look-zone
            then
        then
    then
    2 -28.72 0.0 -20.26 $D 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-6.phase2 ( -- )   \ 00400D90
    c2f-6-lavender story-flag? not if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    got-lunar-refractor story-flag? not if
        fiona-who 5 char-in-area? fiona-who 0 $2D char-heading? and if
            5 3 0 scene-change
        then
    then
    fiona-who 6 char-in-area? fiona-who -45 $2D char-heading? and if
        5 1 0 scene-change
    then
    fiona-who $F char-in-area? fiona-who -67 $32 char-heading? and if
        5 6 0 scene-change
    then
    fiona-who 7 char-in-area? fiona-who 9 char-in-area? or fiona-who -45 $2D char-heading? and if
        5 2 0 scene-change
    then
    fiona-who 8 char-in-area? fiona-who $B char-in-area? or fiona-who 0 $2D char-heading? and if
        5 2 0 scene-change
    then
    fiona-who $A char-in-area? fiona-who $D char-in-area? or fiona-who 45 $2D char-heading? and if
        5 2 0 scene-change
    then
    fiona-who $C char-in-area? fiona-who 90 $2D char-heading? and if
        5 2 0 scene-change
    then
    fiona-who $E $32 char-faces-area? if
        5 4 0 scene-change
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
    $902 self-anim
    self-wait-anim
    fiona-who 3 6 char-sound
    $903 self-anim
    self-wait-anim
    3 message-param-room
    $8019 message   \ "Used ."
    wait-message
    fiona-who $55 3.622 32.279 0 char-to-xz
    1 -17.0 -10.0 30.0 0.0 event-camera
    self-frames-reset
    8 self-wait-frames
    0 0 object-anim
    $8000 $A self-anim-blend
    self-frames-reset
    $14 self-wait-frames
    fiona-who 0 6 char-sound
    self-wait-anim
    self-frames-reset
    self-wait-16
    2 char-start
    got-lunar-refractor story-flag-set
    3 item-use
    4 message-param-room
    4 1 item-give-count
    0 4 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    1 0 0 music
    fiona-who 1 6 char-sound
    0 1 object-anim
    $8001 0 self-anim-blend
    self-wait-anim
    self-frames-reset
    self-wait-16
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
    $C message   \ "There is something scribbled on the board..."
    wait-message
    $F 6 fade
    wait-fade
    $F message   \ "'Luminessants' Luminessants are tiny creatures that respond to the ethereal energies g..."
    wait-message
    $F 7 fade
    wait-fade
    got-doc-luminessants story-flag? not if
        $1D01 $A self-anim-blend
        $D message   \ "'Luminessants'...'Azoth'... No clue what any of this means."
        wait-message
        self-wait-anim
    else
        $E message   \ "Luminessants... judging by the ring of that, I'll bet these are those lightning bu..."
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-6.act02 ( -- )   \ 00400F50
    self-wait-done
    1 ebit? not if
        8 message   \ "Old scholarly masterpieces fill up the shelves. It's almost as if someone has been..."
        wait-message
        1 ebit-set
    else
        9 message   \ "Not to state the obvious, but this is clearly the castle study."
        wait-message
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
    $8002 $A self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    fiona-who 2 6 char-sound
    self-frames-reset
    $B self-wait-frames
    fiona-who 2 6 char-sound
    self-frames-reset
    8 self-wait-frames
    fiona-who 2 6 char-sound
    self-wait-anim
    3 message   \ "The desk is locked."
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-6.act04 ( -- )   \ 00400FC0
    self-wait-done
    0 ebit? not if
        $A message   \ "A massive book stands stalwart. It looks quite old. Perhaps it's an antique?"
        wait-message
        0 ebit-set
    else
        $A01 $A self-anim-blend
        $B message   \ "The script looks almost Latin in nature... But unfortunately, it's all Greek to me."
        wait-message
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
    $10 message   \ "Various formulae and computations have been scribbled on the desktop. This goes way b..."
    wait-message
    self-idle-or-end
;

: castle-2f-6.act07 ( -- )   \ 00401010
    self-wait-done
    -22.26 -41.95 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-6-lavender story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-6.enter castle-2f-6 0 room-script!
' castle-2f-6.char-enter castle-2f-6 6 room-script!
' castle-2f-6.phase1 castle-2f-6 1 room-script!
' castle-2f-6.phase2 castle-2f-6 2 room-script!
' castle-2f-6.phase3 castle-2f-6 3 room-script!
' castle-2f-6.act00 castle-2f-6 $00 action-script!
' castle-2f-6.act01 castle-2f-6 $01 action-script!
' castle-2f-6.act02 castle-2f-6 $02 action-script!
' castle-2f-6.act03 castle-2f-6 $03 action-script!
' castle-2f-6.act04 castle-2f-6 $04 action-script!
' castle-2f-6.act05 castle-2f-6 $05 action-script!
' castle-2f-6.act06 castle-2f-6 $06 action-script!
' castle-2f-6.act07 castle-2f-6 $07 action-script!
