\ subscreen/box.fs - the menu's box: a message ("Used the Camomile."), or a question with its
\ options (yes / no): a page at a time, the cross turns it; on the last the arrows pick an option
\ and the cross answers. Then what was waiting on it runs, given the option picked (0 the first).
IN: subscreen.box
USING: engine messages keys ps2-screen subscreen.state subscreen.art subscreen.input ;

: ask ( msg then-xt -- )  after !  shown !  0 shown-page !  0 picked !  on-box mode ! ;
: last-shown-page? ( -- flag )  shown-page @ 1+  shown @ message-layout  >= ;
: choosing? ( -- flag )  shown @ message-layout drop  message-options 0>  last-shown-page? and ;

create hand  $60 c,   9 constant hand-palette   \ (the font's pointing hand, in its palette 9: Task_DrawPage)
: marker ( -- )   \ the hand before the option picked (an option takes no room: 20 of the PS2's pixels before it)
    shown @ message-layout drop  picked @ message-option-at  {: pg line col :}
    pg line message-line drop col scale text-width  {: indent :}
    word-layer hand 1  $40 sx indent +  scale 10 * -  line 22 * $B0 + sy  $FFFFFFFF scale hand-palette  ui-text-in ;
: box-view ( -- )
    $100 $E0 $1A0 $A0 shade
    shown @ message-layout drop
    shown-page @ message-lines 0 ?do  shown @ shown-page @ i line-of  $40  $B0 i 22 * +  false text  loop
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
