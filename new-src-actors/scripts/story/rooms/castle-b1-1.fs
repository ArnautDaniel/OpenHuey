\ story/rooms/castle-b1-1.fs - the event scripts of room castle-b1-1 ($12; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-1
USING: room-names story.words story.shared ;

\ room 0x12 (Room12_Cmd00_ptmf): byte 3 0: the smoke puffs (SmokePuffs_vtable), their slot in
\ script variable 0; else that slot started
: castle-b1-1.cmd00 ( b0 -- )  drop s" castle-b1-1.cmd00" stub-step ;

: castle-b1-1.enter ( -- )   \ 003F8200
    room-sounds
    1 story-flag? not if
        0 1 $14 door-bits
    then
    0 -35.8 -50.5 -27.25 0 effect-86
    $203 story-flag? not if
        1 0 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        $22C story-flag? not if
            1 -52.5 -59.05 -1.5 flicker-sprite
        then
    then
    0 8 0.562 0.5 0.25 0.5 zone-rect
    $244 story-flag? $245 story-flag? not and if
        2 -18.1 -59.0 -8.1 flicker-sprite
    then
    $281 story-flag? not if
        3 -13.0 9.0 10.0 flicker-sprite
    then
    3 story-flag? $31 story-flag? not and if
        7 state-flag-set
        1 1 $1000010 nav-group
        3 char-unload
        3 char-activate
        0 3 0 char-model-op
        3 $9000 1 0 char-anim-hold
        3 $2A 14.85 17.65 0 char-to-xz
        4 0 object-anim-loop
        0 castle-b1-1.cmd00
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 0 -19.5 -26.0 -31.0 4.0 4.0 100.0 0.0 10.0 scene-effect-71000
    2 0 -53.0 -22.0 -11.0 4.0 5.0 80.0 0.0 30.0 scene-effect-71000
    $321 story-flag? not if
        3 1 $14 door-bits
        4 0 $14 door-bits
        1 2 $20000 nav-group
    else
        3 0 $14 door-bits
        4 1 $14 door-bits
        0 2 $20000 nav-group
    then
    1 $2300 sound-volume
;

: castle-b1-1.act10 ( -- )   \ 003F8DE0
    6 ebit-set
    8 ebit-set
    5 ebit-clear
    $E 0 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 1 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 2 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 3 pvar? if
        $64 chance? if
            5 ebit-set
        then
    then then then then
    $FE char-unseen? not if
        5 ebit-set
    then
    2 creature-action? if
        5 ebit-set
    then
    4 stalker-alert? if
        5 ebit-clear
    then
    $FE camera-follow
    5 ebit? if
        0 $FE $11 action
    else
        $78 1 item-cooldown
        $B ebit-clear
    then
    $E pvar-inc
    exit
;

: castle-b1-1.char-enter ( -- )   \ 003F8350
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 5 -1 char-camera
                0 camera-follow
            else
                1 5 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then then
    0 5 -1 area-camera
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
    0 self-is? if
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
    1 self-is? if
    then
    $FE self-is? if
        9 state-flag? if
            castle-b1-1.act10
        else
            8 ebit-clear
        then
    then
;

: castle-b1-1.phase1 ( -- )   \ 003F83F0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 1 0 1 chars-area-camera
    3 2 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 3 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 3 1 1 chars-area-camera
    $10 1 0 1 chars-area-camera
    $11 5 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $13 char-entered-area? if
        2 map-page
    then
    0 $13 char-left-area? if
        1 map-page
    then
    1 story-flag? 2 story-flag? not and if
        0 5 char-entered-area? $A ebit? not and if
            $A ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 5 action-force
            else
                1 0 5 action-force
            then
        then
    then
    $244 story-flag? not if
        3 -18.1 -60.0 -8.1 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 580 var-set
                $1A 581 var-set
                $1B 2 var-set
                $1C -18100 var-set
                $1D -59000 var-set
                $1E -8100 var-set
                $1F 3 var-set
                0 1 $8B action
            then
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
    3 story-flag? $31 story-flag? not and if
        $32 story-flag? not if
            2 16.5 0.0 16.5 $1E 10 0 zone
        else
            2 16.5 0.0 16.5 $A 10 0 zone
        then
        $32 story-flag? not if
            0 2 3 char-zone-bits? 2 camera-mode? and if
                $32 story-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 0 action-force
                else
                    1 0 0 action-force
                then
            then
        else 0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
            0 0 char-action? $FF panic-stage? not and if
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 2 action-force
            then
        then then
        9 ebit? not if
            6 sound-bank-loaded? if
                $40000001 6 15.0 12.0 22.0 0 0 sound
                9 ebit-set
            then
        else
            $C0000001 6 15.0 12.0 22.0 0 0 sound
        then
        4 16.5 0.0 16.5 $19 10 0 zone
        7 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        7 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        6 16.5 0.0 16.5 $32 10 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 6 9 char-zone-bits? if
                    $1F 6 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    $FE 2 char-C4? not if
        9 state-flag? 6 ebit? not and $FE char-here? and if
            $B ebit-set
            castle-b1-1.act10
        then
    then
    $321 story-flag? not if
        5 22.0 -40.0 -32.0 6 12 0 zone
        0 5 char-in-zone? if
            0 0 6 char-sound
            1 $FF 4 rumble
            3 0 $14 door-bits
            4 1 $14 door-bits
            0 2 $20000 nav-group
            $321 story-flag-set
            0 8 0.234 0.5 0.328 0.5 zone-rect
            19.0 -40.0 -32.0 0 -2144325584 2 0.0 scene-effect-8C
            22.0 -40.0 -32.0 0 -2143272896 2 0.0 scene-effect-8C
            25.0 -40.0 -32.0 0 -2144325584 2 0.0 scene-effect-8C
            0 18.0 -36.0 -33.0 0 0 0 0 dust
            0 23.0 -38.0 -33.0 0 0 0 0 dust
            0 28.0 -36.0 -33.0 0 0 0 0 dust
        then
    then
    7 0.6 0.0 -3.32 $F 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 7 9 char-zone-bits? if
                $1F 7 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 -46.47 -60.0 -29.37 $E 19 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 8 9 char-zone-bits? if
                $1F 8 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
;

: castle-b1-1.phase2 ( -- )   \ 003F8750
    $203 story-flag? not if
        1 -52.5 -60.05 -1.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $203 story-flag-set
            0 0 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -52.5 -60.05 -1.5 0 -2146424784 0 0.0 scene-effect-8C
            3 6 -52.5 -60.05 -1.5 0 0 sound
            $40 $209 noise
            1 -52.5 -59.05 -1.5 flicker-sprite
        then
    else $22C story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
    $244 story-flag? $245 story-flag? not and if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
    $281 story-flag? not if
        9 3 5 5 0 zone-at-effect
        0 9 3 char-zone-bits? if
            5 $13 4 scene-change
        then
    then
    0 $12 $2D char-faces-area? if
        0 -45 $3C char-heading? if
            1 story-flag? not if
                0 $A $2D char-faces-area? if
                    5 4 4 scene-change
                then
            else $FE char-here? not if
                3 ebit-clear
                5 $E 5 scene-change
            else $FE char-unseen? if
                3 ebit-clear
                5 $E 5 scene-change
            else
                $8016 scene-ending
            then then then
        then
        0 45 $3C char-heading? if
            1 story-flag? not if
                0 $A $2D char-faces-area? if
                    5 4 4 scene-change
                then
            else $FE char-here? not if
                3 ebit-set
                5 $E 5 scene-change
            else $FE char-unseen? if
                3 ebit-set
                5 $E 5 scene-change
            else
                $8016 scene-ending
            then then then
        then
    then
    5 scene-request? not if
        0 9 char-in-area? 0 45 $32 char-heading? and if
            5 8 0 scene-change
        then
        0 $B char-in-area? 0 45 $32 char-heading? and if
            5 9 0 scene-change
        then
        0 $C char-in-area? 0 90 $32 char-heading? and if
            5 $A 0 scene-change
        then
        0 $D char-in-area? 0 0 $32 char-heading? and if
            5 $B 0 scene-change
        then
        3 story-flag? not if
            0 $E char-in-area? 0 0 $32 char-heading? and if
                5 $C 0 scene-change
            then
        else $31 story-flag? if
            0 $E char-in-area? 0 0 $32 char-heading? and if
                5 $16 0 scene-change
            then
        then then
        0 3 -2 $32 char-faces-xz? 0 -3 -2 $32 char-faces-xz? or 0 $F char-in-area? and if
            5 $D 0 scene-change
        then
    then
    0 5 3 char-zone-bits? 0 0 $32 char-heading? and if
        5 6 0 scene-change
    then
;

: castle-b1-1.phase3 ( -- )   \ 003F8900
    -19.0 8.0 0.0 13.0 8.0 0.0 -19.0 0.0 0.0 13.0 0.0 0.0 lights-doorway
    -18.1 13.0 -2.7 -16.2 13.0 -2.7 -18.1 0.0 -2.7 -16.2 0.0 -2.7 lights-doorway
    -18.1 13.0 -1.0 -18.1 13.0 -2.7 -18.1 0.0 -1.0 -18.1 0.0 -2.7 lights-doorway
    -19.0 8.0 15.0 -19.0 8.0 0.0 -19.0 0.0 15.0 -19.0 0.0 0.0 lights-doorway
;

: castle-b1-1.phase5 ( -- )   \ 003F89D0
    3 story-flag? $31 story-flag? not and if
        7 state-flag-clear
        1 $FE 0 char-model-op
        3 action-end
        3 char-done
    then
;

: castle-b1-1.act00 ( -- )   \ 003F89F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    4 object-anim-reset
    3 1 movie-play
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
    1 castle-b1-1.cmd00
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    $50 $FF movie-param
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
    3 $9000 1 0 char-anim-hold
    4 0 object-anim-loop
    4 0 object-show
    3 $2A 14.85 17.65 0 char-to-xz
    0 3 0 char-model-op
    1 0 char-visible
    1 action-end
    wait-fade
    0 self-move-16
    0 $18B 43.7 11.8 -75 char-to-xz
    hewie-controlled? not if
        0 2 -1 char-camera
        0 camera-follow
    else
        1 2 -1 char-camera
        1 camera-follow
    then
    camera-restart
    0 castle-b1-1.cmd00
    $F $51 fade
    wait-fade
    $226 item-give
    $1B resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act01 ( -- )   \ 003F8AF0
    self-wait-done
    -52.5 -1.5 self-turn-to-xz
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
            $22C story-flag-set
            1 effect-remove
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

: castle-b1-1.act02 ( -- )   \ 003F8B48
    self-wait-done
    3 self-look-at
    yield
    3 message
    wait-message
    $FF self-look-at
    yield
    self-idle-or-end
;

: castle-b1-1.act03 ( -- )   \ 003F8B60
    self-wait-done
    $FE self-touching? not if
        4 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 4 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    4 door-unlock
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

: castle-b1-1.act04 ( -- )   \ 003F8BA0
    1 self-scripted
    self-wait-done
    -35.35 -34.5 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    0 0 $14 door-bits
    5 message-param-room
    5 1 item-give-count
    0 5 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    1 story-flag-set
    0 state-flag-clear
    5 door-open-clear
    doors-room-in
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act05 ( -- )   \ 003F8BF0
    $18 state-flag-set
    $1B state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
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
    $FE $12 25 2 stalker-to-room
    $FE action-end
    $FE char-done
    4 3 $FF char-load
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
    $FE action-end
    $FE char-done
    $12 state-flag-set
    0 1 char-silent
    camera-restart
    $FE $12 0 room-doors-state
    $18 state-flag-clear
    0 self-scripted
    0 exit-check
    self-idle-or-end
;

: castle-b1-1.act06 ( -- )   \ 0047AA00
    self-wait-done
    $E message
    wait-message
    self-idle-or-end
;

: castle-b1-1.act07 ( -- )   \ 0047AA08
    self-idle-or-end
;

: castle-b1-1.act08 ( -- )   \ 003F8CA0
    self-wait-done
    1 ebit? not if
        8 message
        wait-message
        1 ebit-set
    else
        9 message
        wait-message
    then
    self-idle-or-end
;

: castle-b1-1.act09 ( -- )   \ 003F8CB8
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    $D message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-b1-1.act0A ( -- )   \ 003F8CD0
    self-wait-done
    2 ebit? not if
        $B message
        wait-message
        2 ebit-set
    else
        $C message
        wait-message
    then
    self-idle-or-end
;

: castle-b1-1.act0B ( -- )   \ 0047AA10
    self-wait-done
    $A message
    wait-message
    self-idle-or-end
;

: castle-b1-1.act0C ( -- )   \ 0047AA18
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: castle-b1-1.act0D ( -- )   \ 003F8CE8
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $1E self-wait-frames
    4 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-b1-1.act0F ( -- )   \ 003F8DB0
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    $FE char-busy? if
        4 ebit? not if
            $FE action-end
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act0E ( -- )   \ 003F8D00
    $18 state-flag-set
    8 ebit-clear
    1 self-scripted
    6 ebit-clear
    0 5 char-file-load
    self-wait-done
    0 char-file-use
    3 ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    4 ebit-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-b1-1.act0F goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                8 ebit? $FE char-here? not and if
                    8 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] castle-b1-1.act0F goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-b1-1.act11 ( -- )   \ 003F8E40
    self-wait-done
    $B ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and if
            1 3 self-move-slot
            self-wait-done
        then
    else
        $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and $FE 1 char-in-area? and if
            1 3 self-move-slot
            self-wait-done
        then
    then
    $B ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 6 char-file-load
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        4 ebit-set
        self-wait-anim
    else
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        $1601 self-anim
        self-wait-anim
        4 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-b1-1.act12 ( -- )   \ 003F8EE0
    self-wait-done
    -18.1 -8.1 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $245 story-flag-set
            2 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: castle-b1-1.act13 ( -- )   \ 003F8F40
    self-wait-done
    -13.0 10.0 self-turn-to-xz
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
            $281 story-flag-set
            3 effect-remove
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

