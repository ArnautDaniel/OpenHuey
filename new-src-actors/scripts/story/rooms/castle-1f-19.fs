\ story/rooms/castle-1f-19.fs - the event scripts of room castle-1f-19 ($1E; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-19
USING: room-names story.words story.shared flag-names ;

: castle-1f-19.enter ( -- )   \ 003FD9C0
    room-sounds
    $1A 1.0 0 bgm
    1 char-here? 119 hewie-action? and if
        0 0 8 nav-group
        1 0 $B0 nav-group
        1 $12C char-on-tri? not if
            1 $12C -12.0 -106.0 0 char-to-xz
            1800 2 hewie-anim
        then
    else
        0 0 $30 nav-group
        1 0 $88 nav-group
        1 0 char-in-nav-group? if
            1 $47 char-to-tri
        then
    then
    $10 41.5 40.7 -137.7 $E $80 $80 $80 $40 specks
    $10 5.9 34.6 -158.4 $E $80 $80 $80 $40 specks
;

: castle-1f-19.char-enter ( -- )   \ 003FDA40
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
;

: castle-1f-19.phase1 ( -- )   \ 003FDB00
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    0 6 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 4 char-entered-area? 0 7 char-entered-area? or if
        2 exit-prepare
    then
    1 char-here? if
        118 hewie-action? not if
            4 -11.187 5.0 -117.743 $1E 20 0 zone
            5 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 4 9 char-zone-bits? if
                            5 ebit-set
                            $1E chance? if
                                $1F 4 var-set
                                0 1 $88 action
                            then
                        then
                    then
                then
            then
            35 fiona-started? 1 2 char-C4? not and if
                0 8 char-in-area? $FE char-here? not and 0 -13 -110 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 0 action
                    then
                else $236 story-flag? not if
                    2 game-mode? not if
                        68 0 -274 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                            0 68 -274 $32 char-faces-xz? 1 char-busy? not and if
                                hewie-stays? if
                                    0 0 7 action
                                then
                            then
                        then
                    then
                then then
            then
        else
            44 fiona-started? if
                0 1 1 action
            then
            $FF panic-stage? not if
                0 char-busy? not fiona-half-hidden state-flag? or if
                    2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
                        $FE 2 char-C4? not $FE 8 char-in-area? and if
                            0 counter-set
                            0 1 3 action
                            0 $FE 4 action
                            $FF panic-stage? if
                                3 panic-stage
                            then
                            0 0 char-action? if
                                0 0 2 action-force
                            else
                                1 0 2 action-force
                            then
                        then
                    then
                then
            then
        then
    then
    6 46.51 0.0 -267.37 $1C 6 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 ebit? not if
        0 46.0 0.0 -280.0 $10 4 0 zone
        0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
            0 5 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
            1 $A 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
            $FE 5 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
    then
    2 ebit? not if
        1 52.0 0.0 -240.0 $10 4 0 zone
        0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
            0 5 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
            1 $A 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
            $FE 5 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
    then
    3 ebit? not if
        2 50.0 0.0 -300.0 $10 4 0 zone
        0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
            0 5 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
        1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
            1 $A 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
        $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
            $FE 5 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
    then
    4 ebit? not if
        3 70.0 0.0 -260.0 $10 4 0 zone
        0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
            0 5 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
        1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
            1 $A 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
        $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
            $FE 5 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
    then
    6 ebit? not if
        5 73.0 0.0 -282.0 $10 4 0 zone
        0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
            0 5 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
        1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
            1 $A 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
        $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
            $FE 5 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
    then
;

: castle-1f-19.phase2 ( -- )   \ 003FDEC8
    0 9 char-in-area? 0 0 $32 char-heading? and if
        5 5 0 scene-change
    then
;

: castle-1f-19.phase5 ( -- )   \ 003FDEE0
    1 char-busy? if
        1 action-end
        1 $118 39.008 -266.892 -80 char-to-xz
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
    -1 self-move-16
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-action
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
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    $FF 1 char-visible
    force-calm state-flag-set
    yield
    force-calm state-flag-clear
    0 $F9 9 action
    $10 $28 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    $FE action-end
    3 summon-take
    0 self-move-16
    $FF 0 char-visible
    1 0 0 char-tint
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
    0 0 hewie-action
    1 $2F -90 char-to-tri-facing
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
    1 char-here? 118 hewie-action? and 1 0 char-in-nav-group? and if
        2 message
        wait-message
    else 0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
        0 ebit-clear
    then then
    self-frames-reset
    4 self-wait-frames
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    self-idle-or-end
;

: castle-1f-19.act06 ( -- )   \ 0047AA98
    self-idle-or-end
;

: castle-1f-19.act07 ( -- )   \ 003FE0E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 8 action-force
    else
        stalkers-stay state-flag-clear
    then
    $C04 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
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
    1 0 6 char-sound
    $13F 68.224 -274.752 100 $204 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    fiona-half-hidden state-flag? 0 char-busy? not or if
        $73 $63 item-count? not if
            10 hewie-trust
        then
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $236 story-flag-set
            $73 1 item-give-count
            0 $73 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    then
    1 0 6 char-sound
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
        1 $C8000000 1 char-tint
        yield
    repeat
    1 0 0 char-tint
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
    3 char-unload
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $FF 1 char-visible
    0 $F9 9 action
    $10 $28 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $FF 0 char-visible
    1 0 0 char-tint
    0 3 0.0 light
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-19.enter castle-1f-19 0 room-script!
' castle-1f-19.char-enter castle-1f-19 6 room-script!
' castle-1f-19.phase1 castle-1f-19 1 room-script!
' castle-1f-19.phase2 castle-1f-19 2 room-script!
' castle-1f-19.phase5 castle-1f-19 5 room-script!
' castle-1f-19.act00 castle-1f-19 $00 action-script!
' castle-1f-19.act01 castle-1f-19 $01 action-script!
' castle-1f-19.act02 castle-1f-19 $02 action-script!
' castle-1f-19.act03 castle-1f-19 $03 action-script!
' castle-1f-19.act04 castle-1f-19 $04 action-script!
' castle-1f-19.act05 castle-1f-19 $05 action-script!
' castle-1f-19.act06 castle-1f-19 $06 action-script!
' castle-1f-19.act07 castle-1f-19 $07 action-script!
' castle-1f-19.act08 castle-1f-19 $08 action-script!
' castle-1f-19.act09 castle-1f-19 $09 action-script!
' castle-1f-19.act0A castle-1f-19 $0A action-script!
