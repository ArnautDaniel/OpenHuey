\ story/rooms/old-mansion-1f-17.fs - the event scripts of room old-mansion-1f-17 ($5D; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-17
USING: room-names story.words story.shared flag-names ;

\ room 0x5D (Room5D_Cmd00_ptmf): four objects 60 to the left
: old-mansion-1f-17.cmd00 ( -- )  s" old-mansion-1f-17.cmd00" stub-step ;
\ room 0x5D (Room5D_Cmd01_ptmf): the lever at -60 / 0 / 60 degrees by byte 3
: old-mansion-1f-17.cmd01 ( b0 -- )  drop s" old-mansion-1f-17.cmd01" stub-step ;

: old-mansion-1f-17.enter ( -- )   \ 00411B80
    room-sounds
    $5A door-not-closed-off? not if
        4 1 $14 door-bits
        1 3 8 nav-group
    then
    $4B story-flag? not if
        1 0 $20000 nav-group
    else
        old-mansion-1f-17.cmd00
    then
    $22E story-flag? not if
        1 1 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $293 story-flag? not if
            1 43.0 1.0 -40.0 flicker-sprite
        then
    then
    $22F story-flag? not if
        1 2 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $267 story-flag? not if
        0 -137.37 -41.0 -14.57 flicker-sprite
    then
    $29B story-flag? $29C story-flag? not and if
        2 68.99 1.0 19.99 flicker-sprite
    then
    0 8 0.562 0.5 0.25 0.5 zone-rect
    1 8 0.812 0.687 0.187 0.312 zone-rect
    3 -84.0 -19.0 13.7 1 effect-86
    1 $2300 sound-volume
;

: old-mansion-1f-17.char-enter ( -- )   \ 00411C60
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
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    3 1 1 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $12C char-to-tri
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
            0 0 0 action
        then
        0 exit-taken? if
            0 old-mansion-1f-17.cmd01
        else 3 exit-taken? if
            1 old-mansion-1f-17.cmd01
        else 1 exit-taken? if
            2 old-mansion-1f-17.cmd01
        else 2 exit-taken? if
            2 old-mansion-1f-17.cmd01
        then then then then
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-17.phase1 ( -- )   \ 00411D60
    $5A door-not-closed-off? if
        0 exit-usable? if
            0 exit-check
        then
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    0 4 char-entered-area? 0 6 char-entered-area? or if
        0 exit-prepare
    then
    0 7 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        3 exit-prepare
    then
    0 $11 char-entered-area? if
        2 map-page
    then
    0 $11 char-left-area? if
        1 map-page
    then
    $22F story-flag? not if
        1 -138.5 -50.0 -7.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $22F story-flag-set
            0 2 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -138.5 -50.0 -7.5 1 -2144851928 0 0.0 scene-effect-8C
            $88 5 -138.5 -50.0 -7.5 0 0 sound
            $40 5 noise
        then
    then
    $29B story-flag? not if
        7 68.99 0.0 19.99 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 667 var-set
                $1A 668 var-set
                $1B 2 var-set
                $1C 68990 var-set
                $1D 1000 var-set
                $1E 19990 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
    $4B story-flag? not if
        2 -15.99 0.0 0.03 $19 22 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    $1F 2 var-set
                    $1F 22.0 hewie-look-zone
                then
            then
        then
    then
    3 54.73 0.0 -58.43 $13 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 68.2 0.0 31.14 $14 12 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    1 ebit-set
                    $32 chance? if
                        $1F 4 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    4 68.2 0.0 31.14 $14 12 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    5 -125.41 -50.0 1.03 $14 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 3 8 -4 0 zone-at-effect
    0 8 3 char-zone-bits? 0 8 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 8 3 char-zone-bits? 1 8 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 8 3 char-zone-bits? $FE 8 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
;

: old-mansion-1f-17.phase2 ( -- )   \ 00411FB0
    $4B story-flag? not if
        0 $E char-in-area? 0 -45 $32 char-heading? and if
            5 1 0 scene-change
        then
    then
    0 $10 char-in-area? 0 45 $3C char-heading? and if
        5 5 0 scene-change
    then
    $267 story-flag? not if
        6 0 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
    $29B story-flag? $29C story-flag? not and if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $22E story-flag? not if
        0 43.0 0.0 -40.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $22E story-flag-set
            0 1 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            43.0 0.0 -40.0 0 -2145904616 0 0.0 scene-effect-8C
            3 6 43.0 0.0 -40.0 0 0 sound
            $40 $126 noise
            1 43.0 1.0 -40.0 flicker-sprite
        then
    else $293 story-flag? not if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: old-mansion-1f-17.act00 ( -- )   \ 00412090
    1 self-scripted
    self-wait-done
    world-frozen state-flag-set
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
    $14 $C8 movie-param
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
    wait-fade
    world-held state-flag-set
    world-frozen state-flag-clear
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-17.act01 ( -- )   \ 00412120
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-17.act02 ( -- )   \ 00412140
    self-wait-done
    -137.37 -14.57 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $267 story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-17.act03 ( -- )   \ 004121A0
    self-wait-done
    43.0 -40.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $293 story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: old-mansion-1f-17.act04 ( -- )   \ 00412200
    self-wait-done
    68.99 19.99 self-turn-to-xz
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
            $29C story-flag-set
            2 effect-remove
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

: old-mansion-1f-17.act05 ( -- )   \ 00412260
    self-wait-done
    $137 57.06 30.88 90 $FFFF 5 self-move-to
    self-wait-done
    1 self-scripted
    $339 story-flag? not if
        0 $43 5 char-sound
        $F02 $A self-anim-blend
        $5A threat-add
        1 $FF 8 rumble
        self-wait-anim
        world-frozen state-flag-set
        1 5.0 10.0 0.0 0.0 event-camera
        $339 story-flag-set
        2 message
        wait-message
    else
        world-frozen state-flag-set
        1 5.0 10.0 0.0 0.0 event-camera
        3 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    0 self-scripted
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: old-mansion-1f-17.act06 ( -- )   \ 004122F0
    self-wait-done
    3 -84.0 -19.0 13.7 1 effect-86
    $FF 1 char-visible
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
    $14 $C8 movie-param
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
    $FF 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-17.enter old-mansion-1f-17 0 room-script!
' old-mansion-1f-17.char-enter old-mansion-1f-17 6 room-script!
' old-mansion-1f-17.phase1 old-mansion-1f-17 1 room-script!
' old-mansion-1f-17.phase2 old-mansion-1f-17 2 room-script!
' old-mansion-1f-17.act00 old-mansion-1f-17 $00 action-script!
' old-mansion-1f-17.act01 old-mansion-1f-17 $01 action-script!
' old-mansion-1f-17.act02 old-mansion-1f-17 $02 action-script!
' old-mansion-1f-17.act03 old-mansion-1f-17 $03 action-script!
' old-mansion-1f-17.act04 old-mansion-1f-17 $04 action-script!
' old-mansion-1f-17.act05 old-mansion-1f-17 $05 action-script!
' old-mansion-1f-17.act06 old-mansion-1f-17 $06 action-script!
