\ story/rooms/castle-1f-9.fs - the event scripts of room castle-1f-9 ($25; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-9
USING: room-names story.words story.shared ;

\ the rising motes started
: castle-1f-9.cmd00 ( -- )  s" castle-1f-9.cmd00" stub-step ;
\ room 0x25: Fiona is active and in a reaction (action 4) with no character behind it (+0x100
\ 0xFF).
: castle-1f-9.cond00? ( -- flag )  s" castle-1f-9.cond00?" stub-flag ;

: castle-1f-9.enter ( -- )   \ 00402330
    room-sounds
    $20C story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $20D story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $288 story-flag? not if
            1 -5.5 1.0 -99.0 flicker-sprite
        then
    then
    0 6 0.5 1.0 0.187 0.312 zone-rect
    $248 story-flag? $249 story-flag? not and if
        0 60.0 1.0 37.2 flicker-sprite
    then
    $230 story-flag? not if
        2 -33.0 43.5 15.0 flicker-sprite
    then
    castle-1f-9.cmd00
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $10 20.0 19.7 36.4 $E $80 $80 $80 $40 specks
        $10 30.0 87.0 124.7 $E $80 $80 $80 $40 specks
        $10 -53.9 28.8 -52.0 $A $80 $80 $80 $40 specks
    then
;

: castle-1f-9.char-enter ( -- )   \ 00402420
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
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
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 3 1 char-camera
                0 camera-follow
            else
                1 3 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 1 char-camera
            0 camera-follow
        else
            1 3 1 char-camera
            1 camera-follow
        then
    then then
    2 3 1 area-camera
    0 self-is? if
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
        2 exit-taken? if
            3 map-page
        then
    then
;

: castle-1f-9.phase1 ( -- )   \ 004024F0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 1 -1 1 chars-area-camera
    4 0 0 1 chars-area-camera
    7 1 -1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    $B 1 -1 1 chars-area-camera
    $11 0 0 1 chars-area-camera
    $12 3 1 1 chars-area-camera
    $13 0 0 1 chars-area-camera
    $14 4 2 1 chars-area-camera
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? 0 3 char-entered-area? or if
        1 exit-prepare
    then
    0 4 char-entered-area? if
        2 exit-prepare
    then
    0 4 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        2 map-page
    then
    $20C story-flag? not if
        0 -30.0 0.0 -85.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $20C story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -30.0 0.0 -85.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -30.0 0.0 -85.4 0 0 sound
            $40 $1E7 noise
        then
    then
    $248 story-flag? not if
        2 60.0 0.0 37.2 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 584 var-set
                $1A 585 var-set
                $1B 0 var-set
                $1C 60000 var-set
                $1D 1000 var-set
                $1E 37200 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        2 game-mode? not if
            1 hewie-side? 0 $100000 char-on-nav-flags? and if
                0 -37 16 $32 char-faces-xz? if
                    35 fiona-started? if
                        hewie-stays? if
                            0 0 5 action
                        then
                    then
                then
            then
        then
    then
    $2B story-flag? not if
        castle-1f-9.cond00? if
            $2B story-flag-set
            0 $F1 2 action
        then
    then
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 0.0 71.0 35.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000000 6 0.0 71.0 35.0 0 0 sound
    then
    $230 story-flag? not if
        4 -33.85 42.7 15.04 $2C 30 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        3 -33.85 42.7 15.04 $2C 30 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    $1F 3 var-set
                    $1F 10.0 hewie-look-zone
                then
            then
        then
    then
;

: castle-1f-9.phase2 ( -- )   \ 00402700
    $248 story-flag? $249 story-flag? not and if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    0 $C char-in-area? 0 45 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $D $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 $10 char-in-area? 0 35 -45 $32 char-faces-xz? and if
        5 8 0 scene-change
    then
    0 1 char-group-bit4? if
        -2147483646 scene-request? if
            5 9 1 scene-change
        then
    then
    $20D story-flag? not if
        1 -5.5 0.0 -99.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $20D story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -5.5 0.0 -99.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -5.5 0.0 -99.0 0 0 sound
            $40 $1F1 noise
            1 -5.5 1.0 -99.0 flicker-sprite
        then
    else $288 story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then then
;

: castle-1f-9.phase3 ( -- )   \ 004027E0
    -37.6 70.0 29.5 -9.4 70.0 29.5 -37.6 32.2 29.5 -9.4 32.2 29.5 lights-doorway
    6.0 72.0 63.0 72.0 72.0 63.0 6.0 -2.0 63.0 72.0 -2.0 63.0 lights-doorway
    -39.0 72.0 38.0 32.0 72.0 38.0 -39.0 -2.0 38.0 32.0 -2.0 38.0 lights-doorway
;

: castle-1f-9.phase5 ( -- )   \ 00402880
    $37 story-flag? if
        $2B story-flag? not if
            0 0 char-group-bit4? 0 1 char-group-bit4? or if
                $2B story-flag-set
            then
        then
    then
;

: castle-1f-9.act00 ( -- )   \ 004028A0
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        4 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: castle-1f-9.act01 ( -- )   \ 004028C0
    self-wait-done
    0.0 -60.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    self-idle-or-end
;

: castle-1f-9.act02 ( -- )   \ 004028D0
    2 message
    begin
        fiona-free? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 3 action-force
    self-idle-or-end
;

: castle-1f-9.act03 ( -- )   \ 0047AB18
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: castle-1f-9.act04 ( -- )   \ 004028F0
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    $F $54 fade
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
    $28 $FF movie-param
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
    0 8 -19.086 -6.777 166 char-to-xz
    $12 state-flag-set
    $F $51 fade
    wait-fade
    $F 6 fade
    wait-fade
    $40A9 message
    wait-message
    $F 7 fade
    wait-fade
    $37 story-flag-set
    $A9 message-param-room
    $A9 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    2 char-unload
    0 state-flag-set
    $25 story-flag? not if
        $25 story-flag-set
        $25 0 217 3 6 -1 0 0.0 creature-place
    then
    $14 item-use
    6 door-unlock
    0 self-scripted
    $18 state-flag-clear
    $12 state-flag-clear
    self-idle-or-end
;

: castle-1f-9.act05 ( -- )   \ 004029F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 1 6 action-force
    0 counter-set
    $C04 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    begin
        1 $F char-in-area? not while
        yield
    repeat
    1 2 -1 char-camera
    1 wait-counter
    self-frames-reset
    $10 self-wait-frames
    0 self-scripted
    self-idle-or-end
;

: castle-1f-9.act06 ( -- )   \ 00402A30
    self-wait-done
    hewie-bark
    self-wait-done
    1 camera-follow
    $3E -30.81 100.0 180 $FFFF $A self-move-to
    self-wait-done
    0 2 8 nav-group
    1 2 $30 nav-group
    $CF -35.0 20.3 0.2 50 hewie-go-to
    self-wait-done
    1 2 -1 char-camera
    $230 story-flag? not if
        $71 $63 item-count? not if
            10 hewie-trust
        then
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $230 story-flag-set
            $71 1 item-give-count
            0 $71 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $230 story-flag? if
            2 effect-remove
        then
    then
    $50 -35.79 15.06 100 $FFFF $A self-move-to
    self-wait-done
    0 3 8 nav-group
    1 3 $30 nav-group
    1 counter-set
    $196 18.0 6.0 0.1 100 hewie-go-to
    self-wait-done
    1 2 8 nav-group
    1 3 8 nav-group
    $18 state-flag-clear
    $25 0 406 hewie-to-room
    self-idle-or-end
;

: castle-1f-9.act07 ( -- )   \ 00402B00
    self-wait-done
    60.0 37.2 self-turn-to-xz
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
            $249 story-flag-set
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

: castle-1f-9.act08 ( -- )   \ 00402B60
    self-wait-done
    37.0 -44.0 self-turn-to-xz
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: castle-1f-9.act09 ( -- )   \ 00402B70
    self-wait-done
    1 self-through-exit
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
        6 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-9.act0A ( -- )   \ 00402BA0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 1 5 char-sound
    $14 message-param-room
    $14 item-use
    6 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-9.act0B ( -- )   \ 00402BE0
    self-wait-done
    -5.5 -99.0 self-turn-to-xz
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
            $288 story-flag-set
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

: castle-1f-9.act0C ( -- )   \ 00402C40
    self-wait-done
    $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
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
    $28 $FF movie-param
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
' castle-1f-9.enter castle-1f-9 0 room-script!
' castle-1f-9.char-enter castle-1f-9 6 room-script!
' castle-1f-9.phase1 castle-1f-9 1 room-script!
' castle-1f-9.phase2 castle-1f-9 2 room-script!
' castle-1f-9.phase3 castle-1f-9 3 room-script!
' castle-1f-9.phase5 castle-1f-9 5 room-script!
' castle-1f-9.act00 castle-1f-9 $00 action-script!
' castle-1f-9.act01 castle-1f-9 $01 action-script!
' castle-1f-9.act02 castle-1f-9 $02 action-script!
' castle-1f-9.act03 castle-1f-9 $03 action-script!
' castle-1f-9.act04 castle-1f-9 $04 action-script!
' castle-1f-9.act05 castle-1f-9 $05 action-script!
' castle-1f-9.act06 castle-1f-9 $06 action-script!
' castle-1f-9.act07 castle-1f-9 $07 action-script!
' castle-1f-9.act08 castle-1f-9 $08 action-script!
' castle-1f-9.act09 castle-1f-9 $09 action-script!
' castle-1f-9.act0A castle-1f-9 $0A action-script!
' castle-1f-9.act0B castle-1f-9 $0B action-script!
' castle-1f-9.act0C castle-1f-9 $0C action-script!
