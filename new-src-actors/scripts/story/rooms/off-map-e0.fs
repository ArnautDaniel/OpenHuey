\ story/rooms/off-map-e0.fs - the event scripts of room off-map-e0 ($E0; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e0
USING: room-names story.words story.shared flag-names story.clock floor-flags fiona.sounds stalker.anims ;

\ ---- this room's own event bits and script variables ----
0 constant sound-started               \ (bit 0) its sound started (then kept up)
1 constant found-her                   \ (bit 1) he has found her hiding: she must flee (the avoid prompt)
2 constant will-find-her               \ (bit 2) he will find her hiding (by chance, as he searches: more likely by the progress variable)
5 constant came-while-hidden           \ (bit 5) the stalker came in while she was hiding
6 constant started                     \ (bit 6) the run begun (the world let go; Hewie played)
7 constant cond-was-off                \ (bit 7) the room's condition was false last frame: its hook waits a frame once it holds
0 constant sound-step-var              \ (var 0) which of its sounds next (0..3)
1 constant sound-frames-var            \ (var 1) frames to the next sound (30)

\ RoomE0_Cmd00
: off-map-e0.cmd00 ( -- )  fiona-who 0 char-to-exit ;   \ (Fiona to exit 0: Character_ChooseExit)
\ RoomE0_Cmd01
: off-map-e0.cmd01 ( -- )  fan-turn ;   \ (as off-map-37's)
\ RoomE0_Cmd02
: off-map-e0.cmd02 ( b0 -- )  $1A $20000040  40.491e 30.0e -10.498e  49.502e 30.0e -19.509e  40.491e 1.5e -10.498e  49.502e 1.5e -19.509e  lit-quad ;   \ (the door's reflection, as castle-2f-3's)
\ RoomE0_Cmd03
: off-map-e0.cmd03 ( b0 -- )  $1B $10000040  0.491e 22.0e 69.502e  9.502e 22.0e 60.491e  0.491e 0.0e 69.502e  9.502e 0.0e 60.491e  lit-quad ;   \ (the window's, as castle-2f-3's)
\ RoomE0_Cmd04
: off-map-e0.cmd04 ( -- )  clock-draw ;   \ (each frame: the clock)
\ RoomE0_Cmd05
: off-map-e0.cmd05 ( -- )  clock-start ;
\ RoomE0_Cond00
: off-map-e0.cond00? ( -- flag )  true ;   \ (the progress object's +0x7C: ProgressBase_Slot7C, always 1)

: off-map-e0.enter ( -- )   \ 004480B0
    room-sounds
    0 part-shown
    0 1 track 0 action
    1 part-hidden
    hewie-controlled? if
        started ebit-set
    then
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 off-map-e0.cmd02
    0 off-map-e0.cmd03
;

: off-map-e0.act03 ( -- )   \ 00448440
    will-find-her ebit-clear
    $30 0 pvar? if
        0 chance? if
            will-find-her ebit-set
        then
    else $30 1 pvar? if
        0 chance? if
            will-find-her ebit-set
        then
    else $30 2 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $30 3 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who 4 action
    else
        $78 1 summoner-wait
    then
    $30 pvar-inc
    exit
;

: off-map-e0.char-enter ( -- )   \ 004480F0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? if
            $80 exit-taken? if
                0 exit-prepare
                off-map-e0.cmd00
                $30 0 pvar-set
                $31 0 pvar-set
                $32 0 pvar-set
                $33 0 pvar-set
                fiona-who char-activate
                fiona-who $55 -40.16 11.18 180 char-to-xz
                fiona-who 0 0 char-camera
                hewie-who char-activate
                $E0 0 276 hewie-to-room
                hewie-who $114 -32.33 16.81 -127 char-to-xz
                0 0 player-camera
                stalker-who char-activate
                $FE $E1 406 2 stalker-to-room
                summoner-own-cooldown
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
                0 hewie-who 5 action
            then
        then
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            off-map-e0.act03
        else
            came-while-hidden ebit-clear
        then
    then
;

: off-map-e0.phase1 ( -- )   \ 00448200
    sound-started ebit? not if
        6 sound-bank-loaded? if
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            sound-started ebit-set
        then
    else
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else sound-step-var 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else sound-step-var 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else sound-step-var 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
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
    hewie-who 0 8 char-zone-bits? if
        fiona-who char-here? if
            fiona-who char-busy? not in-panic? not and fiona-free? and if
                -1 control-action? not if
                    stalker-who char-here? not if
                        0 fiona-who 1 action
                    then
                then
            then
        then
    then
;

: off-map-e0.phase3 ( -- )   \ 00448320
    off-map-e0.cond00? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-e0.cmd04
    then then
;

: off-map-e0.phase5 ( -- )   \ 00448340
    fiona-who char-busy? if
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-clear
        fiona-who action-end
        fiona-who $27 13.59 -38.0 0 char-to-xz
    then
;

: off-map-e0.act00 ( -- )   \ 0047B0B8
    begin
        off-map-e0.cmd01
        yield
    again
;

: off-map-e0.act02 ( -- )   \ 00448410
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A blend-out
    0 self-noclip
    found-her ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e0.act01 ( -- )   \ 00448360
    stalkers-stay state-flag-set
    1 self-scripted
    fiona-who 1 char-file-load
    self-wait-done
    $27 14.32 -41.52 180 $FFFF $A self-move-to
    self-wait-done
    came-while-hidden ebit-clear
    fiona-who char-file-use
    $27 $8004 5 14.5 -37.5 180 self-walk-anim
    self-wait-done
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    5 room-motion $A self-anim-blend8
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    found-her ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] off-map-e0.act02 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                came-while-hidden ebit? stalker-who char-here? not and if
                    came-while-hidden ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] off-map-e0.act02 goto
            then
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            5 wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: off-map-e0.act04 ( -- )   \ 00448490
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    there-anim play-out
    found-her ebit-set
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: off-map-e0.act05 ( -- )   \ 004484D0
    self-wait-done
    camera-restart
    world-held state-flag-clear
    $F 1 fade
    wait-fade
    started ebit-set
    self-idle-or-end
;

: off-map-e0.phase2 ( -- )   \ 0047B0B0
;

\ ---- registered ----
off-map-e0 register-scripts   \ (its phases and actions, by their names)
