\ old-mansion-2f-11's place: the stepladder (its remarks), the Jupiter Key fallen; the books in
\ turn; the journals and their file; the notes and their recipes; the Magnesia. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_11.fs
IN: test-old-mansion-2f-11
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-11 ;
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
: room-afresh ( -- )  old-mansion-2f-11 room!  60 run-frames ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: strike ( -- flag )   \ (her blow on it seen; its doing run)
    3e 5e -35.5e blow-at  [: ladder-struck?  ['] struck start drop ;] in-place  [: body-point-off ;] as-her  120 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the stepladder: its state, then what's on top
om2f-11-flag1 story-flag-clear  room-afresh
T{ 9 60 at  said -> ladder-msg }T
T{ 9 60 at  said -> on-top-msg }T
testing the stepladder struck: wobbling, the third time the key falls
T{ strike  om2f-11-flag1 story-flag?  [: tries @ ;] in-place -> -1 0 1 }T
T{ strike drop  strike drop  om2f-11-flag1 story-flag? -> -1 }T
testing the Jupiter Key fallen: taken
om2f-11-flag1 story-flag-set  got-jupiter-key story-flag-clear  room-afresh  jupiter-key items:items-of before !
0 7.3e 0e -32.3e put  circle press  300 read-on
T{ got-jupiter-key story-flag?  jupiter-key items:items-of -> -1 before @ 1+ }T
testing the books: their remarks in turn
T{ $B 90 at  said -> books-msg }T
T{ $C 0 at  said -> languages-msg }T
T{ $E 90 at  said -> titles-msg }T
T{ $B 90 at  said -> books-msg }T
testing the journals: their file
journals-examined story-flag-clear
T{ $A -10 at  said  journals-examined story-flag? -> journals-msg -1 }T
200 read-on
T{ journal-file filed? -> -1 }T
testing the notes: their recipes marked
T{ $11 -45 at  said -> written-msg }T
300 read-on
T{ $63 items:item-bit?  $65 items:item-bit? -> -1 -1 }T
T{ $12 -45 at  said -> written-msg }T
300 read-on
T{ $46 items:item-bit?  $47 items:item-bit? -> -1 -1 }T
testing the Magnesia: taken
om2f-11-magnesia story-flag-clear  room-afresh  magnesia items:items-of before !
0 -6.82e 0e -14.25e put  circle press  300 read-on
T{ om2f-11-magnesia story-flag?  magnesia items:items-of -> -1 before @ 1+ }T
test-summary
