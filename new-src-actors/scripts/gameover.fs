\ gameover.fs - the game over (docs/subsystems/gameover.md; src/game/gameover.c, SceneGame's
\ SubTransition): when the game ends (the `caught` flag: Fiona dragged or carried off, held; the
\ story's own) the world stands still behind it - the stage music faded, any movie stopped,
\ the panic held, the actors and the room still - then its music (by the story so far), the
\ room's tint going purple and its fog clear over 60 frames, the GAMEOVER movie, the tint
\ drifting to blue for a minute (a face button cuts it short), black over 30 frames - and a
\ fresh game: back to the title (`soft-reset`, as the original makes its title scene anew).
IN: gameover
USING: engine game-state actors messages keys facts flag-names story-names ;

6 constant movie-layer   9 constant black-layer

state: gameover-state
  cell field kind          \ how it goes (`game-over-kind`; progress +0x73EB00): 1 faded out
  cell field step          \ -1 not running; the sequence's step (0..11), 12 done
  cell field timer
  12 field tint-from  12 field tint-to   \ the tint's two colour words and its sharp byte; to
  16 field fog-from        \ the fog's colours and range
  16 field look-at         \ (scratch: a look effect's parameters)
end-state

: music-id ( -- id )  s" music" actor-named ;

\ ---- its music (GameOver: by the story so far; the kind 3's own) ----
: track ( -- n )
    kind @ 3 = if  $47 exit  then
    over-music-f story-flag? if  $F exit  then
    over-music-e story-flag? if  $E exit  then
    over-music-d story-flag? if  $D exit  then
    over-music-c story-flag? if  $C exit  then
    over-music-b story-flag? if  $B exit  then  $A ;
: bgm ( track -- )  0 1e f>cell music-id send bgm-want ;

\ ---- the room's tint and fog, a step on: `t` of 60 back toward where they were ----
variable mf  variable mg  variable mt  variable mn
: mix ( from to t n -- c )   \ (each byte: to + t * (from - to) / n)
    mn !  mt !  mg !  mf !  0
    4 0 do
        mf @ i 8 * rshift $FF and  mg @ i 8 * rshift $FF and      ( c f g )
        tuck - mt @ * mn @ / +  $FF and  i 8 * lshift or
    loop ;
: tints-start ( -- )   \ (the tint to purple, the fog to clear - from where they are)
    tint-from 12 0 fill  fog-from 16 0 fill
    $1F tint-from look@ drop  $1D fog-from look@ drop
    $AA0000C8 tint-to l!  $FFFFFFFF tint-to 4 + l!  tint-from 8 + c@ tint-to 8 + c! ;
variable tt  variable tn
: tint! ( t n -- )   \ (the tint `t` of `n` back toward tint-from)
    tn !  tt !  tint-from l@ tint-to l@ tt @ tn @ mix  look-at l!
    tint-from 4 + l@ tint-to 4 + l@ tt @ tn @ mix  look-at 4 + l!
    tint-to 8 + c@ look-at 8 + c!  $1F look-at 9 look-set ;
: fog! ( t -- )   \ (the fog `t` of 60 back toward fog-from; its range kept)
    fog-from look-at 16 move
    fog-from l@ 0 2 pick 60 mix look-at l!  fog-from 4 + l@ 0 rot 60 mix look-at 4 + l!
    $1D look-at 16 look-set ;
: tints-step ( -- )
    timer @ 60 tint!  timer @ fog!
    timer @ 0= if
        s" SYSTEM/GAMEOVER.SFD" movie-open if  $41 $10 $20 movie-compose  7 step !   \ (class 6: three-level)
        else  8 step !  then
    else  -1 timer +!  then ;
: drift-start ( -- )   \ (the movie over: the tint to blue over a minute)
    movie-status 1 = if  exit  then
    1800 timer !  tint-from 12 0 fill  $1F tint-from look@ drop
    $82000082 tint-to l!  $FFFFFFFF tint-to 4 + l!  tint-from 8 + c@ tint-to 8 + c!  9 step ! ;
: skip? ( -- flag )   \ (a face button)
    circle button-pressed? cross button-pressed? or  square button-pressed? or  triangle button-pressed? or ;
: drift ( -- )
    timer @ 1800 tint!
    -1 timer +!  timer @ 0=  skip? or if  $FF bgm  0 timer !  10 step !  then ;

\ ---- the sequence (GameOver_StateOthers / StateMode3: game_over) ----
: freeze ( -- )   \ (the panic held, the actors and the room still; the world still drawn)
    panic-held state-flag-set  1 still! ;
: run ( -- )
    step @ case
        0 of  0 0 30 music-id send music-op  movie-close  1 step !  endof
        1 of  freeze  2 step !  endof
        2 of  movie-status 1 <> if  64 timer !  3 step !  then  endof
        3 of  -1 timer +!  timer @ 0= if  track bgm  60 timer !  4 step !  then  endof
        4 of  -1 timer +!  timer @ 0= if
                  kind @ 3 = if  $39 5 bank-sound  then
                  tints-start  60 timer !  5 step !
              then  endof
        5 of  tints-step  endof
        7 of  movie-status 1 = if  8 step !  then  endof
        8 of  drift-start  endof
        9 of  drift  endof
        10 of  timer @ 30 < if  1 timer +!  then
               timer @ 30 = if  world-held state-flag-set  movie-close  11 step !  then  endof
        11 of  12 step !  soft-reset  endof   \ (to the title: a fresh game)
    endcase ;

\ ---- drawing: the movie (class 6: its strip across the middle), the black ----
variable sx  variable sy  variable sw  variable sh
: draw ( -- )
    movie-layer ui-clear  black-layer ui-clear
    step @ 7 < if  exit  then
    movie-status 1 = if
        screen-size  over 3 * 4 / over min  dup sh !  4 * 3 / sw !  sh @ - 2/ sy !  sw @ - 2/ sx !
        movie-layer  sx @ sw @ 64 * 512 / +  sy @ sh @ 176 * 448 / +  sw @ 384 * 512 /  sh @ 96 * 448 /  ui-movie
    then
    step @ 10 >= if
        black-layer 0 0 screen-size  timer @ 255 * 30 / 255 min  ui-rect
    then ;

behaviour ending-the-game
  on spawned ( -- )  -1 step !  1 kind !  self subscribe tick  self subscribe frame-end ;
  on game-over-kind ( kind -- )  kind ! ;
  on tick ( -- )
      step @ 0< if
          caught state-flag? if  caught state-flag-clear  kind @ 2 = if  12 step !  soft-reset  else  0 step !  then  then
          exit
      then
      run ;
  on frame-end ( -- )  draw ;
end-behaviour

: gameover-spawn ( -- id )  ending-the-game gameover-state s" gameover" spawn ;
