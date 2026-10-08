\ story/rooms/old-mansion-1f-9.fs - the event scripts of room old-mansion-1f-9 ($49; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-9
USING: room-names story.words story.shared ;

\ a lit quad at x -43, z -15.12 .. 5.07, height 30.05 / 10.05
: old-mansion-1f-9.cmd00 ( b0 -- )  drop s" old-mansion-1f-9.cmd00" stub-step ;
\ the depth range (effect 0x1C) opening with the cutscene from its frame 1156: 1 / 1 / 40 / 100,
\ the far two on by 1 a frame up to 80 / 140
: old-mansion-1f-9.cmd01 ( -- )  s" old-mansion-1f-9.cmd01" stub-step ;
\ byte 3 0: the effect Room49Effect_vtable spawned, its slot in event var 0; 1: removed
: old-mansion-1f-9.cmd02 ( b0 -- )  drop s" old-mansion-1f-9.cmd02" stub-step ;

: old-mansion-1f-9.enter ( -- )   \ 00407F30
    room-sounds
    0 old-mansion-1f-9.cmd00
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

: old-mansion-1f-9.char-enter ( -- )   \ 00407F80
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

: old-mansion-1f-9.phase1 ( -- )   \ 00407FE0
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

: old-mansion-1f-9.phase2 ( -- )   \ 004080A0
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

: old-mansion-1f-9.act00 ( -- )   \ 00408120
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

: old-mansion-1f-9.act01 ( -- )   \ 0047AB78
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-9.act02 ( -- )   \ 00408190
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
                1 old-mansion-1f-9.cmd02
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

: old-mansion-1f-9.act03 ( -- )   \ 004082D0
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

: old-mansion-1f-9.act04 ( -- )   \ 00408390
    self-wait-done
    -6.0 -17.0 self-turn-to-xz
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-9.act05 ( -- )   \ 004083A0
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

: old-mansion-1f-9.act06 ( -- )   \ 00408400
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
    0 old-mansion-1f-9.cmd02
    4 ebit-set
    begin
        5 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-1f-9.cmd02
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
        old-mansion-1f-9.cmd01
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

: old-mansion-1f-9.act07 ( -- )   \ 00408480
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

: old-mansion-1f-9.act08 ( -- )   \ 004084E0
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

: old-mansion-1f-9.act09 ( -- )   \ 00408540
    self-wait-done
    0 old-mansion-1f-9.cmd00
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

\ ---- registered ----
' old-mansion-1f-9.enter old-mansion-1f-9 0 room-script!
' old-mansion-1f-9.char-enter old-mansion-1f-9 6 room-script!
' old-mansion-1f-9.phase1 old-mansion-1f-9 1 room-script!
' old-mansion-1f-9.phase2 old-mansion-1f-9 2 room-script!
' old-mansion-1f-9.act00 old-mansion-1f-9 $00 action-script!
' old-mansion-1f-9.act01 old-mansion-1f-9 $01 action-script!
' old-mansion-1f-9.act02 old-mansion-1f-9 $02 action-script!
' old-mansion-1f-9.act03 old-mansion-1f-9 $03 action-script!
' old-mansion-1f-9.act04 old-mansion-1f-9 $04 action-script!
' old-mansion-1f-9.act05 old-mansion-1f-9 $05 action-script!
' old-mansion-1f-9.act06 old-mansion-1f-9 $06 action-script!
' old-mansion-1f-9.act07 old-mansion-1f-9 $07 action-script!
' old-mansion-1f-9.act08 old-mansion-1f-9 $08 action-script!
' old-mansion-1f-9.act09 old-mansion-1f-9 $09 action-script!
