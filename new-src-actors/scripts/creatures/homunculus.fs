\ creatures/homunculus.fs - a homunculus (src/game/creature.c CreatureB_*; docs/subsystems/
\ creatures.md): a small walking figure (O_HMB/HMB_000) that rests until it notices Fiona (or
\ Hewie), then watches her, comes after her, circles her when she can't be reached, grabs her
\ when it gets close; struck it reels and gets up - with no strength left, or its time out, it
\ leaves. It moves by its animations' root motion.
IN: creatures.homunculus
USING: engine game-state actors messages common facts flag-names paths creatures.trip creatures.her ;

state: homunculus-state
  cell field slot                 \ 7..9
  cell field kind  cell field sight  cell field level   \ kind $80..; sight: how near she must come
  /trip field trip                \ where it is, and its way to her room
  cell field model  cell field anim
  cell field strength  cell field time    \ its time awake (9000 frames: it leaves)
  cell field awake                \ (+0x6F) it has noticed her
  cell field want  cell field running     \ its state (+0x69 / +0x62)
  cell field step  cell field sub  cell field frames  cell field wait   \ within a state
  1 floats field fall  1 floats field fall-v   \ dropped from a height: how high still, how fast
  cell field grabbed              \ (+0x6B $80) it has held her: never again; leaving
  cell field held-off             \ (+0x6B $40) it held off from her once
  cell field gave-up  cell field tries
  cell field move-mode            \ (+0xF8) 0, 2 about her, 4 struck or going, 8 to grab her
  cell field untouchable          \ (+0x2D) out of reach of blows
  12 field target  cell field target-tri  1 floats field target-d   \ a spot by her to make for
  cell field grab-spot            \ which of the four spots round her next
  1 floats field her-dist  1 floats field hewie-dist
  cell field her-mode  cell field her-sub  cell field her-doing  cell field her-unseen
  cell field blow-n  cell field blow-damage  cell field blow-pending
  cell field fade
  cell field left-down  cell field right-down   \ its feet down last frame (+0x72 / +0x73)
  cell field stalker  cell field stalker-age   \ the stalker as he tells (stalker-doing), frames since
  cell field shown
  /path field my-path
end-state

: my-room ( -- a )  trip t.room ;
: my-tri ( -- a )  trip t.tri ;
: in-play? ( -- flag )  trip in-play? ;
: my-at ( -- v )  self body-at ;
: walks-anywhere? ( -- flag )  kind @ $80 xor dup 8 = swap 9 = or ;   \ (kinds $88 / $89)
: path-mask ( -- m )  walks-anywhere? if  $40080  else  $04020028  then ;
: stand-mask ( -- m )  walks-anywhere? if  0  else  $04020028  then ;
: full-strength ( -- n )  kind @ $80 xor case  1 of 6 endof  2 of 7 endof  >r 5 r>  endcase ;   \ (D_0042C460)
9000 constant time-out

\ ---- its model and motion ----
: actor@ ( -- a )  model @ actor ;
: play-now ( anim -- )  dup anim !  model @ swap  2dup motion-entry nip  motion-play ;
: play ( anim -- )  dup anim @ = if  drop exit  then  play-now ;
: ended? ( -- flag )  actor@ act.mflags l@ $20 and 0<> ;
: settled? ( -- flag )  actor@ act.fade sf@ 0e f<= ;
: sound ( id -- )  in-play? 0= if  drop exit  then  >r  my-at vec@  r> 5 bank-sound-at ;
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )  model @ root-delta  rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;
: root-move ( -- )   \ turned and moved by its animation
    root@  self body-yaw rm-turn f@ f+ angle-wrap body-turn  rm-x f@ rm-z f@ body-move-local ;
: stride ( F: -- d )  root@ rm-z f@ 0e fmax ;   \ (forward only)

\ ---- turning (CreatureB_TurnToward: 14 degrees x sin 34 a frame, not while its motion fades in) ----
: heading-to ( v -- ) ( F: -- a )  self swap body-heading-to ;
: turn-left ( v -- ) ( F: -- a )  heading-to self body-yaw f- angle-wrap fabs ;
: turn-to ( v -- )  settled? 0= if  drop exit  then  heading-to 7.83e deg>rad body-turn-toward fdrop ;
: faces? ( v deg -- flag )  s>f deg>rad  turn-left f> ;   \ ( within `deg` of it )

