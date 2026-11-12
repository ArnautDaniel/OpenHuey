\ subscreen/options.fs - the options page (src/game/subscreen.c Options_StateList, Options_Draw,
\ Options_Defaults): five rows - the controller type (its buttons' names; fixed to the keyboard
\ here), vibration, the sound out, the volume, the screen's position. The cross edits a row and
\ the arrows change it; the square asks to put them all back. Kept in the system data
\ (saves.fs, as the original's +0x30: told each change, given them back as the game starts); the
\ vibration, the sound out and the volume take effect at once (SubScreen_ApplyOptions) - the screen's
\ position has nothing here to change.
IN: subscreen.options
USING: engine actors messages keys facts subscreen.state subscreen.art subscreen.input subscreen.box ;

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
: apply ( -- )   \ (SubScreen_ApplyOptions: the vibration on or off, the sound out - SndDriver_SetOutputMode:
    \ how a placed sound's sides combine, mono at 0 - everything heard at the volume)
    vibration @ 0<> rumble-allowed!  sound-out @ sound-out!  volume @ s>f 128e f/ game-volume! ;
: defaults ( -- )  1 vibration !  1 sound-out !  128 volume !  0 pos-x !  0 pos-y !  0 layout !  apply ;
: kept ( -- )   \ (to the system data)
    vibration @ sound-out @ volume @ pos-x @ pos-y @ layout @  saves-id send options-changed ;
: restore ( vib out vol x y layout -- )  layout !  pos-y !  pos-x !  volume !  sound-out !  vibration !  apply ;

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
: setting-of ( i -- )   \ a row's setting
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
        i setting-of
    loop
    buttons
    $20 opt-row @ row-y $1C $80 part   \ the cursor
    $82 menu-line $46 $186 false text ;

: put-back ( answer -- )  0= if  defaults kept  then  back-to-page ;
\ ---- a row edited (Options_StateVibration / Sound / Volume / Position): each change heard or seen at
\ once; the cross keeps it, the triangle - or Select - puts back what it was (opt_cancelled) ----
create was 5 cells allot   \ (the settings as the edit began: vibration, sound out, volume, x, y)
: snapshot ( -- )  vibration @ was !  sound-out @ was cell+ !  volume @ was 2 cells + !  pos-x @ was 3 cells + !  pos-y @ was 4 cells + ! ;
: undo ( -- )  was @ vibration !  was cell+ @ sound-out !  was 2 cells + @ volume !  was 3 cells + @ pos-x !  was 4 cells + @ pos-y !  apply ;
: toggled ( -- )   \ (turned on: the pad buzzes)
    vibration @ 0= 1 and vibration !  apply  vibration @ if  0 true 4 rumble-buzz  0 $80 4 rumble  then  se-cursor ;
: sound-round ( d -- )  sound-out @ + 3 + 3 mod sound-out !  apply  se-cursor ;   \ (0 mono, 1 stereo, 2 surround, round)
2 constant volume-step   \ (1/64 of full: 128 here)
: volume-by ( d -- )   \ (down from 0 its sound still; up at full none)
    dup 0< volume @ 128 < or if  se-cursor  then
    volume-step * volume @ + 0 max 128 min volume !  apply ;
: nudge ( d addr -- )   \ (the screen's position, -32..32: a sound each step it takes)
    {: d a :}  a @ d + dup -32 32 1+ within if  a !  se-cursor  else  drop  then ;
: change ( d -- )   \ the row's setting changed (left / right)
    opt-row @ case
        1 of  drop toggled  endof
        2 of  sound-round  endof
        3 of  volume-by  endof
        4 of  pos-x nudge  endof
        >r drop r>
    endcase ;
: cancelled? ( -- flag )  back?  select-button button-pressed? or ;
: editing-keys ( -- )
    left? if  -1 change  else  right? if  1 change  then  then
    opt-row @ 4 = if   \ (the position: up / down too)
        up? if  -1 pos-y nudge  else  down? if  1 pos-y nudge  then  then
    then
    confirm? if  kept  0 opt-editing !  se-decide exit  then
    cancelled? if  undo  0 opt-editing !  se-cancel  then ;
: options-keys ( -- )
    opt-editing @ if  editing-keys exit  then
    confirm? if  snapshot  -1 opt-editing !  se-decide exit  then
    up? if  opt-row @ 1- 5 wrap opt-row !  se-cursor  then
    down? if  opt-row @ 1+ 5 wrap opt-row !  se-cursor  then
    square button-pressed? if  se-decide  $4084 ['] put-back ask exit  then
    page-keys ;
