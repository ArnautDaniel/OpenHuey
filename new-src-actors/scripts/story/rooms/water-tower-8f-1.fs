\ story/rooms/water-tower-8f-1.fs - the event scripts of room water-tower-8f-1 ($C7; Water Tower: 8F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-8f-1
USING: room-names story.words story.shared flag-names ;

\ the screen fade (renderer +0x70) by script variable 1 with Hewie's light 0xF: byte 3 0 clear
\ (0x808080), 2 full (0x80808080); 1 fades in by 0x10 a call and 3 back out, waiting (2),
\ Hewie's +0xE4 set once there
: water-tower-8f-1.cmd00 ( b0 -- )  drop s" water-tower-8f-1.cmd00" stub-step ;
\ (as Room0C_Cmd01, the player only)
: water-tower-8f-1.cmd01 ( b0 -- )  drop s" water-tower-8f-1.cmd01" stub-step ;
\ a cursor effect (RoomC7Cursor_vtable) at (x, y) kept in script variables 7 / 8, its slot in 6:
\ byte 3 0 puts it at (246, 242); 1 moves it 4 a frame by the stick or the d-pad (x 0..492, y
\ 0..420), waiting (2) until confirm (event 4 +0x5C) or cancel (+0x60); 2 ends it
: water-tower-8f-1.cmd02 ( b0 -- )  drop s" water-tower-8f-1.cmd02" stub-step ;
\ (as RoomC0_Cmd01) byte 3 2 up from frame 1268 (2.79 a frame, light 0x23) and 3 from frame 25
\ (4.27, light 0xF), held at 0x80; 1 the fade fully on with light 0xA, else off with light 0x11
\ (+0x64)
: water-tower-8f-1.cmd03 ( b0 -- )  drop s" water-tower-8f-1.cmd03" stub-step ;
\ (as Room2A_Cmd03) the 0xD40-byte effect Debris_vtable started with byte 3
: water-tower-8f-1.cmd04 ( b0 -- )  drop s" water-tower-8f-1.cmd04" stub-step ;
\ (as Room23_Cmd00) the kind-0xB character's model +0xCC8: 0 (byte 3 1) or -0.02
: water-tower-8f-1.cmd05 ( b0 -- )  drop s" water-tower-8f-1.cmd05" stub-step ;
\ (as Room48_Cmd02) byte 3 0 starts the effect BackdropModel_vtable (its slot in event variable
\ 9); else that one is ended (EffectMgr_Remove)
: water-tower-8f-1.cmd06 ( b0 -- )  drop s" water-tower-8f-1.cmd06" stub-step ;
\ script variables 7 / 8 (the player's spot) in 151..269 / 171..219
: water-tower-8f-1.cond00? ( -- flag )  s" water-tower-8f-1.cond00?" stub-flag ;
\ a room callback: the pursuer's Pursuer_GrabHewieBehind
: water-tower-8f-1.cond01? ( -- flag )  s" water-tower-8f-1.cond01?" stub-flag ;

: water-tower-8f-1.enter ( -- )   \ 0042F500
    room-sounds
    $1B 1.0 0 bgm
    $8E story-flag? not if
        0 0 $14 door-bits
        1 0 $14 door-bits
        8 1 object-show
        9 1 object-show
        $A 1 object-show
    else
        0 1 $14 door-bits
        1 1 $14 door-bits
    then
    $91 story-flag? not if
        2 3 var-set
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $33E story-flag? not if
        3 3 var-set
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    $33F story-flag? not if
        4 3 var-set
        6 1 $14 door-bits
        7 0 $14 door-bits
    else
        6 0 $14 door-bits
        7 1 $14 door-bits
    then
    8 1 $14 door-bits
    0 water-tower-8f-1.cmd06
;

: water-tower-8f-1.char-enter ( -- )   \ 0042F590
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
;

: water-tower-8f-1.phase1 ( -- )   \ 0042F5D0
    $AE story-flag? $8E story-flag? and $93 story-flag? not and if
        0 0 char-entered-area? if
            $93 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 $A action-force
            else
                1 0 $A action-force
            then
        then
    else 0 exit-usable? if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 0 0 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 1 -1 1 chars-area-camera
    0 20000.0 camera-value
    1 ebit? not 2 ebit? not and if
        0 ebit? not if
            0 control-action? if
                0 5 char-in-area? if
                    0 -6 -12 $3C char-faces-xz? if
                        hewie-stays? if
                            0 0 var-set
                            0 1 0 action
                        then
                    then
                else 0 6 char-in-area? if
                    0 -9 10 $3C char-faces-xz? if
                        hewie-stays? if
                            0 1 var-set
                            0 1 0 action
                        then
                    then
                else 0 7 char-in-area? if
                    0 12 7 $3C char-faces-xz? if
                        hewie-stays? if
                            0 2 var-set
                            0 1 0 action
                        then
                    then
                then then then
            then
        else 45 fiona-started? if
            $FE 2 char-C4? not if
                stalker-free? if
                    0 0 var? if
                        $FE 8 char-in-area? if
                            water-tower-8f-1.cond01? if
                                1 ebit-set
                                $91 story-flag? if
                                    $FF panic-stage? if
                                        3 panic-stage
                                    then
                                    0 0 8 action-force
                                then
                            then
                        then
                    else 0 1 var? if
                        $FE 9 char-in-area? if
                            water-tower-8f-1.cond01? if
                                1 ebit-set
                            then
                        then
                    else 0 2 var? if
                        $FE $A char-in-area? if
                            water-tower-8f-1.cond01? if
                                1 ebit-set
                            then
                        then
                    then then then
                then
            then
        else 44 fiona-started? if
            2 ebit-set
        then then then
    then
    $91 story-flag? not if
        0 3.0 620.0 79.0 $A 8 0 zone
        0 $C char-in-area? 0 0 char-in-zone? and if
            5 0 var-set
            0 $F1 6 action
        then
    then
    $33E story-flag? not if
        1 -58.0 620.0 54.0 $A 8 0 zone
        0 $D char-in-area? 0 1 char-in-zone? and if
            5 1 var-set
            0 $F2 6 action
        then
    then
    $33F story-flag? not if
        2 -5.0 620.0 -78.0 $A 8 0 zone
        0 $E char-in-area? 0 2 char-in-zone? and if
            5 2 var-set
            0 $F3 6 action
        then
    then
    $8E story-flag? not if
        3 0.0 620.0 41.0 $11 10 0 zone
        8 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: water-tower-8f-1.phase2 ( -- )   \ 0042F7B0
    scene-5-pending state-flag? if
        -2147483646 scene-request? if
            5 9 1 scene-change
        then
    then
    0 $B char-in-area? 0 90 $3C char-heading? and if
        5 2 0 scene-change
    then
    $33E story-flag? not if
        0 $D char-in-area? 0 -58 54 $32 char-faces-xz? and if
            5 $E 0 scene-change
        then
    then
    $33F story-flag? not if
        0 $E char-in-area? 0 -5 -78 $32 char-faces-xz? and if
            5 $E 0 scene-change
        then
    then
    $91 story-flag? not if
        0 $C char-in-area? 0 3 79 $32 char-faces-xz? and if
            5 $E 0 scene-change
        then
    else $8E story-flag? if
        0 $C char-in-area? 0 3 79 $32 char-faces-xz? and if
            5 $F 0 scene-change
        then
    then then
    0 $F char-in-area? 0 -6 -12 $3C char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 $10 char-in-area? 0 -9 10 $3C char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 $11 char-in-area? 0 12 7 $3C char-faces-xz? and if
        5 $12 0 scene-change
    then
;

: water-tower-8f-1.phase3 ( -- )   \ 0042F860
    9 ebit? not if
        17.4 630.0 89.8 -59.9 630.0 69.0 17.4 580.0 89.8 -59.9 580.0 69.0 lights-doorway
    then
    -51.2 630.0 75.7 -91.2 630.0 6.5 -51.2 580.0 75.7 -91.2 580.0 6.5 lights-doorway
    -23.7 617.0 -26.3 -33.3 617.0 12.9 -55.3 617.0 -57.8 -74.5 617.0 29.0 lights-doorway
    6.0 617.0 -35.0 -29.8 617.0 -19.9 15.6 617.0 -78.5 -66.5 617.0 -44.5 lights-doorway
    27.8 617.0 -23.1 6.0 617.0 -35.0 66.5 617.0 -44.4 15.6 617.0 -78.5 lights-doorway
    7.0 617.0 40.4 35.1 617.0 6.9 15.6 617.0 78.5 78.5 617.0 15.6 lights-doorway
    35.1 617.0 6.9 27.8 617.0 -23.0 78.5 617.0 15.6 66.5 617.0 -44.4 lights-doorway
;

: water-tower-8f-1.phase5 ( -- )   \ 0042F9C0
    1 0 char-in-nav-group? if
        1 action-end
        0 0 var? if
            1 $33 2.82 68.25 14 char-to-xz
        else 0 1 var? if
            1 $38 -64.059 -24.05 -107 char-to-xz
        else 0 2 var? if
            1 $4C 58.05 -31.28 121 char-to-xz
        then then then
    then
;

: water-tower-8f-1.act00 ( -- )   \ 0042FA10
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 var? if
        $91 -15.35 -32.75 20 $FFFF $A self-move-to
        self-wait-done
    else 0 1 var? if
        $C7 -22.68 28.06 140 $FFFF $A self-move-to
        self-wait-done
    else 0 2 var? if
        $14 31.63 18.64 -120 $FFFF $A self-move-to
        self-wait-done
    then then then
    1 self-scripted
    1 self-noclip
    0 0 8 nav-group
    1 0 $30 nav-group
    0 0 var? if
        $C4 -5.62 -11.6 20 $204 5 self-move-to
        self-wait-done
    else 0 1 var? if
        $C0 -8.96 10.23 140 $204 5 self-move-to
        self-wait-done
    else 0 2 var? if
        $BE 11.74 6.8 -120 $204 5 self-move-to
        self-wait-done
    then then then
    2 water-tower-8f-1.cmd00
    3 water-tower-8f-1.cmd00
    0 0 var? if
        1 $D0 -0.12 8.95 5 char-to-xz
    else 0 1 var? if
        1 $FF -7.73 -4.22 -125 char-to-xz
    else 0 2 var? if
        1 $F9 6.6 -4.24 117 char-to-xz
    then then then
    1 self-scripted
    0 self-noclip
    $204 0 hewie-anim-root
    0 water-tower-8f-1.cmd00
    1 water-tower-8f-1.cmd00
    0 8 self-anim-blend
    self-frames-reset
    8 self-wait-frames
    $102 self-anim
    self-wait-anim
    2 $A self-anim-blend
    self-frames-reset
    $A self-wait-frames
    0 ebit-set
    begin
        1 ebit? not 2 ebit? not and while
        0 0 var? if
            $FE 8 char-in-area? if
                $FE self-look-at
                yield
            else 0 8 char-in-area? if
                0 self-look-at
                yield
            else
                $FF self-look-at
                yield
            then then
        else 0 1 var? if
            $FE 9 char-in-area? if
                $FE self-look-at
                yield
            else 0 9 char-in-area? if
                0 self-look-at
                yield
            else
                $FF self-look-at
                yield
            then then
        else 0 2 var? if
            $FE $A char-in-area? if
                $FE self-look-at
                yield
            else 0 $A char-in-area? if
                0 self-look-at
                yield
            else
                $FF self-look-at
                yield
            then then
        then then then
        yield
    repeat
    $FF self-look-at
    yield
    1 ebit? if
        $75 0 hewie-action
        3 ebit-clear
        begin
            3 ebit? not if
                1 $38 char-on-nav-flags? not if
                    3 ebit-set
                    0 0 $30 nav-group
                    1 0 8 nav-group
                then
            then
            117 hewie-action? while
            yield
        repeat
        1 $38 char-on-nav-flags? if
            0 0 var? if
                $33 2.39 70.98 1.0 100 hewie-go-to
                self-wait-done
            else 0 1 var? if
                $38 -61.02 -37.59 1.0 100 hewie-go-to
                self-wait-done
            else 0 2 var? if
                $4C 61.9 -34.57 1.0 100 hewie-go-to
                self-wait-done
            then then then
            0 0 $30 nav-group
            1 0 8 nav-group
        then
    else
        hewie-bark
        self-wait-done
        0 0 var? if
            $33 2.39 70.98 1.0 100 hewie-go-to
            self-wait-done
        else 0 1 var? if
            $38 -61.02 -37.59 1.0 100 hewie-go-to
            self-wait-done
        else 0 2 var? if
            $4C 61.9 -34.57 1.0 100 hewie-go-to
            self-wait-done
        then then then
        0 0 $30 nav-group
        1 0 8 nav-group
    then
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-8f-1.act01 ( -- )   \ 0042FCA0
    $FF 1.0 0 bgm
    stalkers-stay state-flag-set
    1 self-scripted
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
    $F 4 fade
    wait-fade
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    0 $F9 $C action
    9 ebit-set
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
    9 ebit-clear
    1 water-tower-8f-1.cmd03
    scene-5-pending state-flag-clear
    no-stalker-camera state-flag-clear
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
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
    0 $F9 $D action
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
    $40 resident-flag-set
    50 hewie-trust
    $FE action-end
    $FE char-done
    $8E story-flag-set
    0 $33 -1.38 70.2 0 char-to-xz
    1 $33 5.0 67.76 0 char-to-xz
    $AE story-flag? not if
        2 0 char-remove
    then
    camera-restart
    $103 door-unlock
    $100 door-unlock
    $E0 door-lock
    $FE $C5 0 room-doors-state
    $FE $C6 0 room-doors-state
    $FE $C7 0 room-doors-state
    1 0 8 nav-group
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    $1B 1.0 0 bgm
    $F $41 fade
    wait-fade
    $41 resident-flag-set
    $24D item-give
    $828B item-give
    shared.act9C
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-8f-1.act02 ( -- )   \ 0042FE40
    self-wait-done
    $8E story-flag? not if
        1 self-scripted
        1 self-noclip
        self-frames-reset
        8 self-wait-frames
        world-frozen state-flag-set
        0 $36 0.0 33.0 180 char-to-xz
        1 10.0 60.0 0.0 5.0 event-camera
        self-frames-reset
        8 self-wait-frames
        4 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 self-scripted
        0 self-noclip
        0 $9D 0.0 41.0 180 char-to-xz
    else $A4 1 item-count? if
        1 self-scripted
        1 self-noclip
        self-frames-reset
        8 self-wait-frames
        world-frozen state-flag-set
        0 $D2 -0.95 33.0 180 char-to-xz
        1 2.0 50.0 0.0 5.0 event-camera
        self-frames-reset
        8 self-wait-frames
        $A message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 self-scripted
        0 self-noclip
        0 $9D 0.0 41.0 180 char-to-xz
    else
        $B message
        wait-message
    then then
    self-idle-or-end
;

: water-tower-8f-1.act07 ( -- )   \ 00430110
    $F $44 fade
    $FF 1.0 0 bgm
    wait-fade
    8 0 $14 door-bits
    1 water-tower-8f-1.cmd06
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    stalkers-stay state-flag-set
    1 action-end
    1 char-done
    2 char-unload
    summoner-on state-flag-clear
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
    0 $F9 $B action
    $10 $50 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    3 message-prepare
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
    1 1 $14 door-bits
    8 1 $14 door-bits
    0 water-tower-8f-1.cmd06
    $100 door-open-clear
    $100 door-lock
    doors-room-in
    camera-restart
    0 $33 6.38 70.12 180 char-to-xz
    1 char-activate
    $C7 0 41 hewie-to-room
    1 $29 -17.04 65.97 -160 char-to-xz
    $FE char-activate
    $FE $C7 89 2 stalker-to-room
    $FE $59 -58.86 10.32 0 char-to-xz
    0 water-tower-8f-1.cmd03
    1 0 8 nav-group
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    scene-5-pending state-flag-set
    9 1.0 0 bgm
    no-stalker-camera state-flag-set
    $F 1 fade
    wait-fade
    $3F resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-8f-1.act05 ( -- )   \ 0042FF20
    self-wait-done
    1 self-scripted
    1 self-noclip
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-set
    0 $36 0.0 33.0 180 char-to-xz
    1 10.0 60.0 0.0 5.0 event-camera
    self-frames-reset
    8 self-wait-frames
    0 water-tower-8f-1.cmd02
    5 message
    begin
        1 water-tower-8f-1.cmd02
        4 ebit? if
            5 ebit? not water-tower-8f-1.cond00? not or if
                1 $86 0.0 0.0 0.0 0 0 sound
                yield
            else
                0 6 -1.0 632.0 31.0 0 0 sound
                0 1 $14 door-bits
                $23 item-use
                6 ebit-set
                2 0 char-remove
                $25 partner-load
                5 message-close
                2 water-tower-8f-1.cmd02
                self-frames-reset
                $1E self-wait-frames
                6 ebit? if
                    ['] water-tower-8f-1.act07 goto
                then
                0 0.0 0.0 0.0 0.0 event-camera
                world-frozen state-flag-clear
                0 self-scripted
                0 self-noclip
                0 $9D 0.0 41.0 180 char-to-xz
                self-idle-or-end
            then
        else
            $2C $85 0.0 0.0 0.0 0 0 sound
            5 message-close
            2 water-tower-8f-1.cmd02
            self-frames-reset
            $1E self-wait-frames
            6 ebit? if
                ['] water-tower-8f-1.act07 goto
            then
            0 0.0 0.0 0.0 0.0 event-camera
            world-frozen state-flag-clear
            0 self-scripted
            0 self-noclip
            0 $9D 0.0 41.0 180 char-to-xz
            self-idle-or-end
        then
    again
;

: water-tower-8f-1.act03 ( -- )   \ 0047ADD0
    5 ebit-clear
    ['] water-tower-8f-1.act05 goto
;

: water-tower-8f-1.act04 ( -- )   \ 0047ADD8
    5 ebit-set
    ['] water-tower-8f-1.act05 goto
;

: water-tower-8f-1.act06 ( -- )   \ 00430000
    0 7 6 char-sound
    5 0 var? if
        2 water-tower-8f-1.cmd01
        0 -1.0 623.0 77.0 0 0 0 0 dust
        0 4.0 623.0 77.0 0 0 0 0 dust
        2 0 var? if
            0 8 6 char-sound
            0 water-tower-8f-1.cmd04
            3 water-tower-8f-1.cmd04
            2 0 $14 door-bits
            3 1 $14 door-bits
            $91 story-flag-set
        then
    else 5 1 var? if
        3 water-tower-8f-1.cmd01
        0 -59.0 623.0 50.0 0 0 0 0 dust
        0 -55.0 623.0 54.0 0 0 0 0 dust
        3 0 var? if
            0 8 6 char-sound
            1 water-tower-8f-1.cmd04
            4 water-tower-8f-1.cmd04
            4 0 $14 door-bits
            5 1 $14 door-bits
            $33E story-flag-set
        then
    else 5 2 var? if
        4 water-tower-8f-1.cmd01
        0 -8.0 623.0 -77.0 0 0 0 0 dust
        0 -2.0 623.0 -77.0 0 0 0 0 dust
        4 0 var? if
            0 8 6 char-sound
            2 water-tower-8f-1.cmd04
            5 water-tower-8f-1.cmd04
            6 0 $14 door-bits
            7 1 $14 door-bits
            $33F story-flag-set
        then
    then then then
    self-frames-reset
    $1E self-wait-frames
    self-idle-or-end
;

: water-tower-8f-1.act08 ( -- )   \ 00430250
    1 self-scripted
    self-wait-done
    $FE self-look-at
    yield
    $C00 self-anim
    0 1 char-wait-motion
    0 $2F 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    ['] water-tower-8f-1.act01 goto
;

: water-tower-8f-1.act09 ( -- )   \ 0047ADE0
    2 message
    self-idle-or-end
;

: water-tower-8f-1.act0A ( -- )   \ 00430270
    $F $54 fade
    $FF 1.0 0 bgm
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    wait-fade
    $FF 1 char-visible
    $B 3 $FF char-load
    $B $1E char-layer
    $FE $A char-layer
    3 char-unload
    1 action-end
    1 char-done
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $2D $41 movie-param
    0 $F9 $11 action
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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
    1 water-tower-8f-1.cmd05
    2 0 char-remove
    3 0 char-remove
    world-held state-flag-set
    0 exit-prepare
    $93 story-flag-set
    $42 resident-flag-set
    $81 exit-check
    self-idle-or-end
;

: water-tower-8f-1.act0B ( -- )   \ 00430330
    0 water-tower-8f-1.cmd03
    begin
        $4F5 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $523 cutscene-cue-reached? not while
        2 water-tower-8f-1.cmd03
        yield
    repeat
    1 water-tower-8f-1.cmd03
    self-idle-or-end
;

: water-tower-8f-1.act0C ( -- )   \ 00430350
    0 water-tower-8f-1.cmd03
    begin
        $1A cutscene-cue-reached? not while
        yield
    repeat
    begin
        $38 cutscene-cue-reached? not while
        3 water-tower-8f-1.cmd03
        yield
    repeat
    1 water-tower-8f-1.cmd03
    self-idle-or-end
;

: water-tower-8f-1.act0D ( -- )   \ 00430370
    begin
        0 cutscene-shot? not while
        yield
    repeat
    begin
        $E8 cutscene-cue-reached? not while
        $3E sprites-additive
        yield
    repeat
    begin
        $E9 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $11C cutscene-cue-reached? not while
        $29 sprites-additive
        yield
    repeat
    self-idle-or-end
;

: water-tower-8f-1.act0E ( -- )   \ 0047ADE8
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: water-tower-8f-1.act0F ( -- )   \ 004303A0
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    7 ebit? not if
        7 ebit-set
        7 message
        wait-message
    else
        7 ebit-clear
        8 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: water-tower-8f-1.act10 ( -- )   \ 0047ADF0
    self-wait-done
    $C message
    wait-message
    self-idle-or-end
;

: water-tower-8f-1.act11 ( -- )   \ 004303C0
    begin
        4 cutscene-shot? not while
        yield
    repeat
    0 water-tower-8f-1.cmd05
    self-idle-or-end
;

: water-tower-8f-1.act12 ( -- )   \ 004303D0
    self-wait-done
    0 $F char-in-area? if
        -6.0 -12.0 self-turn-to-xz
        self-wait-done
    else 0 $10 char-in-area? if
        -9.0 10.0 self-turn-to-xz
        self-wait-done
    else 0 $11 char-in-area? if
        12.0 7.0 self-turn-to-xz
        self-wait-done
    then then then
    $A01 self-anim
    self-frames-reset
    self-wait-16
    9 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: water-tower-8f-1.act13 ( -- )   \ 00430410
    self-wait-done
    0 1 $14 door-bits
    2 1 $14 door-bits
    4 1 $14 door-bits
    6 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $25 partner-load
    2 char-unload
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
    0 $F9 $B action
    $10 $50 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    3 message-prepare
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
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: water-tower-8f-1.act14 ( -- )   \ 004304C0
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    3 1 $14 door-bits
    5 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $25 partner-load
    2 char-unload
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
    0 $F9 $C action
    9 ebit-set
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
    9 ebit-clear
    1 water-tower-8f-1.cmd03
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: water-tower-8f-1.act15 ( -- )   \ 00430580
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    3 1 $14 door-bits
    5 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $17 3 $FF char-load
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
    0 $F9 $D action
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
    1 action-end
    1 char-done
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: water-tower-8f-1.act16 ( -- )   \ 00430630
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    3 1 $14 door-bits
    5 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $17 3 $FF char-load
    $B 4 $FF char-load
    3 char-unload
    4 char-unload
    $FF 1 char-visible
    $B $1E char-layer
    $FE $A char-layer
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $2D $41 movie-param
    0 $F9 $11 action
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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
    1 water-tower-8f-1.cmd05
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' water-tower-8f-1.enter water-tower-8f-1 0 room-script!
' water-tower-8f-1.char-enter water-tower-8f-1 6 room-script!
' water-tower-8f-1.phase1 water-tower-8f-1 1 room-script!
' water-tower-8f-1.phase2 water-tower-8f-1 2 room-script!
' water-tower-8f-1.phase3 water-tower-8f-1 3 room-script!
' water-tower-8f-1.phase5 water-tower-8f-1 5 room-script!
' water-tower-8f-1.act00 water-tower-8f-1 $00 action-script!
' water-tower-8f-1.act01 water-tower-8f-1 $01 action-script!
' water-tower-8f-1.act02 water-tower-8f-1 $02 action-script!
' water-tower-8f-1.act03 water-tower-8f-1 $03 action-script!
' water-tower-8f-1.act04 water-tower-8f-1 $04 action-script!
' water-tower-8f-1.act05 water-tower-8f-1 $05 action-script!
' water-tower-8f-1.act06 water-tower-8f-1 $06 action-script!
' water-tower-8f-1.act07 water-tower-8f-1 $07 action-script!
' water-tower-8f-1.act08 water-tower-8f-1 $08 action-script!
' water-tower-8f-1.act09 water-tower-8f-1 $09 action-script!
' water-tower-8f-1.act0A water-tower-8f-1 $0A action-script!
' water-tower-8f-1.act0B water-tower-8f-1 $0B action-script!
' water-tower-8f-1.act0C water-tower-8f-1 $0C action-script!
' water-tower-8f-1.act0D water-tower-8f-1 $0D action-script!
' water-tower-8f-1.act0E water-tower-8f-1 $0E action-script!
' water-tower-8f-1.act0F water-tower-8f-1 $0F action-script!
' water-tower-8f-1.act10 water-tower-8f-1 $10 action-script!
' water-tower-8f-1.act11 water-tower-8f-1 $11 action-script!
' water-tower-8f-1.act12 water-tower-8f-1 $12 action-script!
' water-tower-8f-1.act13 water-tower-8f-1 $13 action-script!
' water-tower-8f-1.act14 water-tower-8f-1 $14 action-script!
' water-tower-8f-1.act15 water-tower-8f-1 $15 action-script!
' water-tower-8f-1.act16 water-tower-8f-1 $16 action-script!
