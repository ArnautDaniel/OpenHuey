\ story/rooms/castle-1f-11.fs - the event scripts of room castle-1f-11 ($3; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-11
USING: room-names story.words story.shared ;

\ room 0x03 (Room03_Cmd00_ptmf): three objects turned (-60, -60 degrees about x; -90 about z)
: castle-1f-11.cmd00 ( -- )  s" castle-1f-11.cmd00" stub-step ;
\ a lever (pstr_kanagu, tilt +0x18 between -10 and 0 degrees): byte 3 0 back 2 degrees, 1 pulled
\ (-10) with a puff of grey dust at it
: castle-1f-11.cmd01 ( b0 -- )  drop s" castle-1f-11.cmd01" stub-step ;

: castle-1f-11.enter ( -- )   \ 003F0450
    room-sounds
    $16 1.0 0 bgm
    9 story-flag? if
        castle-1f-11.cmd00
    then
    $204 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $287 story-flag? not if
            0 72.5 49.0 8.5 flicker-sprite
        then
    then
    $205 story-flag? not if
        1 2 $8000000 nav-group
        3 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        3 0 $14 door-bits
        2 1 $14 door-bits
        $22A story-flag? not if
            1 53.0 114.0 -113.5 flicker-sprite
        then
    then
    $206 story-flag? not if
        1 1 $8000000 nav-group
        5 1 $14 door-bits
        4 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        5 0 $14 door-bits
        4 1 $14 door-bits
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    $A 3.5 30.7 49.9 $10 $80 $80 $80 $40 specks
    $C 16.0 129.0 35.7 $10 $80 $80 $80 $60 specks
    $235 story-flag? not if
        2 9.646 0.163 27.266 flicker-sprite
    then
;

: castle-1f-11.char-enter ( -- )   \ 003F0540
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
;

: castle-1f-11.phase1 ( -- )   \ 003F0580
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    9 story-flag? not if
        3 1 1 1 chars-area-camera
        4 1 1 1 chars-area-camera
        5 0 0 1 chars-area-camera
        6 0 0 1 chars-area-camera
    then
    7 2 2 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    0 7 char-entered-area? if
        2 map-page
    then
    0 8 char-entered-area? if
        3 map-page
    then
    0 9 char-entered-area? if
        4 map-page
    then
    0 $A char-entered-area? if
        3 map-page
    then
    9 story-flag? not if
        0 castle-1f-11.cmd01
        3 -11.0 113.0 -87.0 5 12 0 zone
        0 3 char-in-zone? if
            1 castle-1f-11.cmd01
            1 $FF 4 rumble
            0 2 var? if
                0 var-inc
                0 $F2 $A action
            else
                0 $F1 3 action
            then
        then
    then
    $235 story-flag? not if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not if
                0 $100000 char-on-nav-flags? not if
                    0 control-action? if
                        4 0 45 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                            0 4 45 $32 char-faces-xz? if
                                hewie-stays? if
                                    0 0 6 action
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    $235 story-flag? not if
        4 9.646 0.163 27.266 $1E 6 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    $1F 4 var-set
                    $1F 6.0 hewie-look-zone
                then
            then
        then
    then
;

: castle-1f-11.phase2 ( -- )   \ 003F06A0
    $204 story-flag? not if
        0 72.5 48.0 8.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $204 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            72.5 48.0 8.5 0 -2142220208 0 0.0 scene-effect-8C
            $88 5 72.5 48.0 8.5 0 0 sound
            $40 $111 noise
            0 72.5 49.0 8.5 flicker-sprite
        then
    else $287 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then then
    $205 story-flag? not if
        1 53.0 113.0 -113.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $205 story-flag-set
            0 2 $8000000 nav-group
            3 0 $14 door-bits
            2 1 $14 door-bits
            53.0 113.0 -113.5 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 53.0 113.0 -113.5 0 0 sound
            $40 $122 noise
            1 53.0 114.0 -113.5 flicker-sprite
        then
    else $22A story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then then
    $206 story-flag? not if
        2 53.0 113.0 -107.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $206 story-flag-set
            0 1 $8000000 nav-group
            5 0 $14 door-bits
            4 1 $14 door-bits
            53.0 113.0 -107.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 53.0 113.0 -107.0 0 0 sound
            $40 $123 noise
        then
    then
    0 2 char-in-area? 0 90 $32 char-heading? and if
        5 2 0 scene-change
    then
;

: castle-1f-11.phase3 ( -- )   \ 003F0820
    78.0 127.0 -100.0 78.0 127.0 -19.0 78.0 90.0 -100.0 78.0 90.0 -19.0 lights-doorway
;

: castle-1f-11.phase5 ( -- )   \ 003F0860
    1 char-busy? if
        1 action-end
        1 $CC 32.509 56.979 60 char-to-xz
        $18 state-flag-clear
    then
    $FE char-busy? 0 ebit? and if
        $FE $8F 2.4 -76.445 90 char-to-xz
        0 ebit-clear
    then
;

: castle-1f-11.act00 ( -- )   \ 003F0890
    1 char-busy? if
        1 action-end
        1 $CC 32.509 56.979 60 char-to-xz
        $18 state-flag-clear
    then
    1 self-scripted
    $FE action-end
    $FE char-done
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $F $44 fade
    $FF 1.0 0 bgm
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
    $FE 2 char-file-load
    0 self-move-16
    0 $8C 1.471 -112.581 -161 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    yield
    camera-restart
    castle-1f-11.cmd00
    9 story-flag-set
    $18 $35 door-copy
    $35 door-close-off-lock
    exits-rebuild
    1 $2F char-in-room? if
        $30 0 -1 hewie-to-room
    then
    $FE char-activate
    $FE 3 153 2 stalker-to-room
    stalker-item-cooldown
    $FE -1 -1 char-camera
    $FE 1 char-visible
    $FE 1 char-silent
    0 $FE 5 action
    $16 1.0 0 bgm
    $F $41 fade
    wait-fade
    $22B item-give
    $18 state-flag-clear
    0 self-scripted
    $FE 3 0 room-doors-state
    0 ebit-clear
    self-idle-or-end
;

: castle-1f-11.act01 ( -- )   \ 003F09C0
    self-wait-done
    9 story-flag? not if
        3 message
        wait-message
    else
        2 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-11.act02 ( -- )   \ 003F09D0
    self-wait-done
    9 story-flag? not if
        $17 state-flag-set
        1 self-scripted
        0 $8E -6.625 -82.651 -144 char-to-xz
        1 10.0 15.0 0.0 0.0 event-camera
        0 0 var? if
            0 message
            wait-message
        else
            1 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        $17 state-flag-clear
        0 self-scripted
    else
        2 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-11.act03 ( -- )   \ 003F0A30
    0 0 6 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    0 var-inc
    self-idle-or-end
;

: castle-1f-11.act04 ( -- )   \ 003F0A40
    self-wait-done
    53.0 -113.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $22A story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: castle-1f-11.act05 ( -- )   \ 003F0AA0
    0 ebit-set
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    yield
    $FE $132 -37.43 44.367 180 char-to-xz
    $FE 0 0 char-camera
    1 self-noclip
    1 self-scripted
    $FE 0 char-visible
    $FE 0 char-silent
    $13C -45.739 -73.201 90 $FFFF $A self-move-to
    self-wait-done
    $8000 5 self-anim-9
    1 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        1 89 var? if
            0 5.0 115.0 -70.0 0 0 0 0 dust
            0 5.0 115.0 -76.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $FE 0 stalker-mode
    0 self-noclip
    0 self-scripted
    0 ebit-clear
    stalker-item-cooldown
    self-idle-or-end
;

: castle-1f-11.act06 ( -- )   \ 003F0B30
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 7 action-force
        $C00 self-anim
        0 1 char-wait-motion
        0 $2E 5 char-sound
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $18 state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-11.act07 ( -- )   \ 003F0B60
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $13 53.585 39.199 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $148 0.0 43.21 1.5 40 hewie-go-to
    self-wait-done
    $145 9.804 32.761 180 $FFFF 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $235 story-flag? not if
        $A state-flag? 0 char-busy? not or if
            $70 $63 item-count? not if
                10 hewie-trust
            then
            $70 message-param-room
            $70 $63 item-count? if
                $8010 message
                wait-message
            else
                $235 story-flag-set
                $70 1 item-give-count
                0 $70 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            $235 story-flag? if
                2 effect-remove
            then
        then
    then
    $148 -7.96 39.28 66 $FFFF 5 self-move-to
    self-wait-done
    $CC 32.509 56.979 1.5 50 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: castle-1f-11.act08 ( -- )   \ 003F0C20
    self-wait-done
    72.5 8.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $287 story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: castle-1f-11.act09 ( -- )   \ 003F0C80
    self-wait-done
    $A 3.5 30.7 49.9 $10 $80 $80 $80 $40 specks
    $C 16.0 129.0 35.7 $10 $80 $80 $80 $60 specks
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-11.act0A ( -- )   \ 003F0D30
    $18 state-flag-set
    0 0 6 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 char-action? if
        0 0 0 action-force
    else
        1 0 0 action-force
    then
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-11.enter castle-1f-11 0 room-script!
' castle-1f-11.char-enter castle-1f-11 6 room-script!
' castle-1f-11.phase1 castle-1f-11 1 room-script!
' castle-1f-11.phase2 castle-1f-11 2 room-script!
' castle-1f-11.phase3 castle-1f-11 3 room-script!
' castle-1f-11.phase5 castle-1f-11 5 room-script!
' castle-1f-11.act00 castle-1f-11 $00 action-script!
' castle-1f-11.act01 castle-1f-11 $01 action-script!
' castle-1f-11.act02 castle-1f-11 $02 action-script!
' castle-1f-11.act03 castle-1f-11 $03 action-script!
' castle-1f-11.act04 castle-1f-11 $04 action-script!
' castle-1f-11.act05 castle-1f-11 $05 action-script!
' castle-1f-11.act06 castle-1f-11 $06 action-script!
' castle-1f-11.act07 castle-1f-11 $07 action-script!
' castle-1f-11.act08 castle-1f-11 $08 action-script!
' castle-1f-11.act09 castle-1f-11 $09 action-script!
' castle-1f-11.act0A castle-1f-11 $0A action-script!
