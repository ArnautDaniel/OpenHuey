\ castle-2f-8's place (scripts/story/places/castle-2f-8.fs): the kiln room and its golem - one, the
\ room's own model (issue #78: two were seen) - the word plate's three words and any other, the
\ remarks, the old parchment, the obsidian choker. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_2f_8.fs
IN: test-castle-2f-8
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    fiona.state story.kit story.places.castle-2f-8 ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: play-on ( n -- )
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: no-golem-figure? ( -- flag )  golem-figure story.base:slot-actor @ 0< ;
: word ( ev -- )  0 swap -1 0 broadcast item-event ;   \ (the word plate used at the golem: its event)
: room-afresh ( -- )  castle-2f-8 room!  60 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  no-flee state-flag-clear
room-afresh
testing one golem: the room's own, before the kiln - nothing else standing in for it
T{ golem-part part?  golem-moved-part part?  golem-dust-part part?  no-golem-figure? -> -1 0 0 -1 }T

testing its remarks: the golem (once, then another), the equipment, the kiln, the mark
T{ 180 10e 0e -28e put  said  180 10e 0e -28e put  said -> golem-msg golem-again }T   \ (before it, where she stands to look)
T{ kiln-area 90 at  said -> kiln-msg }T
T{ mark-area 180 at  said -> mark-msg }T
testing the old parchment, taken
T{ 0 7.826e 0e -7.168e put  said  got-old-parchment story-flag?  parchment-part part? -> file-here-msg -1 0 }T

testing a word that does nothing
T{ 5 word  [: window-shown 0< 0= ;] 200 soon  window-shown -> -1 nothing-msg }T
120 play-on

testing EMETH: the golem comes alive and walks to the door's side - the door open, the golem done, still one
golem-moved story-flag-set  $A word  900 play-on
T{ golem-done story-flag?  golem-door doors:door-locked?  free? -> -1 0 -1 }T
T{ golem-part part?  golem-moved-part part?  golem-dust-part part?  no-golem-figure? -> 0 -1 0 -1 }T
testing ... back in: it stays there
room-afresh
T{ golem-part part?  golem-moved-part part?  golem-dust-part part? -> 0 -1 0 }T

testing METH (another game): the golem to dust, the obsidian choker in it, taken
golem-moved story-flag-clear  golem-done story-flag-clear  room-afresh
golem-crumbled story-flag-set  $B word  900 play-on
T{ golem-done story-flag?  golem-dust-part part?  golem-part part?  [: choker-fx effect @ 0<> ;] in-place -> -1 -1 0 -1 }T
variable before  obsidian-choker items:items-of before !
0 10e 0e -28e put  circle press  400 play-on
T{ got-obsidian-choker story-flag?  obsidian-choker items:items-of -> -1 before @ 1+ }T

testing SALTATIO: the golem dances, and it calms her the first time
golem-crumbled story-flag-clear  golem-done story-flag-clear  room-afresh
$E word
T{ ' doing-busy? 60 soon -> -1 }T
: calmed-said? ( -- flag )  window-shown calmer-msg = ;
T{ ' calmed-said? 3000 soon -> -1 }T
300 play-on
T{ free?  golem-part part?  no-golem-figure? -> -1 -1 -1 }T

testing pushing at the golem: she strains, it won't move
-1 broadcast fiona-straining  2 run-frames  0 broadcast fiona-straining
T{ [: window-shown cant-push-msg = ;] 120 soon -> -1 }T
200 play-on
test-summary
