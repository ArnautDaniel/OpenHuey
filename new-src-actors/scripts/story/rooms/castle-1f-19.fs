\ story/rooms/castle-1f-19.fs - the event scripts of room castle-1f-19 ($1E; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-19
USING: room-names story.words story.shared flag-names story-names ;

: castle-1f-19.enter ( -- )   \ 003FD9C0
    room-sounds
    $1A 1.0 0 bgm
    hewie-who char-here? 119 hewie-action? and if
        0 0 8 nav-group
        1 0 $B0 nav-group
        hewie-who $12C char-on-tri? not if
            hewie-who $12C -12.0 -106.0 0 char-to-xz
            1800 2 hewie-anim
        then
    else
        0 0 $30 nav-group
        1 0 $88 nav-group
        hewie-who 0 char-in-nav-group? if
            hewie-who $47 char-to-tri
        then
    then
    $10 41.5 40.7 -137.7 $E $80 $80 $80 $40 specks
    $10 5.9 34.6 -158.4 $E $80 $80 $80 $40 specks
;

: castle-1f-19.char-enter ( -- )   \ 003FDA40
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
;

: castle-1f-19.phase1 ( -- )   \ 003FDB00
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    fiona-who 6 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 4 char-entered-area? fiona-who 7 char-entered-area? or if
        2 exit-prepare
    then
    hewie-who char-here? if
        118 hewie-action? not if
            4 -11.187 5.0 -117.743 $1E 20 0 zone
            5 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 4 9 char-zone-bits? if
                            5 ebit-set
                            $1E chance? if
                                $1F 4 var-set
                                0 hewie-who $88 action
                            then
                        then
                    then
                then
            then
            35 fiona-started? hewie-who 2 char-cond? not and if
                fiona-who 8 char-in-area? stalker-who char-here? not and fiona-who -13 -110 $32 char-faces-xz? and if
                    hewie-may-break-off? if
                        0 hewie-who 0 action
                    then
                else c1f-19-rubrum-medallion story-flag? not if
                    2 game-mode? not if
                        68 0 -274 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                            fiona-who 68 -274 $32 char-faces-xz? hewie-who char-busy? not and if
                                hewie-may-break-off? if
                                    0 fiona-who 7 action
                                then
                            then
                        then
                    then
                then then
            then
        else
            44 fiona-started? if
                0 hewie-who 1 action
            then
            in-panic? not if
                fiona-who char-busy? not fiona-half-hidden state-flag? or if
                    2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
                        stalker-who 2 char-cond? not stalker-who 8 char-in-area? and if
                            0 counter-set
                            0 hewie-who 3 action
                            0 stalker-who 4 action
                            panic-eased
                            fiona-who 0 char-action? if
                                0 fiona-who 2 action-force
                            else
                                1 fiona-who 2 action-force
                            then
                        then
                    then
                then
            then
        then
    then
    6 46.51 0.0 -267.37 $1C 6 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 ebit? not if
        0 46.0 0.0 -280.0 $10 4 0 zone
        fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
            fiona-who 5 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
            hewie-who $A 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
            stalker-who 5 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
    then
    2 ebit? not if
        1 52.0 0.0 -240.0 $10 4 0 zone
        fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
            fiona-who 5 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
            hewie-who $A 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
            stalker-who 5 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
    then
    3 ebit? not if
        2 50.0 0.0 -300.0 $10 4 0 zone
        fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
            fiona-who 5 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
        hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
            hewie-who $A 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
        stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
            stalker-who 5 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
    then
    4 ebit? not if
        3 70.0 0.0 -260.0 $10 4 0 zone
        fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
            fiona-who 5 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
        hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
            hewie-who $A 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
        stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
            stalker-who 5 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
    then
    6 ebit? not if
        5 73.0 0.0 -282.0 $10 4 0 zone
        fiona-who 5 3 char-zone-bits? fiona-who 5 3 char-zone-bits-before? not and if
            fiona-who 5 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
        hewie-who 5 3 char-zone-bits? hewie-who 5 3 char-zone-bits-before? not and if
            hewie-who $A 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
        stalker-who 5 3 char-zone-bits? stalker-who 5 3 char-zone-bits-before? not and if
            stalker-who 5 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
    then
;

: castle-1f-19.phase2 ( -- )   \ 003FDEC8
    fiona-who 9 char-in-area? fiona-who 0 $32 char-heading? and if
        5 unhurried offer-action
    then
;

: castle-1f-19.phase5 ( -- )   \ 003FDEE0
    hewie-who char-busy? if
        hewie-who action-end
        hewie-who $118 39.008 -266.892 -80 char-to-xz
        stalkers-stay state-flag-clear
    then
;

: castle-1f-19.act00 ( -- )   \ 003FDF00
    self-wait-done
    hewie-bark
    self-wait-done
    $47 -12.0 -125.0 0 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 0 8 nav-group
    1 0 $30 nav-group
    $12C -12.0 -106.0 0 $204 5 self-move-to
    self-wait-done
    3600 2 hewie-anim
    self-idle-or-end
;

: castle-1f-19.act01 ( -- )   \ 003FDF40
    1 self-scripted
    self-wait-done
    $47 -12.0 -125.0 180 $204 5 self-move-to
    self-wait-done
    -1 self-idle-anim
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-do
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: castle-1f-19.act02 ( -- )   \ 003FDF80
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    $FF 1.0 0 bgm
    self-wait-done
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    self-who 1 char-visible
    force-calm state-flag-set
    yield
    force-calm state-flag-clear
    0 $F9 9 action
    $10 $28 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    stalker-who action-end
    3 summon-take
    0 self-idle-anim
    self-who 0 char-visible
    hewie-who 0 0 char-tint
    0 3 0.0 light
    counter-inc
    $1A 1.0 0 bgm
    $F $41 fade
    wait-fade
    $22E item-give
    $23 resident-flag-set
    5 hewie-trust
    $827A item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-19.act03 ( -- )   \ 003FE060
    1 self-scripted
    1 wait-counter
    0 0 hewie-anim
    0 0 hewie-do
    hewie-who $2F -90 char-to-tri-facing
    hewie-timid state-flag-clear
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: castle-1f-19.act04 ( -- )   \ 0047AA90
    1 self-scripted
    1 wait-counter
    0 self-scripted
    self-idle-or-end
;

: castle-1f-19.act05 ( -- )   \ 003FE090
    self-wait-done
    $32 -10.776 -128.378 0 $FFFF 5 self-move-to
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    4 self-wait-frames
    hewie-who char-here? 118 hewie-action? and hewie-who 0 char-in-nav-group? and if
        2 message   \ "Hewie's eyes glimmer through the thick shadow that surrounds him."
        wait-message
    else 0 ebit? not if
        0 message   \ "There is a hole in the wall. I think I can see light on the other end. This must le..."
        wait-message
        0 ebit-set
    else
        1 message   \ "I'm sure I can use this somehow... But I'm just too big and will never fit in there."
        wait-message
        0 ebit-clear
    then then
    self-frames-reset
    4 self-wait-frames
    $802 self-anim
    self-wait-anim
    -1 self-idle-anim
    self-idle-or-end
;

: castle-1f-19.act06 ( -- )   \ 0047AA98
    self-idle-or-end
;

: castle-1f-19.act07 ( -- )   \ 003FE0E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-who char-busy? not if
        0 hewie-who 8 action-force
    else
        stalkers-stay state-flag-clear
    then
    $C04 self-anim
    fiona-who 1 char-wait-motion
    fiona-who $2E 5 char-sound
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    self-idle-or-end
;

: castle-1f-19.act08 ( -- )   \ 003FE110
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $118 39.008 -266.892 100 $204 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    1 self-noclip
    hewie-who 0 6 char-sound
    $13F 68.224 -274.752 100 $204 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    fiona-half-hidden state-flag? fiona-who char-busy? not or if
        $73 $63 item-count? not if
            10 hewie-trust
        then
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-19-rubrum-medallion story-flag-set
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
    then
    hewie-who 0 6 char-sound
    $118 39.008 -266.892 -80 $FFFF $A self-move-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-19.act09 ( -- )   \ 003FE1C0
    begin
        0 cutscene-shot? not while
        yield
    repeat
    begin
        0 cutscene-shot? while
        hewie-who $C8000000 1 char-tint
        yield
    repeat
    hewie-who 0 0 char-tint
    begin
        $1E3 cutscene-cue-reached? not while
        yield
    repeat
    2 3 0.5 light
    self-idle-or-end
;

: castle-1f-19.act0A ( -- )   \ 003FE1F0
    self-wait-done
    $10 41.5 40.7 -137.7 $E $80 $80 $80 $40 specks
    $10 5.9 34.6 -158.4 $E $80 $80 $80 $40 specks
    2 3 $FF char-load
    3 char-start
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    self-who 1 char-visible
    0 $F9 9 action
    $10 $28 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    self-who 0 char-visible
    hewie-who 0 0 char-tint
    0 3 0.0 light
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-1f-19 register-scripts   \ (its phases and actions, by their names)
