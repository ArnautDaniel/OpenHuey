\ subscreen/box.fs - the menu's box: a message ("Used the Camomile."), or a question with its
\ options (yes / no): a page at a time, the cross turns it; on the last the arrows pick an option
\ and the cross answers. Then what was waiting on it runs, given the option picked (0 the first).
IN: subscreen.box
USING: engine messages keys ps2-screen subscreen.state subscreen.art subscreen.input ;

\ (a choice's flags - its 0x0E code, as window.fs reads them: Task_StateType): the cursor starts on the last
\ option ("no") with bit 1; the triangle may go to the last unless bit 0
2 constant starts-on-last   1 constant no-cancel
: last-option ( -- i )  message-options 1- 0 max ;
: first-pick ( -- i )  shown @ message-layout drop  message-choice-flags starts-on-last and if  last-option  else  0  then ;
: ask ( msg then-xt -- )  after !  shown !  0 shown-page !  first-pick picked !  0 box-place !  0 big-box !  on-box mode ! ;
: ask-document ( msg then-xt -- )  ask  -1 big-box ! ;   \ (a document read: the large shade behind its page)
: last-shown-page? ( -- flag )  shown-page @ 1+  shown @ message-layout  >= ;
: choosing? ( -- flag )  shown @ message-layout drop  message-options 0>  last-shown-page? and ;

\ ---- where its words go (Task_Layout, Task_DrawPage): the page's lines, 25 apart, as a block as
\ wide as its widest, centred on its place - (256, 370) unless the message names another (its 0x0A
\ code: D_0044B010; from $80 with no box of its own) ----
25 constant line-pitch
create place-ys  370 , 224 , 330 , 224 , 224 , 224 , 224 , 224 ,
: place-now ( -- n )   \ (this page's, else the one before it; the layout is the shown message's)
    shown-page @ message-place  dup 0< if  drop box-place @  else  dup box-place !  then ;
: place-y ( -- y )  place-now $7F and 7 min cells place-ys + @ ;
: boxed? ( -- flag )  place-now $80 and 0= ;
: block-h ( -- h )  shown-page @ message-lines line-pitch * ;
: words-y ( line -- y )  line-pitch *  place-y block-h 2/ - +  4 + ;   \ (the PS2's)
: line-width ( line -- px )  shown @ shown-page @ rot line-of  scale text-width ;
: widest ( -- px )  0  shown @ message-layout drop  shown-page @ message-lines 0 ?do  i line-width max  loop ;
\ a line's left: 0x12 centres it in the page's widest, 0x14 puts it to that's right (Task_DrawPage)
: line-x ( line -- x )
    {: ln | spare :}  widest ln line-width - to spare
    shown @ message-layout drop  shown-page @ ln message-align
    case  1 of  spare 2/  endof  2 of  spare  endof  0 swap  endcase  $100 sx widest 2/ - + ;
: marker ( -- )   \ the hand before the option picked (an option takes no room: 20 of the PS2's pixels before it)
    shown @ message-layout drop  picked @ message-option-at  {: pg line col :}
    line line-x  pg line message-line drop col scale text-width +  {: x :}   \ (line-x lays it out again)
    word-layer hand 1  x scale 10 * -  line words-y sy  cursor-ink scale glyphs ;
\ its own shade (Task_BoxSize): the block's, less 24 across and 16 down, 6 lower - its soft edges
\ take the words in; a document has the large one behind it too
: own-shade ( -- )
    widest 512 * fw @ /  dup $19 >= if  $18 -  then
    block-h  dup $11 >= if  $10 -  then  {: w h :}
    $100  place-y 6 +  w h shade ;
: box-view ( -- )   \ the box's words: its own task's, colour 0 (added: light on the shade)
    shown @ message-layout drop
    big-box @ if  $100 $E0 $1A0 $160 shade  then
    boxed? if  own-shade  then
    shown @ message-layout drop
    shown-page @ message-lines 0 ?do  word-layer  shown @ shown-page @ i line-of  i line-x  i words-y sy  box-ink scale glyphs  loop
    choosing? if  marker  then ;
\ ---- its keys (Task_StateChoice, Task_StateNextPage): choosing, the cross answers (its sound), the
\ triangle goes to the last option where the choice allows (its sound), up / down round the options,
\ left / right round those on the picked one's line (one alone there: as down / up) - a sound only for
\ a change; else a page turned or the box closed by Select, L1, R1 or a face button (the pad's 0xFC01) ----
: option-line ( i -- line )  message-option-at drop nip ;
: round ( i -- i' )  message-options + message-options mod ;
: row-count ( line -- n )  0  message-options 0 ?do  over i option-line = if  1+  then  loop  nip ;
: along-row ( d -- )
    {: d | ln :}  picked @ option-line to ln
    ln row-count 2 < if  picked @ d + round picked !  exit  then
    picked @  begin  d + round  dup option-line ln =  until  picked ! ;
: answered ( -- )  -1 shown !  back-to-page  picked @ after @ execute ;
: choice-keys ( -- )
    confirm? if  se-decide  answered exit  then
    message-choice-flags no-cancel and 0=  back? and if
        picked @ last-option <> if  last-option picked !  se-cancel  then  exit
    then
    picked @  {: was :}
    right? if  1 along-row  else left? if  -1 along-row
    else up? if  picked @ 1- round picked !  else down? if  picked @ 1+ round picked !  then then then then
    picked @ was <> if  se-cursor  then ;
: box-keys ( -- )
    shown @ message-layout drop
    choosing? if  choice-keys exit  then
    any-button? if
        last-shown-page? 0= if  1 shown-page +!  exit  then
        answered
    then ;
