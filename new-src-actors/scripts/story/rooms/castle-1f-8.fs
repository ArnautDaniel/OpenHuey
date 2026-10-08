\ story/rooms/castle-1f-8.fs - the event scripts of room castle-1f-8 ($18; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-8
USING: room-names story.words story.shared ;

\ room 0x18 (Room18_Cmd00_ptmf): object byte 3's PlacedObject_ToDef
: castle-1f-8.cmd00 ( b0 -- )  drop s" castle-1f-8.cmd00" stub-step ;

: castle-1f-8.enter ( -- )   \ 003FAA20
    5 story-flag? not if
        room-sounds
    then
    0 34.2 18.3 14.2 1 effect-86
    1 34.2 18.3 -14.2 1 effect-86
    2 -34.2 18.3 -14.2 1 effect-86
    3 -34.2 18.3 14.2 1 effect-86
    $25C story-flag? $25D story-flag? not and if
        4 -25.6 1.0 27.6 flicker-sprite
    then
    $295 story-flag? not if
        5 0.0 8.5 25.0 flicker-sprite
    then
    0 1 $14 door-bits
    1 $2300 sound-volume
;

: castle-1f-8.act07 ( -- )   \ 003FB0A0
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 3 char-file-load
    then
    1 ebit-clear
    $D 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $D 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $D 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $D 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    4 stalker-alert? if
        1 ebit-clear
    then
    1 ebit? if
        0 $FE 8 action
    else
        $78 1 item-cooldown
    then
    $D pvar-inc
    exit
;

: castle-1f-8.char-enter ( -- )   \ 003FAAA0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    1 1 0 area-camera
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            castle-1f-8.act07
        else
            2 ebit-clear
        then
    then
;

: castle-1f-8.phase1 ( -- )   \ 003FAB30
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 0 1 chars-area-camera
    5 0 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 0 -1 chars-area-camera
    0 2 char-entered-area? if
        1 exit-prepare
    then
    0 3 char-entered-area? if
        0 exit-prepare
    then
    5 story-flag? not if
        0 2 char-left-area? if
            5 story-flag-set
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
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    $25C story-flag? not if
        0 -25.6 0.0 27.6 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 604 var-set
                $1A 605 var-set
                $1B 4 var-set
                $1C -25600 var-set
                $1D 1000 var-set
                $1E 27600 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
;

: castle-1f-8.phase2 ( -- )   \ 003FACB0
    $295 story-flag? not if
        5 5 5 5 0 zone-at-effect
        0 5 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 5 5 scene-change
        else
            $8016 scene-ending
        then
    then
    5 story-flag? if
        0 $B char-in-area? 0 75 $2D char-heading? and if
            5 1 0 scene-change
        then
    then
    0 8 char-in-area? 0 45 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 9 $2D char-faces-area? if
        5 3 0 scene-change
    then
    0 $A char-in-area? 0 -67 $2D char-heading? and if
        5 4 0 scene-change
    then
    $25C story-flag? $25D story-flag? not and if
        0 4 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
;

: castle-1f-8.phase3 ( -- )   \ 003FAD40
    34.0 20.0 -15.0 42.0 20.0 -15.0 34.0 0.0 -15.0 42.0 0.0 -15.0 lights-doorway
    42.0 20.0 15.0 34.0 20.0 15.0 42.0 0.0 15.0 34.0 0.0 15.0 lights-doorway
    -43.0 20.0 -15.0 -35.0 20.0 -15.0 -43.0 0.0 -15.0 -35.0 0.0 -15.0 lights-doorway
    -35.0 20.0 15.0 -43.0 20.0 15.0 -35.0 0.0 15.0 -43.0 0.0 15.0 lights-doorway
;

: castle-1f-8.act00 ( -- )   \ 003FAE10
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    0 6 22.0 10.0 -24.0 0 0 sound
    $F03 $A self-anim-blend
    self-wait-anim
    $F $44 fade
    $19 3 $FF char-load
    3 char-unload
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
    0 $F9 $C action
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    0 0 $14 door-bits
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
    0 0 char-no-shadow
    3 0 char-remove
    0 1 $14 door-bits
    4 0 object-show
    5 0 object-show
    4 castle-1f-8.cmd00
    5 castle-1f-8.cmd00
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $18 0 299 hewie-to-room
            1 $12B 73.05 6.36 -100 char-to-xz
        then
    then
    0 self-move-16
    0 $48 19.0 -19.0 150 char-to-xz
    hewie-controlled? not if
        0 2 -1 char-camera
        0 camera-follow
    else
        1 2 -1 char-camera
        1 camera-follow
    then
    5 story-flag-set
    $11 door-open-clear
    $11 door-lock
    0 state-flag-set
    $FE char-activate
    $FE $18 218 2 stalker-to-room
    stalker-item-cooldown
    $FE 0 -1 char-camera
    $FE 2 stalker-mode
    0 $FE $A action
    $31 story-flag-set
    5 0 0 music
    $F $81 fade
    wait-fade
    $208 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-8.act01 ( -- )   \ 003FAF80
    self-wait-done
    $48 19.0 -19.0 150 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 1.0 10.0 0.0 0.0 event-camera
    7 message
    wait-message
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-8.act02 ( -- )   \ 0047AA48
    self-wait-done
    8 message
    wait-message
    self-idle-or-end
;

: castle-1f-8.act03 ( -- )   \ 0047AA50
    self-wait-done
    9 message
    wait-message
    self-idle-or-end
;

: castle-1f-8.act04 ( -- )   \ 0047AA58
    self-wait-done
    $A message
    wait-message
    self-idle-or-end
;

: castle-1f-8.act06 ( -- )   \ 003FB070
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-8.act05 ( -- )   \ 003FAFD0
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 2 char-file-load
    self-wait-done
    0 char-file-use
    $141 $8004 5 -0.803 -15.664 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    0 ebit-clear
    0 avoid-prompt
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-1f-8.act06 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? $FE char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] castle-1f-8.act06 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-1f-8.act08 ( -- )   \ 003FB100
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $B2 0.866 0.417 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        0 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        0 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-1f-8.act09 ( -- )   \ 003FB160
    self-wait-done
    -25.6 27.6 self-turn-to-xz
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
            $25D story-flag-set
            4 effect-remove
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

