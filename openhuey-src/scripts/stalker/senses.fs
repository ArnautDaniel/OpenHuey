\ stalker/senses.fs - what a stalker knows of Fiona and Hewie (src/game/pursuer.c: Npc_Senses,
\ Npc_WhoAround, Npc_SeesChar, NPC_FionaInReach, NPC_HewieInReach, NPC_HearNoise), in the room
\ being played only. Each frame: how far one of them is on foot (Fiona and Hewie in turn), whom
\ he sees or can reach, whether he heard a noise (the acoustics actor tells him).
IN: stalker.senses
USING: engine game-state actors messages common facts flag-names paths vectors stalker.state ;

: my-at ( -- v )  self body-at ;
: fiona ( -- id )  fiona-cached @ ;
: hewie ( -- id )  hewie-cached @ ;
: here? ( id -- flag )   \ in the game and in my room
    dup 0< if  drop false exit  then  dup body? 0= if  drop false exit  then  body-room self body-room = ;
\ walking straight from him to someone over the mask reaches their triangle (one off the floor:
\ never - the walk's -1 is a wall)
: straight? ( id mask -- flag )
    {: who mask :}  who body-tri 0< if  false exit  then
    self body-tri my-at who body-at mask v-walk  who body-tri = ;

\ ---- on foot (Npc_DistanceTo): straight there if nothing's in the way, else along a way
\ planned over the floor; -1 no way ----
: foot-dist ( id -- ) ( F: -- d )
    {: who :}  who here? 0= if  -1e exit  then
    who self body-mask straight? if  my-at who body-at vec-dist exit  then
    probe-path self body-tri my-at who body-tri who body-at self body-mask path-plan 0> if
        probe-path my-at path-rest
    else  -1e  then ;

\ ---- sight: within his range and half an angle either side of where he looks, and nothing
\ in the way - to its middle, or to one of 9 points round its far side (its radius out, 22.5
\ degrees apart). Walls are the floor's triangles flagged so (more of them while she's hidden) ----
$40080 constant sight-walls   $40088 constant hidden-walls
: sight-mask ( -- mask )  fiona-hidden state-flag? fiona-half-hidden state-flag? or if  hidden-walls  else  sight-walls  then ;
: in-view? ( v -- flag )   \ (Eye_CanSee)
    my-at over vec-dist-xz f0= if  drop false exit  then
    my-at over vec-dist view-range f@ f> if  drop false exit  then
    my-at swap vec-heading view-heading f@ f- angle-wrap fabs view-half f@ f<= ;
create round-pt /vec allot
: round-in-view? ( who F: a r -- flag )   \ (that point round them: round a wall from them, in his sight)
    {: who F: a r :}
    a r round-pt who body-at vec-ahead
    round-pt 0 v-tri-in dup 0< if  drop false exit  then  {: t :}
    who body-tri who body-at round-pt sight-mask v-walk 0<
    self body-tri my-at round-pt sight-mask v-walk t =  and ;
: sees? ( id -- flag )   \ (Npc_SeesChar)
    {: who :}  who here? 0= if  false exit  then
    who body-at in-view? 0= if  false exit  then
    who sight-mask straight? if  true exit  then
    my-at who body-at vec-heading pi f2/ f+  who body-dims fdrop  {: F: a r :}
    9 0 do
        who a r round-in-view? if  true unloop exit  then
        a 22.5e deg>rad f+ to a
    loop  false ;
: near-straight? ( id F: d reach -- flag )   \ (within reach on foot, and straight there; not
    f< 0=  noclipped @ or if  drop false exit  then          \ while through blocked floor: +0x2B)
    sight-walls straight? ;
: fiona-in-reach? ( -- flag )   \ NPC_FionaInReach
    her-unseen @ if  false exit  then   \ (camouflaged and still: not there to him)
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
    self body-yaw head-yaw f@ f+ angle-wrap view-heading f!   \ (his head's turn added: Npc_WhoAround; stalker/head.fs)
    fiona here? if  fiona-in-reach?  else  false  then  sees-fiona !
    hewie here? if  hewie-in-reach?  else  false  then  sees-hewie ! ;
\ a noise heard this frame (the acoustics actor: its heard / heard-nothing) - NPC_HearNoise
: hear ( loud room tri source -- )  heard-source !  heard-tri !  heard-room !  heard-loud !  -1 did-hear ! ;
: heard-none ( -- )  0 did-hear ! ;
