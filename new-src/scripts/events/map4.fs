\ events/map4.fs - the event scripts of the rooms on the game's map 4 (kMapRooms).
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.map4
USING: events.core events.words events.builtin ;

\ ---- room $81 ----------------------------------------------------------------------------------

: room81.enter ( -- )   \ 00430AB0
    room-sounds
    $99 story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
        0 1 8 nav-group
        1 0 8 nav-group
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
        0 0 8 nav-group
        1 1 8 nav-group
    then
    0 -24.6 22.1 -26.4 1 effect-86
    $2E9 story-flag? $2EA story-flag? not and if
        1 -78.0 1.0 39.0 flicker-sprite
    then
    $99 story-flag? if
        5 ebit-set
    then
    $AF story-flag? if
        $94 story-flag? $AC story-flag? not and if
            $AC story-flag-set
            \ (nop-progress-74: no effect in this game)
            \ (nop-progress-74: no effect in this game)
        then
    then
    1 $51 $10000000 nav-tri-flags
    1 $21C $10000000 nav-tri-flags
    1 $2F $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $23B $10000000 nav-tri-flags
    1 $43 $10000000 nav-tri-flags
    1 $22C $10000000 nav-tri-flags
    1 $44 $10000000 nav-tri-flags
    1 $22D $10000000 nav-tri-flags
    1 $45 $10000000 nav-tri-flags
    1 $22E $10000000 nav-tri-flags
    1 $46 $10000000 nav-tri-flags
    1 $22F $10000000 nav-tri-flags
    1 $40 $10000000 nav-tri-flags
    1 $229 $10000000 nav-tri-flags
    1 $3D $10000000 nav-tri-flags
    1 $226 $10000000 nav-tri-flags
    1 $3A $10000000 nav-tri-flags
    1 $224 $10000000 nav-tri-flags
    1 $28 $10000000 nav-tri-flags
    1 $216 $10000000 nav-tri-flags
    1 $2D $10000000 nav-tri-flags
    1 $21A $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
    1 $21E $10000000 nav-tri-flags
    1 $31 $10000000 nav-tri-flags
    1 $56 $10000000 nav-tri-flags
    1 $23D $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $234 $10000000 nav-tri-flags
;

: room81.char-enter ( -- )   \ 00430C40
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    2 5 5 area-camera
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
        0 exit-taken? 1 exit-taken? or 2 exit-taken? or 4 exit-taken? or if
            2 map-page
        then
    then
;

: room81.phase1 ( -- )   \ 00430D80
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
    hewie-controlled? not if
        0 $17 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 $17 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 4 4 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 5 5 1 chars-area-camera
    $11 2 2 1 chars-area-camera
    $12 2 2 1 chars-area-camera
    $13 2 2 1 chars-area-camera
    $14 6 6 1 chars-area-camera
    $15 6 6 1 chars-area-camera
    $16 6 6 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $E char-entered-area? 0 $10 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $18 char-entered-area? 0 $D char-entered-area? or 0 $F char-entered-area? or 0 5 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 $1D char-entered-area? if
        1 map-page
    then
    0 $1D char-left-area? if
        2 map-page
    then
    0 $1B char-entered-area? if
        $99 story-flag? not if
            0 game-mode? not $24 1 item-count? and if
                $99 story-flag-set
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
    $2E9 story-flag? not if
        0 -78.0 0.0 39.0 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 745 var-set
                $1A 746 var-set
                $1B 1 var-set
                $1C -78000 var-set
                $1D 1000 var-set
                $1E 39000 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    3 ebit? not if
        6 sound-bank-loaded? if
            $40000002 6 -24.6 22.1 -26.4 0 0 sound
            3 ebit-set
        then
    else
        $C0000002 6 -24.6 22.1 -26.4 0 0 sound
    then
    5 ebit? if
        4 ebit? not if
            6 sound-bank-loaded? if
                $40000006 6 -21.86 0.0 33.51 0 0 sound
                4 ebit-set
            then
        else
            $C0000006 6 -21.86 0.0 33.51 0 0 sound
        then
    then
;

: room81.phase2 ( -- )   \ 00430F30
    0 $19 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    $99 story-flag? not if
        0 $1A $32 char-faces-area? if
            5 2 0 scene-change
        then
        0 $1B char-in-area? 0 45 $32 char-heading? and if
            5 3 0 scene-change
        then
    else 0 $1C char-in-area? 0 30 $32 char-heading? and 2 ebit? and if
        5 4 0 scene-change
    then then
    $2E9 story-flag? $2EA story-flag? not and if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room81.phase5 ( -- )   \ 00430F90
    2 ebit? if
        $1B state-flag-clear
        $FE action-end
        $FE action-end
        3 summon-take
        $23 state-flag-clear
        $21 state-flag-clear
    then
;

: room81.phase3 ( -- )   \ 00430FB0
    14.0 74.0 -180.0 14.0 74.0 -160.0 14.0 59.0 -180.0 14.0 59.0 -160.0 lights-doorway
    14.0 74.0 -160.0 10.0 74.0 -80.0 10.0 39.0 -160.0 7.0 39.0 -80.0 lights-doorway
    10.0 73.0 -85.5 43.0 73.0 -85.5 10.0 40.0 -85.5 43.0 40.0 -85.5 lights-doorway
    -43.0 73.0 -85.5 -10.0 73.0 -85.5 -43.0 40.0 -85.5 -10.0 40.0 -85.5 lights-doorway
    -70.0 55.0 -60.2 -38.0 78.0 -60.2 -70.0 40.0 -60.2 -38.0 60.0 -60.2 lights-doorway
;

: room81.act00 ( -- )   \ 004310B0
    0 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $117 0.0 -1.1 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        3 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        0 $84 5 char-sound
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
        0.0 5.0 self-turn-to-xz
        self-wait-done
        4 message
        wait-message
    then
    self-idle-or-end
;

: room81.act01 ( -- )   \ 00431170
    $12 state-flag-set
    $18 state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    wait-fade
    1 char-here? if
        1 ebit-set
        1 action-end
        1 char-done
    then
    $FE action-end
    $FE char-done
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
    0 $F9 5 action
    $10 $FF movie-param
    0 effect-remove
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
    0 $EF -71.08 32.24 50 char-to-xz
    1 ebit? if
        1 char-activate
        1 $20F -2.73 -54.08 70 char-to-xz
        1 ebit-clear
    then
    $FE action-end
    $FE char-done
    $FE 3 char-file-load
    $FE char-file-use
    0 0 8 nav-group
    1 1 $1000010 nav-group
    $FE $81 457 2 stalker-to-room
    $FE $1C9 -53.111 33.905 -82 char-to-xz
    $FE char-activate
    $FE 1 char-silent
    0 $FE 7 action
    $23 state-flag-set
    $21 state-flag-set
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    0 0 $14 door-bits
    1 1 $14 door-bits
    $E0 door-unlock
    $24 item-use
    2 ebit-set
    0 state-flag-clear
    $1B state-flag-set
    0 -24.6 22.1 -26.4 1 effect-86
    5 ebit-set
    $F $51 fade
    wait-fade
    $44 resident-flag-set
    $18 state-flag-clear
    $12 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room81.act02 ( -- )   \ 004312C0
    self-wait-done
    -32.0 34.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room81.act03 ( -- )   \ 004312E0
    self-wait-done
    $10C -30.0 -26.0 90 $FFFF 5 self-move-to
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room81.act04 ( -- )   \ 00431300
    self-wait-done
    -53.0 34.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    2 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room81.act05 ( -- )   \ 00431320
    begin
        6 cutscene-shot? not while
        yield
    repeat
    0 0 $14 door-bits
    1 1 $14 door-bits
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room81.act06 ( -- )   \ 00431340
    self-wait-done
    -78.0 39.0 self-turn-to-xz
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
            $2EA story-flag-set
            1 effect-remove
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

: room81.act07 ( -- )   \ 004313A0
    1 self-scripted
    self-wait-done
    $FE 0 char-silent
    $FF 0 char-visible
    $8000 self-anim
    begin
        yield
    again
;

: room81.act08 ( -- )   \ 004313B0
    self-wait-done
    0 1 $14 door-bits
    $B 3 $FF char-load
    3 char-unload
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
    0 $F9 5 action
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room81.enter $81 0 room-script!
' room81.char-enter $81 6 room-script!
' room81.phase1 $81 1 room-script!
' room81.phase2 $81 2 room-script!
' room81.phase5 $81 5 room-script!
' room81.phase3 $81 3 room-script!
' room81.act00 $81 $00 action-script!
' room81.act01 $81 $01 action-script!
' room81.act02 $81 $02 action-script!
' room81.act03 $81 $03 action-script!
' room81.act04 $81 $04 action-script!
' room81.act05 $81 $05 action-script!
' room81.act06 $81 $06 action-script!
' room81.act07 $81 $07 action-script!
' room81.act08 $81 $08 action-script!

\ ---- room $82 ----------------------------------------------------------------------------------

: room82.enter ( -- )   \ 004314B0
    $297 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $2FA story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $2FC story-flag? not if
            7 -17.0 71.0 -41.0 flicker-sprite
        then
    then
    $2FB story-flag? not if
        1 2 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    0 5 0.812 0.687 0.187 0.312 zone-rect
    0 -102.0 65.4 -31.4 4 effect-86
    1 -43.2 46.0 -31.2 1 effect-86
    2 -72.4 17.2 -1.75 1 effect-86
    3 -102.0 66.0 87.6 4 effect-86
    4 -43.0 46.0 87.7 1 effect-86
    5 -72.4 17.2 117.2 1 effect-86
    8 -101.8 66.0 -31.0 4 effect-86
    9 -101.8 66.0 88.0 4 effect-86
    $2EB story-flag? $2EC story-flag? not and if
        6 -50.0 71.0 -15.0 flicker-sprite
    then
;

: room82.char-enter ( -- )   \ 004315E0
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
                0 6 6 char-camera
                0 camera-follow
            else
                1 6 6 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 6 6 char-camera
            0 camera-follow
        else
            1 6 6 char-camera
            1 camera-follow
        then
    then then
    3 6 6 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    4 5 5 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $C3 -9.0 95.0 180 char-to-xz
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            $82 1 194 hewie-to-room
            1 $C2 0 char-to-tri-facing
            0 0 3 action
        else
            room-sounds
        then
    then
;

: room82.phase1 ( -- )   \ 00431750
    $94 story-flag? if
        0 exit-usable? if
            0 exit-check
        then
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
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 4 4 1 chars-area-camera
    $F 5 5 1 chars-area-camera
    $10 6 6 1 chars-area-camera
    $11 7 7 1 chars-area-camera
    $12 3 3 1 chars-area-camera
    $13 7 7 1 chars-area-camera
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $94 story-flag? not if
        0 0 char-entered-area? 2 ebit? not and if
            2 ebit-set
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
    $297 story-flag? not if
        0 -57.4 70.0 114.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $297 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -57.4 70.0 114.5 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -57.4 70.0 114.5 0 0 sound
            $40 $103 noise
        then
    then
    $2FB story-flag? not if
        9 -3.0 70.0 -41.0 5 8 1 zone
        $FF 9 char-in-zone? if
            $2FB story-flag-set
            0 2 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -3.0 70.0 -41.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -3.0 70.0 -41.0 0 0 sound
            $40 $86 noise
            \ (nop-progress-74: no effect in this game)
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
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    5 4 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    6 5 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 5 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 5 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 5 char-effect-moving
    then
    $2EB story-flag? not if
        7 -50.0 70.0 -15.0 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 747 var-set
                $1A 748 var-set
                $1B 6 var-set
                $1C -50000 var-set
                $1D 71000 var-set
                $1E -15000 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
;

: room82.phase2 ( -- )   \ 00431A60
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 1 1 scene-change
        then
    then
    0 $14 char-in-area? 0 45 $32 char-heading? and if
        5 4 0 scene-change
    then
    $2EB story-flag? $2EC story-flag? not and if
        7 6 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    $2FA story-flag? not if
        8 -17.0 70.0 -41.0 5 8 1 zone
        $FF 8 char-in-zone? if
            $2FA story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -17.0 70.0 -41.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -17.0 70.0 -41.0 0 0 sound
            $40 $95 noise
            7 -17.0 71.0 -41.0 flicker-sprite
        then
    else $2FC story-flag? not if
        8 7 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then then
;

: room82.act00 ( -- )   \ 00431B20
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    0 creatures-clear
    $12 state-flag-set
    \ (nop-progress-14: no effect in this game)
    0 self-scripted
    $80 exit-check
    self-idle-or-end
;

: room82.act01 ( -- )   \ 00431B40
    self-wait-done
    0 self-through-exit
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
            0 message
            wait-message
        then
        1 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $26E -10.0 135.3 0 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 0.0 0.0 2.0 event-camera
        self-frames-reset
        4 self-wait-frames
        1 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        1 ebit-clear
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room82.act02 ( -- )   \ 00431BD0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 3 6 char-sound
    $25 message-param-room
    $E6 door-locked? not if
        $25 item-use
    then
    $E2 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room82.act03 ( -- )   \ 00431C10
    1 self-scripted
    self-wait-done
    camera-restart
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
    $E6 door-lock
    $EA door-lock
    $103 door-lock
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    $FE char-activate
    $FE $82 622 2 stalker-to-room
    $FE $26E 180 char-to-tri-facing
    $FE 1 1 char-camera
    0 state-flag-set
    stalker-item-cooldown
    room-sounds
    $F $41 fade
    wait-fade
    $24F item-give
    $12 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room82.act04 ( -- )   \ 00431CC0
    self-wait-done
    0 $FB -34.0 99.0 90 char-to-xz
    1 46.0 10.0 150.0 0.0 event-camera
    2 message
    wait-message
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: room82.act05 ( -- )   \ 00431CF0
    self-wait-done
    -50.0 -15.0 self-turn-to-xz
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
            $2EC story-flag-set
            6 effect-remove
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

: room82.act06 ( -- )   \ 00431D50
    self-wait-done
    -17.0 -41.0 self-turn-to-xz
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
            $2FC story-flag-set
            7 effect-remove
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

' room82.enter $82 0 room-script!
' room82.char-enter $82 6 room-script!
' room82.phase1 $82 1 room-script!
' room82.phase2 $82 2 room-script!
' room82.act00 $82 $00 action-script!
' room82.act01 $82 $01 action-script!
' room82.act02 $82 $02 action-script!
' room82.act03 $82 $03 action-script!
' room82.act04 $82 $04 action-script!
' room82.act05 $82 $05 action-script!
' room82.act06 $82 $06 action-script!

\ ---- room $83 ----------------------------------------------------------------------------------

: room83.enter ( -- )   \ 00431DE0
    0 1 $14 door-bits
    0 1 object-show
    1 1 object-show
    2 1 object-show
    $343 story-flag? not if
        1 1 $14 door-bits
    then
    $2EF story-flag? $2F0 story-flag? not and if
        0 37.0 1.0 84.0 flicker-sprite
    then
;

: room83.char-enter ( -- )   \ 00431E20
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
    0 self-is? if
        $80 exit-taken? if
            0 $81 0 char-to-tri-facing
            $82 1 -1 hewie-to-room
            8 state-flag-set
            0 0 0 action
        then
    then
