\ story/rooms/castle-1f-6.fs - the event scripts of room castle-1f-6 ($11; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-6
USING: room-names story.words story.shared ;

: castle-1f-6.enter ( -- )   \ 003F7920
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
    $320 story-flag? not if
        1 1 $14 door-bits
    else
        1 0 $14 door-bits
    then
    $242 story-flag? $243 story-flag? not and if
        $E 34.2 1.0 60.5 flicker-sprite
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
;

: castle-1f-6.char-enter ( -- )   \ 003F7A20
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
        $80 exit-taken? if
            0 $E8 char-to-tri
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
    then
;

: castle-1f-6.phase1 ( -- )   \ 003F7AC0
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
    2 0 0 1 chars-area-camera
    3 1 1 -1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        3 story-flag? $31 story-flag? not and if
            3 0 char-remove
        then
        0 exit-prepare
    then
    0 3 char-left-area? if
        3 story-flag? $31 story-flag? not and if
            3 3 1 char-load
        then
        1 exit-prepare
    then
    $242 story-flag? not if
        0 34.2 0.0 60.5 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 578 var-set
                $1A 579 var-set
                $1B 14 var-set
                $1C 34200 var-set
                $1D 1000 var-set
                $1E 60500 var-set
                $1F 0 var-set
                0 1 $8B action
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
    7 6 8 -4 0 zone-at-effect
    0 7 3 char-zone-bits? 0 7 3 char-zone-bits-before? not and if
        0 6 char-effect-moving
    then
    1 7 3 char-zone-bits? 1 7 3 char-zone-bits-before? not and if
        1 6 char-effect-moving
    then
    $FE 7 3 char-zone-bits? $FE 7 3 char-zone-bits-before? not and if
        $FE 6 char-effect-moving
    then
    8 7 8 -4 0 zone-at-effect
    0 8 3 char-zone-bits? 0 8 3 char-zone-bits-before? not and if
        0 7 char-effect-moving
    then
    1 8 3 char-zone-bits? 1 8 3 char-zone-bits-before? not and if
        1 7 char-effect-moving
    then
    $FE 8 3 char-zone-bits? $FE 8 3 char-zone-bits-before? not and if
        $FE 7 char-effect-moving
    then
    9 8 8 -4 0 zone-at-effect
    0 9 3 char-zone-bits? 0 9 3 char-zone-bits-before? not and if
        0 8 char-effect-moving
    then
    1 9 3 char-zone-bits? 1 9 3 char-zone-bits-before? not and if
        1 8 char-effect-moving
    then
    $FE 9 3 char-zone-bits? $FE 9 3 char-zone-bits-before? not and if
        $FE 8 char-effect-moving
    then
    $A 9 8 -4 0 zone-at-effect
    0 $A 3 char-zone-bits? 0 $A 3 char-zone-bits-before? not and if
        0 9 char-effect-moving
    then
    1 $A 3 char-zone-bits? 1 $A 3 char-zone-bits-before? not and if
        1 9 char-effect-moving
    then
    $FE $A 3 char-zone-bits? $FE $A 3 char-zone-bits-before? not and if
        $FE 9 char-effect-moving
    then
    $B $A 8 -4 0 zone-at-effect
    0 $B 3 char-zone-bits? 0 $B 3 char-zone-bits-before? not and if
        0 $A char-effect-moving
    then
    1 $B 3 char-zone-bits? 1 $B 3 char-zone-bits-before? not and if
        1 $A char-effect-moving
    then
    $FE $B 3 char-zone-bits? $FE $B 3 char-zone-bits-before? not and if
        $FE $A char-effect-moving
    then
    $C $B 8 -4 0 zone-at-effect
    0 $C 3 char-zone-bits? 0 $C 3 char-zone-bits-before? not and if
        0 $B char-effect-moving
    then
    1 $C 3 char-zone-bits? 1 $C 3 char-zone-bits-before? not and if
        1 $B char-effect-moving
    then
    $FE $C 3 char-zone-bits? $FE $C 3 char-zone-bits-before? not and if
        $FE $B char-effect-moving
    then
    $D $C 8 -4 0 zone-at-effect
    0 $D 3 char-zone-bits? 0 $D 3 char-zone-bits-before? not and if
        0 $C char-effect-moving
    then
    1 $D 3 char-zone-bits? 1 $D 3 char-zone-bits-before? not and if
        1 $C char-effect-moving
    then
    $FE $D 3 char-zone-bits? $FE $D 3 char-zone-bits-before? not and if
        $FE $C char-effect-moving
    then
;

: castle-1f-6.phase2 ( -- )   \ 003F7E00
    $320 story-flag? not if
        $E 4.0 8.5 -20.0 5 5 0 zone
        0 $E 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    $242 story-flag? $243 story-flag? not and if
        0 $E 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    0 5 char-in-area? 0 -45 $3C char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 45 $3C char-heading? and if
        5 0 0 scene-change
    then
    0 7 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    -2147483646 scene-request? if
        5 5 1 scene-change
    then
;

: castle-1f-6.act00 ( -- )   \ 003F7E80
    self-wait-done
    2 story-flag? not if
        6 message
        wait-message
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-6.act01 ( -- )   \ 003F7E90
    self-wait-done
    4.0 -20.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $320 story-flag-set
    1 0 $14 door-bits
    $13 message-param-room
    $13 1 item-give-count
    0 $13 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-1f-6.act02 ( -- )   \ 003F7EE0
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
    $FFFF message-close
    3 door-open-clear
    doors-room-in
    8 state-flag-set
    $1D 3 $FF char-load
    3 char-unload
    0 exit-prepare
    $32 $FF movie-param
    0 1 $14 door-bits
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
    3 0 char-remove
    wait-fade
    8 state-flag-set
    3 action-end
    3 char-done
    0 $E8 char-to-tri
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    $39 story-flag-set
    $80 exit-check
    0 0 $14 door-bits
    self-idle-or-end
;

: castle-1f-6.act03 ( -- )   \ 003F7FA0
    self-wait-done
    34.2 60.5 self-turn-to-xz
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
            $243 story-flag-set
            $E effect-remove
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

: castle-1f-6.act04 ( -- )   \ 003F8000
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $A00 self-anim
    self-wait-anim
    8 message
    wait-message
    self-idle-or-end
;

: castle-1f-6.act05 ( -- )   \ 003F8020
    self-wait-done
    $FE self-touching? not if
        3 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 3 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    3 door-unlock
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

: castle-1f-6.act06 ( -- )   \ 003F8060
    self-wait-done
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
    $1D 3 $FF char-load
    3 char-unload
    $32 $FF movie-param
    0 1 $14 door-bits
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
    3 0 char-remove
    3 action-end
    3 char-done
    0 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-6.enter castle-1f-6 0 room-script!
' castle-1f-6.char-enter castle-1f-6 6 room-script!
' castle-1f-6.phase1 castle-1f-6 1 room-script!
' castle-1f-6.phase2 castle-1f-6 2 room-script!
' castle-1f-6.act00 castle-1f-6 $00 action-script!
' castle-1f-6.act01 castle-1f-6 $01 action-script!
' castle-1f-6.act02 castle-1f-6 $02 action-script!
' castle-1f-6.act03 castle-1f-6 $03 action-script!
' castle-1f-6.act04 castle-1f-6 $04 action-script!
' castle-1f-6.act05 castle-1f-6 $05 action-script!
' castle-1f-6.act06 castle-1f-6 $06 action-script!
