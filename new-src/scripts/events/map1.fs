\ events/map1.fs - the event scripts of the rooms on the game's map 1 (kMapRooms).
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.map1
USING: events.core events.words events.builtin ;

\ ---- room $2E ----------------------------------------------------------------------------------

\ a lit quad at x -15.96, z -2 .. 6, height 111 / 91
: room2E.cmd00 ( b0 -- )  drop s" room2E.cmd00" stub-step ;
\ the depth range (effect 0x1C) opening out with the cutscene from its frame 1260: 1 / 21 / 40 /
\ 80 on by 0.4 a frame, up to 41 / 61 / 80 / 120
: room2E.cmd03 ( -- )  s" room2E.cmd03" stub-step ;
\ the pursuer's model's +0x9E8 by byte 3: 0 0.15, 1 0, else 0.05
: room2E.cmd04 ( b0 -- )  drop s" room2E.cmd04" stub-step ;

: room2E.enter ( -- )   \ 0041D2E0
    room-sounds
    $4A story-flag? not if
        0 room2E.cmd00
        0 1 $14 door-bits
        $71 story-flag? if
            7 state-flag-set
            $21 state-flag-set
            $23 state-flag-set
            3 char-activate
            $FE 1 char-silent
            $FE $2E 104 2 stalker-to-room
            $FE 1 1 char-camera
            0 3 1 action
            1 0 $1000000 nav-group
        then
    else
        1 1 $14 door-bits
        1 1 $2008000 nav-group
    then
;

: room2E.char-enter ( -- )   \ 0041D330
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
        1 exit-taken? if
            3 map-page
        then
    then
;

: room2E.phase1 ( -- )   \ 0041D3C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 8 char-entered-area? if
        3 map-page
    then
    0 8 char-left-area? if
        2 map-page
    then
    0 $A char-entered-area? if
        2 map-page
    then
    0 $A char-left-area? if
        1 map-page
    then
    $5C story-flag? $71 story-flag? not and if
        0 stalker-alert? not if
            $FE 0 stalker-mode
        then
    then
    $71 story-flag? not if
        0 ebit? not if
            0 8 char-entered-area? if
                0 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 0 action-force
                else
                    1 0 0 action-force
                then
            then
        then
    else $4A story-flag? not if
        0 9 char-entered-area? 2 ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 3 action-force
        then
    then then
    $4A story-flag? not $71 story-flag? and if
        0 -3.91 90.0 3.23 $50 26 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        0 -3.91 90.0 3.23 $50 26 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? $FE char-here? and if
        $FE $80000005 6 char-sound
    then
;

: room2E.phase5 ( -- )   \ 0041D4E0
    7 state-flag-clear
    $71 story-flag? $4A story-flag? not and if
        7 state-flag-clear
        $21 state-flag-clear
        $23 state-flag-clear
        $FE 0 char-silent
        $FE action-end
        $FE char-done
    then
;

: room2E.act00 ( -- )   \ 0041D510
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
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
    1 char-full-health
    1 0 char-set-C4
    $2E 0 49 hewie-to-room
    0 1 $86 action
    0 $F9 2 action
    3 4 0 char-model-op
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
    1 room2E.cmd04
    wait-fade
    8 state-flag-set
    0 self-move-16
    0 $DF 1.052 -47.906 -79 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    3 char-activate
    $FE $2E 104 2 stalker-to-room
    $FE 1 1 char-camera
    0 counter-set
    0 3 1 action
    1 action-end
    1 $31 30 char-to-tri-facing
    $71 story-flag-set
    1 exit-prepare
    0 room2E.cmd00
    1 wait-counter
    5 0 0 music
    $F $41 fade
    wait-fade
    $233 item-give
    $18 state-flag-clear
    $23 state-flag-set
    0 self-scripted
    self-idle-or-end
;

: room2E.act01 ( -- )   \ 0041D620
    3 2 char-file-load
    self-wait-done
    1 self-noclip
    1 self-scripted
    3 $68 -4.538 3.132 -101 char-to-xz
    7 state-flag-set
    $21 state-flag-set
    3 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 0 char-silent
    1 0 $20000 nav-group
    1 counter-set
    $FE 5 6 char-sound
    begin
        $8000 self-anim
        self-wait-anim
    again
;

: room2E.act02 ( -- )   \ 0041D670
    depth-range-off
    begin
        $10E cutscene-cue-reached? not while
        yield
    repeat
    0 room2E.cmd04
    begin
        $1E0 cutscene-cue-reached? not while
        yield
    repeat
    1 room2E.cmd04
    begin
        5 cutscene-shot? not while
        yield
    repeat
    2 room2E.cmd00
    2 room2E.cmd04
    self-frames-reset
    1 self-wait-frames
    1 room2E.cmd04
    begin
        $3A2 cutscene-cue-reached? not while
        yield
    repeat
    0 room2E.cmd00
    begin
        $4B0 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4EC cutscene-cue-reached? not while
        1.0 21.0 40.0 80.0 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        room2E.cmd03
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: room2E.act03 ( -- )   \ 0041D6E0
    2 ebit-set
    1 self-scripted
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    3 self-look-at
    yield
    0 message
    wait-message
    $FF self-look-at
    yield
    2 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: room2E.act04 ( -- )   \ 0041D700
    self-wait-done
    0 1 $14 door-bits
    3 partner-load
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
    0 $F9 2 action
    3 4 0 char-model-op
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
    1 room2E.cmd04
    2 0 char-remove
    0 room2E.cmd00
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room2E.phase2 ( -- )   \ 0047ACC4
;

' room2E.enter $2E 0 room-script!
' room2E.char-enter $2E 6 room-script!
' room2E.phase1 $2E 1 room-script!
' room2E.phase5 $2E 5 room-script!
' room2E.act00 $2E $00 action-script!
' room2E.act01 $2E $01 action-script!
' room2E.act02 $2E $02 action-script!
' room2E.act03 $2E $03 action-script!
' room2E.act04 $2E $04 action-script!
' room2E.phase2 $2E 2 room-script!

\ ---- room $40 ----------------------------------------------------------------------------------

\ the scales ("tenbin") and their pans ("sara_l", "sara_r"): level (byte 3 0) or tipped
: room40.cmd00 ( b0 -- )  drop s" room40.cmd00" stub-step ;

: room40.enter ( -- )   \ 00406550
    room-sounds
    1 0 var-set
    2 0 var-set
    1 room40.cmd00
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    $85 story-flag? not if
        1 0 $20000 nav-group
    else
        0 1 $14 door-bits
        9 1 object-show
        $A 1 object-show
    then
    $27A story-flag? not if
        0 -72.34 59.0 -34.25 flicker-sprite
    then
    2 -19.7 18.3 22.5 0 effect-86
    3 -19.7 18.3 25.0 0 effect-86
    4 -17.2 18.3 22.4 0 effect-86
    5 -17.2 18.3 25.0 0 effect-86
    6 17.2 18.3 22.4 0 effect-86
    7 17.2 18.3 25.0 0 effect-86
    8 19.7 18.3 22.5 0 effect-86
    9 19.7 18.3 25.0 0 effect-86
    1 $2300 sound-volume
    $1C 3 $FF char-load
;

: room40.char-enter ( -- )   \ 00406630
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
    hewie-controlled? not if
        0 self-is? 5 exit-taken? and if
            0 5 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 5 exit-taken? and if
        1 5 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    5 2 2 area-camera
    0 self-is? if
        2 exit-taken? 4 exit-taken? or if
            2 map-page
        then
    then
;

: room40.phase1 ( -- )   \ 00406770
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    hewie-controlled? not if
        0 5 char-in-area? 0 char-busy? not and if
            5 exit-check
        then
    else 1 5 char-in-area? 1 char-busy? not and if
        5 exit-check
    then then
    $B 0 0 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    $10 4 -1 1 chars-area-camera
    0 6 char-entered-area? if
        5 exit-prepare
    then
    0 7 char-entered-area? if
        0 exit-prepare
    then
    0 8 char-entered-area? if
        1 exit-prepare
    then
    0 9 char-entered-area? if
        2 exit-prepare
    then
    0 $A char-entered-area? if
        4 exit-prepare
    then
    0 0.56 0.0 14.62 $23 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    $85 story-flag? if
        4 ebit? not if
            6 sound-bank-loaded? if
                $40000000 6 0.0 0.0 130.0 0 0 sound
                4 ebit-set
            then
        else
            $C0000000 6 0.0 0.0 130.0 0 0 sound
        then
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 -109.95 50.0 -5.34 0 0 sound
                $4000000A 6 -42.0 20.0 117.0 0 0 sound
            else 1 1 var? if
                $40000006 6 -109.95 50.0 -5.34 0 0 sound
                $4000000B 6 -42.0 20.0 117.0 0 0 sound
            else 1 2 var? if
                $40000007 6 -109.95 50.0 -5.34 0 0 sound
                $4000000C 6 -42.0 20.0 117.0 0 0 sound
            else 1 3 var? if
                $40000008 6 -109.95 50.0 -5.34 0 0 sound
                $4000000D 6 -42.0 20.0 117.0 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: room40.phase2 ( -- )   \ 00406930
    0 $11 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    $85 story-flag? not if
        0 $12 char-in-area? 0 0 $32 char-heading? and if
            5 5 0 scene-change
        then
    then
    0 $13 char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $14 char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    $27A story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room40.act00 ( -- )   \ 00406990
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $FE action-end
    $FE char-done
    0 state-flag-clear
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
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    0 $F9 2 action
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
    0 self-move-16
    0 $158 3.0 24.0 83 char-to-xz
    camera-restart
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $40 1 229 hewie-to-room
            1 $E5 26.55 54.18 -129 char-to-xz
        then
    then
    $F9 action-end
    4 1 object-show
    5 1 object-show
    6 1 object-show
    0 11 var? if
        7 0 object-show
        8 1 object-show
        $B item-use
    else
        7 1 object-show
        8 0 object-show
        $C item-use
    then
    $B 0 object-show
    $C 0 object-show
    $D 0 object-show
    $E 0 object-show
    0 room40.cmd00
    0 0 $20000 nav-group
    0 1 $14 door-bits
    9 1 object-show
    $A 1 object-show
    0 ebit-set
    1 6.2 18.0 17.1 flicker-sprite
    $85 story-flag-set
    $5D door-unlock
    $F $51 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room40.act01 ( -- )   \ 00406AF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
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
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 $F9 3 action
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
    $F9 action-end
    0 8 var? if
        8 item-use
    else 0 9 var? if
        9 item-use
    else 0 10 var? if
        $A item-use
    then then then
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    $B 0 object-show
    $C 0 object-show
    $D 0 object-show
    $E 0 object-show
    1 room40.cmd00
    0 self-move-16
    0 $158 5.733 23.039 180 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    $F $51 fade
    wait-fade
    $38 resident-flag-set
    $902 self-anim
    self-wait-anim
    $10 message-param-room
    $10 1 item-give-count
    0 $10 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $361 story-flag-set
    $903 self-anim
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room40.act02 ( -- )   \ 00406C30
    begin
        4 1 object-show
        5 1 object-show
        6 1 object-show
        0 11 var? if
            7 0 object-show
            8 1 object-show
        else
            7 1 object-show
            8 0 object-show
        then
        yield
    again
;

: room40.act03 ( -- )   \ 00406C60
    begin
        $1CC cutscene-cue-reached? not while
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        0 8 var? if
            4 0 object-show
        else 0 9 var? if
            5 0 object-show
        else 0 10 var? if
            6 0 object-show
        then then then
        yield
    repeat
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    self-idle-or-end
;

: room40.act04 ( -- )   \ 00406CC0
    self-wait-done
    0 ebit? not if
        4.0 17.0 self-turn-to-xz
        self-wait-done
        1 ebit? not if
            0 message
            wait-message
            1 ebit-set
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
        0 answer? if
            $158 4.619 23.5 180 $FFFF 5 self-move-to
            self-wait-done
            $902 self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            0 ebit-clear
            1 effect-remove
            1 room40.cmd00
            0 11 var? if
                7 1 object-show
                $B message-param-room
                $B 1 item-give-count
                0 $B item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
            else
                8 1 object-show
                $C message-param-room
                $C 1 item-give-count
                0 $C item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
            then
            $903 self-anim
            self-wait-anim
        then
    then
    self-idle-or-end
;

: room40.act05 ( -- )   \ 00406D70
    self-wait-done
    2 ebit? not if
        0 self-turn-angle
        self-wait-done
        $A00 self-anim
        self-wait-anim
        3 message
        wait-message
        $18 state-flag-set
        1 self-scripted
        $F 6 fade
        wait-fade
        4 message
        wait-message
        2 ebit-set
        $F 7 fade
        wait-fade
        $246 item-give
        $18 state-flag-clear
        0 self-scripted
    else
        $1B9 -3.806 112.773 0 $FFFF 5 self-move-to
        self-wait-done
        $1200 self-anim
        self-wait-anim
        $1203 self-anim
        self-wait-anim
        self-frames-reset
        self-wait-16
        $1202 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        5 message
        wait-message
        2 ebit-clear
    then
    self-idle-or-end
;

: room40.act06 ( -- )   \ 00406DD0
    self-wait-done
    -72.34 -34.25 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $27A story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room40.act07 ( -- )   \ 00406E30
    self-wait-done
    2 -19.7 18.3 22.5 0 effect-86
    3 -19.7 18.3 25.0 0 effect-86
    4 -17.2 18.3 22.4 0 effect-86
    5 -17.2 18.3 25.0 0 effect-86
    6 17.2 18.3 22.4 0 effect-86
    7 17.2 18.3 25.0 0 effect-86
    8 19.7 18.3 22.5 0 effect-86
    9 19.7 18.3 25.0 0 effect-86
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
    0 11 var-set
    0 $F9 2 action
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

: room40.act08 ( -- )   \ 00406F40
    self-wait-done
    2 -19.7 18.3 22.5 0 effect-86
    3 -19.7 18.3 25.0 0 effect-86
    4 -17.2 18.3 22.4 0 effect-86
    5 -17.2 18.3 25.0 0 effect-86
    6 17.2 18.3 22.4 0 effect-86
    7 17.2 18.3 25.0 0 effect-86
    8 19.7 18.3 22.5 0 effect-86
    9 19.7 18.3 25.0 0 effect-86
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
    0 9 var-set
    0 $F9 3 action
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

' room40.enter $40 0 room-script!
' room40.char-enter $40 6 room-script!
' room40.phase1 $40 1 room-script!
' room40.phase2 $40 2 room-script!
' room40.act00 $40 $00 action-script!
' room40.act01 $40 $01 action-script!
' room40.act02 $40 $02 action-script!
' room40.act03 $40 $03 action-script!
' room40.act04 $40 $04 action-script!
' room40.act05 $40 $05 action-script!
' room40.act06 $40 $06 action-script!
' room40.act07 $40 $07 action-script!
' room40.act08 $40 $08 action-script!

\ ---- room $41 ----------------------------------------------------------------------------------

: room41.enter ( -- )   \ 0041CA90
    room-sounds
    $1C $60 char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 6.38 0.0 -112.79 170 char-to-xyz
        $1C $9002 1 0 char-anim-hold
    then
    0 $F1 1 action
    $22 state-flag-set
    $322 story-flag? if
        1 char-here? 1 0 char-in-nav-group? and if
            1 $13 char-to-tri
        then
        0 1 $14 door-bits
        1 0 $20000 nav-group
    then
    $277 story-flag? not if
        0 -10.64 9.5 -116.64 flicker-sprite
    then
    $2BF story-flag? $2C0 story-flag? not and if
        1 -19.52 1.0 -13.68 flicker-sprite
    then
    1 $2300 sound-volume
;

: room41.char-enter ( -- )   \ 0041CB20
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    0 3 -1 area-camera
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
    0 self-is? if
        $80 exit-taken? if
            0 $1A 160 char-to-tri-facing
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
    then
;

: room41.phase1 ( -- )   \ 0041CC00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 1 1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 3 -1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 3 char-entered-area? 0 5 char-entered-area? or if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 6.38 0.0 -112.79 5 10 1 zone
    $FF 0 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 7 var? if
        $1C 2 6 char-sound
    then
    1 47 var? if
        $1C 2 6 char-sound
    then
    $2BF story-flag? not if
        4 -19.52 0.0 -13.68 $A 5 0 zone
        1 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 703 var-set
                $1A 704 var-set
                $1B 1 var-set
                $1C -19520 var-set
                $1D 1000 var-set
                $1E -13680 var-set
                $1F 4 var-set
                0 1 $8B action
            then
        then
    then
    1 -25.44 0.0 -54.79 $C 10 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    2 7.31 0.0 -110.05 $1E 6 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    2 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 7.31 0.0 -110.05 $1E 6 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: room41.phase2 ( -- )   \ 0041CD90
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    0 $D char-in-area? 0 -70 $3C char-heading? and if
        5 5 0 scene-change
    then
    $277 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $2BF story-flag? $2C0 story-flag? not and if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: room41.phase3 ( -- )   \ 0041CDF0
    -10.0 12.0 -89.0 6.0 12.0 -89.0 -10.0 0.0 -89.0 6.0 0.0 -89.0 lights-doorway
    -37.0 8.0 -83.5 -31.0 8.0 -83.5 -37.0 0.0 -83.5 -31.0 0.0 -83.5 lights-doorway
    -37.0 17.0 -83.5 -31.0 17.0 -83.5 -37.0 8.0 -83.5 -31.0 8.0 -83.5 lights-doorway
    -40.0 8.0 -83.5 -34.0 8.0 -83.5 -40.0 0.0 -83.5 -34.0 0.0 -83.5 lights-doorway
    -40.0 17.0 -83.5 -34.0 17.0 -83.5 -40.0 8.0 -83.5 -34.0 8.0 -83.5 lights-doorway
;

: room41.phase5 ( -- )   \ 0041CEE8
    0 2 char-in-area? if
        $1C action-end
        $1C char-done
    then
;

: room41.act00 ( -- )   \ 0041CF00
    self-wait-done
    $FE self-touching? not if
        $56 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $56 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $56 door-unlock
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

: room41.act01 ( -- )   \ 0041CF40
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 6.38 0.0 -112.79 170 char-to-xyz
        $1C $9002 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            1 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9003 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9002 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9002 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: room41.act02 ( -- )   \ 0041D000
    self-wait-done
    fiona-recover
    2 panic-stage
    $FE 3 stalker-mode
    $B01 0 self-anim-blend
    yield
    $F $41 fade
    $B02 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-fade
    self-idle-or-end
;

: room41.act03 ( -- )   \ 0041D020
    self-wait-done
    -10.64 -116.64 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $277 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: room41.act04 ( -- )   \ 0041D080
    self-wait-done
    -19.52 -13.68 self-turn-to-xz
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
            $2C0 story-flag-set
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

: room41.act05 ( -- )   \ 0041D0E0
    self-wait-done
    2.0 -125.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message
    wait-message
    self-idle-or-end
;

' room41.enter $41 0 room-script!
' room41.char-enter $41 6 room-script!
' room41.phase1 $41 1 room-script!
' room41.phase2 $41 2 room-script!
' room41.phase3 $41 3 room-script!
' room41.phase5 $41 5 room-script!
' room41.act00 $41 $00 action-script!
' room41.act01 $41 $01 action-script!
' room41.act02 $41 $02 action-script!
' room41.act03 $41 $03 action-script!
' room41.act04 $41 $04 action-script!
' room41.act05 $41 $05 action-script!

\ ---- room $42 ----------------------------------------------------------------------------------

: room42.enter ( -- )   \ 004070F0
    room-sounds
    $12 1.0 0 bgm
    $77 story-flag? not if
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $1C 3 char-to-tri
    2 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
    then
    0 $F1 4 action
    $22 state-flag-set
    $278 story-flag? not if
        0 -51.07 1.0 19.59 flicker-sprite
    then
    1 $2300 sound-volume
    1 $77 $10000000 nav-tri-flags
    1 $48 $10000000 nav-tri-flags
    1 $75 $10000000 nav-tri-flags
    1 $46 $10000000 nav-tri-flags
    1 $52 $10000000 nav-tri-flags
    1 $81 $10000000 nav-tri-flags
    1 2 $10000000 nav-tri-flags
    1 $61 $10000000 nav-tri-flags
    1 $70 $10000000 nav-tri-flags
    1 $3E $10000000 nav-tri-flags
    1 $39 $10000000 nav-tri-flags
    1 0 $10000000 nav-tri-flags
    1 $71 $10000000 nav-tri-flags
    1 $3F $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $7A $10000000 nav-tri-flags
    1 $4A $10000000 nav-tri-flags
    1 $79 $10000000 nav-tri-flags
    1 $45 $10000000 nav-tri-flags
    1 $74 $10000000 nav-tri-flags
    1 $5F $10000000 nav-tri-flags
    1 $85 $10000000 nav-tri-flags
    1 $51 $10000000 nav-tri-flags
    1 $80 $10000000 nav-tri-flags
    1 $4C $10000000 nav-tri-flags
    1 $7B $10000000 nav-tri-flags
    1 $3C $10000000 nav-tri-flags
    1 $6F $10000000 nav-tri-flags
    1 $4E $10000000 nav-tri-flags
    1 $7D $10000000 nav-tri-flags
    1 $86 $10000000 nav-tri-flags
    1 $60 $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $83 $10000000 nav-tri-flags
    1 6 $10000000 nav-tri-flags
    1 $63 $10000000 nav-tri-flags
    1 $14 $10000000 nav-tri-flags
    1 $6D $10000000 nav-tri-flags
    1 $4D $10000000 nav-tri-flags
    1 $7C $10000000 nav-tri-flags
    1 $18 $10000000 nav-tri-flags
    1 $23 $10000000 nav-tri-flags
    1 $41 $10000000 nav-tri-flags
    1 $72 $10000000 nav-tri-flags
    1 $76 $10000000 nav-tri-flags
    1 $47 $10000000 nav-tri-flags
    1 $78 $10000000 nav-tri-flags
    1 $49 $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
    1 $82 $10000000 nav-tri-flags
    1 $5A $10000000 nav-tri-flags
    1 $84 $10000000 nav-tri-flags
    1 $3B $10000000 nav-tri-flags
;

: room42.char-enter ( -- )   \ 00407300
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

: room42.phase1 ( -- )   \ 00407340
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 2 2 1 chars-area-camera
    0 55.0 0.0 -20.0 5 10 1 zone
    $FF 0 char-in-zone? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 21 var? if
        $1C 2 6 char-sound
    then
    2 66 var? if
        $1C 2 6 char-sound
    then
    1 45.1 0.0 31.88 $1C 18 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    2 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
;

: room42.phase2 ( -- )   \ 00407430
    0 5 char-in-area? 0 20 $32 char-heading? and if
        5 2 0 scene-change
    then
    $278 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    0 7 char-in-area? 0 90 $3C char-heading? and if
        5 6 0 scene-change
    then
    0 8 char-in-area? 0 90 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 9 char-in-area? 0 90 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 $A char-in-area? 0 0 $3C char-heading? and if
        5 9 0 scene-change
    then
    0 $B char-in-area? 0 0 $3C char-heading? and if
        5 $A 0 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        5 9 0 scene-change
    then
    0 $D char-in-area? 0 90 $3C char-heading? and if
        5 $A 0 scene-change
    then
    0 $E char-in-area? 0 0 $3C char-heading? and if
        5 $B 0 scene-change
    then
    0 $F char-in-area? 0 0 $3C char-heading? and if
        5 $C 0 scene-change
    then
    0 $10 char-in-area? 0 0 $3C char-heading? and if
        5 $D 0 scene-change
    then
;

: room42.act0E ( -- )   \ 00407A20
    4 6 -30.0 10.0 20.0 0 0 sound
    self-frames-reset
    self-wait-16
    0 $43 5 char-sound
    $3C threat-raise
    1 $FF 8 rumble
    $F04 self-anim
    self-wait-anim
    0 0 $14 door-bits
    1 1 $14 door-bits
    $42 0 105 $80 5 -1 $68 40.0 creature-place
    $42 0 23 $80 10 -1 $5A 0.0 creature-place
    $77 story-flag-set
    exit
;

: room42.act00 ( -- )   \ 004074F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    5 message
    wait-message
    $902 self-anim
    self-wait-anim
    $B item-use
    $C message-param-room
    $C 1 item-give-count
    0 $C item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $360 story-flag-set
    $903 self-anim
    self-wait-anim
    $77 story-flag? not if
        room42.act0E
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room42.act01 ( -- )   \ 00407560
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    2 message
    wait-message
    $77 story-flag? not if
        room42.act0E
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room42.act02 ( -- )   \ 00407680
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        0 message
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        1 message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $245 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room42.act03 ( -- )   \ 004076D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    $F 6 fade
    wait-fade
    4 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room42.act04 ( -- )   \ 00407700
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        1 ebit? if
            2 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9001 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9000 0 3 char-anim-hold
            2 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9000 0 0 char-anim-hold
            2 0 var-set
        else
            2 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: room42.act05 ( -- )   \ 004077C0
    self-wait-done
    -51.07 19.59 self-turn-to-xz
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
            $278 story-flag-set
            0 effect-remove
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

: room42.act06 ( -- )   \ 00407820
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $62 6.25 -23.4 173 char-to-xz
    $FF 3 -1 char-camera
    self-frames-reset
    4 self-wait-frames
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room42.act07 ( -- )   \ 00407880
    self-wait-done
    -20.0 -40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room42.act08 ( -- )   \ 004078B0
    self-wait-done
    -48.0 -40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room42.act09 ( -- )   \ 004078E0
    self-wait-done
    -10.0 0.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room42.act0A ( -- )   \ 00407910
    self-wait-done
    -40.0 0.0 self-turn-to-xz
    self-wait-done
    $77 story-flag? not if
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 ebit? not if
            6 message
            wait-message
            3 ebit-set
        else
            7 message
            wait-message
        then
        self-wait-anim
    else
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        8 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: room42.act0B ( -- )   \ 00407950
    self-wait-done
    8.0 40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room42.act0C ( -- )   \ 00407980
    $77 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        0 $50 -17.5 26.5 -12 char-to-xz
        $FF 4 -1 char-camera
        self-frames-reset
        4 self-wait-frames
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 ebit? not if
            6 message
            wait-message
            3 ebit-set
        else
            7 message
            wait-message
        then
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        8 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: room42.act0D ( -- )   \ 004079F0
    self-wait-done
    -48.0 40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

' room42.enter $42 0 room-script!
' room42.char-enter $42 6 room-script!
' room42.phase1 $42 1 room-script!
' room42.phase2 $42 2 room-script!
' room42.act00 $42 $00 action-script!
' room42.act01 $42 $01 action-script!
' room42.act02 $42 $02 action-script!
' room42.act03 $42 $03 action-script!
' room42.act04 $42 $04 action-script!
' room42.act05 $42 $05 action-script!
' room42.act06 $42 $06 action-script!
' room42.act07 $42 $07 action-script!
' room42.act08 $42 $08 action-script!
' room42.act09 $42 $09 action-script!
' room42.act0A $42 $0A action-script!
' room42.act0B $42 $0B action-script!
' room42.act0C $42 $0C action-script!
' room42.act0D $42 $0D action-script!
' room42.act0E $42 $0E action-script!

\ ---- room $43 ----------------------------------------------------------------------------------

\ the effect BigFire_vtable (three quad drawers) started with parameter 0
: room43.cmd00 ( -- )  s" room43.cmd00" stub-step ;

: room43.enter ( -- )   \ 00407AE0
    room-sounds
    $19 1.0 0 bgm
    1 1 $300000 nav-group
    $26E story-flag? not if
        0 122.09 8.28 10.26 flicker-sprite
    then
    $26F story-flag? not if
        1 -107.57 1.0 10.1 flicker-sprite
    then
    $270 story-flag? not if
        2 -140.37 16.0 -25.97 flicker-sprite
    then
    8 1 item-count? not 9 1 item-count? not and $A 1 item-count? not and $B 1 item-count? not and $C 1 item-count? not and if
        0 ebit-set
        3 -111.0 17.0 -115.0 flicker-sprite
    then
    $2AB story-flag? $2AC story-flag? not and if
        4 -9.11 1.0 -21.78 flicker-sprite
    then
    room43.cmd00
;

: room43.char-enter ( -- )   \ 00407B70
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 4 4 char-camera
                0 camera-follow
            else
                1 4 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
    then then
    3 4 4 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    4 2 2 area-camera
    0 self-is? if
        3 exit-taken? if
            2 map-page
        then
    then
;

: room43.phase1 ( -- )   \ 00407C00
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 -138.0 11.0 11.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000000 6 -138.0 11.0 11.0 0 0 sound
    then
    3 exit-usable? if
        3 exit-check
    then
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    $2AB story-flag? not if
        3 -9.11 0.0 -21.78 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 683 var-set
                $1A 684 var-set
                $1B 4 var-set
                $1C -9110 var-set
                $1D 1000 var-set
                $1E -21780 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: room43.phase2 ( -- )   \ 00407CB0
    0 $19 char-in-area? 0 -45 $32 char-heading? and if
        5 0 0 scene-change
    then
    $26F story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $2AB story-flag? $2AC story-flag? not and if
        3 4 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: room43.act00 ( -- )   \ 00407D00
    self-wait-done
    -136.0 12.0 self-turn-to-xz
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    $21 0 pvar? if
        $21 pvar-inc
        $11 message
        wait-message
    else $21 1 pvar? if
        1 6 -138.0 5.0 5.0 0 0 sound
        self-frames-reset
        $10 self-wait-frames
        0 $43 5 char-sound
        $F00 self-anim
        $21 pvar-inc
        $FF panic-stage? not if
            3 panic-stage
        then
        $12 message
        wait-message
    else
        $21 pvar-inc
        $13 message
        wait-message
    then then
    self-idle-or-end
;

: room43.act01 ( -- )   \ 0047AB40
    self-idle-or-end
;

: room43.act02 ( -- )   \ 0047AB44
    self-idle-or-end
;

: room43.act03 ( -- )   \ 00407D60
    self-wait-done
    -107.57 10.1 self-turn-to-xz
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
            $26F story-flag-set
            1 effect-remove
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

: room43.act04 ( -- )   \ 0047AB48
    self-idle-or-end
;

: room43.act05 ( -- )   \ 00407DC0
    self-wait-done
    -9.11 -21.78 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $2AC story-flag-set
            4 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: room43.phase5 ( -- )   \ 0047AB3C
;

' room43.enter $43 0 room-script!
' room43.char-enter $43 6 room-script!
' room43.phase1 $43 1 room-script!
' room43.phase2 $43 2 room-script!
' room43.act00 $43 $00 action-script!
' room43.act01 $43 $01 action-script!
' room43.act02 $43 $02 action-script!
' room43.act03 $43 $03 action-script!
' room43.act04 $43 $04 action-script!
' room43.act05 $43 $05 action-script!
' room43.phase5 $43 5 room-script!

\ ---- room $44 ----------------------------------------------------------------------------------

: room44.enter ( -- )   \ 00415BA0
    $264 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $28F story-flag? not if
            0 60.5 0.9 -33.4 flicker-sprite
        then
    then
    0 6 0.812 0.687 0.187 0.312 zone-rect
    $1C $C5 char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 39.3 0.0 50.4 -90 char-to-xyz
        $1C $900A 1 0 char-anim-hold
    then
    0 $F1 0 action
    $22 state-flag-set
    $2C6 story-flag? not if
        1 -6.17 39.94 -29.06 flicker-sprite
    then
    1 $2300 sound-volume
    1 1 8 nav-group
    1 $51 $10000000 nav-tri-flags
    1 $50 $10000000 nav-tri-flags
    1 $EF $10000000 nav-tri-flags
    1 $DB $10000000 nav-tri-flags
    1 $FA $10000000 nav-tri-flags
    1 $F9 $10000000 nav-tri-flags
    1 $55 $10000000 nav-tri-flags
    1 $DA $10000000 nav-tri-flags
    1 $F3 $10000000 nav-tri-flags
    1 $F8 $10000000 nav-tri-flags
;

: room44.char-enter ( -- )   \ 00415C90
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    1 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 4 char-camera
                0 camera-follow
            else
                1 4 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
    then then
    2 4 4 area-camera
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
            8 state-flag-set
            0.0 sound-volume-scale
            0 0 2 action
        else
            room-sounds
            $13 1.0 0 bgm
        then
        1 exit-taken? 2 exit-taken? or if
            1 map-page
        then
        0 exit-taken? if
            2 map-page
        then
    then
;

: room44.phase1 ( -- )   \ 00415DB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        3 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    1 39.3 0.0 50.4 5 10 1 zone
    $FF 1 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 0 var? not if
        2 var-inc
        2 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            2 0 var-set
        then
    then
    0 5 char-entered-area? if
        0 map-page
    then
    0 5 char-left-area? if
        1 map-page
    then
    0 $A char-entered-area? if
        1 map-page
    then
    0 $A char-left-area? if
        2 map-page
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        2 game-mode? not if
            0 hewie-side? 0 $200000 char-on-nav-flags? and if
                -6 40 -29 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                    0 -6 -29 $3C char-faces-xz? if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 3 action
                            then
                        then
                    then
                then
            then
        then
    then
    2 38.73 0.0 50.28 $1A 16 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    1 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 38.73 0.0 50.28 $1A 16 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $2C6 story-flag? not if
        3 -9.31 39.5 -32.27 $32 13 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        3 -9.31 39.5 -32.27 $32 13 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    $1F 3 var-set
                    $1F 0.0 hewie-look-zone
                then
            then
        then
    then
;

: room44.phase2 ( -- )   \ 00415FA0
    $264 story-flag? not if
        0 60.5 -0.1 -33.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $264 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            60.5 -0.1 -33.4 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 60.5 -0.1 -33.4 0 0 sound
            $40 $123 noise
            0 60.5 0.9 -33.4 flicker-sprite
        then
    else $28F story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
;

: room44.phase3 ( -- )   \ 00416030
    37.0 -50.0 50.0 82.5 -50.0 50.0 37.0 -90.0 50.0 82.5 -90.0 50.0 lights-doorway
    52.0 -32.0 45.0 68.0 -32.0 45.0 52.0 -90.0 45.0 68.0 -90.0 45.0 lights-doorway
    -29.0 -4.0 50.0 37.0 -48.0 50.0 -8.0 -80.0 50.0 37.0 -80.0 50.0 lights-doorway
    5.0 0.0 -40.0 -65.0 0.0 -40.0 5.0 -80.0 -40.0 -65.0 -80.0 -40.0 lights-doorway
    -50.0 18.5 -40.0 -50.0 18.5 5.5 -50.0 -50.0 -40.0 -50.0 -50.0 5.5 lights-doorway
;

: room44.phase5 ( -- )   \ 00416130
    $1C action-end
    $1C char-done
    $18 state-flag? if
        1 action-end
        1 $76 -40.52 -28.0 -90 char-to-xz
        $18 state-flag-clear
    then
;

: room44.act00 ( -- )   \ 00416150
    0 exit-taken? 3 exit-taken? or if
        3 $1C char-loaded? not if
            $1C 3 $FF char-load
            3 char-unload
            $1C char-activate
            $1C 39.3 0.0 50.4 -90 char-to-xyz
            $1C $900A 1 0 char-anim-hold
            $1C 1 char-visible
        then
        begin
            0 ebit? if
                1 0 var-set
                0 var-inc
                0 3 var? if
                    $21 chance? if
                        0 5 var-set
                    then
                then
                0 4 var? if
                    $32 chance? if
                        0 5 var-set
                    then
                then
                0 5 var? if
                    0 panic-stage? if
                        1 panic-stage
                    else
                        $F threat-raise
                    then
                    $FE 0 stalker-mode
                    $1C 0 6 char-sound
                else
                    $1C 1 6 char-sound
                then
                0 $8F 5 char-sound
                $1C $900B 0 3 char-anim-hold
                $1C wait-char-anim
                $1C $900A 0 3 char-anim-hold
                1 0 var-set
                0 5 var? if
                    $1C wait-char-anim
                    0 0 var-set
                then
            then
            $1C char-at-motion-event? if
                $1C $900A 0 0 char-anim-hold
                1 0 var-set
            else
                1 var-inc
            then
            self-frames-reset
            1 self-wait-frames
        again
    else
        3 $1C char-loaded? not if
            $1C 3 $FF char-load
            3 char-unload
            $1C char-activate
            $1C 39.3 0.0 50.4 -90 char-to-xyz
            $1C $900A 1 0 char-anim-hold
            $1C 0 char-visible
        then
        begin
            0 ebit? if
                1 0 var-set
                0 var-inc
                0 3 var? if
                    $21 chance? if
                        0 5 var-set
                    then
                then
                0 4 var? if
                    $32 chance? if
                        0 5 var-set
                    then
                then
                0 5 var? if
                    0 panic-stage? if
                        1 panic-stage
                    else
                        $F threat-raise
                    then
                    $FE 0 stalker-mode
                    $1C 0 6 char-sound
                else
                    $1C 1 6 char-sound
                then
                0 $8F 5 char-sound
                $1C $900B 0 3 char-anim-hold
                $1C wait-char-anim
                $1C $900A 0 3 char-anim-hold
                1 0 var-set
                0 5 var? if
                    $1C wait-char-anim
                    0 0 var-set
                then
            then
            $1C char-at-motion-event? if
                $1C $900A 0 0 char-anim-hold
                1 0 var-set
            else
                1 var-inc
            then
            self-frames-reset
            1 self-wait-frames
        again
    then
    self-idle-or-end
;

: room44.act01 ( -- )   \ 004162E0
    self-wait-done
    60.5 -33.4 self-turn-to-xz
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
            $28F story-flag-set
            0 effect-remove
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

: room44.act02 ( -- )   \ 00416340
    1 self-scripted
    self-wait-done
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    effects-arena-flip
    0 char-in
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    0 1 char-to-exit
    1 char-activate
    $44 1 161 hewie-to-room
    1 1 char-to-exit
    $FE $47 -1 2 stalker-to-room
    stalker-item-cooldown
    $FE 0 stalker-mode
    hewie-controlled? not if
        0 3 3 char-camera
        0 camera-follow
    else
        1 3 3 char-camera
        1 camera-follow
    then
    yield
    camera-restart
    room-sounds
    $13 1.0 0 bgm
    $F $51 fade
    wait-fade
    $23F item-give
    $12 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room44.act03 ( -- )   \ 004163F0
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    hewie-bark
    self-wait-done
    $C -59.57 -30.97 90 $FFFF $A self-move-to
    self-wait-done
    0 1 8 nav-group
    1 1 $30 nav-group
    $129 -11.17 -29.06 0.3 50 hewie-go-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $2C6 story-flag? not if
        $A state-flag? 0 char-busy? not or if
            $71 $63 item-count? not if
                10 hewie-trust
            then
            $71 message-param-room
            $71 $63 item-count? if
                $8010 message
                wait-message
            else
                $2C6 story-flag-set
                $71 1 item-give-count
                0 $71 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            $2C6 story-flag? if
                1 effect-remove
            then
        then
    then
    $129 -11.46 -30.1 -90 $FFFF 5 self-move-to
    self-wait-done
    $C -59.57 -30.97 2.0 50 hewie-go-to
    self-wait-done
    1 1 8 nav-group
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

' room44.enter $44 0 room-script!
' room44.char-enter $44 6 room-script!
' room44.phase1 $44 1 room-script!
' room44.phase2 $44 2 room-script!
' room44.phase3 $44 3 room-script!
' room44.phase5 $44 5 room-script!
' room44.act00 $44 $00 action-script!
' room44.act01 $44 $01 action-script!
' room44.act02 $44 $02 action-script!
' room44.act03 $44 $03 action-script!

\ ---- room $45 ----------------------------------------------------------------------------------

: room45.enter ( -- )   \ 00407E70
    1 1 $14 door-bits
    1 1 $20000 nav-group
    1 3 $300000 nav-group
;

: room45.char-enter ( -- )   \ 00407E90
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
;

: room45.phase2 ( -- )   \ 00407ED0
    $45 story-flag? if
        0 $10 char-in-area? 0 0 $32 char-heading? and if
            5 0 0 scene-change
        then
        0 $11 char-in-area? 0 90 $32 char-heading? and if
            5 1 0 scene-change
        then
    then
;

: room45.phase1 ( -- )   \ 0047AB50
    0 exit-usable? if
        0 exit-check
    then
;

: room45.phase5 ( -- )   \ 0047AB58
;

: room45.act00 ( -- )   \ 00407EF8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room45.act01 ( -- )   \ 00407F08
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

' room45.enter $45 0 room-script!
' room45.char-enter $45 6 room-script!
' room45.phase2 $45 2 room-script!
' room45.phase1 $45 1 room-script!
' room45.phase5 $45 5 room-script!
' room45.act00 $45 $00 action-script!
' room45.act01 $45 $01 action-script!

\ ---- room $46 ----------------------------------------------------------------------------------

\ Room46_Cmd00
: room46.cmd00 ( b0 -- )  drop s" room46.cmd00" stub-step ;
\ Room46_Cond00
: room46.cond00? ( -- flag )  s" room46.cond00?" stub-flag ;

: room46.enter ( -- )   \ 00416A50
    room-sounds
    $1C $BB char-to-tri
    3 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 20.0 0.0 -105.0 0 char-to-xyz
        $1C $9006 1 0 char-anim-hold
    then
    0 $F1 0 action
    $22 state-flag-set
    $275 story-flag? not if
        0 -119.67 9.0 151.64 flicker-sprite
    then
    $56 story-flag? not $7B story-flag? not and if
        1 exit-taken? if
            4 char-unload
            $21 $FB char-to-tri
            $21 char-activate
            $21 -97.0 0.0 84.0 180 char-to-xyz
            $21 0 1 0 char-anim-hold
            0 $21 2 action-force
        else
            $7B story-flag-set
        then
    then
    1 -122.2 13.1 139.0 0 effect-86
    2 -120.5 12.8 140.35 0 effect-86
    1 $2300 sound-volume
;

: room46.act07 ( -- )   \ 00417050
    7 ebit-set
    6 ebit-set
    5 ebit-clear
    $22 0 pvar? if
        $A chance? if
            5 ebit-set
        then
    else $22 1 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else $22 2 pvar? if
        $32 chance? if
            5 ebit-set
        then
    else $22 3 pvar? if
        $4B chance? if
            5 ebit-set
        then
    then then then then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 $FE 8 action
    else
        $78 1 item-cooldown
        8 ebit-clear
    then
    0 stalker-alert? if
        $FE 2 stalker-mode
    then
    $22 pvar-inc
    exit
;

: room46.char-enter ( -- )   \ 00416AF0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    1 3 -1 area-camera
    0 self-is? if
        1 exit-taken? if
            2 map-page
        then
    then
    $FE self-is? if
        9 state-flag? if
            6 ebit-set
            room46.act07
        else
            6 ebit-clear
        then
    then
;

: room46.phase1 ( -- )   \ 00416B90
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 1 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    9 3 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $E char-entered-area? if
        1 map-page
    then
    0 $E char-left-area? if
        2 map-page
    then
    $56 story-flag? not $7B story-flag? not and if
        0 $B char-entered-area? if
            2 ebit-set
        then
    then
    0 $A char-entered-area? if
        $7B story-flag? not if
            4 0 char-remove
            $46 0 82 $80 8 -1 $B4 0.0 creature-place
            $7B story-flag-set
        then
    then
    $FE 2 char-C4? not if
        9 state-flag? 7 ebit? not and $FE char-here? and if
            $FE char-busy? not if
                8 ebit-set
                room46.act07
            then
        then
    then
    0 20.0 0.0 -105.0 5 10 1 zone
    $FF 0 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    3 0 var? not if
        4 var-inc
        4 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            4 0 var-set
        then
    then
    1 20.8 0.0 -104.26 $1E 15 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $50 chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 20.8 0.0 -104.26 $1E 15 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    3 1 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    4 2 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
;

: room46.phase2 ( -- )   \ 00416D60
    $275 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    0 $F char-in-area? 0 -45 $3C char-heading? and if
        $FE char-here? not if
            5 5 5 scene-change
        else room46.cond00? not if
            5 5 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    0 $C $3C char-faces-area? if
        5 3 0 scene-change
    then
    0 $D char-in-area? 0 45 $3C char-heading? and if
        5 4 0 scene-change
    then
;

: room46.phase3 ( -- )   \ 00416DB0
    4 camera-mode? not if
        -111.5 18.0 114.0 -94.0 18.0 93.0 -111.5 0.0 114.0 -94.0 0.0 93.0 lights-doorway
    then
;

: room46.act00 ( -- )   \ 00416DF0
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 20.0 0.0 -105.0 0 char-to-xyz
        $1C $9006 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            3 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9007 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9006 0 3 char-anim-hold
            3 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9006 0 0 char-anim-hold
            3 0 var-set
        else
            3 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: room46.act01 ( -- )   \ 00416EB0
    self-wait-done
    -119.67 151.64 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $275 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: room46.act02 ( -- )   \ 00416F10
    0 room46.cmd00
    begin
        2 ebit? not while
        yield
    repeat
    $21 $200 1 4 char-anim-hold
    1 0 var-set
    2 0 var-set
    begin
        1 room46.cmd00
        3 ebit? not while
        yield
    repeat
    begin
        2 room46.cmd00
        1 0 var? while
        yield
    repeat
    self-idle-or-end
;

: room46.act03 ( -- )   \ 00416F50
    self-wait-done
    20.0 -120.0 self-turn-to-xz
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room46.act04 ( -- )   \ 00416F60
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room46.act06 ( -- )   \ 00417020
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
    4 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room46.act05 ( -- )   \ 00416F80
    $18 state-flag-set
    6 ebit-clear
    1 self-scripted
    7 ebit-clear
    0 0 char-file-load
    self-wait-done
    $FF self-look-at
    yield
    0 char-file-use
    $92 $8004 5 -110.16 146.09 -90 self-walk-anim
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
    4 ebit-clear
    0 avoid-prompt
    $FF 4 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room46.act06 goto
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
                ['] room46.act06 goto
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

: room46.act08 ( -- )   \ 004170B0
    self-wait-done
    8 ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
    else $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
        0 3 self-move-slot
        self-wait-done
    then then
    8 ebit-clear
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $20 -100.0 146.09 -90 $FFFF 5 self-move-to
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

: room46.phase5 ( -- )   \ 0047AC40
    $1C action-end
    $1C char-done
;

' room46.enter $46 0 room-script!
' room46.char-enter $46 6 room-script!
' room46.phase1 $46 1 room-script!
' room46.phase2 $46 2 room-script!
' room46.phase3 $46 3 room-script!
' room46.act00 $46 $00 action-script!
' room46.act01 $46 $01 action-script!
' room46.act02 $46 $02 action-script!
' room46.act03 $46 $03 action-script!
' room46.act04 $46 $04 action-script!
' room46.act05 $46 $05 action-script!
' room46.act06 $46 $06 action-script!
' room46.act07 $46 $07 action-script!
' room46.act08 $46 $08 action-script!
' room46.phase5 $46 5 room-script!

\ ---- room $47 ----------------------------------------------------------------------------------

: room47.enter ( -- )   \ 00424B50
    room-sounds
    $2BB story-flag? $2BC story-flag? not and if
        0 -30.81 1.0 -21.85 flicker-sprite
    then
    1 $2300 sound-volume
;

: room47.char-enter ( -- )   \ 00424B70
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
    0 self-is? if
        $80 exit-taken? if
            $82 story-flag? not if
                8 state-flag-set
                0.0 sound-volume-scale
                0 0 0 action
                $82 story-flag-set
            then
        then
    then
;

: room47.phase1 ( -- )   \ 00424C10
    4 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 0.0 11.0 0.0 0 0 sound
            4 ebit-set
        then
    else
        $C0000000 6 0.0 11.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    2 -68.08 0.0 55.74 $1E 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    0 -1.06 0.0 -0.54 $26 17 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    2 ebit-set
                    $32 chance? if
                        $1F 0 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    0 -1.06 0.0 -0.54 $26 17 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 17.0 hewie-look-zone
            then
        then
    then
    $2BB story-flag? not if
        1 -30.81 0.0 -21.85 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 699 var-set
                $1A 700 var-set
                $1B 0 var-set
                $1C -30810 var-set
                $1D 1000 var-set
                $1E -21850 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
;

: room47.phase2 ( -- )   \ 00424D70
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 0 0 $32 char-faces-xz? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 0 0 $32 char-faces-xz? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 -25 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 $C char-in-area? 0 45 $3C char-heading? and if
        5 7 0 scene-change
    then
    $2BB story-flag? $2BC story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room47.act00 ( -- )   \ 00424DE0
    $18 state-flag-set
    1 self-scripted
    1 action-end
    1 char-done
    2 0 char-remove
    self-wait-done
    4 partner-load
    2 char-unload
    $28 state-flag-clear
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
    $FE $47 145 2 stalker-to-room
    3 5 0 char-model-op
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    $FE $91 -13.81 11.69 125 char-to-xz
    0 exit-prepare
    $80 exit-check
    music-stage-end
    2 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    $44 door-lock
    $47 door-lock
    $56 door-lock
    $5D door-lock
    $7D door-lock
    $34 door-lock
    $79 door-close-off-lock
    exits-rebuild
    $FE $60 1 room-doors-state
    $FE $63 1 room-doors-state
    $FE $10B 1 room-doors-state
    0 state-flag-set
    stalker-item-cooldown
    self-idle-or-end
;

: room47.act01 ( -- )   \ 00424ED0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message
    wait-message
    $902 self-anim
    self-wait-anim
    8 item-use
    9 message-param-room
    9 1 item-give-count
    0 9 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room47.act02 ( -- )   \ 00424F40
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    0 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    3 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room47.act03 ( -- )   \ 00425060
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        1 message
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        2 message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $242 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room47.act04 ( -- )   \ 004250B0
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        self-frames-reset
        4 self-wait-frames
        0 $BD 12.41 -5.83 -62 char-to-xz
        1 10.0 -10.0 0.0 3.0 event-camera
        $17 state-flag-set
        4 message
        wait-message
        1 ebit-set
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: room47.act05 ( -- )   \ 00425110
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        self-frames-reset
        4 self-wait-frames
        0 $C0 -11.799 6.147 121 char-to-xz
        1 10.0 -10.0 0.0 3.0 event-camera
        $17 state-flag-set
        4 message
        wait-message
        1 ebit-set
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: room47.act06 ( -- )   \ 00425170
    self-wait-done
    -30.81 -21.85 self-turn-to-xz
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
            $2BC story-flag-set
            0 effect-remove
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

: room47.act07 ( -- )   \ 004251D0
    self-wait-done
    12.8 28.7 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        7 message
        wait-message
        3 ebit-set
    else
        8 message
        wait-message
    then
    self-idle-or-end
;

: room47.act08 ( -- )   \ 00425200
    self-wait-done
    4 partner-load
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
    3 5 0 char-model-op
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room47.enter $47 0 room-script!
' room47.char-enter $47 6 room-script!
' room47.phase1 $47 1 room-script!
' room47.phase2 $47 2 room-script!
' room47.act00 $47 $00 action-script!
' room47.act01 $47 $01 action-script!
' room47.act02 $47 $02 action-script!
' room47.act03 $47 $03 action-script!
' room47.act04 $47 $04 action-script!
' room47.act05 $47 $05 action-script!
' room47.act06 $47 $06 action-script!
' room47.act07 $47 $07 action-script!
' room47.act08 $47 $08 action-script!

\ ---- room $48 ----------------------------------------------------------------------------------

\ room 0x48 (D_004267F8): the handler's objects 2 and 3 swing (phases 60 degrees apart) - byte 3
\ 0 sets them still; 1: while slower than 5, Hewie's movement (the squared length of his last
\ step, +0x3C) past 1 makes them swing for 20 frames (the first also creaks: sounds 4 / 5 by
\ turns, event bit 0x11); the tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36
: room48.cmd00 ( b0 -- )  drop s" room48.cmd00" stub-step ;
\ room 0x48 (Room48_Cmd01_ptmf): character 0x26's +0xE4 cleared
: room48.cmd01 ( -- )  s" room48.cmd01" stub-step ;
\ room 0x48 (D_00426818): byte 3 0 starts the wall shadow (WallShadow_vtable, its slot in event
\ variable 2); else that one is ended
: room48.cmd02 ( b0 -- )  drop s" room48.cmd02" stub-step ;
\ room 0x48 (Room48_Cmd03_ptmf): character 0xFE's model +0x9E8 = -0.15 (byte 3 0) or 0
: room48.cmd03 ( b0 -- )  drop s" room48.cmd03" stub-step ;
\ room 0x48 (Room48_Cmd05_ptmf): the creatures (10) in play in the current room: the list's
\ +0x2C
: room48.cmd05 ( -- )  s" room48.cmd05" stub-step ;
\ room 0x48 (Room48_Cond00_ptmf): door 0 of room 0x48 (Progress_CurRoomFlag)
: room48.cond00? ( -- flag )  s" room48.cond00?" stub-flag ;

: room48.enter ( -- )   \ 004252D0
    room-sounds
    $19 1.0 0 bgm
    1 0 $300000 nav-group
    $7D door-locked? if
        0 4 $200000 nav-group
        1 4 $100000 nav-group
    then
    $26E story-flag? not if
        0 122.09 8.28 10.26 flicker-sprite
    then
    $26F story-flag? not if
        1 -131.47 1.0 9.32 flicker-sprite
    then
    $270 story-flag? not if
        2 -140.37 16.0 -25.97 flicker-sprite
    then
    8 1 item-count? not 9 1 item-count? not and $A 1 item-count? not and $B 1 item-count? not and $C 1 item-count? not and $10 1 item-count? not and if
        0 ebit-set
        3 -111.0 17.0 -115.0 flicker-sprite
    then
    $50 story-flag? if
        $79 story-flag? not if
            $79 story-flag-set
            $48 0 97 4 8 -1 0 0.0 creature-place
            $48 0 488 4 8 -1 0 0.0 creature-place
        then
    then
    1 char-here? 119 hewie-action? and if
        0 2 8 nav-group
        1 2 $B0 nav-group
        1 8 char-on-tri? not if
            1 8 76.218 -13.075 90 char-to-xz
            1800 2 hewie-anim
        then
    else
        0 2 $30 nav-group
        1 2 $88 nav-group
        1 2 char-in-nav-group? if
            1 $175 char-to-tri
        then
    then
    0 room48.cmd00
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    $75 story-flag? $76 story-flag? not and if
        $FE action-end
        $FE char-done
    then
    $76 story-flag? not if
        1 3 $20000 nav-group
        $26 3 $FF char-load
        $80 story-flag? if
            0 $F1 $1C action
        then
        0 room48.cmd02
    then
;

: room48.char-enter ( -- )   \ 00425460
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
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    2 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 5 exit-taken? and if
            0 5 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 5 exit-taken? and if
        1 5 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    5 5 5 area-camera
    0 self-is? if
        0 exit-taken? if
            2 map-page
        then
    then
;

: room48.act23 ( -- )   \ 00426720
    $79 $89 door-copy
    $79 door-reopen-unlock
    exits-rebuild
    0 4 $100000 nav-group
    1 4 $200000 nav-group
    1 char-here? if
        0 hewie-side
    then
    $FE char-here? if
        $FE 0 stalker-mode
    else stalker-active? $FE 2 char-C4? not and if
        $FE $FFFF -1 2 stalker-to-room
    then then
    room48.cmd05
    $12 ebit-clear
    exit
;

: room48.phase1 ( -- )   \ 00425520
    $10 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 -138.0 8.0 12.0 0 0 sound
            $10 ebit-set
        then
    else
        $C0000000 6 -138.0 8.0 12.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 5 char-in-area? 0 char-busy? not and if
            5 exit-check
        then
    else 1 5 char-in-area? 1 char-busy? not and if
        5 exit-check
    then then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    $B 5 5 1 chars-area-camera
    0 $C char-entered-area? if
        0 exit-prepare
    then
    0 $D char-entered-area? if
        2 exit-prepare
    then
    0 $E char-entered-area? if
        5 exit-prepare
    then
    0 $C char-entered-area? if
        $1C 3 0 char-load
    then
    0 $C char-left-area? if
        3 0 char-remove
    then
    0 6 char-entered-area? if
        2 map-page
    then
    0 6 char-left-area? if
        1 map-page
    then
    3 75.003 7.0 -9.266 $E 20 0 zone
    1 3 8 char-zone-bits? if
        1 room48.cmd00
    then
    1 char-here? if
        118 hewie-action? not if
            35 fiona-started? 1 2 char-C4? not and if
                75 7 -9 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and $FE char-here? not and 0 75 -9 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 9 action
                    then
                then
            then
        else
            44 fiona-started? if
                0 1 $A action
            then
            $FF panic-stage? not if
                0 char-busy? not $A state-flag? or if
                    3 stalker-kind-here? $22 stalker-kind-here? or if
                        $FE 2 char-C4? not if
                            75 7 -9 point-on-camera? not 0 char-unseen? not and $FE char-unseen? not and if
                                0 counter-set
                                0 1 $C action
                                0 $FE $D action
                                $FF panic-stage? if
                                    3 panic-stage
                                then
                                0 0 char-action? if
                                    0 0 $B action-force
                                else
                                    1 0 $B action-force
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    $13 ebit? 1 char-busy? not and if
        1 ebit-clear
        $E ebit-clear
    then
    0 ebit? if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not $FE char-here? not and if
                0 control-action? if
                    -111 17 -115 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                        0 -111 -115 $32 char-faces-xz? if
                            hewie-stays? if
                                0 0 5 action
                            then
                        then
                    then
                then
            then
        then
    then
    $2C4 story-flag? not if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not if
                0 control-action? if
                    75 7 95 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                        0 75 95 $32 char-faces-xz? if
                            hewie-stays? if
                                0 0 7 action
                            then
                        then
                    then
                then
            then
        then
    then
    9 ebit? not if
        4 80.25 7.0 -6.0 $10 4 0 zone
        0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
            0 5 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
        1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
            1 $A 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
        $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
            $FE 5 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
    then
    $A ebit? not if
        5 67.4 7.0 -6.7 $10 4 0 zone
        0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
            0 5 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
        1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
            1 $A 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
        $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
            $FE 5 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
    then
    0 ebit? if
        6 -111.73 15.0 -115.37 $3C 6 0 zone
        $B ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 6 9 char-zone-bits? if
                        $B ebit-set
                        $64 chance? if
                            $1F 6 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        6 -111.73 15.0 -115.37 $3C 6 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 6 9 char-zone-bits? if
                    $1F 6 var-set
                    $1F 0.0 hewie-look-zone
                then
            then
        then
    then
    $2C4 story-flag? not if
        7 77.32 0.0 95.28 $3C 10 0 zone
        $C ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 7 9 char-zone-bits? if
                        $C ebit-set
                        $64 chance? if
                            $1F 7 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        7 77.32 0.0 95.28 $3C 10 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 7 9 char-zone-bits? if
                    $1F 7 var-set
                    $1F 8.0 hewie-look-zone
                then
            then
        then
    then
    $12 ebit? if
        room48.cond00? not if
            room48.act23
        then
    then
;

: room48.phase2 ( -- )   \ 004258D0
    -2147483646 scene-request? if
        0 1 char-group-bit4? 0 1 char-in-area? and if
            5 1 1 scene-change
        then
    then
    0 ebit? if
        0 $F char-in-area? 0 -60 $3C char-heading? and if
            5 $1D 0 scene-change
        then
    then
    $26E story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
    $270 story-flag? not if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    0 $18 char-in-area? 0 -40 $3C char-heading? and if
        5 $E 0 scene-change
    then
    0 $10 char-in-area? 0 0 $3C char-heading? and if
        5 $F 0 scene-change
    then
    0 $11 char-in-area? 0 -40 $3C char-heading? and if
        5 $10 0 scene-change
    then
    0 $12 char-in-area? 0 -60 $3C char-heading? and if
        5 $11 0 scene-change
    then
    0 $13 char-in-area? 0 45 $3C char-heading? and if
        5 $12 0 scene-change
    then
    0 $14 char-in-area? 0 62 $3C char-heading? and if
        5 $13 0 scene-change
    then
    0 $15 char-in-area? 0 90 $3C char-heading? and if
        5 $14 0 scene-change
    then
    0 $16 char-in-area? 0 -30 $3C char-heading? and if
        5 $15 0 scene-change
    then
    0 $17 char-in-area? 0 45 $3C char-heading? and if
        5 $16 0 scene-change
    then
;

: room48.phase5 ( -- )   \ 004259B0
    1 char-busy? if
        1 action-end
        1 ebit? if
            1 $1E7 94.1 -5.96 90 char-to-xz
        else $D ebit? if
            1 $BC char-to-tri
            $18 state-flag-clear
        else $E ebit? if
            1 $64 char-to-tri
            $18 state-flag-clear
        then then then
    then
    $12 ebit? if
        room48.act23
    then
;

: room48.act00 ( -- )   \ 004259F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $18 message-param-room
    $18 item-use
    $7C door-unlock
    $76 story-flag-set
    0 3 $20000 nav-group
    $F $54 fade
    $FF 1.0 0 bgm
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    8 state-flag-set
    1 char-here? if
        $F ebit-set
        1 action-end
        1 char-done
    then
    $F1 action-end
    3 char-unload
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
    $F $51 fade
    $19 1.0 0 bgm
    $F ebit? if
        1 char-activate
        $F ebit-clear
        1 $19F 140 char-to-tri-facing
    then
    3 0 char-remove
    1 room48.cmd02
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    0 state-flag-set
    $FE $48 0 room-doors-state
    $FE $66 1 room-doors-state
    self-idle-or-end
;

: room48.act01 ( -- )   \ 00425AE0
    self-wait-done
    $7D self-through-door
    self-wait-done
    0 $7D char-not-at-door? if
        $609 self-anim
    else
        $608 self-anim
    then
    self-frames-reset
    $14 self-wait-frames
    0 $72 5 char-sound
    $7D door-unlock
    $12 ebit-set
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8008 message
    wait-message
    self-idle-or-end
;

: room48.act02 ( -- )   \ 00425B10
    self-wait-done
    122.09 10.26 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $26E story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: room48.act03 ( -- )   \ 00425B70
    self-wait-done
    -131.47 9.32 self-turn-to-xz
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
            $26F story-flag-set
            1 effect-remove
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

: room48.act04 ( -- )   \ 00425BD0
    self-wait-done
    -140.37 -25.97 self-turn-to-xz
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
            $270 story-flag-set
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

: room48.act05 ( -- )   \ 00425C30
    0 counter-set
    0 1 6 action-force
    $D ebit-set
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C00 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 wait-counter
    self-frames-reset
    self-wait-16
    self-frames-reset
    self-wait-16
    $8284 item-give
    0 self-scripted
    self-idle-or-end
;

: room48.act06 ( -- )   \ 00425C70
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $130 -83.0 -83.0 -120 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $133 -111.0 -108.0 180 $204 5 self-move-to
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    3 effect-remove
    0 ebit-clear
    10 hewie-trust
    8 message-param-room
    8 1 item-give-count
    0 8 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    counter-inc
    0 self-turn-angle
    self-wait-done
    $130 -83.0 -83.0 60 $FFFF $A self-move-to
    self-wait-done
    $AF story-flag? if
        5 0 state-flag-16
    then
    0 self-noclip
    $18 state-flag-clear
    $D ebit-clear
    0 self-scripted
    self-idle-or-end
;

: room48.act07 ( -- )   \ 00425D00
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 8 action-force
        $C00 self-anim
        0 1 char-wait-motion
        0 $2E 5 char-sound
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $18 state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: room48.act08 ( -- )   \ 00425D30
    1 self-scripted
    $E ebit-set
    $13 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $67 149.0 91.0 64 $FFFF $A self-move-to
    self-wait-done
    -84 self-turn-angle
    self-wait-done
    $13 ebit-clear
    1 self-noclip
    $66 129.619 91.48 1.7 10 hewie-go-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $2C4 story-flag? not if
        $A state-flag? 0 char-busy? not or if
            $97 $63 item-count? not if
                10 hewie-trust
            then
            $97 message-param-room
            $97 $63 item-count? if
                $8010 message
                wait-message
            else
                $2C4 story-flag-set
                $97 1 item-give-count
                0 $97 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
        then
    then
    $1ED 43.7 110.48 -90 $FFFF $A self-move-to
    self-wait-done
    110 self-turn-angle
    self-wait-done
    $1EE 76.12 104.76 0.5 5 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    $E ebit-clear
    self-idle-or-end
;

: room48.act09 ( -- )   \ 00425DF0
    $13 ebit-set
    1 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $175 92.855 -13.871 -90 $FFFF $A self-move-to
    self-wait-done
    $13 ebit-clear
    1 self-scripted
    0 2 8 nav-group
    1 2 $30 nav-group
    8 76.218 -13.075 -90 $204 5 self-move-to
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $102 $A self-anim-blend
    self-wait-anim
    3600 2 hewie-anim
    1 ebit-clear
    self-idle-or-end
;

: room48.act0A ( -- )   \ 00425E40
    1 self-scripted
    1 ebit-set
    self-wait-done
    9 ebit-clear
    $A ebit-clear
    $175 92.855 -13.871 90 $204 5 self-move-to
    self-wait-done
    -1 self-move-16
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-action
    0 2 $30 nav-group
    1 2 8 nav-group
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: room48.act0B ( -- )   \ 00425E80
    $18 state-flag-set
    1 self-noclip
    1 self-scripted
    $F $54 fade
    $FF 1.0 0 bgm
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
    3 4 0 char-model-op
    $1B state-flag-set
    yield
    $1B state-flag-clear
    0 1 char-no-shadow
    0 $F9 $1F action
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
    1 room48.cmd03
    $FE action-end
    3 summon-take
    5 hewie-trust
    8 state-flag-set
    0 $1BB 112.81 45.98 32 char-to-xz
    hewie-controlled? not if
        0 3 3 char-camera
        0 camera-follow
    else
        1 3 3 char-camera
        1 camera-follow
    then
    0 0 char-no-shadow
    yield
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    $23A item-give
    $8280 item-give
    $31 resident-flag-set
    counter-inc
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room48.act0C ( -- )   \ 00425F70
    1 wait-counter
    0 0 hewie-anim
    0 0 hewie-action
    1 $1DE 100.17 -23.46 -120 char-to-xz
    $1D state-flag-clear
    self-idle-or-end
;

: room48.act0D ( -- )   \ 0047AD30
    1 wait-counter
    self-idle-or-end
;

: room48.act0E ( -- )   \ 00425FA0
    self-wait-done
    -154.0 -28.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    2 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        1 message
        wait-message
    then
    2 ebit? not if
        0 var-inc
    then
    2 ebit-set
    self-idle-or-end
;

: room48.act0F ( -- )   \ 00425FE0
    self-wait-done
    -86.29 -39.06 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        2 message
        wait-message
    then
    3 ebit? not if
        0 var-inc
    then
    3 ebit-set
    self-idle-or-end
;

: room48.act10 ( -- )   \ 00426020
    self-wait-done
    40.56 -52.11 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    4 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        3 message
        wait-message
    then
    4 ebit? not if
        0 var-inc
    then
    4 ebit-set
    self-idle-or-end
;

: room48.act11 ( -- )   \ 00426060
    self-wait-done
    77.34 6.2 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    5 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        4 message
        wait-message
    then
    5 ebit? not if
        0 var-inc
    then
    5 ebit-set
    self-idle-or-end
;

: room48.act12 ( -- )   \ 004260A0
    self-wait-done
    127.66 -13.38 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    6 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        5 message
        wait-message
    then
    6 ebit? not if
        0 var-inc
    then
    6 ebit-set
    self-idle-or-end
;

: room48.act13 ( -- )   \ 004260E0
    self-wait-done
    144.96 39.88 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    7 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        6 message
        wait-message
    then
    7 ebit? not if
        0 var-inc
    then
    7 ebit-set
    self-idle-or-end
;

: room48.act14 ( -- )   \ 00426120
    self-wait-done
    $47 138.0 -98.0 180 $FFFF 5 self-move-to
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 ebit? not if
        8 message
        wait-message
        $F 6 fade
        wait-fade
        9 message
        wait-message
        $F 7 fade
        wait-fade
        8 ebit-set
    else
        $A message
        wait-message
        8 ebit-clear
    then
    self-idle-or-end
;

: room48.act15 ( -- )   \ 00426160
    self-wait-done
    73.11 -9.81 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 char-here? 118 hewie-action? and 1 8 char-on-tri? and if
        $C message
        wait-message
    else
        $B message
        wait-message
    then
    self-idle-or-end
;

: room48.act16 ( -- )   \ 00426190
    self-wait-done
    $1A2 113.0 -122.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 30.0 0.0 event-camera
    1 self-scripted
    $76 story-flag? not if
        $80 story-flag? not if
            $18 state-flag-set
            0 8 char-file-load
            $14 message
            wait-message
            0 answer? if
                0 char-file-use
                3 char-unload
                $26 char-activate
                $26 127.567 0.0 -123.393 -90 char-to-xyz
                room48.cmd01
                $A02 self-anim
                self-wait-anim
                0 $F1 $1E action
                self-frames-reset
                $F self-wait-frames
                0 $43 5 char-sound
                $8000 self-anim
                self-wait-anim
                3 panic-stage? not if
                    3 panic-stage
                then
                $8001 self-anim
                self-wait-anim
                $8003 self-anim
                self-wait-anim
                $80 story-flag-set
            then
            $18 state-flag-clear
        else
            1 0 var? if
                $D message
                wait-message
            then
            1 1 var? if
                $E message
                wait-message
            then
            1 2 var? if
                $800 self-anim
                self-wait-anim
                $F message
                wait-message
                $802 self-anim
                self-wait-anim
            then
            1 var-inc
            1 3 var? if
                1 0 var-set
            then
        then
    else
        $800 self-anim
        self-wait-anim
        $10 message
        wait-message
        $802 self-anim
        self-wait-anim
    then
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room48.act1B ( -- )   \ 00426300
    $F $54 fade
    $FF 1.0 0 bgm
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
    8 state-flag-set
    $F1 action-end
    1 char-here? if
        $F ebit-set
        1 action-end
        1 char-done
    then
    3 char-unload
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
    0 $1A2 108.0 -122.0 90 char-to-xz
    $F ebit? if
        1 char-activate
        $F ebit-clear
        1 $19F 140 char-to-tri-facing
    then
    $80 story-flag-set
    0 $F1 $1C action
    0 8 char-file-load
    0 char-file-use
    $8003 self-anim
    3 panic-stage? not if
        3 panic-stage
    then
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    self-wait-anim
    $32 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room48.act17 ( -- )   \ 00426280
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $19 message-param-room
    $19 item-use
    ['] room48.act1B goto
;

: room48.act18 ( -- )   \ 004262A0
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $1A message-param-room
    $1A item-use
    ['] room48.act1B goto
;

: room48.act19 ( -- )   \ 004262C0
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $1B message-param-room
    $1B item-use
    ['] room48.act1B goto
;

: room48.act1A ( -- )   \ 004262E0
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $1C message-param-room
    $1C item-use
    ['] room48.act1B goto
;

: room48.act1C ( -- )   \ 004263E0
    3 char-unload
    $26 char-activate
    $26 127.567 0.0 -123.393 -90 char-to-xyz
    room48.cmd01
    $26 $9001 1 0 char-anim-hold
    self-idle-or-end
;

: room48.act1D ( -- )   \ 00426400
    self-wait-done
    -112.26 -123.08 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message
    wait-message
    self-idle-or-end
;

: room48.act1E ( -- )   \ 00426418
    $26 $9000 0 0 char-anim-hold
    $26 wait-char-anim
    $26 $9001 1 0 char-anim-hold
    self-idle-or-end
;

: room48.act1F ( -- )   \ 00426430
    0 room48.cmd03
    begin
        2 cutscene-shot? not while
        yield
    repeat
    1 room48.cmd03
    self-idle-or-end
;

: room48.act20 ( -- )   \ 00426440
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    3 partner-load
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
    3 4 0 char-model-op
    0 1 char-no-shadow
    0 $F9 $1F action
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
    1 room48.cmd03
    0 0 char-no-shadow
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room48.act21 ( -- )   \ 00426540
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    0 room48.cmd02
    $26 3 $FF char-load
    3 char-unload
    5 0 movie-play
    4 cutscene-start
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

: room48.act22 ( -- )   \ 00426630
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    0 room48.cmd02
    $26 3 $FF char-load
    3 char-unload
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room48.enter $48 0 room-script!
' room48.char-enter $48 6 room-script!
' room48.phase1 $48 1 room-script!
' room48.phase2 $48 2 room-script!
' room48.phase5 $48 5 room-script!
' room48.act00 $48 $00 action-script!
' room48.act01 $48 $01 action-script!
' room48.act02 $48 $02 action-script!
' room48.act03 $48 $03 action-script!
' room48.act04 $48 $04 action-script!
' room48.act05 $48 $05 action-script!
' room48.act06 $48 $06 action-script!
' room48.act07 $48 $07 action-script!
' room48.act08 $48 $08 action-script!
' room48.act09 $48 $09 action-script!
' room48.act0A $48 $0A action-script!
' room48.act0B $48 $0B action-script!
' room48.act0C $48 $0C action-script!
' room48.act0D $48 $0D action-script!
' room48.act0E $48 $0E action-script!
' room48.act0F $48 $0F action-script!
' room48.act10 $48 $10 action-script!
' room48.act11 $48 $11 action-script!
' room48.act12 $48 $12 action-script!
' room48.act13 $48 $13 action-script!
' room48.act14 $48 $14 action-script!
' room48.act15 $48 $15 action-script!
' room48.act16 $48 $16 action-script!
' room48.act17 $48 $17 action-script!
' room48.act18 $48 $18 action-script!
' room48.act19 $48 $19 action-script!
' room48.act1A $48 $1A action-script!
' room48.act1B $48 $1B action-script!
' room48.act1C $48 $1C action-script!
' room48.act1D $48 $1D action-script!
' room48.act1E $48 $1E action-script!
' room48.act1F $48 $1F action-script!
' room48.act20 $48 $20 action-script!
' room48.act21 $48 $21 action-script!
' room48.act22 $48 $22 action-script!
' room48.act23 $48 $23 action-script!

\ ---- room $49 ----------------------------------------------------------------------------------

\ a lit quad at x -43, z -15.12 .. 5.07, height 30.05 / 10.05
: room49.cmd00 ( b0 -- )  drop s" room49.cmd00" stub-step ;
\ the depth range (effect 0x1C) opening with the cutscene from its frame 1156: 1 / 1 / 40 / 100,
\ the far two on by 1 a frame up to 80 / 140
: room49.cmd01 ( -- )  s" room49.cmd01" stub-step ;
\ byte 3 0: the effect Room49Effect_vtable spawned, its slot in event var 0; 1: removed
: room49.cmd02 ( b0 -- )  drop s" room49.cmd02" stub-step ;

: room49.enter ( -- )   \ 00407F30
    room-sounds
    0 room49.cmd00
    $268 story-flag? not if
        0 -7.97 12.42 21.3 flicker-sprite
    then
    $29D story-flag? $29E story-flag? not and if
        1 7.72 1.0 15.08 flicker-sprite
    then
    1 $78 $10000000 nav-tri-flags
    1 $41 $10000000 nav-tri-flags
    1 $79 $10000000 nav-tri-flags
    1 $42 $10000000 nav-tri-flags
;

: room49.char-enter ( -- )   \ 00407F80
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    $58 story-flag? if
        2 1 object-show
        3 1 object-show
        4 1 object-show
        5 1 object-show
        0 1 $14 door-bits
    then
;

: room49.phase1 ( -- )   \ 00407FE0
    0 exit-usable? if
        0 exit-check
    then
    0 21.03 0.0 -17.8 $14 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    1 -41.22 4.5 -4.36 $1C 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    $29D story-flag? not if
        3 7.72 0.0 15.08 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 669 var-set
                $1A 670 var-set
                $1B 1 var-set
                $1C 7720 var-set
                $1D 1000 var-set
                $1E 15080 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: room49.phase2 ( -- )   \ 004080A0
    0 2 $32 char-faces-area? if
        5 0 0 scene-change
    then
    0 1 char-in-area? 0 -45 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 3 char-in-area? 0 15 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 4 char-in-area? 0 -6 -22 $32 char-faces-xz? and if
        5 4 0 scene-change
    then
    0 5 char-in-area? 0 0 $32 char-heading? and if
        5 8 0 scene-change
    then
    $268 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    $29D story-flag? $29E story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
;

: room49.act00 ( -- )   \ 00408120
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    item-3F-under-10? if
        0 ebit? not if
            $11 message
            wait-message
            0 ebit-set
        then
        $20 story-flag-clear
        2 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        yield
        $20 story-flag? if
            $3F message-param-room
            0 $3F item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $20 story-flag-set
    else 0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $12 message
        wait-message
    then then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room49.act01 ( -- )   \ 0047AB78
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: room49.act02 ( -- )   \ 00408190
    $58 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 message
        wait-message
        0 answer? if
            $58 story-flag-set
            $59 story-flag-set
            $F $44 fade
            $13 3 $FF char-load
            3 char-unload
            self-wait-done
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
            0 $F9 6 action
            4 ebit-clear
            1 char-here? if
                1 ebit-set
                1 action-end
                1 char-done
            then
            3 4 0 char-model-op
            $13 char-activate
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
            0 $74 -32.419 -4.78 -107 char-to-xz
            camera-restart
            3 0 char-remove
            0 $A char-layer
            2 1 object-show
            3 1 object-show
            4 1 object-show
            5 1 object-show
            0 1 $14 door-bits
            4 ebit? if
                1 room49.cmd02
            then
            1 ebit? if
                1 char-activate
                $49 0 88 hewie-to-room
            then
            $FE $49 41 2 stalker-to-room
            $FE $29 33.75 -4.38 -91 char-to-xz
            $FE 0 -1 char-camera
            stalker-item-cooldown
            $F $41 fade
            wait-fade
            $235 item-give
            $50 threat-raise
            $2A resident-flag-set
        then
        $18 state-flag-clear
        0 self-scripted
    else $59 story-flag? if
        self-wait-done
        1 message
        wait-message
    else
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        5 message
        wait-message
        self-wait-anim
    then then
    self-idle-or-end
;

: room49.act03 ( -- )   \ 004082D0
    2 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 6 char-file-load
        $22 18.764 15.099 30 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        2 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 3 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        0 $84 5 char-sound
        3 avoid-prompt
        2 ebit-set
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        3 message
        wait-message
    then
    self-idle-or-end
;

: room49.act04 ( -- )   \ 00408390
    self-wait-done
    -6.0 -17.0 self-turn-to-xz
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room49.act05 ( -- )   \ 004083A0
    self-wait-done
    -7.97 21.3 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $268 story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: room49.act06 ( -- )   \ 00408400
    depth-range-off
    begin
        1 cutscene-shot? not while
        yield
    repeat
    0 $1E char-layer
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 $A char-layer
    0 room49.cmd02
    4 ebit-set
    begin
        5 cutscene-shot? not while
        yield
    repeat
    1 room49.cmd02
    4 ebit-clear
    begin
        6 cutscene-shot? not while
        yield
    repeat
    begin
        $446 cutscene-cue-reached? not while
        $80808000 $404A6038 14.0 420.732 1 fog
        yield
    repeat
    begin
        $44E cutscene-cue-reached? not while
        yield
    repeat
    begin
        $484 cutscene-cue-reached? not while
        1.0 1.0 40.0 100.0 depth-range
        yield
    repeat
    begin
        $4B0 cutscene-cue-reached? not while
        room49.cmd01
        yield
    repeat
    depth-range-off
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: room49.act07 ( -- )   \ 00408480
    self-wait-done
    7.72 15.08 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $29E story-flag-set
            1 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: room49.act08 ( -- )   \ 004084E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $3A -19.0 21.0 0 char-to-xz
    $17 state-flag-set
    1 12.0 -10.0 0.0 7.0 event-camera
    6 message
    wait-message
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    $1D01 $A self-anim-blend
    7 message
    wait-message
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room49.act09 ( -- )   \ 00408540
    self-wait-done
    0 room49.cmd00
    3 partner-load
    2 char-unload
    $13 3 $FF char-load
    3 char-unload
    self-wait-done
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
    0 $F9 6 action
    3 4 0 char-model-op
    $13 char-activate
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
    0 $74 -32.419 -4.78 -107 char-to-xz
    camera-restart
    3 0 char-remove
    0 $A char-layer
    2 1 object-show
    3 1 object-show
    4 1 object-show
    5 1 object-show
    0 1 $14 door-bits
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room49.enter $49 0 room-script!
' room49.char-enter $49 6 room-script!
' room49.phase1 $49 1 room-script!
' room49.phase2 $49 2 room-script!
' room49.act00 $49 $00 action-script!
' room49.act01 $49 $01 action-script!
' room49.act02 $49 $02 action-script!
' room49.act03 $49 $03 action-script!
' room49.act04 $49 $04 action-script!
' room49.act05 $49 $05 action-script!
' room49.act06 $49 $06 action-script!
' room49.act07 $49 $07 action-script!
' room49.act08 $49 $08 action-script!
' room49.act09 $49 $09 action-script!

\ ---- room $4A ----------------------------------------------------------------------------------

: room4A.enter ( -- )   \ 004086A0
    1 1 $300000 nav-group
    $84 story-flag? not if
        1 $56 char-in-room? 119 hewie-action? and if
            $4A 0 65 hewie-to-room
            0 1 4 action
            2 ebit-set
        then
    else
        0 1 $14 door-bits
        2 1 object-show
    then
    $27C story-flag? not if
        0 5.64 9.5 5.58 flicker-sprite
    then
    $2BD story-flag? $2BE story-flag? not and if
        1 -49.36 1.0 -47.3 flicker-sprite
    then
;

: room4A.char-enter ( -- )   \ 00408700
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    0 3 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 3 char-camera
                0 camera-follow
            else
                1 4 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 3 char-camera
            0 camera-follow
        else
            1 4 3 char-camera
            1 camera-follow
        then
    then then
    2 4 3 area-camera
;

: room4A.phase1 ( -- )   \ 00408780
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    $B 3 2 1 chars-area-camera
    $C 4 3 1 chars-area-camera
    0 9 char-entered-area? if
        0 exit-prepare
    then
    0 $A char-entered-area? if
        2 exit-prepare
    then
    $84 story-flag? not if
        0 0 char-in-area? 0 0 $50 char-heading? and if
            1 control-action? 2 ebit? and $FE char-here? not and 0 0 char-action? and if
                0 0 1 action
            then
        then
    then
    $2BD story-flag? not if
        3 -49.36 0.0 -47.3 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 701 var-set
                $1A 702 var-set
                $1B 1 var-set
                $1C -49360 var-set
                $1D 1000 var-set
                $1E -47300 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: room4A.phase2 ( -- )   \ 00408840
    2 game-mode? not $FF panic-stage? not and if
        $84 story-flag? not if
            0 0 char-group-bit4? if
                5 5 1 scene-change
            then
        then
    then
    $2BD story-flag? $2BE story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
;

: room4A.phase5 ( -- )   \ 00408880
    $84 story-flag? not 1 char-here? and 2 ebit? and if
        $56 0 -1 hewie-to-room
        18000 2 hewie-anim
    then
;

: room4A.act00 ( -- )   \ 0047AB80
    self-idle-or-end
;

: room4A.act01 ( -- )   \ 004088A0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C02 self-anim
    0 1 char-wait-motion
    0 $31 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $F $44 fade
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
    50 hewie-trust
    0 1 $14 door-bits
    2 1 object-show
    0 self-move-16
    0 $CB -38.75 -31.7 0 char-to-xz
    camera-restart
    $47 door-unlock
    $84 story-flag-set
    2 ebit-clear
    $57 0 -1 hewie-to-room
    $F $41 fade
    wait-fade
    $8283 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room4A.act02 ( -- )   \ 0047AB84
    self-idle-or-end
;

: room4A.act03 ( -- )   \ 0047AB88
    self-idle-or-end
;

: room4A.act04 ( -- )   \ 00408980
    self-wait-done
    0 self-doorway-fade
    begin
        0 0 var? if
            hewie-bark
            self-wait-done
            $32 chance? if
                0 90 var-set
            else $32 chance? if
                0 150 var-set
            else
                0 30 var-set
            then then
        else
            0 var-dec
            yield
        then
    again
;

: room4A.act05 ( -- )   \ 004089B0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    2 ebit? not if
        1 ebit? not if
            0 self-through-exit
            self-wait-done
            $609 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            self-frames-reset
            8 self-wait-frames
            0 $CB -38.0 -30.0 30 char-to-xz
            hewie-controlled? not if
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
            self-frames-reset
            4 self-wait-frames
            0 message
            wait-message
            1 ebit-set
        else
            $CB -38.0 -30.0 30 $FFFF 5 self-move-to
            self-wait-done
            hewie-controlled? not if
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
            self-frames-reset
            4 self-wait-frames
            1 message
            wait-message
        then
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    else
        $CB -38.0 -30.0 30 $FFFF 5 self-move-to
        self-wait-done
        2 message
        wait-message
        0 0 var-set
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room4A.act06 ( -- )   \ 0047AB8C
    self-idle-or-end
;

: room4A.act07 ( -- )   \ 00408A60
    self-wait-done
    -49.36 -47.3 self-turn-to-xz
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
            $2BE story-flag-set
            1 effect-remove
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

: room4A.act08 ( -- )   \ 00408AC0
    self-wait-done
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room4A.enter $4A 0 room-script!
' room4A.char-enter $4A 6 room-script!
' room4A.phase1 $4A 1 room-script!
' room4A.phase2 $4A 2 room-script!
' room4A.phase5 $4A 5 room-script!
' room4A.act00 $4A $00 action-script!
' room4A.act01 $4A $01 action-script!
' room4A.act02 $4A $02 action-script!
' room4A.act03 $4A $03 action-script!
' room4A.act04 $4A $04 action-script!
' room4A.act05 $4A $05 action-script!
' room4A.act06 $4A $06 action-script!
' room4A.act07 $4A $07 action-script!
' room4A.act08 $4A $08 action-script!

\ ---- room $4B ----------------------------------------------------------------------------------

\ room 0x4B (D_00409910): a lit quad at x -63.65 .. -55.65, z 104.5, from 4 to 21
: room4B.cmd00 ( b0 -- )  drop s" room4B.cmd00" stub-step ;
\ room 0x4B: the curtain ("Curtain") animated by event variable 0 (var0_anim: byte 3 0 forward,
\ 1 back, 2 / 3 back at rest).
: room4B.cmd01 ( b0 -- )  drop s" room4B.cmd01" stub-step ;
\ 1 unless the object at +0x18 exists and its byte +0x28 is 1.
: room4B.cond00? ( -- flag )  s" room4B.cond00?" stub-flag ;

: room4B.enter ( -- )   \ 00408B90
    room-sounds
    $70 story-flag? not if
        room4B.cond00? if
            $4B 0 422 2 8 6 0 0.0 creature-place
        then
    then
    $237 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $290 story-flag? not if
            0 13.5 -9.0 -75.0 flicker-sprite
        then
    then
    $238 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $291 story-flag? not if
            1 13.5 -9.0 -59.5 flicker-sprite
        then
    then
    0 4 0.5 0.5 0.25 0.5 zone-rect
    1 $B 0.4 0.0 0.2 0.5 zone-rect
    $329 story-flag? not if
        4 0 $14 door-bits
        5 1 $14 door-bits
        $32A story-flag? not if
            3 room4B.cmd01
            1 room4B.cmd00
        else
            2 room4B.cmd01
            0 room4B.cmd00
        then
    else
        4 1 $14 door-bits
        5 0 $14 door-bits
        1 room4B.cmd00
        1 2 $2008000 nav-group
    then
    $269 story-flag? not if
        2 -109.83 12.0 72.62 flicker-sprite
    then
    3 132.4 -16.6 -76.8 1 effect-86
    4 85.7 -5.3 -16.8 1 effect-86
    5 85.7 -5.3 16.8 1 effect-86
    6 132.4 -16.6 76.8 1 effect-86
    1 $2300 sound-volume
;

: room4B.char-enter ( -- )   \ 00408CD0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
    then then
    1 2 1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 5 3 char-camera
                0 camera-follow
            else
                1 5 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 5 3 char-camera
            0 camera-follow
        else
            1 5 3 char-camera
            1 camera-follow
        then
    then then
    2 5 3 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 4 2 char-camera
                0 camera-follow
            else
                1 4 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 4 2 char-camera
            0 camera-follow
        else
            1 4 2 char-camera
            1 camera-follow
        then
    then then
    3 4 2 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    4 1 0 area-camera
    hewie-controlled? not if
        0 self-is? 5 exit-taken? and if
            0 5 char-to-exit
            hewie-controlled? not if
                0 6 4 char-camera
                0 camera-follow
            else
                1 6 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 5 exit-taken? and if
        1 5 char-to-exit
        hewie-controlled? not if
            0 6 4 char-camera
            0 camera-follow
        else
            1 6 4 char-camera
            1 camera-follow
        then
    then then
    5 6 4 area-camera
    0 self-is? if
        0 exit-taken? 1 exit-taken? or 4 exit-taken? or 5 exit-taken? or if
            1 map-page
        then
    then
;

: room4B.phase1 ( -- )   \ 00408E50
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    5 exit-usable? if
        5 exit-check
    then
    $D 0 -1 1 chars-area-camera
    $E 1 0 1 chars-area-camera
    $F 1 0 1 chars-area-camera
    $10 2 1 1 chars-area-camera
    $11 1 0 1 chars-area-camera
    $12 3 -1 1 chars-area-camera
    $13 3 -1 1 chars-area-camera
    $14 4 2 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 5 3 1 chars-area-camera
    $17 1 0 1 chars-area-camera
    $18 6 4 1 chars-area-camera
    0 6 char-entered-area? if
        0 exit-prepare
    then
    0 7 char-entered-area? if
        1 exit-prepare
    then
    0 8 char-entered-area? if
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 $A char-entered-area? 0 $B char-entered-area? or if
        4 exit-prepare
    then
    0 $C char-entered-area? if
        5 exit-prepare
    then
    0 $12 char-entered-area? if
        0 map-page
    then
    0 $12 char-left-area? if
        1 map-page
    then
    2 ebit? not if
        $329 story-flag? not $32A story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                $FE $1A char-entered-area? $FE 0 char-action? and if
                    0 $FE 7 action
                then
            then
        then
    then
    2 85.21 -24.0 1.06 $14 19 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
    $329 story-flag? not $32A story-flag? and if
        3 -60.85 0.0 104.12 $10 6 0 zone
        4 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        4 ebit-set
                        $32 chance? if
                            $1F 3 var-set
                            0 1 $93 action
                        then
                    then
                then
            then
        then
    then
    4 -101.62 0.0 100.21 $14 6 0 zone
    5 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    5 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    4 -101.62 0.0 100.21 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    5 -132.75 0.0 71.15 $14 12 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 5 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 5 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    5 -132.75 0.0 71.15 $14 12 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    6 -29.87 0.0 75.83 $14 12 0 zone
    7 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 6 9 char-zone-bits? if
                    7 ebit-set
                    $1E chance? if
                        $1F 6 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    6 -29.87 0.0 75.83 $14 12 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
;

: room4B.phase2 ( -- )   \ 004090C0
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 4 1 scene-change
        then
        0 2 char-group-bit4? if
            5 0 1 scene-change
        then
        0 3 char-group-bit4? if
            5 2 1 scene-change
        then
    then
    0 $19 char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    3 ebit? not if
        0 $1A char-in-area? 0 0 $3C char-heading? and if
            $329 story-flag? not if
                $FE char-here? not if
                    5 6 5 scene-change
                else $FE $1A char-in-area? $FE 0 40 chars-within? or if
                    $8016 scene-ending
                else
                    5 6 5 scene-change
                then then
            else
                5 8 0 scene-change
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        $FE char-busy? not if
            3 ebit-clear
        then
    else
        3 ebit-clear
    then then
    $269 story-flag? not if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
    8 -29.58 0.0 75.55 7 10 0 zone
    0 8 8 char-zone-bits? if
        0 -30 76 $3C char-faces-xz? if
            5 $C 0 scene-change
        then
    then
    9 -133.09 0.0 71.35 7 10 0 zone
    0 9 8 char-zone-bits? if
        0 -133 71 $3C char-faces-xz? if
            5 $C 0 scene-change
        then
    then
    0 $1D char-in-area? 0 -45 $32 char-heading? and if
        5 $D 0 scene-change
    then
    $237 story-flag? not if
        0 13.5 -10.0 -75.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $237 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            13.5 -10.0 -75.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 13.5 -10.0 -75.0 0 0 sound
            $40 $23A noise
            0 13.5 -9.0 -75.0 flicker-sprite
        then
    else $290 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then then
    $238 story-flag? not if
        1 13.5 -10.0 -59.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $238 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            13.5 -10.0 -59.5 0 -2146430960 0 0.0 scene-effect-8C
            $88 5 13.5 -10.0 -59.5 0 0 sound
            $40 $234 noise
            1 13.5 -9.0 -59.5 flicker-sprite
        then
    else $291 story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then then
;

: room4B.phase5 ( -- )   \ 004092C0
    3 ebit? if
        $32A story-flag-set
        $329 story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            $FE 1 stalker-rage
        then
    then
;

: room4B.phase3 ( -- )   \ 004092E0
    58.0 5.0 19.5 58.0 5.0 15.0 58.0 -24.0 19.5 58.0 -24.0 15.0 lights-doorway
    58.0 5.0 -15.0 58.0 5.0 -19.5 58.0 -24.0 -15.0 58.0 -24.0 -19.5 lights-doorway
;

: room4B.act00 ( -- )   \ 00409350
    self-wait-done
    2 self-through-exit
    self-wait-done
    0 ebit? not 2 game-mode? or $FF panic-stage? or if
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
            1 message
            wait-message
        then
        0 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $8F 145.0 -75.0 180 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 3.0 event-camera
        self-frames-reset
        4 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $8F 143.75 -75.3 180 char-to-xz
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room4B.act01 ( -- )   \ 004093E0
    1 self-scripted
    self-wait-done
    2 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 2 6 char-sound
    $D message-param-room
    $52 door-locked? not if
        $D item-use
    then
    $51 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room4B.act02 ( -- )   \ 00409420
    self-wait-done
    3 self-through-exit
    self-wait-done
    1 ebit? not 2 game-mode? or $FF panic-stage? or if
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
            1 message
            wait-message
        then
        1 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $29 145.0 75.0 0 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 3.0 event-camera
        self-frames-reset
        4 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $29 146.25 75.3 0 char-to-xz
        $17 state-flag-clear
        1 self-scripted
    then
    self-idle-or-end
;

: room4B.act03 ( -- )   \ 004094B0
    1 self-scripted
    self-wait-done
    3 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 2 6 char-sound
    $D message-param-room
    $51 door-locked? not if
        $D item-use
    then
    $52 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room4B.act04 ( -- )   \ 004094F0
    self-wait-done
    $FE self-touching? not if
        $4F self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $4F char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $4F door-unlock
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

: room4B.act05 ( -- )   \ 00409530
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    90.0 0.0 self-turn-to-xz
    self-wait-done
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room4B.act06 ( -- )   \ 00409560
    1 self-scripted
    2 ebit-set
    self-wait-done
    0 0 char-file-load
    $21D -59.65 99.14 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $32A story-flag? not if
        0 room4B.cmd00
        0 0 var-set
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            0 room4B.cmd01
            yield
            0 30 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        2 room4B.cmd01
        $32A story-flag-set
    else
        0 0 var-set
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            1 room4B.cmd01
            yield
            0 28 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        3 room4B.cmd01
        $32A story-flag-clear
        1 room4B.cmd00
    then
    2 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: room4B.act07 ( -- )   \ 00409610
    3 ebit-set
    self-wait-done
    $FE 1 char-file-load
    $21D -59.65 99.14 0 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 5 6 char-sound
    $8000 5 self-anim-blend
    self-wait-anim
    $E00 5 self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    $329 story-flag-set
    $FE 1 6 char-sound
    $FE 1 stalker-rage
    4 1 $14 door-bits
    5 0 $14 door-bits
    1 2 $2008000 nav-group
    1 room4B.cmd00
    -57.0 16.0 104.5 1 404232216 4 -15.5 scene-effect-8C
    -61.0 16.0 104.5 1 404232216 4 -15.5 scene-effect-8C
    -57.0 10.0 104.5 1 404232216 4 -9.5 scene-effect-8C
    -61.0 10.0 104.5 1 404232216 4 -9.5 scene-effect-8C
    -57.0 4.0 104.5 1 404232216 4 -3.5 scene-effect-8C
    -61.0 4.0 104.5 1 404232216 4 -3.5 scene-effect-8C
    self-wait-anim
    3 ebit-clear
    self-idle-or-end
;

: room4B.act08 ( -- )   \ 004096F8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room4B.act09 ( -- )   \ 00409710
    self-wait-done
    -109.83 72.62 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $269 story-flag-set
            2 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: room4B.act0A ( -- )   \ 00409770
    self-wait-done
    13.5 -75.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $290 story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: room4B.act0B ( -- )   \ 004097D0
    self-wait-done
    13.5 -59.5 self-turn-to-xz
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
            $291 story-flag-set
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

: room4B.act0C ( -- )   \ 00409830
    self-wait-done
    0 8 8 char-zone-bits? if
        -29.58 75.55 self-turn-to-xz
        self-wait-done
    else
        -133.09 71.35 self-turn-to-xz
        self-wait-done
    then
    4 message
    wait-message
    self-idle-or-end
;

: room4B.act0D ( -- )   \ 00409860
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $F4 -53.0 -16.0 -90 char-to-xz
    $17 state-flag-set
    1 12.0 0.0 0.0 5.0 event-camera
    5 message
    wait-message
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    $1D01 $A self-anim-blend
    6 message
    wait-message
    self-wait-anim
    1 var-inc
    1 2 var? if
        $236 item-give
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

' room4B.enter $4B 0 room-script!
' room4B.char-enter $4B 6 room-script!
' room4B.phase1 $4B 1 room-script!
' room4B.phase2 $4B 2 room-script!
' room4B.phase5 $4B 5 room-script!
' room4B.phase3 $4B 3 room-script!
' room4B.act00 $4B $00 action-script!
' room4B.act01 $4B $01 action-script!
' room4B.act02 $4B $02 action-script!
' room4B.act03 $4B $03 action-script!
' room4B.act04 $4B $04 action-script!
' room4B.act05 $4B $05 action-script!
' room4B.act06 $4B $06 action-script!
' room4B.act07 $4B $07 action-script!
' room4B.act08 $4B $08 action-script!
' room4B.act09 $4B $09 action-script!
' room4B.act0A $4B $0A action-script!
' room4B.act0B $4B $0B action-script!
' room4B.act0C $4B $0C action-script!
' room4B.act0D $4B $0D action-script!

\ ---- room $4C ----------------------------------------------------------------------------------

\ four room objects (Room4C_ObjectNames) pressed in (+0x24 down 0.2 to -0.7, a sound as each
\ starts) while event flag i is set, else back up 0.2 to 0; byte 3 0 all reset
: room4C.cmd00 ( b0 -- )  drop s" room4C.cmd00" stub-step ;
\ room 0x4C (Room4C_Cmd01_ptmf): byte 3 0 starts counting what the player does (her +0x1AD710
\ on), 1 adds this frame's (Fiona_Shakes); at 35 events bit 0x13
: room4C.cmd01 ( b0 -- )  drop s" room4C.cmd01" stub-step ;
\ room 0x4C (D_0040AC08): character 0x11's model parts 0xA2 / 0xA4 / 0xAC get bit 2 when byte 3
\ is 0, else lose it
: room4C.cmd02 ( b0 -- )  drop s" room4C.cmd02" stub-step ;

: room4C.enter ( -- )   \ 00409980
    room-sounds
    0 room4C.cmd00
    $48 story-flag? not if
        0 -0.5 18.5 -35.5 2 effect-86
        1 9.5 18.5 -35.0 2 effect-86
        2 4.8 25.5 -37.5 2 effect-86
        3 4.8 6.2 -33.4 2 effect-86
    else
        0 -0.5 18.5 -35.5 3 effect-86
        1 9.5 18.5 -35.0 3 effect-86
        2 4.8 25.5 -37.5 3 effect-86
        3 4.8 6.2 -33.4 3 effect-86
    then
    3 char-unload
    $11 char-activate
    $49 story-flag? if
        0 room4C.cmd02
    else
        1 room4C.cmd02
    then
    $11 $9005 1 0 char-anim-hold
    $11 9.97 0.0 -0.59 0 char-to-xyz
    $330 story-flag? not if
        $A 1 object-show
        $B 0 object-show
        $F 1 object-show
        $10 0 object-show
    else
        $A 0 object-show
        $B 1 object-show
        $F 0 object-show
        $10 1 object-show
    then
    2 1 $14 door-bits
    $E 1 object-show
    5 0 $14 door-bits
    $26A story-flag? not if
        5 -20.96 8.5 5.57 flicker-sprite
    then
    1 $2300 sound-volume
;

: room4C.act0F ( -- )   \ 0040A7D0
    $18 ebit-clear
    $17 0 pvar? if
        0 chance? if
            $18 ebit-set
        then
    else $17 1 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    else $17 2 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    else $17 3 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    then then then then
    2 creature-action? if
        $18 ebit-set
    then
    $18 ebit? if
        0 $FE $10 action
    else
        $78 1 item-cooldown
    then
    $17 pvar-inc
    exit
;

: room4C.char-enter ( -- )   \ 00409A80
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            $19 ebit-set
            room4C.act0F
        else
            $19 ebit-clear
        then
    then
;

: room4C.phase1 ( -- )   \ 00409AD0
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    0 ebit-clear
    0 $89 char-on-tri? 0 $B1 char-on-tri? or if
        0 ebit-set
    then
    1 $89 char-on-tri? 1 $B1 char-on-tri? or 1 char-here? and if
        0 ebit-set
    then
    $FE $89 char-on-tri? $FE $B1 char-on-tri? or $FE char-here? and if
        0 ebit-set
    then
    1 ebit-clear
    0 $90 char-on-tri? 0 $B2 char-on-tri? or if
        1 ebit-set
    then
    1 $90 char-on-tri? 1 $B2 char-on-tri? or 1 char-here? and if
        1 ebit-set
    then
    $FE $90 char-on-tri? $FE $B2 char-on-tri? or $FE char-here? and if
        1 ebit-set
    then
    2 ebit-clear
    0 $97 char-on-tri? 0 $B3 char-on-tri? or if
        2 ebit-set
    then
    1 $97 char-on-tri? 1 $B3 char-on-tri? or 1 char-here? and if
        2 ebit-set
    then
    $FE $97 char-on-tri? $FE $B3 char-on-tri? or $FE char-here? and if
        2 ebit-set
    then
    3 ebit-clear
    0 $83 char-on-tri? 0 $B0 char-on-tri? or if
        3 ebit-set
    then
    1 $83 char-on-tri? 1 $B0 char-on-tri? or 1 char-here? and if
        3 ebit-set
    then
    $FE $83 char-on-tri? $FE $B0 char-on-tri? or $FE char-here? and if
        3 ebit-set
    then
    1 room4C.cmd00
    $48 story-flag? not if
        1 ebit? 2 ebit? and if
            8 ebit? not 4 ebit? not and if
                0 0 var? if
                    0 var-inc
                then
                4 ebit-set
                9 6 -0.5 18.5 -35.5 0 0 sound
                0 effect-remove
            then
            8 ebit-set
        else
            8 ebit-clear
        then
        0 ebit? 3 ebit? and if
            9 ebit? not 5 ebit? not and if
                0 1 var? if
                    0 var-inc
                then
                5 ebit-set
                9 6 9.5 18.5 -35.0 0 0 sound
                1 effect-remove
            then
            9 ebit-set
        else
            9 ebit-clear
        then
        1 ebit? 3 ebit? and if
            $A ebit? not 6 ebit? not and if
                0 2 var? if
                    0 var-inc
                then
                6 ebit-set
                9 6 4.8 25.5 -37.5 0 0 sound
                2 effect-remove
            then
            $A ebit-set
        else
            $A ebit-clear
        then
        0 ebit? 2 ebit? and if
            $B ebit? not 7 ebit? not and if
                0 3 var? if
                    0 var-inc
                then
                7 ebit-set
                9 6 4.8 6.2 -33.4 0 0 sound
                3 effect-remove
            then
            $B ebit-set
        else
            $B ebit-clear
        then
        0 ebit? 1 ebit? and if
            $1C ebit? not if
                4 ebit? 5 ebit? or 6 ebit? or 7 ebit? or if
                    8 6 4.8 6.2 -33.4 0 0 sound
                then
                4 ebit-clear
                5 ebit-clear
                6 ebit-clear
                7 ebit-clear
                0 -0.5 18.5 -35.5 2 effect-86
                1 9.5 18.5 -35.0 2 effect-86
                2 4.8 25.5 -37.5 2 effect-86
                3 4.8 6.2 -33.4 2 effect-86
                0 0 var-set
            then
            $1C ebit-set
        else
            $1C ebit-clear
        then
        2 ebit? 3 ebit? and if
            $1D ebit? not if
                4 ebit? 5 ebit? or 6 ebit? or 7 ebit? or if
                    8 6 4.8 6.2 -33.4 0 0 sound
                then
                4 ebit-clear
                5 ebit-clear
                6 ebit-clear
                7 ebit-clear
                0 -0.5 18.5 -35.5 2 effect-86
                1 9.5 18.5 -35.0 2 effect-86
                2 4.8 25.5 -37.5 2 effect-86
                3 4.8 6.2 -33.4 2 effect-86
                0 0 var-set
            then
            $1D ebit-set
        else
            $1D ebit-clear
        then
        4 ebit? 5 ebit? and 6 ebit? and 7 ebit? and if
            0 char-busy? not if
                $48 story-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 0 action-force
                else
                    1 0 0 action-force
                then
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $E ebit? not if
            0 23.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        $E ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            0 23.0 0.0 -22.5 7 5 0 zone
            1 0 9 char-zone-bits? not if
                $E ebit-clear
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $F ebit? not if
            1 11.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        $F ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            1 11.0 0.0 -22.5 7 5 0 zone
            1 1 9 char-zone-bits? not if
                $F ebit-clear
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $10 ebit? not if
            2 -1.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 2 9 char-zone-bits? if
                        $10 ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            2 -1.0 0.0 -22.5 7 5 0 zone
            1 2 9 char-zone-bits? not if
                $10 ebit-clear
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $11 ebit? not if
            3 -13.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        $11 ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            3 -13.0 0.0 -22.5 7 5 0 zone
            1 3 9 char-zone-bits? not if
                $11 ebit-clear
            then
        then
    then
    $15 ebit? $16 ebit? not and if
        4 10.8 0.0 14.07 $A 200 0 zone
        1 4 char-in-zone? if
            $16 ebit-set
        then
    then
    $15 ebit? not if
        5 37.99 0.0 24.16 $14 14 0 zone
        $1A ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 5 9 char-zone-bits? if
                        $1A ebit-set
                        $32 chance? if
                            $1F 5 var-set
                            0 1 $92 action
                        then
                    then
                then
            then
        then
        5 37.99 0.0 24.16 $14 14 0 zone
        1 char-here? 2 game-mode? not and if
            hewie-can-command? if
                1 5 9 char-zone-bits? if
                    $1F 5 var-set
                    $1F 14.0 hewie-look-zone
                then
            then
        then
        $48 story-flag? not if
            6 11.81 0.0 -0.03 $14 6 0 zone
            $1B ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 6 9 char-zone-bits? if
                            $1B ebit-set
                            $64 chance? if
                                $1F 6 var-set
                                0 1 $8A action
                            then
                        then
                    then
                then
            then
            6 11.81 0.0 -0.03 $14 6 0 zone
            1 char-here? 1 game-mode? and if
                hewie-can-command? if
                    1 6 9 char-zone-bits? if
                        $1F 6 var-set
                        $1F 6.0 hewie-look-zone
                    then
                then
            then
        then
    then
    $1E ebit? not if
        6 sound-bank-loaded? if
            $40000004 6 40.0 5.0 -26.0 0 0 sound
            $1E ebit-set
        then
    else
        $C0000004 6 40.0 5.0 -26.0 0 0 sound
    then
;

: room4C.phase2 ( -- )   \ 0040A020
    0 3 char-in-area? 0 90 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 4 char-in-area? 0 10 0 $32 char-faces-xz? and if
        $48 story-flag? not if
            5 2 0 scene-change
        else $49 story-flag? not if
            5 5 4 scene-change
        else
            5 4 0 scene-change
        then then
    then
    0 5 char-in-area? 0 45 $32 char-heading? and if
        $330 story-flag? not if
            5 6 0 scene-change
        else $FE char-here? not if
            5 6 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    0 6 char-in-area? 0 0 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 7 char-in-area? 0 55 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 8 char-in-area? 0 45 $32 char-heading? and if
        5 9 0 scene-change
    then
    0 9 char-in-area? 0 0 $32 char-heading? and if
        5 $A 0 scene-change
    then
    0 $A char-in-area? 0 -22 $32 char-heading? and if
        5 $B 0 scene-change
    then
    0 $B char-in-area? 0 -45 $32 char-heading? and if
        5 $C 0 scene-change
    then
    0 $C char-in-area? 0 -45 $32 char-heading? and if
        5 $D 0 scene-change
    then
    $26A story-flag? not if
        7 5 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
;

: room4C.act00 ( -- )   \ 0040A0F0
    0 counter-set
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
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
    wait-fade
    0 $F9 $13 action
    $FF 1 char-visible
    1 char-here? if
        0 1 1 action-force
        1 wait-counter
    then
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
    50 hewie-trust
    0 self-move-16
    camera-restart
    $FF 0 char-visible
    counter-inc
    0 -0.5 18.5 -35.5 3 effect-86
    1 9.5 18.5 -35.0 3 effect-86
    2 4.8 25.5 -37.5 3 effect-86
    3 4.8 6.2 -33.4 3 effect-86
    $11 $9005 1 0 char-anim-hold
    $11 9.97 0.0 -0.59 0 char-to-xyz
    $11 0 char-no-shadow
    $FE $4C 0 room-doors-state
    $F $41 fade
    wait-fade
    $827E item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room4C.act01 ( -- )   \ 0040A200
    begin
        1 char-busy? not while
        yield
    repeat
    $FF 1 char-visible
    counter-inc
    2 wait-counter
    $FF 0 char-visible
    self-idle-or-end
;

: room4C.act02 ( -- )   \ 0040A220
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $47 story-flag? not if
        $8D 9.0 10.0 180 $FFFF 5 self-move-to
        self-wait-done
        0 message
        wait-message
        0 answer? if
            $F $44 fade
            5 0 movie-play
            4 cutscene-start
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
            0 self-move-16
            0 $2F 10.8 14.07 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            $11 $9005 0 0 char-anim-hold
            camera-restart
            1 0 char-visible
            1 action-end
            8 state-flag-set
            0 8 char-file-load
            0 char-file-use
            8 state-flag-clear
            $F $41 fade
        then
    then
    0 answer? if
        self-wait-done
        $47 story-flag? if
            0 8 char-file-load
            0 char-file-use
            1 27.0 10.0 -40.0 0.0 event-camera
            0 $2F 10.8 14.07 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9000 0 0 char-anim-hold
            $8002 5 self-anim-blend
            self-frames-reset
            $17 self-wait-frames
            0 5 6 char-sound
            self-wait-anim
        else
            1 27.0 10.0 -40.0 0.0 event-camera
            0 $2F 12.34 13.19 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9001 0 0 char-anim-hold
            $8003 self-anim
        then
        $15 ebit-set
        $16 ebit-clear
        $13 ebit-clear
        $14 ebit-clear
        0 room4C.cmd01
        begin
            hewie-can-command? if
                $61 0 hewie-action
            then
            0 control-action? 1 control-action? or if
                $1F ebit? not if
                    $1F ebit-set
                    0 $38 5 char-sound
                then
            then
            $1F ebit? if
                2 var-inc
                2 60 var? if
                    2 0 var-set
                    $1F ebit-clear
                then
            then
            1 room4C.cmd01
            1 var-inc
            1 12 var? if
                $A threat-add
                1 0 var-set
            then
            panic-98? if
                $13 ebit-set
                $14 ebit-set
            then
            $13 ebit? not if
                self-at-motion-event? if
                    0 panic-stage? 1 panic-stage? or if
                        $11 $9001 0 5 char-anim-hold
                        $8003 5 self-anim-blend
                    then
                    2 panic-stage? 3 panic-stage? or if
                        $11 $9002 0 5 char-anim-hold
                        $8004 5 self-anim-blend
                    then
                then
            then
            $16 ebit? not if
                self-at-motion-event? not if
                    self-frames-reset
                    1 self-wait-frames
                    false
                else $13 ebit? not if
                    self-frames-reset
                    1 self-wait-frames
                    false
                else
                    true
                then then
            else
                true
            then
        until
        $15 ebit-clear
        0 0.0 0.0 0.0 0.0 event-camera
        hewie-can-command? if
            0 0 hewie-action
        then
        $11 $9003 0 5 char-anim-hold
        $8005 3 self-anim-blend
        self-wait-anim
        $11 $9005 0 5 char-anim-hold
        $14 ebit? if
            4 panic-stage
        then
        $47 story-flag-set
        $2F resident-flag-set
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room4C.act03 ( -- )   \ 0040A490
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $17 state-flag-set
    1 20.0 -15.0 0.0 0.0 event-camera
    0 $97 5.0 -27.0 180 char-to-xz
    self-frames-reset
    4 self-wait-frames
    3 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    4 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room4C.act04 ( -- )   \ 0040A4F0
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    $D ebit? not if
        1 message
        wait-message
        $D ebit-set
    else
        2 message
        wait-message
        $D ebit-clear
    then
    self-idle-or-end
;

: room4C.act05 ( -- )   \ 0040A510
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $49 story-flag-set
    0 room4C.cmd02
    $F message-param-room
    $F 1 item-give-count
    0 $F item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room4C.act0E ( -- )   \ 0040A790
    $17 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    0 2 6 char-sound
    $E 2 object-anim
    self-wait-anim
    0 self-noclip
    $17 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: room4C.act06 ( -- )   \ 0040A560
    $330 story-flag? not if
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        $13 message
        wait-message
    else
        $18 state-flag-set
        $19 ebit-clear
        1 self-scripted
        0 9 char-file-load
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        1 self-noclip
        0 char-file-use
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        2 0 $14 door-bits
        $E 0 object-show
        $E 0 object-anim
        self-frames-reset
        $24 self-wait-frames
        0 2 6 char-sound
        self-wait-anim
        -1 self-move-16
        0 self-noclip
        $18 state-flag-clear
        9 state-flag-set
        $17 ebit-clear
        0 avoid-prompt
        $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
            0 3 6 char-sound
        then
        $FF 3 -1 char-camera
        begin
            0 2 pad? not if
                $FF panic-stage? $17 ebit? or if
                    $FF panic-stage? not if
                        0 $43 5 char-sound
                    then
                    ['] room4C.act0E goto
                else
                    2 panic-grow
                    6 fiona-calm
                    $1E fiona-recovery-lower
                    $19 ebit? $FE char-here? not and if
                        $19 ebit-clear
                        1 avoid-prompt
                    then
                    yield
                    false
                then
            else
                true
            then
        until
        $FE char-here? if
            ['] room4C.act0E goto
        then
        0 camera-follow
        9 state-flag-clear
        1 self-noclip
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 2 6 char-sound
        $E 1 object-anim
        self-wait-anim
        0 self-noclip
        0 self-scripted
    then
    self-idle-or-end
;

: room4C.act07 ( -- )   \ 0040A650
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 9 char-file-load
    5 9.83 27.45 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $330 story-flag? not if
        $8004 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        $A 1 object-show
        $B 0 object-show
        $F 1 object-show
        $10 0 object-show
        $B 0 object-anim
        $10 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        0 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        0 0 6 char-sound
        self-wait-anim
        $330 story-flag-set
    else
        $8003 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        $A 0 object-show
        $B 1 object-show
        $F 0 object-show
        $10 1 object-show
        $A 0 object-anim
        $F 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        0 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        5 self-wait-frames
        0 7 6 char-sound
        self-wait-anim
        $330 story-flag-clear
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room4C.act08 ( -- )   \ 0040A700
    self-wait-done
    110 self-turn-angle
    self-wait-done
    $A message
    wait-message
    self-idle-or-end
;

: room4C.act09 ( -- )   \ 0040A710
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $C ebit? not if
        $B message
        wait-message
        $C ebit-set
    else
        $C message
        wait-message
        $C ebit-clear
    then
    self-idle-or-end
;

: room4C.act0A ( -- )   \ 0040A728
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $D message
    wait-message
    self-idle-or-end
;

: room4C.act0B ( -- )   \ 0040A738
    self-wait-done
    -45 self-turn-angle
    self-wait-done
    $E message
    wait-message
    self-idle-or-end
;

: room4C.act0C ( -- )   \ 0040A750
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $12 ebit? not if
        $F message
        wait-message
        $12 ebit-set
    else
        10.0 10.0 0.0 self-look-at-point
        yield
        $10 message
        wait-message
        $FF self-look-at
        yield
    then
    self-idle-or-end
;

: room4C.act0D ( -- )   \ 0040A780
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    $12 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room4C.act10 ( -- )   \ 0040A820
    3 self-is? $22 self-is? or if
        1 self-scripted
        things-clear
        0 counter-set
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 $11 action-force
        1 self-scripted
        self-wait-done
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        counter-inc
        begin
            yield
        again
    else
        self-wait-done
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        7 26.16 27.36 118 $FFFF 5 self-move-to
        self-wait-done
        $1601 self-anim
        self-wait-anim
        $17 ebit-set
        10 self-move-16
        begin
            0 char-busy? while
            yield
        repeat
        $FE 0 stalker-mode
    then
    self-idle-or-end
;

: room4C.act11 ( -- )   \ 0040A890
    $2C state-flag-set
    $19 state-flag-set
    1 wait-counter
    $F $44 fade
    $12 1 movie-play
    $11 cutscene-start
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
    $A 1 object-show
    $B 1 object-show
    3 stalker-kind? $22 stalker-kind? or if
        3 4 0 char-model-op
    then
    $10 $50 movie-param
    0 1 char-visible
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        movie-loop
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $F 1 fade
        $A cutscene-control
        begin
            7 cutscene-control
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
        until
    then
    $2E resident-flag-set
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room4C.act12 ( -- )   \ 0040A910
    self-wait-done
    -20.96 5.57 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $26A story-flag-set
            5 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room4C.act13 ( -- )   \ 0040A968
    $11 1 char-no-shadow
    begin
        2 cutscene-shot? not while
        yield
    repeat
    $11 0 char-no-shadow
    self-idle-or-end
;

: room4C.act14 ( -- )   \ 0040A980
    self-wait-done
    3 partner-load
    2 char-unload
    $12 1 movie-play
    $11 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $A 1 object-show
    $B 1 object-show
    $F 0 object-show
    $10 1 object-show
    $E 1 object-show
    3 4 0 char-model-op
    $10 $50 movie-param
    0 1 char-visible
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
    0 0 char-visible
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room4C.act15 ( -- )   \ 0040AA30
    self-wait-done
    $A 1 object-show
    $B 0 object-show
    $F 1 object-show
    $10 0 object-show
    $E 1 object-show
    $11 3 $FF char-load
    3 char-unload
    $F $44 fade
    5 0 movie-play
    4 cutscene-start
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

: room4C.act16 ( -- )   \ 0040AAD0
    self-wait-done
    $A 1 object-show
    $B 0 object-show
    $F 1 object-show
    $10 0 object-show
    $E 1 object-show
    $11 3 $FF char-load
    3 char-unload
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
    0 $F9 $13 action
    $FF 1 char-visible
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
    $FF 0 char-visible
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room4C.enter $4C 0 room-script!
' room4C.char-enter $4C 6 room-script!
' room4C.phase1 $4C 1 room-script!
' room4C.phase2 $4C 2 room-script!
' room4C.act00 $4C $00 action-script!
' room4C.act01 $4C $01 action-script!
' room4C.act02 $4C $02 action-script!
' room4C.act03 $4C $03 action-script!
' room4C.act04 $4C $04 action-script!
' room4C.act05 $4C $05 action-script!
' room4C.act06 $4C $06 action-script!
' room4C.act07 $4C $07 action-script!
' room4C.act08 $4C $08 action-script!
' room4C.act09 $4C $09 action-script!
' room4C.act0A $4C $0A action-script!
' room4C.act0B $4C $0B action-script!
' room4C.act0C $4C $0C action-script!
' room4C.act0D $4C $0D action-script!
' room4C.act0E $4C $0E action-script!
' room4C.act0F $4C $0F action-script!
' room4C.act10 $4C $10 action-script!
' room4C.act11 $4C $11 action-script!
' room4C.act12 $4C $12 action-script!
' room4C.act13 $4C $13 action-script!
' room4C.act14 $4C $14 action-script!
' room4C.act15 $4C $15 action-script!
' room4C.act16 $4C $16 action-script!

\ ---- room $4D ----------------------------------------------------------------------------------

: room4D.char-enter ( -- )   \ 0040AC70
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
;

: room4D.phase1 ( -- )   \ 0040AD70
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    0 -24.64 2.5 27.03 $14 6 0 zone
    0 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    0 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    0 -24.64 2.5 27.03 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 25.74 2.5 -26.39 $14 6 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 25.74 2.5 -26.39 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: room4D.enter ( -- )   \ 0047AB90
    2 1.0 0 bgm
;

' room4D.char-enter $4D 6 room-script!
' room4D.phase1 $4D 1 room-script!
' room4D.enter $4D 0 room-script!

\ ---- room $4E ----------------------------------------------------------------------------------

\ room 0x4E (D_0040B4D8): a lit quad at x -44 .. -36, z 60, from 54 to 71
: room4E.cmd00 ( b0 -- )  drop s" room4E.cmd00" stub-step ;
\ room 0x4E: the curtain ("Curtain_2") animated by event variable 0 (var0_anim: byte 3 0
\ forward, 1 back, 2 / 3 back at rest).
: room4E.cmd01 ( b0 -- )  drop s" room4E.cmd01" stub-step ;
\ room 0x4E (D_0040B4F8): the 0x10-byte effect Room4EEffect_vtable made
: room4E.cmd02 ( -- )  s" room4E.cmd02" stub-step ;

: room4E.enter ( -- )   \ 0040AE90
    room-sounds
    $11 1.0 0 bgm
    $45 story-flag? not if
        0 1 $14 door-bits
        1 0 $10020000 nav-group
    else
        1 1 $14 door-bits
        1 1 $10020000 nav-group
        1 2 $300000 nav-group
        1 char-here? if
            1 1 char-in-nav-group? 1 2 char-in-nav-group? or if
                1 $D char-to-tri
            then
        then
    then
    $32B story-flag? not if
        2 0 $14 door-bits
        3 1 $14 door-bits
        $32C story-flag? not if
            3 room4E.cmd01
            1 room4E.cmd00
        else
            2 room4E.cmd01
            0 room4E.cmd00
        then
    else
        2 1 $14 door-bits
        3 0 $14 door-bits
        1 room4E.cmd00
        1 4 $2008000 nav-group
    then
    $299 story-flag? $29A story-flag? not and if
        0 13.96 51.0 -5.34 flicker-sprite
    then
    room4E.cmd02
    1 $2300 sound-volume
    0 7 0.4 0.0 0.2 0.5 zone-rect
;

: room4E.char-enter ( -- )   \ 0040AF40
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
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
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
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
;

: room4E.phase1 ( -- )   \ 0040B040
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
        $76 story-flag? not if
            3 0 char-remove
        then
    then
    0 7 char-entered-area? if
        $76 story-flag? not if
            $26 3 $FF char-load
        then
        3 exit-prepare
    then
    1 ebit? not if
        $32B story-flag? not $32C story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                $FE $13 char-entered-area? $FE 0 char-action? and if
                    0 $FE 4 action
                then
            then
        then
    then
    $32B story-flag? not $32C story-flag? and if
        0 -39.93 50.0 59.59 $C 6 0 zone
        4 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        4 ebit-set
                        $32 chance? if
                            $1F 0 var-set
                            0 1 $93 action
                        then
                    then
                then
            then
        then
    then
    $299 story-flag? not if
        3 13.96 50.0 -5.34 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 665 var-set
                $1A 666 var-set
                $1B 0 var-set
                $1C 13960 var-set
                $1D 51000 var-set
                $1E -5340 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: room4E.phase2 ( -- )   \ 0040B160
    $45 story-flag? if
        0 $10 char-in-area? 0 0 $32 char-heading? and if
            5 0 0 scene-change
        then
        0 $11 char-in-area? 0 90 $32 char-heading? and if
            5 1 0 scene-change
        then
    else 0 $12 char-in-area? 0 -45 $32 char-heading? and if
        5 2 0 scene-change
    then then
    2 ebit? not if
        0 $13 char-in-area? 0 0 $3C char-heading? and if
            $32B story-flag? not if
                $FE char-here? not if
                    5 3 5 scene-change
                else $FE $13 char-in-area? $FE 0 40 chars-within? or if
                    $8016 scene-ending
                else
                    5 3 5 scene-change
                then then
            else
                5 5 0 scene-change
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        $FE char-busy? not if
            2 ebit-clear
        then
    else
        2 ebit-clear
    then then
    $299 story-flag? $29A story-flag? not and if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room4E.phase5 ( -- )   \ 0040B200
    2 ebit? if
        $32C story-flag-set
        $32B story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            $FE 1 stalker-rage
        then
    then
    1 char-here? 1 2 char-C4? and if
        1 1 char-in-nav-group? if
            1 1 char-heal
            1 0 char-set-C4
        then
    then
;

: room4E.act00 ( -- )   \ 0040B230
    self-wait-done
    0 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room4E.act01 ( -- )   \ 0040B240
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room4E.act02 ( -- )   \ 0040B250
    0 ebit? not if
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        1 message
        wait-message
        0 ebit-set
    else
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room4E.act03 ( -- )   \ 0040B2A0
    1 self-scripted
    1 ebit-set
    self-wait-done
    0 0 char-file-load
    $56 -40.0 54.64 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $32C story-flag? not if
        0 room4E.cmd00
        0 0 var-set
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            0 room4E.cmd01
            yield
            0 30 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        2 room4E.cmd01
        $32C story-flag-set
    else
        0 0 var-set
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            1 room4E.cmd01
            yield
            0 28 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        3 room4E.cmd01
        $32C story-flag-clear
        1 room4E.cmd00
    then
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: room4E.act04 ( -- )   \ 0040B350
    2 ebit-set
    self-wait-done
    $FE 1 char-file-load
    $56 -40.0 54.64 0 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 5 6 char-sound
    $8000 5 self-anim-blend
    self-wait-anim
    $E00 5 self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    $32B story-flag-set
    $FE 1 6 char-sound
    $FE 1 stalker-rage
    2 1 $14 door-bits
    3 0 $14 door-bits
    1 4 $2008000 nav-group
    1 room4E.cmd00
    -37.0 66.0 60.0 0 538976288 4 -15.5 scene-effect-8C
    -41.0 66.0 60.0 0 538976288 4 -15.5 scene-effect-8C
    -37.0 60.0 60.0 0 538976288 4 -9.5 scene-effect-8C
    -41.0 60.0 60.0 0 538976288 4 -9.5 scene-effect-8C
    -37.0 54.0 60.0 0 538976288 4 -3.5 scene-effect-8C
    -41.0 54.0 60.0 0 538976288 4 -3.5 scene-effect-8C
    self-wait-anim
    2 ebit-clear
    self-idle-or-end
;

: room4E.act05 ( -- )   \ 0040B438
    self-wait-done
    0 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room4E.act06 ( -- )   \ 0040B450
    self-wait-done
    13.96 -5.34 self-turn-to-xz
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
            $29A story-flag-set
            0 effect-remove
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

' room4E.enter $4E 0 room-script!
' room4E.char-enter $4E 6 room-script!
' room4E.phase1 $4E 1 room-script!
' room4E.phase2 $4E 2 room-script!
' room4E.phase5 $4E 5 room-script!
' room4E.act00 $4E $00 action-script!
' room4E.act01 $4E $01 action-script!
' room4E.act02 $4E $02 action-script!
' room4E.act03 $4E $03 action-script!
' room4E.act04 $4E $04 action-script!
' room4E.act05 $4E $05 action-script!
' room4E.act06 $4E $06 action-script!

\ ---- room $4F ----------------------------------------------------------------------------------

\ room 0x4F (Room4F_Cmd00_ptmf): object byte 3 by byte 4: 0 up (+0x10 0), 1 down (-0.65), 2
\ lowered a step (0.02, not during a movie); once down, events bit 3
: room4F.cmd00 ( b0 b1 -- )  drop drop s" room4F.cmd00" stub-step ;
\ room 0x4F (Room4F_Cmd01_ptmf): an effect (DustShaft_vtable, 0x7460 bytes), not started
: room4F.cmd01 ( -- )  s" room4F.cmd01" stub-step ;
\ byte 3 0: a SpiralSmoke_vtable effect (0x36C0 bytes) spawned, its slot kept in event var 0;
\ else that slot's effect removed
: room4F.cmd02 ( b0 -- )  drop s" room4F.cmd02" stub-step ;
\ lower the object (pstr_sikakebox)'s +0x14 by 0.025 a frame down to -0.78, then event 6 (+0x5C)
: room4F.cmd03 ( -- )  s" room4F.cmd03" stub-step ;
\ Room4F_Cond00
: room4F.cond00? ( -- flag )  s" room4F.cond00?" stub-flag ;

: room4F.enter ( -- )   \ 0040B520
    room-sounds
    $52 story-flag? if
        0 1 room4F.cmd00
    then
    $50 story-flag? if
        1 1 room4F.cmd00
    then
    $51 story-flag? if
        2 1 room4F.cmd00
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
        room4F.cmd01
    else
        5 1 $14 door-bits
        6 0 $14 door-bits
        2 1 $14 door-bits
        0 room4F.cmd02
    then
    $73 story-flag? not $51 story-flag? not and if
        0 12.0 13.0 -24.0 flicker-sprite
    then
    $2A3 story-flag? $2A4 story-flag? not and if
        1 52.04 1.0 5.68 flicker-sprite
    then
;

: room4F.char-enter ( -- )   \ 0040B5C0
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

: room4F.phase1 ( -- )   \ 0040B680
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
                room4F.cond00? if
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

: room4F.phase2 ( -- )   \ 0040B8A0
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

: room4F.act00 ( -- )   \ 0040B940
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

: room4F.act01 ( -- )   \ 0040B9D0
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

: room4F.act02 ( -- )   \ 0040BA10
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

: room4F.act03 ( -- )   \ 0040BAC0
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

: room4F.act04 ( -- )   \ 0040BB20
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

: room4F.act05 ( -- )   \ 0040BB70
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
    1 room4F.cmd02
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
        1 1 room4F.cmd00
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
        room4F.cmd01
        4 6 sound-stop
        $50 story-flag-set
    else
        2 1 $14 door-bits
        5 1 $14 door-bits
        0 room4F.cmd02
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

: room4F.act06 ( -- )   \ 0040BD10
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
            2 2 room4F.cmd00
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

: room4F.act07 ( -- )   \ 0040BD80
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
            0 2 room4F.cmd00
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

: room4F.act08 ( -- )   \ 0040BDF0
    begin
        $44C cutscene-cue-reached? not while
        yield
    repeat
    3 ebit-clear
    1 6 0.0 30.0 -27.0 0 0 sound
    begin
        1 2 room4F.cmd00
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: room4F.act09 ( -- )   \ 0040BE20
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

: room4F.act0A ( -- )   \ 0040BE40
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

: room4F.act0B ( -- )   \ 0040BE60
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

: room4F.act0C ( -- )   \ 0040BEB0
    self-frames-reset
    self-wait-16
    0 1 $14 door-bits
    0 6 -38.0 5.0 -8.0 0 0 sound
    6 ebit-clear
    begin
        room4F.cmd03
        6 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: room4F.act0D ( -- )   \ 0040BEE0
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

: room4F.act0E ( -- )   \ 0040BF40
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

: room4F.act0F ( -- )   \ 0040BF60
    room-sounds
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    self-wait-done
    0 1 room4F.cmd00
    2 1 room4F.cmd00
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

: room4F.act10 ( -- )   \ 0040C000
    0 1 room4F.cmd00
    1 1 room4F.cmd00
    2 1 room4F.cmd00
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

' room4F.enter $4F 0 room-script!
' room4F.char-enter $4F 6 room-script!
' room4F.phase1 $4F 1 room-script!
' room4F.phase2 $4F 2 room-script!
' room4F.act00 $4F $00 action-script!
' room4F.act01 $4F $01 action-script!
' room4F.act02 $4F $02 action-script!
' room4F.act03 $4F $03 action-script!
' room4F.act04 $4F $04 action-script!
' room4F.act05 $4F $05 action-script!
' room4F.act06 $4F $06 action-script!
' room4F.act07 $4F $07 action-script!
' room4F.act08 $4F $08 action-script!
' room4F.act09 $4F $09 action-script!
' room4F.act0A $4F $0A action-script!
' room4F.act0B $4F $0B action-script!
' room4F.act0C $4F $0C action-script!
' room4F.act0D $4F $0D action-script!
' room4F.act0E $4F $0E action-script!
' room4F.act0F $4F $0F action-script!
' room4F.act10 $4F $10 action-script!

\ ---- room $50 ----------------------------------------------------------------------------------

: room50.cmd00 ( -- )  s" room50.cmd00" stub-step ;

: room50.enter ( -- )   \ 0040C190
    room-sounds
    1 0 $1000000 nav-group
    1 0 $4000000 nav-group
    $326 story-flag? if
        0 1 $14 door-bits
        1 0 $20000 nav-group
        0 1 object-show
        1 1 object-show
    else
        0 0 $14 door-bits
        0 0 $20000 nav-group
        0 0 object-show
        1 0 object-show
    then
    $6F story-flag? not if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                $75 story-flag? not $76 story-flag? or if
                    6 ebit-set
                then
            then
        then
    then
    0 1 $20000 nav-group
    6 ebit? if
        $18 state-flag-set
        7 state-flag-set
        $21 state-flag-set
        $23 state-flag-set
        $FE char-activate
        $FE $50 7 2 stalker-to-room
        $FE 2 2 char-camera
        0 $FE $F action
    then
    0 -3.3 31.4 -4.3 0 effect-86
    1 -5.7 31.4 0.3 0 effect-86
    2 -2.8 31.7 5.0 0 effect-86
    3 2.9 31.1 -4.4 0 effect-86
    4 5.6 31.5 0.5 0 effect-86
    5 2.7 31.7 5.0 0 effect-86
    room50.cmd00
;

: room50.act0C ( -- )   \ 0040CA20
    4 ebit-set
    3 self-is? $22 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    $11 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $11 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $11 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $11 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    3 self-is? $22 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE $D action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE $E action
    then then then
    3 0 var-set
    $11 pvar-inc
    exit
;

: room50.char-enter ( -- )   \ 0040C270
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
    $FE self-is? if
        9 state-flag? if
            4 ebit-set
            room50.act0C
        else
            4 ebit-clear
        then
    then
;

: room50.phase1 ( -- )   \ 0040C2C0
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    6 ebit? if
        1 -27.99 0.0 -19.65 $1E 8 0 zone
        7 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        7 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $92 action
                        then
                    then
                then
            then
        then
        1 -27.99 0.0 -19.65 $1E 8 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    $1F 1 var-set
                    $1F 8.0 hewie-look-zone
                then
            then
        then
        6 sound-bank-loaded? 3 stalker-kind? and if
            3 $C0000004 6 char-sound
        then
    then
    2 19.24 0.0 -32.32 $16 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    3 -32.619 0.0 -33.47 $16 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 19.13 0.0 -1.45 $11 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000003 6 -32.0 0.0 -21.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000003 6 -32.0 0.0 -21.0 0 0 sound
    then
;

: room50.phase2 ( -- )   \ 0040C420
    0 5 char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 45 $32 char-heading? and if
        5 1 0 scene-change
    then
    0 7 char-in-area? 0 -45 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    6 ebit? not if
        0 $A char-in-area? 0 -45 $32 char-heading? and if
            5 5 0 scene-change
        then
    then
    0 $B char-in-area? 0 90 $32 char-heading? and if
        5 6 0 scene-change
    then
    0 $C char-in-area? 0 0 $3C char-heading? and if
        6 ebit? if
            5 $12 0 scene-change
        else $FE char-here? not if
            5 7 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    6 ebit? if
        0 3 $14 0 $2D char-touching-facing? if
            5 $10 0 scene-change
        then
    then
;

: room50.phase5 ( -- )   \ 0040C4C0
    6 ebit? if
        2 $FE 0 char-model-op
        $FE action-end
        $FE char-done
        $18 state-flag-clear
        $23 state-flag-clear
        $21 state-flag-clear
        7 state-flag-clear
    then
;

: room50.act00 ( -- )   \ 0040C4E0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 $5F 18.0 -25.5 180 char-to-xz
    1 15.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    self-wait-16
    2 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act01 ( -- )   \ 0040C530
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 $1E 12.5 -3.0 90 char-to-xz
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    1 1.0 45.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    3 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act02 ( -- )   \ 0040C590
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 6 -30.0 -33.5 -90 char-to-xz
    1 10.0 5.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    4 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act03 ( -- )   \ 0040C5E0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 $71 -13.0 21.0 -90 char-to-xz
    1 15.0 -5.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    5 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act04 ( -- )   \ 0040C630
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 $55 -1.0 3.0 -90 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    1 5.0 20.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    6 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act05 ( -- )   \ 0040C690
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 7 -28.0 -20.0 -90 char-to-xz
    1 15.0 10.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    7 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act06 ( -- )   \ 0040C6E0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 $19 -2.0 -24.0 180 char-to-xz
    1 20.0 20.0 0.0 1.0 event-camera
    self-frames-reset
    self-wait-16
    8 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room50.act08 ( -- )   \ 0040C8D0
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $326 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room50.act09 ( -- )   \ 0040C900
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    $FE char-here? if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 2 object-anim
    1 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 1 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $326 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room50.act0A ( -- )   \ 0040C950
    1 self-scripted
    counter-inc
    1 ebit-set
    0 camera-follow
    9 state-flag-clear
    $FE char-here? 0 0 var? and if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $326 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room50.act07 ( -- )   \ 0040C730
    $18 state-flag-set
    4 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $326 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 $B action-force
        2 ebit-set
        1 3 char-file-load
    then
    0 2 char-file-load
    $FE 4 char-file-load
    $7B 19.42 25.02 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    2 ebit? if
        1 self-look-at
        yield
        begin
            3 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    0 ebit? not if
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 0 object-anim
        1 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 1 object-anim
        1 1 object-anim
        self-frames-reset
        $E self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 0 $20000 nav-group
    $18 state-flag-clear
    9 state-flag-set
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 2 6 char-sound
    then
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] room50.act08 goto
                else
                    ['] room50.act09 goto
                then
            else $FF panic-stage? if
                ['] room50.act0A goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            4 ebit? $FE char-here? not and if
                4 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] room50.act0A goto
            then
            counter-inc
            0 camera-follow
            9 state-flag-clear
            $FE char-here? if
                $FE 0 stalker-mode
                $FE action-end
            then
            1 self-noclip
            0 2 object-anim
            1 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            0 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            0 1 6 char-sound
            self-wait-anim
            0 self-noclip
            $FE char-here? if
                $FE self-look-at
                yield
                $FE self-turn-to
                self-wait-done
            then
            $326 story-flag-clear
            0 0 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room50.act0B ( -- )   \ 0040C9A0
    1 self-scripted
    self-wait-done
    $23 24.09 21.5 0 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
        $8000 5 self-anim-9
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-wait-anim
    then
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                1 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    0 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            1 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                0 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room50.act0D ( -- )   \ 0040CAF0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $23 21.86 24.14 0 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $19 self-wait-frames
    $FE 5 6 char-sound
    $28 threat-raise
    self-frames-reset
    9 self-wait-frames
    $FE 5 6 char-sound
    $14 threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: room50.act0E ( -- )   \ 0040CB60
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $23 21.86 24.14 0 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $3C 14.4 16.6 28 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    0 1 var-set
    3 self-is? $22 self-is? or if
        0 3 object-anim
        1 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $10 self-wait-frames
        0 2 var-set
        self-frames-reset
        $A self-wait-frames
        $FE 0 6 char-sound
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        9 state-flag? if
            2 avoid-prompt
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room50.act0F ( -- )   \ 0040CC10
    self-wait-done
    1 self-noclip
    1 self-scripted
    $FE 7 -25.05 -20.33 -90 char-to-xz
    1 1 $20000 nav-group
    $FE 5 char-file-load
    $FE char-file-use
    2 $FE 1 char-model-op
    begin
        3 $40000004 6 char-sound
        $8000 self-anim
        self-wait-anim
    again
;

: room50.act10 ( -- )   \ 0040CC50
    $6F story-flag-set
    6 ebit-clear
    1 self-scripted
    self-wait-done
    $F $54 fade
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
    wait-fade
    $FE action-end
    3 4 0 char-model-op
    1 char-here? if
        5 ebit-set
        1 action-end
        1 char-done
    then
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
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    0 char-in
    0 $57 -1.12 -15.98 -108 char-to-xz
    camera-restart
    8 state-flag-set
    0 1 $20000 nav-group
    2 0 char-remove
    $22 partner-load
    2 char-unload
    $FE char-activate
    $FE $50 7 2 stalker-to-room
    0 $FE $11 action
    8 state-flag-clear
    5 ebit? if
        1 char-activate
        $50 0 30 hewie-to-room
    then
    $F $51 fade
    wait-fade
    $2D resident-flag-set
    0 self-scripted
    7 state-flag-clear
    self-idle-or-end
;

: room50.act11 ( -- )   \ 0040CD80
    $18 state-flag-clear
    7 state-flag-clear
    $21 state-flag-clear
    $23 state-flag-clear
    $FE 7 -24.52 -20.6 -68 char-to-xz
    $FE 2 2 char-camera
    self-idle-or-end
;

: room50.act12 ( -- )   \ 0040CDA8
    self-wait-done
    $FE self-look-at
    yield
    9 message
    wait-message
    self-idle-or-end
;

: room50.act13 ( -- )   \ 0040CDC0
    0 -3.3 31.4 -4.3 0 effect-86
    1 -5.7 31.4 0.3 0 effect-86
    2 -2.8 31.7 5.0 0 effect-86
    3 2.9 31.1 -4.4 0 effect-86
    4 5.6 31.5 0.4 0 effect-86
    5 2.7 31.7 5.0 0 effect-86
    self-wait-done
    3 partner-load
    2 char-unload
    2 $FE 1 char-model-op
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
    3 4 0 char-model-op
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room50.enter $50 0 room-script!
' room50.char-enter $50 6 room-script!
' room50.phase1 $50 1 room-script!
' room50.phase2 $50 2 room-script!
' room50.phase5 $50 5 room-script!
' room50.act00 $50 $00 action-script!
' room50.act01 $50 $01 action-script!
' room50.act02 $50 $02 action-script!
' room50.act03 $50 $03 action-script!
' room50.act04 $50 $04 action-script!
' room50.act05 $50 $05 action-script!
' room50.act06 $50 $06 action-script!
' room50.act07 $50 $07 action-script!
' room50.act08 $50 $08 action-script!
' room50.act09 $50 $09 action-script!
' room50.act0A $50 $0A action-script!
' room50.act0B $50 $0B action-script!
' room50.act0C $50 $0C action-script!
' room50.act0D $50 $0D action-script!
' room50.act0E $50 $0E action-script!
' room50.act0F $50 $0F action-script!
' room50.act10 $50 $10 action-script!
' room50.act11 $50 $11 action-script!
' room50.act12 $50 $12 action-script!
' room50.act13 $50 $13 action-script!

\ ---- room $51 ----------------------------------------------------------------------------------

\ room 0x51 (Room51_Cmd00_ptmf): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves
: room51.cmd00 ( b0 -- )  drop s" room51.cmd00" stub-step ;
: room51.cmd01 ( -- )  s" room51.cmd01" stub-step ;

: room51.enter ( -- )   \ 0040CF40
    room-sounds
    1 0 $1000000 nav-group
    1 0 $4000000 nav-group
    $327 story-flag? if
        0 1 $14 door-bits
        1 0 $20000 nav-group
        8 1 object-show
        9 1 object-show
    else
        0 0 $14 door-bits
        0 0 $20000 nav-group
        8 0 object-show
        9 0 object-show
    then
    $328 story-flag? not $327 story-flag? not and if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                $75 story-flag? not $76 story-flag? or if
                    7 ebit-set
                    7 state-flag-set
                then
            then
        then
    then
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.5 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
    room51.cmd01
;

: room51.act0E ( -- )   \ 0040DC70
    4 ebit-set
    3 self-is? $22 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    $12 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $12 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $12 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $12 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    3 self-is? $22 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE $F action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE $10 action
    then then then
    3 0 var-set
    $12 pvar-inc
    exit
;

: room51.char-enter ( -- )   \ 0040D000
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
    $FE self-is? if
        7 ebit-clear
        7 state-flag-clear
        9 state-flag? if
            4 ebit-set
            room51.act0E
        else
            4 ebit-clear
        then
    then
;

: room51.phase1 ( -- )   \ 0040D060
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    $328 story-flag? not $327 story-flag? not and if
        0 17.96 0.0 -27.69 $F 18 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        0 17.96 0.0 -27.69 $F 18 0 zone
        1 char-here? 2 game-mode? not and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 18.0 hewie-look-zone
                then
            then
        then
    then
    1 1.58 0.0 -9.28 $10 8 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    6 ebit-set
                    $64 chance? if
                        $1F 1 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    2 17.16 0.0 30.67 $16 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    3 -33.99 0.0 32.91 $16 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 15.24 0.0 -0.28 $11 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000003 6 -32.0 0.0 21.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000003 6 -32.0 0.0 21.0 0 0 sound
    then
;

: room51.phase2 ( -- )   \ 0040D1F0
    0 5 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 45 $32 char-heading? and if
        5 1 0 scene-change
    then
    0 7 char-in-area? 0 -45 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 -45 $32 char-heading? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 0 $32 char-heading? and if
        5 6 0 scene-change
    then
    -2147483646 scene-request? if
        5 7 1 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            7 ebit? $52 door-locked? not and if
                5 $11 5 scene-change
            else
                5 9 5 scene-change
            then
        else
            $8016 scene-ending
        then
    then
;

: room51.act00 ( -- )   \ 0040D290
    $43 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-set
        0 $5F 18.0 25.5 0 char-to-xz
        1 15.0 10.0 0.0 0.0 event-camera
        self-frames-reset
        self-wait-16
        2 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
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
            $17 state-flag-clear
            1 char-here? if
                9 ebit-clear
                1 2 char-C4? if
                    0 1 $86 action
                else
                    1 action-end
                    1 char-done
                then
            else
                9 ebit-set
            then
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
            things-clear
            0 self-move-16
            0 $27 15.929 21.234 -2 char-to-xz
            camera-restart
            9 ebit? not if
                1 2 char-C4? if
                    1 0 char-visible
                    1 action-end
                else
                    1 char-activate
                    $51 0 86 hewie-to-room
                    1 $56 1.98 5.93 -2 char-to-xz
                then
            then
            $4D $68 door-copy
            $68 door-close-off-lock
            $4E $69 door-copy
            $69 door-close-off-lock
            $50 $6A door-copy
            $6A door-close-off-lock
            $53 $6B door-copy
            $6B door-close-off-lock
            exits-rebuild
            1 $4D char-in-room? if
                $68 0 -1 hewie-to-room
            then
            $FE char-here? not stalker-active? and $FE 2 char-C4? not and if
                $FE $4B -1 2 stalker-to-room
            then
            $43 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $F $41 fade
            wait-fade
            1 message
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message
        wait-message
    then
    self-idle-or-end
;

: room51.act01 ( -- )   \ 0040D450
    $44 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-set
        0 $1E 12.5 3.0 90 char-to-xz
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
        1 1.0 45.0 0.0 5.0 event-camera
        self-frames-reset
        self-wait-16
        3 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
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
            wait-fade
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $17 state-flag-clear
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
            0 self-move-16
            0 $55 10.203 -1.688 57 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            1 0 char-visible
            1 action-end
            $58 $82 door-copy
            $82 door-close-off-lock
            $5A $83 door-copy
            $83 door-close-off-lock
            exits-rebuild
            1 $5F 0 room-doors-state
            $FE $5F 0 room-doors-state
            $44 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $2B resident-flag-set
            $F $41 fade
            wait-fade
            1 message
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message
        wait-message
    then
    self-idle-or-end
;

: room51.act02 ( -- )   \ 0040D5E0
    $45 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-set
        0 6 -30.0 33.5 -90 char-to-xz
        1 10.0 5.0 0.0 2.0 event-camera
        self-frames-reset
        self-wait-16
        4 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
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
            wait-fade
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $17 state-flag-clear
            things-clear
            $10 $78 movie-param
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
            0 self-move-16
            0 6 -24.369 32.751 -92 char-to-xz
            camera-restart
            1 0 char-visible
            1 action-end
            $78 door-unlock
            $5C $88 door-copy
            $5C door-reopen-unlock
            $88 door-close-off-lock
            exits-rebuild
            $FE char-here? not stalker-active? and $FE 2 char-C4? not and if
                $FE $4B -1 2 stalker-to-room
            then
            $45 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $F $41 fade
            wait-fade
            1 message
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message
        wait-message
    then
    self-idle-or-end
;

: room51.act08 ( -- )   \ 0040D8E0
    self-frames-reset
    8 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 answer? if
        $902 self-anim
        0 exit-door-open? if
            0 room51.cmd00
            self-wait-anim
            0.0 10.0 -40.0 self-look-at-point
            yield
            $903 self-anim
            1 room51.cmd00
            self-frames-reset
            self-wait-16
        else
            self-wait-anim
            0.0 10.0 -40.0 self-look-at-point
            yield
            $903 self-anim
        then
        $72 5 0.0 0.0 -40.0 0 0 sound
        $52 door-open-clear
        doors-room-in
        $52 door-lock
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        self-wait-16
        $FF self-look-at
        yield
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room51.act03 ( -- )   \ 0040D750
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 $71 -13.0 -21.0 -90 char-to-xz
    1 15.0 -5.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    5 message
    wait-message
    ['] room51.act08 goto
;

: room51.act04 ( -- )   \ 0040D790
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 $55 -1.0 -3.0 -90 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    1 5.0 20.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    6 message
    wait-message
    ['] room51.act08 goto
;

: room51.act05 ( -- )   \ 0040D7E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 7 -28.0 20.0 -90 char-to-xz
    1 15.0 10.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    7 message
    wait-message
    ['] room51.act08 goto
;

: room51.act06 ( -- )   \ 0040D820
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 $19 -2.0 24.0 0 char-to-xz
    1 20.0 20.0 0.0 1.0 event-camera
    self-frames-reset
    self-wait-16
    8 message
    wait-message
    ['] room51.act08 goto
;

: room51.act07 ( -- )   \ 0040D860
    $2C state-flag-set
    $19 state-flag-set
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $52 self-through-door
    self-wait-done
    $F $44 fade
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
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    0 $F9 $13 action
    $10 $28 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        movie-loop
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $F 1 fade
        $A cutscene-control
        begin
            7 cutscene-control
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
        until
    then
    $2C resident-flag-set
    0 0 char-no-shadow
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room51.act0A ( -- )   \ 0040DB20
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $327 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room51.act0B ( -- )   \ 0040DB50
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    $FE char-here? if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    8 2 object-anim
    9 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 1 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $327 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room51.act0C ( -- )   \ 0040DBA0
    1 self-scripted
    counter-inc
    1 ebit-set
    0 camera-follow
    9 state-flag-clear
    $FE char-here? 0 0 var? and if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    8 4 object-anim
    9 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $327 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room51.act09 ( -- )   \ 0040D970
    $18 state-flag-set
    4 ebit-clear
    7 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $327 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 $D action-force
        2 ebit-set
        1 $B char-file-load
    then
    0 $A char-file-load
    $FE $C char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    2 ebit? if
        1 self-look-at
        yield
        begin
            3 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    0 ebit? not if
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        8 0 object-show
        9 0 object-show
        8 0 object-anim
        9 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        8 0 object-show
        9 0 object-show
        8 1 object-anim
        9 1 object-anim
        self-frames-reset
        $E self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 0 $20000 nav-group
    $18 state-flag-clear
    9 state-flag-set
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 2 6 char-sound
    then
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] room51.act0A goto
                else
                    ['] room51.act0B goto
                then
            else $FF panic-stage? if
                ['] room51.act0C goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            4 ebit? $FE char-here? not and if
                4 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] room51.act0C goto
            then
            counter-inc
            0 camera-follow
            9 state-flag-clear
            $FE char-here? if
                $FE 0 stalker-mode
                $FE action-end
            then
            1 self-noclip
            8 2 object-anim
            9 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            0 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            0 1 6 char-sound
            self-wait-anim
            0 self-noclip
            $FE char-here? if
                $FE self-look-at
                yield
                $FE self-turn-to
                self-wait-done
            then
            $327 story-flag-clear
            0 0 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room51.act0D ( -- )   \ 0040DBF0
    1 self-scripted
    self-wait-done
    $7D 11.91 -21.5 180 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
        $8000 5 self-anim-9
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-wait-anim
    then
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                1 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    0 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            1 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                0 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room51.act0F ( -- )   \ 0040DD40
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $7B 14.14 -24.14 180 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $19 self-wait-frames
    $FE 5 6 char-sound
    $28 threat-raise
    self-frames-reset
    9 self-wait-frames
    $FE 5 6 char-sound
    $14 threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: room51.act10 ( -- )   \ 0040DDB0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $7B 14.14 -24.14 180 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $1D 9.84 -11.27 153 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    0 1 var-set
    3 self-is? $22 self-is? or if
        8 3 object-anim
        9 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $10 self-wait-frames
        0 2 var-set
        self-frames-reset
        $A self-wait-frames
        $FE 0 6 char-sound
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        9 state-flag? if
            2 avoid-prompt
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room51.act11 ( -- )   \ 0040DE60
    1 self-scripted
    $18 state-flag-set
    7 state-flag-set
    self-wait-done
    0 $D char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
    self-wait-done
    $FE char-activate
    $FE $51 7 2 stalker-to-room
    $FE 0 0 char-camera
    stalker-item-cooldown
    $FE 1 char-visible
    $FE 1 char-silent
    0 char-file-use
    0 counter-set
    0 $FE $12 action
    1 wait-counter
    0 $43 5 char-sound
    $8000 $A self-anim-blend
    self-wait-anim
    $8003 $A self-anim-blend
    self-wait-anim
    counter-inc
    $18 state-flag-clear
    7 state-flag-clear
    7 ebit-clear
    $328 story-flag-set
    0 self-scripted
    self-idle-or-end
;

: room51.act12 ( -- )   \ 0040DED0
    1 self-noclip
    1 self-scripted
    self-wait-done
    $FE $77 15.58 -33.18 0 char-to-xz
    1 self-noclip
    1 self-scripted
    $FE $C char-file-load
    $FE 0 char-visible
    $FE 0 char-silent
    0 0 $14 door-bits
    $FE char-file-use
    $327 story-flag-set
    1 0 $20000 nav-group
    counter-inc
    8 0 object-show
    9 0 object-show
    8 6 object-anim
    9 6 object-anim
    $8003 self-anim
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    2 wait-counter
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: room51.act13 ( -- )   \ 0040DF30
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        4 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: room51.act14 ( -- )   \ 0040DF50
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room51.act15 ( -- )   \ 0040E030
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
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

: room51.act16 ( -- )   \ 0040E110
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
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
    $10 $78 movie-param
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

: room51.act17 ( -- )   \ 0040E1F0
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
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
    0 $F9 $13 action
    $10 $28 movie-param
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

: room51.phase5 ( -- )   \ 0047AB98
    7 state-flag-clear
;

' room51.enter $51 0 room-script!
' room51.char-enter $51 6 room-script!
' room51.phase1 $51 1 room-script!
' room51.phase2 $51 2 room-script!
' room51.act00 $51 $00 action-script!
' room51.act01 $51 $01 action-script!
' room51.act02 $51 $02 action-script!
' room51.act03 $51 $03 action-script!
' room51.act04 $51 $04 action-script!
' room51.act05 $51 $05 action-script!
' room51.act06 $51 $06 action-script!
' room51.act07 $51 $07 action-script!
' room51.act08 $51 $08 action-script!
' room51.act09 $51 $09 action-script!
' room51.act0A $51 $0A action-script!
' room51.act0B $51 $0B action-script!
' room51.act0C $51 $0C action-script!
' room51.act0D $51 $0D action-script!
' room51.act0E $51 $0E action-script!
' room51.act0F $51 $0F action-script!
' room51.act10 $51 $10 action-script!
' room51.act11 $51 $11 action-script!
' room51.act12 $51 $12 action-script!
' room51.act13 $51 $13 action-script!
' room51.act14 $51 $14 action-script!
' room51.act15 $51 $15 action-script!
' room51.act16 $51 $16 action-script!
' room51.act17 $51 $17 action-script!
' room51.phase5 $51 5 room-script!

\ ---- room $52 ----------------------------------------------------------------------------------

: room52.cmd00 ( -- )  s" room52.cmd00" stub-step ;
\ 1 unless the object at +0x18 exists and its byte +0x28 is 1.
: room52.cond00? ( -- flag )  s" room52.cond00?" stub-flag ;

: room52.enter ( -- )   \ 0040E3B0
    room-sounds
    $35 1.0 0 bgm
    $4B story-flag? not if
        room52.cond00? if
            $52 1 323 2 5 6 0 0.0 creature-place
        then
    then
    room52.cmd00
    $75 story-flag? not if
        2 0 object-show
        3 0 object-show
        4 0 object-show
        5 0 object-show
        6 0 object-show
        7 0 object-show
        8 0 object-show
        9 0 object-show
        0 0 $14 door-bits
    else
        2 1 object-show
        3 1 object-show
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        9 1 object-show
        0 1 $14 door-bits
    then
    $10 -54.6 28.6 -20.0 $E $80 $80 $80 $40 specks
    $10 54.6 29.0 -20.0 $E $80 $80 $80 $40 specks
    $10 -7.5 5.0 53.5 $E $80 $80 $80 $40 specks
    $10 0.0 64.8 -20.5 $E $80 $80 $80 $40 specks
    1 0 $10000000 nav-group
;

: room52.char-enter ( -- )   \ 0040E480
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    0 self-is? if
        1 exit-taken? 2 exit-taken? or if
            2 map-page
        then
    then
;

: room52.phase1 ( -- )   \ 0040E550
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 4 char-entered-area? if
        2 exit-prepare
    then
    $75 story-flag? not if
        0 5 char-left-area? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    0 -48.85 0.0 -48.91 $21 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 48.42 -6.0 28.0 $1A 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    2 2.46 -6.0 33.29 $16 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    3 -22.19 -6.0 36.99 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    4 26.75 0.0 -36.99 $E 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: room52.phase2 ( -- )   \ 0040E680
    0 6 $3C char-faces-area? if
        5 2 0 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        5 3 0 scene-change
    then
    $76 story-flag? not if
        $18 1 item-count? not $19 1 item-count? not and $1A 1 item-count? not and $1B 1 item-count? not and $1C 1 item-count? not and if
            0 $A $3C char-faces-area? if
                5 4 0 scene-change
            then
            0 9 $3C char-faces-area? if
                5 5 0 scene-change
            then
            0 8 $3C char-faces-area? if
                5 6 0 scene-change
            then
            0 $B $3C char-faces-area? if
                5 7 0 scene-change
            then
            0 7 char-in-area? 0 90 $3C char-heading? and if
                5 8 0 scene-change
            then
        else
            0 $A $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 9 $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 8 $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 $B $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 7 char-in-area? 0 90 $3C char-heading? and if
                5 0 0 scene-change
            then
        then
    else
        0 $A $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 9 $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 8 $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 $B $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 7 char-in-area? 0 90 $3C char-heading? and if
            5 9 0 scene-change
        then
    then
    0 $D char-in-area? 0 -90 $3C char-heading? and if
        5 $D 0 scene-change
    then
    0 $E char-in-area? 0 -90 $3C char-heading? and if
        5 $D 0 scene-change
    then
;

: room52.act00 ( -- )   \ 0047ABA0
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room52.act01 ( -- )   \ 0040E780
    $75 story-flag-set
    $FE action-end
    $FE char-done
    0 state-flag-clear
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    0 0 door-flag-82
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
    1 char-here? 0 hewie-side? and if
        0 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        0 ebit-set
    then
    0 1 char-no-shadow
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
    2 1 object-show
    3 1 object-show
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    0 1 $14 door-bits
    0 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $52 0 182 hewie-to-room
            1 $B6 35.67 -1.81 99 char-to-xz
        then
    then
    0 $1C 54.36 -8.26 -43 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    yield
    camera-restart
    $58 story-flag? not if
        $58 story-flag-set
    then
    0 1 door-flag-82
    $F $51 fade
    $35 1.0 0 bgm
    wait-fade
    $18 state-flag-clear
    $12 state-flag-clear
    $FE $6C 0 door-lock-for
    self-idle-or-end
;

: room52.act02 ( -- )   \ 0047ABA8
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room52.act03 ( -- )   \ 0040E8D0
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    1 message
    wait-message
    $F 6 fade
    wait-fade
    2 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room52.act0A ( -- )   \ 0040EAD0
    $75 story-flag? $76 story-flag? not and if
        $FE $48 1 room-doors-state
        stalker-active? not if
            $FE $4B 217 2 stalker-to-room
            $FE 2 stalker-mode
        then
        stalker-item-cooldown
    then
    exit
;

: room52.act04 ( -- )   \ 0040E8F0
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $18 message-param-room
        $18 1 item-give-count
        0 $18 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        room52.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room52.act05 ( -- )   \ 0040E950
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $19 message-param-room
        $19 1 item-give-count
        0 $19 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $362 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        room52.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room52.act06 ( -- )   \ 0040E9B0
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1A message-param-room
        $1A 1 item-give-count
        0 $1A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $363 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        room52.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room52.act07 ( -- )   \ 0040EA10
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1B message-param-room
        $1B 1 item-give-count
        0 $1B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $364 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        room52.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room52.act08 ( -- )   \ 0040EA70
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1C message-param-room
        $1C 1 item-give-count
        0 $1C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $365 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        room52.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room52.act09 ( -- )   \ 0047ABB0
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room52.act0B ( -- )   \ 0040EAF0
    $18 state-flag-set
    1 self-scripted
    0 counter-set
    0 1 $C action
    self-wait-done
    1 self-turn-to
    self-wait-done
    1 wait-counter
    $C06 self-anim
    self-wait-anim
    0 self-anim
    self-frames-reset
    1 self-wait-frames
    1 self-look-at
    yield
    2 counter-set
    3 wait-counter
    0 self-scripted
    $18 state-flag-clear
    5 hewie-trust
    self-idle-or-end
;

: room52.act0C ( -- )   \ 0040EB20
    1 self-scripted
    self-wait-done
    0 self-turn-to
    self-wait-done
    1 counter-set
    2 wait-counter
    hewie-bark
    self-wait-done
    1 0 0 char-camera
    1 camera-follow
    $8D -18.0 29.0 0 $FFFF 5 self-move-to
    self-wait-done
    hewie-bark
    self-wait-done
    self-frames-reset
    $1E self-wait-frames
    0 0 0 char-camera
    0 camera-follow
    3 counter-set
    0 self-scripted
    self-idle-or-end
;

: room52.act0D ( -- )   \ 0040EB60
    self-wait-done
    -180 self-turn-angle
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    6 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room52.act0E ( -- )   \ 0040EB80
    self-wait-done
    room52.cmd00
    $10 -54.6 28.6 -20.0 $E $80 $80 $80 $40 specks
    $10 54.6 29.0 -20.0 $E $80 $80 $80 $40 specks
    $10 -7.5 5.0 53.5 $E $80 $80 $80 $40 specks
    $10 0.0 64.8 -20.5 $E $80 $80 $80 $40 specks
    0 0 door-flag-82
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
    0 1 char-no-shadow
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
    0 0 char-no-shadow
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room52.enter $52 0 room-script!
' room52.char-enter $52 6 room-script!
' room52.phase1 $52 1 room-script!
' room52.phase2 $52 2 room-script!
' room52.act00 $52 $00 action-script!
' room52.act01 $52 $01 action-script!
' room52.act02 $52 $02 action-script!
' room52.act03 $52 $03 action-script!
' room52.act04 $52 $04 action-script!
' room52.act05 $52 $05 action-script!
' room52.act06 $52 $06 action-script!
' room52.act07 $52 $07 action-script!
' room52.act08 $52 $08 action-script!
' room52.act09 $52 $09 action-script!
' room52.act0A $52 $0A action-script!
' room52.act0B $52 $0B action-script!
' room52.act0C $52 $0C action-script!
' room52.act0D $52 $0D action-script!
' room52.act0E $52 $0E action-script!

\ ---- room $53 ----------------------------------------------------------------------------------

: room53.enter ( -- )   \ 004268B0
    room-sounds
    $74 story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    8 0 object-show
    9 0 object-show
    $26D story-flag? not if
        0 15.82 1.0 20.96 flicker-sprite
    then
;

: room53.char-enter ( -- )   \ 004268F0
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

: room53.phase1 ( -- )   \ 00426930
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
;

: room53.phase2 ( -- )   \ 00426950
    0 3 $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 4 char-in-area? 0 90 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 5 char-in-area? 0 -45 $3C char-heading? and if
        5 4 0 scene-change
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        5 4 0 scene-change
    then
    0 7 char-in-area? 0 45 $3C char-heading? and if
        5 4 0 scene-change
    then
    0 8 char-in-area? 0 -45 $3C char-heading? and if
        5 5 0 scene-change
    then
    0 9 char-in-area? 0 -45 $3C char-heading? and if
        5 5 0 scene-change
    then
    0 $A char-in-area? 0 45 $3C char-heading? and if
        5 5 0 scene-change
    then
    0 $B $3C char-faces-area? if
        5 6 0 scene-change
    then
    $26D story-flag? not if
        0 0 5 -8 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
;

: room53.act00 ( -- )   \ 004269E0
    1 self-scripted
    3 stalker-kind? $22 stalker-kind? or if
        $FE action-end
        $FE char-done
    then
    self-wait-done
    self-frames-reset
    self-wait-16
    $F $44 fade
    wait-fade
    0 5 movie-play
    yield
    yield
    2 cutscene-control
    1 result? if
        1.0 movie-volume
        yield
        8 state-flag-set
        begin
            0 cutscene-control
            -1 result? not if
                6 2 pad? not if
                    yield
                    false
                else
                    true
                then
            else
                true
            then
        until
        1 cutscene-control
        begin
            movie-playing? while
            yield
        repeat
        yield
        8 state-flag-clear
    then
    $337 story-flag-set
    8 state-flag-set
    $E item-use
    3 stalker-kind? $22 stalker-kind? or if
        0 0 $14 door-bits
        1 1 $14 door-bits
        $74 story-flag-set
        yield
        yield
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
        3 4 0 char-model-op
        $FE $53 43 2 stalker-to-room
        $FE 0 0 char-camera
        0 $F9 7 action
        $FF panic-stage? if
            3 panic-stage
        then
        0 message-prepare
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
        $FE $53 0 room-doors-state
        $FE $75 0 door-lock-for
        $FE $53 43 2 stalker-to-room
        $FE char-activate
        $FE $2B 7.18 33.88 -160 char-to-xz
        stalker-item-cooldown
        1 self-scripted
        $333 story-flag-set
        $18 state-flag-clear
        0 $16 -1.36 20.03 0 char-to-xz
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
        yield
        camera-restart
    else
        0 $76 -6.79 9.91 168 char-to-xz
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
        yield
        camera-restart
        $18 state-flag-clear
    then
    8 0 object-show
    9 0 object-show
    $F $41 fade
    wait-fade
    $33 resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: room53.act01 ( -- )   \ 00426B60
    self-wait-done
    $337 story-flag? not if
        0 ebit? not if
            1 message
            wait-message
            0 ebit-set
        else
            2 message
            wait-message
        then
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: room53.act02 ( -- )   \ 00426B80
    self-wait-done
    $74 story-flag? not if
        3 message
        wait-message
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: room53.act03 ( -- )   \ 00426B90
    self-wait-done
    15.82 20.96 self-turn-to-xz
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
            $26D story-flag-set
            0 effect-remove
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

: room53.act04 ( -- )   \ 0047AD38
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: room53.act05 ( -- )   \ 00426BF0
    self-wait-done
    0 8 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    else 0 9 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    else 0 $A char-in-area? if
        90 self-turn-angle
        self-wait-done
    then then then
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    7 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room53.act06 ( -- )   \ 00426C20
    self-wait-done
    19.0 33.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room53.act07 ( -- )   \ 00426C40
    begin
        $8E8 cutscene-cue-reached? not while
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        $2D sprites-additive
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: room53.act08 ( -- )   \ 00426C60
    self-wait-done
    8 0 object-show
    9 0 object-show
    3 partner-load
    2 char-unload
    0 0 $14 door-bits
    1 1 $14 door-bits
    yield
    yield
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
    3 4 0 char-model-op
    $FE $53 43 2 stalker-to-room
    0 $F9 7 action
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    8 0 object-show
    9 0 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room53.enter $53 0 room-script!
' room53.char-enter $53 6 room-script!
' room53.phase1 $53 1 room-script!
' room53.phase2 $53 2 room-script!
' room53.act00 $53 $00 action-script!
' room53.act01 $53 $01 action-script!
' room53.act02 $53 $02 action-script!
' room53.act03 $53 $03 action-script!
' room53.act04 $53 $04 action-script!
' room53.act05 $53 $05 action-script!
' room53.act06 $53 $06 action-script!
' room53.act07 $53 $07 action-script!
' room53.act08 $53 $08 action-script!

\ ---- room $54 ----------------------------------------------------------------------------------

\ Room54_Cmd00
: room54.cmd00 ( b0 b1 -- )  drop drop s" room54.cmd00" stub-step ;
\ Room54_Cmd01
: room54.cmd01 ( -- )  s" room54.cmd01" stub-step ;

: room54.enter ( -- )   \ 00426D80
    room-sounds
    0 0 var-set
    1 0 var-set
    $63 story-flag? not if
        1 1 $14 door-bits
    then
    $64 story-flag? not if
        2 1 $14 door-bits
    then
    $65 story-flag? not if
        0 1 $14 door-bits
    then
    $66 story-flag? not if
        1 4 $20000 nav-group
        1 5 $1000000 nav-group
        8 1 object-show
    else
        1 5 $20000 nav-group
        2 0 room54.cmd00
    then
    $67 story-flag? if
        $B 1 $14 door-bits
    then
    $68 story-flag? if
        $A 1 $14 door-bits
    then
    $69 story-flag? if
        9 1 $14 door-bits
    then
    $6A story-flag? not if
        1 2 $20000 nav-group
        1 3 $1000000 nav-group
        1 $18A $10000000 nav-tri-flags
        1 $1D3 $10000000 nav-tri-flags
        4 1 object-show
    else
        1 3 $20000 nav-group
        1 $187 $10000000 nav-tri-flags
        1 $1D0 $10000000 nav-tri-flags
        1 $188 $10000000 nav-tri-flags
        1 $1D1 $10000000 nav-tri-flags
        1 0 room54.cmd00
    then
    $6B story-flag? if
        8 1 $14 door-bits
    then
    $6C story-flag? if
        6 1 $14 door-bits
    then
    $6D story-flag? if
        7 1 $14 door-bits
    then
    $6E story-flag? not if
        1 0 $20000 nav-group
        1 1 $1000000 nav-group
        0 1 object-show
        0 6 $200 nav-group
        room54.cmd01
    else
        1 1 $20000 nav-group
        0 0 room54.cmd00
    then
    $62 story-flag? not if
        0 0 0 $69 $12A obstacle-place
    else
        0 0 0 $87 $148 obstacle-place
    then
    1 7 $10000000 nav-group
    $273 story-flag? not if
        0 36.86 51.0 -27.91 flicker-sprite
    then
    $2A7 story-flag? $2A8 story-flag? not and if
        1 39.16 1.0 -38.76 flicker-sprite
    then
    $2B1 story-flag? $2B2 story-flag? not and if
        2 -23.61 51.0 -61.37 flicker-sprite
    then
;

: room54.char-enter ( -- )   \ 00426EF0
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    2 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    3 0 -1 area-camera
    0 self-is? if
        2 exit-taken? 3 exit-taken? or if
            3 map-page
        then
    then
;

: room54.phase1 ( -- )   \ 00426FF0
    1 exit-usable? if
        1 exit-check
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    $24 0 -1 1 chars-area-camera
    $25 1 -1 1 chars-area-camera
    $26 1 -1 -1 chars-area-camera
    $27 2 0 1 chars-area-camera
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    0 $148 obstacle-on? if
        $62 story-flag-set
    else
        $62 story-flag-clear
    then
    $2A7 story-flag? not if
        1 39.16 0.0 -38.76 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 679 var-set
                $1A 680 var-set
                $1B 1 var-set
                $1C 39160 var-set
                $1D 1000 var-set
                $1E -38760 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    $2B1 story-flag? not if
        2 -23.61 50.0 -61.37 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 689 var-set
                $1A 690 var-set
                $1B 2 var-set
                $1C -23610 var-set
                $1D 51000 var-set
                $1E -61370 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    $64 story-flag? not if
        3 -31.67 0.0 -62.5 $B 10 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        3 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        3 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 24.15 0.0 76.38 0 0 sound
                $40000008 6 -44.0 70.0 22.0 0 0 sound
            else 0 1 var? if
                $40000004 6 24.15 0.0 76.38 0 0 sound
                $40000009 6 -44.0 70.0 22.0 0 0 sound
            else 0 2 var? if
                $40000005 6 24.15 0.0 76.38 0 0 sound
                $4000000A 6 -44.0 70.0 22.0 0 0 sound
            else 0 3 var? if
                $40000006 6 24.15 0.0 76.38 0 0 sound
                $4000000B 6 -44.0 70.0 22.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: room54.phase2 ( -- )   \ 004272A0
    $63 story-flag? not if
        0 9 $32 char-faces-area? if
            5 9 4 scene-change
        then
    then
    $64 story-flag? not if
        0 $B char-in-area? $62 story-flag? and if
            5 $A 4 scene-change
        then
    then
    $65 story-flag? not if
        0 $A char-in-area? 0 90 $32 char-heading? and if
            5 $B 4 scene-change
        then
    then
    $66 story-flag? not if
        0 $D char-in-area? 0 -45 $32 char-heading? and if
            $67 story-flag? $68 story-flag? or if
                5 $C 4 scene-change
            else
                5 $C 0 scene-change
            then
        then
        0 $C char-in-area? 0 45 $32 char-heading? and if
            5 $10 0 scene-change
        then
    else
        0 $10 char-in-area? 0 -45 $32 char-heading? and if
            5 $F 0 scene-change
        then
        0 $F char-in-area? 0 45 $32 char-heading? and if
            5 $10 0 scene-change
        then
    then
    $6A story-flag? not if
        0 $11 char-in-area? 0 45 $32 char-heading? and if
            $69 story-flag? $6B story-flag? or if
                5 $D 4 scene-change
            else
                5 $D 0 scene-change
            then
        then
    else 0 $13 char-in-area? 0 45 $32 char-heading? and if
        5 $11 0 scene-change
    then then
    $6E story-flag? not if
        0 $14 char-in-area? 0 90 $32 char-heading? and if
            $6C story-flag? $6D story-flag? or if
                5 $E 4 scene-change
            else
                5 $E 0 scene-change
            then
        then
        0 $15 char-in-area? 0 0 $32 char-heading? and if
            5 $13 0 scene-change
        then
    else
        0 $17 char-in-area? 0 90 $32 char-heading? and if
            5 $12 0 scene-change
        then
        0 $18 char-in-area? 0 0 $32 char-heading? and if
            5 $13 0 scene-change
        then
    then
    0 $19 char-in-area? 0 90 $32 char-heading? and if
        5 $12 0 scene-change
    then
    0 $1B char-in-area? 0 0 $32 char-heading? and if
        5 $13 0 scene-change
    then
    0 $1C char-in-area? 0 90 $32 char-heading? and if
        5 $14 0 scene-change
    then
    0 $1D char-in-area? 0 0 $32 char-heading? and if
        5 $15 0 scene-change
    then
    0 $20 char-in-area? 0 -45 $32 char-heading? and if
        5 $16 0 scene-change
    then
    0 $21 char-in-area? 0 $22 char-in-area? or 0 45 $32 char-heading? and if
        5 $17 0 scene-change
    then
    0 $1E char-in-area? 0 90 $32 char-heading? and if
        5 $18 0 scene-change
    then
    0 $1F char-in-area? 0 0 $32 char-heading? and if
        5 $19 0 scene-change
    then
    $66 story-flag? not if
        0 $1A char-in-area? 0 90 $32 char-heading? and if
            5 $1A 0 scene-change
        then
    then
    0 8 char-in-area? 0 22 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $23 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    $273 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $1B 4 scene-change
        then
    then
    $2A7 story-flag? $2A8 story-flag? not and if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $1C 4 scene-change
        then
    then
    $2B1 story-flag? $2B2 story-flag? not and if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $1D 4 scene-change
        then
    then
;

: room54.act00 ( -- )   \ 00427490
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    1 char-here? 0 hewie-side? and if
        4 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    8 0 object-show
    $66 story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    0 ebit-set
    2 3 room54.cmd00
    begin
        2 1 room54.cmd00
        2 4 room54.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 0.0 -52.5 10.0 0 0 sound
    0 4 $20000 nav-group
    1 5 $20000 nav-group
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $54 0 221 hewie-to-room
            1 $DD -27.0 37.0 60 char-to-xz
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act01 ( -- )   \ 00427570
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    $B 1 $14 door-bits
    $67 story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act02 ( -- )   \ 004275E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    $A 1 $14 door-bits
    $68 story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act03 ( -- )   \ 00427650
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    9 1 $14 door-bits
    $69 story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act04 ( -- )   \ 004276C0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    3 5 room54.cmd00
    0 $18A $10000000 nav-tri-flags
    0 $1D3 $10000000 nav-tri-flags
    1 $187 $10000000 nav-tri-flags
    1 $1D0 $10000000 nav-tri-flags
    1 $188 $10000000 nav-tri-flags
    1 $1D1 $10000000 nav-tri-flags
    1 25.0 20.0 50.0 2.0 event-camera
    1 char-here? 0 hewie-side? and if
        4 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    4 0 object-show
    $6A story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    40.0 15.0 25.0 self-look-at-point
    yield
    0 ebit-set
    1 3 room54.cmd00
    begin
        1 2 room54.cmd00
        1 4 room54.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 39.0 21.0 10.0 0 0 sound
    0 2 $20000 nav-group
    1 3 $20000 nav-group
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $54 0 22 hewie-to-room
            1 $16 -41.0 -4.0 -100 char-to-xz
        then
    then
    $FF self-look-at
    yield
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act05 ( -- )   \ 004277F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    8 1 $14 door-bits
    $6B story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act06 ( -- )   \ 00427860
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    6 1 $14 door-bits
    $6C story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act07 ( -- )   \ 004278D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    7 1 $14 door-bits
    $6D story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act08 ( -- )   \ 00427940
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    1 char-here? 0 hewie-side? and if
        4 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    0 0 object-show
    $6E story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    0 ebit-set
    0 3 room54.cmd00
    begin
        0 2 room54.cmd00
        0 4 room54.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 0.0 0.0 10.0 0 0 sound
    0 0 $20000 nav-group
    1 1 $20000 nav-group
    1 6 $200 nav-group
    room54.cmd01
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $54 0 255 hewie-to-room
            1 $FF 38.0 -20.0 90 char-to-xz
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act09 ( -- )   \ 00427A30
    self-wait-done
    -35.0 66.0 self-turn-to-xz
    self-wait-done
    0 message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    1 0 $14 door-bits
    $63 story-flag-set
    $15 message-param-room
    $15 1 item-give-count
    0 $15 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room54.act0A ( -- )   \ 00427A80
    self-wait-done
    -25.0 -60.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    2 0 $14 door-bits
    $64 story-flag-set
    $16 message-param-room
    $16 1 item-give-count
    0 $16 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: room54.act0B ( -- )   \ 00427AD0
    self-wait-done
    1.0 -30.0 self-turn-to-xz
    self-wait-done
    2 message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0 $14 door-bits
    $65 story-flag-set
    $17 message-param-room
    $17 1 item-give-count
    0 $17 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room54.act0C ( -- )   \ 00427B20
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    $67 story-flag? $68 story-flag? or if
        $904 self-anim
        self-wait-anim
        $67 story-flag? if
            $B 0 $14 door-bits
            $67 story-flag-clear
            $905 self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            $A 0 $14 door-bits
            $68 story-flag-clear
            $905 self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $A message
        wait-message
        self-frames-reset
        4 self-wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act0D ( -- )   \ 00427BF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    $69 story-flag? $6B story-flag? or if
        $904 self-anim
        self-wait-anim
        $69 story-flag? if
            9 0 $14 door-bits
            $69 story-flag-clear
            $905 self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            8 0 $14 door-bits
            $6B story-flag-clear
            $905 self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $B message
        wait-message
        self-frames-reset
        4 self-wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act0E ( -- )   \ 00427CC0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    $6C story-flag? $6D story-flag? or if
        $904 self-anim
        self-wait-anim
        $6C story-flag? if
            6 0 $14 door-bits
            $6C story-flag-clear
            $905 self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            7 0 $14 door-bits
            $6D story-flag-clear
            $905 self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $C message
        wait-message
        self-frames-reset
        4 self-wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room54.act0F ( -- )   \ 00427D88
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room54.act10 ( -- )   \ 00427D98
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room54.act11 ( -- )   \ 00427DA8
    self-wait-done
    90 self-turn-angle
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room54.act12 ( -- )   \ 00427DB8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room54.act13 ( -- )   \ 00427DC8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room54.act14 ( -- )   \ 00427DD8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: room54.act15 ( -- )   \ 00427DE8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: room54.act16 ( -- )   \ 00427DF8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: room54.act17 ( -- )   \ 00427E08
    self-wait-done
    90 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: room54.act18 ( -- )   \ 00427E18
    self-wait-done
    180 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: room54.act19 ( -- )   \ 00427E28
    self-wait-done
    0 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: room54.act1A ( -- )   \ 00427E40
    self-wait-done
    1 ebit? not if
        180 self-turn-angle
        self-wait-done
        8 message
        wait-message
        1 ebit-set
    else
        0.0 -80.0 self-turn-to-xz
        self-wait-done
        $A00 self-anim
        self-frames-reset
        self-wait-16
        9 message
        wait-message
        1 ebit-clear
        self-wait-anim
    then
    self-idle-or-end
;

: room54.act1B ( -- )   \ 00427E70
    self-wait-done
    36.86 -27.91 self-turn-to-xz
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
            $273 story-flag-set
            0 effect-remove
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

: room54.act1C ( -- )   \ 00427ED0
    self-wait-done
    39.16 -38.76 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $97 message-param-room
        $97 $63 item-count? if
            $8010 message
            wait-message
        else
            $2A8 story-flag-set
            1 effect-remove
            $97 1 item-give-count
            0 $97 item-tab
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

: room54.act1D ( -- )   \ 00427F30
    self-wait-done
    -23.61 -61.37 self-turn-to-xz
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
            $2B2 story-flag-set
            2 effect-remove
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

' room54.enter $54 0 room-script!
' room54.char-enter $54 6 room-script!
' room54.phase1 $54 1 room-script!
' room54.phase2 $54 2 room-script!
' room54.act00 $54 $00 action-script!
' room54.act01 $54 $01 action-script!
' room54.act02 $54 $02 action-script!
' room54.act03 $54 $03 action-script!
' room54.act04 $54 $04 action-script!
' room54.act05 $54 $05 action-script!
' room54.act06 $54 $06 action-script!
' room54.act07 $54 $07 action-script!
' room54.act08 $54 $08 action-script!
' room54.act09 $54 $09 action-script!
' room54.act0A $54 $0A action-script!
' room54.act0B $54 $0B action-script!
' room54.act0C $54 $0C action-script!
' room54.act0D $54 $0D action-script!
' room54.act0E $54 $0E action-script!
' room54.act0F $54 $0F action-script!
' room54.act10 $54 $10 action-script!
' room54.act11 $54 $11 action-script!
' room54.act12 $54 $12 action-script!
' room54.act13 $54 $13 action-script!
' room54.act14 $54 $14 action-script!
' room54.act15 $54 $15 action-script!
' room54.act16 $54 $16 action-script!
' room54.act17 $54 $17 action-script!
' room54.act18 $54 $18 action-script!
' room54.act19 $54 $19 action-script!
' room54.act1A $54 $1A action-script!
' room54.act1B $54 $1B action-script!
' room54.act1C $54 $1C action-script!
' room54.act1D $54 $1D action-script!

\ ---- room $55 ----------------------------------------------------------------------------------

\ Room55_Cmd00
: room55.cmd00 ( b0 -- )  drop s" room55.cmd00" stub-step ;

: room55.enter ( -- )   \ 00420B40
    room-sounds
    $11 1.0 0 bgm
    $54 story-flag? not if
        0 1 $14 door-bits
        0 room55.cmd00
    else
        1 1 $14 door-bits
        1 room55.cmd00
    then
    $272 story-flag? not if
        0 65.56 94.52 19.77 flicker-sprite
    then
    1 $3FFF sound-volume
;

: room55.char-enter ( -- )   \ 00420B80
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
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
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    4 1 1 area-camera
    0 self-is? if
        0 exit-taken? 2 exit-taken? or if
            3 map-page
        then
        1 exit-taken? 3 exit-taken? or if
            2 map-page
        then
    then
;

: room55.phase1 ( -- )   \ 00420CD0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $46 story-flag? not 1 ebit? not and if
        0 2 char-in-area? if
            1 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
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
    0 5 char-entered-area? if
        $4A story-flag? not $71 story-flag? and if
            $FE 0 char-file-load
        then
        0 exit-prepare
    then
    0 6 char-entered-area? if
        $4A story-flag? not $71 story-flag? and if
        then
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    0 8 char-entered-area? if
        3 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 $A char-entered-area? if
        4 exit-prepare
    then
    0 6 char-entered-area? if
        2 map-page
    then
    0 6 char-left-area? if
        3 map-page
    then
    0 8 char-entered-area? if
        2 map-page
    then
    0 8 char-left-area? if
        3 map-page
    then
    0 9 char-entered-area? if
        2 map-page
    then
    0 9 char-left-area? if
        1 map-page
    then
;

: room55.phase2 ( -- )   \ 00420D80
    $4A story-flag? not if
        0 1 char-group-bit4? if
            5 5 1 scene-change
        then
    then
    -2147483646 scene-request? if
        0 4 char-group-bit4? if
            5 1 1 scene-change
        then
    then
    0 $B char-in-area? 0 83 $3C char-heading? and if
        5 4 0 scene-change
    then
    $272 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
;

: room55.act00 ( -- )   \ 00420DD0
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    2 exit-check
    self-idle-or-end
;

: room55.act01 ( -- )   \ 00420DF0
    self-wait-done
    0 ebit? not 2 game-mode? or $FF panic-stage? or if
        4 self-through-exit
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
            0 message
            wait-message
        then
        0 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $18C -64.3 0.0 -89 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        self-frames-reset
        4 self-wait-frames
        1 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $18C -64.3 1.25 -89 char-to-xz
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room55.act02 ( -- )   \ 00420E80
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    4 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 0 6 char-sound
    $11 message-param-room
    $11 item-use
    $63 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room55.act03 ( -- )   \ 00420EC0
    self-wait-done
    65.56 19.77 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $272 story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: room55.act04 ( -- )   \ 00420F20
    self-wait-done
    $19F -6.04 30.79 166 $FFFF 5 self-move-to
    self-wait-done
    1 self-scripted
    1 self-noclip
    yield
    $17 state-flag-set
    0 $19E -4.65 24.47 166 char-to-xz
    1 35.0 55.0 0.0 0.0 event-camera
    2 message
    wait-message
    self-frames-reset
    self-wait-16
    0 self-scripted
    0 self-noclip
    yield
    $17 state-flag-clear
    0 $19F -6.04 30.79 166 char-to-xz
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room55.act05 ( -- )   \ 00420F90
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    2 1 movie-play
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
    1 action-end
    1 char-done
    0 creatures-clear
    $5A $FF movie-param
    0 0 $14 door-bits
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
    $62 door-unlock
    1 exit-check
    self-idle-or-end
;

: room55.act06 ( -- )   \ 00421040
    self-wait-done
    2 1 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $5A $FF movie-param
    0 0 $14 door-bits
    0 room55.cmd00
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

' room55.enter $55 0 room-script!
' room55.char-enter $55 6 room-script!
' room55.phase1 $55 1 room-script!
' room55.phase2 $55 2 room-script!
' room55.act00 $55 $00 action-script!
' room55.act01 $55 $01 action-script!
' room55.act02 $55 $02 action-script!
' room55.act03 $55 $03 action-script!
' room55.act04 $55 $04 action-script!
' room55.act05 $55 $05 action-script!
' room55.act06 $55 $06 action-script!

\ ---- room $56 ----------------------------------------------------------------------------------

\ room 0x56 (Room56_Cmd00_ptmf): creatures 7..9 in the current room on a live triangle
\ gRoomEventObj says yes to: +0x10, then the creature list's +0x28
: room56.cmd00 ( -- )  s" room56.cmd00" stub-step ;

: room56.enter ( -- )   \ 0040ED10
    room-sounds
    $322 story-flag? not if
        0 1 $14 door-bits
        1 0 $1000000 nav-group
    else
        1 0 $10020000 nav-group
    then
    1 char-here? 1 1 char-in-nav-group? and if
        1 $A8 char-to-tri
    then
    $84 story-flag? not if
        1 char-here? 119 hewie-action? and if
            1 $96 -78.0 0.0 90 char-to-xz
            18000 2 hewie-anim
            1 1 $B0 nav-group
        else
            1 1 $B8 nav-group
        then
    else
        1 1 $14 door-bits
        1 1 $88 nav-group
    then
;

: room56.char-enter ( -- )   \ 0040ED80
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

: room56.phase1 ( -- )   \ 0040EDC0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    $322 story-flag? not if
        0 35.0 0.0 0.0 8 4 0 zone
        0 0 2 char-zone-bits? 0 0 2 char-zone-bits-before? not and if
            0 0 6 char-sound
        then
    then
    $82 story-flag? $322 story-flag? not and if
        0 35.0 0.0 0.0 8 4 0 zone
        $FE 0 2 char-zone-bits? $FE 0 2 char-zone-bits-before? not and if
            $322 story-flag-set
            0 $FE 2 action
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 4 action-force
            else
                1 0 4 action-force
            then
        then
    then
    $84 story-flag? not if
        1 char-here? if
            118 hewie-action? not if
                35 fiona-started? 1 2 char-C4? not and $FE char-here? not and 1 camera-mode? and 0 char-unseen? not and 1 char-unseen? not and 0 -50 0 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 5 action
                    then
                then
            else 44 fiona-started? if
                0 1 6 action
            then then
        then
    then
    1 -42.06 0.0 -0.81 $1A 5 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $32 chance? if
                        $1F 1 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    2 32.38 0.0 -0.63 $16 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
    3 31.11 0.0 48.2 $1E 15 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: room56.phase2 ( -- )   \ 0040EF30
    0 4 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 5 char-in-area? 0 -45 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 6 char-in-area? 0 90 $32 char-heading? and if
        5 8 0 scene-change
    then
    $322 story-flag? if
        0 3 $32 char-faces-area? if
            5 3 0 scene-change
        then
    else
        0 35.0 0.0 0.0 8 4 0 zone
        0 0 2 char-zone-bits? if
            5 $D 0 scene-change
        then
    then
    0 7 $3C char-faces-area? if
        5 $B 0 scene-change
    then
    0 8 char-in-area? 0 5 37 $32 char-faces-xz? and if
        5 $C 0 scene-change
    then
;

: room56.phase3 ( -- )   \ 0040EFB0
    45.0 -4.0 -20.0 65.0 -4.0 -20.0 45.0 -4.0 20.0 65.0 -4.0 20.0 lights-doorway
;

: room56.act00 ( -- )   \ 0040EFF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A0 29.5 34.5 0 $FFFF 5 self-move-to
    self-wait-done
    2 ebit? not if
        $F 6 fade
        wait-fade
        3 message
        wait-message
        $F 7 fade
        wait-fade
        2 ebit-set
    else
        $1D01 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        2 ebit-clear
    then
    $241 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act01 ( -- )   \ 0040F040
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    $41 room-preload
    wait-fade
    things-clear
    $FFFF message-close
    0 1 6 char-sound
    8 state-flag-set
    self-frames-reset
    $3C self-wait-frames
    $80 exit-check
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act02 ( -- )   \ 0047ABB8
    self-wait-done
    begin
        yield
    again
;

: room56.act03 ( -- )   \ 0040F070
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    2 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room56.act04 ( -- )   \ 0040F080
    $18 state-flag-set
    1 self-scripted
    $FE 0 6 char-sound
    $F $44 fade
    self-wait-done
    wait-fade
    things-clear
    $FFFF message-close
    room56.cmd00
    $FE 1 6 char-sound
    $FE action-end
    3 summon-take
    self-frames-reset
    $3C self-wait-frames
    0 0 char-in-nav-group? if
        0 $68 160 char-to-tri-facing
    then
    0 0 $14 door-bits
    1 0 $10020000 nav-group
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    camera-restart
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act05 ( -- )   \ 0040F0E0
    self-wait-done
    hewie-bark
    self-wait-done
    $A8 -30.0 0.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 1 8 nav-group
    $96 -78.0 0.0 -90 $204 5 self-move-to
    self-wait-done
    18000 2 hewie-anim
    self-idle-or-end
;

: room56.act06 ( -- )   \ 0040F120
    1 self-scripted
    self-wait-done
    $A8 -30.0 0.0 90 $204 5 self-move-to
    self-wait-done
    -1 self-move-16
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-action
    1 1 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: room56.act07 ( -- )   \ 0040F150
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -50.0 0.0 self-turn-to-xz
    self-wait-done
    $84 story-flag? not if
        0 ebit? not if
            6 message
            wait-message
            0 ebit-set
        else
            7 message
            wait-message
        then
    else
        $D message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act08 ( -- )   \ 0040F190
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $A message
    wait-message
    $F 6 fade
    wait-fade
    $B message
    wait-message
    $F 7 fade
    wait-fade
    $C message
    wait-message
    $240 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act09 ( -- )   \ 0040F1C0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A0 29.5 34.5 0 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    1 message
    wait-message
    $902 self-anim
    self-wait-anim
    $10 item-use
    8 message-param-room
    8 1 item-give-count
    0 8 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act0A ( -- )   \ 0040F230
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A0 29.5 34.5 0 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    0 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then then
    $903 self-anim
    self-wait-anim
    0 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act0B ( -- )   \ 0040F370
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -3.17 12.09 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        3 ebit-set
        $E message
        wait-message
    else
        $F message
        wait-message
    then
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act0C ( -- )   \ 0040F3A0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    5.3 37.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $2C5 story-flag? not if
        $10 message
        wait-message
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C5 story-flag-set
            $71 1 item-give-count
            0 $71 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    else
        $11 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room56.act0D ( -- )   \ 0040F410
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $12 message
    wait-message
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

' room56.enter $56 0 room-script!
' room56.char-enter $56 6 room-script!
' room56.phase1 $56 1 room-script!
' room56.phase2 $56 2 room-script!
' room56.phase3 $56 3 room-script!
' room56.act00 $56 $00 action-script!
' room56.act01 $56 $01 action-script!
' room56.act02 $56 $02 action-script!
' room56.act03 $56 $03 action-script!
' room56.act04 $56 $04 action-script!
' room56.act05 $56 $05 action-script!
' room56.act06 $56 $06 action-script!
' room56.act07 $56 $07 action-script!
' room56.act08 $56 $08 action-script!
' room56.act09 $56 $09 action-script!
' room56.act0A $56 $0A action-script!
' room56.act0B $56 $0B action-script!
' room56.act0C $56 $0C action-script!
' room56.act0D $56 $0D action-script!

\ ---- room $57 ----------------------------------------------------------------------------------

: room57.enter ( -- )   \ 0040F490
    room-sounds
    1 0 $300000 nav-group
    $84 story-flag? if
        0 1 $14 door-bits
        2 1 object-show
    then
    $27C story-flag? not if
        0 5.64 9.5 5.58 flicker-sprite
    then
;

: room57.char-enter ( -- )   \ 0040F4C0
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
;

: room57.phase1 ( -- )   \ 0040F540
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    5 0 0 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
        3 0 char-remove
    then
    0 4 char-entered-area? if
        1 exit-prepare
        $1C 3 1 char-load
    then
    0 -44.47 0.0 54.87 $1E 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: room57.phase2 ( -- )   \ 0040F5B0
    0 $D char-in-area? 0 -15 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 $F char-in-area? 0 20 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 $10 char-in-area? 0 45 $3C char-heading? and if
        5 9 0 scene-change
    then
    $27C story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    0 $11 char-in-area? 0 90 $32 char-heading? and if
        5 1 0 scene-change
    then
;

: room57.act00 ( -- )   \ 0040F600
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message
    wait-message
    $902 self-anim
    self-wait-anim
    9 item-use
    $A message-param-room
    $A 1 item-give-count
    0 $A item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room57.act01 ( -- )   \ 0040F670
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    9 message
    wait-message
    $F 6 fade
    wait-fade
    $A message
    wait-message
    $F 7 fade
    wait-fade
    $48 subscreen-bit? not $49 subscreen-bit? not or if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $48 subscreen-bit
        $49 subscreen-bit
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room57.act02 ( -- )   \ 0040F6B0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        3 message
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $243 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room57.act03 ( -- )   \ 0040F700
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    5 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room57.act04 ( -- )   \ 0047ABC4
    self-idle-or-end
;

: room57.act05 ( -- )   \ 0047ABC8
    self-idle-or-end
;

: room57.act06 ( -- )   \ 0047ABCC
    self-idle-or-end
;

: room57.act07 ( -- )   \ 0040F820
    self-wait-done
    5.64 5.58 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $27C story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: room57.act08 ( -- )   \ 0040F880
    self-wait-done
    0.6 5.2 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    7 message
    wait-message
    self-idle-or-end
;

: room57.act09 ( -- )   \ 0040F8A0
    self-wait-done
    46.8 0.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    8 message
    wait-message
    self-idle-or-end
;

: room57.phase5 ( -- )   \ 0047ABC0
;

' room57.enter $57 0 room-script!
' room57.char-enter $57 6 room-script!
' room57.phase1 $57 1 room-script!
' room57.phase2 $57 2 room-script!
' room57.act00 $57 $00 action-script!
' room57.act01 $57 $01 action-script!
' room57.act02 $57 $02 action-script!
' room57.act03 $57 $03 action-script!
' room57.act04 $57 $04 action-script!
' room57.act05 $57 $05 action-script!
' room57.act06 $57 $06 action-script!
' room57.act07 $57 $07 action-script!
' room57.act08 $57 $08 action-script!
' room57.act09 $57 $09 action-script!
' room57.phase5 $57 5 room-script!

\ ---- room $58 ----------------------------------------------------------------------------------

: room58.enter ( -- )   \ 0040F910
    room-sounds
    3 0 var-set
    4 0 var-set
    $261 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $262 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $28D story-flag? not if
            0 10.5 1.0 -28.0 flicker-sprite
        then
    then
    0 3 0.0 0.5 0.312 0.187 zone-rect
    $1C $17F char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
    then
    0 $F1 0 action
    $22 state-flag-set
    $56 story-flag? if
        $78 story-flag? not if
            $78 story-flag-set
            $58 0 291 $81 3 -1 0 0.0 creature-place
            $58 0 426 $81 3 -1 0 0.0 creature-place
            $58 0 205 $81 3 -1 0 0.0 creature-place
        then
    then
    $2C1 story-flag? $2C2 story-flag? not and if
        1 7.38 1.0 20.58 flicker-sprite
    then
    1 $2300 sound-volume
;

: room58.act03 ( -- )   \ 0040FEC0
    3 ebit-set
    2 ebit-clear
    $15 0 pvar? if
        $A chance? if
            2 ebit-set
        then
    else $15 1 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $15 2 pvar? if
        $32 chance? if
            2 ebit-set
        then
    else $15 3 pvar? if
        $4B chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 $FE 4 action
    else
        $78 1 item-cooldown
    then
    $15 pvar-inc
    exit
;

: room58.char-enter ( -- )   \ 0040FA20
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
        0 self-is? 6 exit-taken? and if
            0 6 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 6 exit-taken? and if
        1 6 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    6 1 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            3 ebit-set
            room58.act03
        else
            3 ebit-clear
        then
    then
;

: room58.phase1 ( -- )   \ 0040FAB0
    0 exit-usable? if
        0 exit-check
    then
    6 exit-usable? if
        6 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        6 exit-prepare
    then
    $261 story-flag? not if
        0 -10.3 0.0 -93.8 5 8 1 zone
        $FF 0 char-in-zone? if
            $261 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -10.3 0.0 -93.8 0 -2145378256 0 0.0 scene-effect-8C
            $88 5 -10.3 0.0 -93.8 0 0 sound
            $40 $E1 noise
        then
    then
    2 -3.0 0.0 43.0 5 10 1 zone
    $FF 2 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 1 var? if
        $1C 2 6 char-sound
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    4 ebit-set
                    $50 chance? if
                        $1F 3 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    $2C1 story-flag? not if
        4 7.38 0.0 20.58 $A 5 0 zone
        1 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 705 var-set
                $1A 706 var-set
                $1B 1 var-set
                $1C 7380 var-set
                $1D 1000 var-set
                $1E 20580 var-set
                $1F 4 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        4 var-inc
        4 30 var? if
            4 0 var-set
            3 0 var? if
            else 3 1 var? if
            else 3 2 var? if
            else 3 3 var? if
            then then then then
            3 var-inc
            3 4 var? if
                3 0 var-set
            then
        then
    then
;

: room58.phase2 ( -- )   \ 0040FC70
    0 6 char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $262 story-flag? not if
        1 10.5 0.0 -28.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $262 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            10.5 0.0 -28.0 0 -2145378256 0 0.0 scene-effect-8C
            $88 5 10.5 0.0 -28.0 0 0 sound
            $40 $1AD noise
            0 10.5 1.0 -28.0 flicker-sprite
        then
    else $28D story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
    $2C1 story-flag? $2C2 story-flag? not and if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room58.act00 ( -- )   \ 0040FD30
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            1 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9005 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9004 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9004 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: room58.act02 ( -- )   \ 0040FE90
    1 ebit? if
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
    1 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room58.act01 ( -- )   \ 0040FDF0
    $18 state-flag-set
    3 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $106 $8004 5 -4.6 -231.1 180 self-walk-anim
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
    1 ebit-clear
    0 avoid-prompt
    $FF 2 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 1 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room58.act02 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? $FE char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room58.act02 goto
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

: room58.act04 ( -- )   \ 0040FF10
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 6 char-heading-for? 6 exit-door-open? not and if
        6 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $106 -4.6 -222.0 180 $FFFF 5 self-move-to
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
    1 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room58.act05 ( -- )   \ 0040FF70
    self-wait-done
    10.5 -28.0 self-turn-to-xz
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
            $28D story-flag-set
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

: room58.act06 ( -- )   \ 0040FFD0
    self-wait-done
    7.38 20.58 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C2 story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: room58.phase5 ( -- )   \ 0047ABD0
    $1C action-end
    $1C char-done
;

' room58.enter $58 0 room-script!
' room58.char-enter $58 6 room-script!
' room58.phase1 $58 1 room-script!
' room58.phase2 $58 2 room-script!
' room58.act00 $58 $00 action-script!
' room58.act01 $58 $01 action-script!
' room58.act02 $58 $02 action-script!
' room58.act03 $58 $03 action-script!
' room58.act04 $58 $04 action-script!
' room58.act05 $58 $05 action-script!
' room58.act06 $58 $06 action-script!
' room58.phase5 $58 5 room-script!

\ ---- room $59 ----------------------------------------------------------------------------------

\ room 0x59 (Room59_Cmd00_ptmf): character 0xFE's model +0x9E0 = 0.1 (byte 3 0) or 0
: room59.cmd00 ( b0 -- )  drop s" room59.cmd00" stub-step ;

: room59.enter ( -- )   \ 00410070
    room-sounds
    $324 story-flag? if
        8 1 object-show
    then
    $323 story-flag? if
        9 1 object-show
    then
    0 0 $14 door-bits
    $88 story-flag? $89 story-flag? not and if
        $80 exit-taken? if
            4 ebit-set
        then
    then
    4 ebit? not if
        $276 story-flag? not if
            0 4.58 9.0 14.22 flicker-sprite
        then
    then
;

: room59.char-enter ( -- )   \ 004100B0
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
    0 self-is? if
        4 ebit? if
            0 $4B 0 char-to-tri-facing
            8 state-flag-set
            0.0 sound-volume-scale
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 6 action-force
        then
    then
;

: room59.phase1 ( -- )   \ 00410110
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
;

: room59.phase2 ( -- )   \ 00410140
    $324 story-flag? not if
        0 7 char-in-area? 0 45 $32 char-heading? and if
            3 stalker-kind? $22 stalker-kind? or 4 stalker-kind? or 2 game-mode? and $FF panic-stage? not and $FE 2 char-C4? not and if
                $FE char-here? if
                    $8016 scene-ending
                else
                    5 1 6 scene-change
                then
            else
                5 3 0 scene-change
            then
        then
    then
    $323 story-flag? not if
        0 8 char-in-area? 0 -45 $32 char-heading? and if
            3 stalker-kind? $22 stalker-kind? or 4 stalker-kind? or 2 game-mode? and $FF panic-stage? not and $FE 2 char-C4? not and if
                $FE char-here? if
                    $8016 scene-ending
                else
                    5 2 6 scene-change
                then
            else
                5 4 0 scene-change
            then
        then
    then
    0 9 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $A char-in-area? 0 -15 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 $B char-in-area? 0 -16 -17 $3C char-faces-xz? and if
        5 7 0 scene-change
    then
    0 $C char-in-area? 0 -35 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 $E char-in-area? 0 0 $3C char-heading? and if
        5 9 0 scene-change
    then
    0 $D char-in-area? 0 10 $3C char-heading? and if
        5 $A 0 scene-change
    then
    $276 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    0 $F $3C char-faces-area? if
        5 $B 0 scene-change
    then
;

: room59.act00 ( -- )   \ 00410230
    2 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 $A char-file-load
        $7A 30.0 40.5 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        7 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        0 $84 5 char-sound
        3 avoid-prompt
        2 ebit-set
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        8 message
        wait-message
    then
    self-idle-or-end
;

: room59.act01 ( -- )   \ 004102F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    3 stalker-kind? $22 stalker-kind? or if
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
        3 4 0 char-model-op
    else
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
        3 5 0 char-model-op
    then
    wait-fade
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    $10 $FF movie-param
    3 stalker-kind? $22 stalker-kind? or if
        0 $F9 $D action
    else
        0 $F9 $C action
    then
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
    3 stalker-kind? $22 stalker-kind? or if
        1 room59.cmd00
    then
    wait-fade
    8 1 object-show
    0 self-move-16
    3 stalker-kind? $22 stalker-kind? or if
        0 $4A -7.575 18.619 90 char-to-xz
        $FE $59 16 2 stalker-to-room
        $FE 2 2 char-camera
        $FE $10 -90 char-to-tri-facing
        $FE 0 stalker-mode
        $35 resident-flag-set
    else
        0 $4A -7.719 18.622 90 char-to-xz
        $FE $59 130 2 stalker-to-room
        $FE 2 2 char-camera
        $FE $82 -90 char-to-tri-facing
        $FE 0 stalker-mode
        $37 resident-flag-set
    then
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $59 0 3 hewie-to-room
            1 3 -7.85 29.94 52 char-to-xz
        then
    then
    $324 story-flag-set
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room59.act02 ( -- )   \ 00410470
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    3 stalker-kind? $22 stalker-kind? or if
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
        $10 $FF movie-param
        3 4 0 char-model-op
    else
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
        $10 $FF movie-param
        3 5 0 char-model-op
    then
    wait-fade
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    7 state-flag-set
    yield
    7 state-flag-clear
    0 $F9 $C action
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
    9 1 object-show
    0 self-move-16
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $59 0 56 hewie-to-room
            1 $38 -4.97 35.12 -145 char-to-xz
        then
    then
    $323 story-flag-set
    3 stalker-kind? $22 stalker-kind? or if
        0 $4A 180 char-to-tri-facing
        $FE $59 16 2 stalker-to-room
        $FE $10 23.591 11.878 -30 char-to-xz
        $FE char-activate
        $FE stalker-knock-down
        $34 resident-flag-set
        $80 door-open-clear
        $80 exit-check
    else
        0 $4A -6.968 18.812 -130 char-to-xz
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
        camera-restart
        $FE action-end
        3 summon-take
        $F $41 fade
        wait-fade
        $36 resident-flag-set
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room59.act03 ( -- )   \ 004105E0
    self-wait-done
    32.0 -3.5 self-turn-to-xz
    self-wait-done
    0 ebit? not if
        3 message
        wait-message
        0 ebit-set
    else
        5 message
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: room59.act04 ( -- )   \ 00410600
    self-wait-done
    -29.0 -1.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        4 message
        wait-message
        1 ebit-set
    else
        6 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: room59.act05 ( -- )   \ 00410620
    self-wait-done
    4.58 14.22 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $276 story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: room59.act06 ( -- )   \ 00410680
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    self-wait-done
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 0 $14 door-bits
    3 0 char-remove
    $C0 room-preload
    8 state-flag-set
    $3A resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: room59.act07 ( -- )   \ 00410720
    self-wait-done
    0 $A char-in-area? if
        -30 self-turn-angle
        self-wait-done
        $A02 self-anim
    else
        -16.0 -17.0 self-turn-to-xz
        self-wait-done
        $A01 self-anim
    then
    self-frames-reset
    $28 self-wait-frames
    5 ebit? not if
        5 ebit-set
        9 message
        wait-message
    else
        $E message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room59.act08 ( -- )   \ 00410750
    self-wait-done
    -70 self-turn-angle
    self-wait-done
    6 ebit? not if
        6 ebit-set
        $A message
        wait-message
    else
        $60A self-anim
        self-frames-reset
        $B self-wait-frames
        0 $28 5 char-sound
        self-wait-anim
        $B message
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        wait-message
    then
    self-idle-or-end
;

: room59.act09 ( -- )   \ 00410780
    self-wait-done
    2.0 45.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $C message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room59.act0A ( -- )   \ 004107A0
    self-wait-done
    14.2 44.7 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    $D message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room59.act0B ( -- )   \ 004107C0
    self-wait-done
    6.5 14.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $F message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room59.act0C ( -- )   \ 004107D8
    0 exit-door-open? not if
        self-frames-reset
        $F self-wait-frames
        0 1 self-door-knock
    then
    self-idle-or-end
;

: room59.act0D ( -- )   \ 004107F0
    0 exit-door-open? not if
        self-frames-reset
        $F self-wait-frames
        0 1 self-door-knock
    then
    begin
        6 cutscene-shot? not while
        yield
    repeat
    0 room59.cmd00
    self-idle-or-end
;

: room59.act0E ( -- )   \ 00410810
    self-wait-done
    0 0 $14 door-bits
    3 partner-load
    2 char-unload
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
    $10 $FF movie-param
    3 4 0 char-model-op
    0 $F9 $C action
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
    9 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room59.act0F ( -- )   \ 004108B0
    self-wait-done
    0 0 $14 door-bits
    3 partner-load
    2 char-unload
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
    3 4 0 char-model-op
    $10 $FF movie-param
    0 $F9 $D action
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
    1 room59.cmd00
    8 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room59.act10 ( -- )   \ 00410950
    self-wait-done
    0 0 $14 door-bits
    4 partner-load
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
    $10 $FF movie-param
    3 5 0 char-model-op
    0 $F9 $C action
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
    9 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room59.act11 ( -- )   \ 004109F0
    self-wait-done
    0 0 $14 door-bits
    4 partner-load
    2 char-unload
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
    3 5 0 char-model-op
    $10 $FF movie-param
    0 $F9 $C action
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
    8 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room59.act12 ( -- )   \ 00410A90
    self-wait-done
    0 0 $14 door-bits
    $17 3 $FF char-load
    3 char-unload
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 0 $14 door-bits
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room59.enter $59 0 room-script!
' room59.char-enter $59 6 room-script!
' room59.phase1 $59 1 room-script!
' room59.phase2 $59 2 room-script!
' room59.act00 $59 $00 action-script!
' room59.act01 $59 $01 action-script!
' room59.act02 $59 $02 action-script!
' room59.act03 $59 $03 action-script!
' room59.act04 $59 $04 action-script!
' room59.act05 $59 $05 action-script!
' room59.act06 $59 $06 action-script!
' room59.act07 $59 $07 action-script!
' room59.act08 $59 $08 action-script!
' room59.act09 $59 $09 action-script!
' room59.act0A $59 $0A action-script!
' room59.act0B $59 $0B action-script!
' room59.act0C $59 $0C action-script!
' room59.act0D $59 $0D action-script!
' room59.act0E $59 $0E action-script!
' room59.act0F $59 $0F action-script!
' room59.act10 $59 $10 action-script!
' room59.act11 $59 $11 action-script!
' room59.act12 $59 $12 action-script!

\ ---- room $5B ----------------------------------------------------------------------------------

: room5B.enter ( -- )   \ 0041B020
    $17 1.0 0 bgm
    0 -12.3 17.6 125.0 1 effect-86
    1 -12.3 27.6 70.0 1 effect-86
    2 -12.3 27.6 10.0 1 effect-86
    3 -12.3 27.6 -50.0 1 effect-86
    4 -12.3 17.7 -106.0 1 effect-86
;

: room5B.char-enter ( -- )   \ 0041B080
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
;

: room5B.phase1 ( -- )   \ 0041B100
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
;

' room5B.enter $5B 0 room-script!
' room5B.char-enter $5B 6 room-script!
' room5B.phase1 $5B 1 room-script!

\ ---- room $5C ----------------------------------------------------------------------------------

\ room 0x5C (Room5C_Cmd00_ptmf): the dial (+0x7C, 0..1) from script variable 0 by byte 3: 0
\ 0x2B..0x38 (/ 13, +0x74 0 / +0x78 1), 1 0xC..0x20 (/ 20), 2 7..0x12 (/ 11) (+0x74 1 / +0x78 0)
: room5C.cmd00 ( b0 -- )  drop s" room5C.cmd00" stub-step ;
\ room 0x5C (Room5C_Cond00_ptmf): the first creature within 3 of (59.1, 1.43) is put away with
\ an effect (CreatureVanish_vtable) above it - blue (+0x1571 below 0x12) or red - and its action
\ 0x8B
: room5C.cond00? ( -- flag )  s" room5C.cond00?" stub-flag ;

: room5C.enter ( -- )   \ 00410F70
    room-sounds
    1 0 var-set
    2 0 var-set
    $5A door-not-closed-off? not if
        2 1 $14 door-bits
        1 1 8 nav-group
    then
    $4B story-flag? if
        3 1 $14 door-bits
    then
    2 48.8 10.3 -50.7 0 effect-86
    1 $2300 sound-volume
;

: room5C.act08 ( -- )   \ 00411890
    $FE camera-follow
    2 ebit-clear
    $16 0 pvar? if
        $A chance? if
            2 ebit-set
        then
    else $16 1 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $16 2 pvar? if
        $32 chance? if
            2 ebit-set
        then
    else $16 3 pvar? if
        $4B chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 $FE 9 action
    else
        $78 1 item-cooldown
    then
    $16 pvar-inc
    exit
;

: room5C.char-enter ( -- )   \ 00410FB0
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 -1 char-camera
                0 camera-follow
            else
                1 4 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then then
    2 4 -1 area-camera
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
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 5 3 char-camera
                0 camera-follow
            else
                1 5 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 5 3 char-camera
            0 camera-follow
        else
            1 5 3 char-camera
            1 camera-follow
        then
    then then
    4 5 3 area-camera
    $265 story-flag? not if
        0 48.0 8.0 -53.0 flicker-sprite
    then
    $266 story-flag? not if
        1 53.1 12.0 -26.2 flicker-sprite
    then
    0 self-is? if
        $4A story-flag? not if
            0.0 sound-volume-scale
            0 0 0 action
        then
        $80 exit-taken? if
            0 $B 55.0 1.0 90 char-to-xz
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
        $FE exit-taken? if
            0 $D7 5.5 -116.0 0 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 5 action
        then
    then
    $FE self-is? if
        9 state-flag? if
            3 ebit-set
            room5C.act08
        else
            3 ebit-clear
        then
    then
;

: room5C.phase1 ( -- )   \ 00411180
    6 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 5.0 18.0 -125.0 0 0 sound
            $54 story-flag? not if
                $40000009 6 -120.0 10.0 -105.0 0 0 sound
            then
            6 ebit-set
        then
    else
        $C0000008 6 5.0 18.0 -125.0 0 0 sound
        $54 story-flag? not if
            $C0000009 6 -120.0 10.0 -105.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $5A door-not-closed-off? if
        2 exit-usable? if
            2 exit-check
        then
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 0 0 1 chars-area-camera
    $11 3 -1 1 chars-area-camera
    $12 3 -1 1 chars-area-camera
    $13 4 -1 1 chars-area-camera
    $14 0 0 1 chars-area-camera
    $15 5 3 1 chars-area-camera
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    0 8 char-entered-area? 0 9 char-entered-area? or 0 $A char-entered-area? or if
        3 exit-prepare
    then
    0 $B char-entered-area? if
        4 exit-prepare
    then
    0 ebit? not if
        $4B story-flag? not if
            3 camera-mode? 4 camera-mode? or if
                room5C.cond00? if
                    0 ebit-set
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 1 action-force
                    else
                        1 0 1 action-force
                    then
                then
            then
        then
    then
    0 141.32 0.0 -60.6 $14 21 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 21.0 hewie-look-zone
            then
        then
    then
    1 59.37 0.0 0.93 $14 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    5 2 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000003 6 45.32 0.0 23.5 0 0 sound
            else 1 1 var? if
                $40000004 6 45.32 0.0 23.5 0 0 sound
            else 1 2 var? if
                $40000005 6 45.32 0.0 23.5 0 0 sound
            else 1 3 var? if
                $40000006 6 45.32 0.0 23.5 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: room5C.phase2 ( -- )   \ 00411430
    0 $16 $32 char-faces-area? if
        5 3 0 scene-change
    then
    0 $17 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $18 char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 6 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 $19 char-in-area? 0 -15 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $1A char-in-area? 0 45 $3C char-heading? and if
        5 $C 0 scene-change
    then
    0 $1B char-in-area? 0 -90 $3C char-heading? and if
        5 $D 0 scene-change
    then
    0 $1C char-in-area? 0 0 $3C char-heading? and if
        5 $E 0 scene-change
    then
    -2147483646 scene-request? 0 0 char-group-bit4? and if
        5 $F 1 scene-change
    then
    $265 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
    $266 story-flag? not if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then
;

: room5C.phase3 ( -- )   \ 004114E0
    40.0 20.0 -77.0 17.3 20.0 -77.0 40.0 0.0 -77.0 17.3 0.0 -77.0 lights-doorway
    17.3 20.0 -42.0 40.0 20.0 -42.0 17.3 0.0 -42.0 40.0 0.0 -42.0 lights-doorway
    17.0 20.0 -77.3 17.0 20.0 -100.0 17.0 0.0 -77.3 17.0 0.0 -100.0 lights-doorway
    22.3 20.0 -6.0 22.3 20.0 -30.0 22.3 0.0 -6.0 22.3 0.0 -30.0 lights-doorway
    20.7 20.0 -30.0 20.7 20.0 -6.0 20.7 0.0 -30.0 20.7 0.0 -6.0 lights-doorway
    22.3 20.0 30.0 22.3 20.0 6.0 22.3 0.0 30.0 22.3 0.0 6.0 lights-doorway
    20.7 20.0 6.0 20.7 20.0 25.0 20.7 0.0 6.0 20.7 0.0 25.0 lights-doorway
;

: room5C.act00 ( -- )   \ 00411640
    1 self-scripted
    self-wait-done
    1 char-activate
    $5C 0 302 hewie-to-room
    $62 door-open-clear
    doors-room-in
    $62 door-lock
    $16 state-flag-clear
    1 creatures-clear
    $4A story-flag-set
    camera-restart
    $F $51 fade
    wait-fade
    $234 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5C.act01 ( -- )   \ 00411670
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $5D room-preload
    self-frames-reset
    self-wait-16
    3 1 $14 door-bits
    self-frames-reset
    8 self-wait-frames
    $F $44 fade
    wait-fade
    8 state-flag-set
    $80 exit-check
    self-idle-or-end
;

: room5C.act02 ( -- )   \ 00411690
    1 self-scripted
    3 1 $14 door-bits
    self-wait-done
    camera-restart
    8 state-flag-clear
    $4B story-flag-set
    $54 door-unlock
    2 exit-prepare
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    0 state-flag-set
    self-idle-or-end
;

: room5C.act03 ( -- )   \ 004116C0
    self-wait-done
    60.0 0.0 self-turn-to-xz
    self-wait-done
    $4B story-flag? not if
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room5C.act04 ( -- )   \ 004116E0
    $18 state-flag-set
    1 self-scripted
    $27 room-preload
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    2 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        8 state-flag-set
        $FE exit-check
    else
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room5C.act05 ( -- )   \ 00411720
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5C.act07 ( -- )   \ 00411830
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 0 var-set
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        2 room5C.cmd00
        yield
        0 18 var? not while
        0 6 var? if
            0 0 6 char-sound
        then
        0 var-inc
    repeat
    self-wait-anim
    0 self-noclip
    1 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: room5C.act06 ( -- )   \ 00411740
    $18 state-flag-set
    3 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    $182 -67.8 -119.5 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    1 self-noclip
    0 0 var-set
    $8000 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        0 room5C.cmd00
        yield
        0 56 var? not while
        0 44 var? if
            0 0 6 char-sound
        then
        0 var-inc
    repeat
    self-wait-anim
    -1 self-move-16
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    1 ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 1 6 char-sound
    then
    begin
        0 2 pad? not if
            $FF panic-stage? 1 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room5C.act07 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? $FE char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room5C.act07 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            0 0 var-set
            $8001 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            begin
                1 room5C.cmd00
                yield
                0 32 var? not while
                0 13 var? if
                    0 0 6 char-sound
                then
                0 var-inc
            repeat
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room5C.act09 ( -- )   \ 004118E0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $FE 2 char-heading-for? 2 exit-door-open? not and if
        2 3 self-move-slot
        self-wait-done
    then
    $FE 3 char-heading-for? 3 exit-door-open? not and if
        3 3 self-move-slot
        self-wait-done
    then
    $FE 4 char-heading-for? 4 exit-door-open? not and if
        4 3 self-move-slot
        self-wait-done
    then
    $182 -67.8 -110.0 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    1 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room5C.act0A ( -- )   \ 00411950
    self-wait-done
    48.0 -53.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $265 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room5C.act0B ( -- )   \ 004119B0
    self-wait-done
    53.1 -26.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $266 story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: room5C.act0C ( -- )   \ 00411A10
    self-wait-done
    5 ebit? not if
        5 ebit-set
        138.0 -58.0 self-turn-to-xz
        self-wait-done
        $A01 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 message
        wait-message
    else
        $64 129.0 -60.5 90 $FFFF 5 self-move-to
        self-wait-done
        1 15.0 -10.0 0.0 7.0 event-camera
        $A00 self-anim
        self-frames-reset
        $28 self-wait-frames
        4 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room5C.act0D ( -- )   \ 00411A60
    self-wait-done
    78.0 -70.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    5 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room5C.act0E ( -- )   \ 00411A80
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    62.0 27.0 self-turn-to-xz
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    $43 subscreen-bit? not $45 subscreen-bit? not or if
        $1D01 self-anim
        $A message
        wait-message
        self-wait-anim
        $43 subscreen-bit
        $45 subscreen-bit
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5C.act0F ( -- )   \ 00411AD0
    self-wait-done
    0 self-through-exit
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60A self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $608 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        7 message
        wait-message
    then
    self-idle-or-end
;

' room5C.enter $5C 0 room-script!
' room5C.char-enter $5C 6 room-script!
' room5C.phase1 $5C 1 room-script!
' room5C.phase2 $5C 2 room-script!
' room5C.phase3 $5C 3 room-script!
' room5C.act00 $5C $00 action-script!
' room5C.act01 $5C $01 action-script!
' room5C.act02 $5C $02 action-script!
' room5C.act03 $5C $03 action-script!
' room5C.act04 $5C $04 action-script!
' room5C.act05 $5C $05 action-script!
' room5C.act06 $5C $06 action-script!
' room5C.act07 $5C $07 action-script!
' room5C.act08 $5C $08 action-script!
' room5C.act09 $5C $09 action-script!
' room5C.act0A $5C $0A action-script!
' room5C.act0B $5C $0B action-script!
' room5C.act0C $5C $0C action-script!
' room5C.act0D $5C $0D action-script!
' room5C.act0E $5C $0E action-script!
' room5C.act0F $5C $0F action-script!

\ ---- room $5D ----------------------------------------------------------------------------------

\ room 0x5D (Room5D_Cmd00_ptmf): four objects 60 to the left
: room5D.cmd00 ( -- )  s" room5D.cmd00" stub-step ;
\ room 0x5D (Room5D_Cmd01_ptmf): the lever at -60 / 0 / 60 degrees by byte 3
: room5D.cmd01 ( b0 -- )  drop s" room5D.cmd01" stub-step ;

: room5D.enter ( -- )   \ 00411B80
    room-sounds
    $5A door-not-closed-off? not if
        4 1 $14 door-bits
        1 3 8 nav-group
    then
    $4B story-flag? not if
        1 0 $20000 nav-group
    else
        room5D.cmd00
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

: room5D.char-enter ( -- )   \ 00411C60
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
            0 room5D.cmd01
        else 3 exit-taken? if
            1 room5D.cmd01
        else 1 exit-taken? if
            2 room5D.cmd01
        else 2 exit-taken? if
            2 room5D.cmd01
        then then then then
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
;

: room5D.phase1 ( -- )   \ 00411D60
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

: room5D.phase2 ( -- )   \ 00411FB0
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

: room5D.act00 ( -- )   \ 00412090
    1 self-scripted
    self-wait-done
    $17 state-flag-set
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
    $17 state-flag-clear
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room5D.act01 ( -- )   \ 00412120
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

: room5D.act02 ( -- )   \ 00412140
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

: room5D.act03 ( -- )   \ 004121A0
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

: room5D.act04 ( -- )   \ 00412200
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

: room5D.act05 ( -- )   \ 00412260
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
        $17 state-flag-set
        1 5.0 10.0 0.0 0.0 event-camera
        $339 story-flag-set
        2 message
        wait-message
    else
        $17 state-flag-set
        1 5.0 10.0 0.0 0.0 event-camera
        3 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    0 self-scripted
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room5D.act06 ( -- )   \ 004122F0
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room5D.enter $5D 0 room-script!
' room5D.char-enter $5D 6 room-script!
' room5D.phase1 $5D 1 room-script!
' room5D.phase2 $5D 2 room-script!
' room5D.act00 $5D $00 action-script!
' room5D.act01 $5D $01 action-script!
' room5D.act02 $5D $02 action-script!
' room5D.act03 $5D $03 action-script!
' room5D.act04 $5D $04 action-script!
' room5D.act05 $5D $05 action-script!
' room5D.act06 $5D $06 action-script!

\ ---- room $5E ----------------------------------------------------------------------------------

: room5E.enter ( -- )   \ 00412410
    $26B story-flag? not if
        0 42.0 63.5 6.6 flicker-sprite
    then
;

: room5E.char-enter ( -- )   \ 00412430
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
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
    1 self-is? if
        2 hewie-mode
    then
    $26B story-flag? not if
        0 42.0 63.5 6.6 flicker-sprite
    then
;

: room5E.phase1 ( -- )   \ 00412510
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 43.86 50.0 6.98 $15 16 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    0 43.86 50.0 6.98 $15 16 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 -5.14 50.0 13.21 $44 29 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 29.43 hewie-look-zone
            then
        then
    then
;

: room5E.phase2 ( -- )   \ 004125E0
    $26B story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 0 4 scene-change
        then
    then
    0 7 $3C char-faces-area? if
        5 1 0 scene-change
    then
;

: room5E.act00 ( -- )   \ 00412610
    self-wait-done
    42.0 6.6 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $65 message-param-room
        $65 $63 item-count? if
            $8010 message
            wait-message
        else
            $26B story-flag-set
            0 effect-remove
            $65 1 item-give-count
            0 $65 item-tab
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

: room5E.act01 ( -- )   \ 00412670
    self-wait-done
    50.0 9.0 self-turn-to-xz
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

' room5E.enter $5E 0 room-script!
' room5E.char-enter $5E 6 room-script!
' room5E.phase1 $5E 1 room-script!
' room5E.phase2 $5E 2 room-script!
' room5E.act00 $5E $00 action-script!
' room5E.act01 $5E $01 action-script!

\ ---- room $5F ----------------------------------------------------------------------------------

: room5F.enter ( -- )   \ 0041B130
    $233 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $274 story-flag? not if
            0 -10.5 1.0 -0.4 flicker-sprite
        then
    then
    0 2 0.812 0.687 0.187 0.312 zone-rect
    $50 story-flag? if
        $7A story-flag? not if
            $7A story-flag-set
            $5F 0 259 4 8 -1 0 0.0 creature-place
        then
    then
    1 12.4 17.3 -45.0 1 effect-86
    1 $2300 sound-volume
;

: room5F.char-enter ( -- )   \ 0041B1B0
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
;

: room5F.phase1 ( -- )   \ 0041B270
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        2 exit-prepare
    then
    1 5.8 0.0 15.45 $F 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    2 -65.87 0.0 11.35 $B 15 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    3 -66.99 0.0 -11.47 $B 15 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    6 1 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
;

: room5F.phase2 ( -- )   \ 0041B370
    $233 story-flag? not if
        0 -10.5 0.0 -0.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $233 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -10.5 0.0 -0.4 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -10.5 0.0 -0.4 0 0 sound
            $40 $7B noise
            0 -10.5 1.0 -0.4 flicker-sprite
        then
    else $274 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 0 4 scene-change
        then
    then then
    4 -67.5 0.0 -11.5 8 10 0 zone
    0 4 8 char-zone-bits? if
        0 -67 -11 $3C char-faces-xz? if
            5 1 0 scene-change
        then
    then
    5 -67.5 0.0 11.5 8 10 0 zone
    0 5 8 char-zone-bits? if
        0 -67 11 $3C char-faces-xz? if
            5 1 0 scene-change
        then
    then
;

: room5F.phase3 ( -- )   \ 0041B450
    -13.7 15.0 -28.0 -13.7 15.0 -11.3 -13.7 0.0 -28.0 -13.7 0.0 -11.3 lights-doorway
    -13.7 15.0 14.0 -13.7 15.0 30.7 -13.7 0.0 14.0 -13.7 0.0 30.7 lights-doorway
;

: room5F.act00 ( -- )   \ 0041B4C0
    self-wait-done
    -10.5 -0.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $9B message-param-room
        $9B $63 item-count? if
            $8010 message
            wait-message
        else
            $274 story-flag-set
            0 effect-remove
            $9B 1 item-give-count
            0 $9B item-tab
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

: room5F.act01 ( -- )   \ 0041B520
    self-wait-done
    0 4 8 char-zone-bits? if
        -67.5 -11.5 self-turn-to-xz
        self-wait-done
    else
        -67.5 11.5 self-turn-to-xz
        self-wait-done
    then
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 ebit-set
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

' room5F.enter $5F 0 room-script!
' room5F.char-enter $5F 6 room-script!
' room5F.phase1 $5F 1 room-script!
' room5F.phase2 $5F 2 room-script!
' room5F.phase3 $5F 3 room-script!
' room5F.act00 $5F $00 action-script!
' room5F.act01 $5F $01 action-script!

\ ---- room $60 ----------------------------------------------------------------------------------

\ room 0x60 (D_00429148): the floor light (room effect 0x1B, 20 x 20 at y -0.2) by byte 3 - 1
\ removed with its glow effect (event variable 1); 0 made, with the glow (FloorGlow_vtable);
\ then (and for other values) its strength from event variable 0 (0..4: 0, 30, 60, 90, 128),
\ also sent to the glow
: room60.cmd00 ( b0 -- )  drop s" room60.cmd00" stub-step ;
\ room 0x60 (as Room23_Cmd01, for character 0xFE): byte 3 0 a progress name, 1 wait for
\ character 0xFE (2 while not), else done
: room60.cmd01 ( b0 -- )  drop s" room60.cmd01" stub-step ;
\ (as Room66_Effect) byte 4 0 starts the 8-byte effect Room60Effect_vtable (parameters from byte
\ 3), its slot kept in event variable byte 3 + 2; else that effect is ended
: room60.cmd02 ( b0 b1 -- )  drop drop s" room60.cmd02" stub-step ;
\ room 0x60 (Room60_Cmd03_ptmf): character 0xFE's model +0x9FC 0.4 / +0xA00 1 (byte 3 0), or 0
: room60.cmd03 ( b0 -- )  drop s" room60.cmd03" stub-step ;
\ character 0xFE's model +0x9E0 / +0x9E4 / +0x9E8: byte 3 0 -0.2 / 0.2 / -0.2; 1 eases them by
\ script variable 6 (a step a call, waiting (2) for 60) to 0 / 0.3 / 0; else 0 / 0.3 / 0
: room60.cmd04 ( b0 -- )  drop s" room60.cmd04" stub-step ;
\ room 0x60 (Room60_Cmd05_ptmf): the player's model +0xCC 0 (byte 3 0) or 1
: room60.cmd05 ( b0 -- )  drop s" room60.cmd05" stub-step ;

: room60.enter ( -- )   \ 004280A0
    room-sounds
    9 0 $14 door-bits
    $A 0 $14 door-bits
    $56 story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
        0 0 0 $217 $4E2 obstacle-place
        1 1 0 $12D $3F3 obstacle-place
        2 2 0 $249 $519 obstacle-place
        3 3 0 $272 $544 obstacle-place
        0 ebit-clear
        1 ebit-clear
        2 ebit-clear
        3 ebit-clear
        7 0 $14 door-bits
        0 0 var-set
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
        0 0 0 $20A $4D8 obstacle-place
        1 1 0 $153 $415 obstacle-place
        2 2 0 $15C $41E obstacle-place
        3 3 0 $213 $4DE obstacle-place
        0 obstacle-stop
        1 obstacle-stop
        2 obstacle-stop
        3 obstacle-stop
        0 ebit-set
        1 ebit-set
        2 ebit-set
        3 ebit-set
        7 1 $14 door-bits
        0 0 var-set
        1 0 8 nav-group
        $338 story-flag? not if
            9 1 $14 door-bits
        else
            $A 1 $14 door-bits
        then
        $7F story-flag? not if
            $7F story-flag? not if
                0 1.29 1.0 -17.02 flicker-sprite
            then
        then
    then
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    2 0 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
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
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 room60.cmd00
    1 $2300 sound-volume
;

: room60.char-enter ( -- )   \ 004281E0
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

: room60.phase1 ( -- )   \ 00428220
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    6 sound-bank-loaded? if
        $C0000002 6 0.0 0.0 0.0 0 0 sound
    then
    0 ebit? not if
        0 $20A obstacle-on? if
            0 obstacle-stop
            0 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            0 0 room60.cmd02
        then
    then
    1 ebit? not if
        1 $153 obstacle-on? if
            1 obstacle-stop
            1 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            1 0 room60.cmd02
        then
    then
    2 ebit? not if
        2 $15C obstacle-on? if
            2 obstacle-stop
            2 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            2 0 room60.cmd02
        then
    then
    3 ebit? not if
        3 $213 obstacle-on? if
            3 obstacle-stop
            3 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            3 0 room60.cmd02
        then
    then
    $55 story-flag? not 0 4 char-entered-area? and 7 ebit? not and if
        7 ebit-set
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 char-action? if
            0 0 0 action-force
        else
            1 0 0 action-force
        then
    then
    $55 story-flag? $56 story-flag? not and if
        0 ebit? 1 ebit? and 2 ebit? and 3 ebit? and if
            0 0.0 0.0 0.0 $A 10 0 zone
            $FE 0 3 char-zone-bits? if
                $56 story-flag-set
                $7B story-flag-set
                $1B state-flag-set
                0 counter-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 8 action-force
                else
                    1 0 8 action-force
                then
            then
        then
        $FE char-dead? if
            6 ebit-set
        then
        6 ebit? if
            $FE char-dead? not if
                $23D item-give
            then
        then
    then
    3 room60.cmd00
    0 0 var? if
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 1 $14 door-bits
    else 0 1 var? if
        3 1 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 0 $14 door-bits
    else 0 2 var? if
        3 0 $14 door-bits
        4 1 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 0 $14 door-bits
    else 0 3 var? if
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 1 $14 door-bits
        6 0 $14 door-bits
        8 0 $14 door-bits
    else 0 4 var? if
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 1 $14 door-bits
        8 0 $14 door-bits
    else
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 1 $14 door-bits
    then then then then then
;

: room60.phase2 ( -- )   \ 00428460
    $10 state-flag? if
        -2147483646 scene-request? if
            5 6 1 scene-change
        then
    then
    $56 story-flag? if
        0 5 $2D char-faces-area? if
            5 7 0 scene-change
        then
    then
    $7F story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: room60.phase3 ( -- )   \ 004284A0
    50.0 -29.0 -143.0 -8.8 9.3 -143.0 50.0 -50.0 -143.0 -8.8 -10.0 -143.0 lights-doorway
    66.0 -39.0 -143.0 50.0 -29.0 -143.0 69.0 -55.0 -143.0 50.0 -50.0 -143.0 lights-doorway
;

: room60.act00 ( -- )   \ 00428510
    3 stalker-kind? if
        4 ebit-set
    else
        4 ebit-clear
    then
    2 0 char-remove
    $18 state-flag-set
    1 self-scripted
    $13 state-flag-set
    $E state-flag-set
    $1B state-flag-set
    self-wait-done
    4 ebit? if
        $23 partner-load
        $338 story-flag-clear
    else
        $24 partner-load
        $338 story-flag-set
    then
    0 state-flag-clear
    2 char-unload
    $F $44 fade
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
    8 state-flag-set
    $1D 3 $FF char-load
    0 room60.cmd01
    1 char-activate
    $60 0 433 hewie-to-room
    $FE $60 1571 2 stalker-to-room
    $FE 2 2 char-camera
    $FE action-end
    $FE char-done
    3 4 0 char-model-op
    3 char-unload
    0 $F9 1 action
    2 1 $14 door-bits
    $FE char-activate
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 $247 -0.592 1.575 180 char-to-xz
    1 $2CF 0.346 -5.781 180 char-to-xz
    $FE $50D -1.47 -23.64 0 char-to-xz
    2 0 $14 door-bits
    3 0 char-remove
    2 room60.cmd01
    1 room60.cmd03
    2 room60.cmd04
    0 room60.cmd05
    $70 door-open-clear
    $70 door-lock
    doors-room-in
    $10 state-flag-set
    $55 story-flag-set
    0 state-flag-clear
    0 $247 -0.592 1.575 180 char-to-xz
    1 $2CF 0.346 -5.781 180 char-to-xz
    $FE $50D -1.47 -23.64 0 char-to-xz
    $FE 2 2 char-camera
    $1B state-flag-clear
    8 1.0 0 bgm
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    $13 state-flag-clear
    $E state-flag-clear
    self-idle-or-end
;

: room60.act01 ( -- )   \ 004286A0
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 room60.cmd05
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 room60.cmd05
    begin
        $468 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $477 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $484 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $499 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $49D cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4B3 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $4B4 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4D2 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $4D7 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4F2 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $E cutscene-shot? not while
        yield
    repeat
    0 room60.cmd03
    begin
        $11 cutscene-shot? not while
        yield
    repeat
    0 room60.cmd04
    begin
        $12 cutscene-shot? not while
        yield
    repeat
    1 room60.cmd03
    begin
        $B31 cutscene-cue-reached? not while
        yield
    repeat
    1 room60.cmd04
    begin
        $19 cutscene-shot? not while
        yield
    repeat
    1 room60.cmd01
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room60.act02 ( -- )   \ 00428750
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $FF panic-stage? not if
        $FE self-look-at
        yield
        0 5 char-in-area? if
            0.0 0.0 self-turn-to-xz
            self-wait-done
            $F00 $A self-anim-blend
            self-wait-anim
            $402 $A self-anim-blend
            self-wait-anim
        then
    then
    1 wait-counter
    $FF 1.0 0 bgm
    $13 state-flag-set
    $E state-flag-set
    $F 4 fade
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
    wait-fade
    $FF self-look-at
    yield
    0 1 char-visible
    1 action-end
    1 char-done
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 $F9 3 action
    $1E 1 object-show
    $338 story-flag? not if
        9 1 $14 door-bits
    else
        $A 1 $14 door-bits
    then
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
    2 6 sound-stop
    $1E 1 object-show
    $10 state-flag-clear
    $13 state-flag-clear
    $E state-flag-clear
    $1B state-flag-clear
    2 0 char-remove
    7 1 $14 door-bits
    0 0 var-set
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
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
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 self-scripted
    0 panic-stage
    $7F story-flag? not if
        0 1.29 1.0 -17.02 flicker-sprite
    then
    0 0 char-visible
    1 char-activate
    1 char-full-health
    0 $1E6 -22.22 -47.34 27 char-to-xz
    1 $30 -14.71 -55.27 21 char-to-xz
    1 self-anim
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    1 0 8 nav-group
    1 room60.cmd00
    0 1 room60.cmd02
    1 1 room60.cmd02
    2 1 room60.cmd02
    3 1 room60.cmd02
    0 camera-follow
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    $70 door-unlock
    $FE $60 0 room-doors-state
    $FE $63 0 room-doors-state
    self-idle-or-end
;

: room60.act03 ( -- )   \ 00428950
    begin
        2 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-show
    begin
        4 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-show
    begin
        5 cutscene-shot? not while
        yield
    repeat
    1 room60.cmd00
    0 0 var-set
    begin
        $35A cutscene-cue-reached? not while
        yield
    repeat
    0 1 room60.cmd02
    1 1 room60.cmd02
    2 1 room60.cmd02
    3 1 room60.cmd02
    begin
        6 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-show
    0 0 room60.cmd02
    1 0 room60.cmd02
    2 0 room60.cmd02
    3 0 room60.cmd02
    begin
        $369 cutscene-cue-reached? not while
        yield
    repeat
    0 1 room60.cmd02
    1 1 room60.cmd02
    2 1 room60.cmd02
    3 1 room60.cmd02
    2 6 sound-stop
    begin
        7 cutscene-shot? not while
        yield
    repeat
    2 0 room60.cmd02
    3 0 room60.cmd02
    begin
        $378 cutscene-cue-reached? not if
            yield
        else
            2 1 room60.cmd02
            3 1 room60.cmd02
            $A cutscene-shot? not if
                yield
            else
                $1E 1 object-show
                begin
                    0 cutscene-cue-reached? while
                    yield
                repeat
                self-idle-or-end
            then
        then
    again
;

: room60.act04 ( -- )   \ 00428A10
    $FE camera-follow
    1 self-scripted
    self-wait-done
    $FE $1D char-file-load
    0 -0.03 0.0 -90 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    $8000 5 self-anim-blend
    self-frames-reset
    $18D self-wait-frames
    counter-inc
    self-wait-anim
    begin
        yield
    again
;

: room60.act05 ( -- )   \ 00428A40
    self-wait-done
    0.0 -17.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $7F story-flag-set
    0 effect-remove
    $11 message-param-room
    $11 1 item-give-count
    0 $11 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    50 hewie-trust
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: room60.act06 ( -- )   \ 0047AD40
    1 message
    self-idle-or-end
;

: room60.act07 ( -- )   \ 00428A90
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    5 ebit? not if
        2 message
        wait-message
        5 ebit-set
    else
        $A02 $A self-anim-blend
        self-wait-anim
        3 message
        wait-message
        5 ebit-clear
    then
    self-idle-or-end
;

: room60.act08 ( -- )   \ 00428AC0
    $F 0 fade
    $18 state-flag-set
    $13 state-flag-set
    $E state-flag-set
    1 self-scripted
    self-wait-done
    wait-fade
    8 state-flag-set
    yield
    0 $FE 9 action-force
    1 wait-counter
    0 1 char-visible
    1 action-end
    1 char-done
    $FF 1.0 0 bgm
    $FF 3 -1 char-camera
    0 panic-stage
    counter-inc
    $F 1 fade
    3 wait-counter
    wait-fade
    $F 4 fade
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
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 $F9 3 action
    $1E 1 object-show
    $338 story-flag? not if
        9 1 $14 door-bits
    else
        $A 1 $14 door-bits
    then
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
    wait-fade
    2 6 sound-stop
    $1E 1 object-show
    0 0 char-visible
    $10 state-flag-clear
    $13 state-flag-clear
    $E state-flag-clear
    $1B state-flag-clear
    2 0 char-remove
    7 1 $14 door-bits
    0 0 var-set
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
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
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 self-scripted
    0 panic-stage
    $7F story-flag? not if
        0 1.29 1.0 -17.02 flicker-sprite
    then
    0 0 char-visible
    1 char-activate
    1 char-full-health
    0 $1E6 -22.22 -47.34 27 char-to-xz
    0 self-move-16
    1 $30 -14.71 -55.27 21 char-to-xz
    $29 0 hewie-action
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    1 0 8 nav-group
    1 room60.cmd00
    0 1 room60.cmd02
    1 1 room60.cmd02
    2 1 room60.cmd02
    3 1 room60.cmd02
    0 camera-follow
    $F $41 fade
    wait-fade
    $23E item-give
    $8282 item-give
    builtin.act9C
    $18 state-flag-clear
    $70 door-unlock
    $FE $60 0 room-doors-state
    $FE $63 0 room-doors-state
    self-idle-or-end
;

: room60.act09 ( -- )   \ 00428CD0
    1 self-scripted
    self-wait-done
    $FE $1D char-file-load
    $FE char-file-use
    counter-inc
    2 wait-counter
    $FE $1AB 0.25 0.25 -90 char-to-xz
    $8000 self-anim
    self-frames-reset
    $96 self-wait-frames
    $FE 3 6 char-sound
    self-frames-reset
    $F7 self-wait-frames
    self-wait-anim
    counter-inc
    begin
        yield
    again
;

: room60.act0A ( -- )   \ 00428D10
    self-wait-done
    9 0 $14 door-bits
    $A 0 $14 door-bits
    8 1 $14 door-bits
    0 1 $14 door-bits
    1 0 $14 door-bits
    0 0 0 $217 $4E2 obstacle-place
    1 1 0 $12D $3F3 obstacle-place
    2 2 0 $249 $519 obstacle-place
    3 3 0 $272 $544 obstacle-place
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    3 ebit-clear
    7 0 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
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
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 room60.cmd00
    $23 partner-load
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
    $1D 3 $FF char-load
    0 room60.cmd01
    3 4 0 char-model-op
    3 char-unload
    0 $F9 1 action
    2 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    2 0 $14 door-bits
    3 0 char-remove
    2 room60.cmd01
    1 room60.cmd03
    2 room60.cmd04
    0 room60.cmd05
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room60.act0B ( -- )   \ 00428E70
    self-wait-done
    9 0 $14 door-bits
    $A 0 $14 door-bits
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 0 0 $20A $4D8 obstacle-place
    1 1 0 $153 $415 obstacle-place
    2 2 0 $15C $41E obstacle-place
    3 3 0 $213 $4DE obstacle-place
    0 obstacle-stop
    1 obstacle-stop
    2 obstacle-stop
    3 obstacle-stop
    0 ebit-set
    1 ebit-set
    2 ebit-set
    3 ebit-set
    7 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 1 $14 door-bits
    8 0 $14 door-bits
    2 0 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
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
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 4 var-set
    0 room60.cmd00
    0 0 room60.cmd02
    1 0 room60.cmd02
    2 0 room60.cmd02
    3 0 room60.cmd02
    $23 partner-load
    2 char-unload
    0 1 char-visible
    $FF 3 -1 char-camera
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
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 $F9 3 action
    $1E 1 object-show
    9 1 $14 door-bits
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
    2 6 sound-stop
    $1E 1 object-show
    2 0 char-remove
    7 1 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
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
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    1 room60.cmd00
    0 1 room60.cmd02
    1 1 room60.cmd02
    2 1 room60.cmd02
    3 1 room60.cmd02
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room60.enter $60 0 room-script!
' room60.char-enter $60 6 room-script!
' room60.phase1 $60 1 room-script!
' room60.phase2 $60 2 room-script!
' room60.phase3 $60 3 room-script!
' room60.act00 $60 $00 action-script!
' room60.act01 $60 $01 action-script!
' room60.act02 $60 $02 action-script!
' room60.act03 $60 $03 action-script!
' room60.act04 $60 $04 action-script!
' room60.act05 $60 $05 action-script!
' room60.act06 $60 $06 action-script!
' room60.act07 $60 $07 action-script!
' room60.act08 $60 $08 action-script!
' room60.act09 $60 $09 action-script!
' room60.act0A $60 $0A action-script!
' room60.act0B $60 $0B action-script!

\ ---- room $61 ----------------------------------------------------------------------------------

\ room 0x61 (byte 3): 0 / 2 / 4 set up the paths of characters 0x14 (2 x 3 x 7 cells from (25,
\ 0, 40), 480 frames a stretch; with the glint Glint_vtable) / 0x15 (2 x 3 x 5 from (30, 0, 50),
\ 240) / 0x16 (the same box, 280); 1 / 3 / 5 move them a frame (returning 2: again next frame)
: room61.cmd00 ( b0 -- )  drop s" room61.cmd00" stub-step ;
\ room 0x61: the light shaft: byte 3 0 starts it (LightShaft, with its motes from (30, 0, 70)),
\ its slot in event variable 3; 1 its haze on; 2 off.
: room61.cmd01 ( b0 -- )  drop s" room61.cmd01" stub-step ;

: room61.enter ( -- )   \ 004291C0
    0 $F1 0 action
    0 $F2 1 action
    0 $F3 2 action
    0 0 var-set
    1 20 var-set
    2 90 var-set
    $2A5 story-flag? $2A6 story-flag? not and if
        0 8.95 1.0 11.89 flicker-sprite
    then
    1 $2300 sound-volume
    0 room61.cmd01
    1 2 $10000000 nav-group
;

: room61.char-enter ( -- )   \ 00429210
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
    0 self-is? if
        0 exit-taken? if
            1 map-page
        then
    then
;

: room61.phase1 ( -- )   \ 004292A0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $B char-entered-area? if
        1 map-page
    then
    0 $B char-left-area? if
        0 map-page
    then
    0 3 var? if
        0 game-mode? if
            2 var-dec
            2 0 var? if
                2 90 var-set
                1 20 var? if
                    1 21 var-set
                else 1 21 var? if
                    1 22 var-set
                else 1 22 var? if
                    1 20 var-set
                then then then
            then
            1 20 var? if
                1 char-here? if
                    $14 1 100 chars-within? if
                        $14 0.0 hewie-look-char
                    then
                then
            else 1 21 var? if
                1 char-here? if
                    $15 1 100 chars-within? if
                        $15 0.0 hewie-look-char
                    then
                then
            else 1 22 var? if
                1 char-here? if
                    $16 1 100 chars-within? if
                        $16 0.0 hewie-look-char
                    then
                then
            then then then
        then
    then
    $2A5 story-flag? not if
        0 8.95 0.0 11.89 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 677 var-set
                $1A 678 var-set
                $1B 0 var-set
                $1C 8950 var-set
                $1D 1000 var-set
                $1E 11890 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
;

: room61.phase2 ( -- )   \ 004293E0
    0 $A char-in-area? if
        5 4 0 scene-change
    then
    $2A5 story-flag? $2A6 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
;

: room61.phase3 ( -- )   \ 00429408
    2 camera-mode? if
        1 room61.cmd01
    else
        2 room61.cmd01
    then
;

: room61.phase5 ( -- )   \ 00429418
    3 action-end
    3 char-done
    4 action-end
    4 char-done
    5 action-end
    5 char-done
;

: room61.act00 ( -- )   \ 00429430
    $14 3 $FF char-load
    3 char-unload
    $14 char-activate
    $14 $9000 1 0 char-anim-hold
    $14 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    0 room61.cmd00
    1 room61.cmd00
    self-idle-or-end
;

: room61.act01 ( -- )   \ 00429460
    $15 4 $FF char-load
    4 char-unload
    $15 char-activate
    $15 $9000 1 0 char-anim-hold
    $15 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    2 room61.cmd00
    3 room61.cmd00
    self-idle-or-end
;

: room61.act02 ( -- )   \ 00429490
    $16 5 $FF char-load
    5 char-unload
    self-frames-reset
    $A self-wait-frames
    $16 char-activate
    $16 $9000 1 0 char-anim-hold
    $16 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    4 room61.cmd00
    5 room61.cmd00
    self-idle-or-end
;

: room61.act03 ( -- )   \ 004294C0
    self-wait-done
    8.95 11.89 self-turn-to-xz
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
            $2A6 story-flag-set
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

: room61.act04 ( -- )   \ 00429520
    self-wait-done
    1 self-anim
    self-wait-anim
    -1 self-move-16
    0 message
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

' room61.enter $61 0 room-script!
' room61.char-enter $61 6 room-script!
' room61.phase1 $61 1 room-script!
' room61.phase2 $61 2 room-script!
' room61.phase3 $61 3 room-script!
' room61.phase5 $61 5 room-script!
' room61.act00 $61 $00 action-script!
' room61.act01 $61 $01 action-script!
' room61.act02 $61 $02 action-script!
' room61.act03 $61 $03 action-script!
' room61.act04 $61 $04 action-script!

\ ---- room $62 ----------------------------------------------------------------------------------

\ room 0x62 (Room62_Cmd00_ptmf): object byte 4 swings: byte 3 0 starts it (phase +0x30 0, size
\ +0x34 0.01; object 0 with sound 7), else a step (+0x10 = size x sin(phase), phase on 60
\ degrees, the size down 0.001); 2 until it is still
: room62.cmd00 ( b0 b1 -- )  drop drop s" room62.cmd00" stub-step ;
\ room 0x62 (D_00422318): the room's effect 0 dropped by byte 3 - 0 at rest (speed 0), 1 raised
\ by 0.5; else it falls (gravity 0.5 a frame, turning 0.16) and bounces off 0.7 losing 70% (a
\ sound each bounce) until slower than 0.2 (+0x74 set: landed; 1), else still going (2)
: room62.cmd01 ( b0 -- )  drop s" room62.cmd01" stub-step ;

: room62.enter ( -- )   \ 00421C60
    room-sounds
    2 0 var-set
    3 0 var-set
    $72 story-flag? not if
        0 5.19 35.3 -34.0 flicker-sprite
        0 room62.cmd01
    else $53 story-flag? not if
        0 7.3 0.7 -28.3 flicker-sprite
    then then
    $26C story-flag? not if
        1 -6.82 1.0 -10.25 flicker-sprite
    then
    $AF story-flag? if
        $81 story-flag? not if
            $81 story-flag-set
            $62 0 15 $80 8 -1 $5A 0.0 creature-place
        then
    then
;

: room62.char-enter ( -- )   \ 00421CD0
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
;

: room62.phase1 ( -- )   \ 00421D50
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 1 1 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 3.0 0.0 -35.5 9 15 0 zone
    1 -5.75 0.0 -33.25 9 15 0 zone
    2 -14.5 0.0 -31.0 9 15 0 zone
    $72 story-flag? not if
        0 0 char-in-zone? 0 1 char-in-zone? or 0 2 char-in-zone? or if
            0 $F1 $B action
        then
    then
    4 21.19 0.0 -20.13 $C 10 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    5 19.29 0.0 -1.64 $14 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    6 sound-bank-loaded? if
        3 var-inc
        3 30 var? if
            3 0 var-set
            2 0 var? if
                $40000003 6 37.86 0.0 -38.87 0 0 sound
            else 2 1 var? if
                $40000004 6 37.86 0.0 -38.87 0 0 sound
            else 2 2 var? if
                $40000005 6 37.86 0.0 -38.87 0 0 sound
            else 2 3 var? if
                $40000006 6 37.86 0.0 -38.87 0 0 sound
            then then then then
            2 var-inc
            2 4 var? if
                2 0 var-set
            then
        then
    then
;

: room62.phase2 ( -- )   \ 00421EF0
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    $72 story-flag? $53 story-flag? not and if
        3 0 5 5 0 zone-at-effect
        0 3 2 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    0 8 char-in-area? 0 90 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 9 char-in-area? 0 60 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 -10 $32 char-heading? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 90 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $C char-in-area? 0 0 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $D char-in-area? 0 -80 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $E char-in-area? 0 90 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $10 char-in-area? 0 -45 $32 char-heading? and if
        5 7 0 scene-change
    then
    $26C story-flag? not if
        6 1 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
    0 $11 char-in-area? 0 -45 $32 char-heading? and if
        5 9 0 scene-change
    then
    0 $12 char-in-area? 0 -45 $32 char-heading? and if
        5 $A 0 scene-change
    then
;

: room62.phase3 ( -- )   \ 00421FC0
    -7.0 30.0 42.0 -10.8 30.0 29.2 -7.0 0.0 42.0 -10.8 0.0 29.2 lights-doorway
;

: room62.act00 ( -- )   \ 00422000
    self-wait-done
    $FE self-touching? not if
        $67 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $67 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $67 door-unlock
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

: room62.act01 ( -- )   \ 00422040
    self-wait-done
    7.3 -28.3 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $53 story-flag-set
    0 effect-remove
    $D message-param-room
    $D 1 item-give-count
    0 $D item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: room62.act02 ( -- )   \ 00422090
    $72 story-flag-set
    2 room62.cmd01
    0 7.3 0.7 -28.3 flicker-sprite
    self-idle-or-end
;

: room62.act03 ( -- )   \ 004220A8
    0 0 room62.cmd00
    1 0 room62.cmd00
    self-idle-or-end
;

: room62.act04 ( -- )   \ 004220C0
    self-wait-done
    5.19 -34.0 self-turn-to-xz
    self-wait-done
    0 ebit? not $72 story-flag? or if
        0 message
        wait-message
        0 ebit-set
    else
        $A00 self-anim
        self-wait-anim
        1 message
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: room62.act05 ( -- )   \ 004220F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -20 self-turn-angle
    self-wait-done
    $331 story-flag? not if
        2 message
        wait-message
        $331 story-flag-set
    else
        3 message
        wait-message
    then
    $F 6 fade
    wait-fade
    4 message
    wait-message
    $F 7 fade
    wait-fade
    $237 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room62.act06 ( -- )   \ 00422130
    self-wait-done
    -6.82 -10.25 self-turn-to-xz
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
            $26C story-flag-set
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

: room62.act07 ( -- )   \ 00422190
    self-wait-done
    0 $B char-in-area? if
        180 self-turn-angle
        self-wait-done
    else 0 $C char-in-area? if
        0 self-turn-angle
        self-wait-done
    else 0 $D char-in-area? if
        160 self-turn-angle
        self-wait-done
    else 0 $E char-in-area? if
        180 self-turn-angle
        self-wait-done
    else 0 $F char-in-area? if
        0 self-turn-angle
        self-wait-done
    else 0 $10 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    then then then then then then
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 0 var? if
        5 message
        wait-message
        1 var-inc
    else 1 1 var? if
        6 message
        wait-message
        1 var-inc
    else 1 2 var? if
        7 message
        wait-message
        1 0 var-set
    then then then
    self-wait-anim
    self-idle-or-end
;

: room62.act08 ( -- )   \ 00422208
    0 1 room62.cmd00
    1 1 room62.cmd00
    self-idle-or-end
;

: room62.act09 ( -- )   \ 00422220
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    $A message
    wait-message
    $F 7 fade
    wait-fade
    $63 subscreen-bit? not $65 subscreen-bit? not or if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $63 subscreen-bit
        $65 subscreen-bit
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room62.act0A ( -- )   \ 00422260
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    $46 subscreen-bit? not $47 subscreen-bit? not or if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $46 subscreen-bit
        $47 subscreen-bit
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room62.act0B ( -- )   \ 004222A0
    0 $F2 3 action
    0 $F4 8 action
    0 var-inc
    0 3 var? if
        0 $F3 2 action
    else
        1 room62.cmd01
    then
    begin
        fiona-free? not while
        yield
    repeat
    self-idle-or-end
;

' room62.enter $62 0 room-script!
' room62.char-enter $62 6 room-script!
' room62.phase1 $62 1 room-script!
' room62.phase2 $62 2 room-script!
' room62.phase3 $62 3 room-script!
' room62.act00 $62 $00 action-script!
' room62.act01 $62 $01 action-script!
' room62.act02 $62 $02 action-script!
' room62.act03 $62 $03 action-script!
' room62.act04 $62 $04 action-script!
' room62.act05 $62 $05 action-script!
' room62.act06 $62 $06 action-script!
' room62.act07 $62 $07 action-script!
' room62.act08 $62 $08 action-script!
' room62.act09 $62 $09 action-script!
' room62.act0A $62 $0A action-script!
' room62.act0B $62 $0B action-script!

\ ---- room $63 ----------------------------------------------------------------------------------

\ room 0x63: the script's character walks to Fiona's nav triangle (character move 6, with
\ 0x204).
: room63.cmd00 ( -- )  s" room63.cmd00" stub-step ;

defer room63.act05
defer room63.act06
defer room63.act07
: room63.enter ( -- )   \ 00421130
    room-sounds
    $46 story-flag? not if
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 0 action-force
    then
    $57 story-flag? not if
        1 char-here? 1 0 char-in-nav-group? and if
            0 exit-taken? if
                1 $187 char-to-tri
            else
                1 $195 char-to-tri
            then
        then
        1 0 $1000000 nav-group
        1 1 $1000000 nav-group
        1 2 $1000000 nav-group
        1 3 $1000000 nav-group
    then
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
    1 $2300 sound-volume
;

: room63.char-enter ( -- )   \ 004211D0
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
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
;

: room63.phase1 ( -- )   \ 00421250
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $57 story-flag? not if
        1 2 char-C4? not $FE char-here? not and 1 char-here? and if
            35 fiona-started? 45 fiona-started? or if
                0 0 -55 $32 char-faces-xz? 0 0 -100 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 5 action
                    then
                then
                0 0 55 $32 char-faces-xz? 0 0 110 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 6 action
                    then
                then
            then
            44 fiona-started? if
                hewie-stays? if
                    0 1 8 action
                then
            then
        then
        0 1 char-in-nav-group? if
            1 1 var? not if
                0 ebit-set
            then
            1 1 var-set
        else 0 2 char-in-nav-group? if
            1 2 var? not if
                0 ebit-set
            then
            1 2 var-set
        else 0 3 char-in-nav-group? if
            1 3 var? not if
                0 ebit-set
            then
            1 3 var-set
        else
            1 0 var-set
        then then then
        0 ebit? if
            4 ebit? not if
                0 1 6 char-sound
                0 0 var? if
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 1 action-force
                    else
                        1 0 1 action-force
                    then
                else 0 1 var? if
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 2 action-force
                    else
                        1 0 2 action-force
                    then
                else 0 2 var? if
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 3 action-force
                    else
                        1 0 3 action-force
                    then
                then then then
                0 var-inc
                4 ebit-set
            then
            0 ebit-clear
        then
    then
    $57 story-flag? not if
        5 ebit? if
            $23C item-give
            $8281 item-give
        then
        0 -0.27 0.0 10.71 $51 8 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $92 action
                        then
                    then
                then
            then
        then
    then
    1 0.0 0.0 -68.2 $12 19 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
;

: room63.phase2 ( -- )   \ 00421400
    0 6 char-in-area? 0 0 -68 $41 char-faces-xz? and if
        5 9 0 scene-change
    then
;

: room63.phase5 ( -- )   \ 00421420
    $57 story-flag? not if
        1 char-here? 1 0 char-in-nav-group? and if
            0 0 char-in-area? if
                1 $187 char-to-tri
            else
                1 $195 char-to-tri
            then
        then
    then
;

: room63.act00 ( -- )   \ 00421440
    $18 state-flag-set
    1 self-scripted
    self-wait-done
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
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 0 $14 door-bits
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
    0 self-move-16
    0 $135 0.064 100.888 171 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    $46 story-flag-set
    0 self-scripted
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room63.act04 ( -- )   \ 00421700
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    0 1 $14 door-bits
    exit
;

: room63.act01 ( -- )   \ 00421500
    $18 state-flag-set
    1 self-scripted
    4 ebit-set
    $F $44 fade
    self-wait-done
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
    1 char-busy? not if
        1 char-here? 1 2 char-C4? and if
            0 1 $86 action
        then
    then
    $FF 1 char-visible
    0 0 $14 door-bits
    $14 $78 movie-param
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
    1 char-here? 1 2 char-C4? and if
        1 0 char-visible
        1 action-end
    then
    $FF 0 char-visible
    room63.act04
    $F $41 fade
    wait-fade
    4 ebit-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room63.act02 ( -- )   \ 004215C0
    $18 state-flag-set
    1 self-scripted
    4 ebit-set
    $F $44 fade
    self-wait-done
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
    wait-fade
    1 char-busy? not if
        1 char-here? 1 2 char-C4? and if
            0 1 $86 action
        then
    then
    $FF 1 char-visible
    0 0 $14 door-bits
    $14 $78 movie-param
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
    1 char-here? 1 2 char-C4? and if
        1 0 char-visible
        1 action-end
    then
    $FF 0 char-visible
    room63.act04
    $F $41 fade
    wait-fade
    4 ebit-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room63.act03 ( -- )   \ 00421680
    $2C state-flag-set
    $18 state-flag-set
    1 self-scripted
    $19 state-flag-set
    4 ebit-set
    $F $44 fade
    self-wait-done
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
    wait-fade
    1 action-end
    1 char-done
    $FF 1 char-visible
    0 0 $14 door-bits
    $10 $46 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        movie-loop
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $F 1 fade
        $A cutscene-control
        begin
            7 cutscene-control
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
        until
    then
    $30 resident-flag-set
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room63.act08 ( -- )   \ 00421840
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 $1000000 nav-group
    room63.cmd00
    begin
        self-done? not if
            35 fiona-started? 45 fiona-started? or if
                0 0 -55 $32 char-faces-xz? 0 0 -100 $32 char-faces-xz? and if
                    self-idle
                    self-wait-done
                    ['] room63.act05 goto
                then
                0 0 55 $32 char-faces-xz? 0 0 110 $32 char-faces-xz? and if
                    self-idle
                    self-wait-done
                    ['] room63.act06 goto
                then
            then
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] room63.act07 goto
            then
            yield
        else
            1 0 char-in-nav-group? if
                ['] room63.act07 goto
            else
                1 0 $1000000 nav-group
            then
            self-idle-or-end
        then
    again
;

:noname   \ room63.act06 (00421790; deferred: used before it is defined)
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 $1000000 nav-group
    $11F $204 6 self-move-tri
    begin
        self-done? not if
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] room63.act07 goto
            then
            44 fiona-started? if
                self-idle
                self-wait-done
                ['] room63.act08 goto
            then
            yield
        else
            1 0 $1000000 nav-group
            self-idle-or-end
        then
    again
; is room63.act06

:noname   \ room63.act07 (004217D0; deferred: used before it is defined)
    self-wait-done
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-done
    0 self-look-at
    yield
    begin
        35 fiona-started? 45 fiona-started? or if
            0 0 -55 $32 char-faces-xz? 0 0 -100 $32 char-faces-xz? and if
                $FF self-look-at
                yield
                $101 self-anim
                self-wait-anim
                -1 self-move-16
                ['] room63.act05 goto
            then
            0 0 55 $32 char-faces-xz? 0 0 110 $32 char-faces-xz? and if
                $FF self-look-at
                yield
                $101 self-anim
                self-wait-anim
                -1 self-move-16
                ['] room63.act06 goto
            then
        then
        44 fiona-started? if
            $FF self-look-at
            yield
            $101 self-anim
            self-wait-anim
            -1 self-move-16
            ['] room63.act08 goto
        then
        yield
    again
; is room63.act07

:noname   \ room63.act05 (00421750; deferred: used before it is defined)
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 $1000000 nav-group
    $187 $204 6 self-move-tri
    begin
        self-done? not if
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] room63.act07 goto
            then
            44 fiona-started? if
                self-idle
                self-wait-done
                ['] room63.act08 goto
            then
            yield
        else
            1 0 $1000000 nav-group
            self-idle-or-end
        then
    again
; is room63.act05

: room63.act09 ( -- )   \ 004218B0
    self-wait-done
    0.0 -68.0 self-turn-to-xz
    self-wait-done
    $57 story-flag? not if
        $A00 self-anim
        self-frames-reset
        self-wait-16
        0 message
        wait-message
        self-wait-anim
        0 answer? if
            $904 self-anim
            self-wait-anim
            0 2 6 char-sound
            $57 story-flag-set
            30 hewie-trust
            0 0 $1000000 nav-group
            0 1 $1000000 nav-group
            0 2 $1000000 nav-group
            0 3 $1000000 nav-group
            $905 self-anim
            self-wait-anim
        then
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room63.act0A ( -- )   \ 00421910
    self-wait-done
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room63.act0B ( -- )   \ 004219E0
    self-wait-done
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
    $FF 1 char-visible
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
    $14 $78 movie-param
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
    $14 $78 movie-param
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
    $10 $46 movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room63.enter $63 0 room-script!
' room63.char-enter $63 6 room-script!
' room63.phase1 $63 1 room-script!
' room63.phase2 $63 2 room-script!
' room63.phase5 $63 5 room-script!
' room63.act00 $63 $00 action-script!
' room63.act01 $63 $01 action-script!
' room63.act02 $63 $02 action-script!
' room63.act03 $63 $03 action-script!
' room63.act04 $63 $04 action-script!
' room63.act05 $63 $05 action-script!
' room63.act06 $63 $06 action-script!
' room63.act07 $63 $07 action-script!
' room63.act08 $63 $08 action-script!
' room63.act09 $63 $09 action-script!
' room63.act0A $63 $0A action-script!
' room63.act0B $63 $0B action-script!

\ ---- room $66 ----------------------------------------------------------------------------------

\ Room66_Cmd00
: room66.cmd00 ( b0 -- )  drop s" room66.cmd00" stub-step ;
\ Room66_Cmd01
: room66.cmd01 ( b0 b1 -- )  drop drop s" room66.cmd01" stub-step ;
\ Room66_Cmd02
: room66.cmd02 ( b0 -- )  drop s" room66.cmd02" stub-step ;
\ Room66_Cmd03
: room66.cmd03 ( b0 b1 -- )  drop drop s" room66.cmd03" stub-step ;
\ Room66_Cmd04
: room66.cmd04 ( -- )  s" room66.cmd04" stub-step ;
\ Room66_Cond00
: room66.cond00? ( b0 -- flag )  drop s" room66.cond00?" stub-flag ;
\ Room66_Cond01
: room66.cond01? ( b0 b1 b2 b3 b4 -- flag )  drop drop drop drop drop s" room66.cond01?" stub-flag ;

: room66.enter ( -- )   \ 0041E110
    room-sounds
    0 1 $14 door-bits
    1 4 $20000 nav-group
    $5D story-flag? not if
        1 0 $20000 nav-group
        0 0 room66.cmd01
    then
    $5E story-flag? not if
        1 1 $20000 nav-group
        1 0 room66.cmd01
    then
    $5F story-flag? not if
        1 2 $20000 nav-group
        2 0 room66.cmd01
    then
    $60 story-flag? not if
        1 3 $20000 nav-group
        3 0 room66.cmd01
    then
    $271 story-flag? not if
        0 2.06 6.5 -99.68 flicker-sprite
    then
    $2C3 story-flag? not if
        2 39.98 1.0 -40.67 flicker-sprite
    then
    $2A9 story-flag? $2AA story-flag? not and if
        1 -138.94 1.0 -114.31 flicker-sprite
    then
    3 117.5 11.7 -21.8 0 effect-86
    4 -217.0 20.1 20.0 1 effect-86
    5 -175.0 20.1 4.7 1 effect-86
    6 -70.0 20.1 -124.7 1 effect-86
    7 -20.0 20.1 -96.7 1 effect-86
    8 11.7 20.1 -55.8 1 effect-86
    9 43.0 20.1 -26.5 1 effect-86
    $A -33.2 20.1 -6.2 1 effect-86
    $B 89.3 20.1 50.0 1 effect-86
    $C -12.4 28.3 24.6 1 effect-86
    $D -40.0 20.1 95.6 1 effect-86
    $E -65.5 20.1 0.0 1 effect-86
    1 $2300 sound-volume
;

: room66.char-enter ( -- )   \ 0041E260
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 6 4 char-camera
                0 camera-follow
            else
                1 6 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 6 4 char-camera
            0 camera-follow
        else
            1 6 4 char-camera
            1 camera-follow
        then
    then then
    0 6 4 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 5 3 char-camera
                0 camera-follow
            else
                1 5 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 3 char-camera
            0 camera-follow
        else
            1 5 3 char-camera
            1 camera-follow
        then
    then then
    1 5 3 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 2 char-camera
                0 camera-follow
            else
                1 4 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 2 char-camera
            0 camera-follow
        else
            1 4 2 char-camera
            1 camera-follow
        then
    then then
    2 4 2 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 2 0 char-camera
                0 camera-follow
            else
                1 2 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 0 char-camera
            0 camera-follow
        else
            1 2 0 char-camera
            1 camera-follow
        then
    then then
    3 2 0 area-camera
;

: room66.phase1 ( -- )   \ 0041E360
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
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
    8 0 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    $A 0 -1 1 chars-area-camera
    $B 2 0 1 chars-area-camera
    $C 1 -1 1 chars-area-camera
    $D 3 1 1 chars-area-camera
    $E 3 1 1 chars-area-camera
    $F 4 2 1 chars-area-camera
    $10 3 1 1 chars-area-camera
    $11 5 3 1 chars-area-camera
    $12 5 3 1 chars-area-camera
    $13 6 4 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    0 5 char-entered-area? if
        $C ebit-set
    then
    0 5 char-left-area? if
        $C ebit? not if
            3 0 char-remove
        then
        $C ebit-clear
    then
    0 4 char-entered-area? if
        $D ebit-set
    then
    0 4 char-left-area? if
        $D ebit? not if
            3 0 char-remove
            4 0 char-remove
            5 0 char-remove
        then
        $D ebit-clear
    then
    $C ebit? loader-done? not and if
        $1C 3 1 char-load
        $C ebit-clear
    then
    $D ebit? loader-done? not and if
        $14 3 $FF char-load
        $15 4 $FF char-load
        $16 5 $FF char-load
        $D ebit-clear
    then
    0 $11 char-entered-area? if
        8 ebit-set
    then
    0 $11 char-left-area? if
        8 ebit-clear
    then
    0 4 char-entered-area? if
        9 ebit-set
    then
    0 4 char-left-area? if
        9 ebit-clear
    then
    7 ebit? not if
        6 sound-bank-loaded? if
            $5D story-flag? not if
                $40000007 6 -185.0 10.0 20.0 0 0 sound
            else $5E story-flag? not if
                $40000007 6 30.0 10.0 -42.0 0 0 sound
            then then
            $5F story-flag? not if
                $40000008 6 -20.0 10.0 10.0 0 0 sound
            else $60 story-flag? not if
                $40000008 6 -40.0 10.0 -110.0 0 0 sound
            then then
            7 ebit-set
        then
    else
        $5D story-flag? not $5E story-flag? not or if
            8 ebit? $5D story-flag? not and $5E story-flag? or if
                $C0000007 6 -185.0 10.0 20.0 0 0 sound
            then
            8 ebit? not $5E story-flag? not and $5D story-flag? or if
                $C0000007 6 30.0 10.0 -42.0 0 0 sound
            then
        then
        $5F story-flag? not $60 story-flag? not or if
            9 ebit? not $5F story-flag? not and $60 story-flag? or if
                $C0000008 6 -20.0 10.0 10.0 0 0 sound
            then
            9 ebit? $60 story-flag? not and $5F story-flag? or if
                $C0000008 6 -40.0 10.0 -110.0 0 0 sound
            then
        then
    then
    $5D story-flag? not if
        0 $1C char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        then
    then
    $5E story-flag? not if
        0 $1E char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $15 action-force
        then
    then
    $5F story-flag? not if
        0 $20 char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $16 action-force
        then
    then
    $60 story-flag? not if
        0 $22 char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $17 action-force
        then
    then
    $2A9 story-flag? not if
        2 -138.94 0.0 -114.31 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 681 var-set
                $1A 682 var-set
                $1B 1 var-set
                $1C -138940 var-set
                $1D 1000 var-set
                $1E -114310 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    0 144.66 0.0 -5.88 $12 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    4 73.86 0.0 -40.16 $C 10 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    5 73.86 0.0 -40.16 $19 20 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 20.0 hewie-look-zone
            then
        then
    then
    6 3 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
;

: room66.phase2 ( -- )   \ 0041E700
    0 $14 $32 char-faces-area? if
        5 8 0 scene-change
    then
    0 $15 $32 char-faces-area? if
        5 $B 0 scene-change
    then
    0 $16 char-in-area? 0 75 -40 $32 char-faces-xz? and if
        5 $C 0 scene-change
    then
    0 $1B char-in-area? if
        5 $E 0 scene-change
    then
    0 $1D char-in-area? if
        5 $12 0 scene-change
    then
    0 $1F char-in-area? if
        5 $13 0 scene-change
    then
    0 $21 char-in-area? if
        5 $14 0 scene-change
    then
    $271 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $18 4 scene-change
        then
    then
    $2A9 story-flag? $2AA story-flag? not and if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $19 4 scene-change
        then
    then
    $2C3 story-flag? not if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $1A 4 scene-change
        then
    then
;

: room66.act05 ( -- )   \ 0041EB40
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        0 $FF $FF $15 $A0 room66.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    $60 story-flag? not if
        3 1 room66.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5F story-flag? if
        8 6 sound-stop
    then
    $60 story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    3 room66.cmd02
    $60 story-flag? not if
        $76 door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $60 story-flag-set
    0 3 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: room66.act04 ( -- )   \ 0041EA90
    9 ebit-set
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 $FF $FE $52 $50 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        8 $13 char-entered-area? if
            8 6 4 char-camera
        then
        yield
    repeat
    1 room66.cond00? if
        ['] room66.act05 goto
    then
    1 self-noclip
    begin
        1 $FF $FD $DD $20 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    3 0 room66.cmd03
    8 $A 6 char-sound
    begin
        1 $FF $FD $B6 $10 room66.cond01? while
        2 room66.cmd00
        yield
    repeat
    3 1 room66.cmd03
    3 ebit-set
    self-idle-or-end
;

: room66.act03 ( -- )   \ 0041E9A0
    8 ebit-set
    8 3 6 char-sound
    8 $401 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FD $DD $20 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        8 $11 char-entered-area? if
            8 5 3 char-camera
        then
        yield
    repeat
    1 room66.cond00? if
        ['] room66.act04 goto
    then
    1 self-noclip
    begin
        0 $FF $FD $67 $F0 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    $5D story-flag? not if
        0 1 room66.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5E story-flag? if
        7 6 sound-stop
    then
    $5D story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    0 room66.cmd02
    $5D story-flag? not if
        $5E door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5D story-flag-set
    0 0 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: room66.act07 ( -- )   \ 0041ECE0
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        1 $FF $FF $EC $78 room66.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        8 $F char-entered-area? if
            8 4 2 char-camera
        then
        yield
    repeat
    $5F story-flag? not if
        2 1 room66.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $60 story-flag? if
        8 6 sound-stop
    then
    $5F story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    2 room66.cmd02
    $5F story-flag? not if
        $7B door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5F story-flag-set
    0 2 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: room66.act06 ( -- )   \ 0041EC00
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FF $B1 $E0 room66.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        yield
    repeat
    1 room66.cond00? if
        ['] room66.act07 goto
    then
    1 self-noclip
    begin
        0 0 0 $3A $98 room66.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    $5E story-flag? not if
        1 1 room66.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5D story-flag? if
        7 6 sound-stop
    then
    $5E story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    1 room66.cmd02
    $5E story-flag? not if
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5E story-flag-set
    0 1 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: room66.act02 ( -- )   \ 0041E8D0
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 0 0 $4E $20 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        yield
    repeat
    0 room66.cond00? if
        ['] room66.act03 goto
    then
    begin
        1 $FF $FF $63 $C0 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        yield
    repeat
    1 room66.cond00? if
        ['] room66.act06 goto
    then
    1 self-noclip
    begin
        1 $FF $FE $EE $90 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    2 0 room66.cmd03
    8 $A 6 char-sound
    begin
        1 $FF $FE $C7 $80 room66.cond01? while
        2 room66.cmd00
        yield
    repeat
    2 1 room66.cmd03
    3 ebit-set
    self-idle-or-end
;

: room66.act01 ( -- )   \ 0041E830
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 room66.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FE $C7 $80 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        yield
    repeat
    1 room66.cond00? if
        ['] room66.act02 goto
    then
    1 self-noclip
    begin
        0 $FF $FE $79 $60 room66.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    1 0 room66.cmd03
    8 $A 6 char-sound
    begin
        0 $FF $FE $52 $50 room66.cond01? while
        2 room66.cmd00
        yield
    repeat
    1 1 room66.cmd03
    3 ebit-set
    self-idle-or-end
;

: room66.act00 ( -- )   \ 0041E7A0
    2 0 var-set
    0 room66.cmd00
    begin
        1 0 1 $38 $80 room66.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 room66.cmd00
        8 $D char-entered-area? if
            8 3 1 char-camera
        then
        yield
    repeat
    1 room66.cond00? if
        ['] room66.act01 goto
    then
    1 self-noclip
    begin
        1 0 1 $86 $A0 room66.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 room66.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    0 0 room66.cmd03
    8 $A 6 char-sound
    begin
        1 0 1 $AD $B0 room66.cond01? not while
        2 room66.cmd00
        yield
    repeat
    0 1 room66.cmd03
    3 ebit-set
    self-idle-or-end
;

: room66.act08 ( -- )   \ 0041EDA0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    item-3F-under-10? if
        0 ebit? not if
            $11 message
            wait-message
            0 ebit-set
        then
        $20 story-flag-clear
        2 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        yield
        $20 story-flag? if
            $3F message-param-room
            0 $3F item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $20 story-flag-set
    else 0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $12 message
        wait-message
    then then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room66.act09 ( -- )   \ 0047ACD0
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: room66.act0A ( -- )   \ 0041EE10
    $18 state-flag-set
    1 self-scripted
    1 ebit-clear
    2 ebit-clear
    3 ebit-clear
    5 ebit-set
    6 ebit-set
    $F $44 fade
    begin
        loader-done? while
        yield
    repeat
    8 3 $FF char-load
    self-wait-done
    $5A story-flag? not if
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
        3 char-unload
        wait-fade
        0 0 $14 door-bits
        0 4 $20000 nav-group
        1 char-here? if
            0 1 $10 action-force
        then
        $28 $78 movie-param
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
        $F state-flag-set
        0 self-move-16
        0 $56 83.405 -17.137 -28 char-to-xz
        8 $126 74.6 13.8 0 char-to-xz
        8 3 1 char-camera
        $5A story-flag-set
    else
        wait-fade
        $FF panic-stage? if
            3 panic-stage
        then
        $F state-flag-set
        1 char-here? if
            0 1 $10 action-force
        then
        3 char-unload
        0 0 $14 door-bits
        0 4 $20000 nav-group
        0 $5E 92.522 -37.951 -62 char-to-xz
        8 $196 75.0 -40.0 0 char-to-xz
        8 1 -1 char-camera
    then
    8 char-activate
    8 $200 1 0 char-anim-hold
    8 camera-follow
    camera-restart
    0 8 0 action-force
    6 message
    $F 1 fade
    wait-fade
    begin
        3 ebit? not while
        0 2 pad? 1 ebit? not and if
            6 message-close
            1 ebit-set
            2 ebit-set
        then
        yield
    repeat
    $F 4 fade
    wait-fade
    $F state-flag-clear
    6 message-close
    1 char-busy? if
        1 action-end
    then
    8 action-end
    8 char-done
    3 0 char-remove
    0 1 $14 door-bits
    1 4 $20000 nav-group
    0 camera-follow
    camera-restart
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room66.act0B ( -- )   \ 0041EF90
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    7 message
    wait-message
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room66.act0C ( -- )   \ 0041EFB0
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    5 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room66.act0D ( -- )   \ 0047ACD8
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: room66.act0E ( -- )   \ 0041EFC0
    self-wait-done
    -170.0 20.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room66.act0F ( -- )   \ 0041EFE0
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 -45 $3C char-heading? if
        $F00 self-anim
    else 0 45 $3C char-heading? if
        $F01 self-anim
    else 0 0 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: room66.act10 ( -- )   \ 0041F020
    begin
        1 char-busy? not while
        yield
    repeat
    $FF 1 char-visible
    begin
        yield
    again
;

: room66.act11 ( -- )   \ 0041F030
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    begin
        loader-done? while
        yield
    repeat
    8 3 $FF char-load
    3 char-unload
    $37 1.0 1 bgm
    wait-fade
    $19 state-flag-set
    $FF panic-stage? if
        3 panic-stage
    then
    $F state-flag-set
    0 $5D 75.0 -30.0 180 char-to-xz
    1 30.0 -10.0 0.0 4.0 event-camera
    $FF 1 char-visible
    0 0 $14 door-bits
    8 char-activate
    0 4 $20000 nav-group
    8 $196 75.0 -40.0 0 char-to-xz
    1 4 $20000 nav-group
    begin
        0 adx? not while
        yield
    repeat
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    9 0 var-set
    room66.cmd04
    self-frames-reset
    $A self-wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        room66.cmd04
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $7C $74 $78 $5A $44 $46 $52 $4B 0 9 effect-string
    0 1 $14 door-bits
    8 action-end
    8 char-done
    3 0 char-remove
    $FF 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    $F state-flag-clear
    $19 state-flag-clear
    $F $41 fade
    wait-fade
    $B ebit? not if
        $1D01 self-anim
        8 message
        wait-message
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 $84 5 char-sound
        $B ebit-set
        self-frames-reset
        $1E self-wait-frames
    else
        9 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room66.act12 ( -- )   \ 0041F140
    self-wait-done
    12.0 -41.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room66.act13 ( -- )   \ 0041F160
    self-wait-done
    -19.0 -6.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room66.act14 ( -- )   \ 0041F180
    self-wait-done
    -60.0 -110.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room66.act15 ( -- )   \ 0041F1A0
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 45 $3C char-heading? if
        $F00 self-anim
    else 0 -45 $3C char-heading? if
        $F01 self-anim
    else 0 90 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: room66.act16 ( -- )   \ 0041F1E0
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 0 $3C char-heading? if
        $F00 self-anim
    else 0 90 $3C char-heading? if
        $F01 self-anim
    else 0 45 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: room66.act17 ( -- )   \ 0041F220
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 45 $3C char-heading? if
        $F00 self-anim
    else 0 -45 $3C char-heading? if
        $F01 self-anim
    else 0 90 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: room66.act18 ( -- )   \ 0041F260
    self-wait-done
    2.06 -99.68 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $271 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: room66.act19 ( -- )   \ 0041F2C0
    self-wait-done
    -138.94 -114.31 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $2AA story-flag-set
            1 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: room66.act1A ( -- )   \ 0041F320
    self-wait-done
    39.98 -40.67 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $75 message-param-room
        $75 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C3 story-flag-set
            2 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
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

: room66.act1B ( -- )   \ 0041F380
    self-wait-done
    0 0 room66.cmd01
    1 0 room66.cmd01
    2 0 room66.cmd01
    3 0 room66.cmd01
    3 117.5 11.7 -21.8 0 effect-86
    4 -217.0 20.1 20.0 1 effect-86
    5 -175.0 20.1 4.7 1 effect-86
    6 -70.0 20.1 -124.7 1 effect-86
    7 -20.0 20.1 -96.7 1 effect-86
    8 11.7 20.1 -55.8 1 effect-86
    9 43.0 20.1 -26.5 1 effect-86
    $A -33.2 20.1 -6.2 1 effect-86
    $B 89.3 20.1 50.0 1 effect-86
    $C -12.4 28.3 24.6 1 effect-86
    $D -40.0 20.1 95.6 1 effect-86
    $E -65.5 20.1 0.0 1 effect-86
    8 3 $FF char-load
    3 char-unload
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
    $28 $78 movie-param
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

: room66.phase5 ( -- )   \ 0047ACC8
;

' room66.enter $66 0 room-script!
' room66.char-enter $66 6 room-script!
' room66.phase1 $66 1 room-script!
' room66.phase2 $66 2 room-script!
' room66.act00 $66 $00 action-script!
' room66.act01 $66 $01 action-script!
' room66.act02 $66 $02 action-script!
' room66.act03 $66 $03 action-script!
' room66.act04 $66 $04 action-script!
' room66.act05 $66 $05 action-script!
' room66.act06 $66 $06 action-script!
' room66.act07 $66 $07 action-script!
' room66.act08 $66 $08 action-script!
' room66.act09 $66 $09 action-script!
' room66.act0A $66 $0A action-script!
' room66.act0B $66 $0B action-script!
' room66.act0C $66 $0C action-script!
' room66.act0D $66 $0D action-script!
' room66.act0E $66 $0E action-script!
' room66.act0F $66 $0F action-script!
' room66.act10 $66 $10 action-script!
' room66.act11 $66 $11 action-script!
' room66.act12 $66 $12 action-script!
' room66.act13 $66 $13 action-script!
' room66.act14 $66 $14 action-script!
' room66.act15 $66 $15 action-script!
' room66.act16 $66 $16 action-script!
' room66.act17 $66 $17 action-script!
' room66.act18 $66 $18 action-script!
' room66.act19 $66 $19 action-script!
' room66.act1A $66 $1A action-script!
' room66.act1B $66 $1B action-script!
' room66.phase5 $66 5 room-script!

\ ---- room $68 ----------------------------------------------------------------------------------

: room68.enter ( -- )   \ 00424880
    3 1.0 0 bgm
    0 exit-taken? if
        $11 3 $FF char-load
    then
;

: room68.char-enter ( -- )   \ 00424890
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
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
;

: room68.phase1 ( -- )   \ 00424990
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        $76 story-flag? not if
            3 stalker-kind? $22 stalker-kind? or if
            then
        then
        2 exit-prepare
    then
    0 7 char-entered-area? if
        $76 story-flag? not if
            3 stalker-kind? $22 stalker-kind? or if
                $FE 0 char-file-load
            then
        then
        3 exit-prepare
    then
    0 -26.02 2.5 -26.23 $14 6 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    0 -26.02 2.5 -26.23 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 26.56 2.5 25.45 $14 6 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    2 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 26.56 2.5 25.45 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: room68.phase2 ( -- )   \ 00424AC0
    0 8 char-in-area? 0 8 $3C char-faces-area? and if
        5 0 0 scene-change
    then
    0 9 char-in-area? 0 9 $3C char-faces-area? and if
        5 0 0 scene-change
    then
    0 $A char-in-area? 0 $A $3C char-faces-area? and if
        5 0 0 scene-change
    then
;

: room68.phase4 ( -- )   \ 00424AF0
    0 0 char-in-area? if
        3 0 char-remove
    then
;

: room68.act00 ( -- )   \ 00424B00
    self-wait-done
    0 8 char-in-area? if
        6.92 -26.68 self-turn-to-xz
        self-wait-done
    else 0 9 char-in-area? if
        -20.57 -19.02 self-turn-to-xz
        self-wait-done
    else 0 $A char-in-area? if
        -26.62 7.92 self-turn-to-xz
        self-wait-done
    then then then
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $332 story-flag? not if
        $332 story-flag-set
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

' room68.enter $68 0 room-script!
' room68.char-enter $68 6 room-script!
' room68.phase1 $68 1 room-script!
' room68.phase2 $68 2 room-script!
' room68.phase4 $68 4 room-script!
' room68.act00 $68 $00 action-script!

\ ---- room $69 ----------------------------------------------------------------------------------

\ room 0x69 (D_004386F8): the three-dial lock by byte 3 - 0 the dials (and their lit twins) set
\ to their settings; 1 the player at it: up / down pick the dial (event variable 0; its lit twin
\ shown), left / right turn it (its setting, sound bit 3), cancel leaves (event bit 2); 2 the
\ picked dial turns 4 degrees a frame to its setting, then - 1, 0, 2 - the lock opens (event
\ bits 2 off, 4)
: room69.cmd00 ( b0 -- )  drop s" room69.cmd00" stub-step ;

: room69.enter ( -- )   \ 00437E50
    room-sounds
    0 room69.cmd00
    3 1 object-show
    4 1 object-show
    5 1 object-show
    $2AF story-flag? $2B0 story-flag? not and if
        0 82.73 31.0 -85.2 flicker-sprite
    then
    1 -31.0 51.9 -132.6 1 effect-86
    2 0.0 51.9 -142.7 1 effect-86
    3 30.9 51.8 -132.6 1 effect-86
    4 50.0 51.9 -106.3 1 effect-86
    5 -50.0 51.9 -106.3 1 effect-86
    6 82.0 51.9 -62.7 1 effect-86
    7 160.0 51.9 -62.7 1 effect-86
    8 215.7 21.9 -62.6 1 effect-86
    $2CC story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $2CD story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    0 $C 0.812 0.687 0.187 0.312 zone-rect
    1 $3FFF sound-volume
;

: room69.char-enter ( -- )   \ 00437F50
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    0 3 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
;

: room69.phase1 ( -- )   \ 00437FD0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 2 1 1 chars-area-camera
    5 3 -1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 1 1 chars-area-camera
    8 0 0 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $2AF story-flag? not if
        2 82.73 30.0 -85.2 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 687 var-set
                $1A 688 var-set
                $1B 0 var-set
                $1C 82730 var-set
                $1D 31000 var-set
                $1E -85200 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    3 -103.36 30.0 -72.17 $19 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 288.99 0.0 -98.66 $19 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $2CC story-flag? not if
        0 -114.0 30.0 -48.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $2CC story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -114.0 30.0 -48.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -114.0 30.0 -48.0 0 0 sound
            $40 $DD noise
        then
    then
    $2CD story-flag? not if
        1 -114.0 30.0 -42.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $2CD story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -114.0 30.0 -42.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -114.0 30.0 -42.0 0 0 sound
            $40 $DF noise
        then
    then
    6 sound-bank-loaded? if
        3 var-inc
        3 30 var? if
            3 0 var-set
            2 0 var? if
                $40000005 6 74.0 30.0 -110.0 0 0 sound
            else 2 1 var? if
                $40000006 6 74.0 30.0 -110.0 0 0 sound
            else 2 2 var? if
                $40000007 6 74.0 30.0 -110.0 0 0 sound
            else 2 3 var? if
                $40000008 6 74.0 30.0 -110.0 0 0 sound
            then then then then
            2 var-inc
            2 4 var? if
                2 0 var-set
            then
        then
    then
;

: room69.phase2 ( -- )   \ 00438230
    0 $A $32 char-faces-area? if
        5 2 0 scene-change
    then
    0 $B char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 $C char-in-area? 0 22 $3C char-heading? and if
        5 6 0 scene-change
    then
    -2147483646 scene-request? 0 0 char-group-bit4? and if
        5 1 1 scene-change
    then
    0 $D char-in-area? 0 50 $32 char-heading? and if
        5 $84 3 scene-change
    then
    $2AF story-flag? $2B0 story-flag? not and if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: room69.phase3 ( -- )   \ 00438290
    251.4 20.0 -115.0 251.4 20.0 -90.0 251.4 0.0 -115.0 251.4 0.0 -90.0 lights-doorway
;

: room69.act00 ( -- )   \ 004382D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 4 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message
    wait-message
    $902 self-anim
    self-wait-anim
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room69.act01 ( -- )   \ 00438340
    self-wait-done
    0 self-through-exit
    self-wait-done
    5 ebit? not 2 game-mode? or $FF panic-stage? or if
        $FF panic-stage? 2 game-mode? or if
            $60A self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $608 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            9 message
            wait-message
        then
        5 ebit-set
    else
        $800 self-anim
        self-wait-anim
        $801 self-anim
        $A message
        wait-message
        $802 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        5 ebit-clear
    then
    self-idle-or-end
;

: room69.act02 ( -- )   \ 00438390
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $54 story-flag? not if
        1 message
        wait-message
        0 answer? if
            0 $4B 293.136 -95.0 180 char-to-xz
            $17 state-flag-set
            1 5.0 0.0 0.0 2.0 event-camera
            2 message
            2 ebit-set
            0 0 var-set
            0 1 object-show
            3 0 object-show
            begin
                3 ebit? not if
                    1 room69.cmd00
                    3 ebit? if
                        1 6 293.0 12.0 -102.0 0 0 sound
                    then
                    6 ebit? if
                        0 6 293.0 12.0 -102.0 0 0 sound
                    then
                else
                    2 room69.cmd00
                then
                2 ebit? while
                yield
            repeat
            2 message-close
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
            0 0 object-show
            1 0 object-show
            2 0 object-show
            3 1 object-show
            4 1 object-show
            5 1 object-show
            4 ebit? if
                2 6 318.0 12.0 -80.0 0 0 sound
                1 $FF $D2 rumble
                1 self-anim
                self-wait-anim
                0 self-anim
                self-frames-reset
                $32 self-wait-frames
                318.0 10.0 -80.0 self-look-at-point
                yield
                $72 5 318.0 12.0 -80.0 0 0 sound
                $74 door-unlock
                $62 door-unlock
                $54 story-flag-set
                self-frames-reset
                $1E self-wait-frames
                0 3 6 char-sound
                self-frames-reset
                $5A self-wait-frames
                $FF self-look-at
                yield
            else $333 story-flag? if
                $23B item-give
            then then
        else $333 story-flag? if
            $23B item-give
        then then
    else
        7 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room69.act03 ( -- )   \ 004384E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 ebit? not if
        $F 6 fade
        wait-fade
        3 message
        wait-message
        1 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        1 ebit-clear
    then
    $244 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room69.act04 ( -- )   \ 00438530
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 4 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    5 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room69.act05 ( -- )   \ 00438650
    self-wait-done
    82.73 -85.2 self-turn-to-xz
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
            $2B0 story-flag-set
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

: room69.act06 ( -- )   \ 004386B0
    self-wait-done
    -69.5 -46.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room69.phase5 ( -- )   \ 0047AE7C
;

' room69.enter $69 0 room-script!
' room69.char-enter $69 6 room-script!
' room69.phase1 $69 1 room-script!
' room69.phase2 $69 2 room-script!
' room69.phase3 $69 3 room-script!
' room69.act00 $69 $00 action-script!
' room69.act01 $69 $01 action-script!
' room69.act02 $69 $02 action-script!
' room69.act03 $69 $03 action-script!
' room69.act04 $69 $04 action-script!
' room69.act05 $69 $05 action-script!
' room69.act06 $69 $06 action-script!
' room69.phase5 $69 5 room-script!

\ ---- room $6A ----------------------------------------------------------------------------------

\ room 0x6A (D_00438CE0): a lit quad at x 120, z 47 .. 39, from 4 to 21
: room6A.cmd00 ( b0 -- )  drop s" room6A.cmd00" stub-step ;
\ room 0x6A (D_00438CF0): its object's animation (+0x74 forward, +0x78 back) at a point +0x7C
\ (0..1) by byte 3 - 0 / 1 from event variable 0 (12..30 over 18, 12..28 over 16), 2 / 3 at the
\ start / end
: room6A.cmd01 ( b0 -- )  drop s" room6A.cmd01" stub-step ;

: room6A.enter ( -- )   \ 00438730
    room-sounds
    2 0 object-show
    $32D story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
        $32E story-flag? not if
            3 room6A.cmd01
            1 room6A.cmd00
        else
            2 room6A.cmd01
            0 room6A.cmd00
        then
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 room6A.cmd00
        1 0 $2008000 nav-group
    then
    $2AD story-flag? $2AE story-flag? not and if
        0 88.5 1.0 71.56 flicker-sprite
    then
    1 $3FFF sound-volume
    0 9 0.4 0.0 0.2 0.5 zone-rect
;

: room6A.char-enter ( -- )   \ 004387A0
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
        $80 exit-taken? if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            0 0 0 action
            1 $59 char-in-room? if
                $6A 0 55 hewie-to-room
            then
        then
    then
;

: room6A.phase1 ( -- )   \ 004388C0
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
    $A 2 2 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        2 exit-prepare
    then
    0 5 char-entered-area? 0 6 char-entered-area? or if
        0 exit-prepare
    then
    0 7 char-entered-area? 0 8 char-entered-area? or if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        3 0 char-remove
    then
    0 7 char-entered-area? if
        $1C 3 1 char-load
    then
    0 8 char-entered-area? if
        $56 story-flag? not $7B story-flag? not and if
            4 0 char-remove
        then
    then
    0 9 char-entered-area? if
        $56 story-flag? not $7B story-flag? not and if
            $21 4 3 char-load
        then
        3 exit-prepare
    then
    1 ebit? not if
        $32D story-flag? not $32E story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                $FE $E char-entered-area? $FE 0 char-action? and if
                    0 $FE 2 action
                then
            then
        then
    then
    $2AD story-flag? not if
        0 88.5 0.0 71.56 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 685 var-set
                $1A 686 var-set
                $1B 0 var-set
                $1C 88500 var-set
                $1D 1000 var-set
                $1E 71560 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    0 ebit? if
        $FE char-here? if
            0 ebit-clear
        then
    then
;

: room6A.phase2 ( -- )   \ 004389F0
    0 exit-door-open? not 0 ebit? and if
        0 0 char-group-bit4? if
            0 scene-ending
        then
    then
    2 ebit? not if
        0 $E char-in-area? 0 45 $3C char-heading? and if
            $32D story-flag? not if
                $FE char-here? not if
                    5 1 5 scene-change
                else $FE $E char-in-area? $FE 0 40 chars-within? or if
                    $8016 scene-ending
                else
                    5 1 5 scene-change
                then then
            else
                5 3 0 scene-change
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        $FE char-busy? not if
            2 ebit-clear
        then
    else
        2 ebit-clear
    then then
    $2AD story-flag? $2AE story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: room6A.phase5 ( -- )   \ 00438A70
    2 ebit? if
        $32E story-flag-set
        $32D story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            $FE 1 stalker-rage
        then
    then
;

: room6A.act00 ( -- )   \ 00438A90
    1 self-scripted
    0 ebit-set
    $F $41 fade
    0 $28 5 char-sound
    self-wait-done
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room6A.act01 ( -- )   \ 00438AB0
    1 self-scripted
    1 ebit-set
    self-wait-done
    0 0 char-file-load
    $F8 114.64 43.0 90 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $32E story-flag? not if
        0 room6A.cmd00
        0 0 var-set
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            0 room6A.cmd01
            yield
            0 12 var? if
                0 0 6 char-sound
            then
            0 30 var? not while
            0 var-inc
        repeat
        self-wait-anim
        2 room6A.cmd01
        $32E story-flag-set
    else
        0 0 var-set
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            1 room6A.cmd01
            yield
            0 12 var? if
                0 0 6 char-sound
            then
            0 28 var? not while
            0 var-inc
        repeat
        self-wait-anim
        3 room6A.cmd01
        $32E story-flag-clear
        1 room6A.cmd00
    then
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: room6A.act02 ( -- )   \ 00438B60
    2 ebit-set
    self-wait-done
    $FE 1 char-file-load
    $F8 114.64 43.0 90 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 5 6 char-sound
    $8000 5 self-anim-blend
    self-wait-anim
    $E00 5 self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    $FE 1 6 char-sound
    $32D story-flag-set
    $FE 1 stalker-rage
    0 0 $14 door-bits
    1 1 $14 door-bits
    1 0 $2008000 nav-group
    1 room6A.cmd00
    120.0 16.0 45.0 0 943208504 4 -15.5 scene-effect-8C
    120.0 16.0 41.0 0 943208504 4 -15.5 scene-effect-8C
    120.0 10.0 45.0 0 943208504 4 -9.5 scene-effect-8C
    120.0 10.0 41.0 0 943208504 4 -9.5 scene-effect-8C
    120.0 4.0 45.0 0 943208504 4 -3.5 scene-effect-8C
    120.0 4.0 41.0 0 943208504 4 -3.5 scene-effect-8C
    self-wait-anim
    2 ebit-clear
    self-idle-or-end
;

: room6A.act03 ( -- )   \ 00438C48
    self-wait-done
    90 self-turn-angle
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: room6A.act04 ( -- )   \ 00438C60
    self-wait-done
    88.5 71.56 self-turn-to-xz
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
            $2AE story-flag-set
            0 effect-remove
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

' room6A.enter $6A 0 room-script!
' room6A.char-enter $6A 6 room-script!
' room6A.phase1 $6A 1 room-script!
' room6A.phase2 $6A 2 room-script!
' room6A.phase5 $6A 5 room-script!
' room6A.act00 $6A $00 action-script!
' room6A.act01 $6A $01 action-script!
' room6A.act02 $6A $02 action-script!
' room6A.act03 $6A $03 action-script!
' room6A.act04 $6A $04 action-script!

\ ---- room $6B ----------------------------------------------------------------------------------

: room6B.enter ( -- )   \ 00438D30
    room-sounds
    $56 story-flag? if
        $7C story-flag? not if
            $7C story-flag-set
            $6B 0 109 $80 5 -1 $B4 0.0 creature-place
        then
    then
    1 $2300 sound-volume
;

: room6B.act06 ( -- )   \ 00439080
    5 ebit-set
    4 ebit-clear
    $13 0 pvar? if
        $A chance? if
            4 ebit-set
        then
    else $13 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $13 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $13 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    2 creature-action? if
        4 ebit-set
    then
    4 ebit? if
        0 $FE 7 action
    else
        $78 1 item-cooldown
    then
    $13 pvar-inc
    exit
;

: room6B.char-enter ( -- )   \ 00438D60
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
    0 self-is? if
        $FE exit-taken? if
            0 $14 0.0 -110.0 0 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 1 action
        then
    then
    $FE self-is? if
        9 state-flag? if
            5 ebit-set
            room6B.act06
        else
            5 ebit-clear
        then
    then
;

: room6B.phase1 ( -- )   \ 00438E20
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 3 char-entered-area? if
        $1C 3 1 char-load
    then
    0 2 char-entered-area? if
        3 0 char-remove
    then
    2 0.75 0.0 -116.99 $14 10 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    2 0.75 0.0 -116.99 $14 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 10.0 hewie-look-zone
            then
        then
    then
    7 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 0.0 9.0 -125.0 0 0 sound
            7 ebit-set
        then
    else
        $C0000008 6 0.0 9.0 -125.0 0 0 sound
    then
;

: room6B.phase2 ( -- )   \ 00438F10
    0 7 char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 4 5 scene-change
        else
            $8016 scene-ending
        then
    then
;

: room6B.act00 ( -- )   \ 00438F40
    $18 state-flag-set
    1 self-scripted
    $27 room-preload
    3 0 char-remove
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    0 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        8 state-flag-set
        $FE exit-check
    else
        1 exit-prepare
        $1C 3 1 char-load
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room6B.act01 ( -- )   \ 00438F80
    1 self-scripted
    1 exit-prepare
    $1C 3 1 char-load
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    $F 1 fade
    wait-fade
    8 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room6B.act02 ( -- )   \ 0047AE88
    self-idle-or-end
;

: room6B.act03 ( -- )   \ 0047AE8C
    self-idle-or-end
;

: room6B.act05 ( -- )   \ 00439050
    3 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    $FF 0 char-visible
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

: room6B.act04 ( -- )   \ 00438FA0
    $18 state-flag-set
    5 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $31 $8004 5 82.92 -88.12 180 self-walk-anim
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
    $FF 2 -1 char-camera
    $FF 1 char-visible
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room6B.act05 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? if
                    $FE char-here? not if
                        5 ebit-clear
                        1 avoid-prompt
                    then
                then
                yield
            then
        else
            $FE char-here? if
                ['] room6B.act05 goto
            then
            0 camera-follow
            $FF 0 char-visible
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

: room6B.act07 ( -- )   \ 004390D0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $4F 83.66 -76.9 180 $FFFF 5 self-move-to
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
    3 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room6B.phase5 ( -- )   \ 0047AE84
;

' room6B.enter $6B 0 room-script!
' room6B.char-enter $6B 6 room-script!
' room6B.phase1 $6B 1 room-script!
' room6B.phase2 $6B 2 room-script!
' room6B.act00 $6B $00 action-script!
' room6B.act01 $6B $01 action-script!
' room6B.act02 $6B $02 action-script!
' room6B.act03 $6B $03 action-script!
' room6B.act04 $6B $04 action-script!
' room6B.act05 $6B $05 action-script!
' room6B.act06 $6B $06 action-script!
' room6B.act07 $6B $07 action-script!
' room6B.phase5 $6B 5 room-script!

\ ---- room $6C ----------------------------------------------------------------------------------

: room6C.enter ( -- )   \ 00439160
    room-sounds
    $1C $18C char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 12.0 0.0 0.0 -90 char-to-xyz
        $1C $900C 1 0 char-anim-hold
    then
    0 $F1 3 action
    $22 state-flag-set
    $7E story-flag? not if
        0 1 $14 door-bits
        1 0 $20000 nav-group
    else
        1 1 $14 door-bits
    then
    $27E story-flag? not if
        0 12.3 8.5 177.61 flicker-sprite
    then
    $2B3 story-flag? $2B4 story-flag? not and if
        1 0.34 1.0 62.0 flicker-sprite
    then
    $2B5 story-flag? $2B6 story-flag? not and if
        2 13.99 1.0 94.5 flicker-sprite
    then
    $2B7 story-flag? $2B8 story-flag? not and if
        3 -13.99 1.0 126.99 flicker-sprite
    then
    $2B9 story-flag? $2BA story-flag? not and if
        4 -42.23 51.0 236.98 flicker-sprite
    then
    1 $2300 sound-volume
;

: room6C.char-enter ( -- )   \ 00439220
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    2 3 3 area-camera
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
        2 exit-taken? if
            2 map-page
        then
    then
;

: room6C.phase1 ( -- )   \ 00439320
    $82 story-flag? if
        0 exit-usable? if
            0 exit-check
        then
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
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 3 3 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    0 4 char-entered-area? 0 6 char-entered-area? or if
        3 exit-prepare
    then
    0 5 char-entered-area? 0 8 char-entered-area? or if
        0 exit-prepare
    then
    0 7 char-entered-area? if
        1 exit-prepare
    then
    0 9 char-entered-area? if
        2 exit-prepare
    then
    0 $11 char-entered-area? if
        2 map-page
    then
    0 $11 char-left-area? if
        1 map-page
    then
    $7E story-flag? not if
        0 -50.0 50.0 355.0 6 12 0 zone
        0 0 char-in-zone? if
            0 5 6 char-sound
            0 0 $14 door-bits
            1 1 $14 door-bits
            0 0 $20000 nav-group
            $64 door-unlock
            $7E story-flag-set
            0 6 0.25 0.5 0.25 0.5 zone-rect
            -53.0 52.0 357.0 0 -2144325584 2 0.0 scene-effect-8C
            -50.0 50.0 357.0 0 -2143272896 2 0.0 scene-effect-8C
            -47.0 50.0 357.0 0 -2144325584 2 0.0 scene-effect-8C
            0 -55.0 55.0 357.0 0 0 0 0 dust
            0 -50.0 52.0 357.0 0 0 0 0 dust
            0 -45.0 54.0 357.0 0 0 0 0 dust
        then
    then
    1 12.0 0.0 0.0 5 10 1 zone
    $FF 1 char-in-zone? if
        2 ebit-set
    else
        2 ebit-clear
    then
    1 0 var? not if
        2 var-inc
        2 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            2 0 var-set
        then
    then
    2 11.58 0.0 -0.41 $17 16 0 zone
    7 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    7 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 11.58 0.0 -0.41 $17 16 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $2B3 story-flag? not if
        5 0.34 0.0 62.0 $A 5 0 zone
        1 5 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 691 var-set
                $1A 692 var-set
                $1B 1 var-set
                $1C 340 var-set
                $1D 1000 var-set
                $1E 62000 var-set
                $1F 5 var-set
                0 1 $8B action
            then
        then
    then
    $2B5 story-flag? not if
        6 13.99 0.0 94.5 $A 5 0 zone
        1 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 693 var-set
                $1A 694 var-set
                $1B 2 var-set
                $1C 13990 var-set
                $1D 1000 var-set
                $1E 94500 var-set
                $1F 6 var-set
                0 1 $8B action
            then
        then
    then
    $2B7 story-flag? not if
        7 -13.99 0.0 126.99 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 695 var-set
                $1A 696 var-set
                $1B 3 var-set
                $1C -13990 var-set
                $1D 1000 var-set
                $1E 126990 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
    $2B9 story-flag? not if
        8 -42.23 50.0 236.98 $A 5 0 zone
        1 8 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 697 var-set
                $1A 698 var-set
                $1B 4 var-set
                $1C -42230 var-set
                $1D 51000 var-set
                $1E 236980 var-set
                $1F 8 var-set
                0 1 $8B action
            then
        then
    then
    $56 story-flag? $82 story-flag? not and 9 ebit? not and if
        0 0 char-entered-area? if
            9 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 9 action-force
            else
                1 0 9 action-force
            then
        then
    then
;

: room6C.phase2 ( -- )   \ 004396D0
    $7E story-flag? not if
        0 0 2 char-zone-bits? 0 0 $32 char-heading? and if
            5 2 0 scene-change
        then
    then
    0 $10 $3C char-faces-area? if
        5 $A 0 scene-change
    then
    $27E story-flag? not if
        4 0 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $2B3 story-flag? $2B4 story-flag? not and if
        5 1 5 5 0 zone-at-effect
        0 5 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    $2B5 story-flag? $2B6 story-flag? not and if
        6 2 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
    $2B7 story-flag? $2B8 story-flag? not and if
        7 3 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    $2B9 story-flag? $2BA story-flag? not and if
        8 4 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then
;

: room6C.act00 ( -- )   \ 0047AEA0
    self-idle-or-end
;

: room6C.act01 ( -- )   \ 0047AEA4
    self-idle-or-end
;

: room6C.act02 ( -- )   \ 00439780
    self-wait-done
    0 self-turn-angle
    self-wait-done
    1 ebit? not if
        2 message
        wait-message
        1 ebit-set
    else
        3 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: room6C.act03 ( -- )   \ 004397A0
    1 0 var-set
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 12.0 0.0 0.0 -90 char-to-xyz
        $1C $900C 1 0 char-anim-hold
    then
    begin
        2 ebit? if
            1 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            $1C 1 6 char-sound
            0 $8F 5 char-sound
            $1C $900D 0 3 char-anim-hold
            $1C wait-char-anim
            0 5 var? if
                $1C $900E 0 3 char-anim-hold
                self-frames-reset
                $10 self-wait-frames
                0 0 var-set
                1 7.0 11.0 0.0 5 10 0 zone
                0 1 3 char-zone-bits? if
                    $1C 0 6 char-sound
                    $1C 1 fiona-thrown
                    3 panic-stage? if
                        4 panic-stage
                    else
                        3 panic-stage
                    then
                then
                $1C wait-char-anim
            then
            $1C $900C 0 3 char-anim-hold
            1 0 var-set
        then
        $1C char-at-motion-event? if
            $1C $900C 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: room6C.act04 ( -- )   \ 00439880
    self-wait-done
    12.3 177.61 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $9B message-param-room
        $9B $63 item-count? if
            $8010 message
            wait-message
        else
            $27E story-flag-set
            0 effect-remove
            $9B 1 item-give-count
            0 $9B item-tab
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

: room6C.act05 ( -- )   \ 004398E0
    self-wait-done
    0.34 62.0 self-turn-to-xz
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
            $2B4 story-flag-set
            1 effect-remove
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

: room6C.act06 ( -- )   \ 00439940
    self-wait-done
    13.99 94.5 self-turn-to-xz
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
            $2B6 story-flag-set
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

: room6C.act07 ( -- )   \ 004399A0
    self-wait-done
    -13.99 126.99 self-turn-to-xz
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
            $2B8 story-flag-set
            3 effect-remove
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

: room6C.act08 ( -- )   \ 00439A00
    self-wait-done
    -42.23 236.98 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $98 message-param-room
        $98 $63 item-count? if
            $8010 message
            wait-message
        else
            $2BA story-flag-set
            4 effect-remove
            $98 1 item-give-count
            0 $98 item-tab
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

: room6C.act09 ( -- )   \ 00439A60
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    wait-fade
    8 state-flag-set
    $12 state-flag-set
    1 action-end
    1 char-done
    0 exit-prepare
    $18 state-flag-clear
    0 self-scripted
    $80 exit-check
    self-idle-or-end
;

: room6C.act0A ( -- )   \ 00439A90
    self-wait-done
    0.5 56.7 self-turn-to-xz
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room6C.phase5 ( -- )   \ 0047AE98
    $1C action-end
    $1C char-done
;

' room6C.enter $6C 0 room-script!
' room6C.char-enter $6C 6 room-script!
' room6C.phase1 $6C 1 room-script!
' room6C.phase2 $6C 2 room-script!
' room6C.act00 $6C $00 action-script!
' room6C.act01 $6C $01 action-script!
' room6C.act02 $6C $02 action-script!
' room6C.act03 $6C $03 action-script!
' room6C.act04 $6C $04 action-script!
' room6C.act05 $6C $05 action-script!
' room6C.act06 $6C $06 action-script!
' room6C.act07 $6C $07 action-script!
' room6C.act08 $6C $08 action-script!
' room6C.act09 $6C $09 action-script!
' room6C.act0A $6C $0A action-script!
' room6C.phase5 $6C 5 room-script!

\ ---- room $6D ----------------------------------------------------------------------------------

: room6D.enter ( -- )   \ 00439B10
    $21E story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $28E story-flag? not if
            0 20.0 1.0 -72.5 flicker-sprite
        then
    then
    $2CE story-flag? not if
        1 3 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 3 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $2CF story-flag? not if
        1 2 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    $2D0 story-flag? not if
        1 1 $8000000 nav-group
        6 1 $14 door-bits
        7 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        6 0 $14 door-bits
        7 1 $14 door-bits
    then
    0 6 0.345 0.687 0.187 0.312 zone-rect
    $56 story-flag? if
        $7D story-flag? not if
            $7D story-flag-set
            $6D 0 65 $80 3 -1 $FF80 44.0 creature-place
            $6D 0 36 $80 3 -1 $81 44.0 creature-place
        then
    then
    1 $2300 sound-volume
;

: room6D.char-enter ( -- )   \ 00439C00
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
    then then
    0 2 1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    1 3 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    2 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    3 1 0 area-camera
    0 self-is? if
        3 exit-taken? if
            2 map-page
        then
    then
;

: room6D.phase1 ( -- )   \ 00439D00
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
    $A 0 -1 1 chars-area-camera
    $B 1 0 1 chars-area-camera
    $C 0 -1 1 chars-area-camera
    $D 2 1 1 chars-area-camera
    $E 2 1 1 chars-area-camera
    $F 3 -1 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? 0 7 char-entered-area? or 0 8 char-entered-area? or if
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 9 char-entered-area? if
        2 map-page
    then
    0 9 char-left-area? if
        1 map-page
    then
    0 6 char-entered-area? if
        $1C 3 1 char-load
    then
    0 7 char-entered-area? if
        3 0 char-remove
    then
    0 4 char-entered-area? if
        $1C 3 0 char-load
    then
    0 5 char-entered-area? if
        3 0 char-remove
    then
    1 19.93 0.0 -73.33 $D 10 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    $2CE story-flag? not if
        2 6.4 0.0 -132.4 5 8 1 zone
        $FF 2 char-in-zone? if
            $2CE story-flag-set
            0 3 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            6.4 0.0 -132.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 6.4 0.0 -132.4 0 0 sound
            $40 $100 noise
        then
    then
    $2CF story-flag? not if
        3 -63.9 0.0 -147.4 5 8 1 zone
        $FF 3 char-in-zone? if
            $2CF story-flag-set
            0 2 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -63.9 0.0 -147.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -63.9 0.0 -147.4 0 0 sound
            $40 $112 noise
        then
    then
    $2D0 story-flag? not if
        4 -68.8 0.0 -156.4 5 8 1 zone
        $FF 4 char-in-zone? if
            $2D0 story-flag-set
            0 1 $8000000 nav-group
            6 0 $14 door-bits
            7 1 $14 door-bits
            -68.8 0.0 -156.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -68.8 0.0 -156.4 0 0 sound
            $40 $115 noise
        then
    then
;

: room6D.phase2 ( -- )   \ 00439F10
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    $21E story-flag? not if
        0 20.0 0.0 -72.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $21E story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            20.0 0.0 -72.5 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 20.0 0.0 -72.5 0 0 sound
            $40 $35 noise
            0 20.0 1.0 -72.5 flicker-sprite
        then
    else $28E story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
;

: room6D.phase3 ( -- )   \ 00439FB0
    38.0 10.0 -99.0 68.0 30.0 -99.0 38.0 0.0 -99.0 68.0 0.0 -99.0 lights-doorway
    68.0 30.0 -99.0 90.0 50.0 -99.0 68.0 0.0 -99.0 90.0 0.0 -99.0 lights-doorway
;

: room6D.act00 ( -- )   \ 0043A020
    self-wait-done
    $FE self-touching? not if
        $44 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $44 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $44 door-unlock
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

: room6D.act01 ( -- )   \ 0043A060
    self-wait-done
    20.0 -72.5 self-turn-to-xz
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
            $28E story-flag-set
            0 effect-remove
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

' room6D.enter $6D 0 room-script!
' room6D.char-enter $6D 6 room-script!
' room6D.phase1 $6D 1 room-script!
' room6D.phase2 $6D 2 room-script!
' room6D.phase3 $6D 3 room-script!
' room6D.act00 $6D $00 action-script!
' room6D.act01 $6D $01 action-script!

\ ---- room $6E ----------------------------------------------------------------------------------

: room6E.enter ( -- )   \ 0043A120
    room-sounds
    1 0 $1000000 nav-group
    1 0 $4000000 nav-group
    $325 story-flag? if
        0 1 $14 door-bits
        1 0 $20000 nav-group
        0 1 object-show
        1 1 object-show
    else
        0 0 $14 door-bits
        0 0 $20000 nav-group
        0 0 object-show
        1 0 object-show
    then
    $27F story-flag? not if
        0 147.17 11.0 86.89 flicker-sprite
    then
    1 -36.7 19.1 -34.8 1 effect-86
    2 -36.7 19.1 34.8 1 effect-86
    1 $2300 sound-volume
;

: room6E.act06 ( -- )   \ 0043A630
    4 ebit-set
    3 self-is? $22 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    $10 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $10 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $10 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $10 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    3 self-is? $22 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE 7 action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE 8 action
    then then then
    3 0 var-set
    $10 pvar-inc
    exit
;

: room6E.char-enter ( -- )   \ 0043A1A0
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
    $FE self-is? if
        9 state-flag? if
            4 ebit-set
            room6E.act06
        else
            4 ebit-clear
        then
    then
;

: room6E.phase1 ( -- )   \ 0043A230
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 2 char-entered-area? if
        $1C 3 0 char-load
    then
    0 3 char-entered-area? if
        3 0 char-remove
    then
;

: room6E.phase2 ( -- )   \ 0043A280
    0 6 char-in-area? 0 45 $3C char-heading? and if
        $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $27F story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
;

: room6E.phase3 ( -- )   \ 0043A2B0
    -4.5 15.0 110.0 -4.5 15.0 79.8 -4.5 -15.0 110.0 -4.5 -15.0 79.8 lights-doorway
    -3.0 3.0 92.5 -33.6 3.0 86.4 -3.0 -20.0 92.5 -33.6 -20.0 86.4 lights-doorway
    149.4 25.0 59.1 159.4 25.0 59.1 149.4 0.0 59.1 159.4 0.0 59.1 lights-doorway
;

: room6E.act00 ( -- )   \ 0047AEB8
    self-idle-or-end
;

: room6E.act02 ( -- )   \ 0043A4E0
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $325 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room6E.act03 ( -- )   \ 0043A510
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    $FE char-here? if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 2 object-anim
    1 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 1 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $325 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room6E.act04 ( -- )   \ 0043A560
    1 self-scripted
    counter-inc
    1 ebit-set
    0 camera-follow
    9 state-flag-clear
    $FE char-here? 0 0 var? and if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $325 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room6E.act01 ( -- )   \ 0043A350
    $18 state-flag-set
    4 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $325 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 5 action-force
        2 ebit-set
        1 3 char-file-load
    then
    0 2 char-file-load
    $FE 4 char-file-load
    $47 144.89 65.089 90 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    2 ebit? if
        1 self-look-at
        yield
        begin
            3 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    0 ebit? not if
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 0 object-anim
        1 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 1 object-anim
        1 1 object-anim
        self-frames-reset
        $E self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 0 $20000 nav-group
    $18 state-flag-clear
    9 state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] room6E.act02 goto
                else
                    ['] room6E.act03 goto
                then
            else $FF panic-stage? if
                ['] room6E.act04 goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            4 ebit? if
                $FE char-here? not if
                    4 ebit-clear
                    1 avoid-prompt
                then
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] room6E.act04 goto
            then
            counter-inc
            0 camera-follow
            9 state-flag-clear
            $FE char-here? if
                $FE 0 stalker-mode
                $FE action-end
            then
            1 self-noclip
            0 2 object-anim
            1 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            0 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            0 1 6 char-sound
            self-wait-anim
            0 self-noclip
            $FE char-here? if
                $FE self-look-at
                yield
                $FE self-turn-to
                self-wait-done
            then
            $325 story-flag-clear
            0 0 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room6E.act05 ( -- )   \ 0043A5B0
    1 self-scripted
    self-wait-done
    $47 140.97 60.42 90 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
        $8000 5 self-anim-9
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-wait-anim
    then
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                1 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    0 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            1 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                0 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room6E.act07 ( -- )   \ 0043A700
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $47 144.89 65.089 90 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $19 self-wait-frames
    $FE 5 6 char-sound
    $28 threat-raise
    self-frames-reset
    9 self-wait-frames
    $FE 5 6 char-sound
    $14 threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: room6E.act08 ( -- )   \ 0043A770
    self-wait-done
    $FE 2 char-heading-for? 2 exit-door-open? not and if
        2 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $47 144.89 65.089 90 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $4F 134.56 61.52 80 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    0 1 var-set
    3 self-is? $22 self-is? or if
        0 3 object-anim
        1 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $10 self-wait-frames
        0 2 var-set
        self-frames-reset
        $A self-wait-frames
        $FE 0 6 char-sound
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        9 state-flag? if
            2 avoid-prompt
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room6E.act09 ( -- )   \ 0043A820
    self-wait-done
    147.17 86.89 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $27F story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

' room6E.enter $6E 0 room-script!
' room6E.char-enter $6E 6 room-script!
' room6E.phase1 $6E 1 room-script!
' room6E.phase2 $6E 2 room-script!
' room6E.phase3 $6E 3 room-script!
' room6E.act00 $6E $00 action-script!
' room6E.act01 $6E $01 action-script!
' room6E.act02 $6E $02 action-script!
' room6E.act03 $6E $03 action-script!
' room6E.act04 $6E $04 action-script!
' room6E.act05 $6E $05 action-script!
' room6E.act06 $6E $06 action-script!
' room6E.act07 $6E $07 action-script!
' room6E.act08 $6E $08 action-script!
' room6E.act09 $6E $09 action-script!

\ ---- room $6F ----------------------------------------------------------------------------------

: room6F.enter ( -- )   \ 0043A8F0
    room-sounds
    0 0 var-set
    1 0 var-set
    $335 story-flag? not if
        0 1 $14 door-bits
    then
    $29F story-flag? $2A0 story-flag? not and if
        0 0.03 1.0 -85.54 flicker-sprite
    then
    $2A1 story-flag? $2A2 story-flag? not and if
        1 109.8 -9.0 -94.58 flicker-sprite
    then
    1 $2300 sound-volume
    1 $9D $10000000 nav-tri-flags
    1 $8D $10000000 nav-tri-flags
    1 $2B $10000000 nav-tri-flags
    1 $90 $10000000 nav-tri-flags
    1 $8A $10000000 nav-tri-flags
    1 $C3 $10000000 nav-tri-flags
    1 $87 $10000000 nav-tri-flags
    1 $7C $10000000 nav-tri-flags
    1 $BA $10000000 nav-tri-flags
    1 $9E $10000000 nav-tri-flags
    1 $8F $10000000 nav-tri-flags
    1 $2E $10000000 nav-tri-flags
    1 $92 $10000000 nav-tri-flags
    1 $8C $10000000 nav-tri-flags
    1 $C6 $10000000 nav-tri-flags
    1 $88 $10000000 nav-tri-flags
    1 $C2 $10000000 nav-tri-flags
    1 $86 $10000000 nav-tri-flags
    1 $B8 $10000000 nav-tri-flags
    1 $7A $10000000 nav-tri-flags
    1 $BE $10000000 nav-tri-flags
    1 $80 $10000000 nav-tri-flags
    1 $C0 $10000000 nav-tri-flags
    1 $84 $10000000 nav-tri-flags
    1 $B3 $10000000 nav-tri-flags
    1 $1E $10000000 nav-tri-flags
    1 $9F $10000000 nav-tri-flags
    1 $42 $10000000 nav-tri-flags
    1 $BD $10000000 nav-tri-flags
    1 $7F $10000000 nav-tri-flags
    1 $BB $10000000 nav-tri-flags
    1 $7D $10000000 nav-tri-flags
;

: room6F.act02 ( -- )   \ 0043AE10
    2 ebit-set
    1 ebit-clear
    $14 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $14 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $14 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $14 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 $FE 3 action
    else
        $78 1 item-cooldown
    then
    $14 pvar-inc
    exit
;

: room6F.char-enter ( -- )   \ 0043AA40
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
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
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            room6F.act02
        else
            2 ebit-clear
        then
    then
;

: room6F.phase1 ( -- )   \ 0043AB10
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
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    0 3 char-entered-area? 0 5 char-entered-area? or if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    $29F story-flag? not if
        0 0.03 0.0 -85.54 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 671 var-set
                $1A 672 var-set
                $1B 0 var-set
                $1C 30 var-set
                $1D 1000 var-set
                $1E -85540 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    $2A1 story-flag? not if
        1 109.8 -10.0 -94.58 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 673 var-set
                $1A 674 var-set
                $1B 1 var-set
                $1C 109800 var-set
                $1D -9000 var-set
                $1E -94580 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 79.45 0.0 -93.49 0 0 sound
            else 0 1 var? if
                $40000004 6 79.45 0.0 -93.49 0 0 sound
            else 0 2 var? if
                $40000005 6 79.45 0.0 -93.49 0 0 sound
            else 0 3 var? if
                $40000006 6 79.45 0.0 -93.49 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: room6F.phase2 ( -- )   \ 0043ACB0
    0 $B char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $C char-in-area? 0 -45 $3C char-heading? and if
        $FE char-here? not if
            5 0 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $335 story-flag? not if
        0 $D char-in-area? 0 45 $32 char-heading? and if
            5 6 4 scene-change
        then
    then
    $29F story-flag? $2A0 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $2A1 story-flag? $2A2 story-flag? not and if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: room6F.act01 ( -- )   \ 0043ADE0
    0 ebit? if
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
    0 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room6F.act00 ( -- )   \ 0043AD20
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $20 $8004 5 105.64 -103.65 -90 self-walk-anim
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
    0 ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 1 6 char-sound
    then
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room6F.act01 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? $FE char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room6F.act01 goto
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

: room6F.act03 ( -- )   \ 0043AE60
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $20 113.0 -103.65 -90 $FFFF 5 self-move-to
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
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room6F.act04 ( -- )   \ 0043AEC0
    self-wait-done
    0.03 -85.54 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $2A0 story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: room6F.act05 ( -- )   \ 0043AF20
    self-wait-done
    109.8 -94.58 self-turn-to-xz
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
            $2A2 story-flag-set
            1 effect-remove
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

: room6F.act06 ( -- )   \ 0043AF80
    self-wait-done
    127.0 -125.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $335 story-flag-set
    0 0 $14 door-bits
    $1E message-param-room
    $1E 1 item-give-count
    0 $1E item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room6F.phase5 ( -- )   \ 0047AEBC
;

' room6F.enter $6F 0 room-script!
' room6F.char-enter $6F 6 room-script!
' room6F.phase1 $6F 1 room-script!
' room6F.phase2 $6F 2 room-script!
' room6F.act00 $6F $00 action-script!
' room6F.act01 $6F $01 action-script!
' room6F.act02 $6F $02 action-script!
' room6F.act03 $6F $03 action-script!
' room6F.act04 $6F $04 action-script!
' room6F.act05 $6F $05 action-script!
' room6F.act06 $6F $06 action-script!
' room6F.phase5 $6F 5 room-script!

\ ---- room $70 ----------------------------------------------------------------------------------

defer room70.act02
: room70.enter ( -- )   \ 0043B020
    room-sounds
    $263 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $292 story-flag? not if
            0 -97.5 1.0 1.0 flicker-sprite
        then
    then
    0 8 0.812 0.687 0.187 0.312 zone-rect
    $76 story-flag? not if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                4 ebit-set
            then
        then
    then
    0 1 8 nav-group
    4 ebit? if
        1 exit-taken? not if
            $FE 2 char-file-load
        then
        7 state-flag-set
        $21 state-flag-set
        $23 state-flag-set
        $FE char-activate
        $FE 1 char-silent
        $FE $70 49 2 stalker-to-room
        0 $FE 4 action
    then
    $27D story-flag? not if
        1 -98.51 1.0 -12.98 flicker-sprite
    then
    1 $2300 sound-volume
;

: room70.char-enter ( -- )   \ 0043B0D0
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
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
;

: room70.phase1 ( -- )   \ 0043B1D0
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
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    0 4 char-entered-area? 0 6 char-entered-area? or if
        0 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    0 5 char-entered-area? if
        3 exit-prepare
    then
    $76 story-flag? not if
        1 -76.01 0.0 -50.35 $46 17 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        1 -76.01 0.0 -50.35 $46 17 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    $1F 1 var-set
                    $1F 17.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? if
        $FE $80000000 6 char-sound
        $FE $80000001 6 char-sound
    then
;

: room70.phase2 ( -- )   \ 0043B2C0
    4 ebit? if
        0 $FE $A 0 $2D char-touching-facing? if
            6 ebit? not if
                5 5 0 scene-change
            then
        then
    then
    0 $A char-in-area? 0 -90 $1E char-heading? and if
        5 9 0 scene-change
    then
    0 $B char-in-area? 0 -67 $3C char-heading? and if
        5 9 0 scene-change
    then
    $27D story-flag? not if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then
    $263 story-flag? not if
        0 -97.5 0.0 1.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $263 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -97.5 0.0 1.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -97.5 0.0 1.0 0 0 sound
            $40 $B4 noise
            0 -97.5 1.0 1.0 flicker-sprite
        then
    else $292 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: room70.phase5 ( -- )   \ 0043B390
    4 ebit? if
        $21 state-flag-clear
        $23 state-flag-clear
        1 $FE 0 char-model-op
        $FE 0 char-silent
        $FE action-end
        $FE char-done
    then
    7 state-flag-clear
;

: room70.act00 ( -- )   \ 0047AEC8
    self-idle-or-end
;

: room70.act01 ( -- )   \ 0047AECC
    self-idle-or-end
;

: room70.act07 ( -- )   \ 0043B4E0
    self-wait-anim
    $8005 7 self-anim-blend
    self-wait-anim
    $8001 $14 self-anim-blend
    self-wait-anim
    6 ebit-clear
    ['] room70.act02 goto
;

: room70.act06 ( -- )   \ 0043B4C0
    self-wait-anim
    $8004 $14 self-anim-blend
    self-wait-anim
    $8003 7 self-anim-blend
    self-wait-anim
    6 ebit-clear
    begin
        6 ebit? if
            ['] room70.act07 goto
        then
        $8003 self-anim
        self-wait-anim
    again
;

:noname   \ room70.act02 (0043B3B0; deferred: used before it is defined)
    begin
        6 ebit? if
            ['] room70.act06 goto
        then
        $8001 self-anim
        self-wait-anim
    again
; is room70.act02

: room70.act03 ( -- )   \ 0043B3C0
    self-wait-done
    -97.5 1.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $292 story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: room70.act04 ( -- )   \ 0043B420
    1 self-noclip
    1 self-scripted
    self-wait-done
    $FE $31 -76.0 -56.5 180 char-to-xz
    1 1 8 nav-group
    6 ebit-set
    $FE char-file-use
    6 ebit-clear
    0 $FE 0 char-model-op
    $FE 0 char-silent
    ['] room70.act02 goto
;

: room70.act05 ( -- )   \ 0043B450
    1 self-scripted
    $13 state-flag-set
    self-wait-done
    6 ebit-set
    begin
        6 ebit? while
        yield
    repeat
    $1D 1 item-count? not if
        $FE 0 6 char-sound
        0 message
        self-frames-reset
        $1E self-wait-frames
        wait-message
        $1D message-param-room
        $1D 1 item-give-count
        0 $1D item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $366 story-flag-set
    else
        $FE 1 6 char-sound
        1 message
        self-frames-reset
        $5A self-wait-frames
        wait-message
    then
    6 ebit-set
    begin
        6 ebit? while
        yield
    repeat
    0 self-scripted
    $13 state-flag-clear
    self-idle-or-end
;

: room70.act08 ( -- )   \ 0043B500
    self-wait-done
    -98.51 -12.98 self-turn-to-xz
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
            $27D story-flag-set
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

: room70.act09 ( -- )   \ 0047AED0
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

' room70.enter $70 0 room-script!
' room70.char-enter $70 6 room-script!
' room70.phase1 $70 1 room-script!
' room70.phase2 $70 2 room-script!
' room70.phase5 $70 5 room-script!
' room70.act00 $70 $00 action-script!
' room70.act01 $70 $01 action-script!
' room70.act02 $70 $02 action-script!
' room70.act03 $70 $03 action-script!
' room70.act04 $70 $04 action-script!
' room70.act05 $70 $05 action-script!
' room70.act06 $70 $06 action-script!
' room70.act07 $70 $07 action-script!
' room70.act08 $70 $08 action-script!
' room70.act09 $70 $09 action-script!
