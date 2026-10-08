\ story/rooms/castle-2f-5.fs - the event scripts of room castle-2f-5 ($32; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-5
USING: room-names story.words story.shared ;

\ room 0x32: room effect slot 1 made anew as the first TV screen (TvScreenA).
: castle-2f-5.cmd00 ( -- )  s" castle-2f-5.cmd00" stub-step ;
\ room 0x32 (D_0042C2A8): the fan turns, except while a movie plays
: castle-2f-5.cmd01 ( -- )  s" castle-2f-5.cmd01" stub-step ;
\ room 0x32 (D_0042C2B8)
: castle-2f-5.cmd02 ( b0 -- )  drop s" castle-2f-5.cmd02" stub-step ;
\ a room callback: byte 3 0 a progress name, 1 wait for character 3 (2 while not), else done
: castle-2f-5.cmd03 ( b0 -- )  drop s" castle-2f-5.cmd03" stub-step ;
\ room 0x32 (D_0042C2D8): byte 3 0..3 the lit quad (room effect 0x1A) as room 0x21's; 4 and up
\ the mirror fragment's reflection (room effect 0x1A, MirrorFragment_vtable) on the object
\ "a_fragment0": 1.8 across, -0.1 down, strength 1, kind 2, alpha 0xFF
: castle-2f-5.cmd04 ( b0 -- )  drop s" castle-2f-5.cmd04" stub-step ;
\ the player's model tint: white (byte 3 0) or blue halved
: castle-2f-5.cmd05 ( b0 -- )  drop s" castle-2f-5.cmd05" stub-step ;

: castle-2f-5.enter ( -- )   \ 0042B0B0
    room-sounds
    castle-2f-5.cmd00
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
        1 noise-level
    then
    0 $F1 $12 action
    $25E story-flag? $25F story-flag? not and if
        2 -9.0 1.0 -11.0 flicker-sprite
    then
    0 0 var-set
    1 0 var-set
    0 castle-2f-5.cmd02
    0 castle-2f-5.cmd04
    $42 story-flag? not if
        1 1 $14 door-bits
        2 0 $14 door-bits
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
    else
        1 0 $14 door-bits
        2 0 $14 door-bits
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 1 $14 door-bits
        6 1 $14 door-bits
    then
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
;

: castle-2f-5.act04 ( -- )   \ 0042B850
    5 ebit-clear
    2 0 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 1 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 2 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else 2 3 pvar? if
        $32 chance? if
            5 ebit-set
        then
    then then then then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 $FE $B action
    else
        $78 1 item-cooldown
    then
    2 pvar-inc
    exit
;

: castle-2f-5.char-enter ( -- )   \ 0042B160
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
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
    0 self-is? if
    then
    1 self-is? if
    then
    $FE self-is? if
        9 state-flag? if
            $B ebit-set
            $300 story-flag? if
                castle-2f-5.act04
            else
                castle-2f-5.act04
            then
        else
            $B ebit-clear
        then
    then
;

: castle-2f-5.phase1 ( -- )   \ 0042B200
    3 ebit? not if
        6 sound-bank-loaded? if
            $300 story-flag? if
                $40000001 6 0.0 3.0 7.0 0 0 sound
            then
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            3 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000001 6 0.0 3.0 7.0 0 0 sound
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
        0 $8000000C 6 char-sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    3 0 0 1 chars-area-camera
    8 1 -1 1 chars-area-camera
    9 2 -1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
    then
    $41 story-flag? $42 story-flag? not and if
        1 2 pad? 2 2 pad? or 3 2 pad? or 0 control-action? or 1 control-action? or 2 control-action? or 3 control-action? or 4 control-action? or fiona-free? and if
            0 0 $19 action
        then
        99.0 fiona-fear
    then
    $25E story-flag? not if
        7 -9.0 0.0 -11.0 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 606 var-set
                $1A 607 var-set
                $1B 2 var-set
                $1C -9000 var-set
                $1D 1000 var-set
                $1E -11000 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
    0 -1.41 -8.0 8.99 $D 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 -46.87 0.0 -38.5 $14 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    2 -39.23 0.0 -59.47 $14 6 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    3 -37.71 -8.0 63.24 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 46.35 0.0 -40.12 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    5 39.18 0.0 -19.5 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    6 -0.22 -8.0 59.41 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-5.phase2 ( -- )   \ 0042B520
    0 4 char-in-area? 0 90 $5A char-heading? and if
        5 0 0 scene-change
    then
    0 5 char-in-area? 0 90 $3C char-heading? and if
        $41 story-flag? $42 story-flag? not and if
            5 $1A 0 scene-change
        else $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    0 $B char-in-area? 0 -67 $3C char-heading? and if
        5 $14 0 scene-change
    then
    0 $C char-in-area? 0 45 $3C char-heading? and if
        5 $15 0 scene-change
    then
    0 $11 char-in-area? 0 0 $3C char-heading? and if
        5 $17 0 scene-change
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        5 $18 0 scene-change
    then
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        5 $1C 0 scene-change
    then
    0 $13 $32 char-faces-area? if
        5 $1D 0 scene-change
    then
    0 $14 char-in-area? 0 -45 $32 char-heading? and if
        5 $1E 0 scene-change
    then
    0 $15 char-in-area? 0 90 $32 char-heading? and if
        $AE story-flag? not if
            5 $1F 0 scene-change
        else
            5 $20 0 scene-change
        then
    then
    0 $D char-in-area? 0 22 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 $16 char-in-area? 0 22 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 7 char-in-area? 0 -45 $32 char-heading? and if
        5 $84 3 scene-change
    then
    $41 story-flag? $42 story-flag? not and if
        -2147483646 scene-request? 2 scene-request? or if
            5 $19 1 scene-change
        then
    then
    $25E story-flag? $25F story-flag? not and if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            $41 story-flag? $42 story-flag? not and if
                5 $19 4 scene-change
            else
                5 $A 4 scene-change
            then
        then
    then
;

: castle-2f-5.act00 ( -- )   \ 0042B640
    self-wait-done
    $6A -1.0 13.5 160 $FFFF 5 self-move-to
    self-wait-done
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $306 story-flag? not if
        $306 story-flag-set
        $29 message
        wait-message
    then
    $902 self-anim
    self-wait-anim
    5 0 pvar? 5 1 pvar? or if
        $300 story-flag? not if
            $300 story-flag-set
            0 0 6 char-sound
            $40000001 6 0.0 3.0 7.0 0 0 sound
            $40 $6A noise
            1 noise-level
        else
            $300 story-flag-clear
            0 2 6 char-sound
            1 6 sound-stop
            0 noise-level
        then
        $903 self-anim
        self-wait-anim
    else
        $903 self-anim
        self-wait-anim
        $28 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act03 ( -- )   \ 0042B820
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-2f-5.act01 ( -- )   \ 0042B6E0
    $18 state-flag-set
    $B ebit-clear
    1 self-scripted
    0 counter-set
    6 ebit-clear
    7 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 50 chars-within? and if
        0 1 2 action-force
        6 ebit-set
    then
    0 8 char-file-load
    self-wait-done
    6 ebit? if
        $27 14.5 -37.5 180 $FFFF 5 self-move-to
        self-wait-done
        0 char-file-use
        1 self-look-at
        yield
        begin
            7 ebit? not while
            yield
        repeat
        counter-inc
        1 self-noclip
        $FF self-look-at
        yield
        $8000 5 self-anim-9
        self-frames-reset
        $1C self-wait-frames
        0 $7E 5 char-sound
        self-wait-anim
    else
        0 char-file-use
        $27 $8004 5 14.5 -37.5 180 self-walk-anim
        self-wait-done
        counter-inc
        1 self-noclip
        0 $7E 5 char-sound
        $8005 $A self-anim-9
        self-wait-anim
    then
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    4 ebit-clear
    0 avoid-prompt
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-2f-5.act03 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                $B ebit? $FE char-here? not and if
                    $B ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            counter-inc
            $FE char-here? if
                ['] castle-2f-5.act03 goto
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

: castle-2f-5.act02 ( -- )   \ 0042B7D0
    1 self-scripted
    1 9 char-file-load
    self-wait-done
    $88 18.5 -37.5 180 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    7 ebit-set
    1 wait-counter
    1 self-noclip
    $8000 5 self-anim-blend
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                $8002 5 self-anim-9
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-5.act05 ( -- )   \ 0047AD78
    self-wait-done
    -1 self-move-16
    self-wait-anim
    self-idle
    self-wait-done
    self-idle-or-end
;

: castle-2f-5.act06 ( -- )   \ 0042B8A0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $6A -4.0 13.0 135 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    $FE 2 6 char-sound
    1 6 sound-stop
    0 noise-level
    $300 story-flag-clear
    5 pvar-inc
    $FE 3 stalker-mode
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: castle-2f-5.act07 ( -- )   \ 0042B8F0
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $31 message
    wait-message
    self-idle-or-end
;

: castle-2f-5.act08 ( -- )   \ 0042B900
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $42 story-flag? not if
        $31 message
        wait-message
    else
        $32 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-5.act09 ( -- )   \ 0042B920
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    1 castle-2f-5.cmd05
    begin
        2 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 castle-2f-5.cmd05
    begin
        $A cutscene-shot? not while
        yield
    repeat
    2 castle-2f-5.cmd04
    begin
        $A03 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 1 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    begin
        $B cutscene-shot? not while
        yield
    repeat
    0 castle-2f-5.cmd04
    begin
        $A67 cutscene-cue-reached? not while
        yield
    repeat
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    begin
        $A70 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    begin
        $A89 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 1 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    begin
        $AB7 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    $E 0 object-show
    $F 0 object-show
    $10 0 object-show
    $11 0 object-show
    $12 0 object-show
    $13 0 object-show
    $14 0 object-show
    $15 0 object-show
    $16 0 object-show
    $17 0 object-show
    $18 0 object-show
    begin
        $AE7 cutscene-cue-reached? not while
        yield
    repeat
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    2 castle-2f-5.cmd04
    begin
        $BF3 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 0 $14 door-bits
    0 castle-2f-5.cmd04
    begin
        $CD2 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    begin
        $10 cutscene-shot? not while
        4 castle-2f-5.cmd04
        yield
    repeat
    0 castle-2f-5.cmd04
    begin
        $11 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    1 castle-2f-5.cmd03
    self-idle-or-end
;

: castle-2f-5.act0A ( -- )   \ 0042BB10
    self-wait-done
    -9.0 -11.0 self-turn-to-xz
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
            $25F story-flag-set
            2 effect-remove
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

: castle-2f-5.act0B ( -- )   \ 0042BB70
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE $A char-file-load
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        $FE char-file-use
        4 $FE 1 char-model-op
        $8000 self-anim
        self-wait-anim
        4 $FE 0 char-model-op
    else
        $1601 self-anim
        self-wait-anim
    then
    4 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-2f-5.act0C ( -- )   \ 0047AD80
    self-idle-or-end
;

: castle-2f-5.act0D ( -- )   \ 0047AD84
    self-idle-or-end
;

: castle-2f-5.act0E ( -- )   \ 0047AD88
    self-idle-or-end
;

: castle-2f-5.act0F ( -- )   \ 0047AD8C
    self-idle-or-end
;

: castle-2f-5.act10 ( -- )   \ 0047AD90
    self-idle-or-end
;

: castle-2f-5.act11 ( -- )   \ 0047AD94
    self-idle-or-end
;

: castle-2f-5.act12 ( -- )   \ 0047AD98
    begin
        castle-2f-5.cmd01
        yield
    again
;

: castle-2f-5.act13 ( -- )   \ 0047ADA0
    self-idle-or-end
;

: castle-2f-5.act14 ( -- )   \ 0042BBD0
    self-wait-done
    -40.0 -60.0 self-turn-to-xz
    self-wait-done
    $23 message
    wait-message
    self-idle-or-end
;

: castle-2f-5.act15 ( -- )   \ 0042BBE0
    self-wait-done
    $B5 43.571 -39.929 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $312 story-flag-set
    0 ebit? not if
        $24 message
        wait-message
        0 ebit-set
    else
        $25 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act16 ( -- )   \ 0047ADA4
    self-idle-or-end
;

: castle-2f-5.act17 ( -- )   \ 0042BC40
    self-wait-done
    $9F -37.0 63.0 0 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $314 story-flag-set
    1 ebit? not if
        $2D message
        wait-message
        1 ebit-set
    else $312 story-flag? $313 story-flag? or if
        $2E message
        wait-message
    else
        $2D message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act18 ( -- )   \ 0042BCB0
    self-wait-done
    9 -40.418 -39.428 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $313 story-flag-set
    2 ebit? not if
        $2B message
        wait-message
        2 ebit-set
    else $312 story-flag? $314 story-flag? and if
        $2C message
        wait-message
    else
        $2B message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act19 ( -- )   \ 0042BD20
    self-wait-done
    $100E self-anim
    0 $C 6 char-sound
    self-frames-reset
    $14 self-wait-frames
    8 ebit? not if
        $2F message
        8 ebit-set
    else
        $30 message
        8 ebit-clear
    then
    2 $A self-anim-blend
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-5.act1A ( -- )   \ 0042BD50
    self-wait-done
    $21 message
    wait-message
    1 answer? if
        self-idle-or-end
    then
    $F $54 fade
    $D 0 movie-play
    $C cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 castle-2f-5.cmd03
    wait-fade
    $300 story-flag-clear
    1 6 sound-stop
    0 noise-level
    1 action-end
    1 char-done
    $FE $32 212 2 stalker-to-room
    $FE 1 -1 char-camera
    $FE $80808080 1 char-tint
    $FE $1E char-layer
    0 $F9 9 action
    $FF panic-stage? if
        3 panic-stage
    then
    $1A message-prepare
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
    0 castle-2f-5.cmd04
    $FE 0 0 char-tint
    $FE $A char-layer
    wait-fade
    8 state-flag-set
    0 counter-set
    2 castle-2f-5.cmd03
    0 castle-2f-5.cmd05
    1 char-activate
    $32 0 39 hewie-to-room
    0 1 $22 action
    $FE $D4 25.182 -33.041 -113 char-to-xz
    stalker-item-cooldown
    $42 story-flag-set
    fiona-calm-reset
    0 $21 4.683 -40.0 49 char-to-xz
    hewie-controlled? not if
        0 1 -1 char-camera
        0 camera-follow
    else
        1 1 -1 char-camera
        1 camera-follow
    then
    camera-restart
    1 door-unlock
    3 door-unlock
    $C door-unlock
    $14 state-flag-clear
    $1E state-flag-clear
    1 wait-counter
    2 counter-set
    $1B state-flag-clear
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    5 state-flag-clear
    $231 item-give
    $F $51 fade
    wait-fade
    stalker-item-cooldown
    self-idle-or-end
;

: castle-2f-5.act1B ( -- )   \ 0047ADA8
    self-idle-or-end
;

: castle-2f-5.act1C ( -- )   \ 0042BEA0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $E8 -25.0 63.0 -5 char-to-xz
    1 20.0 10.0 40.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 ebit? not if
        1 message
        wait-message
        9 ebit-set
    else
        2 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act1D ( -- )   \ 0042BEF0
    self-wait-done
    5.0 40.5 self-turn-to-xz
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: castle-2f-5.act1E ( -- )   \ 0042BF00
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $55 -40.0 12.0 -90 char-to-xz
    1 30.0 20.0 -60.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act1F ( -- )   \ 0042BF48
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $40 message
    wait-message
    self-idle-or-end
;

: castle-2f-5.act20 ( -- )   \ 0042BF60
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message
    wait-message
    0 answer? if
        7 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            0 $10 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                3 state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                3 state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                3 state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                3 state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                3 state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                3 state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                3 state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            1 char-here? if
                1 2 char-C4? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: castle-2f-5.act21 ( -- )   \ 0047ADAC
    self-idle-or-end
;

: castle-2f-5.act22 ( -- )   \ 0042C0B0
    1 self-scripted
    1 self-noclip
    self-wait-done
    1 $25 13.928 -57.528 -90 char-to-xz
    1 9 char-file-load
    1 char-file-use
    $8001 self-anim
    self-wait-anim
    1 counter-set
    2 wait-counter
    1 $6A 5 char-sound
    $8002 5 self-anim-9
    self-wait-anim
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: castle-2f-5.act23 ( -- )   \ 0042C0F0
    self-wait-done
    0 $F1 $12 action
    0 castle-2f-5.cmd02
    0 castle-2f-5.cmd04
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    3 partner-load
    2 char-unload
    $D 0 movie-play
    $C cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 castle-2f-5.cmd03
    $FE $32 212 2 stalker-to-room
    $FE $80808080 1 char-tint
    $FE $1E char-layer
    0 $F9 9 action
    $FF panic-stage? if
        3 panic-stage
    then
    $1A message-prepare
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
    0 castle-2f-5.cmd04
    $FE 0 0 char-tint
    $FE $A char-layer
    2 castle-2f-5.cmd03
    0 castle-2f-5.cmd05
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-5.enter castle-2f-5 0 room-script!
' castle-2f-5.char-enter castle-2f-5 6 room-script!
' castle-2f-5.phase1 castle-2f-5 1 room-script!
' castle-2f-5.phase2 castle-2f-5 2 room-script!
' castle-2f-5.act00 castle-2f-5 $00 action-script!
' castle-2f-5.act01 castle-2f-5 $01 action-script!
' castle-2f-5.act02 castle-2f-5 $02 action-script!
' castle-2f-5.act03 castle-2f-5 $03 action-script!
' castle-2f-5.act04 castle-2f-5 $04 action-script!
' castle-2f-5.act05 castle-2f-5 $05 action-script!
' castle-2f-5.act06 castle-2f-5 $06 action-script!
' castle-2f-5.act07 castle-2f-5 $07 action-script!
' castle-2f-5.act08 castle-2f-5 $08 action-script!
' castle-2f-5.act09 castle-2f-5 $09 action-script!
' castle-2f-5.act0A castle-2f-5 $0A action-script!
' castle-2f-5.act0B castle-2f-5 $0B action-script!
' castle-2f-5.act0C castle-2f-5 $0C action-script!
' castle-2f-5.act0D castle-2f-5 $0D action-script!
' castle-2f-5.act0E castle-2f-5 $0E action-script!
' castle-2f-5.act0F castle-2f-5 $0F action-script!
' castle-2f-5.act10 castle-2f-5 $10 action-script!
' castle-2f-5.act11 castle-2f-5 $11 action-script!
' castle-2f-5.act12 castle-2f-5 $12 action-script!
' castle-2f-5.act13 castle-2f-5 $13 action-script!
' castle-2f-5.act14 castle-2f-5 $14 action-script!
' castle-2f-5.act15 castle-2f-5 $15 action-script!
' castle-2f-5.act16 castle-2f-5 $16 action-script!
' castle-2f-5.act17 castle-2f-5 $17 action-script!
' castle-2f-5.act18 castle-2f-5 $18 action-script!
' castle-2f-5.act19 castle-2f-5 $19 action-script!
' castle-2f-5.act1A castle-2f-5 $1A action-script!
' castle-2f-5.act1B castle-2f-5 $1B action-script!
' castle-2f-5.act1C castle-2f-5 $1C action-script!
' castle-2f-5.act1D castle-2f-5 $1D action-script!
' castle-2f-5.act1E castle-2f-5 $1E action-script!
' castle-2f-5.act1F castle-2f-5 $1F action-script!
' castle-2f-5.act20 castle-2f-5 $20 action-script!
' castle-2f-5.act21 castle-2f-5 $21 action-script!
' castle-2f-5.act22 castle-2f-5 $22 action-script!
' castle-2f-5.act23 castle-2f-5 $23 action-script!
