\ story/rooms/front-garden-2.fs - the event scripts of room front-garden-2 ($13; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-2
USING: room-names story.words story.shared flag-names ;

\ room 0x13: the grate ("kousi"): byte 3 0 shut (turn 0), else swung open a quarter turn.
: front-garden-2.cmd00 ( b0 -- )  drop s" front-garden-2.cmd00" stub-step ;

: front-garden-2.enter ( -- )   \ 003F91A0
    room-sounds
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $10 152.0 92.2 -60.0 $A $80 $80 $80 $30 specks
        $E 128.0 92.2 -60.0 $C $80 $80 $80 $30 specks
        $10 152.0 56.7 12.9 $A $80 $80 $80 $30 specks
        $A 128.0 56.7 12.9 $A $80 $80 $80 $30 specks
    then
    $28 story-flag? not if
        0 front-garden-2.cmd00
        0 1 $20000 nav-group
        1 0 $20000 nav-group
    else
        1 front-garden-2.cmd00
        0 0 $20000 nav-group
        1 1 $20000 nav-group
    then
    $42 story-flag? not if
        6 1 $14 door-bits
    else
        7 1 $14 door-bits
    then
    2 story-flag? 3 story-flag? not and if
        0 30 var-set
        0 $F1 6 action
    then
    $20F story-flag? not if
        1 2 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $210 story-flag? not if
        1 3 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 3 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $283 story-flag? not if
            0 130.0 61.0 -237.0 flicker-sprite
        then
    then
    $211 story-flag? not if
        1 4 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 4 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    0 6 0.0 1.0 0.187 0.312 zone-rect
;

: front-garden-2.char-enter ( -- )   \ 003F92E0
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
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    2 3 -1 area-camera
    $FE self-is? if
    then
;

: front-garden-2.phase1 ( -- )   \ 003F93A0
    $18 story-flag? not 3 story-flag? and $B story-flag? not and if
    else 0 exit-usable? if
        0 exit-check
    then then
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
    9 2 2 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 -1 1 chars-area-camera
    0 3 char-entered-area? if
        2 exit-prepare
    then
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        2 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    3 story-flag? $28 story-flag? not and if
        0 $F char-entered-area? 0 $F char-left-area? or $FE char-here? not and if
            $28 story-flag-set
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
    $18 story-flag? not 3 story-flag? and $B story-flag? not and if
        0 0 char-entered-area? 0 ebit? not and if
            0 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 2 action-force
            else
                1 0 2 action-force
            then
        then
    then
    $20F story-flag? not if
        0 131.0 60.0 -244.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $20F story-flag-set
            0 2 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            131.0 60.0 -244.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 131.0 60.0 -244.0 0 0 sound
            $40 $1E4 noise
        then
    then
    $211 story-flag? not if
        2 137.0 60.0 -240.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $211 story-flag-set
            0 4 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            137.0 60.0 -240.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 137.0 60.0 -240.0 0 0 sound
            $40 $1E4 noise
        then
    then
;

: front-garden-2.phase2 ( -- )   \ 003F9530
    $28 story-flag? not 0 $E char-in-area? and 0 -45 $32 char-heading? and if
        5 0 1 scene-change
    then
    0 $10 char-in-area? 0 90 $3C char-heading? and if
        5 3 0 scene-change
    then
    0 $11 char-in-area? 0 -45 $3C char-heading? and if
        5 4 0 scene-change
    then
    $210 story-flag? not if
        1 130.0 60.0 -237.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $210 story-flag-set
            0 3 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            130.0 60.0 -237.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 130.0 60.0 -237.0 0 0 sound
            $40 $1E4 noise
            0 130.0 61.0 -237.0 flicker-sprite
        then
    else $283 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
;

: front-garden-2.phase3 ( -- )   \ 003F95F0
    151.9 66.3 -63.6 151.9 66.3 -43.5 151.9 44.1 -63.6 151.9 44.1 -43.8 lights-doorway
    148.0 97.0 -176.0 175.0 97.0 -176.0 148.0 97.0 -150.0 175.0 97.0 -150.0 lights-doorway
    109.0 124.0 -176.0 148.0 97.0 -176.0 109.0 124.0 -150.0 148.0 97.0 -150.0 lights-doorway
    148.0 101.0 -176.0 175.0 101.0 -176.0 148.0 97.0 -176.0 175.0 97.0 -176.0 lights-doorway
    109.0 128.0 -176.0 148.0 101.0 -176.0 109.0 124.0 -176.0 148.0 97.0 -176.0 lights-doorway
;

: front-garden-2.act00 ( -- )   \ 003F96E8
    self-wait-done
    $1D02 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: front-garden-2.act01 ( -- )   \ 003F9700
    stalkers-stay state-flag-set
    1 self-scripted
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
    self-wait-done
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
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    wait-fade
    0 $20A 133.34 -76.62 -35 char-to-xz
    0 self-move-16
    camera-restart
    $28 story-flag-set
    $B door-unlock
    1 front-garden-2.cmd00
    0 0 $20000 nav-group
    1 1 $20000 nav-group
    stalkers-stay state-flag-clear
    0 self-scripted
    $F $41 fade
    self-idle-or-end
;

: front-garden-2.act02 ( -- )   \ 003F97C0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    scene-locked state-flag-set
    0 exit-prepare
    stalkers-stay state-flag-clear
    0 self-scripted
    $81 exit-check
    0.0 sound-volume-scale
    self-idle-or-end
;

: front-garden-2.act03 ( -- )   \ 003F97F0
    self-wait-done
    70.0 -160.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    self-idle-or-end
;

: front-garden-2.act04 ( -- )   \ 003F9800
    self-wait-done
    100.0 -210.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    self-idle-or-end
;

: front-garden-2.act05 ( -- )   \ 003F9810
    self-wait-done
    130.0 -237.0 self-turn-to-xz
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
            $283 story-flag-set
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

: front-garden-2.act06 ( -- )   \ 003F9870
    begin
        yield
        0 var-dec
        0 0 var? if
            $1E chance? if
                $F 6 228.0 5.0 78.0 0 0 sound
                0 180 var-set
            else
                0 60 var-set
            then
        then
    again
;

: front-garden-2.act07 ( -- )   \ 003F98B0
    self-wait-done
    $10 152.0 92.2 -60.0 $A $80 $80 $80 $30 specks
    $E 128.0 92.2 -60.0 $C $80 $80 $80 $30 specks
    $10 152.0 56.7 12.9 $A $80 $80 $80 $30 specks
    $A 128.0 56.7 12.9 $A $80 $80 $80 $30 specks
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
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' front-garden-2.enter front-garden-2 0 room-script!
' front-garden-2.char-enter front-garden-2 6 room-script!
' front-garden-2.phase1 front-garden-2 1 room-script!
' front-garden-2.phase2 front-garden-2 2 room-script!
' front-garden-2.phase3 front-garden-2 3 room-script!
' front-garden-2.act00 front-garden-2 $00 action-script!
' front-garden-2.act01 front-garden-2 $01 action-script!
' front-garden-2.act02 front-garden-2 $02 action-script!
' front-garden-2.act03 front-garden-2 $03 action-script!
' front-garden-2.act04 front-garden-2 $04 action-script!
' front-garden-2.act05 front-garden-2 $05 action-script!
' front-garden-2.act06 front-garden-2 $06 action-script!
' front-garden-2.act07 front-garden-2 $07 action-script!
