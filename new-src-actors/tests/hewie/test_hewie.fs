\ Hewie, phase H1: his own mind, keeping with Fiona, his barks, following her from room to room
\ (docs/subsystems/hewie.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/hewie/test_hewie.fs
IN: test-hewie
USING: tester engine game-state actors messages common room-names game hewie.state hewie.body hewie.model hewie.states fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  hewie enter  execute  leave-actor ;
: his! ( xt -- )  hewie enter  execute  leave-actor ;
: action ( -- n )  [: his-action @ ;] his@ ;
: apart ( F: -- d )  hewie body-at fiona body-at vec-dist ;
1 seed !

5 frames
testing before he joins: no body, not shown
T{ hewie body? -> 0 }T
hewie send join-fiona  1 frames
testing he joins at her heel, in her room, calm, the default action chosen
T{ hewie body?  hewie body-room -> -1 room-id }T
T{ apart 20e f< -> -1 }T
T{ hewie body-tri 0< -> 0 }T
T{ [: his-model @ actor act.visible l@ ;] his@ -> 1 }T
T{ [: his-hp @ his-cond @ his-trust @ ;] his@ -> 100 0 0 }T

testing left to himself he does one thing after another, on the floor
variable seen   \ a bit an action (under 0x40)
: watch ( n -- )  0 ?do  game-tick  action dup $40 < if  1 swap lshift seen @ or seen !  else  drop  then  loop ;
: count-bits ( n -- c )  0 swap  begin  ?dup while  dup 1 and rot + swap 1 rshift  repeat ;
0 seen !  1200 watch
T{ seen @ count-bits 3 >= -> -1 }T
T{ hewie body-tri 0< -> 0 }T
T{ apart 200e f<  hewie body-room -> -1 room-id }T   \ (still in her room; free to roam it since the opening lets go of the world)

testing his trust: points to levels (100 280 450 ... 1000)
T{ [: 99 add-trust his-trust @  1 add-trust his-trust @  900 add-trust his-trust @ ;] his@ -> 0 1 7 }T
T{ [: -10000 add-trust his-trust @ ;] his@ -> 0 }T

testing barking at her (action 0xB): a growl-loud bark heard ($1B)
state: ear-state  cell field dummy  end-state
variable loudest
behaviour ear
  on heard ( loud room tri door source -- )  hewie-noise = if  2drop drop loudest @ max loudest !  else  2drop 2drop  then ;
  on heard-nothing ( -- ) ;
end-behaviour
ear ear-state s" ear" spawn constant ear
deliver  ear enter  fiona body-room fiona body-tri fiona body-pos body-place  0 fiona-noise acoustics send listen  leave-actor
: barking ( -- )  [: $B 0 set-action ;] his!  ;
T{ 0 loudest !  barking  300 frames  loudest @ -> $1B }T
ear kill

testing angry he sulks (mood 3 for 450 frames), then calms down
T{ [: 3 -1 set-mood  his-mood @ his-mood-time @ ;] his@ -> 3 450 }T
T{ 451 frames  [: his-mood @ ;] his@ -> 0 }T

\ ---- following her: front-garden-2's exit 0 is a door; she goes out by it (as her exit check does) ----
front-garden-2 -1 rooms send go-to-room  5 frames
hewie send part-from-fiona  2 frames  hewie send join-fiona  2 frames
: door0 ( -- d )  front-garden-2 0 room-exit-door ;
door0 4 * progress pr.doors + 2 swap l!   \ (open)
: obeying ( -- )  [: 1000 add-trust  obeys  0 0 set-action ;] his! ;   \ (trusting: at trust 0 he may dawdle off screen)
: arrives? ( n -- flag )   \ within n frames he is in the room being played
    0 ?do  game-tick  hewie body-room room-id =  [: his-away @ ;] his@ 0= and if  true unloop exit  then  loop  false ;
testing she goes out by the door; he follows - off screen, then comes in after her
obeying  10 frames
T{ 0 rooms send go-through  3 frames  room-id -> front-garden-2 0 room-exit-leads drop }T
T{ hewie body-room room-id = -> 0 }T
variable took
: arrives-in ( n -- frames|-1 )  0 ?do  game-tick  hewie body-room room-id =  [: his-away @ ;] his@ 0= and if  i unloop exit  then  loop  -1 ;
3000 arrives-in dup took !  .( took ) . cr
T{ took @ 0< -> 0 }T
T{ hewie body-tri 0< -> 0 }T

testing parted, he is gone
hewie send part-from-fiona  2 frames
T{ hewie body?  [: his-model @ actor act.visible l@ ;] his@ -> 0 0 }T

test-summary
