\ story/rooms/castle-1f-10.fs - the event scripts of room castle-1f-10 ($29; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-10
USING: room-names story.words story.shared flag-names ;

\ the stalker in play is chasing (+0x153C 2, 6 or 7, not +0xC4 2) with the progress state 2: in
\ this room, whether the camera sees it; elsewhere 1
: castle-1f-10.cond00? ( -- flag )  s" castle-1f-10.cond00?" stub-flag ;
\ no thing of kind 3 lies about
: castle-1f-10.cond01? ( -- flag )  s" castle-1f-10.cond01?" stub-flag ;

: castle-1f-10.enter ( -- )   \ 00403F90
    $31D story-flag? if
        2 1 object-show
    then
    0 24.35 9.0 19.7 0 effect-86
    $228 story-flag? not if
        1 11.0 12.0 -58.5 flicker-sprite
    then
    $34 story-flag? $21 story-flag? not and if
        1 0 $1000010 nav-group
        3 char-unload
        2 char-activate
        2 $9000 1 0 char-anim-hold
        2 $83 12.913 14.864 90 char-to-xz
    then
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    0 0 $14 door-bits
    $349 story-flag? if
        $93 1 item-count? not castle-1f-10.cond01? and if
            0 1 $14 door-bits
            2 ebit-set
        then
    then
;

: castle-1f-10.char-enter ( -- )   \ 00404040
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
;

: castle-1f-10.phase1 ( -- )   \ 00404080
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 7.42 0.0 22.15 $F 9 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 9.0 hewie-look-zone
            then
        then
    then
;

: castle-1f-10.phase2 ( -- )   \ 00404100
    $34 story-flag? $21 story-flag? not and if
    else $31D story-flag? not if
        0 5 char-in-area? 0 0 $2D char-heading? and if
            2 game-mode? $FF panic-stage? not and if
                castle-1f-10.cond00? if
                    5 0 6 scene-change
                else
                    $8016 scene-ending
                then
            else
                5 1 0 scene-change
            then
        then
    then then
    $228 story-flag? not if
        0 1 7 4 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $34 story-flag? $21 story-flag? not and if
        0 2 $14 0 $2D char-touching-facing? if
            $22 story-flag? not if
                5 2 0 scene-change
            else 1 ebit? not if
                5 6 0 scene-change
            then then
        then
    then
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 7 char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    2 ebit? if
        0 8 $3C char-faces-area? if
            5 7 4 scene-change
        then
    then
;

: castle-1f-10.phase5 ( -- )   \ 00404198
    $34 story-flag? $21 story-flag? not and if
        2 action-end
        2 char-done
    then
;

: castle-1f-10.act00 ( -- )   \ 004041B0
    1 $29 1 room-doors-state
    1 self-scripted
    $FE $29 116 2 stalker-to-room
    $FE $74 180 char-to-tri-facing
    $FE 0 -1 char-camera
    $FE 1 char-visible
    $FE action-end
    $FE char-done
    $F $44 fade
    3 0 movie-play
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
    1 char-here? if
        1 action-end
        1 char-done
        0 ebit-set
    then
    $20 door-open-set
    doors-room-in
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
    $FE action-end
    $FE char-done
    0 self-scripted
    world-held state-flag-set
    0 exit-prepare
    $20 door-open-clear
    0 ebit? if
        $80 exit-check
    else
        $81 exit-check
    then
    1 $29 0 room-doors-state
    $31D story-flag-set
    $22 resident-flag-set
    self-idle-or-end
;

: castle-1f-10.act01 ( -- )   \ 00404280
    self-wait-done
    $98 7.28 20.316 15 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 20.0 0.0 event-camera
    $A01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-10.act02 ( -- )   \ 004042D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    4 0 movie-play
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
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
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
    0 $4B -13.414 -1.767 -6 char-to-xz
    0 self-move-16
    2 $83 12.913 14.864 90 char-to-xz
    2 $9000 1 0 char-anim-hold
    $22 story-flag-set
    1 0 char-visible
    1 action-end
    camera-restart
    $F $41 fade
    wait-fade
    $29 resident-flag-set
    7 message-param-room
    7 1 item-give-count
    0 7 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-10.act03 ( -- )   \ 004043D0
    self-wait-done
    11.0 -58.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $228 story-flag-set
            1 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: castle-1f-10.act04 ( -- )   \ 00404428
    self-wait-done
    $A01 $A self-anim-blend
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-10.act05 ( -- )   \ 00404440
    self-wait-done
    0 $9C 12.0 15.0 90 char-to-xz
    1 10.0 10.0 0.0 2.0 event-camera
    world-frozen state-flag-set
    1 self-scripted
    self-frames-reset
    self-wait-16
    2 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-10.act06 ( -- )   \ 00404490
    1 self-scripted
    self-wait-done
    2 $9002 0 7 char-anim-hold
    2 wait-char-anim
    2 $9001 1 7 char-anim-hold
    1 ebit-set
    self-idle-or-end
;

: castle-1f-10.act07 ( -- )   \ 004044B0
    self-wait-done
    -3.61 2.23 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $93 message-param-room
    $93 $63 item-count? if
        $8010 message
        wait-message
    else
        $93 1 item-give-count
        2 ebit-clear
        0 0 $14 door-bits
        0 $93 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8011 message
        wait-message
    then
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-1f-10.act08 ( -- )   \ 00404500
    self-wait-done
    0 24.35 9.0 19.7 0 effect-86
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    2 3 $FF char-load
    3 char-unload
    3 0 movie-play
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
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-10.act09 ( -- )   \ 004045E0
    self-wait-done
    0 24.35 9.0 19.7 0 effect-86
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    2 3 $FF char-load
    3 char-unload
    4 0 movie-play
    1 cutscene-start
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
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-10.enter castle-1f-10 0 room-script!
' castle-1f-10.char-enter castle-1f-10 6 room-script!
' castle-1f-10.phase1 castle-1f-10 1 room-script!
' castle-1f-10.phase2 castle-1f-10 2 room-script!
' castle-1f-10.phase5 castle-1f-10 5 room-script!
' castle-1f-10.act00 castle-1f-10 $00 action-script!
' castle-1f-10.act01 castle-1f-10 $01 action-script!
' castle-1f-10.act02 castle-1f-10 $02 action-script!
' castle-1f-10.act03 castle-1f-10 $03 action-script!
' castle-1f-10.act04 castle-1f-10 $04 action-script!
' castle-1f-10.act05 castle-1f-10 $05 action-script!
' castle-1f-10.act06 castle-1f-10 $06 action-script!
' castle-1f-10.act07 castle-1f-10 $07 action-script!
' castle-1f-10.act08 castle-1f-10 $08 action-script!
' castle-1f-10.act09 castle-1f-10 $09 action-script!
' castle-1f-10.char-enter castle-1f-10 $38 action-script!
' castle-1f-10.char-enter castle-1f-10 $3D action-script!
