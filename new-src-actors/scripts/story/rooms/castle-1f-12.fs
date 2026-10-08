\ story/rooms/castle-1f-12.fs - the event scripts of room castle-1f-12 ($4; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-12
USING: room-names story.words story.shared flag-names ;

\ two dials (byte 3: pstr_syuukouki2 on var 0, pstr_syuukouki1 on var 1, from -90 degrees), byte
\ 4 the step
: castle-1f-12.cmd00 ( b0 b1 -- )  drop drop s" castle-1f-12.cmd00" stub-step ;
\ room 0x04 (Room04_Cmd01_ptmf): five objects turned -75 / 75 degrees in turn
: castle-1f-12.cmd01 ( -- )  s" castle-1f-12.cmd01" stub-step ;
\ room 0x04 (Room04_Cmd02_ptmf): byte 3 0: room effect 0x1B (MirrorFragment_vtable) on its
\ object, a box (640, -560, 1000, 0, 0x60); else the effect gone
: castle-1f-12.cmd02 ( b0 -- )  drop s" castle-1f-12.cmd02" stub-step ;
\ room 0x04 (Room04_Cmd03_ptmf): an effect on one object (byte 3 0: at 90 degrees) or the other
\ (0)
: castle-1f-12.cmd03 ( b0 -- )  drop s" castle-1f-12.cmd03" stub-step ;

: castle-1f-12.enter ( -- )   \ 003F0DF0
    $26 story-flag? not if
        room-sounds
        1 1 $20000 nav-group
    else
        castle-1f-12.cmd01
        0 1 $20000 nav-group
    then
    $30 story-flag? not if
        2 1 $14 door-bits
        2 1 object-show
    else
        2 0 $14 door-bits
        0 0 castle-1f-12.cmd00
    then
    1 0 castle-1f-12.cmd00
    $207 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $289 story-flag? not if
            0 -73.5 -102.0 55.5 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    1 $2300 sound-volume
;

: castle-1f-12.char-enter ( -- )   \ 003F0E80
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
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    1 2 -1 area-camera
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
        $80 exit-taken? $81 exit-taken? or if
            0 $56 -68.279 -15.589 -90 char-to-xz
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
            0 0 5 action
        then
    then
;

: castle-1f-12.phase1 ( -- )   \ 003F0F70
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    1 52.29 -103.0 49.3 $1E 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 30.64 -103.0 -51.68 $1E 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    0 3 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        1 exit-prepare
    then
    0 4 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        0 exit-prepare
    then
    0 5 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
            $FE $C char-file-load
        then
        2 exit-prepare
    then
;

: castle-1f-12.phase2 ( -- )   \ 003F1060
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    $26 story-flag? not if
        0 $E char-in-area? 0 -45 $32 char-heading? and if
            5 2 0 scene-change
        then
    then
    0 $C char-in-area? 0 54 49 $32 char-faces-xz? and if
        5 3 0 scene-change
    then
    0 $D char-in-area? 0 30 -51 $32 char-faces-xz? and if
        5 6 0 scene-change
    then
    $207 story-flag? not if
        0 -73.5 -103.0 55.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $207 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -73.5 -103.0 55.5 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -73.5 -103.0 55.5 0 0 sound
            $40 $1E3 noise
            0 -73.5 -102.0 55.5 flicker-sprite
        then
    else $289 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then then
;

: castle-1f-12.act00 ( -- )   \ 003F1130
    self-wait-done
    $FE self-touching? not if
        $1E self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $1E char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $1E door-unlock
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

: castle-1f-12.act01 ( -- )   \ 003F1170
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    $A room-preload
    self-wait-done
    wait-fade
    force-calm state-flag-set
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-1f-12.act02 ( -- )   \ 003F1190
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $A0 -76.0 -15.0 -90 char-to-xz
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 -10.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    3 ebit? not if
        3 message
        wait-message
        3 ebit-set
    else
        4 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-12.act03 ( -- )   \ 003F11F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $26 story-flag? if
        1 message
        wait-message
    else $30 story-flag? not if
        self-frames-reset
        8 self-wait-frames
        world-frozen state-flag-set
        0 $73 61.0 50.0 -90 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        self-frames-reset
        8 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
    else
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        world-frozen state-flag-set
        0 $73 61.0 50.0 -90 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        5 message
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                0 1 castle-1f-12.cmd00
                1 ebit? not if
                    0 6 54.0 -90.0 49.0 0 0 sound
                then
            else
                0 2 castle-1f-12.cmd00
                1 ebit? 0 5 pvar? and if
                    1 6 54.0 -90.0 49.0 0 0 sound
                    0 castle-1f-12.cmd03
                then
            then
            yield
        repeat
        5 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            ['] castle-1f-12.act01 goto
        then
    then then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act04 ( -- )   \ 003F1310
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    3 1 movie-play
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
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 $F9 9 action
    $1E $C8 movie-param
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
    1 castle-1f-12.cmd02
    2 0 $14 door-bits
    1 1 object-show
    2 0 object-show
    0 self-move-16
    0 $73 63.3 50.28 -68 char-to-xz
    1 0 char-visible
    1 action-end
    camera-restart
    4 item-use
    $30 story-flag-set
    $F $41 fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act05 ( -- )   \ 003F13E0
    1 self-scripted
    self-wait-done
    $FE char-here? if
        0 $FE 7 action
        yield
    then
    world-frozen state-flag-set
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $80 exit-taken? if
        $A room-preload
    else
        7 room-preload
    then
    wait-fade
    $10 $FF movie-param
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
    $80 exit-taken? if
        world-held state-flag-set
        world-frozen state-flag-clear
        $81 exit-check
    else
        $22 door-unlock
        $26 story-flag-set
        0 0 castle-1f-12.cmd00
        1 0 castle-1f-12.cmd00
        castle-1f-12.cmd01
        0 1 $20000 nav-group
        5 0 creature-count
        camera-restart
        force-calm state-flag-clear
        stalkers-stay state-flag-clear
        world-frozen state-flag-clear
        0 self-scripted
        $F $41 fade
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act06 ( -- )   \ 003F14C0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $26 story-flag? if
        1 message
        wait-message
    else
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        world-frozen state-flag-set
        0 $BE 30.0 -44.0 180 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        5 message
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                1 1 castle-1f-12.cmd00
                1 ebit? not if
                    0 6 30.0 -90.0 -51.0 0 0 sound
                then
            else
                1 2 castle-1f-12.cmd00
                1 ebit? 1 2 pvar? and if
                    1 6 30.0 -90.0 -51.0 0 0 sound
                    1 castle-1f-12.cmd03
                then
            then
            yield
        repeat
        5 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            ['] castle-1f-12.act01 goto
        then
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act07 ( -- )   \ 0047A990
    self-wait-done
    begin
        yield
    again
;

: castle-1f-12.act08 ( -- )   \ 003F1590
    self-wait-done
    -73.5 55.5 self-turn-to-xz
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
            $289 story-flag-set
            0 effect-remove
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

: castle-1f-12.act09 ( -- )   \ 003F15F0
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    begin
        cutscene-near-end? not while
        0 castle-1f-12.cmd02
        yield
    repeat
    self-idle-or-end
;

: castle-1f-12.act0A ( -- )   \ 003F1610
    self-wait-done
    2 1 $14 door-bits
    2 1 object-show
    3 1 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 9 action
    $1E $C8 movie-param
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
    1 castle-1f-12.cmd02
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-12.act0B ( -- )   \ 003F16B0
    self-wait-done
    $FF 1 char-visible
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $37 room-preload
    $10 $FF movie-param
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
' castle-1f-12.enter castle-1f-12 0 room-script!
' castle-1f-12.char-enter castle-1f-12 6 room-script!
' castle-1f-12.phase1 castle-1f-12 1 room-script!
' castle-1f-12.phase2 castle-1f-12 2 room-script!
' castle-1f-12.act00 castle-1f-12 $00 action-script!
' castle-1f-12.act01 castle-1f-12 $01 action-script!
' castle-1f-12.act02 castle-1f-12 $02 action-script!
' castle-1f-12.act03 castle-1f-12 $03 action-script!
' castle-1f-12.act04 castle-1f-12 $04 action-script!
' castle-1f-12.act05 castle-1f-12 $05 action-script!
' castle-1f-12.act06 castle-1f-12 $06 action-script!
' castle-1f-12.act07 castle-1f-12 $07 action-script!
' castle-1f-12.act08 castle-1f-12 $08 action-script!
' castle-1f-12.act09 castle-1f-12 $09 action-script!
' castle-1f-12.act0A castle-1f-12 $0A action-script!
' castle-1f-12.act0B castle-1f-12 $0B action-script!
