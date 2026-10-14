\ subscreen/art.fs - drawing the sub screen: the original's art (SUBSCR/SUBBASE's parts, the
\ background's panels: SUBBACK, PLATE or SYN_SLOT) and its words, on the PS2's 512 x 448 screen
\ fitted to a 4:3 frame of the window, faded in with the menu (src/game/subscreen.c
\ SubScreen_DrawPanel, SubScreen_DrawPart, Task_ShowText, Task_DrawBox).
IN: subscreen.art
USING: engine game-state actors messages ps2-screen subscreen.state subscreen.tables ;

10 constant art-layer   11 constant word-layer   \ (the UI layers: ... 9 the game over's; 10, 11 this)
\ the texture cache's groups (as the original's): the parts, the background, an item's picture
$18 constant base-group   $19 constant back-group   $27 constant picture-group

: faded ( a -- rgba )  openness @ * 128 / 2* 255 min  $FFFFFF00 or ;   \ white, at the PS2's alpha (128 opaque)

\ ---- a picture: part of a cache texture, drawn at x y (screen units) ----
variable at-x  variable at-y  variable size-w  variable size-h
: picture ( x y w h -- )  size-h !  size-w !  at-y !  at-x ! ;
: from ( group tex palette u v uw vh alpha -- )   \ (after `picture`: where it comes from)
    {: group tex palette u v uw vh alpha :}
    art-layer  at-x @ sx  at-y @ sy  size-w @ sw  size-h @ sy
    group tex palette  u v uw vh  alpha faded  ui-image ;

\ a panel (a background piece) and a part (a piece of the menu's own art), by number
variable piece
: column ( k -- n )  cells piece @ + @ ;
: panel ( i alpha -- )   \ panels: texture, u, v, w, h, x, y
    {: i alpha :}  i /panel * panels + piece !
    5 column 6 column 3 column 4 column picture
    back-group 0 column -1  1 column 2 column 3 column 4 column  alpha from ;
: part ( x y i alpha -- )   \ parts: u, v, w, h, the size drawn, palette, texture, blending
    {: x y i alpha :}  i /part * parts + piece !
    x y 4 column 5 column picture
    base-group 7 column 6 column  0 column 1 column 2 column 3 column  alpha from ;
: page-art ( kind -- )   \ the background panels of a kind of page (kSubScreenFadePanels)
    4 * cells page-panels +  4 0 do  dup i cells + @ dup 0< if  drop leave  then  $80 panel  loop  drop ;
: shade ( cx cy w h -- )   \ a dark box centred on cx, cy (Task_DrawBox)
    {: cx cy w h :}
    art-layer  cx w 2/ - sx  cy h 2/ - sy  w sw  h sy  $C0 openness @ * 128 / $10100C00 or  ui-rect ;

\ ---- words ----
: scale ( -- n )  448 sy 300 / 1 max ;
: ink ( light? -- rgba )   \ the gold of a choice, or plain; faded in with the menu
    if  $FFD87800  else  $E8E0D000  then  openness @ 2* 255 min or ;
: text ( addr len x y light? -- )   \ a line at x y
    {: addr len x y light :}  word-layer addr len  x sx  y sy  light ink  scale  ui-text ;
: centred ( addr len x y -- )   \ a line centred on x
    {: addr len x y :}
    word-layer addr len  x sx  len char-size drop * scale * 2/ -  y sy  false ink  scale  ui-text ;
: line-of ( msg page line -- addr len )  rot message-layout drop  message-line ;
: menu-line ( msg -- addr len )  $4000 or 0 0 line-of ;   \ the menu's own messages (the second table)
: name ( id -- addr len )  $8100 + 0 0 line-of ;         \ an item's name
: blink ( -- a )  $80 frames @ 4 * $FF and - abs ;        \ the arrows' alpha

\ ---- a few words of text built at a time ----
create spell 128 allot   variable spelt
: spell-reset ( -- )  0 spelt ! ;
: +spell ( addr len -- )  dup >r  spell spelt @ + swap move  r> spelt +! ;
: spelled ( -- addr len )  spell spelt @ ;
: count$ ( n -- addr len )  spell-reset  s" x" +spell  n>s +spell  spelled ;        \ "x3"
: of$ ( a b -- addr len )  spell-reset  swap n>s +spell  s" /" +spell  n>s +spell  spelled ;   \ "1/2"
: load-bank ( file len group -- )   \ a .TEX file (its name less the extension) into a group
    >r  spell-reset +spell  s" .TEX" +spell  spelled r> tex-bank drop ;
