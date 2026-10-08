\ story/rooms/castle-1f-3.fs - the event scripts of room castle-1f-3 ($9; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-3
USING: room-names story.words story.shared flag-names ;

\ room 0x09 (Room09_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in
\ another room than 9 (the player about too)
: castle-1f-3.cond00? ( -- flag )  s" castle-1f-3.cond00?" stub-flag ;

: castle-1f-3.enter ( -- )   \ 003F3240
    $3B story-flag? not if
        4 1.0 0 bgm
    then
    0 4 $1000000 nav-group
    $301 story-flag? not if
        1 0 $800088 nav-group
        1 1 $40 nav-group
    else
        2 1 object-show
        0 0 $800088 nav-group
        0 1 $40 nav-group
        0 1 $14 door-bits
        1 2 8 nav-group
        1 3 $40 nav-group
    then
    $223 story-flag? not if
        0 -6.0 11.5 -46.0 flicker-sprite
    then
    $3C story-flag? not if
        3 1 $14 door-bits
    then
    $3B story-flag? not if
        1 1 $14 door-bits
    else
        2 1 $14 door-bits
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 $2300 sound-volume
;

: castle-1f-3.char-enter ( -- )   \ 003F32D0
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
    0 self-is? if
        $301 story-flag? not if
            $3D story-flag? not if
                2 game-mode? $FF panic-stage? not and if
                    castle-1f-3.cond00? if
                        $FE char-here? not if
                            0 0 $C action
                        then
                    then
                then
            then
        then
    then
;

: castle-1f-3.phase1 ( -- )   \ 003F3330
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 1 1 1 chars-area-camera
    $301 story-flag? not if
        0 8 char-in-area? if
            2 game-mode? $FF panic-stage? not and if
                castle-1f-3.cond00? if
                    0 0 0 action
                then
            then
        then
    then
    3 13.13 0.0 -30.63 $1C 11 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    4 ebit-set
                    $32 chance? if
                        $1F 3 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    3 13.13 0.0 -30.63 $1C 11 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    2 26.99 0.0 27.94 $18 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
;

: castle-1f-3.phase2 ( -- )   \ 003F3400
    -2147483644 scene-request? if
        0 0 6 action
    else $301 story-flag? not if
        0 9 char-in-area? 0 90 $2D char-heading? and if
            5 7 0 scene-change
        then
        0 8 char-in-area? 0 0 $2D char-heading? and if
            5 8 0 scene-change
        then
    then then
    0 $A char-in-area? 0 -67 $5A char-heading? and if
        5 $A 0 scene-change
    then
    0 $B char-in-area? 0 45 $2D char-heading? and if
        5 $B 0 scene-change
    then
    0 $C char-in-area? 0 -45 $2D char-heading? and if
        5 $F 0 scene-change
    then
    $223 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
    $3C story-flag? not if
        1 10.0 6.0 -17.0 5 5 0 zone
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: castle-1f-3.phase3 ( -- )   \ 003F34A0
    -0.9 11.8 -26.5 -2.9 11.7 -22.0 0.2 -1.0 -26.0 -1.9 -1.0 -22.0 lights-doorway
;

: castle-1f-3.phase5 ( -- )   \ 003F34D8
    2 ebit? if
        stalkers-stay state-flag-clear
        force-chased state-flag-clear
    then
;

: castle-1f-3.act00 ( -- )   \ 003F34F0
    2 ebit? if
        stalkers-stay state-flag-clear
        $F1 action-end
    then
    1 self-scripted
    $301 story-flag-set
    1 9 1 room-doors-state
    $FE 9 103 2 stalker-to-room
    $FE $67 180 char-to-tri-facing
    $FE 1 char-visible
    $FE action-end
    $FE char-done
    $F $54 fade
    2 story-flag? not if
        6 4 movie-play
        5 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
    else
        1 4 movie-play
        0 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
    then
    wait-fade
    0 $F9 3 action
    1 char-here? if
        0 1 2 action-force
        $303 story-flag-set
    then
    0 1 char-no-shadow
    $FE 1 char-no-shadow
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
    1 char-here? if
        1 action-end
    then
    camera-restart
    $FE 9 143 2 stalker-to-room
    $FE $8F -34.144 33.682 90 char-to-xz
    $FE 0 0 char-camera
    $FE char-activate
    $FE stalker-knock-down
    2 story-flag? not if
        $18 resident-flag-set
    else
        $19 resident-flag-set
    then
    0 self-scripted
    world-held state-flag-set
    0 0 char-no-shadow
    $FE 0 char-no-shadow
    $E door-open-clear
    0 exit-prepare
    0 exit-check
    1 9 0 room-doors-state
    $301 story-flag-set
    $302 story-flag-clear
    self-idle-or-end
;

: castle-1f-3.act01 ( -- )   \ 003F3618
    castle-1f-3.cond00? not if
        0 message
        wait-message
    else
        ['] castle-1f-3.act00 goto
    then
    self-idle-or-end
;

: castle-1f-3.act02 ( -- )   \ 003F3630
    self-wait-done
    $D 4.0 13.0 0 $FFFF $A self-move-to
    self-wait-done
    1 4 $1000000 nav-group
    self-idle-or-end
;

: castle-1f-3.act03 ( -- )   \ 003F3650
    begin
        4 0 cutscene-passed? if
            0 exit-door-open? not if
                $E door-open-set
                doors-room-in
                $FE $28 5 char-sound
            then
        then
        4 1 cutscene-passed? if
            $FE $13 7 char-sound
        then
        4 2 cutscene-passed? if
            1 char-here? if
                0 1 4 action-force
            then
        then
        4 3 cutscene-passed? not while
        yield
    repeat
    self-idle-or-end
;

: castle-1f-3.act04 ( -- )   \ 003F3690
    self-wait-done
    $A1 6.0 33.0 0 $FFFF $A self-move-to
    self-wait-done
    self-idle-or-end
;

: castle-1f-3.act05 ( -- )   \ 003F36B0
    3 ebit-set
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $3B story-flag? not if
        $F $44 fade
        4 4 movie-play
        3 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
        wait-fade
        0 1 char-no-shadow
        0 $F2 $E action
        $FF panic-stage? if
            3 panic-stage
        then
        7 message-prepare
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
        $FF 1.0 0 bgm
        wait-fade
        0 0 char-no-shadow
        $F2 action-end
        1 0 $14 door-bits
        2 1 $14 door-bits
        2 0 object-show
        0 self-move-16
        0 $13C 23.771 -27.607 -73 char-to-xz
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        camera-restart
        $3B story-flag-set
        $F $41 fade
        wait-fade
    else
        10.0 -17.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $3C story-flag-set
        3 0 $14 door-bits
        $14 message-param-room
        $14 1 item-give-count
        0 $14 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $903 self-anim
        self-wait-anim
    then
    2 ebit? not if
        stalkers-stay state-flag-clear
    then
    0 self-scripted
    3 ebit-clear
    self-idle-or-end
;

: castle-1f-3.act06 ( -- )   \ 003F37E0
    begin
        0 char-busy? not while
        yield
    repeat
    1 message
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: castle-1f-3.act07 ( -- )   \ 0047A9B8
    0 message
    wait-message
    self-idle-or-end
;

: castle-1f-3.act08 ( -- )   \ 003F3800
    self-wait-done
    stalkers-stay state-flag-set
    world-frozen state-flag-set
    1 self-scripted
    0 $DA -26.45 -13.04 0 char-to-xz
    1 20.0 -18.0 8.0 0.0 event-camera
    0 message
    wait-message
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-3.act09 ( -- )   \ 003F3850
    self-wait-done
    -6.0 -46.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $223 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: castle-1f-3.act0A ( -- )   \ 003F38B0
    self-wait-done
    0 ebit? not if
        2 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-3.act0B ( -- )   \ 003F38D0
    self-wait-done
    $77 24.585 26.685 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 60.0 -45.0 30.0 event-camera
    self-frames-reset
    $10 self-wait-frames
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    1 ebit? not if
        4 message
        wait-message
        1 ebit-set
    else
        5 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-3.act0C ( -- )   \ 003F3930
    stalkers-stay state-flag-set
    force-chased state-flag-set
    1 self-scripted
    $3D story-flag-set
    2 ebit-set
    self-wait-done
    1 $A self-anim-blend
    6 message
    self-wait-anim
    10 self-move-16
    wait-message
    0 self-scripted
    0 $F1 $D action
    self-idle-or-end
;

: castle-1f-3.act0D ( -- )   \ 003F3960
    self-frames-reset
    $F0 self-wait-frames
    begin
        3 ebit? while
        yield
    repeat
    stalkers-stay state-flag-clear
    force-chased state-flag-clear
    2 ebit-clear
    self-idle-or-end
;

: castle-1f-3.act0E ( -- )   \ 003F3980
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    $FF 1.0 0 bgm
    begin
        4 cutscene-shot? not while
        yield
    repeat
    1 0 $14 door-bits
    2 1 $14 door-bits
    self-idle-or-end
;

: castle-1f-3.act0F ( -- )   \ 003F39A0
    world-frozen state-flag-set
    1 self-scripted
    0 $13B 20.181 -28.363 -90 char-to-xz
    1 10.0 20.0 -30.0 0.0 event-camera
    $3B story-flag? not if
        8 message
        wait-message
    else 5 ebit? not if
        9 message
        wait-message
        5 ebit-set
    else
        $A message
        wait-message
    then then
    self-frames-reset
    $10 self-wait-frames
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-3.act10 ( -- )   \ 003F3A00
    self-wait-done
    4 1.0 0 bgm
    3 1 $14 door-bits
    1 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    4 4 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 char-no-shadow
    0 $F2 $E action
    $FF panic-stage? if
        3 panic-stage
    then
    7 message-prepare
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
    $FF 1.0 0 bgm
    0 0 char-no-shadow
    $F2 action-end
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-3.act11 ( -- )   \ 003F3AB0
    self-wait-done
    2 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    2 3 $FF char-load
    3 char-unload
    6 4 movie-play
    5 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 3 action
    0 1 char-no-shadow
    $FE 1 char-no-shadow
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

: castle-1f-3.act12 ( -- )   \ 003F3B60
    self-wait-done
    2 1 $14 door-bits
    2 3 $FF char-load
    3 char-unload
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 3 action
    0 1 char-no-shadow
    $FE 1 char-no-shadow
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
' castle-1f-3.enter castle-1f-3 0 room-script!
' castle-1f-3.char-enter castle-1f-3 6 room-script!
' castle-1f-3.phase1 castle-1f-3 1 room-script!
' castle-1f-3.phase2 castle-1f-3 2 room-script!
' castle-1f-3.phase3 castle-1f-3 3 room-script!
' castle-1f-3.phase5 castle-1f-3 5 room-script!
' castle-1f-3.act00 castle-1f-3 $00 action-script!
' castle-1f-3.act01 castle-1f-3 $01 action-script!
' castle-1f-3.act02 castle-1f-3 $02 action-script!
' castle-1f-3.act03 castle-1f-3 $03 action-script!
' castle-1f-3.act04 castle-1f-3 $04 action-script!
' castle-1f-3.act05 castle-1f-3 $05 action-script!
' castle-1f-3.act06 castle-1f-3 $06 action-script!
' castle-1f-3.act07 castle-1f-3 $07 action-script!
' castle-1f-3.act08 castle-1f-3 $08 action-script!
' castle-1f-3.act09 castle-1f-3 $09 action-script!
' castle-1f-3.act0A castle-1f-3 $0A action-script!
' castle-1f-3.act0B castle-1f-3 $0B action-script!
' castle-1f-3.act0C castle-1f-3 $0C action-script!
' castle-1f-3.act0D castle-1f-3 $0D action-script!
' castle-1f-3.act0E castle-1f-3 $0E action-script!
' castle-1f-3.act0F castle-1f-3 $0F action-script!
' castle-1f-3.act10 castle-1f-3 $10 action-script!
' castle-1f-3.act11 castle-1f-3 $11 action-script!
' castle-1f-3.act12 castle-1f-3 $12 action-script!
