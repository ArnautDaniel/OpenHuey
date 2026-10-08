\ screen.fs - the screen's fade (docs/subsystems/screen.md; the original's event +0x20, Events_Fade):
\ black over the picture, under the message window. The room's scripts fade it in or out over
\ some frames and wait for it.
IN: screen
USING: engine actors messages ;

1 constant fade-layer   \ (the UI layers: 0 the movie, 1 the fade, 2 the message window, 3 the prompt)

state: screen-state
  cell field level        \ 0 clear .. 1000 black
  cell field from  cell field to  cell field frames  cell field t
  cell field asked-by     \ who waits for it (-1 none)
end-state

: fading? ( -- flag )  t @ frames @ < ;
\ a fade over `frames` frames; kind and 0xF: 0 / 1 in (from black), others out (to black). (Its
\ bits 0xC0 / 0x30 fade the music and the volume with it: with the music.)
: start ( frames kind -- )
    $F and dup 1 = swap 0= or if  1000 0  else  0 1000  then   ( frames from to )
    to !  from !  0 max frames !  0 t !
    frames @ 0= if  to @ level !  then ;
: finish ( -- )   \ it counts as over
    frames @ t !  to @ level !
    asked-by @ dup 0< if  drop exit  then  -1 asked-by !  send fade-done ;
: step ( -- )
    fading? 0= if  exit  then
    1 t +!  to @ from @ - t @ * frames @ / from @ + level !
    fading? 0= if  finish  then ;
: draw ( -- )
    fade-layer ui-clear
    level @ dup 0> 0= if  drop exit  then
    255 * 1000 / 0 max 255 min  >r  fade-layer 0 0 screen-size r> ui-rect ;   \ (black, that much opaque)

behaviour fading
  on spawned ( -- )  -1 asked-by !  self subscribe tick  self subscribe frame-end ;
  on fade ( frames kind -- )  start  sender asked-by !  fading? 0= if  finish  then ;
  on fade-finish ( -- )  finish ;
  on tick ( -- )  step ;
  on frame-end ( -- )  draw ;
end-behaviour

: screen-spawn ( -- id )  fading screen-state s" screen" spawn ;
