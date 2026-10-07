\ events/map3.fs - the event scripts of the rooms on the game's map 3 (kMapRooms).
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.map3
USING: events.core events.words events.builtin ;

\ ---- room $80 ----------------------------------------------------------------------------------

\ (as Room2A_Cmd03) the 0x14-byte effect BackdropModel2_vtable started with byte 3 as a word
: room80.cmd00 ( b0 -- )  drop s" room80.cmd00" stub-step ;

: room80.enter ( -- )   \ 0043EA90
    $18 1.0 0 bgm
    $10 22.8 95.4 53.7 $E $80 $80 $80 $40 specks
    $10 -23.1 95.4 53.7 $E $80 $80 $80 $40 specks
    $94 story-flag? not if
        0 1 $14 door-bits
        0 room80.cmd00
    else
        1 0 $20000 nav-group
        1 room80.cmd00
    then
;

: room80.char-enter ( -- )   \ 0043EAE0
    $94 story-flag? not if
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
    else
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
    then
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
;

: room80.phase1 ( -- )   \ 0043EBA0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 1 1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $94 story-flag? if
        0 -2.26 38.0 -25.57 $1E 15 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        1 ebit-set
                        $1E chance? if
                            $1F 0 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: room80.phase2 ( -- )   \ 0043EC20
    $94 story-flag? if
        0 6 char-in-area? 0 90 $32 char-heading? and if
            5 0 0 scene-change
        then
    then
;

: room80.act00 ( -- )   \ 0043EC40
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 ebit? not if
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        0 message
        wait-message
        self-wait-anim
        0 ebit-set
    else
        $1D02 self-anim
        self-frames-reset
        self-wait-16
        1 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

' room80.enter $80 0 room-script!
' room80.char-enter $80 6 room-script!
' room80.phase1 $80 1 room-script!
' room80.phase2 $80 2 room-script!
' room80.act00 $80 $00 action-script!

\ ---- room $C1 ----------------------------------------------------------------------------------

\ room 0xC1: the summoner's countdown (progress +0x764) has run out.
: roomC1.cond00? ( -- flag )  s" roomC1.cond00?" stub-flag ;

: roomC1.enter ( -- )   \ 0043EC80
    room-sounds
    $17 stalker-kind? if
        $23 state-flag-set
    then
    $2D6 story-flag? $2D7 story-flag? not and if
        0 -39.66 1.0 0.99 flicker-sprite
    then
    1 $2300 sound-volume
    1 $1F $10000000 nav-tri-flags
    1 $2F $10000000 nav-tri-flags
    1 $89 $10000000 nav-tri-flags
;

: roomC1.char-enter ( -- )   \ 0043ECC0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
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
;

: roomC1.phase1 ( -- )   \ 0043ED80
    0 exit-usable? if
        0 exit-check
    then
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
    7 0 -1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    4 stalker-kind? $17 stalker-kind? or if
        $FE char-here? $FE char-busy? not and stalker-active? and if
            roomC1.cond00? $FE char-unseen? and if
                $FE action-end
                1 summon-take
            then
        then
    then
    $2D6 story-flag? not if
        0 -39.66 0.0 0.99 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 726 var-set
                $1A 727 var-set
                $1B 0 var-set
                $1C -39660 var-set
                $1D 1000 var-set
                $1E 990 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 25.0 10.0 136.5 0 0 sound
            else 1 1 var? if
                $40000006 6 25.0 10.0 136.5 0 0 sound
            else 1 2 var? if
                $40000007 6 25.0 10.0 136.5 0 0 sound
            else 1 3 var? if
                $40000008 6 25.0 10.0 136.5 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: roomC1.phase2 ( -- )   \ 0043EF00
    0 $B $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 $C char-in-area? 0 -22 $32 char-heading? and if
        5 0 0 scene-change
    then
    $2D6 story-flag? $2D7 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: roomC1.phase3 ( -- )   \ 0043EF40
    -83.5 3.0 -34.7 -18.0 3.0 -20.4 -83.5 -12.0 -34.7 -18.0 -12.0 -20.4 lights-doorway
;

: roomC1.act00 ( -- )   \ 0043EF80
    0 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $32 -64.094 52.322 -45 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        0 message
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
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        1 message
        wait-message
    then
    self-idle-or-end
;

: roomC1.act01 ( -- )   \ 0043F040
    self-wait-done
    -39.66 0.99 self-turn-to-xz
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
            $2D7 story-flag-set
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

' roomC1.enter $C1 0 room-script!
' roomC1.char-enter $C1 6 room-script!
' roomC1.phase1 $C1 1 room-script!
' roomC1.phase2 $C1 2 room-script!
' roomC1.phase3 $C1 3 room-script!
' roomC1.act00 $C1 $00 action-script!
' roomC1.act01 $C1 $01 action-script!

\ ---- room $C2 ----------------------------------------------------------------------------------

\ byte 3 0 / 1: shut / open (pi/2); 2 / 3 opening / shutting by script variable 0 (0..120
\ frames, eased by a sine)
: roomC2.cmd00 ( b0 -- )  drop s" roomC2.cmd00" stub-step ;
\ byte 3 to the player's Character_ChooseExit while she's active
: roomC2.cmd01 ( b0 -- )  drop s" roomC2.cmd01" stub-step ;
\ room 0xC2: the summoner's countdown (progress +0x764) has run out.
: roomC2.cond00? ( -- flag )  s" roomC2.cond00?" stub-flag ;

: roomC2.enter ( -- )   \ 0043F0C0
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
        0 roomC2.cmd00
        0 1 $14 door-bits
        1 0 $14 door-bits
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
    else
        1 roomC2.cmd00
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

: roomC2.char-enter ( -- )   \ 0043F1A0
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

: roomC2.phase1 ( -- )   \ 0043F230
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
            roomC2.cond00? if
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

: roomC2.phase2 ( -- )   \ 0043F480
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

: roomC2.phase5 ( -- )   \ 0043F4D0
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

: roomC2.act00 ( -- )   \ 0043F500
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
        1 roomC2.cmd01
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
        0 roomC2.cmd01
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

: roomC2.act01 ( -- )   \ 0043F620
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

: roomC2.act02 ( -- )   \ 0043F680
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

: roomC2.act03 ( -- )   \ 0043F6C0
    0 0 var-set
    2 ebit? not if
        2 6 -45.0 152.0 0.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            2 roomC2.cmd00
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
        1 roomC2.cmd00
        1 roomC2.cmd01
    else
        2 6 0.0 152.0 45.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            3 roomC2.cmd00
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
        0 roomC2.cmd00
        0 roomC2.cmd01
    then
    1 ebit-clear
    self-idle-or-end
;

: roomC2.act04 ( -- )   \ 0043F7C0
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

: roomC2.act05 ( -- )   \ 0043F820
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

' roomC2.enter $C2 0 room-script!
' roomC2.char-enter $C2 6 room-script!
' roomC2.phase1 $C2 1 room-script!
' roomC2.phase2 $C2 2 room-script!
' roomC2.phase5 $C2 5 room-script!
' roomC2.act00 $C2 $00 action-script!
' roomC2.act01 $C2 $01 action-script!
' roomC2.act02 $C2 $02 action-script!
' roomC2.act03 $C2 $03 action-script!
' roomC2.act04 $C2 $04 action-script!
' roomC2.act05 $C2 $05 action-script!

\ ---- room $C3 ----------------------------------------------------------------------------------

\ (as RoomC2_Cmd00, the room object named RoomC3_ObjectNames, opening to -pi/2)
: roomC3.cmd00 ( b0 -- )  drop s" roomC3.cmd00" stub-step ;
\ the player (active, +0xE0 clear) put in action 0xB / 0x21 / 0xFF unless held (7); then
\ progress +0x7B8 gets 50
: roomC3.cmd01 ( -- )  s" roomC3.cmd01" stub-step ;
\ sound 3 (bank 6) at the room object named pstr_doramukan_2[0]
: roomC3.cmd02 ( -- )  s" roomC3.cmd02" stub-step ;
\ room 0xC3: the summoner's countdown (progress +0x764) has run out.
: roomC3.cond00? ( -- flag )  s" roomC3.cond00?" stub-flag ;

