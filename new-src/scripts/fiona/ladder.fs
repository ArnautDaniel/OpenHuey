\ fiona/ladder.fs - ladders (src/game/fiona.c Fiona_StateLadder*, Fiona_StateOnLadder): the nav
\ mesh's links between two levels (nav-link-*: the original's nav door regions), who is at each
\ (Progress_WhoIsWhere's +0x1000 bytes), her request to climb (Progress_CharRequests: 3, the
\ side, the link) and climbing: walking to its foot or top, on it rung by rung by the stick (W /
\ S), off at either end.
IN: fiona.ladder
USING: engine game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain fiona.commands fiona.doors fiona.panic ;

\ ---- who is at each link (progress +0x1000 + link x 4: [0] on it, [1] / [2] at side 1 / 0,
\ [3] at it; a bit a slot) ----
create link-who 5 4 * allot  link-who 20 0 fill
: lw# ( link i -- addr )  swap 4 * + link-who + ;
: on-ladder ( link -- )  0 lw# dup c@ 1 me lshift or swap c! ;     \ RoomSlots_Enter
: off-ladder ( link -- )  0 lw# dup c@ -2 me lshift and swap c! ;  \ RoomSlots_Leave
: link-bytes ( link slot -- mask )   \ RoomSlots_Bytes: which bytes have the slot
    1 swap lshift >r  0
    4 0 do  over i lw# c@ r@ and if  1 i lshift or  then  loop  nip r> drop ;
: links-who ( -- )
    5 0 do  0 i 1 lw# c!  0 i 2 lw# c!  0 i 3 lw# c!  loop
    nav-links 5 min 0 ?do
        characters 6 min 0 do
            i occupant? if
                i c-pos j nav-link-at? if
                    j 3 lw# dup c@ 1 i lshift or swap c!
                    i c-pos i c-tri j nav-link-side dup 0< 0= if
                        0= if  2  else  1  then  j swap lw# dup c@ 1 i lshift or swap c!
                    else  drop  then
                then
            then
        loop
    loop ;
:noname ( -- )  fiona.doors:who-is-where links-who ; is event-who-is-where

\ ---- Progress_CharRequests' links: no door asked of her, not panicking - alone at a side of a
\ link and facing its way (Actor_FacingDoor): request 3 (the side, the link) ----
: facing? ( link side -- flag )  nav-link-yaw  f-yaw f- fcos f0> ;
: link-request ( -- )
    request @ if  exit  then
    panic @ 4 >= if  exit  then
    nav-links 5 min 0 ?do
        i 2 lw# c@ $FD and 1 = if  i 0 facing? if  3 request !  0 request-arg !  i request-b !  then
        else i 1 lw# c@ $FD and 1 = if  i 1 facing? if  3 request !  1 request-arg !  i request-b !  then
        then then
    loop ;
:noname ( -- )  fiona.doors:exit-request link-request ; is event-exit-request

\ ---- climbing (her +0x100: the link; +0x104: the side she started from) ----
variable lad-link  variable lad-side
\ the ladder's end (0 the foot, 1 the top: nav +0x5C)
: ladder-end-y ( side -- ) ( F: -- y )  lad-link @ swap nav-link-spot fdrop fswap fdrop ;
\ her place in front of a side (Actor_DoorFront): her triangle there
: front ( side -- tri ) ( F: ox oz -- x y z )  lad-link @ swap nav-link-front ;
: let-go ( -- )  lad-link @ off-ladder  0 f-turn-mode !  0 f-2a !  0 f-2b !  to-idle ;   \ door_give_up
\ moved by the root motion, up and down too, off the mesh
: climb-move ( -- )
    root-turn  rm-x f@ rm-z f@ f-yaw rotate-by                        ( F: dx dz )
    f-pos 8 + dup sf@ f+ sf!  f-pos dup sf@ f+ sf!
    f-pos 4 + dup sf@ rm-y f@ f+ sf!  me c-sync ;
\ Fiona_StateLadderOff: at its end off the ladder at the top (0x707) or the foot (0x703),
\ placed in front of it
: st-ladder-off ( -- )
    0 f-cam-on !
    f-end? if
        f-anim@ case
            $707 of  0e -7.1132e 1 front  dup 0< if  drop fdrop fdrop fdrop  else  me c-tri!  f-pos vec!  me c-sync  then  endof
            $703 of  0e -7.1132e 0 front  dup 0< if  drop fdrop fdrop fdrop  else  me c-tri!  f-pos vec!  me c-sync  then  endof
        endcase
        let-go exit
    then
    climb-move ;
\ after a new move her triangle is the one at the end she's nearer
: tri-at-end ( -- )
    f-anim@ dup $703 = swap $704 = or if  0e -7.1132e 0 front  else  0e -7.1132e 1 front  then
    fdrop fdrop fdrop  me c-tri! ;
\ the pursuer at a side of the ladder (his bytes 2 / 1: bit 4 the foot's side, 2 the top's)
: pu-at? ( bit -- flag )
    f-pu-ok @ 1 = if  lad-link @ pursuer-slot @ link-bytes and 0<>  else  drop false  then ;
