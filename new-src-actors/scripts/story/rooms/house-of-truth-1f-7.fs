\ story/rooms/house-of-truth-1f-7.fs - the event scripts of room house-of-truth-1f-7 ($8F; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-7
USING: room-names story.words story.shared ;

\ a struggle: byte 3 0 resets Fiona's shake tracking (+0x1AD710 / +0x1AD714); 1 adds her shakes
\ to script variable byte 4, with a grunt (voice 0x3D or 0x45 at random) when the cool-down
\ variable byte 6 is out (it then runs 45 / 60), and at 100 the event byte 5 (+0x5C)
: house-of-truth-1f-7.cmd00 ( bytes.. n -- )  0 ?do drop loop s" house-of-truth-1f-7.cmd00" stub-step ;
\ byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(0, 0); else
\ Progress_SpeechCall
: house-of-truth-1f-7.cmd01 ( b0 -- )  drop s" house-of-truth-1f-7.cmd01" stub-step ;
\ room 0x8F: three grey smoke effects (Effect79B00, size 50) at the room's spots 0, 1, 6
\ (grey_three).
: house-of-truth-1f-7.cmd02 ( -- )  s" house-of-truth-1f-7.cmd02" stub-step ;
\ room 0x8F: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-7.cmd03 ( -- )  s" house-of-truth-1f-7.cmd03" stub-step ;
\ the pursuer is active and out of view: state 3 (+0xE8), or the camera's on-screen test (+0xD4)
\ fails
: house-of-truth-1f-7.cond00? ( -- flag )  s" house-of-truth-1f-7.cond00?" stub-flag ;

: house-of-truth-1f-7.enter ( -- )   \ 00435990
    room-sounds
    $9E story-flag? not if
        0 1 $14 door-bits
    else
        0 0 $14 door-bits
    then
    $9F story-flag? not if
        2 0 object-show
        3 0 object-show
        1 0 8 nav-group
    else
        2 1 object-show
        3 1 object-show
    then
    4 0 object-show
    $A4 story-flag? not if
        1 0 $14 door-bits
        2 1 $14 door-bits
    else
        1 1 8 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    then
    0 -59.1 18.5 -16.0 1 effect-86
    1 -10.4 17.5 -26.3 1 effect-86
    2 84.1 19.8 -2.3 1 effect-86
    3 -57.4 20.0 24.4 1 effect-86
    4 115.9 19.5 33.2 1 effect-86
    $34E story-flag? if
        5 1.0 0 bgm
    then
    shared.act95
    $A3 story-flag? if
        house-of-truth-1f-7.cmd02
    then
    $2E7 story-flag? not if
        5 13.05 6.54 108.47 flicker-sprite
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-7.act08 ( -- )   \ 00436180
    5 ebit-set
    6 ebit-set
    4 ebit-clear
    $25 0 pvar? if
        $A chance? if
            4 ebit-set
        then
    else $25 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $25 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $25 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    $FE char-unseen? not if
        4 ebit-set
    then
    2 creature-action? if
        4 ebit-set
    then
    4 ebit? if
        0 $FE 9 action
    else
        $78 1 item-cooldown
        8 ebit-clear
    then
    0 stalker-alert? if
        $FE 2 stalker-mode
    then
    $25 pvar-inc
    exit
;

: house-of-truth-1f-7.char-enter ( -- )   \ 00435A60
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
                0 4 4 char-camera
                0 camera-follow
            else
                1 4 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
    then then
    1 4 4 area-camera
    $FE self-is? if
        9 state-flag? if
            house-of-truth-1f-7.act08
        else
            6 ebit-clear
        then
    then
;

: house-of-truth-1f-7.phase1 ( -- )   \ 00435AF0
    9 ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    7 camera-mode? if
        $15 2 2 1 chars-area-camera
        $16 2 2 1 chars-area-camera
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    $C 4 4 1 chars-area-camera
    $D 5 5 1 chars-area-camera
    $E 5 5 1 chars-area-camera
    $F 0 0 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $A1 story-flag? $9F story-flag? not and if
        1 $10 char-in-area? if
            0 ebit? not if
                2 game-mode? not if
                    1 2 char-C4? if
                        1 1 char-heal
                    else
                        0 1 1 action
                    then
                then
            then
        then
    then
    0 ebit? 1 char-busy? not and 1 $10 char-in-area? not and if
        0 ebit-clear
    then
    1 2 char-C4? if
        0 ebit-clear
    then
    $A4 story-flag? if
        $A0 story-flag? not if
            0 $11 char-entered-area? if
                $A0 story-flag-set
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
    then
    $A3 story-flag? not if
        $A5 story-flag? not if
            0 $13 char-entered-area? if
                $A5 story-flag-set
                $8F 0 235 $88 0 -1 0 0.0 creature-place
            then
        then
    then
    $FE 2 char-C4? not if
        9 state-flag? 5 ebit? not and $FE char-here? and if
            $FE char-busy? not if
                8 ebit-set
                house-of-truth-1f-7.act08
            then
        then
    then
    7 ebit? $FE $17 char-in-area? and if
        0 $FE $D action
    then
    house-of-truth-1f-7.cmd03
;

: house-of-truth-1f-7.phase2 ( -- )   \ 00435C20
    $A5 story-flag? if
        0 $14 char-in-area? 0 90 $3C char-heading? and if
            $FE char-here? not if
                5 6 5 scene-change
            else house-of-truth-1f-7.cond00? not if
                5 6 5 scene-change
            else
                $8016 scene-ending
            then then
        then
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 5 1 scene-change
        then
    then
    $9F story-flag? not if
        0 $12 char-in-area? 0 -45 $32 char-heading? and if
            0 ebit? if
                5 0 0 scene-change
            then
        then
    else $9E story-flag? not if
        0 $12 char-in-area? 0 -45 $32 char-heading? and if
            5 2 0 scene-change
        then
    then then
    $2E7 story-flag? not if
        0 5 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
;

: house-of-truth-1f-7.phase3 ( -- )   \ 00435CA0
    -72.3 18.0 56.0 -85.0 18.0 56.4 -72.3 -2.0 51.4 -85.0 -2.0 51.4 lights-doorway
    72.0 16.0 -0.1 91.2 16.0 -0.1 72.0 0.0 -2.0 91.2 0.0 -2.0 lights-doorway
    $A3 story-flag? 9 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        9 ebit-set
    then
;

: house-of-truth-1f-7.act00 ( -- )   \ 00435D30
    $18 state-flag-set
    1 self-scripted
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
    self-wait-done
    -89.0 75.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    self-wait-16
    0 message
    wait-message
    self-wait-anim
    $F $44 fade
    wait-fade
    $26 3 pvar? if
        0 house-of-truth-1f-7.cmd01
    else $26 2 pvar? if
        1 house-of-truth-1f-7.cmd01
    then then
    1 action-end
    1 char-done
    $14 $64 movie-param
    0 $F9 $B action
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
    2 1 object-show
    3 1 object-show
    0 0 8 nav-group
    $9F story-flag-set
    0 $B3 -74.86 74.73 -91 char-to-xz
    1 char-activate
    1 $B5 -51.58 76.38 -80 char-to-xz
    50 hewie-trust
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    $26 3 pvar? $26 2 pvar? or if
        3 house-of-truth-1f-7.cmd01
    then
    $F $41 fade
    wait-fade
    $4C resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-7.act01 ( -- )   \ 00435E50
    0 self-scripted
    0 ebit-set
    self-wait-done
    -89.0 75.0 self-turn-to-xz
    self-wait-done
    begin
        hewie-bark
        self-wait-done
        2 game-mode? until
    0 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-7.act02 ( -- )   \ 00435E70
    self-wait-done
    -89.0 75.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 message
    wait-message
    self-wait-anim
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0 $14 door-bits
    $9E story-flag-set
    $26 message-param-room
    $26 1 item-give-count
    0 $26 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-7.act03 ( -- )   \ 00435ED0
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
    $FE char-here? if
        0 $FE 4 action-force
    then
    1 char-here? if
        0 1 4 action-force
    then
    $F8 action-end
    $F $14 fade
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    $FE 1 char-visible
    1 1 char-visible
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
    $50 resident-flag-set
    4 0 object-show
    1 1 8 nav-group
    $A0 story-flag-set
    0 5 char-file-load
    0 char-file-use
    0 $192 -37.91 65.69 180 char-to-xz
    hewie-controlled? not if
        0 7 -1 char-camera
        0 camera-follow
    else
        1 7 -1 char-camera
        1 camera-follow
    then
    camera-restart
    $18 state-flag-clear
    0 1 house-of-truth-1f-7.cmd00
    $8000 self-anim
    4 0 object-anim
    $40000000 6 -37.0 0.0 63.0 0 0 sound
    $FE char-here? if
        $FE action-end
    then
    1 char-here? if
        1 action-end
    then
    $FE 0 char-visible
    1 0 char-visible
    $FE 1 char-in-nav-group? if
        $FE $194 char-to-tri
    then
    1 1 char-in-nav-group? if
        1 $18B char-to-tri
    then
    $F $11 fade
    shared.act95
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    7 ebit-set
    begin
        self-at-motion-event? if
            $8000 self-anim
            4 0 object-anim
        then
        1 0 var? not if
            1 var-dec
        then
        1 0 1 1 4 house-of-truth-1f-7.cmd00
        self-frames-reset
        1 self-wait-frames
        1 ebit? until
    self-wait-anim
    0 $45 5 char-sound
    $8001 self-anim
    4 1 object-anim
    self-wait-anim
    $FE char-busy? if
        $FE action-end
    then
    7 ebit-clear
    $E state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-7.act04 ( -- )   \ 0047AE60
    begin
        self-frames-reset
        1 self-wait-frames
    again
;

: house-of-truth-1f-7.act05 ( -- )   \ 00436050
    self-wait-done
    $FE self-touching? not if
        $EA self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $EA char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $EA door-unlock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    $E6 door-lock
    $E9 door-lock
    self-idle-or-end
;

: house-of-truth-1f-7.act07 ( -- )   \ 00436150
    3 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    $FF 0 char-visible
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    3 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-7.act06 ( -- )   \ 004360A0
    $18 state-flag-set
    6 ebit-clear
    1 self-scripted
    5 ebit-clear
    0 8 char-file-load
    self-wait-done
    $FF self-look-at
    yield
    0 char-file-use
    $FE $8004 5 50.28 39.2 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    3 ebit-clear
    0 avoid-prompt
    $FF 6 -1 char-camera
    $FF 1 char-visible
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] house-of-truth-1f-7.act07 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                6 ebit? $FE char-here? not and if
                    6 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] house-of-truth-1f-7.act07 goto
            then
            $FF 0 char-visible
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

