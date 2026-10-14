\ window.fs - messages on screen (docs/subsystems/window.md): the message window (the original's
\ text Task, src/game/text.c) and the action prompt (SceneGame_ActionPrompt). A message is laid
\ out by C (game/messages.c: pages of lines, its options); the window shows a page at a time,
\ the action button turns the page; on the last page of a choice the arrows pick an option and
\ the button answers it - the window then shows the message the option leads to, or closes.
\ Closing, it tells whoever showed it, with the option chosen. Drawn on the retained UI layer.
IN: window
USING: engine actors messages keys ;

2 constant text-layer      \ (the UI layers: 0 the movie, 1 the fade, 2 the window, 3 the prompt)
3 constant prompt-layer

state: window-state
  cell field shown          \ the message shown (-1: none)
  cell field asked-by       \ who showed it
  cell field pages  cell field page  cell field picked  cell field chosen
  cell field prompt-kind    \ the action prompt (0: none)
  cell field prompt-age     \ frames since it was last offered
  cell field blink          \ frames drawn (the page mark blinks)
end-state

: open? ( -- flag )  shown @ 0< 0= ;
: last-page? ( -- flag )  page @ pages @ 1- >= ;
: choosing? ( -- flag )  last-page? message-options 0> and ;
: lay-out ( msg -- )
    dup shown !  message-layout pages !  0 page !
    message-choice-flags 2 and if  message-options 1- 0 max  else  0  then  picked ! ;
\ ---- drawing: a panel low in the picture, sized to the page's lines, the words scaled with the
\ screen's height (the PS2's glyph 16 x 21 of 448 lines: a pen of height / 224) ----
variable sw  variable sh  variable scale  variable lh  variable pad
variable box-x  variable box-y  variable box-w  variable box-h
$0A0A12D8 constant panel-colour   $8C7A5AC0 constant edge-colour
$F0EBE0FF constant words-colour   $C8B48CFF constant mark-colour
: measure ( lines -- )   \ (a long page - the tutorials' - drawn smaller, to stay in the lower 3/5)
    screen-size sh ! sw !
    dup 4 max 12 *  sh @ 3 * 5 / swap /  sh @ 224 / min  1 max scale !
    scale @ 12 * lh !  scale @ 6 * pad !
    sw @ 84 * 100 /  sh @ 16 * 10 / min  box-w !   \ (at most 16:10 of the height across: wide screens keep it readable)
    sw @ box-w @ - 2/ box-x !
    3 max lh @ *  pad @ 2* +  box-h !
    sh @  box-h @ -  sh @ 20 / -  box-y ! ;
: rect ( x y w h rgba -- )  >r >r >r >r >r  text-layer r> r> r> r> r> ui-rect ;
: frame-edges ( -- )   \ a thin edge all round
    box-x @ box-y @  box-w @ 1  edge-colour rect
    box-x @ box-y @ box-h @ + 1-  box-w @ 1  edge-colour rect
    box-x @ box-y @  1 box-h @  edge-colour rect
    box-x @ box-w @ + 1- box-y @  1 box-h @  edge-colour rect ;
: panel ( -- )  box-x @ box-y @ box-w @ box-h @ panel-colour rect  frame-edges ;
: at-line ( n -- x y )  box-x @ pad @ 2* +  swap lh @ *  box-y @ pad @ + + ;
: words ( addr len x y rgba -- )  >r >r >r >r >r  text-layer r> r> r> r> r> scale @ ui-text ;
: more-mark ( -- )   \ a small arrow down at the bottom right, blinking: another page follows
    blink @ 32 and if  exit  then
    scale @ 3 * 0 do
        box-x @ box-w @ + pad @ 3 * - i +   box-y @ box-h @ + pad @ - scale @ 3 * - i +
        scale @ 6 * i 2* - 1 max  1  mark-colour rect
    loop ;
: cursor ( -- )   \ the arrow before the option picked
    picked @ message-option drop  >r nip  ( line ) r>   \ ( line col )
    over page @ swap message-line drop swap scale @ text-width >r
    at-line swap r> + scale @ 6 * - swap  s" >" 2swap mark-colour words ;
: draw ( -- )
    text-layer ui-clear
    open? 0= if  exit  then
    shown @ message-layout drop   \ (the C layout is one at a time: lay it out again to read it)
    page @ message-lines measure  panel
    page @ message-lines 0 ?do  page @ i message-line  i at-line  words-colour words  loop
    choosing? if  cursor  then
    last-page? 0= if  more-mark  then ;
$C0 constant circle-glyph   \ (the font's pad buttons from $C0: circle, triangle, square, cross, L1 ...)
create look-text 8 allot
: look-words ( -- addr len )   \ "(circle) look"
    circle-glyph look-text c!  s"  look" look-text 1+ swap move  look-text 6 ;
: draw-prompt ( -- )
    prompt-layer ui-clear
    prompt-kind @ 0= open? or if  exit  then
    screen-size sh ! sw !  sh @ 224 / 1 max scale !
    look-words 2dup scale @ text-width  sw @ swap - 2/  sh @ 9 * 10 /
    >r >r >r >r  prompt-layer r> r> r> r>  $FFFFFFFF scale @ ui-text ;

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
    cross button-pressed? if   \ (the menus' confirm: cross - Pads_FaceButtons' menu bit 4)
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
  on frame-end ( -- )  1 blink +!  draw  draw-prompt ;
end-behaviour

: window-spawn ( -- id )  showing window-state s" window" spawn ;