variable moved  variable idle
: own ( anim -- )  fiona.doors:play-own  1 moved ! ;
\ a rung down from 0x700 / 0x702 cuts in, else blends (up: from 0x704 / 0x706)
: rung-down ( anim new -- )  swap dup $700 = swap $702 = or if  -1 f-play  1 moved !  else  own  then ;
: rung-up ( anim new -- )  swap dup $704 = swap $706 = or if  -1 f-play  1 moved !  else  own  then ;
: goes-down ( anim -- )   \ (the stick's < 0 way): near the foot (18) off, unless the pursuer waits there
    0 ladder-end-y f-pos 4 + sf@ f- 18e f< if
        4 pu-at? if  $708 <> if  $708 own  then
        else  $703 rung-down  ['] st-ladder-off behave  then
    else  $701 rung-down  then ;
: goes-up ( anim -- )     \ near the top (3) off
    f-pos 4 + sf@ 1 ladder-end-y f- 3e f< if  $707 rung-up  ['] st-ladder-off behave
    else  $705 rung-up  then ;
: climb-down ( anim -- )
    dup $700 - case
        0 of  goes-down  endof  2 of  goes-down  endof  6 of  goes-down  endof  8 of  goes-down  endof
        1 of  drop  $702 -1 f-play  1 moved !  endof
        4 of  drop  4 pu-at? if  $708 own  else  $703 own  ['] st-ladder-off behave  then  endof
        5 of  drop  $702 own  endof
        9 of  drop  $702 own  endof
        >r drop r>
    endcase ;
: climb-up ( anim -- )
    dup $700 - case
        0 of  drop  $707 own  ['] st-ladder-off behave  endof
        1 of  drop  $706 own  endof  9 of  drop  $706 own  endof
        2 of  goes-up  endof  4 of  goes-up  endof  6 of  goes-up  endof  8 of  goes-up  endof
        5 of  drop  $706 -1 f-play  1 moved !  endof
        >r drop r>
    endcase ;
\ the pursuer above her on the ladder (his byte 1, within his height): she holds
: pursuer-above? ( -- flag )
    2 pu-at? 0= if  false exit  then
    f-pos 4 + sf@ pursuer-slot @ c-pos 4 + sf@ f-  pursuer-slot @ c-height f< ;
defer fall-off ( -- )  ' noop is fall-off   \ (panicking on it: Fiona_StateCaughtCrawling - with the stalkers)
\ Fiona_StateOnLadder: when a move ends (or holding) the stick picks the next; released, hold
: st-on-ladder ( -- )
    1 f-2a !  0 f-cam-on !
    f-settled? 0= if  exit  then
    f-fear-bits @ 2 and if  fall-off exit  then
    f-anim@ $708 - 2 u<  f-end? or if
        0 moved !  1 idle !
        sz f@ fabs 0.5e f> if
            0 idle !
            sz f@ f0< if  f-anim@ climb-down
            else  pursuer-above? if  1 idle !  else  f-anim@ climb-up  then  then
        then
        idle @ if
            f-anim@ $700 - case
                0 of  $708 own  endof  2 of  $708 own  endof  4 of  $708 own  endof  6 of  $708 own  endof
                1 of  $709 own  endof  5 of  $709 own  endof
            endcase  1 moved !
        then
        moved @ if  tri-at-end  then
    then
    climb-move ;
\ Fiona_StateLadderWalk: walking to it; there, onto it (from side 1 0x700, from side 0 0x704)
: st-ladder-walk ( -- )
    walk-to-spot dup 0< if  drop let-go exit  then
    if  exit  then
    1 f-2a !  7 f-sub!
    lad-side @ if  $700  else  $704  then  fiona.doors:play-own  ['] st-on-ladder behave ;
\ Fiona_StateLadder: its spot (side 1: 6.7 out) and facing walked to
: st-ladder ( -- )
    0e  lad-side @ if  6.7e  else  0e  then  lad-side @ front     ( tri ) ( F: x y z )
    dup 0< if  drop fdrop fdrop fdrop let-go exit  then
    1 f-2b !   \ (+0x2B: through blocked floor - the ladder's foot and the hole's cover are flagged 8)
    lad-link @ lad-side @ nav-link-yaw  ['] st-ladder-walk walk-spot ;
\ Fiona_StateBlock 3: not with the pursuer on her; onto the link's side
: ladder-request ( -- )
    pursuer-slot @ dup 0< 0= if  me swap 0e 0e c-touching? if  exit  then  else  drop  then
    me 2 relations:req-word-of lad-link !  me 1 relations:req-word-of lad-side !
    lad-link @ on-ladder  6 f-sub!  0 f-target-t !  2 f-act !  3 f-mode!
    ['] st-ladder behave ;
:noname ( kind -- )
    dup 3 = if  drop ladder-request exit  then
    2 = if  fiona.doors:door-request-2  then ; is door-request
