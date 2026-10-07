\ pursuer/target.fs - whom the stalker goes for (src/game/pursuer.c Pursuer_PickTarget /
\ Pursuer_LocateTarget): Fiona seen (5 / 7), Hewie seen (1 / 2), a noise heard; his route there
\ through the house (Character_Route: the doors, +0x138C, +0x1384 their number); and the
\ commands the progress gives him (Pursuer_GrabOrder).
IN: pursuer.target
USING: engine game-state events.core events.words chars relations fiona.doors partner.route noises pursuer.core pursuer.stubs pursuer.npc pursuer.modes ;

\ ---- Character_Route: his route to room `to` (avoiding the doors in +0x148C): its door count
\ (0 there, -1 none) ----
create p-avoid 13 cells allot  p-avoid 13 cells 0 fill   \ (+0x148C, a bit a door)
create p-route 64 cells allot
: char-route ( to -- n )
    p-room swap p-avoid find-route
    dup 0> if  route p-route 64 cells move  then
    dup $1384 pu-l!  0 $1388 pu-l! ;

\ the noise he heard (c.heardSlot, c.heard.room / .tri: with the noises)
: heard-slot ( -- slot | $FF )  me c-heard-slot ;
: heard-room ( -- room )  me c-heard-room ;   : heard-tri ( -- tri )  me c-heard-tri ;

\ ---- Pursuer_PickTarget (vtable +0xCC): what drew him (+0x16CB) and how much (+0x16CC); when
\ it is more than what he follows (+0x16C9), his route to it (none: back to his own room's and
\ he forgot them) ----
: at-least ( n off -- )  dup pu-c@ rot max swap pu-c! ;
: above0 ( n off -- )  dup pu-c@ 0> if  2drop  else  pu-c!  then ;
create plt-pt 12 allot
: pick-target ( -- flag )
    0 $16CB pu-c!  0 $16CC pu-c!
    $1544 pu-c@ 1 = if  5 $16CB pu-c!  7 $16CC pu-c!  then
    $1545 pu-c@ 1 = if  1 $16CB above0  2 $16CC at-least  then
    heard-slot case
        0 of  5 $16CB pu-c!  6 $16CC at-least  endof
        1 of  1 $16CB above0  1 $16CC above0  endof
        3 of  4 $16CB at-least  5 $16CC at-least  endof
    endcase
    $16CB pu-c@ 0=  $16CB pu-c@ $16C9 pu-c@ < or if  false exit  then
    in-played-room? p-room heard-room = and  $16CC pu-c@ 5 = and  p-mode 8 = and if
        heard-tri dup tri-center plt-pt vec!  plt-pt sees-point? if  false exit  then
    then
    heard-room  $16CB pu-c@ case
        5 of  drop her c-room  endof
        1 of  drop dog c-room  endof
    endcase
    char-route 0< 0= if
        mode 0<>  mode 4 <> and if  1 $16F2 pu-c!  1 $16F1 pu-c!  then
        $16CA pu-c@ $16CC pu-c@ < if
            in-played-room? $16F8 pu-c@ 0= and if  1 $16F3 pu-c!  then
            $16CC pu-c@ $16CA pu-c!  $16CB pu-c@ $16C9 pu-c!
        then
        $D0 vcall  true exit
    then
    $1594 pu-l@ char-route drop
    0 $1544 pu-c!  0 $1545 pu-c!  0 $1546 pu-c!  false ;
' pick-target $CC vt!

\ ---- Pursuer_LocateTarget (vtable +0xD0): where he goes for it (+0x179C its triangle) ----
: locate-target ( -- )
    $16CB pu-c@ case
        5 of
            $16F8 pu-c@ 0= if  $B0 vcall
            else side-behind her-side-behind <> if  her-side-behind $1598 pu-l!  then then
            her c-tri $179C pu-l!
        endof
        1 of
            $16F8 pu-c@ 0= if  dog $B4 vcall  then
            played dog c-room = if  dog c-tri $179C pu-l!  then
        endof
        4 of  not-yet" Pursuer_LocateTarget: a noise"  endof
    endcase
    $16F8 pu-c@ 0= in-played-room? and reached-room? and if  $D8 vcall drop  then ;
' locate-target $D0 vt!

\ ---- Pursuer_GrabOrder (vtable +0x110): a command the progress gave him (none: -1) ----
: grab-order ( -- n )
    me cmd? 0= if  -1 exit  then
    not-yet" Pursuer_GrabOrder: his commands (Hewie held, Fiona caught)"
    me cmd-cancel  -1 ;
' grab-order $110 vt!
