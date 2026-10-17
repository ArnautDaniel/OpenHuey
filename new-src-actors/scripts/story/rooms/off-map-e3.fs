\ story/rooms/off-map-e3.fs - the event scripts of room off-map-e3 ($E3; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e3
USING: room-names story.words story.shared flag-names story.clock ;

\ room 0xE3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e3.cmd00 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e3.cond00? ( -- flag )  true ;

: off-map-e3.enter ( -- )   \ 00448D20
    0 -35.8 -50.5 -27.25 0 flame-in
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 0 -19.5 -26.0 -31.0 4.0 4.0 100.0 0.0 10.0 wanderer
    1 0 -53.0 -22.0 -11.0 4.0 5.0 80.0 0.0 30.0 wanderer
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
    stalker-who char-unseen? not if
        5 ebit-set
    then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
        $B ebit-clear
    then
    $31 pvar-inc
    exit
;

: off-map-e3.char-enter ( -- )   \ 00448DA0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            5 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        5 -1 player-camera
    then then
    0 5 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            2 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        2 -1 player-camera
    then then
    1 2 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
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
    hewie-who 2 char-entered-area? if
        0 exit-prepare
    then
    hewie-who 3 char-entered-area? if
        1 exit-prepare
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    stalker-who 2 char-cond? not if
        fiona-hidden state-flag? 6 ebit? not and stalker-who char-here? and if
            $B ebit-set
            off-map-e3.act02
        then
    then
    1 -27.83 -60.0 -33.59 $A 5 0 zone
    hewie-who 1 8 char-zone-bits? if
        fiona-who char-here? if
            fiona-who char-busy? not in-panic? not and if
                -1 control-action? not if
                    stalker-who char-here? not if
                        3 ebit-clear
                        0 fiona-who 0 action
                    then
                then
            then
        then
    then
    2 -44.45 -60.0 -32.32 $A 5 0 zone
    hewie-who 2 8 char-zone-bits? if
        fiona-who char-here? if
            fiona-who char-busy? not in-panic? not and fiona-free? and if
                -1 control-action? not if
                    stalker-who char-here? not if
                        3 ebit-set
                        0 fiona-who 0 action
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
    fiona-who char-busy? if
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-clear
        fiona-who action-end
        fiona-who $161 -26.17 -25.52 90 char-to-xz
    then
;

: off-map-e3.act01 ( -- )   \ 00449110
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    stalker-who char-busy? if
        4 ebit? not if
            stalker-who action-end
            stalker-who 0 stalker-command
        then
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e3.act00 ( -- )   \ 00449030
    stalkers-stay state-flag-set
    1 self-scripted
    fiona-who 0 char-file-load
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
    fiona-who char-file-use
    3 ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    4 ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            in-panic? 4 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] off-map-e3.act01 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                8 ebit? stalker-who char-here? not and if
                    8 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] off-map-e3.act01 goto
            then
            fiona-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
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
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    else
        stalker-who 0 char-heading-for? 0 exit-door-open? not and stalker-who 0 char-in-area? and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and stalker-who 1 char-in-area? and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    then
    $B ebit-clear
    $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    4 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
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
