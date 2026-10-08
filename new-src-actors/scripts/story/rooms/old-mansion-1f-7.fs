\ story/rooms/old-mansion-1f-7.fs - the event scripts of room old-mansion-1f-7 ($47; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-7
USING: room-names story.words story.shared ;

: old-mansion-1f-7.enter ( -- )   \ 00424B50
    room-sounds
    $2BB story-flag? $2BC story-flag? not and if
        0 -30.81 1.0 -21.85 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-1f-7.char-enter ( -- )   \ 00424B70
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
        $80 exit-taken? if
            $82 story-flag? not if
                8 state-flag-set
                0.0 sound-volume-scale
                0 0 0 action
                $82 story-flag-set
            then
        then
    then
;

: old-mansion-1f-7.phase1 ( -- )   \ 00424C10
    4 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 0.0 11.0 0.0 0 0 sound
            4 ebit-set
        then
    else
        $C0000000 6 0.0 11.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    2 -68.08 0.0 55.74 $1E 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    0 -1.06 0.0 -0.54 $26 17 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    2 ebit-set
                    $32 chance? if
                        $1F 0 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    0 -1.06 0.0 -0.54 $26 17 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 17.0 hewie-look-zone
            then
        then
    then
    $2BB story-flag? not if
        1 -30.81 0.0 -21.85 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 699 var-set
                $1A 700 var-set
                $1B 0 var-set
                $1C -30810 var-set
                $1D 1000 var-set
                $1E -21850 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
;

: old-mansion-1f-7.phase2 ( -- )   \ 00424D70
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 0 0 $32 char-faces-xz? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 0 0 $32 char-faces-xz? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 -25 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 $C char-in-area? 0 45 $3C char-heading? and if
        5 7 0 scene-change
    then
    $2BB story-flag? $2BC story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: old-mansion-1f-7.act00 ( -- )   \ 00424DE0
    $18 state-flag-set
    1 self-scripted
    1 action-end
    1 char-done
    2 0 char-remove
    self-wait-done
    4 partner-load
    2 char-unload
    $28 state-flag-clear
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
    $FE $47 145 2 stalker-to-room
    3 5 0 char-model-op
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    8 state-flag-set
    $FE $91 -13.81 11.69 125 char-to-xz
    0 exit-prepare
    $80 exit-check
    music-stage-end
    2 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    $44 door-lock
    $47 door-lock
    $56 door-lock
    $5D door-lock
    $7D door-lock
    $34 door-lock
    $79 door-close-off-lock
    exits-rebuild
    $FE $60 1 room-doors-state
    $FE $63 1 room-doors-state
    $FE $10B 1 room-doors-state
    0 state-flag-set
    stalker-item-cooldown
    self-idle-or-end
;

: old-mansion-1f-7.act01 ( -- )   \ 00424ED0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message
    wait-message
    $902 self-anim
    self-wait-anim
    8 item-use
    9 message-param-room
    9 1 item-give-count
    0 9 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-7.act02 ( -- )   \ 00424F40
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    0 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    3 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-7.act03 ( -- )   \ 00425060
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        1 message
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        2 message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $242 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-7.act04 ( -- )   \ 004250B0
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        self-frames-reset
        4 self-wait-frames
        0 $BD 12.41 -5.83 -62 char-to-xz
        1 10.0 -10.0 0.0 3.0 event-camera
        $17 state-flag-set
        4 message
        wait-message
        1 ebit-set
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-7.act05 ( -- )   \ 00425110
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        self-frames-reset
        4 self-wait-frames
        0 $C0 -11.799 6.147 121 char-to-xz
        1 10.0 -10.0 0.0 3.0 event-camera
        $17 state-flag-set
        4 message
        wait-message
        1 ebit-set
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-7.act06 ( -- )   \ 00425170
    self-wait-done
    -30.81 -21.85 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $2BC story-flag-set
            0 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
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

: old-mansion-1f-7.act07 ( -- )   \ 004251D0
    self-wait-done
    12.8 28.7 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        7 message
        wait-message
        3 ebit-set
    else
        8 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-7.act08 ( -- )   \ 00425200
    self-wait-done
    4 partner-load
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
    3 5 0 char-model-op
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-7.enter old-mansion-1f-7 0 room-script!
' old-mansion-1f-7.char-enter old-mansion-1f-7 6 room-script!
' old-mansion-1f-7.phase1 old-mansion-1f-7 1 room-script!
' old-mansion-1f-7.phase2 old-mansion-1f-7 2 room-script!
' old-mansion-1f-7.act00 old-mansion-1f-7 $00 action-script!
' old-mansion-1f-7.act01 old-mansion-1f-7 $01 action-script!
' old-mansion-1f-7.act02 old-mansion-1f-7 $02 action-script!
' old-mansion-1f-7.act03 old-mansion-1f-7 $03 action-script!
' old-mansion-1f-7.act04 old-mansion-1f-7 $04 action-script!
' old-mansion-1f-7.act05 old-mansion-1f-7 $05 action-script!
' old-mansion-1f-7.act06 old-mansion-1f-7 $06 action-script!
' old-mansion-1f-7.act07 old-mansion-1f-7 $07 action-script!
' old-mansion-1f-7.act08 old-mansion-1f-7 $08 action-script!
