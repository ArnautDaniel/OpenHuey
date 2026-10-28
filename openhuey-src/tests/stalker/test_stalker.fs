\ The stalkers, P1: Debilitas in the house (docs/subsystems/stalker.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_stalker.fs
IN: test-stalker
USING: stalker-names tester engine game-state actors messages room-names game debug paths doors stalker stalker.state stalker.head stalker.senses stalker.moving stalker.search stalker.travel stalker.attack stalker.chase stalker.grab fiona.state fiona.model fiona.seized keys facts flag-names gameover stalker.struck stalker.doorway fiona.defend fiona.slam story.base stalker.names stalker.pursue ;
fvariable dx  fvariable dy  fvariable dz  fvariable dyaw

: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: danger-room ( -- room )  game:danger enter  danger:s-room @  leave-actor ;   \ (the room he last told)
: drawn? ( -- flag )  [: him act.visible l@ 0<> ;] his ;

free-play  20 run-frames
testing out of the game at first: no body, not shown
T{ game:debilitas body?  drawn? -> 0 0 }T
testing into Fiona's room: placed, shown, waiting about (mode 3), full health
s" fiona" actor-named body-tri debug:debilitas-tri  2 run-frames
T{ game:debilitas body?  game:debilitas body-room room-id =  drawn? -> -1 -1 -1 }T
T{ [: my-mode @  hp @  hp-max @ ;] his -> 3 70 70 }T
testing he tells the danger he is about
T{ danger-room -> room-id }T
testing in another room he isn't shown
front-garden-2 room!  5 run-frames
T{ drawn? -> 0 }T
testing out of the game
debug:debilitas-out  2 run-frames
T{ game:debilitas body?  drawn? -> 0 0 }T

testing his senses (P1b): seen ahead in his view, within reach close behind, not far behind; a noise heard
front-garden-2 room!  10 run-frames
: her-y ( F: -- y )  s" fiona" actor-named body-pos fdrop fswap fdrop ;
140.64e her-y -217.33e 0e tp-facing  2 run-frames   \ (an open spot: him there, facing +z)
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
: face ( -- )   \ (him facing 340 degrees: open floor 40 ahead and 15 behind, a wall 30 behind)
    [: 340e deg>rad body-turn ;] his  1 run-frames
    game:debilitas body-pos dz f! dy f! dx f!  game:debilitas body-yaw dyaw f! ;
face
fvariable dd
: there ( F: d -- )   \ (Fiona put d along his heading: negative behind him)
    dd f!  dd f@ dyaw f@ fsin f* dx f@ f+  dy f@  dd f@ dyaw f@ fcos f* dz f@ f+  tp  4 run-frames ;
: on-floor? ( -- flag )  s" fiona" actor-named body-tri 0< 0= ;
: seen ( -- flag )  [: sees-fiona @ ;] his ;
T{ 40e there  on-floor?  seen  [: d-fiona f@ ;] his 40e f- fabs 1e f< -> -1 -1 -1 }T
T{ -15e there  on-floor?  seen -> -1 -1 }T
T{ -30e there  on-floor?  seen -> -1 0 }T
testing ... camouflaged and still (fiona-unseen): not seen ahead, nor in reach behind; seen again after
-1 broadcast fiona-unseen  deliver
T{ 40e there  seen  -15e there  seen -> 0 0 }T
0 broadcast fiona-unseen  deliver  face   \ (him turned back as he was: searching for her meanwhile)
T{ 40e there  seen -> -1 }T
testing ... his head looks at her (look mode 1): turned toward her, his sight with it
40e there  [: 0.4e yaw f+ body-turn  1 stalker.state:look-mode ! ;] his  1 run-frames
T{ [: head-yaw f@ fabs 0.05e f>  view-heading f@ yaw head-yaw f@ f+ angle-wrap f- fabs 0.01e f< ;] his -> -1 -1 }T
testing ... but one step (8 degrees) at most: his Think eases it back each frame before his HeadLook
[: 1 stalker.state:look-mode ! ;] his  1 run-frames   \ (a frame: going after her he turns, his head with him)
T{ [: head-yaw f@ fabs 8e deg>rad f- fabs 0.001e f< ;] his -> -1 }T
testing his head bones by his model (+0x8B0 / +0x8B4): Debilitas's $16 / $F; the tinted ones' (Kind23Model) $23 / $1A
T{ [: head-bone @ neck-bone @ ;] his -> $16 $F }T
T{ [: tintstalker-kind head-bones!  head-bone @ neck-bone @  kind37-kind head-bones!  head-bone @ neck-bone @
      debilitas-kind head-bones! ;] his -> $23 $1A $23 $1A }T
: heard-any ( -- flag )   \ (a noise each frame; heard on the frame after its end)
    0 10 0 do  $14 room-id s" fiona" actor-named body-tri -1 0 game:acoustics send noise  game-tick  [: did-hear @ ;] his or  loop ;
T{ heard-any -> -1 }T
testing her shove's hold on her (struck home: 5 frames still) runs out whatever she is doing (Fiona_Panic's
testing ... timers), and his blow on her lets it go at once (Fiona_React) - not left frozen in his reaction
: frozen? ( -- flag )  [: her act.mflags l@ fiona.slam:motion-frozen and 0<> ;] game:fiona enter execute leave-actor ;
: recoil@ ( -- n )  game:fiona enter  her-recoil @  leave-actor ;
game:fiona enter  fiona.slam:recoil  leave-actor
T{ recoil@ frozen?  6 run-frames  recoil@ frozen? -> 5 -1 0 0 }T
game:fiona enter  fiona.slam:recoil  leave-actor
[: blow-request 0 10e f>cell fiona send hit ;] his  2 run-frames
T{ recoil@ frozen? -> 0 0 }T
: on-her-feet ( -- )   \ (her reaction over: free again, as the tests after want her)
    600 0 do  game:fiona enter  her-mode @ 0=  leave-actor  if  unloop exit  then  game-tick  loop ;
