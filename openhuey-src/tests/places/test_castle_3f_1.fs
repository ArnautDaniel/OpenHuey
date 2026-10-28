\ castle-3f-1's place (scripts/story/places/castle-3f-1.fs): the 3F hall - the boxes below once the
\ way is open; the symbol door locked (tried, then its symbol; the file); the Saturnus Key at it
\ (before the chapter turns Daniella's scene and out; after, it unlocks); Daniella busy here once she
\ has rested; the movie replayed. Headless:
\   build/openhuey --test tests/places/test_castle_3f_1.fs
IN: test-castle-3f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit story.places.castle-3f-1 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: room-afresh ( -- )  castle-3f-1 room!  60 run-frames ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-the-door ( -- )  0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;   \ (her side of the symbol door)
: ~= ( F: a b -- flag )  f- fabs 0.01e f< ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
c1f-14-flag2 story-flag-clear  over-music-b story-flag-clear

testing the boxes below once the way is open: where they ended
c2f-9-flag1 story-flag-set  room-afresh
T{ s" oshi00" place-x object@ -35e ~=  s" oshi03" place-z object@ 35e ~= -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
c2f-9-flag1 story-flag-clear

testing the symbol door locked: tried ("locked"), then its symbol - the file both times
symbol-door s" doors" actor-named send lock  room-afresh  by-the-door
T{ said  symbol-file filed? -> locked-msg -1 }T
by-the-door
T{ said -> symbol-msg }T

testing the Saturnus Key at it after the chapter turned: unlocked, the key spent, "Used"
over-music-b story-flag-set  saturnus-key 1 items send give-item  10 run-frames  room-afresh  by-the-door
1 use-on-place  [: window-shown used-msg = ;] 600 soon drop
T{ window-shown  symbol-door doors:door-locked?  saturnus-key items:items-of -> used-msg 0 0 }T
200 play-on

testing ... before: Daniella's scene ("Dinner is served") - out by the forced way
over-music-b story-flag-clear  symbol-door s" doors" actor-named send lock  saturnus-key 1 items send give-item
room-afresh  by-the-door  1 use-on-place  900 play-on
T{ room-id castle-3f-1 = -> 0 }T

testing Daniella here once she has rested: busy at her task, unafraid of, no camera on her - let be as she leaves
c1f-14-flag2 story-flag-set  c3f-2-flag1 story-flag-clear  daniella-kind story.base:partner-load  5 run-frames
room-afresh
T{ partner body-room  stalker-no-fear state-flag?  no-stalker-camera state-flag?  force-followed state-flag? -> castle-3f-1 -1 -1 -1 }T
castle-1f-1 room!  20 run-frames
T{ force-followed state-flag? -> 0 }T
c1f-14-flag2 story-flag-clear  partner send stalker-out  stalker-no-fear state-flag-clear  no-stalker-camera state-flag-clear

testing the movie replayed (the extras): played, back to the extras room
off-map-37 room!  30 run-frames   \ (from the extras room, as the theatre sends her)
0 $2A var!  events-held state-flag-set  room-afresh  600 play-on
T{ room-id  events-held state-flag? -> off-map-37 0 }T
test-summary
