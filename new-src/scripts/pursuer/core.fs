\ pursuer/core.fs - the stalker (the original's gCharPursuer: src/game/pursuer.c, the shared
\ Pursuer base of Debilitas, Daniella, Riccardo and Lorenzo): his object, his vtable and his
\ animations. The port keeps the original's shape so the two read side by side:
\   - his own fields are a 0x1800-byte block at the original's offsets: `$16EE pu c@` is
\     PU(p, 0x16EE, u8); the Character's fields others read (mode, sub, room, triangle, place,
\     health, requests) are the scripts' character record (chars.fs, char.*);
\   - his virtual functions are a table of words called by the original's offsets: `$114 vcall`
\     is VCALL(p, 0x114, ...); each kind (Debilitas, ...) fills the table with its own;
\   - a PTMF (a state, the behaviour +0x174C, the off-screen move +0x17A0) is an xt in a
\     variable; one naming a virtual function is a word that calls it.
\ Words keep the original's names in their comments (Pursuer_*, Npc_*).
IN: pursuer.core
USING: engine game-state events.core events.words chars relations ;

2 constant me      \ his character slot (gCharPursuer is slot 2)
0 constant her     \ gCharPlayer
1 constant dog     \ gCharPartner

\ ---- his fields ----
create pu-mem $1800 allot  pu-mem $1800 0 fill
: pu ( off -- addr )  pu-mem + ;
: pu-c@ ( off -- n )  pu c@ ;          : pu-c! ( n off -- )  pu c! ;
: pu-l@ ( off -- n )  pu sl@ ;         : pu-l! ( n off -- )  pu l! ;
: pu-f@ ( off -- ) ( F: -- r )  pu sf@ ;   : pu-f! ( off -- ) ( F: r -- )  pu sf! ;
: pu-w@ ( off -- n )  pu w@ ;          : pu-w! ( n off -- )  pu w! ;
: pu-1+ ( off -- )  pu dup sl@ 1+ swap l! ;
: pu-1- ( off -- )  pu dup sl@ 1- swap l! ;

\ ---- his virtual functions (the vtable: 0x330 bytes) ----
create pvt $330 4 / cells allot
: vt! ( xt off -- )  4 / cells pvt + ! ;
: vcall ( i*x off -- j*x )
    dup 4 / cells pvt + @ ?dup if  nip execute exit  then
    ." pursuer: empty vtable slot $" hex . decimal .s cr ;
\ a PTMF naming a virtual function: `$294 vptmf v-offscreen-step`
: vptmf ( off "name" -- )  create ,  does> @ vcall ;
\ a PTMF slot: the state (a.state), the behaviour (+0x174C), the off-screen move (+0x17A0)
variable p-state    variable p-behave   variable p-move
: run ( slot -- )  @ ?dup if  execute  then ;   \ ptmf_test / ptmf_scall
: behave ( xt -- )  p-state ! ;                 \ Actor_SetState
: behaviour! ( xt -- )  p-behave !  -1 $1758 pu-l! ;   \ ptmf_set +0x174C, +0x1758 -1
: set-move ( xt -- )  p-move ! ;                   \ Pursuer_SetMove (+0x17A0)

