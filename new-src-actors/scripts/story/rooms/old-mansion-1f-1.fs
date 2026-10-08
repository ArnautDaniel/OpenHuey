\ story/rooms/old-mansion-1f-1.fs - the event scripts of room old-mansion-1f-1 ($2E; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-1
USING: room-names story.words story.shared ;

\ a lit quad at x -15.96, z -2 .. 6, height 111 / 91
: old-mansion-1f-1.cmd00 ( b0 -- )  drop s" old-mansion-1f-1.cmd00" stub-step ;
\ the depth range (effect 0x1C) opening out with the cutscene from its frame 1260: 1 / 21 / 40 /
\ 80 on by 0.4 a frame, up to 41 / 61 / 80 / 120
: old-mansion-1f-1.cmd03 ( -- )  s" old-mansion-1f-1.cmd03" stub-step ;
\ the pursuer's model's +0x9E8 by byte 3: 0 0.15, 1 0, else 0.05
: old-mansion-1f-1.cmd04 ( b0 -- )  drop s" old-mansion-1f-1.cmd04" stub-step ;

: old-mansion-1f-1.enter ( -- )   \ 0041D2E0
    room-sounds
    $4A story-flag? not if
        0 old-mansion-1f-1.cmd00
        0 1 $14 door-bits
        $71 story-flag? if
            7 state-flag-set
            $21 state-flag-set
            $23 state-flag-set
            3 char-activate
            $FE 1 char-silent
            $FE $2E 104 2 stalker-to-room
            $FE 1 1 char-camera
            0 3 1 action
            1 0 $1000000 nav-group
        then
    else
        1 1 $14 door-bits
        1 1 $2008000 nav-group
    then
;

: old-mansion-1f-1.char-enter ( -- )   \ 0041D330
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
    0 self-is? if
        1 exit-taken? if
            3 map-page
        then
    then
;

: old-mansion-1f-1.phase1 ( -- )   \ 0041D3C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 8 char-entered-area? if
        3 map-page
    then
    0 8 char-left-area? if
        2 map-page
    then
    0 $A char-entered-area? if
        2 map-page
    then
    0 $A char-left-area? if
        1 map-page
    then
    $5C story-flag? $71 story-flag? not and if
        0 stalker-alert? not if
            $FE 0 stalker-mode
        then
    then
    $71 story-flag? not if
        0 ebit? not if
            0 8 char-entered-area? if
                0 ebit-set
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
    else $4A story-flag? not if
        0 9 char-entered-area? 2 ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 3 action-force
        then
    then then
    $4A story-flag? not $71 story-flag? and if
        0 -3.91 90.0 3.23 $50 26 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        0 -3.91 90.0 3.23 $50 26 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? $FE char-here? and if
        $FE $80000005 6 char-sound
    then
;

: old-mansion-1f-1.phase5 ( -- )   \ 0041D4E0
    7 state-flag-clear
    $71 story-flag? $4A story-flag? not and if
        7 state-flag-clear
        $21 state-flag-clear
        $23 state-flag-clear
        $FE 0 char-silent
        $FE action-end
        $FE char-done
    then
;

: old-mansion-1f-1.act00 ( -- )   \ 0041D510
    $18 state-flag-set
    1 self-scripted
    self-wait-done
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
    wait-fade
    1 char-full-health
    1 0 char-set-C4
    $2E 0 49 hewie-to-room
    0 1 $86 action
    0 $F9 2 action
    3 4 0 char-model-op
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
    1 old-mansion-1f-1.cmd04
    wait-fade
    8 state-flag-set
    0 self-move-16
    0 $DF 1.052 -47.906 -79 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    3 char-activate
    $FE $2E 104 2 stalker-to-room
    $FE 1 1 char-camera
    0 counter-set
    0 3 1 action
    1 action-end
    1 $31 30 char-to-tri-facing
    $71 story-flag-set
    1 exit-prepare
    0 old-mansion-1f-1.cmd00
    1 wait-counter
    5 0 0 music
    $F $41 fade
    wait-fade
    $233 item-give
    $18 state-flag-clear
    $23 state-flag-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-1.act01 ( -- )   \ 0041D620
    3 2 char-file-load
    self-wait-done
    1 self-noclip
    1 self-scripted
    3 $68 -4.538 3.132 -101 char-to-xz
    7 state-flag-set
    $21 state-flag-set
    3 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 0 char-silent
    1 0 $20000 nav-group
    1 counter-set
    $FE 5 6 char-sound
    begin
        $8000 self-anim
        self-wait-anim
    again
;

: old-mansion-1f-1.act02 ( -- )   \ 0041D670
    depth-range-off
    begin
        $10E cutscene-cue-reached? not while
        yield
    repeat
    0 old-mansion-1f-1.cmd04
    begin
        $1E0 cutscene-cue-reached? not while
        yield
    repeat
    1 old-mansion-1f-1.cmd04
    begin
        5 cutscene-shot? not while
        yield
    repeat
    2 old-mansion-1f-1.cmd00
    2 old-mansion-1f-1.cmd04
    self-frames-reset
    1 self-wait-frames
    1 old-mansion-1f-1.cmd04
    begin
        $3A2 cutscene-cue-reached? not while
        yield
    repeat
    0 old-mansion-1f-1.cmd00
    begin
        $4B0 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4EC cutscene-cue-reached? not while
        1.0 21.0 40.0 80.0 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        old-mansion-1f-1.cmd03
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: old-mansion-1f-1.act03 ( -- )   \ 0041D6E0
    2 ebit-set
    1 self-scripted
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    3 self-look-at
    yield
    0 message
    wait-message
    $FF self-look-at
    yield
    2 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-1.act04 ( -- )   \ 0041D700
    self-wait-done
    0 1 $14 door-bits
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
    0 $F9 2 action
    3 4 0 char-model-op
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
    1 old-mansion-1f-1.cmd04
    2 0 char-remove
    0 old-mansion-1f-1.cmd00
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-1f-1.phase2 ( -- )   \ 0047ACC4
;

\ ---- registered ----
' old-mansion-1f-1.enter old-mansion-1f-1 0 room-script!
' old-mansion-1f-1.char-enter old-mansion-1f-1 6 room-script!
' old-mansion-1f-1.phase1 old-mansion-1f-1 1 room-script!
' old-mansion-1f-1.phase5 old-mansion-1f-1 5 room-script!
' old-mansion-1f-1.act00 old-mansion-1f-1 $00 action-script!
' old-mansion-1f-1.act01 old-mansion-1f-1 $01 action-script!
' old-mansion-1f-1.act02 old-mansion-1f-1 $02 action-script!
' old-mansion-1f-1.act03 old-mansion-1f-1 $03 action-script!
' old-mansion-1f-1.act04 old-mansion-1f-1 $04 action-script!
' old-mansion-1f-1.phase2 old-mansion-1f-1 2 room-script!
