\ story/rooms/castle-1f-13.fs - the event scripts of room castle-1f-13 ($8; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-13
USING: room-names story.words story.shared ;

\ room 0x08 (Room08_Cmd00_ptmf): the cutscene director's +0x6C 3 (byte 3 0) or 2
: castle-1f-13.cmd00 ( b0 -- )  drop s" castle-1f-13.cmd00" stub-step ;
\ room 0x08 (D_003F31D8): the hanging object named by the handler's string 0xA - byte 3 0 sets
\ it still (+0x30 / +0x38 0, travel +0x3C 0.9); 1: the player's travel (+0x3C, its last move's
\ length) past 5 makes it creak (sounds 4 / 5 by turns, event bit 7) and swing for 20 frames:
\ its tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 a frame
: castle-1f-13.cmd01 ( b0 -- )  drop s" castle-1f-13.cmd01" stub-step ;
\ door be16 cmd[3..4]: Progress_DoorOpen
: castle-1f-13.cond00? ( b0 b1 -- flag )  drop drop s" castle-1f-13.cond00?" stub-flag ;
\ room 0x08 (Room08_Cond01_ptmf): the stalker is there but not about
: castle-1f-13.cond01? ( -- flag )  s" castle-1f-13.cond01?" stub-flag ;

: castle-1f-13.enter ( -- )   \ 003F2500
    0 $35 castle-1f-13.cond00? if
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $E story-flag? if
        $2D story-flag? not if
            $2D story-flag-set
            8 0 248 4 15 -1 0 0.0 creature-place
        then
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        9 1 object-show
        room-sounds
        $1A 1.0 0 bgm
    else
        4 0 object-show
        5 0 object-show
        6 0 object-show
        7 0 object-show
        8 0 object-show
        9 0 object-show
    then
    7 story-flag? if
        $38 story-flag? not if
            $38 story-flag-set
            8 0 274 4 15 -1 0 0.0 creature-place
            8 0 448 4 15 -1 0 0.0 creature-place
        then
    then
    $31B story-flag? if
        castle-1f-13.cond01? 0 state-flag? and if
            $FE char-activate
            $FE 8 376 2 stalker-to-room
            $FE 4 -1 char-camera
            $FE char-full-health
            $FE 0 stalker-mode
            $FE 0 stalker-search-delay
            $FE $178 120 char-to-tri-facing
            stalker-item-cooldown
        then
        $31B story-flag-clear
    then
    $250 story-flag? $251 story-flag? not and if
        0 -254.5 1.0 -95.0 flicker-sprite
    then
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    0 castle-1f-13.cmd01
    $294 story-flag? not if
        1 -283.73 1.0 2.22 flicker-sprite
    then
;

: castle-1f-13.act09 ( -- )   \ 003F2E60
    $FE camera-follow
    4 ebit-clear
    $B 0 pvar? if
        0 chance? if
            4 ebit-set
        then
    else $B 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $B 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $B 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    2 creature-action? if
        4 ebit-set
    then
    4 stalker-alert? if
        4 ebit-clear
    then
    4 ebit? if
        0 $FE $A action
    else
        $78 1 item-cooldown
    then
    $B pvar-inc
    exit
;

: castle-1f-13.char-enter ( -- )   \ 003F2640
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 4 -1 char-camera
                0 camera-follow
            else
                1 4 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then then
    0 4 -1 area-camera
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    2 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    3 3 3 area-camera
    0 self-is? if
        $80 exit-taken? $81 exit-taken? or $82 exit-taken? or if
            0 $10E 70 char-to-tri-facing
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            $81 exit-taken? if
                0 0 6 action
            else
                0 0 0 action
            then
        then
    then
    $FE self-is? if
        9 state-flag? if
            5 ebit-set
            castle-1f-13.act09
        else
            5 ebit-clear
        then
    then
;

