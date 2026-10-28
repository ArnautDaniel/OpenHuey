\ water-tower-1f-1's place: coming in - the holes in its floor, the Medallion's sparkle; the water (a
\ recovery point, then its second remark); the clock; the Medallion Hewie sniffs out, then taken;
\ Riccardo taken off when his time is up out of her sight. Headless:
\   build/openhuey --test tests/places/test_water_tower_1f_1.fs
IN: test-water-tower-1f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.water-tower-1f-1 ;
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
: room-afresh ( -- )  water-tower-1f-1 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: answer-no ( -- )  60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
wt1f-1-medallion-found story-flag-set  wt1f-1-medallion story-flag-clear  got-doc-recovery story-flag-set  room-afresh
testing coming in: the holes in its floor, the Medallion's sparkle (found, not taken)
T{ $1F nav-flags floor-flags:hole-floor and 0<>  $89 nav-flags floor-flags:hole-floor and 0<>  [: medallion-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T
testing the water: a recovery point; then its second remark
T{ water-area -44 at  circle press  [: window-shown 0< 0= if  cross press  then  [: rested @ ;] in-place  doing-busy? 0= and ;] 900 soon -> -1 }T
T{ water-area -44 at  said -> helped-msg }T
testing ... not facing it: nothing
T{ water-area 136 at  [: fiona water-area in-area?  fiona -44 50 heading? ;] in-place -> -1 0 }T
testing the clock: saving (or Hewie not by her) - no
T{ clock-area 0 at  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
answer-no
T{ doing-busy? -> 0 }T
testing the Medallion: taken
medallion items:items-of before !
T{ 0 -39.66e 1e 0.99e put  circle press  300 read-on  wt1f-1-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing ... Hewie sniffs it out first
wt1f-1-medallion-found story-flag-clear  wt1f-1-medallion story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
T{ [: medallion-fx effect @ 0<> ;] in-place -> 0 }T
[: ['] medallion-sniffed start drop ;] in-place
T{ [: wt1f-1-medallion-found story-flag? ;] 600 soon -> -1 }T
400 run-frames
testing Riccardo: in her sight, his time up - he stays; out of her sight - taken off (not while the summoner still counts)
game:hewie send part-from-fiona  room-afresh  4 story.words:partner-load  5 run-frames
water-area -44 at  [: 60 summoner-waits ;] in-place  5 run-frames
[: water-tower-1f-1 200 stalker-to ;] in-place  30 run-frames  [: 0 summoner-waits ;] in-place  60 run-frames
T{ [: stalker-in?  stalker unseen? ;] in-place -> -1 0 }T
[: partner send stalker-out ;] in-place  10 run-frames  [: 60 summoner-waits ;] in-place  5 run-frames
[: water-tower-1f-1 10 stalker-to ;] in-place  30 run-frames
T{ [: stalker-in?  stalker unseen? ;] in-place -> -1 -1 }T
T{ [: 0 summoner-waits ;] in-place  [: [: stalker-in? 0= ;] in-place ;] 60 soon -> -1 }T
test-summary
