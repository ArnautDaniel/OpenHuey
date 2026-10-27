\ story/places/house-of-truth-1f-6.fs - the House of Truth's 1F room Lorenzo shuts her in ($8E):
\ brought here before he has had his way (her track $E not yet on) - the door slammed behind her, his
\ voice - and from then on, by stages, his taunts, each with its music and its line: the locked door
\ tried (three times), hidden, half hidden, her defence, the far door opened, a spot she examines, the
\ shelf falling, time passing (and again), standing still; the hiding place behind the cabinet (Hewie
\ hidden beside her, if he is by her); late, the far door open, as she steps away - the scene ("Come to
\ me, Fiona...": the door opened), then out by the forced way; the flame stirred; the replay. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-6.fs
\ (converted from the game's, not loaded).
IN: story.places.house-of-truth-1f-6
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags story.kit story.places.house-of-truth fiona.anims fiona.sounds scenes keys ;

own-reset  truth-owns owns !

0 constant come-msg        \ "Come to me, Fiona... Come to me, Fiona..." (the scene's)
1 constant naughty-msg     \ "You've been very naughty, Fiona. You are causing me much grief."
\ (his taunts: messages 2 .. $12, by stage - "What are you doing, Fiona? Just where do you intend to run?" ...)
own: door-opened   own: half-hid   own: defended   own: waited-out   own: far-door-opened   own: spot-taken
own: hid           own: shelf-done own: shelf-down own: hewie-along own: hewie-placed own: late          own: far-open
own: stepped-away  own: went-on
own: stage                 \ his taunt to come (-1 none; 2 .. 18; busy: a scene or a try under way)
own: time-here             \ frames since the door slammed (-1: not yet)
own: tries                 \ her tries at the locked door
own: still                 \ frames she has stood still
own: hiding-step           \ the hiding place: 1 she's in, 2 out
-1 constant no-stage   32768 constant busy-stage
0 constant flame-fx   $A constant shelf-area   9 constant hiding-area   $B constant spot-area   8 constant away-area
0 constant rumble-sound   1 constant heave-sound   2 constant drop-sound   5 constant her-motions   6 constant his-motions
1 constant come-movie   0 constant come-cutscene   $E9 constant slammed-door   $49 constant come-seen   $2C constant slam-track
create stage-tracks   $1E , $1F , 0 , $20 , $21 , $2E , $22 , $23 , $24 , $25 , $26 , $2F , $27 , $28 , $29 , $2A , $2B ,   \ (stages 2 .. 18; 0 none)
: flame ( -- )  flame-fx 0 79.7e 11.1e -39.8e flame-at ;
: track-wanted ( track -- )  1 1e f>cell music send bgm-want ;   \ (made ready, held till his line)
: track-on ( -- )  music send bgm-resume ;

\ ---- the door, the shelf ----
: door-swung ( F: deg -- )  0 true door-swing ;
: door-slams ( -- )   \ (from -90 swung shut 10 degrees a frame)
    -90 begin  10 + 0 min  dup s>f door-swung  dup 0< while  yield  repeat  drop ;
: shelf ( -- addr len )  s" tana" ;
: shelf-falls ( -- )   \ (its angle on by speed, speed by its push, push by a jerk of 0.005 - tilted till it is over at 90)
    shelf placed-here? 0= if  exit  then
    0e shelf $30 object!  0e shelf $34 object!  0e shelf $38 object!  0.005e shelf $3C object!
    begin
        shelf $3C object@ shelf $38 object+!  shelf $38 object@ shelf $34 object+!  shelf $34 object@ shelf $30 object+!
        90e  shelf $30 object@ 90e fmin  f- deg>rad fsin  1e fswap f-  pi f2/ f*  shelf turn-x object!
        shelf $30 object@ 90e f< while  yield
    repeat ;

\ ---- his taunts, by stage ----
: taunt ( -- )   \ (his line for the stage, its music on)
    scene-locked state!  yield  track-on  stage @ say
    stage @ busy-stage <> if  no-stage stage !  then  scene-locked state-off ;
: stage-by-time ( -- )   \ (his taunts as time passes - after 2250 she is late; then again and again)
    1 time-here +!  time-here @ case
        450 of  9 stage !  endof   900 of  10 stage !  endof   1350 of  11 stage !  endof   1800 of  12 stage !  endof
        2250 of  -1 late !  13 stage !  endof   2700 of  14 stage !  endof   3150 of  15 stage !  2250 time-here !  endof
    endcase ;
: standing-still ( -- )   \ (300 frames without a move, a shake or a gesture: the last taunt)
    waited-out @  stage @ no-stage <> or if  exit  then
    fiona held? 0=  shook-now @ 0= and  fiona-cmd-now @ -1 = and if
        1 still +!  still @ 300 = if  -1 waited-out !  18 stage !  then
    else  0 still !  then ;
: taunts ( -- )   \ (an activity: for good)
    begin
        stage @ busy-stage <> if
            time-here @ 0< 0= if
                stage @ no-stage = if  stage-by-time  then
                standing-still
                stage @ 2 19 within if  stage @ 2 - cells stage-tracks + @ ?dup if  track-wanted  then  taunt  then
            then
        then  yield
    again ;
: stages ( -- )   \ (each frame: what she does setting his next taunt)
    stage @ no-stage <> if  exit  then
    hid @ 0=  fiona-hidden state? and if  -1 hid !  5 stage !  then
    half-hid @ 0=  fiona-half-hidden state? and if  -1 half-hid !  16 stage !  then
    defended @ 0=  fiona-doing-now @ 8 = and if  -1 defended !  17 stage !  then
    far-door-opened @ 0=  1 exit-open? and if  -1 far-door-opened !  6 stage !  then ;

\ ---- brought here: the door slammed behind her, his voice ----
: shut-in ( -- )
    fiona hold  stalker-unloaded  lorenzo2-kind stalker-ready  fiona wait-move  slam-track track-wanted
    door-slams  $28 common-bank fiona sound-by  slammed-door false open-mark  slammed-door lock  doors-redone
    seized-sound common-bank fiona sound-by  fiona look-behind-anim 3 play-blend
    fiona 0 to-door  fiona rattle-behind-slow-anim start-anim  16 frames
    naughty-msg show  track-on  fiona wait-anim  fiona -1 idle  7 frames  fiona rest-anim 10 play-blend  wait-closed
    0 time-here !  0 still ! ;

\ ---- the locked door tried: his taunts 2, 3, then 4 ----
: tries-door ( -- )
    busy-stage stage !  fiona 0 to-door
    tries @ 2 < if  tries @ 2 +  1 tries +!  else  4  then  stage !
    fiona rattle-behind-slow-anim play  fiona -1 idle  7 frames ;
\ ---- a spot she examines: his taunt 7 ----
: the-spot ( -- )  -1 spot-taken !  7 stage ! ;
\ ---- the shelf falling as she comes to it: his taunt 8 ----
: falls-over ( -- )  shelf-falls  rumble-sound room-bank fiona sound-by  1 $FF 8 rumble-pad  -1 shelf-down !  8 stage ! ;
: shelf-scene ( -- )
    -1 shelf-done !  busy-stage stage !  0 solid-floor floor-on  fiona wait-move  0 shelf-down !
    ['] falls-over start drop  8 frames  text-gone  fiona uneasy-anim 3 start-blend
    begin  shelf-down @ 0= while  yield  repeat
    knocked-sound common-bank fiona sound-by  fiona uneasy-2-anim 10 play-blend  fiona -1 idle  7 frames ;

\ ---- the hiding place behind the cabinet (Hewie hidden beside her if he is by her) ----
: cabinet ( op -- )  >r  3 1 r@ object  4 1 r> object ;   \ (its two doors' animation)
: hewie-hides ( -- )
    hewie hold  hewie $153 -90 -1 walk-path-move 26.2e -63.6e go-to  hewie use-motions  -1 hewie-placed !
    begin  hiding-step @ 1 < while  yield  repeat
    hewie true noclip  hewie 1 room-motion + 5 start-blend8  hewie wait-anim  hewie false noclip  hewie-hidden state!
    begin  fiona held?  hiding-step @ 2 < and while  yield  repeat
    hewie-hidden state-off  hewie true noclip  hewie 2 room-motion + 0 play-blend  hewie false noclip ;
: hides ( -- )
    hid @ 0= if  busy-stage stage !  then
    stalkers-stay state!  fiona wait-move  0 hiding-step !  0 hewie-along !  0 hewie-placed !
    hewie held? 0=  hewie-near-now @ and  fiona hewie 30e near-her? and if
        -1 hewie-along !  hewie his-motions motions-for  ['] hewie-hides hewie start-holding drop
    then
    fiona her-motions motions-for  fiona $BD -90 -1 walk-move 22.68e -68.27e go-to  fiona use-motions
    hewie-along @ if  fiona hewie look-at  yield  begin  hewie-placed @ 0= while  yield  repeat  then
    1 hiding-step !  fiona true noclip  fiona -1 look-at  yield
    fiona 1 room-motion + 5 start-blend8  5 frames  0 hide-part  3 object-show  4 object-show  1 cabinet
    14 frames  heave-sound room-bank fiona sound-by  60 frames  drop-sound room-bank fiona sound-by  fiona wait-anim
    fiona false noclip  stalkers-stay state-off  fiona-hidden state!  0 avoid-prompt  45 frames
    hid @ 0= if  -1 hid !  5 stage !  then
    begin  circle button-pressed? 0= while  2 ease-panic  6 calm-her  $1E recover  yield  repeat
    2 hiding-step !  fiona follow  fiona-hidden state-off  fiona true noclip  2 cabinet
    fiona 2 room-motion + 0 start-blend  16 frames  heave-sound room-bank fiona sound-by  56 frames
    drop-sound room-bank fiona sound-by  fiona wait-anim  fiona false noclip ;

\ ---- late, the far door open, as she steps away: "Come to me, Fiona..." ----
: come-scene ( -- )  come-movie scenes:unseen-movie come-cutscene load-scene drop  panic-eased  come-msg run-scene ;
: come-to-me ( -- )
    busy-stage stage !  ['] taunts stop-doing  stalker let-go  0 summon-take
    scene-locked state!  stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    come-scene  fiona $1A -21 0.49e -36.88e place-at  0 0 player-camera  camera-restart
    -1 door-opened !  slammed-door true open-mark  slammed-door unlock  doors-redone
    15 fade-in  come-seen resident-flag-set  stalkers-stay state-off  scene-locked state-off ;
: goes-on ( -- )   \ (after it, her step through the door: on by the forced way)
    stalkers-stay state!  house-of-truth-b1-2 preload  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    world-held state!  scene-locked state!  forced-exit take-exit ;
: replayed ( -- )  fiona hold  flame-fx 0 79.6e 11.1e -39.7e flame-at  come-scene  replay-over ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,  6 , 0 , 0 ,  7 , 2 , 2 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
: before-him ( -- )   \ (her track $E not yet on: his game)
    door-opened @ 0= if
        far-open @  late @ and  fiona away-area left? and  stepped-away @ 0= and if
            -1 stepped-away !  panic-eased  ['] come-to-me fiona start-holding drop
        then
    else  fiona 0 entered?  went-on @ 0= and if  -1 went-on !  panic-eased  ['] goes-on fiona start-holding drop  then  then
    shelf-done @ 0=  far-open @ and  fiona shelf-area in-area? and  fiona 0 on-group? 0= and  fiona held? 0= and if
        ['] shelf-scene fiona start-holding drop
    then
    stages  1 exit-open? if  -1 far-open !  then ;
: spots ( -- )
    fiona hiding-area in-area?  fiona -45 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    request-now @ 2 =  fiona 0 before-door? and if  ['] tries-door any-time offer  then
    over-music-e fact? 0=  stage @ no-stage = and  spot-taken @ 0= and
    fiona spot-area in-area? and  fiona 0 50 heading? and if  ['] the-spot unhurried offer  then ;
: each-frame ( -- )
    over-music-e fact? if  0 exit-out  then  1 exit-out  cameras 4 cameras-at  ahead 2 ways-out
    over-music-e fact? 0= if  before-him  then
    flame-fx 79.7e 11.1e -39.8e 8e -4e flame-stirred
    spots ;
: came-in ( exit -- )
    drop  room-sounds  -1 time-here !  no-stage stage !  flame
    over-music-e fact? 0= if  ['] taunts start drop  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    0 0 0 way-in
    over-music-e fact? 0= if  fiona $85 180 0e 10e place-at  ['] shut-in start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-6 place!
