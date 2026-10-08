\ story/rooms/house-of-truth-1f-3.fs - the event scripts of room house-of-truth-1f-3 ($8A; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-3
USING: room-names story.words story.shared ;

: house-of-truth-1f-3.enter ( -- )   \ 00433580
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

: house-of-truth-1f-3.char-enter ( -- )   \ 00433600
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

: house-of-truth-1f-3.phase1 ( -- )   \ 00433700
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
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
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

: house-of-truth-1f-3.phase2 ( -- )   \ 004338F0
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

: house-of-truth-1f-3.act00 ( -- )   \ 00433950
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

: house-of-truth-1f-3.act01 ( -- )   \ 00433A50
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

: house-of-truth-1f-3.act02 ( -- )   \ 00433AC0
    self-wait-done
    0 $4F -33.0 -24.0 90 char-to-xz
    1 42.0 10.0 -160.0 0.0 event-camera
    0 message
    wait-message
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: house-of-truth-1f-3.act03 ( -- )   \ 00433AF0
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

: house-of-truth-1f-3.act04 ( -- )   \ 00433B50
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

: house-of-truth-1f-3.act05 ( -- )   \ 00433BB0
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

\ ---- registered ----
' house-of-truth-1f-3.enter house-of-truth-1f-3 0 room-script!
' house-of-truth-1f-3.char-enter house-of-truth-1f-3 6 room-script!
' house-of-truth-1f-3.phase1 house-of-truth-1f-3 1 room-script!
' house-of-truth-1f-3.phase2 house-of-truth-1f-3 2 room-script!
' house-of-truth-1f-3.act00 house-of-truth-1f-3 $00 action-script!
' house-of-truth-1f-3.act01 house-of-truth-1f-3 $01 action-script!
' house-of-truth-1f-3.act02 house-of-truth-1f-3 $02 action-script!
' house-of-truth-1f-3.act03 house-of-truth-1f-3 $03 action-script!
' house-of-truth-1f-3.act04 house-of-truth-1f-3 $04 action-script!
' house-of-truth-1f-3.act05 house-of-truth-1f-3 $05 action-script!
