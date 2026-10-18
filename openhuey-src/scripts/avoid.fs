\ avoid.fs - the avoid prompt: the button to press to dodge or struggle free, shown by the story
\ (its 0xB4) for a second at a time - the scripts show it again each frame they want it up
\ (src/game/scene_game.c AvoidPrompt_*). One row of SYSTEM/AVOID.TEX, 160 x 32 each, drawn
\ 240 x 48 in the middle of the screen.
IN: avoid
USING: engine actors messages ps2-screen ;

12 constant avoid-layer   \ (the UI layers: ... 10, 11 the sub screen; 12 this)
$2D constant avoid-group  \ (the texture cache's group, as the original's)
30 constant showing-for   \ frames

state: avoid-state
  cell field row       \ the row shown (-1: none)
  cell field left      \ frames left to show it
  cell field loaded    \ the texture is in
end-state

: load-art ( -- )  s" SYSTEM/AVOID.TEX" avoid-group tex-bank loaded ! ;
: draw ( -- )
    frame!  avoid-layer ui-clear
    row @ 0< loaded @ 0= or if  exit  then
    avoid-layer  $88 sx $C8 sy  240 sw 48 sy  avoid-group 0 -1  0 row @ 32 *  160 32  $FFFFFFFF  ui-image ;

behaviour prompting
  on spawned ( -- )  -1 row !  load-art
      self subscribe tick  self subscribe frame-end  self subscribe arrived ;
  on show-avoid ( row -- )  row !  showing-for left ! ;
  on tick ( -- )  row @ 0< if  exit  then  -1 left +!  left @ 0= if  -1 row !  then ;
  on arrived ( room exit -- )  2drop  -1 row ! ;
  on frame-end ( -- )  draw ;
end-behaviour

: avoid-spawn ( -- id )  prompting avoid-state s" avoid" spawn ;
