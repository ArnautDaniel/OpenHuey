\ off-map-27's place: by its way in - placed, the world let go; the first time (the costumes
\ unlocked) the "Costumes" document filed; the machine topping up what she lacks (its sparkle), then
\ nothing; the slot machine - a right word's jewellery once (the Magist Medallion if she has it),
\ any other nothing; its remarks; the inscription; the book (the combinations marked read); the
\ slots' menu; the chest before her costumes (the castle's outfit, the one she took off), the
\ costumes' chest after; out by its way, faded - back to the room she came from. Headless:
\   build/openhuey --test tests/places/test_off_map_27.fs
IN: test-off-map-27
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names gameover story.places.off-map-27 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: hput ( deg F: x y z -- )  on-floor  s>f deg>rad xyzw>cells  game:hewie send place  10 run-frames ;   \ (Hewie, there on the floor)
: hat ( area deg -- )  >r area-middle drop r> hput ;
: his-camera@ ( -- set path )  game:hewie cam-setup-of ;
: his-tri ( -- tri )  [: self body-tri ;] his ;
: on-tri ( tri -- )  0 swap tri-center put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: spare@ ( -- room )  game:rooms enter  rooms:spare @  leave-actor ;   \ (the room loaded ahead; -1 none)
: her-camera@ ( -- set path )  game:fiona cam-setup-of ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: hacross ( gate side -- )   \ (Hewie stepped over the gate's line, as across)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  hput
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  hput  10 run-frames ;
: hinto ( area -- )   \ (a gate: crossed one way - the one that gives him a camera, if either does - else back too)
    dup area-kind if  0 hat exit  then
    game:hewie 99 99 cam-char-setup  dup 1 hacross  his-camera@ 99 = swap 99 = and if  -1 hacross  else  drop  then ;
: into ( area -- )   \ (a gate crossed both ways; else its middle)
    dup area-kind 0= if  dup 1 across  -1 across  else  0 at  then ;
: leads ( room exit -- room' )  room-exit-leads drop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: replay-for ( act -- )   \ (the theatre's variable set to a movie its room replays by that act)
    {: act :}  110 0 do  i $2A var!  [: replay-act ;] in-place act = if  unloop exit  then  loop ;
: over-kind ( -- kind )  game:gameover enter  gameover:kind @  leave-actor ;
: played-till ( xt n -- flag )   \ (played on - its messages read, its movies skipped - till xt holds)
    0 ?do  dup execute if  drop true unloop exit  then
        window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop
    drop false ;
: quiet ( -- )  events-held state-flag-clear  off-map-0d 1 room-by!  30 run-frames  world-held state-flag-set ;   \ (between replays: a room of no scenes)
variable before
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
: answer-no ( -- )  60 run-frames  cross press  30 run-frames  key: Down dup true key-hold  game-tick  false key-hold  game-tick  10 run-frames  cross press  100 run-frames ;
: in ( -- )  off-map-27 $FE room-by!  60 run-frames  300 read-on ;
: has ( item -- n )  items:items-of ;
: none-of ( item -- )  begin  dup has 0> while  dup game:items send take-item  5 run-frames  repeat  drop ;
: one-of ( item -- )  dup has 0= if  1 game:items send give-item  5 run-frames  else  drop  then ;
: ev ( ev -- )  0 swap -1 0 broadcast item-event  5 run-frames ;
: word-is ( item -- )  1 swap broadcast item-var ;   \ (the word plate's: what it gives - items.fs)
variable before
testing in by its way, the costumes unlocked: placed, the world let go; the document filed, once
fg-3-flag3 story-flag-clear  x-37-flag2 story-flag-set  got-doc-action story-flag-clear  world-held state-flag-set
antimony-powder none-of  magnesia one-of
T{ in  [: her-tri @ ;] her  world-held state-flag?  got-doc-action story-flag?  doc-costume items:items-of 0> -> $60 0 -1 -1 }T
testing the machine: she lacks the Antimony Powder - its sparkle; one given; then nothing there
T{ [: topping-fx effect @ 0<> ;] in-place  antimony-powder has -> -1 0 }T
T{ 0 -21.456e 0e 6.443e put  0 -21.456e 0e 6.443e put  -90 -21.456e 0e 6.443e put  circle press  300 read-on  antimony-powder has  [: taken @ ;] in-place -> 1 -1 }T
T{ said -> nothing-msg }T
testing ... nothing lacking: a strange machine
T{ in  -90 -21.456e 0e 6.443e put  said -> strange-msg }T
testing the inscription
T{ inscription-area -180 at  circle press  shown-soon -> written-msg }T
300 read-on
testing the book: read, its combinations marked
T{ book-area 90 at  said  $42 items:item-bit?  $44 items:item-bit? -> archaic-msg -1 -1 }T
T{ said -> archaic-msg }T
testing the slots' menu (by the board)
T{ -100 -16.2e 0e -5.68e put  circle press  [: subscreen-wanted state-flag? ;] 120 soon -> -1 }T
[: triangle press  subscreen-wanted state-flag? 0= ;] 300 soon drop  30 run-frames
testing the chest before her costumes: no document - a chest; the document, in the scalpel's outfit - the castle's
got-doc-action story-flag-clear
T{ chest-area 180 at  said -> chest-msg }T
got-doc-action story-flag-set  3 $26 var!
T{ chest-area 180 at  circle press  shown-soon  120 read-on  [: $26 var@ 2 = ;] 600 soon -> castle-outfit-msg -1 }T
testing ... in another: the scalpel's (no, kept as she is)
T{ chest-area 180 at  circle press  shown-soon  answer-no  $26 var@ -> scalpel-msg 2 }T
testing the costumes' chest once she has them: the question (no: nothing changed)
fg-3-flag3 story-flag-set  in
T{ chest-area 180 at  circle press  shown-soon  answer-no  $26 var@ -> change-msg 2 }T
testing the slot machine's remark: none given yet - its slot; one given - a plate-key's machine
x-27-magist-medallion story-flag-clear  x-27-magist-medallion-2 story-flag-clear  x-27-magist-medallion-3 story-flag-clear  x-27-magist-medallion-4 story-flag-clear
T{ second-chest-area 180 at  said -> slot-msg }T
testing the slot machine: a right word - its jewellery, once; had already - the Magist Medallion
alchemist-earrings has before !
T{ alchemist-earrings word-is  medallion-ev ev  600 read-on  alchemist-earrings has  x-27-magist-medallion story-flag? -> before @ 1+ -1 }T
magist-medallion has before !
T{ alchemist-earrings word-is  medallion-ev ev  600 read-on  magist-medallion has -> before @ 1+ }T
T{ second-chest-area 180 at  said -> plate-key-msg }T
testing ... any other word: nothing happened
T{ no-medallion-ev ev  [: window-shown nothing-happened-msg = ;] 600 soon -> -1 }T
300 read-on
testing out by its way ($FE): faded, the world held - back to the room she came from (kept loaded ahead)
front-garden-3 0 room-by!  60 run-frames  in
T{ way-out-area 90 at  circle press  [: room-id off-map-27 <> ;] 400 soon  room-id  world-held state-flag? -> -1 front-garden-3 -1 }T
test-summary
