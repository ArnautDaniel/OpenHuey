\ story/rooms/old-mansion-3f-1.fs - the event scripts of room old-mansion-3f-1 ($60; Belli Castle: Old Mansion 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-3f-1
USING: actors messages room-names story.words story.shared flag-names story-names floor-flags ;

\ room 0x60 (D_00429148): the floor light (room effect 0x1B, 20 x 20 at y -0.2) by byte 3 - 1
\ removed with its glow effect (event variable 1); 0 made, with the glow (FloorGlow_vtable);
\ then (and for other values) its strength from event variable 0 (0..4: 0, 30, 60, 90, 128),
\ also sent to the glow
: old-mansion-3f-1.cmd00 ( b0 -- )   \ (the floor light: a reflecting floor 20 x 20 at y -0.2 with its glow (FloorGlow)
    \ - b0 1 both gone; 0 made, the glow kept in variable 1; then for any but 1 their strength
    \ by variable 0: 0..4 for 0, 30, 60, 90, 128)
    {: b0 | k :}
    b0 1 = if  1 $1B 0  12 0 do  0e  loop  lit-quad  1 var @ dup alive? if  kill  else  drop  then  exit  then
    b0 0= if  1 effects:floor-glow var-set  then
    0 var @ case  1 of $1E endof  2 of $3C endof  3 of $5A endof  4 of $80 endof  >r 0 r>  endcase  to k
    0 $1B k  10e -0.2e -10e  -10e -0.2e -10e  10e -0.2e 10e  -10e -0.2e 10e  lit-quad
    k if  k 1 var @ dup alive? if  send effect-command  else  2drop  then  then ;
\ room 0x60 (as Room23_Cmd01, for character 0xFE): byte 3 0 a progress name, 1 wait for
\ character 0xFE (2 while not), else done
: old-mansion-3f-1.cmd01 ( b0 -- )   \ (the stalker's face - character $FE: 0 loaded, 1 shown, else done)
    case  0 of  s" O_DNL\DNL_203.TEX" speech-load  endof  1 of  stalker-who 0 speak  endof  >r speech-done r>  endcase ;
\ (as Room66_Effect) byte 4 0 starts the 8-byte effect Room60Effect_vtable (parameters from byte
\ 3), its slot kept in event variable byte 3 + 2; else that effect is ended
: old-mansion-3f-1.cmd02 ( b0 b1 -- )  2drop ;   \ (a cone of light at a corner, Room60Effect: drawn only into the bloom's mask - not here)
\ room 0x60 (Room60_Cmd03_ptmf): character 0xFE's model +0x9FC 0.4 / +0xA00 1 (byte 3 0), or 0
: old-mansion-3f-1.cmd03 ( b0 -- )  drop ;   \ (the stalker's model's springs (+0x9FC / +0xA00): not simulated here yet)
\ character 0xFE's model +0x9E0 / +0x9E4 / +0x9E8: byte 3 0 -0.2 / 0.2 / -0.2; 1 eases them by
\ script variable 6 (a step-floor a call, waiting (2) for 60) to 0 / 0.3 / 0; else 0 / 0.3 / 0
: old-mansion-3f-1.cmd04 ( b0 -- )   \ (the stalker's springs eased by variable 6 over 60 frames: the springs
    \ aren't simulated here yet - the wait is kept)
    1 = if  begin  6 var-inc  6 var @ 61 < while  yield  repeat  then ;
\ room 0x60 (Room60_Cmd05_ptmf): the player's model +0xCC 0 (byte 3 0) or 1
: old-mansion-3f-1.cmd05 ( b0 -- )  drop ;   \ (the model hooks it calls are empty on PC and PS2 alike: CharModel_SetFlag / _VtCC / _VtD0)

: old-mansion-3f-1.enter ( -- )   \ 004280A0
    room-sounds
    9 part-hidden
    $A part-hidden
    om3f-1-flag2 story-flag? not if
        0 part-shown
        1 part-hidden
        0 0 0 $217 $4E2 obstacle-place
        1 1 0 $12D $3F3 obstacle-place
        2 2 0 $249 $519 obstacle-place
        3 3 0 $272 $544 obstacle-place
        0 ebit-clear
        1 ebit-clear
        2 ebit-clear
        3 ebit-clear
        7 part-hidden
        0 0 var-set
    else
        0 part-hidden
        1 part-shown
        0 0 0 $20A $4D8 obstacle-place
        1 1 0 $153 $415 obstacle-place
        2 2 0 $15C $41E obstacle-place
        3 3 0 $213 $4DE obstacle-place
        0 obstacle-stop
        1 obstacle-stop
        2 obstacle-stop
        3 obstacle-stop
        0 ebit-set
        1 ebit-set
        2 ebit-set
        3 ebit-set
        7 part-shown
        0 0 var-set
        0 solid-floor floor-on
        om3f-1-flag3 story-flag? not if
            9 part-shown
        else
            $A part-shown
        then
        got-mars-key story-flag? not if
            got-mars-key story-flag? not if
                0 1.29 1.0 -17.02 flicker-sprite
            then
        then
    then
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    8 part-shown
    2 part-hidden
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    $19 1 object-hidden
    $1A 1 object-hidden
    $1B 1 object-hidden
    0 old-mansion-3f-1.cmd00
    1 $2300 sound-volume
;

: old-mansion-3f-1.char-enter ( -- )   \ 004281E0
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
;

: old-mansion-3f-1.phase1 ( -- )   \ 00428220
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    6 sound-bank-loaded? if
        $C0000002 6 0.0 0.0 0.0 0 0 sound
    then
    0 ebit? not if
        0 $20A obstacle-on? if
            0 obstacle-stop
            0 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            fiona-who 1 6 char-sound
            0 0 old-mansion-3f-1.cmd02
        then
    then
    1 ebit? not if
        1 $153 obstacle-on? if
            1 obstacle-stop
            1 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            fiona-who 1 6 char-sound
            1 0 old-mansion-3f-1.cmd02
        then
    then
    2 ebit? not if
        2 $15C obstacle-on? if
            2 obstacle-stop
            2 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            fiona-who 1 6 char-sound
            2 0 old-mansion-3f-1.cmd02
        then
    then
    3 ebit? not if
        3 $213 obstacle-on? if
            3 obstacle-stop
            3 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            fiona-who 1 6 char-sound
            3 0 old-mansion-3f-1.cmd02
        then
    then
    om3f-1-flag1 story-flag? not fiona-who 4 char-entered-area? and 7 ebit? not and if
        7 ebit-set
        panic-eased
        fiona-who 0 char-action? if
            0 fiona-who 0 action-force
        else
            1 fiona-who 0 action-force
        then
    then
    om3f-1-flag1 story-flag? om3f-1-flag2 story-flag? not and if
        0 ebit? 1 ebit? and 2 ebit? and 3 ebit? and if
            0 0.0 0.0 0.0 $A 10 0 zone
            stalker-who 0 3 char-zone-bits? if
                om3f-1-flag2 story-flag-set
                om1f-6-flag1 story-flag-set
                force-calm state-flag-set
                0 counter-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 8 action-force
                else
                    1 fiona-who 8 action-force
                then
            then
        then
        stalker-who char-dead? if
            6 ebit-set
        then
        6 ebit? if
            stalker-who char-dead? not if
                $23D item-give
            then
        then
    then
    3 old-mansion-3f-1.cmd00
    0 0 var? if
        3 part-hidden
        4 part-hidden
        5 part-hidden
        6 part-hidden
        8 part-shown
    else 0 1 var? if
        3 part-shown
        4 part-hidden
        5 part-hidden
        6 part-hidden
        8 part-hidden
    else 0 2 var? if
        3 part-hidden
        4 part-shown
        5 part-hidden
        6 part-hidden
        8 part-hidden
    else 0 3 var? if
        3 part-hidden
        4 part-hidden
        5 part-shown
        6 part-hidden
        8 part-hidden
    else 0 4 var? if
        3 part-hidden
        4 part-hidden
        5 part-hidden
        6 part-shown
        8 part-hidden
    else
        3 part-hidden
        4 part-hidden
        5 part-hidden
        6 part-hidden
        8 part-shown
    then then then then then
;

: old-mansion-3f-1.phase2 ( -- )   \ 00428460
    scene-5-pending state-flag? if
        -2147483646 scene-request? if
            6 always offer-action
        then
    then
    om3f-1-flag2 story-flag? if
        fiona-who 5 $2D char-faces-area? if
            7 unhurried offer-action
        then
    then
    got-mars-key story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then
;

: old-mansion-3f-1.phase3 ( -- )   \ 004284A0
    50.0 -29.0 -143.0 -8.8 9.3 -143.0 50.0 -50.0 -143.0 -8.8 -10.0 -143.0 lights-doorway
    66.0 -39.0 -143.0 50.0 -29.0 -143.0 69.0 -55.0 -143.0 50.0 -50.0 -143.0 lights-doorway
;

: old-mansion-3f-1.act00 ( -- )   \ 00428510
    3 stalker-kind? if
        4 ebit-set
    else
        4 ebit-clear
    then
    2 0 char-remove
    stalkers-stay state-flag-set
    1 self-scripted
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    force-calm state-flag-set
    self-wait-done
    4 ebit? if
        $23 partner-load
        om3f-1-flag3 story-flag-clear
    else
        $24 partner-load
        om3f-1-flag3 story-flag-set
    then
    summoner-on state-flag-clear
    2 char-start
    $F $44 fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    world-held state-flag-set
    $1D 3 $FF char-load
    0 old-mansion-3f-1.cmd01
    hewie-who char-activate
    $60 0 433 hewie-to-room
    $FE $60 1571 2 stalker-to-room
    stalker-who 2 2 char-camera
    stalker-who action-end
    stalker-who char-done
    3 4 0 char-model-op
    3 char-start
    0 $F9 1 action
    2 part-shown
    stalker-who char-activate
    panic-eased
    0 message-prepare   \ "Hewie! Blood, Flesh Woman. You vile creature. You lure the man into your filthy..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who $247 -0.592 1.575 180 char-to-xz
    hewie-who $2CF 0.346 -5.781 180 char-to-xz
    stalker-who $50D -1.47 -23.64 0 char-to-xz
    2 part-hidden
    3 0 char-remove
    2 old-mansion-3f-1.cmd01
    1 old-mansion-3f-1.cmd03
    2 old-mansion-3f-1.cmd04
    0 old-mansion-3f-1.cmd05
    $70 door-open-clear
    $70 door-lock
    doors-room-in
    scene-5-pending state-flag-set
    om3f-1-flag1 story-flag-set
    summoner-on state-flag-clear
    fiona-who $247 -0.592 1.575 180 char-to-xz
    hewie-who $2CF 0.346 -5.781 180 char-to-xz
    stalker-who $50D -1.47 -23.64 0 char-to-xz
    stalker-who 2 2 char-camera
    force-calm state-flag-clear
    8 1.0 0 bgm
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    self-idle-or-end
;

: old-mansion-3f-1.act01 ( -- )   \ 004286A0
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd05
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 old-mansion-3f-1.cmd05
    begin
        $468 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $477 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $484 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $499 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $49D cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4B3 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $4B4 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4D2 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $4D7 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4F2 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $E cutscene-shot? not while
        yield
    repeat
    0 old-mansion-3f-1.cmd03
    begin
        $11 cutscene-shot? not while
        yield
    repeat
    0 old-mansion-3f-1.cmd04
    begin
        $12 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd03
    begin
        $B31 cutscene-cue-reached? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd04
    begin
        $19 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd01
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-3f-1.act02 ( -- )   \ 00428750
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    in-panic? not if
        stalker-who self-look-at
        yield
        fiona-who 5 char-in-area? if
            0.0 0.0 self-turn-to-xz
            self-wait-done
            $F00 $A self-anim-blend
            self-wait-anim
            $402 $A self-anim-blend
            self-wait-anim
        then
    then
    1 wait-counter
    $FF 1.0 0 bgm
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    $F 4 fade
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    self-who self-look-at
    yield
    fiona-who 1 char-visible
    hewie-who action-end
    hewie-who char-done
    0 part-hidden
    1 part-shown
    0 $F9 3 action
    $1E 1 object-hidden
    om3f-1-flag3 story-flag? not if
        9 part-shown
    else
        $A part-shown
    then
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    2 6 sound-stop
    $1E 1 object-hidden
    scene-5-pending state-flag-clear
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    force-calm state-flag-clear
    2 0 char-remove
    7 part-shown
    0 0 var-set
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    8 part-shown
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    $19 1 object-hidden
    $1A 1 object-hidden
    $1B 1 object-hidden
    0 self-scripted
    0 panic-stage
    got-mars-key story-flag? not if
        0 1.29 1.0 -17.02 flicker-sprite
    then
    fiona-who 0 char-visible
    hewie-who char-activate
    hewie-who char-full-health
    fiona-who $1E6 -22.22 -47.34 27 char-to-xz
    hewie-who $30 -14.71 -55.27 21 char-to-xz
    1 self-anim
    2 2 player-camera
    camera-restart
    0 solid-floor floor-on
    1 old-mansion-3f-1.cmd00
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    fiona-who camera-follow
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    $70 door-unlock
    stalker-who $60 0 room-doors-state
    stalker-who $63 0 room-doors-state
    self-idle-or-end
;

: old-mansion-3f-1.act03 ( -- )   \ 00428950
    begin
        2 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-hidden
    begin
        4 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-hidden
    begin
        5 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd00
    0 0 var-set
    begin
        $35A cutscene-cue-reached? not while
        yield
    repeat
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    begin
        6 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-hidden
    0 0 old-mansion-3f-1.cmd02
    1 0 old-mansion-3f-1.cmd02
    2 0 old-mansion-3f-1.cmd02
    3 0 old-mansion-3f-1.cmd02
    begin
        $369 cutscene-cue-reached? not while
        yield
    repeat
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    2 6 sound-stop
    begin
        7 cutscene-shot? not while
        yield
    repeat
    2 0 old-mansion-3f-1.cmd02
    3 0 old-mansion-3f-1.cmd02
    begin
        $378 cutscene-cue-reached? not if
            yield
        else
            2 1 old-mansion-3f-1.cmd02
            3 1 old-mansion-3f-1.cmd02
            $A cutscene-shot? not if
                yield
            else
                $1E 1 object-hidden
                begin
                    0 cutscene-cue-reached? while
                    yield
                repeat
                self-idle-or-end
            then
        then
    again
;

: old-mansion-3f-1.act04 ( -- )   \ 00428A10
    stalker-who camera-follow
    1 self-scripted
    self-wait-done
    stalker-who $1D char-file-load
    0 -0.03 0.0 -90 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-file-use
    $8000 5 self-anim-blend
    self-frames-reset
    $18D self-wait-frames
    counter-inc
    self-wait-anim
    begin
        yield
    again
;

: old-mansion-3f-1.act05 ( -- )   \ 00428A40
    self-wait-done
    0.0 -17.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-mars-key story-flag-set
    0 effect-remove
    $11 message-param-room
    $11 1 item-give-count
    0 $11 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    50 hewie-trust
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-3f-1.act06 ( -- )   \ 0047AD40
    1 message   \ "Damn! The door is locked!"
    self-idle-or-end
;

: old-mansion-3f-1.act07 ( -- )   \ 00428A90
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    5 ebit? not if
        2 message   \ "A shard of glass pierces through her body and pins her to the ground. I can't belie..."
        wait-message
        5 ebit-set
    else
        $A02 $A self-anim-blend
        self-wait-anim
        3 message   \ "When she was alive, her facial expressions always seemed ice-cold and devoid of fee..."
        wait-message
        5 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-3f-1.act08 ( -- )   \ 00428AC0
    $F 0 fade
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    1 self-scripted
    self-wait-done
    wait-fade
    world-held state-flag-set
    yield
    0 stalker-who 9 action-force
    1 wait-counter
    fiona-who 1 char-visible
    hewie-who action-end
    hewie-who char-done
    $FF 1.0 0 bgm
    self-who 3 -1 char-camera
    0 panic-stage
    counter-inc
    $F 1 fade
    3 wait-counter
    wait-fade
    $F 4 fade
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 part-hidden
    1 part-shown
    0 $F9 3 action
    $1E 1 object-hidden
    om3f-1-flag3 story-flag? not if
        9 part-shown
    else
        $A part-shown
    then
    $1E $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    2 6 sound-stop
    $1E 1 object-hidden
    fiona-who 0 char-visible
    scene-5-pending state-flag-clear
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    force-calm state-flag-clear
    2 0 char-remove
    7 part-shown
    0 0 var-set
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    8 part-shown
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    $19 1 object-hidden
    $1A 1 object-hidden
    $1B 1 object-hidden
    0 self-scripted
    0 panic-stage
    got-mars-key story-flag? not if
        0 1.29 1.0 -17.02 flicker-sprite
    then
    fiona-who 0 char-visible
    hewie-who char-activate
    hewie-who char-full-health
    fiona-who $1E6 -22.22 -47.34 27 char-to-xz
    0 self-idle-anim
    hewie-who $30 -14.71 -55.27 21 char-to-xz
    $29 0 hewie-do
    2 2 player-camera
    camera-restart
    0 solid-floor floor-on
    1 old-mansion-3f-1.cmd00
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    fiona-who camera-follow
    $F $41 fade
    wait-fade
    $23E item-give
    $8282 item-give
    shared.act9C
    stalkers-stay state-flag-clear
    $70 door-unlock
    stalker-who $60 0 room-doors-state
    stalker-who $63 0 room-doors-state
    self-idle-or-end
;

: old-mansion-3f-1.act09 ( -- )   \ 00428CD0
    1 self-scripted
    self-wait-done
    stalker-who $1D char-file-load
    stalker-who char-file-use
    counter-inc
    2 wait-counter
    stalker-who $1AB 0.25 0.25 -90 char-to-xz
    $8000 self-anim
    self-frames-reset
    $96 self-wait-frames
    stalker-who 3 6 char-sound
    self-frames-reset
    $F7 self-wait-frames
    self-wait-anim
    counter-inc
    begin
        yield
    again
;

: old-mansion-3f-1.act0A ( -- )   \ 00428D10
    self-wait-done
    9 part-hidden
    $A part-hidden
    8 part-shown
    0 part-shown
    1 part-hidden
    0 0 0 $217 $4E2 obstacle-place
    1 1 0 $12D $3F3 obstacle-place
    2 2 0 $249 $519 obstacle-place
    3 3 0 $272 $544 obstacle-place
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    3 ebit-clear
    7 part-hidden
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    $19 1 object-hidden
    $1A 1 object-hidden
    $1B 1 object-hidden
    0 old-mansion-3f-1.cmd00
    $23 partner-load
    2 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $1D 3 $FF char-load
    0 old-mansion-3f-1.cmd01
    3 4 0 char-model-op
    3 char-start
    0 $F9 1 action
    2 part-shown
    panic-eased
    0 message-prepare   \ "Hewie! Blood, Flesh Woman. You vile creature. You lure the man into your filthy..."
    1 result? if
        scene-play
    then
    2 part-hidden
    3 0 char-remove
    2 old-mansion-3f-1.cmd01
    1 old-mansion-3f-1.cmd03
    2 old-mansion-3f-1.cmd04
    0 old-mansion-3f-1.cmd05
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-3f-1.act0B ( -- )   \ 00428E70
    self-wait-done
    9 part-hidden
    $A part-hidden
    0 part-hidden
    1 part-shown
    0 0 0 $20A $4D8 obstacle-place
    1 1 0 $153 $415 obstacle-place
    2 2 0 $15C $41E obstacle-place
    3 3 0 $213 $4DE obstacle-place
    0 obstacle-stop
    1 obstacle-stop
    2 obstacle-stop
    3 obstacle-stop
    0 ebit-set
    1 ebit-set
    2 ebit-set
    3 ebit-set
    7 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-shown
    8 part-hidden
    2 part-hidden
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    $19 1 object-hidden
    $1A 1 object-hidden
    $1B 1 object-hidden
    0 4 var-set
    0 old-mansion-3f-1.cmd00
    0 0 old-mansion-3f-1.cmd02
    1 0 old-mansion-3f-1.cmd02
    2 0 old-mansion-3f-1.cmd02
    3 0 old-mansion-3f-1.cmd02
    $23 partner-load
    2 char-start
    fiona-who 1 char-visible
    self-who 3 -1 char-camera
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-hidden
    1 part-shown
    0 $F9 3 action
    $1E 1 object-hidden
    9 part-shown
    $1E $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 6 sound-stop
    $1E 1 object-hidden
    2 0 char-remove
    7 part-shown
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    8 part-shown
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    $19 1 object-hidden
    $1A 1 object-hidden
    $1B 1 object-hidden
    1 old-mansion-3f-1.cmd00
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-3f-1 register-scripts   \ (its phases and actions, by their names)
