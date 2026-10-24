\ The stalkers, P1: Debilitas in the house (docs/subsystems/stalker.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_stalker.fs
IN: test-stalker
USING: stalker-names story.words tester engine game-state actors messages room-names game debug paths doors stalker stalker.state stalker.head stalker.senses stalker.moving stalker.search stalker.travel stalker.attack stalker.chase stalker.grab fiona.state fiona.model fiona.seized keys facts flag-names gameover stalker.struck stalker.doorway fiona.defend fiona.slam ;
fvariable dx  fvariable dy  fvariable dz  fvariable dyaw

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: danger-room ( -- room )  game:danger enter  danger:s-room @  leave-actor ;   \ (the room he last told)
: shown? ( -- flag )  [: him act.visible l@ 0<> ;] his ;

free-play  20 frames
testing out of the game at first: no body, not shown
T{ game:debilitas body?  shown? -> 0 0 }T
testing into Fiona's room: placed, shown, waiting about (mode 3), full health
s" fiona" actor-named body-tri debug:debilitas-tri  2 frames
T{ game:debilitas body?  game:debilitas body-room room-id =  shown? -> -1 -1 -1 }T
T{ [: my-mode @  hp @  hp-max @ ;] his -> 3 70 70 }T
testing he tells the danger he is about
T{ danger-room -> room-id }T
testing in another room he isn't shown
front-garden-2 room!  5 frames
T{ shown? -> 0 }T
testing out of the game
debug:debilitas-out  2 frames
T{ game:debilitas body?  shown? -> 0 0 }T

testing his senses (P1b): seen ahead in his view, within reach close behind, not far behind; a noise heard
front-garden-2 room!  10 frames
: her-y ( F: -- y )  s" fiona" actor-named body-pos fdrop fswap fdrop ;
140.64e her-y -217.33e 0e tp-facing  2 frames   \ (an open spot: him there, facing +z)
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
: face ( -- )   \ (him facing 340 degrees: open floor 40 ahead and 15 behind, a wall 30 behind)
    [: 340e deg>rad body-turn ;] his  1 frames
    game:debilitas body-pos dz f! dy f! dx f!  game:debilitas body-yaw dyaw f! ;
face
fvariable dd
: there ( F: d -- )   \ (Fiona put d along his heading: negative behind him)
    dd f!  dd f@ dyaw f@ fsin f* dx f@ f+  dy f@  dd f@ dyaw f@ fcos f* dz f@ f+  tp  4 frames ;
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
40e there  [: 0.4e yaw f+ body-turn  1 stalker.state:look-mode ! ;] his  1 frames
T{ [: head-yaw f@ fabs 0.05e f>  view-heading f@ yaw head-yaw f@ f+ angle-wrap f- fabs 0.01e f< ;] his -> -1 -1 }T
testing ... but one step (8 degrees) at most: his Think eases it back each frame before his HeadLook
[: 1 stalker.state:look-mode ! ;] his  10 frames
T{ [: head-yaw f@ fabs 8e deg>rad f- fabs 0.001e f< ;] his -> -1 }T
testing his head bones by his model (+0x8B0 / +0x8B4): Debilitas's $16 / $F; the tinted ones' (Kind23Model) $23 / $1A
T{ [: head-bone @ neck-bone @ ;] his -> $16 $F }T
T{ [: tintstalker-kind head-bones!  head-bone @ neck-bone @  kind37-kind head-bones!  head-bone @ neck-bone @
      debilitas-kind head-bones! ;] his -> $23 $1A $23 $1A }T
: heard-any ( -- flag )   \ (a noise each frame; heard on the frame after its end)
    0 10 0 do  $14 room-id s" fiona" actor-named body-tri -1 0 game:acoustics send noise  game-tick  [: did-hear @ ;] his or  loop ;
T{ heard-any -> -1 }T
debug:debilitas-out  2 frames

testing searching (P1c): he sets off for a stop; his route over, waiting about (mode 3) he makes for an exit at random to go
front-garden-2 room!  10 frames
140.64e her-y -217.33e 0e tp-facing  2 frames
s" fiona" actor-named body-tri  -500e her-y -500e tp  2 frames   \ (her well out of his sight)
debug:debilitas-tri  2 frames
create p0 12 allot  p0 game:debilitas body-at vec-copy
: moved ( F: -- d )  game:debilitas body-at p0 vec-dist ;
: wait-for ( xt n -- flag )   \ (up to n frames for xt, run as him, to be true)
    0 ?do  dup his if  drop true unloop exit  then  game-tick  loop  drop false ;
T{ 120 frames  moved 3e f>  [: route-end @ 1 >= ;] his -> -1 -1 }T
T{ [: out-going @ 0<> ;] 300 wait-for  [: my-mode @  making-for @ 0< 0= ;] his -> -1 3 -1 }T
testing ... searching again (2..5 stops) when told to
[: search-again ;] his  3 frames
T{ [: my-mode @  route-end @ dup 2 >= swap 5 <= and  search-phase @ ;] his -> 2 -1 1 }T
T{ [: route-next @ 1 >= ;] 900 wait-for -> -1 }T
testing seeing her he's after her (mode 0): closer on foot
140.64e her-y -217.33e 0e tp-facing  2 frames   \ (the open spot again: him there, her away)
s" fiona" actor-named body-tri  -500e her-y -500e tp  2 frames  debug:debilitas-tri  1 frames
face  40e there
T{ on-floor?  [: my-mode @  sees-fiona @ ;] his -> -1 0 -1 }T
: d-her ( F: -- d )  s" fiona" actor-named body-at  game:debilitas body-at vec-dist ;
fvariable d0  d-her d0 f!  90 frames
T{ d-her d0 f@ 5e f- f< -> -1 }T
debug:debilitas-out  2 frames

testing out of sight (P1d): heading for her room he comes through the doors into it, on its floor, shown, searching
front-garden-3 room!  10 frames
front-garden-2 0 game:debilitas send stalker-in  2 frames
T{ game:debilitas body-room  game:debilitas body-tri  shown? -> front-garden-2 -1 0 }T
[: 1 mode! ;] his
T{ [: my-room front-garden-3 = ;] 900 wait-for  game:debilitas body-tri 0< 0=  shown? -> -1 -1 -1 }T
T{ [: my-mode @ dup 0= swap 2 = or ;] 120 wait-for -> -1 }T   \ (once he has walked in from the doorway)
testing she leaves his room: out of sight, after her he follows her into the next
[: 0 mode! ;] his  front-garden-2 room!  2 frames
T{ game:debilitas body-tri  shown?  [: goal-room @ ;] his -> -1 0 front-garden-2 }T
T{ [: my-room front-garden-2 = ;] 900 wait-for  game:debilitas body-tri 0< 0= -> -1 -1 }T
testing she comes into his room: he's put on its floor
debug:debilitas-out  2 frames  front-garden-3 0 game:debilitas send stalker-in  2 frames
T{ game:debilitas body-tri shown? -> -1 0 }T
front-garden-3 room!  3 frames
T{ game:debilitas body-tri 0< 0=  shown?  game:debilitas body-room -> -1 -1 front-garden-3 }T
debug:debilitas-out  2 frames

testing at a shut door into her room he knocks, waits, then comes in and it swings open
castle-1f-2 room!  10 frames
castle-1f-10 debug:debilitas-hunt  2 frames
T{ [: knock-t @ 0> ;] 900 wait-for  [: away? ;] his -> -1 -1 }T
testing ... coming up to it his footsteps were heard through the wall (P4b)
T{ [: step-foot @ 0> ;] his -> -1 }T
T{ [: my-room castle-1f-2 = ;] 200 wait-for  game:debilitas body-tri 0< 0= -> -1 -1 }T
T{ 30 frames  32 state@ opened-bit and 0<> -> -1 }T
debug:debilitas-out  2 frames
testing ... one who shuts the doors he comes in by (Riccardo, Daniella: DoorBreak): shut again once he is clear of it
32 opened-bit clear-bits  32 changed  2 frames
castle-1f-10 debug:debilitas-hunt  [: shuts-doors door-manner ! ;] his  2 frames
T{ [: 32 state@ opened-bit and 0<> ;] 1200 wait-for -> -1 }T
T{ [: 32 state@ opened-bit and 0=  to-shut @ 0< and ;] 600 wait-for  [: my-room ;] his -> -1 castle-1f-2 }T
[: leaves-doors door-manner ! ;] his  debug:debilitas-out  2 frames

testing held off out of sight (P4b): to the nearer of his own rooms - as kind 6 ($20, $02:
testing ... D_0041BE40) castle-1f-2 - and there he stays; Debilitas (kind 2) has one, $20
front-garden-2 room!  10 frames   \ (the summoner takes him after 5 s held off: checked within it)
6 partner-load  deliver  2 frames
castle-1f-10 0 game:debilitas send stalker-in  2 frames
[: 4 mode!  900 mode-t !  0 doors-n !  0 door-i ! ;] his
T{ [: own-room? ;] 1500 wait-for  [: my-room ;] his -> -1 castle-1f-2 }T
T{ 40 frames  [: own-room?  my-mode @  route-over? ;] his -> -1 4 -1 }T
debug:debilitas-out  2 frames
2 partner-load  deliver  2 frames
castle-1f-10 0 game:debilitas send stalker-in  2 frames
[: 4 mode!  900 mode-t !  0 doors-n !  0 door-i ! ;] his
T{ 10 frames  [: goal-room @  my-room castle-1f-2 = own-room? and ;] his -> castle-2f-2 0 }T
debug:debilitas-out  2 frames

testing after her (P2a): seeing her he chases her; his blows knock her down, a hard one throws her, a hold catches her
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
0 game:fiona send costume  front-garden-2 room!  10 frames
: blind ( on -- )  if  stalkers-blind state-flag-set  else  stalkers-blind state-flag-clear  then ;
: face-off ( -- )   \ (him on the open spot facing 340 degrees, her 10 ahead facing him; his attacks held meanwhile)
    true blind  140.64e her-y -217.33e 0e tp-facing  2 frames
    s" fiona" actor-named body-tri  -500e her-y -500e tp  2 frames  debug:debilitas-tri  1 frames
    [: 340e deg>rad body-turn ;] his  1 frames
    game:debilitas body-pos dz f! dy f! dx f!
    340e deg>rad fsin 10e f* dx f@ f+  dy f@  340e deg>rad fcos 10e f* dz f@ f+  160e tp-facing  3 frames  false blind  2 frames ;
face-off
T{ [: chasing? my-mode @ ;] his -> -1 0 }T
: struck ( combo -- )  [: combo-start ;] his ;
: her-sub? ( -- sub )  [: her-sub @ ;] hers ;
: reacted ( n -- sub )   \ (up to n frames for a reaction: her sub then)
    0 ?do  [: her-mode @ 4 = her-mode @ $B = or ;] hers if  her-sub? unloop exit  then  game-tick  loop  -1 ;
T{ 0 struck  60 reacted  $C $10 within  [: took @ ;] his -> -1 1 }T
T{ [: her-mode @ 0= ;] hers 0= -> -1 }T   \ (down a while)
: up ( n -- flag )  0 ?do  [: her-mode @ 0= ;] hers if  true unloop exit  then  game-tick  loop  false ;
T{ 300 up -> -1 }T
face-off  3 struck
T{ 60 reacted -> $A }T
300 up drop  face-off  6 struck
T{ 90 reacted  [: her-mode @ ;] hers -> $20 $B }T
testing a blow while she is down isn't taken
300 up drop  face-off  0 struck  60 reacted drop
T{ [: 2 0 10e f>cell stalker.senses:fiona send hit  0 took ! ;] his  3 frames  [: took @ ;] his -> 0 }T
debug:debilitas-out  2 frames

testing seized (P2b): taken by the hand - led to his side, dragged; six drags and she's dragged off (the game's end)
: over? ( -- flag )  game:gameover enter  gameover:step @  leave-actor  0< 0= ;   \ (the game over begun)
: unover ( -- )   \ (and called off: the world going again)
    game:gameover enter  -1 gameover:step !  leave-actor
    0 still!  panic-held state-flag-clear  world-held state-flag-clear ;
