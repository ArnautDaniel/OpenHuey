\ pursuer/frame.fs - the stalker's frame (src/game/debilitas_body.inc Debilitas_Update, the
\ shared Stalker_ThinkStart / Stalker_ThinkEnd): his senses and mode first; in the room being
\ played a door in his way, the floor around Fiona, a cry he caused, his behaviour, a growl now
\ and then, the slot commands, his sounds, his height and his floor; elsewhere his behaviour and
\ his steps heard through the walls; then his timers, his model and his footsteps.
IN: pursuer.frame
USING: engine game-state events.core events.words chars relations fiona.doors pursuer.core pursuer.stubs pursuer.npc pursuer.modes ;

' p-start-action $114 vt!   ' p-p-start-action-next $118 vt!

\ ---- held still a moment by a blow (+0x14D0: Motion_Freeze, counted down: Debilitas_StunDown) ----
: mflags! ( set? bits -- )
    p-actor dup 0< if  drop 2drop exit  then  actor act.mflags >r
    swap if  r@ l@ or  else  invert r@ l@ and  then  r> l! ;
: freeze ( -- )  5 p-freeze!  true $40 mflags! ;
: stun-down ( -- )
    p-freeze 0> if  p-freeze 1- p-freeze!  p-freeze 0<= if  false $40 mflags!  then  then ;