\ ---- its states: entered afresh (cr19_enter: settled, a wait of 150 / 300 / 450) ----
: settle ( -- )
    0 step !  0 sub !  0 frames !  my-path path-clear
    16 random 3 mod 1+ 150 * wait !  -1 target-tri ! ;
: become ( st -- )  want !  0 move-mode !  settle ;
: become-move ( st move -- )  move-mode !  want !  settle ;
: first ( anim -- )  step @ if  drop exit  then  play-now  1 step ! ;
: coin ( a b -- n )  2 random if  nip  else  drop  then ;

\ ---- seeing her ----
: hidden-or-busy? ( -- flag )  hidden? her-doing @ 0<> or ;
: reaches? ( v tri -- flag )   \ straight from it onto that triangle
    >r  self body-tri my-at rot path-mask v-walk  r> = ;
: sees-her? ( -- flag )  her-at her-tri reaches? ;   \ (Actor_CanWalkBetween)
: divided? ( -- flag )  her-tri self body-tri across-divider? ;

: x-z-of ( v -- ) ( F: -- x z )  dup sf@  8 + sf@ ;

\ ---- along a path (Character_FollowWaypointsBlocked): a stride on, turned to it ----
: plan ( tri v -- ok? )  >r >r  my-path self body-tri my-at r> r> path-mask path-plan 0> ;
: walk-path ( -- going? )
    my-path path-left? 0= if  false exit  then
    stride fdup f0= if  fdrop true exit  then
    my-path my-at path-ahead  dup my-path path-i!  drop                   \ (the point a stride on: `ahead`)
    ahead turn-to  ahead x-z-of stride body-advance drop
    my-path path-left? ;

\ ---- straight at her (CreatureB_StraightAt): when it can walk straight onto her triangle; on
\ another level it gives up after 30 tries close by ----
: another-level? ( -- flag )  my-at 4 + sf@  her-at 4 + sf@  f= 0= ;
: straight-at-her ( -- tri | -1 )
    sees-her? 0= if  -1 exit  then
    my-path path-clear
    another-level? if
        her-dist f@ 2e f< if  1 tries +!  then
        tries @ 30 >=  self body-tri her-tri <> and if  0 tries !  true gave-up !  -1 exit  then
    then
    her-dist f@ 2e f> if  her-at x-z-of stride body-advance drop  then
    her-tri ;

\ ---- approaching a spot by her (CreatureB_Approach, _TargetNearFiona) ----
fvariable try-turn
: turn-of ( k -- ) ( F: -- a )  dup 8 > if  8 - negate  then  s>f 0.3925e f* ;   \ (D_0042C650)
: spot-try ( i -- found? )   \ before or behind her, turned by the last try's turn, reachable
    2 mod if  -1e  else  1e  then  target-d f@ 16 random s>f f+ f*  0e fswap try-turn f@ rotate-by   ( F: dx dz )
    her-at 8 + sf@ f+  target 8 + sf!  her-at sf@ f+  target sf!  her-at 4 + sf@ target 4 + sf!
    16 random turn-of try-turn f!
    target v-tri dup target-tri !
    dup 0< if  drop false exit  then
    self body-tri my-at target path-mask v-walk 0< 0=  swap self body-tri <> and ;
