\ the panic and Fiona's fear (docs/subsystems/panic.md, fiona.md F2). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/panic/test_panic.fs
IN: test-panic
USING: tester engine game-state actors messages room-names common game fiona.state fiona.model ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: stage ( -- s )  game:panic enter  panic:stage @  leave-actor ;
: level ( F: -- l )  game:panic enter  panic:level f@  leave-actor ;
: about ( F: x y -- flag )  f- fabs 0.05e f< ;
: her@ ( xt -- x )  fiona enter  execute  leave-actor ;
: bits ( -- b )  [: her-fear-bits @ ;] her@ ;
: anim ( -- id )  ['] anim@ her@ ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;

3 frames
game:story send story-stop  deliver  progress pr.state 8 0 fill   \ (no room scripts: this test sets the flags by hand)
testing calm at the start
T{ stage -> 0 }T
testing a level set by the story: its stage
T{ 70 game:panic send panic-level!  game-tick  stage -> 1 }T
testing calming with time (calm danger: 0.1 a frame) - down to stage 0 under 60
T{ 99 frames  level 60e about -> -1 }T
T{ 5 frames  stage -> 0 }T

testing a fright: half of it lasting, half passing (30 -> +30)
0 game:panic send panic-level!  game-tick
T{ 30e f>cell game:panic send fright  game-tick  level 30e about -> -1 }T

testing out of breath from stage 2: Fiona's bit 1
T{ 80 game:panic send panic-level!  3 frames  stage  bits 1 and -> 2 1 }T

testing panicking (stage 4): she screams, her fear bit 2, the panic run
T{ 4 game:panic send panic-stage!  3 frames  stage  bits 2 and -> 4 2 }T
T{ key: W hold  8 frames  anim -> $206 }T   \ (by frame 13 the run has taken her into a wall: a stumble)
T{ key: W release  anim dup $206 = swap $1001 = or -> -1 }T      \ (she runs on blindly, or stumbles)
testing running blindly into walls: stumbles (0x1001), then a fall (0xB00) that ends the panic
: seen ( n anim -- flag )  false swap rot 0 ?do  game-tick  dup anim = if  nip true swap leave  then  loop  drop ;
T{ 600 $1001 seen -> -1 }T
testing whatever happens, the panic passes and she is free again
T{ 1200 frames  stage  [: her-doing @ ;] her@ -> 0 0 }T

testing a fall (one stumble in five: the dice loaded) ends the panic early, she stays down
testing until it has passed (0xB01), then gets up (0xB02)
: loaded ( -- )   \ the next roll under 0.2
    1 begin  dup seed !  rnd 0.2e f< 0= while  1+  repeat  seed ! ;
: fall-seen ( n -- flag )  false swap 0 ?do  loaded game-tick  anim $B00 = if  drop true leave  then  loop ;
4 game:panic send panic-stage!  key: W hold  5 frames
T{ 900 fall-seen  key: W release  3 frames  stage -> -1 5 }T
T{ 200 $B01 seen  stage -> -1 5 }T
T{ 900 $B02 seen -> -1 }T
T{ 120 frames  stage  [: her-doing @ ;] her@ -> 0 0 }T

testing turning to flee (Q): she whips round (0x403), her fear up 10
: fear ( F: -- f )  fiona enter  her-fear f@  leave-actor ;
: press ( key -- )  dup true key-hold  game-tick  false key-hold ;
T{ fear f>s  key: Q press  60 $403 seen  -> 0 -1 }T
T{ 60 frames  fear 2.95e about  [: her-doing @ ;] her@ -> -1 0 }T   \ (10 up, then calming 0.15 a frame standing)

test-summary
