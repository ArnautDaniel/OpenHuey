\ story/rooms/house-of-truth-1f-1.fs - the event scripts of room house-of-truth-1f-1 ($81; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-1
USING: room-names story.words story.shared flag-names ;

: house-of-truth-1f-1.enter ( -- )   \ 00430AB0
    room-sounds
    $99 story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
        0 1 8 nav-group
        1 0 8 nav-group
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
        0 0 8 nav-group
        1 1 8 nav-group
    then
    0 -24.6 22.1 -26.4 1 effect-86
    $2E9 story-flag? $2EA story-flag? not and if
        1 -78.0 1.0 39.0 flicker-sprite
    then
    $99 story-flag? if
        5 ebit-set
    then
    $AF story-flag? if
        $94 story-flag? $AC story-flag? not and if
            $AC story-flag-set
            $81 0 624 4 10 -1 0 0.0 creature-place
            $81 0 321 4 10 -1 0 0.0 creature-place
        then
    then
    1 $51 $10000000 nav-tri-flags
    1 $21C $10000000 nav-tri-flags
    1 $2F $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $23B $10000000 nav-tri-flags
    1 $43 $10000000 nav-tri-flags
    1 $22C $10000000 nav-tri-flags
    1 $44 $10000000 nav-tri-flags
    1 $22D $10000000 nav-tri-flags
    1 $45 $10000000 nav-tri-flags
    1 $22E $10000000 nav-tri-flags
    1 $46 $10000000 nav-tri-flags
    1 $22F $10000000 nav-tri-flags
    1 $40 $10000000 nav-tri-flags
    1 $229 $10000000 nav-tri-flags
    1 $3D $10000000 nav-tri-flags
    1 $226 $10000000 nav-tri-flags
    1 $3A $10000000 nav-tri-flags
    1 $224 $10000000 nav-tri-flags
    1 $28 $10000000 nav-tri-flags
    1 $216 $10000000 nav-tri-flags
    1 $2D $10000000 nav-tri-flags
    1 $21A $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
    1 $21E $10000000 nav-tri-flags
    1 $31 $10000000 nav-tri-flags
    1 $56 $10000000 nav-tri-flags
    1 $23D $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $234 $10000000 nav-tri-flags
;

: house-of-truth-1f-1.char-enter ( -- )   \ 00430C40
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
                0 4 4 char-camera
                0 camera-follow
            else
                1 4 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
    then then
    1 4 4 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    2 5 5 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    4 2 2 area-camera
    0 self-is? if
        0 exit-taken? 1 exit-taken? or 2 exit-taken? or 4 exit-taken? or if
            2 map-page
        then
    then
;

: house-of-truth-1f-1.phase1 ( -- )   \ 00430D80
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    hewie-controlled? not if
        0 $17 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 $17 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 4 4 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 5 5 1 chars-area-camera
    $11 2 2 1 chars-area-camera
    $12 2 2 1 chars-area-camera
    $13 2 2 1 chars-area-camera
    $14 6 6 1 chars-area-camera
    $15 6 6 1 chars-area-camera
    $16 6 6 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 $E char-entered-area? 0 $10 char-entered-area? or if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        3 exit-prepare
    then
    0 $18 char-entered-area? 0 $D char-entered-area? or 0 $F char-entered-area? or 0 5 char-entered-area? or if
        4 exit-prepare
    then
    0 $1D char-entered-area? if
        1 map-page
    then
    0 $1D char-left-area? if
        2 map-page
    then
    0 $1B char-entered-area? if
        $99 story-flag? not if
            0 game-mode? not $24 1 item-count? and if
                $99 story-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 1 action-force
                else
                    1 0 1 action-force
                then
            then
        then
    then
    $2E9 story-flag? not if
        0 -78.0 0.0 39.0 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 745 var-set
                $1A 746 var-set
                $1B 1 var-set
                $1C -78000 var-set
                $1D 1000 var-set
                $1E 39000 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    3 ebit? not if
        6 sound-bank-loaded? if
            $40000002 6 -24.6 22.1 -26.4 0 0 sound
            3 ebit-set
        then
    else
        $C0000002 6 -24.6 22.1 -26.4 0 0 sound
    then
    5 ebit? if
        4 ebit? not if
            6 sound-bank-loaded? if
                $40000006 6 -21.86 0.0 33.51 0 0 sound
                4 ebit-set
            then
        else
            $C0000006 6 -21.86 0.0 33.51 0 0 sound
        then
    then
;

: house-of-truth-1f-1.phase2 ( -- )   \ 00430F30
    0 $19 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    $99 story-flag? not if
        0 $1A $32 char-faces-area? if
            5 2 0 scene-change
        then
        0 $1B char-in-area? 0 45 $32 char-heading? and if
            5 3 0 scene-change
        then
    else 0 $1C char-in-area? 0 30 $32 char-heading? and 2 ebit? and if
        5 4 0 scene-change
    then then
    $2E9 story-flag? $2EA story-flag? not and if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: house-of-truth-1f-1.phase5 ( -- )   \ 00430F90
    2 ebit? if
        force-calm state-flag-clear
        $FE action-end
        $FE action-end
        3 summon-take
        no-stalker-camera state-flag-clear
        stalker-no-fear state-flag-clear
    then
;

: house-of-truth-1f-1.phase3 ( -- )   \ 00430FB0
    14.0 74.0 -180.0 14.0 74.0 -160.0 14.0 59.0 -180.0 14.0 59.0 -160.0 lights-doorway
    14.0 74.0 -160.0 10.0 74.0 -80.0 10.0 39.0 -160.0 7.0 39.0 -80.0 lights-doorway
    10.0 73.0 -85.5 43.0 73.0 -85.5 10.0 40.0 -85.5 43.0 40.0 -85.5 lights-doorway
    -43.0 73.0 -85.5 -10.0 73.0 -85.5 -43.0 40.0 -85.5 -10.0 40.0 -85.5 lights-doorway
    -70.0 55.0 -60.2 -38.0 78.0 -60.2 -70.0 40.0 -60.2 -38.0 60.0 -60.2 lights-doorway
;

: house-of-truth-1f-1.act00 ( -- )   \ 004310B0
    0 ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $117 0.0 -1.1 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        3 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        0 $84 5 char-sound
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0.0 5.0 self-turn-to-xz
        self-wait-done
        4 message
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-1.act01 ( -- )   \ 00431170
    scene-locked state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    wait-fade
    1 char-here? if
        1 ebit-set
        1 action-end
        1 char-done
    then
    $FE action-end
    $FE char-done
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
    0 $F9 5 action
    $10 $FF movie-param
    0 effect-remove
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
    0 $EF -71.08 32.24 50 char-to-xz
    1 ebit? if
        1 char-activate
        1 $20F -2.73 -54.08 70 char-to-xz
        1 ebit-clear
    then
    $FE action-end
    $FE char-done
    $FE 3 char-file-load
    $FE char-file-use
    0 0 8 nav-group
    1 1 $1000010 nav-group
    $FE $81 457 2 stalker-to-room
    $FE $1C9 -53.111 33.905 -82 char-to-xz
    $FE char-activate
    $FE 1 char-silent
    0 $FE 7 action
    no-stalker-camera state-flag-set
    stalker-no-fear state-flag-set
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    0 0 $14 door-bits
    1 1 $14 door-bits
    $E0 door-unlock
    $24 item-use
    2 ebit-set
    summoner-on state-flag-clear
    force-calm state-flag-set
    0 -24.6 22.1 -26.4 1 effect-86
    5 ebit-set
    $F $51 fade
    wait-fade
    $44 resident-flag-set
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-1.act02 ( -- )   \ 004312C0
    self-wait-done
    -32.0 34.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-1.act03 ( -- )   \ 004312E0
    self-wait-done
    $10C -30.0 -26.0 90 $FFFF 5 self-move-to
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-1.act04 ( -- )   \ 00431300
    self-wait-done
    -53.0 34.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    2 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-1.act05 ( -- )   \ 00431320
    begin
        6 cutscene-shot? not while
        yield
    repeat
    0 0 $14 door-bits
    1 1 $14 door-bits
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: house-of-truth-1f-1.act06 ( -- )   \ 00431340
    self-wait-done
    -78.0 39.0 self-turn-to-xz
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
            $2EA story-flag-set
            1 effect-remove
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

: house-of-truth-1f-1.act07 ( -- )   \ 004313A0
    1 self-scripted
    self-wait-done
    $FE 0 char-silent
    $FF 0 char-visible
    $8000 self-anim
    begin
        yield
    again
;

: house-of-truth-1f-1.act08 ( -- )   \ 004313B0
    self-wait-done
    0 1 $14 door-bits
    $B 3 $FF char-load
    3 char-unload
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
    0 $F9 5 action
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
' house-of-truth-1f-1.enter house-of-truth-1f-1 0 room-script!
' house-of-truth-1f-1.char-enter house-of-truth-1f-1 6 room-script!
' house-of-truth-1f-1.phase1 house-of-truth-1f-1 1 room-script!
' house-of-truth-1f-1.phase2 house-of-truth-1f-1 2 room-script!
' house-of-truth-1f-1.phase5 house-of-truth-1f-1 5 room-script!
' house-of-truth-1f-1.phase3 house-of-truth-1f-1 3 room-script!
' house-of-truth-1f-1.act00 house-of-truth-1f-1 $00 action-script!
' house-of-truth-1f-1.act01 house-of-truth-1f-1 $01 action-script!
' house-of-truth-1f-1.act02 house-of-truth-1f-1 $02 action-script!
' house-of-truth-1f-1.act03 house-of-truth-1f-1 $03 action-script!
' house-of-truth-1f-1.act04 house-of-truth-1f-1 $04 action-script!
' house-of-truth-1f-1.act05 house-of-truth-1f-1 $05 action-script!
' house-of-truth-1f-1.act06 house-of-truth-1f-1 $06 action-script!
' house-of-truth-1f-1.act07 house-of-truth-1f-1 $07 action-script!
' house-of-truth-1f-1.act08 house-of-truth-1f-1 $08 action-script!