\ ---- his Character fields ----
: p-char ( -- addr )  me character ;
: p-mode ( -- n )  p-char char.mode sl@ ;       : p-mode! ( n -- )  p-char char.mode l! ;   \ +0xF8
: p-sub ( -- n )  p-char char.sub sl@ ;         : p-sub! ( n -- )  p-char char.sub l! ;     \ +0xFC
: p-room ( -- n )  p-char char.room sl@ ;       : p-room! ( n -- )  p-char char.room l! ;   \ a.room
: p-tri ( -- tri )  me c-tri ;                  : p-tri! ( tri -- )  me c-tri! ;            \ a.navTri
: p-pos ( -- v )  me c-pos ;
: p-yaw ( F: -- a )  me c-yaw ;                 : p-yaw! ( F: a -- )  me c-yaw! ;          \ a.angle[1]
: p-cond ( -- n )  p-char char.cond sl@ ;       : p-cond! ( n -- )  p-char char.cond l! ;   \ a.unkC4
: p-hp ( -- n )  p-char char.hp sl@ ;           : p-hp! ( n -- )  p-char char.hp l! ;
: p-scripted ( -- n )  p-char char.scripted sl@ ;                                          \ +0xE0
: p-req ( i -- n )  4 * p-char char.req + sl@ ;                                             \ state[i]
: p-req! ( n i -- )  4 * p-char char.req + l! ;
\ the rest of his Character, private: at their offsets in his block
: p-hp-max ( -- n )  $14CC pu-l@ ;   : p-hp-max! ( n -- )  $14CC pu-l! ;   \ (c.hpMax)
: p-who ( -- n )  $100 pu-l@ ;   : p-who! ( n -- )  $100 pu-l! ;       \ +0x100 (a door, a character)
: p-104 ( -- n )  $104 pu-l@ ;   : p-104! ( n -- )  $104 pu-l! ;       \ +0x104[0]
: p-freeze ( -- n )  $14D0 pu-l@ ;   : p-freeze! ( n -- )  $14D0 pu-l! ;   \ +0x14D0
: p-2a ( -- n )  $2A pu-c@ ;   : p-2a! ( n -- )  $2A pu-c! ;
: p-2b ( -- n )  $2B pu-c@ ;   : p-2b! ( n -- )  $2B pu-c! ;
: p-2d ( -- n )  $2D pu-c@ ;   : p-2d! ( n -- )  $2D pu-c! ;
: p-hear ( -- n )  $152A pu-w@ ;   : p-hear! ( n -- )  $152A pu-w! ;   \ (c.hearThreshold)
\ the target (+0x1540): a character slot
: p-target ( -- cs )  $1540 pu-l@ ;   : p-target! ( cs -- )  $1540 pu-l! ;
\ the path (+0x124 its waypoints, +0x128 the one reached): new-src's character paths
: p-path-n ( -- n )  me path-n ;   : p-path-i ( -- n )  me path-i ;
: p-path-left? ( -- flag )  me path-left? ;    \ (unk128 < unk124)
: p-path-end ( -- )  me c-path-end ;           \ (unk124 = unk128)

\ the room being played (gProgress +0xC); him in it (Npc_InPlayedRoom)
\ chance (gRandom +0x1C: 0 <= r < 1)
: rnd01 ( F: -- r )  32768 random s>f 32768e f/ ;
: played ( -- room )  room-id ;
variable hard-mode   \ (progress +0x30 bit 0x8000, the hard setting: not kept by new-src yet)
: in-played-room? ( -- flag )  p-room played = ;
\ progress +0x1FBEC1: the countdown, Hewie under control
: countdown? ( -- flag )  hewie-control @ 0<> ;

\ ---- his model and animations (his motion: +0x55C the animation, +0x550 the fade weight,
\ +0x6A4 +0x18 the key flags) ----
: p-actor ( -- a )  me c-actor ;
: p-anim ( -- id )  p-actor dup 0< if  exit  then  motion@ ;        \ MOTION_ANIM
: p-faded? ( -- flag )  p-actor dup 0< if  drop true exit  then  actor act.fade sf@ 0e f<= ;   \ +0x550 <= 0
: p-keys ( -- bits )  p-actor dup 0< if  drop 0 exit  then  actor act.mflags l@ ;   \ MOTION_KEYS
: p-ended? ( -- flag )  p-keys $20 and 0<> ;    \ MOTION_KEY_END
: p-events ( -- bits )  p-actor dup 0< if  drop 0 exit  then  0 0 1 motion-events ;   \ Motion_EventFlags(m, 0, 0, 1)
: p-events-at ( dt -- bits )  p-actor dup 0< if  2drop 0 exit  then  0 rot 1 motion-events ;
: p-entry ( anim -- blend flags )  p-actor dup 0< if  2drop 0 0 exit  then  swap motion-entry nip ;
variable ps-a  variable ps-b  variable ps-f
: p-start ( anim blend flags -- )
    ps-f !  ps-b !  ps-a !
    p-actor dup 0< if  drop exit  then
    dup ps-a @ has-motion? 0= if  drop exit  then   \ (one his model lacks: nothing)
    dup actor 1e act.rate sf!  ps-a @ ps-b @ ps-f @ motion-play ;
