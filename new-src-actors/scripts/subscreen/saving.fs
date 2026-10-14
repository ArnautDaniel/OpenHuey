\ subscreen/saving.fs - the save slots (the scripts' mode 1: a save point - the story's shared
\ actions $84 / $85 ask first; src/game/subscreen.c SubScreen_StateSave, memcard.c's save screen):
\ the eight slots listed - each its place and the time played, or empty; the arrows choose, the
\ cross saves there (a used one asked first: write over it?), the triangle leaves. Saved, it
\ says so a moment and the menu closes. (The original's memory card screens are its own; this is
\ hga's: its saves are files - saves.fs.)
IN: subscreen.saving
USING: engine actors messages keys saves subscreen.state subscreen.art subscreen.input ;

\ where a save is made (D_00412800: room, map -> its place's name, the menu's messages $300..;
\ none known: "No Data")
create places
    $21 , 3 , $300 ,  $32 , 3 , $300 ,  $F , 2 , $301 ,  $1A , 2 , $302 ,  $1C , 2 , $303 ,
    $7 , 2 , $304 ,  $5C , 2 , $305 ,  $6F , 1 , $306 ,  $62 , 2 , $307 ,  $54 , 1 , $308 ,
    $69 , 2 , $309 ,  $54 , 3 , $30A ,  $40 , 1 , $30B ,  $40 , 2 , $30C ,  $106 , 0 , $30D ,
    $C1 , 1 , $30E ,  $C5 , 7 , $30F ,  $C6 , 7 , $30F ,  $9B , 1 , $310 ,  $8D , 1 , $311 ,
    $85 , 0 , $312 ,  $88 , 1 , $313 ,  $91 , 1 , $314 ,
23 constant #places
$85 constant no-data
: place ( i -- addr )  3 cells * places + ;
: place-of ( room map -- msg )   \ (the room and map both; else the room's first)
    {: room map :}
    #places 0 do  i place @ room =  i place cell+ @ map = and if  i place 2 cells + @ unloop exit  then  loop
    #places 0 do  i place @ room = if  i place 2 cells + @ unloop exit  then  loop
    no-data ;

\ the time played as hh:mm:ss
create clock 8 allot
: two ( n at -- )  >r  10 /mod [char] 0 + r@ c!  [char] 0 + r> 1+ c! ;
: clock$ ( frames -- addr len )
    30 /  dup 3600 / 99 min clock two  [char] : clock 2 + c!
    dup 60 / 60 mod clock 3 + two  [char] : clock 5 + c!  60 mod clock 6 + two  clock 8 ;

: saves-id ( -- id )  s" saves" actor-named ;
: start-saving ( -- )  0 save-at !  0 save-step !  0 save-t ! ;
: write-here ( -- )   \ (her room and map: where it is made)
    2 save-step !  save-at @  room-id her-map @ place-of  saves-id send save-to-slot ;
: saving-keys ( -- )
    save-step @ case
        0 of
            up? if  save-at @ 1- #slots wrap save-at !  se-cursor  then
            down? if  save-at @ 1+ #slots wrap save-at !  se-cursor  then
            confirm? if  se-decide  save-at @ slot-used? if  0 save-yes !  1 save-step !  else  write-here  then  then
            back? if  se-cancel  close-menu  then  endof
        1 of
            left? right? or up? or down? or if  save-yes @ 0= save-yes !  se-cursor  then
            confirm? if  save-yes @ if  se-decide  write-here  else  se-cancel  0 save-step !  then  then
            back? if  se-cancel  0 save-step !  then  endof
        3 of  1 save-t +!  save-t @ 60 >=  confirm? or if  close-menu  then  endof
    endcase ;
: saved-now ( -- )  3 save-step !  0 save-t !  se-used ;   \ (saves.fs answers: it is written)

: slot-line ( i -- )
    {: i | y :}  i 22 * $78 + to y
    i 1+ n>s  $40 y  i save-at @ = text
    i slot-used? if
        i slot-place@ menu-line  $60 y  i save-at @ = text
        i slot-frames@ clock$  $170 y  i save-at @ = text
    else  s" - empty -"  $60 y  i save-at @ = text  then ;
: saving-view ( -- )
    $100 $E0 $1C0 $150 shade
    s" Save the game" $100 $48 centred
    #slots 0 do  i slot-line  loop
    save-step @ case
        0 of  s" Cross: save here     Triangle: back" $100 $1A0 centred  endof
        1 of  s" Write over this save?" $100 $1A0 centred
              s" Yes" $D0 $1B8 save-yes @ text  s" No" $118 $1B8 save-yes @ 0= text  endof
        2 of  s" Saving..." $100 $1A0 centred  endof
        3 of  s" Saved." $100 $1A0 centred  endof
    endcase ;
