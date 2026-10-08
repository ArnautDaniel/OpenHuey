\ story/rooms/house-of-truth-b1-2.fs - the event scripts of room house-of-truth-b1-2 ($85; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-2
USING: room-names story.words story.shared flag-names ;

\ room 0x85: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-2.cmd00 ( -- )  s" house-of-truth-b1-2.cmd00" stub-step ;

: house-of-truth-b1-2.enter ( -- )   \ 00432700
    room-sounds
    0 0 var-set
    1 0 var-set
    $2E3 story-flag? not if
        0 -45.77 5.75 19.99 flicker-sprite
    then
;

: house-of-truth-b1-2.char-enter ( -- )   \ 00432730
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
            world-held state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 1 action-force
        then
    then
;

: house-of-truth-b1-2.phase1 ( -- )   \ 004327D0
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
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    house-of-truth-b1-2.cmd00
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

: house-of-truth-b1-2.phase2 ( -- )   \ 004328A0
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

: house-of-truth-b1-2.act00 ( -- )   \ 004328E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $F $54 fade
    wait-fade
    scene-locked state-flag-set
    1 exit-prepare
    stalkers-stay state-flag-clear
    0 self-scripted
    $80 exit-check
    scene-locked state-flag-clear
    self-idle-or-end
;

: house-of-truth-b1-2.act01 ( -- )   \ 00432910
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
    scene-locked state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-2.act02 ( -- )   \ 004329D0
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

: house-of-truth-b1-2.act03 ( -- )   \ 00432A30
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

: house-of-truth-b1-2.act04 ( -- )   \ 00432A60
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
' house-of-truth-b1-2.enter house-of-truth-b1-2 0 room-script!
' house-of-truth-b1-2.char-enter house-of-truth-b1-2 6 room-script!
' house-of-truth-b1-2.phase1 house-of-truth-b1-2 1 room-script!
' house-of-truth-b1-2.phase2 house-of-truth-b1-2 2 room-script!
' house-of-truth-b1-2.act00 house-of-truth-b1-2 $00 action-script!
' house-of-truth-b1-2.act01 house-of-truth-b1-2 $01 action-script!
' house-of-truth-b1-2.act02 house-of-truth-b1-2 $02 action-script!
' house-of-truth-b1-2.act03 house-of-truth-b1-2 $03 action-script!
' house-of-truth-b1-2.act04 house-of-truth-b1-2 $04 action-script!
