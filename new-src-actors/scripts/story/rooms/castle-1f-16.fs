\ story/rooms/castle-1f-16.fs - the event scripts of room castle-1f-16 ($1A; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-16
USING: room-names story.words story.shared flag-names ;

\ room 0x1A (D_003FC660): the fan turns
: castle-1f-16.cmd00 ( -- )  s" castle-1f-16.cmd00" stub-step ;
\ room 0x1A: room effect slot 0 made anew as the second TV screen (TvScreenB).
: castle-1f-16.cmd01 ( -- )  s" castle-1f-16.cmd01" stub-step ;

: castle-1f-16.enter ( -- )   \ 003FC060
    room-sounds
    0 $F1 0 action
    castle-1f-16.cmd01
    $30A story-flag? not if
        0 1 $14 door-bits
        0 0 1 effect-string
    else
        0 0 $14 door-bits
        0 1 1 effect-string
        1 noise-level
    then
    $226 story-flag? not if
        1 -25.0 10.0 13.0 flicker-sprite
    then
;

: castle-1f-16.act07 ( -- )   \ 003FC590
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 1 char-file-load
    then
    1 ebit-clear
    8 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else 8 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else 8 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else 8 3 pvar? if
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
        0 $FE 5 action
    else
        $78 1 item-cooldown
    then
    8 pvar-inc
    exit
;

: castle-1f-16.char-enter ( -- )   \ 003FC0A0
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
    $FE self-is? if
        fiona-hidden state-flag? if
            4 ebit-set
            $30A story-flag? 2 creature-action? not and if
                2 self-is? 6 self-is? or 7 self-is? or if
                    $78 1 item-cooldown
                    0 $FE 6 action
                else
                    castle-1f-16.act07
                then
            else
                castle-1f-16.act07
            then
        else
            4 ebit-clear
        then
    then
;

: castle-1f-16.phase1 ( -- )   \ 003FC110
    2 ebit? not if
        6 sound-bank-loaded? if
            $30A story-flag? if
                $40000001 6 23.0 11.0 9.0 0 0 sound
            then
            2 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 27.0 15.0 -6.0 0 0 sound
            else 0 1 var? if
                $40000006 6 27.0 15.0 -6.0 0 0 sound
            else 0 2 var? if
                $40000007 6 27.0 15.0 -6.0 0 0 sound
            else 0 3 var? if
                $40000008 6 27.0 15.0 -6.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $309 story-flag? if
            $C0000001 6 23.0 11.0 9.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    5 2 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    $30A story-flag? not if
        0 1 $14 door-bits
        0 0 1 effect-string
    else
        0 0 $14 door-bits
        0 1 1 effect-string
    then
;

: castle-1f-16.phase2 ( -- )   \ 003FC220
    0 8 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 9 char-in-area? 0 60 $5A char-heading? and if
        5 2 0 scene-change
    then
    0 $A char-in-area? 0 45 $3C char-heading? and if
        $FE char-here? not if
            5 3 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 $B char-in-area? 0 -72 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 $C char-in-area? 0 0 $32 char-heading? and if
        5 9 0 scene-change
    then
    $226 story-flag? not if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: castle-1f-16.phase3 ( -- )   \ 003FC290
    camera-setup-changed? if
        1 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else 2 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then then
    then
;

: castle-1f-16.act00 ( -- )   \ 0047AA68
    begin
        castle-1f-16.cmd00
        yield
    again
;

: castle-1f-16.act01 ( -- )   \ 003FC2D0
    self-wait-done
    -25.0 13.0 self-turn-to-xz
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
            $226 story-flag-set
            1 effect-remove
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

: castle-1f-16.act02 ( -- )   \ 003FC330
    self-wait-done
    $E1 18.5 13.6 120 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $902 self-anim
    self-wait-anim
    9 0 pvar? 9 1 pvar? or if
        $30A story-flag? not if
            $30A story-flag-set
            0 0 6 char-sound
            $309 story-flag? not if
                3 $86 0.0 0.0 0.0 0 0 sound
            else
                $40000001 6 23.0 11.0 9.0 0 0 sound
            then
            $40 7 noise
            1 noise-level
        else
            0 2 6 char-sound
            1 6 sound-stop
            $30A story-flag-clear
            0 noise-level
        then
        $309 story-flag? not if
            1 self-scripted
            $5A threat-add
            1 $FF 8 rumble
            0 $43 5 char-sound
            $F04 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            $902 self-anim
            self-wait-anim
            $40000001 6 23.0 11.0 9.0 0 0 sound
            $903 self-anim
            self-wait-anim
            $309 story-flag-set
            0 self-scripted
        else
            $903 self-anim
            self-wait-anim
        then
    else
        $903 self-anim
        self-wait-anim
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-16.act04 ( -- )   \ 003FC4C0
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    fiona-hidden state-flag-clear
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

: castle-1f-16.act03 ( -- )   \ 003FC420
    stalkers-stay state-flag-set
    4 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $CC $8004 5 -4.7 -39.3 90 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 ebit-clear
    0 avoid-prompt
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-1f-16.act04 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                4 ebit? $FE char-here? not and if
                    4 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] castle-1f-16.act04 goto
            then
            0 camera-follow
            fiona-hidden state-flag-clear
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

: castle-1f-16.act05 ( -- )   \ 003FC4F0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $CE -12.0 -37.0 90 $FFFF 5 self-move-to
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

: castle-1f-16.act06 ( -- )   \ 003FC540
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    7 14.0 18.0 120 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    $FE 2 6 char-sound
    1 6 sound-stop
    $30A story-flag-clear
    0 noise-level
    9 pvar-inc
    $FE 3 stalker-mode
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: castle-1f-16.act08 ( -- )   \ 003FC5F0
    self-wait-done
    -145 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: castle-1f-16.act09 ( -- )   \ 003FC600
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $A00 self-anim
    self-frames-reset
    self-wait-16
    3 ebit? not if
        1 message
        wait-message
        3 ebit-set
    else
        2 message
        wait-message
        3 ebit-clear
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-16.enter castle-1f-16 0 room-script!
' castle-1f-16.char-enter castle-1f-16 6 room-script!
' castle-1f-16.phase1 castle-1f-16 1 room-script!
' castle-1f-16.phase2 castle-1f-16 2 room-script!
' castle-1f-16.phase3 castle-1f-16 3 room-script!
' castle-1f-16.act00 castle-1f-16 $00 action-script!
' castle-1f-16.act01 castle-1f-16 $01 action-script!
' castle-1f-16.act02 castle-1f-16 $02 action-script!
' castle-1f-16.act03 castle-1f-16 $03 action-script!
' castle-1f-16.act04 castle-1f-16 $04 action-script!
' castle-1f-16.act05 castle-1f-16 $05 action-script!
' castle-1f-16.act06 castle-1f-16 $06 action-script!
' castle-1f-16.act07 castle-1f-16 $07 action-script!
' castle-1f-16.act08 castle-1f-16 $08 action-script!
' castle-1f-16.act09 castle-1f-16 $09 action-script!