on-her-feet  30 run-frames
testing held still while she reads on the dimmed screen (OpenHuey's, the user's call: a tip read in
testing ... peace), then on again: the kit's dim fade tells the stalkers, Hewie and the summoner
: where ( -- ) ( F: -- x z )  game:debilitas body-pos fswap fdrop ;
: moved? ( F: x z -- flag )  where {: F: x z x' z' :}  x x' f- fabs  z z' f- fabs f+ 0.01e f> ;
[: after-her my-mode !  chase-begin ;] his  5 run-frames
true broadcast characters-still  2 run-frames  where
T{ 30 run-frames  moved?  [: still-held @ ;] his  game:summoner enter summoner:held-still @ leave-actor -> 0 -1 -1 }T
false broadcast characters-still  where
T{ 30 run-frames  moved?  [: still-held @ ;] his -> -1 0 }T
testing his combo over, no move of it is left to run (Pursuer_BehaviourEnded): its act none
T{ [: 0 combo-start  combo-over  act-xt @ ['] noop =  combo @ ;] his -> -1 -1 }T
testing through a door, what he was about in the room he left is let go (Pursuer_ThroughDoor): unshaken,
testing ... whom he struck and who struck him, his attack's wait, his cry, his blow's hold
[: -1 unshaken !  1 took !  -1 by-her !  30 hold-off !  5 cry !  3 freeze-t !  room-id 0 room-exit-leads drop 0< 0= ;] his drop
T{ [: 0 through  unshaken @ took @ by-her @ hold-off @ cry @ freeze-t @  my-room room-id <> ;] his -> 0 0 0 0 -1 0 -1 }T
testing ... and as she leaves his room (Pursuer_LeaveScreen's Pursuer_ResetForRoom), and as he comes in (Pursuer_Reset)
[: -1 unshaken !  3 freeze-t !  2 combo ! ;] his  front-garden-3 room!  5 run-frames
T{ [: unshaken @ freeze-t @ combo @ ;] his -> 0 0 -1 }T
[: -1 unshaken !  -1 by-her ! ;] his  s" fiona" actor-named body-tri debug:debilitas-tri  2 run-frames
T{ [: unshaken @ by-her @ ;] his -> 0 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
debug:debilitas-out  2 run-frames

testing searching (P1c): he sets off for a stop; his route over, waiting about (mode 3) he makes for an exit at random to go
front-garden-2 room!  10 run-frames
140.64e her-y -217.33e 0e tp-facing  2 run-frames
s" fiona" actor-named body-tri  -500e her-y -500e tp  2 run-frames   \ (her well out of his sight)
debug:debilitas-tri  2 run-frames
create p0 12 allot  p0 game:debilitas body-at vec-copy
: moved ( F: -- d )  game:debilitas body-at p0 vec-dist ;
: wait-for ( xt n -- flag )   \ (up to n frames for xt, run as him, to be true)
    0 ?do  dup his if  drop true unloop exit  then  game-tick  loop  drop false ;
T{ 120 run-frames  moved 3e f>  [: route-end @ 1 >= ;] his -> -1 -1 }T
T{ [: out-going @ 0<> ;] 300 wait-for  [: my-mode @  making-for @ 0< 0= ;] his -> -1 3 -1 }T
testing ... searching again (2..5 stops) when told to
[: search-again ;] his  3 run-frames
T{ [: my-mode @  route-end @ dup 2 >= swap 5 <= and  search-phase @ ;] his -> 2 -1 1 }T
T{ [: route-next @ 1 >= ;] 900 wait-for -> -1 }T
testing seeing her he's after her (mode 0): closer on foot
140.64e her-y -217.33e 0e tp-facing  2 run-frames   \ (the open spot again: him there, her away)
s" fiona" actor-named body-tri  -500e her-y -500e tp  2 run-frames  debug:debilitas-tri  1 run-frames
face  40e there
T{ on-floor?  [: my-mode @  sees-fiona @ ;] his -> -1 0 -1 }T
: dist-to-her ( F: -- d )  s" fiona" actor-named body-at  game:debilitas body-at vec-dist ;
fvariable d0  dist-to-her d0 f!  90 run-frames
T{ dist-to-her d0 f@ 5e f- f< -> -1 }T
debug:debilitas-out  2 run-frames

testing out of sight (P1d): heading for her room he comes through the doors into it, on its floor, shown, searching
front-garden-3 room!  10 run-frames
front-garden-2 0 game:debilitas send stalker-in  2 run-frames
T{ game:debilitas body-room  game:debilitas body-tri  drawn? -> front-garden-2 -1 0 }T
[: 1 mode! ;] his
T{ [: my-room front-garden-3 = ;] 900 wait-for  game:debilitas body-tri 0< 0=  drawn? -> -1 -1 -1 }T
T{ [: my-mode @ dup 0= swap 2 = or ;] 120 wait-for -> -1 }T   \ (once he has walked in from the doorway)
testing she leaves his room: out of sight, after her he follows her into the next
[: 0 mode! ;] his  front-garden-2 room!  2 run-frames
T{ game:debilitas body-tri  drawn?  [: goal-room @ ;] his -> -1 0 front-garden-2 }T
T{ [: my-room front-garden-2 = ;] 900 wait-for  game:debilitas body-tri 0< 0= -> -1 -1 }T
testing she comes into his room: he's put on its floor
debug:debilitas-out  2 run-frames  front-garden-3 0 game:debilitas send stalker-in  2 run-frames
T{ game:debilitas body-tri drawn? -> -1 0 }T
front-garden-3 room!  3 run-frames
T{ game:debilitas body-tri 0< 0=  drawn?  game:debilitas body-room -> -1 -1 front-garden-3 }T
debug:debilitas-out  2 run-frames
testing ... held off (given up), she follows him into his room: still held off, on out by his way home
front-garden-2 room!  10 run-frames  front-garden-3 0 game:debilitas send stalker-in  2 run-frames
[: 4 mode!  30 hp ! ;] his  5 run-frames  front-garden-3 room!  3 run-frames
T{ [: my-mode @  out-going @ 0<>  making-for @ 0< 0=  hp @ ;] his -> 4 -1 -1 30 }T
T{ 120 run-frames  [: my-mode @  chasing? ;] his -> 4 0 }T
testing ... held off out of sight, his clock out: still held off - away till the summoner takes him
castle-1f-10 room!  3 run-frames  [: 4 mode!  1 mode-t ! ;] his  5 run-frames
T{ [: away?  my-mode @  hp @ ;] his -> -1 4 30 }T
testing ... brought in afresh by the summoner: his health full
debug:debilitas-out  2 run-frames  front-garden-3 0 0 0 game:debilitas send stalker-summoned  2 run-frames
T{ [: hp @ hp-max @ =  my-mode @ ;] his -> -1 3 }T
debug:debilitas-out  2 run-frames

testing at a shut door into her room he knocks, waits, then comes in and it swings open
castle-1f-2 room!  10 run-frames
castle-1f-10 debug:debilitas-hunt  2 run-frames
T{ [: knock-t @ 0> ;] 900 wait-for  [: away? ;] his -> -1 -1 }T
testing ... coming up to it his footsteps were heard through the wall (P4b)
T{ [: step-foot @ 0> ;] his -> -1 }T
T{ [: my-room castle-1f-2 = ;] 200 wait-for  game:debilitas body-tri 0< 0= -> -1 -1 }T
T{ 30 run-frames  32 state@ opened-bit and 0<> -> -1 }T
debug:debilitas-out  2 run-frames
testing ... taken hold of by her as he comes through (the doors refuse him): he waits beyond it,
testing ... barges it open (action $C, Pursuer_DoorBarge) and only then walks in - never in past it shut
32 opened-bit clear-bits  32 changed  2 run-frames
: door-32-exit ( -- exit )  8 0 do  room-id i room-exit-door 32 = if  i unloop exit  then  loop  -1 ;
room-id door-32-exit 1 doors-id send hold-door  2 run-frames   \ (held by her)
stalkers-blind state-flag-set   \ (the door alone: he doesn't go for her once in)
castle-1f-10 debug:debilitas-hunt  2 run-frames
[: after-her my-mode !  castle-1f-2 body-room!  door-32-exit came-by !  enter-her-room ;] his
: beyond? ( -- flag )   \ (at its spot 12 beyond, in the doorway outside)
    door-32-exit 1 exit-spot drop  game:debilitas body-pos  {: F: x y z ax ay az :}  x ax f- fabs 1e f<  z az f- fabs 1e f< and ;
T{ beyond?  [: entering @ ;] his -> -1 -1 }T
T{ [: barging @ ;] 30 wait-for  beyond? -> -1 -1 }T
: shut-and-in? ( -- flag )   \ (on through it - in-doorway over - while it is not yet open)
    32 state@ opened-bit and 0=  [: entering @ 0= ;] his and ;
: watched ( n -- bad? )  false swap 0 ?do  shut-and-in? or  game-tick  loop ;
T{ 200 watched  32 state@ opened-bit and 0<>  [: entering @ barging @ ;] his -> 0 -1 0 0 }T
debug:debilitas-out  32 held-bit clear-bits  32 changed  stalkers-blind state-flag-clear  2 run-frames
testing ... one who shuts the doors he comes in by (Riccardo, Daniella: DoorBreak): shut again once he is clear of it
32 opened-bit clear-bits  32 changed  2 run-frames
castle-1f-10 debug:debilitas-hunt  [: shuts-doors door-manner ! ;] his  2 run-frames
T{ [: 32 state@ opened-bit and 0<> ;] 1200 wait-for -> -1 }T
T{ [: 32 state@ opened-bit and 0=  to-shut @ 0< and ;] 600 wait-for  [: my-room ;] his -> -1 castle-1f-2 }T
[: leaves-doors door-manner ! ;] his  debug:debilitas-out  2 run-frames

testing held off out of sight (P4b): to the nearer of his own rooms - as kind 6 ($20, $02:
testing ... D_0041BE40) castle-1f-2 - and there he stays; Debilitas (kind 2) has one, $20
front-garden-2 room!  10 run-frames   \ (the summoner takes him after 5 s held off: checked within it)
6 partner-load  deliver  2 run-frames
castle-1f-10 0 game:debilitas send stalker-in  2 run-frames
[: 4 mode!  900 mode-t !  0 doors-n !  0 door-i ! ;] his
T{ [: own-room? ;] 1500 wait-for  [: my-room ;] his -> -1 castle-1f-2 }T
T{ 40 run-frames  [: own-room?  my-mode @  route-over? ;] his -> -1 4 -1 }T
debug:debilitas-out  2 run-frames
2 partner-load  deliver  2 run-frames
castle-1f-10 0 game:debilitas send stalker-in  2 run-frames
[: 4 mode!  900 mode-t !  0 doors-n !  0 door-i ! ;] his
T{ 10 run-frames  [: goal-room @  my-room castle-1f-2 = own-room? and ;] his -> castle-2f-2 0 }T
debug:debilitas-out  2 run-frames

testing after her (P2a): seeing her he chases her; his blows knock her down, a hard one throws her, a hold catches her
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
0 game:fiona send costume  front-garden-2 room!  10 run-frames
: blind ( on -- )  if  stalkers-blind state-flag-set  else  stalkers-blind state-flag-clear  then ;
: face-off ( -- )   \ (him on the open spot facing 340 degrees, her 10 ahead facing him; his attacks held meanwhile)
    true blind  140.64e her-y -217.33e 0e tp-facing  2 run-frames
    s" fiona" actor-named body-tri  -500e her-y -500e tp  2 run-frames  debug:debilitas-tri  1 run-frames
    [: 340e deg>rad body-turn ;] his  1 run-frames
    game:debilitas body-pos dz f! dy f! dx f!
    340e deg>rad fsin 10e f* dx f@ f+  dy f@  340e deg>rad fcos 10e f* dz f@ f+  160e tp-facing  3 run-frames  false blind  2 run-frames ;
face-off
T{ [: chasing? my-mode @ ;] his -> -1 0 }T
: strike-combo ( combo -- )  [: combo-start ;] his ;
: her-sub? ( -- sub )  [: her-sub @ ;] hers ;
: reacted ( n -- sub )   \ (up to n frames for a reaction: her sub then)
    0 ?do  [: her-mode @ 4 = her-mode @ $B = or ;] hers if  her-sub? unloop exit  then  game-tick  loop  -1 ;
T{ 0 strike-combo  60 reacted  $C $10 within  [: took @ ;] his -> -1 1 }T
T{ [: her-mode @ 0= ;] hers 0= -> -1 }T   \ (down a while)
: up ( n -- flag )  0 ?do  [: her-mode @ 0= ;] hers if  true unloop exit  then  game-tick  loop  false ;
T{ 300 up -> -1 }T
face-off  3 strike-combo
T{ 60 reacted -> $A }T
300 up drop  face-off  6 strike-combo
T{ 90 reacted  [: her-mode @ ;] hers -> $20 $B }T
testing a blow while she is down isn't taken
300 up drop  face-off  0 strike-combo  60 reacted drop
T{ [: 2 0 10e f>cell stalker.senses:fiona send hit  0 took ! ;] his  3 run-frames  [: took @ ;] his -> 0 }T
debug:debilitas-out  2 run-frames

testing seized (P2b): taken by the hand - led to his side, dragged; six drags and she's dragged off (the game's end)
: over? ( -- flag )  game:gameover enter  gameover:step @  leave-actor  0< 0= ;   \ (the game over begun)
: unover ( -- )   \ (and called off: the world going again)
    game:gameover enter  -1 gameover:step !  leave-actor
    0 still!  panic-held state-flag-clear  world-held state-flag-clear ;
variable take-type
: be-taken ( type -- )   \ (face to face, no way of his left to walk on; then he takes her)
    take-type !  face-off  [: chase-now  my-path path-clear  take-type @ take-her ;] his ;
: led? ( -- flag )  [: her-led @ ;] hers ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
300 up drop  6 be-taken
T{ ' led? 60 soon  [: [: leading @ -1 = ;] his ;] 5 soon  [: her-sub @ ;] hers -> -1 -1 9 }T
T{ ' over? 1500 soon  [: taunts @ ;] his  [: her-drags @ ;] hers -> -1 6 6 }T
unover  debug:debilitas-out  10 run-frames  [: free! 0 her-mode ! ;] hers
\ (from here on he doesn't frighten her - flag $21: her panic isn't what's tested)
stalker-no-fear state-flag-set  0 s" panic" actor-named send panic-stage!  2 run-frames
testing ... shaking free: she breaks away, he lets go
6 be-taken  ' led? 60 soon drop
: dragging? ( -- flag )  [: anim@ $1401 = ;] hers ;
' dragging? 300 soon drop
: shake ( n -- )  0 ?do  cross debug:press  game-tick  loop ;   \ (a press, then up)
12 shake  2 run-frames  true blind   \ (no taking her again meanwhile)
T{ [: her-free @ ;] hers  [: answer @ ;] his -> -1 3 }T
T{ [: [: her-mode @ 0= ;] hers ;] 300 soon -> -1 }T
T{ [: [: leading @ 0= ;] his ;] 200 soon  over? -> -1 0 }T
false blind
testing carried off walking: at its end the game's over
8 be-taken
T{ ' led? 60 soon  ' over? 600 soon -> -1 -1 }T
unover  debug:debilitas-out  10 run-frames  [: free! 0 her-mode ! ;] hers
testing ... taking her, he looks once as his stand fades in (Pursuer_CloseOnFiona): not facing her
testing ... (20 degrees) or out of reach (20), she isn't taken - a look round ($1A00, action $16)
: looked-round? ( -- flag )  [: chase-act @ look-around-act =  anim @ $1A00 = and ;] his ;
: take-turned ( -- )   \ (face to face, him turned 90 degrees off her)
    face-off  [: chase-now  my-path path-clear  yaw 90e deg>rad f+ body-turn  by-the-hand take-her ;] his ;
take-turned
T{ ' looked-round? 60 soon  led? -> -1 0 }T
: take-far ( -- )   \ (face to face, her 25 off)
    face-off  340e deg>rad fsin 25e f* dx f@ f+  dy f@  340e deg>rad fcos 25e f* dz f@ f+  160e tp-facing  2 run-frames
    [: chase-now  my-path path-clear  by-the-hand take-her ;] his ;
take-far
T{ ' looked-round? 60 soon  led? -> -1 0 }T
testing her panic calming down, his chase is the Pursuer's (Debilitas_ChaseDecision: progress $7B8 at 5)
\ (her stage as he was told it: the panic tells it again each frame)
face-off
T{ [: calming-down her-stage !  chase-begin  stalker.state:step @ ['] pursue-run = ;] his -> -1 }T
T{ [: composed her-stage !  chase-begin  stalker.state:step @ ['] chase-run = ;] his -> -1 }T
testing round her, her past 100 off: he stalks her (Debilitas_Chase $1A, his chase table's eighth)
T{ [: chase-now  round-her  150e d-fiona f!  chase-run  chase-act @ stalk-act =  hold-t @ seen-near stalk-frames = ;] his -> -1 -1 }T
testing ... and in his own chase, at a step's end (Debilitas_Chase: his next free)
T{ [: chase-now  calming-down her-stage !  chase-run  stalker.state:step @ ['] pursue-run = ;] his -> -1 }T
testing crouched (L1) with him in her room 25 off, his back to her: not found while he hasn't her in
testing ... sight or reach - the story's cond 0x23 from her to him asks his +0x1544; he turns, finds her: startled
\ (a script has him, so he stays put; his senses go on)
: crouched? ( -- flag )  fiona-half-hidden state-flag? ;
: behind-him ( -- )   \ (him facing 160 degrees, her 25 behind him)
    face-off  [: -1 script-held !  160e deg>rad body-turn ;] his
    340e deg>rad fsin 25e f* dx f@ f+  dy f@  340e deg>rad fcos 25e f* dz f@ f+  160e tp-facing  5 run-frames ;
fvariable kept-yaw
: facing ( n F: deg -- )   \ (n frames, him kept facing so)
    deg>rad kept-yaw f!  0 ?do  [: kept-yaw f@ body-turn ;] his  game-tick  loop ;
no-flee state-flag-clear  behind-him
key: Q true key-hold  60 160e facing
T{ crouched?  [: sees-fiona @ ;] his -> -1 0 }T
30 340e facing
T{ [: sees-fiona @ ;] his  crouched? -> -1 0 }T
key: Q false key-hold  [: 0 script-held ! ;] his  120 run-frames
debug:debilitas-out  10 run-frames
testing seized outright (a kind 3 hold): held, at its key the game's over
s" fiona" actor-named body-tri debug:debilitas-tri  2 run-frames
T{ [: 3 0 30e f>cell stalker.senses:fiona send hit ;] his  3 run-frames  her-sub? -> $10 }T
T{ ' over? 300 soon -> -1 }T
unover  fiona-occupied state-flag-clear  debug:debilitas-out  10 run-frames  [: 0 her-mode ! ;] hers
testing she isn't free: refused - his attack tables instead
face-off  [: 4 her-mode ! ;] hers  [: chase-now 6 take-her ;] his  10 run-frames
T{ [: leading @  chase-act @ $14 = ;] his -> 0 0 }T
[: 0 her-mode ! ;] hers  debug:debilitas-out  2 run-frames

testing faded from sight (item $89 worn, still), he walks into her: he has her (a hold, reaction $20)
: wear-89 ( id -- )  progress pr.equipped 4 + l! ;
$89 wear-89  true blind  face-off  true blind
[: 1 her-unseen !  0 her-fade !  0 her-mode !  0 her-sub ! ;] hers
s" fiona" actor-named body-tri debug:debilitas-tri
T{ 10 reacted  [: her-unseen @ ;] hers -> $20 0 }T
-1 wear-89  false blind  60 run-frames  debug:debilitas-out  10 run-frames  [: free! 0 her-mode ! ;] hers

testing struck (P3a): her shove makes him flinch, her kick takes 5; out of health he's down a while, then up
: close-up ( -- )   \ (face to face 7 apart, his attacks held)
    true blind  140.64e her-y -217.33e 0e tp-facing  2 run-frames
    s" fiona" actor-named body-tri  -500e her-y -500e tp  2 run-frames  debug:debilitas-tri  1 run-frames
    [: 340e deg>rad body-turn ;] his  1 run-frames
    game:debilitas body-pos dz f! dy f! dx f!
    340e deg>rad fsin 7e f* dx f@ f+  dy f@  340e deg>rad fcos 7e f* dz f@ f+  160e tp-facing  3 run-frames
    [: dx f@ dy f@ dz f@ body-place-at  340e deg>rad body-turn ;] his  1 run-frames ;   \ (him back on his spot:
    \ whatever he was about before, he stands where the test says - clear of the doorway nearby)
: flinching? ( -- flag )  [: anim @ $1000 $1006 within ;] his ;
close-up  [: 70 hp !  0 hits !  0 unshaken !  0 reeling ! ;] his  square debug:press   \ (not mid-move: in an unshaken move a shove only gets a grunt)
T{ ' flinching? 60 soon  [: hp @ hits @ ;] his -> -1 69 1 }T
: kick ( -- )  [: 6 her-doing !  8 her-mode !  $1B her-sub !  ['] kick-start her-act ! ;] hers ;
: settled ( -- )  [: [: reeling @ 0= ;] his  [: her-mode @ 0= ;] hers and ;] 300 soon drop ;
settled  close-up  [: 70 hp !  0 hits ! ;] his  kick
T{ [: [: hits @ 0> ;] his ;] 90 soon  [: hp @ hits @ ;] his -> -1 65 5 }T
settled  close-up  [: 1 hp ! ;] his  square debug:press
T{ [: [: cond @ 2 = ;] his ;] 60 soon  [: hp @  down-t @ 0> ;] his -> -1 0 -1 }T
[: 1 down-t ! ;] his
T{ [: [: cond @ 0=  reeling @ 0= and ;] his ;] 400 soon  [: hp @ hp-max @ = ;] his -> -1 -1 }T
testing her shove carries her (OpenHuey's): from 20 apart, him held still, she lunges on to him and it lands
: face-apart ( F: d -- )   \ (face to face d apart, as close-up)
    {: F: d :}  true blind  140.64e her-y -217.33e 0e tp-facing  2 run-frames
    s" fiona" actor-named body-tri  -500e her-y -500e tp  2 run-frames  debug:debilitas-tri  1 run-frames
    [: 340e deg>rad body-turn ;] his  1 run-frames
    game:debilitas body-pos dz f! dy f! dx f!
    340e deg>rad fsin d f* dx f@ f+  dy f@  340e deg>rad fcos d f* dz f@ f+  160e tp-facing  3 run-frames ;
: held-still ( n -- )  0 do  [: dx f@ dy f@ dz f@ body-place-at ;] his  1 run-frames  loop ;
settled  20e face-apart  [: 70 hp !  0 hits !  0 unshaken !  0 reeling ! ;] his  square debug:press  45 held-still
T{ [: hits @ ;] his -> 1 }T
testing ... met as she sets off: struck at once, her shove on from its key (frame 18)
settled  7e face-apart  [: 70 hp !  0 hits !  0 unshaken !  0 reeling ! ;] his  square debug:press  2 held-still
T{ [: hits @ ;] his  [: her act.frame sf@ 18e f< 0= ;] hers -> 1 -1 }T
testing ... 20 damage and he gives up: his cry, held off (mode 4), out of the room
settled  close-up  [: 70 hp !  25 hits ! ;] his  square debug:press
T{ [: [: my-mode @ 4 = ;] his ;] 300 soon  [: hits @ ;] his -> -1 0 }T
T{ [: [: away? ;] his ;] 900 soon -> -1 }T
testing ... (P3b) by its door: lined up, his animation ($602), the door swung open with it - left open
T{ room-id 0 room-exit-door door-open?  room-id 0 room-exit-door door-held? -> -1 0 }T
false blind  debug:debilitas-out  2 run-frames
testing ... seeing her - striking back at her shove - he gives up all the same, at the end of that
2 common:seed !   \ (his giving up is by chance: the rolls from here on fixed, whatever came before - Hewie's own)
testing ... step (Pursuer_FrameUpdate); out by the first door of his way home (Debilitas_PickDestination)
settled  close-up  false blind  [: 70 hp !  25 hits !  0 unshaken !  0 reeling ! ;] his  square debug:press
T{ [: [: my-mode @ 4 = ;] his ;] 300 soon  [: hits @ ;] his -> -1 0 }T
T{ [: goal-room @ dup $20 = swap $02 = or  making-for @ 0< 0= ;] his -> -1 -1 }T
T{ [: [: away? ;] his ;] 900 soon -> -1 }T
debug:debilitas-out  2 run-frames
testing ... in his own room (castle-2f-2): held off there, on in the room - not left flinching
: struck-to-give-up ( room tri -- )   \ (in that room with her, on that triangle, 25 damage had: one more blow)
    swap room!  20 run-frames  debug:debilitas-tri  5 run-frames
    [: 70 hp !  25 hits !  0 reeling !  0 unshaken ! ;] his  kick-blow 5 0 game:debilitas send blow ;
: tri-she-is-on ( -- tri )  s" fiona" actor-named body-tri ;
true blind  castle-2f-2 room!  20 run-frames  castle-2f-2 tri-she-is-on struck-to-give-up  90 run-frames
T{ [: my-mode @  stalker.state:step @ ['] flinching =  making-for @ ;] his -> 4 0 -1 }T
debug:debilitas-out  2 run-frames
testing ... held off at a door of his way home he can't line up at (castle-1f-2's exit 1, from the
testing ... hall's far side): that door avoided, his way home another
castle-1f-2 room!  20 run-frames  tri-she-is-on debug:debilitas-tri  [: room-id 694 dup tri-center body-place ;] his  5 run-frames
[: held-off mode!  avoid-clear  1 making-for !  line-up ;] his  2 run-frames
: avoided? ( door -- flag )  [: dup 5 rshift cells avoid + @  swap 31 and rshift 1 and 0<> ;] his ;
T{ castle-1f-2 1 room-exit-door avoided?  [: my-mode @  making-for @ 1 <> ;] his -> -1 4 -1 }T
debug:debilitas-out  2 run-frames  false blind

testing a door slammed on him (P3b): Fiona beside the open door, him in its way - shut, 3 damage, knocked through and out (stunned 150)
: door0 ( -- door )  room-id 0 room-exit-door ;
settled  front-garden-3 room!  10 run-frames  front-garden-2 room!  10 run-frames
door0 opened-bit set-bits  room-in  5 run-frames
68e 124e -174e 180e tp-facing  3 run-frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
create d-at 12 allot  66e 124e -185e d-at vec!
[: 70 hp !  room-id d-at v-tri d-at vec@ body-place  0e body-turn ;] his  2 run-frames
create goal-spot 12 allot  68e 124e -178e goal-spot vec!
T{ [: goal-spot true slam-door ;] hers -> -1 }T
T{ [: [: anim @ $1004 = ;] his ;] 5 soon  [: hp @ ;] his -> -1 67 }T
T{ [: [: away? ;] his ;] 30 soon  [: stun-t @ 100 > ;] his -> -1 -1 }T
T{ [: [: door0 door-open? 0=  door0 door-held? 0= and ;] his ;] 30 soon -> -1 }T
debug:debilitas-out  2 run-frames
testing ... out of health as it knocks him out of the room (Pursuer_StateBargedThrough: +0xC4 2): down there,
testing ... on his back, not travelling; she comes in after him: lying there, getting up when his time is out
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
door0 opened-bit set-bits  room-in  5 run-frames
68e 124e -174e 180e tp-facing  3 run-frames
[: 1 hp !  room-id d-at v-tri d-at vec@ body-place  0e body-turn ;] his  2 run-frames
T{ [: goal-spot true slam-door ;] hers -> -1 }T
T{ [: [: away? ;] his ;] 30 soon  [: cond @ down-cond =  lie-anim @  stalker.state:step @ ['] downed = ;] his -> -1 -1 $1802 -1 }T
T{ [: my-room ;] his  30 run-frames  [: my-room ;] his = -> -1 }T   \ (not travelling while down)
[: 100 down-t ! ;] his  [: my-room ;] his room!  10 run-frames
T{ [: away?  anim @  stalker.state:step @ ['] downed = ;] his -> 0 $1802 -1 }T
T{ [: [: cond @ well-cond = ;] his ;] 400 soon -> -1 }T
debug:debilitas-out  front-garden-2 room!  10 run-frames
testing ... off screen behind the door he makes for (Pursuer_HitOffscreen kind 5): its damage counted toward his
testing ... giving up; health left, stunned 60..120 frames; out of health, down there; not again while stunned or down
castle-1f-10 debug:debilitas-hunt  2 run-frames
T{ [: 70 hp !  0 hits !  0 stun-t !  3 struck-away  hp @ hits @  stun-t @ 60 121 within ;] his -> 67 3 -1 }T
T{ [: 3 struck-away  hp @ ;] his -> 67 }T   \ (stunned: not again)
T{ [: 0 stun-t !  2 hp !  3 struck-away  hp @  cond @ down-cond =  lie-anim @ ;] his -> 0 -1 $1802 }T
T{ [: 0 stun-t !  3 struck-away  hits @ ;] his -> 6 }T   \ (down: not again)
debug:debilitas-out  2 run-frames
testing ... away from it (not in its way): staggered back ($1005), still in the room, after her
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
5 3 0 game:debilitas send blow  2 run-frames
T{ [: anim @  away?  my-mode @ ;] his -> $1005 0 0 }T
T{ [: [: reeling @ 0= ;] his ;] 120 soon -> -1 }T
debug:debilitas-out  2 run-frames
testing a felling blow (Pursuer_Hit kind 3) in the doorway: kept at 1, the low flinch there ($1005)
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
[: 70 hp !  room-id d-at v-tri d-at vec@ body-place  0e body-turn ;] his  2 run-frames
T{ [: at-door? ;] his -> -1 }T
3 10 0 game:debilitas send blow  2 run-frames
T{ [: anim @ hp @ cond @ ;] his -> $1005 1 0 }T
T{ [: [: reeling @ 0= ;] his ;] 120 soon -> -1 }T
debug:debilitas-out  2 run-frames

testing round to her (P3b): she across his own room ($40: halves joined through other rooms) - out by a door on his half, back in on hers
$40 0 room-by!  20 run-frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
[: room-id 178 dup tri-center body-place  4 came-by !  0 mode!  chase-start ;] his  3 run-frames
T{ [: across?  out-going @  making-for @ ;] his -> -1 -1 4 }T
T{ [: [: away? ;] his ;] 300 soon -> -1 }T
T{ [: [: away? 0=  my-way her-way = and ;] his ;] 3000 soon -> -1 }T
debug:debilitas-out  2 run-frames

test-summary
