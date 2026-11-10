\ water-tower-b1-1's place (scripts/story/places/water-tower-b1-1.fs): Riccardo's dungeon - coming in
\ (its parts, flames, sparkles); brought in from the old mansion: she wakes in the cell; the key on
\ the desk; Hewie sent for it - out; the pipes bursting; the things to take; the Sublimatum container;
\ his scene out of the cell's side; the gap she hides in; the hole into the hidden room and back; the
\ replays. Headless:
\   build/openhuey --test tests/places/test_water_tower_b1_1.fs
IN: test-water-tower-b1-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit items.names story.places.water-tower-b1-1 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: reading ( xt n -- flag )   \ (soon, its messages read on and its movies skipped meanwhile)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set
        dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: has ( item -- n )  items:items-of ;
: give ( item -- )  1 game:items send give-item  5 run-frames ;
: started ( cmd -- )  [: started-now ! ;] in-place  game-tick ;
: room-afresh ( -- )  water-tower-b1-1 room!  60 run-frames ;
: fx? ( k -- flag )  [: effect @ 0<> ;] in-place ;
: barred? ( group -- flag )   \ (the first of its triangles barred)
    nav-tris 0 ?do  i over nav-in-group? if  drop i nav-flags barred-floor and 0<> unloop exit  then  loop  drop false ;
: tri ( -- n )  [: her-tri @ ;] her ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in (nothing done yet): the map, the figurine and the disc on the walls; the pipes whole; six flames; the jerky's sparkle
room-afresh
T{ 9 part?  $A part?  8 part?  2 part?  3 part?  4 part?  6 part? -> -1 -1 -1 -1 0 -1 -1 }T
T{ 0 fx?  5 fx?  jerky-fx fx?  miniature-fx fx? -> -1 -1 -1 0 }T
testing ... out of the cell before: its door open
out-of-cell story-flag-set  room-afresh
T{ cell-open-part part? -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing brought in from the old mansion: she wakes in the cell (its scene), the tinted Riccardo loaded, Hewie hers to command
out-of-cell story-flag-clear  cf-13-flag1 story-flag-set  x-37-flag2 story-flag-clear  riccardo-come story-flag-set
game:hewie send join-fiona  10 run-frames
water-tower-b1-1 forced-exit rooms send go-to-room  5 run-frames
T{ [: x-37-flag2 story-flag? ;] 600 soon -> -1 }T
600 play-on
T{ free?  tri $46 =  0 barred?  tintstalker-kind partner-is? hewie-commandable state-flag? -> -1 -1 -1 -1 -1 }T
T{ dungeon-file filed?  cell-file filed? -> -1 0 }T   \ (its $82xx files the other game's: not in this one)
testing the cell's door kicked: it rattles - "I'm not that strong!"
: kick ( -- )  square press  60 run-frames ;
T{ 0 -17e 0e 0e put  kick  [: window-shown 0< 0= ;] 120 soon  window-shown -> -1 not-strong-msg }T
300 play-on
testing the key on the desk: a look at it, then her remark
T{ $11 90 at  said  $11 90 at  said -> key-msg scared-msg }T
testing Hewie sent for it ("go there": the stick up): the key fetched, used; out of the cell, its floor no longer barred
key: I true key-hold  game-tick  key: I false key-hold
T{ [: out-of-cell story-flag? ;] 1500 reading -> -1 }T
600 play-on
T{ prison-key has  0 barred?  cell-open-part part?  free? -> 0 0 -1 -1 }T

testing the pipes: the one by the hall bursts at her kick - over she goes
riccardo-come story-flag-set  stalkers-blind state-flag-set
180 -33e 0e -160e put  square press
T{ [: wtb1-pipe-1-burst story-flag? ;] 60 soon -> -1 }T
300 play-on
T{ 2 part?  3 part?  free? -> 0 -1 -1 }T
testing ... the far one kicked: the Wooden Miniature behind it, taken
180 -48e 0e -289e put  square press
T{ [: wtb1-pipe-2-burst story-flag? ;] 60 soon  miniature-fx fx? -> -1 -1 }T
180 -48e 0e -294e put  circle press  300 play-on
T{ wooden-miniature has  wtb1-miniature story-flag?  miniature-fx fx? -> 1 -1 0 }T

testing the map and the memo, taken together
T{ $12 0 at  circle press  600 play-on  tower-sketch has  folded-memo has  wtb1-map story-flag?  9 part? -> 1 1 -1 0 }T

testing the Sublimatum container: its words, then her remark; a Red Godstone laid in - the Philosopher Earrings
T{ $1B 64 at  said  $1B 64 at  said -> container-msg mansion-msg }T
red-godstone give  $1B 64 at
0 $14 1 red-godstone broadcast item-event  5 run-frames
600 play-on
T{ red-godstone has  philosopher-earrings has  wtb1-container-used story-flag?  free? -> 0 1 -1 -1 }T

testing the gap: in to hide - hidden; out by the button
$19 0 at  circle press
T{ [: fiona-hidden state-flag? ;] 900 soon -> -1 }T
60 run-frames  circle press
T{ [: fiona-hidden state-flag? 0= free? and ;] 900 soon -> -1 }T

testing the hole: in at her yes - the hidden room; back by its way, out of the hole
$15 -90 at  circle press
T{ [: window-shown 0< 0= if  cross press  then  room-id off-map-27 = ;] 900 soon -> -1 }T
60 play-on
water-tower-b1-1 $FE rooms send go-to-room
T{ [: room-id water-tower-b1-1 = ;] 300 soon -> -1 }T
200 play-on
T{ tri $100 =  free? -> -1 -1 }T

testing his scene: out of the cell's side the first time - the tinted Riccardo in, after her; once
riccardo-come story-flag-clear  stalkers-blind state-flag-clear  out-of-cell story-flag-set  room-afresh
tintstalker-kind stalker-ready  30 run-frames
180 90e 0e -108e put  180 90e 0e -120e put   \ (across its gate, z -114 between x 74 and 106)
T{ [: riccardo-come story-flag? ;] 60 reading  [: free? ;] 1500 reading  [: stalker-in? ;] in-place -> -1 -1 -1 }T
T{ riccardo-file filed? -> 0 }T   \ (the other game's)
stalkers-blind state-flag-set  stalker-unloaded  30 run-frames

testing the replays (the extras): each played (skipped) and over
: replays ( v -- )  $2A var!  events-held state-flag-set  water-tower-b1-1 -1 rooms send go-to-room  5 run-frames ;
: replay-over? ( -- flag )  [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon ;
T{ $53 replays  replay-over?  $54 replays  replay-over?  $55 replays  replay-over? -> -1 -1 -1 }T

test-summary
