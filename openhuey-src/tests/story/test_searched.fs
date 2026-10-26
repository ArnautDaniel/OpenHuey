\ The stalker down on his front, searched (the shared $80, offered by the shared after-phase 2 -
\ cond 0x4B, Pursuer_InStance2Anim): close and facing him, she bends to him and finds the next of
\ his kind's gifts (0xB6) - in a place (story/common.fs) and in a room of converted scripts. Down on
\ his back he isn't offered. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_searched.fs
IN: test-searched
USING: tester engine game-state actors messages keys room-names game debug facts flag-names
    items.names stalker.state stalker.struck stalker.anims fiona.state ;

: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: jerky ( -- n )  chicken-jerky items:items-of ;
: beef ( -- n )  beef-jerky items:items-of ;
: gifts ( -- n )  progress pr.gifts l@ ;
: lying? ( -- flag )  [: anim @ lying-front-anim = ;] his ;
: felled ( xt -- )   \ (him on her floor, out of health, down by that way of falling)
    s" fiona" actor-named body-tri debug:debilitas-tri  5 run-frames
    [: 0 hp !  execute ;] his  ;
: beside ( -- )   \ (her 8 along +x from him, facing him)
    game:debilitas body-pos {: F: x y z :}  x 8e f+ y z -90e tp-facing  3 run-frames ;
: searched ( -- )   \ (the offer taken, its messages read on)
    circle press  60 0 do  window-shown 0< 0= if  cross press  then  5 run-frames  loop ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set
2 story.words:partner-load  5 run-frames   \ (Debilitas the story's stalker, as the story has him)

testing in a place (front-garden-3): down on his front, she beside him facing him - searched, his first gift (chicken jerky, $8011)
front-garden-3 room!  30 run-frames
variable before  jerky before !  0 progress pr.gifts l!  0 progress pr.gift-next l!
' leg-down felled  T{ ' lying? 120 soon -> -1 }T
beside
circle press  T{ [: window-shown $8011 = ;] 200 soon -> -1 }T
searched
T{ jerky  gifts  progress pr.gift-next l@ -> before @ 1+  1 1 }T
testing ... after the first a gift is due 1 in 10 (counted to 8); none due, he comes round (his time down over)
: dues ( n -- hits )  0 swap 0 ?do  story.words:gift-due? if  1+  1 progress pr.gifts l!  then  loop ;
1 progress pr.gifts l!
T{ 2000 dues 140 260 within -> -1 }T   \ (about 200)
: rolls ( n -- )  0 ?do  story.words:gift-due? drop  loop ;
8 progress pr.gifts l!  200 rolls  T{ gifts -> 8 }T   \ (not past 8)
1 progress pr.gifts l!
game:debilitas send stalker-down-over
T{ [: [: cond @ down-cond <> ;] his ;] 400 soon -> -1 }T   \ (his getting up played out)
debug:debilitas-out  2 run-frames

testing ... down on his back: not offered - and he stays down (his search waiting meanwhile)
front-garden-3 room!  30 run-frames  jerky before !
[: fall-back-anim lying-back-anim fall ;] felled  T{ [: [: anim @ lying-back-anim = ;] his ;] 300 soon -> -1 }T
beside  searched
T{ jerky  gifts  [: anim @ lying-back-anim = ;] his -> before @  1 -1 }T
debug:debilitas-out  2 run-frames

testing in a room of converted scripts (front-garden-2): the same offer (cond 0x4B), the next of his row once due (beef jerky)
front-garden-2 room!  30 run-frames  beef before !  0 progress pr.gifts l!
: her-y ( F: -- y )  s" fiona" actor-named body-pos fdrop fswap fdrop ;
140.64e her-y -217.33e 0e tp-facing  5 run-frames   \ (an open spot)
' leg-down felled  T{ ' lying? 120 soon -> -1 }T
beside
circle press  T{ [: window-shown $8011 = ;] 200 soon -> -1 }T
searched
T{ beef  gifts  progress pr.gift-next l@ -> before @ 1+  1 2 }T
debug:debilitas-out  2 run-frames
test-summary
