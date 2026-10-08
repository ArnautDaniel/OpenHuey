\ story/rooms/castle-2f-8.fs - the event scripts of room castle-2f-8 ($24; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-8
USING: room-names story.words story.shared ;

\ Room24_Cmd00
: castle-2f-8.cmd00 ( b0 -- )  drop s" castle-2f-8.cmd00" stub-step ;
\ Room24_Cmd01
: castle-2f-8.cmd01 ( -- )  s" castle-2f-8.cmd01" stub-step ;
\ Room24_Cmd02
: castle-2f-8.cmd02 ( -- )  s" castle-2f-8.cmd02" stub-step ;

: castle-2f-8.enter ( -- )   \ 00401890
    room-sounds
    $1B story-flag? not if
        1 0 $20000 nav-group
        1 5 $800000 nav-group
        1 1 $4000000 nav-group
        0 1 $14 door-bits
    else
        0 0 $14 door-bits
        $1A story-flag? if
            1 2 $20000 nav-group
            1 6 $800000 nav-group
            1 3 $4000000 nav-group
            1 1 $14 door-bits
        else $19 story-flag? if
            2 1 $14 door-bits
        then then
    then
    $1E story-flag? not if
        5 1 $14 door-bits
    then
    castle-2f-8.cmd01
    $209 story-flag? not if
        1 4 $8000000 nav-group
        3 1 $14 door-bits
        4 0 $14 door-bits
    else
        0 4 $8000000 nav-group
        3 0 $14 door-bits
        4 1 $14 door-bits
    then
    0 7 0.812 0.687 0.187 0.312 zone-rect
    $19 story-flag? $315 story-flag? not and if
        0 10.0 1.0 -25.0 flicker-sprite
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    2 2 1.5 light
;

: castle-2f-8.char-enter ( -- )   \ 00401950
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
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
    0 $F1 4 action
;

: castle-2f-8.phase1 ( -- )   \ 004019D0
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000009 6 57.0 0.0 -2.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000009 6 57.0 0.0 -2.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $209 story-flag? not if
        0 34.0 0.0 29.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $209 story-flag-set
            0 4 $8000000 nav-group
            3 0 $14 door-bits
            4 1 $14 door-bits
            34.0 0.0 29.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 34.0 0.0 29.0 0 0 sound
            $40 $FF noise
        then
    then
    1 0 4 4 0 zone-at-effect
    $1B story-flag? if
        $1A story-flag? if
            2 43.743 0.0 -29.309 $F 20 0 zone
            2 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 2 9 char-zone-bits? if
                            2 ebit-set
                            $1E chance? if
                                $1F 2 var-set
                                0 1 $89 action
                            then
                        then
                    then
                then
            then
        else $19 story-flag? if
            1 0 char-in-area? not if
                2 10.0 0.0 -25.0 $A 20 0 zone
                2 ebit? not 1 char-here? and 1 0 char-C4? and if
                    2 game-mode? not if
                        hewie-can-command? if
                            1 2 9 char-zone-bits? if
                                2 ebit-set
                                $1E chance? if
                                    $1F 2 var-set
                                    0 1 $89 action
                                then
                            then
                        then
                    then
                then
            then
        then then
    then
    3 49.99 0.0 -0.54 $12 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-8.phase2 ( -- )   \ 00401B50
    -2147483644 scene-request? if
        0 0 8 action
    else $1B story-flag? not if
        0 6 $32 char-faces-area? if
            5 0 0 scene-change
        then
    else 0 $B $32 char-faces-area? $1A story-flag? and if
        5 9 0 scene-change
    then then then
    0 $C char-in-area? $1E story-flag? not and if
        0 0 $32 char-heading? if
            5 6 4 scene-change
        then
    else 0 7 $3C char-faces-area? if
        5 1 0 scene-change
    then then
    0 8 char-in-area? 0 45 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 9 char-in-area? 0 $A char-in-area? or 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    0 1 2 char-zone-bits? if
        5 7 4 scene-change
    then
;

: castle-2f-8.act00 ( -- )   \ 00401BD0
    self-wait-done
    $F5 10.0 -28.0 180 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 10.0 -40.0 0.0 0.0 event-camera
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.act01 ( -- )   \ 00401C30
    self-wait-done
    6.0 4.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    6 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act02 ( -- )   \ 00401C50
    self-wait-done
    56.58 -1.5 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    7 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act03 ( -- )   \ 00401C68
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act04 ( -- )   \ 00401C80
    0 castle-2f-8.cmd00
    begin
        1 castle-2f-8.cmd00
        1 2 1.1 light
        self-frames-reset
        2 self-wait-frames
        1 castle-2f-8.cmd00
        0 2 0.0 light
        2 2 1.5 light
        yield
    again
;

: castle-2f-8.act05 ( -- )   \ 00401CB0
    0 0 6 char-sound
    self-wait-done
    self-frames-reset
    $1E self-wait-frames
    9 message
    wait-message
    self-idle-or-end
;

: castle-2f-8.act06 ( -- )   \ 00401CD0
    self-wait-done
    $D6 7.826 -7.168 0 $FFFF 5 self-move-to
    self-wait-done
    $A message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    5 0 $14 door-bits
    $1E story-flag-set
    $A0 message-param-room
    $A0 1 item-give-count
    0 $A0 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act07 ( -- )   \ 00401D30
    self-wait-done
    10.0 -25.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $8D 1 item-count? not if
        0 effect-remove
        $315 story-flag-set
        $8D message-param-room
        $8D 1 item-give-count
        0 $8D item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else
        $75 message-param-room
        $75 $63 item-count? if
            $8010 message
            wait-message
        else
            $315 story-flag-set
            0 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    then
    $901 self-anim
    self-wait-anim
    wait-message
    self-idle-or-end
;

: castle-2f-8.act08 ( -- )   \ 00401DC0
    begin
        0 char-busy? not while
        yield
    repeat
    $1203 0 self-anim-blend
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    1 message
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: castle-2f-8.act09 ( -- )   \ 00401DE0
    self-wait-done
    45.0 -30.0 self-turn-to-xz
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: castle-2f-8.act0A ( -- )   \ 00401DF0
    $18 state-flag-set
    1 self-scripted
    8 3 $FF char-load
    self-wait-done
    $F $54 fade
    3 char-unload
    8 1 char-no-shadow
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
    wait-fade
    8 char-activate
    0 0 $14 door-bits
    $FF 1 char-visible
    $F $3C movie-param
    2 0 char-remove
    6 partner-load
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
    2 char-unload
    $A 1 object-show
    3 0 char-remove
    0 $A8 -160 char-to-tri-facing
    camera-restart
    $13 door-unlock
    $1B story-flag-set
    1 0 0 music
    0 0 $20000 nav-group
    0 5 $800000 nav-group
    0 1 $4000000 nav-group
    1 2 $20000 nav-group
    1 6 $800000 nav-group
    1 3 $4000000 nav-group
    1 1 $14 door-bits
    $FF 0 char-visible
    $F $51 fade
    $16 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.act0B ( -- )   \ 00401EF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    0 0 $14 door-bits
    $FF 1 char-visible
    2 1 $14 door-bits
    0 10.0 1.0 -25.0 flicker-sprite
    $F $96 movie-param
    2 0 char-remove
    6 partner-load
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
    2 char-unload
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    0 $A8 -160 char-to-tri-facing
    camera-restart
    $13 door-unlock
    $1B story-flag-set
    1 0 0 music
    0 0 $20000 nav-group
    0 5 $800000 nav-group
    0 1 $4000000 nav-group
    $FF 0 char-visible
    $F $51 fade
    $17 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.act0C ( -- )   \ 00401FF0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    8 3 $FF char-load
    3 char-unload
    8 1 char-no-shadow
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
    0 0 $14 door-bits
    $FF 1 char-visible
    $F $3C movie-param
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
    3 0 char-remove
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-2f-8.act0D ( -- )   \ 004020A0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 0 $14 door-bits
    $FF 1 char-visible
    2 1 $14 door-bits
    $F $96 movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-2f-8.act0E ( -- )   \ 00402150
    $18 state-flag-set
    1 self-scripted
    8 3 $FF char-load
    self-wait-done
    $F $44 fade
    3 char-unload
    $37 1.0 1 bgm
    wait-fade
    $19 state-flag-set
    $FF panic-stage? if
        3 panic-stage
    then
    $F state-flag-set
    0 $EE 10.0 -20.0 180 char-to-xz
    1 23.0 -12.0 0.0 5.0 event-camera
    $FF 1 char-visible
    0 0 $14 door-bits
    8 char-activate
    0 0 $20000 nav-group
    0 5 $800000 nav-group
    0 1 $4000000 nav-group
    8 $EF 10.0 -30.0 0 char-to-xz
    1 0 $20000 nav-group
    1 5 $800000 nav-group
    1 1 $4000000 nav-group
    begin
        0 adx? not while
        yield
    repeat
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    0 0 var-set
    castle-2f-8.cmd02
    self-frames-reset
    $A self-wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        castle-2f-8.cmd02
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 1 $14 door-bits
    8 action-end
    8 char-done
    3 0 char-remove
    $FF 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    $F state-flag-clear
    $19 state-flag-clear
    $F $41 fade
    wait-fade
    3 ebit? not if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 $84 5 char-sound
        3 ebit-set
        self-frames-reset
        $1E self-wait-frames
    else
        $C message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.phase5 ( -- )   \ 0047AB10
;

\ ---- registered ----
' castle-2f-8.enter castle-2f-8 0 room-script!
' castle-2f-8.char-enter castle-2f-8 6 room-script!
' castle-2f-8.phase1 castle-2f-8 1 room-script!
' castle-2f-8.phase2 castle-2f-8 2 room-script!
' castle-2f-8.act00 castle-2f-8 $00 action-script!
' castle-2f-8.act01 castle-2f-8 $01 action-script!
' castle-2f-8.act02 castle-2f-8 $02 action-script!
' castle-2f-8.act03 castle-2f-8 $03 action-script!
' castle-2f-8.act04 castle-2f-8 $04 action-script!
' castle-2f-8.act05 castle-2f-8 $05 action-script!
' castle-2f-8.act06 castle-2f-8 $06 action-script!
' castle-2f-8.act07 castle-2f-8 $07 action-script!
' castle-2f-8.act08 castle-2f-8 $08 action-script!
' castle-2f-8.act09 castle-2f-8 $09 action-script!
' castle-2f-8.act0A castle-2f-8 $0A action-script!
' castle-2f-8.act0B castle-2f-8 $0B action-script!
' castle-2f-8.act0C castle-2f-8 $0C action-script!
' castle-2f-8.act0D castle-2f-8 $0D action-script!
' castle-2f-8.act0E castle-2f-8 $0E action-script!
' castle-2f-8.phase5 castle-2f-8 5 room-script!
