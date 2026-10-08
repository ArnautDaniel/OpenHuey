\ story/rooms/front-garden-5.fs - the event scripts of room front-garden-5 ($2D; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-5
USING: room-names story.words story.shared flag-names ;

\ a hanging thing (the room's +0x34 (byte 3 + 2) object) swinging, by byte 4: 0 / 2 set going
\ (12 degrees) away from the partner / Fiona; 1 a step (22.5 degrees of its swing, shrinking to
\ 0.4 at each end; under half a degree it stops) - 2 while it swings
: front-garden-5.cmd00 ( b0 b1 -- )  drop drop s" front-garden-5.cmd00" stub-step ;
\ something dropped (effect LoopingSprite_vtable, its slot in event var 1) from (-276.5, 3,
\ 160), by byte 3: 0 started (event var 0 the frame count); 1 a frame (2 while falling): it
\ drifts 0.5 a frame in x and falls 0.05 x n(n+1)/2, gone below -10
: front-garden-5.cmd01 ( b0 -- )  drop s" front-garden-5.cmd01" stub-step ;
\ room 0x2D: two hanging things (the room's objects 4 and 5) that Fiona pushes as she walks by:
\ past a step total of 5 they swing for 20 frames and the first toggles event flag 3 with a
\ sound (hangers_swing).
: front-garden-5.cmd02 ( b0 -- )  drop s" front-garden-5.cmd02" stub-step ;
\ Hewie (in state 0x7F, +0xF3564) within 5 of the spot by byte 3: 0 (-259.5, 190), 1 (-276, 160)
\ (others: what the caller left)
: front-garden-5.cond00? ( b0 -- flag )  drop s" front-garden-5.cond00?" stub-flag ;

: front-garden-5.enter ( -- )   \ 00405AC0
    room-sounds
    $16 1.0 0 bgm
    0 1 object-show
    1 1 object-show
    2 1 $14 door-bits
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    then
    $212 story-flag? not if
        1 2 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $284 story-flag? not if
            2 -14.0 -9.0 97.0 flicker-sprite
        then
    then
    0 5 0.562 0.5 0.25 0.5 zone-rect
    $23A story-flag? if
        $23B story-flag? not if
            0 -265.0 -9.0 160.0 flicker-sprite
        then
    then
    $23E story-flag? $23F story-flag? not and if
        1 -228.4 -9.0 103.2 flicker-sprite
    then
    0 front-garden-5.cmd02
    $2F5 story-flag? not if
        3 -180.0 5.61 176.0 flicker-sprite
    then
;

: front-garden-5.act09 ( -- )   \ 00406180
    $FE camera-follow
    1 ebit-clear
    $A 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $A 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $A 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $A 3 pvar? if
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
        0 $FE $A action
    else
        $78 1 item-cooldown
    then
    $A pvar-inc
    exit
;

: front-garden-5.char-enter ( -- )   \ 00405B80
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
    $FE self-is? if
        fiona-hidden state-flag? if
            2 ebit-set
            front-garden-5.act09
        else
            2 ebit-clear
        then
    then
;

: front-garden-5.phase1 ( -- )   \ 00405BD0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 1 1 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    0 game-mode? if
        1 char-here? 1 2 char-C4? not and if
            0 char-unseen? not 1 char-unseen? not and if
                0 -130 115 $32 char-faces-xz? -130 -10 115 point-on-camera? not and if
                    1 3 char-in-area? if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 0 action
                            then
                        else 44 fiona-started? if
                            0 3 char-in-area? not if
                                hewie-stays? if
                                    0 1 0 action
                                then
                            then
                        then then
                    else 1 3 char-in-area? not if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 5 action
                            then
                        else 44 fiona-started? if
                            0 3 char-in-area? if
                                hewie-stays? if
                                    0 1 5 action
                                then
                            then
                        then then
                    then then
                then
                0 -180 180 $32 char-faces-xz? -180 5 180 point-on-camera? not and if
                    1 4 char-in-area? not if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 1 action
                            then
                        else 44 fiona-started? if
                            0 4 char-in-area? if
                                hewie-stays? if
                                    0 1 1 action
                                then
                            then
                        then then
                    else 1 4 char-in-area? if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 2 action
                            then
                        else 44 fiona-started? if
                            0 4 char-in-area? not if
                                hewie-stays? if
                                    0 1 2 action
                                then
                            then
                        then then
                    then then
                then
                0 -276 160 $1E char-faces-xz? -276 -10 160 point-on-camera? not and if
                    35 fiona-started? if
                        hewie-stays? if
                            0 1 3 action
                        then
                    then
                then
                0 -260 190 $1E char-faces-xz? -260 -10 190 point-on-camera? not and if
                    35 fiona-started? if
                        hewie-stays? if
                            0 1 4 action
                        then
                    then
                then
            then
        then
    then
    5 -276.0 -10.0 160.0 5 15 0 zone
    4 -259.5 -10.0 190.0 5 15 0 zone
    0 4 char-in-zone? if
        0 $F1 $D action
    then
    0 5 char-in-zone? if
        0 $F1 $E action
    then
    $23E story-flag? not if
        3 -228.4 -10.0 103.2 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 574 var-set
                $1A 575 var-set
                $1B 1 var-set
                $1C -228400 var-set
                $1D -9000 var-set
                $1E 103200 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    2 -97.0 -4.0 82.0 $E 20 0 zone
    0 2 8 char-zone-bits? if
        1 front-garden-5.cmd02
    then
    6 -275.96 -10.0 159.38 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    7 -259.2 -10.0 189.64 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 7 9 char-zone-bits? if
                $1F 7 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
;

: front-garden-5.phase2 ( -- )   \ 00405E30
    0 story-flag? if
        0 9 char-in-area? if
            $FE char-here? not if
                5 7 5 scene-change
            else
                $8016 scene-ending
            then
        then
    then
    $23A story-flag? if
        $23B story-flag? not if
            1 0 5 5 0 zone-at-effect
            0 1 3 char-zone-bits? if
                5 $10 4 scene-change
            then
        then
    then
    $23E story-flag? $23F story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $11 4 scene-change
        then
    then
    0 $E char-in-area? 0 90 $3C char-heading? and if
        5 $12 0 scene-change
    then
    $212 story-flag? not if
        0 -14.0 -10.0 97.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $212 story-flag-set
            0 2 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -14.0 -10.0 97.0 0 -2143272896 0 0.0 scene-effect-8C
            3 6 -14.0 -10.0 97.0 0 0 sound
            $40 $10A noise
            2 -14.0 -9.0 97.0 flicker-sprite
        then
    else $284 story-flag? not if
        0 2 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $13 4 scene-change
        then
    then then
    $2F5 story-flag? not if
        8 3 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 $14 4 scene-change
        then
    then
;

: front-garden-5.act00 ( -- )   \ 00405F30
    self-wait-done
    hewie-bark
    self-wait-done
    $14C -90.0 115.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 0 8 nav-group
    1 0 $30 nav-group
    $155 -130.0 115.0 1.5 35 hewie-go-to
    self-wait-done
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: front-garden-5.act01 ( -- )   \ 00405F80
    self-wait-done
    hewie-bark
    self-wait-done
    0 1 8 nav-group
    $63 -130.0 180.0 -90 $FFFF $A self-move-to
    self-wait-done
    $137 $FFFF $B self-move-tri
    self-wait-done
    0 1 $30 nav-group
    self-idle-or-end
;

: front-garden-5.act02 ( -- )   \ 00405FB0
    self-wait-done
    hewie-bark
    self-wait-done
    0 1 8 nav-group
    $39 -230.0 180.0 90 $FFFF $A self-move-to
    self-wait-done
    $1B9 $FFFF $B self-move-tri
    self-wait-done
    0 1 $30 nav-group
    self-idle-or-end
;

: front-garden-5.act03 ( -- )   \ 00405FE0
    self-wait-done
    hewie-bark
    self-wait-done
    0 $F1 $B action
    $1C -236.0 160.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    $193 -276.0 160.0 1.0 35 hewie-go-to
    self-wait-done
    0 self-scripted
    self-idle-or-end
;

: front-garden-5.act04 ( -- )   \ 00406010
    self-wait-done
    hewie-bark
    self-wait-done
    0 $F1 $C action
    $161 -220.0 190.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    $8A -259.5 190.1 1.0 35 hewie-go-to
    self-wait-done
    0 self-scripted
    self-idle-or-end
;

: front-garden-5.act05 ( -- )   \ 00406040
    self-wait-done
    hewie-bark
    self-wait-done
    $4E -170.0 115.0 90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 0 8 nav-group
    1 0 $30 nav-group
    $155 -130.0 115.0 1.5 35 hewie-go-to
    self-wait-done
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: front-garden-5.act06 ( -- )   \ 0047AB38
    self-idle-or-end
;

: front-garden-5.act08 ( -- )   \ 00406150
    0 ebit? if
        2 avoid-prompt
    then
    fiona-hidden state-flag-clear
    0 camera-follow
    stalkers-stay state-flag-set
    $C3 -67.3 79.93 89 $207 5 self-move-to
    self-wait-done
    4 $14 self-anim-blend
    self-wait-anim
    0 self-scripted
    0 self-noclip
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: front-garden-5.act07 ( -- )   \ 00406090
    stalkers-stay state-flag-set
    2 ebit-clear
    1 self-scripted
    0 ebit-clear
    self-wait-done
    $E5 -74.756 82.505 -90 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    $800 self-anim
    self-wait-anim
    $D6 -100.488 79.931 -90 $803 5 self-move-to
    self-wait-done
    $D6 -100.488 79.931 90 $803 5 self-move-to
    self-wait-done
    $801 self-anim
    fiona-hidden state-flag-set
    stalkers-stay state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] front-garden-5.act08 goto
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
            fiona-hidden state-flag-clear
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] front-garden-5.act08 goto
            then
            stalkers-stay state-flag-set
            $E5 -74.756 82.505 90 $803 5 self-move-to
            self-wait-done
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            self-idle-or-end
        then
    again
;

: front-garden-5.act0A ( -- )   \ 004061D0
    self-wait-done
    $1E6 -45.859 82.656 -90 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: front-garden-5.act0B ( -- )   \ 00406200
    begin
        1 front-garden-5.cond00? not while
        yield
    repeat
    1 $6C 5 char-sound
    $23A story-flag? not if
        0 $F2 $F action
    then
    1 0 front-garden-5.cmd00
    1 1 front-garden-5.cmd00
    self-idle-or-end
;

: front-garden-5.act0C ( -- )   \ 00406230
    begin
        0 front-garden-5.cond00? not while
        yield
    repeat
    1 $6C 5 char-sound
    0 0 front-garden-5.cmd00
    0 1 front-garden-5.cmd00
    self-idle-or-end
;

: front-garden-5.act0D ( -- )   \ 00406250
    0 $8F 5 char-sound
    0 2 front-garden-5.cmd00
    0 1 front-garden-5.cmd00
    self-idle-or-end
;

: front-garden-5.act0E ( -- )   \ 00406270
    0 $8F 5 char-sound
    1 2 front-garden-5.cmd00
    1 1 front-garden-5.cmd00
    self-idle-or-end
;

: front-garden-5.act0F ( -- )   \ 00406290
    0 front-garden-5.cmd01
    1 front-garden-5.cmd01
    $23A story-flag-set
    $23B story-flag? not if
        0 -265.0 -9.0 160.0 flicker-sprite
    then
    self-idle-or-end
;

: front-garden-5.act10 ( -- )   \ 004062B0
    self-wait-done
    -265.0 160.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $23B story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: front-garden-5.act11 ( -- )   \ 00406310
    self-wait-done
    -228.4 103.2 self-turn-to-xz
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
            $23F story-flag-set
            1 effect-remove
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

: front-garden-5.act12 ( -- )   \ 00406370
    self-wait-done
    $41 -190.101 72.001 180 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $1200 self-anim
    self-wait-anim
    0 0 6 char-sound
    $1203 self-anim
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 message
    wait-message
    self-idle-or-end
;

: front-garden-5.act13 ( -- )   \ 004063B0
    self-wait-done
    -14.0 97.0 self-turn-to-xz
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
            $284 story-flag-set
            2 effect-remove
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

: front-garden-5.act14 ( -- )   \ 00406410
    self-wait-done
    -180.0 176.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $90 message-param-room
        $90 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F5 story-flag-set
            3 effect-remove
            $90 1 item-give-count
            0 $90 item-tab
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
' front-garden-5.enter front-garden-5 0 room-script!
' front-garden-5.char-enter front-garden-5 6 room-script!
' front-garden-5.phase1 front-garden-5 1 room-script!
' front-garden-5.phase2 front-garden-5 2 room-script!
' front-garden-5.act00 front-garden-5 $00 action-script!
' front-garden-5.act01 front-garden-5 $01 action-script!
' front-garden-5.act02 front-garden-5 $02 action-script!
' front-garden-5.act03 front-garden-5 $03 action-script!
' front-garden-5.act04 front-garden-5 $04 action-script!
' front-garden-5.act05 front-garden-5 $05 action-script!
' front-garden-5.act06 front-garden-5 $06 action-script!
' front-garden-5.act07 front-garden-5 $07 action-script!
' front-garden-5.act08 front-garden-5 $08 action-script!
' front-garden-5.act09 front-garden-5 $09 action-script!
' front-garden-5.act0A front-garden-5 $0A action-script!
' front-garden-5.act0B front-garden-5 $0B action-script!
' front-garden-5.act0C front-garden-5 $0C action-script!
' front-garden-5.act0D front-garden-5 $0D action-script!
' front-garden-5.act0E front-garden-5 $0E action-script!
' front-garden-5.act0F front-garden-5 $0F action-script!
' front-garden-5.act10 front-garden-5 $10 action-script!
' front-garden-5.act11 front-garden-5 $11 action-script!
' front-garden-5.act12 front-garden-5 $12 action-script!
' front-garden-5.act13 front-garden-5 $13 action-script!
' front-garden-5.act14 front-garden-5 $14 action-script!
