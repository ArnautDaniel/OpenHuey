\ story/rooms/off-map-e0.fs - the event scripts of room off-map-e0 ($E0; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e0
USING: room-names story.words story.shared ;

\ RoomE0_Cmd00
: off-map-e0.cmd00 ( -- )  s" off-map-e0.cmd00" stub-step ;
\ RoomE0_Cmd01
: off-map-e0.cmd01 ( -- )  s" off-map-e0.cmd01" stub-step ;
\ RoomE0_Cmd02
: off-map-e0.cmd02 ( b0 -- )  drop s" off-map-e0.cmd02" stub-step ;
\ RoomE0_Cmd03
: off-map-e0.cmd03 ( b0 -- )  drop s" off-map-e0.cmd03" stub-step ;
\ RoomE0_Cmd04
: off-map-e0.cmd04 ( -- )  s" off-map-e0.cmd04" stub-step ;
\ RoomE0_Cmd05
: off-map-e0.cmd05 ( -- )  s" off-map-e0.cmd05" stub-step ;
\ RoomE0_Cond00
: off-map-e0.cond00? ( -- flag )  s" off-map-e0.cond00?" stub-flag ;

: off-map-e0.enter ( -- )   \ 004480B0
    room-sounds
    0 1 $14 door-bits
    0 $F1 0 action
    1 0 $14 door-bits
    hewie-controlled? if
        6 ebit-set
    then
    0 0 var-set
    1 0 var-set
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 off-map-e0.cmd02
    0 off-map-e0.cmd03
;

: off-map-e0.act03 ( -- )   \ 00448440
    2 ebit-clear
    $30 0 pvar? if
        0 chance? if
            2 ebit-set
        then
    else $30 1 pvar? if
        0 chance? if
            2 ebit-set
        then
    else $30 2 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $30 3 pvar? if
        $32 chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 $FE 4 action
    else
        $78 1 item-cooldown
    then
    $30 pvar-inc
    exit
;

: off-map-e0.char-enter ( -- )   \ 004480F0
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
        0 self-is? if
            $80 exit-taken? if
                0 exit-prepare
                off-map-e0.cmd00
                $30 0 pvar-set
                $31 0 pvar-set
                $32 0 pvar-set
                $33 0 pvar-set
                0 char-activate
                0 $55 -40.16 11.18 180 char-to-xz
                0 0 0 char-camera
                1 char-activate
                $E0 0 276 hewie-to-room
                1 $114 -32.33 16.81 -127 char-to-xz
                hewie-controlled? not if
                    0 0 0 char-camera
                    0 camera-follow
                else
                    1 0 0 char-camera
                    1 camera-follow
                then
                $FE char-activate
                $FE $E1 406 2 stalker-to-room
                stalker-item-cooldown
                $2E 1 pvar? if
                    1 music-stage
                else
                    2 music-stage
                then
                2 0 0 music
                4 0 0 music
                off-map-e0.cmd05
                $11D door-open-clear
                $11E door-open-clear
                $11F door-open-clear
                $120 door-open-clear
                $121 door-open-clear
                $123 door-open-clear
                $124 door-open-clear
                $125 door-open-clear
                $126 door-open-clear
                $127 door-open-clear
                $128 door-open-clear
                $129 door-open-clear
                $12A door-open-clear
                $126 door-lock
                $127 door-lock
                $128 door-lock
                $129 door-lock
                $12A door-lock
                doors-room-in
                0 1 5 action
            then
        then
    then
    $FE self-is? if
        9 state-flag? if
            5 ebit-set
            off-map-e0.act03
        else
            5 ebit-clear
        then
    then
;

: off-map-e0.phase1 ( -- )   \ 00448200
    0 ebit? not if
        6 sound-bank-loaded? if
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            0 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
    0 11.0 0.0 -45.0 $A 5 0 zone
    1 0 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 1 action
                    then
                then
            then
        then
    then
;

: off-map-e0.phase3 ( -- )   \ 00448320
    off-map-e0.cond00? not if
        7 ebit-set
    else 7 ebit? if
        7 ebit-clear
    else
        off-map-e0.cmd04
    then then
;

: off-map-e0.phase5 ( -- )   \ 00448340
    0 char-busy? if
        $18 state-flag-clear
        9 state-flag-clear
        0 action-end
        0 $27 13.59 -38.0 0 char-to-xz
    then
;

: off-map-e0.act00 ( -- )   \ 0047B0B8
    begin
        off-map-e0.cmd01
        yield
    again
;

: off-map-e0.act02 ( -- )   \ 00448410
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    1 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e0.act01 ( -- )   \ 00448360
    $18 state-flag-set
    1 self-scripted
    0 1 char-file-load
    self-wait-done
    $27 14.32 -41.52 180 $FFFF $A self-move-to
    self-wait-done
    5 ebit-clear
    0 char-file-use
    $27 $8004 5 14.5 -37.5 180 self-walk-anim
    self-wait-done
    1 self-noclip
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    1 ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            $FF panic-stage? 1 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] off-map-e0.act02 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? $FE char-here? not and if
                    5 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] off-map-e0.act02 goto
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

: off-map-e0.act04 ( -- )   \ 00448490
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    1 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: off-map-e0.act05 ( -- )   \ 004484D0
    self-wait-done
    camera-restart
    8 state-flag-clear
    $F 1 fade
    wait-fade
    6 ebit-set
    self-idle-or-end
;

: off-map-e0.phase2 ( -- )   \ 0047B0B0
;

\ ---- registered ----
' off-map-e0.enter off-map-e0 0 room-script!
' off-map-e0.char-enter off-map-e0 6 room-script!
' off-map-e0.phase1 off-map-e0 1 room-script!
' off-map-e0.phase3 off-map-e0 3 room-script!
' off-map-e0.phase5 off-map-e0 5 room-script!
' off-map-e0.act00 off-map-e0 $00 action-script!
' off-map-e0.act01 off-map-e0 $01 action-script!
' off-map-e0.act02 off-map-e0 $02 action-script!
' off-map-e0.act03 off-map-e0 $03 action-script!
' off-map-e0.act04 off-map-e0 $04 action-script!
' off-map-e0.act05 off-map-e0 $05 action-script!
' off-map-e0.phase2 off-map-e0 2 room-script!