: roomC3.enter ( -- )   \ 0043F8F0
    room-sounds
    0 exit-taken? if
        $342 story-flag? not if
            $FF door-lock
            $101 door-lock
        else
            $FF door-unlock
            $101 door-unlock
        then
    else 1 exit-taken? if
        $342 story-flag-set
        $FF door-unlock
        $101 door-unlock
    then then
    2 0 $14 door-bits
    3 0 $14 door-bits
    0 0 object-show
    1 1 object-show
    2 1 object-show
    3 1 object-show
    $342 story-flag? not if
        0 roomC3.cmd00
        0 1 $14 door-bits
        1 0 $14 door-bits
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
    else
        1 roomC3.cmd00
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 1 $10020000 nav-group
        0 0 $10020000 nav-group
    then
    $33A story-flag? if
        4 1 object-show
    else
        4 0 object-show
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
    $2DF story-flag? not if
        0 -71.41 143.0 -7.67 flicker-sprite
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    $2F8 story-flag? not if
        5 1 $14 door-bits
        6 0 $14 door-bits
    else
        5 0 $14 door-bits
        6 1 $14 door-bits
        $2F9 story-flag? not if
            1 -86.45 47.0 -8.0 flicker-sprite
        then
    then
    1 $2300 sound-volume
;

: roomC3.char-enter ( -- )   \ 0043F9F0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 3 char-camera
                0 camera-follow
            else
                1 0 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 3 char-camera
            0 camera-follow
        else
            1 0 3 char-camera
            1 camera-follow
        then
    then then
    0 0 3 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    1 3 2 area-camera
    0 self-is? if
        1 exit-taken? if
            7 map-page
        then
    then
;

: roomC3.phase1 ( -- )   \ 0043FA80
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
    4 0 3 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 0 3 1 chars-area-camera
    7 2 1 1 chars-area-camera
    8 0 3 1 chars-area-camera
    9 2 1 1 chars-area-camera
    $A 2 1 1 chars-area-camera
    $B 3 2 1 chars-area-camera
    $14 1 0 1 chars-area-camera
    $13 6 5 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $15 char-entered-area? if
        4 map-page
    then
    0 $15 char-left-area? 0 $16 char-entered-area? or if
        5 map-page
    then
    0 $16 char-left-area? 0 $17 char-entered-area? or if
        6 map-page
    then
    0 $17 char-left-area? if
        7 map-page
    then
    0 $10 char-entered-area? 0 $11 char-entered-area? or 0 $12 char-entered-area? or if
        0 0 3 char-camera
    then
    $8D story-flag? $8E story-flag? not and if
        4 stalker-kind? $17 stalker-kind? or $FE char-here? not and if
            $33A story-flag? not if
                0 $F char-entered-area? if
                    0 5 4 char-camera
                then
                0 $D char-entered-area? 0 $E char-entered-area? or if
                    $33A story-flag-set
                    0 $F1 1 action
                then
            else $8C story-flag? if
                5 ebit? not if
                    1 1800 var-set
                    stalker-active? not if
                        5 ebit-set
                    then
                else stalker-active? if
                    5 ebit-clear
                else 1 0 var? not if
                    1 var-dec
                else 1 ebit? not $18 state-flag? not and if
                    $342 story-flag? not if
                        $FE $C3 249 2 stalker-to-room
                        $FE 0 stalker-mode
                        stalker-item-cooldown
                        5 ebit-clear
                    else
                        $FE $C3 944 2 stalker-to-room
                        $FE 0 stalker-mode
                        stalker-item-cooldown
                        5 ebit-clear
                    then
                then then then then
            then then
        then
    then
    4 stalker-kind? $17 stalker-kind? or if
        $FE char-busy? not stalker-active? and if
            roomC3.cond00? if
                $FE char-unseen? if
                    $FE action-end
                    1 summon-take
                then
            then
        then
    then
    $34D story-flag? not if
        1 -47.23 142.0 1.87 $26 10 0 zone
        6 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        6 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: roomC3.phase2 ( -- )   \ 0043FC10
    0 $C char-in-area? 0 90 $32 char-heading? and if
        1 ebit? not if
            5 0 0 scene-change
        then
    then
    $2DF story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $2F8 story-flag? not if
        2 -86.45 46.0 -8.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $2F8 story-flag-set
            5 0 $14 door-bits
            6 1 $14 door-bits
            -86.45 46.0 -8.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -86.45 46.0 -8.0 0 0 sound
            $40 $131 noise
            1 -86.45 47.0 -8.0 flicker-sprite
        then
    else $2F9 story-flag? not if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
;

: roomC3.phase5 ( -- )   \ 0043FCC0
    $23 state-flag-clear
    4 ebit? if
        0 state-flag-set
    then
;

: roomC3.act00 ( -- )   \ 0043FCD0
    $18 state-flag-set
    1 self-scripted
    1 ebit-set
    3 ebit-clear
    $342 story-flag? not if
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
    0 5 char-file-load
    $166 73.23 -0.48 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $342 story-flag? not if
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
        $342 story-flag-set
        2 1 object-show
        3 1 object-show
        0 0 $14 door-bits
        1 1 $14 door-bits
        $C3 0 1055 $A 4 -1 0 0.0 creature-place
        $C3 0 1053 $A 6 -1 0 0.0 creature-place
        $C3 0 618 $A 7 -1 0 0.0 creature-place
        $C3 0 1051 $A 7 -1 0 0.0 creature-place
        $C3 0 607 $A 4 -1 0 0.0 creature-place
        $C3 0 615 $A 6 -1 0 0.0 creature-place
        $34D story-flag-set
    else
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
        $342 story-flag-clear
        2 1 object-show
        3 1 object-show
        0 1 $14 door-bits
        1 0 $14 door-bits
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: roomC3.act01 ( -- )   \ 0043FE40
    4 0 object-anim
    self-frames-reset
    $11 self-wait-frames
    1 $FF 8 rumble
    roomC3.cmd01
    roomC3.cmd02
    self-frames-reset
    $3C self-wait-frames
    $8C story-flag? not if
        $13 7 54.0 112.0 -56.0 0 0 sound
        $FE $C3 830 2 stalker-to-room
        $FE 0 stalker-mode
        stalker-item-cooldown
        $8C story-flag-set
    then
    self-idle-or-end
;

: roomC3.act02 ( -- )   \ 0043FE80
    1 self-scripted
    self-wait-done
    2 ebit? not if
        $260 9.39 -18.29 180 $FFFF $A self-move-to
        self-wait-done
    else
        $260 9.39 -18.29 180 $FFFF $A self-move-to
        self-wait-done
    then
    0 self-scripted
    begin
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: roomC3.act03 ( -- )   \ 0043FEC0
    0 0 var-set
    2 ebit? not if
        2 6 0.0 142.0 45.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            2 roomC3.cmd00
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
            1 $260 char-to-tri
        then
        1 1 $10020000 nav-group
        0 0 $10020000 nav-group
        0 0 $1000000 nav-group
        0 0 $30 nav-group
        0 1 $30 nav-group
        1 roomC3.cmd00
    else
        2 6 -45.0 142.0 0.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            3 roomC3.cmd00
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
            1 $260 char-to-tri
        then
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
        0 1 $1000000 nav-group
        0 0 $30 nav-group
        0 1 $30 nav-group
        0 roomC3.cmd00
    then
    $342 story-flag? not if
        $FF door-lock
        $101 door-lock
    else
        $FF door-unlock
        $101 door-unlock
    then
    1 ebit-clear
    self-idle-or-end
;

: roomC3.act04 ( -- )   \ 0043FFC0
    self-wait-done
    -71.41 -7.67 self-turn-to-xz
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
            $2DF story-flag-set
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

: roomC3.act05 ( -- )   \ 00440020
    self-wait-done
    -86.45 -8.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F9 story-flag-set
            1 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

