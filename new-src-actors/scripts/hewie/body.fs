\ hewie/body.fs - Hewie and the others as he sees them: his body (moved by the engine's body
\ operations), Fiona's and the others' bodies (facts), whom he is with, his way over the floor,
\ chance, and the words that choose his next action (filled in by hewie.actions).
IN: hewie.body
USING: items engine game-state actors messages common facts paths vectors flag-names hewie-names hewie.state ;

$29020008 constant his-floor   \ the nav triangles he can't stand on
8 constant through-mask       \ ... going through blocked floor

\ ---- chance (the original's gRandom: 0 <= r < 1) ----
: roll ( n -- i )  s>f rnd f* f>s ;   \ (s32)(n x r)
\ his costume (the story's variable $27): his own coat; the black German Shepherd (harder bites,
\ his own times and lists); a third, in which he is never knocked out
enum: dog-costume  own-coat shepherd-coat third-coat ;
: his-costume ( -- n )  progress pr.vars $27 + c@ ;
: black-shepherd? ( -- flag )  his-costume shepherd-coat = ;
: sylvesta? ( -- flag )  bite-boost boost-on? ;   \ (Sylvesta Esca's 450 frames: items.fs)
\ his bites' and tackles' damage: twice as the black shepherd, half again with Sylvesta Esca's
: bite-damage ( base -- n )  black-shepherd? if  2*  then  sylvesta? if  dup 2/ +  then ;

\ ---- himself ----
: his-at ( -- v )  self body-at ;
: his-yaw ( F: -- a )  self body-yaw ;
: his-yaw! ( F: a -- )  body-turn ;
: his-tri ( -- tri )  self body-tri ;
: his-room ( -- room )  self body-room ;
: his-place ( F: x y z -- )  body-place-at ;   \ (its triangle found under it)
: his-y! ( F: y -- )   \ his height alone (a leap)
    his-at v.x sf@ fswap his-at v.z sf@  his-room his-tri  body-place ;
: here? ( -- flag )  his-away @ 0= ;           \ in the room being played
: away! ( flag -- )  0<> his-away ! ;
: hp! ( n -- )  his-hp ! ;
: cond! ( n -- )  his-cond ! ;
\ Hewie_TakeDamage: his health down by n (never below 0); in his third costume never knocked out
\ - he keeps 1
: hurt-by ( n -- )
    abs his-hp @ swap - 0 max
    dup 0= his-costume third-coat = and if  drop 1  then  hp! ;

\ ---- the others: their bodies (facts), Fiona's doings (her broadcast) ----
: her ( -- id )  his-fiona @ ;
: active? ( id -- flag )  dup 0< if  drop false exit  then  dup alive? if  body?  else  drop false  then ;
\ the stalker in the game, as he last told (H3: `stalker-doing` each frame; -1 none)
: pursuer ( -- id )  st-age @ 2 < if  st-id @  else  -1  then ;
: pursuer? ( id -- flag )  dup 0< if  drop false exit  then  pursuer = ;
\ the creature in a slot, as it last told (-1 none); which slot an actor is in (-1 none)
: creature ( slot -- id )  dup cells cr-ages + @ 2 < if  cells cr-ids + @  else  drop -1  then ;
: creature-slot ( id -- slot | -1 )
    dup 0< if  drop -1 exit  then
    10 0 do  dup i creature = if  drop i unloop exit  then  loop  drop -1 ;
: creature-up? ( slot -- flag )  cells cr-ups + @ 0<> ;
: cond-of ( id -- n )
    dup her = if  drop fiona-cond @ exit  then  pursuer? if  st-cond @  else  0  then ;
: mode-of ( id -- n )
    dup her = if  drop fiona-mode @ exit  then
    dup pursuer? if  drop st-mode @ exit  then
    creature-slot dup 0< if  drop 0  else  cells cr-modes + @  then ;
\ in_his_room / Hewie_WithChar: active, not down, in his room - and on the floor of the room
\ being played if that's his
: with? ( id -- flag )
    dup active? 0= if  drop false exit  then
    dup cond-of down-cond = if  drop false exit  then
    dup body-room his-room <> if  drop false exit  then
    his-room room-id <> if  drop true exit  then
    body-tri 0< 0= ;
: her-with? ( -- flag )  her with? ;
: fiona-at ( -- v )  her body-at ;
: her-dist ( F: -- d )  his-at fiona-at vec-dist ;
: heading-to ( v -- ) ( F: -- yaw )  self swap body-heading-to ;   \ from him (his heading when right above)

\ ---- his way over the floor (Hewie_PlanTo / Hewie_PlanAndGo) ----
\ triangles either side of a room's divider (their nav flags)
$100000 constant near-side   $200000 constant far-side
: side-of ( tri -- bits )  nav-flags near-side far-side or and ;
: across? ( tri -- flag )   \ the other side of the divider from him
    side-of his-tri side-of {: a b :}  a far-side = b near-side = and  a near-side = b far-side = and or ;
\ a way to v on `tri` (not across the divider): true if there is one
: plan-to ( tri v -- flag )
    {: tri v :}  tri across? if  false exit  then
    his-path his-tri his-at tri v self body-mask path-plan 0> ;
: plan-and-go ( tri v -- 0 | -1 )  plan-to if  0  else  -1  then ;
: path-done? ( -- flag )  his-path path-left? 0= ;
: path-stop ( -- )  his-path path-end ;
: rest ( F: -- d )  his-path his-at path-rest ;
: ahead@ ( F: d -- ) ( -- i )  his-path his-at path-ahead ;   \ the point d along it: `ahead`
: free-ahead ( F: yaw d -- free )  -1 body-free ;

\ ---- what to do next (the actions are hewie.actions') ----
defer set-action ( act arg -- )     ' 2drop is set-action      \ Hewie_SetAction
defer adjust-action ( act -- act' ) ' noop is adjust-action    \ Hewie_AdjustAction
\ hewie_want: the action his situation makes of `act` (with `arg` only if it stays itself)
: want ( act arg -- )  {: act arg :}  act adjust-action  dup act = if  arg  else  0  then  set-action ;
: to-default ( -- )  choose-act 0 want ;       \ Hewie_ToDefault
: start ( act -- )  adjust-action 0 set-action ;   \ Hewie_Start
: behave ( xt -- )  his-act ! ;
: trust-of ( table -- n )  his-trust @ cells + @ ;
: by-chance ( table -- flag )  trust-of 0 100 clamp  100 roll > ;
\ the signals and noises he gives
: danger-id ( -- id )  s" danger" actor-named ;
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: noise-make ( loud tri -- )   \ heard in his room (tri -1: anywhere in it)
    his-room swap -1 hewie-noise acoustics-id send noise ;
: alerted ( -- )  hewie-alert danger-id send danger-signal ;   \ he's alert to a stalker
\ barking or growling at the stalker in his room makes him sore (Relation_Request kind 6; not
\ while he mayn't attack)
: snap-at ( -- )
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  exit  then
    pursuer dup with? if  send snap  else  drop  then ;
