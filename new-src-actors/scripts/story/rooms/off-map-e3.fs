\ story/rooms/off-map-e3.fs - the event scripts of room off-map-e3 ($E3; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e3
USING: room-names story.words story.shared ;

\ room 0xE3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e3.cmd00 ( -- )  s" off-map-e3.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e3.cond00? ( -- flag )  s" off-map-e3.cond00?" stub-flag ;

: off-map-e3.enter ( -- )   \ 00448D20
    0 -35.8 -50.5 -27.25 0 effect-86
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 0 -19.5 -26.0 -31.0 4.0 4.0 100.0 0.0 10.0 scene-effect-71000
    1 0 -53.0 -22.0 -11.0 4.0 5.0 80.0 0.0 30.0 scene-effect-71000
    3 0 $14 door-bits
    4 0 $14 door-bits
    0 2 $20000 nav-group
    1 $2300 sound-volume
;

: off-map-e3.act02 ( -- )   \ 00449140
    6 ebit-set
    8 ebit-set
    5 ebit-clear
    $31 0 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $31 1 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $31 2 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $31 3 pvar? if
        $64 chance? if
            5 ebit-set
        then
    then then then then
    $FE char-unseen? not if
        5 ebit-set
    then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 $FE 3 action
    else
        $78 1 item-cooldown
        $B ebit-clear
    then
    $31 pvar-inc
    exit
;

: off-map-e3.char-enter ( -- )   \ 00448DA0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 5 -1 char-camera
                0 camera-follow
            else
                1 5 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then then
    0 5 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    1 2 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            off-map-e3.act02
        else
            8 ebit-clear
        then
    then
;

: off-map-e3.phase1 ( -- )   \ 00448E30
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 1 0 1 chars-area-camera
    3 2 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 3 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 3 1 1 chars-area-camera
    $10 1 0 1 chars-area-camera
    $11 5 -1 1 chars-area-camera
    1 2 char-entered-area? if
        0 exit-prepare
    then
    1 3 char-entered-area? if
        1 exit-prepare
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    $FE 2 char-C4? not if
        9 state-flag? 6 ebit? not and $FE char-here? and if
            $B ebit-set
            off-map-e3.act02
        then
    then
    1 -27.83 -60.0 -33.59 $A 5 0 zone
    1 1 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and if
                -1 control-action? not if
                    $FE char-here? not if
                        3 ebit-clear
                        0 0 0 action
                    then
                then
            then
        then
    then
    2 -44.45 -60.0 -32.32 $A 5 0 zone
    1 2 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        3 ebit-set
                        0 0 0 action
                    then
                then
            then
        then
    then
;

: off-map-e3.phase3 ( -- )   \ 00448F30
    off-map-e3.cond00? not if
        $C ebit-set
    else $C ebit? if
        $C ebit-clear
    else
        off-map-e3.cmd00
    then then
    -19.0 8.0 0.0 13.0 8.0 0.0 -19.0 0.0 0.0 13.0 0.0 0.0 lights-doorway
    -18.1 13.0 -2.7 -16.2 13.0 -2.7 -18.1 0.0 -2.7 -16.2 0.0 -2.7 lights-doorway
    -18.1 13.0 -1.0 -18.1 13.0 -2.7 -18.1 0.0 -1.0 -18.1 0.0 -2.7 lights-doorway
    -19.0 8.0 15.0 -19.0 8.0 0.0 -19.0 0.0 15.0 -19.0 0.0 0.0 lights-doorway
;

: off-map-e3.phase5 ( -- )   \ 00449010
    0 char-busy? if
        $18 state-flag-clear
        9 state-flag-clear
        0 action-end
        0 $161 -26.17 -25.52 90 char-to-xz
    then
;

: off-map-e3.act01 ( -- )   \ 00449110
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    $FE char-busy? if
        4 ebit? not if
            $FE action-end
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e3.act00 ( -- )   \ 00449030
    $18 state-flag-set
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    3 ebit? not if
        $14A -24.3 -32.94 -90 $FFFF $A self-move-to
        self-wait-done
    else
        $1FA -48.07 -31.71 90 $FFFF $A self-move-to
        self-wait-done
    then
    8 ebit-clear
    6 ebit-clear
    0 char-file-use
    3 ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    4 ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] off-map-e3.act01 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                8 ebit? $FE char-here? not and if
                    8 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] off-map-e3.act01 goto
            then
            9 state-flag-clear
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

: off-map-e3.act03 ( -- )   \ 004491A0
    self-wait-done
    $B ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and if
            1 3 self-move-slot
            self-wait-done
        then
    else
        $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and $FE 1 char-in-area? and if
            1 3 self-move-slot
            self-wait-done
        then
    then
    $B ebit-clear
    $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    4 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: off-map-e3.phase2 ( -- )   \ 0047B0E0
;

\ ---- registered ----
' off-map-e3.enter off-map-e3 0 room-script!
' off-map-e3.char-enter off-map-e3 6 room-script!
' off-map-e3.phase1 off-map-e3 1 room-script!
' off-map-e3.phase3 off-map-e3 3 room-script!
' off-map-e3.phase5 off-map-e3 5 room-script!
' off-map-e3.act00 off-map-e3 $00 action-script!
' off-map-e3.act01 off-map-e3 $01 action-script!
' off-map-e3.act02 off-map-e3 $02 action-script!
' off-map-e3.act03 off-map-e3 $03 action-script!
' off-map-e3.phase2 off-map-e3 2 room-script!
