\ screen.fs - the screen's fade (docs/subsystems/screen.md; the original's event +0x20, Events_Fade):
\ black over the picture, under the message window. The room's scripts fade it in or out over
\ some frames and wait for it.
IN: screen
USING: engine actors messages ;

1 constant fade-layer   \ (the UI layers: 0 the movie, 1 the fade, 2 the message window, 3 the prompt)

state: screen-state
  cell field level        \ no-fade .. full-fade
  cell field from  cell field to  cell field frames  cell field t
  cell field asked-by     \ who waits for it (-1 none)
  cell field white        \ in white (kinds 2 and 3), not black
end-state

: fading? ( -- flag )  t @ frames @ < ;
\ a fade over `frames` frames, by kind (ScreenFade_Step; the kind's bits $C0 / $30 fade the music
\ and the volume with it: with the music). The even kinds go out, the odd come in.
enum: fade-kind  to-black from-black to-white from-white to-black-2 from-black-2 to-dim from-dim ;
0 constant no-fade   500 constant half-fade   1000 constant full-fade   \ (its level)
begin-structure /fade  field: fade-from  field: fade-to  end-structure
create fades
    no-fade , full-fade ,   full-fade , no-fade ,   no-fade , full-fade ,   full-fade , no-fade ,
    no-fade , full-fade ,   full-fade , no-fade ,   no-fade , half-fade ,   half-fade , no-fade ,
: fade-of ( kind -- addr )  /fade * fades + ;
: white-kind? ( kind -- flag )  dup to-white = swap from-white = or ;
: start ( frames kind -- )
    $F and  dup from-dim > if  drop to-black-2  then        \ (8..15: no such kind - as out)
    dup white-kind? white !
    fade-of  dup fade-from @ from !  fade-to @ to !
    0 max frames !  0 t !
    frames @ 0= if  to @ level !  then ;
: finish ( -- )   \ it counts as over
    frames @ t !  to @ level !
    asked-by @ dup 0< if  drop exit  then  -1 asked-by !
    dup alive? if  send fade-done  else  drop  then ;   \ (who asked may be gone - the story stopped)
: step ( -- )
    fading? 0= if  exit  then
    1 t +!  to @ from @ - t @ * frames @ / from @ + level !
    fading? 0= if  finish  then ;
: opacity ( -- 0..255 )  level @ 255 * full-fade / 0 max 255 min ;
: draw ( -- )   \ black or white, that much opaque
    fade-layer ui-clear
    level @ 0> 0= if  exit  then
    opacity  white @ if  $FFFFFF00 or  then  {: rgba :}
    fade-layer 0 0 screen-size rgba ui-rect ;

behaviour fading
  on spawned ( -- )  -1 asked-by !  self subscribe tick  self subscribe frame-end ;
  on fade ( frames kind -- )  start  sender asked-by !  fading? 0= if  finish  then ;
  on fade-finish ( -- )  finish ;
  on tick ( -- )  step ;
  on frame-end ( -- )  draw ;
end-behaviour

: screen-spawn ( -- id )  fading screen-state s" screen" spawn ;
