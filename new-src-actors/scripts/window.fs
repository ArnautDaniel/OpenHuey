\ window.fs - messages on screen (docs/subsystems/window.md): the message window (the original's
\ text Task, src/game/text.c) and the action prompt (SceneGame_ActionPrompt). A message is laid
\ out by C (game/messages.c: pages of lines, its options); the window shows a page at a time,
\ the action button turns the page; on the last page of a choice the arrows pick an option and
\ the button answers it - the window then shows the message the option leads to, or closes.
\ Closing, it tells whoever showed it, with the option chosen. Drawn on the retained UI layer.
IN: window
USING: engine actors messages keys ;

1 constant text-layer      \ (the UI layers it draws on)
2 constant prompt-layer

state: window-state
  cell field shown          \ the message shown (-1: none)
  cell field asked-by       \ who showed it
  cell field pages  cell field page  cell field picked  cell field chosen
  cell field prompt-kind    \ the action prompt (0: none)
  cell field prompt-age     \ frames since it was last offered
end-state

: open? ( -- flag )  shown @ 0< 0= ;
: last-page? ( -- flag )  page @ pages @ 1- >= ;
: choosing? ( -- flag )  last-page? message-options 0> and ;
: lay-out ( msg -- )
    dup shown !  message-layout pages !  0 page !
    message-choice-flags 2 and if  message-options 1- 0 max  else  0  then  picked ! ;
\ ---- drawing (pixels from the screen's size: the box over the lower part of the picture) ----
variable sw  variable sh  variable lh
: at-line ( n -- x y )  sw @ 10 / 24 +  swap lh @ *  sh @ 3 * 4 / 12 + + ;
: draw ( -- )
    text-layer ui-clear
    open? 0= if  exit  then
    shown @ message-layout drop   \ (the C layout is one at a time: lay it out again to read it)
    screen-size sh ! sw !  2 pen-scale  char-size nip 5 * 4 / lh !
    text-layer  sw @ 10 /  sh @ 3 * 4 /  sw @ 8 * 10 /  sh @ 5 /  $101018C0 ui-rect
    page @ message-lines 0 ?do  text-layer  page @ i message-line  i at-line  $FFFFFFFF 2 ui-text  loop
    choosing? if
        text-layer  s" >"  picked @ message-option drop  char-size drop * >r  nip at-line swap r> + swap  $FFFFFFFF 2 ui-text
    then
    last-page? 0= if  text-layer  s" (Space)"  sw @ 8 * 10 /  sh @ 9 * 10 /  $C0C0C0FF 2 ui-text  then ;
: draw-prompt ( -- )
    prompt-layer ui-clear
    prompt-kind @ 0= open? or if  exit  then
    screen-size sh ! sw !
    prompt-layer  s" Space: look"  sw @ 2/ 60 -  sh @ 9 * 10 /  $FFFFFFFF 2 ui-text ;

: close ( -- )
    shown @  chosen @  asked-by @ send text-closed
    -1 shown !  0 broadcast text-shown ;
: answer ( -- )
    picked @ chosen !
    picked @ message-option >r drop 2drop r>  dup $FFFF = if  drop close exit  then
    shown @ $C000 and or lay-out ;   \ (the option's message, from the same table)
: keys ( -- )
    choosing? if
        key: Up key-pressed?  key: Left key-pressed? or if  picked @ 1- 0 max picked !  then
        key: Down key-pressed?  key: Right key-pressed? or if  picked @ 1+ message-options 1- min picked !  then
    then
    circle button-pressed? if
        choosing? if  answer exit  then
        last-page? if  close  else  1 page +!  then
    then ;

behaviour showing
  on spawned ( -- )  -1 shown !  self subscribe tick  self subscribe frame-end ;
  on show-text ( msg -- )
      open? 0= if  -1 broadcast text-shown  then
      sender asked-by !  -1 chosen !  lay-out ;
  on close-text ( msg -- )
      open? 0= if  drop exit  then
      dup $FFFF = swap shown @ = or if  close  then ;
  on prompt ( kind -- )  prompt-kind !  0 prompt-age ! ;
  on tick ( -- )
      open? if  shown @ message-layout drop  keys  then
      1 prompt-age +!  prompt-age @ 2 > if  0 prompt-kind !  then ;
  on frame-end ( -- )  draw  draw-prompt ;
end-behaviour

: window-spawn ( -- id )  showing window-state s" window" spawn ;
