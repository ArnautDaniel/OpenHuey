\ story/rooms/house-of-truth-1f-6.fs - the event scripts of room house-of-truth-1f-6 ($8E; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-6
USING: engine common actors messages room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant door-opened-bit             \ (bit 0) the door opened (act00)
1 constant half-hid                    \ (bit 1) she half-hid (stage 16)
2 constant defended                    \ (bit 2) she defended herself (stage 17)
3 constant waited-out                  \ (bit 3) she waited 300 frames (stage 18)
4 constant door-open                   \ (bit 4) the door is open (stage 6)
5 constant offer-taken                 \ (bit 5) its action taken (stage 7)
6 constant hid                         \ (bit 6) she hid (stage 5)
7 constant scene-done-bit              \ (bit 7) act05 played (stage 32768)
8 constant hewie-along                 \ (bit 8) Hewie came along with her (near, commandable)
9 constant hewie-in-place              \ (bit 9) Hewie at his place for the scene
10 constant late                       \ (bit 10) the time ran past 2250 (stage 13)
11 constant exit1-open                 \ (bit 11) exit 1's door open
12 constant left-area8                 \ (bit 12) she left area 8 (her reaction made)
13 constant area0-reacted              \ (bit 13) her reaction on entering the area made (once a visit)
0 constant door-angle-var              \ (var 0) the door's angle (from -90)
1 constant stage-var                   \ (var 1) the scene's stage (-1 none; 5 .. 18; 32768 done)
2 constant time-var                    \ (var 2) its time (to 450, 1800, 2250, 2700)
3 constant tries-var                   \ (var 3) her tries at the door
4 constant still-frames-var            \ (var 4) her frames standing still (300)
0 constant rumble-sound                \ (snd 0) the room shaking
1 constant heave-sound                 \ (snd 1) her heaving it
2 constant drop-sound                  \ (snd 2) it dropping?

\ door 0 swung by script variable 0: byte 3 0 sets it to -90; 1 opens it 10 degrees a step to 0
\ (doors +0x74), waiting (2) until there
: house-of-truth-1f-6.cmd00 ( b0 -- )   \ (door 0 by variable 0: b0 0 it to -90; 1 opened 10 degrees a frame from there to 0)
    case
        0 of  door-angle-var -90 var-set  endof
        1 of  begin
                  door-angle-var var @ 10 + 0 min  dup door-angle-var swap var-set
                  dup s>f  0 true door-swing
                  0< while  yield
              repeat  endof
    endcase ;
\ the room object named pstr_tana falling over: byte 3 0 starts it (angle +0x30, speed +0x34 and
\ acceleration +0x38 0, jerk +0x3C 0.005); 1 steps them, its tilt +0x10 = (1 - sin(90 - angle))
\ * pi/2, waiting (2) until the angle reaches 90; 2 puts it down (sin(pi/2))
: house-of-truth-1f-6.cmd01 ( b0 -- )   \ ("tana", the shelf, falling over: b0 0 starts it - angle (+0x30), speed,
    \ acceleration 0, jerk 0.005; 1 a step each frame, its tilt (1 - sin(90 - angle)) x pi/2,
    \ till the angle reaches 90; 2 put down)
    {: b0 :}  s" tana" object-here? 0= if  exit  then
    b0 case
        0 of  0e s" tana" $30 object!  0e s" tana" $34 object!  0e s" tana" $38 object!  0.005e s" tana" $3C object!  endof
        1 of  begin
                  s" tana" $3C object@ s" tana" $38 object+!
                  s" tana" $38 object@ s" tana" $34 object+!
                  s" tana" $34 object@ s" tana" $30 object+!
                  90e  s" tana" $30 object@ 90e fmin  f- deg>rad fsin  1e fswap f-  pi f2/ f*  s" tana" turn-x object!
                  s" tana" $30 object@ 90e f< while  yield
              repeat  endof
        2 of  pi f2/ fsin s" tana" turn-x object!  endof
    endcase ;

defer house-of-truth-1f-6.act02
: house-of-truth-1f-6.enter ( -- )   \ 00435050
    room-sounds
    time-var -1 var-set
    stage-var -1 var-set
    over-music-e story-flag? not if
        0 1 track 2 action
    then
    0 79.7 11.1 -39.8 0 flame-in
;

: house-of-truth-1f-6.char-enter ( -- )   \ 00435080
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    fiona-who self-is? if
        over-music-e story-flag? not if
            fiona-who $85 0.0 10.0 180 char-to-xz
            0 fiona-who 1 action
        then
    then
;

: house-of-truth-1f-6.phase1 ( -- )   \ 004350E0
    over-music-e story-flag? if
        0 exit-usable? if
            0 exit-check
        then
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 2 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    over-music-e story-flag? not if
        door-opened-bit ebit? not if
            exit1-open ebit? late ebit? and if
                fiona-who 8 char-left-area? left-area8 ebit? not and if
                    left-area8 ebit-set
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 0 action-force
                    else
                        1 fiona-who 0 action-force
                    then
                then
            then
        else fiona-who 0 char-entered-area? area0-reacted ebit? not and if
            area0-reacted ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who $A action-force
            else
                1 fiona-who $A action-force
            then
        then then
        scene-done-bit ebit? not exit1-open ebit? and if
            fiona-who $A char-in-area? if
                fiona-who 0 char-in-nav-group? not if
                    0 fiona-who 5 action
                then
            then
        then
        stage-var -1 var? if
            hid ebit? not if
                fiona-hidden state-flag? if
                    hid ebit-set
                    stage-var 5 var-set
                then
            then
            half-hid ebit? not if
                fiona-half-hidden state-flag? if
                    half-hid ebit-set
                    stage-var 16 var-set
                then
            then
            defended ebit? not if
                fiona-who 8 char-action? if
                    defended ebit-set
                    stage-var 17 var-set
                then
            then
            door-open ebit? not if
                1 exit-door-open? if
                    door-open ebit-set
                    stage-var 6 var-set
                then
            then
        then
        1 exit-door-open? if
            exit1-open ebit-set
        then
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
;

: house-of-truth-1f-6.phase2 ( -- )   \ 00435200
    fiona-who 9 char-in-area? fiona-who -45 $3C char-heading? and if
        stalker-who char-here? not if
            8 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            4 always offer-action
        then
    then
    over-music-e story-flag? not if
        stage-var -1 var? if
            offer-taken ebit? not if
                fiona-who $B char-in-area? fiona-who 0 $32 char-heading? and if
                    7 unhurried offer-action
                then
            then
        then
    then
;

: house-of-truth-1f-6.act00 ( -- )   \ 00435250
    stage-var 32768 var-set
    1 track action-end
    stalker-who action-end
    stalker-who char-done
    scene-locked state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "Come to me, Fiona... Come to me, Fiona..."
    1 result? if
        scene-play
    then
    fiona-who $1A 0.49 -36.88 -21 char-to-xz
    0 0 player-camera
    camera-restart
    door-opened-bit ebit-set
    $E9 door-open-set
    $E9 door-unlock
    doors-room-in
    $F $41 fade
    wait-fade
    $49 resident-flag-set
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-6.act01 ( -- )   \ 00435320
    2 0 char-remove
    $A partner-load
    self-wait-done
    $2C 1.0 1 bgm
    0 house-of-truth-1f-6.cmd00
    1 house-of-truth-1f-6.cmd00
    fiona-who $28 5 char-sound
    $E9 door-open-clear
    $E9 door-lock
    doors-room-in
    fiona-who seized-sound 5 char-sound
    look-behind-anim 3 blend-out
    0 self-through-exit
    self-wait-done
    rattle-behind-slow-anim self-anim
    16 wait-frames
    [: 0 adx? ;] wait-until
    1 show-message   \ "You've been very naughty, Fiona. You are causing me much grief."
    0 0.0 $FF bgm
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    rest-anim $A blend-out
    wait-message
    [: 1 adx? ;] wait-until
    time-var 0 var-set
    still-frames-var 0 var-set
    2 char-start
    self-idle-or-end
;

: house-of-truth-1f-6.act03 ( -- )   \ 00435570
    scene-locked state-flag-set
    yield
    [: 0 adx? ;] wait-until
    0 0.0 $FF bgm
    stage-var 2 var? if
        2 say   \ "What are you doing, Fiona? Just where do you intend to run?"
    else stage-var 3 var? if
        3 say   \ "How adorable you are, Fiona."
    else stage-var 4 var? if
        4 say   \ "Damn, it's locked!"
    else stage-var 5 var? if
        5 say   \ "Do you really think you can hide from me there?"
    else stage-var 6 var? if
        6 say   \ "Aha. Looks like a dead end, dear. How unfortunate."
    else stage-var 7 var? if
        7 say   \ "I can see you clearly from here, Fiona..."
    else stage-var 8 var? if
        8 say   \ "That was a close one, eh Fiona?"
    else stage-var 9 var? if
        9 say   \ "Oh, how I love thee, Azoth!"
    else stage-var 10 var? if
        $A say   \ "You simply cannot understand."
    else stage-var 11 var? if
        $B say   \ "Why must you run from me?"
    else stage-var 12 var? if
        $C say   \ "Everything I do is for the Great Truth!"
    else stage-var 13 var? if
        $D say   \ "At last! You are mine, Fiona!"
    else stage-var 14 var? if
        $E say   \ "My dear Fiona..."
    else stage-var 15 var? if
        $F say   \ "I will have you in my grasp soon enough."
    else stage-var 16 var? if
        $10 say   \ "Have you longed for me as I long for you?"
    else stage-var 17 var? if
        $11 say   \ "No need to get so upset, my dear."
    else stage-var 18 var? if
        $12 say   \ "Well? Have you given up?"
    then then then then then then then then then then then then then then then then then
    [: 1 adx? ;] wait-until
    stage-var 32768 var? not if
        stage-var -1 var-set
    then
    scene-locked state-flag-clear
    ['] house-of-truth-1f-6.act02 goto
;

:noname   \ house-of-truth-1f-6.act02 (00435390; deferred: used before it is defined)
    begin
        stage-var 32768 var? if
            yield
        else
            time-var -1 var? not if
                stage-var -1 var? if
                    time-var var-inc
                    time-var 450 var? if
                        stage-var 9 var-set
                    else time-var 900 var? if
                        stage-var 10 var-set
                    else time-var 1350 var? if
                        stage-var 11 var-set
                    else time-var 1800 var? if
                        stage-var 12 var-set
                    else time-var 2250 var? if
                        late ebit-set
                        stage-var 13 var-set
                    else time-var 2700 var? if
                        stage-var 14 var-set
                    else time-var 3150 var? if
                        stage-var 15 var-set
                        time-var 2250 var-set
                    then then then then then then then
                then
                waited-out ebit? not if
                    stage-var -1 var? if
                        fiona-who char-busy? not $FF $FF pad? not and -1 control-action? and if
                            still-frames-var var-inc
                            still-frames-var 300 var? if
                                waited-out ebit-set
                                stage-var 18 var-set
                            then
                        else
                            still-frames-var 0 var-set
                        then
                    then
                then
                stage-var 2 var? if
                    $1E 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 3 var? if
                    $1F 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 4 var? if
                    house-of-truth-1f-6.act03
                else stage-var 5 var? if
                    $20 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 6 var? if
                    $21 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 7 var? if
                    $2E 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 8 var? if
                    $22 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 9 var? if
                    $23 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 10 var? if
                    $24 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 11 var? if
                    $25 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 12 var? if
                    $26 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 13 var? if
                    $2F 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 14 var? if
                    $27 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 15 var? if
                    $28 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 16 var? if
                    $29 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 17 var? if
                    $2A 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else stage-var 18 var? if
                    $2B 1.0 1 bgm
                    house-of-truth-1f-6.act03
                then then then then then then then then then then then then then then then then then
            then
            yield
        then
    again
; is house-of-truth-1f-6.act02

: house-of-truth-1f-6.act04 ( -- )   \ 00435670
    stage-var 32768 var-set
    self-wait-done
    0 self-through-exit
    self-wait-done
    tries-var 0 var? if
        tries-var var-inc
        stage-var 2 var-set
    else tries-var 1 var? if
        tries-var var-inc
        stage-var 3 var-set
    else tries-var 2 var? if
        stage-var 4 var-set
    then then then
    rattle-behind-slow-anim play-out
    -1 self-idle-anim
    7 wait-frames
    self-idle-or-end
;

: house-of-truth-1f-6.act05 ( -- )   \ 004356C0
    scene-done-bit ebit-set
    stage-var 32768 var-set
    0 solid-floor floor-on
    self-wait-done
    0 counter-set
    0 2 track 6 action
    8 wait-frames
    $FFFF message-close
    uneasy-anim 3 self-anim-blend
    1 wait-counter
    fiona-who knocked-sound 5 char-sound
    uneasy-2-anim $A blend-out
    -1 self-idle-anim
    7 wait-frames
    self-idle-or-end
;

: house-of-truth-1f-6.act06 ( -- )   \ 00435700
    0 house-of-truth-1f-6.cmd01
    1 house-of-truth-1f-6.cmd01
    fiona-who rumble-sound 6 char-sound
    1 $FF 8 rumble-pad
    1 counter-set
    stage-var 8 var-set
    self-idle-or-end
;

: house-of-truth-1f-6.act07 ( -- )   \ 00435720
    offer-taken ebit-set
    stage-var 7 var-set
    self-idle-or-end
;

: house-of-truth-1f-6.act08 ( -- )   \ 00435730
    hid ebit? not if
        stage-var 32768 var-set
    then
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0 counter-set
    hewie-along ebit-clear
    hewie-in-place ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who 9 action-force
        hewie-along ebit-set
        hewie-who 6 char-file-load
    then
    fiona-who 5 char-file-load
    $BD 22.68 -68.27 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    hewie-along ebit? if
        hewie-who self-look-at
        yield
        [: hewie-in-place ebit? ;] wait-until
    then
    counter-inc
    1 self-noclip
    self-who self-look-at
    yield
    1 room-motion 5 self-anim-blend8
    5 wait-frames
    0 part-hidden
    3 0 object-hidden
    4 0 object-hidden
    3 1 object-anim
    4 1 object-anim
    $E wait-frames
    fiona-who heave-sound 6 char-sound
    $3C wait-frames
    fiona-who drop-sound 6 char-sound
    self-wait-anim
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    $2D wait-frames
    hid ebit? not if
        hid ebit-set
        stage-var 5 var-set
    then
    begin
        0 2 pad? not while
        2 panic-calm-in
        6 fiona-calm
        $1E fiona-recovery-lower
        yield
    repeat
    counter-inc
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    3 2 object-anim
    4 2 object-anim
    2 room-motion 0 self-anim-blend
    $10 wait-frames
    fiona-who heave-sound 6 char-sound
    $38 wait-frames
    fiona-who drop-sound 6 char-sound
    self-wait-anim
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-6.act09 ( -- )   \ 00435820
    1 self-scripted
    self-wait-done
    $153 26.2 -63.6 -90 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    hewie-in-place ebit-set
    1 wait-counter
    1 self-noclip
    1 room-motion 5 self-anim-blend8
    self-wait-anim
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        fiona-who char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                2 room-motion 0 blend-out
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            2 room-motion 0 blend-out
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: house-of-truth-1f-6.act0A ( -- )   \ 00435870
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $85 room-preload
    $F $44 fade
    wait-fade
    world-held state-flag-set
    scene-locked state-flag-set
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-1f-6.act0B ( -- )   \ 00435890
    self-wait-done
    0 79.6 11.1 -39.7 0 flame-in
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "Come to me, Fiona... Come to me, Fiona..."
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-6 register-scripts   \ (its phases and actions, by their names)
