\ subscreen/box.fs - the menu's box: a message ("Used the Camomile."), or a question with its
\ options (yes / no): a page at a time, the cross turns it; on the last the arrows pick an option
\ and the cross answers. Then what was waiting on it runs, given the option picked (0 the first).
IN: subscreen.box
USING: engine messages keys ps2-screen subscreen.state subscreen.art subscreen.input ;

: ask ( msg then-xt -- )  after !  shown !  0 shown-page !  0 picked !  on-box mode ! ;
: last-shown-page? ( -- flag )  shown-page @ 1+  shown @ message-layout  >= ;
: choosing? ( -- flag )  shown @ message-layout drop  message-options 0>  last-shown-page? and ;

\ a line's left: 0x12 centres it in the page's widest, 0x14 puts it to that's right (Task_DrawPage)
: line-width ( line -- px )  shown @ shown-page @ rot line-of  scale text-width ;
: widest ( -- px )  0  shown @ message-layout drop  shown-page @ message-lines 0 ?do  i line-width max  loop ;
: line-x ( line -- x )
    {: ln | spare :}  widest ln line-width - to spare
    shown @ message-layout drop  shown-page @ ln message-align
    case  1 of  spare 2/  endof  2 of  spare  endof  0 swap  endcase  $40 sx + ;
: marker ( -- )   \ the hand before the option picked (an option takes no room: 20 of the PS2's pixels before it)
    shown @ message-layout drop  picked @ message-option-at  {: pg line col :}
    line line-x  pg line message-line drop col scale text-width +  {: x :}   \ (line-x lays it out again)
    word-layer hand 1  x scale 10 * -  line 22 * $B0 + sy  cursor-ink scale glyphs ;
: box-view ( -- )   \ the box's words: its own task's, colour 0 (added: light on the shade)
    $100 $E0 $1A0 $A0 shade
    shown @ message-layout drop
    shown-page @ message-lines 0 ?do  word-layer  shown @ shown-page @ i line-of  i line-x  $B0 i 22 * + sy  box-ink scale glyphs  loop
    choosing? if  marker  then ;
: box-keys ( -- )
    choosing? if
        left? up? or if  0 picked !  se-cursor  then
        right? down? or if  message-options 1- picked !  se-cursor  then
    then
    confirm? if
        last-shown-page? 0= if  1 shown-page +!  exit  then
        -1 shown !  back-to-page  picked @ after @ execute
    then ;
