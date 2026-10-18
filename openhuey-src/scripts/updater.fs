\ updater.fs - the title's UPDATE (a release build's only: `updater?`; docs/library-and-mods.md,
\ "Updating"): looks for the newest build - the rolling openhuey-latest release - and if it is
\ another than this one, downloads it and puts it in place beside the executable (library/, user/
\ and mods/ left as they are). The engine does the work on a thread of its own
\ (src/platform/update.c); this shows how it goes. Once updated, the cross starts the new build;
\ the triangle goes back (the update goes on if it was going).
IN: updater
USING: engine keys ;

\ update-state's: (update.c)
enum: update-state-n  not-begun looking downloading unpacking installing current updated failed ;

: busy? ( -- flag )  update-state  dup looking >=  swap installing <= and ;
: open ( -- )  busy? 0= if  update-start  then ;

\ ---- drawn on the caller's layer ----
create line-text 160 allot   variable line-n
: +line ( addr len -- )  line-n @ over + 160 > if  2drop exit  then
    line-text line-n @ + swap dup line-n +! move ;
: mb ( bytes -- addr len )  20 rshift n>s ;
: progress-line ( -- addr len )   \ "12 of 31 MB"
    0 line-n !  update-progress {: have total :}  have mb +line  s"  of " +line  total mb +line  s"  MB" +line
    line-text line-n @ ;
: status ( -- addr len )
    update-state case
        looking of  s" Looking for a newer version ..."  endof
        downloading of  s" Downloading "  endof
        unpacking of  s" Unpacking it ..."  endof
        installing of  s" Putting it in place ..."  endof
        current of  s" Up to date: this is the newest build."  endof
        updated of  s" Updated. Start it now?"  endof
        failed of  s" It can't be updated:"  endof
        >r s" "  r>
    endcase ;
: bar ( layer x y w -- )   \ the download's bar
    {: layer x y w :}  update-progress {: have total :}
    layer  x y w 12  $46403AFF ui-rect
    total 0> if  layer  x y  w have * total /  w min  12  $C89646FF ui-rect  then ;
\ the keys' line, with the pad's own glyphs (the game's font: $C1 triangle, $C3 cross)
$C1 constant triangle-glyph   $C3 constant cross-glyph
: +glyph ( c -- )  line-text line-n @ + c!  1 line-n +! ;
: footer ( -- addr len )
    0 line-n !
    update-state updated = if  cross-glyph +glyph  s"  start it    " +line  then
    triangle-glyph +glyph  s"  back" +line  line-text line-n @ ;
: draw ( layer -- )
    {: layer | sw sh left :}  screen-size to sh to sw  sw 10 / to left
    3 pen-scale
    layer  s" UPDATE"  left  sh 8 /  $D8D0C8FF 3 ui-text
    2 pen-scale
    layer  s" This build: "  left  sh 4 /  $8C8880FF 2 ui-text
    layer  openhuey-build  left 12 char-size drop * +  sh 4 /  $B0A8A0FF 2 ui-text
    layer  status  left  sh 3 /  $D8D0C8FF 2 ui-text
    update-state downloading = if
        layer  update-text  left 12 char-size drop * +  sh 3 /  $D8D0C8FF 2 ui-text
        layer  left  sh 2/  sw 6 * 10 /  bar
        layer  progress-line  left  sh 2/ 20 +  $8C8880FF 2 ui-text
    then
    update-state dup current = swap updated = or if
        layer  s" Newest: "  left  sh 3 / char-size nip 2* +  $8C8880FF 2 ui-text
        layer  update-text  left 12 char-size drop * +  sh 3 / char-size nip 2* +  $B0A8A0FF 2 ui-text
    then
    update-state failed = if
        layer  update-text  left  sh 3 / char-size nip 2* +  $D09060FF 2 ui-text
    then
    layer  footer  left  sh 85 * 100 /  $8C8880FF 2 ui-text ;

\ ---- the keys: true when it is done (the triangle) ----
: keys ( -- done? )
    update-state updated =  cross button-pressed? and if
        update-restart if  bye  then
    then
    triangle button-pressed? ;
