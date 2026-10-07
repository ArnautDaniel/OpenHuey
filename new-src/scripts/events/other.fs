\ events/other.fs - the event scripts of the rooms on none of the game's maps.
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.other
USING: events.core events.words events.map0 ;

\ ---- room $0D ----------------------------------------------------------------------------------

: room0D.char-enter ( -- )   \ 003F5490
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 5 -1 char-camera
                0 camera-follow
            else
                1 5 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then then
    1 5 -1 area-camera
;

: room0D.phase1 ( -- )   \ 003F54D0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    0 $BB char-on-tri? 0 $1C7 char-on-tri? or if
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then
    0 $1D0 char-on-tri? 0 $E0 char-on-tri? or if
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then
    0 $1F1 char-on-tri? 0 $1ED char-on-tri? or 0 $EA char-on-tri? or if
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then
;

: room0D.enter ( -- )   \ 0047A9D0
    1 $3FFF sound-volume
;

: room0D.act00 ( -- )   \ 0047A9D8
    self-idle-or-end
;

' room0D.char-enter $0D 6 room-script!
' room0D.phase1 $0D 1 room-script!
' room0D.enter $0D 0 room-script!
' room0D.act00 $0D $00 action-script!

\ ---- room $27 ----------------------------------------------------------------------------------

\ the items 0x91 / 0x92, by byte 3: 0 which of them Fiona lacks (one each) kept in event var 0
\ (0x91 low byte, 0x92 the next), and event +0x5C(3) when any; 1 they are given back, event
\ +0x5C(0) / (1)
: room27.cmd00 ( b0 -- )  drop s" room27.cmd00" stub-step ;

: room27.enter ( -- )   \ 0041C390
    room-sounds
    $36 1.0 0 bgm
    $FE exit-taken? if
        0 $60 -90 char-to-tri-facing
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
        0 0 0 action
        0 room27.cmd00
        0 0 var? not if
            0 -27.75 10.0 5.68 flicker-sprite
        then
    then
    $AE story-flag? if
        0 1 $14 door-bits
    then
;

: room27.phase1 ( -- )   \ 0041C3E0
    0 0.699 0.0 -17.321 5 10 0 zone
    1 -16.2 0.0 -5.68 5 10 0 zone
    2 -21.456 0.0 6.443 5 10 0 zone
;

: room27.phase2 ( -- )   \ 0041C420
    0 0 char-in-area? 0 45 $32 char-heading? and if
        5 1 0 scene-change
    then
    0 1 char-in-area? 0 -90 $37 char-heading? and if
        5 5 0 scene-change
    then
    0 2 char-in-area? 0 90 $37 char-heading? and if
        $AE story-flag? not if
            5 3 0 scene-change
        else
            5 6 0 scene-change
        then
    then
    0 1 2 char-zone-bits? if
        0 -50 $3C char-heading? if
            5 2 0 scene-change
        then
    then
    0 2 2 char-zone-bits? if
        0 -45 $3C char-heading? if
            5 4 0 scene-change
        then
    then
    0 4 char-in-area? 0 45 $32 char-heading? and if
        5 $A 0 scene-change
    then
    $AE story-flag? if
        0 3 char-in-area? 0 90 $37 char-heading? and if
            5 7 0 scene-change
        then
    then
;

: room27.act00 ( -- )   \ 0041C4A0
    8 state-flag-clear
    camera-restart
    $F 1 fade
    wait-fade
    $AE story-flag? not if
        $89 story-flag? $34B story-flag? not and if
            $F 6 fade
            wait-fade
            $40AB message
            wait-message
            $F 7 fade
            wait-fade
            $34B story-flag-set
            $AB message-param-room
            $AB 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
        then
    then
    self-idle-or-end
;

: room27.act01 ( -- )   \ 0041C4E8
    self-wait-done
    $F 0 fade
    wait-fade
    8 state-flag-set
    $FE exit-check
    self-idle-or-end
;

: room27.act02 ( -- )   \ 0041C500
    self-wait-done
    -100 self-turn-angle
    self-wait-done
    3 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    self-idle-or-end
;

: room27.act03 ( -- )   \ 0041C510
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $34B story-flag? not if
        $40 message
        wait-message
    else $26 3 pvar? if
        $57 message
        wait-message
        0 answer? if
            $F $44 fade
            wait-fade
            8 state-flag-set
            0 0 6 char-sound
            self-frames-reset
            $3C self-wait-frames
            3 state-flag-set
            1 sound-set
            2 fiona-costume
            $26 2 pvar-set
            effects-arena-flip
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            0 3 -0.912 -13.497 180 char-to-xz
            camera-restart
            $F $41 fade
            wait-fade
        then
    else
        $58 message
        wait-message
        0 answer? if
            $F $44 fade
            wait-fade
            8 state-flag-set
            0 0 6 char-sound
            self-frames-reset
            $3C self-wait-frames
            3 state-flag-set
            1 sound-set
            3 fiona-costume
            $26 3 pvar-set
            effects-arena-flip
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            0 3 -0.912 -13.497 180 char-to-xz
            camera-restart
            $F $41 fade
            wait-fade
        then
    then then
    self-idle-or-end
;

: room27.act04 ( -- )   \ 0041C5B0
    self-wait-done
    -28.0 6.0 self-turn-to-xz
    self-wait-done
    3 ebit? if
        2 ebit? not if
            $902 self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            0 ebit-clear
            1 ebit-clear
            1 room27.cmd00
            0 effect-remove
            0 ebit? if
                $91 message-param-room
                0 $91 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            1 ebit? if
                self-frames-reset
                self-wait-16
                $92 message-param-room
                0 $92 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            2 ebit-set
            $903 self-anim
            self-wait-anim
        else
            7 message
            wait-message
        then
    else
        6 message
        wait-message
    then
    self-idle-or-end
;

: room27.act05 ( -- )   \ 0041C640
    1 self-scripted
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: room27.act06 ( -- )   \ 0041C660
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message
    wait-message
    0 answer? if
        $FF 1.0 0 bgm
        7 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            0 0 6 char-sound
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
        $36 1.0 0 bgm
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    self-idle-or-end
;

: room27.act07 ( -- )   \ 0041C7B0
    self-wait-done
    -12.0 -22.0 self-turn-to-xz
    self-wait-done
    $368 story-flag? not $369 story-flag? not and $36A story-flag? not and $36B story-flag? not and if
        $1D01 self-anim
        $A message
        wait-message
        self-wait-anim
    else
        $B message
        wait-message
    then
    self-idle-or-end
;

: room27.act08 ( -- )   \ 0041C7E0
    self-wait-done
    $46 -9.0 -13.0 -160 $FFFF 5 self-move-to
    self-wait-done
    $902 self-anim
    self-wait-anim
    0 4 6 char-sound
    self-frames-reset
    4 self-wait-frames
    $903 self-anim
    self-wait-anim
    0 5 6 char-sound
    1 -11.0 3.0 -17.0 flicker-sprite
    self-frames-reset
    self-wait-16
    self-frames-reset
    $1E self-wait-frames
    $900 self-anim
    self-wait-anim
    1 effect-remove
    1 136 var? if
        $88 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            $88 message-param-room
            $88 1 item-give-count
            0 $88 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $368 story-flag-set
    else 1 140 var? if
        $8C 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            $8C message-param-room
            $8C 1 item-give-count
            0 $8C item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $369 story-flag-set
    else 1 131 var? if
        $83 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            $83 message-param-room
            $83 1 item-give-count
            0 $83 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $36A story-flag-set
    else 1 137 var? if
        $89 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            $89 message-param-room
            $89 1 item-give-count
            0 $89 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $36B story-flag-set
    then then then then
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: room27.act09 ( -- )   \ 0041C9B0
    self-wait-done
    $46 -9.0 -13.0 -160 $FFFF 5 self-move-to
    self-wait-done
    $902 self-anim
    self-wait-anim
    0 4 6 char-sound
    self-frames-reset
    4 self-wait-frames
    $903 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    $1D01 self-anim
    $C message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room27.act0A ( -- )   \ 0041C9F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    8.0 -4.0 self-turn-to-xz
    self-wait-done
    $D message
    wait-message
    $F 6 fade
    wait-fade
    $E message
    wait-message
    $F 7 fade
    wait-fade
    $42 subscreen-bit? not $44 subscreen-bit? not or if
        $1D01 self-anim
        $F message
        wait-message
        self-wait-anim
        $42 subscreen-bit
        $44 subscreen-bit
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room27.char-enter ( -- )   \ 0047ACB0
;

' room27.enter $27 0 room-script!
' room27.phase1 $27 1 room-script!
' room27.phase2 $27 2 room-script!
' room27.act00 $27 $00 action-script!
' room27.act01 $27 $01 action-script!
' room27.act02 $27 $02 action-script!
' room27.act03 $27 $03 action-script!
' room27.act04 $27 $04 action-script!
' room27.act05 $27 $05 action-script!
' room27.act06 $27 $06 action-script!
' room27.act07 $27 $07 action-script!
' room27.act08 $27 $08 action-script!
' room27.act09 $27 $09 action-script!
' room27.act0A $27 $0A action-script!
' room27.char-enter $27 6 room-script!

\ ---- room $33 ----------------------------------------------------------------------------------

: room33.char-enter ( -- )   \ 00442DA0
    0 self-is? if
        $88 story-flag? if
            $80 exit-taken? if
                0 0 0 char-to-tri-facing
                8 state-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 0 action-force
            then
        then
    then
;

: room33.act00 ( -- )   \ 00442DD0
    4 resident-flag? not if
        $19 state-flag-set
    then
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    self-wait-done
    0 1 char-visible
    $17 1 char-no-shadow
    4 char-unload
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
    0 $F9 1 action
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
    $2D 3 pvar-set
    8 state-flag-set
    2 game-over-flag
    $C state-flag-set
    $17 0 char-no-shadow
    self-idle-or-end
;

: room33.act01 ( -- )   \ 00442E80
    self-frames-reset
    1 self-wait-frames
    begin
        4 cutscene-shot? if
            0 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room33.act02 ( -- )   \ 00442EA0
    self-wait-done
    0 1 $14 door-bits
    $17 3 $FF char-load
    3 char-unload
    0 1 char-visible
    $17 1 char-no-shadow
    4 char-unload
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
    0 $F9 1 action
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
    $37 room-preload
    3 0 char-remove
    4 0 char-remove
    $17 0 char-no-shadow
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room33.enter ( -- )   \ 0047AF70
    0 1 $14 door-bits
;

: room33.phase1 ( -- )   \ 0047AF78
;

' room33.char-enter $33 6 room-script!
' room33.act00 $33 $00 action-script!
' room33.act01 $33 $01 action-script!
' room33.act02 $33 $02 action-script!
' room33.enter $33 0 room-script!
' room33.phase1 $33 1 room-script!

\ ---- room $34 ----------------------------------------------------------------------------------

: room34.char-enter ( -- )   \ 00442950
    0 self-is? if
        $88 story-flag? if
            $80 exit-taken? if
                0 0 0 char-to-tri-facing
                8 state-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 0 action-force
            then
        then
    then
;

: room34.act00 ( -- )   \ 00442980
    4 resident-flag? not if
        $19 state-flag-set
    then
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
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
    0 $F9 1 action
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
    $17 action-end
    $17 char-done
    $20 4 $FF char-load
    $33 room-preload
    $80 exit-check
    self-idle-or-end
;

: room34.act01 ( -- )   \ 00442A20
    0 1 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $547 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 1 $14 door-bits
    5 0 $14 door-bits
    begin
        $569 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $587 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 0 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    begin
        $592 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 0 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    begin
        $59E cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 1 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $5AE cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $5D4 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 0 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    begin
        $5EE cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 1 $14 door-bits
    5 0 $14 door-bits
    begin
        $610 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        cutscene-near-end? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    self-idle-or-end
;

: room34.act02 ( -- )   \ 00442B80
    self-wait-done
    $17 3 $FF char-load
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
    0 $F9 1 action
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
    $20 4 $FF char-load
    $33 room-preload
    3 0 char-remove
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: room34.act4D ( -- )   \ 003F263F
    $F1 exit-check
    fade-finish
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
    \ (never runs in the original: an else outside any block)
    \   1 self-is? 0 exit-taken? and if
    \   1 0 char-to-exit
    \   hewie-controlled? not if
    \   0 4 -1 char-camera
    \   0 camera-follow
    \   else
    \   1 4 -1 char-camera
    \   1 camera-follow
    \   then
    \   then
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
            9 call-action   \ (this room has no script 9: the original reads past its table)
        else
            5 ebit-clear
        then
    then
;

: room34.enter ( -- )   \ 0047AF58
;

: room34.phase1 ( -- )   \ 0047AF5C
;

' room34.char-enter $34 6 room-script!
' room34.act00 $34 $00 action-script!
' room34.act01 $34 $01 action-script!
' room34.act02 $34 $02 action-script!
' room29.char-enter $34 $38 action-script!
' room29.char-enter $34 $3D action-script!
' room34.act4D $34 $4D action-script!
' room34.enter $34 0 room-script!
' room34.phase1 $34 1 room-script!

