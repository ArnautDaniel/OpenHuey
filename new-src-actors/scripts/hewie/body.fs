\ hewie/body.fs - Hewie and the others as he sees them: his body (moved by the engine's body
\ operations), Fiona's and the others' bodies (facts), whom he is with, his way over the floor,
\ chance, and the words that choose his next action (filled in by hewie.actions).
IN: hewie.body
USING: engine game-state actors messages common facts paths hewie.state ;

$29020008 constant his-floor   \ the nav triangles he can't stand on (through blocked floor: 8)

\ ---- chance (the original's gRandom: 0 <= r < 1) ----
: roll ( n -- i )  s>f rnd f* f>s ;   \ (s32)(n x r)
: hard? ( -- flag )  progress pr.vars $27 + c@ 1 = ;   \ the hard difficulty

\ ---- himself ----
: his-at ( -- v )  self body-at ;
: his-yaw ( F: -- a )  self body-yaw ;
: his-yaw! ( F: a -- )  body-turn ;
: his-tri ( -- tri )  self body-tri ;
: his-room ( -- room )  self body-room ;
: his-place ( F: x y z -- )  body-place-at ;   \ (its triangle found under it)
: his-y! ( F: y -- )   \ his height alone (a leap)
    his-at sf@ fswap his-at 8 + sf@  his-room his-tri  body-place ;
: here? ( -- flag )  his-away @ 0= ;           \ in the room being played
: away! ( flag -- )  0<> his-away ! ;
: hp! ( n -- )  his-hp ! ;
: cond! ( n -- )  his-cond ! ;

\ ---- the others: their bodies (facts), Fiona's doings (her broadcast) ----
: her ( -- id )  his-fiona @ ;
: active? ( id -- flag )  dup 0< if  drop false exit  then  dup alive? if  body?  else  drop false  then ;
: cond-of ( id -- n )  her = if  fiona-cond @  else  0  then ;   \ (the stalkers' with H3)
: mode-of ( id -- n )  her = if  fiona-mode @  else  0  then ;
\ in_his_room / Hewie_WithChar: active, not down, in his room - and on the floor of the room
\ being played if that's his
: with? ( id -- flag )
    dup active? 0= if  drop false exit  then
    dup cond-of 2 = if  drop false exit  then
    dup body-room his-room <> if  drop false exit  then
    his-room room-id <> if  drop true exit  then
    body-tri 0< 0= ;
: her-with? ( -- flag )  her with? ;
: fiona-at ( -- v )  her body-at ;
: her-dist ( F: -- d )  his-at fiona-at vec-dist ;
: heading-to ( v -- ) ( F: -- yaw )  self swap body-heading-to ;   \ from him (his heading when right above)
: pursuer ( -- id )  -1 ;   \ the stalker in play (-1 none: H3)

\ ---- his way over the floor (Hewie_PlanTo / Hewie_PlanAndGo) ----
\ triangles either side of a room's divider (flags 0x100000 / 0x200000)
: level-of ( tri -- bits )  nav-flags $300000 and ;
: across? ( tri -- flag )
    level-of his-tri level-of
    2dup $100000 = swap $200000 = and >r  $200000 = swap $100000 = and  r> or ;
\ a way to v on `tri` (not across the divider): true if there is one
: plan-to ( tri v -- flag )
    over across? if  2drop false exit  then
    >r >r  his-path his-tri his-at r> r> self body-mask path-plan 0> ;
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
: want ( act arg -- )  over adjust-action rot over <> if  nip 0  else  swap  then  set-action ;
: to-default ( -- )  0 0 want ;                \ Hewie_ToDefault
: start ( act -- )  adjust-action 0 set-action ;   \ Hewie_Start
: behave ( xt -- )  his-act ! ;
: trust-of ( table -- n )  his-trust @ cells + @ ;
: by-chance ( table -- flag )  trust-of 0 100 clamp  100 roll > ;
\ the signals and noises he gives
: danger-id ( -- id )  s" danger" actor-named ;
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: noise-make ( loud tri -- )   \ heard in his room (tri -1: anywhere in it)
    his-room swap -1 hewie-noise acoustics-id send noise ;
: alerted ( -- )  4 danger-id send danger-signal ;   \ he's alert to a stalker (the danger's bit 4)
