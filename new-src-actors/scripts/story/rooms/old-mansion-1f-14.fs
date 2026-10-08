\ story/rooms/old-mansion-1f-14.fs - the event scripts of room old-mansion-1f-14 ($53; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-14
USING: room-names story.words story.shared flag-names ;

: old-mansion-1f-14.enter ( -- )   \ 004268B0
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

: old-mansion-1f-14.char-enter ( -- )   \ 004268F0
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

: old-mansion-1f-14.phase1 ( -- )   \ 00426930
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
;

: old-mansion-1f-14.phase2 ( -- )   \ 00426950
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

: old-mansion-1f-14.act00 ( -- )   \ 004269E0
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
        world-held state-flag-set
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
        world-held state-flag-clear
    then
    $337 story-flag-set
    world-held state-flag-set
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
        world-held state-flag-set
        $FE $53 0 room-doors-state
        $FE $75 0 door-lock-for
        $FE $53 43 2 stalker-to-room
        $FE char-activate
        $FE $2B 7.18 33.88 -160 char-to-xz
        stalker-item-cooldown
        1 self-scripted
        $333 story-flag-set
        stalkers-stay state-flag-clear
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
        stalkers-stay state-flag-clear
    then
    8 0 object-show
    9 0 object-show
    $F $41 fade
    wait-fade
    $33 resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-14.act01 ( -- )   \ 00426B60
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

: old-mansion-1f-14.act02 ( -- )   \ 00426B80
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

: old-mansion-1f-14.act03 ( -- )   \ 00426B90
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

: old-mansion-1f-14.act04 ( -- )   \ 0047AD38
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-14.act05 ( -- )   \ 00426BF0
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

: old-mansion-1f-14.act06 ( -- )   \ 00426C20
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

: old-mansion-1f-14.act07 ( -- )   \ 00426C40
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

: old-mansion-1f-14.act08 ( -- )   \ 00426C60
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
    8 0 object-show
    9 0 object-show
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-14.enter old-mansion-1f-14 0 room-script!
' old-mansion-1f-14.char-enter old-mansion-1f-14 6 room-script!
' old-mansion-1f-14.phase1 old-mansion-1f-14 1 room-script!
' old-mansion-1f-14.phase2 old-mansion-1f-14 2 room-script!
' old-mansion-1f-14.act00 old-mansion-1f-14 $00 action-script!
' old-mansion-1f-14.act01 old-mansion-1f-14 $01 action-script!
' old-mansion-1f-14.act02 old-mansion-1f-14 $02 action-script!
' old-mansion-1f-14.act03 old-mansion-1f-14 $03 action-script!
' old-mansion-1f-14.act04 old-mansion-1f-14 $04 action-script!
' old-mansion-1f-14.act05 old-mansion-1f-14 $05 action-script!
' old-mansion-1f-14.act06 old-mansion-1f-14 $06 action-script!
' old-mansion-1f-14.act07 old-mansion-1f-14 $07 action-script!
' old-mansion-1f-14.act08 old-mansion-1f-14 $08 action-script!
