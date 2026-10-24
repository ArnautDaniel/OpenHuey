\ creatures/homunculus.fs - a homunculus (src/game/creature.c CreatureB_*; docs/subsystems/
\ creatures.md): a small walking figure (O_HMB/HMB_000) that rests until it notices Fiona (or
\ Hewie), then watches her, comes after her, circles her when she can't be reached, grabs her
\ when it gets close; struck it reels and gets up - with no strength left, or its time out, it
\ leaves. It moves by its animations' root motion.
IN: creatures.homunculus
USING: engine game-state actors messages common facts flag-names paths vectors creatures.trip creatures.her floor-flags ;

\ ---- its states (+0x69 the wanted one, +0x62 the one running): watching her, quiet or
\ chattering; reeling from a blow; grabbing her; leaving; after her (the doors' 6 and 8:
\ creatures/trip.fs door-watch); closing in on her while she's hidden or held; hanging back while
\ she's hidden; about her while she's where it can't stand; dropped from a height; circling her
\ while she can't be seen; turning to face her; to its spot; biding while she's hidden or busy;
\ looking for her out of its sight; held off by the stalker at its door; across the divider ----
enum: hstate#  watching chattering reeling grabbing leaving chasing 7 closing-in 9 hanging-back
    her-off-floor dropped circling facing-her to-its-spot biding out-of-sight stalker-at-door
    across-divider ;