: castle-1f-13.phase1 ( -- )   \ 003F2780
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 1 1 1 chars-area-camera
    $F 0 0 1 chars-area-camera
    $14 0 0 1 chars-area-camera
    $15 3 3 1 chars-area-camera
    $16 0 0 1 chars-area-camera
    $17 2 2 1 chars-area-camera
    $18 0 0 1 chars-area-camera
    $19 4 -1 1 chars-area-camera
    $1A 0 0 1 chars-area-camera
    $1B 4 -1 1 chars-area-camera
    $1C 0 0 1 chars-area-camera
    $1D 4 -1 1 chars-area-camera
    0 6 char-entered-area? 0 $A char-entered-area? or if
        0 exit-prepare
    then
    0 7 char-entered-area? 0 8 char-entered-area? or if
        1 exit-prepare
    then
    0 9 char-entered-area? if
        2 exit-prepare
    then
    0 $B char-entered-area? if
        3 exit-prepare
    then
    0 -277.0 0.0 -142.0 6 10 0 zone
    0 0 8 char-zone-bits? if
        1 castle-1f-13.cmd01
    then
    $250 story-flag? not if
        1 -254.5 0.0 -95.0 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 592 var-set
                $1A 593 var-set
                $1B 0 var-set
                $1C -254500 var-set
                $1D 1000 var-set
                $1E -95000 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
;

: castle-1f-13.phase2 ( -- )   \ 003F28A0
    $294 story-flag? not if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $D 4 scene-change
        then
    then
    0 $13 char-in-area? if
        $FE char-here? not if
            5 7 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 $10 $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 $11 char-in-area? 0 45 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 $12 char-in-area? 0 45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $250 story-flag? $251 story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $C 4 scene-change
        then
    then
;

: castle-1f-13.phase3 ( -- )   \ 003F2910
    -219.9 59.3 20.8 -249.2 59.3 -7.4 -194.2 59.3 -5.9 -223.5 59.3 -34.2 lights-doorway
    -180.0 20.0 -119.9 -200.0 20.0 -119.9 -180.0 0.0 -119.9 -200.0 0.0 -119.9 lights-doorway
;

: castle-1f-13.phase5 ( -- )   \ 003F2980
    $FE char-busy? 6 ebit? and if
        $FE $1C0 -275.75 41.09 -60 char-to-xz
        $FE 0 0 char-camera
        6 ebit-clear
    then
;

: castle-1f-13.act00 ( -- )   \ 003F29A0
    $E story-flag? not if
        1 self-scripted
        1 char-here? if
            0 1 4 action-force
        then
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
        1 1 char-visible
        1 castle-1f-13.cmd00
        $1E $C8 movie-param
        3 char-unload
        4 3 char-hand-over
        room-sounds
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
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        9 1 object-show
        0 $1D0 -80 char-to-tri-facing
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
        camera-restart
        begin
            6 sound-bank-loaded? not while
            yield
        repeat
        $FE 8 383 2 stalker-to-room
        $FE -1 -1 char-camera
        $FE char-activate
        stalker-item-cooldown
        0 $FE 5 action
        1 char-here? if
            $2F 0 -1 hewie-to-room
        then
        $B01 0 self-anim-blend
        3 0 char-remove
        4 0 char-remove
        $1A 1.0 0 bgm
        $F $41 fade
        $B02 self-anim
        self-wait-anim
        $E story-flag-set
        $2D story-flag-set
        8 0 248 4 15 -1 0 0.0 creature-place
        wait-fade
        $21 resident-flag-set
        $227 item-give
        0 self-scripted
    else
        self-wait-done
        camera-restart
        $F 1 fade
        wait-fade
        $12 state-flag-clear
    then
    $18 state-flag-clear
    0 self-scripted
    1 exit-prepare
    self-idle-or-end
;

: castle-1f-13.act01 ( -- )   \ 003F2AE0
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: castle-1f-13.act02 ( -- )   \ 003F2B00
    1 ebit? not if
        2 message
        wait-message
        $902 self-anim
        self-wait-anim
        $903 self-anim
        self-wait-anim
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-13.act03 ( -- )   \ 003F2B20
    2 ebit? not if
        2 message
        wait-message
        $902 self-anim
        self-wait-anim
        $903 self-anim
        self-wait-anim
        3 message
        wait-message
        2 ebit-set
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-13.act04 ( -- )   \ 0047A9AC
    begin
        yield
    again
;

: castle-1f-13.act05 ( -- )   \ 003F2B40
    6 ebit-set
    self-wait-done
    $FE $B char-file-load
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    yield
    $FE -219.01 60.17 14.42 -60 char-to-xyz
    1 self-noclip
    1 self-scripted
    $FE 0 char-visible
    $FE 0 char-silent
    0 7 -147.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -155.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -163.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -171.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -179.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -187.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -195.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -203.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -211.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -219.0 60.0 14.5 0 0 sound
    $8000 5 self-anim-9
    0 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        0 60 var? if
            0 -283.0 1.0 37.0 0 0 0 0 dust
            0 -280.0 1.0 42.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        0 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $FE 0 0 char-camera
    $FE char-full-health
    $FE 2 stalker-mode
    $FE 0 stalker-search-delay
    stalker-item-cooldown
    6 ebit-clear
    self-idle-or-end