: house-of-truth-1f-7.act09 ( -- )   \ 004361E0
    self-wait-done
    8 ebit? not if
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
    8 ebit-clear
    $FE 45.21 54.9 180 $FFFF 5 self-move-to
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

: house-of-truth-1f-7.act0A ( -- )   \ 00436250
    self-wait-done
    13.05 108.47 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $97 message-param-room
        $97 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E7 story-flag-set
            5 effect-remove
            $97 1 item-give-count
            0 $97 item-tab
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

: house-of-truth-1f-7.act0B ( -- )   \ 004362B0
    begin
        2 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    $26 3 pvar? $26 2 pvar? or if
        2 house-of-truth-1f-7.cmd01
    then
    self-idle-or-end
;

: house-of-truth-1f-7.act0C ( -- )   \ 0047AE68
    self-idle-or-end
;

: house-of-truth-1f-7.act0D ( -- )   \ 004362D0
    self-wait-done
    7 ebit? not if
        self-idle-or-end
    then
    $18E -25.136 68.404 -102 $FFFF 5 self-move-to
    self-wait-done
    7 ebit? not if
        self-idle-or-end
    then
    $E04 self-anim
    self-frames-reset
    $14 self-wait-frames
    $FF panic-stage? if
        3 panic-stage
    then
    1 0 $E action-force
    4 2 object-anim
    self-frames-reset
    $1B self-wait-frames
    $40000001 6 -37.0 0.0 63.0 0 0 sound
    begin
        yield
    again