;

: room83.phase1 ( -- )   \ 00431E80
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    $2EF story-flag? not if
        0 37.0 0.0 84.0 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 751 var-set
                $1A 752 var-set
                $1B 0 var-set
                $1C 37000 var-set
                $1D 1000 var-set
                $1E 84000 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
;

: room83.phase2 ( -- )   \ 00431EF0
    $343 story-flag? not if
        0 3 char-in-area? 0 52 $32 char-heading? and if
            5 1 4 scene-change
        then
    then
    0 4 char-in-area? 0 -34 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 5 char-in-area? 0 32 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 6 char-in-area? 0 -3 $32 char-heading? and if
        5 5 0 scene-change
    then
    0 7 char-in-area? 0 46 $32 char-heading? and if
        5 6 0 scene-change
    then
    0 9 char-in-area? 0 -35 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 8 char-in-area? 0 -41 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 $A char-in-area? 0 -24 $32 char-heading? and if
        5 9 0 scene-change
    then
    $2EF story-flag? $2F0 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    0 $B char-in-area? 0 12 $32 char-heading? and if
        5 $B 0 scene-change
    then
;

: room83.phase3 ( -- )   \ 00431F90
    40.0 20.0 -9.0 17.0 20.0 -9.0 40.0 0.0 -9.0 23.2 0.0 -9.0 lights-doorway
    -20.0 16.0 40.0 3.0 16.0 40.0 -20.0 0.0 40.0 3.0 0.0 40.0 lights-doorway
    30.0 16.0 32.4 45.0 16.0 32.4 30.0 0.0 32.4 45.0 0.0 32.4 lights-doorway
;

: room83.act00 ( -- )   \ 00432030
    1 self-scripted
    $FE action-end
    $FE char-done
    $10 3 $FF char-load
    $B partner-load
    0 0 $14 door-bits
    self-wait-done
    0 $28 5 char-sound
    3 char-unload
    2 char-unload
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $A action
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
    3 0 char-remove
    $FE action-end
    $FE char-done
    0 self-scripted
    $94 story-flag-set
    0 0 char-no-shadow
    $80 exit-check
    $E2 door-open-clear
    $E2 door-lock
    $43 resident-flag-set
    \ (nop-progress-4C: no effect in this game)
    \ (nop-progress-48: no effect in this game)
    2 0 0 music
    3 0 0 music
    4 0 0 music
    self-idle-or-end
;

: room83.act01 ( -- )   \ 00432100
    self-wait-done
    40.0 54.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $343 story-flag-set
    1 0 $14 door-bits
    $27 message-param-room
    $27 1 item-give-count
    0 $27 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room83.act02 ( -- )   \ 00432150
    self-wait-done
    $9F 5.3 80.92 -68 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 8.0 0.0 0.0 4.5 event-camera
    1 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room83.act03 ( -- )   \ 004321A0
    self-wait-done
    37.0 84.0 self-turn-to-xz
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
            $2F0 story-flag-set
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

: room83.act04 ( -- )   \ 0047AE00
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: room83.act05 ( -- )   \ 004321F8
    self-wait-done
    $A02 $A self-anim-blend
    3 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room83.act06 ( -- )   \ 0047AE08
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room83.act07 ( -- )   \ 0047AE10
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room83.act08 ( -- )   \ 00432210
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 ebit? not if
        4 message
        wait-message
        0 ebit-set
    else
        $A02 self-anim
        9 message
        wait-message
        self-wait-anim
        $F 6 fade
        wait-fade
        $A message
        wait-message
        $F 7 fade
        wait-fade
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room83.act09 ( -- )   \ 00432250
    self-wait-done
    $2F6 story-flag? not if
        5 message
        wait-message
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F6 story-flag-set
            $73 1 item-give-count
            0 $73 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: room83.act0A ( -- )   \ 004322B0
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: room83.act0B ( -- )   \ 004322D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    25 self-turn-angle
    self-wait-done
    6 message
    wait-message
    $F 6 fade
    wait-fade
    7 message
    wait-message
    $F 7 fade
    wait-fade
    $4B subscreen-bit? not if
        $1D01 self-anim
        8 message
        wait-message
        self-wait-anim
        $4B subscreen-bit
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room83.act0C ( -- )   \ 00432310
    self-wait-done
    $B 3 $FF char-load
    3 char-unload
    $10 4 $FF char-load
    4 char-unload
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $A action
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
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room83.enter $83 0 room-script!
' room83.char-enter $83 6 room-script!
' room83.phase1 $83 1 room-script!
' room83.phase2 $83 2 room-script!
' room83.phase3 $83 3 room-script!
' room83.act00 $83 $00 action-script!
' room83.act01 $83 $01 action-script!
' room83.act02 $83 $02 action-script!
' room83.act03 $83 $03 action-script!
' room83.act04 $83 $04 action-script!
' room83.act05 $83 $05 action-script!
' room83.act06 $83 $06 action-script!
' room83.act07 $83 $07 action-script!
' room83.act08 $83 $08 action-script!
' room83.act09 $83 $09 action-script!
' room83.act0A $83 $0A action-script!
' room83.act0B $83 $0B action-script!
' room83.act0C $83 $0C action-script!

\ ---- room $84 ----------------------------------------------------------------------------------

\ room 0x84: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room84.cmd00 ( -- )  s" room84.cmd00" stub-step ;

: room84.enter ( -- )   \ 00432420
    0 1 $14 door-bits
    0 1 object-show
    1 1 object-show
    2 1 object-show
;

: room84.char-enter ( -- )   \ 00432440
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
        $80 exit-taken? if
            0 $2A 0 char-to-tri-facing
            8 state-flag-set
            0.0 sound-volume-scale
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 0 action-force
        then
    then
;

: room84.phase1 ( -- )   \ 004324A0
    0 exit-usable? if
        0 exit-check
    then
    room84.cmd00
;

: room84.phase2 ( -- )   \ 0047AE18
;

: room84.act00 ( -- )   \ 004324B0
    $18 state-flag-set
    1 self-scripted
    $FE action-end
    $FE char-done
    self-wait-done
    9 3 $FF char-load
    3 char-unload
    0 0 $14 door-bits
    4 0 movie-play
    3 cutscene-start
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
    3 0 char-remove
    $FE action-end
    $FE char-done
    0 $25 -1.96 -22.63 0 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    $95 story-flag-set
    1 0 0 music
    $EF door-unlock
    $EF door-open-set
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
    $FE char-activate
    $FE $84 44 2 stalker-to-room
    $FE $2C -140 char-to-tri-facing
    $FE 0 0 char-camera
    0 state-flag-set
    stalker-item-cooldown
    1 action-end
    1 char-done
    1 char-activate
    $85 0 82 hewie-to-room
    0 1 $14 door-bits
    0 1 object-show
    1 1 object-show
    2 1 object-show
    $F $51 fade
    wait-fade
    $4B resident-flag-set
    $253 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room84.act01 ( -- )   \ 00432620
    self-wait-done
    9 3 $FF char-load
    $A 4 $FF char-load
    3 char-unload
    4 char-unload
    4 0 movie-play
    3 cutscene-start
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
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room84.enter $84 0 room-script!
' room84.char-enter $84 6 room-script!
' room84.phase1 $84 1 room-script!
' room84.phase2 $84 2 room-script!
' room84.act00 $84 $00 action-script!
' room84.act01 $84 $01 action-script!

\ ---- room $85 ----------------------------------------------------------------------------------

\ room 0x85: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room85.cmd00 ( -- )  s" room85.cmd00" stub-step ;

: room85.enter ( -- )   \ 00432700
    room-sounds
    0 0 var-set
    1 0 var-set
    $2E3 story-flag? not if
        0 -45.77 5.75 19.99 flicker-sprite
    then
;

: room85.char-enter ( -- )   \ 00432730
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
    0 self-is? if
        $98 story-flag? not $80 exit-taken? and if
            8 state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 1 action-force
        then
    then
;

: room85.phase1 ( -- )   \ 004327D0
    0 exit-usable? if
        0 exit-check
    then
    $95 story-flag? if
        1 exit-usable? if
            1 exit-check
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
    7 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    room85.cmd00
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 -41.0 20.0 25.0 0 0 sound
            else 0 1 var? if
                $40000004 6 -41.0 20.0 25.0 0 0 sound
            else 0 2 var? if
                $40000005 6 -41.0 20.0 25.0 0 0 sound
            else 0 3 var? if
                $40000006 6 -41.0 20.0 25.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: room85.phase2 ( -- )   \ 004328A0
    0 8 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    $95 story-flag? not if
        0 1 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 3 1 scene-change
        then
    then
    $2E3 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: room85.act00 ( -- )   \ 004328E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $F $54 fade
    wait-fade
    $12 state-flag-set
    \ (nop-progress-14: no effect in this game)
    $18 state-flag-clear
    0 self-scripted
    $80 exit-check
    $12 state-flag-clear
    self-idle-or-end
;

: room85.act01 ( -- )   \ 00432910
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
    0 $27 -0.18 14.47 -79 char-to-xz
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    $EF door-open-clear
    $E9 door-lock
    $EF door-lock
    doors-room-in
    $98 story-flag-set
    $F $41 fade
    wait-fade
    $4A resident-flag-set
    $252 item-give
    $12 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room85.act02 ( -- )   \ 004329D0
    self-wait-done
    -45.77 19.99 self-turn-to-xz
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
            $2E3 story-flag-set
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

: room85.act03 ( -- )   \ 00432A30
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
        1 message
        wait-message
    then
    self-idle-or-end
;

: room85.act04 ( -- )   \ 00432A60
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room85.enter $85 0 room-script!
' room85.char-enter $85 6 room-script!
' room85.phase1 $85 1 room-script!
' room85.phase2 $85 2 room-script!
' room85.act00 $85 $00 action-script!
' room85.act01 $85 $01 action-script!
' room85.act02 $85 $02 action-script!
' room85.act03 $85 $03 action-script!
' room85.act04 $85 $04 action-script!

\ ---- room $86 ----------------------------------------------------------------------------------

\ room 0x86: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room86.cmd00 ( -- )  s" room86.cmd00" stub-step ;

: room86.enter ( -- )   \ 0042C770
    $A7 story-flag? not if
        0 state-flag-clear
        $A7 story-flag-set
        $FE action-end
        1 summon-take
    then
    1 $2300 sound-volume
;

: room86.char-enter ( -- )   \ 0042C790
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
;

: room86.phase1 ( -- )   \ 0042C810
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
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    room86.cmd00
;

: room86.act00 ( -- )   \ 0047ADB0
    self-idle-or-end
;

' room86.enter $86 0 room-script!
' room86.char-enter $86 6 room-script!
' room86.phase1 $86 1 room-script!
' room86.act00 $86 $00 action-script!

\ ---- room $87 ----------------------------------------------------------------------------------

\ room 0x87: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room87.cmd00 ( -- )  s" room87.cmd00" stub-step ;

: room87.enter ( -- )   \ 00432B10
    $350 story-flag? not if
        room-sounds
        $27 state-flag-set
    then
    $345 story-flag-clear
    $346 story-flag-clear
    $347 story-flag-clear
    $348 story-flag-clear
    $F1 door-open-clear
    $F2 door-open-clear
    $104 door-open-clear
    $105 door-open-clear
    $106 door-open-clear
    $107 door-open-clear
    $108 door-open-clear
    $109 door-open-clear
    $10A door-open-clear
    $10B door-open-clear
    doors-room-in
    $354 story-flag? if
        $FE $87 2 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
;

: room87.char-enter ( -- )   \ 00432B80
    0 self-is? if
        $80 exit-taken? if
            0 $129 129.0 0.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $87 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: room87.phase1 ( -- )   \ 00432BC0
    0 ebit? not if
        0 0 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-18: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-18: no effect in this game)
    then
    builtin.act98
    room87.cmd00
;

: room87.phase2 ( -- )   \ 00432C10
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: room87.act00 ( -- )   \ 00432C20
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room87.act01 ( -- )   \ 00432C30
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room87.act02 ( -- )   \ 00432C40
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    $350 story-flag? not if
        $350 story-flag-set
    else
        $254 item-give
    then
    0 self-scripted
    self-idle-or-end
;

: room87.act03 ( -- )   \ 00432C60
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
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

: room87.phase5 ( -- )   \ 0047AE38
    $12 state-flag-clear
;

' room87.enter $87 0 room-script!
' room87.char-enter $87 6 room-script!
' room87.phase1 $87 1 room-script!
' room87.phase2 $87 2 room-script!
' room87.act00 $87 $00 action-script!
' room87.act01 $87 $01 action-script!
' room87.act02 $87 $02 action-script!
' room87.act03 $87 $03 action-script!
' room87.phase5 $87 5 room-script!

\ ---- room $88 ----------------------------------------------------------------------------------

\ room 0x88: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room88.cmd00 ( -- )  s" room88.cmd00" stub-step ;

: room88.enter ( -- )   \ 00432CA0
    room-sounds
    $298 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2E8 story-flag? not if
            1 -0.4 71.0 -123.2 flicker-sprite
        then
    then
    0 1 0.812 0.687 0.187 0.312 zone-rect
    $2E6 story-flag? not if
        0 -33.17 71.0 33.19 flicker-sprite
    then
    $2F1 story-flag? $2F2 story-flag? not and if
        2 35.0 71.0 33.0 flicker-sprite
    then
    1 $51 $10000000 nav-tri-flags
    1 $21C $10000000 nav-tri-flags
    1 $2F $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $23B $10000000 nav-tri-flags
    1 $43 $10000000 nav-tri-flags
    1 $22C $10000000 nav-tri-flags
    1 $350 $10000000 nav-tri-flags
    1 $41 $10000000 nav-tri-flags
    1 $22D $10000000 nav-tri-flags
    1 $354 $10000000 nav-tri-flags
    1 $232 $10000000 nav-tri-flags
    1 $355 $10000000 nav-tri-flags
    1 $34C $10000000 nav-tri-flags
    1 $35C $10000000 nav-tri-flags
    1 $351 $10000000 nav-tri-flags
    1 $48 $10000000 nav-tri-flags
    1 $235 $10000000 nav-tri-flags
    1 $356 $10000000 nav-tri-flags
    1 $234 $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $23D $10000000 nav-tri-flags
    1 $56 $10000000 nav-tri-flags
    1 $31 $10000000 nav-tri-flags
    1 $21E $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
;

: room88.char-enter ( -- )   \ 00432DF0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 6 6 char-camera
                0 camera-follow
            else
                1 6 6 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 6 6 char-camera
            0 camera-follow
        else
            1 6 6 char-camera
            1 camera-follow
        then
    then then
    0 6 6 area-camera
    0 self-is? if
        $FE exit-taken? if
            0 $28 -60.0 42.0 180 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
        $80 exit-taken? if
            0 $3F -23.0 25.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $88 0 64 hewie-to-room
            1 $40 -90 char-to-tri-facing
            0 0 3 action
        then
    then
;

