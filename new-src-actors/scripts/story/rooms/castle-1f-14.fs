\ story/rooms/castle-1f-14.fs - the event scripts of room castle-1f-14 ($E; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-14
USING: room-names story.words story.shared flag-names ;

: castle-1f-14.enter ( -- )   \ 003F5540
    $25A story-flag? $25B story-flag? not and if
        1 -70.0 61.0 54.1 flicker-sprite
    then
    $21F story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $220 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $22B story-flag? not if
            0 -68.0 1.0 -58.0 flicker-sprite
        then
    then
    $221 story-flag? not if
        1 2 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
        $28A story-flag? not if
            2 -66.0 1.0 55.0 flicker-sprite
        then
    then
    1 $2300 sound-volume
    $AF story-flag? if
        $3F story-flag? not if
            $3F story-flag-set
            $E 0 829 $80 10 -1 $FF80 0.0 creature-place
        then
    then
;

: castle-1f-14.char-enter ( -- )   \ 003F5610
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
    0 self-is? if
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
        0 exit-taken? if
            3 map-page
        then
        $80 exit-taken? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        then
    then
    0 4 0.0 0.687 0.187 0.312 zone-rect
;

: castle-1f-14.phase1 ( -- )   \ 003F5710
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 1 1 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 4 4 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        2 exit-prepare
    then
    0 $D char-entered-area? if
        3 map-page
    then
    0 $D char-left-area? if
        2 map-page
    then
    $25A story-flag? not if
        3 -70.0 60.0 54.1 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 602 var-set
                $1A 603 var-set
                $1B 1 var-set
                $1C -70000 var-set
                $1D 61000 var-set
                $1E 54100 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: castle-1f-14.phase2 ( -- )   \ 003F57E0
    $21F story-flag? not if
        0 -61.0 0.0 -52.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $21F story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -61.0 0.0 -52.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -61.0 0.0 -52.0 0 0 sound
            $40 $55 noise
        then
    then
    $220 story-flag? not if
        1 -68.0 0.0 -58.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $220 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -68.0 0.0 -58.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -68.0 0.0 -58.0 0 0 sound
            $40 $2BF noise
            0 -68.0 1.0 -58.0 flicker-sprite
        then
    else $22B story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
    $221 story-flag? not if
        2 -66.0 0.0 55.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $221 story-flag-set
            0 2 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -66.0 0.0 55.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -66.0 0.0 55.0 0 0 sound
            $40 $295 noise
            2 -66.0 1.0 55.0 flicker-sprite
        then
    else $28A story-flag? not if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    $25A story-flag? $25B story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: castle-1f-14.phase3 ( -- )   \ 003F5980
    30.0 54.1 -51.5 -45.0 54.1 -51.5 30.0 54.1 -90.0 -45.0 54.1 -90.0 lights-doorway
    70.0 54.1 -23.0 57.0 54.1 -56.0 100.0 54.1 -30.0 80.0 54.1 -65.0 lights-doorway
    77.0 54.1 18.0 70.0 54.1 -23.0 100.0 54.1 30.0 100.0 54.1 -30.0 lights-doorway
    52.0 54.1 48.0 77.0 54.1 18.0 82.0 54.1 65.0 100.0 54.1 30.0 lights-doorway
    65.0 20.0 18.0 75.0 20.0 13.0 65.0 0.0 18.0 75.0 0.0 13.0 lights-doorway
    74.5 20.0 -19.5 68.5 20.0 -24.5 74.5 0.0 -19.5 68.5 0.0 -24.5 lights-doorway
    53.0 20.0 -48.0 53.0 20.0 -60.0 53.0 0.0 -48.0 53.0 0.0 -60.0 lights-doorway
    -55.0 20.0 -62.0 -55.0 20.0 -47.0 -55.0 0.0 -62.0 -55.0 0.0 -47.0 lights-doorway
    -70.0 20.0 -26.0 -78.0 20.0 -22.0 -70.0 0.0 -26.0 -78.0 0.0 -22.0 lights-doorway
    -78.0 20.0 20.0 -69.0 20.0 25.0 -78.0 0.0 20.0 -69.0 0.0 25.0 lights-doorway
    -55.0 20.0 47.0 -55.0 20.0 54.0 -55.0 0.0 47.0 -55.0 0.0 54.0 lights-doorway
    52.0 20.0 57.0 52.0 20.0 47.0 52.0 0.0 57.0 52.0 0.0 47.0 lights-doorway
;

: castle-1f-14.act00 ( -- )   \ 003F5BD0
    self-wait-done
    $FE self-touching? not if
        0 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 0 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    0 door-unlock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: castle-1f-14.act01 ( -- )   \ 003F5C10
    self-wait-done
    -68.0 -58.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $22B story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: castle-1f-14.act02 ( -- )   \ 003F5C70
    self-wait-done
    -70.0 54.1 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $25B story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: castle-1f-14.act03 ( -- )   \ 003F5CD0
    self-wait-done
    -66.0 55.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message
            wait-message
        else
            $28A story-flag-set
            2 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
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

: castle-1f-14.act04 ( -- )   \ 003F5D30
    3 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    self-wait-done
    0 1 char-visible
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    $B 3 $FF char-load
    $10 4 $FF char-load
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
    3 char-unload
    4 char-unload
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    4 0 char-remove
    $2D 2 pvar-set
    2 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: castle-1f-14.act05 ( -- )   \ 003F5DE0
    self-wait-done
    0 1 char-visible
    $B 3 $FF char-load
    $10 4 $FF char-load
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
    $37 room-preload
    3 char-unload
    4 char-unload
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-14.enter castle-1f-14 0 room-script!
' castle-1f-14.char-enter castle-1f-14 6 room-script!
' castle-1f-14.phase1 castle-1f-14 1 room-script!
' castle-1f-14.phase2 castle-1f-14 2 room-script!
' castle-1f-14.phase3 castle-1f-14 3 room-script!
' castle-1f-14.act00 castle-1f-14 $00 action-script!
' castle-1f-14.act01 castle-1f-14 $01 action-script!
' castle-1f-14.act02 castle-1f-14 $02 action-script!
' castle-1f-14.act03 castle-1f-14 $03 action-script!
' castle-1f-14.act04 castle-1f-14 $04 action-script!
' castle-1f-14.act05 castle-1f-14 $05 action-script!
