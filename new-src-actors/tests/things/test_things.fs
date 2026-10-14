\ The things lying about (things.fs, game/things.c): Fiona lets go of what is in her equipment's
\ slot 2 - thrown, aimed, set down - and what becomes of each. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/things/test_things.fs
IN: test-things
USING: tester engine game-state actors messages room-names game debug keys facts items fiona.state fiona.model ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: have ( item n -- )  game:items send give-item  deliver ;
: equip ( item -- )  progress pr.equipped 8 + l! ;   \ (slot 2)
: count-of ( item -- n )  game:items enter  items:items-of  leave-actor ;
: things ( -- n )  0  128 0 do  i thing-used? if  1+  then  loop ;
: first ( -- i | -1 )  128 0 do  i thing-used? if  i unloop exit  then  loop  -1 ;
: kind@ ( i -- k )  thing-info 2drop 2drop ;
: state@ ( i -- s )  thing-info drop nip nip nip ;
: free? ( -- flag )  [: her-mode @ 0= ;] hers ;
: let-go ( -- )   \ (the triangle, once she is free)
    ['] free? 200 soon drop  key: V true key-hold  game-tick  key: V false key-hold ;

free-play  10 frames  0 game:fiona send costume  front-garden-2 room!  10 frames
140.64e  game:fiona body-pos fdrop fswap fdrop  -217.33e 0e debug:tp-facing  5 frames
testing the ball (slot 2): thrown - her animation ($D00), a ball on its way, hers used up
$90 1 have  $90 equip
T{ things  let-go  [: [: anim@ ;] hers $D00 = ;] 20 soon -> 0 -1 }T
T{ ' things 60 soon drop  things  first kind@  $90 count-of -> 1 0 0 }T
testing it lands and rolls to rest on the floor, a thing of her room
T{ [: first state@ 1 = ;] 200 soon  first thing-info 2drop drop nip  room-id = -> -1 -1 }T
testing her ball gone (its 3 minutes): hers again
first 5399 swap thing-age!  3 frames
T{ things  $90 count-of -> 0 1 }T

testing a powder (antimony, aimed): it bursts on what it hits, and is gone
-1 equip  $91 2 have  $91 equip  20 frames
T{ let-go  ' things 60 soon  first kind@ -> -1 1 }T
T{ [: things 0= ;] 300 soon  $91 count-of -> -1 1 }T
testing a magnesia (set down): there till armed (30 s), then it goes off
-1 equip  $92 1 have  $92 equip  20 frames
T{ let-go  ' things 60 soon  first kind@ -> -1 2 }T
T{ 60 frames  things -> 1 }T
first 899 swap thing-age!
T{ [: things 0= ;] 10 soon -> -1 }T

testing a doll (thrown) is kept in a save: its line written, and back as it lay
-1 equip  $93 1 have  $93 equip  20 frames
T{ let-go  ' things 60 soon  first kind@ -> -1 3 }T
120 frames
: lies ( -- x z )  first thing-at dup sf@ f>s swap 8 + sf@ f>s ;
variable lay-x  variable lay-z  lies lay-z ! lay-x !
game:saves enter  saves:quick-save  leave-actor  deliver  3 frames
things-clear  T{ things -> 0 }T
game:saves enter  saves:quick-load  leave-actor  60 frames
T{ things  first kind@  lies lay-z @ = swap lay-x @ = -> 1 3 -1 -1 }T

test-summary
