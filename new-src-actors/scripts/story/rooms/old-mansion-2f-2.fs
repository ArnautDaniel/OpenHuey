\ story/rooms/old-mansion-2f-2.fs - the event scripts of room old-mansion-2f-2 ($4A; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-2
USING: room-names story.words story.shared ;

: old-mansion-2f-2.enter ( -- )   \ 004086A0
    1 1 $300000 nav-group
    $84 story-flag? not if
        1 $56 char-in-room? 119 hewie-action? and if
            $4A 0 65 hewie-to-room
            0 1 4 action
            2 ebit-set
        then
    else
        0 1 $14 door-bits
        2 1 object-show
    then
    $27C story-flag? not if
        0 5.64 9.5 5.58 flicker-sprite
    then
    $2BD story-flag? $2BE story-flag? not and if
        1 -49.36 1.0 -47.3 flicker-sprite
    then
;

: old-mansion-2f-2.char-enter ( -- )   \ 00408700
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    0 3 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 3 char-camera
                0 camera-follow
            else
                1 4 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 3 char-camera
            0 camera-follow
        else
            1 4 3 char-camera
            1 camera-follow
        then
    then then
    2 4 3 area-camera
;

: old-mansion-2f-2.phase1 ( -- )   \ 00408780
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    $B 3 2 1 chars-area-camera
    $C 4 3 1 chars-area-camera
    0 9 char-entered-area? if
        0 exit-prepare
    then
    0 $A char-entered-area? if
        2 exit-prepare
    then
    $84 story-flag? not if
        0 0 char-in-area? 0 0 $50 char-heading? and if
            1 control-action? 2 ebit? and $FE char-here? not and 0 0 char-action? and if
                0 0 1 action
            then
        then
    then
    $2BD story-flag? not if
        3 -49.36 0.0 -47.3 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 701 var-set
                $1A 702 var-set
                $1B 1 var-set
                $1C -49360 var-set
                $1D 1000 var-set
                $1E -47300 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: old-mansion-2f-2.phase2 ( -- )   \ 00408840
    2 game-mode? not $FF panic-stage? not and if
        $84 story-flag? not if
            0 0 char-group-bit4? if
                5 5 1 scene-change
            then
        then
    then
    $2BD story-flag? $2BE story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
;

: old-mansion-2f-2.phase5 ( -- )   \ 00408880
    $84 story-flag? not 1 char-here? and 2 ebit? and if
        $56 0 -1 hewie-to-room
        18000 2 hewie-anim
    then
;

: old-mansion-2f-2.act00 ( -- )   \ 0047AB80
    self-idle-or-end
;

: old-mansion-2f-2.act01 ( -- )   \ 004088A0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C02 self-anim
    0 1 char-wait-motion
    0 $31 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
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
    8 state-flag-set
    50 hewie-trust
    0 1 $14 door-bits
    2 1 object-show
    0 self-move-16
    0 $CB -38.75 -31.7 0 char-to-xz
    camera-restart
    $47 door-unlock
    $84 story-flag-set
    2 ebit-clear
    $57 0 -1 hewie-to-room
    $F $41 fade
    wait-fade
    $8283 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-2.act02 ( -- )   \ 0047AB84
    self-idle-or-end
;

: old-mansion-2f-2.act03 ( -- )   \ 0047AB88
    self-idle-or-end
;

: old-mansion-2f-2.act04 ( -- )   \ 00408980
    self-wait-done
    0 self-doorway-fade
    begin
        0 0 var? if
            hewie-bark
            self-wait-done
            $32 chance? if
                0 90 var-set
            else $32 chance? if
                0 150 var-set
            else
                0 30 var-set
            then then
        else
            0 var-dec
            yield
        then
    again
;

: old-mansion-2f-2.act05 ( -- )   \ 004089B0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    2 ebit? not if
        1 ebit? not if
            0 self-through-exit
            self-wait-done
            $609 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            self-frames-reset
            8 self-wait-frames
            0 $CB -38.0 -30.0 30 char-to-xz
            hewie-controlled? not if
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
            self-frames-reset
            4 self-wait-frames
            0 message
            wait-message
            1 ebit-set
        else
            $CB -38.0 -30.0 30 $FFFF 5 self-move-to
            self-wait-done
            hewie-controlled? not if
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
            self-frames-reset
            4 self-wait-frames
            1 message
            wait-message
        then
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    else
        $CB -38.0 -30.0 30 $FFFF 5 self-move-to
        self-wait-done
        2 message
        wait-message
        0 0 var-set
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-2.act06 ( -- )   \ 0047AB8C
    self-idle-or-end
;

: old-mansion-2f-2.act07 ( -- )   \ 00408A60
    self-wait-done
    -49.36 -47.3 self-turn-to-xz
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
            $2BE story-flag-set
            1 effect-remove
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

: old-mansion-2f-2.act08 ( -- )   \ 00408AC0
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-2.enter old-mansion-2f-2 0 room-script!
' old-mansion-2f-2.char-enter old-mansion-2f-2 6 room-script!
' old-mansion-2f-2.phase1 old-mansion-2f-2 1 room-script!
' old-mansion-2f-2.phase2 old-mansion-2f-2 2 room-script!
' old-mansion-2f-2.phase5 old-mansion-2f-2 5 room-script!
' old-mansion-2f-2.act00 old-mansion-2f-2 $00 action-script!
' old-mansion-2f-2.act01 old-mansion-2f-2 $01 action-script!
' old-mansion-2f-2.act02 old-mansion-2f-2 $02 action-script!
' old-mansion-2f-2.act03 old-mansion-2f-2 $03 action-script!
' old-mansion-2f-2.act04 old-mansion-2f-2 $04 action-script!
' old-mansion-2f-2.act05 old-mansion-2f-2 $05 action-script!
' old-mansion-2f-2.act06 old-mansion-2f-2 $06 action-script!
' old-mansion-2f-2.act07 old-mansion-2f-2 $07 action-script!
' old-mansion-2f-2.act08 old-mansion-2f-2 $08 action-script!
