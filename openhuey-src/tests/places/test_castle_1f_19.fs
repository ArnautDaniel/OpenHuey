\ castle-1f-19's place (scripts/story/places/castle-1f-19.fs): the flooded passage - the hole's
\ remarks, a splash, Hewie into the hole at her "go there" and out at her "come back", Debilitas by
\ the hole: the ambush, Hewie fetching the Rubrum Medallion across the water, the replay. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_19.fs
IN: test-castle-1f-19
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-19 items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-1f-19 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: hewie-z ( F: -- z )  game:hewie body-pos ;
: debilitas-at ( F: x y z -- )  on-floor {: F: x y z :}  x f>cell y f>cell z f>cell 0e f>cell  game:debilitas send place ;
: hewie-brought ( -- )  castle-1f-19 $47 0 game:hewie send hewie-place  60 run-frames ;   \ (by the hole, with her)
: command ( cmd -- )  broadcast fiona-started  1 run-frames ;   \ (her command to Hewie, as she gives it)
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the hole's floor hers (no one hiding); the hole's remarks in turn
room-afresh
T{ hole-area 0 at  said  hole-area 0 at  said -> hole-msg hole-2-msg }T

testing a splash as she steps into the water (once a visit)
T{ 0 40e 0e -280e put  0 46e 0e -280e put  [: splashed @ 1 and ;] in-place -> 1 }T

testing Hewie into the hole at her "go there" toward it, out at her "come back"
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
hewie-brought  0 -10e 0e -145e put  60 run-frames   \ (before it, facing it)
go-there-cmd command
T{ [: [: hiding? ;] in-place ;] 900 soon -> -1 }T
come-back-cmd command
T{ [: [: hiding? 0= ;] in-place ;] 900 soon -> -1 }T

testing Hewie fetching the Rubrum Medallion across the water
c1f-19-rubrum-medallion story-flag-clear  rubrum-medallion items:items-of before !
[: ['] fetches start drop ;] in-place
T{ [: c1f-19-rubrum-medallion story-flag? ;] 1500 soon  rubrum-medallion items:items-of -> -1 before @ 1+ }T
300 read-on

testing the ambush: Hewie in the hole, Debilitas by it - the scene, its file, his trust
room-afresh  hewie-brought  0 -10e 0e -145e put  60 run-frames  go-there-cmd command
T{ [: [: hiding? ;] in-place ;] 900 soon -> -1 }T
-90 -60e 0e -200e put  2 story.words:partner-load  5 run-frames
castle-1f-19 $47 game:debilitas send stalker-in  5 run-frames
-10e 0e -140e debilitas-at   \ (by the hole)
T{ [: movie-skipped state-flag-set  $23 resident-flag? ;] 1500 soon  $22E filed? -> -1 -1 }T
300 play-on
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-19 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