\ ---- room $35 ----------------------------------------------------------------------------------

: room35.char-enter ( -- )   \ 00443080
    0 self-is? if
        $80 exit-taken? if
            0 0 0 action
        then
        $81 exit-taken? if
            $34 story-flag? if
                0 0 1 action
            else
                0 0 2 action
            then
        then
    then
;

: room35.act00 ( -- )   \ 004430A0
    3 resident-flag? not if
        $19 state-flag-set
    then
    8 state-flag-set
    $18 state-flag-set
    1 char-activate
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
    0 1 char-no-shadow
    1 1 char-no-shadow
    $1F $44 $4E $6C $64 $66 $6C $60 $46 0 9 effect-string
    $B090C8BA 0 1 screen-blend
    $40424C74 $4244489C 8.54 25.61 1 fog
    wait-fade
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    $E room-preload
    0 0 char-no-shadow
    1 0 char-no-shadow
    $80 exit-check
    $18 state-flag-clear
    self-idle-or-end
;

: room35.act01 ( -- )   \ 00443170
    1 resident-flag? not if
        $19 state-flag-set
    then
    8 state-flag-set
    2 3 $FF char-load
    3 char-unload
    1 char-activate
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
    0 $F9 3 action
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
    $36 room-preload
    0 0 char-no-shadow
    $80 exit-check
    self-idle-or-end
;

: room35.act02 ( -- )   \ 00443210
    2 resident-flag? not if
        $19 state-flag-set
    then
    8 state-flag-set
    1 char-activate
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
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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
    $2D 1 pvar-set
    2 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room35.act03 ( -- )   \ 004432A0
    begin
        $11 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        $12 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: room35.act04 ( -- )   \ 004432C0
    self-wait-done
    2 3 $FF char-load
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
    wait-fade
    0 $F9 3 action
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
    $36 room-preload
    0 0 char-no-shadow
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: room35.act05 ( -- )   \ 00443350
    self-wait-done
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
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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

: room35.act06 ( -- )   \ 004433E0
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
    0 1 char-no-shadow
    1 1 char-no-shadow
    $1F $44 $4E $6C $64 $66 $6C $60 $46 0 9 effect-string
    $B090C8BA 0 1 screen-blend
    $40424C74 $4244489C 8.54 25.61 1 fog
    wait-fade
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    $E room-preload
    0 0 char-no-shadow
    1 0 char-no-shadow
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: room35.enter ( -- )   \ 0047AF98
;

' room35.char-enter $35 6 room-script!
' room35.act00 $35 $00 action-script!
' room35.act01 $35 $01 action-script!
' room35.act02 $35 $02 action-script!
' room35.act03 $35 $03 action-script!
' room35.act04 $35 $04 action-script!
' room35.act05 $35 $05 action-script!
' room35.act06 $35 $06 action-script!
' room35.enter $35 0 room-script!

\ ---- room $36 ----------------------------------------------------------------------------------

: room36.char-enter ( -- )   \ 00443E60
    0 self-is? if
        $80 exit-taken? if
            0 0 0 action
        then
    then
;

: room36.enter ( -- )   \ 0047AFC0
;

: room36.act00 ( -- )   \ 00443E70
    1 resident-flag? not if
        $19 state-flag-set
    then
    8 state-flag-set
    1 action-end
    1 char-done
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
    3 0 char-remove
    $2D 0 pvar-set
    2 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room36.act01 ( -- )   \ 00443F10
    self-wait-done
    $FF 1 char-visible
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
    $37 room-preload
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room36.char-enter $36 6 room-script!
' room36.enter $36 0 room-script!
' room36.act00 $36 $00 action-script!
' room36.act01 $36 $01 action-script!

\ ---- room $37 ----------------------------------------------------------------------------------

\ Room37_Cmd00
: room37.cmd00 ( -- )  s" room37.cmd00" stub-step ;

defer room37.act00
: room37.enter ( -- )   \ 00445BC0
    room-sounds
    $D state-flag-set
    $13 state-flag-set
    1 resident-flag? 2 resident-flag? and 3 resident-flag? and 4 resident-flag? and if
        $15 resident-flag-set
    then
    1 resident-flag? 2 resident-flag? or if
        $55 resident-flag-set
    then
    $57 resident-flag? $58 resident-flag? or if
        $59 resident-flag-set
    then
    $10C door-lock
    $10D door-lock
    1.0 sound-volume-scale
    0 $F1 $D action
    1.0 sound-volume-scale
    0 1 $14 door-bits
;

: room37.char-enter ( -- )   \ 00445C20
    0 self-is? if
        0 exit-taken? if
            $32 1.0 0 bgm
            8 state-flag-set
            $2B 0 pvar-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
        then
        $80 exit-taken? if
            8 state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $B action-force
        then
        $81 exit-taken? if
            $32 1.0 0 bgm
            8 state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        then
    then
;

: room37.phase1 ( -- )   \ 00445C70
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
;

: room37.phase2 ( -- )   \ 00445C90
    0 $C char-in-area? 0 45 $3C char-heading? and if
        0 char-busy? not if
            3 message
        else
            3 message-close
        then
        5 7 7 scene-change
    else
        3 message-close
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        0 char-busy? not if
            $14 message
        else
            $14 message-close
        then
        5 6 7 scene-change
    else
        $14 message-close
    then
    0 $15 char-in-area? 0 90 $32 char-heading? and if
        0 char-busy? not if
            $40 message
        else
            $40 message-close
        then
        5 2 7 scene-change
    else
        $40 message-close
    then
    0 4 char-in-area? 0 90 $3C char-heading? and if
        0 char-busy? not if
            1 message
        else
            1 message-close
        then
        5 5 7 scene-change
    else
        1 message-close
    then
    0 $1B $2D char-faces-area? if
        0 char-busy? not if
            0 message
        else
            0 message-close
        then
        5 0 7 scene-change
    else
        0 message-close
    then
    0 0 char-group-bit4? if
        0 char-busy? not if
            $15 message
        else
            $15 message-close
        then
        5 1 7 scene-change
    else
        $15 message-close
    then
    0 1 char-group-bit4? if
        0 char-busy? not if
            $16 message
        else
            $16 message-close
        then
        5 8 7 scene-change
    else
        $16 message-close
    then
    0 5 char-in-area? 0 90 $3C char-heading? and if
        0 char-busy? not if
            2 message
        else
            2 message-close
        then
        5 4 7 scene-change
    else
        2 message-close
    then
;

: room37.phase3 ( -- )   \ 00445D70
    15.0 8.0 -47.0 0.0 8.0 -47.0 15.0 0.0 -47.0 0.0 0.0 -47.0 lights-doorway
    -22.0 6.0 59.0 -20.0 6.0 32.0 -22.0 -8.0 59.0 -20.0 -8.0 32.0 lights-doorway
;

: room37.act03 ( -- )   \ 004461B0
    $2A 0 pvar? if
        $21 room-preload
    else $2A 1 pvar? if
        $21 room-preload
    else $2A 2 pvar? if
        $20 room-preload
    else $2A 3 pvar? if
        $23 room-preload
    else $2A 4 pvar? if
        $24 room-preload
    else $2A 5 pvar? if
        $24 room-preload
    else $2A 6 pvar? if
        9 room-preload
    else $2A 7 pvar? if
        $25 room-preload
    else $2A 8 pvar? if
        9 room-preload
    else $2A 9 pvar? if
        9 room-preload
    else $2A $A pvar? if
        $12 room-preload
    else $2A $B pvar? if
        $F room-preload
    else $2A $C pvar? if
        $31 room-preload
    else $2A $D pvar? if
        $2A room-preload
    else $2A $E pvar? if
        $13 room-preload
    else $2A $F pvar? if
        $21 room-preload
    else $2A $10 pvar? if
        $21 room-preload
    else $2A $11 pvar? if
        0 room-preload
    else $2A $12 pvar? if
        0 room-preload
    else $2A $13 pvar? if
        $12 room-preload
    else $2A $14 pvar? if
        $F room-preload
    else $2A $15 pvar? if
        $15 room-preload
    else $2A $16 pvar? if
        $18 room-preload
    else $2A $17 pvar? if
        $19 room-preload
    else $2A $18 pvar? if
        $19 room-preload
    else $2A $19 pvar? if
        $19 room-preload
    else $2A $1A pvar? if
        8 room-preload
    else $2A $1B pvar? if
        8 room-preload
    else $2A $1C pvar? if
        $1C room-preload
    else $2A $1D pvar? if
        $29 room-preload
    else $2A $1E pvar? if
        $1E room-preload
    else $2A $1F pvar? if
        2 room-preload
    else $2A $20 pvar? if
        3 room-preload
    else $2A $21 pvar? if
        $17 room-preload
    else $2A $22 pvar? if
        $23 room-preload
    else $2A $23 pvar? if
        4 room-preload
    else $2A $24 pvar? if
        $A room-preload
    else $2A $25 pvar? if
        $C room-preload
    else $2A $26 pvar? if
        $C room-preload
    else $2A $27 pvar? if
        $C room-preload
    else $2A $28 pvar? if
        $C room-preload
    else $2A $29 pvar? if
        $C room-preload
    else $2A $2B pvar? if
        6 room-preload
    else $2A $2C pvar? if
        $11 room-preload
    else $2A $2D pvar? if
        $32 room-preload
    else $2A $2A pvar? if
        $29 room-preload
    else $2A $31 pvar? if
        $49 room-preload
    else $2A $2E pvar? if
        $2E room-preload
    else $2A $2F pvar? if
        $55 room-preload
    else $2A $30 pvar? if
        $5D room-preload
    else $2A $32 pvar? if
        $4F room-preload
    else $2A $33 pvar? if
        $4F room-preload
    else $2A $34 pvar? if
        $51 room-preload
    else $2A $35 pvar? if
        $51 room-preload
    else $2A $36 pvar? if
        $51 room-preload
    else $2A $37 pvar? if
        $51 room-preload
    else $2A $38 pvar? if
        $50 room-preload
    else $2A $39 pvar? if
        $4C room-preload
    else $2A $3A pvar? if
        $4C room-preload
    else $2A $3B pvar? if
        $4C room-preload
    else $2A $45 pvar? if
        $63 room-preload
    else $2A $46 pvar? if
        $63 room-preload
    else $2A $3C pvar? if
        $48 room-preload
    else $2A $3D pvar? if
        $52 room-preload
    else $2A $3E pvar? if
        $48 room-preload
    else $2A $3F pvar? if
        $48 room-preload
    else $2A $40 pvar? if
        $66 room-preload
    else $2A $42 pvar? if
        $53 room-preload
    else $2A $43 pvar? if
        $59 room-preload
    else $2A $44 pvar? if
        $59 room-preload
    else $2A $47 pvar? if
        $60 room-preload
    else $2A $48 pvar? if
        $60 room-preload
    else $2A $49 pvar? if
        $47 room-preload
    else $2A $4A pvar? if
        $59 room-preload
    else $2A $4B pvar? if
        $59 room-preload
    else $2A $4C pvar? if
        $4A room-preload
    else $2A $4D pvar? if
        $40 room-preload
    else $2A $4E pvar? if
        $40 room-preload
    else $2A $4F pvar? if
        $106 room-preload
    else $2A $50 pvar? if
        $108 room-preload
    else $2A $51 pvar? if
        $10B room-preload
    else $2A $52 pvar? if
        $59 room-preload
    else $2A $53 pvar? if
        $C0 room-preload
    else $2A $54 pvar? if
        $C0 room-preload
    else $2A $55 pvar? if
        $C0 room-preload
    else $2A $56 pvar? if
        $C5 room-preload
    else $2A $57 pvar? if
        $C7 room-preload
    else $2A $58 pvar? if
        $C7 room-preload
    else $2A $59 pvar? if
        $C7 room-preload
    else $2A $5A pvar? if
        $C7 room-preload
    else $2A $5B pvar? if
        $83 room-preload
    else $2A $5C pvar? if
        $81 room-preload
    else $2A $5D pvar? if
        $8A room-preload
    else $2A $5E pvar? if
        $8C room-preload
    else $2A $5F pvar? if
        $8C room-preload
    else $2A $60 pvar? if
        $8C room-preload
    else $2A $61 pvar? if
        $8E room-preload
    else $2A $62 pvar? if
        $85 room-preload
    else $2A $63 pvar? if
        $84 room-preload
    else $2A $64 pvar? if
        $8F room-preload
    else $2A $65 pvar? if
        $92 room-preload
    else $2A $66 pvar? if
        $92 room-preload
    else $2A $67 pvar? if
        $92 room-preload
    else $2A $68 pvar? if
        $8F room-preload
    else $2A $69 pvar? if
        $8D room-preload
    else $2A $6A pvar? if
        $35 room-preload
    else $2A $6B pvar? if
        $35 room-preload
    else $2A $6C pvar? if
        $35 room-preload
    else $2A $6D pvar? if
        $34 room-preload
    then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then
    exit
;