;

: house-of-truth-1f-7.act0E ( -- )   \ 00436320
    $2C state-flag-set
    self-wait-done
    0 $41 5 char-sound
    $1100 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    1 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: house-of-truth-1f-7.act0F ( -- )   \ 00436340
    self-wait-done
    0 1 $14 door-bits
    2 1 $14 door-bits
    0 -59.1 18.5 -16.0 1 effect-86
    1 -10.4 17.5 -26.3 1 effect-86
    2 84.1 19.8 -2.3 1 effect-86
    3 -57.4 20.0 24.4 1 effect-86
    4 115.9 19.5 33.2 1 effect-86
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
    $26 3 pvar? if
        0 house-of-truth-1f-7.cmd01
    else $26 2 pvar? if
        1 house-of-truth-1f-7.cmd01
    then then
    $14 $64 movie-param
    0 $F9 $B action
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
    $26 3 pvar? $26 2 pvar? or if
        3 house-of-truth-1f-7.cmd01
    then
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: house-of-truth-1f-7.act10 ( -- )   \ 00436440
    self-wait-done
    2 1 object-show
    3 1 object-show
    1 1 $14 door-bits
    0 -59.1 18.5 -16.0 1 effect-86
    1 -10.4 17.5 -26.3 1 effect-86
    2 84.1 19.8 -2.3 1 effect-86
    3 -57.4 20.0 24.4 1 effect-86
    4 115.9 19.5 33.2 1 effect-86
    house-of-truth-1f-7.cmd02
    7 0 movie-play
    6 cutscene-start
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-1f-7.enter house-of-truth-1f-7 0 room-script!
' house-of-truth-1f-7.char-enter house-of-truth-1f-7 6 room-script!
' house-of-truth-1f-7.phase1 house-of-truth-1f-7 1 room-script!
' house-of-truth-1f-7.phase2 house-of-truth-1f-7 2 room-script!
' house-of-truth-1f-7.phase3 house-of-truth-1f-7 3 room-script!
' house-of-truth-1f-7.act00 house-of-truth-1f-7 $00 action-script!
' house-of-truth-1f-7.act01 house-of-truth-1f-7 $01 action-script!
' house-of-truth-1f-7.act02 house-of-truth-1f-7 $02 action-script!
' house-of-truth-1f-7.act03 house-of-truth-1f-7 $03 action-script!
' house-of-truth-1f-7.act04 house-of-truth-1f-7 $04 action-script!
' house-of-truth-1f-7.act05 house-of-truth-1f-7 $05 action-script!
' house-of-truth-1f-7.act06 house-of-truth-1f-7 $06 action-script!
' house-of-truth-1f-7.act07 house-of-truth-1f-7 $07 action-script!
' house-of-truth-1f-7.act08 house-of-truth-1f-7 $08 action-script!
' house-of-truth-1f-7.act09 house-of-truth-1f-7 $09 action-script!
' house-of-truth-1f-7.act0A house-of-truth-1f-7 $0A action-script!
' house-of-truth-1f-7.act0B house-of-truth-1f-7 $0B action-script!
' house-of-truth-1f-7.act0C house-of-truth-1f-7 $0C action-script!
' house-of-truth-1f-7.act0D house-of-truth-1f-7 $0D action-script!
' house-of-truth-1f-7.act0E house-of-truth-1f-7 $0E action-script!
' house-of-truth-1f-7.act0F house-of-truth-1f-7 $0F action-script!
' house-of-truth-1f-7.act10 house-of-truth-1f-7 $10 action-script!
