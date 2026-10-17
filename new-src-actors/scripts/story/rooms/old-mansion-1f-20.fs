\ story/rooms/old-mansion-1f-20.fs - the event scripts of room old-mansion-1f-20 ($6C; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-20
USING: room-names story.words story.shared flag-names story-names ;

: old-mansion-1f-20.enter ( -- )   \ 00439160
    room-sounds
    $1C $18C char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 12.0 0.0 0.0 -90 char-to-xyz
        $1C $900C 1 0 char-anim-hold
    then
    0 $F1 3 action
    hunted state-flag-set
    om1f-20-breakable-broken story-flag? not if
        0 1 $14 door-bits
        1 0 $20000 nav-group
    else
        1 1 $14 door-bits
    then
    om1f-20-weeping-stone story-flag? not if
        0 12.3 8.5 177.61 flicker-sprite
    then
    om1f-20-medallion-found story-flag? om1f-20-medallion story-flag? not and if
        1 0.34 1.0 62.0 flicker-sprite
    then
    om1f-20-medallion-sniffed-2 story-flag? om1f-20-medallion-2 story-flag? not and if
        2 13.99 1.0 94.5 flicker-sprite
    then
    om1f-20-medallion-sniffed-3 story-flag? om1f-20-medallion-3 story-flag? not and if
        3 -13.99 1.0 126.99 flicker-sprite
    then
    om1f-20-prima-magnesia-found story-flag? om1f-20-prima-magnesia story-flag? not and if
        4 -42.23 51.0 236.98 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-1f-20.char-enter ( -- )   \ 00439220
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 2 player-camera
    then then
    0 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        3 3 player-camera
    then then
    2 3 3 area-camera
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
    fiona-who self-is? if
        2 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-20.phase1 ( -- )   \ 00439320
    over-music-c story-flag? if
        0 exit-usable? if
            0 exit-check
        then
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
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 3 3 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    fiona-who 4 char-entered-area? fiona-who 6 char-entered-area? or if
        3 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 8 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        2 exit-prepare
    then
    fiona-who $11 char-entered-area? if
        2 map-page
    then
    fiona-who $11 char-left-area? if
        1 map-page
    then
    om1f-20-breakable-broken story-flag? not if
        0 -50.0 50.0 355.0 6 12 0 zone
        fiona-who 0 char-in-zone? if
            fiona-who 5 6 char-sound
            0 0 $14 door-bits
            1 1 $14 door-bits
            0 0 $20000 nav-group
            $64 door-unlock
            om1f-20-breakable-broken story-flag-set
            0 6 0.25 0.5 0.25 0.5 zone-rect
            -53.0 52.0 357.0 0 -2144325584 2 0.0 shards-at
            -50.0 50.0 357.0 0 -2143272896 2 0.0 shards-at
            -47.0 50.0 357.0 0 -2144325584 2 0.0 shards-at
            0 -55.0 55.0 357.0 0 0 0 0 dust
            0 -50.0 52.0 357.0 0 0 0 0 dust
            0 -45.0 54.0 357.0 0 0 0 0 dust
        then
    then
    1 12.0 0.0 0.0 5 10 1 zone
    self-who 1 char-in-zone? if
        2 ebit-set
    else
        2 ebit-clear
    then
    1 0 var? not if
        2 var-inc
        2 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            2 0 var-set
        then
    then
    2 11.58 0.0 -0.41 $17 16 0 zone
    7 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    7 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    2 11.58 0.0 -0.41 $17 16 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    om1f-20-medallion-found story-flag? not if
        5 0.34 0.0 62.0 $A 5 0 zone
        hewie-who 5 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-20-medallion-found var-set
                $1A om1f-20-medallion var-set
                $1B 1 var-set
                $1C 340 var-set
                $1D 1000 var-set
                $1E 62000 var-set
                $1F 5 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-20-medallion-sniffed-2 story-flag? not if
        6 13.99 0.0 94.5 $A 5 0 zone
        hewie-who 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-20-medallion-sniffed-2 var-set
                $1A om1f-20-medallion-2 var-set
                $1B 2 var-set
                $1C 13990 var-set
                $1D 1000 var-set
                $1E 94500 var-set
                $1F 6 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-20-medallion-sniffed-3 story-flag? not if
        7 -13.99 0.0 126.99 $A 5 0 zone
        hewie-who 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-20-medallion-sniffed-3 var-set
                $1A om1f-20-medallion-3 var-set
                $1B 3 var-set
                $1C -13990 var-set
                $1D 1000 var-set
                $1E 126990 var-set
                $1F 7 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-20-prima-magnesia-found story-flag? not if
        8 -42.23 50.0 236.98 $A 5 0 zone
        hewie-who 8 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-20-prima-magnesia-found var-set
                $1A om1f-20-prima-magnesia var-set
                $1B 4 var-set
                $1C -42230 var-set
                $1D 51000 var-set
                $1E 236980 var-set
                $1F 8 var-set
                0 hewie-who $8B action
            then
        then
    then
    om3f-1-flag2 story-flag? over-music-c story-flag? not and 9 ebit? not and if
        fiona-who 0 char-entered-area? if
            9 ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 9 action-force
            else
                1 fiona-who 9 action-force
            then
        then
    then
;

: old-mansion-1f-20.phase2 ( -- )   \ 004396D0
    om1f-20-breakable-broken story-flag? not if
        fiona-who 0 2 char-zone-bits? fiona-who 0 $32 char-heading? and if
            2 unhurried offer-action
        then
    then
    fiona-who $10 $3C char-faces-area? if
        $A unhurried offer-action
    then
    om1f-20-weeping-stone story-flag? not if
        4 0 5 5 0 zone-at-effect
        fiona-who 4 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    om1f-20-medallion-found story-flag? om1f-20-medallion story-flag? not and if
        5 1 5 5 0 zone-at-effect
        fiona-who 5 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    om1f-20-medallion-sniffed-2 story-flag? om1f-20-medallion-2 story-flag? not and if
        6 2 5 5 0 zone-at-effect
        fiona-who 6 3 char-zone-bits? if
            6 calm offer-action
        then
    then
    om1f-20-medallion-sniffed-3 story-flag? om1f-20-medallion-3 story-flag? not and if
        7 3 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            7 calm offer-action
        then
    then
    om1f-20-prima-magnesia-found story-flag? om1f-20-prima-magnesia story-flag? not and if
        8 4 5 5 0 zone-at-effect
        fiona-who 8 3 char-zone-bits? if
            8 calm offer-action
        then
    then
;

: old-mansion-1f-20.act00 ( -- )   \ 0047AEA0
    self-idle-or-end
;

: old-mansion-1f-20.act01 ( -- )   \ 0047AEA4
    self-idle-or-end
;

: old-mansion-1f-20.act02 ( -- )   \ 00439780
    self-wait-done
    0 self-turn-angle
    self-wait-done
    1 ebit? not if
        2 message   \ "The knob has rotted off. I can't open the door like this."
        wait-message
        1 ebit-set
    else
        3 message   \ "The door is completely rotted through and through. A good kick should knock the doo..."
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-20.act03 ( -- )   \ 004397A0
    1 0 var-set
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 12.0 0.0 0.0 -90 char-to-xyz
        $1C $900C 1 0 char-anim-hold
    then
    begin
        2 ebit? if
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
            $1C 1 6 char-sound
            fiona-who $8F 5 char-sound
            $1C $900D 0 3 char-anim-hold
            $1C wait-char-anim
            0 5 var? if
                $1C $900E 0 3 char-anim-hold
                self-frames-reset
                $10 self-wait-frames
                0 0 var-set
                1 7.0 11.0 0.0 5 10 0 zone
                fiona-who 1 3 char-zone-bits? if
                    $1C 0 6 char-sound
                    $1C 1 fiona-thrown
                    3 panic-stage? if
                        4 panic-stage
                    else
                        3 panic-stage
                    then
                then
                $1C wait-char-anim
            then
            $1C $900C 0 3 char-anim-hold
            1 0 var-set
        then
        $1C char-at-motion-event? if
            $1C $900C 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-1f-20.act04 ( -- )   \ 00439880
    self-wait-done
    12.3 177.61 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $9B message-param-room
        $9B $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-20-weeping-stone story-flag-set
            0 effect-remove
            $9B 1 item-give-count
            0 $9B item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-20.act05 ( -- )   \ 004398E0
    self-wait-done
    0.34 62.0 self-turn-to-xz
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
            om1f-20-medallion story-flag-set
            1 effect-remove
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

: old-mansion-1f-20.act06 ( -- )   \ 00439940
    self-wait-done
    13.99 94.5 self-turn-to-xz
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
            om1f-20-medallion-2 story-flag-set
            2 effect-remove
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

: old-mansion-1f-20.act07 ( -- )   \ 004399A0
    self-wait-done
    -13.99 126.99 self-turn-to-xz
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
            om1f-20-medallion-3 story-flag-set
            3 effect-remove
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

: old-mansion-1f-20.act08 ( -- )   \ 00439A00
    self-wait-done
    -42.23 236.98 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $98 message-param-room
        $98 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-20-prima-magnesia story-flag-set
            4 effect-remove
            $98 1 item-give-count
            0 $98 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-20.act09 ( -- )   \ 00439A60
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    wait-fade
    world-held state-flag-set
    scene-locked state-flag-set
    hewie-who action-end
    hewie-who char-done
    0 exit-prepare
    stalkers-stay state-flag-clear
    0 self-scripted
    $80 exit-check
    self-idle-or-end
;

: old-mansion-1f-20.act0A ( -- )   \ 00439A90
    self-wait-done
    0.5 56.7 self-turn-to-xz
    self-wait-done
    4 message   \ "A pair of scales lies in the center of the corridor. A balance in a corridor... Just on..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-20.phase5 ( -- )   \ 0047AE98
    $1C action-end
    $1C char-done
;

\ ---- registered ----
old-mansion-1f-20 register-scripts   \ (its phases and actions, by their names)
