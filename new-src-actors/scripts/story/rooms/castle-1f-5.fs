\ story/rooms/castle-1f-5.fs - the event scripts of room castle-1f-5 ($10; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-5
USING: room-names story.words story.shared ;

\ room 0x10 (Room10_Cond00_ptmf): the stalker is there, not about, in mode 2, 6 or 7
: castle-1f-5.cond00? ( -- flag )  s" castle-1f-5.cond00?" stub-flag ;

: castle-1f-5.enter ( -- )   \ 003F6F80
    room-sounds
    $1B story-flag? $308 story-flag? not and if
        1 exit-taken? if
            castle-1f-5.cond00? if
                $308 story-flag-set
                7 state-flag-set
                $C door-open-clear
                doors-room-in
                0 ebit-set
                $13 state-flag-set
                $FE char-activate
                $FE $10 161 2 stalker-to-room
                $FE -1 -1 char-camera
                $FE char-full-health
                0 $FE 2 action
            then
        then
    then
    $200 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $28B story-flag? not if
            3 41.0 1.0 -19.0 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 6.0 21.8 -3.2 0 effect-86
    1 29.0 41.8 41.9 0 effect-86
    2 -46.8 93.8 18.5 0 effect-86
    5 ebit-clear
    $39 story-flag? $41 story-flag? not and if
        1 action-end
        1 char-done
    then
    1 $2300 sound-volume
;

: castle-1f-5.char-enter ( -- )   \ 003F7060
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    0 3 -1 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 3 char-camera
                0 camera-follow
            else
                1 4 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 3 char-camera
            0 camera-follow
        else
            1 4 3 char-camera
            1 camera-follow
        then
    then then
    2 4 3 area-camera
    0 self-is? if
        1 exit-taken? if
            2 ebit-set
        then
        $39 story-flag? $41 story-flag? not and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 5 action
        then
        0 exit-taken? 2 exit-taken? or if
            3 map-page
        then
    then
    $FE self-is? if
        6 ebit-clear
    then
;

: castle-1f-5.phase1 ( -- )   \ 003F7150
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $D 0 0 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 4 3 1 chars-area-camera
    $17 3 -1 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-left-area? 0 5 char-entered-area? or if
        2 exit-prepare
    then
    0 6 char-left-area? if
        1 exit-prepare
    then
    0 $12 char-entered-area? if
        2 ebit-set
    then
    0 $12 char-left-area? if
        2 ebit-clear
    then
    4 stalker-alert? not 6 ebit? not and $FE 2 char-C4? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 stalker-alert? 2 stalker-alert? or if
                $FE $11 char-in-area? 2 ebit? and if
                    0 $FE 4 action
                    6 ebit-set
                then
            then
        then
    then
    $41 story-flag? $42 story-flag? not and if
        1 2 pad? 2 2 pad? or 3 2 pad? or 0 control-action? or 1 control-action? or 2 control-action? or 3 control-action? or 4 control-action? or fiona-free? and if
            0 0 6 action
        then
        99.0 fiona-fear
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
    0 5 char-entered-area? if
        3 map-page
    then
    0 5 char-left-area? if
        2 map-page
    then
    6 sound-bank-loaded? if
        0 $80000000 6 char-sound
    then
;

: castle-1f-5.phase2 ( -- )   \ 003F72D0
    0 $F char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $10 $2D char-faces-area? if
        5 1 0 scene-change
    then
    0 $13 char-in-area? 0 45 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $14 char-in-area? 0 45 $32 char-heading? and if
        5 8 0 scene-change
    then
    $41 story-flag? $42 story-flag? not and if
        -2147483646 scene-request? 2 scene-request? or if
            5 6 1 scene-change
        then
    then
    $200 story-flag? not if
        0 41.0 0.0 -19.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $200 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            41.0 0.0 -19.0 0 -2146430960 0 0.0 scene-effect-8C
            $88 5 41.0 0.0 -19.0 0 0 sound
            $40 $232 noise
            3 41.0 1.0 -19.0 flicker-sprite
        then
    else $28B story-flag? not if
        0 3 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then then
;

: castle-1f-5.phase3 ( -- )   \ 003F73B0
    -8.0 82.0 -5.0 -8.0 82.0 45.0 -8.0 30.0 -5.0 -8.0 30.0 45.0 lights-doorway
    -5.3 54.0 -4.1 -5.3 54.0 47.0 -58.0 54.0 -4.1 -58.0 54.0 47.0 lights-doorway
    15.0 30.0 -3.0 45.0 30.0 -3.0 15.0 15.0 -3.0 45.0 15.0 -3.0 lights-doorway
    15.0 15.0 -3.0 45.0 15.0 -3.0 15.0 0.0 -3.0 45.0 0.0 -3.0 lights-doorway
    -11.0 30.0 -3.0 15.0 30.0 -3.0 -11.0 15.0 -3.0 15.0 15.0 -3.0 lights-doorway
    -11.0 15.0 -3.0 15.0 15.0 -3.0 -11.0 0.0 -3.0 15.0 0.0 -3.0 lights-doorway
;

: castle-1f-5.act03 ( -- )   \ 003F7560
    $13 state-flag-clear
    7 state-flag-clear
    0 ebit-clear
    $FE action-end
    $FE char-done
    exit
;

: castle-1f-5.phase5 ( -- )   \ 003F74E0
    0 ebit? if
        castle-1f-5.act03
    then
    5 ebit? $FE char-busy? and if
        $FE action-end
        $FE $196 0 char-to-tri-facing
        $FE 0 char-no-shadow
        5 ebit-clear
    then
;

: castle-1f-5.act00 ( -- )   \ 003F7500
    self-wait-done
    -177 self-turn-angle
    self-wait-done
    $A02 self-anim
    self-wait-anim
    4 message
    wait-message
    self-idle-or-end
;

: castle-1f-5.act01 ( -- )   \ 003F7510
    self-wait-done
    1 ebit? not if
        40.0 -11.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        2 message
        wait-message
        1 ebit-set
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-5.act02 ( -- )   \ 003F7530
    1 self-scripted
    self-wait-done
    1 $FF 8 rumble
    $FE $A1 -90 char-to-tri-facing
    0 exit-door-open? not if
        $1B4 $FFFF $B self-move-tri
        self-wait-done
        0 3 self-move-slot
        self-wait-done
    then
    $3C $FFFF $B self-move-tri
    self-wait-done
    0 self-scripted
    castle-1f-5.act03
    self-idle-or-end
;

: castle-1f-5.act04 ( -- )   \ 003F7570
    self-wait-done
    $FE 0 char-file-load
    $8A 31.949 -18.333 0 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    5 ebit-set
    $8000 5 self-anim-9
    0 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        0 60 var? if
            0 29.0 19.0 25.0 0 0 0 0 dust
            0 36.0 19.0 25.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        0 var-inc
        0 camera-mode? not if
            $FE 1 char-no-shadow
        else
            $FE 0 char-no-shadow
        then
        self-at-motion-event? not while
        yield
    repeat
    0 self-noclip
    0 self-scripted
    $FE 0 char-no-shadow
    5 ebit-clear
    $FE 2 stalker-mode
    self-idle-or-end
;

: castle-1f-5.act05 ( -- )   \ 003F7600
    1 action-end
    1 char-done
    self-wait-done
    3 partner-load
    2 char-unload
    $FE $E 1 room-doors-state
    $FE $F 1 room-doors-state
    $FE $25 1 room-doors-state
    $FE $60 1 room-doors-state
    $FE $63 1 room-doors-state
    $FE $4C 1 room-doors-state
    $FE $66 1 room-doors-state
    $FE $61 1 room-doors-state
    $FE $53 1 room-doors-state
    $FE $54 1 room-doors-state
    $FE $6C 1 door-lock-for
    1 $53 1 room-doors-state
    1 $5F 1 room-doors-state
    $FE $5F 1 room-doors-state
    1 door-reopen-unlock
    $D door-reopen-unlock
    $58 door-close-off-lock
    $5A door-close-off-lock
    $4D door-close-off-lock
    $4E door-close-off-lock
    $50 door-close-off-lock
    $53 door-close-off-lock
    $5C door-close-off-lock
    $79 door-close-off-lock
    exits-rebuild
    camera-restart
    2 door-lock
    $34 door-lock
    $62 door-lock
    $44 door-lock
    $47 door-lock
    $4F door-lock
    $56 door-lock
    $67 door-lock
    $7D door-lock
    $51 door-lock
    $52 door-lock
    $64 door-lock
    $6C door-lock
    $74 door-lock
    $78 door-lock
    $5E door-lock
    $76 door-lock
    $7B door-lock
    $7C door-lock
    $63 door-lock
    $54 door-lock
    1 door-open-clear
    1 door-lock
    3 door-open-clear
    3 door-lock
    $C door-open-clear
    $C door-lock
    $D door-unlock
    $D door-open-set
    doors-room-in
    $14 state-flag-set
    $1E state-flag-set
    $D state-flag-set
    $12 state-flag-clear
    8 state-flag-set
    $41 story-flag-set
    5 state-flag-set
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    effects-arena-flip
    0 char-in
    begin
        yield
        4 sound-bank-loaded? until
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    music-stage-end
    1 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    1 char-activate
    $10 0 155 hewie-to-room
    1 $9B 0 char-to-tri-facing
    $28 state-flag-clear
    $F 1 fade
    wait-fade
    self-idle-or-end
;

: castle-1f-5.act06 ( -- )   \ 003F77A0
    self-wait-done
    $100E self-anim
    0 0 6 char-sound
    self-frames-reset
    $14 self-wait-frames
    3 ebit? not if
        5 message
        3 ebit-set
    else
        6 message
        3 ebit-clear
    then
    2 $A self-anim-blend
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-5.act07 ( -- )   \ 003F77D0
    self-wait-done
    $17B 94.0 -27.5 90 $FFFF 5 self-move-to
    self-wait-done
    1 15.0 0.0 0.0 8.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 ebit? not if
        7 message
        wait-message
        4 ebit-set
    else
        8 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-5.act08 ( -- )   \ 003F7820
    self-wait-done
    $19A 94.0 27.5 90 $FFFF 5 self-move-to
    self-wait-done
    1 15.0 0.0 0.0 8.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-5.act09 ( -- )   \ 003F7870
    self-wait-done
    41.0 -19.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message
            wait-message
        else
            $28B story-flag-set
            3 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
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
' castle-1f-5.enter castle-1f-5 0 room-script!
' castle-1f-5.char-enter castle-1f-5 6 room-script!
' castle-1f-5.phase1 castle-1f-5 1 room-script!
' castle-1f-5.phase2 castle-1f-5 2 room-script!
' castle-1f-5.phase3 castle-1f-5 3 room-script!
' castle-1f-5.phase5 castle-1f-5 5 room-script!
' castle-1f-5.act00 castle-1f-5 $00 action-script!
' castle-1f-5.act01 castle-1f-5 $01 action-script!
' castle-1f-5.act02 castle-1f-5 $02 action-script!
' castle-1f-5.act03 castle-1f-5 $03 action-script!
' castle-1f-5.act04 castle-1f-5 $04 action-script!
' castle-1f-5.act05 castle-1f-5 $05 action-script!
' castle-1f-5.act06 castle-1f-5 $06 action-script!
' castle-1f-5.act07 castle-1f-5 $07 action-script!
' castle-1f-5.act08 castle-1f-5 $08 action-script!
' castle-1f-5.act09 castle-1f-5 $09 action-script!
