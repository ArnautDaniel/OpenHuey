\ old-mansion-1f-1's place: the entrance hall - the first time in, Daniella's scene and its file, her
\ at her task after; "She's gone utterly mad..."; the flooded hall. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_1.fs
IN: test-old-mansion-1f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-1 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  old-mansion-1f-1 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  daniella-kind story.base:partner-load  5 run-frames   \ (one stalker in the game)
testing the first time in: Daniella's scene - its file, her at her task in the hall
found-file-233 story-flag-clear  found-file-234 story-flag-clear  room-afresh
0 0e 70e 46e put  0 0e 70e 34e put  1500 play-on   \ (across the gate, from the stairs)
T{ found-file-233 story-flag?  $233 filed?  partner body-room  [: busy-step @ ;] in-place -> -1 -1 old-mansion-1f-1 1 }T
testing ... after: "She's gone utterly mad..." as she steps further in
room-afresh  0 0e 90e -8e put  0 0e 90e -20e put
T{ shown-soon  300 read-on -> mad-msg }T
testing the hall flooded: its water, no Daniella
found-file-234 story-flag-set  partner send stalker-out  room-afresh
T{ 1 part? -> -1 }T
test-summary
