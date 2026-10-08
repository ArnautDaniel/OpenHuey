\ story/rooms/chaos-forest-10.fs - the event scripts of room chaos-forest-10 ($108; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-10
USING: room-names story.words story.shared ;

\ byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(1, 0), then the partner's
\ message slot shows progress +0x73EDC0; else Progress_SpeechCall and the slot is closed
: chaos-forest-10.cmd00 ( b0 -- )  drop s" chaos-forest-10.cmd00" stub-step ;

: chaos-forest-10.enter ( -- )   \ 00418DC0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $21D story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2CA story-flag? not if
            0 76.5 -25.5 134.5 flicker-sprite
        then
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    $86 story-flag? $88 story-flag? not and if
        1 char-activate
        1 $118 4.7 54.573 180 char-to-xz
        0 1 1 action
    then
;

: chaos-forest-10.char-enter ( -- )   \ 00418E40
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    3 3 3 area-camera
;

: chaos-forest-10.phase1 ( -- )   \ 00418F40
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    8 2 2 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    0 $D char-entered-area? if
        $86 story-flag? $87 story-flag? not and if
            $87 story-flag-set
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
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
    $86 story-flag? $88 story-flag? not and if
        0 0 var? if
            $1E chance? if
                1 $40000001 6 char-sound
                0 180 var-set
            else
                0 60 var-set
            then
        else
            0 var-dec
            1 $C0000001 6 char-sound
        then
    then
;

: chaos-forest-10.phase2 ( -- )   \ 00419050
    $86 story-flag? $88 story-flag? not and if
        $87 story-flag? 0 1 $A $A $2D char-touching-facing? and if
            5 2 0 scene-change
        then
    then
    $21D story-flag? not if
        0 76.5 -26.5 134.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $21D story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            76.5 -26.5 134.5 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 76.5 -26.5 134.5 0 0 sound
            $40 $262 noise
            0 76.5 -25.5 134.5 flicker-sprite
        then
    else $2CA story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: chaos-forest-10.phase5 ( -- )   \ 004190F0
    $86 story-flag? $88 story-flag? not and if
        1 action-end
        1 char-done
        $27 0 pvar? if
            3 chaos-forest-10.cmd00
        then
    then
;

: chaos-forest-10.act00 ( -- )   \ 00419110
    1 self-scripted
    $18 state-flag-set
    $FE action-end
    $FE char-done
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
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    1 action-end
    self-frames-reset
    1 self-wait-frames
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
    30 hewie-trust
    8 state-flag-set
    $87 story-flag-set
    0 $9E 12.16 49.91 -7 char-to-xz
    1 $118 4.7 54.573 180 char-to-xz
    $27 0 pvar? if
        3 chaos-forest-10.cmd00
    then
    0 counter-set
    0 1 1 action
    1 wait-counter
    $FE char-activate
    $FE $108 305 2 stalker-to-room
    $FE 3 3 char-camera
    $FE 2 stalker-mode
    0 state-flag-set
    stalker-item-cooldown
    camera-restart
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    $39 resident-flag-set
    $259 item-give
    $8285 item-give
    0 self-scripted
    $18 state-flag-clear
    $25 state-flag-clear
    $D state-flag-clear
    self-idle-or-end
;

: chaos-forest-10.act01 ( -- )   \ 00419220
    1 self-noclip
    1 self-scripted
    self-wait-done
    $1004 0 self-anim-blend
    $27 0 pvar? if
        $87 story-flag? not if
            0 chaos-forest-10.cmd00
        else
            1 chaos-forest-10.cmd00
        then
        2 chaos-forest-10.cmd00
    then
    1 counter-set
    begin
        yield
    again
;

: chaos-forest-10.act02 ( -- )   \ 00419250
    self-wait-done
    $800 self-anim
    self-wait-anim
    1 message
    wait-message
    1 $5C 5 char-sound
    $802 self-anim
    self-wait-anim
    self-idle-or-end
;

: chaos-forest-10.act03 ( -- )   \ 00419270
    self-wait-done
    76.5 134.5 self-turn-to-xz
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
            $2CA story-flag-set
            0 effect-remove
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

: chaos-forest-10.act04 ( -- )   \ 004192D0
    self-wait-done
    $27 0 pvar? if
        0 chaos-forest-10.cmd00
        2 chaos-forest-10.cmd00
    then
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
    $27 0 pvar? if
        3 chaos-forest-10.cmd00
    then
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' chaos-forest-10.enter chaos-forest-10 0 room-script!
' chaos-forest-10.char-enter chaos-forest-10 6 room-script!
' chaos-forest-10.phase1 chaos-forest-10 1 room-script!
' chaos-forest-10.phase2 chaos-forest-10 2 room-script!
' chaos-forest-10.phase5 chaos-forest-10 5 room-script!
' chaos-forest-10.act00 chaos-forest-10 $00 action-script!
' chaos-forest-10.act01 chaos-forest-10 $01 action-script!
' chaos-forest-10.act02 chaos-forest-10 $02 action-script!
' chaos-forest-10.act03 chaos-forest-10 $03 action-script!
' chaos-forest-10.act04 chaos-forest-10 $04 action-script!
