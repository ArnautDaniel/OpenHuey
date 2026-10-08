\ story/rooms/castle-3f-1.fs - the event scripts of room castle-3f-1 ($6; Belli Castle: 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-3f-1
USING: room-names story.words story.shared flag-names ;

\ room 0x06 (Room06_Cmd00_ptmf): its four objects (the name's 6th letter counting) to their
\ places
: castle-3f-1.cmd00 ( -- )  s" castle-3f-1.cmd00" stub-step ;

: castle-3f-1.enter ( -- )   \ 003F1B60
    room-sounds
    $1D story-flag? not if
        1 exit-taken? if
            0 $584 $DF8 obstacle-save-at
            1 $65C $ED0 obstacle-save-at
            2 $76A $FDE obstacle-save-at
            3 $36E $BE2 obstacle-save-at
        else
            0 0 obstacle-model-back
            1 1 obstacle-model-back
            2 2 obstacle-model-back
            3 3 obstacle-model-back
        then
    else
        castle-3f-1.cmd00
    then
    $5B story-flag? $5C story-flag? not and if
        force-followed state-flag-set
        stalker-no-fear state-flag-set
        no-stalker-camera state-flag-set
        $FE char-activate
        $FE 1 char-silent
        $FE 6 236 2 stalker-to-room
        0 $FE 0 char-model-op
        0 $FE 2 action
    then
    1 $2300 sound-volume
;

: castle-3f-1.char-enter ( -- )   \ 003F1BD0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    2 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    3 0 -1 area-camera
;

: castle-3f-1.phase1 ( -- )   \ 003F1CD0
    0 exit-usable? if
        0 exit-check
    then
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
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    8 1 -1 1 chars-area-camera
    9 0 -1 1 chars-area-camera
    $E 1 -1 1 chars-area-camera
    $F 2 -1 1 chars-area-camera
    $10 1 -1 1 chars-area-camera
    $11 2 -1 1 chars-area-camera
    0 4 char-entered-area? 0 $C char-entered-area? or if
        0 exit-prepare
    then
    0 6 char-entered-area? 0 $B char-entered-area? or if
        1 exit-prepare
    then
    0 5 char-entered-area? 0 $A char-entered-area? or if
        2 exit-prepare
    then
    0 7 char-entered-area? 0 $D char-entered-area? or if
        3 exit-prepare
    then
;

: castle-3f-1.phase2 ( -- )   \ 003F1D58
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
;

: castle-3f-1.phase3 ( -- )   \ 003F1D70
    0.0 -2.8 -39.0 -83.0 -2.8 -39.0 0.0 -2.8 -60.0 -83.0 -2.8 -60.0 lights-doorway
    40.0 -2.8 -39.0 0.0 -2.8 -39.0 40.0 -2.8 -60.0 0.0 -2.8 -60.0 lights-doorway
    83.0 -2.8 -39.0 40.0 -2.8 -39.0 83.0 -2.8 -60.0 40.0 -2.8 -60.0 lights-doorway
    0.0 -2.8 39.0 40.0 -2.8 39.0 0.0 -2.8 60.0 40.0 -2.8 60.0 lights-doorway
    40.0 -2.8 39.0 83.0 -2.8 39.0 40.0 -2.8 60.0 83.0 -2.8 60.0 lights-doorway
    -83.0 -2.8 39.0 0.0 -2.8 39.0 -83.0 -2.8 60.0 0.0 -2.8 60.0 lights-doorway
    30.0 -2.0 -57.0 -20.0 -2.0 -57.0 30.0 -2.0 -80.0 -20.0 -2.0 -80.0 lights-doorway
    38.7 19.3 -60.1 20.2 19.3 -60.1 38.7 -4.0 -60.1 20.2 -4.0 -60.1 lights-doorway
;

: castle-3f-1.phase5 ( -- )   \ 003F1F00
    force-followed state-flag-clear
    $5B story-flag? $5C story-flag? not and if
        1 $FE 0 char-model-op
        $FE 0 char-silent
        $FE action-end
        $FE char-done
        0 0 char-in-area? 0 1 char-in-area? or if
            stalker-no-fear state-flag-clear
            no-stalker-camera state-flag-clear
        then
    then
;

: castle-3f-1.act00 ( -- )   \ 003F1F30
    self-wait-done
    0 self-through-exit
    self-wait-done
    0 ebit? not 2 game-mode? or $FF panic-stage? or if
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
            1 message
            wait-message
        then
        0 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $12E 0.0 -76.5 180 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        self-frames-reset
        4 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $12E -1.25 -75.3 180 char-to-xz
        world-frozen state-flag-clear
        0 self-scripted
    then
    $258 item-give
    self-idle-or-end
;

: castle-3f-1.act01 ( -- )   \ 003F1FD0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $34 self-through-door
    self-wait-done
    $41 story-flag? not if
        $F $44 fade
        scene-locked state-flag-set
        3 partner-load
        2 char-unload
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
        1 action-end
        1 char-done
        $11 room-preload
        0 1 $14 door-bits
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
        wait-fade
        world-held state-flag-set
        3 action-end
        3 char-done
        $80 exit-check
        0 0 $14 door-bits
    else
        $609 self-anim
        self-frames-reset
        $14 self-wait-frames
        0 0 6 char-sound
        6 message-param-room
        6 item-use
        $34 door-unlock
        20 self-move-16
        self-frames-reset
        $14 self-wait-frames
        $8019 message
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-3f-1.act02 ( -- )   \ 003F20B0
    1 self-noclip
    1 self-scripted
    $FE $129 -4.85 -47.03 180 char-to-xz
    $FE -1 -1 char-camera
    self-wait-done
    $FE char-file-use
    begin
        $8001 self-anim
        self-wait-anim
    again
;

: castle-3f-1.act03 ( -- )   \ 003F20D0
    self-wait-done
    3 partner-load
    2 char-unload
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
    0 1 $14 door-bits
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
    3 action-end
    3 char-done
    0 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-3f-1.enter castle-3f-1 0 room-script!
' castle-3f-1.char-enter castle-3f-1 6 room-script!
' castle-3f-1.phase1 castle-3f-1 1 room-script!
' castle-3f-1.phase2 castle-3f-1 2 room-script!
' castle-3f-1.phase3 castle-3f-1 3 room-script!
' castle-3f-1.phase5 castle-3f-1 5 room-script!
' castle-3f-1.act00 castle-3f-1 $00 action-script!
' castle-3f-1.act01 castle-3f-1 $01 action-script!
' castle-3f-1.act02 castle-3f-1 $02 action-script!
' castle-3f-1.act03 castle-3f-1 $03 action-script!
