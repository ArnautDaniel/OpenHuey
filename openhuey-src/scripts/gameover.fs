\ gameover.fs - the game over (docs/subsystems/gameover.md; src/game/gameover.c, SceneGame's
\ SubTransition): when the game ends (the `caught` flag: Fiona dragged or carried off, held; the
\ story's own) the world stands still behind it - the stage music faded, any movie stopped,
\ the panic held, the actors and the room still - then its music (by the story so far), the
\ room's tint going purple and its fog clear over 60 frames, the GAMEOVER movie, the tint
\ drifting to blue for a minute (a face button cuts it short), black over 30 frames - and a
\ fresh game: back to the title (`soft-reset`, as the original makes its title scene anew).
\ A death in a movie (kind 0: the story's 0x8B 0) leaves the world running: its music (6) at
\ once, the movie stopped but its last picture kept over everything, then the tints and on.
IN: gameover
USING: engine game-state actors messages keys facts flag-names story-names ps2-screen ;

6 constant movie-layer   9 constant black-layer

\ how it goes (`game-over-kind`; progress +0x73EB00): a death in a movie; faded out; the story's
\ end - over at once, the ending follows (ending.fs); the kind with its own music and sound
enum: over-kind  movie-death faded-out story-end own-music-death ;
\ the sequence's steps (none: not running)
-1 constant not-running
enum: over-step  music-out freeze-world wait-movie afterimage music-in tinting 7 over-movie
    drift-start drifting to-black restart over-done ;
\ a look's tint (slot $1F): its two colour words and its sharp byte
begin-structure /tint  lfield: tint-a  lfield: tint-b  cfield: tint-sharp  3 + end-structure
$1F constant tint-slot   $1D constant fog-slot

state: gameover-state
  cell field kind          \ (enum: over-kind)
  cell field step          \ (enum: over-step)
  cell field timer
  /tint field tint-from  /tint field tint-to   \ the tint from where it was, and to
  16 field fog-from        \ the fog's colours and range
  16 field look-at         \ (a look's parameters, as set)
  cell field held          \ (a movie death) the movie's last picture kept, drawn over everything
end-state

: music-id ( -- id )  s" music" actor-named ;

\ ---- its music (GameOver: by the story so far; the own-music kind's own) ----
: challenge-track ( -- n )   \ (the mini-game's run - GameOver_StateSpecial: by its challenge, A..D)
    progress pr.vars $2E + c@ case  1 of  $B  endof  2 of  $F  endof  3 of  $C  endof  >r $A r>  endcase ;
: track ( -- n )
    hewie-played? if  challenge-track exit  then
    kind @ own-music-death = if  $47 exit  then
    over-music-f story-flag? if  $F exit  then
    over-music-e story-flag? if  $E exit  then
    over-music-d story-flag? if  $D exit  then
    over-music-c story-flag? if  $C exit  then
    over-music-b story-flag? if  $B exit  then  $A ;
$FF constant no-track
: bgm ( track -- )  0 1e f>cell music-id send bgm-want ;

\ ---- the room's tint and fog, a step on: `t` of `n` back toward where they were ----
: mix ( from to t n -- c )   \ each byte: to + t * (from - to) / n
    {: was goal t n | c :}
    4 0 do
        was i 8 * rshift $FF and  goal i 8 * rshift $FF and  {: f g :}
        g  f g - t * n / +  $FF and  i 8 * lshift  c or to c
    loop  c ;
: tint-toward ( a b -- )   \ (from where it is now to those colours, its sharpness kept)
    {: a b :}  tint-from /tint 0 fill  tint-slot tint-from look@ drop
    a tint-to tint-a l!  b tint-to tint-b l!  tint-from tint-sharp c@ tint-to tint-sharp c! ;
: tints-start ( -- )   \ (the tint to purple, the fog to clear - from where they are)
    fog-from 16 0 fill  fog-slot fog-from look@ drop
    $AA0000C8 $FFFFFFFF tint-toward ;
: tint! ( t n -- )   \ (the tint `t` of `n` back toward tint-from)
    {: t n :}
    tint-from tint-a l@  tint-to tint-a l@  t n mix  look-at tint-a l!
    tint-from tint-b l@  tint-to tint-b l@  t n mix  look-at tint-b l!
    tint-to tint-sharp c@ look-at tint-sharp c!  tint-slot look-at 9 look-set ;
: fog! ( t -- )   \ (the fog `t` of 60 back toward fog-from - to none; its range kept)
    {: t :}  fog-from look-at 16 move
    fog-from l@ 0 t 60 mix look-at l!  fog-from 4 + l@ 0 t 60 mix look-at 4 + l!
    fog-slot look-at 16 look-set ;
: tints-step ( -- )
    timer @ 60 tint!  timer @ fog!
    timer @ 0= if
        s" SYSTEM/GAMEOVER.SFD" movie-open if  $41 $10 $20 movie-compose  over-movie step !   \ (class 6: three-level)
        else  drift-start step !  then
    else  -1 timer +!  then ;
1800 constant drift-frames   \ (a minute)
: start-drift ( -- )   \ (the movie over: the tint to blue over a minute)
    movie-status 1 = if  exit  then
    drift-frames timer !  $82000082 $FFFFFFFF tint-toward  drifting step ! ;
: skip? ( -- flag )   \ (a face button)
    circle button-pressed? cross button-pressed? or  square button-pressed? or  triangle button-pressed? or ;
: drift ( -- )
    timer @ drift-frames tint!
    -1 timer +!  timer @ 0=  skip? or if  no-track bgm  0 timer !  to-black step !  then ;

\ ---- the sequence (GameOver_StateOthers / StateMode3: game_over) ----
: freeze ( -- )   \ (the panic held - the camera still - the actors and the room still; the room still
    panic-held state-flag-set  1 still!  0e cam-shake!  chars-hide   \ drawn, without the characters, from the room's next camera set)
    s" camera" actor-named send camera-hold ;
: run ( -- )
    step @ case
        music-out of  0 0 30 music-id send music-op  no-track bgm  movie-close  freeze-world step !  endof   \ (the stage music and the room's track out)
        freeze-world of  freeze  wait-movie step !  endof
        wait-movie of  0 frame-blend  movie-status 1 <> if  64 timer !  afterimage step !  then  endof
        afterimage of  64 timer @ - frame-blend   \ (each frame blended with the last, rising)
              -1 timer +!  timer @ 0= if  track bgm  60 timer !  music-in step !  then  endof
        music-in of  -1 timer +!  timer @ 0= if
                  kind @ own-music-death = if  $39 5 bank-sound  then
                  tints-start  60 timer !  tinting step !
              then  endof
        tinting of  tints-step  endof
        over-movie of  movie-status 1 = if  drift-start step !  then  endof
        drift-start of  start-drift  endof
        drifting of  drift  endof
        to-black of  timer @ 30 < if  1 timer +!  then
               timer @ 30 = if  world-held state-flag-set  movie-close  restart step !  then  endof
        restart of  over-done step !  held @ if  movie-unhold  0 held !  then  soft-reset  endof   \ (to the title: a fresh game)
    endcase ;
\ a death in a movie (GameOver_StateMovie): no freeze, no wipe - its music at once, the movie
\ stopped, its last picture kept over everything; the tints, then on as the others
6 constant movie-death-track
: run-movie ( -- )
    step @ case
        music-out of  movie-death-track bgm  0 0 30 music-id send music-op  movie-close  60 timer !  afterimage step !  endof
        afterimage of  -1 timer +!  timer @ 0= if  tints-start  60 timer !  tinting step !  then  endof
        >r run r>
    endcase ;

\ ---- drawing: the movie (class 6: its strip across the middle - 384 x 96 at (64, 176) of the
\ PS2's 512 x 448), the black ----
: draw ( -- )
    movie-layer ui-clear  black-layer ui-clear
    step @ not-running =  step @ over-done = or if  exit  then
    screen-size middle-4:3  {: w h sx sy sw sh :}
    held @ if   \ (a movie death: its last picture over everything, the game over's movie over that)
        movie-layer  0 0 w h  $000000FF ui-rect  movie-layer  sx sy sw sh ui-held
    then
    step @ over-movie < if  exit  then
    movie-status 1 = if
        movie-layer  sx sw 64 * 512 / +  sy sh 176 * 448 / +  sw 384 * 512 /  sh 96 * 448 /  ui-movie
    then
    step @ to-black >= if
        black-layer 0 0 w h  timer @ 255 * 30 / 255 min  ui-rect
    then ;

behaviour ending-the-game
  on spawned ( -- )  not-running step !  faded-out kind !  self subscribe tick  self subscribe frame-end ;
  on game-over-kind ( kind -- )  kind ! ;
  on tick ( -- )
      step @ not-running = if
          caught state-flag? if
              caught state-flag-clear  0 held !
              hewie-played? if  faded-out kind !  then   \ (the run: its own - GameOver_StateSpecial, faded out)
              kind @ story-end = if  over-done step !  broadcast the-ending  exit  then
              kind @ movie-death = if  movie-status 0<> if  movie-hold held !  then  then   \ (GameOver_StateStart: its last picture)
              music-out step !
          then
          exit
      then
      kind @ movie-death = if  run-movie  else  run  then ;
  on frame-end ( -- )  draw ;
end-behaviour

: gameover-spawn ( -- id )  ending-the-game gameover-state s" gameover" spawn ;
