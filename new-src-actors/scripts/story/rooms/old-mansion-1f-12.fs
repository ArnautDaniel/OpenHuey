\ story/rooms/old-mansion-1f-12.fs - the event scripts of room old-mansion-1f-12 ($4F; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-12
USING: room-names story.words story.shared ;

\ room 0x4F (Room4F_Cmd00_ptmf): object byte 3 by byte 4: 0 up (+0x10 0), 1 down (-0.65), 2
\ lowered a step (0.02, not during a movie); once down, events bit 3
: old-mansion-1f-12.cmd00 ( b0 b1 -- )  drop drop s" old-mansion-1f-12.cmd00" stub-step ;
\ room 0x4F (Room4F_Cmd01_ptmf): an effect (DustShaft_vtable, 0x7460 bytes), not started
: old-mansion-1f-12.cmd01 ( -- )  s" old-mansion-1f-12.cmd01" stub-step ;
\ byte 3 0: a SpiralSmoke_vtable effect (0x36C0 bytes) spawned, its slot kept in event var 0;
\ else that slot's effect removed
: old-mansion-1f-12.cmd02 ( b0 -- )  drop s" old-mansion-1f-12.cmd02" stub-step ;
\ lower the object (pstr_sikakebox)'s +0x14 by 0.025 a frame down to -0.78, then event 6 (+0x5C)
: old-mansion-1f-12.cmd03 ( -- )  s" old-mansion-1f-12.cmd03" stub-step ;
\ Room4F_Cond00
: old-mansion-1f-12.cond00? ( -- flag )  s" old-mansion-1f-12.cond00?" stub-flag ;

: old-mansion-1f-12.enter ( -- )   \ 0040B520
    room-sounds
    $52 story-flag? if
        0 1 old-mansion-1f-12.cmd00
    then
    $50 story-flag? if
        1 1 old-mansion-1f-12.cmd00
    then
    $51 story-flag? if
        2 1 old-mansion-1f-12.cmd00
    then
    $70 story-flag? if
        0 1 $14 door-bits
        3 1 $14 door-bits
        8 1 object-show
    else
        0 0 $14 door-bits
        3 0 $14 door-bits
        8 0 object-show
    then
    $4C story-flag? if
        4 0 $14 door-bits
    else
        4 1 $14 door-bits
    then
    $50 story-flag? if
        5 0 $14 door-bits
        6 1 $14 door-bits
        1 1 $14 door-bits
        old-mansion-1f-12.cmd01
    else
        5 1 $14 door-bits
        6 0 $14 door-bits
        2 1 $14 door-bits
        0 old-mansion-1f-12.cmd02
    then
    $73 story-flag? not $51 story-flag? not and if
        0 12.0 13.0 -24.0 flicker-sprite
    then
    $2A3 story-flag? $2A4 story-flag? not and if
        1 52.04 1.0 5.68 flicker-sprite
    then
;

: old-mansion-1f-12.char-enter ( -- )   \ 0040B5C0
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
    0 self-is? if
        2 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-12.phase1 ( -- )   \ 0040B680
    $50 story-flag? not if
        7 ebit? not if
            6 sound-bank-loaded? if
                $40000004 6 38.0 5.0 -10.0 0 0 sound
                7 ebit-set
            then
        else
            $C0000004 6 38.0 5.0 -10.0 0 0 sound
        then
    then
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
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    4 ebit? not if
        $70 story-flag? not if
            0 camera-mode? 1 camera-mode? or if
                old-mansion-1f-12.cond00? if
                    4 ebit-set
                    $70 story-flag-set
                    0 $F1 $C action
                then
            then
        then
    then
    $73 story-flag? not $51 story-flag? not and if
        0 0 4 4 0 zone-at-effect
    then
    $50 story-flag? not if
        1 0.32 0.0 -28.08 $19 31 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $92 action
                        then
                    then
                then
            then
        then
    then
    2 -12.29 0.0 -26.2 $18 15 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    1 0.32 0.0 -28.08 $19 31 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 31.0 hewie-look-zone
            then
        then
    then
    3 12.19 0.0 -23.96 $18 15 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    4 38.13 0.0 -9.78 $1E 30 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 30.0 hewie-look-zone
            then
        then
    then
    5 -35.94 0.0 -7.54 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    $2A3 story-flag? not if
        6 52.04 0.0 5.68 $A 5 0 zone
        1 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 675 var-set
                $1A 676 var-set
                $1B 1 var-set
                $1C 52040 var-set
                $1D 1000 var-set
                $1E 5680 var-set
                $1F 6 var-set
                0 1 $8B action
            then
        then
    then
;

: old-mansion-1f-12.phase2 ( -- )   \ 0040B8A0
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    0 5 char-in-area? 0 90 $32 char-heading? and if
        $70 story-flag? $4C story-flag? not and if
            5 2 4 scene-change
        else
            5 2 0 scene-change
        then
    then
    0 6 $32 char-faces-area? if
        5 3 0 scene-change
    then
    0 7 char-in-area? 0 0 $32 char-heading? and if
        5 4 0 scene-change
    then
    $73 story-flag? not $51 story-flag? not and if
        0 0 2 char-zone-bits? if
            5 $B 4 scene-change
        then
    else 0 8 char-in-area? 0 90 $32 char-heading? and if
        5 9 0 scene-change
    then then
    0 9 char-in-area? 0 90 $32 char-heading? and if
        5 9 0 scene-change
    then
    0 $F char-in-area? 0 -45 $3C char-heading? and if
        5 $E 0 scene-change
    then
    $2A3 story-flag? $2A4 story-flag? not and if
        6 1 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 $D 4 scene-change
        then
    then
;

: old-mansion-1f-12.act00 ( -- )   \ 0040B940
    self-wait-done
    1 ebit? not 2 game-mode? or $FF panic-stage? or if
        0 self-through-exit
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
            8 message
            wait-message
        then
        1 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $150 -60.0 15.0 -90 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        self-frames-reset
        4 self-wait-frames
        9 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $150 -60.3 16.25 -90 char-to-xz
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-12.act01 ( -- )   \ 0040B9D0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 3 6 char-sound
    $F message-param-room
    $F item-use
    $6C door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act02 ( -- )   \ 0040BA10
    self-wait-done
    $70 story-flag? $4C story-flag? not and if
        $14E -33.036 -2.992 -130 $FFFF 5 self-move-to
        self-wait-done
        $900 self-anim
        self-wait-anim
        4 0 $14 door-bits
        $4C story-flag-set
        $E message-param-room
        $E 1 item-give-count
        0 $E item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $901 self-anim
        self-wait-anim
    else
        $17 state-flag-set
        1 self-scripted
        0 $14E -38.0 -2.5 180 char-to-xz
        1 10.0 -10.0 0.0 0.0 event-camera
        self-frames-reset
        4 self-wait-frames
        $70 story-flag? not if
            $B message
            wait-message
        else
            $C message
            wait-message
        then
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-12.act03 ( -- )   \ 0040BAC0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 story-flag? not if
        38.0 -10.0 self-turn-to-xz
        self-wait-done
        $402 self-anim
        self-wait-anim
        5 message
        wait-message
    else
        6 message
        wait-message
        0 answer? if
            $F 4 fade
            wait-fade
            0 5 6 char-sound
            0 $10C 180 char-to-tri-facing
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            2 exit-prepare
            self-frames-reset
            $5A self-wait-frames
            2 map-page
            $F 1 fade
            wait-fade
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act04 ( -- )   \ 0040BB20
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A message
    wait-message
    0 answer? if
        $F 4 fade
        wait-fade
        0 5 6 char-sound
        0 $141 0 char-to-tri-facing
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        camera-restart
        1 exit-prepare
        self-frames-reset
        $5A self-wait-frames
        1 map-page
        $F 1 fade
        wait-fade
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act05 ( -- )   \ 0040BB70
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    4 1 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    2 0 $14 door-bits
    1 old-mansion-1f-12.cmd02
    0 ebit? if
        0 $F9 8 action
    then
    $10 $C8 movie-param
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
    wait-fade
    8 state-flag-set
    0 self-move-16
    0 $11E -5.7 17.0 180 char-to-xz
    1 $A3 -22.21 -11.73 -45 char-to-xz
    camera-restart
    0 ebit? if
        1 1 old-mansion-1f-12.cmd00
        5 0 $14 door-bits
        6 1 movie-play
        5 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
        wait-fade
        1 action-end
        1 char-done
        $10 $C8 movie-param
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
        wait-fade
        1 char-activate
        $4F 0 163 hewie-to-room
        1 $A3 -22.21 -11.73 -45 char-to-xz
        50 hewie-trust
        1 1 $14 door-bits
        6 1 $14 door-bits
        old-mansion-1f-12.cmd01
        4 6 sound-stop
        $50 story-flag-set
    else
        2 1 $14 door-bits
        5 1 $14 door-bits
        0 old-mansion-1f-12.cmd02
    then
    $F $51 fade
    wait-fade
    0 ebit? not if
        self-frames-reset
        self-wait-16
        3 message
        wait-message
    then
    $827D item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act06 ( -- )   \ 0040BD10
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    12.0 -24.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    2 6 12.0 10.0 -25.0 0 0 sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    0 ebit? if
        0 effect-remove
        3 ebit-clear
        1 6 12.0 10.0 -25.0 0 0 sound
        begin
            2 2 old-mansion-1f-12.cmd00
            3 ebit? not while
            yield
        repeat
        $51 story-flag-set
    else
        self-frames-reset
        $1E self-wait-frames
        3 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act07 ( -- )   \ 0040BD80
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -12.0 -24.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    2 6 -12.0 10.0 -25.0 0 0 sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    0 ebit? if
        3 ebit-clear
        1 6 -12.0 10.0 -25.0 0 0 sound
        begin
            0 2 old-mansion-1f-12.cmd00
            3 ebit? not while
            yield
        repeat
        $52 story-flag-set
    else
        self-frames-reset
        $1E self-wait-frames
        3 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act08 ( -- )   \ 0040BDF0
    begin
        $44C cutscene-cue-reached? not while
        yield
    repeat
    3 ebit-clear
    1 6 0.0 30.0 -27.0 0 0 sound
    begin
        1 2 old-mansion-1f-12.cmd00
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-1f-12.act09 ( -- )   \ 0040BE20
    self-wait-done
    $50 story-flag? not if
        $52 story-flag? not $51 story-flag? not or if
            0 message
            wait-message
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-12.act0A ( -- )   \ 0040BE40
    self-wait-done
    0.0 -27.0 self-turn-to-xz
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    4 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-12.act0B ( -- )   \ 0040BE60
    self-wait-done
    12.0 -24.0 self-turn-to-xz
    self-wait-done
    $D message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $73 story-flag-set
    0 effect-remove
    $A3 message-param-room
    $A3 1 item-give-count
    0 $A3 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-12.act0C ( -- )   \ 0040BEB0
    self-frames-reset
    self-wait-16
    0 1 $14 door-bits
    0 6 -38.0 5.0 -8.0 0 0 sound
    6 ebit-clear
    begin
        old-mansion-1f-12.cmd03
        6 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-1f-12.act0D ( -- )   \ 0040BEE0
    self-wait-done
    52.04 5.68 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2A4 story-flag-set
            1 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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

: old-mansion-1f-12.act0E ( -- )   \ 0040BF40
    self-wait-done
    -63.0 -10.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $E message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-12.act0F ( -- )   \ 0040BF60
    room-sounds
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    self-wait-done
    0 1 old-mansion-1f-12.cmd00
    2 1 old-mansion-1f-12.cmd00
    5 1 $14 door-bits
    4 1 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 8 action
    $10 $C8 movie-param
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

: old-mansion-1f-12.act10 ( -- )   \ 0040C000
    0 1 old-mansion-1f-12.cmd00
    1 1 old-mansion-1f-12.cmd00
    2 1 old-mansion-1f-12.cmd00
    6 1 movie-play
    5 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $10 $C8 movie-param
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
' old-mansion-1f-12.enter old-mansion-1f-12 0 room-script!
' old-mansion-1f-12.char-enter old-mansion-1f-12 6 room-script!
' old-mansion-1f-12.phase1 old-mansion-1f-12 1 room-script!
' old-mansion-1f-12.phase2 old-mansion-1f-12 2 room-script!
' old-mansion-1f-12.act00 old-mansion-1f-12 $00 action-script!
' old-mansion-1f-12.act01 old-mansion-1f-12 $01 action-script!
' old-mansion-1f-12.act02 old-mansion-1f-12 $02 action-script!
' old-mansion-1f-12.act03 old-mansion-1f-12 $03 action-script!
' old-mansion-1f-12.act04 old-mansion-1f-12 $04 action-script!
' old-mansion-1f-12.act05 old-mansion-1f-12 $05 action-script!
' old-mansion-1f-12.act06 old-mansion-1f-12 $06 action-script!
' old-mansion-1f-12.act07 old-mansion-1f-12 $07 action-script!
' old-mansion-1f-12.act08 old-mansion-1f-12 $08 action-script!
' old-mansion-1f-12.act09 old-mansion-1f-12 $09 action-script!
' old-mansion-1f-12.act0A old-mansion-1f-12 $0A action-script!
' old-mansion-1f-12.act0B old-mansion-1f-12 $0B action-script!
' old-mansion-1f-12.act0C old-mansion-1f-12 $0C action-script!
' old-mansion-1f-12.act0D old-mansion-1f-12 $0D action-script!
' old-mansion-1f-12.act0E old-mansion-1f-12 $0E action-script!
' old-mansion-1f-12.act0F old-mansion-1f-12 $0F action-script!
' old-mansion-1f-12.act10 old-mansion-1f-12 $10 action-script!