: castle-1f-8.act0A ( -- )   \ 003FB1C0
    self-wait-done
    4 ebit-set
    $FE $DA 100 char-to-tri-facing
    0 3 self-move-slot
    self-wait-done
    4 ebit-clear
    self-idle-or-end
;

: castle-1f-8.act0B ( -- )   \ 003FB1D0
    self-wait-done
    0.0 25.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $295 story-flag-set
            5 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: castle-1f-8.act0C ( -- )   \ 003FB228
    begin
        8 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    self-idle-or-end
;

: castle-1f-8.act0D ( -- )   \ 003FB240
    self-wait-done
    $19 3 $FF char-load
    3 char-unload
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
    0 $F9 $C action
    0 34.2 18.3 14.2 1 effect-86
    1 34.2 18.3 -14.2 1 effect-86
    2 -34.2 18.3 -14.2 1 effect-86
    3 -34.2 18.3 14.2 1 effect-86
    0 0 $14 door-bits
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
    0 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-8.phase5 ( -- )   \ 0047AA40
    4 ebit? if
        $FE 0 stalker-mode
    then
;

\ ---- registered ----
' castle-1f-8.enter castle-1f-8 0 room-script!
' castle-1f-8.char-enter castle-1f-8 6 room-script!
' castle-1f-8.phase1 castle-1f-8 1 room-script!
' castle-1f-8.phase2 castle-1f-8 2 room-script!
' castle-1f-8.phase3 castle-1f-8 3 room-script!
' castle-1f-8.act00 castle-1f-8 $00 action-script!
' castle-1f-8.act01 castle-1f-8 $01 action-script!
' castle-1f-8.act02 castle-1f-8 $02 action-script!
' castle-1f-8.act03 castle-1f-8 $03 action-script!
' castle-1f-8.act04 castle-1f-8 $04 action-script!
' castle-1f-8.act05 castle-1f-8 $05 action-script!
' castle-1f-8.act06 castle-1f-8 $06 action-script!
' castle-1f-8.act07 castle-1f-8 $07 action-script!
' castle-1f-8.act08 castle-1f-8 $08 action-script!
' castle-1f-8.act09 castle-1f-8 $09 action-script!
' castle-1f-8.act0A castle-1f-8 $0A action-script!
' castle-1f-8.act0B castle-1f-8 $0B action-script!
' castle-1f-8.act0C castle-1f-8 $0C action-script!
' castle-1f-8.act0D castle-1f-8 $0D action-script!
' castle-1f-8.phase5 castle-1f-8 5 room-script!