\ ---- Pursuer_CryHeard: those who took his blow (the request's +0xC, not yet answered
\ +0x1760): his cry (+0x1764) and a moment held, held off them for a while (+0x178C: his table
\ +0x1748's first word, 90) ----
: cry-heard ( -- mask )
    me ask-accepted $1760 pu-c@ invert and $FF and
    dup if
        $1748 pu-l@ ?dup if  exe-l@  else  90  then  $178C pu-l!
        dup $1760 pu-c@ or $1760 pu-c!
        $1764 pu-l@ dup 0< 0= if  7 0 0 p-sound  freeze  else  drop  then
    then  -1 $1764 pu-l! ;

\ ---- Npc_BoneHeight: his height to his head bone (his model's +0x80), at least 3 ----
variable head-bone   $16 head-bone !
: bone-height ( -- )
    p-actor dup 0< if  drop exit  then  head-bone @ bone-pos fdrop fswap fdrop
    p-pos 4 + sf@ f-  3e fmax  p-char char.height sf! ;

\ ---- Npc_ProbeAroundFiona: the floor 20 from him in 8 ways round toward her (+0x1548) ----
: probe-around-fiona ( -- )
    8 0 do
        me her c-pos c-heading-to  i s>f 6.2831855e f* 8e f/ f+  20e tri-at-direction
        i 4 * $1548 + pu-l!
    loop ;

\ ---- Pursuer_DoorNear: a door in his way (not while already at one: actions 0x22 0x11 0x21
\ 0x10, mode 2): he goes to it - running (0x11), else the door behaviour (0x10); after her
\ (not when down); through it (+0x2B) when it's open and he is on its sides ----
defer door-behaviour   ' noop is door-behaviour   \ (D_003ECB80: with the doors)
: door-near ( -- )
    $175C pu-l@ dup $22 = over $11 = or over $21 = or swap $10 = or  p-mode 2 = or if  exit  then
    door-on-way dup $FF = if  drop exit  then
    dup p-who!
    p-sub $A = if  $11 $114 vcall  else  ['] door-behaviour behaviour!  $10 $118 vcall  then
    p-cond 2 <> if  0 mode!  $2BC vcall  6 $16C9 pu-c!  7 $16CA pu-c!  then
    dup exit-open? swap me group-fields 8 and 0<> and if  1 p-2b!  then ;

\ ---- Actor_TeleportRandom (kind -1): onto a triangle at random he may stand on with both area
\ bits 0x300000 (the room's event areas not checked: not kept yet; 1000 tries) ----
: teleport-random ( -- )
    in-played-room? 0= if  exit  then
    1000 0 do
        nav-tris s>f rnd01 f* f>s
        dup tri-flags p-mask and 0=  over tri-flags $300000 and $300000 = and if
            tri-center me c-place!  unloop exit
        then  drop
    loop ;
\ ---- Pursuer_KeepOnWalkable: off his floor, back onto the nearest he may stand on (none: put
\ anywhere, Actor_TeleportRandom); standing, onto his triangle's plane ----
: keep-on-walkable ( -- )
    p-tri tri-blocked? p-2b 0= and in-played-room? and if
        p-tri tri-if-standable dup p-tri!
        dup 0< 0= if  dup tri-blocked? 0= if  tri-center me c-place!  else  drop  then  else  drop  then
        p-tri 0< if  teleport-random  then
    then ;

\ ---- Pursuer_AnimSounds: his animations' sounds (his table +0x16AC: a pair a motion, by the
\ event keys 1 / 0x10; both for 0x11), bank 7; falling on water: the splash (0x1D, bank 6) ----
: anim-sound# ( anim second? -- id )
    >r  p-actor swap motion-index dup 0< if  r> 2drop -1 exit  then
    2* r> if  1+  then  2* $16AC pu-l@ + exe-sw@ ;
: anim-sound ( anim id -- )
    dup 0< if  2drop exit  then
    over dup $1804 = over $1800 = or swap $1709 = or  over $11 - 2 u< and if
        p-tri tri-flags $2008000 and $2008000 = if  2drop $1D 6 0 0 p-sound exit  then
    then  nip 7 0 0 p-sound ;
: anim-sounds ( anim -- )
    $16AC pu-l@ 0= if  drop exit  then
    dup -1 = if  drop p-anim  then
    dup $8000 and if  drop exit  then
    p-events $11 and case
        $11 of  dup dup 0 anim-sound# anim-sound  dup 1 anim-sound# anim-sound  endof
        1 of  dup 0 anim-sound# anim-sound  endof
        $10 of  dup 1 anim-sound# anim-sound  endof
        >r drop r>
    endcase ;

\ ---- Pursuer_Footsteps (vtable +0x100): as a foot comes down, its floor's sound (bank 7: by
\ the triangle's bits, the feet alternating; water bank 6), louder the faster he goes ----
: contact ( prev? -- l r )
    p-actor swap -4 swap motion-track 0= if  fdrop fdrop fdrop 0 0 exit  then
    fdrop  f0> 1 and  f0> 1 and ;
variable fs-l  variable fs-r  variable fs-step
: step-speed ( F: -- k )   \ (his step against his slow and fast pace: vtable +0x2FC / +0x2F8)
    p-actor dup 0< if  drop 0e exit  then  root-delta fswap fdrop fswap fdrop fswap fdrop
    $2FC vcall f-  $2F8 vcall $2FC vcall f- f/  0e fmax 1e fmin ;
: footsteps ( -- )
    p-char char.disabled sl@ p-tri 0< or if  exit  then
    0 contact fs-r ! fs-l !
    $1788 pu-l@ dup $1600 = swap 0= or if
        p-faded? 0= if  -1 contact fs-r ! fs-l !  else  1 fs-l !  1 fs-r !  then
    then
    0 fs-step !
    fs-l @ 1 = $16A8 pu-c@ 0= and if  1 fs-step !
    else fs-r @ 1 = $16A9 pu-c@ 0= and if  -1 fs-step !  then then
    fs-l @ $16A8 pu-c!  fs-r @ $16A9 pu-c!
    fs-step @ 0= if
        p-faded? if  $1788 pu-l@ dup $1600 = swap 0= or if  0 $16A4 pu-l!  then  then  exit
    then
    p-sub 7 = if  not-yet" Pursuer_Footsteps on the stairs"  then
    7 >r  p-tri tri-flags
    dup $2000000 and if  $8000 and if  $14 r> drop 6 >r  else  8  then
    else  $18000 and case
            $18000 of  6  endof  $10000 of  4  endof  $8000 of  2  endof
            >r 0 r>
        endcase
    then
    $16A4 pu-l@ 1 and +  $16A4 pu-1+
    r> 0  step-speed 2e f* f>s $7F and  p-sound
    step-speed 0.5e f< if  $1C  else  $1F  then  p-room p-tri $FFFF 2 noise-make-in ;   \ (heard: his slot 2)

\ ---- the timers (Stalker_ThinkTimers): frames in his state / behaviour (+0x1784 / +0x1780),
\ the room wait +0x1664 or the route's rest +0x17B4, standing down (+0x1790: up again), held
\ off (+0x178C), the stun, the route growing back (+0x1794) ----
: think-timers ( -- )
    $1784 pu-l@ -1 <> if  $1784 pu-1+  then
    $1780 pu-l@ -1 <> if  $1780 pu-1+  then
    $1664 pu-l@ if  $1664 pu-1-
    else $17B4 pu-l@ if  $17B4 pu-1-  $17B4 pu-l@ 0= if  $1621 pu-c@ $1620 pu-c!  then  then then
    $1790 pu-l@ 0<> p-sub 9 <> and if
        $1790 pu-1-  $1790 pu-l@ 0= if  0 p-cond!  0 $16F5 pu-c!  then
    then
    $178C pu-l@ if  $178C pu-1-  $178C pu-l@ 0= if  0 $1760 pu-c!  then  then
    stun-down
    $1794 pu-l@ if
        $1794 pu-1-  $1794 pu-l@ 0= $1620 pu-c@ $1621 pu-c@ < and if  $1620 pu-c@ 1+ $1621 pu-c!  then
    then ;

\ ---- Pursuer_MotionGroup: the group of his animation (+0x1788); one not known: his stand ----
: motion-group ( -- )
    p-anim dup -1 = if  drop exit  then
    dup 4 u< if  drop 0
    else dup $200 = over $202 = or over $204 = or if  drop $200
    else dup $201 = over $203 = or over $205 = or over $206 = or if  drop $201
    else dup $404 = if  drop $401
    else dup $400 $409 within if  drop $400
    else dup $600 $604 within if  drop $600
    else dup $700 $708 within if  drop $700
    else dup $E00 $E09 within over $2300 $2305 within or if  drop $E00
    else dup $1000 $1007 within if  drop $1000
    else dup $1300 $1308 within if  drop $1300
    else dup $1600 $1603 within if  drop $1600
    else dup $1700 $170A within if  drop $1700
    else dup $1800 $1808 within if  drop $1800
    else dup $1900 $1906 within over $1A00 $1A02 within or if  drop $1900
    else dup $FF invert and dup $8000 = swap $9000 = or if  drop $8000
    else  drop  $320 vcall play-anim  0
    then then then then then then then then then then then then then then then
    $1788 pu-l! ;

\ ---- Stalker_ThinkStart / Stalker_ThinkEnd ----
: think-start ( -- )
    $84 vcall
    p-2b 1 = if  8  else  $A8 vcall  then  me c-mask!
    senses-watching drop
    $16F6 pu-c@ 1 = if  who-around
    else  0 $1544 pu-c!  0 $1545 pu-c!  0 $1546 pu-c!  0 $16CB pu-c!  0 $16CC pu-c!  then
    $120 vcall  motion-group ;
: think-end ( -- )  think-timers  $40 vcall  $100 vcall ;

\ ---- Debilitas_Update (vtable +0x30): his frame ----
variable growl-t   \ (+0x17EC)
: debilitas-update ( -- )
    think-start
    in-played-room? if
        door-near  probe-around-fiona  cry-heard drop
        p-behave run
        $17EC pu-1+  $17EC pu-l@ 90 >= if
            0 $17EC pu-l!
            p-mode 0=  $1788 pu-l@ $1300 <> and  $1788 pu-l@ $1600 <> and if  $2A 7 0 0 p-sound  then
        then
        $110 vcall drop
        p-freeze 0<=  p-freeze 5 = or if  -1 anim-sounds  then
        bone-height  keep-on-walkable
    else
        p-behave run
        not-yet" Pursuer_FootstepsThroughWalls"
    then
    think-end ;
' debilitas-update $30 vt!   ' footsteps $100 vt!
