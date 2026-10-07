\ pursuer/hit.fs - the stalker struck (src/game/pursuer.c Pursuer_Hit, his request 4: [1] the
\ kind - 1 / 2 / 4 blows, 3 a killing one, 5 a door, 6 Hewie snapping, 7 a held one, 0xA /
\ 0xB; [2] by whom; [3] the damage; [4] 0x8000 a stumble): his damage, Hewie's bites counted
\ (+0x16BC against +0x16DC), his flinch (0x1000..0x1005) or fall (0x1800 / 0x1804) and getting
\ up, and the frame update after (Pursuer_FrameUpdate: giving up for a while, or Hewie).
IN: pursuer.hit
USING: engine game-state events.core events.words chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.target pursuer.chase pursuer.attack ;

\ Character_TimerDown (vtable +0x94): his health less |n|; true at 0
: take-damage ( n -- down? )  abs p-hp swap - dup 0> if  p-hp! false  else  drop 0 p-hp! true  then ;
' take-damage $94 vt!
:noname  $16D0 pu-l@ $1664 pu-l! ; $2CC vt!   \ Pursuer_NoGiveUp
:noname  ; $104 vt!                          \ Pursuer_StopSounds (his looped sounds: none kept)
: clear-message ( -- )  0 0 p-req!  0 1 p-req! ;
: bites ( -- n )  $16BC pu-l@ ;   : bites! ( n -- )  1000 min $16BC pu-l! ;
\ Pursuer_AddBiteDamage: Hewie's bite by where it came (his table +0x173C: in front / behind,
\ high / low)
create bite-off  0 , 8 , 4 , 12 ,
: add-bite ( dir -- )
    $173C pu-l@ 0= over 3 > or if  drop exit  then
    cells bite-off + @ $173C pu-l@ + exe-l@ bites + bites! ;
\ PursuerGroup_Find: the exit at which `slot` has any of the flag bits (exit_flags), $FF none
: group-find ( bits slot -- exit | $FF )
    8 0 do  i over group-fields 2 pick and if  2drop i unloop exit  then  loop  2drop $FF ;

