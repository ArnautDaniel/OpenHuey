\ Hewie fetching (hewie/fetch.fs): Fiona throws the ball - his to fetch; by his skills he goes
\ after it, picks it up and brings it back (her ball hers again), or watches it go; the skills he
\ used move with the praise after; they are kept in the save. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_fetch.fs
IN: test-fetch
USING: tester engine game-state actors messages common room-names game debug keys facts items fiona.state hewie.state hewie.body hewie.model hewie.states hewie.commands hewie.fetch ;

: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: his@ ( xt -- x )  game:hewie enter  execute  leave-actor ;
: his! ( xt -- )  game:hewie enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: have ( item n -- )  game:items send give-item  deliver ;
: equip ( item -- )  progress pr.equipped 8 + l! ;   \ (slot 2)
: count-of ( item -- n )  game:items enter  items:items-of  leave-actor ;
: balls ( -- n )  0  128 0 do  i thing-used? if  i thing-kind 0= if  1+  then  then  loop ;
: free? ( -- flag )  [: her-mode @ 0= ;] hers ;
: throw ( -- )  ['] free? 200 soon drop  key: V true key-hold  game-tick  key: V false key-hold ;
: skills! ( n -- )  [: 6 0 do  dup i cells his-skill + !  loop  drop ;] his! ;
: skill@ ( k -- n )  [: cells his-skill + @ ;] his@ ;
: action ( -- n )  [: his-action @ ;] his@ ;
1 seed !

free-play  10 run-frames  0 game:fiona send costume  castle-1f-1 room!  10 run-frames
$B7 tri-center 0e debug:tp-facing  5 run-frames  game:hewie send join-fiona  30 run-frames
\ a ball rolling slowly away ahead of her, 15 on (where it stops is open floor - a thrown one
\ rolls on to the walls, where he can't go)
: ball-ahead ( -- )
    0  game:fiona body-tri  game:fiona body-at vec@ 15e f+ f>cell >r f>cell >r f>cell  r> r>
    0e f>cell 0e f>cell 0.6e f>cell  s" things" actor-named send thing-made  deliver ;
[: 1000 add-trust  obeys  0 -1 set-mood ;] his!

testing he starts middling at it (16 of 32 each)
T{ 0 skill@ 5 skill@ -> 16 16 }T
testing the ball thrown: his to fetch - after it (0x78), skills at their best
31 skills!  $90 1 have  $90 equip
T{ throw  [: [: his-follows @ 0< 0= ;] his@ ;] 60 soon -> -1 }T
T{ [: action $78 = ;] 300 soon -> -1 }T
[: 0 0 want ;] his!  [: -1 his-follows ! ;] his!  things-clear  10 run-frames   \ (the thrown one gone: she has none)
T{ ball-ahead  [: [: his-follows @ 0< 0= ;] his@ ;] 5 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
[: $78 0 want  10000 his-wait ! ;] his!   \ (his wait for the next thing to do held off)
testing ... at it, he picks it up - gone; hers again (she has none)
T{ [: balls 0= ;] 900 soon  $90 count-of -> -1 1 }T
testing ... and back to her; sat by her a while, his doing counted as praised
T{ [: [: his-act @ ['] st-sat-by-her = ;] his@ ;] 900 soon -> -1 }T
T{ [: action $78 <> ;] 200 soon  [: his-follows @ ;] his@  0 skill@ -> -1 -1 31 }T

testing not going after it (going 0, watching 31): his eyes on it as it goes
[: 0 0 want ;] his!  10 run-frames
0 skills!  31 1 cells [: his-skill + ! ;] his!  $90 equip
T{ throw  [: action $78 = ;] 300 soon -> -1 }T
T{ [: [: his-act @ ['] st-watch = ;] his@ ;] 30 soon -> -1 }T
T{ [: [: his-act @ ['] st-sat-by-her = ;] his@ ;] 300 soon -> -1 }T
testing ... praised: the skills he used up (watching), the one that failed down (going)
T{ [: action $78 <> ;] 200 soon  0 skill@ 1 skill@ -> -1 0 31 }T


testing her command 0x23 with a ball lying in front of her: he answers (0x1D), for that ball next
[: 0 0 want ;] his!  [: -1 his-follows ! ;] his!  things-clear  10 run-frames  31 skills!
$B7 tri-center 0e debug:tp-facing  5 run-frames   \ (back where she began: the throws moved her)
ball-ahead  60 run-frames   \ (lying still)
T{ [: $23 fiona-command  his-follows @ 0< 0=  his-action @ ;] his@ -> -1 -1 $1D }T
testing ... her command acted on (Hewie_ActOnCommand 0x23: no straight walk to her asked): for the ball all the same
[: 0 0 want  -1 his-follows ! ;] his!  things-clear  10 run-frames
$B7 tri-center 0e debug:tp-facing  game:hewie send join-fiona  30 run-frames   \ (both back by where she began)
\ a ball lying still 12 ahead of her (not rolling: not thrown, so not his to fetch till she says)
: ball-still ( -- )
    0  game:fiona body-tri  game:fiona body-at vec@ 12e f+ f>cell >r f>cell >r f>cell  r> r>
    0e f>cell 0e f>cell 0e f>cell  s" things" actor-named send thing-made  deliver ;
ball-still  10 run-frames
T{ [: 0 0 want  -1 his-follows !  go-there-cmd act-on-command  his-follows @ 0< 0=  his-action @  his-next @ ;] his@ -> -1 -1 $1D $78 }T
[: 0 0 want ;] his!
test-summary
