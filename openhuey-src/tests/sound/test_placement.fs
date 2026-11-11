\ where a placed sound is heard from (src/platform/snddrv.c `place`: SndLib_Place with the game's
\ own output 2 - the surround tables, SndDriver_Start's 0x360002 - and the options' sound out
\ combining its two sides, SubScreen_ApplyOptions). Headless:
\   build/openhuey --test tests/sound/test_placement.fs
IN: test-placement
USING: tester engine game-state actors messages game debug facts flag-names room-names ;

: voice ( -- l r )   \ (the first effects voice sounding)
    48 24 do  i voice-lr if  unloop exit  then  2drop  loop  0 0 ;
free-play 30 run-frames  0 game:fiona send costume  10 run-frames
fvariable px  fvariable py  fvariable pz
game:fiona body-pos pz f! py f! px f!
: at ( F: dx -- )   \ (a door's creak beside her, across the screen from her by dx)
    px f@ f+  py f@ 10e f+  pz f@  $27 5 0 0 sound-at  1 run-frames ;
testing on the screen's left it is louder on the left, on its right louder on the right
T{ -40e at  voice > -> -1 }T
60 run-frames
T{ 40e at  voice < -> -1 }T
60 run-frames
testing the sound out mono (0): its two sides alike
T{ 0 sound-out!  -40e at  voice = -> -1 }T
1 sound-out!
testing the room's sounds kept (state flag $27, the House of Truth's colour rooms): coming into another room
testing ... the bank before stays - its sounds go on (SceneGame_EnterRoom); the flag off, the room's own
create was-bank 64 allot  variable was-len
: bank@ ( -- addr len )  6 sound-bank-file ;
6 sound-load  bank@ dup was-len !  was-bank swap move
keep-room-sounds state-flag-set  castle-1f-1 room!  30 run-frames  6 sound-load
T{ bank@ was-bank was-len @ compare -> 0 }T
keep-room-sounds state-flag-clear  6 sound-load
T{ bank@ was-bank was-len @ compare 0= -> 0 }T
test-summary