' roomC3.enter $C3 0 room-script!
' roomC3.char-enter $C3 6 room-script!
' roomC3.phase1 $C3 1 room-script!
' roomC3.phase2 $C3 2 room-script!
' roomC3.phase5 $C3 5 room-script!
' roomC3.act00 $C3 $00 action-script!
' roomC3.act01 $C3 $01 action-script!
' roomC3.act02 $C3 $02 action-script!
' roomC3.act03 $C3 $03 action-script!
' roomC3.act04 $C3 $04 action-script!
' roomC3.act05 $C3 $05 action-script!

\ ---- room $C5 ----------------------------------------------------------------------------------

\ the 0x10-byte effect ObjectGlow_vtable on room object k + 1 (byte 4 = k, 1..8; script variable
\ 11 - k keeps its slot): made on first use when byte 3 is set, then sent (on byte 3, index 8 -
\ k, the variable, the object), with sound 1 at the object when on and the camera director's
\ +0x38 is clear
: roomC5.cmd00 ( b0 b1 -- )  drop drop s" roomC5.cmd00" stub-step ;

: roomC5.enter ( -- )   \ 004401B0
    room-sounds
    $22 state-flag-set
    $30 1.0 0 bgm
    2 8 var-set
    3 -1 var-set
    4 -1 var-set
    5 -1 var-set
    6 -1 var-set
    7 -1 var-set
    8 -1 var-set
    9 -1 var-set
    $A -1 var-set
    1 8 roomC5.cmd00
    $FE action-end
    1 summon-take
    $34A story-flag? not if
        $34A story-flag-set
        1 creatures-clear
    then
    1 $2300 sound-volume
;

: roomC5.char-enter ( -- )   \ 00440210
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
;

: roomC5.phase1 ( -- )   \ 00440250
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    6 sound-bank-loaded? if
        $C var-inc
        $C 30 var? if
            $C 0 var-set
            $B 0 var? if
                $40000005 6 -127.0 1.7 10.0 0 0 sound
            else $B 1 var? if
                $40000006 6 -127.0 1.7 10.0 0 0 sound
            else $B 2 var? if
                $40000007 6 -127.0 1.7 10.0 0 0 sound
            else $B 3 var? if
                $40000008 6 -127.0 1.7 10.0 0 0 sound
            then then then then
            $B var-inc
            $B 4 var? if
                $B 0 var-set
            then
        then
    then
    8 ebit? not if
        0 0 var? if
            9 ebit-set
        else
            9 ebit-clear
        then
        1 0 var? if
            $A ebit-set
        else
            $A ebit-clear
        then
        0 0 var-set
        1 0 var-set
        0 0.0 0.0 0.0 $A 5 0 zone
        0 0 8 char-zone-bits? if
            0 1 var-set
        then
        1 0 8 char-zone-bits? if
            1 1 var-set
        then
        1 -22.0 0.0 0.0 5 5 0 zone
        0 1 8 char-zone-bits? if
            0 2 var-set
        then
        1 1 8 char-zone-bits? if
            1 2 var-set
        then
        2 8.0 0.0 -30.0 5 5 0 zone
        0 0 var? if
            0 2 8 char-zone-bits? if
                0 3 var-set
            then
        then
        1 0 var? if
            1 2 8 char-zone-bits? if
                1 3 var-set
            then
        then
        3 34.65 0.0 20.0 5 5 0 zone
        0 0 var? if
            0 3 8 char-zone-bits? if
                0 4 var-set
            then
        then
        1 0 var? if
            1 3 8 char-zone-bits? if
                1 4 var-set
            then
        then
        4 -25.0 0.0 43.31 5 5 0 zone
        0 0 var? if
            0 4 8 char-zone-bits? if
                0 5 var-set
            then
        then
        1 0 var? if
            1 4 8 char-zone-bits? if
                1 5 var-set
            then
        then
        5 -40.37 0.0 -40.38 5 5 0 zone
        0 0 var? if
            0 5 8 char-zone-bits? if
                0 6 var-set
            then
        then
        1 0 var? if
            1 5 8 char-zone-bits? if
                1 6 var-set
            then
        then
        6 48.58 0.0 -48.58 5 5 0 zone
        0 0 var? if
            0 6 8 char-zone-bits? if
                0 7 var-set
            then
        then
        1 0 var? if
            1 6 8 char-zone-bits? if
                1 7 var-set
            then
        then
        7 57.25 0.0 57.25 5 5 0 zone
        0 0 var? if
            0 7 8 char-zone-bits? if
                0 8 var-set
            then
        then
        1 0 var? if
            1 7 8 char-zone-bits? if
                1 8 var-set
            then
        then
        9 ebit? 0 0 var? not and if
            0 1 var? if
                0 $11 6 char-sound
            else 0 2 var? if
                0 $10 6 char-sound
            else 0 3 var? if
                0 $F 6 char-sound
            else 0 4 var? if
                0 $E 6 char-sound
            else 0 5 var? if
                0 $D 6 char-sound
            else 0 6 var? if
                0 $C 6 char-sound
            else 0 7 var? if
                0 $B 6 char-sound
            else 0 8 var? if
                0 $A 6 char-sound
            then then then then then then then then
        else $A ebit? 1 0 var? not and if
            1 1 var? if
                1 $11 6 char-sound
            else 1 2 var? if
                1 $10 6 char-sound
            else 1 3 var? if
                1 $F 6 char-sound
            else 1 4 var? if
                1 $E 6 char-sound
            else 1 5 var? if
                1 $D 6 char-sound
            else 1 6 var? if
                1 $C 6 char-sound
            else 1 7 var? if
                1 $B 6 char-sound
            else 1 8 var? if
                1 $A 6 char-sound
            then then then then then then then then
        then then
        9 ebit? not 0 0 var? and if
        else $A ebit? not 1 0 var? and if
        then then
        2 8 var? if
            0 8 var? 1 8 var? or if
                2 7 var-set
                1 7 roomC5.cmd00
            then
        else 2 7 var? if
            0 8 var? 1 8 var? or if
                0 7 var? 1 7 var? or if
                    2 6 var-set
                    1 6 roomC5.cmd00
                then
            else
                2 8 var-set
                0 7 roomC5.cmd00
                0 ebit-clear
            then
        else 2 6 var? if
            0 7 var? 1 7 var? or if
                0 6 var? 1 6 var? or if
                    2 5 var-set
                    1 5 roomC5.cmd00
                then
            else
                0 6 roomC5.cmd00
                0 8 var? 1 8 var? or if
                    2 7 var-set
                    1 ebit-clear
                else
                    2 8 var-set
                    0 7 roomC5.cmd00
                    1 ebit-clear
                    0 ebit-clear
                then
            then
        else 2 5 var? if
            0 6 var? 1 6 var? or if
                0 5 var? 1 5 var? or if
                    2 4 var-set
                    1 4 roomC5.cmd00
                then
            else
                0 5 roomC5.cmd00
                0 7 var? 1 7 var? or if
                    2 6 var-set
                    2 ebit-clear
                else
                    0 6 roomC5.cmd00
                    0 8 var? 1 8 var? or if
                        2 7 var-set
                        1 ebit-clear
                    else
                        2 8 var-set
                        0 7 roomC5.cmd00
                        1 ebit-clear
                        0 ebit-clear
                    then
                then
            then
        else 2 4 var? if
            0 5 var? 1 5 var? or if
                0 4 var? 1 4 var? or if
                    2 3 var-set
                    1 3 roomC5.cmd00
                then
            else
                0 4 roomC5.cmd00
                0 6 var? 1 6 var? or if
                    2 5 var-set
                    3 ebit-clear
                else
                    0 5 roomC5.cmd00
                    0 7 var? 1 7 var? or if
                        2 6 var-set
                        2 ebit-clear
                    else
                        0 6 roomC5.cmd00
                        0 8 var? 1 8 var? or if
                            2 7 var-set
                            1 ebit-clear
                        else
                            2 8 var-set
                            0 7 roomC5.cmd00
                            1 ebit-clear
                            0 ebit-clear
                        then
                    then
                then
            then
        else 2 3 var? if
            0 4 var? 1 4 var? or if
                0 3 var? 1 3 var? or if
                    2 2 var-set
                    1 2 roomC5.cmd00
                then
            else
                0 3 roomC5.cmd00
                0 5 var? 1 5 var? or if
                    2 4 var-set
                    4 ebit-clear
                else
                    0 4 roomC5.cmd00
                    0 6 var? 1 6 var? or if
                        2 5 var-set
                        3 ebit-clear
                    else
                        0 5 roomC5.cmd00
                        0 7 var? 1 7 var? or if
                            2 6 var-set
                            2 ebit-clear
                        else
                            0 6 roomC5.cmd00
                            0 8 var? 1 8 var? or if
                                2 7 var-set
                                1 ebit-clear
                            else
                                2 8 var-set
                                0 7 roomC5.cmd00
                                1 ebit-clear
                                0 ebit-clear
                            then
                        then
                    then
                then
            then
        else 2 2 var? if
            0 3 var? 1 3 var? or if
                0 2 var? 1 2 var? or if
                    2 1 var-set
                    1 1 roomC5.cmd00
                then
            else
                0 2 roomC5.cmd00
                0 4 var? 1 4 var? or if
                    2 3 var-set
                    5 ebit-clear
                else
                    0 3 roomC5.cmd00
                    0 5 var? 1 5 var? or if
                        2 4 var-set
                        4 ebit-clear
                    else
                        0 4 roomC5.cmd00
                        0 6 var? 1 6 var? or if
                            2 5 var-set
                            3 ebit-clear
                        else
                            0 5 roomC5.cmd00
                            0 7 var? 1 7 var? or if
                                2 6 var-set
                                2 ebit-clear
                            else
                                0 6 roomC5.cmd00
                                0 8 var? 1 8 var? or if
                                    2 7 var-set
                                    1 ebit-clear
                                else
                                    2 8 var-set
                                    0 7 roomC5.cmd00
                                    1 ebit-clear
                                    0 ebit-clear
                                then
                            then
                        then
                    then
                then
            then
        else 2 1 var? if
            0 2 var? 1 2 var? or if
                0 1 var? 1 1 var? or if
                    8 ebit-set
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 0 action-force
                    else
                        1 0 0 action-force
                    then
                then
            else
                0 1 roomC5.cmd00
                0 3 var? 1 3 var? or if
                    2 2 var-set
                    6 ebit-clear
                else
                    0 2 roomC5.cmd00
                    0 4 var? 1 4 var? or if
                        2 3 var-set
                        5 ebit-clear
                    else
                        0 3 roomC5.cmd00
                        0 5 var? 1 5 var? or if
                            2 4 var-set
                            4 ebit-clear
                        else
                            0 4 roomC5.cmd00
                            0 6 var? 1 6 var? or if
                                2 5 var-set
                                3 ebit-clear
                            else
                                0 5 roomC5.cmd00
                                0 7 var? 1 7 var? or if
                                    2 6 var-set
                                    2 ebit-clear
                                else
                                    0 6 roomC5.cmd00
                                    0 8 var? 1 8 var? or if
                                        2 7 var-set
                                        1 ebit-clear
                                    else
                                        2 8 var-set
                                        0 7 roomC5.cmd00
                                        1 ebit-clear
                                        0 ebit-clear
                                    then
                                then
                            then
                        then
                    then
                then
            then
        then then then then then then then then
        35 fiona-started? if
            2 8 var? 0 8 var? not and if
                0 57 57 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 7 var? 0 7 var? not and if
                0 49 -49 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 6 var? 0 6 var? not and if
                0 -40 -40 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 5 var? 0 5 var? not and if
                0 -25 43 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 4 var? 0 4 var? not and if
                0 35 20 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 3 var? 0 3 var? not and if
                0 8 -30 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 2 var? 0 2 var? not and if
                0 -22 0 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 1 var? if
                0 0 0 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            7 ebit? not if
                0 0.0 0.0 0.0 $A 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 0 9 char-zone-bits? if
                            7 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                0 0.0 0.0 0.0 $C 5 0 zone
                1 0 9 char-zone-bits? not if
                    7 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            6 ebit? not if
                1 -22.0 0.0 0.0 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 1 9 char-zone-bits? if
                            6 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                1 -22.0 0.0 0.0 6 5 0 zone
                1 1 9 char-zone-bits? not if
                    6 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            5 ebit? not if
                2 8.0 0.0 -30.0 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 2 9 char-zone-bits? if
                            5 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                2 8.0 0.0 -30.0 6 5 0 zone
                1 2 9 char-zone-bits? not if
                    5 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            4 ebit? not if
                3 34.65 0.0 20.0 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 3 9 char-zone-bits? if
                            4 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                3 34.65 0.0 20.0 7 5 0 zone
                1 3 9 char-zone-bits? not if
                    4 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            3 ebit? not if
                4 -25.0 0.0 43.31 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 4 9 char-zone-bits? if
                            3 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                4 -25.0 0.0 43.31 7 5 0 zone
                1 4 9 char-zone-bits? not if
                    3 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            2 ebit? not if
                5 -40.37 0.0 -40.38 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 5 9 char-zone-bits? if
                            2 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                5 -40.37 0.0 -40.38 6 5 0 zone
                1 5 9 char-zone-bits? not if
                    2 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            1 ebit? not if
                6 48.58 0.0 -48.58 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 6 9 char-zone-bits? if
                            1 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                6 48.58 0.0 -48.58 6 5 0 zone
                1 6 9 char-zone-bits? not if
                    1 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            0 ebit? not if
                7 57.25 0.0 57.25 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 7 9 char-zone-bits? if
                            0 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                7 57.25 0.0 57.25 7 5 0 zone
                1 7 9 char-zone-bits? not if
                    0 ebit-clear
                then
            then
        then
    then
