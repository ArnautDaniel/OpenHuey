\ story/rooms/castle-1f-4.fs - the event scripts of room castle-1f-4 ($F; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-4
USING: room-names story.words story.shared flag-names ;

\ room 0x0F: the floor plate ("fumi_yuka") fading: its +0x24 toward 1 while event variable 0 is
\ unset, toward 0 once set; byte 3 0 at once, else by 0.2 a step (var_fade).
: castle-1f-4.cmd00 ( b0 -- )  drop s" castle-1f-4.cmd00" stub-step ;
\ room 0x0F (Room0F_Cmd01_ptmf): three objects' +0x14 back to 0
: castle-1f-4.cmd01 ( -- )  s" castle-1f-4.cmd01" stub-step ;
\ room 0x0F (Room0F_Cmd02_ptmf): an effect (DriftingFlecks_vtable, 0x840 bytes) with its box
: castle-1f-4.cmd02 ( -- )  s" castle-1f-4.cmd02" stub-step ;
\ room 0x0F (Room0F_Cmd03_ptmf): the player's model +0xBC (1, 0.25) or (0, 0) by byte 3
: castle-1f-4.cmd03 ( b0 -- )  drop s" castle-1f-4.cmd03" stub-step ;
\ room 0x0F (Room0F_Cmd04_ptmf): the player's model +0xD0 (0, 1.5, -2 / -1.5 by byte 3) and
\ +0xCC
: castle-1f-4.cmd04 ( b0 -- )  drop s" castle-1f-4.cmd04" stub-step ;

: castle-1f-4.enter ( -- )   \ 003F5EC0
    room-sounds
    $11 story-flag? if
        0 0 $14 door-bits
        2 0 object-show
        3 0 object-show
        4 0 object-show
    else
        0 1 $14 door-bits
        2 0 object-show
        3 0 object-show
        4 0 object-show
    then
    7 door-locked? if
        1 0 $800008 nav-group
    then
    $80 exit-taken? if
        $13 story-flag-set
        0 1 var-set
        1 24 var-set
    else
        3 exit-taken? if
        else
            $310 story-flag-clear
            $13 story-flag-clear
        then
        $310 story-flag? if
            $13 story-flag-set
            0 1 var-set
            1 12 var-set
        else $13 story-flag? not if
            0 0 var-set
            1 0 var-set
        else
            0 1 var-set
            1 24 var-set
        then then
    then
    0 castle-1f-4.cmd00
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 effect-86
    $319 story-flag? not if
        1 1 $14 door-bits
        3 ebit-clear
    else
        2 1 $14 door-bits
        $239 story-flag? not if
            1 140.0 -9.0 94.0 flicker-sprite
        then
        3 ebit-set
    then
    $31F story-flag? not if
        4 1 $14 door-bits
    else
        4 0 $14 door-bits
    then
    8 door-locked? if
        5 0 $14 door-bits
    else
        5 1 $14 door-bits
    then
    3 0 var-set
    4 0 var-set
    1 $2300 sound-volume
;

: castle-1f-4.char-enter ( -- )   \ 003F5FC0
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
    0 self-is? if
        1 story-flag? 2 story-flag? not and if
            0.0 sound-volume-scale
            world-held state-flag-set
            0 0 0 action
            3 0 $14 door-bits
            7 0 object-show
            8 0 object-show
        else
            3 1 $14 door-bits
            7 1 object-show
            8 1 object-show
        then
        $80 exit-taken? if
            0 $221 -90 char-to-tri-facing
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            world-frozen state-flag-clear
            $311 story-flag-set
            0 0 $B action
        then
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
    then
;

: castle-1f-4.phase1 ( -- )   \ 003F6120
    6 sound-bank-loaded? if
        $11 story-flag? not if
            $C0000003 6 -72.0 10.0 15.0 0 0 sound
        else
            $C0000001 6 -72.0 10.0 15.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    3 story-flag? $31 story-flag? not and if
        1 exit-usable? if
            3 3 1 char-load
            1 exit-check
        then
    else 1 exit-usable? if
        1 exit-check
    then then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    3 story-flag? $31 story-flag? not and if
        0 6 char-entered-area? 0 9 char-left-area? or if
            3 0 char-remove
        then
    then
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-left-area? 0 6 char-entered-area? or if
        3 exit-prepare
    then
    0 7 char-left-area? 0 8 char-entered-area? or if
        3 story-flag? $31 story-flag? not and if
            3 3 1 char-load
        then
        1 exit-prepare
    then
    0 9 char-left-area? if
        2 exit-prepare
    then
    0 0 var? if
        1 $D char-in-area? if
            $13 story-flag-set
            0 1 var-set
            1 0 6 char-sound
            0 $F1 $D action
        else 0 $D char-in-area? if
            $13 story-flag-set
            0 1 var-set
            0 0 6 char-sound
            $11 story-flag? $311 story-flag? not and $14 story-flag? not and if
                0 0 $A action
            else
                0 $F1 $D action
            then
        else $FE $D char-in-area? if
            $13 story-flag-set
            0 1 var-set
            $FE 0 6 char-sound
            0 $F1 $D action
        then then then
    else 1 $D char-in-area? not 0 $D char-in-area? not and $FE $D char-in-area? not and if
        $13 story-flag-clear
        0 0 var-set
        0 $F2 $D action
    then then
    1 castle-1f-4.cmd00
    $11 story-flag? not if
        0 -28.0 0.0 15.0 $A 15 0 zone
        0 0 char-in-zone? if
            0 $F5 $1C action
        then
    then
    2 140.0 -10.0 94.0 6 5 0 zone
    $319 story-flag? not if
        6 ebit? not if
            0 2 char-in-zone? if
                2 2 var? if
                    0 $F4 $1B action
                else
                    0 $F3 8 action
                then
            then
        then
    then
    6 sound-bank-loaded? if
        4 var-inc
        4 30 var? if
            4 0 var-set
            3 0 var? if
                $40000005 6 -17.0 22.0 -9.0 0 0 sound
            else 3 1 var? if
                $40000006 6 -17.0 22.0 -9.0 0 0 sound
            else 3 2 var? if
                $40000007 6 -17.0 22.0 -9.0 0 0 sound
            else 3 3 var? if
                $40000008 6 -17.0 22.0 -9.0 0 0 sound
            then then then then
            3 var-inc
            3 4 var? if
                3 0 var-set
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    $319 story-flag? not if
        3 140.0 -10.0 94.0 $1E 20 0 zone
        4 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        4 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        5 ebit? not if
            5 -38.5 0.0 -65.0 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 5 9 char-zone-bits? if
                        5 ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            5 -38.5 0.0 -65.0 7 5 0 zone
            1 5 9 char-zone-bits? not if
                5 ebit-clear
            then
        then
    then
;

: castle-1f-4.phase2 ( -- )   \ 003F6420
    0 $A char-in-area? if
        -2147483644 scene-request? if
            0 0 6 action
        else $11 story-flag? not 0 -45 $2D char-heading? and if
            5 2 0 scene-change
        then then
    then
    0 $E char-in-area? 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    0 $10 char-in-area? 0 0 $2D char-heading? and if
        5 $E 0 scene-change
    then
    0 $D char-in-area? if
        5 4 0 scene-change
    then
    0 $F char-in-area? 0 -45 $32 char-heading? and if
        5 $84 3 scene-change
    then
    3 ebit? $239 story-flag? not and if
        $239 story-flag? not if
            2 1 5 5 0 zone-at-effect
            0 2 3 char-zone-bits? if
                5 $10 4 scene-change
            then
        then
    else 0 2 2 char-zone-bits? 0 45 $32 char-heading? and if
        5 9 0 scene-change
    then then
    -2147483646 scene-request? if
        0 2 char-group-bit4? if
            5 $13 1 scene-change
        then
    then
    0 $11 char-in-area? 0 -45 $32 char-heading? and if
        5 $15 0 scene-change
    then
    $31F story-flag? not if
        4 131.392 -30.0 147.223 5 5 0 zone
        0 4 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
;

: castle-1f-4.act00 ( -- )   \ 003F64F0
    $FE $F 443 2 stalker-to-room
    $FE $1BB 0 char-to-tri-facing
    $FE action-end
    $FE char-done
    3 char-unload
    4 action-end
    4 char-done
    6 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $11 action
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
    1 castle-1f-4.cmd03
    scene-locked state-flag-clear
    0 0 char-silent
    $31 room-preload
    fiona-calm-reset
    world-held state-flag-set
    $FE action-end
    $FE char-done
    3 0 char-remove
    $80 exit-check
    self-idle-or-end
;

: castle-1f-4.act01 ( -- )   \ 003F65A0
    0 counter-set
    1 self-scripted
    stalkers-stay state-flag-set
    self-wait-done
    0 item-use
    7 door-unlock
    $11 story-flag-set
    0 $F0 5 action
    1 wait-counter
    0 $1DD -4.507 15.509 -90 char-to-xz
    0 self-move-16
    camera-restart
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-4.act02 ( -- )   \ 003F65E0
    self-wait-done
    $1FD -16.0 14.0 -90 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 -10.0 0.0 0.0 event-camera
    0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $10 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    $228 item-give
    self-idle-or-end
;

: castle-1f-4.act03 ( -- )   \ 003F6640
    self-wait-done
    2 ebit? not if
        $16 message
        wait-message
        $C1 0.0 -84.0 0 $FFFF 5 self-move-to
        self-wait-done
        $18 message
        wait-message
        2 ebit-set
    else
        $17 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act04 ( -- )   \ 003F6670
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    self-wait-16
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    $15 message
    wait-message
    self-idle-or-end
;

: castle-1f-4.act05 ( -- )   \ 003F6690
    $F $54 fade
    5 0 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    0 0 $14 door-bits
    2 0 object-show
    3 0 object-show
    4 0 object-show
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 $F9 $18 action
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
    1 castle-1f-4.cmd04
    0 0 $800008 nav-group
    castle-1f-4.cmd01
    1 0 char-visible
    1 action-end
    counter-inc
    $F $51 fade
    wait-fade
    self-idle-or-end
;

: castle-1f-4.act06 ( -- )   \ 003F6750
    begin
        0 char-busy? not while
        yield
    repeat
    $1203 0 self-anim-blend
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    1 ebit? not if
        $11 message
        1 ebit-set
    else
        $12 message
    then
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: castle-1f-4.act07 ( -- )   \ 003F6780
    self-wait-done
    self-frames-reset
    $19 self-wait-frames
    $5A threat-raise
    1 $FF 8 rumble
    0 $43 5 char-sound
    $F00 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: castle-1f-4.act08 ( -- )   \ 003F67A0
    1 $FF 4 rumble
    6 ebit-set
    0 9 6 char-sound
    0 139.0 -9.0 91.5 0 0 0 0 dust
    0 139.0 -9.0 94.5 0 0 0 0 dust
    begin
        fiona-free? not while
        yield
    repeat
    2 var-inc
    6 ebit-clear
    self-idle-or-end
;

: castle-1f-4.act09 ( -- )   \ 003F67E0
    self-wait-done
    $319 story-flag? not if
        $13 message
        wait-message
    else
        $14 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act0A ( -- )   \ 003F67F0
    stalkers-stay state-flag-set
    1 self-scripted
    $15 room-preload
    self-wait-done
    $F 0 fade
    wait-fade
    world-held state-flag-set
    scene-locked state-flag-set
    world-frozen state-flag-set
    subscreen-locked state-flag-set
    $FF panic-stage? if
        3 panic-stage
    then
    panic-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-1f-4.act0B ( -- )   \ 003F6820
    1 self-scripted
    self-wait-done
    world-held state-flag-clear
    camera-restart
    1 0 self-anim-blend
    panic-held state-flag-clear
    $F 1 fade
    wait-fade
    1 message
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    scene-locked state-flag-clear
    subscreen-locked state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-4.act0C ( -- )   \ 003F6850
    self-wait-done
    1 ebit? not if
        $11 message
        wait-message
        1 ebit-set
    else
        $12 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act0D ( -- )   \ 003F6870
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $310 story-flag-set
    $11 story-flag? not if
        $40000003 6 -72.0 10.0 15.0 0 0 sound
    else
        $40000001 6 -72.0 10.0 15.0 0 0 sound
    then
    $13 story-flag? not if
        begin
            1 0 var? not $13 story-flag? not and while
            1 var-dec
            self-frames-reset
            1 self-wait-frames
        repeat
        $13 story-flag? not if
            $11 story-flag? not if
                3 6 sound-stop
                $40000004 6 -72.0 10.0 15.0 0 0 sound
            else
                1 6 sound-stop
                $40000002 6 -72.0 10.0 15.0 0 0 sound
            then
            $310 story-flag-clear
        then
    else
        begin
            1 24 var? not $13 story-flag? and while
            1 var-inc
            self-frames-reset
            3 self-wait-frames
        repeat
        $13 story-flag? if
            $11 story-flag? not if
                3 6 sound-stop
                $40000004 6 -72.0 10.0 15.0 0 0 sound
            else
                1 6 sound-stop
                $40000002 6 -72.0 10.0 15.0 0 0 sound
            then
            $310 story-flag-clear
        then
    then
    self-idle-or-end
;

: castle-1f-4.act0E ( -- )   \ 003F6960
    self-wait-done
    65.0 107.0 self-turn-to-xz
    self-wait-done
    2 ebit? if
        2 message
        wait-message
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act0F ( -- )   \ 003F6980
    self-frames-reset
    $29 self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    self-frames-reset
    $2E self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    self-frames-reset
    $25 self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    $239 story-flag? not if
        1 140.0 -9.0 94.0 flicker-sprite
    then
    3 ebit-set
    self-idle-or-end
;

: castle-1f-4.act10 ( -- )   \ 003F69E0
    self-wait-done
    140.0 94.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $239 story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: castle-1f-4.act11 ( -- )   \ 003F6A40
    begin
        $F cutscene-shot? not while
        yield
    repeat
    $FE 1 char-no-shadow
    begin
        $10 cutscene-shot? not while
        yield
    repeat
    $FE 0 char-no-shadow
    begin
        $17 cutscene-shot? if
            0 castle-1f-4.cmd03
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-1f-4.act12 ( -- )   \ 003F6A70
    self-wait-done
    131.392 147.223 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $31F story-flag-set
    4 0 $14 door-bits
    $12 message-param-room
    $12 1 item-give-count
    0 $12 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-1f-4.act13 ( -- )   \ 003F6AC0
    self-wait-done
    2 self-through-exit
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60B self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $609 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        6 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act14 ( -- )   \ 003F6AF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    0 $C 6 char-sound
    5 1 $14 door-bits
    self-frames-reset
    $1E self-wait-frames
    0 $72 5 char-sound
    8 door-unlock
    $903 self-anim
    130.0 -20.0 170.0 self-look-at-point
    yield
    self-frames-reset
    $3C self-wait-frames
    $FF self-look-at
    yield
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-4.act15 ( -- )   \ 003F6B50
    self-wait-done
    $A6 120.0 154.0 -90 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 15.0 -10.0 0.0 0.0 event-camera
    8 door-locked? if
        4 message
        wait-message
    else
        5 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-4.act16 ( -- )   \ 003F6BB0
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    7 message
    wait-message
    self-idle-or-end
;

: castle-1f-4.act17 ( -- )   \ 003F6BE0
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    0 $C 6 char-sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    8 message
    wait-message
    self-idle-or-end
;

: castle-1f-4.act18 ( -- )   \ 003F6C10
    0 castle-1f-4.cmd04
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 castle-1f-4.cmd04
    self-idle-or-end
;

: castle-1f-4.act19 ( -- )   \ 003F6C20
    self-wait-done
    0 1 $14 door-bits
    2 0 object-show
    3 0 object-show
    4 0 object-show
    1 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 22.8 14.65 68.75 0 effect-86
    4 3 $FF char-load
    3 char-unload
    2 partner-load
    2 char-unload
    6 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $11 action
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
    1 castle-1f-4.cmd03
    2 0 char-remove
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-4.act1A ( -- )   \ 003F6CF0
    self-wait-done
    5 0 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 0 $14 door-bits
    2 0 object-show
    3 0 object-show
    4 0 object-show
    0 22.8 14.65 68.75 0 effect-86
    0 $F9 $18 action
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
    1 castle-1f-4.cmd04
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-4.act1B ( -- )   \ 003F6DA0
    1 $FF 4 rumble
    $319 story-flag-set
    0 9 6 char-sound
    $4000000A 6 140.0 -10.0 92.0 0 0 sound
    1 0 $14 door-bits
    2 1 $14 door-bits
    0 139.0 -9.0 91.5 0 0 0 0 dust
    0 139.0 -9.0 94.5 0 0 0 0 dust
    0 2 0.7 0.5 0.3 0.125 zone-rect
    137.0 -10.0 90.0 0 -2145378272 1 0.0 scene-effect-8C
    137.0 -10.0 95.0 0 -2145378272 1 0.0 scene-effect-8C
    castle-1f-4.cmd02
    0 $F1 $F action
    begin
        fiona-free? not while
        yield
    repeat
    0 0 7 action
    self-idle-or-end
;

: castle-1f-4.act1C ( -- )   \ 003F6E50
    0 $91 5 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    0 0 $C action
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-4.enter castle-1f-4 0 room-script!
' castle-1f-4.char-enter castle-1f-4 6 room-script!
' castle-1f-4.phase1 castle-1f-4 1 room-script!
' castle-1f-4.phase2 castle-1f-4 2 room-script!
' castle-1f-4.act00 castle-1f-4 $00 action-script!
' castle-1f-4.act01 castle-1f-4 $01 action-script!
' castle-1f-4.act02 castle-1f-4 $02 action-script!
' castle-1f-4.act03 castle-1f-4 $03 action-script!
' castle-1f-4.act04 castle-1f-4 $04 action-script!
' castle-1f-4.act05 castle-1f-4 $05 action-script!
' castle-1f-4.act06 castle-1f-4 $06 action-script!
' castle-1f-4.act07 castle-1f-4 $07 action-script!
' castle-1f-4.act08 castle-1f-4 $08 action-script!
' castle-1f-4.act09 castle-1f-4 $09 action-script!
' castle-1f-4.act0A castle-1f-4 $0A action-script!
' castle-1f-4.act0B castle-1f-4 $0B action-script!
' castle-1f-4.act0C castle-1f-4 $0C action-script!
' castle-1f-4.act0D castle-1f-4 $0D action-script!
' castle-1f-4.act0E castle-1f-4 $0E action-script!
' castle-1f-4.act0F castle-1f-4 $0F action-script!
' castle-1f-4.act10 castle-1f-4 $10 action-script!
' castle-1f-4.act11 castle-1f-4 $11 action-script!
' castle-1f-4.act12 castle-1f-4 $12 action-script!
' castle-1f-4.act13 castle-1f-4 $13 action-script!
' castle-1f-4.act14 castle-1f-4 $14 action-script!
' castle-1f-4.act15 castle-1f-4 $15 action-script!
' castle-1f-4.act16 castle-1f-4 $16 action-script!
' castle-1f-4.act17 castle-1f-4 $17 action-script!
' castle-1f-4.act18 castle-1f-4 $18 action-script!
' castle-1f-4.act19 castle-1f-4 $19 action-script!
' castle-1f-4.act1A castle-1f-4 $1A action-script!
' castle-1f-4.act1B castle-1f-4 $1B action-script!
' castle-1f-4.act1C castle-1f-4 $1C action-script!
