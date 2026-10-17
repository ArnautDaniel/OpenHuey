\ story/rooms/house-of-truth-1f-6.fs - the event scripts of room house-of-truth-1f-6 ($8E; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-6
USING: engine common actors messages room-names story.words story.shared flag-names story-names ;

\ door 0 swung by script variable 0: byte 3 0 sets it to -90; 1 opens it 10 degrees a step to 0
\ (doors +0x74), waiting (2) until there
: house-of-truth-1f-6.cmd00 ( b0 -- )   \ (door 0 by variable 0: b0 0 it to -90; 1 opened 10 degrees a frame from there to 0)
    case
        0 of  0 -90 var-set  endof
        1 of  begin
                  0 var @ 10 + 0 min  dup 0 swap var-set
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
    2 -1 var-set
    1 -1 var-set
    over-music-e story-flag? not if
        0 $F1 2 action
    then
    0 79.7 11.1 -39.8 0 flame-in
;

: house-of-truth-1f-6.char-enter ( -- )   \ 00435080
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
        0 ebit? not if
            $B ebit? $A ebit? and if
                fiona-who 8 char-left-area? $C ebit? not and if
                    $C ebit-set
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 0 action-force
                    else
                        1 fiona-who 0 action-force
                    then
                then
            then
        else fiona-who 0 char-entered-area? $D ebit? not and if
            $D ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who $A action-force
            else
                1 fiona-who $A action-force
            then
        then then
        7 ebit? not $B ebit? and if
            fiona-who $A char-in-area? if
                fiona-who 0 char-in-nav-group? not if
                    0 fiona-who 5 action
                then
            then
        then
        1 -1 var? if
            6 ebit? not if
                fiona-hidden state-flag? if
                    6 ebit-set
                    1 5 var-set
                then
            then
            1 ebit? not if
                fiona-half-hidden state-flag? if
                    1 ebit-set
                    1 16 var-set
                then
            then
            2 ebit? not if
                fiona-who 8 char-action? if
                    2 ebit-set
                    1 17 var-set
                then
            then
            4 ebit? not if
                1 exit-door-open? if
                    4 ebit-set
                    1 6 var-set
                then
            then
        then
        1 exit-door-open? if
            $B ebit-set
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
        1 -1 var? if
            5 ebit? not if
                fiona-who $B char-in-area? fiona-who 0 $32 char-heading? and if
                    7 unhurried offer-action
                then
            then
        then
    then
;

: house-of-truth-1f-6.act00 ( -- )   \ 00435250
    1 32768 var-set
    $F1 action-end
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
    0 ebit-set
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
    fiona-who $43 5 char-sound
    $F01 3 self-anim-blend
    self-wait-anim
    0 self-through-exit
    self-wait-done
    $60B self-anim
    self-frames-reset
    self-wait-16
    begin
        0 adx? not while
        yield
    repeat
    1 message   \ "You've been very naughty, Fiona. You are causing me much grief."
    0 0.0 $FF bgm
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    1 $A self-anim-blend
    self-wait-anim
    wait-message
    begin
        1 adx? not while
        yield
    repeat
    2 0 var-set
    4 0 var-set
    2 char-start
    self-idle-or-end
;

: house-of-truth-1f-6.act03 ( -- )   \ 00435570
    scene-locked state-flag-set
    yield
    begin
        0 adx? not while
        yield
    repeat
    0 0.0 $FF bgm
    1 2 var? if
        2 message   \ "What are you doing, Fiona? Just where do you intend to run?"
        wait-message
    else 1 3 var? if
        3 message   \ "How adorable you are, Fiona."
        wait-message
    else 1 4 var? if
        4 message   \ "Damn, it's locked!"
        wait-message
    else 1 5 var? if
        5 message   \ "Do you really think you can hide from me there?"
        wait-message
    else 1 6 var? if
        6 message   \ "Aha. Looks like a dead end, dear. How unfortunate."
        wait-message
    else 1 7 var? if
        7 message   \ "I can see you clearly from here, Fiona..."
        wait-message
    else 1 8 var? if
        8 message   \ "That was a close one, eh Fiona?"
        wait-message
    else 1 9 var? if
        9 message   \ "Oh, how I love thee, Azoth!"
        wait-message
    else 1 10 var? if
        $A message   \ "You simply cannot understand."
        wait-message
    else 1 11 var? if
        $B message   \ "Why must you run from me?"
        wait-message
    else 1 12 var? if
        $C message   \ "Everything I do is for the Great Truth!"
        wait-message
    else 1 13 var? if
        $D message   \ "At last! You are mine, Fiona!"
        wait-message
    else 1 14 var? if
        $E message   \ "My dear Fiona..."
        wait-message
    else 1 15 var? if
        $F message   \ "I will have you in my grasp soon enough."
        wait-message
    else 1 16 var? if
        $10 message   \ "Have you longed for me as I long for you?"
        wait-message
    else 1 17 var? if
        $11 message   \ "No need to get so upset, my dear."
        wait-message
    else 1 18 var? if
        $12 message   \ "Well? Have you given up?"
        wait-message
    then then then then then then then then then then then then then then then then then
    begin
        1 adx? not while
        yield
    repeat
    1 32768 var? not if
        1 -1 var-set
    then
    scene-locked state-flag-clear
    ['] house-of-truth-1f-6.act02 goto
;

:noname   \ house-of-truth-1f-6.act02 (00435390; deferred: used before it is defined)
    begin
        1 32768 var? if
            yield
        else
            2 -1 var? not if
                1 -1 var? if
                    2 var-inc
                    2 450 var? if
                        1 9 var-set
                    else 2 900 var? if
                        1 10 var-set
                    else 2 1350 var? if
                        1 11 var-set
                    else 2 1800 var? if
                        1 12 var-set
                    else 2 2250 var? if
                        $A ebit-set
                        1 13 var-set
                    else 2 2700 var? if
                        1 14 var-set
                    else 2 3150 var? if
                        1 15 var-set
                        2 2250 var-set
                    then then then then then then then
                then
                3 ebit? not if
                    1 -1 var? if
                        fiona-who char-busy? not $FF $FF pad? not and -1 control-action? and if
                            4 var-inc
                            4 300 var? if
                                3 ebit-set
                                1 18 var-set
                            then
                        else
                            4 0 var-set
                        then
                    then
                then
                1 2 var? if
                    $1E 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 3 var? if
                    $1F 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 4 var? if
                    house-of-truth-1f-6.act03
                else 1 5 var? if
                    $20 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 6 var? if
                    $21 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 7 var? if
                    $2E 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 8 var? if
                    $22 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 9 var? if
                    $23 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 10 var? if
                    $24 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 11 var? if
                    $25 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 12 var? if
                    $26 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 13 var? if
                    $2F 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 14 var? if
                    $27 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 15 var? if
                    $28 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 16 var? if
                    $29 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 17 var? if
                    $2A 1.0 1 bgm
                    house-of-truth-1f-6.act03
                else 1 18 var? if
                    $2B 1.0 1 bgm
                    house-of-truth-1f-6.act03
                then then then then then then then then then then then then then then then then then
            then
            yield
        then
    again
; is house-of-truth-1f-6.act02

: house-of-truth-1f-6.act04 ( -- )   \ 00435670
    1 32768 var-set
    self-wait-done
    0 self-through-exit
    self-wait-done
    3 0 var? if
        3 var-inc
        1 2 var-set
    else 3 1 var? if
        3 var-inc
        1 3 var-set
    else 3 2 var? if
        1 4 var-set
    then then then
    $60B self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: house-of-truth-1f-6.act05 ( -- )   \ 004356C0
    7 ebit-set
    1 32768 var-set
    1 0 8 nav-group
    self-wait-done
    0 counter-set
    0 $F2 6 action
    self-frames-reset
    8 self-wait-frames
    $FFFF message-close
    $F03 3 self-anim-blend
    1 wait-counter
    fiona-who $3E 5 char-sound
    $F04 $A self-anim-blend
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: house-of-truth-1f-6.act06 ( -- )   \ 00435700
    0 house-of-truth-1f-6.cmd01
    1 house-of-truth-1f-6.cmd01
    fiona-who 0 6 char-sound
    1 $FF 8 rumble
    1 counter-set
    1 8 var-set
    self-idle-or-end
;

: house-of-truth-1f-6.act07 ( -- )   \ 00435720
    5 ebit-set
    1 7 var-set
    self-idle-or-end
;

: house-of-truth-1f-6.act08 ( -- )   \ 00435730
    6 ebit? not if
        1 32768 var-set
    then
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0 counter-set
    8 ebit-clear
    9 ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who 9 action-force
        8 ebit-set
        hewie-who 6 char-file-load
    then
    fiona-who 5 char-file-load
    $BD 22.68 -68.27 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    8 ebit? if
        hewie-who self-look-at
        yield
        begin
            9 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    self-who self-look-at
    yield
    $8001 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    0 0 $14 door-bits
    3 0 object-hidden
    4 0 object-hidden
    3 1 object-anim
    4 1 object-anim
    self-frames-reset
    $E self-wait-frames
    fiona-who 1 6 char-sound
    self-frames-reset
    $3C self-wait-frames
    fiona-who 2 6 char-sound
    self-wait-anim
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    self-frames-reset
    $2D self-wait-frames
    6 ebit? not if
        6 ebit-set
        1 5 var-set
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
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    fiona-who 1 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    fiona-who 2 6 char-sound
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
    9 ebit-set
    1 wait-counter
    1 self-noclip
    $8001 5 self-anim-blend8
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
                $8002 0 self-anim-blend
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            $8002 0 self-anim-blend
            self-wait-anim
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
