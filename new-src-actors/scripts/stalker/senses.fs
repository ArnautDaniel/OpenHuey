\ stalker/senses.fs - what a stalker knows of Fiona and Hewie (src/game/pursuer.c: Npc_Senses,
\ Npc_WhoAround, Npc_SeesChar, NPC_FionaInReach, NPC_HewieInReach, NPC_HearNoise), in the room
\ being played only. Each frame: how far one of them is on foot (Fiona and Hewie in turn), whom
\ he sees or can reach, whether he heard a noise (the acoustics actor tells him).
IN: stalker.senses
USING: engine game-state actors messages common facts flag-names paths stalker.state ;

: my-at ( -- v )  self body-at ;
: fiona ( -- id )  fiona-id @ ;
: hewie ( -- id )  hewie-id @ ;
: here? ( id -- flag )   \ in the game and in my room
    dup 0< if  drop false exit  then  dup body? 0= if  drop false exit  then  body-room self body-room = ;

\ ---- on foot (Npc_DistanceTo): straight there if nothing's in the way, else along a way
\ planned over the floor; -1 no way ----
: foot-dist ( id -- ) ( F: -- d )
    dup here? 0= if  drop -1e exit  then
    >r  self body-tri my-at r@ body-at self body-mask v-walk r@ body-tri = if
        my-at r@ body-at vec-dist  r> drop exit
    then
    probe-path self body-tri my-at r@ body-tri r@ body-at self body-mask path-plan 0> if
        probe-path my-at path-rest
    else  -1e  then  r> drop ;

\ ---- sight: within his range and half an angle either side of where he looks, and nothing
\ in the way - to its middle, or to one of 9 points round its far side (its radius out, 22.5
\ degrees apart). Walls are the floor's triangles flagged $40080 ($40088 while she's hidden) ----
: sight-mask ( -- mask )  fiona-hidden state-flag? fiona-half-hidden state-flag? or if  $40088  else  $40080  then ;
: in-view? ( v -- flag )   \ (Eye_CanSee)
    my-at over vec-dist-xz f0= if  drop false exit  then
    my-at over vec-dist view-range f@ f> if  drop false exit  then
    my-at swap vec-heading view-heading f@ f- angle-wrap fabs view-half f@ f<= ;
create round-pt 12 allot  fvariable round-a  fvariable round-r  variable sc-who  variable sc-t
: sees? ( id -- flag )   \ (Npc_SeesChar)
    dup here? 0= if  drop false exit  then  sc-who !
    sc-who @ body-at in-view? 0= if  false exit  then
    self body-tri my-at sc-who @ body-at sight-mask v-walk  sc-who @ body-tri = if  true exit  then
    my-at sc-who @ body-at vec-heading pi f2/ f+ round-a f!
    sc-who @ body-dims fdrop round-r f!
    9 0 do
        round-a f@ round-r f@ round-pt sc-who @ body-at vec-ahead
        round-pt 0 v-tri-in dup 0< if  drop  else
            sc-t !
            sc-who @ body-tri sc-who @ body-at round-pt sight-mask v-walk 0<      \ (round a wall from it)
            self body-tri my-at round-pt sight-mask v-walk sc-t @ =  and          \ (and in his sight)
            if  true unloop exit  then
        then
        round-a f@ 0.3926991e f+ round-a f!
    loop  false ;
: near-straight? ( id F: d reach -- flag )   \ (within reach on foot, and straight there)
    f< 0= if  drop false exit  then
    >r  self body-tri my-at r@ body-at $40080 v-walk r> body-tri = ;
: fiona-in-reach? ( -- flag )   \ NPC_FionaInReach (her hiding place: with the hiding, F5)
    fiona sees? if  true exit  then
    fiona-hidden state-flag? if  false exit  then
    d-fiona f@ f0> 0= if  false exit  then
    fiona  d-fiona f@  fiona-half-hidden state-flag? if  10e  else  20e  then  near-straight? ;
: hewie-in-reach? ( -- flag )   \ NPC_HewieInReach
    hewie sees? if  true exit  then
    hewie-hidden state-flag? if  false exit  then
    d-hewie f@ f0> 0= if  false exit  then
    hewie  d-hewie f@ 20e near-straight? ;

\ ---- each frame, in the room being played (Npc_Senses' sense mode 0: one of them measured a
\ frame, in turn; then Npc_WhoAround) ----
: senses ( -- )
    self body-room room-id <> if
        -1e d-fiona f!  -1e d-hewie f!  0 sees-fiona !  0 sees-hewie !  exit
    then
    sense-turn @ if  fiona foot-dist d-fiona f!  else  hewie foot-dist d-hewie f!  then
    sense-turn @ 0= sense-turn !
    self body-yaw view-heading f!          \ (his head's turn added: with his head, later)
    fiona here? if  fiona-in-reach?  else  false  then  sees-fiona !
    hewie here? if  hewie-in-reach?  else  false  then  sees-hewie ! ;
\ a noise heard this frame (the acoustics actor: its heard / heard-nothing) - NPC_HearNoise
: hear ( loud room tri -- )  heard-tri !  heard-room !  heard-loud !  -1 did-hear ! ;
: heard-none ( -- )  0 did-hear ! ;