: find-spot ( -- found? )  0e try-turn f!  17 0 do  i spot-try if  true unloop exit  then  loop  -1 target-tri !  false ;
: touching-her? ( -- flag )  my-at her-at vec-dist-xz  self body-dims fdrop  her body-dims fdrop f+  f< ;
: may-grab? ( -- flag )  running @ 3 <>  grabbed @ 0= and  her-mode @ 8 <> and ;
: approach ( F: d -- )
    sub @ case
        0 of  target-d f!  1 sub !  my-path path-clear  endof
        1 of  fdrop  2 sub !
              my-path path-left? if  exit  then
              find-spot 0= if  1 sub !  target-d f@ 1e f+ target-d f!  then  endof
        2 of  fdrop  target 30 faces? if  3 sub !  else  target turn-to  then  endof
        3 of  fdrop
              target-tri @ 0< if  1 sub !  exit  then
              her-unseen @ touching-her? and if   \ (she can't be seen: touching her, it grabs)
                  my-path path-clear  may-grab? if  3 become  exit  then
              then
              my-path path-left? 0= if  target-tri @ target plan 0= if  1 sub !  exit  then  then
              walk-path 0= if  4 sub !  then  endof
        4 of  fdrop
              her-at 30 faces? 0= if  her-at turn-to  exit  then
              ended? if  0 1 coin 2 become-move  then  endof
        >r fdrop r>
    endcase ;

\ ---- the states ----
: watch ( chatter? other -- )   \ 0 / 1: watching her, deciding now and then
    {: chatter other :}
    root-move  her-at turn-to
    chatter frames @ 44 mod 0= and if  2 sound  then
    1 frames +!
    hidden? if  frames @ 150 mod 0= if  9 2 become-move  then  exit  then
    her-unseen @ if  frames @ 150 mod 0= if  $C 2 become-move  then  exit  then
    sees-her? 0= if  frames @ 150 mod 0= if  $10 become  then  exit  then
    frames @ 90 mod if  exit  then
    held-off @ if  7 other coin  else  5  then  become ;
\ ---- the stalker at a door it stands by (in the exit's area; he at the door on its front -
\ Progress_WhoIsWhere's bit 4 for him): it holds off ($11, CreatureB_StateDoor) till he isn't ----
: stalker-here? ( -- flag )
    stalker-age @ 2 <  stalker @ 0< 0= and if  stalker @ body? if  stalker @ body-room my-room @ =  exit  then  then  false ;
: at-the-door? ( exit -- flag )   \ (the stalker)
    dup stalker @ body-at vec@ door-near?  swap 0 stalker @ body-at vec@ door-in-area? and ;
: stalker-at-my-door? ( -- flag )
    stalker-here? 0= if  false exit  then
    8 0 do  i door-here? if  i my-at by-exit? i at-the-door? and if  true unloop exit  then  then  loop  false ;
: held-by-stalker ( -- )   \ $11
    0 first  stalker-at-my-door? 0= if  5 become  then ;
: after-her ( -- )   \ 5
    $200 first
    hidden? if  9 2 become-move exit  then
    her-unseen @ if  $C 2 become-move exit  then
    hidden-or-busy? half-hidden? 0= and if  $F become exit  then
    stalker-at-my-door? if  $11 become exit  then
    divided? if  $12 become exit  then
    her-at turn-to
    her-mode @ dup 4 = swap 3 = or if  true held-off !  7 become exit  then
    1 frames +!
    gave-up @ if  -1  else  straight-at-her  then  {: t :}
    cutscene-active? in-play? 0= her-doing @ 0<> or and if  9 2 become-move exit  then
    t 0< 0= her-dist f@ 10e f<= and may-grab? and if  t her-tri = if  3 become  then  exit  then
    t 0< if
        my-path path-left? 0= if
            her-tri 0< if  exit  then
            her-tri nav-flags $04020028 and if  $A become exit  then
            her-tri her-at plan 0= if  sees-her? 0= if  $10 become  then  exit  then
        then
        walk-path drop
    then
    her-dist f@ 40e f< her-dist f@ 80e f> or if  exit  then
    frames @ wait @ mod if  exit  then
    0 1 coin 0 become-move ;
: walk-step ( -- )  $200 first ;
: approach-10 ( -- )   \ 7: while she's hidden, held or hiding (her move 4 / 3)
    $200 first  hidden? her-mode @ dup 4 = swap 3 = or or if  10e approach exit  then
    0 held-off !  5 become ;
: hang-back ( -- )  $200 first  hidden? if  20e approach  else  0 become  then ;   \ 9
: off-mesh ( -- )   \ $A: her triangle one it can't stand on
    $200 first  her-tri nav-flags $04020028 and if  5e approach  else  0 become  then ;
: circling ( -- )  $200 first  her-unseen @ if  20e approach  else  5 become  then ;   \ $C
: turn-to-her ( -- )  $200 first  her-at 20 faces? if  5 become  else  her-at turn-to  then ;   \ $D
: its-spot ( -- )   \ $E: to its spot in rooms $8F / $91, then to her
    $200 first
    sub @ if  turn-to-her exit  then
    my-room @ $91 = if  kind @ $80 xor 8 = if  $D0  else  $9F  then  else  $E  then  {: tri :}
    my-path path-left? 0= if  tri dup tri-center target vec!  target plan 0= if  exit  then  then
    walk-path 0= if  1 sub !  then ;
: waiting ( -- )   \ $F: she's hidden or busy
    0 first
    hidden? if  9 2 become-move exit  then
    half-hidden? hidden-or-busy? 0= or if  5 become  then ;
: out-of-sight ( -- )   \ $10
    $200 first
    sees-her? if  0 become exit  then
    divided? if  $12 become exit  then
    10e approach ;
: held-across ( -- )   \ $12: across the divider from her, idling and facing her
    root-move  her-at turn-to  1 frames +!
    divided? 0= if  0 become exit  then
    step @ 0= if  sub @ if  $1C00  else  0  then  play-now  1 step !  then
    frames @ 90 mod 0= if  0 frames !  0 step !  sub @ 0= sub !  then ;

: knocked-down ( -- )   \ $B: dropped from a height (or knocked down), and up again
    step @ case
        0 of  $1001 play
              fall f@ fall-v f@ f-  fall f!   fall-v f@ 0.085e f+ fall-v f!
              fall f@ f0> 0= if  0e fall f!  0e fall-v f!  5 sound  1 step !  then  endof
        1 of  ended? if  1 frames +!  frames @ 32 >= if  2 step !  then  then  endof
        2 of  0 frames !  $1800 play-now  3 step !  endof
        3 of  1 frames +!  frames @ 34 = if  5 sound  then
              root-move  ended? if  0 untouchable !  5 become  then  endof
    endcase ;

: leave ( -- )  4 want !  true grabbed !  4 move-mode !  settle ;
: struck ( -- )   \ 2: a blow - it reels, then is up; no strength left, it leaves
    step @ 1 <> if  root-move  then
    step @ case
        0 of  blow-damage @ negate strength +!
              her-at 90 faces? if  $1001  else  $1000  then  play-now  3 sound  1 step !  endof
        1 of  true untouchable !
              ended? 0= if  root-move exit  then
              strength @ 0> 0= if  leave exit  then
              1 frames +!  frames @ 30 > if  2 step !  then  endof
        2 of  0 frames !  $1800 play-now  3 step !  endof
        3 of  1 frames +!  frames @ 34 = if  5 sound  then
              ended? if  0 untouchable !  $D become  then  endof
    endcase ;

\ grabbing her (3): to each spot round her in turn (D_0042C610: 2 behind, 2 before, 2 right, 2
\ left) until she's held (her move 4, sub $12); then held, chattering; let go, the threat up
\ and a noise; then it leaves
create grab-spots  0 , -2 ,   0 , 2 ,   2 , 0 ,   -2 , 0 ,
: before-her? ( -- flag )   \ on her front side
    her-at my-at vec-heading  her body-yaw f- angle-wrap fabs  1.5707964e f< ;
: spot-of ( i -- ) ( F: -- x z )   \ a grab spot, turned by her heading, from her place
    2 * cells grab-spots +  dup @ s>f  cell+ @ s>f  her body-yaw rotate-by
    her-at 8 + sf@ f+  fswap her-at sf@ f+  fswap ;
: next-grab-spot ( -- )
    grab-spot @ case
        0 of  before-her? if  1  else  0  then  endof
        1 of  before-her? if  0  else  1  then  endof
        2 of  2  endof
        >r 3 r>
    endcase
    spot-of  her-at 4 + sf@ fswap  target 8 + sf!  target 4 + sf!  target sf!
    grab-spot @ 1+ 4 mod grab-spot ! ;
: her-held? ( -- flag )  her-mode @ 4 =  her-sub @ $12 = and ;
: grabbing ( -- )
    step @ case
        0 of
            my-path path-left? 0= if
                next-grab-spot  target v-tri dup target-tri !
                dup 0< if  drop exit  then
                dup nav-flags $04020038 and if  drop exit  then
                target plan 0= if  exit  then
            then
            8 move-mode !
            walk-path if  $C my-at vec@ xyz>cells 0  her send fiona-request exit  then   \ (on its way: her request to be held)
            her-held? 0= if  exit  then
            her-at heading-to body-turn  8 sound  $1900 play-now  1 step !
        endof
        1 of  ended? if  9 sound  4 sound  $1901 play-now  2 step !  0 frames !  then  endof
        2 of
            1 frames +!  frames @ 35 mod 0= if  9 sound  then  frames @ 70 mod 0= if  4 sound  then
            her-held? if  exit  then
            4 move-mode !  $1902 play-now
            75e f>cell s" panic" actor-named send threat
            $80 my-room @ self body-tri -1 world-noise  s" acoustics" actor-named send noise
            3 step !
        endof
        3 of  root-move  ended? if  true untouchable !  leave  then  endof
    endcase ;

: gone ( -- )   \ its model let go, its slot free (the bubbles: `going`'s welling)
    model @ actor-free  -1 model !
    slot @ s" creatures" actor-named send creature-gone  self kill ;

: going ( -- )   \ 4: leaving - $1801, fading, bubbles welling up where it sinks; at its end gone
    step @ 0= if
        $1801 play-now  1 step !  true untouchable !  $FF fade !
        in-play? if  self body-tri my-at vec@ welling  then
    then
    root-move
    in-play? 0= if  gone exit  then
    1 frames +!  frames @ 65 = sub @ 0= and if  1 sub !  $A sound  then
    fade @ 10 < if  0  else  fade @ 2 -  then  fade !
    ended? if  gone  then ;
\ its footsteps (CreatureB_Footsteps): walking ($200) in the room being played, sound 6 as its
\ right foot comes down, 7 its left - not in a scene, nor while the world is held
: footsteps ( -- )
    model @ 1 foot-down?  model @ 0 foot-down?                       ( left right )
    in-play?  cutscene-active? 0= and  world-held state-flag? 0= and  anim @ $200 = and if
        dup right-down @ 0= and if  6 sound  then
        over left-down @ 0= and if  7 sound  then
    then  right-down !  left-down ! ;
: run-state ( -- )
    running @ case
        0 of  0 first  false 1 watch  endof
        1 of  $1C00 first  true 0 watch  endof
        2 of  struck  endof
        3 of  grabbing  endof
        4 of  going  endof
        7 of  approach-10  endof
        9 of  hang-back  endof
        $A of  off-mesh  endof
        $B of  knocked-down  endof
        $C of  circling  endof
        $D of  turn-to-her  endof
        $E of  its-spot  endof
        $F of  waiting  endof
        $10 of  out-of-sight  endof
        $11 of  held-by-stalker  endof
        $12 of  held-across  endof
        >r after-her r>   \ (5; the doors' 6 and 8: trip.fs door-watch)
    endcase ;

\ ---- kept in the progress (CreatureB_Save / _Load) ----
: record ( -- addr )  slot @ /kept-creature *  progress pr.creatures + ;
: keep ( -- )
    in-play? if  self body-tri my-tri !  then
    1 record kc.used l!  kind @ record kc.kind l!  sight @ record kc.reach l!  level @ record kc.level l!
    my-room @ record kc.room l!  my-tri @ record kc.tri l!  time @ record kc.time l!
    0 record kc.out l!  awake @ record kc.come l! ;
: unkeep ( -- )  record /kept-creature 0 fill ;

\ ---- waking (CreatureB_Distances, _Notices): her on a triangle it reaches within its sight,
\ Hewie in its room on his within 30, a blow, or no sight at all ----
: hewie ( -- id )  s" hewie" actor-named ;
: measure ( -- )
    in-play? 0= if  exit  then
    my-at her-at vec-dist-xz her-dist f!
    -1e hewie-dist f!
    hewie dup body? if  dup body-room my-room @ = if  body-at my-at vec-dist-xz hewie-dist f!  exit  then  then  drop ;
: notices? ( hewie? -- flag )
    sees-her? her-dist f@ sight @ s>f f<= and if  drop true exit  then
    if  hewie-dist f@ f0< 0=  hewie-dist f@ 30e f<= and
        hewie body? and if  hewie body-at hewie body-tri reaches? if  true exit  then  then  then
    blow-pending @ if  true exit  then
    sight @ 0= ;
: wake ( -- )
    in-play? 0= awake @ or if  exit  then
    fall f@ f0= if
        true notices? if  1 sound  true awake !  walks-anywhere? if  $E  else  $D  then  become  then
    else
        true untouchable !  false notices? if  true awake !  then   \ (high up: it drops once it notices her)
    then ;

\ ---- each frame (CreatureB_Frame, _States) ----
: on-the-floor ( -- )   \ in the room being played: on its triangle, or somewhere it may stand
    stand-mask body-mask!
    my-tri @ dup 0< if  drop  nav-tris random  then
    dup my-tri !  room-id swap dup tri-center body-place ;
: blow-taken ( -- )   \ (CreatureB_ActionState) a blow: struck
    blow-pending @ 0= if  exit  then  0 blow-pending !
    move-mode @ 4 = if  exit  then
    2 want !  2 running !  4 move-mode !  settle ;
: show ( -- )   \ its model where its body is (raised while it drops), seen in play
    model @ 0< if  exit  then
    in-play? self body? and 0= if  0 actor@ act.visible l!  exit  then
    my-at sf@ actor@ act.x sf!  my-at 4 + sf@ fall f@ f+ actor@ act.y sf!  my-at 8 + sf@ actor@ act.z sf!
    self body-yaw actor@ act.yaw sf!  1 actor@ act.visible l!
    running @ 4 = if  fade @ $80 min s>f 128e f/  else  1e  then  actor@ act.alpha sf!   \ (leaving: fading)
    slot @ my-room @ move-mode @ awake @ 0<> broadcast creature-doing ;
: frame ( -- )
    stalker-age @ 100 < if  1 stalker-age +!  then
    measure  wake
    awake @ 0= if  show keep exit  then
    1 time +!
    in-play? 0= if
        time @ time-out >= if  gone exit  then
        grabbed @ if  gone exit  then
        0.35e 0.35e trip trip-step if  stand-mask body-mask!  my-tri @ 0< if  on-the-floor  then  then
        show keep exit
    then
    another-level? 0= if  0 gave-up !  then
    blow-taken
    trip my-at door-watch if   \ (by a swinging door: standing, its state 8; left shut, through it - 6)
        trip gone-through? if  body-off  then  show keep exit
    then
    want @ running @ <> if  want @ running !  then
    run-state  model @ 0< if  exit  then  footsteps  show keep ;

: start ( -- )   \ its model, on the floor if in the room being played, facing as placed
    s" O_HMB/HMB_000" actor-load dup model !  $42C730 motion-table
    0 actor@ act.visible l!  0 play-now
    3e 6e body-size
    in-play? if  on-the-floor  else  body-off  then
    full-strength strength !  trip trip-reset  settle
    fall f@ f0> if  $B want !  $B running !  then
    keep ;

behaviour homunculus
  on spawned ( -- )
      -1 model !  trip trip-reset  body-off  0e fall f!  0e fall-v f!
      self subscribe tick  self subscribe arrived  self subscribe leaving-room
      self subscribe fiona-doing  self subscribe fiona-unseen  self subscribe fiona-blow
      self subscribe stalker-doing  -1 stalker !  100 stalker-age ! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      2drop 2drop 2drop 2drop drop  sender stalker !  0 stalker-age ! ;
  on homunculus-setup ( slot room level tri kind sight yaw height -- )
      cell>f fall f!  >r  10 * sight !  kind !  my-tri !  level !  my-room !  slot !
      start  in-play? if  r@ s>f deg>rad angle-wrap body-turn  then  r> drop ;
  on homunculus-kept ( slot -- )
      slot !  record kc.kind l@ kind !  record kc.reach l@ sight !  record kc.level l@ level !
      record kc.room l@ my-room !  record kc.tri l@ my-tri !  record kc.time l@ time !
      record kc.come l@ 0<> awake !  start  awake @ if  5 become  then ;
  on tick ( -- )  model @ 0< if  exit  then  frame ;
  on fiona-doing ( mode sub cond cmd doing -- )  her-doing !  drop drop  her-sub !  her-mode ! ;
  on fiona-unseen ( on -- )  her-unseen ! ;
  on fiona-blow ( n damage x y z reach -- )   \ (Fiona_BlowCreatures) once a blow, if it reaches it
      {: n damage x y z reach :}
      n blow-n @ = untouchable @ or  in-play? 0= or if  exit  then
      reach cell>f f0< if
          touching-her? 0= if  exit  then
      else
          x cell>f y cell>f z cell>f target vec!
          target my-at vec-dist-xz  self body-dims fdrop  reach cell>f f+  f< 0= if  exit  then
          y cell>f  my-at 4 + sf@  reach cell>f f-  f> 0= if  exit  then
          y cell>f  my-at 4 + sf@  self body-dims fswap fdrop f+  reach cell>f f+  f< 0= if  exit  then
      then
      n blow-n !  damage blow-damage !  true blow-pending !  sender send blow-taken ;
  on leaving-room ( room exit -- )   \ she leaves: in her room and awake, it follows
      over my-room @ = awake @ and grabbed @ 0= and if  trip swap follow-out drop exit  then  2drop ;
  on arrived ( room exit -- )  2drop  in-play? if  on-the-floor  else  body-off  then ;
  on killed ( -- )  model @ dup 0< if  drop exit  then  actor-free ;
  on set-level ( level -- )  level ! ;   \ (its floor of a split room: a room's command - Room48)
end-behaviour

: homunculus-spawn ( -- id )  homunculus homunculus-state s" homunculus" spawn ;