\ ---- its animations (O_HMB/HMB_000's motion table) ----
0 constant rest-anim   $1C00 constant chatter-anim   $200 constant walk-anim
$1000 constant reel-forward-anim   $1001 constant reel-back-anim   \ (reel-back also its fall)
$1800 constant get-up-anim   $1801 constant sink-anim
$1900 constant grab-anim   $1901 constant holding-anim   $1902 constant let-go-anim

\ ---- its sounds (bank 5) ----
1 constant wake-cry   2 constant chatter   3 constant hurt-cry   4 constant giggle   5 constant thud
6 constant right-step   7 constant left-step   8 constant grab-cry   9 constant hold-chatter
$A constant sink-sound

state: homunculus-state
  cell field slot                 \ 7..9
  cell field kind  cell field sight  cell field level   \ kind homunculus-kind..; sight: how near she must come
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
  cell field move-mode            \ (+0xF8) creature-move
  cell field untouchable          \ (+0x2D) out of reach of blows
  cell field to-spot-free         \ (kinds 8 / 9, till they come to their spot) blocking nothing, its paths $40080
  /vec field target  cell field target-tri  1 floats field target-d   \ a spot by her to make for
  cell field grab-spot            \ which of the four spots round her next
  1 floats field her-dist  1 floats field hewie-dist
  cell field her-mode  cell field her-sub  cell field her-doing  cell field her-unseen
  cell field blow-n  cell field blow-damage  cell field blow-pending  cell field striker   \ (+0x14F0: who struck - her, Hewie; -1 a thing)
  1 floats field blow-yaw         \ (+0x14FC) a thing's blow: the heading it faces
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
: k ( -- n )  kind @ homunculus-kind xor ;
: walks-to-a-spot? ( -- flag )  k 8 10 within ;   \ (kinds 8 / 9: awake, to their spot first)
\ its masks (CreatureB_SetupKind: kinds 8 / 9 not yet awake block nothing, plan as kind 6, till
\ they come to their spot - CreatureB_StateWalkSpot; the others $04020028)
: path-mask ( -- m )  to-spot-free @ if  creature-path-floor  else  creature-floor  then ;
: stand-mask ( -- m )  to-spot-free @ if  0  else  creature-floor  then ;
: full-strength ( -- n )  k case  1 of 6 endof  2 of 7 endof  >r 5 r>  endcase ;   \ (D_0042C460)
5 minutes constant time-out

\ ---- its model and motion ----
: actor@ ( -- a )  model @ actor ;
: play-now ( anim -- )  dup anim !  model @ swap  2dup motion-entry nip  motion-play ;
: play ( anim -- )  dup anim @ = if  drop exit  then  play-now ;
: ended? ( -- flag )  actor@ act.mflags l@ $20 and 0<> ;   \ (its motion's end flag)
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
: settle ( -- )   \ (+0x6B cleared unless it has held her: held off no more)
    0 step !  0 sub !  0 frames !  my-path path-clear  grabbed @ 0= if  0 held-off !  then
    16 random 3 mod 1+ 150 * wait !  -1 target-tri ! ;
: become ( st -- )  want !  creature-free move-mode !  settle ;
: become-move ( st move -- )  move-mode !  want !  settle ;
: first ( anim -- )  step @ if  drop exit  then  play-now  1 step ! ;
: coin ( a b -- n )  2 random if  nip  else  drop  then ;

\ ---- seeing her ----
: hidden-or-busy? ( -- flag )  hidden? her-doing @ 0<> or ;
: reaches? ( v tri mask -- flag )   \ straight from it onto that triangle, kept off the mask's floor (Actor_TriTo)
    {: v tri m :}  self body-tri my-at v m v-walk  tri = ;
: straight-to-her? ( -- flag )  her-at her-tri path-mask reaches? ;   \ (its paths' mask)
: sees-her? ( -- flag )   \ (Actor_CanWalkBetween, mask 0: both ways over $40080 / $4000)
    self body-tri my-at her-tri her-at 0 can-walk-between? ;
: divided? ( -- flag )  her-tri self body-tri across-divider? ;

: x-z-of ( v -- ) ( F: -- x z )  dup v.x sf@  v.z sf@ ;

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
: another-level? ( -- flag )  my-at v.y sf@  her-at v.y sf@  f= 0= ;
: straight-at-her ( -- tri | -1 )
    straight-to-her? 0= if  -1 exit  then
    my-path path-clear
    another-level? if
        her-dist f@ 2e f< if  1 tries +!  then
        tries @ 30 >=  self body-tri her-tri <> and if  0 tries !  true gave-up !  -1 exit  then
    then
    her-dist f@ 2e f> if  her-at x-z-of stride body-advance drop  then
    her-tri ;

\ ---- approaching a spot by her (CreatureB_Approach, _TargetNearFiona): how far from her, a spot
\ there, turned to it, walking there, then turned to her ----
enum: approach#  setting-off seeking-spot turning-to-spot walking-to-spot turning-to-her ;
fvariable try-turn
: turn-of ( k -- ) ( F: -- a )  dup 8 > if  8 - negate  then  s>f 0.3925e f* ;   \ (D_0042C650)
: spot-try ( i -- found? )   \ before or behind her, turned by the last try's turn, reachable
    2 mod if  -1e  else  1e  then  target-d f@ 16 random s>f f+ f*  0e fswap try-turn f@ rotate-by   ( F: dx dz )
    her-at v.z sf@ f+  target v.z sf!  her-at v.x sf@ f+  target v.x sf!  her-at v.y sf@ target v.y sf!
    16 random turn-of try-turn f!
    target v-tri dup target-tri !
    dup 0< if  drop false exit  then
    self body-tri my-at target path-mask v-walk 0< 0=  swap self body-tri <> and ;
: find-spot ( -- found? )  0e try-turn f!  17 0 do  i spot-try if  true unloop exit  then  loop  -1 target-tri !  false ;
: touching-her? ( -- flag )  my-at her-at vec-dist-xz  self body-dims fdrop  her body-dims fdrop f+  f< ;
: may-grab? ( -- flag )  running @ grabbing <>  grabbed @ 0= and  her-mode @ defend-mode <> and ;
: approach ( F: d -- )
    sub @ case
        setting-off of  target-d f!  seeking-spot sub !  my-path path-clear  endof
        seeking-spot of  fdrop  turning-to-spot sub !
              my-path path-left? if  exit  then
              find-spot 0= if  seeking-spot sub !  target-d f@ 1e f+ target-d f!  then  endof
        turning-to-spot of  fdrop  target 30 faces? if  walking-to-spot sub !  else  target turn-to  then  endof
        walking-to-spot of  fdrop
              target-tri @ 0< if  seeking-spot sub !  exit  then
              her-unseen @ touching-her? and if   \ (she can't be seen: touching her, it grabs)
                  my-path path-clear  may-grab? if  grabbing become  exit  then
              then
              my-path path-left? 0= if  target-tri @ target plan 0= if  seeking-spot sub !  exit  then  then
              walk-path 0= if  turning-to-her sub !  then  endof
        turning-to-her of  fdrop
              her-at 30 faces? 0= if  her-at turn-to  exit  then
              ended? if  watching chattering coin creature-about become-move  then  endof
        >r fdrop r>
    endcase ;

\ ---- the states ----
: watch ( chatter? other -- )   \ watching her, deciding now and then
    {: chatters other :}
    root-move  her-at turn-to
    chatters frames @ 44 mod 0= and if  chatter sound  then
    1 frames +!
    hidden? if  frames @ 150 mod 0= if  hanging-back creature-about become-move  then  exit  then
    her-unseen @ if  frames @ 150 mod 0= if  circling creature-about become-move  then  exit  then
    sees-her? 0= if  frames @ 150 mod 0= if  out-of-sight become  then  exit  then
    frames @ 90 mod if  exit  then
    held-off @ if  closing-in other coin  else  chasing  then  become ;
\ ---- the stalker at a door it stands by (in the exit's area; he at the door on its front -
\ Progress_WhoIsWhere's bit 4 for him): it holds off (CreatureB_StateDoor) till he isn't ----
: stalker-here? ( -- flag )
    stalker-age @ 2 <  stalker @ 0< 0= and if  stalker @ body? if  stalker @ body-room my-room @ =  exit  then  then  false ;
: at-the-door? ( exit -- flag )   \ (the stalker)
    dup stalker @ body-at vec@ door-near?  swap 0 stalker @ body-at vec@ door-in-area? and ;
: stalker-at-my-door? ( -- flag )
    stalker-here? 0= if  false exit  then
    8 0 do  i door-here? if  i my-at by-exit? i at-the-door? and if  true unloop exit  then  then  loop  false ;
: wait-out-stalker ( -- )
    rest-anim first  stalker-at-my-door? 0= if  chasing become  then ;
: on-ladder-or-thrown? ( -- flag )  her-mode @ dup thrown-mode = swap ladder-mode = or ;
: after-her ( -- )
    walk-anim first
    hidden? if  hanging-back creature-about become-move exit  then
    her-unseen @ if  circling creature-about become-move exit  then
    hidden-or-busy? half-hidden? 0= and if  biding become exit  then
    stalker-at-my-door? if  stalker-at-door become exit  then
    divided? if  across-divider become exit  then
    her-at turn-to
    on-ladder-or-thrown? if  true held-off !  closing-in become exit  then
    1 frames +!
    gave-up @ if  -1  else  straight-at-her  then  {: t :}
    cutscene-active? in-play? 0= her-doing @ 0<> or and if  hanging-back creature-about become-move exit  then
    t 0< 0= her-dist f@ 10e f<= and may-grab? and if  t her-tri = if  grabbing become  then  exit  then
    t 0< if
        my-path path-left? 0= if
            her-tri 0< if  exit  then
            her-tri nav-flags creature-floor and if  her-off-floor become exit  then
            her-tri her-at plan 0= if  sees-her? 0= if  out-of-sight become  then  exit  then
        then
        walk-path drop
    then
    her-dist f@ 40e f< her-dist f@ 80e f> or if  exit  then
    frames @ wait @ mod if  exit  then
    watching chattering coin become ;
: close-in ( -- )   \ while she's hidden, on a ladder or thrown
    walk-anim first  hidden? on-ladder-or-thrown? or if  10e approach exit  then
    0 held-off !  chasing become ;
: hang-back ( -- )  walk-anim first  hidden? if  20e approach  else  watching become  then ;
: about-off-floor ( -- )   \ her triangle one it can't stand on
    walk-anim first  her-tri nav-flags creature-floor and if  5e approach  else  watching become  then ;
: circle-her ( -- )  walk-anim first  her-unseen @ if  20e approach  else  chasing become  then ;
: turn-to-her ( -- )  walk-anim first  her-at 20 faces? if  chasing become  else  her-at turn-to  then ;
: go-to-its-spot ( -- )   \ to its spot in rooms $8F / $91 (a triangle by kind), out of reach of
    walk-anim first                \ blows; there its masks $04020028 again; then to her
    sub @ if  turn-to-her exit  then
    true untouchable !
    my-room @ $91 = if  k 8 = if  $D0  else  $9F  then  else  $E  then  {: tri :}
    my-path path-left? 0= if  tri dup tri-center target vec!  target plan 0= if  exit  then  then
    walk-path if  exit  then
    1 sub !  0 untouchable !  false to-spot-free !  stand-mask body-mask! ;
: bide ( -- )   \ she's hidden or busy
    rest-anim first
    hidden? if  hanging-back creature-about become-move exit  then
    half-hidden? hidden-or-busy? 0= or if  chasing become  then ;
: seek-her ( -- )
    walk-anim first
    sees-her? if  watching become exit  then
    divided? if  across-divider become exit  then
    10e approach ;
: face-across ( -- )   \ across the divider from her, idling and facing her (resting, chattering in turn)
    root-move  her-at turn-to  1 frames +!
    divided? 0= if  watching become exit  then
    step @ 0= if  sub @ if  chatter-anim  else  rest-anim  then  play-now  1 step !  then
    frames @ 90 mod 0= if  0 frames !  0 step !  sub @ 0= sub !  then ;

: up-again ( -- )  0 frames !  get-up-anim play-now ;
: getting-up ( -- up? )   \ (its thud as it's on its feet)
    1 frames +!  frames @ 34 = if  thud sound  then  ended? ;
\ dropped from a height (or knocked down), and up again
enum: down#  falling lying rising-up standing-up ;
: knocked-down ( -- )
    step @ case
        falling of  reel-back-anim play
              fall f@ fall-v f@ f-  fall f!   fall-v f@ 0.085e f+ fall-v f!
              fall f@ f0> 0= if  0e fall f!  0e fall-v f!  thud sound  lying step !  then  endof
        lying of  ended? if  1 frames +!  frames @ 32 >= if  rising-up step !  then  then  endof
        rising-up of  up-again  standing-up step !  endof
        standing-up of  root-move  getting-up if  0 untouchable !  chasing become  then  endof
    endcase ;

: leave ( -- )  leaving want !  true grabbed !  creature-struck move-mode !  settle ;
\ a blow - it reels, then is up; no strength left, it leaves
enum: blow#  blow-landed reeling-back rising up-from-blow ;
: blow-came-from ( F: -- yaw )   \ (+0x14F0: from her, from Hewie - whoever struck - or a thing's heading)
    striker @ dup 0< if  drop blow-yaw f@ exit  then
    dup body? if  body-at  else  drop her-at  then  heading-to ;
: struck ( -- )
    step @ reeling-back <> if  root-move  then
    step @ case
        blow-landed of  blow-damage @ negate strength +!   \ (reeling back facing where it came from, else forward)
              blow-came-from  self body-yaw f- angle-wrap fabs  pi f2/ f< if  reel-back-anim  else  reel-forward-anim  then
              play-now  hurt-cry sound  reeling-back step !  endof
        reeling-back of  true untouchable !
              ended? 0= if  root-move exit  then
              strength @ 0> 0= if  leave exit  then
              1 frames +!  frames @ 30 > if  rising step !  then  endof
        rising of  up-again  up-from-blow step !  endof
        up-from-blow of  getting-up if  0 untouchable !  facing-her become  then  endof
    endcase ;

\ grabbing her: to each spot round her in turn (D_0042C610: 2 behind, 2 before, 2 right, 2
\ left) until she's held (thrown, held down); then held, chattering; let go, the threat up
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
: her-held? ( -- flag )  her-mode @ thrown-mode =  her-sub @ held-down-r = and ;
: holdable? ( -- flag )   \ (Fiona_CanInteract $C: not on a ladder or down; thrown - only held down)
    her-mode @ thrown-mode = if  her-sub @ held-down-r =  else  her-mode @ dup ladder-mode = swap down-mode = or 0=  then ;
enum: grab#  making-for-her seizing holding-her letting-go ;
75 constant grab-threat       \ (her fright as it lets go)
$80 constant grab-noise
: grab-her ( -- )
    step @ case
        making-for-her of
            holdable? 0= if  exit  then   \ (it waits till she can be held)
            my-path path-left? 0= if
                next-grab-spot  target v-tri dup target-tri !
                dup 0< if  drop exit  then
                dup nav-flags creature-floor keeps-fiona-off or and if  drop exit  then
                target plan 0= if  exit  then
            then
            creature-holding move-mode !
            walk-path if  homunculi-request my-at vec@ xyz>cells 0  her send fiona-request exit  then   \ (on its way: her request to be held)
            her-held? 0= if  exit  then
            her-at heading-to body-turn  grab-cry sound  grab-anim play-now  seizing step !
        endof
        seizing of  ended? if  hold-chatter sound  giggle sound  holding-anim play-now  holding-her step !  0 frames !  then  endof
        holding-her of
            1 frames +!  frames @ 35 mod 0= if  hold-chatter sound  then  frames @ 70 mod 0= if  giggle sound  then
            her-held? if  exit  then
            creature-struck move-mode !  let-go-anim play-now
            grab-threat s>f f>cell s" panic" actor-named send threat
            grab-noise my-room @ self body-tri -1 world-noise  s" acoustics" actor-named send noise
            letting-go step !
        endof
        letting-go of  root-move  ended? if  true untouchable !  leave  then  endof
    endcase ;

: gone ( -- )   \ its model let go, its slot free (the bubbles: `going`'s welling)
    model @ actor-free  -1 model !
    slot @ s" creatures" actor-named send creature-gone  self kill ;

: going ( -- )   \ leaving - sinking, fading, bubbles welling up where it sinks; at its end gone
    step @ 0= if
        sink-anim play-now  1 step !  true untouchable !  $FF fade !
        in-play? if  self body-tri my-at vec@ welling  then
    then
    root-move
    in-play? 0= if  gone exit  then
    1 frames +!  frames @ 65 = sub @ 0= and if  1 sub !  sink-sound sound  then
    fade @ 10 < if  0  else  fade @ 2 -  then  fade !
    ended? if  gone  then ;
\ its footsteps (CreatureB_Footsteps): walking in the room being played, a step's sound as each
\ foot comes down - not in a scene, nor while the world is held
: footsteps ( -- )
    model @ 1 foot-down?  model @ 0 foot-down?                       ( left right )
    in-play?  cutscene-active? 0= and  world-held state-flag? 0= and  anim @ walk-anim = and if
        dup right-down @ 0= and if  right-step sound  then
        over left-down @ 0= and if  left-step sound  then
    then  right-down !  left-down ! ;
: run-state ( -- )
    running @ case
        watching of  rest-anim first  false chattering watch  endof
        chattering of  chatter-anim first  true watching watch  endof
        reeling of  struck  endof
        grabbing of  grab-her  endof
        leaving of  going  endof
        closing-in of  close-in  endof
        hanging-back of  hang-back  endof
        her-off-floor of  about-off-floor  endof
        dropped of  knocked-down  endof
        circling of  circle-her  endof
        facing-her of  turn-to-her  endof
        to-its-spot of  go-to-its-spot  endof
        biding of  bide  endof
        out-of-sight of  seek-her  endof
        stalker-at-door of  wait-out-stalker  endof
        across-divider of  face-across  endof
        >r after-her r>   \ (chasing)
    endcase ;

\ ---- kept in the progress (CreatureB_Save / _Load) ----
: record ( -- addr )  slot @ /kept-creature *  progress pr.creatures + ;
: kept-state ( -- addr )  slot @ /kept-state *  progress pr.creature-states + ;
: keep ( -- )   \ (leaving, not in use: it doesn't come back)
    in-play? if  self body-tri my-tri !  then
    want @ leaving <> 1 and record kc.used l!
    kind @ record kc.kind l!  sight @ record kc.reach l!  level @ record kc.level l!
    my-room @ record kc.room l!  my-tri @ record kc.tri l!  time @ record kc.time l!
    0 record kc.out l!  awake @ record kc.come l!
    strength @ kept-state ks.strength l!  running @ kept-state ks.state l!
    fall f@ kept-state ks.fall sf!  self body? if  self body-yaw kept-state ks.heading sf!  then ;
: unkeep ( -- )  record /kept-creature 0 fill ;

\ ---- waking (CreatureB_Distances, _Notices): her on a triangle it reaches within its sight,
\ Hewie in its room on his within 30, a blow, or no sight at all ----
: hewie ( -- id )  s" hewie" actor-named ;
: measure ( -- )
    in-play? 0= if  exit  then
    my-at her-at vec-dist-xz her-dist f!
    -1e hewie-dist f!
    hewie dup body? if  dup body-room my-room @ = if  body-at my-at vec-dist-xz hewie-dist f!  exit  then  then  drop ;
: notices? ( hewie? -- flag )   \ (reached over its own mask)
    her-at her-tri stand-mask reaches?  her-dist f@ sight @ s>f f<= and if  drop true exit  then
    if  hewie-dist f@ f0< 0=  hewie-dist f@ 30e f<= and
        hewie body? and if  hewie body-at hewie body-tri stand-mask reaches? if  true exit  then  then  then
    blow-pending @ if  true exit  then
    sight @ 0= ;
: wake ( -- )
    in-play? 0= awake @ or if  exit  then
    fall f@ f0= if
        true notices? if  wake-cry sound  true awake !  walks-to-a-spot? if  to-its-spot  else  facing-her  then  become  then
    else
        true untouchable !  false notices? if  true awake !  then   \ (high up: it drops once it notices her)
    then ;

\ ---- each frame (CreatureB_Frame, _States) ----
: on-its-level? ( flags -- flag )   \ (Actor_TeleportRandom: level 0 / 1 off the other half; else both halves)
    level @ case
        0 of  one-half and 0=  endof
        1 of  other-half and 0=  endof
        >r both-halves and both-halves = r>
    endcase ;
: may-be-put? ( tri -- flag )   \ not its mask's floor, on its level, off the doors' spots
    dup nav-flags {: tri fl :}
    fl stand-mask and if  false exit  then  fl on-its-level? 0= if  false exit  then  tri off-the-doors? ;
: somewhere ( -- tri | -1 )
    nav-tris 0= if  -1 exit  then
    1000 0 do  nav-tris random  dup may-be-put? if  unloop exit  then  drop  loop  -1 ;
: put-on ( tri -- )  dup my-tri !  dup 0< if  drop exit  then  room-id swap dup tri-center body-place ;
: here-already? ( -- flag )  self body? if  self body-room room-id =  else  false  then ;
: where-it-was ( -- )   \ (its place kept off screen, the triangle under it)
    my-at v-tri dup 0< if  drop my-tri @  then  dup my-tri !  my-at vec@  room-id swap body-place ;
\ in the room being played (CreatureB_EnterRoom): where it was left here, else on its triangle;
\ none - put somewhere at random, watching her
: on-the-floor ( -- )
    stand-mask body-mask!
    my-tri @ 0< if  somewhere put-on  watching want !  watching running !  exit  then
    here-already? if  where-it-was  else  my-tri @ put-on  then ;
: blow-taken ( -- )   \ (CreatureB_ActionState) a blow: struck
    blow-pending @ 0= if  exit  then  0 blow-pending !
    move-mode @ creature-struck = if  exit  then
    reeling want !  reeling running !  creature-struck move-mode !  settle ;
: show ( -- )   \ its model where its body is (raised while it drops), seen in play
    model @ 0< if  exit  then
    in-play? self body? and 0= if  0 actor@ act.visible l!  exit  then
    my-at v.x sf@ actor@ act.x sf!  my-at v.y sf@ fall f@ f+ actor@ act.y sf!  my-at v.z sf@ actor@ act.z sf!
    self body-yaw actor@ act.yaw sf!  1 actor@ act.visible l!
    running @ leaving = if  fade @ $80 min s>f 128e f/  else  1e  then  actor@ act.alpha sf!   \ (leaving: fading)
    slot @ my-room @ move-mode @ awake @ 0<> broadcast creature-doing ;
: frame ( -- )
    stalker-age @ 100 < if  1 stalker-age +!  then
    measure  wake
    awake @ 0= if  show keep exit  then
    1 time +!
    in-play? 0= if
        time @ time-out >= if  gone exit  then
        grabbed @ if  gone exit  then
        stalkers-stay state-flag? 0= if   \ (CreatureB_Travel: not while the story keeps them where they are)
            0.35e 0.35e trip trip-step if  stand-mask body-mask!  my-tri @ 0< if  on-the-floor  then  then
        then
        show keep exit
    then
    another-level? 0= if  0 gave-up !  then
    blow-taken
    trip my-at door-watch if   \ (by a swinging door: standing (its state 8); left shut, through it (6))
        trip gone-through? if  body-off  then  show keep exit
    then
    want @ running @ <> if  want @ running !  then
    run-state  model @ 0< if  exit  then  footsteps  show keep ;

: start ( -- )   \ its model, on the floor if in the room being played, facing as placed
    walks-to-a-spot? awake @ 0= and to-spot-free !
    s" O_HMB/HMB_000" actor-load dup model !  s" motions-homunculus" motion-table   \ (D_0042C730: its motions)
    0 actor@ act.visible l!  rest-anim play-now
    3e 6e body-size
    in-play? if  on-the-floor  else  body-off  then
    full-strength strength !  trip trip-reset  settle
    fall f@ f0> if  dropped want !  dropped running !  then
    keep ;

\ ---- she leaves by `exit` (CreatureB_FionaLeft): it makes for the exit's far spot along a path
\ it walks off screen (`walk-on`, at 0.35), through its door at the end. No way there (not across
\ the divider): if another open exit has one, a way there anyhow; none - its trip from here.
\ Nearer her exit than she is, it walks there but keeps the door it last made for (through that
\ one at the end, if it is on this side). Already as near the door as the far spot, and straight
\ to it: through at once ----
create far /vec allot   create door-at /vec allot
: far-spot ( exit -- tri )  2 exit-spot far vec! ;
: way-to-far? ( exit -- ok? )   \ (CreatureB_Path)
    far-spot dup 0< if  drop false exit  then
    dup self body-tri across-divider? if  drop false exit  then  far plan ;
: leads-open? ( exit -- flag )   \ (Rooms_ExitLeads, Progress_ExitOpen)
    my-room @ swap 2dup room-exit-leads drop {: room ex there :}
    there 0< there room = or if  false exit  then
    room ex room-exit-door dup 0< if  drop false exit  then
    dup door-flags $20 and if  drop false exit  then  door-open? ;
: another-way? ( -- flag )  8 0 do  i leads-open? if  i way-to-far? if  true unloop exit  then  then  loop  false ;
: door-spot ( exit -- )  1 exit-spot drop door-at vec! ;   \ (the exit's near spot: Rooms_ExitPointIn)
: nearer-than-her? ( exit -- flag )   \ (Actor_NearerRoom)
    door-spot  my-at door-at vec-dist  her-at door-at vec-dist f< ;
: head-out ( exit -- )
    {: ex :}
    trip t.closed 13 cells 0 fill  0 trip t.n !  0 trip t.i !  -1 trip t.exit !
    my-path trip t.path !  0 trip t.walking !  my-path path-clear
    ex 0< 0= if  ex way-to-far? trip t.walking !  then
    trip t.walking @ 0= if  another-way? 0= if  exit  then  then
    ex 0< if  exit  then
    ex nearer-than-her? if  exit  then
    trip t.walking @ 0= if   \ (a way to hers anyhow)
        ex far-spot dup 0< if  drop exit  then  far plan 0= if  exit  then  true trip t.walking !
    then
    my-room @ ex room-exit-door trip t.last !
    ex far-spot drop  ex door-spot
    my-at door-at vec-dist  far door-at vec-dist f<= if
        self body-tri my-at door-at path-mask v-walk 0< 0= if  my-path path-end  then
    then ;

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
  on homunculus-kept ( slot -- )   \ (CreatureB_Load: its state as it was - reeling, it starts afresh;
      \ not awake and high up, dropping)
      slot !  record kc.kind l@ kind !  record kc.reach l@ sight !  record kc.level l@ level !
      record kc.room l@ my-room !  record kc.tri l@ my-tri !  record kc.time l@ time !
      record kc.come l@ 0<> awake !  kept-state ks.fall sf@ fall f!
      kept-state ks.strength sl@  kept-state ks.state sl@  kept-state ks.heading sf@  {: str st F: yaw :}
      start   \ (which keeps it afresh)
      str ?dup if  strength !  then   \ (a save from before it was kept: full)
      st dup want !  running !
      running @ reeling = if  watching become  then
      awake @ 0= fall f@ f0> and if  dropped want !  dropped running !  then
      yaw body-turn  keep ;
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
      n blow-n !  damage blow-damage !  true blow-pending !  sender striker !  sender send blow-taken ;
  on leaving-room ( room exit -- )   \ she leaves: in her room and awake, it follows
      over my-room @ = awake @ and grabbed @ 0= and if  nip head-out exit  then  2drop ;
  on arrived ( room exit -- )  2drop  in-play? if  on-the-floor  then ;   \ (elsewhere it keeps its place there)
  on thing-blow ( damage yaw -- )   \ (a magnesia's: ThingShared_ReachStalkers / _StompStalkers - in any
      \ room, untouchable or not; it is struck once it is in play, facing that way)
      cell>f blow-yaw f!  blow-damage !  -1 striker !  true blow-pending ! ;
  on killed ( -- )  model @ dup 0< if  drop exit  then  actor-free ;
  on set-level ( level -- )  level ! ;   \ (its floor of a split room: a room's command - Room48)
end-behaviour

: homunculus-spawn ( -- id )  homunculus homunculus-state s" homunculus" spawn ;