variable take-type
: take ( type -- )   \ (face to face; then he takes her)
    take-type !  face-off  [: chase-now  take-type @ take-her ;] his ;
: led? ( -- flag )  [: her-led @ ;] hers ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
300 up drop  6 take
T{ ' led? 60 soon  [: [: leading @ -1 = ;] his ;] 5 soon  [: her-sub @ ;] hers -> -1 -1 9 }T
T{ ' over? 1500 soon  [: taunts @ ;] his  [: her-drags @ ;] hers -> -1 6 6 }T
unover  debug:debilitas-out  10 frames  [: free! 0 her-mode ! ;] hers
\ (from here on he doesn't frighten her - flag $21: her panic isn't what's tested)
stalker-no-fear state-flag-set  0 s" panic" actor-named send panic-stage!  2 frames
testing ... shaking free: she breaks away, he lets go
6 take  ' led? 60 soon drop
: dragging? ( -- flag )  [: anim@ $1401 = ;] hers ;
' dragging? 300 soon drop
: shake ( n -- )  0 ?do  cross debug:press  game-tick  loop ;   \ (a press, then up)
12 shake  2 frames  true blind   \ (no taking her again meanwhile)
T{ [: her-free @ ;] hers  [: answer @ ;] his -> -1 3 }T
T{ [: [: her-mode @ 0= ;] hers ;] 300 soon -> -1 }T
T{ [: [: leading @ 0= ;] his ;] 200 soon  over? -> -1 0 }T
false blind
testing carried off walking: at its end the game's over
8 take
T{ ' led? 60 soon  ' over? 600 soon -> -1 -1 }T
unover  debug:debilitas-out  10 frames  [: free! 0 her-mode ! ;] hers
testing seized outright (a kind 3 hold): held, at its key the game's over
s" fiona" actor-named body-tri debug:debilitas-tri  2 frames
T{ [: 3 0 30e f>cell stalker.senses:fiona send hit ;] his  3 frames  her-sub? -> $10 }T
T{ ' over? 300 soon -> -1 }T
unover  fiona-occupied state-flag-clear  debug:debilitas-out  10 frames  [: 0 her-mode ! ;] hers
testing she isn't free: refused - his attack tables instead
face-off  [: 4 her-mode ! ;] hers  [: chase-now 6 take-her ;] his  10 frames
T{ [: leading @  chase-act @ $14 = ;] his -> 0 0 }T
[: 0 her-mode ! ;] hers  debug:debilitas-out  2 frames

testing faded from sight (item $89 worn, still), he walks into her: he has her (a hold, reaction $20)
: wear-89 ( id -- )  progress pr.equipped 4 + l! ;
$89 wear-89  true blind  face-off  true blind
[: 1 her-unseen !  0 her-fade !  0 her-mode !  0 her-sub ! ;] hers
s" fiona" actor-named body-tri debug:debilitas-tri
T{ 10 reacted  [: her-unseen @ ;] hers -> $20 0 }T
-1 wear-89  false blind  60 frames  debug:debilitas-out  10 frames  [: free! 0 her-mode ! ;] hers

testing struck (P3a): her shove makes him flinch, her kick takes 5; out of health he's down a while, then up
: close-up ( -- )   \ (face to face 7 apart, his attacks held)
    true blind  140.64e her-y -217.33e 0e tp-facing  2 frames
    s" fiona" actor-named body-tri  -500e her-y -500e tp  2 frames  debug:debilitas-tri  1 frames
    [: 340e deg>rad body-turn ;] his  1 frames
    game:debilitas body-pos dz f! dy f! dx f!
    340e deg>rad fsin 7e f* dx f@ f+  dy f@  340e deg>rad fcos 7e f* dz f@ f+  160e tp-facing  3 frames ;
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
testing ... 20 damage and he gives up: his cry, held off (mode 4), out of the room
settled  close-up  [: 70 hp !  25 hits ! ;] his  square debug:press
T{ [: [: my-mode @ 4 = ;] his ;] 300 soon  [: hits @ ;] his -> -1 0 }T
T{ [: [: away? ;] his ;] 900 soon -> -1 }T
testing ... (P3b) by its door: lined up, his animation ($602), the door swung open with it - left open
T{ room-id 0 room-exit-door door-open?  room-id 0 room-exit-door door-held? -> -1 0 }T
false blind  debug:debilitas-out  2 frames
testing ... seeing her - striking back at her shove - he gives up all the same, at the end of that
2 common:seed !   \ (his giving up is by chance: the rolls from here on fixed, whatever came before - Hewie's own)
testing ... step (Pursuer_FrameUpdate); out by the first door of his way home (Debilitas_PickDestination)
settled  close-up  false blind  [: 70 hp !  25 hits !  0 unshaken !  0 reeling ! ;] his  square debug:press
T{ [: [: my-mode @ 4 = ;] his ;] 300 soon  [: hits @ ;] his -> -1 0 }T
T{ [: goal-room @ dup $20 = swap $02 = or  making-for @ 0< 0= ;] his -> -1 -1 }T
T{ [: [: away? ;] his ;] 900 soon -> -1 }T
debug:debilitas-out  2 frames

testing a door slammed on him (P3b): Fiona beside the open door, him in its way - shut, 3 damage, knocked through and out (stunned 150)
: door0 ( -- door )  room-id 0 room-exit-door ;
settled  front-garden-3 room!  10 frames  front-garden-2 room!  10 frames
door0 opened-bit set-bits  room-in  5 frames
68e 124e -174e 180e tp-facing  3 frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
create d-at 12 allot  66e 124e -185e d-at vec!
[: 70 hp !  room-id d-at v-tri d-at vec@ body-place  0e body-turn ;] his  2 frames
create to-at 12 allot  68e 124e -178e to-at vec!
T{ [: to-at true slam-door ;] hers -> -1 }T
T{ [: [: anim @ $1004 = ;] his ;] 5 soon  [: hp @ ;] his -> -1 67 }T
T{ [: [: away? ;] his ;] 30 soon  [: stun-t @ 100 > ;] his -> -1 -1 }T
T{ [: [: door0 door-open? 0=  door0 door-held? 0= and ;] his ;] 30 soon -> -1 }T
debug:debilitas-out  2 frames
testing ... away from it (not in its way): staggered back ($1005), still in the room, after her
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
5 3 0 game:debilitas send blow  2 frames
T{ [: anim @  away?  my-mode @ ;] his -> $1005 0 0 }T
T{ [: [: reeling @ 0= ;] his ;] 120 soon -> -1 }T
debug:debilitas-out  2 frames
testing a felling blow (Pursuer_Hit kind 3) in the doorway: kept at 1, the low flinch there ($1005)
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
[: 70 hp !  room-id d-at v-tri d-at vec@ body-place  0e body-turn ;] his  2 frames
T{ [: at-door? ;] his -> -1 }T
3 10 0 game:debilitas send blow  2 frames
T{ [: anim @ hp @ cond @ ;] his -> $1005 1 0 }T
T{ [: [: reeling @ 0= ;] his ;] 120 soon -> -1 }T
debug:debilitas-out  2 frames

testing round to her (P3b): she across his own room ($40: halves joined through other rooms) - out by a door on his half, back in on hers
$40 0 room-by!  20 frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
[: room-id 178 dup tri-center body-place  4 came-by !  0 mode!  chase-start ;] his  3 frames
T{ [: across?  out-going @  making-for @ ;] his -> -1 -1 4 }T
T{ [: [: away? ;] his ;] 300 soon -> -1 }T
T{ [: [: away? 0=  my-way her-way = and ;] his ;] 3000 soon -> -1 }T
debug:debilitas-out  2 frames

test-summary
