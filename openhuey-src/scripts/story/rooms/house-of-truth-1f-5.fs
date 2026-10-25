\ story/rooms/house-of-truth-1f-5.fs - the event scripts of room house-of-truth-1f-5 ($8D; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-5
USING: engine actors messages room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
1 constant stalkers-held               \ (bit 1) the stalkers held until she enters area $B
2 constant sound-started               \ (bit 2) its sound started (then kept up)
3 constant music-changed               \ (bit 3) the room's music changed (act01)
4 constant drone-on                    \ (bit 4) the drone sound ($30, bank $47) going
0 constant marker-var                  \ (var 0) the marker's effect
1 constant shrink-var                  \ (var 1) the marker shrinking by it (334)
2 constant sound-step-var              \ (var 2) which of its sounds next (0..3)
3 constant sound-frames-var            \ (var 3) frames to the next sound (30)
0 constant room-loop-sound             \ (snd 0) its steady sound (started as it comes in, then kept up)
3 constant ambient1-sound              \ (snd 3) its ambient sounds in turn (every 30 frames): the first
4 constant ambient2-sound              \ (snd 4) its ambient sounds in turn (every 30 frames): the second
5 constant ambient3-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the third
6 constant ambient4-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the fourth

\ room 0x8D: three grey smoke effects (Effect79B00, size 60) at the room's spots 7, 4, 3
\ (grey_three).
: house-of-truth-1f-5.cmd00 ( -- )   \ (the ceiling coming down: stones of three kinds falling from 60)
    7 60e 0e 0.5e 0.5e stones-falling  4 60e 0e 0e 0.5e stones-falling  3 60e 0e 0e 1e stones-falling ;
\ byte 3: 0 made (lights 0x14 on characters 3 and 0) and 1 lit on character 3 (second kind, size
\ 1, sparks from its own slot); 2 ending, 3 ended; 4 / 6 sized 1 / 0.5; 5 / 7 shrinking with
\ variable 1 (a step a call, to 0.5 + v / 200 or v / 334)
: marker! ( v -- )   \ (the marker kept in variable 0 told: -1 done, -2 a pause, else its size x 1000)
    marker-var var @ dup alive? if  send effect-command  else  2drop  then ;
: shrunk ( -- v )  shrink-var var @ 1- 0 max  dup shrink-var swap var-set ;   \ (variable 1 down a step)
: house-of-truth-1f-5.cmd01 ( b0 -- )
    case
        0 of  3 $14 char-layer  fiona-who $14 char-layer
              3 char-model 0 char-model 1  1e effects:marker  marker-var swap var-set  endof
        1 of  1000 marker!  endof
        2 of  -1 marker!  endof
        3 of  -2 marker!  endof
        4 of  1000 marker!  endof
        5 of  shrunk  5 *  500 +  marker!  endof
        6 of  500 marker!  endof
        7 of  shrunk  1000 334 */  marker!  endof
    endcase ;
\ room 0x8D: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-5.cmd02 ( -- )  slam-shake ;

: house-of-truth-1f-5.enter ( -- )   \ 004348E0
    room-sounds
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    over-music-f story-flag? found-file-257 story-flag? not and if
        stalkers-held ebit-set
    then
    stalkers-held ebit? if
        stalkers-stay state-flag-set
    then
    over-music-f story-flag? if
        0 barred-floor floor-on
        0 part-shown
    else
        1 part-shown
    then
    found-file-257 story-flag? if
        2 part-shown
    then
    tremor-warned story-flag? if
        5 1.0 0 bgm
    then
    shared.act95
    ht1f-9-flag2 story-flag? if
        house-of-truth-1f-5.cmd00
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-5.char-enter ( -- )   \ 00434940
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
;

: house-of-truth-1f-5.phase1 ( -- )   \ 00434A00
    music-changed ebit? not if
        over-music-f story-flag? if
            sound-started ebit? not if
                6 sound-bank-loaded? if
                    room-loop-sound volume-scaled 6 0.0 20.0 100.0 0 0 sound
                    sound-started ebit-set
                then
            else
                room-loop-sound kept-going volume-scaled 6 -60.0 20.0 50.0 0 0 sound
            then
        then
    then
    drone-on ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        2 exit-prepare
    then
    fiona-who $B char-entered-area? if
        stalkers-held ebit? if
            stalkers-held ebit-clear
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    house-of-truth-1f-5.cmd02
    music-changed ebit? not if
        6 sound-bank-loaded? if
            sound-frames-var var-inc
            sound-frames-var 30 var? if
                sound-frames-var 0 var-set
                sound-step-var 0 var? if
                    ambient1-sound volume-scaled 6 28.0 20.0 -71.0 0 0 sound
                else sound-step-var 1 var? if
                    ambient2-sound volume-scaled 6 28.0 20.0 -71.0 0 0 sound
                else sound-step-var 2 var? if
                    ambient3-sound volume-scaled 6 28.0 20.0 -71.0 0 0 sound
                else sound-step-var 3 var? if
                    ambient4-sound volume-scaled 6 28.0 20.0 -71.0 0 0 sound
                then then then then
                sound-step-var var-inc
                sound-step-var 4 var? if
                    sound-step-var 0 var-set
                then
            then
        then
    then
;

: house-of-truth-1f-5.phase2 ( -- )   \ 00434B50
    fiona-who $A char-in-area? fiona-who 67 $32 char-heading? and if
        found-file-257 story-flag? not if
            $84 calm-hewie-up offer-action
        else
            6 unhurried offer-action
        then
    then
    fiona-who $C char-in-area? fiona-who 0 $32 char-heading? and if
        found-file-257 story-flag? if
            1 unhurried offer-action
        else
            2 unhurried offer-action
        then
    then
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            found-file-257 story-flag? not if
                3 always offer-action
            else
                6 unhurried offer-action
            then
        then
        fiona-who 1 char-before-door? if
            found-file-257 story-flag? not if
                4 always offer-action
            else
                6 unhurried offer-action
            then
        then
        fiona-who 2 char-before-door? if
            found-file-257 story-flag? if
                6 unhurried offer-action
            then
        then
    then
    found-file-257 story-flag? if
        0 0.0 0.0 74.25 8 5 0 zone
        fiona-who 0 8 char-zone-bits? if
            8 unhurried offer-action
        then
    then
;

: house-of-truth-1f-5.phase3 ( -- )   \ 00434BE0
    ht1f-9-flag2 story-flag? drone-on ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        drone-on ebit-set
    then
;

: house-of-truth-1f-5.phase5 ( -- )   \ 00434C00
    stalkers-held ebit? if
        stalkers-stay state-flag-clear
    then
;

: house-of-truth-1f-5.act00 ( -- )   \ 00434C10
    1 self-scripted
    tremor-warned story-flag-clear
    $FF 1.0 0 bgm
    8 track action-end
    $F $14 fade
    self-wait-done
    wait-fade
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    $1E 3 $FF char-load
    $1F 4 $FF char-load
    force-followed state-flag-set
    3 char-start
    4 char-start
    0 part-shown
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 5 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 house-of-truth-1f-5.cmd01
    3 0 char-remove
    4 0 char-remove
    fiona-who $63 -0.61 47.71 0 char-to-xz
    1 1 player-camera
    camera-restart
    hewie-who char-activate
    $8D 0 123 hewie-to-room
    hewie-who $7B -20 char-to-tri-facing
    hewie-no-attack state-flag-clear
    2 part-shown
    $EA door-open-clear
    $EA door-lock
    doors-room-in
    3 0.0 0.0 65.0 $80 $80 $80 1 dust
    $F $11 fade
    wait-fade
    $51 resident-flag-set
    $257 item-give
    $828E item-give
    stalkers-stay state-flag-clear
    found-file-257 story-flag-set
    stalkers-held ebit-clear
    shared.act95
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-5.act01 ( -- )   \ 00434D50
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    2 say   \ "You feel a mystical power flowing all throughout the door. Open the door? YES NO"
    0 answer? if
        $F $54 fade
        $35 room-preload
        $1C 1.0 1 bgm
        wait-fade
        music-changed ebit-set
        $30 7 sound-stop
        room-loop-sound 6 sound-stop
        ambient1-sound 6 sound-stop
        ambient2-sound 6 sound-stop
        ambient3-sound 6 sound-stop
        ambient4-sound 6 sound-stop
        [: 0 adx? ;] wait-until
        1.0 sound-volume-scale
        0 0.0 $FF bgm
        [: 1 adx? ;] wait-until
        $81 exit-check
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-5.act02 ( -- )   \ 00434DC0
    self-wait-done
    $CA 0.0 96.99 0 $FFFF 5 self-move-to
    self-wait-done
    4 wait-frames
    lean-anim play-out
    strain-anim self-anim
    16 wait-frames
    let-go-push-anim play-out
    -1 self-idle-anim
    7 wait-frames
    1 say   \ "This door is closed so tightly you'd think it was welded shut. It almost as if it's sea..."
    self-idle-or-end
;

: house-of-truth-1f-5.act03 ( -- )   \ 00434DF0
    self-wait-done
    0 self-through-exit
    self-wait-done
    in-panic? 2 game-mode? or if
        rattle-front-slow-anim play-out
        -1 self-idle-anim
        7 wait-frames
    else
        rattle-front-anim play-out
        -1 self-idle-anim
        7 wait-frames
        4 say   \ "It's locked and won't open. How am I going to get this open?"
    then
    self-idle-or-end
;

: house-of-truth-1f-5.act04 ( -- )   \ 00434E20
    self-wait-done
    1 self-through-exit
    self-wait-done
    in-panic? 2 game-mode? or if
        rattle-behind-slow-anim play-out
        -1 self-idle-anim
        7 wait-frames
    else
        rattle-behind-anim play-out
        -1 self-idle-anim
        7 wait-frames
        4 say   \ "It's locked and won't open. How am I going to get this open?"
    then
    self-idle-or-end
;

: house-of-truth-1f-5.act05 ( -- )   \ 00434E50
    [: 0 cutscene-shot? ;] wait-until
    0 house-of-truth-1f-5.cmd01
    [: 2 cutscene-shot? ;] wait-until
    2 house-of-truth-1f-5.cmd01
    [: 3 cutscene-shot? ;] wait-until
    0 house-of-truth-1f-5.cmd01
    [: 4 cutscene-shot? ;] wait-until
    3 house-of-truth-1f-5.cmd01
    [: $1E0 cutscene-cue-reached? ;] wait-until
    4 house-of-truth-1f-5.cmd01
    [: 7 cutscene-shot? ;] wait-until
    2 house-of-truth-1f-5.cmd01
    [: 8 cutscene-shot? ;] wait-until
    0 house-of-truth-1f-5.cmd01
    [: 9 cutscene-shot? ;] wait-until
    2 house-of-truth-1f-5.cmd01
    [: $A cutscene-shot? ;] wait-until
    0 house-of-truth-1f-5.cmd01
    [: $B cutscene-shot? ;] wait-until
    2 house-of-truth-1f-5.cmd01
    [: $C cutscene-shot? ;] wait-until
    0 house-of-truth-1f-5.cmd01
    6 house-of-truth-1f-5.cmd01
    [: $58C cutscene-cue-reached? ;] wait-until
    4 house-of-truth-1f-5.cmd01
    [: $D cutscene-shot? ;] wait-until
    2 house-of-truth-1f-5.cmd01
    [: $E cutscene-shot? ;] wait-until
    0 house-of-truth-1f-5.cmd01
    [: $65E cutscene-cue-reached? ;] wait-until
    shrink-var 334 var-set
    begin
        0 cutscene-cue-reached? while
        7 house-of-truth-1f-5.cmd01
        yield
    repeat
    self-idle-or-end
;

: house-of-truth-1f-5.act06 ( -- )   \ 00434F10
    self-wait-done
    rest-anim self-anim
    5 say   \ "I have to get out of here now!"
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-5.act07 ( -- )   \ 00434F20
    self-wait-done
    house-of-truth-1f-5.cmd00
    $1E 3 $FF char-load
    $1F 4 $FF char-load
    3 char-start
    4 char-start
    0 part-shown
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 5 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 house-of-truth-1f-5.cmd01
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: house-of-truth-1f-5.act08 ( -- )   \ 00434FC8
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    3 say   \ "This time... you really are dead. You bastard."
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-5 register-scripts   \ (its phases and actions, by their names)
