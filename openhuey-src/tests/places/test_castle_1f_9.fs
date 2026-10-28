\ castle-1f-9's place (scripts/story/places/castle-1f-9.fs): the hall of the half stairs - the
\ remarks (the stairs in turn, the well, the bench), the garden door locked and the Garden Key used
\ (the Luminessants' scene), her first fright, the vases (the Magnesia), the Rubrum Medallion, Hewie
\ fetching the Album Medallion, the replay. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_9.fs
IN: test-castle-1f-9
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-9 doors items.names ;

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
: room-afresh ( -- )  castle-1f-9 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: struck ( xt F: x y z -- )  blow-at  in-place  [: body-point-off ;] as-her  10 run-frames ;
: break-vase-1 ( -- )  vase-1 breaks? drop ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the vases whole, the Album Medallion's sparkle up the stairs
c1f-9-breakable1-broken story-flag-clear  c1f-9-breakable2-broken story-flag-clear  c1f-9-album-medallion story-flag-clear
c1f-9-flag1 story-flag-set  room-afresh
T{ 0 part?  2 part?  [: album-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the remarks: the stairs in turn, the well, the bench
T{ stairs-area 45 at  said  stairs-area 45 at  said -> stairs-msg stairs-2-msg }T
T{ 180 0e 0e -45e put  said -> well-msg }T   \ (facing it, a little way off)
T{ bench-area 90 at  said -> bench-msg }T

testing the garden door locked (exit 1, door 6): tried, it won't open
garden-door-done story-flag-clear  garden-door s" doors" actor-named send lock  room-afresh
1 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ said -> locked-msg }T

testing the Garden Key used at it: the Luminessants' scene - the door unlocked, the key spent, the file
garden-key 1 items send give-item  10 run-frames  4 use-on-place  900 play-on
T{ garden-door-done story-flag?  garden-door doors:door-locked?  garden-key items:items-of  luminessants-doc items:items-of 0> -> -1 0 0 -1 }T

testing her first fright here: thrown, nobody behind it - "What the!"
c1f-9-flag1 story-flag-clear  room-afresh
T{ [: thrown-mode fiona-mode-now !  -1 struck-by-now !  frightened ;] in-place  [: window-shown what-msg = ;] 120 soon  c1f-9-flag1 story-flag? -> -1 -1 }T
300 read-on

testing the vases broken: the Magnesia showing; picked up
' break-vase-1 -30e 1e -85.4e struck  ' spots -5.5e 1e -99e struck
T{ c1f-9-breakable1-broken story-flag?  c1f-9-breakable2-broken story-flag?  [: magnesia-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T
magnesia items:items-of before !
-5.5e 0e -99e picked
T{ c1f-9-magnesia story-flag?  magnesia items:items-of -> -1 before @ 1+ }T

testing the Rubrum Medallion Hewie found: it shows; picked up
c1f-9-rubrum-medallion-found story-flag-set  c1f-9-rubrum-medallion story-flag-clear  room-afresh
T{ [: rubrum-fx effect @ 0<> ;] in-place -> -1 }T
rubrum-medallion items:items-of before !
60e 0e 37.2e picked
T{ c1f-9-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> -1 before @ 1+ }T

testing Hewie fetching the Album Medallion off the stairs
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  60 run-frames
castle-1f-9 406 0 game:hewie send hewie-place  60 run-frames
album-medallion items:items-of before !
[: ['] fetches start drop ;] in-place
T{ [: c1f-9-album-medallion story-flag? ;] 1500 soon  album-medallion items:items-of -> -1 before @ 1+ }T
300 read-on
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  castle-1f-9 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