\ ---- the struck states ----
: back-to-stand ( -- )   \ (Pursuer_StateHitOver's end)
    p-cond if  0 p-cond!  then
    1 step-next!  $320 vcall play-anim  0 $1761 pu-c!  0 $1624 pu-l!  0 $1628 pu-l!  0 $1784 pu-l!
    1 step-done! ;
: state-hit-over ( -- )
    p-ended? if  back-to-stand exit  then
    0 step-next!  root-move-masked ;
: state-hit-reaction ( -- )   \ down: held there while he's to wait (+0x1664), then up (+1)
    $1624 pu-l@ dup $FF00 and $1800 = if
        $1664 pu-l@ if
            $1628 pu-l@ 1 = if  1- play-anim  2 $1628 pu-l!
            else  p-ended? $1628 pu-l@ 2 = and if  play-anim-blend  0 $1628 pu-l!  else  drop  then  then
            root-move-masked exit
        then
        1+ play-anim  0 $1628 pu-l!  0 $1784 pu-l!  ['] state-hit-over behave exit
    then  drop
    p-anim $1624 pu-l!  0 $1628 pu-l!  0 step-next! ;
: hit-react ( -- )
    p-cond 2 <>  p-anim $1709 = and if  $1806  else  p-anim  then  $1624 pu-l!
    0 $1628 pu-l!  0 step-next!
    $1624 pu-l@ dup $1807 <> swap $1803 <> and if  ['] state-hit-reaction behave  state-hit-reaction
    else  ['] state-hit-over behave  state-hit-over  then ;
: state-down-and-up ( -- )
    p-ended? 0= if
        p-anim $1709 = if
            $20 me group-find $FF <> p-hp 0<= and if  1 p-hp!  0 p-cond!  0 $1664 pu-l!  then
        then
        root-move-masked exit
    then
    p-cond 2 <> p-anim $1709 = and if  0 $1784 pu-l!  hit-react exit  then
    p-anim case
        $1709 of  $1806 play-anim  endof   $1804 of  $1806 play-anim  endof
        $1800 of  $1802 play-anim  endof
    endcase
    0 $1784 pu-l!  ['] hit-react behave ;
\ Npc_WalkableAhead: floor his height ahead
create wa-at 12 allot
: walkable-ahead? ( -- flag )
    wa-at p-pos p-yaw p-char char.height sf@ vec-ahead
    me wa-at p-mask c-tri-to 0< 0= ;
\ Pursuer_StateKnockedDown (action 0x1F): down (0x1804 forward when there's floor, else 0x1800;
\ a fall 0x1709 blended); out of health: down for good a while (+0x2CC), counted (+0xFBC)
variable knockouts
: state-knocked-down ( -- )
    0 $16EC pu-c!  0 step-next!
    p-104 $1709 = if  $1709 play-anim-blend
    else walkable-ahead? if  $1804  else  $1800  then  play-anim  then
    0 p-104!
    p-hp 0<= if
        $2CC vcall  0 $1790 pu-l!  0 $16F5 pu-c!  2 p-cond!
        0 $16C9 pu-c!  0 $16CB pu-c!  0 $16CA pu-c!  0 $16CC pu-c!  -1 $179C pu-l!
        0 $16F1 pu-c!  0 $16F3 pu-c!  0 $16F2 pu-c!  0 $16F4 pu-c!
        0 $31C vcall  knockouts @ 1+ 9999 min knockouts !
    then
    0 $1628 pu-l!  0 $1544 pu-c!  0 $1545 pu-c!  $104 vcall  0 $1784 pu-l!
    ['] state-down-and-up behave  state-down-and-up ;
' state-knocked-down is st.Pursuer_StateKnockedDown
\ Pursuer_StateFlinch (action 0x1E): by the side and height of the blow (+0x104); at an exit's
\ door (0x20) the short ones
: state-flinch ( -- )
    0 $16EC pu-c!  0 step-next!
    $20 me group-find $FF <> if  p-104 2 and if  $1005 $D  else  $1002 $C  then
    else
        p-104 case
            0 of  $1001 $E  endof  1 of  $1000 $E  endof  2 of  $1004 $F  endof  3 of  $1003 $F  endof
            >r $1002 $C r>
        endcase
    then
    p-sub!  play-anim  $104 vcall  0 $1784 pu-l!
    ['] state-end-step behave
    p-ended? if  1 step-done!  1 step-next!  else  root-move-masked  then ;
' state-flinch is st.Pursuer_StateFlinch
: state-hurt ( -- )
    0 $16EC pu-c!  0 step-next!  0 $1784 pu-l!
    p-anim dup $1804 <> over $1800 <> and swap $1709 <> and if  ['] hit-react behave  hit-react
    else  ['] state-down-and-up behave  state-down-and-up  then ;
' state-hurt is st.Pursuer_StateHurt

\ ---- Pursuer_Hit ----
create hit-at 12 allot
: hit-now ( -- )
    p-sub $A = 1 p-req 5 <> and if  $1628 pu-l@ 0= if  1 $1628 pu-l!  then  clear-message exit  then
    1 p-req 6 = if   \ (Hewie snapping at him)
        d-hewie 100e f<  d-hewie f0< 0= and  p-mode 4 <> and  p-mode 7 <> and  mode 0<> and  mode 4 <> and if
            $173C pu-l@ ?dup if  9 state-flag? $A state-flag? or if  $14  else  $10  then  + exe-l@ bites + bites!  then
            $16DC pu-l@ bites u<  $E state-flag? 0= and  $16C9 pu-c@ 3 < and  p-target dog <> and  p-mode 0= and if
                [: $270 vcall ;] behaviour!
            then
        then  clear-message exit
    then
    3 p-req $94 vcall drop
    $16C4 pu-l@ 3 p-req + 999 min $16C4 pu-l!
    1 p-req 3 = if  p-hp $94 vcall drop  then
    2 p-req $FF <> if  hit-at 2 p-req c-pos vec-copy  else  hit-at p-pos vec-copy  then
    me hit-at c-heading-to p-yaw f- angle-wrap fabs 1.5707964e f< if  0  else  1  then
    1 p-req dup 2 = swap 4 = or if  2 or  then                   ( dir )
    2 p-req 1 =  1 p-req dup $A <> swap $B <> and and if  dup add-bite  then
    p-anim $FF00 and $700 = if  drop p-hp 0<= if  1 p-hp!  then  clear-message exit  then
    p-sub 9 = if
        drop  1 p-req $B =  4 p-req $8000 and and if  1 p-cond!  900 $1790 pu-l!  1 $16F5 pu-c!  then
        2 p-req 1 = if  $1D  else  $1C  then  7 0 0 p-sound  clear-message exit
    then
    $16F7 pu-c@ 1 =  1 p-req 1 = and  p-hp 0> and if  drop $1C 7 0 0 p-sound  clear-message exit  then
    2 p-req $FF <> if  $1761 pu-c@ 1 2 p-req lshift or $1761 pu-c!  then
    1 p-req 5 = if
        drop  p-sub $A = if  $22 $114 vcall  else  [: $29C vcall ;] behaviour!  $21 $118 vcall  then
        p-cond 2 <> if  0 mode!  $2BC vcall  6 $16C9 pu-c!  7 $16CA pu-c!  then
        4 p-req p-who!  clear-message exit
    then
    p-mode 2 = if  $FF leave-door  1 step-next!  then
    1 p-req $A = if  drop [: $29C vcall ;] behaviour!  $23 $118 vcall  4 p-req p-104!  clear-message exit  then
    p-hp 0<= if
        $20 me group-find $FF <> if  1 p-hp!
        else  [: $29C vcall ;] behaviour!  $1F $118 vcall  then
    then
    1 p-req 3 =  p-hp 0> and  p-mode 4 <> and if  [: $29C vcall ;] behaviour!  $1E $118 vcall  2 p-104!  then
    p-mode 4 = if  drop clear-message exit  then
    [: $29C vcall ;] behaviour!
    2 p-req 0= if  0 $178C pu-l!  0 $1760 pu-c!  p-target dog = if  chance-roll drop  then  then
    4 p-req $8000 and if  1 p-cond!  900 $1790 pu-l!  1 $16F5 pu-c!  then
    1 p-req case
        7 of  drop  p-target dog = if  $1761 pu-c@ 1 or $1761 pu-c!  chance-roll drop  then
              4 p-req $7FFF and p-104!  $24 $118 vcall  endof
        1 of  p-104!  $1E $118 vcall  endof   2 of  p-104!  $1E $118 vcall  endof
        4 of  p-104!  $1E $118 vcall  endof
        >r drop r>
    endcase
    clear-message ;
' hit-now is hit

\ ---- Pursuer_BehaviourEnded (vtable +0x29C): the action asked for, then his frame update ----
: behaviour-ended ( -- )
    $1758 pu-l@ dup -1 <> over -2 <> and if  $114 vcall  else  drop  then
    0 $16F6 pu-c!  0 $16ED pu-c!  0 step-done!  0 $16EF pu-c!  -1 $1758 pu-l!  0 $1780 pu-l!
    0 $178C pu-l!  0 p-freeze!  p-actor dup 0< 0= if  actor act.mflags dup l@ $40 invert and swap l!  else  drop  then
    0 $16F8 pu-c!  0 $16F7 pu-c!
    [: $2A0 vcall ;] behaviour!  $2A0 vcall ;
' behaviour-ended $29C vt!
\ Pursuer_StanceByFiona: in her room, he sees her: after her; not on her side of the room:
\ heading for it
: stance-by-fiona ( -- )
    countdown? if  mode 4 =  p-room her c-room <> or if  exit  then
    else  mode 4 =  in-played-room? 0= or if  exit  then  then
    p-yaw head-yaw f@ f+ angle-wrap $1574 pu-f!
    $C0 vcall 1 and dup $1544 pu-c!
    1 = if  $CC vcall drop  0 mode!  $2BC vcall  then
    mode 0= if  side-behind her-side-behind <> if  1 mode!  $B0 vcall  then  then ;
\ Pursuer_FrameUpdate (vtable +0x2A0): his state; the step over - he gives up a while
\ (+0x11C: held off, mode 4, his cry 0x20); Hewie hurt him enough: after Hewie; else his
\ stance and next step; his growl after a stumble (0x1F). At a strike key, struck by her
\ (+0x1761 1) in his flinch: his table for her place (8 in front / 9 behind)
: frame-update ( -- )
    p-cond 2 = if  0 $1544 pu-c!  0 $1545 pu-c!  0 $1546 pu-c!  then
    p-state run
    step-done? if
        0 p-2a!  0 p-2b!  p-hp 0<= if  p-hp-max p-hp!  then
        $11C vcall if
            0 $16C4 pu-l!  $20 7 0 0 p-sound  $1384 pu-l@ $1388 pu-l!
            [: $2AC vcall ;] behaviour!  $25 $118 vcall  4 mode!  0 $2C8 vcall
        else $16DC pu-l@ bites u<  $E state-flag? 0= and  mode 4 <> and
            $1761 pu-c@ 2 and  p-target dog = or and if
            [: $270 vcall ;] behaviour!  1 $16F8 pu-c!
        else  stance-by-fiona  $13C vcall  then then
        0 step-done!  0 $1761 pu-c!
        $16F5 pu-c@  mode 4 <> and if  $1F 7 0 0 p-sound  then
        0 $16F5 pu-c!  mode 4 <> if  1 $16F6 pu-c!  then
        exit
    then
    p-events 2 and if
        may-go?  $1761 pu-c@ 1 and and  p-target her = and  $175C pu-l@ $1E = and  mode 4 <> and if
            me p-target c-pos c-heading-to p-yaw f- angle-wrap fabs 1.5707964e f< if  8  else  9  then
            $130 vcall  0 $1761 pu-c!  pick-from-table
        then
    then ;
' frame-update $2A0 vt!
\ Debilitas_GivesUp (vtable +0x11C): hit 20 times (+0x16C4), not held off, at no exit's door
\ (0x20... PursuerGroup_Find 1), and a place to go (+0xE8) - else his route back
: gives-up? ( -- flag )
    $16C4 pu-l@ 20 < if  false exit  then
    mode 4 = if  false exit  then
    1 me group-find $FF <> if  false exit  then
    $E8 vcall 1 and if  true exit  then
    $1594 pu-l@ char-route drop  false ;
' gives-up? $11C vt!

\ ---- Pursuer_EventConcerns (vtable +0x68): the requests he takes ----
:noname ( cs kind asker b -- flag )
    >r >r nip r> r>                                        ( kind asker b )
    2 pick 5 = if  nip has-door? 0= if  drop false exit  then
    else  drop dup c-ok? 0= if  2drop false exit  then
        dup c-active? 0= swap character char.disabled sl@ 0<> and if  drop false exit  then
    then
    case
        1 of true endof  2 of true endof  3 of true endof  4 of true endof  5 of true endof
        7 of true endof  8 of true endof  11 of true endof
        6 of  p-mode 4 <>  endof
        9 of  p-sub 9 <> p-sub $A <> and p-mode 3 <> and  endof
        10 of  p-sub 9 <> p-sub $A <> and p-mode 3 <> and  endof
        >r false r>
    endcase ;  me accepts!
