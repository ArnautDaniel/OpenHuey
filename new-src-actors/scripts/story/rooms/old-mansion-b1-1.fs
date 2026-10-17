\ story/rooms/old-mansion-b1-1.fs - the event scripts of room old-mansion-b1-1 ($44; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds hewie.anims ;

\ ---- this room's own event bits and script variables ----
0 constant in-zone1                    \ (bit 0) someone in zone 1 (the figure's hops count)
1 constant hewie-zone2-noticed         \ (bit 1) Hewie has reacted to zone 2 (once a visit)
2 constant hewie-zone3-noticed         \ (bit 2) Hewie has reacted to zone 3 (once a visit)
0 constant hops-var                    \ (var 0) the figure's hops (5: away)
1 constant still-var                   \ (var 1) the figure still (0) or hopping
2 constant call-frames-var             \ (var 2) frames to the figure's next call (60)

: old-mansion-b1-1.enter ( -- )   \ 00415BA0
    omb1-1-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        omb1-1-chicken-jerky story-flag? not if
            0 60.5 0.9 -33.4 flicker-sprite
        then
    then
    0 6 0.812 0.687 0.187 0.312 zone-rect
    $1C $C5 char-to-tri
    still-var 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 39.3 0.0 50.4 -90 char-to-xyz
        $1C $900A 1 0 char-anim-hold
    then
    0 1 track 0 action
    hunted state-flag-set
    omb1-1-album-medallion story-flag? not if
        1 -6.17 39.94 -29.06 flicker-sprite
    then
    1 $2300 sound-volume
    1 solid-floor floor-on
    $51 hole-floor tri-floor-on
    $50 hole-floor tri-floor-on
    $EF hole-floor tri-floor-on
    $DB hole-floor tri-floor-on
    $FA hole-floor tri-floor-on
    $F9 hole-floor tri-floor-on
    $55 hole-floor tri-floor-on
    $DA hole-floor tri-floor-on
    $F3 hole-floor tri-floor-on
    $F8 hole-floor tri-floor-on
;

: old-mansion-b1-1.char-enter ( -- )   \ 00415C90
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 2 player-camera
    then
    0 2 2 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        3 3 player-camera
    then
    1 3 3 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        4 4 player-camera
    then
    2 4 4 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        1 1 player-camera
    then
    3 1 1 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            world-held state-flag-set
            0.0 sound-volume-scale
            0 fiona-who 2 action
        else
            room-sounds
            $13 1.0 0 bgm
        then
        1 exit-taken? 2 exit-taken? or if
            1 map-page
        then
        0 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-b1-1.phase1 ( -- )   \ 00415DB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        2 exit-prepare
    then
    1 39.3 0.0 50.4 5 10 1 zone
    self-who 1 char-in-zone? if
        in-zone1 ebit-set
    else
        in-zone1 ebit-clear
    then
    still-var 0 var? not if
        call-frames-var var-inc
        call-frames-var 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            call-frames-var 0 var-set
        then
    then
    fiona-who 5 char-entered-area? if
        0 map-page
    then
    fiona-who 5 char-left-area? if
        1 map-page
    then
    fiona-who $A char-entered-area? if
        1 map-page
    then
    fiona-who $A char-left-area? if
        2 map-page
    then
    hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
        2 game-mode? not if
            0 hewie-side? fiona-who $200000 char-on-nav-flags? and if
                -6 40 -29 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                    fiona-who -6 -29 $3C char-faces-xz? if
                        35 fiona-started? if
                            hewie-may-break-off? if
                                0 hewie-who 3 action
                            then
                        then
                    then
                then
            then
        then
    then
    2 38.73 0.0 50.28 $1A 16 0 zone
    hewie-zone2-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    hewie-zone2-noticed ebit-set
                    $50 chance? if
                        hewie-zone-var 2 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    2 38.73 0.0 50.28 $1A 16 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 16.0 hewie-look-zone
            then
        then
    then
    omb1-1-album-medallion story-flag? not if
        3 -9.31 39.5 -32.27 $32 13 0 zone
        hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        hewie-zone3-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 3 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
        3 -9.31 39.5 -32.27 $32 13 0 zone
        hewie-who char-here? 1 game-mode? and if
            hewie-can-command? if
                hewie-who 3 9 char-zone-bits? if
                    hewie-zone-var 3 var-set
                    hewie-zone-var 0.0 hewie-look-zone
                then
            then
        then
    then
;

: old-mansion-b1-1.phase2 ( -- )   \ 00415FA0
    omb1-1-breakable-broken story-flag? not if
        0 60.5 -0.1 -33.4 5 8 1 zone
        self-who 0 char-in-zone? if
            omb1-1-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            60.5 -0.1 -33.4 0 -2144325584 0 0.0 shards-at
            $88 5 60.5 -0.1 -33.4 0 0 sound
            $40 $123 noise
            0 60.5 0.9 -33.4 flicker-sprite
        then
    else omb1-1-chicken-jerky story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            1 calm offer-action
        then
    then then
;

: old-mansion-b1-1.phase3 ( -- )   \ 00416030
    37.0 -50.0 50.0 82.5 -50.0 50.0 37.0 -90.0 50.0 82.5 -90.0 50.0 lights-doorway
    52.0 -32.0 45.0 68.0 -32.0 45.0 52.0 -90.0 45.0 68.0 -90.0 45.0 lights-doorway
    -29.0 -4.0 50.0 37.0 -48.0 50.0 -8.0 -80.0 50.0 37.0 -80.0 50.0 lights-doorway
    5.0 0.0 -40.0 -65.0 0.0 -40.0 5.0 -80.0 -40.0 -65.0 -80.0 -40.0 lights-doorway
    -50.0 18.5 -40.0 -50.0 18.5 5.5 -50.0 -50.0 -40.0 -50.0 -50.0 5.5 lights-doorway
;

: old-mansion-b1-1.phase5 ( -- )   \ 00416130
    $1C action-end
    $1C char-done
    stalkers-stay state-flag? if
        hewie-who action-end
        hewie-who $76 -40.52 -28.0 -90 char-to-xz
        stalkers-stay state-flag-clear
    then
;

: old-mansion-b1-1.act00 ( -- )   \ 00416150
    0 exit-taken? 3 exit-taken? or if
        3 $1C char-loaded? not if
            $1C 3 $FF char-load
            3 char-start
            $1C char-activate
            $1C 39.3 0.0 50.4 -90 char-to-xyz
            $1C $900A 1 0 char-anim-hold
            $1C 1 char-visible
        then
        begin
            in-zone1 ebit? if
                still-var 0 var-set
                hops-var var-inc
                hops-var 3 var? if
                    $21 chance? if
                        hops-var 5 var-set
                    then
                then
                hops-var 4 var? if
                    $32 chance? if
                        hops-var 5 var-set
                    then
                then
                hops-var 5 var? if
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
                fiona-who shove-hit-sound 5 char-sound
                $1C $900B 0 3 char-anim-hold
                $1C wait-char-anim
                $1C $900A 0 3 char-anim-hold
                still-var 0 var-set
                hops-var 5 var? if
                    $1C wait-char-anim
                    hops-var 0 var-set
                then
            then
            $1C char-at-motion-event? if
                $1C $900A 0 0 char-anim-hold
                still-var 0 var-set
            else
                still-var var-inc
            then
            1 wait-frames
        again
    else
        3 $1C char-loaded? not if
            $1C 3 $FF char-load
            3 char-start
            $1C char-activate
            $1C 39.3 0.0 50.4 -90 char-to-xyz
            $1C $900A 1 0 char-anim-hold
            $1C 0 char-visible
        then
        begin
            in-zone1 ebit? if
                still-var 0 var-set
                hops-var var-inc
                hops-var 3 var? if
                    $21 chance? if
                        hops-var 5 var-set
                    then
                then
                hops-var 4 var? if
                    $32 chance? if
                        hops-var 5 var-set
                    then
                then
                hops-var 5 var? if
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
                fiona-who shove-hit-sound 5 char-sound
                $1C $900B 0 3 char-anim-hold
                $1C wait-char-anim
                $1C $900A 0 3 char-anim-hold
                still-var 0 var-set
                hops-var 5 var? if
                    $1C wait-char-anim
                    hops-var 0 var-set
                then
            then
            $1C char-at-motion-event? if
                $1C $900A 0 0 char-anim-hold
                still-var 0 var-set
            else
                still-var var-inc
            then
            1 wait-frames
        again
    then
    self-idle-or-end
;

: old-mansion-b1-1.act01 ( -- )   \ 004162E0
    self-wait-done
    60.5 -33.4 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-1-chicken-jerky story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-1.act02 ( -- )   \ 00416340
    1 self-scripted
    self-wait-done
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    effects-arena-flip
    0 char-in
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    fiona-who 1 char-to-exit
    hewie-who char-activate
    $44 1 161 hewie-to-room
    hewie-who 1 char-to-exit
    $FE $47 -1 2 stalker-to-room
    summoner-own-cooldown
    stalker-who 0 stalker-command
    3 3 player-camera
    yield
    camera-restart
    room-sounds
    $13 1.0 0 bgm
    $F $51 fade
    wait-fade
    $23F item-give
    scene-locked state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-1.act03 ( -- )   \ 004163F0
    1 self-scripted
    stalkers-stay state-flag-set
    self-wait-done
    hewie-bark
    self-wait-done
    $C -59.57 -30.97 90 $FFFF $A self-move-to
    self-wait-done
    1 solid-floor floor-off
    1 keeps-fiona-off keeps-pursuers-off or floor-on
    $129 -11.17 -29.06 0.3 50 hewie-go-to
    self-wait-done
    sit-a-while-anim $A blend-out
    omb1-1-album-medallion story-flag? not if
        fiona-half-hidden state-flag? fiona-who char-busy? not or if
            $71 $63 item-count? not if
                10 hewie-trust
            then
            $71 message-param-room
            $71 $63 item-count? if
                $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
            else
                omb1-1-album-medallion story-flag-set
                $71 1 item-give-count
                0 $71 item-tab
                item-fanfare
                $8011 say   \ "You've obtained ."
            then
            omb1-1-album-medallion story-flag? if
                1 effect-remove
            then
        then
    then
    $129 -11.46 -30.1 -90 $FFFF 5 self-move-to
    self-wait-done
    $C -59.57 -30.97 2.0 50 hewie-go-to
    self-wait-done
    1 solid-floor floor-on
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
old-mansion-b1-1 register-scripts   \ (its phases and actions, by their names)
