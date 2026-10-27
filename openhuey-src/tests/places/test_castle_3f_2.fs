\ castle-3f-2's place (scripts/story/places/castle-3f-2.fs): the 3F corridor - once she has rested,
\ the first time on from the door, Daniella ahead (the door tried behind her, then locked; Hewie
\ brought), once. Headless:
\   build/openhuey --test tests/places/test_castle_3f_2.fs
IN: test-castle-3f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit story.places.castle-3f-2 doors ;

: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: room-afresh ( -- )  castle-3f-2 room!  60 run-frames ;
: stepped-on ( -- )  by-the-door-area 0 at  to-exit-1-area 0 at ;   \ (out of the area by the door)

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
daniella-kind story.words:partner-load  5 run-frames

testing before she has rested: on from the door, nothing
c1f-14-flag2 story-flag-clear  c3f-2-flag1 story-flag-clear  room-afresh  stepped-on
T{ doing-busy?  c3f-2-flag1 story-flag? -> 0 0 }T

testing once she has rested: Daniella ahead - the door locked behind, Hewie brought, she free after
c1f-14-flag2 story-flag-set  hewie-freed story-flag-set  room-afresh  the-door unlock  doors-redone  stepped-on
T{ doing-busy?  c3f-2-flag1 story-flag? -> -1 -1 }T
T{ [: doing-busy? 0= ;] 900 soon  free? -> -1 -1 }T
T{ the-door doors:door-locked?  partner body-room  game:hewie body-room -> -1 castle-3f-2 castle-3f-2 }T
testing ... and kept from her alert after (till the old mansion's file)
30 run-frames
T{ [: stalker-told? if  st-alert-now @  else  0  then ;] in-place -> 0 }T
testing ... once: back in, on again - nothing
partner send stalker-out  room-afresh  stepped-on
T{ doing-busy? -> 0 }T
test-summary