: room88.phase1 ( -- )   \ 00432E90
    2 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 -60.0 20.0 50.0 0 0 sound
            2 ebit-set
        then
    else
        $C0000008 6 -60.0 20.0 50.0 0 0 sound
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000009 6 -78.0 21.0 46.5 0 0 sound
            else 0 1 var? if
                $4000000A 6 -78.0 21.0 46.5 0 0 sound
            else 0 2 var? if
                $4000000B 6 -78.0 21.0 46.5 0 0 sound
            else 0 3 var? if
                $4000000C 6 -78.0 21.0 46.5 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    4 0 0 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 2 2 1 chars-area-camera
    7 3 3 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 5 5 1 chars-area-camera
    $10 5 5 1 chars-area-camera
    $11 5 5 1 chars-area-camera
    $17 0 0 1 chars-area-camera
    $18 7 7 1 chars-area-camera
    $19 3 3 1 chars-area-camera
    $1A 6 6 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-18: no effect in this game)
    then
    0 $15 char-entered-area? 0 $16 char-entered-area? or if
        1 map-page
    then
    0 $15 char-left-area? 0 $16 char-left-area? or if
        2 map-page
    then
    1 -61.4 0.0 46.49 $14 18 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    1 -61.4 0.0 46.49 $14 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    0 0 char-action? $FF panic-stage? not and 0 1 char-entered-area? and if
        0 0 7 action
    then
    $2F1 story-flag? not if
        3 35.0 70.0 33.0 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 753 var-set
                $1A 754 var-set
                $1B 2 var-set
                $1C 35000 var-set
                $1D 71000 var-set
                $1E 33000 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    room88.cmd00
;

: room88.phase2 ( -- )   \ 004330C0
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $13 char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $14 char-in-area? 0 0 $32 char-heading? and if
        5 1 0 scene-change
    then
    $2E6 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $298 story-flag? not if
        0 -0.4 70.0 -123.2 5 8 1 zone
        $FF 0 char-in-zone? if
            $298 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -0.4 70.0 -123.2 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -0.4 70.0 -123.2 0 0 sound
            $40 $34E noise
            1 -0.4 71.0 -123.2 flicker-sprite
        then
    else $2E8 story-flag? not if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
    $2F1 story-flag? $2F2 story-flag? not and if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room88.phase3 ( -- )   \ 004331B0
    -20.0 76.0 -132.0 20.0 76.0 -132.0 -20.0 50.0 -132.0 20.0 50.0 -132.0 lights-doorway
    38.0 78.0 -60.2 70.0 55.0 -60.2 38.0 60.0 -60.2 70.0 40.0 -60.2 lights-doorway
    10.0 73.0 -85.5 43.0 73.0 -85.5 10.0 40.0 -85.5 43.0 40.0 -85.5 lights-doorway
    -43.0 73.0 -85.5 -10.0 73.0 -85.5 -43.0 40.0 -85.5 -10.0 40.0 -85.5 lights-doorway
    -70.0 55.0 -60.2 -38.0 78.0 -60.2 -70.0 40.0 -60.2 -38.0 60.0 -60.2 lights-doorway
;

: room88.act00 ( -- )   \ 004332B0
    0 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $117 0.0 -1.1 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        0 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        0 $84 5 char-sound
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
        0.0 5.0 self-turn-to-xz
        self-wait-done
        1 message
        wait-message
    then
    self-idle-or-end
;

: room88.act01 ( -- )   \ 00433370
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -60.0 55.0 self-turn-to-xz
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

: room88.act02 ( -- )   \ 004333B0
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    1 map-page
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room88.act03 ( -- )   \ 004333D0
    1 self-scripted
    self-wait-done
    1 map-page
    \ (nop-progress-18: no effect in this game)
    $F $41 fade
    wait-fade
    0 state-flag-set
    0 self-scripted
    self-idle-or-end
;

: room88.act04 ( -- )   \ 004333F0
    self-wait-done
    -33.17 33.19 self-turn-to-xz
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
            $2E6 story-flag-set
            0 effect-remove
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

: room88.act05 ( -- )   \ 00433450
    self-wait-done
    -0.4 -123.2 self-turn-to-xz
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
            $2E8 story-flag-set
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

: room88.act06 ( -- )   \ 004334B0
    self-wait-done
    35.0 33.0 self-turn-to-xz
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
            $2F2 story-flag-set
            2 effect-remove
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

: room88.act07 ( -- )   \ 00433510
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    3 message
    wait-message
    self-idle-or-end
;

' room88.enter $88 0 room-script!
' room88.char-enter $88 6 room-script!
' room88.phase1 $88 1 room-script!
' room88.phase2 $88 2 room-script!
' room88.phase3 $88 3 room-script!
' room88.act00 $88 $00 action-script!
' room88.act01 $88 $01 action-script!
' room88.act02 $88 $02 action-script!
' room88.act03 $88 $03 action-script!
' room88.act04 $88 $04 action-script!
' room88.act05 $88 $05 action-script!
' room88.act06 $88 $06 action-script!
' room88.act07 $88 $07 action-script!

\ ---- room $8A ----------------------------------------------------------------------------------

: room8A.enter ( -- )   \ 00433580
    $9D story-flag? not if
        0 1 $14 door-bits
    then
    0 -42.5 45.4 -36.9 1 effect-86
    1 -72.4 17.2 -7.1 1 effect-86
    2 -42.5 45.4 87.3 1 effect-86
    3 -72.4 17.2 117.2 1 effect-86
    $2E2 story-flag? not if
        4 -23.41 1.0 -14.78 flicker-sprite
    then
    $2ED story-flag? $2EE story-flag? not and if
        5 3.0 1.0 168.0 flicker-sprite
    then
    1 $2300 sound-volume
;

: room8A.char-enter ( -- )   \ 00433600
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
;

: room8A.phase1 ( -- )   \ 00433700
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
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $97 story-flag? not if
        0 9 char-left-area? if
            $97 story-flag-set
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
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    $9D story-flag? not if
        0 -61.25 0.0 -4.27 $11 15 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    $2ED story-flag? not if
        2 3.0 0.0 168.0 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 749 var-set
                $1A 750 var-set
                $1B 5 var-set
                $1C 3000 var-set
                $1D 1000 var-set
                $1E 168000 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
;

: room8A.phase2 ( -- )   \ 004338F0
    $9D story-flag? not if
        0 $A char-in-area? 0 0 $32 char-heading? and if
            5 1 4 scene-change
        then
    then
    0 $B char-in-area? 0 45 $32 char-heading? and if
        5 2 0 scene-change
    then
    $2E2 story-flag? not if
        1 4 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $2ED story-flag? $2EE story-flag? not and if
        2 5 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: room8A.act00 ( -- )   \ 00433950
    $18 state-flag-set
    $FE action-end
    $FE char-done
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    1 char-here? if
        0 ebit-set
        1 action-end
        1 char-done
    then
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
    0 $BC -12.31 49.96 0 char-to-xz
    0 ebit? if
        1 char-activate
        1 $1E4 -15.99 63.61 0 char-to-xz
        0 ebit-clear
    then
    $FE char-activate
    $FE $8A 480 2 stalker-to-room
    $FE $1E0 -11.74 29.25 0 char-to-xz
    $FE 0 0 char-camera
    0 state-flag-set
    stalker-item-cooldown
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
    $45 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    $97 story-flag-set
    self-idle-or-end
;

: room8A.act01 ( -- )   \ 00433A50
    self-wait-done
    -62.5 -1.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    0 0 $14 door-bits
    $9D story-flag-set
    $AF story-flag? not if
        $24E item-add
    then
    $24 message-param-room
    $24 1 item-give-count
    0 $24 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    $AF story-flag? not if
        $C $85 0.0 0.0 0.0 0 0 sound
    then
    self-idle-or-end
;

: room8A.act02 ( -- )   \ 00433AC0
    self-wait-done
    0 $4F -33.0 -24.0 90 char-to-xz
    1 42.0 10.0 -160.0 0.0 event-camera
    0 message
    wait-message
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: room8A.act03 ( -- )   \ 00433AF0
    self-wait-done
    -23.41 -14.78 self-turn-to-xz
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
            $2E2 story-flag-set
            4 effect-remove
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

: room8A.act04 ( -- )   \ 00433B50
    self-wait-done
    3.0 168.0 self-turn-to-xz
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
            $2EE story-flag-set
            5 effect-remove
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

: room8A.act05 ( -- )   \ 00433BB0
    self-wait-done
    0 -42.5 45.4 -36.9 1 effect-86
    1 -72.4 17.2 -7.1 1 effect-86
    2 -42.5 45.4 87.3 1 effect-86
    3 -72.4 17.2 117.2 1 effect-86
    $B 3 $FF char-load
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

' room8A.enter $8A 0 room-script!
' room8A.char-enter $8A 6 room-script!
' room8A.phase1 $8A 1 room-script!
' room8A.phase2 $8A 2 room-script!
' room8A.act00 $8A $00 action-script!
' room8A.act01 $8A $01 action-script!
' room8A.act02 $8A $02 action-script!
' room8A.act03 $8A $03 action-script!
' room8A.act04 $8A $04 action-script!
' room8A.act05 $8A $05 action-script!

\ ---- room $8C ----------------------------------------------------------------------------------

