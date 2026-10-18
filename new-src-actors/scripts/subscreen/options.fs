\ subscreen/options.fs - the options page (src/game/subscreen.c Options_StateList, Options_Draw,
\ Options_Defaults): five rows - the controller type (its buttons' names; fixed to the keyboard
\ here), vibration, the sound out, the volume, the screen's position. The cross edits a row and
\ the arrows change it; the square asks to put them all back. Kept for the session; the
\ vibration and the volume take effect at once (SubScreen_ApplyOptions) - the sound out and the
\ screen's position have nothing here to change.
IN: subscreen.options
USING: engine keys facts subscreen.state subscreen.art subscreen.input subscreen.box ;

\ each row: its icon (a part), its label (a menu message), its line
create option-rows   $10 , $60 , $54 ,   $11 , $77 , $F4 ,   $12 , $7A , $114 ,   $13 , $7E , $134 ,   $14 , $81 , $154 ,
: row ( i -- addr )  3 * cells option-rows + ;
: row-y ( i -- y )  row 2 cells + @ ;
\ the controller types' button names (D_0044B570): its name, then the buttons' actions
create layouts
    $61 , $6D , $6F , $70 , $71 , $75 , $76 ,   $62 , $70 , $6F , $6D , $71 , $75 , $76 ,
    $63 , $6D , $75 , $70 , $6F , $71 , $76 ,   $64 , $6F , $76 , $6D , $70 , $71 , $75 ,
\ ... and Hewie's, played in the mini-game's run (D_0044B570's second set)
create run-layouts
    $61 , $4B , $4C , $4D , $71 , $4E , $4F ,   $62 , $4D , $4C , $4B , $71 , $4E , $4F ,
    $63 , $4B , $4E , $4D , $4C , $71 , $4F ,   $64 , $4C , $4F , $4B , $4D , $71 , $4E ,
: action-name ( i -- msg )  layout @ 7 * + cells  hewie-played? if  run-layouts  else  layouts  then  + @ ;
: apply ( -- )   \ (SubScreen_ApplyOptions: the vibration on or off, everything heard at the volume)
    vibration @ 0<> rumble-allowed!  volume @ s>f 128e f/ game-volume! ;
: defaults ( -- )  1 vibration !  1 sound-out !  128 volume !  0 pos-x !  0 pos-y !  0 layout !  apply ;

: say ( msg x y -- )  {: msg x y :}  msg menu-line x y false text ;
: buttons ( -- )   \ the buttons and what they do, in two columns of four
    4 0 do
        $65 i +  $40  $74 i $20 * +  say      i 1+ action-name  $60  $74 i $20 * +  say
        $69 i +  $110 $74 i $20 * +  say
    loop
    $73 $130 $74 say  $74 $130 $94 say  5 action-name $130 $B4 say  6 action-name $130 $D4 say ;
: position$ ( n label len -- addr len )  spell-reset +spell  n>s +spell  spelled ;
variable line-y
: beside ( msg -- )  menu-line $160 line-y @ centred ;   \ a setting's word, centred beside its label
: value ( i -- )   \ a row's setting
    dup row-y 6 + line-y !
    case
        0 of  0 action-name beside  endof
        1 of  vibration @ if  $78  else  $79  then  beside  endof
        2 of  sound-out @ $7B + beside  endof
        3 of  $7F $F0 line-y @ say  $80 $1A8 line-y @ say  volume @ $118 +  line-y @ 6 -  $19 $80 part  endof   \ (min, max, the knob along its bar)
        4 of  pos-x @ s" X : " position$  $11C line-y @ false text  pos-y @ s" Y : " position$  $170 line-y @ false text  endof
    endcase ;
: options-view ( -- )
    3 page-art
    5 0 do
        $20  i row-y  i row @  $80 part
        i row cell+ @ menu-line  $48  i row-y 6 +  i opt-row @ = opt-editing @ and  text
        i value
    loop
    buttons
    $20 opt-row @ row-y $1C $80 part   \ the cursor
    $82 menu-line $46 $186 false text ;

: change ( d -- )   \ the row's setting changed (left / right)
    opt-row @ case
        1 of  drop  vibration @ 0= 1 and vibration !  apply
              vibration @ if  0 true 4 rumble-buzz  0 $80 4 rumble  then  endof   \ (turned on: the pad buzzes)
        2 of  sound-out @ + 0 max 2 min sound-out !  endof
        3 of  16 * volume @ + 0 max 128 min volume !  apply  endof
        4 of  pos-x @ + -32 max 32 min pos-x !  endof
        >r drop r>
    endcase ;
: put-back ( answer -- )  0= if  defaults  then  back-to-page ;
: editing-keys ( -- )
    left? if  -1 change  se-cursor  then
    right? if  1 change  se-cursor  then
    opt-row @ 4 = if   \ (the position: up / down too)
        up? if  pos-y @ 1- -32 max pos-y !  then
        down? if  pos-y @ 1+ 32 min pos-y !  then
    then
    confirm? if  0 opt-editing !  se-decide  then
    back? if  0 opt-editing !  se-cancel  then ;
: options-keys ( -- )
    opt-editing @ if  editing-keys exit  then
    confirm? if  -1 opt-editing !  se-decide exit  then
    up? if  opt-row @ 1- 5 wrap opt-row !  se-cursor  then
    down? if  opt-row @ 1+ 5 wrap opt-row !  se-cursor  then
    square button-pressed? if  se-decide  $4084 ['] put-back ask exit  then
    page-keys ;
