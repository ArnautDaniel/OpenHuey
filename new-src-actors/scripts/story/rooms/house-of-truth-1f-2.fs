\ story/rooms/house-of-truth-1f-2.fs - the event scripts of room house-of-truth-1f-2 ($88; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-2
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ room 0x88: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-2.cmd00 ( -- )  slam-shake ;

: house-of-truth-1f-2.enter ( -- )   \ 00432CA0
    room-sounds
    ht1f-2-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        ht1f-2-refined-antimony story-flag? not if
            1 -0.4 71.0 -123.2 flicker-sprite
        then
    then
    0 1 0.812 0.687 0.187 0.312 zone-rect
    ht1f-2-refined-magnesia story-flag? not if
        0 -33.17 71.0 33.19 flicker-sprite
    then
    ht1f-2-prima-magnesia-found story-flag? ht1f-2-prima-magnesia story-flag? not and if
        2 35.0 71.0 33.0 flicker-sprite
    then
    $51 hole-floor tri-floor-on
    $21C hole-floor tri-floor-on
    $2F hole-floor tri-floor-on
    $54 hole-floor tri-floor-on
    $23B hole-floor tri-floor-on
    $43 hole-floor tri-floor-on
    $22C hole-floor tri-floor-on
    $350 hole-floor tri-floor-on
    $41 hole-floor tri-floor-on
    $22D hole-floor tri-floor-on
    $354 hole-floor tri-floor-on
    $232 hole-floor tri-floor-on
    $355 hole-floor tri-floor-on
    $34C hole-floor tri-floor-on
    $35C hole-floor tri-floor-on
    $351 hole-floor tri-floor-on
    $48 hole-floor tri-floor-on
    $235 hole-floor tri-floor-on
    $356 hole-floor tri-floor-on
    $234 hole-floor tri-floor-on
    $4B hole-floor tri-floor-on
    $23D hole-floor tri-floor-on
    $56 hole-floor tri-floor-on
    $31 hole-floor tri-floor-on
    $21E hole-floor tri-floor-on
    $53 hole-floor tri-floor-on
;

: house-of-truth-1f-2.char-enter ( -- )   \ 00432DF0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            6 6 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        6 6 player-camera
    then then
    0 6 6 area-camera
    fiona-who self-is? if
        $FE exit-taken? if
            fiona-who $28 -60.0 42.0 180 char-to-xz
            0 0 player-camera
            0 fiona-who 2 action
        then
        $80 exit-taken? if
            fiona-who $3F -23.0 25.0 -90 char-to-xz
            0 0 player-camera
            $88 0 64 hewie-to-room
            hewie-who $40 -90 char-to-tri-facing
            0 fiona-who 3 action
        then
    then
;

: house-of-truth-1f-2.phase1 ( -- )   \ 00432E90
    2 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 -60.0 20.0 50.0 0 0 sound
            2 ebit-set
        then
    else
        $C0000008 6 -60.0 20.0 50.0 0 0 sound
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000009 6 -78.0 21.0 46.5 0 0 sound
            else 0 1 var? if
                $4000000A 6 -78.0 21.0 46.5 0 0 sound
            else 0 2 var? if
                $4000000B 6 -78.0 21.0 46.5 0 0 sound
            else 0 3 var? if
                $4000000C 6 -78.0 21.0 46.5 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    4 0 0 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 2 2 1 chars-area-camera
    7 3 3 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 5 5 1 chars-area-camera
    $10 5 5 1 chars-area-camera
    $11 5 5 1 chars-area-camera
    $17 0 0 1 chars-area-camera
    $18 7 7 1 chars-area-camera
    $19 3 3 1 chars-area-camera
    $1A 6 6 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        $27 room-preload
    then
    fiona-who $15 char-entered-area? fiona-who $16 char-entered-area? or if
        1 map-page
    then
    fiona-who $15 char-left-area? fiona-who $16 char-left-area? or if
        2 map-page
    then
    1 -61.4 0.0 46.49 $14 18 0 zone
    1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 hewie-who $88 action
                    then
                then
            then
        then
    then
    1 -61.4 0.0 46.49 $14 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    fiona-who 0 char-action? in-panic? not and fiona-who 1 char-entered-area? and if
        0 fiona-who 7 action
    then
    ht1f-2-prima-magnesia-found story-flag? not if
        3 35.0 70.0 33.0 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 ht1f-2-prima-magnesia-found var-set
                $1A ht1f-2-prima-magnesia var-set
                $1B 2 var-set
                $1C 35000 var-set
                $1D 71000 var-set
                $1E 33000 var-set
                $1F 3 var-set
                0 hewie-who $8B action
            then
        then
    then
    house-of-truth-1f-2.cmd00
;

: house-of-truth-1f-2.phase2 ( -- )   \ 004330C0
    fiona-who $12 char-in-area? fiona-who 0 $32 char-heading? and if
        0 unhurried offer-action
    then
    fiona-who $13 char-in-area? fiona-who 0 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    fiona-who $14 char-in-area? fiona-who 0 $32 char-heading? and if
        1 unhurried offer-action
    then
    ht1f-2-refined-magnesia story-flag? not if
        2 0 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    ht1f-2-breakable-broken story-flag? not if
        0 -0.4 70.0 -123.2 5 8 1 zone
        self-who 0 char-in-zone? if
            ht1f-2-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -0.4 70.0 -123.2 0 -2145378272 0 0.0 shards-at
            $88 5 -0.4 70.0 -123.2 0 0 sound
            $40 $34E noise
            1 -0.4 71.0 -123.2 flicker-sprite
        then
    else ht1f-2-refined-antimony story-flag? not if
        0 1 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 calm offer-action
        then
    then then
    ht1f-2-prima-magnesia-found story-flag? ht1f-2-prima-magnesia story-flag? not and if
        3 2 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: house-of-truth-1f-2.phase3 ( -- )   \ 004331B0
    -20.0 76.0 -132.0 20.0 76.0 -132.0 -20.0 50.0 -132.0 20.0 50.0 -132.0 lights-doorway
    38.0 78.0 -60.2 70.0 55.0 -60.2 38.0 60.0 -60.2 70.0 40.0 -60.2 lights-doorway
    10.0 73.0 -85.5 43.0 73.0 -85.5 10.0 40.0 -85.5 43.0 40.0 -85.5 lights-doorway
    -43.0 73.0 -85.5 -10.0 73.0 -85.5 -43.0 40.0 -85.5 -10.0 40.0 -85.5 lights-doorway
    -70.0 55.0 -60.2 -38.0 78.0 -60.2 -70.0 40.0 -60.2 -38.0 60.0 -60.2 lights-doorway
;

: house-of-truth-1f-2.act00 ( -- )   \ 004332B0
    0 ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        fiona-who 0 char-file-load
        $117 0.0 -1.1 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        0 message   \ "I hope this water is pure..."
        wait-message
        fiona-who char-file-use
        4 room-motion $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        fiona-who $84 5 char-sound
        self-frames-reset
        $1E self-wait-frames
        got-doc-recovery story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message   \ "'Recovery Points' Search a recovery point to calm Fiona from a state of pan..."
            wait-message
            $F 7 fade
            wait-fade
            got-doc-recovery story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message   \ "has been saved to your file."
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0.0 5.0 self-turn-to-xz
        self-wait-done
        1 message   \ "Ah, that tasted good."
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act01 ( -- )   \ 00433370
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -60.0 55.0 self-turn-to-xz
    self-wait-done
    look-close-anim $A self-anim-blend
    self-wait-anim
    2 message   \ "Unsettling sounds and smells waft forth from the eerie hole. Enter the hole? YES NO"
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        fiona-who slide-sound 5 char-sound
        world-held state-flag-set
        $FE exit-check
    else
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act02 ( -- )   \ 004333B0
    1 self-scripted
    self-wait-done
    camera-restart
    fiona-who slide-sound 5 char-sound
    1 map-page
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-2.act03 ( -- )   \ 004333D0
    1 self-scripted
    self-wait-done
    1 map-page
    $27 room-preload
    $F $41 fade
    wait-fade
    summoner-on state-flag-set
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-2.act04 ( -- )   \ 004333F0
    self-wait-done
    -33.17 33.19 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            ht1f-2-refined-magnesia story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act05 ( -- )   \ 00433450
    self-wait-done
    -0.4 -123.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            ht1f-2-refined-antimony story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act06 ( -- )   \ 004334B0
    self-wait-done
    35.0 33.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $98 message-param-room
        $98 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            ht1f-2-prima-magnesia story-flag-set
            2 effect-remove
            $98 1 item-give-count
            0 $98 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act07 ( -- )   \ 00433510
    self-wait-done
    90 self-turn-angle
    self-wait-done
    turn-round-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    3 message   \ "If I went back I'm sure I would get lost. Better to keep moving forward."
    wait-message
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-2 register-scripts   \ (its phases and actions, by their names)