;

: castle-1f-13.act06 ( -- )   \ 003F2CC0
    yield
    camera-restart
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
    $1E $C8 movie-param
    $2F room-preload
    3 char-unload
    4 3 char-hand-over
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
    $FE action-end
    $FE char-done
    8 state-flag-set
    3 0 char-remove
    4 0 char-remove
    $20 resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-1f-13.act08 ( -- )   \ 003F2E20
    3 ebit? if
        2 avoid-prompt
    then
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    9 state-flag-clear
    0 camera-follow
    $18 state-flag-set
    $114 -258.97 -140.95 79 $207 5 self-move-to
    self-wait-done
    4 $14 self-anim-blend
    self-wait-anim
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    self-idle-or-end
;

: castle-1f-13.act07 ( -- )   \ 003F2D60
    $18 state-flag-set
    5 ebit-clear
    1 self-scripted
    3 ebit-clear
    self-wait-done
    $1D4 -267.0 -140.0 -110 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    $800 self-anim
    self-wait-anim
    $122 -291.573 -146.706 -110 $803 5 self-move-to
    self-wait-done
    $122 -291.573 -146.706 80 $803 5 self-move-to
    self-wait-done
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-1f-13.act08 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? $FE char-here? not and if
                    5 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            9 state-flag-clear
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] castle-1f-13.act08 goto
            then
            $18 state-flag-set
            $1D4 -267.0 -140.0 70 $803 5 self-move-to
            self-wait-done
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            self-idle-or-end
        then
    again
;

: castle-1f-13.act0A ( -- )   \ 003F2EB0
    self-wait-done
    $1A1 -238.675 -139.193 -90 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    3 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-1f-13.act0B ( -- )   \ 0047A9B0
    self-idle-or-end
;

: castle-1f-13.act0C ( -- )   \ 003F2EE0
    self-wait-done
    -254.5 -95.0 self-turn-to-xz
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
            $251 story-flag-set
            0 effect-remove
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

: castle-1f-13.act0D ( -- )   \ 003F2F40
    self-wait-done
    -283.73 2.22 self-turn-to-xz
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
            $294 story-flag-set
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

: castle-1f-13.act0E ( -- )   \ 003F2FA0
    self-wait-done
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    $E 3 $FF char-load
    $F 4 char-load-2
    2 5 $FF char-load
    3 char-unload
    4 3 char-hand-over
    5 char-unload
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
    4 0 char-remove
    5 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-13.act0F ( -- )   \ 003F3090
    self-wait-done
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    $E 3 $FF char-load
    $F 4 char-load-2
    2 5 $FF char-load
    3 char-unload
    4 3 char-hand-over
    5 char-unload
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
    $1E $C8 movie-param
    1 castle-1f-13.cmd00
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
    4 0 char-remove
    5 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-13.enter castle-1f-13 0 room-script!
' castle-1f-13.char-enter castle-1f-13 6 room-script!
' castle-1f-13.phase1 castle-1f-13 1 room-script!
' castle-1f-13.phase2 castle-1f-13 2 room-script!
' castle-1f-13.phase3 castle-1f-13 3 room-script!
' castle-1f-13.phase5 castle-1f-13 5 room-script!
' castle-1f-13.act00 castle-1f-13 $00 action-script!
' castle-1f-13.act01 castle-1f-13 $01 action-script!
' castle-1f-13.act02 castle-1f-13 $02 action-script!
' castle-1f-13.act03 castle-1f-13 $03 action-script!
' castle-1f-13.act04 castle-1f-13 $04 action-script!
' castle-1f-13.act05 castle-1f-13 $05 action-script!
' castle-1f-13.act06 castle-1f-13 $06 action-script!
' castle-1f-13.act07 castle-1f-13 $07 action-script!
' castle-1f-13.act08 castle-1f-13 $08 action-script!
' castle-1f-13.act09 castle-1f-13 $09 action-script!
' castle-1f-13.act0A castle-1f-13 $0A action-script!
' castle-1f-13.act0B castle-1f-13 $0B action-script!
' castle-1f-13.act0C castle-1f-13 $0C action-script!
' castle-1f-13.act0D castle-1f-13 $0D action-script!
' castle-1f-13.act0E castle-1f-13 $0E action-script!
' castle-1f-13.act0F castle-1f-13 $0F action-script!
