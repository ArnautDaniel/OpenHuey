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

$FFFF constant no-message   \ (an option leading nowhere; to close-text: whatever is shown)
$C000 constant message-table-bits   \ (bit 15 a system message, bit 14 the second table)
2 constant starts-on-last   \ (a choice's flag: the cursor starts on the last option - "no")
-1 constant none

state: window-state
  cell field shown          \ the message shown (none)
  cell field asked-by       \ who showed it
  cell field pages  cell field page
  cell field picked         \ the option under the cursor
  cell field chosen         \ the option answered (none: no choice made)
  cell field prompt-kind    \ the action prompt (0: none)
  cell field prompt-age     \ frames since it was last offered
  cell field blink          \ frames drawn (the page mark blinks)
  \ the panel, measured each frame for the page shown (window pixels)
  cell field scale  cell field line-height  cell field pad
  cell field box-x  cell field box-y  cell field box-w  cell field box-h
end-state

: open? ( -- flag )  shown @ none <> ;
: last-page? ( -- flag )  page @ pages @ 1- >= ;
: choosing? ( -- flag )  last-page? message-options 0> and ;
: last-option ( -- i )  message-options 1- 0 max ;
: lay-out ( msg -- )
    dup shown !  message-layout pages !  0 page !
    message-choice-flags starts-on-last and if  last-option  else  0  then  picked ! ;

\ ---- drawing: a panel low in the picture, sized to the page's lines, the words scaled with the
\ screen's height (the PS2's glyph 16 x 21 of 448 lines: a pen of height / 224) ----
$0A0A12D8 constant panel-colour   $8C7A5AC0 constant edge-colour
$F0EBE0FF constant words-colour   $C8B48CFF constant mark-colour
: measure ( lines -- )   \ (a long page - the tutorials' - drawn smaller, to stay in the lower 3/5)
    {: lines | w h :}  screen-size to h to w
    h 3 * 5 /  lines 4 max 12 * /  h 224 / min  1 max  scale !
    scale @ 12 * line-height !  scale @ 6 * pad !
    w 84 * 100 /  h 16 * 10 / min  box-w !   \ (at most 16:10 of the height across: wide screens keep it readable)
    w box-w @ - 2/ box-x !
    lines 3 max line-height @ *  pad @ 2* +  box-h !
    h  box-h @ -  h 20 / -  box-y ! ;
: rect ( x y w h rgba -- )  {: x y w h rgba :}  text-layer x y w h rgba ui-rect ;
: words ( addr len x y rgba -- )  {: a n x y rgba :}  text-layer a n x y rgba scale @ ui-text ;
: frame-edges ( -- )   \ a thin edge all round
    {: | x y w h :}  box-x @ to x  box-y @ to y  box-w @ to w  box-h @ to h
    x y          w 1  edge-colour rect
    x y h + 1-   w 1  edge-colour rect
    x y          1 h  edge-colour rect
    x w + 1- y   1 h  edge-colour rect ;
: panel ( -- )  box-x @ box-y @ box-w @ box-h @ panel-colour rect  frame-edges ;
: line-x ( -- x )  box-x @ pad @ 2* + ;
: line-y ( n -- y )  line-height @ *  box-y @ pad @ + + ;
: more-mark ( -- )   \ a small arrow down at the bottom right, blinking: another page follows
    blink @ 32 and if  exit  then
    {: | x y :}  box-x @ box-w @ + pad @ 3 * -  to x   box-y @ box-h @ + pad @ - scale @ 3 * -  to y
    scale @ 3 * 0 do   \ (a row each, narrowing: a triangle)
        x i +  y i +  scale @ 6 * i 2* - 1 max  1  mark-colour rect
    loop ;
: indent ( line col -- px )  {: line col :}  page @ line message-line drop  col scale @ text-width ;
: cursor ( -- )   \ the arrow before the option picked
    picked @ message-option-at  {: pg line col :}
    s" >"  line-x line col indent +  scale @ 6 * -  line line-y  mark-colour words ;
: draw ( -- )
    text-layer ui-clear
    open? 0= if  exit  then
    shown @ message-layout drop   \ (the C layout is one at a time: lay it out again to read it)
    page @ message-lines measure  panel
    page @ message-lines 0 ?do  page @ i message-line  line-x i line-y  words-colour words  loop
    choosing? if  cursor  then
    last-page? 0= if  more-mark  then ;

$C0 constant circle-glyph   \ (the font's pad buttons from $C0: circle, triangle, square, cross, L1 ...)
create look-text 8 allot
: look-words ( -- addr len )   \ "(circle) look"
    circle-glyph look-text c!  s"  look" look-text 1+ swap move  look-text 6 ;
: draw-prompt ( -- )
    prompt-layer ui-clear
    prompt-kind @ 0= open? or if  exit  then
    look-words  screen-size  {: a n w h | pen :}  h 224 / 1 max to pen
    prompt-layer a n  w a n pen text-width - 2/  h 9 * 10 /  $FFFFFFFF pen ui-text ;

\ ---- answering ----
: close ( -- )
    shown @  chosen @  asked-by @ send text-closed
    none shown !  0 broadcast text-shown ;
: answer ( -- )   \ the option's message follows (from the same table), or the window closes
    picked @ chosen !
    picked @ message-option-leads  dup no-message = if  drop close exit  then
    shown @ message-table-bits and or lay-out ;
: pick-up ( -- )    picked @ 1- 0 max picked ! ;
: pick-down ( -- )  picked @ 1+ last-option min picked ! ;
: keys ( -- )
    choosing? if
        key: Up key-pressed?  key: Left key-pressed? or if  pick-up  then
        key: Down key-pressed?  key: Right key-pressed? or if  pick-down  then
    then
    cross button-pressed? if   \ (the menus' confirm: cross - Pads_FaceButtons' menu bit 4)
        choosing? if  answer exit  then
        last-page? if  close  else  1 page +!  then
    then ;

behaviour showing
  on spawned ( -- )  none shown !  self subscribe tick  self subscribe frame-end ;
  on show-text ( msg -- )
      open? 0= if  -1 broadcast text-shown  then
      sender asked-by !  none chosen !  lay-out ;
  on close-text ( msg -- )
      open? 0= if  drop exit  then
      dup no-message = swap shown @ = or if  close  then ;
  on prompt ( kind -- )  prompt-kind !  0 prompt-age ! ;
  on tick ( -- )
      open? if  shown @ message-layout drop  keys  then
      1 prompt-age +!  prompt-age @ 2 > if  0 prompt-kind !  then ;
  on frame-end ( -- )  1 blink +!  draw  draw-prompt ;
end-behaviour

: window-spawn ( -- id )  showing window-state s" window" spawn ;
