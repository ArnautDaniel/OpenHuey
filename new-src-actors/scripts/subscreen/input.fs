\ subscreen/input.fs - the menu's keys and sounds (Pads_FaceButtons: the cross confirms, the
\ triangle backs out, the square sorts / resets, L1 / R1 turn pages; the arrows - or WASD - move,
\ again after 12 frames held, then every 2) and the menu sounds (bank 5).
IN: subscreen.input
USING: engine keys subscreen.state ;

: se ( id -- )  5 bank-sound ;
: se-cursor ( -- )  $2A se ;   : se-decide ( -- )  $2B se ;   : se-cancel ( -- )  $2C se ;
: se-buzzer ( -- )  $85 se ;   : se-page ( -- )  $87 se ;     : se-used ( -- )  $84 se ;
: se-open ( -- )  $94 se ;     : se-close ( -- )  $95 se ;

: confirm? ( -- flag )  cross button-pressed? ;
: back? ( -- flag )  triangle button-pressed? ;
: next-page? ( -- flag )  r1 button-pressed? ;
: prev-page? ( -- flag )  l1 button-pressed? ;

\ the arrows: pressed, or held (the pad's repeat)
variable arrows
: arrow-keys ( i -- k1 k2 )   \ 0 up, 1 right, 2 down, 3 left
    case  0 of  key: Up key: W  endof  1 of  key: Right key: D  endof  2 of  key: Down key: S  endof
          >r key: Left key: A r>  endcase ;
: repeats? ( frames -- flag )   \ held that long: pressed on its first frame, then the 13th, 15th, 17th...
    dup 1 = if  drop true exit  then
    dup 13 < if  drop false exit  then
    13 - 2 mod 0= ;
: read-arrows ( -- )   \ (once a frame)
    0 arrows !
    4 0 do
        i cells held-for +  i arrow-keys key-down? swap key-down? or if
            dup 1 swap +!  @ repeats? if  1 i lshift arrows @ or arrows !  then
        else  0 swap !  then
    loop ;
: arrow? ( i -- flag )  1 swap lshift arrows @ and 0<> ;
: up? ( -- flag )  0 arrow? ;   : right? ( -- flag )  1 arrow? ;
: down? ( -- flag )  2 arrow? ;  : left? ( -- flag )  3 arrow? ;

\ a cursor round 0..n-1
: wrap ( i n -- i' )  tuck + swap mod ;

\ the pages round, as L1 / R1 turn them (the three lists, the map, the files, the options)
create page-order  pg-special , pg-items , pg-other , pg-map , pg-files , pg-options ,
: place-in-order ( pg -- i )  6 0 do  i cells page-order + @ over = if  drop i unloop exit  then  loop  drop 0 ;
: turn-page ( d -- )  page @ place-in-order +  6 wrap  cells page-order + @ page !  se-page ;
: page-keys ( -- )   \ (every page: L1 / R1 turn it - not the title's options alone - the triangle closes the menu)
    options-only @ 0= if
        next-page? if  1 turn-page  then
        prev-page? if  -1 turn-page  then
    then
    back? if  close-menu  then ;
