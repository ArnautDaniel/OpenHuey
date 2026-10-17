\ story/rooms/castle-1f-1.fs - the event scripts of room castle-1f-1 ($1; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

: castle-1f-1.enter ( -- )   \ 003EE770
    garden-door-done story-flag? c1f-1-flag1 story-flag? not and if
        c1f-1-flag1 story-flag-set
        1 0 443 4 8 -1 0 0.0 creature-place
    then
    c1f-3-flag1 story-flag? not if
        1 exit-door-open? if
            4 0.1 0 bgm
        else
            4 0.01 0 bgm
        then
    then
    c1f-2-flag1 story-flag? not if
        1 part-shown
        0 solid-floor floor-on
    else
        0 part-shown
    then
    c2f-3-flag1 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $A 103.4 28.8 -51.9 $A $80 $80 $80 $20 specks
        $10 83.3 29.0 14.8 $E $80 $80 $80 $50 specks
        $10 -44.8 13.0 -13.0 $E $80 $80 $80 $40 specks
        $C -87.5 12.9 36.0 $A $80 $80 $80 $40 specks
    then
;

: castle-1f-1.char-enter ( -- )   \ 003EE820
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 -1 player-camera
    then then
    0 2 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 -1 player-camera
    then then
    1 0 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 -1 player-camera
    then then
    2 0 -1 area-camera
    fiona-who self-is? if
        c1f-3-bookshelf-examined story-flag? c1f-1-flag2 story-flag? not and if
            0.0 sound-volume-scale
            0 -1 player-camera
            fiona-who $B7 0 char-to-tri-facing
            c1f-3-flag2 story-flag? if
                1 0 306 hewie-to-room
                hewie-who $132 0 char-to-tri-facing
            then
            c1f-1-flag2 story-flag-set
            0 ebit-set
            0 $F1 1 action
        then
    then
;

: castle-1f-1.phase1 ( -- )   \ 003EE920
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    8 0 -1 1 chars-area-camera
    9 1 0 1 chars-area-camera
    $A 1 0 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    $C 0 -1 -1 chars-area-camera
    got-doc-crouch-evasion story-flag? not if
        $D 3 -1 1 chars-area-camera
    then
    c1f-2-flag1 story-flag? not if
        fiona-who 3 char-entered-area? if
            1 exit-prepare
        then
        fiona-who 4 char-entered-area? if
            0 exit-prepare
        then
    else
        fiona-who 3 char-entered-area? fiona-who 5 char-entered-area? or if
            1 exit-prepare
        then
        fiona-who 4 char-entered-area? if
            0 exit-prepare
        then
        fiona-who 6 char-entered-area? if
            2 exit-prepare
        then
    then
    0 ebit? if
        stalker-who char-here? if
            0 ebit-clear
        then
    then
    c1f-1-refined-magnesia story-flag? not if
        hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
            2 game-mode? not if
                0 control-action? if
                    -40 -16 60 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                        fiona-who -40 60 $32 char-faces-xz? if
                            hewie-may-break-off? if
                                0 fiona-who 3 action
                            then
                        then
                    then
                then
            then
        then
        2 -33.33 -16.93 69.69 $28 6 0 zone
        3 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 2 9 char-zone-bits? if
                        3 ebit-set
                        $64 chance? if
                            $1F 2 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
        2 -33.33 -16.93 69.69 $28 6 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    $1F 2 var-set
                    $1F 6.0 hewie-look-zone
                then
            then
        then
    then
    1 ebit? not if
        0 -46.0 -16.0 70.0 $C 4 0 zone
        fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
            fiona-who 5 -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
            hewie-who $A -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
            stalker-who 5 -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
    then
    2 ebit? not if
        1 -23.0 -16.0 67.0 $C 4 0 zone
        fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
            fiona-who 5 -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
            hewie-who $A -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
            stalker-who 5 -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
    then
    got-doc-crouch-evasion story-flag? not if
        fiona-who $E char-in-area? fiona-free? and if
            2 game-mode? fiona-who stalker-who 70 chars-within? not and 2 game-mode? not or if
                got-doc-crouch-evasion story-flag-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 5 action-force
                else
                    1 fiona-who 5 action-force
                then
            then
        then
    then
    c1f-3-flag1 story-flag? not if
        1 exit-door-open? not if
            4 0.01 0 bgm
        else
            4 0.1 0 bgm
        then
    then
;

: castle-1f-1.phase2 ( -- )   \ 003EEB90
    c1f-2-flag1 story-flag? not if
        fiona-who 7 char-in-area? fiona-who -67 $2D char-heading? and if
            0 unhurried offer-action
        then
    then
    1 exit-door-open? not 0 ebit? and if
        fiona-who 1 char-before-door? if
            0 examine-message   \ "It's too dangerous to go back now..."
        then
    then
;

: castle-1f-1.phase3 ( -- )   \ 003EEBC0
    -41.7 -6.2 58.0 -70.6 -6.2 58.0 -41.7 -18.6 58.0 -70.6 -18.6 58.0 lights-doorway
;

: castle-1f-1.phase5 ( -- )   \ 003EEC00
    0 ebit? stalker-who char-here? not and stalker-who 2 char-cond? and if
        stalker-who action-end
        2 summon-take
    then
    hewie-who char-busy? if
        hewie-who action-end
        hewie-who $FE -40.709 45.642 180 char-to-xz
        stalkers-stay state-flag-clear
    then
;

: castle-1f-1.act00 ( -- )   \ 003EEC28
    self-wait-done
    hands-clasped-anim $A self-anim-blend
    1 message   \ "The gate is closed. There's no way through. I am going to have to find another way."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-1.act01 ( -- )   \ 003EEC40
    fiona-who $28 5 char-sound
    5 0 0 music
    force-followed state-flag-set
    yield
    force-followed state-flag-clear
    world-held state-flag-clear
    items-40-as-70 story-flag? not if
        $22A item-add
    then
    $F $51 fade
    wait-fade
    items-40-as-70 story-flag? not if
        $C $85 0.0 0.0 0.0 0 0 sound
    then
    self-idle-or-end
;

: castle-1f-1.act02 ( -- )   \ 0047A980
    self-wait-done
    0 message   \ "It's too dangerous to go back now..."
    self-idle-or-end
;

: castle-1f-1.act03 ( -- )   \ 003EEC80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-who char-busy? not if
        0 hewie-who 4 action-force
        go-for-it-gesture-anim self-anim
        fiona-who 1 char-wait-motion
        fiona-who $2E 5 char-sound
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
    else
        stalkers-stay state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-1.act04 ( -- )   \ 003EECB0
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $1E0 -35.67 31.26 -10 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $1BD -42.443 71.623 1.0 45 hewie-go-to
    self-wait-done
    $1BE -31.549 70.055 150 $FFFF 5 self-move-to
    self-wait-done
    sit-a-while-anim $A self-anim-blend
    self-wait-anim
    fiona-half-hidden state-flag? fiona-who char-busy? not or if
        $95 $63 item-count? not if
            10 hewie-trust
        then
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-1-refined-magnesia story-flag-set
            $95 1 item-give-count
            0 $95 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
    then
    $1C0 -42.94 81.76 176 $FFFF 5 self-move-to
    self-wait-done
    $FE -40.709 45.642 1.0 45 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-1.act05 ( -- )   \ 003EED70
    1 self-scripted
    self-wait-done
    world-frozen state-flag-set
    draw-when-frozen state-flag-set
    $F 6 fade
    wait-fade
    $40AA message   \ "'Crouch Evasion' In addition to evasion points, you can avoid enemies by pressing t..."
    wait-message
    $F 7 fade
    wait-fade
    got-doc-crouch-evasion story-flag-set
    $AA message-param-room
    $AA 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message   \ "has been saved to your file."
    wait-message
    draw-when-frozen state-flag-clear
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
castle-1f-1 register-scripts   \ (its phases and actions, by their names)
