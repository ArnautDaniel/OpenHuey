\ story/rooms/off-map-e5.fs - the event scripts of room off-map-e5 ($E5; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e5
USING: room-names story.words story.shared flag-names story.clock fiona.sounds stalker.anims ;

\ (as Room14_Cmd00) the same for the room object D_0047B0FC
: off-map-e5.cmd00 ( b0 -- )  s" Cartain" curtain ;   \ (as castle-b1-2's)
\ room 0xE5: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e5.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e5.cond00? ( -- flag )  true ;

: off-map-e5.enter ( -- )   \ 00449570
    room-sounds
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 off-map-e5.cmd00
    1 $3FFF sound-volume
;

: off-map-e5.act02 ( -- )   \ 004497A0
    1 ebit-clear
    $32 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $32 1 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $32 2 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $32 3 pvar? if
        $19 chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
    then
    $32 pvar-inc
    exit
;

: off-map-e5.char-enter ( -- )   \ 00449590
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            3 ebit-set
            off-map-e5.act02
        else
            3 ebit-clear
        then
    then
;

: off-map-e5.phase1 ( -- )   \ 004495E0
    0 exit-usable? if
        0 exit-check
    then
    4 0 0 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    0 62.08 0.0 -16.33 $A 5 0 zone
    hewie-who 0 8 char-zone-bits? if
        fiona-who char-here? if
            fiona-who char-busy? not in-panic? not and fiona-free? and if
                -1 control-action? not if
                    stalker-who char-here? not if
                        0 fiona-who 0 action
                    then
                then
            then
        then
    then
;

: off-map-e5.phase3 ( -- )   \ 00449640
    off-map-e5.cond00? not if
        4 ebit-set
    else 4 ebit? if
        4 ebit-clear
    else
        off-map-e5.cmd01
    then then
;

: off-map-e5.phase5 ( -- )   \ 00449660
    fiona-who char-busy? if
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-clear
        fiona-who action-end
        fiona-who $15 62.35 -10.65 0 char-to-xz
    then
;

: off-map-e5.act01 ( -- )   \ 00449750
    2 avoid-prompt
    1 self-scripted
    fiona-hidden state-flag-clear
    1 self-noclip
    0 0 var-set
    2 ebit? if
        counter-inc
    then
    2 room-motion 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    begin
        2 off-map-e5.cmd00
        yield
        0 28 var? not while
        0 var-inc
    repeat
    3 off-map-e5.cmd00
    in-panic? not if
        fiona-who seized-sound 5 char-sound
    then
    self-wait-anim
    0 self-noclip
    2 ebit? if
        2 wait-counter
    then
    stalker-who action-end
    0 self-scripted
    self-idle-or-end
;

: off-map-e5.act04 ( -- )   \ 00449840
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-hidden state-flag-clear
    1 self-noclip
    0 0 var-set
    3 room-motion 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    begin
        5 off-map-e5.cmd00
        yield
        0 18 var? not while
        0 var-inc
    repeat
    3 off-map-e5.cmd00
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        stalker-who action-end
        stalker-who char-here? if
            stalker-who 0 stalker-command
        then
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e5.act00 ( -- )   \ 00449680
    stalkers-stay state-flag-set
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    $15 60.918 -14.76 180 $FFFF $A self-move-to
    self-wait-done
    3 ebit-clear
    1 self-noclip
    fiona-who char-file-use
    self-who self-look-at
    yield
    0 0 var-set
    0 room-motion 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    begin
        0 off-map-e5.cmd00
        yield
        0 54 var? not while
        0 var-inc
    repeat
    self-wait-anim
    4 off-map-e5.cmd00
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 ebit-clear
    2 ebit-clear
    0 counter-set
    0 avoid-prompt
    begin
        2 ebit? if
            ['] off-map-e5.act01 goto
        else -1 control-action? if
            in-panic? 0 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] off-map-e5.act04 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? stalker-who char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                stalker-who action-end
                ['] off-map-e5.act04 goto
            then
            fiona-hidden state-flag-clear
            1 self-noclip
            0 0 var-set
            1 room-motion 0 self-anim-blend8
            begin
                1 off-map-e5.cmd00
                yield
                0 21 var? not while
                0 var-inc
            repeat
            3 off-map-e5.cmd00
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then then
    again
;

: off-map-e5.act03 ( -- )   \ 004497F0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $FA $FFFF 6 self-move-tri
    self-wait-done
    $10 58.883 -8.841 180 $FFFF 5 self-move-to
    self-wait-done
    there-anim self-anim
    self-wait-anim
    0 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: off-map-e5.phase2 ( -- )   \ 0047B0F4
;

\ ---- registered ----
off-map-e5 register-scripts   \ (its phases and actions, by their names)
