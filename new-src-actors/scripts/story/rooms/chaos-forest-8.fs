\ story/rooms/chaos-forest-8.fs - the event scripts of room chaos-forest-8 ($106; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-8
USING: room-names story.words story.shared ;

\ room 0x106: starts a water drip effect (OneDrip, 0xC0 bytes).
: chaos-forest-8.cmd00 ( -- )  s" chaos-forest-8.cmd00" stub-step ;

: chaos-forest-8.enter ( -- )   \ 004182D0
    $86 story-flag? if
        room-sounds
        $19 1.0 0 bgm
    then
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $215 story-flag? not if
        1 2 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2C8 story-flag? not if
            0 -367.0 1.0 -118.7 flicker-sprite
        then
    then
    $216 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $217 story-flag? not if
        1 0 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    $218 story-flag? not if
        1 3 $8000000 nav-group
        7 1 $14 door-bits
        6 0 $14 door-bits
    else
        0 3 $8000000 nav-group
        7 0 $14 door-bits
        6 1 $14 door-bits
    then
    $219 story-flag? not if
        1 4 $8000000 nav-group
        9 1 $14 door-bits
        8 0 $14 door-bits
    else
        0 4 $8000000 nav-group
        9 0 $14 door-bits
        8 1 $14 door-bits
        $2C9 story-flag? not if
            1 -275.5 1.0 -96.0 flicker-sprite
        then
    then
    $21A story-flag? not if
        1 5 $8000000 nav-group
        $B 1 $14 door-bits
        $A 0 $14 door-bits
    else
        0 5 $8000000 nav-group
        $B 0 $14 door-bits
        $A 1 $14 door-bits
    then
    $21B story-flag? not if
        1 6 $8000000 nav-group
        $D 1 $14 door-bits
        $C 0 $14 door-bits
    else
        0 6 $8000000 nav-group
        $D 0 $14 door-bits
        $C 1 $14 door-bits
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    chaos-forest-8.cmd00
;

: chaos-forest-8.char-enter ( -- )   \ 00418430
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
        $86 story-flag? not if
            0 0 0 action
        then
    then
;

: chaos-forest-8.phase1 ( -- )   \ 004184C0
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
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        $1A 4 1 char-load
        $E 5 1 char-load
        $86 story-flag? if
            3 0 char-remove
        then
        1 exit-prepare
    then
    0 3 char-left-area? if
        $86 story-flag? if
            $1C 3 0 char-load
        then
        4 0 char-remove
        5 0 char-remove
    then
    $216 story-flag? not if
        1 -376.0 0.0 -115.9 5 8 1 zone
        $FF 1 char-in-zone? if
            $216 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -376.0 0.0 -115.9 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -376.0 0.0 -115.9 0 0 sound
            $40 $F0 noise
        then
    then
    $217 story-flag? not if
        2 -381.0 0.0 -106.4 5 8 1 zone
        $FF 2 char-in-zone? if
            $217 story-flag-set
            0 0 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -381.0 0.0 -106.4 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -381.0 0.0 -106.4 0 0 sound
            $40 $E6 noise
        then
    then
    $218 story-flag? not if
        3 -275.5 0.0 -105.0 5 8 1 zone
        $FF 3 char-in-zone? if
            $218 story-flag-set
            0 3 $8000000 nav-group
            7 0 $14 door-bits
            6 1 $14 door-bits
            -275.5 0.0 -105.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -275.5 0.0 -105.0 0 0 sound
            $40 $84 noise
        then
    then
    $21A story-flag? not if
        5 -244.5 0.0 -105.0 5 8 1 zone
        $FF 5 char-in-zone? if
            $21A story-flag-set
            0 5 $8000000 nav-group
            $B 0 $14 door-bits
            $A 1 $14 door-bits
            -244.5 0.0 -105.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -244.5 0.0 -105.0 0 0 sound
            $40 $8E noise
        then
    then
    $21B story-flag? not if
        6 -244.5 0.0 -96.0 5 8 1 zone
        $FF 6 char-in-zone? if
            $21B story-flag-set
            0 6 $8000000 nav-group
            $D 0 $14 door-bits
            $C 1 $14 door-bits
            -244.5 0.0 -96.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -244.5 0.0 -96.0 0 0 sound
            $40 $F7 noise
        then
    then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
;

: chaos-forest-8.phase2 ( -- )   \ 00418730
    0 6 $32 char-faces-area? if
        $334 story-flag? not if
            5 1 0 scene-change
        else
            5 $85 3 scene-change
        then
    then
    $215 story-flag? not if
        0 -367.0 0.0 -118.7 5 8 1 zone
        $FF 0 char-in-zone? if
            $215 story-flag-set
            0 2 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -367.0 0.0 -118.7 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -367.0 0.0 -118.7 0 0 sound
            $40 $E8 noise
            0 -367.0 1.0 -118.7 flicker-sprite
        then
    else $2C8 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then then
    $219 story-flag? not if
        4 -275.5 0.0 -96.0 5 8 1 zone
        $FF 4 char-in-zone? if
            $219 story-flag-set
            0 4 $8000000 nav-group
            9 0 $14 door-bits
            8 1 $14 door-bits
            -275.5 0.0 -96.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -275.5 0.0 -96.0 0 0 sound
            $40 $FE noise
            1 -275.5 1.0 -96.0 flicker-sprite
        then
    else $2C9 story-flag? not if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: chaos-forest-8.act00 ( -- )   \ 00418860
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $34 door-lock
    $FE $10B 1 room-doors-state
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
    wait-fade
    50 hewie-trust
    1 action-end
    1 char-done
    3 0 char-remove
    4 0 char-remove
    begin
        loader-done? while
        yield
    repeat
    0 self-move-16
    0 $BD -78.171 -25.963 -73 char-to-xz
    camera-restart
    $86 story-flag-set
    $19 1.0 0 bgm
    $F $41 fade
    wait-fade
    $247 item-give
    $18 state-flag-clear
    0 self-scripted
    $25 state-flag-set
    self-idle-or-end
;

: chaos-forest-8.act01 ( -- )   \ 00418930
    self-wait-done
    0 message
    wait-message
    $334 story-flag-set
    self-idle-or-end
;

: chaos-forest-8.act02 ( -- )   \ 00418940
    self-wait-done
    -367.0 -118.7 self-turn-to-xz
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
            $2C8 story-flag-set
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

: chaos-forest-8.act03 ( -- )   \ 004189A0
    self-wait-done
    -275.5 -96.0 self-turn-to-xz
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
            $2C9 story-flag-set
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

: chaos-forest-8.act04 ( -- )   \ 00418A00
    self-wait-done
    chaos-forest-8.cmd00
    $E 3 $FF char-load
    $F 4 char-load-2
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
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' chaos-forest-8.enter chaos-forest-8 0 room-script!
' chaos-forest-8.char-enter chaos-forest-8 6 room-script!
' chaos-forest-8.phase1 chaos-forest-8 1 room-script!
' chaos-forest-8.phase2 chaos-forest-8 2 room-script!
' chaos-forest-8.act00 chaos-forest-8 $00 action-script!
' chaos-forest-8.act01 chaos-forest-8 $01 action-script!
' chaos-forest-8.act02 chaos-forest-8 $02 action-script!
' chaos-forest-8.act03 chaos-forest-8 $03 action-script!
' chaos-forest-8.act04 chaos-forest-8 $04 action-script!
