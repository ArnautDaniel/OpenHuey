\ story/rooms/old-mansion-1f-4.fs - the event scripts of room old-mansion-1f-4 ($42; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-4
USING: room-names story.words story.shared flag-names story-names ;

: old-mansion-1f-4.enter ( -- )   \ 004070F0
    room-sounds
    $12 1.0 0 bgm
    om1f-4-flag1 story-flag? not if
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $1C 3 char-to-tri
    2 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
    then
    0 $F1 4 action
    hunted state-flag-set
    om1f-4-chicken-jerky story-flag? not if
        0 -51.07 1.0 19.59 flicker-sprite
    then
    1 $2300 sound-volume
    1 $77 $10000000 nav-tri-flags
    1 $48 $10000000 nav-tri-flags
    1 $75 $10000000 nav-tri-flags
    1 $46 $10000000 nav-tri-flags
    1 $52 $10000000 nav-tri-flags
    1 $81 $10000000 nav-tri-flags
    1 2 $10000000 nav-tri-flags
    1 $61 $10000000 nav-tri-flags
    1 $70 $10000000 nav-tri-flags
    1 $3E $10000000 nav-tri-flags
    1 $39 $10000000 nav-tri-flags
    1 0 $10000000 nav-tri-flags
    1 $71 $10000000 nav-tri-flags
    1 $3F $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $7A $10000000 nav-tri-flags
    1 $4A $10000000 nav-tri-flags
    1 $79 $10000000 nav-tri-flags
    1 $45 $10000000 nav-tri-flags
    1 $74 $10000000 nav-tri-flags
    1 $5F $10000000 nav-tri-flags
    1 $85 $10000000 nav-tri-flags
    1 $51 $10000000 nav-tri-flags
    1 $80 $10000000 nav-tri-flags
    1 $4C $10000000 nav-tri-flags
    1 $7B $10000000 nav-tri-flags
    1 $3C $10000000 nav-tri-flags
    1 $6F $10000000 nav-tri-flags
    1 $4E $10000000 nav-tri-flags
    1 $7D $10000000 nav-tri-flags
    1 $86 $10000000 nav-tri-flags
    1 $60 $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $83 $10000000 nav-tri-flags
    1 6 $10000000 nav-tri-flags
    1 $63 $10000000 nav-tri-flags
    1 $14 $10000000 nav-tri-flags
    1 $6D $10000000 nav-tri-flags
    1 $4D $10000000 nav-tri-flags
    1 $7C $10000000 nav-tri-flags
    1 $18 $10000000 nav-tri-flags
    1 $23 $10000000 nav-tri-flags
    1 $41 $10000000 nav-tri-flags
    1 $72 $10000000 nav-tri-flags
    1 $76 $10000000 nav-tri-flags
    1 $47 $10000000 nav-tri-flags
    1 $78 $10000000 nav-tri-flags
    1 $49 $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
    1 $82 $10000000 nav-tri-flags
    1 $5A $10000000 nav-tri-flags
    1 $84 $10000000 nav-tri-flags
    1 $3B $10000000 nav-tri-flags
;

: old-mansion-1f-4.char-enter ( -- )   \ 00407300
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
;

: old-mansion-1f-4.phase1 ( -- )   \ 00407340
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 2 2 1 chars-area-camera
    0 55.0 0.0 -20.0 5 10 1 zone
    self-who 0 char-in-zone? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 21 var? if
        $1C 2 6 char-sound
    then
    2 66 var? if
        $1C 2 6 char-sound
    then
    1 45.1 0.0 31.88 $1C 18 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
    2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    2 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-4.phase2 ( -- )   \ 00407430
    fiona-who 5 char-in-area? fiona-who 20 $32 char-heading? and if
        2 unhurried offer-action
    then
    om1f-4-chicken-jerky story-flag? not if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    fiona-who 7 char-in-area? fiona-who 90 $3C char-heading? and if
        6 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who 90 $3C char-heading? and if
        7 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who 90 $3C char-heading? and if
        8 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 0 $3C char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 0 $3C char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 90 $3C char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $D char-in-area? fiona-who 90 $3C char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who 0 $3C char-heading? and if
        $B unhurried offer-action
    then
    fiona-who $F char-in-area? fiona-who 0 $3C char-heading? and if
        $C unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who 0 $3C char-heading? and if
        $D unhurried offer-action
    then
;

: old-mansion-1f-4.act0E ( -- )   \ 00407A20
    4 6 -30.0 10.0 20.0 0 0 sound
    self-frames-reset
    self-wait-16
    fiona-who $43 5 char-sound
    $3C threat-raise
    1 $FF 8 rumble
    $F04 self-anim
    self-wait-anim
    0 0 $14 door-bits
    1 1 $14 door-bits
    $42 0 105 $80 5 -1 $68 40.0 creature-place
    $42 0 23 $80 10 -1 $5A 0.0 creature-place
    om1f-4-flag1 story-flag-set
    exit
;

: old-mansion-1f-4.act00 ( -- )   \ 004074F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    5 message   \ "Oh my! It actually transformed..."
    wait-message
    $902 self-anim
    self-wait-anim
    $B item-use
    $C message-param-room
    $C 1 item-give-count
    0 $C item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    got-red-godstone story-flag-set
    $903 self-anim
    self-wait-anim
    om1f-4-flag1 story-flag? not if
        old-mansion-1f-4.act0E
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act01 ( -- )   \ 00407560
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 9 var? if
        9 message-param-room
        0 9 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 10 var? if
        $A message-param-room
        0 $A item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    2 message   \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
    wait-message
    om1f-4-flag1 story-flag? not if
        old-mansion-1f-4.act0E
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act02 ( -- )   \ 00407680
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        0 message   \ "Transmuter Synthesizing two objects together is a task any fool could surely comple..."
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        1 message   \ "A transmuter? I've never heard of such a device before... It appears you can insert..."
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $245 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act03 ( -- )   \ 004076D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    $F 6 fade
    wait-fade
    4 message
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act04 ( -- )   \ 00407700
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        1 ebit? if
            2 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                stalker-who 0 stalker-command
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            fiona-who $8F 5 char-sound
            $1C $9001 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9000 0 3 char-anim-hold
            2 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9000 0 0 char-anim-hold
            2 0 var-set
        else
            2 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-1f-4.act05 ( -- )   \ 004077C0
    self-wait-done
    -51.07 19.59 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-4-chicken-jerky story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act06 ( -- )   \ 00407820
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $62 6.25 -23.4 173 char-to-xz
    self-who 3 -1 char-camera
    self-frames-reset
    4 self-wait-frames
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message   \ "Is this some kind of incubator? There is something inside..."
        wait-message
        3 ebit-set
    else
        7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
        wait-message
    then
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    2 2 player-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act07 ( -- )   \ 00407880
    self-wait-done
    -20.0 -40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message   \ "Is this some kind of incubator? There is something inside..."
        wait-message
        3 ebit-set
    else
        7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act08 ( -- )   \ 004078B0
    self-wait-done
    -48.0 -40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message   \ "Is this some kind of incubator? There is something inside..."
        wait-message
        3 ebit-set
    else
        7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act09 ( -- )   \ 004078E0
    self-wait-done
    -10.0 0.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message   \ "Is this some kind of incubator? There is something inside..."
        wait-message
        3 ebit-set
    else
        7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act0A ( -- )   \ 00407910
    self-wait-done
    -40.0 0.0 self-turn-to-xz
    self-wait-done
    om1f-4-flag1 story-flag? not if
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 ebit? not if
            6 message   \ "Is this some kind of incubator? There is something inside..."
            wait-message
            3 ebit-set
        else
            7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
            wait-message
        then
        self-wait-anim
    else
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        8 message   \ "It's empty now. I can't believe that thing inside was alive..."
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act0B ( -- )   \ 00407950
    self-wait-done
    8.0 40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message   \ "Is this some kind of incubator? There is something inside..."
        wait-message
        3 ebit-set
    else
        7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act0C ( -- )   \ 00407980
    om1f-4-flag1 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        fiona-who $50 -17.5 26.5 -12 char-to-xz
        self-who 4 -1 char-camera
        self-frames-reset
        4 self-wait-frames
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 ebit? not if
            6 message   \ "Is this some kind of incubator? There is something inside..."
            wait-message
            3 ebit-set
        else
            7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
            wait-message
        then
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        1 1 player-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        8 message   \ "It's empty now. I can't believe that thing inside was alive..."
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act0D ( -- )   \ 004079F0
    self-wait-done
    -48.0 40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message   \ "Is this some kind of incubator? There is something inside..."
        wait-message
        3 ebit-set
    else
        7 message   \ "It just looks like a clump of meat. It's all fatty and gross."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-4 register-scripts   \ (its phases and actions, by their names)
