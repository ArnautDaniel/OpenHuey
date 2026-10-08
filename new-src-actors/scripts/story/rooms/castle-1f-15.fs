\ story/rooms/castle-1f-15.fs - the event scripts of room castle-1f-15 ($19; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-15
USING: room-names story.words story.shared ;

\ room 0x19 (Room19_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) while
\ Hewie is controlled: in another room, or 30 or more from the player
: castle-1f-15.cond00? ( -- flag )  s" castle-1f-15.cond00?" stub-flag ;

: castle-1f-15.enter ( -- )   \ 003FB3B0
    room-sounds
    $FE exit-taken? if
        0 $23F 90 char-to-tri-facing
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
        0 0 9 action
    then
    $254 story-flag? $255 story-flag? not and if
        0 34.3 1.0 -13.7 flicker-sprite
    then
    $256 story-flag? $257 story-flag? not and if
        1 56.3 51.0 -71.5 flicker-sprite
    then
    1 $2300 sound-volume
    $AF story-flag? if
        $40 story-flag? not if
            $40 story-flag-set
            $19 0 389 $80 10 -1 $FFDC 0.0 creature-place
        then
    then
;

: castle-1f-15.char-enter ( -- )   \ 003FB420
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
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
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    4 1 1 area-camera
    0 self-is? if
        4 exit-taken? 3 exit-taken? or if
            3 map-page
        then
    then
;

: castle-1f-15.phase1 ( -- )   \ 003FB520
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 -88.0 65.0 -95.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000008 6 -88.0 65.0 -95.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $10 1 1 1 chars-area-camera
    $11 2 -1 1 chars-area-camera
    $12 1 1 1 chars-area-camera
    $13 2 -1 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? 0 7 char-entered-area? or if
        2 exit-prepare
    then
    0 6 char-entered-area? 0 8 char-entered-area? or if
        3 exit-prepare
    then
    0 9 char-entered-area? if
        4 exit-prepare
    then
    0 $15 char-entered-area? 0 $16 char-entered-area? or if
        $27 room-preload
    then
    0 6 char-entered-area? if
        3 map-page
    then
    0 6 char-left-area? if
        2 map-page
    then
    0 -88.0 50.0 -94.617 $1E 20 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    0 -88.0 50.0 -94.617 $1E 20 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 -9.96 0.0 43.07 $53 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $254 story-flag? not if
        5 34.3 0.0 -13.7 $A 5 0 zone
        1 5 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 596 var-set
                $1A 597 var-set
                $1B 0 var-set
                $1C 34300 var-set
                $1D 1000 var-set
                $1E -13700 var-set
                $1F 5 var-set
                0 1 $8B action
            then
        then
    then
    $256 story-flag? not if
        6 56.3 50.0 -71.5 $A 5 0 zone
        1 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 598 var-set
                $1A 599 var-set
                $1B 1 var-set
                $1C 56300 var-set
                $1D 51000 var-set
                $1E -71500 var-set
                $1F 6 var-set
                0 1 $8B action
            then
        then
    then
;

: castle-1f-15.phase2 ( -- )   \ 003FB720
    0 $C char-in-area? 0 90 $2D char-heading? and if
        5 1 0 scene-change
    then
    0 $D $3C char-faces-area? if
        5 2 0 scene-change
    then
    0 $E char-in-area? 0 $F char-in-area? or 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    0 $14 char-in-area? 0 -45 $5A char-heading? and if
        5 4 0 scene-change
    then
    7 ebit? not 9 ebit? not and if
        0 $18 char-in-area? if
            2 game-mode? $FF panic-stage? not and if
                castle-1f-15.cond00? if
                    1 char-here? not 1 2 char-C4? or if
                        9 ebit-set
                        $FF panic-stage? if
                            3 panic-stage
                        then
                        0 0 char-action? if
                            0 0 5 action-force
                        else
                            1 0 5 action-force
                        then
                    else
                        3 ebit-clear
                        7 0 pvar? if
                            0 chance? if
                                3 ebit-set
                            then
                        else 7 1 pvar? if
                            $32 chance? if
                                3 ebit-set
                            then
                        else 7 2 pvar? if
                            $5A chance? if
                                3 ebit-set
                            then
                        else 7 3 pvar? if
                            $5A chance? if
                                3 ebit-set
                            then
                        then then then then
                        3 ebit? not if
                            9 ebit-set
                            $FF panic-stage? if
                                3 panic-stage
                            then
                            0 0 char-action? if
                                0 0 6 action-force
                            else
                                1 0 6 action-force
                            then
                        else
                            9 ebit-set
                            $FF panic-stage? if
                                3 panic-stage
                            then
                            0 0 char-action? if
                                0 0 7 action-force
                            else
                                1 0 7 action-force
                            then
                        then
                    then
                then
            then
        then
    then
    0 $17 char-in-area? 0 -45 $32 char-heading? and if
        5 8 0 scene-change
    then
    $254 story-flag? $255 story-flag? not and if
        5 0 5 5 0 zone-at-effect
        0 5 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
    $256 story-flag? $257 story-flag? not and if
        6 1 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then
;

: castle-1f-15.phase3 ( -- )   \ 003FB840
    camera-setup-changed? if
        0 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
    28.7 47.4 22.4 28.7 47.4 -29.0 74.0 47.4 22.4 74.0 47.4 -29.0 lights-doorway
    28.7 47.4 -29.0 28.7 47.4 -80.4 74.0 47.4 -29.0 74.0 47.4 -80.4 lights-doorway
    26.3 48.0 30.0 26.3 48.0 -35.9 72.1 48.0 30.0 72.1 48.0 -35.9 lights-doorway
    -55.1 48.0 -14.9 -55.1 48.0 39.6 -100.9 48.0 -14.9 -100.9 48.0 39.6 lights-doorway
    -65.1 48.0 18.6 -65.1 48.0 72.7 -110.9 48.0 18.2 -110.9 48.0 72.7 lights-doorway
    -65.1 48.0 61.0 -65.1 48.0 91.0 -110.9 48.0 61.0 -110.9 48.0 91.0 lights-doorway
;

: castle-1f-15.act00 ( -- )   \ 0047AA60
    self-idle-or-end
;

: castle-1f-15.act01 ( -- )   \ 003FB990
    self-wait-done
    $112 -0.195 -27.107 175 $FFFF 5 self-move-to
    self-wait-done
    $A02 self-anim
    self-wait-anim
    0 message
    wait-message
    self-idle-or-end
;

: castle-1f-15.act02 ( -- )   \ 003FB9B0
    self-wait-done
    0 ebit? not if
        1 message
        wait-message
        0 ebit-set
    else
        2 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-15.act03 ( -- )   \ 003FB9D0
    self-wait-done
    1 ebit? not if
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-15.act04 ( -- )   \ 003FB9F0
    self-wait-done
    $173 33.471 79.86 -90 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 20.0 50.0 15.0 0.0 event-camera
    2 ebit? not if
        5 message
        wait-message
        2 ebit-set
    else
        6 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act05 ( -- )   \ 003FBA50
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
    $F $44 fade
    self-wait-done
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
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
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    8 state-flag-set
    0 $169 60.358 52.0 180 char-to-xz
    $B01 0 self-anim-blend
    camera-restart
    $FE $19 358 2 stalker-to-room
    $FE $166 55.085 70.506 180 char-to-xz
    $FE 1 1 char-camera
    1 0 char-visible
    1 action-end
    $18 state-flag-clear
    4 $1E self-anim-blend
    self-wait-anim
    $F $41 fade
    wait-fade
    $E state-flag-clear
    $64 threat-raise
    7 ebit-set
    $1F resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act06 ( -- )   \ 003FBB40
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
    $F $44 fade
    self-wait-done
    1 0 movie-play
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
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
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
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    8 state-flag-set
    0 $16C 63.654 67.0 -57 char-to-xz
    0 self-move-16
    camera-restart
    1 $166 56.0 62.0 -57 char-to-xz
    $FE $19 161 2 stalker-to-room
    $FE $A1 11.784 75.875 -90 char-to-xz
    $FE stalker-knock-down
    $FE 0 0 char-camera
    7 pvar-inc
    $18 state-flag-clear
    $F $41 fade
    wait-fade
    $E state-flag-clear
    10 hewie-trust
    $8278 item-give
    7 ebit-set
    $1D resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act07 ( -- )   \ 003FBC20
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
    $F $44 fade
    self-wait-done
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
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
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    8 state-flag-set
    0 $16C 63.054 67.67 -57 char-to-xz
    $B01 0 self-anim-blend
    camera-restart
    1 $166 62.709 59.408 -57 char-to-xz
    $FE $19 359 2 stalker-to-room
    $FE $167 35.69 79.463 90 char-to-xz
    $FE 1 1 char-camera
    $18 state-flag-clear
    $F $41 fade
    wait-fade
    $E state-flag-clear
    7 ebit-set
    $1E resident-flag-set
    $8279 item-give
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act08 ( -- )   \ 003FBD00
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    7 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        8 state-flag-set
        $FE exit-check
    else
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: castle-1f-15.act09 ( -- )   \ 003FBD30
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    3 map-page
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act0A ( -- )   \ 003FBD50
    self-wait-done
    34.3 -13.7 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $255 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-15.act0B ( -- )   \ 003FBDB0
    self-wait-done
    56.3 -71.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $257 story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-15.act0C ( -- )   \ 003FBE10
    self-wait-done
    2 3 $FF char-load
    3 char-unload
    1 0 movie-play
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
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
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
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-15.act0D ( -- )   \ 003FBEA0
    self-wait-done
    2 3 $FF char-load
    3 char-unload
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
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
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-15.act0E ( -- )   \ 003FBF30
    self-wait-done
    2 3 $FF char-load
    3 char-unload
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
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
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-15.enter castle-1f-15 0 room-script!
' castle-1f-15.char-enter castle-1f-15 6 room-script!
' castle-1f-15.phase1 castle-1f-15 1 room-script!
' castle-1f-15.phase2 castle-1f-15 2 room-script!
' castle-1f-15.phase3 castle-1f-15 3 room-script!
' castle-1f-15.act00 castle-1f-15 $00 action-script!
' castle-1f-15.act01 castle-1f-15 $01 action-script!
' castle-1f-15.act02 castle-1f-15 $02 action-script!
' castle-1f-15.act03 castle-1f-15 $03 action-script!
' castle-1f-15.act04 castle-1f-15 $04 action-script!
' castle-1f-15.act05 castle-1f-15 $05 action-script!
' castle-1f-15.act06 castle-1f-15 $06 action-script!
' castle-1f-15.act07 castle-1f-15 $07 action-script!
' castle-1f-15.act08 castle-1f-15 $08 action-script!
' castle-1f-15.act09 castle-1f-15 $09 action-script!
' castle-1f-15.act0A castle-1f-15 $0A action-script!
' castle-1f-15.act0B castle-1f-15 $0B action-script!
' castle-1f-15.act0C castle-1f-15 $0C action-script!
' castle-1f-15.act0D castle-1f-15 $0D action-script!
' castle-1f-15.act0E castle-1f-15 $0E action-script!
