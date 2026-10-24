\ window.fs - messages on screen (docs/subsystems/window.md): the message window (the original's
\ text Task, src/game/text.c) and the action prompt (SceneGame_ActionPrompt). A message is laid
\ out by C (game/messages.c: pages of lines, its options, each character's colour and the frame
\ it is typed on); the window types a page out (Task_StateType: holding cancel - the triangle -
\ types up to the next pause), then any face button, a shoulder or Select turns it; on the last
\ page of a choice the arrows pick an option and the cross answers it - the window then shows the
\ message the option leads to, or closes; a message may close itself some frames after it is
\ typed (0x10). Closing, it tells whoever showed it, with the option chosen. Drawn on the
\ retained UI layer.
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
  cell field place         \ the box's place when the message gives none (0x0A n)
  cell field prompt-kind    \ the action prompt (0: none)
  cell field prompt-age     \ frames since it was last offered
  cell field blink          \ frames drawn (the page mark blinks)
  cell field typed          \ frames the page has been typing (game/messages.c: each glyph's frame)
  \ the panel, measured each frame for the page shown (window pixels)
  cell field scale  cell field line-height  cell field pad
  cell field box-x  cell field box-y  cell field box-w  cell field box-h
  cell field text-w         \ the page's widest line
end-state

: open? ( -- flag )  shown @ none <> ;
: last-page? ( -- flag )  page @ pages @ 1- >= ;
: choosing? ( -- flag )  last-page? message-options 0> and ;
: last-option ( -- i )  message-options 1- 0 max ;
: place-now ( -- n )  page @ message-place  dup 0< if  drop place @  then ;
: lay-out ( msg -- )
    dup shown !  message-layout pages !  0 page !  0 typed !
    message-choice-flags starts-on-last and if  last-option  else  0  then  picked ! ;

\ ---- drawing: a panel sized to the page's lines about the place the message gives it, the words
\ scaled with the screen's height (the PS2's glyph 16 x 21 of 448 lines: a pen of height / 224) ----
$0A0A12D8 constant panel-colour   $8C7A5AC0 constant edge-colour
$F0EBE0FF constant words-colour   $C8B48CFF constant mark-colour
\ the places (0x0A n, D_0044B010): the text's centre down the PS2's 448 lines - low (the default),
\ the middle (the notes on the walls, the tutorials), a little higher (the memory card's); n's
\ bit 7: no box, the words alone
create place-ys  370 , 224 , 330 , 224 , 224 , 224 , 224 , 224 ,
$80 constant no-box   6 constant box-drop   \ (Task_BoxSize: the box 6 lines below the text's centre)
: place-y ( n -- y )  $7F and 7 min cells place-ys + @ ;
: boxed? ( -- flag )  place-now no-box and 0= ;
: line-width ( line -- px )  page @ swap message-line  scale @ text-width ;
: widest ( lines -- px )  0 swap 0 ?do  i line-width max  loop ;
: measure ( lines -- )   \ (a page taller than the screen - none in the game - drawn smaller)
    {: lines | w h :}  screen-size to h to w
    h 9 * 10 /  lines 3 max 1+ 12 * /  h 224 / min  1 max  scale !
    scale @ 12 * line-height !  scale @ 6 * pad !
    lines widest text-w !
    w 84 * 100 /  h 16 * 10 / min  box-w !   \ (at most 16:10 of the height across: wide screens keep it readable)
    w box-w @ - 2/ box-x !
    lines 3 max line-height @ *  pad @ 2* +  box-h !
    place-now place-y box-drop + h * 448 /  box-h @ 2/ -   \ (about the place's centre, kept on the screen)
    h 20 /  h box-h @ - h 20 / -  clamp  box-y ! ;
: rect ( x y w h rgba -- )  {: x y w h rgba :}  text-layer x y w h rgba ui-rect ;
: words ( addr len x y rgba -- )  {: a n x y rgba :}  text-layer a n x y rgba scale @ ui-text ;
: frame-edges ( -- )   \ a thin edge all round
    {: | x y w h :}  box-x @ to x  box-y @ to y  box-w @ to w  box-h @ to h
    x y          w 1  edge-colour rect
    x y h + 1-   w 1  edge-colour rect
    x y          1 h  edge-colour rect
    x w + 1- y   1 h  edge-colour rect ;
: panel ( -- )  box-x @ box-y @ box-w @ box-h @ panel-colour rect  frame-edges ;
: line-x ( -- x )   \ the text's left: in the panel; no box, the text centred (Task_DrawPage)
    boxed? if  box-x @ pad @ 2* +  else  screen-size drop text-w @ - 2/  then ;
: line-start ( line -- x )   \ a line's: 0x12 centred in the text's width, 0x14 to its right
    {: ln | spare :}  text-w @ ln line-width - to spare
    page @ ln message-align  case  1 of  spare 2/  endof  2 of  spare  endof  0 swap  endcase
    line-x + ;
: line-y ( n -- y )  line-height @ *  box-y @ pad @ + + ;
: more-mark ( -- )   \ a small arrow down at the bottom right, blinking: another page follows
    blink @ 32 and if  exit  then
    {: | x y :}  box-x @ box-w @ + pad @ 3 * -  to x   box-y @ box-h @ + pad @ - scale @ 3 * -  to y
    scale @ 3 * 0 do   \ (a row each, narrowing: a triangle)
        x i +  y i +  scale @ 6 * i 2* - 1 max  1  mark-colour rect
    loop ;
: indent ( line col -- px )  {: line col :}  page @ line message-line drop  col scale @ text-width ;
: page-typed? ( -- flag )  typed @  page @ message-typed  >= ;
: palette-words ( addr len x y palette -- )   \ (the message's colour: the font's palette n)
    {: a n x y c :}  text-layer a n x y  c if  $FFFFFFFF  else  words-colour  then  scale @ c ui-text-in ;
: typed-line ( line -- )   \ what of it is typed so far, run by run of a colour
    {: ln | col :}
    begin  page @ ln col typed @ message-run  {: a n c :}  n 0>  while
        a n  ln line-start ln col indent +  ln line-y  c palette-words  n col + to col
    repeat ;
$60 constant cursor-glyph   9 constant cursor-palette   \ (the font's pointing hand, in its palette 9)
create cursor-text  cursor-glyph c,
: cursor ( -- )   \ the hand before the option picked: 20 of the PS2's pixels left of it (Task_DrawPage)
    picked @ message-option-at  {: pg line col :}
    cursor-text 1  line line-start line col indent +  scale @ 10 * -  line line-y  cursor-palette palette-words ;
: draw ( -- )
    text-layer ui-clear
    open? 0= if  exit  then
    shown @ message-layout drop   \ (the C layout is one at a time: lay it out again to read it)
    page @ message-lines measure  boxed? if  panel  then
    page @ message-lines 0 ?do  i typed-line  loop
    page-typed? 0= if  exit  then
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
\ (Task_StateChoice: the message an option leads to opens where this one was, boxed)
: answer ( -- )   \ the option's message follows (from the same table), or the window closes
    picked @ chosen !  place-now no-box invert and place !
    picked @ message-option-leads  dup no-message = if  drop close exit  then
    shown @ message-table-bits and or lay-out ;
\ ---- choosing (Task_StateChoice): the cross answers; the triangle, when the choice allows it,
\ goes to the last option; up / down round the options, left / right round those on the same
\ line (one alone there: as down / up). The menu's sounds ----
: se ( id -- )  5 bank-sound ;
$2A constant cursor-sound   $2B constant decide-sound   $2C constant cancel-sound
: may-cancel? ( -- flag )  message-choice-flags 1 and 0= ;   \ (the flags' bit 0 clear)
: option-line ( i -- line )  message-option-at drop nip ;
: options ( -- n )  message-options ;
: round ( i -- i' )  options + options mod ;
: row-count ( line -- n )  0  options 0 ?do  over i option-line = if  1+  then  loop  nip ;
: along-row ( d -- )   \ to the next / last option on the picked one's line, round
    {: d | ln :}  picked @ option-line to ln
    ln row-count 2 < if  picked @ d + round picked !  exit  then
    picked @  begin  d + round  dup option-line ln =  until  picked ! ;
: choice-keys ( -- )
    cross button-pressed? if  answer  decide-sound se  exit  then
    may-cancel? triangle button-pressed? and if
        picked @ last-option <> if  last-option picked !  cancel-sound se  then  exit
    then
    picked @  {: was :}
    key: Right key-pressed? if  1 along-row
    else key: Left key-pressed? if  -1 along-row
    else key: Up key-pressed? if  picked @ 1- round picked !
    else key: Down key-pressed? if  picked @ 1+ round picked !  then then then then
    picked @ was <> if  cursor-sound se  then ;

\ ---- a page turned or the message closed (Task_StateNextPage / WaitButton: Select, L1, R1 and
\ the face buttons - the pad's 0xFC01) ----
: page-button? ( -- flag )
    false  select-button 1+ 0 do  i start-button <> if  i button-pressed? or  then  loop ;
: next-page ( -- )  1 page +!  0 typed ! ;
: closes-itself ( -- )   \ (0x10 n: n frames after it is typed; 0: left up, no button closes it)
    message-close-after ?dup if  typed @  page @ message-typed -  <= if  close  then  then ;
: keys ( -- )
    page-typed? 0= if   \ (typing: holding cancel types up to the next pause)
        triangle button-pressed? if  page @ typed @ message-skip typed !  then  exit
    then
    last-page? message-close-after 0< 0= and if  closes-itself exit  then
    choosing? if  choice-keys exit  then
    page-button? if  last-page? if  close  else  next-page  then  then ;

behaviour showing
  on spawned ( -- )  none shown !  self subscribe tick  self subscribe frame-end ;
  on show-text ( msg -- )
      open? 0= if  -1 broadcast text-shown  then
      sender asked-by !  none chosen !  0 place !  lay-out ;
  on close-text ( msg -- )
      open? 0= if  drop exit  then
      dup no-message = swap shown @ = or if  close  then ;
  on prompt ( kind -- )  prompt-kind !  0 prompt-age ! ;
  on tick ( -- )
      open? if  shown @ message-layout drop  keys  1 typed +!  then
      1 prompt-age +!  prompt-age @ 2 > if  0 prompt-kind !  then ;
  on frame-end ( -- )  1 blink +!  draw  draw-prompt ;
end-behaviour

: window-spawn ( -- id )  showing window-state s" window" spawn ;
