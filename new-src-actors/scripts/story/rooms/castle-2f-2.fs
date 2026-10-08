\ story/rooms/castle-2f-2.fs - the event scripts of room castle-2f-2 ($20; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-2
USING: room-names story.words story.shared ;

\ room 0x20 (Room20_Cmd01_ptmf): the falling object back up in place
: castle-2f-2.cmd01 ( -- )  s" castle-2f-2.cmd01" stub-step ;
\ a pendulum (room object Room20_ObjectNames[byte 3]): byte 4 0 still; 1 its phase +0x30 on by 2
\ degrees (a tick sound at (-85, 30, 90) each turn), swinging 15 degrees (+0x14)
: castle-2f-2.cmd02 ( b0 b1 -- )  drop drop s" castle-2f-2.cmd02" stub-step ;
\ room 0x20 (Room20_Cmd03_ptmf): the falling object falls (gravity 0.2 a frame on its velocity
\ +0x30, spinning 1 degree a frame) along the nav mesh, events bit 1 set while it lies on open
\ floor; landing on floor that isn't 0x10000 raises dust
: castle-2f-2.cmd03 ( -- )  s" castle-2f-2.cmd03" stub-step ;
\ room 0x20 (Room20_Cmd05_ptmf): the character turns to face object byte 3
: castle-2f-2.cmd05 ( b0 -- )  drop s" castle-2f-2.cmd05" stub-step ;
\ room 0x20 (Room20_Cond00_ptmf): the player, free and within 5 of the falling object, knocks it
\ - it gets a push (0, 1, 1) turned by her facing, and its nav triangle
: castle-2f-2.cond00? ( -- flag )  s" castle-2f-2.cond00?" stub-flag ;
\ room 0x20 (Room20_Cond01_ptmf): the character's script value is at least be32 bytes 3..6
: castle-2f-2.cond01? ( b0 b1 b2 b3 -- flag )  drop drop drop drop s" castle-2f-2.cond01?" stub-flag ;
\ room 0x20 (Room20_Cond02_ptmf): object byte 3 becomes event point byte 4 (radii 5)
: castle-2f-2.cond02? ( b0 b1 -- flag )  drop drop s" castle-2f-2.cond02?" stub-flag ;

: castle-2f-2.enter ( -- )   \ 003FE460
    room-sounds
    $D story-flag? not if
        0 $F2 9 action
    then
    $1C story-flag? not if
        0 0 0 $36B $3AF obstacle-place
    else
        0 0 0 $377 $3BB obstacle-place
        0 obstacle-stop
    then
    0 23.3 12.75 87.0 0 effect-86
    1 25.15 12.75 86.0 0 effect-86
    $24A story-flag? $24B story-flag? not and if
        2 -11.6 1.0 142.2 flicker-sprite
    then
    $A story-flag? if
        $2E story-flag? not if
            $2E story-flag-set
            $20 0 316 $D 15 -1 0 0.0 creature-place
            $20 0 361 4 15 -1 0 0.0 creature-place
        then
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 0 castle-2f-2.cmd02
    4 0 castle-2f-2.cmd02
    0 $F4 $11 action
    5 1 object-show
    $349 story-flag? not if
        1 ebit-clear
        $D story-flag? $31C story-flag? not and if
            5 0 object-show
            castle-2f-2.cmd01
            1 ebit-set
            7 ebit-set
        then
    then
    5 story-flag? 6 story-flag? not and if
        $80 exit-taken? if
        else
            0 $F3 $E action
        then
    then
;

: castle-2f-2.char-enter ( -- )   \ 003FE540
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    3 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    4 3 3 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $142 0 char-to-tri-facing
            0 0 0 action
        then
    then
;

: castle-2f-2.phase1 ( -- )   \ 003FE680
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    5 story-flag? 6 story-flag? not and if
        2 exit-usable? if
            3 3 2 char-load
            4 4 2 char-load
            2 exit-check
        then
    else 2 exit-usable? if
        2 exit-check
    then then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $D 3 3 1 chars-area-camera
    $E 1 1 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    $10 2 2 1 chars-area-camera
    $14 3 3 1 chars-area-camera
    $15 0 0 1 chars-area-camera
    $1A 1 1 1 chars-area-camera
    0 5 char-entered-area? 0 $B char-entered-area? or if
        0 exit-prepare
    then
    0 6 char-entered-area? 0 7 char-entered-area? or if
        1 exit-prepare
    then
    0 9 char-entered-area? if
        5 story-flag? 6 story-flag? not and if
            3 3 2 char-load
            4 4 2 char-load
        then
        2 exit-prepare
    then
    0 $C char-entered-area? if
        3 exit-prepare
    then
    0 8 char-entered-area? if
        4 exit-prepare
    then
    0 $A char-entered-area? if
        5 story-flag? 6 story-flag? not and if
            3 0 char-remove
            4 0 char-remove
        then
        4 exit-prepare
    then
    $D story-flag? not if
        0 $17 char-entered-area? 8 ebit? not and if
            8 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 3 action-force
            else
                1 0 3 action-force
            then
        then
    then
    $1C story-flag? not if
        0 $377 obstacle-on? if
            $1C story-flag-set
            0 obstacle-stop
            $12 door-unlock
            $FE $24 0 room-doors-state
        then
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    $24A story-flag? not if
        2 -11.6 0.0 142.2 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 586 var-set
                $1A 587 var-set
                $1B 2 var-set
                $1C -11600 var-set
                $1D 1000 var-set
                $1E 142200 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    $349 story-flag? not if
        1 ebit? if
            castle-2f-2.cond00? if
                0 4 6 char-sound
                1 ebit-clear
                7 ebit-clear
                $F1 action-end
                0 $F1 $B action
            then
            5 6 castle-2f-2.cond02? if
            then
        then
    then
    5 story-flag? 6 story-flag? not and if
        1 2 char-in-area? not if
            1 -144.0 0.0 109.0 $1E 20 0 zone
            3 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 1 9 char-zone-bits? if
                            3 ebit-set
                            $64 chance? if
                                $1F 1 var-set
                                0 1 $8A action
                            then
                        then
                    then
                then
            then
        then
    then
    $1C story-flag? 1 4 char-in-area? not and if
        0 33.656 0.0 111.061 $A 20 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        2 ebit-set
                        $1E chance? if
                            $1F 0 var-set
                            0 1 $89 action
                        then
                    then
                then
            then
        then
    then
    $FE $1B char-in-area? 4 stalker-alert? and $FE 2 char-C4? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 $FE $D action
        then
    then
    3 -177.49 0.0 89.35 $12 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 55.23 0.0 -184.17 $14 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    5 -84.16 0.0 90.89 $12 28 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 28.0 hewie-look-zone
            then
        then
    then
    6 sound-bank-loaded? if
        $D story-flag? not if
            $C0000002 6 -82.0 0.0 92.0 0 0 sound
        then
        $C0000001 6 -85.0 30.0 90.0 0 0 sound
    then
;

: castle-2f-2.phase2 ( -- )   \ 003FE9C0
    0 2 char-group-bit4? if
        5 story-flag? 6 story-flag? not and $FE char-here? not and if
            5 2 0 scene-change
        then
        6 story-flag? -2147483646 scene-request? and if
            5 5 1 scene-change
        then
    then
    0 $11 $2D char-faces-area? if
        5 6 0 scene-change
    then
    0 $12 $2D char-faces-area? if
        5 7 0 scene-change
    then
    0 $13 $2D char-faces-area? if
        5 8 0 scene-change
    then
    $1C story-flag? not 0 $16 char-in-area? and if
        5 1 0 scene-change
    then
    0 $18 char-in-area? 0 $19 char-in-area? or 0 0 $32 char-heading? and if
        5 $A 0 scene-change
    then
    $24A story-flag? $24B story-flag? not and if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $C 4 scene-change
        then
    then
    $349 story-flag? not if
        1 ebit? if
            5 6 castle-2f-2.cond02? if
                0 6 3 char-zone-bits? if
                    5 $F 0 scene-change
                then
            then
        then
    then
;

: castle-2f-2.phase3 ( -- )   \ 003FEA60
    107.0 8.0 -164.5 85.8 8.3 -178.2 107.3 -25.5 -164.4 85.8 -25.5 -178.2 lights-doorway
    87.9 8.3 -176.8 50.5 8.3 -179.8 87.9 -25.5 -176.8 87.9 -25.5 -176.8 lights-doorway
;

: castle-2f-2.phase5 ( -- )   \ 003FEAD0
    $FE char-busy? if
        4 ebit? if
            $FE action-end
            0 summon-take
        then
    then
    6 story-flag? $11 door-locked? and if
        $11 door-unlock
    then
;

: castle-2f-2.act00 ( -- )   \ 003FEAF0
    1 self-scripted
    self-wait-done
    $F02 0 self-anim-blend
    6 story-flag-set
    8 state-flag-clear
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    $F $41 fade
    wait-fade
    $209 item-give
    0 ebit-set
    $18 state-flag-clear
    $64 threat-raise
    self-wait-anim
    0 self-scripted
    self-idle-or-end
;

: castle-2f-2.act01 ( -- )   \ 0047AAC0
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: castle-2f-2.act02 ( -- )   \ 003FEB30
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $11 self-through-door
    self-wait-done
    $609 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $F3 action-end
    $A02 self-anim
    self-frames-reset
    $16 self-wait-frames
    $F $44 fade
    wait-fade
    8 state-flag-set
    3 char-unload
    4 char-unload
    0 1 char-visible
    $80 exit-check
    self-idle-or-end
;

: castle-2f-2.act03 ( -- )   \ 003FEB70
    self-wait-done
    $F2 action-end
    $F $54 fade
    wait-fade
    $FE $20 552 2 stalker-to-room
    $FE action-end
    $FE char-done
    0 1 char-no-shadow
    $FE 1 char-no-shadow
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
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
    0 0 char-no-shadow
    $FE 0 char-no-shadow
    $FE action-end
    $FE char-done
    0 2 char-file-load
    0 char-file-use
    $FE char-activate
    $FE 0 stalker-mode
    $349 story-flag? not if
        castle-2f-2.cmd01
        1 ebit-set
    then
    $FE $30A -7.812 78.949 179 char-to-xz
    $FE 3 3 char-camera
    0 $FE 4 action
    0 $2E9 -8.354 49.017 -10 char-to-xz
    hewie-controlled? not if
        0 4 4 char-camera
        0 camera-follow
    else
        1 4 4 char-camera
        1 camera-follow
    then
    camera-restart
    $1B state-flag-clear
    $8001 0 self-anim-blend
    $D story-flag-set
    8 state-flag-clear
    5 0 0 music
    $F $91 fade
    $8003 self-anim
    self-wait-anim
    stalker-item-cooldown
    wait-fade
    $224 item-give
    self-idle-or-end
;

: castle-2f-2.act04 ( -- )   \ 003FEC80
    self-wait-done
    0 self-look-at
    yield
    $2302 $A self-anim-blend
    self-wait-anim
    self-frames-reset
    begin
        $19 chance? if
            $1302 $A self-anim-blend
            self-wait-anim
        else $19 chance? if
            $1305 $A self-anim-blend
            self-wait-anim
        then then
        4 camera-mode? not 0 0 1 $C2 castle-2f-2.cond01? or if
            self-idle-or-end
        else
            yield
        then
    again
;

: castle-2f-2.act05 ( -- )   \ 003FECB8
    self-wait-done
    0 ebit? if
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-2.act06 ( -- )   \ 003FECD0
    self-wait-done
    $304 story-flag? not if
        $304 story-flag-set
        0 $43 5 char-sound
        $F00 self-anim
        $5A threat-add
        1 $FF 8 rumble
        self-frames-reset
        self-wait-16
        2 message
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        wait-message
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-2.act07 ( -- )   \ 003FED00
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-2.act08 ( -- )   \ 003FED10
    self-wait-done
    $292 -173.0 90.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 6.0 event-camera
    self-frames-reset
    $10 self-wait-frames
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    4 message
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-2.act09 ( -- )   \ 003FED60
    0 180 var-set
    begin
        yield
        6 sound-bank-loaded? if
            0 180 var? if
                0 0 var-set
                $40000002 6 -82.0 0.0 92.0 0 0 sound
            else
                0 var-inc
            then
        then
    again
;

: castle-2f-2.act0A ( -- )   \ 003FEDA0
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    7 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-2.act0B ( -- )   \ 003FEDB8
    $31C story-flag-set
    begin
        castle-2f-2.cmd03
        yield
    again
;

: castle-2f-2.act0C ( -- )   \ 003FEDD0
    self-wait-done
    -11.6 142.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $24B story-flag-set
            2 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
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

: castle-2f-2.act0D ( -- )   \ 003FEE30
    4 ebit-set
    self-wait-done
    $FE 6 char-file-load
    $1A2 77.27 -192.48 0 $FFFF $A self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    $8000 5 self-anim-9
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        self-at-motion-event? not while
        yield
    repeat
    0 self-noclip
    0 self-scripted
    4 ebit-clear
    $FE action-end
    0 summon-take
    self-idle-or-end
;

: castle-2f-2.act0E ( -- )   \ 003FEE70
    begin
        5 ebit? not if
            5 ebit-set
            $1E chance? if
                1 31 var-set
            else $32 chance? if
                1 61 var-set
            else
                1 1 var-set
            then then
        then
        yield
        1 var-dec
        1 0 var? 1 30 var? or 1 60 var? or 1 90 var? or if
            3 6 -145.0 10.0 150.0 0 0 sound
            1 0 var? if
                5 ebit-clear
            then
            $1E chance? if
                self-frames-reset
                $1E self-wait-frames
            else $32 chance? if
                self-frames-reset
                $2D self-wait-frames
            else
                self-frames-reset
                $5A self-wait-frames
            then then
        then
    again
;

: castle-2f-2.act0F ( -- )   \ 003FEEF0
    self-wait-done
    5 castle-2f-2.cmd05
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $93 message-param-room
    $93 $63 item-count? if
        $8010 message
        wait-message
    else
        $93 1 item-give-count
        $349 story-flag-set
        5 1 object-show
        0 $93 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8011 message
        wait-message
    then
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-2f-2.act10 ( -- )   \ 003FEF40
    self-wait-done
    2 3 $FF char-load
    3 char-unload
    0 0 0 $36B $3AF obstacle-place
    0 23.3 12.75 87.0 0 effect-86
    1 25.15 12.75 86.0 0 effect-86
    3 0 castle-2f-2.cmd02
    4 0 castle-2f-2.cmd02
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 1 char-no-shadow
    $FE 1 char-no-shadow
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
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
    0 0 char-no-shadow
    $FE 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-2f-2.act11 ( -- )   \ 003FF018
    begin
        3 1 castle-2f-2.cmd02
        4 1 castle-2f-2.cmd02
        yield
    again
;

\ ---- registered ----
' castle-2f-2.enter castle-2f-2 0 room-script!
' castle-2f-2.char-enter castle-2f-2 6 room-script!
' castle-2f-2.phase1 castle-2f-2 1 room-script!
' castle-2f-2.phase2 castle-2f-2 2 room-script!
' castle-2f-2.phase3 castle-2f-2 3 room-script!
' castle-2f-2.phase5 castle-2f-2 5 room-script!
' castle-2f-2.act00 castle-2f-2 $00 action-script!
' castle-2f-2.act01 castle-2f-2 $01 action-script!
' castle-2f-2.act02 castle-2f-2 $02 action-script!
' castle-2f-2.act03 castle-2f-2 $03 action-script!
' castle-2f-2.act04 castle-2f-2 $04 action-script!
' castle-2f-2.act05 castle-2f-2 $05 action-script!
' castle-2f-2.act06 castle-2f-2 $06 action-script!
' castle-2f-2.act07 castle-2f-2 $07 action-script!
' castle-2f-2.act08 castle-2f-2 $08 action-script!
' castle-2f-2.act09 castle-2f-2 $09 action-script!
' castle-2f-2.act0A castle-2f-2 $0A action-script!
' castle-2f-2.act0B castle-2f-2 $0B action-script!
' castle-2f-2.act0C castle-2f-2 $0C action-script!
' castle-2f-2.act0D castle-2f-2 $0D action-script!
' castle-2f-2.act0E castle-2f-2 $0E action-script!
' castle-2f-2.act0F castle-2f-2 $0F action-script!
' castle-2f-2.act10 castle-2f-2 $10 action-script!
' castle-2f-2.act11 castle-2f-2 $11 action-script!
