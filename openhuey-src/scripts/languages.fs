\ languages.fs - the title's LANGUAGE (only with a library made from the European disc, which has
\ the game in English, French, German, Italian and Spanish; docs/library-and-mods.md "Languages").
\ The disc's own language screen's art (EURO/LANGUAGE.TEX): the language under the cursor's word
\ for "language" and its "select your language", then each language's flag and name. Up / Down
\ pick one; the cross sets it - the game's words, and its pictures with words in them, change at
\ once (core/files.h) - and keeps it in user/language.fs, read at start-up; the triangle goes back.
IN: languages
USING: engine keys ps2-screen ;

5 constant #languages   \ (the engine's order: 0 English, 1 French, 2 German, 3 Italian, 4 Spanish)
shared-variable at   \ the language under the cursor

: languages? ( -- flag )  1 language-present? ;   \ (the European disc's: all of them, or none)

\ ---- kept: user/language.fs, read at start-up (not by a test: they run in English) ----
create file-name 300 allot   shared-variable file-len
: language-file ( -- addr len )
    user-dir dup file-len !  file-name swap move
    s" language.fs" file-name file-len @ + swap dup file-len +! move
    file-name file-len @ ;
: save ( -- )
    testing? if  exit  then
    language-file to-file
    ." \ the language, chosen on the title (LANGUAGE): " language language-code type cr
    ." USING: engine ;" cr
    language n>s type ."  language!" cr
    end-file ;
: load-language ( -- )  testing? 0= language-file file-exists? and if  language-file included  then ;
' load-language catch drop

\ ---- the art: EURO/LANGUAGE.TEX (512 x 256), a 32-line row a language - its word for
\ "language" (u 0, 144 wide), its "select your language" (u 144, 176 wide), its name (u 336, 96
\ wide) - and the flags under them (v 160: 48 x 32 each). Palettes: 0 the words grey, 3 gold;
\ flag i in palette i + 1, its own colours ----
$30 constant art-group   \ (the texture cache's group: free, above the ending's picture $2E)
32 constant row-h
0 constant word-u   144 constant word-w   144 constant prompt-u   176 constant prompt-w
336 constant name-u   96 constant name-w   160 constant flags-v   48 constant flag-w
0 constant grey   3 constant gold

: open ( -- )  language at !  s" EURO/LANGUAGE.TEX" art-group tex-bank drop ;
\ a piece of the art drawn at x y (the PS2's 512 x 448 screen), w x h there
: piece ( layer x y w h u v uw vh palette -- )
    {: layer x y w h u v uw vh palette :}
    layer  x sx  y sy  w sw  h sy  art-group 0 palette  u v uw vh  $FFFFFFFF ui-image ;
: row-y ( i -- y )  44 * 168 + ;
: row ( layer i -- )
    {: layer i :}  i language-present? 0= if  exit  then
    layer  48 i row-y  flag-w row-h  i flag-w * flags-v flag-w row-h  i 1+  piece
    layer  112 i row-y  name-w row-h  name-u i row-h * name-w row-h  i at @ = if  gold  else  grey  then  piece ;

\ the keys' line, with the pad's own glyphs (the game's font: $C1 triangle, $C3 cross)
$C1 constant triangle-glyph   $C3 constant cross-glyph
create footer-text 64 allot   shared-variable footer-n
: +footer ( addr len -- )  footer-text footer-n @ + swap dup footer-n +! move ;
: +glyph ( c -- )  footer-text footer-n @ + c!  1 footer-n +! ;
: footer ( -- addr len )
    0 footer-n !  cross-glyph +glyph  s"  set    " +footer  triangle-glyph +glyph  s"  back" +footer
    footer-text footer-n @ ;
: draw ( layer -- )
    {: layer | width height :}  screen-size to height to width  frame!
    layer  40 56  word-w 3 2 */  row-h 3 2 */  word-u at @ row-h * word-w row-h  gold  piece
    layer  48 108  prompt-w row-h  prompt-u at @ row-h * prompt-w row-h  grey  piece
    #languages 0 do  layer i row  loop
    2 pen-scale
    layer  footer  width 10 /  height 85 * 100 /  $8C8880FF 2 ui-text ;

\ ---- the keys: true when it is done (set, or the triangle) ----
: step ( delta -- )   \ to the next language there is that way (the cursor stays at the end)
    {: delta :}  at @
    begin  delta +  dup 0 #languages within  while
        dup language-present? if  at !  exit  then
    repeat  drop ;
: keys ( -- done? )
    key: Up pressed? if  -1 step  then
    key: Down pressed? if  1 step  then
    cross button-pressed? if  at @ language!  save  true exit  then
    triangle button-pressed? ;
