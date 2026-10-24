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
: fix ( a -- fix )  openness @ * 128 / ;   \ a fixed alpha (FIX, 128 = 1), faded in with the menu

\ how a picture or a word mixes with what is under it (the GS's ALPHA, `ui-blend`): by its own
\ alpha, or by a fixed one (FIX) - the colour taken off what is there, mixed with it, added to it
0 constant by-alpha   1 constant subtracted   2 constant mixed   3 constant added

\ ---- a picture: part of a cache texture, drawn at x y (screen units) ----
variable at-x  variable at-y  variable size-w  variable size-h
: picture ( x y w h -- )  size-h !  size-w !  at-y !  at-x ! ;
: blended-from ( group tex palette u v uw vh alpha blend -- )   \ (after `picture`: where it comes from)
    {: group tex palette u v uw vh alpha blend :}
    art-layer  at-x @ sx  at-y @ sy  size-w @ sw  size-h @ sy
    group tex palette  u v uw vh  blend by-alpha = if  alpha faded  else  $FFFFFFFF  then  ui-image
    art-layer blend alpha fix ui-blend ;
: from ( group tex palette u v uw vh alpha -- )  by-alpha blended-from ;

\ a panel (a background piece) and a part (a piece of the menu's own art), by number
variable piece
: column ( k -- n )  cells piece @ + @ ;
: panel ( i alpha -- )   \ panels: texture, u, v, w, h, x, y
    {: i alpha :}  i /panel * panels + piece !
    5 column 6 column 3 column 4 column picture
    back-group 0 column -1  1 column 2 column 3 column 4 column  alpha mixed blended-from ;   \ (SubScreen_DrawPanel: GS 0x64)
: part ( x y i alpha -- )   \ parts: u, v, w, h, the size drawn, palette, texture, blending
    {: x y i alpha :}  i /part * parts + piece !   \ (its blending: by-alpha .. added, SubScreen_DrawPart's kAlpha)
    x y 4 column 5 column picture
    base-group 7 column 6 column  0 column 1 column 2 column 3 column  alpha  8 column  blended-from ;
: page-art ( kind -- )   \ the background panels of a kind of page (kSubScreenFadePanels)
    4 * cells page-panels +  4 0 do  dup i cells + @ dup 0< if  drop leave  then  $80 panel  loop  drop ;
: shade ( cx cy w h -- )   \ a dark box centred on cx, cy (Task_DrawBox)
    {: cx cy w h :}
    art-layer  cx w 2/ - sx  cy h 2/ - sy  w sw  h sy  $C0 openness @ * 128 / $10100C00 or  ui-rect ;

\ ---- words ----
\ A word's colour is the original's byte (Task_DrawGlyph): its low 6 bits the font's palette; $80
\ subtracted by the alpha (palettes 4..6: drawn by their own alpha), else $40 by its own alpha,
\ else added by the alpha. The menu's words are mostly $80 - dark on the paper - the chosen $82;
\ the boxes' (their own tasks) 0, light on the dark.
: scale ( -- n )  448 sy 224 / 1 max ;   \ (the PS2's glyph 16 x 21 of 448 lines)
$80 constant menu-ink   $82 constant chosen-ink   0 constant box-ink
create hand  $60 c,   9 constant cursor-ink   \ (the font's pointing hand, the cursor; its palette 9)
$BFBFBF constant glyph-grey   \ (the glyphs' vertex colour $60606060: three quarters of white)
: ink-blend ( colour -- blend )
    dup $80 and if  $3F and  dup 4 >= swap 7 < and if  by-alpha  else  subtracted  then  exit  then
    $40 and if  by-alpha  else  added  then ;
: ink ( chosen? -- colour )  if  chosen-ink  else  menu-ink  then ;
: glyphs ( layer addr len x y colour scale -- )   \ at x y (window pixels), the alpha the menu's own
    {: layer addr len x y colour sc :}
    layer addr len x y  glyph-grey 8 lshift  $80 faded $FF and or  sc  colour $3F and  ui-text-in
    layer colour ink-blend $80 fix ui-blend ;
: inked-on ( layer addr len x y colour -- )  {: l addr len x y c :}  l addr len  x sx  y sy  c scale glyphs ;
: inked ( addr len x y colour -- )  {: a n x y c :}  word-layer a n x y c inked-on ;
: text ( addr len x y chosen? -- )  ink inked ;   \ a line of the menu's at x y
: centred ( addr len x y -- )   \ a line centred on x by its width (Text_LineWidth)
    {: addr len x y :}
    word-layer addr len  x sx  addr len scale text-width 2/ -  y sy  menu-ink scale glyphs ;
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
