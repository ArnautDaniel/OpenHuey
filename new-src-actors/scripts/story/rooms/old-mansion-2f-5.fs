\ story/rooms/old-mansion-2f-5.fs - the event scripts of room old-mansion-2f-5 ($58; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-5
USING: room-names story.words story.shared flag-names story-names ;

: old-mansion-2f-5.enter ( -- )   \ 0040F910
    room-sounds
    3 0 var-set
    4 0 var-set
    om2f-5-breakable1-broken story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    om2f-5-breakable2-broken story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        om2f-5-medallion story-flag? not if
            0 10.5 1.0 -28.0 flicker-sprite
        then
    then
    0 3 0.0 0.5 0.312 0.187 zone-rect
    $1C $17F char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
    then
    0 $F1 0 action
    hunted state-flag-set
    om3f-1-flag2 story-flag? if
        om2f-5-flag1 story-flag? not if
            om2f-5-flag1 story-flag-set
            $58 0 291 $81 3 -1 0 0.0 creature-place
            $58 0 426 $81 3 -1 0 0.0 creature-place
            $58 0 205 $81 3 -1 0 0.0 creature-place
        then
    then
    om2f-5-viride-medallion-found story-flag? om2f-5-viride-medallion story-flag? not and if
        1 7.38 1.0 20.58 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-2f-5.act03 ( -- )   \ 0040FEC0
    3 ebit-set
    2 ebit-clear
    $15 0 pvar? if
        $A chance? if
            2 ebit-set
        then
    else $15 1 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $15 2 pvar? if
        $32 chance? if
            2 ebit-set
        then
    else $15 3 pvar? if
        $4B chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 stalker-who 4 action
    else
        $78 1 summoner-wait
    then
    $15 pvar-inc
    exit
;

: old-mansion-2f-5.char-enter ( -- )   \ 0040FA20
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
    hewie-controlled? not if
        fiona-who self-is? 6 exit-taken? and if
            fiona-who 6 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 6 exit-taken? and if
        hewie-who 6 char-to-exit
        1 -1 player-camera
    then then
    6 1 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            3 ebit-set
            old-mansion-2f-5.act03
        else
            3 ebit-clear
        then
    then
;

: old-mansion-2f-5.phase1 ( -- )   \ 0040FAB0
    0 exit-usable? if
        0 exit-check
    then
    6 exit-usable? if
        6 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        6 exit-prepare
    then
    om2f-5-breakable1-broken story-flag? not if
        0 -10.3 0.0 -93.8 5 8 1 zone
        self-who 0 char-in-zone? if
            om2f-5-breakable1-broken story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -10.3 0.0 -93.8 0 -2145378256 0 0.0 shards-at
            $88 5 -10.3 0.0 -93.8 0 0 sound
            $40 $E1 noise
        then
    then
    2 -3.0 0.0 43.0 5 10 1 zone
    self-who 2 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 1 var? if
        $1C 2 6 char-sound
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 3 9 char-zone-bits? if
                    4 ebit-set
                    $50 chance? if
                        $1F 3 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    om2f-5-viride-medallion-found story-flag? not if
        4 7.38 0.0 20.58 $A 5 0 zone
        hewie-who 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om2f-5-viride-medallion-found var-set
                $1A om2f-5-viride-medallion var-set
                $1B 1 var-set
                $1C 7380 var-set
                $1D 1000 var-set
                $1E 20580 var-set
                $1F 4 var-set
                0 hewie-who $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        4 var-inc
        4 30 var? if
            4 0 var-set
            3 0 var? if
            else 3 1 var? if
            else 3 2 var? if
            else 3 3 var? if
            then then then then
            3 var-inc
            3 4 var? if
                3 0 var-set
            then
        then
    then
;

: old-mansion-2f-5.phase2 ( -- )   \ 0040FC70
    fiona-who 6 char-in-area? fiona-who 90 $3C char-heading? and if
        stalker-who char-here? not if
            1 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    om2f-5-breakable2-broken story-flag? not if
        1 10.5 0.0 -28.0 5 8 1 zone
        self-who 1 char-in-zone? if
            om2f-5-breakable2-broken story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            10.5 0.0 -28.0 0 -2145378256 0 0.0 shards-at
            $88 5 10.5 0.0 -28.0 0 0 sound
            $40 $1AD noise
            0 10.5 1.0 -28.0 flicker-sprite
        then
    else om2f-5-medallion story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then then
    om2f-5-viride-medallion-found story-flag? om2f-5-viride-medallion story-flag? not and if
        4 1 5 5 0 zone-at-effect
        fiona-who 4 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: old-mansion-2f-5.act00 ( -- )   \ 0040FD30
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            1 0 var-set
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
            $1C $9005 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9004 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9004 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-2f-5.act02 ( -- )   \ 0040FE90
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    1 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-5.act01 ( -- )   \ 0040FDF0
    stalkers-stay state-flag-set
    3 ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $106 $8004 5 -4.6 -231.1 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    1 ebit-clear
    0 avoid-prompt
    self-who 2 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? 1 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] old-mansion-2f-5.act02 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? stalker-who char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] old-mansion-2f-5.act02 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-2f-5.act04 ( -- )   \ 0040FF10
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 6 char-heading-for? 6 exit-door-open? not and if
        6 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        stalker-who 1 char-file-load
    then
    $106 -4.6 -222.0 180 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
        4 stalker-who 1 char-model-op
        $8000 self-anim
        self-wait-anim
        4 stalker-who 0 char-model-op
    else
        $1601 self-anim
        self-wait-anim
    then
    1 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-2f-5.act05 ( -- )   \ 0040FF70
    self-wait-done
    10.5 -28.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-5-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-5.act06 ( -- )   \ 0040FFD0
    self-wait-done
    7.38 20.58 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-5-viride-medallion story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-5.phase5 ( -- )   \ 0047ABD0
    $1C action-end
    $1C char-done
;

\ ---- registered ----
old-mansion-2f-5 register-scripts   \ (its phases and actions, by their names)
