\ story/rooms/water-tower-2f-1.fs - the event scripts of room water-tower-2f-1 ($C2; Water Tower: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-2f-1
USING: room-names story.words story.shared ;

\ byte 3 0 / 1: shut / open (pi/2); 2 / 3 opening / shutting by script variable 0 (0..120
\ frames, eased by a sine)
: water-tower-2f-1.cmd00 ( b0 -- )  drop s" water-tower-2f-1.cmd00" stub-step ;
\ byte 3 to the player's Character_ChooseExit while she's active
: water-tower-2f-1.cmd01 ( b0 -- )  drop s" water-tower-2f-1.cmd01" stub-step ;
\ room 0xC2: the summoner's countdown (progress +0x764) has run out.
: water-tower-2f-1.cond00? ( -- flag )  s" water-tower-2f-1.cond00?" stub-flag ;

: water-tower-2f-1.enter ( -- )   \ 0043F0C0
    room-sounds
    0 exit-taken? if
        $341 story-flag-clear
    else 1 exit-taken? if
        $341 story-flag-set
    then then
    2 0 $14 door-bits
    3 0 $14 door-bits
    0 0 object-show
    1 1 object-show
    2 1 object-show
    3 1 object-show
    0 2 $20000 nav-group
    $341 story-flag? not if
        0 water-tower-2f-1.cmd00
        0 1 $14 door-bits
        1 0 $14 door-bits
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
    else
        1 water-tower-2f-1.cmd00
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 1 $10020000 nav-group
        0 0 $10020000 nav-group
    then
    $17 stalker-kind? if
        $23 state-flag-set
    then
    0 state-flag? if
        4 ebit-set
        0 state-flag-clear
    then
    $FE action-end
    1 summon-take
    $2D8 story-flag? $2D9 story-flag? not and if
        0 4.12 105.0 -18.51 flicker-sprite
    then
    $2DE story-flag? not if
        1 -19.9 153.0 74.61 flicker-sprite
    then
    $2F7 story-flag? not if
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    1 $2300 sound-volume
;

: water-tower-2f-1.char-enter ( -- )   \ 0043F1A0
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
    0 self-is? if
        1 exit-taken? if
            4 map-page
        then
    then
;

: water-tower-2f-1.phase1 ( -- )   \ 0043F230
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
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    1 $A char-entered-area? if
        1 hewie-side
    then
    1 $A char-left-area? if
        0 hewie-side
    then
    0 $F char-entered-area? if
        2 map-page
    then
    0 $F char-left-area? 0 $10 char-entered-area? or if
        3 map-page
    then
    0 $10 char-left-area? if
        4 map-page
    then
    $8D story-flag? $8E story-flag? not and if
        4 stalker-kind? $17 stalker-kind? or $FE char-here? not and if
            $8B story-flag? not if
                0 ebit? not if
                    0 $D char-left-area? if
                        0 ebit-set
                        0 $F2 1 action
                    then
                then
                0 5 char-entered-area? if
                    $F2 action-end
                then
                0 $E char-entered-area? if
                    $FE $C2 825 1 stalker-to-room
                    $FE 0 stalker-mode
                    stalker-item-cooldown
                    $8B story-flag-set
                then
                0 7 char-entered-area? if
                    $FE $C2 816 1 stalker-to-room
                    $FE 0 stalker-mode
                    stalker-item-cooldown
                    $8B story-flag-set
                then
            else 5 ebit? not if
                1 1800 var-set
                stalker-active? not if
                    5 ebit-set
                then
            else stalker-active? if
                5 ebit-clear
            else 1 0 var? not if
                1 var-dec
            else 1 ebit? not $18 state-flag? not and if
                $341 story-flag? not if
                    $FE $C2 473 1 stalker-to-room
                    $FE 0 stalker-mode
                    stalker-item-cooldown
                    5 ebit-clear
                else
                    $FE $C2 888 0 stalker-to-room
                    $FE 0 stalker-mode
                    stalker-item-cooldown
                    5 ebit-clear
                then
            then then then then then
        then
    then
    4 stalker-kind? $17 stalker-kind? or if
        $FE char-busy? not stalker-active? and if
            water-tower-2f-1.cond00? if
                $FE char-unseen? if
                    $FE action-end
                    1 summon-take
                then
            then
        then
    then
    $2D8 story-flag? not if
        0 4.12 104.0 -18.51 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 728 var-set
                $1A 729 var-set
                $1B 0 var-set
                $1C 4120 var-set
                $1D 105000 var-set
                $1E -18510 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    $2F7 story-flag? not if
        3 -16.5 152.0 -47.5 5 8 1 zone
        $FF 3 char-in-zone? if
            $2F7 story-flag-set
            4 0 $14 door-bits
            5 1 $14 door-bits
            -16.5 152.0 -47.5 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -16.5 152.0 -47.5 0 0 sound
            $40 $2B9 noise
            $C2 1 291 $24 5 6 0 0.0 creature-place
        then
    then
    $34C story-flag? not if
        2 1.97 152.0 46.65 $26 10 0 zone
        6 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 2 9 char-zone-bits? if
                        6 ebit-set
                        $64 chance? if
                            $1F 2 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: water-tower-2f-1.phase2 ( -- )   \ 0043F480
    0 $C char-in-area? 0 -45 $32 char-heading? and if
        1 ebit? not if
            5 0 0 scene-change
        then
    then
    $2D8 story-flag? $2D9 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $2DE story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: water-tower-2f-1.phase5 ( -- )   \ 0043F4D0
    1 char-here? if
        1 2 char-in-nav-group? if
            $341 story-flag? not if
                $C2 1 874 hewie-to-room
            else
                $C2 0 887 hewie-to-room
            then
        then
    then
    $23 state-flag-clear
    4 ebit? if
        0 state-flag-set
    then
;

: water-tower-2f-1.act00 ( -- )   \ 0043F500
    $18 state-flag-set
    1 self-scripted
    1 ebit-set
    3 ebit-clear
    $341 story-flag? not if
        2 ebit-clear
        1 1 $10000030 nav-group
        1 char-here? 1 1 char-in-nav-group? and if
            0 1 2 action-force
        else
            1 1 $1000000 nav-group
            3 ebit-set
        then
    else
        2 ebit-set
        1 0 $10000030 nav-group
        1 char-here? 1 0 char-in-nav-group? and if
            0 1 2 action-force
        else
            1 0 $1000000 nav-group
            3 ebit-set
        then
    then
    self-wait-done
    things-clear
    0 4 char-file-load
    $13A -0.47 39.8 -90 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $341 story-flag? not if
        1 water-tower-2f-1.cmd01
        $8004 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        1 0 $14 door-bits
        2 1 object-show
        3 0 object-show
        3 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        0 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        0 0 6 char-sound
        self-wait-anim
        0 $F3 3 action
        $341 story-flag-set
        2 1 object-show
        3 1 object-show
        0 0 $14 door-bits
        1 1 $14 door-bits
        $34C story-flag-set
    else
        0 water-tower-2f-1.cmd01
        $8003 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        1 0 $14 door-bits
        2 0 object-show
        3 1 object-show
        2 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        0 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        0 0 6 char-sound
        self-wait-anim
        0 $F3 3 action
        $341 story-flag-clear
        2 1 object-show
        3 1 object-show
        0 1 $14 door-bits
        1 0 $14 door-bits
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: water-tower-2f-1.act01 ( -- )   \ 0043F620
    $27 7 54.0 112.0 -56.0 0 0 sound
    self-frames-reset
    $3C self-wait-frames
    begin
        4 stalker-kind-here? not $17 stalker-kind-here? not and if
            0 7 54.0 112.0 -56.0 0 0 sound
        then
        self-frames-reset
        $1E self-wait-frames
        4 stalker-kind-here? not $17 stalker-kind-here? not and if
            1 7 54.0 112.0 -56.0 0 0 sound
        then
        self-frames-reset
        $1E self-wait-frames
    again
;

: water-tower-2f-1.act02 ( -- )   \ 0043F680
    1 self-scripted
    self-wait-done
    2 ebit? not if
        $3B6 -22.18 0.43 120 $FFFF $A self-move-to
        self-wait-done
    else
        $3C3 2.56 19.55 -160 $FFFF $A self-move-to
        self-wait-done
    then
    0 self-scripted
    begin
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: water-tower-2f-1.act03 ( -- )   \ 0043F6C0
    0 0 var-set
    2 ebit? not if
        2 6 -45.0 152.0 0.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            2 water-tower-2f-1.cmd00
            3 ebit? not if
                1 1 char-in-nav-group? not if
                    1 1 $1000000 nav-group
                    3 ebit-set
                then
            then
            yield
        repeat
        1 char-here? 1 1 char-in-nav-group? and if
            1 action-end
            1 $3B6 char-to-tri
            1 hewie-side
        then
        1 1 $10020000 nav-group
        0 0 $10020000 nav-group
        0 0 $1000000 nav-group
        0 0 $30 nav-group
        0 1 $30 nav-group
        1 water-tower-2f-1.cmd00
        1 water-tower-2f-1.cmd01
    else
        2 6 0.0 152.0 45.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            3 water-tower-2f-1.cmd00
            3 ebit? not if
                1 1 char-in-nav-group? not if
                    1 0 $1000000 nav-group
                    3 ebit-set
                then
            then
            yield
        repeat
        1 char-here? 1 0 char-in-nav-group? and if
            1 action-end
            1 $3BF char-to-tri
            1 hewie-side
        then
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
        0 1 $1000000 nav-group
        0 0 $30 nav-group
        0 1 $30 nav-group
        0 water-tower-2f-1.cmd00
        0 water-tower-2f-1.cmd01
    then
    1 ebit-clear
    self-idle-or-end
;

: water-tower-2f-1.act04 ( -- )   \ 0043F7C0
    self-wait-done
    4.12 -18.51 self-turn-to-xz
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
            $2D9 story-flag-set
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

: water-tower-2f-1.act05 ( -- )   \ 0043F820
    self-wait-done
    -19.9 74.61 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $2DE story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

\ ---- registered ----
' water-tower-2f-1.enter water-tower-2f-1 0 room-script!
' water-tower-2f-1.char-enter water-tower-2f-1 6 room-script!
' water-tower-2f-1.phase1 water-tower-2f-1 1 room-script!
' water-tower-2f-1.phase2 water-tower-2f-1 2 room-script!
' water-tower-2f-1.phase5 water-tower-2f-1 5 room-script!
' water-tower-2f-1.act00 water-tower-2f-1 $00 action-script!
' water-tower-2f-1.act01 water-tower-2f-1 $01 action-script!
' water-tower-2f-1.act02 water-tower-2f-1 $02 action-script!
' water-tower-2f-1.act03 water-tower-2f-1 $03 action-script!
' water-tower-2f-1.act04 water-tower-2f-1 $04 action-script!
' water-tower-2f-1.act05 water-tower-2f-1 $05 action-script!