: castle-b1-1.act14 ( -- )   \ 003F8FA0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
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

: castle-b1-1.act15 ( -- )   \ 003F9040
    self-wait-done
    3 3 $FF char-load
    3 char-unload
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $50 $FF movie-param
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

: castle-b1-1.act16 ( -- )   \ 003F90D0
    self-wait-done
    $C ebit? not if
        6 message
        wait-message
        $C ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-1.enter castle-b1-1 0 room-script!
' castle-b1-1.char-enter castle-b1-1 6 room-script!
' castle-b1-1.phase1 castle-b1-1 1 room-script!
' castle-b1-1.phase2 castle-b1-1 2 room-script!
' castle-b1-1.phase3 castle-b1-1 3 room-script!
' castle-b1-1.phase5 castle-b1-1 5 room-script!
' castle-b1-1.act00 castle-b1-1 $00 action-script!
' castle-b1-1.act01 castle-b1-1 $01 action-script!
' castle-b1-1.act02 castle-b1-1 $02 action-script!
' castle-b1-1.act03 castle-b1-1 $03 action-script!
' castle-b1-1.act04 castle-b1-1 $04 action-script!
' castle-b1-1.act05 castle-b1-1 $05 action-script!
' castle-b1-1.act06 castle-b1-1 $06 action-script!
' castle-b1-1.act07 castle-b1-1 $07 action-script!
' castle-b1-1.act08 castle-b1-1 $08 action-script!
' castle-b1-1.act09 castle-b1-1 $09 action-script!
' castle-b1-1.act0A castle-b1-1 $0A action-script!
' castle-b1-1.act0B castle-b1-1 $0B action-script!
' castle-b1-1.act0C castle-b1-1 $0C action-script!
' castle-b1-1.act0D castle-b1-1 $0D action-script!
' castle-b1-1.act0E castle-b1-1 $0E action-script!
' castle-b1-1.act0F castle-b1-1 $0F action-script!
' castle-b1-1.act10 castle-b1-1 $10 action-script!
' castle-b1-1.act11 castle-b1-1 $11 action-script!
' castle-b1-1.act12 castle-b1-1 $12 action-script!
' castle-b1-1.act13 castle-b1-1 $13 action-script!
' castle-b1-1.act14 castle-b1-1 $14 action-script!
' castle-b1-1.act15 castle-b1-1 $15 action-script!
' castle-b1-1.act16 castle-b1-1 $16 action-script!
