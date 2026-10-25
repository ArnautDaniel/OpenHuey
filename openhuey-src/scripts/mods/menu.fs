\ mods/menu.fs - the mods menu (the title's MODS, shown when there are mods; docs/library-and-mods.md):
\ the mods in mods/ in their load order, each on or off, and why one asked for didn't load this
\ run. Up / Down pick one; the cross turns it on or off; Left / Right (or L1 / R1) move it in the
\ order; the triangle goes back, writing mods/order.txt if anything changed - it applies when OpenHuey
\ next starts (a mod's scripts load with the game's). The engine keeps the list (src/game/mods.c).
IN: mods.menu
USING: engine keys ;

variable at         \ the mod under the cursor
variable saved      \ order.txt written this visit (0 not, 1 written, -1 couldn't be)

: open ( -- )  0 at !  0 saved ! ;
: rows ( -- n )  mods-menu-count ;
: pick-row ( delta -- )  at @ +  0 max  rows 1- 0 max min  at ! ;
: reorder ( delta -- )  rows if  at @ swap mods-menu-move at !  else  drop  then ;

\ ---- drawn on the caller's layer: the title at the top, a row a mod, the keys at the foot ----
: row-colour ( i -- rgba )  at @ = if  $FFFFFFFF  else  $B0A8A0FF  then ;
: row ( layer i -- )
    {: layer i | sw sh y left :}  screen-size to sh to sw
    sh 4 /  i char-size nip 2* *  +  to y   sw 10 /  to left
    i mods-menu-mod {: a n on :}
    layer  on if  s" ON "  else  s" OFF"  then  left  y  on if  $C8F0A0FF  else  $807870FF  then  2 ui-text
    layer  a n  left 5 char-size drop * +  y  i row-colour  2 ui-text
    i mods-menu-why dup if
        layer -rot  left 28 char-size drop * +  y  $D09060FF  2 ui-text
    else  2drop  then ;
\ the keys' line, with the pad's own glyphs (the game's font: from $C0 circle, triangle, square,
\ cross, L1, L2, R1)
$C1 constant triangle-glyph   $C3 constant cross-glyph   $C4 constant l1-glyph   $C6 constant r1-glyph
create footer-text 64 allot   variable footer-n
: +footer ( addr len -- )  footer-text footer-n @ + swap dup footer-n +! move ;
: +glyph ( c -- )  footer-text footer-n @ + c!  1 footer-n +! ;
: footer ( -- addr len )
    0 footer-n !  cross-glyph +glyph  s"  on / off    " +footer  l1-glyph +glyph  s"  " +footer  r1-glyph +glyph
    s"  load order    " +footer  triangle-glyph +glyph  s"  back" +footer  footer-text footer-n @ ;
: draw ( layer -- )
    {: layer | sw sh :}  screen-size to sh to sw
    3 pen-scale
    layer  s" MODS"  sw 10 /  sh 8 /  $D8D0C8FF 3 ui-text
    2 pen-scale
    rows 0= if  layer  s" - no mods in mods/ -"  sw 10 /  sh 4 /  $807870FF 2 ui-text  exit  then
    rows 0 do  layer i row  loop
    layer  s" >"  sw 10 / char-size drop 2* -  sh 4 / at @ char-size nip 2* * +  $FFFFFFFF 2 ui-text
    layer  footer  sw 10 /  sh 85 * 100 /  $8C8880FF 2 ui-text
    mods-menu-changed? if
        layer  s" changes apply when OpenHuey starts again"  sw 10 /  sh 90 * 100 /  $D09060FF 2 ui-text
    then
    saved @ 0< if
        layer  s" mods/order.txt can't be written"  sw 10 /  sh 95 * 100 /  $E06050FF 2 ui-text
    then ;

\ ---- the keys: true when it is done (the triangle) ----
: keys ( -- done? )
    key: Up pressed? if  -1 pick-row  then
    key: Down pressed? if  1 pick-row  then
    key: Left pressed?  l1 button-pressed? or if  -1 reorder  then
    key: Right pressed?  r1 button-pressed? or if  1 reorder  then
    cross button-pressed? rows 0> and if  at @ mods-menu-switch  then
    triangle button-pressed? if
        mods-menu-changed? if  mods-menu-save if  1  else  -1  then  saved !  then
        saved @ 0< 0=   \ (written, or nothing to write: back; not written: stay, say so)
    else  false  then ;