;

: roomC5.phase2 ( -- )   \ 00440E00
    0 $D $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 $E $32 char-faces-area? if
        5 3 0 scene-change
    then
;

: roomC5.act00 ( -- )   \ 00440E20
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $FF 1.0 0 bgm
    $F $54 fade
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
    things-clear
    -1 self-move-16
    $C6 room-preload
    $10 $FF movie-param
    0 $F9 2 action
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
    50 hewie-trust
    8 state-flag-set
    $FF door-reopen-unlock
    $101 door-close-off-lock
    exits-rebuild
    $C6 0 -1 hewie-to-room
    $3E resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: roomC5.act01 ( -- )   \ 00440EE0
    self-wait-done
    hewie-bark
    self-wait-done
    2 8 var? if
        $37 57.25 57.25 -175 $204 5 self-move-to
        self-wait-done
    else 2 7 var? if
        $5C 48.58 -48.58 -84 $204 5 self-move-to
        self-wait-done
    else 2 6 var? if
        $FA -40.37 -40.38 10 $204 5 self-move-to
        self-wait-done
    else 2 5 var? if
        $1B -25.0 43.31 111 $204 5 self-move-to
        self-wait-done
    else 2 4 var? if
        $3F 34.65 20.0 -151 $204 5 self-move-to
        self-wait-done
    else 2 3 var? if
        $6A 8.0 -30.0 -44 $204 5 self-move-to
        self-wait-done
    else 2 2 var? if
        4 -22.0 0.0 89 $204 5 self-move-to
        self-wait-done
    else 2 1 var? if
        $D8 0.0 0.0 89 $204 5 self-move-to
        self-wait-done
    then then then then then then then then
    0 $A hewie-anim-root
    self-frames-reset
    $A self-wait-frames
    self-idle-or-end
;

: roomC5.act02 ( -- )   \ 00440FC0
    begin
        $33C cutscene-cue-reached? not while
        yield
    repeat
    0 8 roomC5.cmd00
    0 7 roomC5.cmd00
    0 6 roomC5.cmd00
    0 5 roomC5.cmd00
    0 4 roomC5.cmd00
    0 3 roomC5.cmd00
    0 2 roomC5.cmd00
    0 1 roomC5.cmd00
    self-idle-or-end
