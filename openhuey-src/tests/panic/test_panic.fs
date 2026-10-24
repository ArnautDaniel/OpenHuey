\ the panic and Fiona's fear (docs/subsystems/panic.md, fiona.md F2). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/panic/test_panic.fs
IN: test-panic
USING: tester engine game-state actors messages room-names common facts flag-names game fiona.state fiona.model ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: stage ( -- s )  game:panic enter  panic:stage @  leave-actor ;
: level ( F: -- l )  game:panic enter  panic:level f@  leave-actor ;
: about ( F: x y -- flag )  f- fabs 0.05e f< ;
: her@ ( xt -- x )  fiona enter  execute  leave-actor ;
: bits ( -- b )  [: her-fear-bits @ ;] her@ ;
: anim ( -- id )  ['] anim@ her@ ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;

3 frames
0 fiona send costume  1 frames   \ (her clothes: these are her mechanics, not the new game's slip)
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
T{ fear f>s  key: E press  60 $403 seen  -> 0 -1 }T
T{ 60 frames  fear 2.95e about  [: her-doing @ ;] her@ -> -1 0 }T   \ (10 up, then calming 0.15 a frame standing)

testing the screen's heartbeat (ScreenFade_Frame): at each breath the pulse 0, back to $80 by 4 a frame; the grey with the level
: pulse ( -- n )  game:panic enter  panic:pulse @  leave-actor ;
: grey ( -- n )  game:panic enter  panic:shake @  leave-actor ;
: beat-seen ( -- flag )  120 0 do  game-tick  pulse 4 < if  true unloop exit  then  loop  false ;
T{ 85 game:panic send panic-level!  beat-seen -> -1 }T
T{ 10 frames  pulse 36 49 within  grey 0> -> -1 -1 }T   \ (4 a frame)
T{ 30 frames  pulse -> $80 }T

testing the passing part lasts: a fright's half held 30 frames, then down by the recovery (0.167 a frame)
: passing ( F: -- p )  game:panic enter  panic:passing f@  leave-actor ;
0 game:panic send panic-level!  game-tick
T{ 40e f>cell game:panic send fright  game-tick  passing 20e about -> -1 }T
T{ 25 frames  passing 20e about  level 35e f> -> -1 -1 }T
T{ 65 frames  passing 10e 1e f- f>  passing 10e 1e f+ f< -> -1 -1 }T
T{ 100 frames  passing f0= -> -1 }T

testing Hewie near her weighs the inputs: within 20 calming twice as fast (0.2 a frame, calm danger)
: near-frames ( near n -- )  0 ?do  dup broadcast hewie-near  game-tick  loop  drop ;
70 game:panic send panic-level!  game-tick
T{ level  1 10 near-frames  level f- 2e about -> -1 }T
T{ level  10 frames  level f- 1e about -> -1 }T
T{ level  3 10 near-frames  level f- 1.5e about -> -1 }T

testing no breath while the world is held - the beat, but not her breath nor the pad's (the screen fade's level 0: Panic_Update under flag 8)
: felt? ( -- flag )  rumble-now nip 0> ;
: breath-felt ( -- flag )  120 0 do  game-tick  felt? if  true unloop exit  then  loop  false ;
85 game:panic send panic-level!  world-held state-flag-set
T{ breath-felt -> 0 }T
T{ beat-seen -> -1 }T
world-held state-flag-clear  85 game:panic send panic-level!
T{ breath-felt -> -1 }T

testing panicking from the level: its first breath at once and the shake a step toward 60 (Panic_Stage's fall-through)
0 game:panic send panic-level!  40 frames
T{ 100 game:panic send panic-level!  game-tick  stage pulse grey -> 4 0 1 }T

test-summary
