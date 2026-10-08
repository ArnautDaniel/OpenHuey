\ story/rooms/castle-2f-1.fs - the event scripts of room castle-2f-1 ($A; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-1
USING: room-names story.words story.shared ;

\ the dial pstr_syuukouki on progress var 3
: castle-2f-1.cmd00 ( b0 -- )  drop s" castle-2f-1.cmd00" stub-step ;
\ room 0x0A (Room0A_Cmd01_ptmf): an effect on its object at -2.88
: castle-2f-1.cmd01 ( -- )  s" castle-2f-1.cmd01" stub-step ;

: castle-2f-1.enter ( -- )   \ 003F3C90
    $26 story-flag? not if
        room-sounds
    then
    $A story-flag? if
        $2F story-flag? not if
            $2F story-flag-set
            $A 0 43 4 15 -1 0 0.0 creature-place
        then
    then
    $24C story-flag? $24D story-flag? not and if
        0 39.5 81.0 -38.8 flicker-sprite
    then
    $282 story-flag? not if
        1 41.5 69.0 31.0 flicker-sprite
    then
    0 castle-2f-1.cmd00
    1 $2300 sound-volume
;

: castle-2f-1.char-enter ( -- )   \ 003F3CF0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
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
        $80 exit-taken? $81 exit-taken? or if
            0 $EB -41.573 -3.787 50 char-to-xz
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            $80 exit-taken? if
                0 0 2 action
            else
                0 0 3 action
            then
        then
    then
;

: castle-2f-1.phase1 ( -- )   \ 003F3DA0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $24C story-flag? not if
        0 39.5 80.0 -38.8 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 588 var-set
                $1A 589 var-set
                $1B 0 var-set
                $1C 39500 var-set
                $1D 81000 var-set
                $1E -38800 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    1 -35.43 90.0 3.42 $10 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 16.78 80.0 -16.27 $17 8 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 8.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-1.phase2 ( -- )   \ 003F3E90
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    0 6 $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 7 $32 char-faces-area? if
        5 5 0 scene-change
    then
    0 8 char-in-area? 0 90 $32 char-heading? and if
        5 6 0 scene-change
    then
    $24C story-flag? $24D story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $282 story-flag? not if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then
;

: castle-2f-1.phase3 ( -- )   \ 003F3EF0
    -3.0 80.0 3.0 -10.0 80.0 -10.0 -2.1 65.0 3.0 -10.0 65.0 -10.0 lights-doorway
    10.0 80.0 10.0 -3.0 80.0 3.0 10.0 65.0 10.0 -3.0 65.0 3.0 lights-doorway
    50.0 100.0 10.0 10.0 100.0 10.0 50.0 40.0 10.0 10.0 40.0 10.0 lights-doorway
    -10.0 80.0 -10.0 -10.0 80.0 -30.0 -10.0 30.0 -10.0 -10.0 30.0 -30.0 lights-doorway
    -26.0 120.0 8.0 -26.0 120.0 13.0 -26.0 90.0 8.0 -26.0 90.0 13.0 lights-doorway
;

: castle-2f-1.act00 ( -- )   \ 003F3FF0
    self-wait-done
    $FE self-touching? not if
        $15 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $15 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $15 door-unlock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: castle-2f-1.act02 ( -- )   \ 003F4110
    1 self-scripted
    self-wait-done
    $80 exit-taken? if
        $FE char-here? if
            0 $FE 7 action
            yield
        then
        $17 state-flag-set
    then
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    4 room-preload
    wait-fade
    $17 state-flag-set
    $10 $FF movie-param
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
    8 state-flag-set
    $17 state-flag-clear
    3 ebit? if
        $80 exit-check
    else
        $81 exit-check
    then
    0 self-scripted
    self-idle-or-end
;

: castle-2f-1.act01 ( -- )   \ 003F4030
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $26 story-flag? if
        1 message
        wait-message
    else
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        $17 state-flag-set
        0 $E6 -43.61 1.45 62 char-to-xz
        1 11.0 0.0 0.0 0.0 event-camera
        2 message
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                1 castle-2f-1.cmd00
                1 ebit? not if
                    0 6 -36.0 -102.0 5.0 0 0 sound
                then
            else
                2 castle-2f-1.cmd00
                1 ebit? 3 3 pvar? and if
                    1 6 -36.0 -102.0 5.0 0 0 sound
                    castle-2f-1.cmd01
                then
            then
            yield
        repeat
        0 $EB -42.2 -4.35 52 char-to-xz
        2 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            3 ebit-set
            $1B state-flag-set
            $F $44 fade
            ['] castle-2f-1.act02 goto
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-1.act03 ( -- )   \ 003F41C0
    1 self-scripted
    self-wait-done
    $22 door-unlock
    $26 story-flag-set
    5 0 creature-count
    $1B state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    camera-restart
    0 exit-prepare
    $F $41 fade
    self-idle-or-end
;

: castle-2f-1.act04 ( -- )   \ 003F41E0
    self-wait-done
    39.5 -38.8 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $24D story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: castle-2f-1.act05 ( -- )   \ 003F4240
    self-wait-done
    18.0 -16.0 self-turn-to-xz
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: castle-2f-1.act06 ( -- )   \ 003F4250
    self-wait-done
    180 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: castle-2f-1.act07 ( -- )   \ 0047A9C0
    self-wait-done
    begin
        yield
    again
;

: castle-2f-1.act08 ( -- )   \ 003F4260
    self-wait-done
    41.5 31.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $282 story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: castle-2f-1.act09 ( -- )   \ 003F42C0
    self-wait-done
    $FF 1 char-visible
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    4 room-preload
    $10 $FF movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-1.enter castle-2f-1 0 room-script!
' castle-2f-1.char-enter castle-2f-1 6 room-script!
' castle-2f-1.phase1 castle-2f-1 1 room-script!
' castle-2f-1.phase2 castle-2f-1 2 room-script!
' castle-2f-1.phase3 castle-2f-1 3 room-script!
' castle-2f-1.act00 castle-2f-1 $00 action-script!
' castle-2f-1.act01 castle-2f-1 $01 action-script!
' castle-2f-1.act02 castle-2f-1 $02 action-script!
' castle-2f-1.act03 castle-2f-1 $03 action-script!
' castle-2f-1.act04 castle-2f-1 $04 action-script!
' castle-2f-1.act05 castle-2f-1 $05 action-script!
' castle-2f-1.act06 castle-2f-1 $06 action-script!
' castle-2f-1.act07 castle-2f-1 $07 action-script!
' castle-2f-1.act08 castle-2f-1 $08 action-script!
' castle-2f-1.act09 castle-2f-1 $09 action-script!