;

: roomC5.act03 ( -- )   \ 00441000
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0.0 -105.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: roomC5.act04 ( -- )   \ 00441030
    self-wait-done
    3 -1 var-set
    4 -1 var-set
    5 -1 var-set
    6 -1 var-set
    7 -1 var-set
    8 -1 var-set
    9 -1 var-set
    $A -1 var-set
    1 8 roomC5.cmd00
    1 7 roomC5.cmd00
    1 6 roomC5.cmd00
    1 5 roomC5.cmd00
    1 4 roomC5.cmd00
    1 3 roomC5.cmd00
    1 2 roomC5.cmd00
    1 1 roomC5.cmd00
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
    0 $F9 2 action
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

: roomC5.phase5 ( -- )   \ 0047AF20
    $22 state-flag-clear
;

' roomC5.enter $C5 0 room-script!
' roomC5.char-enter $C5 6 room-script!
' roomC5.phase1 $C5 1 room-script!
' roomC5.phase2 $C5 2 room-script!
' roomC5.act00 $C5 $00 action-script!
' roomC5.act01 $C5 $01 action-script!
' roomC5.act02 $C5 $02 action-script!
' roomC5.act03 $C5 $03 action-script!
' roomC5.act04 $C5 $04 action-script!
' roomC5.phase5 $C5 5 room-script!

\ ---- room $C6 ----------------------------------------------------------------------------------

: roomC6.enter ( -- )   \ 00441170
    room-sounds
    $22 state-flag-set
    $30 1.0 0 bgm
    $2DA story-flag? $2DB story-flag? not and if
        0 -9.0 140.97 -10.23 flicker-sprite
    then
;

: roomC6.char-enter ( -- )   \ 004411A0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 1 char-camera
                0 camera-follow
            else
                1 3 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 1 char-camera
            0 camera-follow
        else
            1 3 1 char-camera
            1 camera-follow
        then
    then then
    0 3 1 area-camera
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
    0 self-is? if
        $80 exit-taken? if
            0 $1C2 -45.44 -52.11 105 char-to-xz
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
            0.0 sound-volume-scale
            0 0 0 action
        then
        $81 exit-taken? if
            0 char-activate
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 1 char-camera
                0 camera-follow
            else
                1 3 1 char-camera
                1 camera-follow
            then
            1 char-activate
            $C6 0 836 hewie-to-room
            1 $344 6.81 -30.23 0 char-to-xz
            0.0 sound-volume-scale
            0 0 1 action
        then
        0 exit-taken? if
            8 map-page
        then
    then
    1 self-is? if
        $80 exit-taken? if
            1 $6D -14.34 -72.11 -30 char-to-xz
        then
    then
;

: roomC6.phase1 ( -- )   \ 004412A0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 1 -1 1 chars-area-camera
    8 1 -1 1 chars-area-camera
    9 2 0 1 chars-area-camera
    $A 2 0 1 chars-area-camera
    $B 3 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $F char-entered-area? if
        7 map-page
    then
    0 $F char-left-area? if
        8 map-page
    then
    $2DA story-flag? not if
        0 -9.0 139.97 -10.23 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 730 var-set
                $1A 731 var-set
                $1B 0 var-set
                $1C -9000 var-set
                $1D 140970 var-set
                $1E -10230 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -127.0 1.7 10.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -127.0 1.7 10.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -127.0 1.7 10.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -127.0 1.7 10.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: roomC6.phase2 ( -- )   \ 00441400
    0 $C $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 $E $32 char-faces-area? if
        5 3 0 scene-change
    then
    $2DA story-flag? $2DB story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: roomC6.act00 ( -- )   \ 00441430
    yield
    camera-restart
    1 exit-prepare
    self-frames-reset
    self-wait-16
    8 state-flag-clear
    $F $51 fade
    $828A item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: roomC6.act01 ( -- )   \ 00441450
    8 state-flag-clear
    $F $51 fade
    wait-fade
    $18 state-flag-clear
    $FF 0 char-visible
    0 self-scripted
    self-idle-or-end
;

: roomC6.act02 ( -- )   \ 00441470
    self-wait-done
    -9.0 -10.23 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2DB story-flag-set
            0 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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

: roomC6.act03 ( -- )   \ 004414D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0.0 -105.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: roomC6.phase5 ( -- )   \ 0047AF30
    $22 state-flag-clear
;

' roomC6.enter $C6 0 room-script!
' roomC6.char-enter $C6 6 room-script!
' roomC6.phase1 $C6 1 room-script!
' roomC6.phase2 $C6 2 room-script!
' roomC6.act00 $C6 $00 action-script!
' roomC6.act01 $C6 $01 action-script!
' roomC6.act02 $C6 $02 action-script!
' roomC6.act03 $C6 $03 action-script!
' roomC6.phase5 $C6 5 room-script!

\ ---- room $C7 ----------------------------------------------------------------------------------

