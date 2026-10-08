\ story/rooms/house-of-truth-2f-2.fs - the event scripts of room house-of-truth-2f-2 ($83; House of Truth: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-2f-2
USING: room-names story.words story.shared flag-names ;

: house-of-truth-2f-2.enter ( -- )   \ 00431DE0
    0 1 $14 door-bits
    0 1 object-show
    1 1 object-show
    2 1 object-show
    $343 story-flag? not if
        1 1 $14 door-bits
    then
    $2EF story-flag? $2F0 story-flag? not and if
        0 37.0 1.0 84.0 flicker-sprite
    then
;

: house-of-truth-2f-2.char-enter ( -- )   \ 00431E20
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
    0 self-is? if
        $80 exit-taken? if
            0 $81 0 char-to-tri-facing
            $82 1 -1 hewie-to-room
            world-held state-flag-set
            0 0 0 action
        then
    then
;

: house-of-truth-2f-2.phase1 ( -- )   \ 00431E80
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    $2EF story-flag? not if
        0 37.0 0.0 84.0 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 751 var-set
                $1A 752 var-set
                $1B 0 var-set
                $1C 37000 var-set
                $1D 1000 var-set
                $1E 84000 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
;

: house-of-truth-2f-2.phase2 ( -- )   \ 00431EF0
    $343 story-flag? not if
        0 3 char-in-area? 0 52 $32 char-heading? and if
            5 1 4 scene-change
        then
    then
    0 4 char-in-area? 0 -34 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 5 char-in-area? 0 32 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 6 char-in-area? 0 -3 $32 char-heading? and if
        5 5 0 scene-change
    then
    0 7 char-in-area? 0 46 $32 char-heading? and if
        5 6 0 scene-change
    then
    0 9 char-in-area? 0 -35 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 8 char-in-area? 0 -41 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 $A char-in-area? 0 -24 $32 char-heading? and if
        5 9 0 scene-change
    then
    $2EF story-flag? $2F0 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    0 $B char-in-area? 0 12 $32 char-heading? and if
        5 $B 0 scene-change
    then
;

: house-of-truth-2f-2.phase3 ( -- )   \ 00431F90
    40.0 20.0 -9.0 17.0 20.0 -9.0 40.0 0.0 -9.0 23.2 0.0 -9.0 lights-doorway
    -20.0 16.0 40.0 3.0 16.0 40.0 -20.0 0.0 40.0 3.0 0.0 40.0 lights-doorway
    30.0 16.0 32.4 45.0 16.0 32.4 30.0 0.0 32.4 45.0 0.0 32.4 lights-doorway
;

: house-of-truth-2f-2.act00 ( -- )   \ 00432030
    1 self-scripted
    $FE action-end
    $FE char-done
    $10 3 $FF char-load
    $B partner-load
    0 0 $14 door-bits
    self-wait-done
    0 $28 5 char-sound
    3 char-unload
    2 char-unload
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $A action
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
    3 0 char-remove
    $FE action-end
    $FE char-done
    0 self-scripted
    $94 story-flag-set
    0 0 char-no-shadow
    $80 exit-check
    $E2 door-open-clear
    $E2 door-lock
    $43 resident-flag-set
    music-stage-end
    3 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    self-idle-or-end
;

: house-of-truth-2f-2.act01 ( -- )   \ 00432100
    self-wait-done
    40.0 54.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $343 story-flag-set
    1 0 $14 door-bits
    $27 message-param-room
    $27 1 item-give-count
    0 $27 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-2f-2.act02 ( -- )   \ 00432150
    self-wait-done
    $9F 5.3 80.92 -68 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 8.0 0.0 0.0 4.5 event-camera
    1 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-2.act03 ( -- )   \ 004321A0
    self-wait-done
    37.0 84.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F0 story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: house-of-truth-2f-2.act04 ( -- )   \ 0047AE00
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: house-of-truth-2f-2.act05 ( -- )   \ 004321F8
    self-wait-done
    $A02 $A self-anim-blend
    3 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-2f-2.act06 ( -- )   \ 0047AE08
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: house-of-truth-2f-2.act07 ( -- )   \ 0047AE10
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: house-of-truth-2f-2.act08 ( -- )   \ 00432210
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0 ebit? not if
        4 message
        wait-message
        0 ebit-set
    else
        $A02 self-anim
        9 message
        wait-message
        self-wait-anim
        $F 6 fade
        wait-fade
        $A message
        wait-message
        $F 7 fade
        wait-fade
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-2.act09 ( -- )   \ 00432250
    self-wait-done
    $2F6 story-flag? not if
        5 message
        wait-message
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F6 story-flag-set
            $73 1 item-give-count
            0 $73 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-2f-2.act0A ( -- )   \ 004322B0
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: house-of-truth-2f-2.act0B ( -- )   \ 004322D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    25 self-turn-angle
    self-wait-done
    6 message
    wait-message
    $F 6 fade
    wait-fade
    7 message
    wait-message
    $F 7 fade
    wait-fade
    $4B subscreen-bit? not if
        $1D01 self-anim
        8 message
        wait-message
        self-wait-anim
        $4B subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-2.act0C ( -- )   \ 00432310
    self-wait-done
    $B 3 $FF char-load
    3 char-unload
    $10 4 $FF char-load
    4 char-unload
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $A action
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
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-2f-2.enter house-of-truth-2f-2 0 room-script!
' house-of-truth-2f-2.char-enter house-of-truth-2f-2 6 room-script!
' house-of-truth-2f-2.phase1 house-of-truth-2f-2 1 room-script!
' house-of-truth-2f-2.phase2 house-of-truth-2f-2 2 room-script!
' house-of-truth-2f-2.phase3 house-of-truth-2f-2 3 room-script!
' house-of-truth-2f-2.act00 house-of-truth-2f-2 $00 action-script!
' house-of-truth-2f-2.act01 house-of-truth-2f-2 $01 action-script!
' house-of-truth-2f-2.act02 house-of-truth-2f-2 $02 action-script!
' house-of-truth-2f-2.act03 house-of-truth-2f-2 $03 action-script!
' house-of-truth-2f-2.act04 house-of-truth-2f-2 $04 action-script!
' house-of-truth-2f-2.act05 house-of-truth-2f-2 $05 action-script!
' house-of-truth-2f-2.act06 house-of-truth-2f-2 $06 action-script!
' house-of-truth-2f-2.act07 house-of-truth-2f-2 $07 action-script!
' house-of-truth-2f-2.act08 house-of-truth-2f-2 $08 action-script!
' house-of-truth-2f-2.act09 house-of-truth-2f-2 $09 action-script!
' house-of-truth-2f-2.act0A house-of-truth-2f-2 $0A action-script!
' house-of-truth-2f-2.act0B house-of-truth-2f-2 $0B action-script!
' house-of-truth-2f-2.act0C house-of-truth-2f-2 $0C action-script!
