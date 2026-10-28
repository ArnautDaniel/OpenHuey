\ house-of-truth-2f-2's place: the Mystery Sketch; the painting's title; the closet; the bobbles; the
\ books; the untitled book in turn with them; the Rubrum Medallion wedged between two, then the books;
\ the medicine's name (marked in her menu); the Viride Medallion Hewie sniffs out; brought here by the
\ forced way, Lorenzo's scene (its fact, the door shut behind her); the replay. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_2f_2.fs
IN: test-house-of-truth-2f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth-2f-2 ;
full-suite   \ (on: waits, scenes, walks - the full run's)
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
: room-afresh ( -- )  house-of-truth-2f-2 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
got-mystery-sketch story-flag-clear  ht2f-2-rubrum-medallion story-flag-clear  ht2f-2-viride-medallion-found story-flag-clear
ht2f-2-viride-medallion story-flag-clear  room-afresh
testing the remarks: the title, the closet, the bobbles, the books (two shelves)
T{ 4 -68 at  said  5 64 at  said  6 -6 at  said -> title-msg closet-msg bobbles-msg }T
T{ 7 92 at  said  9 -70 at  said -> books-msg books-msg }T
testing the untitled book, in turn with the books' remark
T{ 8 -82 at  said  8 -82 at  said  8 -82 at  said -> books-msg untitled-msg books-msg }T
testing the Mystery Sketch
mystery-sketch items:items-of before !
T{ 3 104 at  circle press  300 read-on  got-mystery-sketch story-flag?  sketch-part [: part? ;] in-place  mystery-sketch items:items-of -> -1 0 before @ 1+ }T
testing the Rubrum Medallion wedged between two books; then the books
rubrum-medallion items:items-of before !
T{ $A -48 at  said  ht2f-2-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> wedged-msg -1 before @ 1+ }T
T{ $A -48 at  said -> books-msg }T
testing the medicine's name: marked in her menu
T{ $B 24 at  said  fortis-bit items:item-bit? -> written-msg -1 }T
testing the Viride Medallion: Hewie sniffs it out; taken
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
[: ['] viride-sniffed start drop ;] in-place
T{ [: ht2f-2-viride-medallion-found story-flag? ;] 600 soon  [: viride-fx effect @ 0<> ;] in-place -> -1 -1 }T
400 run-frames  viride-medallion items:items-of before !
T{ 0 37e 1e 84e put  circle press  300 read-on  ht2f-2-viride-medallion story-flag?  viride-medallion items:items-of -> -1 before @ 1+ }T
testing brought here by the forced way: Lorenzo's scene - its fact, the door shut behind her
over-music-d story-flag-clear  house-of-truth-2f-2 forced-exit rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  study-seen resident-flag? ;] 2000 soon  over-music-d story-flag?  study-door doors:door-locked? -> -1 -1 -1 }T
600 play-on
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  house-of-truth-2f-2 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