\ the screen fade (renderer +0x70) by script variable 1 with Hewie's light 0xF: byte 3 0 clear
\ (0x808080), 2 full (0x80808080); 1 fades in by 0x10 a call and 3 back out, waiting (2),
\ Hewie's +0xE4 set once there
: roomC7.cmd00 ( b0 -- )  drop s" roomC7.cmd00" stub-step ;
\ (as Room0C_Cmd01, the player only)
: roomC7.cmd01 ( b0 -- )  drop s" roomC7.cmd01" stub-step ;
\ a cursor effect (RoomC7Cursor_vtable) at (x, y) kept in script variables 7 / 8, its slot in 6:
\ byte 3 0 puts it at (246, 242); 1 moves it 4 a frame by the stick or the d-pad (x 0..492, y
\ 0..420), waiting (2) until confirm (event 4 +0x5C) or cancel (+0x60); 2 ends it
: roomC7.cmd02 ( b0 -- )  drop s" roomC7.cmd02" stub-step ;
\ (as RoomC0_Cmd01) byte 3 2 up from frame 1268 (2.79 a frame, light 0x23) and 3 from frame 25
\ (4.27, light 0xF), held at 0x80; 1 the fade fully on with light 0xA, else off with light 0x11
\ (+0x64)
: roomC7.cmd03 ( b0 -- )  drop s" roomC7.cmd03" stub-step ;
\ (as Room2A_Cmd03) the 0xD40-byte effect Debris_vtable started with byte 3
: roomC7.cmd04 ( b0 -- )  drop s" roomC7.cmd04" stub-step ;
\ (as Room23_Cmd00) the kind-0xB character's model +0xCC8: 0 (byte 3 1) or -0.02
: roomC7.cmd05 ( b0 -- )  drop s" roomC7.cmd05" stub-step ;
\ (as Room48_Cmd02) byte 3 0 starts the effect BackdropModel_vtable (its slot in event variable
\ 9); else that one is ended (EffectMgr_Remove)
: roomC7.cmd06 ( b0 -- )  drop s" roomC7.cmd06" stub-step ;
\ script variables 7 / 8 (the player's spot) in 151..269 / 171..219
: roomC7.cond00? ( -- flag )  s" roomC7.cond00?" stub-flag ;
\ a room callback: the pursuer's Pursuer_GrabHewieBehind
: roomC7.cond01? ( -- flag )  s" roomC7.cond01?" stub-flag ;

: roomC7.enter ( -- )   \ 0042F500
    room-sounds
    $1B 1.0 0 bgm
    $8E story-flag? not if
        0 0 $14 door-bits
        1 0 $14 door-bits
        8 1 object-show
        9 1 object-show
        $A 1 object-show
    else
        0 1 $14 door-bits
        1 1 $14 door-bits
    then
    $91 story-flag? not if
        2 3 var-set
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $33E story-flag? not if
        3 3 var-set
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    $33F story-flag? not if
        4 3 var-set
        6 1 $14 door-bits
        7 0 $14 door-bits
    else
        6 0 $14 door-bits
        7 1 $14 door-bits
    then
    8 1 $14 door-bits
    0 roomC7.cmd06
;

: roomC7.char-enter ( -- )   \ 0042F590
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
;

: roomC7.phase1 ( -- )   \ 0042F5D0
    $AE story-flag? $8E story-flag? and $93 story-flag? not and if
        0 0 char-entered-area? if
            $93 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 $A action-force
            else
                1 0 $A action-force
            then
        then
    else 0 exit-usable? if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 0 0 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 1 -1 1 chars-area-camera
    0 20000.0 camera-value
    1 ebit? not 2 ebit? not and if
        0 ebit? not if
            0 control-action? if
                0 5 char-in-area? if
                    0 -6 -12 $3C char-faces-xz? if
                        hewie-stays? if
                            0 0 var-set
                            0 1 0 action
                        then
                    then
                else 0 6 char-in-area? if
                    0 -9 10 $3C char-faces-xz? if
                        hewie-stays? if
                            0 1 var-set
                            0 1 0 action
                        then
                    then
                else 0 7 char-in-area? if
                    0 12 7 $3C char-faces-xz? if
                        hewie-stays? if
                            0 2 var-set
                            0 1 0 action
                        then
                    then
                then then then
            then
        else 45 fiona-started? if
            $FE 2 char-C4? not if
                stalker-free? if
                    0 0 var? if
                        $FE 8 char-in-area? if
                            roomC7.cond01? if
                                1 ebit-set
                                $91 story-flag? if
                                    $FF panic-stage? if
                                        3 panic-stage
                                    then
                                    0 0 8 action-force
                                then
                            then
                        then
                    else 0 1 var? if
                        $FE 9 char-in-area? if
                            roomC7.cond01? if
                                1 ebit-set
                            then
                        then
                    else 0 2 var? if
                        $FE $A char-in-area? if
                            roomC7.cond01? if
                                1 ebit-set
                            then
                        then
                    then then then
                then
            then
        else 44 fiona-started? if
            2 ebit-set
        then then then
    then
    $91 story-flag? not if
        0 3.0 620.0 79.0 $A 8 0 zone
        0 $C char-in-area? 0 0 char-in-zone? and if
            5 0 var-set
            0 $F1 6 action
        then
    then
    $33E story-flag? not if
        1 -58.0 620.0 54.0 $A 8 0 zone
        0 $D char-in-area? 0 1 char-in-zone? and if
            5 1 var-set
            0 $F2 6 action
        then
    then
    $33F story-flag? not if
        2 -5.0 620.0 -78.0 $A 8 0 zone
        0 $E char-in-area? 0 2 char-in-zone? and if
            5 2 var-set
            0 $F3 6 action
        then
    then
    $8E story-flag? not if
        3 0.0 620.0 41.0 $11 10 0 zone
        8 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: roomC7.phase2 ( -- )   \ 0042F7B0
    $10 state-flag? if
        -2147483646 scene-request? if
            5 9 1 scene-change
        then
    then
    0 $B char-in-area? 0 90 $3C char-heading? and if
        5 2 0 scene-change
    then
    $33E story-flag? not if
        0 $D char-in-area? 0 -58 54 $32 char-faces-xz? and if
            5 $E 0 scene-change
        then
    then
    $33F story-flag? not if
        0 $E char-in-area? 0 -5 -78 $32 char-faces-xz? and if
            5 $E 0 scene-change
        then
    then
    $91 story-flag? not if
        0 $C char-in-area? 0 3 79 $32 char-faces-xz? and if
            5 $E 0 scene-change
        then
    else $8E story-flag? if
        0 $C char-in-area? 0 3 79 $32 char-faces-xz? and if
            5 $F 0 scene-change
        then
    then then
    0 $F char-in-area? 0 -6 -12 $3C char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 $10 char-in-area? 0 -9 10 $3C char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 $11 char-in-area? 0 12 7 $3C char-faces-xz? and if
        5 $12 0 scene-change
    then
;

: roomC7.phase3 ( -- )   \ 0042F860
    9 ebit? not if
        17.4 630.0 89.8 -59.9 630.0 69.0 17.4 580.0 89.8 -59.9 580.0 69.0 lights-doorway
    then
    -51.2 630.0 75.7 -91.2 630.0 6.5 -51.2 580.0 75.7 -91.2 580.0 6.5 lights-doorway
    -23.7 617.0 -26.3 -33.3 617.0 12.9 -55.3 617.0 -57.8 -74.5 617.0 29.0 lights-doorway
    6.0 617.0 -35.0 -29.8 617.0 -19.9 15.6 617.0 -78.5 -66.5 617.0 -44.5 lights-doorway
    27.8 617.0 -23.1 6.0 617.0 -35.0 66.5 617.0 -44.4 15.6 617.0 -78.5 lights-doorway
    7.0 617.0 40.4 35.1 617.0 6.9 15.6 617.0 78.5 78.5 617.0 15.6 lights-doorway
    35.1 617.0 6.9 27.8 617.0 -23.0 78.5 617.0 15.6 66.5 617.0 -44.4 lights-doorway
;

: roomC7.phase5 ( -- )   \ 0042F9C0
    1 0 char-in-nav-group? if
        1 action-end
        0 0 var? if
            1 $33 2.82 68.25 14 char-to-xz
        else 0 1 var? if
            1 $38 -64.059 -24.05 -107 char-to-xz
        else 0 2 var? if
            1 $4C 58.05 -31.28 121 char-to-xz
        then then then
    then
;

: roomC7.act00 ( -- )   \ 0042FA10
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 var? if
        $91 -15.35 -32.75 20 $FFFF $A self-move-to
        self-wait-done
    else 0 1 var? if
        $C7 -22.68 28.06 140 $FFFF $A self-move-to
        self-wait-done
    else 0 2 var? if
        $14 31.63 18.64 -120 $FFFF $A self-move-to
        self-wait-done
    then then then
    1 self-scripted
    1 self-noclip
    0 0 8 nav-group
    1 0 $30 nav-group
    0 0 var? if
        $C4 -5.62 -11.6 20 $204 5 self-move-to
        self-wait-done
    else 0 1 var? if
        $C0 -8.96 10.23 140 $204 5 self-move-to
        self-wait-done
    else 0 2 var? if
        $BE 11.74 6.8 -120 $204 5 self-move-to
        self-wait-done
    then then then
    2 roomC7.cmd00
    3 roomC7.cmd00
    0 0 var? if
        1 $D0 -0.12 8.95 5 char-to-xz
    else 0 1 var? if
        1 $FF -7.73 -4.22 -125 char-to-xz
    else 0 2 var? if
        1 $F9 6.6 -4.24 117 char-to-xz
    then then then
    1 self-scripted
    0 self-noclip
    $204 0 hewie-anim-root
    0 roomC7.cmd00
    1 roomC7.cmd00
    0 8 self-anim-blend
    self-frames-reset
    8 self-wait-frames
    $102 self-anim
    self-wait-anim
    2 $A self-anim-blend
    self-frames-reset
    $A self-wait-frames
    0 ebit-set
    begin
        1 ebit? not 2 ebit? not and while
        0 0 var? if
            $FE 8 char-in-area? if
                $FE self-look-at
                yield
            else 0 8 char-in-area? if
                0 self-look-at
                yield
            else
                $FF self-look-at
                yield
            then then
        else 0 1 var? if
            $FE 9 char-in-area? if
                $FE self-look-at
                yield
            else 0 9 char-in-area? if
                0 self-look-at
                yield
            else
                $FF self-look-at
                yield
            then then
        else 0 2 var? if
            $FE $A char-in-area? if
                $FE self-look-at
                yield
            else 0 $A char-in-area? if
                0 self-look-at
                yield
            else
                $FF self-look-at
                yield
            then then
        then then then
        yield
    repeat
    $FF self-look-at
    yield
    1 ebit? if
        $75 0 hewie-action
        3 ebit-clear
        begin
            3 ebit? not if
                1 $38 char-on-nav-flags? not if
                    3 ebit-set
                    0 0 $30 nav-group
                    1 0 8 nav-group
                then
            then
            117 hewie-action? while
            yield
        repeat
        1 $38 char-on-nav-flags? if
            0 0 var? if
                $33 2.39 70.98 1.0 100 hewie-go-to
                self-wait-done
            else 0 1 var? if
                $38 -61.02 -37.59 1.0 100 hewie-go-to
                self-wait-done
            else 0 2 var? if
                $4C 61.9 -34.57 1.0 100 hewie-go-to
                self-wait-done
            then then then
            0 0 $30 nav-group
            1 0 8 nav-group
        then
    else
        hewie-bark
        self-wait-done
        0 0 var? if
            $33 2.39 70.98 1.0 100 hewie-go-to
            self-wait-done
        else 0 1 var? if
            $38 -61.02 -37.59 1.0 100 hewie-go-to
            self-wait-done
        else 0 2 var? if
            $4C 61.9 -34.57 1.0 100 hewie-go-to
            self-wait-done
        then then then
        0 0 $30 nav-group
        1 0 8 nav-group
    then
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: roomC7.act01 ( -- )   \ 0042FCA0
    $FF 1.0 0 bgm
    $18 state-flag-set
    1 self-scripted
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
    $F 4 fade
    wait-fade
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    0 $F9 $C action
    9 ebit-set
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
    9 ebit-clear
    1 roomC7.cmd03
    $10 state-flag-clear
    $23 state-flag-clear
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $D action
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
    $40 resident-flag-set
    50 hewie-trust
    $FE action-end
    $FE char-done
    $8E story-flag-set
    0 $33 -1.38 70.2 0 char-to-xz
    1 $33 5.0 67.76 0 char-to-xz
    $AE story-flag? not if
        2 0 char-remove
    then
    camera-restart
    $103 door-unlock
    $100 door-unlock
    $E0 door-lock
    $FE $C5 0 room-doors-state
    $FE $C6 0 room-doors-state
    $FE $C7 0 room-doors-state
    1 0 8 nav-group
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    $1B 1.0 0 bgm
    $F $41 fade
    wait-fade
    $41 resident-flag-set
    $24D item-give
    $828B item-give
    builtin.act9C
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: roomC7.act02 ( -- )   \ 0042FE40
    self-wait-done
    $8E story-flag? not if
        1 self-scripted
        1 self-noclip
        self-frames-reset
        8 self-wait-frames
        $17 state-flag-set
        0 $36 0.0 33.0 180 char-to-xz
        1 10.0 60.0 0.0 5.0 event-camera
        self-frames-reset
        8 self-wait-frames
        4 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 self-scripted
        0 self-noclip
        0 $9D 0.0 41.0 180 char-to-xz
    else $A4 1 item-count? if
        1 self-scripted
        1 self-noclip
        self-frames-reset
        8 self-wait-frames
        $17 state-flag-set
        0 $D2 -0.95 33.0 180 char-to-xz
        1 2.0 50.0 0.0 5.0 event-camera
        self-frames-reset
        8 self-wait-frames
        $A message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 self-scripted
        0 self-noclip
        0 $9D 0.0 41.0 180 char-to-xz
    else
        $B message
        wait-message
    then then
    self-idle-or-end
;

: roomC7.act07 ( -- )   \ 00430110
    $F $44 fade
    $FF 1.0 0 bgm
    wait-fade
    8 0 $14 door-bits
    1 roomC7.cmd06
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    $18 state-flag-set
    1 action-end
    1 char-done
    2 char-unload
    0 state-flag-clear
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $B action
    $10 $50 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    3 message-prepare
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
    1 1 $14 door-bits
    8 1 $14 door-bits
    0 roomC7.cmd06
    $100 door-open-clear
    $100 door-lock
    doors-room-in
    camera-restart
    0 $33 6.38 70.12 180 char-to-xz
    1 char-activate
    $C7 0 41 hewie-to-room
    1 $29 -17.04 65.97 -160 char-to-xz
    $FE char-activate
    $FE $C7 89 2 stalker-to-room
    $FE $59 -58.86 10.32 0 char-to-xz
    0 roomC7.cmd03
    1 0 8 nav-group
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    $10 state-flag-set
    9 1.0 0 bgm
    $23 state-flag-set
    $F 1 fade
    wait-fade
    $3F resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: roomC7.act05 ( -- )   \ 0042FF20
    self-wait-done
    1 self-scripted
    1 self-noclip
    self-frames-reset
    8 self-wait-frames
    $17 state-flag-set
    0 $36 0.0 33.0 180 char-to-xz
    1 10.0 60.0 0.0 5.0 event-camera
    self-frames-reset
    8 self-wait-frames
    0 roomC7.cmd02
    5 message
    begin
        1 roomC7.cmd02
        4 ebit? if
            5 ebit? not roomC7.cond00? not or if
                1 $86 0.0 0.0 0.0 0 0 sound
                yield
            else
                0 6 -1.0 632.0 31.0 0 0 sound
                0 1 $14 door-bits
                $23 item-use
                6 ebit-set
                2 0 char-remove
                $25 partner-load
                5 message-close
                2 roomC7.cmd02
                self-frames-reset
                $1E self-wait-frames
                6 ebit? if
                    ['] roomC7.act07 goto
                then
                0 0.0 0.0 0.0 0.0 event-camera
                $17 state-flag-clear
                0 self-scripted
                0 self-noclip
                0 $9D 0.0 41.0 180 char-to-xz
                self-idle-or-end
            then
        else
            $2C $85 0.0 0.0 0.0 0 0 sound
            5 message-close
            2 roomC7.cmd02
            self-frames-reset
            $1E self-wait-frames
            6 ebit? if
                ['] roomC7.act07 goto
            then
            0 0.0 0.0 0.0 0.0 event-camera
            $17 state-flag-clear
            0 self-scripted
            0 self-noclip
            0 $9D 0.0 41.0 180 char-to-xz
            self-idle-or-end
        then
    again
;

: roomC7.act03 ( -- )   \ 0047ADD0
    5 ebit-clear
    ['] roomC7.act05 goto
;

: roomC7.act04 ( -- )   \ 0047ADD8
    5 ebit-set
    ['] roomC7.act05 goto
;

: roomC7.act06 ( -- )   \ 00430000
    0 7 6 char-sound
    5 0 var? if
        2 roomC7.cmd01
        0 -1.0 623.0 77.0 0 0 0 0 dust
        0 4.0 623.0 77.0 0 0 0 0 dust
        2 0 var? if
            0 8 6 char-sound
            0 roomC7.cmd04
            3 roomC7.cmd04
            2 0 $14 door-bits
            3 1 $14 door-bits
            $91 story-flag-set
        then
    else 5 1 var? if
        3 roomC7.cmd01
        0 -59.0 623.0 50.0 0 0 0 0 dust
        0 -55.0 623.0 54.0 0 0 0 0 dust
        3 0 var? if
            0 8 6 char-sound
            1 roomC7.cmd04
            4 roomC7.cmd04
            4 0 $14 door-bits
            5 1 $14 door-bits
            $33E story-flag-set
        then
    else 5 2 var? if
        4 roomC7.cmd01
        0 -8.0 623.0 -77.0 0 0 0 0 dust
        0 -2.0 623.0 -77.0 0 0 0 0 dust
        4 0 var? if
            0 8 6 char-sound
            2 roomC7.cmd04
            5 roomC7.cmd04
            6 0 $14 door-bits
            7 1 $14 door-bits
            $33F story-flag-set
        then
    then then then
    self-frames-reset
    $1E self-wait-frames
    self-idle-or-end
;

: roomC7.act08 ( -- )   \ 00430250
    1 self-scripted
    self-wait-done
    $FE self-look-at
    yield
    $C00 self-anim
    0 1 char-wait-motion
    0 $2F 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    ['] roomC7.act01 goto
;

: roomC7.act09 ( -- )   \ 0047ADE0
    2 message
    self-idle-or-end
;

: roomC7.act0A ( -- )   \ 00430270
    $F $54 fade
    $FF 1.0 0 bgm
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    wait-fade
    $FF 1 char-visible
    $B 3 $FF char-load
    $B $1E char-layer
    $FE $A char-layer
    3 char-unload
    1 action-end
    1 char-done
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $2D $41 movie-param
    0 $F9 $11 action
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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
    1 roomC7.cmd05
    2 0 char-remove
    3 0 char-remove
    8 state-flag-set
    0 exit-prepare
    $93 story-flag-set
    $42 resident-flag-set
    $81 exit-check
    self-idle-or-end
;

: roomC7.act0B ( -- )   \ 00430330
    0 roomC7.cmd03
    begin
        $4F5 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $523 cutscene-cue-reached? not while
        2 roomC7.cmd03
        yield
    repeat
    1 roomC7.cmd03
    self-idle-or-end
;

: roomC7.act0C ( -- )   \ 00430350
    0 roomC7.cmd03
    begin
        $1A cutscene-cue-reached? not while
        yield
    repeat
    begin
        $38 cutscene-cue-reached? not while
        3 roomC7.cmd03
        yield
    repeat
    1 roomC7.cmd03
    self-idle-or-end
;

: roomC7.act0D ( -- )   \ 00430370
    begin
        0 cutscene-shot? not while
        yield
    repeat
    begin
        $E8 cutscene-cue-reached? not while
        $3E sprites-additive
        yield
    repeat
    begin
        $E9 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $11C cutscene-cue-reached? not while
        $29 sprites-additive
        yield
    repeat
    self-idle-or-end
;

: roomC7.act0E ( -- )   \ 0047ADE8
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: roomC7.act0F ( -- )   \ 004303A0
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    7 ebit? not if
        7 ebit-set
        7 message
        wait-message
    else
        7 ebit-clear
        8 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: roomC7.act10 ( -- )   \ 0047ADF0
    self-wait-done
    $C message
    wait-message
    self-idle-or-end
;

: roomC7.act11 ( -- )   \ 004303C0
    begin
        4 cutscene-shot? not while
        yield
    repeat
    0 roomC7.cmd05
    self-idle-or-end
;

: roomC7.act12 ( -- )   \ 004303D0
    self-wait-done
    0 $F char-in-area? if
        -6.0 -12.0 self-turn-to-xz
        self-wait-done
    else 0 $10 char-in-area? if
        -9.0 10.0 self-turn-to-xz
        self-wait-done
    else 0 $11 char-in-area? if
        12.0 7.0 self-turn-to-xz
        self-wait-done
    then then then
    $A01 self-anim
    self-frames-reset
    self-wait-16
    9 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: roomC7.act13 ( -- )   \ 00430410
    self-wait-done
    0 1 $14 door-bits
    2 1 $14 door-bits
    4 1 $14 door-bits
    6 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $25 partner-load
    2 char-unload
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $B action
    $10 $50 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    3 message-prepare
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

: roomC7.act14 ( -- )   \ 004304C0
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    3 1 $14 door-bits
    5 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $25 partner-load
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
    0 $F9 $C action
    9 ebit-set
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
    9 ebit-clear
    1 roomC7.cmd03
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: roomC7.act15 ( -- )   \ 00430580
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    3 1 $14 door-bits
    5 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $17 3 $FF char-load
    3 char-unload
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $D action
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
    1 action-end
    1 char-done
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: roomC7.act16 ( -- )   \ 00430630
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    3 1 $14 door-bits
    5 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 20000.0 camera-value
    $17 3 $FF char-load
    $B 4 $FF char-load
    3 char-unload
    4 char-unload
    $FF 1 char-visible
    $B $1E char-layer
    $FE $A char-layer
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $2D $41 movie-param
    0 $F9 $11 action
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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
    1 roomC7.cmd05
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' roomC7.enter $C7 0 room-script!
' roomC7.char-enter $C7 6 room-script!
' roomC7.phase1 $C7 1 room-script!
' roomC7.phase2 $C7 2 room-script!
' roomC7.phase3 $C7 3 room-script!
' roomC7.phase5 $C7 5 room-script!
' roomC7.act00 $C7 $00 action-script!
' roomC7.act01 $C7 $01 action-script!
' roomC7.act02 $C7 $02 action-script!
' roomC7.act03 $C7 $03 action-script!
' roomC7.act04 $C7 $04 action-script!
' roomC7.act05 $C7 $05 action-script!
' roomC7.act06 $C7 $06 action-script!
' roomC7.act07 $C7 $07 action-script!
' roomC7.act08 $C7 $08 action-script!
' roomC7.act09 $C7 $09 action-script!
' roomC7.act0A $C7 $0A action-script!
' roomC7.act0B $C7 $0B action-script!
' roomC7.act0C $C7 $0C action-script!
' roomC7.act0D $C7 $0D action-script!
' roomC7.act0E $C7 $0E action-script!
' roomC7.act0F $C7 $0F action-script!
' roomC7.act10 $C7 $10 action-script!
' roomC7.act11 $C7 $11 action-script!
' roomC7.act12 $C7 $12 action-script!
' roomC7.act13 $C7 $13 action-script!
' roomC7.act14 $C7 $14 action-script!
' roomC7.act15 $C7 $15 action-script!
' roomC7.act16 $C7 $16 action-script!

\ ---- room $C8 ----------------------------------------------------------------------------------

\ an effect TurningModel_vtable (8 bytes), not started
: roomC8.cmd00 ( -- )  s" roomC8.cmd00" stub-step ;

: roomC8.enter ( -- )   \ 00441520
    $18 1.0 0 bgm
    $8E story-flag? not $94 story-flag? or if
        1 0 $10020000 nav-group
    else
        0 1 $14 door-bits
    then
    $2E0 story-flag? not if
        0 19.59 39.0 -247.61 flicker-sprite
    then
    roomC8.cmd00
;

: roomC8.char-enter ( -- )   \ 00441560
    $8E story-flag? not $94 story-flag? or if
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
    else
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
    then
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
;

: roomC8.phase1 ( -- )   \ 00441620
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
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $8E story-flag? not if
        1 -4.8 38.0 -171.89 $35 15 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        2 ebit-set
                        $1E chance? if
                            $1F 1 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: roomC8.phase2 ( -- )   \ 004416A0
    $8E story-flag? if
        0 4 char-in-area? 0 0 $32 char-heading? and if
            5 0 0 scene-change
        then
        0 5 char-in-area? 0 90 $32 char-heading? and if
            5 0 0 scene-change
        then
    else 0 6 char-in-area? 0 0 $32 char-heading? and if
        5 1 0 scene-change
    then then
    $2E0 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: roomC8.act00 ( -- )   \ 004416F0
    self-wait-done
    0 ebit? not if
        0 4 char-in-area? if
            0 message
            $A01 self-anim
            self-wait-anim
        else
            180 self-turn-angle
            self-wait-done
            1 message
            $A00 self-anim
            self-wait-anim
        then
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        wait-message
        0 ebit-set
    else
        $900 self-anim
        self-wait-anim
        2 message
        wait-message
        $901 self-anim
        self-wait-anim
        0 ebit-clear
    then
    self-idle-or-end
;

: roomC8.act01 ( -- )   \ 00441730
    self-wait-done
    1 ebit? not if
        1 ebit-set
        3 message
        wait-message
    else
        1 ebit-clear
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        4 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: roomC8.act02 ( -- )   \ 00441750
    self-wait-done
    19.59 -247.61 self-turn-to-xz
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
            $2E0 story-flag-set
            0 effect-remove
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

' roomC8.enter $C8 0 room-script!
' roomC8.char-enter $C8 6 room-script!
' roomC8.phase1 $C8 1 room-script!
' roomC8.phase2 $C8 2 room-script!
' roomC8.act00 $C8 $00 action-script!
' roomC8.act01 $C8 $01 action-script!
' roomC8.act02 $C8 $02 action-script!
