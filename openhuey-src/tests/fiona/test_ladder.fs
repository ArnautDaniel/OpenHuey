\ Fiona on a ladder (docs/subsystems/fiona.md, fiona/ladder.fs): castle-1f-2's first ladder - its
\ foot (side 0) on the ground floor, its top (side 1) on the floor above. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_ladder.fs
IN: test-ladder
USING: tester engine game-state actors messages room-names game debug keys facts flag-names fiona.state fiona.model fiona.ladder stalker.state stalker.search stalker.ladder doors ;

: hers ( xt -- x )  fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;
: free? ( -- flag )  [: her-mode @ 0= ;] hers ;
: on? ( -- flag )  [: her-mode @ 3 = ;] hers ;
: anim-now ( -- id )  ['] anim@ hers ;
: height ( -- n )  [: her-at 4 + sf@ f>s ;] hers ;
: ladder ( -- link side )  [: her-ladder @ her-ladder-side @ ;] hers ;
: tri ( -- tri )  [: her-tri @ ;] hers ;
\ Fiona at an end's spot, facing its way (or turned from it)
: at-end ( side turn -- )
    >r  0 over  dup if  0e 6.7e  else  0e -7.1068e  then  nav-link-front drop
    0 swap nav-link-yaw  180e f* pi f/  r> s>f f+  tp-facing  10 run-frames ;

2 run-frames
0 fiona send costume  1 run-frames
game:story send story-stop  deliver  progress pr.state 8 0 fill
castle-1f-2 room!  20 run-frames
testing the room's ladders: two, the first's foot on the ground floor and its top above
T{ nav-links  0 0 nav-link-spot fdrop f>s fdrop  0 1 nav-link-spot fdrop f>s fdrop -> 2 26 -1 }T

testing at its top facing it, the action button: on from the top ($700), then holding ($708)
1 0 at-end
T{ circle press  ladder  on? -> 0 1 -1 }T
T{ [: anim-now $700 = ;] 30 soon  [: anim-now $708 = ;] 120 soon  on? -> -1 -1 -1 }T
testing panicking on it (fear bit 2): she lets go - knocked off as by a blow (Fiona_StateOnLadder: mode 4, sub 8), near its top pulled off ($F02)
10 run-frames
T{ [: 2 her-fear-bits ! ;] hers  2 run-frames  [: her-sub @ ;] hers  anim-now -> 8 $F02 }T
[: 0 her-fear-bits ! ;] hers
T{ [: free? ;] 200 soon  ladder drop -> -1 -1 }T
1 0 at-end  circle press  [: anim-now $708 = ;] 200 soon drop   \ (on again, holding at its top)
testing ... a stalker after her in the room (Npc_OpenDoorFionaHides, his action $12): to beside its foot, on ($704), up the rungs ($705)
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: his-anim ( -- a )  [: stalker.state:anim @ ;] his ;
: his-height ( -- n )  [: self body-at 4 + sf@ f>s ;] his ;
: after-her ( tri -- )   \ (after her at once: by its doorway he would wander off)
    stalkers-blind state-flag-set  debug:debilitas-tri  deliver  [: 0 stalker:ordered ;] his  3 run-frames  stalkers-blind state-flag-clear ;
368 after-her
T{ [: [: at-ladder @ 0=  ladder-side @ 0= and ;] his ;] 30 soon -> -1 }T
T{ [: his-anim $704 = ;] 300 soon  [: his-anim $705 = ;] 300 soon -> -1 -1 }T
testing ... touching her, she is pulled off at its top ($F02); he gets off there ($707), on the floor above, after her again
T{ [: anim-now $F02 = ;] 300 soon  [: his-anim $707 = ;] 300 soon -> -1 -1 }T
T{ [: [: at-ladder @ 0< ;] his ;] 200 soon  his-height  [: free? ;] 200 soon -> -1 -1 -1 }T
stalkers-blind state-flag-set  debug:debilitas-out  3 run-frames  stalkers-blind state-flag-clear
1 0 at-end  circle press  [: anim-now $708 = ;] 200 soon drop
testing down (W - the original's stick, pushed up, goes down): rungs down to the foot, off there ($703) on the ground floor - free, the ladder let go
key: W hold
T{ [: anim-now $701 = ;] 60 soon  [: anim-now $703 = ;] 600 soon  [: free? ;] 120 soon -> -1 -1 -1 }T
key: W release
T{ height  tri  ladder drop -> 26 368 -1 }T

testing at its foot turned from it: the action button does nothing
0 180 at-end
T{ circle press  2 run-frames  free?  ladder drop -> -1 -1 }T
testing facing it: on from the foot ($704); up (S) to the top, off there ($707) on the floor above
0 0 at-end
T{ circle press  [: anim-now $704 = ;] 60 soon -> -1 }T
key: S hold
T{ [: anim-now $705 = ;] 300 soon  [: anim-now $707 = ;] 300 soon  [: free? ;] 120 soon -> -1 -1 -1 }T
key: S release
T{ height  tri  ladder drop -> -1 68 -1 }T

testing the stalker at its foot: she can't get off there - she holds on ($708)
1 0 at-end  circle press
key: W hold
T{ [: height 3 > ;] 600 soon -> -1 }T
368 debug:debilitas-tri  deliver  [: 0 stalker:ordered ;] his  1 run-frames   \ (after her: kept by its foot)
T{ [: 0 stalker-on ;] hers  [: anim-now $708 = ;] 300 soon  on?  height 6 > -> -1 -1 -1 -1 }T
key: W release

testing struck on it (by him): knocked off - within 20 of its top pulled off up there ($F02), then free
: strike ( kind how -- )  {: k h :}  game:debilitas enter  k h 10e f>cell fiona send hit  leave-actor  deliver ;
T{ 1 0 strike  anim-now  [: free? ;] 200 soon  ladder drop  height -> $F02 -1 -1 -1 }T   \ (y is down: the floor above at -1)

testing the charm (item $88 worn): a hold doesn't take her
: wear ( item -- )  progress pr.equipped 4 + l! ;
5 run-frames
T{ $88 wear  6 1 strike  3 run-frames  [: her-mode @ her-sub @ ;] hers -> 0 0 }T
-1 wear
testing a door pushed into her (request 5): thrown ($1008 / $100B), then up again
: door-exit ( -- e )  8 0 do  i door-here? if  i unloop exit  then  loop  -1 ;
T{ door-exit 0< 0= -> -1 }T
T{ 5 door-exit strike  3 run-frames  [: her-sub @ ;] hers  anim-now dup $1008 = swap $100B = or -> $B -1 }T
T{ [: free? ;] 300 soon -> -1 }T
T{ 5 door-exit strike  3 run-frames  [: her-sub @ ;] hers -> $B }T

testing no way to her on foot (Pursuer_TargetOutOfReach, Npc_RoundThroughDoor): she on the floor above, he below - round by the ladder, up from its foot
stalkers-blind state-flag-set  debug:debilitas-out  3 run-frames  stalkers-blind state-flag-clear
1 180 at-end  30 run-frames
368 after-her
T{ [: [: at-ladder @ 0=  ladder-side @ 0= and ;] his ;] 30 soon -> -1 }T
T{ [: his-anim $704 = ;] 300 soon  [: his-anim $707 = ;] 600 soon  [: [: at-ladder @ 0< ;] his ;] 200 soon  his-height -> -1 -1 -1 -1 }T
testing ... she below, he above: down from its top ($700, $701, off at its foot $703)
stalkers-blind state-flag-set  debug:debilitas-out  3 run-frames  stalkers-blind state-flag-clear
0 180 at-end  30 run-frames
68 after-her
T{ [: [: at-ladder @ 0=  ladder-side @ 1 = and ;] his ;] 30 soon -> -1 }T
T{ [: his-anim $700 = ;] 300 soon  [: his-anim $701 = ;] 120 soon  [: his-anim $703 = ;] 600 soon -> -1 -1 -1 }T
T{ [: [: at-ladder @ 0< ;] his ;] 200 soon  his-height -> -1 26 }T

testing a shut door between them on his floor (Npc_ExitsToTri, his action $B): to its spot on his side, opened, let go open - after her again
stalkers-blind state-flag-set  debug:debilitas-out  3 run-frames  stalkers-blind state-flag-clear
: the-door ( -- door )  0 exit-door ;
T{ the-door state@ opened-bit and -> 0 }T
702 tp-tri  20 run-frames
68 after-her
T{ [: [: for-her @  making-for @ ;] his  0= and ;] 60 soon -> -1 }T
T{ [: the-door state@ opened-bit and 0<> ;] 1200 soon  [: [: for-her @ ;] his 0= ;] 120 soon -> -1 -1 }T   \ (a long way round to its spot)

test-summary
