\ old-mansion-1f-14's place: the projection room - the projector and screen remarks, the film played,
\ the mannequins, the books, the parts, the jerky. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_14.fs
IN: test-old-mansion-1f-14
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-14 ;
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
: room-afresh ( -- )  old-mansion-1f-14 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
film-reel-used-om1f-14 story-flag-clear  film-reel-used-om1f-14-2 story-flag-clear  room-afresh
testing the projector in turn, the screen
T{ 3 0 at  said  3 0 at  said -> projector-msg screen-set-msg }T
T{ 4 180 at  said -> screen-msg }T
testing the mannequins, the books, the parts
T{ 5 -90 at  said -> mannequin-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ 8 -90 at  said -> books-msg }T
T{ $B 0 at  said -> parts-msg }T
testing the Film Reel used (no Daniella about): played - the reel spent
film-reel 1 items send give-item  10 run-frames
0 0 -1 0 broadcast item-event  [: start-button press  10 run-frames  film-reel-used-om1f-14-2 story-flag? ;] 900 soon drop  300 read-on
T{ film-reel-used-om1f-14-2 story-flag?  film-reel items:items-of  doing-busy? -> -1 0 0 }T
T{ 3 0 at  said -> played-msg }T
testing the Chicken Jerky
om1f-14-chicken-jerky story-flag-clear  room-afresh  chicken-jerky items:items-of before !
0 15.82e 0e 16.96e put  circle press  300 read-on
T{ om1f-14-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
test-summary