\ (as RoomC7_Cmd01)
: room8C.cmd00 ( b0 -- )  drop s" room8C.cmd00" stub-step ;
\ the room object named pstr_dynamo swung: byte 3 0 starts it (rest +0x30 from +0x20, phase
\ +0x34 0, amplitude +0x3C 1); 1 steps the phase back 60 degrees and the amplitude down 0.25,
\ height +0x28 = +0x38 + amplitude * sin, waiting (2) until it has died out
: room8C.cmd01 ( b0 -- )  drop s" room8C.cmd01" stub-step ;
\ a turning machine: the wheel pstr_roller (angle +0x18, height +0x24 5.1) driven by the belt
\ pstr_belt (offset +0x20 wrapping at 10, height +0x24 -3, speed +0x30). Byte 3 0 sets it up
\ (speed 0.4); 1 runs it a frame (both shaking by up to 0.05); 2 also slows it by 0.01, waiting
\ (2) until it stops. (The wheel's wrap steps +0x10, not the angle.)
: room8C.cmd02 ( b0 -- )  drop s" room8C.cmd02" stub-step ;
\ Hewie's +0x14C8 to script variable 2 (byte 3 0), or back from it (1; 0 there gives 10)
: room8C.cmd03 ( b0 -- )  drop s" room8C.cmd03" stub-step ;
\ the placed things of kinds 0, 2, 3, 5, 7 and 8 the event manager finds in area 0xB (+0x10):
\ their timer (+0xE4) to 300000
: room8C.cmd04 ( -- )  s" room8C.cmd04" stub-step ;

: room8C.enter ( -- )   \ 00433CC0
    room-sounds
    $9A story-flag? not $9B story-flag? not and if
        0 3 var-set
        7 1 object-show
        8 0 object-show
        9 0 object-show
        $A 1 object-show
        $B 1 object-show
        $C 0 object-show
        $D 0 object-show
        $E 0 object-show
        $F 0 object-show
        $10 0 object-show
        $11 0 object-show
        $12 0 object-show
        $13 0 object-show
        $14 0 object-show
        $15 0 object-show
        $16 0 object-show
    else
        7 1 object-show
        8 0 object-show
        9 0 object-show
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
        $9C story-flag? not if
            0 0.0 -2.2 65.0 flicker-sprite
        then
    then
    0 0 $14 door-bits
    $9B story-flag? not if
        7 state-flag-set
    then
    1 $2300 sound-volume
;

: room8C.char-enter ( -- )   \ 00433D80
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
;

: room8C.phase1 ( -- )   \ 00433E00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 3 3 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $9B story-flag? not if
        1 ebit? not if
            0 50.0 0.0 -28.0 5 11 0 zone
            0 0 char-in-zone? if
                0 $F1 1 action
            then
        then
    then
    $9A story-flag? $9B story-flag? not and if
        1 ebit? 2 ebit? not and if
            $FE $B char-in-area? if
                2 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 4 action-force
                else
                    1 0 4 action-force
                then
            else 0 $B char-entered-area? if
                2 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 3 action-force
                else
                    1 0 3 action-force
                then
            then then
        then
        3 ebit? if
            0 6 char-entered-area? if
                0 room8C.cmd03
                1 action-end
                1 char-done
                1 char-activate
                1 room8C.cmd03
                room8C.cmd04
                $21 chance? if
                    1 $146 char-to-tri
                else $32 chance? if
                    1 $130 char-to-tri
                else
                    1 $11C char-to-tri
                then then
                1 0 $1000000 nav-group
                3 ebit-clear
            else 1 0 char-in-nav-group? not if
                1 0 $1000000 nav-group
                3 ebit-clear
            then then
        then
        7 ebit? if
            0 6 char-entered-area? if
                room8C.cmd04
                7 ebit-clear
            then
        then
    then
    3 44.93 0.0 -26.49 $1B 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 10.0 hewie-look-zone
            then
        then
    then
    1 ebit? if
        $C0000002 6 24.0 5.0 65.0 0 0 sound
    then
;

: room8C.phase2 ( -- )   \ 00433F50
    0 $A char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    -2147483646 scene-request? if
        5 7 1 scene-change
    then
    $9B story-flag? $9C story-flag? not and if
        1 0.0 -3.2 65.0 5 5 0 zone
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    $9A story-flag? not if
        0 $C $32 char-faces-area? if
            5 $A 0 scene-change
        then
    then
;

: room8C.act00 ( -- )   \ 00433FA0
    self-wait-done
    50.0 -32.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    $9B story-flag? not if
        0 ebit? not if
            0 message
            wait-message
            0 ebit-set
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room8C.act01 ( -- )   \ 00433FD0
    0 0 6 char-sound
    1 $FF 4 rumble
    0 room8C.cmd00
    0 room8C.cmd01
    1 room8C.cmd01
    0 0 var? if
        $9A story-flag? not if
            begin
                fiona-free? not while
                yield
            repeat
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 2 action-force
            else
                1 0 2 action-force
            then
        else
            0 $F2 6 action
        then
    then
    self-frames-reset
    $1E self-wait-frames
    self-idle-or-end
;

: room8C.act02 ( -- )   \ 00434020
    7 state-flag-clear
    $12 state-flag-set
    $18 state-flag-set
    1 self-scripted
    room8C.cmd04
    self-frames-reset
    1 self-wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
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
    0 $F9 9 action
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
    0 $106 46.98 -19.5 -44 char-to-xz
    1 char-activate
    1 $53 23.54 2.5 -94 char-to-xz
    $FE char-activate
    $FE $8C 103 2 stalker-to-room
    $FE $67 -69.97 2.34 98 char-to-xz
    $FE 3 3 char-camera
    $E5 door-open-clear
    1 0 self-door-knock
    doors-room-in
    $E5 door-lock
    $10 state-flag-set
    $FE char-full-health
    6 0 object-show
    7 1 object-show
    8 0 object-show
    9 0 object-show
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
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    $9A story-flag-set
    $F $51 fade
    wait-fade
    $46 resident-flag-set
    $250 item-give
    $18 state-flag-clear
    $12 state-flag-clear
    0 self-scripted
    0 $F2 6 action
    self-idle-or-end
;

: room8C.act03 ( -- )   \ 00434180
    $2C state-flag-set
    $19 state-flag-set
    $12 state-flag-set
    1 self-scripted
    room8C.cmd04
    self-frames-reset
    1 self-wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    $F2 action-end
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
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
    $47 resident-flag-set
    0 game-over-flag
    $C state-flag-set
    1.0 sound-volume-scale
    self-idle-or-end
;

: room8C.act04 ( -- )   \ 00434200
    $12 state-flag-set
    1 self-scripted
    room8C.cmd04
    self-frames-reset
    1 self-wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    $40000001 6 sound-stop
    $F2 action-end
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
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
    $64 $FF movie-param
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
    3 summon-take
    0 state-flag-clear
    $E5 door-unlock
    0 $7A -10.04 37.76 18 char-to-xz
    1 char-activate
    1 $9D 6.01 27.57 18 char-to-xz
    50 hewie-trust
    $9B story-flag-set
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    6 0 object-show
    7 1 object-show
    8 0 object-show
    9 0 object-show
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
    0 0.0 -2.2 65.0 flicker-sprite
    0 0 $14 door-bits
    \ (nop-progress-14: no effect in this game)
    $F $51 fade
    wait-fade
    $48 resident-flag-set
    0 0 $1000000 nav-group
    $12 state-flag-clear
    $10 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room8C.act05 ( -- )   \ 00434350
    1 self-scripted
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 effect-remove
    $25 message-param-room
    $25 1 item-give-count
    0 $25 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    $9C story-flag-set
    0 self-scripted
    self-idle-or-end
;

: room8C.act06 ( -- )   \ 004343A0
    0 1 $14 door-bits
    $40000002 6 24.0 5.0 65.0 0 0 sound
    1 ebit-set
    0 1 $14 door-bits
    3 ebit-set
    7 ebit-set
    1 0 var-set
    0 room8C.cmd02
    begin
        1 room8C.cmd02
        self-frames-reset
        1 self-wait-frames
        1 var-inc
        1 240 var? until
    $40000001 6 24.0 5.0 65.0 0 0 sound
    1 ebit-clear
    0 0 $14 door-bits
    2 room8C.cmd02
    0 0 $1000000 nav-group
    0 3 var-set
    0 0 $14 door-bits
    self-idle-or-end
;

: room8C.act07 ( -- )   \ 00434410
    $10 state-flag? if
        3 message
    else
        self-wait-done
        0 self-through-exit
        self-wait-done
        4 ebit? not 2 game-mode? or $FF panic-stage? or if
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
                4 message
                wait-message
            then
            4 ebit-set
        else
            self-frames-reset
            4 self-wait-frames
            0 $BF 90.5 0.0 90 char-to-xz
            $17 state-flag-set
            1 self-scripted
            1 20.0 0.0 0.0 2.0 event-camera
            self-frames-reset
            4 self-wait-frames
            5 message
            wait-message
            self-frames-reset
            4 self-wait-frames
            0 0.0 0.0 0.0 0.0 event-camera
            $17 state-flag-clear
            0 self-scripted
            4 ebit-clear
        then
        $251 item-give
    then
    self-idle-or-end
;

: room8C.act08 ( -- )   \ 004344B0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 3 6 char-sound
    $25 message-param-room
    $E2 door-locked? not if
        $25 item-use
    then
    $E6 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room8C.act09 ( -- )   \ 004344F0
    0 0 $14 door-bits
    begin
        $F cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $12 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $13 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $14 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $17 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $28 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $29 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $2A cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $2B cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $2C cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $31 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $37 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $38 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $39 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $3A cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $3B cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $3C cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $53 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $54 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $63 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room8C.act0A ( -- )   \ 00434600
    self-wait-done
    6 ebit? not if
        6 message
        wait-message
        6 ebit-set
    else
        $1D01 $A self-anim-blend
        self-wait-anim
        7 message
        wait-message
    then
    self-idle-or-end
;

: room8C.act0B ( -- )   \ 00434620
    self-wait-done
    7 1 object-show
    $A 1 object-show
    $B 1 object-show
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
    0 $F9 9 action
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

: room8C.act0C ( -- )   \ 004346B0
    self-wait-done
    7 1 object-show
    $A 1 object-show
    $B 1 object-show
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

: room8C.act0D ( -- )   \ 00434740
    self-wait-done
    7 1 object-show
    $A 1 object-show
    $B 1 object-show
    $B 3 $FF char-load
    3 char-unload
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
    $64 $FF movie-param
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

: room8C.phase5 ( -- )   \ 0047AE50
    7 state-flag-clear
;

' room8C.enter $8C 0 room-script!
' room8C.char-enter $8C 6 room-script!
' room8C.phase1 $8C 1 room-script!
' room8C.phase2 $8C 2 room-script!
' room8C.act00 $8C $00 action-script!
' room8C.act01 $8C $01 action-script!
' room8C.act02 $8C $02 action-script!
' room8C.act03 $8C $03 action-script!
' room8C.act04 $8C $04 action-script!
' room8C.act05 $8C $05 action-script!
' room8C.act06 $8C $06 action-script!
' room8C.act07 $8C $07 action-script!
' room8C.act08 $8C $08 action-script!
' room8C.act09 $8C $09 action-script!
' room8C.act0A $8C $0A action-script!
' room8C.act0B $8C $0B action-script!
' room8C.act0C $8C $0C action-script!
' room8C.act0D $8C $0D action-script!
' room8C.phase5 $8C 5 room-script!

\ ---- room $8D ----------------------------------------------------------------------------------

\ room 0x8D: three grey smoke effects (Effect79B00, size 60) at the room's spots 7, 4, 3
\ (grey_three).
: room8D.cmd00 ( -- )  s" room8D.cmd00" stub-step ;
\ byte 3: 0 made (lights 0x14 on characters 3 and 0) and 1 lit on character 3 (second kind, size
\ 1, sparks from its own slot); 2 ending, 3 ended; 4 / 6 sized 1 / 0.5; 5 / 7 shrinking with
\ variable 1 (a step a call, to 0.5 + v / 200 or v / 334)
: room8D.cmd01 ( b0 -- )  drop s" room8D.cmd01" stub-step ;
\ room 0x8D: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room8D.cmd02 ( -- )  s" room8D.cmd02" stub-step ;

: room8D.enter ( -- )   \ 004348E0
    room-sounds
    2 0 var-set
    3 0 var-set
    $A4 story-flag? $96 story-flag? not and if
        1 ebit-set
    then
    1 ebit? if
        $18 state-flag-set
    then
    $A4 story-flag? if
        1 0 $20000 nav-group
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $96 story-flag? if
        2 1 $14 door-bits
    then
    $34E story-flag? if
        5 1.0 0 bgm
    then
    builtin.act95
    $A3 story-flag? if
        room8D.cmd00
    then
    1 $2300 sound-volume
;

: room8D.char-enter ( -- )   \ 00434940
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
;

: room8D.phase1 ( -- )   \ 00434A00
    3 ebit? not if
        $A4 story-flag? if
            2 ebit? not if
                6 sound-bank-loaded? if
                    $40000000 6 0.0 20.0 100.0 0 0 sound
                    2 ebit-set
                then
            else
                $C0000000 6 -60.0 20.0 50.0 0 0 sound
            then
        then
    then
    4 ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $B char-entered-area? if
        1 ebit? if
            1 ebit-clear
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
    room8D.cmd02
    3 ebit? not if
        6 sound-bank-loaded? if
            3 var-inc
            3 30 var? if
                3 0 var-set
                2 0 var? if
                    $40000003 6 28.0 20.0 -71.0 0 0 sound
                else 2 1 var? if
                    $40000004 6 28.0 20.0 -71.0 0 0 sound
                else 2 2 var? if
                    $40000005 6 28.0 20.0 -71.0 0 0 sound
                else 2 3 var? if
                    $40000006 6 28.0 20.0 -71.0 0 0 sound
                then then then then
                2 var-inc
                2 4 var? if
                    2 0 var-set
                then
            then
        then
    then
;

: room8D.phase2 ( -- )   \ 00434B50
    0 $A char-in-area? 0 67 $32 char-heading? and if
        $96 story-flag? not if
            5 $84 3 scene-change
        else
            5 6 0 scene-change
        then
    then
    0 $C char-in-area? 0 0 $32 char-heading? and if
        $96 story-flag? if
            5 1 0 scene-change
        else
            5 2 0 scene-change
        then
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            $96 story-flag? not if
                5 3 1 scene-change
            else
                5 6 0 scene-change
            then
        then
        0 1 char-group-bit4? if
            $96 story-flag? not if
                5 4 1 scene-change
            else
                5 6 0 scene-change
            then
        then
        0 2 char-group-bit4? if
            $96 story-flag? if
                5 6 0 scene-change
            then
        then
    then
    $96 story-flag? if
        0 0.0 0.0 74.25 8 5 0 zone
        0 0 8 char-zone-bits? if
            5 8 0 scene-change
        then
    then
;

: room8D.phase3 ( -- )   \ 00434BE0
    $A3 story-flag? 4 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        4 ebit-set
    then
;

: room8D.phase5 ( -- )   \ 00434C00
    1 ebit? if
        $18 state-flag-clear
    then
;

: room8D.act00 ( -- )   \ 00434C10
    1 self-scripted
    $34E story-flag-clear
    $FF 1.0 0 bgm
    $F8 action-end
    $F $14 fade
    self-wait-done
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    $1E 3 $FF char-load
    $1F 4 $FF char-load
    7 state-flag-set
    3 char-unload
    4 char-unload
    0 1 $14 door-bits
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
    0 $F9 5 action
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
    2 room8D.cmd01
    3 0 char-remove
    4 0 char-remove
    0 $63 -0.61 47.71 0 char-to-xz
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    1 char-activate
    $8D 0 123 hewie-to-room
    1 $7B -20 char-to-tri-facing
    $13 state-flag-clear
    2 1 $14 door-bits
    $EA door-open-clear
    $EA door-lock
    doors-room-in
    3 0.0 0.0 65.0 $80 $80 $80 1 dust
    $F $11 fade
    wait-fade
    $51 resident-flag-set
    $257 item-give
    $828E item-give
    $18 state-flag-clear
    $96 story-flag-set
    1 ebit-clear
    builtin.act95
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    0 self-scripted
    self-idle-or-end
;

: room8D.act01 ( -- )   \ 00434D50
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    2 message
    wait-message
    0 answer? if
        $F $54 fade
        \ (nop-progress-18: no effect in this game)
        $1C 1.0 1 bgm
        wait-fade
        3 ebit-set
        $30 7 sound-stop
        0 6 sound-stop
        3 6 sound-stop
        4 6 sound-stop
        5 6 sound-stop
        6 6 sound-stop
        begin
            0 adx? not while
            yield
        repeat
        1.0 sound-volume-scale
        0 0.0 $FF bgm
        begin
            1 adx? not while
            yield
        repeat
        $81 exit-check
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room8D.act02 ( -- )   \ 00434DC0
    self-wait-done
    $CA 0.0 96.99 0 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $1200 self-anim
    self-wait-anim
    $1203 self-anim
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 message
    wait-message
    self-idle-or-end
;

: room8D.act03 ( -- )   \ 00434DF0
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
        4 message
        wait-message
    then
    self-idle-or-end
;

: room8D.act04 ( -- )   \ 00434E20
    self-wait-done
    1 self-through-exit
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
        4 message
        wait-message
    then
    self-idle-or-end
;

: room8D.act05 ( -- )   \ 00434E50
    begin
        0 cutscene-shot? not while
        yield
    repeat
    0 room8D.cmd01
    begin
        2 cutscene-shot? not while
        yield
    repeat
    2 room8D.cmd01
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 room8D.cmd01
    begin
        4 cutscene-shot? not while
        yield
    repeat
    3 room8D.cmd01
    begin
        $1E0 cutscene-cue-reached? not while
        yield
    repeat
    4 room8D.cmd01
    begin
        7 cutscene-shot? not while
        yield
    repeat
    2 room8D.cmd01
    begin
        8 cutscene-shot? not while
        yield
    repeat
    0 room8D.cmd01
    begin
        9 cutscene-shot? not while
        yield
    repeat
    2 room8D.cmd01
    begin
        $A cutscene-shot? not while
        yield
    repeat
    0 room8D.cmd01
    begin
        $B cutscene-shot? not while
        yield
    repeat
    2 room8D.cmd01
    begin
        $C cutscene-shot? not while
        yield
    repeat
    0 room8D.cmd01
    6 room8D.cmd01
    begin
        $58C cutscene-cue-reached? not while
        yield
    repeat
    4 room8D.cmd01
    begin
        $D cutscene-shot? not while
        yield
    repeat
    2 room8D.cmd01
    begin
        $E cutscene-shot? not while
        yield
    repeat
    0 room8D.cmd01
    begin
        $65E cutscene-cue-reached? not while
        yield
    repeat
    1 334 var-set
    begin
        0 cutscene-cue-reached? while
        7 room8D.cmd01
        yield
    repeat
    self-idle-or-end
;

: room8D.act06 ( -- )   \ 00434F10
    self-wait-done
    1 self-anim
    5 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room8D.act07 ( -- )   \ 00434F20
    self-wait-done
    room8D.cmd00
    $1E 3 $FF char-load
    $1F 4 $FF char-load
    3 char-unload
    4 char-unload
    0 1 $14 door-bits
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
    0 $F9 5 action
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
    2 room8D.cmd01
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room8D.act08 ( -- )   \ 00434FC8
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    3 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

' room8D.enter $8D 0 room-script!
' room8D.char-enter $8D 6 room-script!
' room8D.phase1 $8D 1 room-script!
' room8D.phase2 $8D 2 room-script!
' room8D.phase3 $8D 3 room-script!
' room8D.phase5 $8D 5 room-script!
' room8D.act00 $8D $00 action-script!
' room8D.act01 $8D $01 action-script!
' room8D.act02 $8D $02 action-script!
' room8D.act03 $8D $03 action-script!
' room8D.act04 $8D $04 action-script!
' room8D.act05 $8D $05 action-script!
' room8D.act06 $8D $06 action-script!
' room8D.act07 $8D $07 action-script!
' room8D.act08 $8D $08 action-script!

\ ---- room $8E ----------------------------------------------------------------------------------

\ door 0 swung by script variable 0: byte 3 0 sets it to -90; 1 opens it 10 degrees a step to 0
\ (doors +0x74), waiting (2) until there
: room8E.cmd00 ( b0 -- )  drop s" room8E.cmd00" stub-step ;
\ the room object named pstr_tana falling over: byte 3 0 starts it (angle +0x30, speed +0x34 and
\ acceleration +0x38 0, jerk +0x3C 0.005); 1 steps them, its tilt +0x10 = (1 - sin(90 - angle))
\ * pi/2, waiting (2) until the angle reaches 90; 2 puts it down (sin(pi/2))
: room8E.cmd01 ( b0 -- )  drop s" room8E.cmd01" stub-step ;

defer room8E.act02
: room8E.enter ( -- )   \ 00435050
    room-sounds
    2 -1 var-set
    1 -1 var-set
    $95 story-flag? not if
        0 $F1 2 action
    then
    0 79.7 11.1 -39.8 0 effect-86
;

: room8E.char-enter ( -- )   \ 00435080
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
        $95 story-flag? not if
            0 $85 0.0 10.0 180 char-to-xz
            0 0 1 action
        then
    then
;

: room8E.phase1 ( -- )   \ 004350E0
    $95 story-flag? if
        0 exit-usable? if
            0 exit-check
        then
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $95 story-flag? not if
        0 ebit? not if
            $B ebit? $A ebit? and if
                0 8 char-left-area? $C ebit? not and if
                    $C ebit-set
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
        else 0 0 char-entered-area? $D ebit? not and if
            $D ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 $A action-force
            else
                1 0 $A action-force
            then
        then then
        7 ebit? not $B ebit? and if
            0 $A char-in-area? if
                0 0 char-in-nav-group? not if
                    0 0 5 action
                then
            then
        then
        1 -1 var? if
            6 ebit? not if
                9 state-flag? if
                    6 ebit-set
                    1 5 var-set
                then
            then
            1 ebit? not if
                $A state-flag? if
                    1 ebit-set
                    1 16 var-set
                then
            then
            2 ebit? not if
                0 8 char-action? if
                    2 ebit-set
                    1 17 var-set
                then
            then
            4 ebit? not if
                1 exit-door-open? if
                    4 ebit-set
                    1 6 var-set
                then
            then
        then
        1 exit-door-open? if
            $B ebit-set
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
;

: room8E.phase2 ( -- )   \ 00435200
    0 9 char-in-area? 0 -45 $3C char-heading? and if
        $FE char-here? not if
            5 8 5 scene-change
        else
            $8016 scene-ending
        then
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 4 1 scene-change
        then
    then
    $95 story-flag? not if
        1 -1 var? if
            5 ebit? not if
                0 $B char-in-area? 0 0 $32 char-heading? and if
                    5 7 0 scene-change
                then
            then
        then
    then
;

: room8E.act00 ( -- )   \ 00435250
    1 32768 var-set
    $F1 action-end
    $FE action-end
    $FE char-done
    $12 state-flag-set
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
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
    0 $1A 0.49 -36.88 -21 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    0 ebit-set
    $E9 door-open-set
    $E9 door-unlock
    doors-room-in
    $F $41 fade
    wait-fade
    $49 resident-flag-set
    $18 state-flag-clear
    $12 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room8E.act01 ( -- )   \ 00435320
    2 0 char-remove
    $A partner-load
    self-wait-done
    $2C 1.0 1 bgm
    0 room8E.cmd00
    1 room8E.cmd00
    0 $28 5 char-sound
    $E9 door-open-clear
    $E9 door-lock
    doors-room-in
    0 $43 5 char-sound
    $F01 3 self-anim-blend
    self-wait-anim
    0 self-through-exit
    self-wait-done
    $60B self-anim
    self-frames-reset
    self-wait-16
    begin
        0 adx? not while
        yield
    repeat
    1 message
    0 0.0 $FF bgm
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 $A self-anim-blend
    self-wait-anim
    wait-message
    begin
        1 adx? not while
        yield
    repeat
    2 0 var-set
    4 0 var-set
    2 char-unload
    self-idle-or-end
;

: room8E.act03 ( -- )   \ 00435570
    $12 state-flag-set
    yield
    begin
        0 adx? not while
        yield
    repeat
    0 0.0 $FF bgm
    1 2 var? if
        2 message
        wait-message
    else 1 3 var? if
        3 message
        wait-message
    else 1 4 var? if
        4 message
        wait-message
    else 1 5 var? if
        5 message
        wait-message
    else 1 6 var? if
        6 message
        wait-message
    else 1 7 var? if
        7 message
        wait-message
    else 1 8 var? if
        8 message
        wait-message
    else 1 9 var? if
        9 message
        wait-message
    else 1 10 var? if
        $A message
        wait-message
    else 1 11 var? if
        $B message
        wait-message
    else 1 12 var? if
        $C message
        wait-message
    else 1 13 var? if
        $D message
        wait-message
    else 1 14 var? if
        $E message
        wait-message
    else 1 15 var? if
        $F message
        wait-message
    else 1 16 var? if
        $10 message
        wait-message
    else 1 17 var? if
        $11 message
        wait-message
    else 1 18 var? if
        $12 message
        wait-message
    then then then then then then then then then then then then then then then then then
    begin
        1 adx? not while
        yield
    repeat
    1 32768 var? not if
        1 -1 var-set
    then
    $12 state-flag-clear
    ['] room8E.act02 goto
;

:noname   \ room8E.act02 (00435390; deferred: used before it is defined)
    begin
        1 32768 var? if
            yield
        else
            2 -1 var? not if
                1 -1 var? if
                    2 var-inc
                    2 450 var? if
                        1 9 var-set
                    else 2 900 var? if
                        1 10 var-set
                    else 2 1350 var? if
                        1 11 var-set
                    else 2 1800 var? if
                        1 12 var-set
                    else 2 2250 var? if
                        $A ebit-set
                        1 13 var-set
                    else 2 2700 var? if
                        1 14 var-set
                    else 2 3150 var? if
                        1 15 var-set
                        2 2250 var-set
                    then then then then then then then
                then
                3 ebit? not if
                    1 -1 var? if
                        0 char-busy? not $FF $FF pad? not and -1 control-action? and if
                            4 var-inc
                            4 300 var? if
                                3 ebit-set
                                1 18 var-set
                            then
                        else
                            4 0 var-set
                        then
                    then
                then
                1 2 var? if
                    $1E 1.0 1 bgm
                    room8E.act03
                else 1 3 var? if
                    $1F 1.0 1 bgm
                    room8E.act03
                else 1 4 var? if
                    room8E.act03
                else 1 5 var? if
                    $20 1.0 1 bgm
                    room8E.act03
                else 1 6 var? if
                    $21 1.0 1 bgm
                    room8E.act03
                else 1 7 var? if
                    $2E 1.0 1 bgm
                    room8E.act03
                else 1 8 var? if
                    $22 1.0 1 bgm
                    room8E.act03
                else 1 9 var? if
                    $23 1.0 1 bgm
                    room8E.act03
                else 1 10 var? if
                    $24 1.0 1 bgm
                    room8E.act03
                else 1 11 var? if
                    $25 1.0 1 bgm
                    room8E.act03
                else 1 12 var? if
                    $26 1.0 1 bgm
                    room8E.act03
                else 1 13 var? if
                    $2F 1.0 1 bgm
                    room8E.act03
                else 1 14 var? if
                    $27 1.0 1 bgm
                    room8E.act03
                else 1 15 var? if
                    $28 1.0 1 bgm
                    room8E.act03
                else 1 16 var? if
                    $29 1.0 1 bgm
                    room8E.act03
                else 1 17 var? if
                    $2A 1.0 1 bgm
                    room8E.act03
                else 1 18 var? if
                    $2B 1.0 1 bgm
                    room8E.act03
                then then then then then then then then then then then then then then then then then
            then
            yield
        then
    again
; is room8E.act02

: room8E.act04 ( -- )   \ 00435670
    1 32768 var-set
    self-wait-done
    0 self-through-exit
    self-wait-done
    3 0 var? if
        3 var-inc
        1 2 var-set
    else 3 1 var? if
        3 var-inc
        1 3 var-set
    else 3 2 var? if
        1 4 var-set
    then then then
    $60B self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: room8E.act05 ( -- )   \ 004356C0
    7 ebit-set
    1 32768 var-set
    1 0 8 nav-group
    self-wait-done
    0 counter-set
    0 $F2 6 action
    self-frames-reset
    8 self-wait-frames
    $FFFF message-close
    $F03 3 self-anim-blend
    1 wait-counter
    0 $3E 5 char-sound
    $F04 $A self-anim-blend
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: room8E.act06 ( -- )   \ 00435700
    0 room8E.cmd01
    1 room8E.cmd01
    0 0 6 char-sound
    1 $FF 8 rumble
    1 counter-set
    1 8 var-set
    self-idle-or-end
;

: room8E.act07 ( -- )   \ 00435720
    5 ebit-set
    1 7 var-set
    self-idle-or-end
;

: room8E.act08 ( -- )   \ 00435730
    6 ebit? not if
        1 32768 var-set
    then
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 counter-set
    8 ebit-clear
    9 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 9 action-force
        8 ebit-set
        1 6 char-file-load
    then
    0 5 char-file-load
    $BD 22.68 -68.27 -90 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    8 ebit? if
        1 self-look-at
        yield
        begin
            9 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    $8001 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    0 0 $14 door-bits
    3 0 object-show
    4 0 object-show
    3 1 object-anim
    4 1 object-anim
    self-frames-reset
    $E self-wait-frames
    0 1 6 char-sound
    self-frames-reset
    $3C self-wait-frames
    0 2 6 char-sound
    self-wait-anim
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    0 avoid-prompt
    self-frames-reset
    $2D self-wait-frames
    6 ebit? not if
        6 ebit-set
        1 5 var-set
    then
    begin
        0 2 pad? not while
        2 panic-grow
        6 fiona-calm
        $1E fiona-recovery-lower
        yield
    repeat
    counter-inc
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    3 2 object-anim
    4 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 1 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 2 6 char-sound
    self-wait-anim
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: room8E.act09 ( -- )   \ 00435820
    1 self-scripted
    self-wait-done
    $153 26.2 -63.6 -90 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    9 ebit-set
    1 wait-counter
    1 self-noclip
    $8001 5 self-anim-9
    self-wait-anim
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                $8002 0 self-anim-blend
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            $8002 0 self-anim-blend
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room8E.act0A ( -- )   \ 00435870
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    \ (nop-progress-18: no effect in this game)
    $F $44 fade
    wait-fade
    8 state-flag-set
    $12 state-flag-set
    $80 exit-check
    self-idle-or-end
;

: room8E.act0B ( -- )   \ 00435890
    self-wait-done
    0 79.6 11.1 -39.7 0 effect-86
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room8E.enter $8E 0 room-script!
' room8E.char-enter $8E 6 room-script!
' room8E.phase1 $8E 1 room-script!
' room8E.phase2 $8E 2 room-script!
' room8E.act00 $8E $00 action-script!
' room8E.act01 $8E $01 action-script!
' room8E.act02 $8E $02 action-script!
' room8E.act03 $8E $03 action-script!
' room8E.act04 $8E $04 action-script!
' room8E.act05 $8E $05 action-script!
' room8E.act06 $8E $06 action-script!
' room8E.act07 $8E $07 action-script!
' room8E.act08 $8E $08 action-script!
' room8E.act09 $8E $09 action-script!
' room8E.act0A $8E $0A action-script!
' room8E.act0B $8E $0B action-script!

\ ---- room $8F ----------------------------------------------------------------------------------

\ a struggle: byte 3 0 resets Fiona's shake tracking (+0x1AD710 / +0x1AD714); 1 adds her shakes
\ to script variable byte 4, with a grunt (voice 0x3D or 0x45 at random) when the cool-down
\ variable byte 6 is out (it then runs 45 / 60), and at 100 the event byte 5 (+0x5C)
: room8F.cmd00 ( bytes.. n -- )  0 ?do drop loop s" room8F.cmd00" stub-step ;
\ byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(0, 0); else
\ Progress_SpeechCall
: room8F.cmd01 ( b0 -- )  drop s" room8F.cmd01" stub-step ;
\ room 0x8F: three grey smoke effects (Effect79B00, size 50) at the room's spots 0, 1, 6
\ (grey_three).
: room8F.cmd02 ( -- )  s" room8F.cmd02" stub-step ;
\ room 0x8F: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room8F.cmd03 ( -- )  s" room8F.cmd03" stub-step ;
\ the pursuer is active and out of view: state 3 (+0xE8), or the camera's on-screen test (+0xD4)
\ fails
: room8F.cond00? ( -- flag )  s" room8F.cond00?" stub-flag ;

: room8F.enter ( -- )   \ 00435990
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
    builtin.act95
    $A3 story-flag? if
        room8F.cmd02
    then
    $2E7 story-flag? not if
        5 13.05 6.54 108.47 flicker-sprite
    then
    1 $2300 sound-volume
;

: room8F.act08 ( -- )   \ 00436180
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

: room8F.char-enter ( -- )   \ 00435A60
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
            room8F.act08
        else
            6 ebit-clear
        then
    then
;

: room8F.phase1 ( -- )   \ 00435AF0
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
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
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
                \ (nop-progress-74: no effect in this game)
            then
        then
    then
    $FE 2 char-C4? not if
        9 state-flag? 5 ebit? not and $FE char-here? and if
            $FE char-busy? not if
                8 ebit-set
                room8F.act08
            then
        then
    then
    7 ebit? $FE $17 char-in-area? and if
        0 $FE $D action
    then
    room8F.cmd03
;

: room8F.phase2 ( -- )   \ 00435C20
    $A5 story-flag? if
        0 $14 char-in-area? 0 90 $3C char-heading? and if
            $FE char-here? not if
                5 6 5 scene-change
            else room8F.cond00? not if
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

: room8F.phase3 ( -- )   \ 00435CA0
    -72.3 18.0 56.0 -85.0 18.0 56.4 -72.3 -2.0 51.4 -85.0 -2.0 51.4 lights-doorway
    72.0 16.0 -0.1 91.2 16.0 -0.1 72.0 0.0 -2.0 91.2 0.0 -2.0 lights-doorway
    $A3 story-flag? 9 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        9 ebit-set
    then
;

: room8F.act00 ( -- )   \ 00435D30
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
        0 room8F.cmd01
    else $26 2 pvar? if
        1 room8F.cmd01
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
        3 room8F.cmd01
    then
    $F $41 fade
    wait-fade
    $4C resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room8F.act01 ( -- )   \ 00435E50
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

: room8F.act02 ( -- )   \ 00435E70
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

: room8F.act03 ( -- )   \ 00435ED0
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
    0 1 room8F.cmd00
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
    builtin.act95
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
        1 0 1 1 4 room8F.cmd00
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

: room8F.act04 ( -- )   \ 0047AE60
    begin
        self-frames-reset
        1 self-wait-frames
    again
;

: room8F.act05 ( -- )   \ 00436050
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

: room8F.act07 ( -- )   \ 00436150
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

: room8F.act06 ( -- )   \ 004360A0
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
                ['] room8F.act07 goto
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
                ['] room8F.act07 goto
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

: room8F.act09 ( -- )   \ 004361E0
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

: room8F.act0A ( -- )   \ 00436250
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

: room8F.act0B ( -- )   \ 004362B0
    begin
        2 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    $26 3 pvar? $26 2 pvar? or if
        2 room8F.cmd01
    then
    self-idle-or-end
;

: room8F.act0C ( -- )   \ 0047AE68
    self-idle-or-end
;

: room8F.act0D ( -- )   \ 004362D0
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

: room8F.act0E ( -- )   \ 00436320
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

: room8F.act0F ( -- )   \ 00436340
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
        0 room8F.cmd01
    else $26 2 pvar? if
        1 room8F.cmd01
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
        3 room8F.cmd01
    then
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room8F.act10 ( -- )   \ 00436440
    self-wait-done
    2 1 object-show
    3 1 object-show
    1 1 $14 door-bits
    0 -59.1 18.5 -16.0 1 effect-86
    1 -10.4 17.5 -26.3 1 effect-86
    2 84.1 19.8 -2.3 1 effect-86
    3 -57.4 20.0 24.4 1 effect-86
    4 115.9 19.5 33.2 1 effect-86
    room8F.cmd02
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

' room8F.enter $8F 0 room-script!
' room8F.char-enter $8F 6 room-script!
' room8F.phase1 $8F 1 room-script!
' room8F.phase2 $8F 2 room-script!
' room8F.phase3 $8F 3 room-script!
' room8F.act00 $8F $00 action-script!
' room8F.act01 $8F $01 action-script!
' room8F.act02 $8F $02 action-script!
' room8F.act03 $8F $03 action-script!
' room8F.act04 $8F $04 action-script!
' room8F.act05 $8F $05 action-script!
' room8F.act06 $8F $06 action-script!
' room8F.act07 $8F $07 action-script!
' room8F.act08 $8F $08 action-script!
' room8F.act09 $8F $09 action-script!
' room8F.act0A $8F $0A action-script!
' room8F.act0B $8F $0B action-script!
' room8F.act0C $8F $0C action-script!
' room8F.act0D $8F $0D action-script!
' room8F.act0E $8F $0E action-script!
' room8F.act0F $8F $0F action-script!
' room8F.act10 $8F $10 action-script!

\ ---- room $91 ----------------------------------------------------------------------------------

\ room 0x91 (Room91_Cmd00_ptmf): door 0's +0x68 (0)
: room91.cmd00 ( -- )  s" room91.cmd00" stub-step ;
\ script variable 0 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0
: room91.cmd01 ( -- )  s" room91.cmd01" stub-step ;
\ room 0x91: three grey smoke effects (Effect79B00, size 70) at the room's spots 1, 3, 0xB
\ (grey_three).
: room91.cmd02 ( -- )  s" room91.cmd02" stub-step ;
\ room 0x91: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room91.cmd03 ( -- )  s" room91.cmd03" stub-step ;

: room91.enter ( -- )   \ 00436600
    room-sounds
    0 72.4 17.2 117.2 1 effect-86
    1 101.9 66.0 87.1 4 effect-86
    2 97.6 29.5 144.8 1 effect-86
    4 101.9 66.0 87.4 4 effect-86
    $A3 story-flag? if
        1 0 $20000 nav-group
        0 1 $14 door-bits
    then
    $34E story-flag? if
        5 1.0 0 bgm
    then
    $2F3 story-flag? $2F4 story-flag? not and if
        3 86.0 1.0 68.0 flicker-sprite
    then
    $80 exit-taken? not if
        builtin.act95
        $A3 story-flag? if
            room91.cmd02
        then
    else
        2 ebit-set
    then
;

: room91.char-enter ( -- )   \ 00436690
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
            0 $1FC 22.0 100.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 1 action
            0.0 sound-volume-scale
        then
    then
;

: room91.phase1 ( -- )   \ 00436780
    2 ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
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
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $A3 story-flag? $A4 story-flag? not and if
        0 6 char-entered-area? 1 ebit? not and if
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
    then
    $EB door-locked? if
        4 -37.0 0.0 177.0 7 20 0 zone
        0 4 char-in-zone? if
            0 $F1 4 action
        then
        1 char-here? 1 2 char-C4? not and 0 $C char-in-area? and 1 $C char-in-area? and 0 -37 177 $3C char-faces-xz? and 0 control-action? and if
            hewie-stays? if
                0 1 5 action
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
    $A6 story-flag? not if
        0 $B char-entered-area? if
            $A6 story-flag-set
            \ (nop-progress-74: no effect in this game)
            \ (nop-progress-74: no effect in this game)
        then
    then
    $2F3 story-flag? not if
        3 86.0 0.0 68.0 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 755 var-set
                $1A 756 var-set
                $1B 3 var-set
                $1C 86000 var-set
                $1D 1000 var-set
                $1E 68000 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    room91.cmd03
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 48.0 0.0 115.0 0 0 sound
            else 1 1 var? if
                $40000006 6 48.0 0.0 115.0 0 0 sound
            else 1 2 var? if
                $40000007 6 48.0 0.0 115.0 0 0 sound
            else 1 3 var? if
                $40000008 6 48.0 0.0 115.0 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: room91.phase2 ( -- )   \ 00436A00
    0 $A char-in-area? 0 -22 $32 char-heading? and if
        5 $84 3 scene-change
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 6 1 scene-change
        then
    then
    $2F3 story-flag? $2F4 story-flag? not and if
        3 3 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: room91.phase3 ( -- )   \ 00436A40
    $A3 story-flag? 2 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        2 ebit-set
    then
;

: room91.act00 ( -- )   \ 00436A60
    $18 state-flag-set
    1 self-scripted
    \ (nop-progress-14: no effect in this game)
    $F $54 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    self-idle-or-end
;

: room91.act01 ( -- )   \ 00436A80
    1 self-scripted
    \ (nop-progress-14: no effect in this game)
    self-wait-done
    $A4 story-flag-set
    $91 0 493 hewie-to-room
    1 $1ED 45 char-to-tri-facing
    $FE char-activate
    $FE $92 -1 2 stalker-to-room
    $FE 0 stalker-mode
    \ (nop-progress-24: no effect in this game)
    $13 state-flag-set
    camera-restart
    room91.cmd02
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    $F $11 fade
    wait-fade
    0 $F1 3 action
    self-frames-reset
    8 self-wait-frames
    0 $43 5 char-sound
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    $1E self-wait-frames
    $802 self-anim
    2 message
    wait-message
    $34E story-flag-set
    5 1.0 0 bgm
    $34F story-flag-set
    $F1 action-end
    builtin.act95
    0 5 var-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room91.act02 ( -- )   \ 00436B10
    self-wait-done
    86.0 68.0 self-turn-to-xz
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
            $2F4 story-flag-set
            3 effect-remove
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

: room91.act03 ( -- )   \ 00436B70
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    0 0 $1E rumble
    1 $FF $1E rumble
    $31 $87 0.0 0.0 0.0 0 0 sound
    self-frames-reset
    begin
        $1E frames? not while
        1.0 camera-shake
        yield
    repeat
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    begin
        0.1 camera-shake
        yield
    again
;

: room91.act04 ( -- )   \ 00436BD0
    0 exit-door-open? not if
        room91.cmd01
        $91 5 -37.0 10.0 177.0 0 0 sound
        0 0 var? if
            $EB door-unlock
            room91.cmd00
            $EB door-open-set
        then
    then
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: room91.act05 ( -- )   \ 00436C10
    self-wait-done
    hewie-bark
    self-wait-done
    $202 -4.422 167.237 -73 $FFFF $A self-move-to
    self-wait-done
    $90 -32.0 177.5 0.5 50 hewie-go-to
    self-frames-reset
    8 self-wait-frames
    0 exit-door-open? not if
        $EB door-unlock
        $91 5 -37.0 10.0 177.0 0 0 sound
        room91.cmd00
        $EB door-open-set
    then
    self-wait-done
    self-idle-or-end
;

: room91.act06 ( -- )   \ 00436C60
    self-wait-done
    0 ebit? not if
        0 self-through-exit
        self-wait-done
        $60B self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        0 ebit-set
    then
    0 message
    wait-message
    self-idle-or-end
;

' room91.enter $91 0 room-script!
' room91.char-enter $91 6 room-script!
' room91.phase1 $91 1 room-script!
' room91.phase2 $91 2 room-script!
' room91.phase3 $91 3 room-script!
' room91.act00 $91 $00 action-script!
' room91.act01 $91 $01 action-script!
' room91.act02 $91 $02 action-script!
' room91.act03 $91 $03 action-script!
' room91.act04 $91 $04 action-script!
' room91.act05 $91 $05 action-script!
' room91.act06 $91 $06 action-script!

\ ---- room $92 ----------------------------------------------------------------------------------

\ room objects 4 / 5 spinning (+0x10) at a speed (+0x30) eased by script variable 4 / 5: byte 3
\ 0 sets them up (speed and base 4 degrees, top 20); else each frame (unless the progress' +0x54
\ says no): state 0 slows by 5% of the base to 0, 1 speeds by 2% up to the base, 2 by 10% up to
\ the top, 3 jumps to the base
: room92.cmd00 ( b0 -- )  drop s" room92.cmd00" stub-step ;
\ room 0x92: three grey smoke effects (Effect79B00, size 50) at the room's spots 2, 7, 5
\ (grey_three).
: room92.cmd01 ( -- )  s" room92.cmd01" stub-step ;
\ up to 6 things out (script variable 6 counts them): 1..3 more placed things of kind 9 (+0x8),
\ each tied to the first room object 10..15 flagged (+0 = 1) (+0x122 its index), set up (+0xC,
\ +0x28 on) and dropped at a random spot 40..50 out and 25..40 up in any direction that lands on
\ the nav mesh (+0x3C)
: room92.cmd02 ( -- )  s" room92.cmd02" stub-step ;
\ the placed thing 10 brought back (list +0x14 / +0x8, its +0xC, +0x28 on) and put on one of
\ five spots round a circle (script variable 1, then on by 2 of 5): turned to the spot's angle
\ (with a little random), 2.1 up and 1.5..2 out on triangle 0x3B; then effect SmokeTrail_vtable
\ on it
: room92.cmd03 ( -- )  s" room92.cmd03" stub-step ;
\ (as Room49_Cmd02) byte 3 0: the effect WispColumn_vtable spawned (told 1), its slot in event
\ var 0; 1: it is told 0
: room92.cmd04 ( b0 -- )  drop s" room92.cmd04" stub-step ;
\ the things that fell below -30: placed things of kind 9 are reset (+0x28) and each counts down
\ script variable 6 (and the event manager's +0x5C); room objects 10..15 that did get +0 set
: room92.cmd05 ( -- )  s" room92.cmd05" stub-step ;
\ the 0x20E0-byte effect SparkSpray_vtable (sent byte 3): byte 3 0 / 1 one made, its slot in
\ script variable 2 / 3; 2 / 3 the one in variable 2 / 3 (if any) sent nothing
: room92.cmd06 ( b0 -- )  drop s" room92.cmd06" stub-step ;
\ (as slam_shake, both of Lorenzo's forms: 0xA and 0x27)
: room92.cmd07 ( -- )  s" room92.cmd07" stub-step ;
\ room objects 8 / 9 raised (+0x24 down 0.2 a call to 0) while the event manager's +0x58 test 4
\ / 5 holds, else lowered back (up 0.4 a call to 0.7)
: room92.cmd08 ( -- )  s" room92.cmd08" stub-step ;
\ a noise at (-70 or 70 by byte 3, 14, 0): byte 4 0 / 1 / 2 kind 1 / 2 / 4
: room92.cmd09 ( b0 b1 -- )  drop drop s" room92.cmd09" stub-step ;
\ Room92_Cmd0A
: room92.cmd0A ( -- )  s" room92.cmd0A" stub-step ;
\ Room92_Cmd0B
: room92.cmd0B ( -- )  s" room92.cmd0B" stub-step ;
\ Room92_Cmd0C
: room92.cmd0C ( b0 b1 -- )  drop drop s" room92.cmd0C" stub-step ;
\ Room92_Cmd0D
: room92.cmd0D ( b0 -- )  drop s" room92.cmd0D" stub-step ;

defer room92.act0F
: room92.enter ( -- )   \ 00436CF0
    room-sounds
    0 -36.4 13.4 54.4 1 effect-86
    1 58.7 10.0 -29.9 1 effect-86
    $A2 story-flag? if
        0 1 $14 door-bits
        0 $F1 9 action
        4 3 var-set
        5 3 var-set
    then
    $34E story-flag? if
        5 1.0 0 bgm
    then
    $80 exit-taken? not if
        builtin.act95
        $A3 story-flag? if
            room92.cmd01
            0 0 room92.cmd0C
            0 room92.cmd0D
        then
    else
        $B ebit-set
    then
    1 0 $18000020 nav-group
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
;

: room92.char-enter ( -- )   \ 00436D70
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
        $80 exit-taken? if
            0 $85 char-to-tri
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 $A action
            0.0 sound-volume-scale
        then
    then
;

: room92.phase1 ( -- )   \ 00436DE0
    $A2 story-flag? if
        8 ebit? not if
            6 sound-bank-loaded? if
                $40000000 6 0.0 50.0 0.0 0 0 sound
                $40000001 6 0.0 0.0 0.0 0 0 sound
                8 ebit-set
            then
        else
            $C0000000 6 0.0 50.0 0.0 0 0 sound
            $C0000001 6 0.0 0.0 0.0 0 0 sound
            $C0000003 6 0.0 0.0 0.0 0 0 sound
            $C0000002 6 -70.0 15.0 0.0 0 0 sound
            $C0000006 6 70.0 15.0 0.0 0 0 sound
        then
    then
    $B ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    $A1 story-flag? not if
        0 control-action? 1 char-here? and 1 2 char-C4? not and $FE char-here? not and 0 4 char-in-area? and 0 0 65 $32 char-faces-xz? and if
            hewie-stays? if
                0 0 3 action
            then
        then
    then
    $A2 story-flag? $A3 story-flag? not and if
        $FE 2 char-C4? if
            $A3 story-flag-set
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
    $A2 story-flag? $A3 story-flag? not and if
        room92.cmd05
        3 ebit? not if
            9 ebit? if
                3 ebit-set
                0 $F2 $B action
            then
        then
        4 ebit? not if
            0 5 char-in-area? 1 5 char-in-area? or $FE 5 char-in-area? or if
                4 ebit-set
                0 $F3 $C action
            then
        else 6 ebit? if
            0 7 char-in-area? 0 0 char-action? and if
                0 0 room92.cmd09
            then
            1 7 char-in-area? 1 0 char-action? and if
                0 1 room92.cmd09
            then
            $FE 7 char-in-area? $FE 0 char-action? and if
                0 2 room92.cmd09
            then
        then then
        5 ebit? not if
            0 6 char-in-area? 1 6 char-in-area? or $FE 6 char-in-area? or if
                5 ebit-set
                0 $F4 $D action
            then
        else 7 ebit? if
            0 8 char-in-area? 0 0 char-action? and if
                1 0 room92.cmd09
            then
            1 8 char-in-area? 1 0 char-action? and if
                1 1 room92.cmd09
            then
            $FE 8 char-in-area? $FE 0 char-action? and if
                1 2 room92.cmd09
            then
        then then
    else
        4 ebit? not if
            0 5 char-in-area? 1 5 char-in-area? or $FE 5 char-in-area? or if
                5 6 -40.0 0.0 -40.0 0 0 sound
                4 ebit-set
            then
        else 0 5 char-in-area? not 1 5 char-in-area? not and $FE 5 char-in-area? not and if
            4 ebit-clear
        then then
        5 ebit? not if
            0 6 char-in-area? 1 6 char-in-area? or $FE 6 char-in-area? or if
                5 6 44.0 0.0 37.0 0 0 sound
                5 ebit-set
            then
        else 0 6 char-in-area? not 1 6 char-in-area? not and $FE 6 char-in-area? not and if
            5 ebit-clear
        then then
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
    $A1 story-flag? not if
        3 1.06 0.0 59.99 $1A 14 0 zone
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
    then
    4 0.21 0.0 0.03 $21 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
    room92.cmd07
    room92.cmd08
;

: room92.phase2 ( -- )   \ 004370F0
    0 3 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    $10 state-flag? if
        -2147483646 scene-request? if
            5 6 1 scene-change
        then
    then
    $A3 story-flag? if
        2 0.0 0.0 0.0 $17 4 0 zone
        0 2 3 char-zone-bits? 0 0 0 $32 char-faces-xz? and if
            5 7 0 scene-change
        then
    then
;

: room92.phase3 ( -- )   \ 00437140
    $A3 story-flag? $B ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        $B ebit-set
    then
;

: room92.act02 ( -- )   \ 00437320
    1 self-look-at
    yield
    0 counter-set
    0 1 1 action-force
    1 wait-counter
    $A1 story-flag-set
    50 hewie-trust
    3 message
    wait-message
    $FF self-look-at
    yield
    $255 item-give
    $828C item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room92.act00 ( -- )   \ 00437160
    self-wait-done
    $A3 story-flag? not if
        $A1 story-flag? not if
            1 char-here? 1 2 char-C4? not and $FE char-here? not and if
                $18 state-flag-set
                1 self-scripted
                self-frames-reset
                8 self-wait-frames
                0 $31 0.0 58.0 0 char-to-xz
                1 15.0 10.0 0.0 5.0 event-camera
                $17 state-flag-set
                self-frames-reset
                8 self-wait-frames
                2 message
                wait-message
                self-frames-reset
                8 self-wait-frames
                $17 state-flag-clear
                0 0.0 0.0 0.0 0.0 event-camera
                0 $62 -10.0 54.0 140 char-to-xz
                ['] room92.act02 goto
            else 0 ebit? not if
                self-frames-reset
                8 self-wait-frames
                0 $31 0.0 58.0 0 char-to-xz
                1 15.0 10.0 0.0 5.0 event-camera
                $17 state-flag-set
                self-frames-reset
                8 self-wait-frames
                0 message
                wait-message
                self-frames-reset
                8 self-wait-frames
                $17 state-flag-clear
                0 0.0 0.0 0.0 0.0 event-camera
                0 ebit-set
            else
                1 message
                wait-message
            then then
        else
            0.0 65.0 self-turn-to-xz
            self-wait-done
            self-frames-reset
            8 self-wait-frames
            0 $31 0.0 58.0 0 char-to-xz
            1 15.0 10.0 0.0 5.0 event-camera
            $17 state-flag-set
            self-frames-reset
            8 self-wait-frames
            4 message
            wait-message
            self-frames-reset
            8 self-wait-frames
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
    else 0 ebit? not if
        self-frames-reset
        8 self-wait-frames
        0 $31 0.0 58.0 0 char-to-xz
        1 15.0 10.0 0.0 5.0 event-camera
        $17 state-flag-set
        self-frames-reset
        8 self-wait-frames
        5 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
        0 ebit-set
    else
        0.0 65.0 self-turn-to-xz
        self-wait-done
        7 message
        wait-message
    then then
    self-idle-or-end
;

: room92.act01 ( -- )   \ 004372F0
    1 self-scripted
    self-wait-done
    $31 3.0 55.0 -10 $204 5 self-move-to
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    0 self-look-at
    yield
    hewie-bark
    self-wait-done
    counter-inc
    $FF self-look-at
    yield
    0 self-scripted
    self-idle-or-end
;

: room92.act03 ( -- )   \ 00437350
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C04 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    ['] room92.act02 goto
;

: room92.act04 ( -- )   \ 00437370
    0 state-flag-clear
    $FE action-end
    $FE char-done
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $F $44 fade
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
    1 action-end
    1 char-done
    0 1 $14 door-bits
    $26 item-use
    0 creatures-clear
    $A ebit-clear
    $10 $C8 movie-param
    0 $F9 8 action
    $FF 1.0 0 bgm
    $FF panic-stage? if
        3 panic-stage
    then
    9 message-prepare
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
    $1F $7B $32 $2A $64 $24 $2C $36 $4E 0 9 effect-string
    $8080805E 0 1 screen-blend
    0 self-move-16
    0 $7E 0.023 24.438 174 char-to-xz
    $FE action-end
    $FE char-done
    2 0 char-remove
    $27 partner-load
    2 char-unload
    $FE char-activate
    $FE $92 133 2 stalker-to-room
    $FE $85 -0.009 -44.982 30 char-to-xz
    $FE 0 0 char-camera
    $92 0 151 hewie-to-room
    1 $97 180 char-to-tri-facing
    4 0 object-show
    5 0 object-show
    0 $F1 9 action
    4 3 var-set
    5 3 var-set
    $ED door-open-clear
    doors-room-in
    $ED door-lock
    $A2 story-flag-set
    $10 state-flag-set
    4 ebit-clear
    5 ebit-clear
    7 20 var-set
    0 $F5 $F action
    $A ebit? not if
        0 0 room92.cmd0C
    then
    2 2 room92.cmd0C
    1 room92.cmd0D
    7 1.0 0 bgm
    $F 1 fade
    wait-fade
    $4D resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room92.act05 ( -- )   \ 004374E0
    $FF 1.0 0 bgm
    $F2 action-end
    $F3 action-end
    $F4 action-end
    $F5 action-end
    4 ebit-clear
    5 ebit-clear
    6 ebit-clear
    7 ebit-clear
    3 ebit-clear
    9 ebit-clear
    $18 state-flag-set
    1 self-scripted
    $F $54 fade
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
    wait-fade
    1 action-end
    1 char-done
    room92.cmd0A
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    4 3 var-set
    5 3 var-set
    2 room92.cmd0D
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
    $92 0 151 hewie-to-room
    1 $97 180 char-to-tri-facing
    50 hewie-trust
    0 self-move-16
    0 $2B 54.575 -2.153 -90 char-to-xz
    camera-restart
    $10 state-flag-clear
    $FE action-end
    $FE char-done
    2 0 char-remove
    $C partner-load
    2 char-unload
    $ED door-unlock
    4 0 object-show
    5 0 object-show
    $E6 door-lock
    $E7 door-lock
    $EB door-open-clear
    $EB door-lock
    room92.cmd01
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    0 room92.cmd0D
    $F $51 fade
    wait-fade
    $4E resident-flag-set
    $256 item-give
    $828D item-give
    builtin.act9C
    $18 state-flag-clear
    0 self-scripted
    builtin.act95
    self-idle-or-end
;

: room92.act06 ( -- )   \ 0047AE6C
    8 message
    self-idle-or-end
;

: room92.act07 ( -- )   \ 00437630
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not $A4 story-flag? not and if
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        6 message
        wait-message
        self-wait-anim
        1 ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: room92.act08 ( -- )   \ 00437660
    depth-range-off
    begin
        $21C cutscene-cue-reached? not while
        yield
    repeat
    begin
        $2ED cutscene-cue-reached? not while
        1.0 1.0 80.0 160.0 depth-range
        yield
    repeat
    begin
        $348 cutscene-cue-reached? not while
        room92.cmd0B
        yield
    repeat
    depth-range-off
    begin
        4 cutscene-shot? not while
        yield
    repeat
    depth-range-off
    $ED door-open-clear
    doors-room-in
    $1F $72 $96 $94 $6E $60 $56 $70 $3C 0 9 effect-string
    $9EEAEAFF 0 1 screen-blend
    begin
        5 cutscene-shot? not while
        yield
    repeat
    $1F $7B $32 $2A $64 $24 $2C $36 $4E 0 9 effect-string
    $8080805E 0 1 screen-blend
    begin
        $47E cutscene-cue-reached? not while
        yield
    repeat
    0 0 room92.cmd0C
    $A ebit-set
    2 6 room92.cmd0C
    begin
        $492 cutscene-cue-reached? not while
        yield
    repeat
    1 2 room92.cmd0C
    begin
        8 cutscene-shot? not while
        yield
    repeat
    1 0 room92.cmd0C
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    self-idle-or-end
;

: room92.act09 ( -- )   \ 00437700
    0 room92.cmd00
    begin
        1 room92.cmd00
        yield
    again
;

: room92.act0A ( -- )   \ 00437710
    1 self-scripted
    1 char-here? if
        $91 0 -1 hewie-to-room
    then
    self-wait-done
    $FF 1 char-visible
    room92.cmd01
    0 0 room92.cmd0C
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
    8 state-flag-set
    $FE action-end
    $FE char-done
    $FF 0 char-visible
    1 room92.cmd04
    $4F resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: room92.act0B ( -- )   \ 004377C0
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        0.3 camera-shake
        yield
    repeat
    0 room92.cmd04
    $40000003 6 0.0 0.0 0.0 0 0 sound
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    room92.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    room92.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    room92.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    1 room92.cmd04
    9 ebit-clear
    3 ebit-clear
    self-idle-or-end
;

: room92.act0C ( -- )   \ 00437870
    $40000002 6 -70.0 15.0 0.0 0 0 sound
    4 2 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    0 room92.cmd06
    self-frames-reset
    $F self-wait-frames
    6 ebit-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    4 0 var-set
    2 room92.cmd06
    self-frames-reset
    $F self-wait-frames
    6 ebit-clear
    self-frames-reset
    $384 self-wait-frames
    4 1 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    4 ebit-clear
    self-idle-or-end
;

: room92.act0D ( -- )   \ 004378D0
    $40000006 6 70.0 15.0 0.0 0 0 sound
    5 2 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    1 room92.cmd06
    self-frames-reset
    $F self-wait-frames
    7 ebit-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    5 0 var-set
    3 room92.cmd06
    self-frames-reset
    $F self-wait-frames
    7 ebit-clear
    self-frames-reset
    $384 self-wait-frames
    5 1 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    5 ebit-clear
    self-idle-or-end
;

: room92.act0E ( -- )   \ 0047AE70
    self-idle-or-end
;

:noname   \ room92.act0F (00437930; deferred: used before it is defined)
    7 0 var? 7 1 var? or 7 2 var? or if
        8 10 var-set
        begin
            0.5 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        room92.cmd02
        8 10 var-set
        begin
            1.0 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        8 10 var-set
        begin
            0.5 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        7 150 var-set
    then
    self-frames-reset
    $1E self-wait-frames
    7 var-dec
    7 var-dec
    $32 chance? if
        7 var-dec
    then
    ['] room92.act0F goto
; is room92.act0F

: room92.act10 ( -- )   \ 004379B0
    self-wait-done
    0 -36.4 13.4 54.4 1 effect-86
    1 58.7 10.0 -29.9 1 effect-86
    $A 3 $FF char-load
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
    0 1 $14 door-bits
    $10 $C8 movie-param
    0 $F9 8 action
    $FF panic-stage? if
        3 panic-stage
    then
    9 message-prepare
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

: room92.act11 ( -- )   \ 00437A70
    self-wait-done
    $A 3 $FF char-load
    3 char-unload
    0 0 room92.cmd0C
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
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room92.act12 ( -- )   \ 00437B20
    self-wait-done
    0 -36.4 13.4 54.4 1 effect-86
    1 58.7 10.0 -29.9 1 effect-86
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $FF 1 char-visible
    room92.cmd01
    0 0 room92.cmd0C
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room92.enter $92 0 room-script!
' room92.char-enter $92 6 room-script!
' room92.phase1 $92 1 room-script!
' room92.phase2 $92 2 room-script!
' room92.phase3 $92 3 room-script!
' room92.act00 $92 $00 action-script!
' room92.act01 $92 $01 action-script!
' room92.act02 $92 $02 action-script!
' room92.act03 $92 $03 action-script!
' room92.act04 $92 $04 action-script!
' room92.act05 $92 $05 action-script!
' room92.act06 $92 $06 action-script!
' room92.act07 $92 $07 action-script!
' room92.act08 $92 $08 action-script!
' room92.act09 $92 $09 action-script!
' room92.act0A $92 $0A action-script!
' room92.act0B $92 $0B action-script!
' room92.act0C $92 $0C action-script!
' room92.act0D $92 $0D action-script!
' room92.act0E $92 $0E action-script!
' room92.act0F $92 $0F action-script!
' room92.act10 $92 $10 action-script!
' room92.act11 $92 $11 action-script!
' room92.act12 $92 $12 action-script!

\ ---- room $93 ----------------------------------------------------------------------------------

\ Room93_Cmd00
: room93.cmd00 ( -- )  s" room93.cmd00" stub-step ;

: room93.enter ( -- )   \ 00444170
    $345 story-flag-set
    $354 story-flag? if
        $FE $93 124 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $2E4 story-flag? not if
        0 -117.05 1.0 101.81 flicker-sprite
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
;

: room93.char-enter ( -- )   \ 004441C0
    0 self-is? if
        $80 exit-taken? if
            0 $129 129.0 0.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $93 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: room93.phase1 ( -- )   \ 00444200
    1 ebit? not if
        0 0 char-in-area? if
            1 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            1 ebit-set
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        $347 story-flag? not if
            \ (nop-progress-18: no effect in this game)
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    0 3 char-entered-area? if
        $345 story-flag? $346 story-flag? and $347 story-flag? and $348 story-flag? and if
            0 ebit-set
            \ (nop-progress-18: no effect in this game)
        else
            0 ebit-clear
            \ (nop-progress-18: no effect in this game)
        then
    then
    builtin.act98
    room93.cmd00
;

: room93.phase2 ( -- )   \ 00444270
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $2E4 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: room93.act00 ( -- )   \ 004442A0
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room93.act01 ( -- )   \ 004442B0
    1 self-scripted
    0 ebit? if
        $FF 1.0 0 bgm
        $27 state-flag-clear
    then
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room93.act02 ( -- )   \ 004442D0
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: room93.act03 ( -- )   \ 004442E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    1 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room93.act04 ( -- )   \ 00444300
    self-wait-done
    -117.05 101.81 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E4 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room93.phase5 ( -- )   \ 0047AFD8
    $12 state-flag-clear
;

' room93.enter $93 0 room-script!
' room93.char-enter $93 6 room-script!
' room93.phase1 $93 1 room-script!
' room93.phase2 $93 2 room-script!
' room93.act00 $93 $00 action-script!
' room93.act01 $93 $01 action-script!
' room93.act02 $93 $02 action-script!
' room93.act03 $93 $03 action-script!
' room93.act04 $93 $04 action-script!
' room93.phase5 $93 5 room-script!

\ ---- room $94 ----------------------------------------------------------------------------------

\ room 0x94: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room94.cmd00 ( -- )  s" room94.cmd00" stub-step ;

: room94.enter ( -- )   \ 00444390
    $346 story-flag-set
    $354 story-flag? if
        $FE $94 2 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
;

: room94.char-enter ( -- )   \ 004443C0
    0 self-is? if
        $80 exit-taken? if
            0 $129 129.0 0.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $94 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: room94.phase1 ( -- )   \ 00444400
    0 ebit? not if
        0 0 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        $348 story-flag? not if
            \ (nop-progress-18: no effect in this game)
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    0 3 char-entered-area? if
        $347 story-flag? not if
            \ (nop-progress-18: no effect in this game)
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    builtin.act98
    room94.cmd00
;

: room94.phase2 ( -- )   \ 00444460
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: room94.act00 ( -- )   \ 00444470
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room94.act01 ( -- )   \ 00444480
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room94.act02 ( -- )   \ 00444490
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: room94.act03 ( -- )   \ 004444A0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
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

: room94.phase5 ( -- )   \ 0047AFE0
    $12 state-flag-clear
;

' room94.enter $94 0 room-script!
' room94.char-enter $94 6 room-script!
' room94.phase1 $94 1 room-script!
' room94.phase2 $94 2 room-script!
' room94.act00 $94 $00 action-script!
' room94.act01 $94 $01 action-script!
' room94.act02 $94 $02 action-script!
' room94.act03 $94 $03 action-script!
' room94.phase5 $94 5 room-script!

\ ---- room $95 ----------------------------------------------------------------------------------

\ room 0x95: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room95.cmd00 ( -- )  s" room95.cmd00" stub-step ;

: room95.enter ( -- )   \ 004444E0
    $347 story-flag-set
    $354 story-flag? if
        $FE $95 124 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $2E5 story-flag? not if
        0 -119.22 1.0 -96.82 flicker-sprite
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
;

: room95.char-enter ( -- )   \ 00444530
    0 self-is? if
        $80 exit-taken? if
            0 $129 129.0 0.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $95 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: room95.phase1 ( -- )   \ 00444570
    0 ebit? not if
        0 0 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        $346 story-flag? not if
            \ (nop-progress-18: no effect in this game)
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    0 3 char-entered-area? if
        $348 story-flag? not if
            \ (nop-progress-18: no effect in this game)
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    builtin.act98
    room95.cmd00
;

: room95.phase2 ( -- )   \ 004445D0
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $2E5 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: room95.act00 ( -- )   \ 00444600
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room95.act01 ( -- )   \ 00444610
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room95.act02 ( -- )   \ 00444620
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: room95.act03 ( -- )   \ 00444630
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    3 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room95.act04 ( -- )   \ 00444650
    self-wait-done
    -119.22 -96.82 self-turn-to-xz
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
            $2E5 story-flag-set
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

: room95.phase5 ( -- )   \ 0047AFE8
    $12 state-flag-clear
;

' room95.enter $95 0 room-script!
' room95.char-enter $95 6 room-script!
' room95.phase1 $95 1 room-script!
' room95.phase2 $95 2 room-script!
' room95.act00 $95 $00 action-script!
' room95.act01 $95 $01 action-script!
' room95.act02 $95 $02 action-script!
' room95.act03 $95 $03 action-script!
' room95.act04 $95 $04 action-script!
' room95.phase5 $95 5 room-script!

\ ---- room $96 ----------------------------------------------------------------------------------

\ room 0x96: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room96.cmd00 ( -- )  s" room96.cmd00" stub-step ;

: room96.enter ( -- )   \ 004446E0
    $348 story-flag-set
    $354 story-flag? if
        $FE $96 2 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
;

: room96.char-enter ( -- )   \ 00444710
    0 self-is? if
        $80 exit-taken? if
            0 $129 129.0 0.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $96 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: room96.phase1 ( -- )   \ 00444750
    1 ebit? not if
        0 0 char-in-area? if
            1 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            1 ebit-set
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        $345 story-flag? $346 story-flag? and $347 story-flag? and $348 story-flag? and if
            0 ebit-set
            \ (nop-progress-18: no effect in this game)
        else
            0 ebit-clear
            \ (nop-progress-18: no effect in this game)
        then
    then
    0 3 char-entered-area? if
        $345 story-flag? not if
            \ (nop-progress-18: no effect in this game)
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    builtin.act98
    room96.cmd00
;

: room96.phase2 ( -- )   \ 004447C0
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: room96.act00 ( -- )   \ 004447D0
    1 self-scripted
    0 ebit? if
        $FF 1.0 0 bgm
        $27 state-flag-clear
    then
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room96.act01 ( -- )   \ 004447F0
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: room96.act02 ( -- )   \ 00444800
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: room96.act03 ( -- )   \ 00444810
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
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

: room96.phase5 ( -- )   \ 0047AFF0
    $12 state-flag-clear
;

' room96.enter $96 0 room-script!
' room96.char-enter $96 6 room-script!
' room96.phase1 $96 1 room-script!
' room96.phase2 $96 2 room-script!
' room96.act00 $96 $00 action-script!
' room96.act01 $96 $01 action-script!
' room96.act02 $96 $02 action-script!
' room96.act03 $96 $03 action-script!
' room96.phase5 $96 5 room-script!

\ ---- room $97 ----------------------------------------------------------------------------------

\ room 0x97: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room97.cmd00 ( -- )  s" room97.cmd00" stub-step ;

: room97.enter ( -- )   \ 00437D60
    1 0 8 nav-group
    1 2 $5000000 nav-group
    $FE $F3 1 door-lock-for
    1 $F3 1 door-lock-for
;

: room97.char-enter ( -- )   \ 00437D80
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
;

: room97.phase1 ( -- )   \ 00437DC0
    0 ebit? not if
        0 0 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-18: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    room97.cmd00
;

: room97.phase2 ( -- )   \ 0047AE74
;

: room97.act00 ( -- )   \ 00437E10
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $FE action-end
    1 summon-take
    0 room-frames-set
    $31 1.0 0 bgm
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

' room97.enter $97 0 room-script!
' room97.char-enter $97 6 room-script!
' room97.phase1 $97 1 room-script!
' room97.phase2 $97 2 room-script!
' room97.act00 $97 $00 action-script!

\ ---- room $98 ----------------------------------------------------------------------------------

\ Room98_Cmd00
: room98.cmd00 ( -- )  s" room98.cmd00" stub-step ;

: room98.char-enter ( -- )   \ 00442F60
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

: room98.phase1 ( -- )   \ 00442FE0
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
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    room98.cmd00
    $A8 story-flag? not if
        0 $A char-entered-area? if
            $A8 story-flag-set
            $FE $98 380 2 stalker-to-room
            $FE 0 stalker-mode
            0 $FE 0 action
        then
    then
;

: room98.enter ( -- )   \ 0047AF88
    1 1 8 nav-group
;

: room98.phase2 ( -- )   \ 0047AF90
;

: room98.act00 ( -- )   \ 00443040
    1 self-scripted
    $FF 1 char-visible
    self-wait-done
    $FF 0 char-visible
    $FE $168 65 char-to-tri-facing
    $FE 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    0 self-scripted
    $FE 0 stalker-mode
    self-idle-or-end
;

' room98.char-enter $98 6 room-script!
' room98.phase1 $98 1 room-script!
' room98.enter $98 0 room-script!
' room98.phase2 $98 2 room-script!
' room98.act00 $98 $00 action-script!

\ ---- room $99 ----------------------------------------------------------------------------------

\ Room99_Cmd00
: room99.cmd00 ( -- )  s" room99.cmd00" stub-step ;

: room99.char-enter ( -- )   \ 00443560
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

: room99.phase1 ( -- )   \ 004435E0
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
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    room99.cmd00
    $A8 story-flag? $A9 story-flag? not and 0 $B char-entered-area? and if
        $FE char-here? not if
            $A9 story-flag-set
            $FE action-end
            1 summon-take
        else
            0 ebit-set
        then
    then
    0 ebit? $FE 0 char-action? and if
        0 $FE 0 action
    then
    $AA story-flag? $AB story-flag? not and 0 $D char-left-area? and if
        $FE char-here? not if
            $AB story-flag-set
            $FE $99 310 2 stalker-to-room
            $FE 0 stalker-mode
            0 $FE 1 action
        else
            1 ebit-set
        then
    then
    1 ebit? $FE 0 char-action? and if
        0 $FE 2 action
    then
;

: room99.phase5 ( -- )   \ 00443680
    0 ebit? $A9 story-flag? not and if
        $A9 story-flag-set
        $FE action-end
        1 summon-take
    then
    1 ebit? $AB story-flag? not and if
        $AB story-flag-set
        $FE $99 310 2 stalker-to-room
        $FE 0 stalker-mode
    then
;

: room99.act00 ( -- )   \ 004436B0
    1 self-scripted
    self-wait-done
    $1304 self-anim
    self-wait-anim
    $A9 story-flag-set
    0 ebit-clear
    $FE action-end
    1 summon-take
    self-idle-or-end
;

: room99.act01 ( -- )   \ 004436D0
    1 self-scripted
    $FF 1 char-visible
    self-wait-done
    $FF 0 char-visible
    $FE $136 0 char-to-tri-facing
    $FE 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    $FE 0 stalker-mode
    1 ebit-clear
    $AB story-flag-set
    0 self-scripted
    self-idle-or-end
;

: room99.act02 ( -- )   \ 004436F8
    1 self-scripted
    self-wait-done
    $1304 self-anim
    self-wait-anim
    $FF 1 char-visible
    ['] room99.act01 goto
;

: room99.enter ( -- )   \ 0047AFA0
    1 1 8 nav-group
;

: room99.phase2 ( -- )   \ 0047AFA8
;

' room99.char-enter $99 6 room-script!
' room99.phase1 $99 1 room-script!
' room99.phase5 $99 5 room-script!
' room99.act00 $99 $00 action-script!
' room99.act01 $99 $01 action-script!
' room99.act02 $99 $02 action-script!
' room99.enter $99 0 room-script!
' room99.phase2 $99 2 room-script!

\ ---- room $9A ----------------------------------------------------------------------------------

\ room 0x9A: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: room9A.cmd00 ( -- )  s" room9A.cmd00" stub-step ;

: room9A.char-enter ( -- )   \ 00443740
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

: room9A.phase1 ( -- )   \ 004437C0
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
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    room9A.cmd00
    $A9 story-flag? $AA story-flag? not and if
        0 $C char-entered-area? if
            $AA story-flag-set
            $FE $9A 1065 2 stalker-to-room
            $FE 0 stalker-mode
            0 $FE 0 action
        then
    then
;

: room9A.enter ( -- )   \ 0047AFB0
    1 1 8 nav-group
;

: room9A.phase2 ( -- )   \ 0047AFB8
;

: room9A.act00 ( -- )   \ 00443830
    1 self-scripted
    $FF 1 char-visible
    self-wait-done
    $FF 0 char-visible
    $FE $429 170 char-to-tri-facing
    $FE 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    0 self-scripted
    $FE 0 stalker-mode
    self-idle-or-end
;

' room9A.char-enter $9A 6 room-script!
' room9A.phase1 $9A 1 room-script!
' room9A.enter $9A 0 room-script!
' room9A.phase2 $9A 2 room-script!
' room9A.act00 $9A $00 action-script!

\ ---- room $9B ----------------------------------------------------------------------------------

\ (as Room2D_Cmd02) three hanging things (+0x34 0..2), pushed by the square of Fiona's step past
\ 1, event flag 4 with sounds 4 / 5
: room9B.cmd00 ( b0 -- )  drop s" room9B.cmd00" stub-step ;

: room9B.enter ( -- )   \ 00443870
    room-sounds
    0 room9B.cmd00
    $2E1 story-flag? not if
        0 29.7 5.25 32.54 flicker-sprite
    then
;

: room9B.act04 ( -- )   \ 00443CF0
    $FE camera-follow
    1 ebit-clear
    $24 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $24 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $24 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $24 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 $FE 5 action
    else
        $78 1 item-cooldown
    then
    $24 pvar-inc
    exit
;

: room9B.char-enter ( -- )   \ 00443890
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
    0 self-is? if
        $FE exit-taken? if
            0 $7E 60.0 45.0 180 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 1 action
        then
        0 exit-taken? if
            2 map-page
        then
    then
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            room9B.act04
        else
            2 ebit-clear
        then
    then
;

: room9B.phase1 ( -- )   \ 00443910
    5 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 60.0 20.0 50.0 0 0 sound
            5 ebit-set
        then
    else
        $C0000008 6 60.0 20.0 50.0 0 0 sound
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $4000000A 6 58.0 21.0 -76.0 0 0 sound
            else 0 1 var? if
                $4000000B 6 58.0 21.0 -76.0 0 0 sound
            else 0 2 var? if
                $4000000C 6 58.0 21.0 -76.0 0 0 sound
            else 0 3 var? if
                $4000000D 6 58.0 21.0 -76.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    4 0 0 1 chars-area-camera
    5 2 2 1 chars-area-camera
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 8 char-entered-area? if
        \ (nop-progress-18: no effect in this game)
    then
    0 9 char-entered-area? if
        1 map-page
    then
    0 9 char-left-area? if
        2 map-page
    then
    0 30.47 0.0 -9.97 $B 10 0 zone
    0 0 8 char-zone-bits? if
        1 room9B.cmd00
    then
    1 60.49 0.0 45.99 $14 18 0 zone
    3 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    3 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    1 60.49 0.0 45.99 $14 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: room9B.phase2 ( -- )   \ 00443AB0
    0 $A char-in-area? 0 0 $3C char-heading? and if
        $FE char-here? not if
            5 2 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 3 char-in-area? 0 90 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 6 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    $2E1 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: room9B.phase3 ( -- )   \ 00443B00
    2 camera-mode? not if
        38.0 78.0 -60.2 70.0 55.0 -60.2 38.0 60.0 -60.2 70.0 40.0 -60.2 lights-doorway
    then
    2 camera-mode? not 0 camera-mode? not and if
        90.0 90.0 -40.5 60.0 90.0 -40.5 90.0 52.0 -40.5 60.0 52.0 -40.5 lights-doorway
    then
    91.0 51.8 -63.0 91.0 51.8 -40.0 70.0 51.8 -63.0 70.0 51.8 -40.0 lights-doorway
;

: room9B.act00 ( -- )   \ 00443BA0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    60.0 55.0 self-turn-to-xz
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
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room9B.act01 ( -- )   \ 00443BE0
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    1 map-page
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room9B.act03 ( -- )   \ 00443CC0
    0 ebit? if
        2 avoid-prompt
    then
    9 state-flag-clear
    0 camera-follow
    $18 state-flag-set
    $11F 32.119 -37.94 180 $207 5 self-move-to
    self-wait-done
    4 $14 self-anim-blend
    self-wait-anim
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    self-idle-or-end
;

: room9B.act02 ( -- )   \ 00443C00
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 ebit-clear
    self-wait-done
    $11A 31.33 -24.45 0 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    $800 self-anim
    self-wait-anim
    $28 32.119 -7.5 0 $803 5 self-move-to
    self-wait-done
    $28 32.119 -7.5 180 $803 5 self-move-to
    self-wait-done
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room9B.act03 goto
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
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] room9B.act03 goto
            then
            $18 state-flag-set
            $11A 31.33 -24.45 180 $803 5 self-move-to
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

: room9B.act05 ( -- )   \ 00443D40
    self-wait-done
    $11F 40.89 -25.76 -5 $FFFF 5 self-move-to
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

: room9B.act06 ( -- )   \ 00443D70
    self-wait-done
    29.7 32.54 self-turn-to-xz
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
            $2E1 story-flag-set
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

' room9B.enter $9B 0 room-script!
' room9B.char-enter $9B 6 room-script!
' room9B.phase1 $9B 1 room-script!
' room9B.phase2 $9B 2 room-script!
' room9B.phase3 $9B 3 room-script!
' room9B.act00 $9B $00 action-script!
' room9B.act01 $9B $01 action-script!
' room9B.act02 $9B $02 action-script!
' room9B.act03 $9B $03 action-script!
' room9B.act04 $9B $04 action-script!
' room9B.act05 $9B $05 action-script!
' room9B.act06 $9B $06 action-script!
