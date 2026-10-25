\ stalker/riccardo.fs - Riccardo's own (src/game/riccardo.c): his behaviour and chase
\ (Riccardo_StateBehaviour / Riccardo_Chase), his flurry of blows with his gun (Riccardo_StartFlurry,
\ FlurryBlow, StateBlow / StateBlowFiona / StateBlowHewie, StateAfterBlow), his lunge
\ (Riccardo_StartLunge), his attack by how near (Riccardo_AttackForDistance) and the impact of his
\ gun's shots (Riccardo_Impact). Her panic calming down he chases as the Pursuer does (stalker/pursue.fs).
\ Seeing her: further than 100 he holds off; near (60) and seen by her he stands to her, coming on
\ a few times (his waits' count) before walking at her; else by a roll he walks at her or stalks
\ her for his waits' frames, and when they run out within 100 lunges (by his lunge table) or
\ starts again. Within his reach (20), in front of him, on her floor: his attack table. His
\ flurry: 1..7 blows by his chances; on her floor a swing, not on it a slam down at her (one
\ blow); at Hewie on his floor his swing, else he gives up. A blow at its key: whoever it
\ reaches - Hewie in the way of her, or her (by how near: his grab within 10, caught - below the
\ panic's height - or a hard blow; crouched under it, it passes over) - and else its debris
\ where it lands. Its impact: a noise where he stands.
IN: stalker.riccardo
USING: engine game-state actors messages common facts flag-names paths vectors stalker-names stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.names stalker.anims stalker.sounds effects floor-flags ;

\ his waits (his kit's): frames walking, stalking, walking once seen, stalking when far, the
\ times he comes on; then the chance to walk
enum: wait#  walk-wait stalk-wait seen-walk-wait far-stalk-wait comings-wait ;
: wait# ( i -- n )  cells waits + @ ;
: wait-chance ( F: -- c )  5 cells waits + f@ ;
: come-reach ( F: -- r )  my-kind @ kind37-kind = if  30e  else  60e  then ;   \ (+0x17C0: his other class's 30)
: calming? ( -- flag )  her-stage @ calming-down = ;
\ his animations: his swings, his shots aimed down (his taunt's too), his heavy blows, his lunge
$E00 constant swing-anim   $E04 constant swing-2-anim   $E05 constant heavy-anim   $E06 constant aim-down-anim
$2301 constant shot-down-anim   $2303 constant shot-down-2-anim   $1300 constant lunge-anim
\ his grunts as he swings (bank 7): at her, at her again, at Hewie; his snort (bank 5)
$22 constant swing-grunt   $23 constant swing-2-grunt   $15 constant dog-grunt   $24 constant snort-sound

\ ---- his chase (Riccardo_Chase) ----
defer ric-begin ( -- )
: ric-back-or-hold ( F: roll -- )   \ (Riccardo_BackOrHold)
    wait-chance f<= if  walk-at-act walk-wait wait# hold-for  else  stalk-act stalk-wait wait# hold-for  then ;
: lunge? ( -- flag )   \ (Riccardo_Lunge: by his lunge table)
    rnd 100e f*  lunge-chance s>f f< if  lunge-situation choose true  else  false  then ;
: ric-strike-or-wait ( -- )  lunge? if  exit  then  rnd 100e f* ric-back-or-hold ;
: ric-stand-to ( -- )  stand-watch  comings-wait wait# comings !  0 hold-t ! ;
: near-seen? ( -- flag )  d-her come-reach f<=  faces-me? and ;
: standing-step ( -- )
    chase-t @ 60 mod 0= if  lunge? if  exit  then  then
    settled? if  fiona body-at 60e deg>rad 150e deg>rad way-past dup no-turn <> if  turn-to-her  else  drop  then  then
    rnd 100e f*
    d-her f0< if  fdrop  out-of-reach 0= if  search-again  then  exit  then   \ (no way to her: Pursuer_ChaseFionaHere)
    faces-me? 0=  d-her come-reach 20e f+ f> or if  ric-back-or-hold exit  then  fdrop
    comings @ 0> if  -1 comings +!  else  walk-at-act seen-walk-wait wait# hold-for  then ;
: stalking-step ( -- )
    near-seen? if  comings @ 0> if  ric-stand-to  else  walk-at-act seen-walk-wait wait# hold-for  then  exit  then
    hold-t @ 0>  d-her 100e f< 0= or if  -1 hold-t +!  exit  then
    ric-strike-or-wait ;
: walking-step ( -- )
    near-seen? comings @ 0> and if  ric-stand-to exit  then
    hold-t @ 0> if  d-her 100e f<= if  -1 hold-t +!  else  stalk-act far-stalk-wait wait# hold-for  then  exit  then
    ric-strike-or-wait ;
: ric-run ( -- )
    1 chase-t +!
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    calming? moving-act? and if  pursue-now exit  then   \ (her panic calming down: the Pursuer's chase)
    chase-act @ case
        watch-act of  standing-step  endof
        walk-at-act of  walking-step  endof
        stalk-act of  stalking-step  endof
        round-act of  d-her 100e f> if  stalk-act far-stalk-wait wait# hold-for  else  step-done @ if  stand-watch  then  then  endof
        >r  step-done @ if  ric-begin  then  r>
    endcase
    moving-act? if  in-reach? if  attack!  then  then ;
\ his behaviour (Riccardo_ChaseDecision / StateBehaviour): the Pursuer's while her panic calms down
:noname ( -- )
    calming? if  pursue-now exit  then
    no-search search-phase !  ['] ric-run step!  0 chase-t !  0 hold-off !  -1 combo !
    sees-fiona @ 0= if  chase-start exit  then
    d-her 100e f> if  stalk-act far-stalk-wait wait# hold-for
    else  near-seen? if  stand-watch  0 hold-t !  else  rnd 100e f* ric-back-or-hold  then  then
    comings-wait wait# comings ! ; is ric-begin

\ ---- his attack by how near (Riccardo_AttackForDistance): a chance to attack at all by the
\ distance; at Hewie a hard blow; at her caught (below the panic's height), a hard blow or his grab
\ by the distance's chances. 0: none ----
: early? ( -- flag )  her-stage @ panicking < ;
: attack-for ( F: d -- k )
    0e 0e 0e {: F: grab-c blow-c catch-c :}
    fdup 10e f< if  fdrop 100e  100e to grab-c
    else fdup 30e f< if  fdrop 50e  early? if  50e to catch-c  then  100e to blow-c
    else fdup 60e f< if  fdrop 25e  early? if  75e to catch-c  then  100e to blow-c
    else fdup 90e f< if  fdrop 15e  early? if  75e to catch-c  then  100e to blow-c
    else 150e f< if  10e  early? if  100e to catch-c  else  100e to blow-c  then
    else  0e  then then then then then
    rnd 100e f* f> 0= if  0 exit  then
    rnd 100e f*  fdup catch-c f<= if  fdrop hold-request exit  then
    fdup blow-c f<= if  fdrop throw-request exit  then  grab-c f<= if  grab-request  else  0  then ;

\ ---- his blows ----
: blow-entry ( -- addr )  blow-move @ move-of ;
: blow-key? ( -- flag )  my-model @ 0 -1 1 motion-events 2 and 0<> ;
: line-dist ( a b p -- ) ( F: -- d )   \ (Vec_LineDistance: p off the line a - b, across the floor)
    {: a b p :}
    b v.x sf@ a v.x sf@ f-  b v.z sf@ a v.z sf@ f-  {: F: lx lz :}
    p v.x sf@ a v.x sf@ f-  p v.z sf@ a v.z sf@ f-  {: F: px pz :}
    lx pz f*  lz px f* f- fabs
    lx fdup f*  lz fdup f* f+ fsqrt  fdup f0= if  fdrop  else  f/  then ;
\ ---- where his shot strikes (Riccardo_BlowFloorPoint): aimed down 10 out along his gun, on the
\ floor there; aimed level straight ahead to where the floor gives out, 15 up - none against a
\ wall. Its debris there: a brown cloud (Riccardo_Debris) ----
$31 constant gun-bone
create hit-at /vec allot   create probe /vec allot
: slam? ( -- flag )  anim @ dup shot-down-anim = over shot-down-2-anim = or over aim-down-anim = or  swap taunt-anim = or ;
: swing? ( -- flag )  anim @ dup swing-anim = swap swing-2-anim = or ;
: down-the-gun ( -- ok? )
    my-model @ gun-bone -10e 0e 0e bone-point hit-at vec!
    hit-at vec@ nav-floor if  hit-at v.y sf!  true  else  fdrop false  then ;
: floor-ahead ( -- ok? )   \ (2 at a time, 200 at most)
    hit-at my-at vec-copy
    100 1 do
        probe my-at  yaw i 2* s>f vec-ahead
        probe floor-edge v-tri-in  dup 0< if  drop  hit-at v.y sf@ 15e f+ hit-at v.y sf!  true unloop exit  then
        nav-flags wall-floor and if  false unloop exit  then
        hit-at probe vec-copy
    loop  false ;
: floor-hit ( -- ok? )
    slam? if  down-the-gun exit  then  swing? if  floor-ahead exit  then  false ;
: floor-debris ( -- )  floor-hit if  $80 $50 $40 hit-at vec@ debris-cloud  then ;
\ his shot (Riccardo_Impact: at his animation's key $20): smoke off his gun (its strike mark), a
\ noise where he stands, its debris from one aimed down; his grab marks her
defer cry-hit ( who -- )
$40 constant shot-noise
: impact ( -- )
    effect-key? 0= if  exit  then
    my-model @ gun-bone bone-pos gun-smoke
    shot-noise room-id self body-tri -1 stalker-noise acoustics-id send noise
    anim @ dup shot-down-anim = over shot-down-2-anim = or swap taunt-anim = or if  floor-debris  then
    anim @ carry-anim = if  fiona cry-hit  then ;
\ Hewie struck: the move's kind and damage
: strike-hewie ( -- )  blow-entry m.kind @  blow-entry m.damage @  0  hewie send blow ;
\ her struck by k (caught: the heavy move with its stun - from his swing the move 6, his second
\ swing the move 7, his swing at Hewie the move 8 - and its debris; a hard blow: a stumble three
\ times in four)
: heavy ( n -- n' )  case  0 of 6 endof  3 of 7 endof  1 of 8 endof  dup  endcase ;
: strike-her ( k -- )
    dup 0= if  drop floor-debris exit  then
    0 {: stun :}
    dup hold-request = if
        blow-move @ heavy blow-move !
        blow-entry m.bone @ $FFFF and  dup $8000 and if  $10000 -  then  to stun  floor-debris   \ (its bone field: the stun)
    then
    dup throw-request = if  rnd 0.75e f< if  stumbles to stun  then  then
    stun  blow-entry m.threat f@ f>cell  fiona send hit ;
: within-heading? ( id -- flag ) ( F: a -- )   \ within a of his heading
    body-at heading-to yaw f- angle-wrap fabs f> ;
: dog-floor? ( -- flag )
    hewie body-at v.y sf@  my-at v.y sf@  {: F: hy my :}  hy my 15e f+ f<=  hy my 10e f- f>= and ;
: d-dog ( F: -- d )  my-at hewie body-at vec-dist ;
\ she in the way of a blow at Hewie: within 4 of the line to him, and no further than he is
: her-in-way? ( -- flag )
    my-at hewie body-at fiona body-at line-dist 4e f<=  d-dog d-her f< 0= and ;
\ (Riccardo_OutOfLine) she's clear of a blow at Hewie: off its line, or further than he is
: her-clear? ( -- flag )  her-in-way? 0= ;

\ ---- the flurry ----
defer flurry-blow ( -- )
: blow-over ( -- )  0 blow-aim !  -1 step-done ! ;
: after-blow ( -- )   \ (Riccardo_StateAfterBlow: another while it's in front, 90 degrees)
    -1 blows +!
    blows @ 0> 0= if  blow-over exit  then
    at-dog @ if
        sees-hewie @ 0=  d-hewie f@ 30e f> or if  blow-over exit  then
        hewie  pi f2/ within-heading? 0= if  blow-over exit  then
    else
        sees-fiona @ 0=  d-her 100e f> or  calming? or if  blow-over exit  then
        fiona  pi f2/ within-heading? 0= if  blow-over exit  then
    then
    flurry-blow ;
: grunt ( id -- )   \ (at the motion's key 1, half the time)
    my-model @ 0 0 1 motion-events 1 and  rnd 0.5e f< and if  7 sound  else  drop  then ;
: hold-move? ( -- flag )  blow-entry m.kind @ hold-request = ;
\ a hold (Riccardo_Blow): at its key, whoever is within its reach
: blowing ( -- )
    blow-key? if
        quarry body-at my-at vec-dist  blow-entry m.reach f@ f< if
            at-dog @ if  strike-hewie
            else  blow-entry m.kind @  blow-entry m.bone @ $FFFF and  blow-entry m.threat f@ f>cell  fiona send hit  then
        then
    then
    ended? if  after-blow exit  then  root-move ;
\ at Hewie (Riccardo_StateBlowHewie)
: blowing-dog ( -- )
    blow-key? if
        0 blow-aim !
        dog-floor?  sees-hewie @ and  d-hewie f@ 30e f< and  hewie 30e deg>rad within-heading? and if
            her-in-way? if  d-her attack-for strike-her
            else  d-hewie f@ attack-for if  strike-hewie  else  floor-debris  then  then
        then
    then
    blow-aim @ if  hewie body-at turn-rate turn-to fdrop  then
    ended? if  after-blow exit  then
    dog-grunt grunt  root-move ;
\ a target of the story's by her (Events_NearestPoint: a zone of a kind with bit 1) his shot
\ strikes instead - within 50 of her, further than 20 from him, about his height (10 under him to
\ 15 over), a straight way to it, she and Hewie off the line to it or beyond it. Struck, it is his
\ point (Pursuer_AttackPoint) a few frames: the story's zones see it there
create shot-at /vec allot
: target# ( i -- v )  3 * floats target-at + ;
: nearest-target ( -- i | -1 )
    -1 0e {: best F: best-d :}
    #targets 0 do
        i cells target-on + @ if
            i target# fiona body-at vec-dist
            best 0<  fdup best-d f< or if  to best-d  i to best  else  fdrop  then
        then
    loop  best ;
: off-line? ( id -- flag )   \ (Riccardo_OutOfLine: off the line to the target, or beyond it)
    {: who :}  my-at shot-at who body-at line-dist 4e f>  my-at shot-at vec-dist  my-at who body-at vec-dist f< or ;
: target-shot? ( -- flag )
    nearest-target dup 0< if  drop false exit  then  target# shot-at swap vec-copy
    shot-at fiona body-at vec-dist 50e f<  my-at shot-at vec-dist 20e f> and 0= if  false exit  then
    my-at v.y sf@ shot-at v.y sf@ f-  fdup 10e f<  -15e f> and 0= if  false exit  then
    self body-tri my-at shot-at 0 v-walk 0< if  false exit  then
    fiona off-line? 0= if  false exit  then
    hewie here? if  hewie off-line? 0= if  false exit  then  then
    true ;
: shoot-target ( -- )  shot-at vec@ body-point!  3 shot-t ! ;
: shot-fades ( -- )   \ (his point gone again)
    shot-t @ 0= if  exit  then  -1 shot-t +!  shot-t @ 0= if  body-point-off  then ;
\ at her (Riccardo_StateBlowFiona): a target by her takes it; Hewie in the way takes it; crouched
\ under it, it passes over
: crouched? ( -- flag )   \ (her top below his gun, near)
    d-her 20e f<  fiona body-at v.y sf@ fiona body-dims fswap fdrop f+  my-at v.y sf@ 5e f+ f< and ;
: blowing-her ( -- )
    blow-key? if
        0 blow-aim !
        sees-fiona @ same-floor? and  fiona 30e deg>rad within-heading? and  target-shot? and if
            shoot-target
        else  sees-fiona @ same-floor? and  fiona 30e deg>rad within-heading? and if
            hewie here?  fiona body-at hewie body-at vec-dist 50e f< and  d-hewie f@ 20e f> and
            dog-floor? and  her-clear? and if
                d-hewie f@ attack-for if  strike-hewie  else  floor-debris  then
            else
                crouched? if  floor-debris  else  d-her attack-for strike-her  then
            then
        then  then
    then
    blow-aim @ if  fiona body-at turn-rate turn-to fdrop  then
    ended? if  after-blow exit  then
    blow-move @ case  0 of swing-grunt grunt endof  3 of swing-2-grunt grunt endof  endcase  root-move ;
:noname ( -- )   \ (Riccardo_FlurryBlow: his swings 0 / 3, at Hewie 1, slams down 8 / 9)
    at-dog @ if
        dog-floor? 0= if  0 blows !  stand-watch exit  then  1 blow-move !
    else
        same-floor? if  rnd 0.5e f< if  0  else  3  then
        else  rnd 0.5e f< if  9  else  8  then  1 blows !  then  blow-move !
    then
    blow-entry m.anim @ play-now  -1 blow-aim !
    hold-move? if  ['] blowing
    else  at-dog @ if  ['] blowing-dog  else  ['] blowing-her  then  then  own-act-1 act! ; is flurry-blow
: more? ( n -- n flag ) ( F: roll -- roll )
    dup 6 < if  dup floats flurry-chances + f@ fover f<  else  false  then ;
: flurry-start ( -- )   \ (Riccardo_StartFlurry: 1..7 blows by his chances)
    world-held state-flag?  stalkers-blind state-flag? or if  stand-watch exit  then
    rnd 100e f*  0  begin  more?  while  1+  repeat  fdrop  1+ blows !  flurry-blow ;

\ ---- the one he struck cries out (Riccardo_CryHit): in his flurry (not a hold) a mark at a height
\ on them at random; his heavy blows a big one high on them ----
:noname ( who -- )
    dup body? 0= if  drop exit  then
    chase-act @ own-act-1 = if
        hold-move? if  drop exit  then
        0  0.4e rnd 0.5e f* f+  mark-on exit
    then
    anim @ dup heavy-anim = swap carry-anim = or if  -1 0.9e mark-on  else  drop  then ; is cry-hit

\ ---- his lunge (Riccardo_StartLunge; her panic calming down, into his move 5) ----
: lunging ( -- )
    root-move
    ended? 0= if  exit  then
    calming? if  5 combo-start  else  -1 step-done !  then ;
: lunge-start ( -- )  lunge-anim play-now  ['] lunging own-act-2 act! ;
create riccardo-acts  ' flurry-start , ' lunge-start ,

\ ---- his frame's own (Riccardo_Update): his rage while her panic calms down (a roar), his
\ snort at his door animation's key, his gun's shots ----
: riccardo-frame ( -- )
    shot-fades
    raging @ if  calming? 0= if  0 raging !  then
    else  calming? reeling @ 0= and  my-mode @ after-her = and if  -1 raging !  rage-roar 7 sound  then  then
    anim @ door-anims =  my-model @ 0 0 1 motion-events 1 and and if  snort-sound 5 sound  then
    impact ;

\ ---- his other class (kind 37; riccardo.c Kind37_Update): struck enough (100 damage) he is
\ alerted - hurt, his alert tables and waits - and calms again below ----
\ its tint (Kind37_Update; the renderer's layer $11, drawn over the room by 80 + 80 sin of its
\ pulse, out of 128): whole for 150 frames after each turn of what it does (counted while it is
\ free), then pulsing - its phase on by 4 cos of it and up to 10 more, degrees a frame
: struck-or-down? ( -- flag )  cond @ hurt-cond down-cond 1+ within ;
: kind37-tint ( -- )
    my-mode @ tint-was @ <> if  150 tint-t !  my-mode @ tint-was !  then
    tint-phase f@ fsin 80e f* 80e f+  128e fmin 128e f/  my-model @ actor act.alpha sf!
    tint-t @ if
        pi f2/ tint-phase f!  struck-or-down? 0= if  -1 tint-t +!  then
    else
        tint-phase f@ fcos 4e f*  rnd 10e f* f+  deg>rad  tint-phase f@ f+ angle-wrap tint-phase f!
    then ;
: kind37-frame ( -- )
    hits @ 100 < if  cond @ hurt-cond = if  well-cond cond !  then
    else  cond @ hurt-cond <> cond @ down-cond <> and if  hurt-cond cond !  then  then
    cond @ hurt-cond = alt-tables !
    kind37-tint ;

\ ---- the TintStalker (kind 23; tintstalker.c TintStalker_Tint / Fade / SetDelay): seen only a
\ while (150 frames) after each turn of what it does - counting while it is free. Its tint (the
\ renderer's layer $11, drawn over the room by its alpha out of 128): shown, fading in to 112 -
\ held whole (130) while beaten -; its while over, fading out by 4 a frame, then gone ----
0 constant tint-still   1 constant tint-in   2 constant tint-out   3 constant tint-whole
: beaten-now? ( -- flag )  cond @ down-cond = ;
: in-her-room? ( -- flag )  in-game @ if  self body-room room-id =  else  false  then ;   \ (Npc_InPlayedRoom)
: tint-show ( -- )
    in-her-room? 0= if  exit  then
    -1 tint-shown !  beaten-now? if  tint-whole  else  tint-in  then  tint-fade ! ;
: tint-fading ( -- )   \ (TintStalker_Fade)
    tint-fade @ case
        tint-in of
            tint-alpha f@ 116e f< if  112e tint-alpha f!  tint-still tint-fade !
            else  tint-alpha f@ 4e f- tint-alpha f!  then
        endof
        tint-whole of  130e tint-alpha f!  endof
        tint-out of
            tint-alpha f@ 4e f-  fdup tint-alpha f!
            f0> 0= if  0e tint-alpha f!  tint-still tint-fade !  0 tint-shown !  then
        endof
    endcase ;
: tint-frame ( -- )
    my-mode @ tint-was @ <> if  150 tint-t !  my-mode @ tint-was !  then
    tint-t @ if
        tint-shown @ 0=  beaten-now? tint-fade @ tint-whole <> and  or
        beaten-now? 0= tint-fade @ tint-whole = and  or if  tint-show  then
        struck-or-down? 0= if  -1 tint-t +!  then
    then
    tint-t @ 0=  in-her-room? and  tint-shown @ and  tint-fade @ tint-out <> and if  tint-out tint-fade !  then
    tint-fading
    tint-shown @ 0= unseen !
    tint-alpha f@ 128e fmin 128e f/  my-model @ actor act.alpha sf! ;

\ ---- kind 12 (lorenzo.c Kind12_Update / EventState): never hurt (its health back each frame); in
\ her room, free to go for her, it seizes her (her fright 100) - not in her special costumes ----
\ its marker (Kind12_ShowUp / EnterRoom: the Marker on it - the first kind, size 1 - as it comes
\ into her room; done once it has gone from there: Marker_Draw)
: kind12-marker ( -- )
    in-her-room? if
        mark-id @ dup 0< 0= swap alive? and if  exit  then
        my-model @ -1 0 1e marker mark-id !  exit
    then
    mark-id @ dup 0< 0= over alive? and if  -1 swap send effect-command  else  drop  then  -1 mark-id ! ;
: her-costume ( -- n )  $26 progress pr.vars + c@ ;
: kind12-frame ( -- )
    kind12-marker
    hp-max @ hp !
    world-held state-flag?  stalkers-blind state-flag? or if  exit  then
    her-costume dup illegal-outfit = swap cowgirl-outfit = or if  exit  then
    fiona here? 0= if  exit  then
    her-plight @ unreachable-plight = if  exit  then   \ (struck or held already)
    grab-request 0 100e f>cell fiona send hit ;