\ Motion_PlayTable: the table's fade and flags; Motion_Play: its flags, cut in
: play-table ( anim -- )  dup p-entry p-start ;
: play ( anim -- )  dup p-entry nip 0 swap p-start ;
\ Pursuer_PlayAnim: restarted (Motion_PlayTable)
: play-anim ( anim -- )  play-table ;
\ Pursuer_StartAnim / Pursuer_PlayAnimIf: unless it is playing (and not at its end, or held at
\ it - table flag 4); `cut` plays it cut in. 1 if (re)started
: start-anim? ( anim cut -- flag )
    over p-anim = if
        drop  p-faded? 0= if  drop false exit  then
        p-ended? 0= if  dup p-entry nip 4 and if  drop false exit  then  then
        play-table true exit
    then
    if  play  else  play-table  then  true ;
: play-anim-if ( anim blend -- flag )  start-anim? ;
\ Pursuer_PlayAnimBlend: restarted if it's the current one, else cut in
: play-anim-blend ( anim -- )  dup p-anim = if  play-anim  else  play  then ;

\ ---- his sounds (Pursuer_Sound: not during an event's own sound, progress flag 8) ----
: p-sound ( id bank vol pitch -- )
    8 state-flag? if  2drop 2drop exit  then
    >r >r >r >r  me r> r> r> r>  p-pos vec@ actor-sound ;

\ ---- the actions (vtable +0x114, Pursuer_StartAction: kPursuerSteps or his own +0x1714) ----
\ an action: its state, id (+0x175C), move mode and sub, sense mode (+0x15C0), look (+0x1710)
6 cells constant /action
create actions  $2A /action * allot   \ (pursuer.steps fills it)
variable own-steps   \ +0x1714: his own table (0x1000 + i)
: action# ( kind -- addr )
    dup $1000 and if  $FFF and /action * own-steps @ +  else  /action * actions +  then ;
: p-start-action ( kind -- )
    action# >r  r@ @ behave  r@ cell+ @ $175C pu-l!  r@ 2 cells + @ p-mode!  r@ 3 cells + @ p-sub!
    r@ 4 cells + @ $15C0 pu-c!  r> 5 cells + @ $1710 pu-c!  1 $15A0 pu-c! ;
: p-p-start-action-next ( kind -- )   \ Pursuer_StartActionNext: its id, mode and sub only
    dup $1758 pu-l!  action# >r  r@ cell+ @ $175C pu-l!  r@ 2 cells + @ p-mode!  r> 3 cells + @ p-sub! ;


\ the behaviour step's flags
: step-done? ( -- flag )  $16EE pu-c@ 1 = ;   : step-done! ( n -- )  $16EE pu-c! ;   \ PURSUER_STEP_DONE
: step-next? ( -- flag )  $16F0 pu-c@ 1 = ;   : step-next! ( n -- )  $16F0 pu-c! ;   \ PURSUER_STEP_NEXT

\ ---- what isn't ported yet: says so once on the console ----
create told 64 32 * allot  variable ntold   \ (the names told: 31 characters each)
: told# ( i -- addr )  32 * told + ;
: told$ ( i -- addr len )  told# dup 1+ swap c@ ;
: (not-yet) ( addr len -- )
    31 min
    ntold @ 0 ?do  i told$ 2over compare 0= if  2drop unloop exit  then  loop
    ntold @ 64 >= if  2drop exit  then
    dup ntold @ told# c!  ntold @ told# 1+ swap move  1 ntold +!
    ." pursuer: not yet " ntold @ 1- told$ type cr ;
: not-yet" ( "text" -- )  postpone s" postpone (not-yet) ; immediate

\ ---- the game's own tables (his pointer fields hold their addresses in the executable) ----
: exe-l@ ( va -- n )  4 exe-bytes dup if  sl@  then ;
: exe-w@ ( va -- n )  2 exe-bytes dup if  w@  then ;
: exe-sw@ ( va -- n )  exe-w@ dup $8000 and if  $10000 -  then ;
: exe-c@ ( va -- n )  1 exe-bytes dup if  c@  then ;
: exe-f@ ( va -- ) ( F: -- r )  4 exe-bytes dup if  sf@  else  drop 0e  then ;
