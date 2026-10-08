\ story/rooms/old-mansion-1f-16.fs - the event scripts of room old-mansion-1f-16 ($55; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-16
USING: room-names story.words story.shared flag-names ;

\ Room55_Cmd00
: old-mansion-1f-16.cmd00 ( b0 -- )  drop s" old-mansion-1f-16.cmd00" stub-step ;

: old-mansion-1f-16.enter ( -- )   \ 00420B40
    room-sounds
    $11 1.0 0 bgm
    $54 story-flag? not if
        0 1 $14 door-bits
        0 old-mansion-1f-16.cmd00
    else
        1 1 $14 door-bits
        1 old-mansion-1f-16.cmd00
    then
    $272 story-flag? not if
        0 65.56 94.52 19.77 flicker-sprite
    then
    1 $3FFF sound-volume
;

: old-mansion-1f-16.char-enter ( -- )   \ 00420B80
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
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    4 1 1 area-camera
    0 self-is? if
        0 exit-taken? 2 exit-taken? or if
            3 map-page
        then
        1 exit-taken? 3 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-16.phase1 ( -- )   \ 00420CD0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $46 story-flag? not 1 ebit? not and if
        0 2 char-in-area? if
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
    else 2 exit-usable? if
        2 exit-check
    then then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    0 5 char-entered-area? if
        $4A story-flag? not $71 story-flag? and if
            $FE 0 char-file-load
        then
        0 exit-prepare
    then
    0 6 char-entered-area? if
        $4A story-flag? not $71 story-flag? and if
        then
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    0 8 char-entered-area? if
        3 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 $A char-entered-area? if
        4 exit-prepare
    then
    0 6 char-entered-area? if
        2 map-page
    then
    0 6 char-left-area? if
        3 map-page
    then
    0 8 char-entered-area? if
        2 map-page
    then
    0 8 char-left-area? if
        3 map-page
    then
    0 9 char-entered-area? if
        2 map-page
    then
    0 9 char-left-area? if
        1 map-page
    then
;

: old-mansion-1f-16.phase2 ( -- )   \ 00420D80
    $4A story-flag? not if
        0 1 char-group-bit4? if
            5 5 1 scene-change
        then
    then
    -2147483646 scene-request? if
        0 4 char-group-bit4? if
            5 1 1 scene-change
        then
    then
    0 $B char-in-area? 0 83 $3C char-heading? and if
        5 4 0 scene-change
    then
    $272 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
;

: old-mansion-1f-16.act00 ( -- )   \ 00420DD0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    2 exit-check
    self-idle-or-end
;

: old-mansion-1f-16.act01 ( -- )   \ 00420DF0
    self-wait-done
    0 ebit? not 2 game-mode? or $FF panic-stage? or if
        4 self-through-exit
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
            0 message
            wait-message
        then
        0 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $18C -64.3 0.0 -89 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        self-frames-reset
        4 self-wait-frames
        1 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $18C -64.3 1.25 -89 char-to-xz
        world-frozen state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-16.act02 ( -- )   \ 00420E80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 0 6 char-sound
    $11 message-param-room
    $11 item-use
    $63 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-16.act03 ( -- )   \ 00420EC0
    self-wait-done
    65.56 19.77 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $272 story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: old-mansion-1f-16.act04 ( -- )   \ 00420F20
    self-wait-done
    $19F -6.04 30.79 166 $FFFF 5 self-move-to
    self-wait-done
    1 self-scripted
    1 self-noclip
    yield
    world-frozen state-flag-set
    0 $19E -4.65 24.47 166 char-to-xz
    1 35.0 55.0 0.0 0.0 event-camera
    2 message
    wait-message
    self-frames-reset
    self-wait-16
    0 self-scripted
    0 self-noclip
    yield
    world-frozen state-flag-clear
    0 $19F -6.04 30.79 166 char-to-xz
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: old-mansion-1f-16.act05 ( -- )   \ 00420F90
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
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
    wait-fade
    1 action-end
    1 char-done
    0 creatures-clear
    $5A $FF movie-param
    0 0 $14 door-bits
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
    world-held state-flag-set
    $62 door-unlock
    1 exit-check
    self-idle-or-end
;

: old-mansion-1f-16.act06 ( -- )   \ 00421040
    self-wait-done
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
    $5A $FF movie-param
    0 0 $14 door-bits
    0 old-mansion-1f-16.cmd00
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
' old-mansion-1f-16.enter old-mansion-1f-16 0 room-script!
' old-mansion-1f-16.char-enter old-mansion-1f-16 6 room-script!
' old-mansion-1f-16.phase1 old-mansion-1f-16 1 room-script!
' old-mansion-1f-16.phase2 old-mansion-1f-16 2 room-script!
' old-mansion-1f-16.act00 old-mansion-1f-16 $00 action-script!
' old-mansion-1f-16.act01 old-mansion-1f-16 $01 action-script!
' old-mansion-1f-16.act02 old-mansion-1f-16 $02 action-script!
' old-mansion-1f-16.act03 old-mansion-1f-16 $03 action-script!
' old-mansion-1f-16.act04 old-mansion-1f-16 $04 action-script!
' old-mansion-1f-16.act05 old-mansion-1f-16 $05 action-script!
' old-mansion-1f-16.act06 old-mansion-1f-16 $06 action-script!
