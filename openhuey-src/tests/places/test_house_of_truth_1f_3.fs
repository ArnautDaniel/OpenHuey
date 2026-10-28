\ house-of-truth-1f-3's place: coming in - the four flames lit, the candlestick there, the Rubrum
\ Medallion's sparkle; the first time she goes on from the hall, the scene (its fact, Lorenzo after
\ her); the Golden Candlestick taken (its file); the bars' remark; the Rubrum Medallion; the Viola
\ Medallion Hewie sniffs out, taken; the replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_3.fs
IN: test-house-of-truth-1f-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth-1f-3 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
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
: room-afresh ( -- )  house-of-truth-1f-3 room!  60 run-frames ;
: fx? ( k -- flag )  [: effect @ 0<> ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
got-golden-candlestick story-flag-clear  ht1f-3-rubrum-medallion story-flag-clear  ht1f-3-viola-medallion-found story-flag-clear
ht1f-3-viola-medallion story-flag-clear  ht1f-3-flag1 story-flag-set  items-40-as-70 story-flag-clear  room-afresh
testing coming in: the flames lit, the candlestick there, the Rubrum Medallion's sparkle
T{ 0 fx?  3 fx?  candlestick-part [: part? ;] in-place  rubrum-fx fx? -> -1 -1 -1 -1 }T
testing the Golden Candlestick taken: its part gone, its file
golden-candlestick items:items-of before !
T{ candlestick-area 0 at  circle press  300 read-on  got-golden-candlestick story-flag?  candlestick-part [: part? ;] in-place -> -1 0 }T
T{ golden-candlestick items:items-of  candlestick-file filed? -> before @ 1+ -1 }T
testing the bars' remark
T{ bars-area 45 at  said -> bars-msg }T
testing the Rubrum Medallion
rubrum-medallion items:items-of before !
T{ 0 -23.41e 1e -14.78e put  circle press  300 read-on  ht1f-3-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> -1 before @ 1+ }T
testing the Viola Medallion: Hewie sniffs it out; taken
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
house-of-truth-1f-3 3e 0e 168e nav-tri 0 game:hewie send hewie-place  30 run-frames
[: ['] viola-sniffed start drop ;] in-place
T{ [: ht1f-3-viola-medallion-found story-flag? ;] 600 soon  viola-fx fx? -> -1 -1 }T
400 run-frames  viola-medallion items:items-of before !
T{ 0 3e 1e 168e put  circle press  300 read-on  ht1f-3-viola-medallion story-flag?  viola-medallion items:items-of -> -1 before @ 1+ }T
testing the first time she goes on from the hall: the scene - its fact; Lorenzo after her
ht1f-3-flag1 story-flag-clear  lorenzo2-kind story.words:partner-load  5 run-frames  room-afresh
0 -12.5e 0e 17e put
T{ 0 -12.5e 0e 37e put  ht1f-3-flag1 story-flag? -> -1 }T
T{ [: movie-skipped state-flag-set  hall-seen resident-flag? ;] 2000 soon -> -1 }T
T{ [: [: stalker-in? ;] in-place ;] 300 soon  summoner-on state-flag? -> -1 -1 }T
300 play-on  game:lorenzo2 send stalker-out  summoner-on state-flag-clear  10 run-frames
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  house-of-truth-1f-3 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