:noname   \ room37.act00 (00445DE0; deferred: used before it is defined)
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 camera-mode? if
        $115 -34.0 27.0 0 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 10.0 -20.0 0.0 event-camera
    else
        $11B -34.0 60.0 180 $FFFF 5 self-move-to
        self-wait-done
        1 40.0 10.0 160.0 0.0 event-camera
    then
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    8 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $2A $FF pvar? not if
        $2A $41 pvar? if
            1 1 char-silent
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
            8 state-flag-set
            0 $11B -34.0 60.0 180 char-to-xz
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
            $37 0 267 hewie-to-room
            1 $10B 0 char-to-tri-facing
            1 1 1 char-camera
            yield
            camera-restart
            1 0 char-silent
            ['] room37.act00 goto
        else
            room37.act03
            $2C 0 pvar-set
            $80 exit-check
            8 state-flag-set
            0 state-flag-clear
            $26 state-flag-set
        then
    else
        $2B 0 pvar-set
        $32 1.0 0 bgm
        $F $41 fade
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
; is room37.act00

: room37.act01 ( -- )   \ 00445EF0
    $2E $FF pvar-set
    self-wait-done
    $FF 1.0 0 bgm
    $A subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $2E 0 pvar? if
        $D9 room-preload
        7 partner-load
        $1B story-flag-set
        $41 story-flag-clear
        $6F story-flag-clear
        $82 story-flag-clear
        $88 story-flag-clear
        $89 story-flag-clear
        $94 story-flag-clear
        $95 story-flag-clear
    else $2E 1 pvar? if
        $E0 room-preload
        $22 partner-load
        $1B story-flag-set
        $41 story-flag-set
        $6F story-flag-set
        $82 story-flag-clear
        $88 story-flag-clear
        $89 story-flag-clear
        $94 story-flag-clear
        $95 story-flag-clear
    else $2E 2 pvar? if
        $D9 room-preload
        $C partner-load
        $1B story-flag-set
        $41 story-flag-set
        $6F story-flag-set
        $82 story-flag-set
        $88 story-flag-set
        $89 story-flag-set
        $94 story-flag-set
        $95 story-flag-set
    else $2E 3 pvar? if
        $E0 room-preload
        $17 partner-load
        $1B story-flag-set
        $41 story-flag-set
        $6F story-flag-set
        $82 story-flag-set
        $88 story-flag-set
        $89 story-flag-set
        $94 story-flag-clear
        $95 story-flag-clear
    then then then then
    $F 1 fade
    wait-fade
    $2E $FF pvar? not if
        $10C door-unlock
        0 3 self-move-slot
        self-wait-done
        0 1 9 action
        $76 -16.0 -82.0 180 $FFFF 5 self-move-to
        self-wait-done
        2 char-unload
        $FF 1.0 0 bgm
        $F 0 fade
        wait-fade
        fiona-calm-reset
        0 panic-stage
        1 action-end
        1 char-done
        8 state-flag-set
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
        begin
            yield
            4 sound-bank-loaded? until
        $27 0 pvar? if
            0 hewie-model
        else $27 1 pvar? if
            1 hewie-model
        else $27 2 pvar? if
            2 hewie-model
        then then then
        effects-arena-flip
        1 char-in
        1 action-end
        1 char-done
        $13 state-flag-clear
        $E state-flag-clear
        $80 exit-check
    else
        $32 1.0 0 bgm
    then
    self-idle-or-end
;

: room37.act02 ( -- )   \ 00446070
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
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
    $32 1.0 0 bgm
    $F $41 fade
    wait-fade
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room37.act04 ( -- )   \ 00446520
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $B subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room37.act05 ( -- )   \ 00446560
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $121 0.0 10.5 180 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $FF 1.0 0 bgm
    self-frames-reset
    $1E self-wait-frames
    $D subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    self-frames-reset
    8 self-wait-frames
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room37.act06 ( -- )   \ 004465C0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    9 -41.0 -38.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $E subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    self-frames-reset
    8 self-wait-frames
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room37.act07 ( -- )   \ 00446620
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $D7 45.0 -39.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $F subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    self-frames-reset
    8 self-wait-frames
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room37.act08 ( -- )   \ 00446680
    self-wait-done
    $10D door-unlock
    1 3 self-move-slot
    self-wait-done
    $F 0 fade
    $FF 1.0 0 bgm
    $7C 17.142 22.877 90 $FFFF 5 self-move-to
    self-wait-done
    wait-fade
    8 state-flag-set
    begin
        1 adx? not while
        yield
    repeat
    self-frames-reset
    $1E self-wait-frames
    begin
        $1C state-flag-set
        yield
    again
;

: room37.act09 ( -- )   \ 004466C0
    1 char-full-health
    1 0 char-set-C4
    self-wait-done
    $76 -16.0 -82.0 180 $FFFF $A self-move-to
    self-wait-done
    begin
        yield
    again
;

: room37.act0A ( -- )   \ 004466E0
    self-wait-done
    $2A 0 pvar-set
    $2B 0 pvar-set
    0 $15 0 char-to-tri-facing
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    $37 0 267 hewie-to-room
    1 $10B 0 char-to-tri-facing
    1 1 1 char-camera
    yield
    camera-restart
    $28 state-flag-clear
    $F 1 fade
    wait-fade
    $10 resident-flag? not if
        $F 6 fade
        wait-fade
        $40AC message
        wait-message
        $F 7 fade
        wait-fade
        $10 resident-flag-set
        $AC message-param-room
        $AC 1 item-give-count
        $83 $85 0.0 0.0 0.0 0 0 sound
        $801B message
        self-frames-reset
        $1E self-wait-frames
        wait-message
    else
        $AC 1 item-give-count
    then
    $11 resident-flag? not if
        $56 resident-flag? 1 resident-flag? and if
            $25A item-give
            $11 resident-flag-set
        then
    else
        $25A item-add
    then
    $12 resident-flag? not if
        8 resident-flag? $C resident-flag? or $56 resident-flag? and 1 resident-flag? and 3 resident-flag? and $59 resident-flag? and if
            $25B item-give
            $12 resident-flag-set
        then
    else
        $25B item-add
    then
    $13 resident-flag? not if
        1 resident-flag? if
            $25C item-give
            $13 resident-flag-set
        then
    else
        $25C item-add
    then
    $14 resident-flag? not if
        $15 resident-flag? if
            $25D item-give
            $14 resident-flag-set
        then
    else
        $25D item-add
    then
    $5A resident-flag? not if
        8 resident-flag? $C resident-flag? or $56 resident-flag? and 1 resident-flag? and 3 resident-flag? and if
            $25E item-give
            $5A resident-flag-set
        then
    else
        $25E item-add
    then
    $5B resident-flag? not if
        5 resident-flag? 6 resident-flag? and 7 resident-flag? and 8 resident-flag? and 9 resident-flag? and $A resident-flag? and $B resident-flag? and $C resident-flag? and $D resident-flag? and $E resident-flag? and $F resident-flag? and if
            $25F item-give
            $5B resident-flag-set
        then
    else
        $25F item-add
    then
    $5C resident-flag? not if
        $52 resident-flag? $53 resident-flag? and if
            $260 item-give
            $5C resident-flag-set
        then
    else
        $260 item-add
    then
    $5D resident-flag? not if
        $11 resident-flag? $12 resident-flag? and $13 resident-flag? and $14 resident-flag? and $5A resident-flag? and $5B resident-flag? and $5C resident-flag? and if
            $261 item-give
            $5D resident-flag-set
        then
    else
        $261 item-add
    then
    self-idle-or-end
;

: room37.act0B ( -- )   \ 00446880
    self-wait-done
    0 $11B -34.0 60.0 180 char-to-xz
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    $37 0 267 hewie-to-room
    1 $10B 0 char-to-tri-facing
    1 1 1 char-camera
    yield
    camera-restart
    ['] room37.act00 goto
;

: room37.act0C ( -- )   \ 004468C0
    4 0 char-remove
    5 0 char-remove
    self-wait-done
    0 panic-stage
    fiona-calm-reset
    0 $13 -16.0 -66.5 0 char-to-xz
    0 0 self-anim-blend
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    music-stage-end
    1 $C4 -10.03 -61.53 0 char-to-xz
    1 1 1 char-camera
    yield
    camera-restart
    $F $51 fade
    wait-fade
    $5C resident-flag? not if
        $52 resident-flag? $53 resident-flag? and if
            $260 item-give
            $5C resident-flag-set
        then
    else
        $260 item-add
    then
    $5D resident-flag? not if
        $11 resident-flag? $12 resident-flag? and $13 resident-flag? and $14 resident-flag? and $5A resident-flag? and $5B resident-flag? and $5C resident-flag? and if
            $261 item-give
            $5D resident-flag-set
        then
    else
        $261 item-add
    then
    self-idle-or-end
;

: room37.act0D ( -- )   \ 0047B020
    begin
        room37.cmd00
        yield
    again
;

' room37.enter $37 0 room-script!
' room37.char-enter $37 6 room-script!
' room37.phase1 $37 1 room-script!
' room37.phase2 $37 2 room-script!
' room37.phase3 $37 3 room-script!
' room37.act00 $37 $00 action-script!
' room37.act01 $37 $01 action-script!
' room37.act02 $37 $02 action-script!
' room37.act03 $37 $03 action-script!
' room37.act04 $37 $04 action-script!
' room37.act05 $37 $05 action-script!
' room37.act06 $37 $06 action-script!
' room37.act07 $37 $07 action-script!
' room37.act08 $37 $08 action-script!
' room37.act09 $37 $09 action-script!
' room37.act0A $37 $0A action-script!
' room37.act0B $37 $0B action-script!
' room37.act0C $37 $0C action-script!
' room37.act0D $37 $0D action-script!

\ ---- room $5A ----------------------------------------------------------------------------------

\ the three dials (Room5A_ObjectNames, progress vars gSndProgressVars: 0..3, 90 degrees each),
\ event var 0 the one picked: byte 3 0 set (the original sets the picked one three times), 1 up
\ / down picks one (event var 0), left / right turns it (event +0x5C 3), cancel leaves (+0x60
\ 2); 2 turning to it 4 degrees a step, and there: solved at 1 / 0 / 2 (+0x60 2, +0x5C 4), else
\ +0x60 3; 3 wait
: room5A.cmd00 ( b0 -- )  drop s" room5A.cmd00" stub-step ;

: room5A.char-enter ( -- )   \ 00410BE0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 6 -1 char-camera
                0 camera-follow
            else
                1 6 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 6 -1 char-camera
            0 camera-follow
        else
            1 6 -1 char-camera
            1 camera-follow
        then
    then then
    0 6 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    1 3 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    2 3 2 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
    then then
    3 2 1 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
            0 0 1 action
            1 $59 char-in-room? if
            then
        then
    then
;

: room5A.phase1 ( -- )   \ 00410D00
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
    $A 5 4 1 chars-area-camera
    $B 6 -1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 5 4 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 1 -1 1 chars-area-camera
    $10 1 -1 1 chars-area-camera
    $11 4 3 1 chars-area-camera
    $12 4 3 1 chars-area-camera
    $13 3 2 1 chars-area-camera
    $14 2 1 1 chars-area-camera
    $15 3 2 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? 0 6 char-entered-area? or if
        1 exit-prepare
    then
    0 7 char-entered-area? 0 8 char-entered-area? or if
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
;

: room5A.phase2 ( -- )   \ 00410D90
    0 $16 $32 char-faces-area? if
        5 2 0 scene-change
    then
    1 exit-door-open? not 0 ebit? and if
        0 1 char-group-bit4? if
            0 scene-ending
        then
    then
    0 $17 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: room5A.phase5 ( -- )   \ 00410DC0
    0 ebit? $FE char-here? not and $FE 2 char-C4? and if
        $FE action-end
        2 summon-take
    then
;

: room5A.act00 ( -- )   \ 00410DD0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5A.act01 ( -- )   \ 00410E10
    0 ebit-set
    $F $41 fade
    0 $28 5 char-sound
    self-wait-done
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5A.act02 ( -- )   \ 00410E30
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 message
    wait-message
    0 answer? if
        0 $D2 293.136 -95.0 180 char-to-xz
        $17 state-flag-set
        1 5.0 0.0 0.0 2.0 event-camera
        2 message
        2 ebit-set
        0 0 var-set
        begin
            3 ebit? not if
                1 room5A.cmd00
            else
                2 room5A.cmd00
            then
            2 ebit? while
            yield
        repeat
        2 message-close
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
        4 ebit? if
            self-frames-reset
            self-wait-16
            0 $72 5 char-sound
            318.0 10.0 80.0 self-look-at-point
            yield
            self-frames-reset
            $3C self-wait-frames
            $FF self-look-at
            yield
        then
    then
    \ (never runs in the original: an else outside any block)
    \   7 message
    \   wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5A.act03 ( -- )   \ 00410ED0
    self-wait-done
    -115.0 -75.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: room5A.act04 ( -- )   \ 00410EF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -115.0 -75.0 self-turn-to-xz
    self-wait-done
    5 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room5A.enter ( -- )   \ 0047ABE0
    0 room5A.cmd00
;

' room5A.char-enter $5A 6 room-script!
' room5A.phase1 $5A 1 room-script!
' room5A.phase2 $5A 2 room-script!
' room5A.phase5 $5A 5 room-script!
' room5A.act00 $5A $00 action-script!
' room5A.act01 $5A $01 action-script!
' room5A.act02 $5A $02 action-script!
' room5A.act03 $5A $03 action-script!
' room5A.act04 $5A $04 action-script!
' room5A.enter $5A 0 room-script!

\ ---- room $64 ----------------------------------------------------------------------------------

: room64.char-enter ( -- )   \ 004295A0
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

: room64.phase1 ( -- )   \ 00429620
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

: room64.enter ( -- )   \ 0047AD44
;

' room64.char-enter $64 6 room-script!
' room64.phase1 $64 1 room-script!
' room64.enter $64 0 room-script!

\ ---- room $65 ----------------------------------------------------------------------------------

: room65.char-enter ( -- )   \ 00429640
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
;

: room65.phase1 ( -- )   \ 00429700
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
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        2 exit-prepare
    then
;

: room65.enter ( -- )   \ 0047AD50
;

' room65.char-enter $65 6 room-script!
' room65.phase1 $65 1 room-script!
' room65.enter $65 0 room-script!

\ ---- room $C4 ----------------------------------------------------------------------------------

: roomC4.char-enter ( -- )   \ 00440100
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    1 5 5 area-camera
;

: roomC4.phase1 ( -- )   \ 00440140
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    2 0 0 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 1 1 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 3 3 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 4 4 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 5 5 1 chars-area-camera
    $C 4 4 1 chars-area-camera
    $D 5 5 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 6 6 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
;

: roomC4.enter ( -- )   \ 0047AF0C
;

: roomC4.act00 ( -- )   \ 0047AF10
    self-idle-or-end
;

' roomC4.char-enter $C4 6 room-script!
' roomC4.phase1 $C4 1 room-script!
' roomC4.enter $C4 0 room-script!
' roomC4.act00 $C4 $00 action-script!

\ ---- room $D0 ----------------------------------------------------------------------------------

\ RoomD0_Cmd00
: roomD0.cmd00 ( -- )  s" roomD0.cmd00" stub-step ;
\ RoomD0_Cmd01
: roomD0.cmd01 ( -- )  s" roomD0.cmd01" stub-step ;
\ RoomD0_Cmd02
: roomD0.cmd02 ( -- )  s" roomD0.cmd02" stub-step ;
\ RoomD0_Cond00
: roomD0.cond00? ( -- flag )  s" roomD0.cond00?" stub-flag ;

: roomD0.enter ( -- )   \ 004469C0
    0 $16F 8 nav-tri-flags
    0 $174 8 nav-tri-flags
    $19 1.0 0 bgm
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: roomD0.char-enter ( -- )   \ 00446A00
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    0 3 3 area-camera
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

: roomD0.phase1 ( -- )   \ 00446A80
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    2 0 0 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 1 1 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 4 4 1 chars-area-camera
    7 5 5 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 5 -1 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 5 5 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 5 -1 1 chars-area-camera
    1 2 char-entered-area? if
        1 exit-prepare
    then
    1 ebit? not if
        0 $10 char-in-area? if
            1 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 0 action-force
            0 1 1 action-force
        then
    then
;

: roomD0.phase3 ( -- )   \ 00446B00
    roomD0.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomD0.cmd02
    then then
;

: roomD0.phase2 ( -- )   \ 0047B030
;

: roomD0.act00 ( -- )   \ 00446B20
    $E state-flag-set
    $18 state-flag-set
    1 self-scripted
    roomD0.cmd01
    $FF 1.0 0 bgm
    begin
        1 adx? not while
        yield
    repeat
    $34 1.0 1 bgm
    0 $1E 0 music
    self-frames-reset
    $1E self-wait-frames
    self-wait-done
    begin
        0 adx? not while
        yield
    repeat
    0 0.0 $FF bgm
    begin
        1 adx? not while
        yield
    repeat
    $37 room-preload
    $C subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    9 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $FF 1.0 0 bgm
    $F $50 fade
    wait-fade
    roomD0.cmd00
    $37 0 -1 hewie-to-room
    $10C door-open-clear
    2 0 char-remove
    $18 state-flag-clear
    $81 exit-check
    self-idle-or-end
;

: roomD0.act01 ( -- )   \ 0047B038
    1 self-scripted
    self-wait-done
    begin
        yield
    again
;

' roomD0.enter $D0 0 room-script!
' roomD0.char-enter $D0 6 room-script!
' roomD0.phase1 $D0 1 room-script!
' roomD0.phase3 $D0 3 room-script!
' roomD0.phase2 $D0 2 room-script!
' roomD0.act00 $D0 $00 action-script!
' roomD0.act01 $D0 $01 action-script!

\ ---- room $D1 ----------------------------------------------------------------------------------

\ as Room106_Cmd00
: roomD1.cmd00 ( -- )  s" roomD1.cmd00" stub-step ;
\ room 0xD1: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD1.cmd01 ( -- )  s" roomD1.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD1.cond00? ( -- flag )  s" roomD1.cond00?" stub-flag ;

: roomD1.enter ( -- )   \ 00446C00
    room-sounds
    $19 1.0 0 bgm
    roomD1.cmd00
;

: roomD1.char-enter ( -- )   \ 00446C10
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
;

: roomD1.phase1 ( -- )   \ 00446C90
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    1 2 char-entered-area? if
        0 exit-prepare
    then
    1 3 char-entered-area? if
        $1A 4 1 char-load
        $E 5 1 char-load
        1 exit-prepare
    then
    1 3 char-left-area? if
        4 0 char-remove
        5 0 char-remove
    then
;

: roomD1.phase3 ( -- )   \ 00446CF0
    roomD1.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomD1.cmd01
    then then
;

: roomD1.phase2 ( -- )   \ 0047B048
;

: roomD1.act00 ( -- )   \ 0047B04C
    self-idle-or-end
;

' roomD1.enter $D1 0 room-script!
' roomD1.char-enter $D1 6 room-script!
' roomD1.phase1 $D1 1 room-script!
' roomD1.phase3 $D1 3 room-script!
' roomD1.phase2 $D1 2 room-script!
' roomD1.act00 $D1 $00 action-script!

\ ---- room $D2 ----------------------------------------------------------------------------------

\ room 0xD2: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD2.cmd00 ( -- )  s" roomD2.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD2.cond00? ( -- flag )  s" roomD2.cond00?" stub-flag ;

: roomD2.char-enter ( -- )   \ 00446D60
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

: roomD2.phase1 ( -- )   \ 00446E60
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
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
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 5 char-entered-area? if
        1 exit-prepare
    then
    1 6 char-entered-area? if
        2 exit-prepare
    then
    1 7 char-entered-area? if
        3 exit-prepare
    then
;

: roomD2.phase3 ( -- )   \ 00446EE0
    roomD2.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomD2.cmd00
    then then
;

: roomD2.enter ( -- )   \ 0047B054
;

' roomD2.char-enter $D2 6 room-script!
' roomD2.phase1 $D2 1 room-script!
' roomD2.phase3 $D2 3 room-script!
' roomD2.enter $D2 0 room-script!

\ ---- room $D3 ----------------------------------------------------------------------------------

\ room 0xD3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD3.cmd00 ( -- )  s" roomD3.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD3.cond00? ( -- flag )  s" roomD3.cond00?" stub-flag ;

: roomD3.char-enter ( -- )   \ 00446F30
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
;

: roomD3.phase1 ( -- )   \ 00447030
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
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
    8 2 2 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 5 char-entered-area? if
        1 exit-prepare
    then
    1 6 char-entered-area? if
        2 exit-prepare
    then
    1 7 char-entered-area? if
        3 exit-prepare
    then
;

: roomD3.phase3 ( -- )   \ 004470D0
    roomD3.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomD3.cmd00
    then then
;

: roomD3.enter ( -- )   \ 0047B058
    $19 1.0 0 bgm
;

: roomD3.phase2 ( -- )   \ 0047B060
;

: roomD3.act00 ( -- )   \ 0047B064
    self-idle-or-end
;

' roomD3.char-enter $D3 6 room-script!
' roomD3.phase1 $D3 1 room-script!
' roomD3.phase3 $D3 3 room-script!
' roomD3.enter $D3 0 room-script!
' roomD3.phase2 $D3 2 room-script!
' roomD3.act00 $D3 $00 action-script!

\ ---- room $D4 ----------------------------------------------------------------------------------

\ room 0xD4: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD4.cmd00 ( -- )  s" roomD4.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD4.cond00? ( -- flag )  s" roomD4.cond00?" stub-flag ;

: roomD4.char-enter ( -- )   \ 00447130
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

: roomD4.phase1 ( -- )   \ 00447230
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
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
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 5 char-entered-area? if
        1 exit-prepare
    then
    1 6 char-entered-area? if
        2 exit-prepare
    then
    1 7 char-entered-area? if
        3 exit-prepare
    then
;

: roomD4.phase3 ( -- )   \ 004472B0
    roomD4.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomD4.cmd00
    then then
;

: roomD4.enter ( -- )   \ 0047B06C
;

' roomD4.char-enter $D4 6 room-script!
' roomD4.phase1 $D4 1 room-script!
' roomD4.phase3 $D4 3 room-script!
' roomD4.enter $D4 0 room-script!

\ ---- room $D5 ----------------------------------------------------------------------------------

\ (as Room109_Cmd00) character kind 0x1A: byte 3 0 starts Kind26_MoveTo(2, -6, 257); else waits
\ (2) until Kind26_MoveDone says done
: roomD5.cmd00 ( b0 -- )  drop s" roomD5.cmd00" stub-step ;
\ as Room109_Cmd01
: roomD5.cmd01 ( b0 -- )  drop s" roomD5.cmd01" stub-step ;
\ room 0xD5: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD5.cmd02 ( -- )  s" roomD5.cmd02" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD5.cond00? ( -- flag )  s" roomD5.cond00?" stub-flag ;

: roomD5.enter ( -- )   \ 00447300
    room-sounds
    $19 1.0 0 bgm
    $1E chance? if
        $1A 4 $FF char-load
        0 $F1 0 action
    then
    $35B story-flag? not if
        $E 5 $FF char-load
        0 $F2 1 action
    then
;

: roomD5.char-enter ( -- )   \ 00447330
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
;

: roomD5.phase1 ( -- )   \ 00447460
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
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
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    $A 3 3 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 3 3 1 chars-area-camera
    1 5 char-entered-area? if
        0 exit-prepare
    then
    1 6 char-entered-area? if
        1 exit-prepare
    then
    1 7 char-entered-area? if
        2 exit-prepare
    then
    1 8 char-entered-area? if
        3 exit-prepare
    then
    1 9 char-entered-area? if
        4 exit-prepare
    then
    0 ebit? $35B story-flag? not and if
        0 -24.0 8.31 -132.0 $19 10 0 zone
        0 0 2 char-zone-bits? 1 0 2 char-zone-bits? or if
            $35B story-flag-set
            $F2 action-end
            0 $F2 2 action
        then
    then
;

: roomD5.phase5 ( -- )   \ 00447550
    $1A action-end
    $1A char-done
    $E action-end
    $E char-done
;

: roomD5.phase3 ( -- )   \ 00447560
    roomD5.cond00? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
    else
        roomD5.cmd02
    then then
;

: roomD5.act00 ( -- )   \ 00447580
    4 char-unload
    $1A char-activate
    $1A 1 3.0 235.0 180 char-to-xz
    $1A $9000 1 0 char-anim-hold
    0 roomD5.cmd00
    1 roomD5.cmd00
    $1A action-end
    $1A char-done
    self-idle-or-end
;

: roomD5.act01 ( -- )   \ 004475B0
    5 char-unload
    0 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -21.17 31.84 -137.07 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: roomD5.act02 ( -- )   \ 004475F0
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 roomD5.cmd01
    1 roomD5.cmd01
    $E action-end
    $E char-done
    self-idle-or-end
;

' roomD5.enter $D5 0 room-script!
' roomD5.char-enter $D5 6 room-script!
' roomD5.phase1 $D5 1 room-script!
' roomD5.phase5 $D5 5 room-script!
' roomD5.phase3 $D5 3 room-script!
' roomD5.act00 $D5 $00 action-script!
' roomD5.act01 $D5 $01 action-script!
' roomD5.act02 $D5 $02 action-script!

\ ---- room $D6 ----------------------------------------------------------------------------------

\ (as Room105_Cmd00) pushed by the character slot byte 4 names
: roomD6.cmd00 ( b0 b1 -- )  drop drop s" roomD6.cmd00" stub-step ;
\ room 0xD6: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD6.cmd01 ( -- )  s" roomD6.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD6.cond00? ( -- flag )  s" roomD6.cond00?" stub-flag ;

: roomD6.act02 ( -- )   \ 00447990
    1 ebit-clear
    $2F 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $2F 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $2F 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $2F 3 pvar? if
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
    $2F pvar-inc
    exit
;

: roomD6.char-enter ( -- )   \ 00447680
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
    $FE self-is? if
        9 state-flag? $FE 1 char-heading-for? and if
            2 ebit-set
            roomD6.act02
        else
            2 ebit-clear
        then
    then
;

: roomD6.phase1 ( -- )   \ 00447790
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
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
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 6 char-entered-area? if
        2 exit-prepare
    then
    1 7 char-entered-area? if
        3 exit-prepare
    then
    1 70.5 5.0 -5.5 $14 10 0 zone
    1 1 8 char-zone-bits? if
        0 char-here? 0 1 char-heading-for? and if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 0 action
                    else $FE 1 char-heading-for? not if
                        0 0 0 action
                    then then
                then
            then
        then
    then
    0 70.5 5.0 -5.5 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 0 roomD6.cmd00
    else 1 0 8 char-zone-bits? if
        1 1 roomD6.cmd00
    else $FE 0 8 char-zone-bits? if
        1 2 roomD6.cmd00
    then then then
    2 0 roomD6.cmd00
;

: roomD6.phase3 ( -- )   \ 00447890
    roomD6.cond00? not if
        4 ebit-set
    else 4 ebit? if
        4 ebit-clear
    else
        roomD6.cmd01
    then then
;

: roomD6.phase5 ( -- )   \ 004478B0
    0 char-busy? if
        9 state-flag-clear
        $18 state-flag-clear
        0 action-end
        0 $E7 90.99 -3.17 90 char-to-xz
    then
;

: roomD6.act01 ( -- )   \ 00447960
    0 ebit? if
        2 avoid-prompt
    then
    9 state-flag-clear
    $18 state-flag-set
    $FE self-look-at
    yield
    4 $14 self-anim-blend
    self-wait-anim
    $FF self-look-at
    yield
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    0 0 $40000 nav-group
    self-idle-or-end
;

: roomD6.act00 ( -- )   \ 004478D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $19A 70.5 -5.5 90 $FFFF $A self-move-to
    self-wait-done
    1 0 $40000 nav-group
    2 ebit-clear
    1 self-noclip
    0 ebit-clear
    $800 self-anim
    self-wait-anim
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] roomD6.act01 goto
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
            9 state-flag-clear
            $FE action-end
            2 game-mode? if
                ['] roomD6.act01 goto
            then
            $18 state-flag-set
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            0 0 $40000 nav-group
            self-idle-or-end
        then
    again
;

: roomD6.act03 ( -- )   \ 004479E0
    self-wait-done
    $E7 90.5 -4.5 -90 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: roomD6.enter ( -- )   \ 0047B070
    room-sounds
    0 0 roomD6.cmd00
;

: roomD6.phase2 ( -- )   \ 0047B078
;

' roomD6.char-enter $D6 6 room-script!
' roomD6.phase1 $D6 1 room-script!
' roomD6.phase3 $D6 3 room-script!
' roomD6.phase5 $D6 5 room-script!
' roomD6.act00 $D6 $00 action-script!
' roomD6.act01 $D6 $01 action-script!
' roomD6.act02 $D6 $02 action-script!
' roomD6.act03 $D6 $03 action-script!
' roomD6.enter $D6 0 room-script!
' roomD6.phase2 $D6 2 room-script!

\ ---- room $D7 ----------------------------------------------------------------------------------

\ as Room107_Cmd00
: roomD7.cmd00 ( b0 -- )  drop s" roomD7.cmd00" stub-step ;
\ room 0xD7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD7.cmd01 ( -- )  s" roomD7.cmd01" stub-step ;
\ as Room107_Cmd01
: roomD7.cmd02 ( -- )  s" roomD7.cmd02" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD7.cond01? ( -- flag )  s" roomD7.cond01?" stub-flag ;

: roomD7.enter ( -- )   \ 00447A70
    room-sounds
    $19 1.0 0 bgm
    $35E story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
    roomD7.cmd02
;

: roomD7.char-enter ( -- )   \ 00447A90
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

: roomD7.phase1 ( -- )   \ 00447AD0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    0 ebit? $35E story-flag? not and if
        roomD7.cond01? if
            $35E story-flag-set
            $F1 action-end
            0 $F1 1 action
        then
    then
;

: roomD7.phase3 ( -- )   \ 00447B00
    roomD7.cond01? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
    else
        roomD7.cmd01
    then then
;

: roomD7.phase2 ( -- )   \ 0047B07C
;

: roomD7.phase5 ( -- )   \ 0047B080
    $E action-end
    $E char-done
;

: roomD7.act00 ( -- )   \ 00447B20
    5 char-unload
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -339.86 125.95 -391.22 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: roomD7.act01 ( -- )   \ 00447B50
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 roomD7.cmd00
    1 roomD7.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

' roomD7.enter $D7 0 room-script!
' roomD7.char-enter $D7 6 room-script!
' roomD7.phase1 $D7 1 room-script!
' roomD7.phase3 $D7 3 room-script!
' roomD7.phase2 $D7 2 room-script!
' roomD7.phase5 $D7 5 room-script!
' roomD7.act00 $D7 $00 action-script!
' roomD7.act01 $D7 $01 action-script!

\ ---- room $D8 ----------------------------------------------------------------------------------

\ as Room109_Cmd01
: roomD8.cmd00 ( b0 -- )  drop s" roomD8.cmd00" stub-step ;
\ room 0xD8: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD8.cmd01 ( -- )  s" roomD8.cmd01" stub-step ;
\ (as Room10A_Cond00) last frame's noise requests (gProgress +0x10D4) of kind 0xD8 / 0xD7 and
\ loudness 0x20 or more
: roomD8.cond00? ( -- flag )  s" roomD8.cond00?" stub-flag ;
\ the progress object's +0x7C with the caller's arguments
: roomD8.cond01? ( -- flag )  s" roomD8.cond01?" stub-flag ;

: roomD8.enter ( -- )   \ 00447BD0
    room-sounds
    $19 1.0 0 bgm
    $35C story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
;

: roomD8.char-enter ( -- )   \ 00447BF0
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
;

: roomD8.phase1 ( -- )   \ 00447CF0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
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
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 5 char-entered-area? 1 6 char-entered-area? or 1 8 char-entered-area? or if
        1 exit-prepare
    then
    1 7 char-entered-area? if
        2 exit-prepare
    then
    1 9 char-entered-area? if
        3 exit-prepare
    then
    0 ebit? $35C story-flag? not and if
        roomD8.cond00? if
            $35C story-flag-set
            $F1 action-end
            0 $F1 1 action
        then
    then
;

: roomD8.phase3 ( -- )   \ 00447DA0
    roomD8.cond01? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
    else
        roomD8.cmd01
    then then
;

: roomD8.phase2 ( -- )   \ 0047B090
;

: roomD8.phase5 ( -- )   \ 0047B098
    $E action-end
    $E char-done
;

: roomD8.act00 ( -- )   \ 00447DC0
    5 char-unload
    0 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -25.61 38.13 -136.96 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: roomD8.act01 ( -- )   \ 00447E00
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 roomD8.cmd00
    1 roomD8.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

' roomD8.enter $D8 0 room-script!
' roomD8.char-enter $D8 6 room-script!
' roomD8.phase1 $D8 1 room-script!
' roomD8.phase3 $D8 3 room-script!
' roomD8.phase2 $D8 2 room-script!
' roomD8.phase5 $D8 5 room-script!
' roomD8.act00 $D8 $00 action-script!
' roomD8.act01 $D8 $01 action-script!

\ ---- room $D9 ----------------------------------------------------------------------------------

\ room 0xD9: starts the countdown clock (clock_start: progress +0x1FBEC1 on, Hewie restarted,
\ the camera director +0x40 14, the time zeroed).
: roomD9.cmd00 ( -- )  s" roomD9.cmd00" stub-step ;
\ room 0xD9: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomD9.cmd01 ( -- )  s" roomD9.cmd01" stub-step ;
\ (as Room48_Cmd04) room 0x48 (Room48_Cmd04_ptmf): the player's Character_ChooseExit(0)
: roomD9.cmd02 ( -- )  s" roomD9.cmd02" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomD9.cond00? ( -- flag )  s" roomD9.cond00?" stub-flag ;

: roomD9.enter ( -- )   \ 00447E80
    $19 1.0 0 bgm
    hewie-controlled? if
        0 ebit-set
    then
;

: roomD9.char-enter ( -- )   \ 00447E90
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
        0 self-is? if
            $80 exit-taken? if
                0 exit-prepare
                roomD9.cmd00
                $2F 0 pvar-set
                0 char-activate
                0 $23 -2.79 -21.69 81 char-to-xz
                0 0 0 char-camera
                1 char-activate
                $D9 0 119 hewie-to-room
                1 $77 -14.67 -14.87 120 char-to-xz
                hewie-controlled? not if
                    0 0 0 char-camera
                    0 camera-follow
                else
                    1 0 0 char-camera
                    1 camera-follow
                then
                $FE char-activate
                $FE $D8 123 2 stalker-to-room
                stalker-item-cooldown
                $2E 0 pvar? if
                    0 music-stage
                else
                    3 music-stage
                then
                2 0 0 music
                4 0 0 music
                roomD9.cmd02
                0 1 0 action
            then
        then
    then
;

: roomD9.phase1 ( -- )   \ 00447F40
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
;

: roomD9.phase3 ( -- )   \ 00447F60
    roomD9.cond00? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
    else
        roomD9.cmd01
    then then
;

: roomD9.act00 ( -- )   \ 00447F78
    self-wait-done
    camera-restart
    8 state-flag-clear
    $F 1 fade
    wait-fade
    0 ebit-set
    self-idle-or-end
;

' roomD9.enter $D9 0 room-script!
' roomD9.char-enter $D9 6 room-script!
' roomD9.phase1 $D9 1 room-script!
' roomD9.phase3 $D9 3 room-script!
' roomD9.act00 $D9 $00 action-script!

\ ---- room $E0 ----------------------------------------------------------------------------------

\ RoomE0_Cmd00
: roomE0.cmd00 ( -- )  s" roomE0.cmd00" stub-step ;
\ RoomE0_Cmd01
: roomE0.cmd01 ( -- )  s" roomE0.cmd01" stub-step ;
\ RoomE0_Cmd02
: roomE0.cmd02 ( b0 -- )  drop s" roomE0.cmd02" stub-step ;
\ RoomE0_Cmd03
: roomE0.cmd03 ( b0 -- )  drop s" roomE0.cmd03" stub-step ;
\ RoomE0_Cmd04
: roomE0.cmd04 ( -- )  s" roomE0.cmd04" stub-step ;
\ RoomE0_Cmd05
: roomE0.cmd05 ( -- )  s" roomE0.cmd05" stub-step ;
\ RoomE0_Cond00
: roomE0.cond00? ( -- flag )  s" roomE0.cond00?" stub-flag ;

: roomE0.enter ( -- )   \ 004480B0
    room-sounds
    0 1 $14 door-bits
    0 $F1 0 action
    1 0 $14 door-bits
    hewie-controlled? if
        6 ebit-set
    then
    0 0 var-set
    1 0 var-set
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 roomE0.cmd02
    0 roomE0.cmd03
;

: roomE0.act03 ( -- )   \ 00448440
    2 ebit-clear
    $30 0 pvar? if
        0 chance? if
            2 ebit-set
        then
    else $30 1 pvar? if
        0 chance? if
            2 ebit-set
        then
    else $30 2 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $30 3 pvar? if
        $32 chance? if
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
    $30 pvar-inc
    exit
;

: roomE0.char-enter ( -- )   \ 004480F0
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
        0 self-is? if
            $80 exit-taken? if
                0 exit-prepare
                roomE0.cmd00
                $30 0 pvar-set
                $31 0 pvar-set
                $32 0 pvar-set
                $33 0 pvar-set
                0 char-activate
                0 $55 -40.16 11.18 180 char-to-xz
                0 0 0 char-camera
                1 char-activate
                $E0 0 276 hewie-to-room
                1 $114 -32.33 16.81 -127 char-to-xz
                hewie-controlled? not if
                    0 0 0 char-camera
                    0 camera-follow
                else
                    1 0 0 char-camera
                    1 camera-follow
                then
                $FE char-activate
                $FE $E1 406 2 stalker-to-room
                stalker-item-cooldown
                $2E 1 pvar? if
                    1 music-stage
                else
                    2 music-stage
                then
                2 0 0 music
                4 0 0 music
                roomE0.cmd05
                $11D door-open-clear
                $11E door-open-clear
                $11F door-open-clear
                $120 door-open-clear
                $121 door-open-clear
                $123 door-open-clear
                $124 door-open-clear
                $125 door-open-clear
                $126 door-open-clear
                $127 door-open-clear
                $128 door-open-clear
                $129 door-open-clear
                $12A door-open-clear
                $126 door-lock
                $127 door-lock
                $128 door-lock
                $129 door-lock
                $12A door-lock
                doors-room-in
                0 1 5 action
            then
        then
    then
    $FE self-is? if
        9 state-flag? if
            5 ebit-set
            roomE0.act03
        else
            5 ebit-clear
        then
    then
;

: roomE0.phase1 ( -- )   \ 00448200
    0 ebit? not if
        6 sound-bank-loaded? if
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            0 ebit-set
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
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
    0 11.0 0.0 -45.0 $A 5 0 zone
    1 0 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 1 action
                    then
                then
            then
        then
    then
;

: roomE0.phase3 ( -- )   \ 00448320
    roomE0.cond00? not if
        7 ebit-set
    else 7 ebit? if
        7 ebit-clear
    else
        roomE0.cmd04
    then then
;

: roomE0.phase5 ( -- )   \ 00448340
    0 char-busy? if
        $18 state-flag-clear
        9 state-flag-clear
        0 action-end
        0 $27 13.59 -38.0 0 char-to-xz
    then
;

: roomE0.act00 ( -- )   \ 0047B0B8
    begin
        roomE0.cmd01
        yield
    again
;

: roomE0.act02 ( -- )   \ 00448410
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
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

: roomE0.act01 ( -- )   \ 00448360
    $18 state-flag-set
    1 self-scripted
    0 1 char-file-load
    self-wait-done
    $27 14.32 -41.52 180 $FFFF $A self-move-to
    self-wait-done
    5 ebit-clear
    0 char-file-use
    $27 $8004 5 14.5 -37.5 180 self-walk-anim
    self-wait-done
    1 self-noclip
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    1 ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            $FF panic-stage? 1 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] roomE0.act02 goto
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
            $FE char-here? if
                ['] roomE0.act02 goto
            then
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

: roomE0.act04 ( -- )   \ 00448490
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
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

: roomE0.act05 ( -- )   \ 004484D0
    self-wait-done
    camera-restart
    8 state-flag-clear
    $F 1 fade
    wait-fade
    6 ebit-set
    self-idle-or-end
;

: roomE0.phase2 ( -- )   \ 0047B0B0
;

' roomE0.enter $E0 0 room-script!
' roomE0.char-enter $E0 6 room-script!
' roomE0.phase1 $E0 1 room-script!
' roomE0.phase3 $E0 3 room-script!
' roomE0.phase5 $E0 5 room-script!
' roomE0.act00 $E0 $00 action-script!
' roomE0.act01 $E0 $01 action-script!
' roomE0.act02 $E0 $02 action-script!
' roomE0.act03 $E0 $03 action-script!
' roomE0.act04 $E0 $04 action-script!
' roomE0.act05 $E0 $05 action-script!
' roomE0.phase2 $E0 2 room-script!

\ ---- room $E1 ----------------------------------------------------------------------------------

\ room 0xE1: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE1.cmd00 ( -- )  s" roomE1.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE1.cond00? ( -- flag )  s" roomE1.cond00?" stub-flag ;

: roomE1.enter ( -- )   \ 00448590
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 6.0 21.8 -3.2 0 effect-86
    1 29.0 41.8 41.9 0 effect-86
    2 -46.8 93.8 18.5 0 effect-86
    1 $2300 sound-volume
;

: roomE1.char-enter ( -- )   \ 004485E0
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

: roomE1.phase1 ( -- )   \ 00448660
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $D 0 0 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 4 3 1 chars-area-camera
    $17 3 -1 1 chars-area-camera
    1 4 char-left-area? 1 5 char-entered-area? or if
        2 exit-prepare
    then
    1 6 char-left-area? if
        1 exit-prepare
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
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
;

: roomE1.phase3 ( -- )   \ 00448740
    -8.0 82.0 -5.0 -8.0 82.0 45.0 -8.0 30.0 -5.0 -8.0 30.0 45.0 lights-doorway
    -5.3 54.0 -4.1 -5.3 54.0 47.0 -58.0 54.0 -4.1 -58.0 54.0 47.0 lights-doorway
    15.0 30.0 -3.0 45.0 30.0 -3.0 15.0 15.0 -3.0 45.0 15.0 -3.0 lights-doorway
    15.0 15.0 -3.0 45.0 15.0 -3.0 15.0 0.0 -3.0 45.0 0.0 -3.0 lights-doorway
    -11.0 30.0 -3.0 15.0 30.0 -3.0 -11.0 15.0 -3.0 15.0 15.0 -3.0 lights-doorway
    -11.0 15.0 -3.0 15.0 15.0 -3.0 -11.0 0.0 -3.0 15.0 0.0 -3.0 lights-doorway
    roomE1.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomE1.cmd00
    then then
;

: roomE1.phase2 ( -- )   \ 0047B0C8
;

: roomE1.act00 ( -- )   \ 0047B0CC
    self-idle-or-end
;

' roomE1.enter $E1 0 room-script!
' roomE1.char-enter $E1 6 room-script!
' roomE1.phase1 $E1 1 room-script!
' roomE1.phase3 $E1 3 room-script!
' roomE1.phase2 $E1 2 room-script!
' roomE1.act00 $E1 $00 action-script!

\ ---- room $E2 ----------------------------------------------------------------------------------

\ room 0xE2: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE2.cmd00 ( -- )  s" roomE2.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE2.cond00? ( -- flag )  s" roomE2.cond00?" stub-flag ;

: roomE2.enter ( -- )   \ 004488B0
    0 1.2 15.55 -24.813 0 effect-86
    1 -1.26 15.55 -24.365 0 effect-86
    2 4.387 15.55 -6.264 0 effect-86
    3 2.327 15.55 -4.824 0 effect-86
    4 -3.559 15.55 -5.985 0 effect-86
    5 -1.887 15.55 -0.304 0 effect-86
    6 1.651 15.55 3.417 0 effect-86
    7 2.473 15.55 5.786 0 effect-86
    8 -2.85 15.55 13.116 0 effect-86
    9 -0.431 15.55 13.794 0 effect-86
    $A 3.347 15.55 19.167 0 effect-86
    $B 1.908 15.55 24.905 0 effect-86
    $C -0.597 15.55 25.104 0 effect-86
    1 0 $14 door-bits
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
;

: roomE2.char-enter ( -- )   \ 00448990
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
;

: roomE2.phase1 ( -- )   \ 00448A10
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 0 0 1 chars-area-camera
    3 1 1 -1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    1 2 char-entered-area? if
        0 exit-prepare
    then
    1 3 char-left-area? if
        1 exit-prepare
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
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    4 4 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    5 5 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 5 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 5 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 5 char-effect-moving
    then
    6 6 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 6 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 6 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 6 char-effect-moving
    then
    7 7 8 -4 0 zone-at-effect
    0 7 3 char-zone-bits? 0 7 3 char-zone-bits-before? not and if
        0 7 char-effect-moving
    then
    1 7 3 char-zone-bits? 1 7 3 char-zone-bits-before? not and if
        1 7 char-effect-moving
    then
    $FE 7 3 char-zone-bits? $FE 7 3 char-zone-bits-before? not and if
        $FE 7 char-effect-moving
    then
    8 8 8 -4 0 zone-at-effect
    0 8 3 char-zone-bits? 0 8 3 char-zone-bits-before? not and if
        0 8 char-effect-moving
    then
    1 8 3 char-zone-bits? 1 8 3 char-zone-bits-before? not and if
        1 8 char-effect-moving
    then
    $FE 8 3 char-zone-bits? $FE 8 3 char-zone-bits-before? not and if
        $FE 8 char-effect-moving
    then
    9 9 8 -4 0 zone-at-effect
    0 9 3 char-zone-bits? 0 9 3 char-zone-bits-before? not and if
        0 9 char-effect-moving
    then
    1 9 3 char-zone-bits? 1 9 3 char-zone-bits-before? not and if
        1 9 char-effect-moving
    then
    $FE 9 3 char-zone-bits? $FE 9 3 char-zone-bits-before? not and if
        $FE 9 char-effect-moving
    then
    $A $A 8 -4 0 zone-at-effect
    0 $A 3 char-zone-bits? 0 $A 3 char-zone-bits-before? not and if
        0 $A char-effect-moving
    then
    1 $A 3 char-zone-bits? 1 $A 3 char-zone-bits-before? not and if
        1 $A char-effect-moving
    then
    $FE $A 3 char-zone-bits? $FE $A 3 char-zone-bits-before? not and if
        $FE $A char-effect-moving
    then
    $B $B 8 -4 0 zone-at-effect
    0 $B 3 char-zone-bits? 0 $B 3 char-zone-bits-before? not and if
        0 $B char-effect-moving
    then
    1 $B 3 char-zone-bits? 1 $B 3 char-zone-bits-before? not and if
        1 $B char-effect-moving
    then
    $FE $B 3 char-zone-bits? $FE $B 3 char-zone-bits-before? not and if
        $FE $B char-effect-moving
    then
    $C $C 8 -4 0 zone-at-effect
    0 $C 3 char-zone-bits? 0 $C 3 char-zone-bits-before? not and if
        0 $C char-effect-moving
    then
    1 $C 3 char-zone-bits? 1 $C 3 char-zone-bits-before? not and if
        1 $C char-effect-moving
    then
    $FE $C 3 char-zone-bits? $FE $C 3 char-zone-bits-before? not and if
        $FE $C char-effect-moving
    then
;

: roomE2.phase3 ( -- )   \ 00448CD0
    roomE2.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomE2.cmd00
    then then
;

: roomE2.phase2 ( -- )   \ 0047B0D4
;

: roomE2.act00 ( -- )   \ 0047B0D8
    self-idle-or-end
;

' roomE2.enter $E2 0 room-script!
' roomE2.char-enter $E2 6 room-script!
' roomE2.phase1 $E2 1 room-script!
' roomE2.phase3 $E2 3 room-script!
' roomE2.phase2 $E2 2 room-script!
' roomE2.act00 $E2 $00 action-script!

\ ---- room $E3 ----------------------------------------------------------------------------------

\ room 0xE3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE3.cmd00 ( -- )  s" roomE3.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE3.cond00? ( -- flag )  s" roomE3.cond00?" stub-flag ;

: roomE3.enter ( -- )   \ 00448D20
    0 -35.8 -50.5 -27.25 0 effect-86
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 0 -19.5 -26.0 -31.0 4.0 4.0 100.0 0.0 10.0 scene-effect-71000
    1 0 -53.0 -22.0 -11.0 4.0 5.0 80.0 0.0 30.0 scene-effect-71000
    3 0 $14 door-bits
    4 0 $14 door-bits
    0 2 $20000 nav-group
    1 $2300 sound-volume
;

: roomE3.act02 ( -- )   \ 00449140
    6 ebit-set
    8 ebit-set
    5 ebit-clear
    $31 0 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $31 1 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $31 2 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $31 3 pvar? if
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
    5 ebit? if
        0 $FE 3 action
    else
        $78 1 item-cooldown
        $B ebit-clear
    then
    $31 pvar-inc
    exit
;

: roomE3.char-enter ( -- )   \ 00448DA0
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
    $FE self-is? if
        9 state-flag? if
            roomE3.act02
        else
            8 ebit-clear
        then
    then
;

: roomE3.phase1 ( -- )   \ 00448E30
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
    1 2 char-entered-area? if
        0 exit-prepare
    then
    1 3 char-entered-area? if
        1 exit-prepare
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
    $FE 2 char-C4? not if
        9 state-flag? 6 ebit? not and $FE char-here? and if
            $B ebit-set
            roomE3.act02
        then
    then
    1 -27.83 -60.0 -33.59 $A 5 0 zone
    1 1 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and if
                -1 control-action? not if
                    $FE char-here? not if
                        3 ebit-clear
                        0 0 0 action
                    then
                then
            then
        then
    then
    2 -44.45 -60.0 -32.32 $A 5 0 zone
    1 2 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        3 ebit-set
                        0 0 0 action
                    then
                then
            then
        then
    then
;

: roomE3.phase3 ( -- )   \ 00448F30
    roomE3.cond00? not if
        $C ebit-set
    else $C ebit? if
        $C ebit-clear
    else
        roomE3.cmd00
    then then
    -19.0 8.0 0.0 13.0 8.0 0.0 -19.0 0.0 0.0 13.0 0.0 0.0 lights-doorway
    -18.1 13.0 -2.7 -16.2 13.0 -2.7 -18.1 0.0 -2.7 -16.2 0.0 -2.7 lights-doorway
    -18.1 13.0 -1.0 -18.1 13.0 -2.7 -18.1 0.0 -1.0 -18.1 0.0 -2.7 lights-doorway
    -19.0 8.0 15.0 -19.0 8.0 0.0 -19.0 0.0 15.0 -19.0 0.0 0.0 lights-doorway
;

: roomE3.phase5 ( -- )   \ 00449010
    0 char-busy? if
        $18 state-flag-clear
        9 state-flag-clear
        0 action-end
        0 $161 -26.17 -25.52 90 char-to-xz
    then
;

: roomE3.act01 ( -- )   \ 00449110
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
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

: roomE3.act00 ( -- )   \ 00449030
    $18 state-flag-set
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    3 ebit? not if
        $14A -24.3 -32.94 -90 $FFFF $A self-move-to
        self-wait-done
    else
        $1FA -48.07 -31.71 90 $FFFF $A self-move-to
        self-wait-done
    then
    8 ebit-clear
    6 ebit-clear
    0 char-file-use
    3 ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
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
        -1 control-action? if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] roomE3.act01 goto
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
                ['] roomE3.act01 goto
            then
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

: roomE3.act03 ( -- )   \ 004491A0
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
    $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    4 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: roomE3.phase2 ( -- )   \ 0047B0E0
;

' roomE3.enter $E3 0 room-script!
' roomE3.char-enter $E3 6 room-script!
' roomE3.phase1 $E3 1 room-script!
' roomE3.phase3 $E3 3 room-script!
' roomE3.phase5 $E3 5 room-script!
' roomE3.act00 $E3 $00 action-script!
' roomE3.act01 $E3 $01 action-script!
' roomE3.act02 $E3 $02 action-script!
' roomE3.act03 $E3 $03 action-script!
' roomE3.phase2 $E3 2 room-script!

\ ---- room $E4 ----------------------------------------------------------------------------------

\ (as Room0F_Cmd00) the room object RoomE4_ObjectNames by event variable 2
: roomE4.cmd00 ( b0 -- )  drop s" roomE4.cmd00" stub-step ;
\ room 0xE4: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE4.cmd01 ( -- )  s" roomE4.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE4.cond00? ( -- flag )  s" roomE4.cond00?" stub-flag ;

: roomE4.enter ( -- )   \ 00449250
    room-sounds
    0 0 $14 door-bits
    1 0 object-show
    2 0 object-show
    3 0 object-show
    4 1 object-show
    5 1 object-show
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 effect-86
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    0 0 var-set
    1 0 var-set
    1 $2300 sound-volume
    0 roomE4.cmd00
;

: roomE4.char-enter ( -- )   \ 004492C0
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

: roomE4.phase1 ( -- )   \ 00449380
    1 exit-usable? if
        1 exit-check
    then
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
    1 5 char-left-area? 1 6 char-entered-area? or if
        3 exit-prepare
    then
    1 7 char-left-area? 1 8 char-entered-area? or if
        1 exit-prepare
    then
    1 9 char-left-area? if
        2 exit-prepare
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -17.0 22.0 -9.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -17.0 22.0 -9.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -17.0 22.0 -9.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -17.0 22.0 -9.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    2 0 var? if
        1 $D char-in-area? if
            2 1 var-set
            1 0 6 char-sound
        else 0 $D char-in-area? if
            2 1 var-set
            0 0 6 char-sound
        else $FE $D char-in-area? if
            2 1 var-set
            $FE 0 6 char-sound
        then then then
    else 1 $D char-in-area? not 0 $D char-in-area? not and $FE $D char-in-area? not and if
        2 0 var-set
    then then
    1 roomE4.cmd00
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
;

: roomE4.phase3 ( -- )   \ 004494F0
    roomE4.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomE4.cmd01
    then then
;

: roomE4.phase2 ( -- )   \ 0047B0E8
;

: roomE4.act00 ( -- )   \ 0047B0EC
    self-idle-or-end
;

' roomE4.enter $E4 0 room-script!
' roomE4.char-enter $E4 6 room-script!
' roomE4.phase1 $E4 1 room-script!
' roomE4.phase3 $E4 3 room-script!
' roomE4.phase2 $E4 2 room-script!
' roomE4.act00 $E4 $00 action-script!

\ ---- room $E5 ----------------------------------------------------------------------------------

\ (as Room14_Cmd00) the same for the room object D_0047B0FC
: roomE5.cmd00 ( b0 -- )  drop s" roomE5.cmd00" stub-step ;
\ room 0xE5: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE5.cmd01 ( -- )  s" roomE5.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE5.cond00? ( -- flag )  s" roomE5.cond00?" stub-flag ;

: roomE5.enter ( -- )   \ 00449570
    room-sounds
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 roomE5.cmd00
    1 $3FFF sound-volume
;

: roomE5.act02 ( -- )   \ 004497A0
    1 ebit-clear
    $32 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $32 1 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $32 2 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $32 3 pvar? if
        $19 chance? if
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
    $32 pvar-inc
    exit
;

: roomE5.char-enter ( -- )   \ 00449590
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
            3 ebit-set
            roomE5.act02
        else
            3 ebit-clear
        then
    then
;

: roomE5.phase1 ( -- )   \ 004495E0
    0 exit-usable? if
        0 exit-check
    then
    4 0 0 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    0 62.08 0.0 -16.33 $A 5 0 zone
    1 0 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 0 action
                    then
                then
            then
        then
    then
;

: roomE5.phase3 ( -- )   \ 00449640
    roomE5.cond00? not if
        4 ebit-set
    else 4 ebit? if
        4 ebit-clear
    else
        roomE5.cmd01
    then then
;

: roomE5.phase5 ( -- )   \ 00449660
    0 char-busy? if
        $18 state-flag-clear
        9 state-flag-clear
        0 action-end
        0 $15 62.35 -10.65 0 char-to-xz
    then
;

: roomE5.act01 ( -- )   \ 00449750
    2 avoid-prompt
    1 self-scripted
    9 state-flag-clear
    1 self-noclip
    0 0 var-set
    2 ebit? if
        counter-inc
    then
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        2 roomE5.cmd00
        yield
        0 28 var? not while
        0 var-inc
    repeat
    3 roomE5.cmd00
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    self-wait-anim
    0 self-noclip
    2 ebit? if
        2 wait-counter
    then
    $FE action-end
    0 self-scripted
    self-idle-or-end
;

: roomE5.act04 ( -- )   \ 00449840
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    9 state-flag-clear
    1 self-noclip
    0 0 var-set
    $8003 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        5 roomE5.cmd00
        yield
        0 18 var? not while
        0 var-inc
    repeat
    3 roomE5.cmd00
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: roomE5.act00 ( -- )   \ 00449680
    $18 state-flag-set
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    $15 60.918 -14.76 180 $FFFF $A self-move-to
    self-wait-done
    3 ebit-clear
    1 self-noclip
    0 char-file-use
    $FF self-look-at
    yield
    0 0 var-set
    $8000 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        0 roomE5.cmd00
        yield
        0 54 var? not while
        0 var-inc
    repeat
    self-wait-anim
    4 roomE5.cmd00
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    0 ebit-clear
    2 ebit-clear
    0 counter-set
    0 avoid-prompt
    begin
        2 ebit? if
            ['] roomE5.act01 goto
        else -1 control-action? if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] roomE5.act04 goto
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
                $FE action-end
                ['] roomE5.act04 goto
            then
            9 state-flag-clear
            1 self-noclip
            0 0 var-set
            $8001 0 self-anim-9
            begin
                1 roomE5.cmd00
                yield
                0 21 var? not while
                0 var-inc
            repeat
            3 roomE5.cmd00
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then then
    again
;

: roomE5.act03 ( -- )   \ 004497F0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $FA $FFFF 6 self-move-tri
    self-wait-done
    $10 58.883 -8.841 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: roomE5.phase2 ( -- )   \ 0047B0F4
;

' roomE5.enter $E5 0 room-script!
' roomE5.char-enter $E5 6 room-script!
' roomE5.phase1 $E5 1 room-script!
' roomE5.phase3 $E5 3 room-script!
' roomE5.phase5 $E5 5 room-script!
' roomE5.act00 $E5 $00 action-script!
' roomE5.act01 $E5 $01 action-script!
' roomE5.act02 $E5 $02 action-script!
' roomE5.act03 $E5 $03 action-script!
' roomE5.act04 $E5 $04 action-script!
' roomE5.phase2 $E5 2 room-script!

\ ---- room $E6 ----------------------------------------------------------------------------------

\ the room object named by RoomE6_ObjectNames[0]: +0x24 -25.3, +0x34 0
: roomE6.cmd00 ( -- )  s" roomE6.cmd00" stub-step ;
\ room 0xE6: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE6.cmd01 ( -- )  s" roomE6.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE6.cond00? ( -- flag )  s" roomE6.cond00?" stub-flag ;

: roomE6.enter ( -- )   \ 004498F0
    1 0 $20000 nav-group
    roomE6.cmd00
    1 1 object-show
    0 1 $14 door-bits
    1 0 $14 door-bits
;

: roomE6.char-enter ( -- )   \ 00449910
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

: roomE6.phase1 ( -- )   \ 00449990
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
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 0 0 1 chars-area-camera
    1 2 char-left-area? if
        0 exit-prepare
    then
    1 3 char-entered-area? if
        1 exit-prepare
    then
;

: roomE6.phase3 ( -- )   \ 004499E0
    roomE6.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomE6.cmd01
    then then
    33.0 23.0 -5.9 45.0 23.0 -5.9 33.0 11.0 -5.9 45.0 11.0 -5.9 lights-doorway
    33.0 11.0 -5.9 45.0 11.0 -5.9 33.0 0.0 -5.9 45.0 0.0 -5.9 lights-doorway
    45.0 23.0 -5.1 33.0 23.0 -5.1 45.0 0.0 -5.1 33.0 0.0 -5.1 lights-doorway
;

: roomE6.phase2 ( -- )   \ 0047B100
;

: roomE6.act00 ( -- )   \ 0047B104
    self-idle-or-end
;

' roomE6.enter $E6 0 room-script!
' roomE6.char-enter $E6 6 room-script!
' roomE6.phase1 $E6 1 room-script!
' roomE6.phase3 $E6 3 room-script!
' roomE6.phase2 $E6 2 room-script!
' roomE6.act00 $E6 $00 action-script!

\ ---- room $E7 ----------------------------------------------------------------------------------

\ room 0xE7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE7.cmd00 ( -- )  s" roomE7.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE7.cond00? ( -- flag )  s" roomE7.cond00?" stub-flag ;

: roomE7.enter ( -- )   \ 00449AD0
    0 34.2 18.3 14.2 1 effect-86
    1 34.2 18.3 -14.2 1 effect-86
    2 -34.2 18.3 -14.2 1 effect-86
    3 -34.2 18.3 14.2 1 effect-86
    0 1 $14 door-bits
    1 $2300 sound-volume
;

: roomE7.act02 ( -- )   \ 00449E00
    1 ebit-clear
    $33 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $33 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $33 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $33 3 pvar? if
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
    $33 pvar-inc
    exit
;

: roomE7.char-enter ( -- )   \ 00449B20
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
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    1 1 0 area-camera
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            roomE7.act02
        else
            2 ebit-clear
        then
    then
;

: roomE7.phase1 ( -- )   \ 00449BB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 0 1 chars-area-camera
    5 0 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 0 -1 chars-area-camera
    1 2 char-entered-area? if
        1 exit-prepare
    then
    1 3 char-entered-area? if
        0 exit-prepare
    then
    4 -0.56 0.0 -21.78 $A 5 0 zone
    1 4 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 0 action
                    then
                then
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
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
;

: roomE7.phase3 ( -- )   \ 00449CE0
    roomE7.cond00? not if
        3 ebit-set
    else 3 ebit? if
        3 ebit-clear
    else
        roomE7.cmd00
    then then
;

: roomE7.phase5 ( -- )   \ 00449D00
    0 char-busy? if
        $18 state-flag-clear
        9 state-flag-clear
        0 action-end
        0 $141 -0.1 -18.54 0 char-to-xz
    then
;

: roomE7.act01 ( -- )   \ 00449DD0
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
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

: roomE7.act00 ( -- )   \ 00449D20
    $18 state-flag-set
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    $141 -0.12 -19.4 180 $FFFF $A self-move-to
    self-wait-done
    2 ebit-clear
    0 char-file-use
    $141 $8004 5 -0.803 -15.664 180 self-walk-anim
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
    begin
        -1 control-action? if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] roomE7.act01 goto
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
                ['] roomE7.act01 goto
            then
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

: roomE7.act03 ( -- )   \ 00449E50
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $B2 0.866 0.417 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: roomE7.phase2 ( -- )   \ 0047B118
;

' roomE7.enter $E7 0 room-script!
' roomE7.char-enter $E7 6 room-script!
' roomE7.phase1 $E7 1 room-script!
' roomE7.phase3 $E7 3 room-script!
' roomE7.phase5 $E7 5 room-script!
' roomE7.act00 $E7 $00 action-script!
' roomE7.act01 $E7 $01 action-script!
' roomE7.act02 $E7 $02 action-script!
' roomE7.act03 $E7 $03 action-script!
' roomE7.phase2 $E7 2 room-script!

\ ---- room $E8 ----------------------------------------------------------------------------------

\ room 0xE8: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: roomE8.cmd00 ( -- )  s" roomE8.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE8.cond00? ( -- flag )  s" roomE8.cond00?" stub-flag ;

: roomE8.char-enter ( -- )   \ 00449EE0
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
;

: roomE8.phase1 ( -- )   \ 00449F60
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 1 1 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 4 4 1 chars-area-camera
    1 4 char-entered-area? if
        1 exit-prepare
    then
    1 5 char-entered-area? if
        2 exit-prepare
    then
;

: roomE8.phase3 ( -- )   \ 00449FB0
    roomE8.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomE8.cmd00
    then then
    30.0 54.1 -51.5 -45.0 54.1 -51.5 30.0 54.1 -90.0 -45.0 54.1 -90.0 lights-doorway
    70.0 54.1 -23.0 57.0 54.1 -56.0 100.0 54.1 -30.0 80.0 54.1 -65.0 lights-doorway
    77.0 54.1 18.0 70.0 54.1 -23.0 100.0 54.1 30.0 100.0 54.1 -30.0 lights-doorway
    52.0 54.1 48.0 77.0 54.1 18.0 82.0 54.1 65.0 100.0 54.1 30.0 lights-doorway
    -78.0 54.1 -20.0 -77.0 54.1 20.0 -100.0 54.1 -25.0 -100.0 54.1 20.0 lights-doorway
    -60.0 54.1 -50.0 -78.0 54.1 -20.0 -80.0 54.1 -60.0 -100.0 54.1 -25.0 lights-doorway
    65.0 20.0 18.0 75.0 20.0 13.0 65.0 0.0 18.0 75.0 0.0 13.0 lights-doorway
    74.5 20.0 -19.5 68.5 20.0 -24.5 74.5 0.0 -19.5 68.5 0.0 -24.5 lights-doorway
    53.0 20.0 -48.0 53.0 20.0 -60.0 53.0 0.0 -48.0 53.0 0.0 -60.0 lights-doorway
    -55.0 20.0 -62.0 -55.0 20.0 -47.0 -55.0 0.0 -62.0 -55.0 0.0 -47.0 lights-doorway
    -70.0 20.0 -26.0 -78.0 20.0 -22.0 -70.0 0.0 -26.0 -78.0 0.0 -22.0 lights-doorway
    -78.0 20.0 20.0 -69.0 20.0 25.0 -78.0 0.0 20.0 -69.0 0.0 25.0 lights-doorway
    -55.0 20.0 47.0 -55.0 20.0 54.0 -55.0 0.0 47.0 -55.0 0.0 54.0 lights-doorway
    52.0 20.0 57.0 52.0 20.0 47.0 52.0 0.0 57.0 52.0 0.0 47.0 lights-doorway
;

: roomE8.enter ( -- )   \ 0047B120
    1 $2300 sound-volume
;

: roomE8.phase2 ( -- )   \ 0047B128
;

: roomE8.act00 ( -- )   \ 0047B12C
    self-idle-or-end
;

' roomE8.char-enter $E8 6 room-script!
' roomE8.phase1 $E8 1 room-script!
' roomE8.phase3 $E8 3 room-script!
' roomE8.enter $E8 0 room-script!
' roomE8.phase2 $E8 2 room-script!
' roomE8.act00 $E8 $00 action-script!

\ ---- room $E9 ----------------------------------------------------------------------------------

\ room 0xE9: stops the countdown clock (clock_stop: progress +0x1FBEC1 off, the camera director
\ +0x40 -1).
: roomE9.cmd00 ( -- )  s" roomE9.cmd00" stub-step ;
\ room 0xE9: saves the countdown clock's time in script variables 0..2 (clock_save).
: roomE9.cmd01 ( -- )  s" roomE9.cmd01" stub-step ;
\ (as Room00_Cmd00) the same four spots for bytes 3..6
: roomE9.cmd02 ( b0 b1 -- )  drop drop s" roomE9.cmd02" stub-step ;
\ room 0xE9: draws the countdown clock, frozen at the saved time while event flag 1 is set
\ (clock_draw_saved; a frame hook).
: roomE9.cmd03 ( -- )  s" roomE9.cmd03" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: roomE9.cond00? ( -- flag )  s" roomE9.cond00?" stub-flag ;

: roomE9.enter ( -- )   \ 0044A2C0
    $2E 1 pvar? if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $80C0F2AA 0 1 screen-blend
        3 1 $14 door-bits
        4 1 $14 door-bits
        5 1 $14 door-bits
        6 1 $14 door-bits
        7 1 $14 door-bits
        8 1 $14 door-bits
        3 0 roomE9.cmd02
        4 0 roomE9.cmd02
        5 0 roomE9.cmd02
        6 0 roomE9.cmd02
        $10 -368.0 100.0 55.0 $A $80 $80 $80 $40 specks
        $A -332.0 100.8 -12.0 $E $80 $80 $80 $40 specks
        $10 -332.0 100.8 165.0 $E $80 $80 $80 $40 specks
        $10 -368.0 100.8 165.0 $C $80 $80 $80 $40 specks
        $A -327.5 82.0 236.3 $E $80 $80 $80 $50 specks
        $A -372.5 82.0 236.3 $E $80 $80 $80 $50 specks
    then
    0 0 $14 door-bits
;

: roomE9.char-enter ( -- )   \ 0044A380
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
;

: roomE9.phase1 ( -- )   \ 0044A3C0
    0 exit-usable? if
        0 exit-check
    then
    6 1 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 2 0 1 chars-area-camera
    9 0 -1 1 chars-area-camera
    $2E 3 pvar? if
        0 $C char-entered-area? if
            $1E chance? if
                0 $F8 0 action
            then
        then
        0 $C char-left-area? if
            $1E chance? if
                0 $F8 0 action
            then
        then
        0 $D char-entered-area? if
            $1E chance? if
                0 $F7 0 action
            then
        then
        0 $D char-left-area? if
            $1E chance? if
                0 $F7 0 action
            then
        then
        0 $E char-entered-area? if
            $1E chance? if
                0 $F6 0 action
            then
        then
        0 $E char-left-area? if
            $1E chance? if
                0 $F5 0 action
            then
        then
        $14 chance? if
            1 chance? if
                0 $F8 0 action
            else 1 chance? if
                0 $F7 0 action
            else 1 chance? if
                0 $F6 0 action
            else 1 chance? if
                0 $F5 0 action
            then then then then
        then
    then
    1 ebit? not if
        0 $10 char-in-area? if
            1 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 1 action-force
            0 1 2 action-force
        then
    then
;

: roomE9.phase3 ( -- )   \ 0044A470
    camera-setup-changed? if
        2 camera-mode? if
            1.0 1.0 100.0 200.0 depth-range
        else
            depth-range-off
        then
    then
    -370.0 76.0 178.0 -370.0 76.0 109.0 -370.0 20.0 178.0 -370.0 20.0 109.0 lights-doorway
    2 camera-mode? if
        -380.0 74.0 250.0 -368.0 74.0 175.0 -380.0 22.0 250.0 -368.0 22.0 175.0 lights-doorway
    then
    -370.0 76.0 110.0 -370.0 76.0 41.0 -370.0 20.0 110.0 -370.0 20.0 41.0 lights-doorway
    -370.0 76.0 38.0 -370.0 76.0 -8.0 -370.0 20.0 39.0 -370.0 20.0 -8.0 lights-doorway
    roomE9.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        roomE9.cmd03
    then then
;

: roomE9.act00 ( -- )   \ 0044A570
    $F8 self-is? if
        3 0 $14 door-bits
        3 1 roomE9.cmd02
        yield
        3 1 $14 door-bits
        3 0 roomE9.cmd02
        yield
        3 0 $14 door-bits
        3 1 roomE9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            3 1 $14 door-bits
            3 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            3 1 roomE9.cmd02
            yield
            3 1 $14 door-bits
            3 0 roomE9.cmd02
            yield
            3 0 $14 door-bits
            3 1 roomE9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            3 1 $14 door-bits
            3 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            3 1 roomE9.cmd02
            yield
            3 1 $14 door-bits
            3 0 roomE9.cmd02
            yield
            3 0 $14 door-bits
            3 1 roomE9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            3 1 $14 door-bits
            3 0 roomE9.cmd02
            yield
            3 0 $14 door-bits
            3 1 roomE9.cmd02
            yield
            3 1 $14 door-bits
            3 0 roomE9.cmd02
            yield
            3 0 $14 door-bits
            3 1 roomE9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        3 1 $14 door-bits
        3 0 roomE9.cmd02
        yield
        3 0 $14 door-bits
        3 1 roomE9.cmd02
        yield
        3 1 $14 door-bits
        3 0 roomE9.cmd02
    else $F7 self-is? if
        4 0 $14 door-bits
        4 1 roomE9.cmd02
        yield
        4 1 $14 door-bits
        4 0 roomE9.cmd02
        yield
        4 0 $14 door-bits
        4 1 roomE9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            4 1 $14 door-bits
            4 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            4 1 roomE9.cmd02
            yield
            4 1 $14 door-bits
            4 0 roomE9.cmd02
            yield
            4 0 $14 door-bits
            4 1 roomE9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            4 1 $14 door-bits
            4 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            4 1 roomE9.cmd02
            yield
            4 1 $14 door-bits
            4 0 roomE9.cmd02
            yield
            4 0 $14 door-bits
            4 1 roomE9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            4 1 $14 door-bits
            4 0 roomE9.cmd02
            yield
            4 0 $14 door-bits
            4 1 roomE9.cmd02
            yield
            4 1 $14 door-bits
            4 0 roomE9.cmd02
            yield
            4 0 $14 door-bits
            4 1 roomE9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        4 1 $14 door-bits
        4 0 roomE9.cmd02
        yield
        4 0 $14 door-bits
        4 1 roomE9.cmd02
        yield
        4 1 $14 door-bits
        4 0 roomE9.cmd02
    else $F6 self-is? if
        5 0 $14 door-bits
        5 1 roomE9.cmd02
        yield
        5 1 $14 door-bits
        5 0 roomE9.cmd02
        yield
        5 0 $14 door-bits
        5 1 roomE9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            5 1 $14 door-bits
            5 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            5 1 roomE9.cmd02
            yield
            5 1 $14 door-bits
            5 0 roomE9.cmd02
            yield
            5 0 $14 door-bits
            5 1 roomE9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            5 1 $14 door-bits
            5 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            5 1 roomE9.cmd02
            yield
            5 1 $14 door-bits
            5 0 roomE9.cmd02
            yield
            5 0 $14 door-bits
            5 1 roomE9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            5 1 $14 door-bits
            5 0 roomE9.cmd02
            yield
            5 0 $14 door-bits
            5 1 roomE9.cmd02
            yield
            5 1 $14 door-bits
            5 0 roomE9.cmd02
            yield
            5 0 $14 door-bits
            5 1 roomE9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        5 1 $14 door-bits
        5 0 roomE9.cmd02
        yield
        5 0 $14 door-bits
        5 1 roomE9.cmd02
        yield
        5 1 $14 door-bits
        5 0 roomE9.cmd02
    else $F5 self-is? if
        6 0 $14 door-bits
        6 1 roomE9.cmd02
        yield
        6 1 $14 door-bits
        6 0 roomE9.cmd02
        yield
        6 0 $14 door-bits
        6 1 roomE9.cmd02
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            6 1 $14 door-bits
            6 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            6 1 roomE9.cmd02
            yield
            6 1 $14 door-bits
            6 0 roomE9.cmd02
            yield
            6 0 $14 door-bits
            6 1 roomE9.cmd02
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            6 1 $14 door-bits
            6 0 roomE9.cmd02
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            6 1 roomE9.cmd02
            yield
            6 1 $14 door-bits
            6 0 roomE9.cmd02
            yield
            6 0 $14 door-bits
            6 1 roomE9.cmd02
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            6 1 $14 door-bits
            6 0 roomE9.cmd02
            yield
            6 0 $14 door-bits
            6 1 roomE9.cmd02
            yield
            6 1 $14 door-bits
            6 0 roomE9.cmd02
            yield
            6 0 $14 door-bits
            6 1 roomE9.cmd02
            self-frames-reset
            $20 self-wait-frames
        then
        6 1 $14 door-bits
        6 0 roomE9.cmd02
        yield
        6 0 $14 door-bits
        6 1 roomE9.cmd02
        yield
        6 1 $14 door-bits
        6 0 roomE9.cmd02
    then then then then
    self-idle-or-end
;

: roomE9.act01 ( -- )   \ 0044A8C0
    $18 state-flag-set
    $E state-flag-set
    1 self-scripted
    roomE9.cmd01
    $34 1.0 1 bgm
    0 $1E 0 music
    self-frames-reset
    $1E self-wait-frames
    self-wait-done
    begin
        0 adx? not while
        yield
    repeat
    0 0.0 $FF bgm
    begin
        1 adx? not while
        yield
    repeat
    $37 room-preload
    $C subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    9 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    $F $50 fade
    wait-fade
    roomE9.cmd00
    $37 0 -1 hewie-to-room
    $10C door-open-clear
    2 0 char-remove
    $18 state-flag-clear
    $81 exit-check
    self-idle-or-end
;

: roomE9.act02 ( -- )   \ 0047B138
    1 self-scripted
    self-wait-done
    begin
        yield
    again
;

: roomE9.phase2 ( -- )   \ 0047B134
;

' roomE9.enter $E9 0 room-script!
' roomE9.char-enter $E9 6 room-script!
' roomE9.phase1 $E9 1 room-script!
' roomE9.phase3 $E9 3 room-script!
' roomE9.act00 $E9 $00 action-script!
' roomE9.act01 $E9 $01 action-script!
' roomE9.act02 $E9 $02 action-script!
' roomE9.phase2 $E9 2 room-script!
