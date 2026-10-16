\ Fiona on a ladder (docs/subsystems/fiona.md, fiona/ladder.fs): castle-1f-2's first ladder - its
\ foot (side 0) on the ground floor, its top (side 1) on the floor above. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/fiona/test_ladder.fs
IN: test-ladder
USING: tester engine game-state actors messages room-names game debug keys facts flag-names fiona.state fiona.model fiona.ladder stalker.state stalker.search stalker.ladder ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: hers ( xt -- x )  fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;
: free? ( -- flag )  [: her-mode @ 0= ;] hers ;
: on? ( -- flag )  [: her-mode @ 3 = ;] hers ;
: anim ( -- id )  ['] anim@ hers ;
: height ( -- n )  [: her-at 4 + sf@ f>s ;] hers ;
: ladder ( -- link side )  [: her-ladder @ her-ladder-side @ ;] hers ;
: tri ( -- tri )  [: her-tri @ ;] hers ;
\ Fiona at an end's spot, facing its way (or turned from it)
: at-end ( side turn -- )
    >r  0 over  dup if  0e 6.7e  else  0e -7.1068e  then  nav-link-front drop
    0 swap nav-link-yaw  180e f* pi f/  r> s>f f+  tp-facing  10 frames ;

2 frames
0 fiona send costume  1 frames
game:story send story-stop  deliver  progress pr.state 8 0 fill
castle-1f-2 room!  20 frames
testing the room's ladders: two, the first's foot on the ground floor and its top above
T{ nav-links  0 0 nav-link-spot fdrop f>s fdrop  0 1 nav-link-spot fdrop f>s fdrop -> 2 26 -1 }T

testing at its top facing it, the action button: on from the top ($700), then holding ($708)
1 0 at-end
T{ circle press  ladder  on? -> 0 1 -1 }T
T{ [: anim $700 = ;] 30 soon  [: anim $708 = ;] 120 soon  on? -> -1 -1 -1 }T
testing ... a stalker after her in the room (Npc_OpenDoorFionaHides, Pursuer_DoorApproach): to beside its foot (else its top), there facing her
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
stalkers-blind state-flag-set  368 debug:debilitas-tri  deliver  [: 0 stalker:ordered ;] his  3 frames  stalkers-blind state-flag-clear   \ (after her at once: by its doorway he would wander off)
T{ [: [: at-ladder @ 0 = ;] his ;] 30 soon -> -1 }T
T{ [: [: step @ ['] by-the-ladder = ;] his ;] 600 soon -> -1 }T
stalkers-blind state-flag-set  debug:debilitas-out  3 frames  stalkers-blind state-flag-clear
testing down (S): rungs down to the foot, off there ($703) on the ground floor - free, the ladder let go
key: S hold
T{ [: anim $701 = ;] 60 soon  [: anim $703 = ;] 600 soon  [: free? ;] 120 soon -> -1 -1 -1 }T
key: S release
T{ height  tri  ladder drop -> 26 368 -1 }T

testing at its foot turned from it: the action button does nothing
0 180 at-end
T{ circle press  2 frames  free?  ladder drop -> -1 -1 }T
testing facing it: on from the foot ($704); up (W) to the top, off there ($707) on the floor above
0 0 at-end
T{ circle press  [: anim $704 = ;] 60 soon -> -1 }T
key: W hold
T{ [: anim $705 = ;] 300 soon  [: anim $707 = ;] 300 soon  [: free? ;] 120 soon -> -1 -1 -1 }T
key: W release
T{ height  tri  ladder drop -> -1 68 -1 }T

testing the stalker at its foot: she can't get off there - she holds on ($708)
1 0 at-end  circle press
key: S hold
T{ [: height 3 > ;] 600 soon -> -1 }T
368 debug:debilitas-tri  deliver  [: 0 stalker:ordered ;] his  1 frames   \ (after her: kept by its foot)
T{ [: 0 stalker-on ;] hers  [: anim $708 = ;] 300 soon  on?  height 6 > -> -1 -1 -1 -1 }T
key: S release

testing struck on it (by him): knocked off - within 20 of its top pulled off up there ($F02), then free
: strike ( kind how -- )  {: k h :}  game:debilitas enter  k h 10e f>cell fiona send hit  leave-actor  deliver ;
T{ 1 0 strike  anim  [: free? ;] 200 soon  ladder drop  height -> $F02 -1 -1 -1 }T   \ (y is down: the floor above at -1)

testing the charm (item $88 worn): a hold doesn't take her
: wear ( item -- )  progress pr.equipped 4 + l! ;
5 frames
T{ $88 wear  6 1 strike  3 frames  [: her-mode @ her-sub @ ;] hers -> 0 0 }T
-1 wear
testing a door pushed into her (request 5): thrown ($1008 / $100B), then up again
: door-exit ( -- e )  8 0 do  i door-here? if  i unloop exit  then  loop  -1 ;
T{ door-exit 0< 0= -> -1 }T
T{ 5 door-exit strike  3 frames  [: her-sub @ ;] hers  anim dup $1008 = swap $100B = or -> $B -1 }T
T{ [: free? ;] 300 soon -> -1 }T
T{ 5 door-exit strike  3 frames  [: her-sub @ ;] hers -> $B }T

test-summary
