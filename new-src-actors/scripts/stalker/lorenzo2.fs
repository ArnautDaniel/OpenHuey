\ stalker/lorenzo2.fs - Lorenzo's other class (P4c; kind 10, src/game/lorenzo.c Lorenzo2_*): the
\ Pursuer's chase with his tables; from the panic's first stage his "rage" (mode 2: his rage
\ tables, his reach 18 - else 70, hard 40 - for his attacks from afar). His own actions:
\ - his first (Lorenzo2_StartSink): 30 or more from her over the floor - he sinks into it and
\   comes up 10 short of her (where he is, nearer than 20), out of the floor; nearer, his move 1,
\   or (no way to her) a look about;
\ - his second (Lorenzo2_StartStalkBelow): a way to her - he sinks and comes up by her (5 short
\   of her) in his sweep: at its key she and Hewie within its reach (over his floor) are struck,
\   once each, a stumble by its chance;
\ - his third (Lorenzo2_StartGrab): his reach from afar: aimed where she is at its 12th frame; at
\   its key whoever is within its reach of there is struck.
\ Under the floor 15 frames, out of sight.
IN: stalker.lorenzo2
USING: engine game-state actors messages common facts flag-names paths vectors stalker-names stalker.names stalker.anims stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue ;

\ his animations: sinking (to come up behind, by her), coming up, his reach from afar, his sweep,
\ his slam; his blows
$1303 constant sink-anim   $1304 constant sink-to-her-anim   $1305 constant come-up-anim
$E01 constant reach-anim   $E02 constant sweep-anim   $2301 constant slam-anim
$1904 constant hold-blow-anim   $E00 constant blow-anim   $E04 constant blow-2-anim   $E05 constant blow-3-anim   $E06 constant arm-blow-anim
: key? ( dt -- flag )  my-model @ 0 rot 1 motion-events 2 and 0<> ;
\ ---- his way to her over the floor: how far (-1 none) ----
: way-to-her ( F: -- d )
    quarry body-tri dup 0< if  drop -1e exit  then  quarry body-at plan 0> if  way-left  else  -1e  then ;
: goal-here ( -- )  rise-at my-at vec-copy  self body-tri rise-tri ! ;
: goal-along ( F: d -- )   \ the point that far along his way (planned by way-to-her)
    my-path my-at path-ahead drop  rise-at ahead vec-copy  rise-at self body-mask v-tri-in dup 0< if  drop self body-tri  then  rise-tri ! ;
\ ---- under the floor and up again ----
\ (where he sinks away and comes up: a ring of light - LightRing)
0 constant sinking   1 constant rising
: go-under ( rise-anim rise-xt -- )   \ (Lorenzo2_Sink: out of sight, 15 frames)
    rise-xt !  rise-anim !  0 my-model @ actor act.visible l!  15 under-t !
    sinking my-at vec@ sink-ring ;
: up-again ( -- )   \ at his goal, facing her, seen again
    room-id rise-tri @ rise-at vec@ body-place
    quarry body-at heading-to yaw!  -1 my-model @ actor act.visible l!
    rising my-at vec@ sink-ring
    rise-anim @ play-now  0 swept !  rise-xt @ own-act-1 act! ;
: under ( -- )  -1 under-t +!  under-t @ 0> 0= if  up-again  then ;
: playing-out ( -- )  root-move  ended? if  -1 step-done !  then ;
\ his sweep (Lorenzo2_StateSweep): she and Hewie within move 2's reach, reached over his
\ floor, struck once each
: reached? ( id -- flag ) ( F: reach -- )
    dup here? 0= if  drop fdrop false exit  then
    my-at over body-at vec-dist fdup f0< if  fdrop fdrop drop false exit  then  f< if  drop false exit  then
    self body-tri my-at rot body-at self body-mask v-walk 0< 0= ;
: stumble? ( e -- how )  m.stumble f@ rnd 100e f* f< 0= if  stumbles  else  0  then ;
\ (kind 39's: whoever it sees - Lorenzo2b_Sweep, Npc_WhoSeen)
: sweep-reaches? ( e id -- flag )
    my-kind @ kind39-kind = if  nip fiona = if  sees-fiona  else  sees-hewie  then  @ 0<> exit  then
    swap m.reach f@ reached? ;
: sweep-hits ( e -- )
    dup fiona sweep-reaches?  swept @ 1 and 0= and if
        dup m.kind @  over stumble?  2 pick m.threat f@ f>cell  fiona send hit  1 swept @ or swept !
    then
    dup hewie sweep-reaches?  swept @ 2 and 0= and if
        dup m.kind @  over m.damage @  2 pick stumble?  hewie send blow  2 swept @ or swept !
    then  drop ;
: sweeping ( -- )  root-move  0 key? if  2 move-of sweep-hits  then  ended? if  -1 step-done !  then ;

\ ---- his first: sinking to come up by her (Lorenzo2_StartSink / StateSink) ----
: sinking-behind ( -- )
    root-move  ended? 0= if  exit  then
    way-to-her fdup 20e f< if  fdrop goal-here  else  10e f- goal-along  then
    come-up-anim ['] playing-out go-under  ['] under own-act-1 act! ;
: sink-start ( -- )
    way-to-her fdup 30e f< if
        f0< if  stand-watch  else  1 combo-start  then  exit
    then  fdrop
    sink-anim play-now  ['] sinking-behind own-act-1 act! ;
\ ---- his second: stalking from below (Lorenzo2_StartStalkBelow / StateApproach / StateRise) ----
: sinking-to-her ( -- )
    root-move  ended? 0= if  exit  then
    way-to-her fdup f0< if  fdrop goal-here  else  5e f- 0e fmax goal-along  then
    sweep-anim ['] sweeping go-under  ['] under own-act-1 act! ;
: below-start ( -- )
    way-to-her f0< if  stand-watch exit  then
    sink-to-her-anim play-now  ['] sinking-to-her own-act-1 act! ;
\ ---- his third: his reach from afar (Lorenzo2_StartGrab / StateGrab) ----
: grabbing ( -- )
    {: | m :}  root-move
    12 key? if  rise-at quarry body-at vec-copy  then
    0 key? if
        3 move-of to m
        rise-at m m.reach f@ reaches? if  m m.kind @  m stumble?  m m.threat f@ f>cell  fiona send hit  then
        rise-at m m.reach f@ reaches-dog? if  m m.kind @  m m.damage @  m stumble?  hewie send blow  then
        0  rise-at vec@ 0e 1e 0e mag-burst-new particles-engines   \ (its burst there: BurstA)
    then
    ended? if  -1 step-done !  then ;
: grab-start ( -- )  rise-at quarry body-at vec-copy  reach-anim play-now  ['] grabbing own-act-1 act! ;
create lorenzo2-acts  ' sink-start , ' below-start , ' grab-start ,

\ ---- his blows' effects (Lorenzo2_SlamDust, Lorenzo2_BlowSparks): his slam at its key, eight
\ grey dust clouds at his hand, 320..640 big; his blows at their effect key ($20), seven sparks
\ about his hand (his arm's for the last) ----
$32 constant his-hand   $28 constant his-arm
create spark-offsets  0.866e f, 0.866e f, 0e f,   -0.866e f, -0.866e f, 0e f,   0.866e f, -0.866e f, 0e f,
    -0.866e f, 0.866e f, 0e f,   0e f, 0e f, 1e f,   0e f, 0e f, -1e f,   0e f, 0e f, 0e f,
create at-bone /vec allot
: slam-dust ( -- )
    anim @ slam-anim <>  0 key? 0= or if  exit  then
    my-model @ his-hand bone-pos at-bone vec!
    8 0 do  2 $50 $50 $50  rnd 320e f* f>s 320 +  at-bone vec@ burst-new particles-engines  loop ;
: spark-bone ( -- bone | -1 )
    anim @ case
        hold-blow-anim of  his-hand  endof   blow-anim of  his-hand  endof   blow-2-anim of  his-hand  endof
        blow-3-anim of  his-hand  endof    arm-blow-anim of  his-arm  endof
        >r -1 r>
    endcase ;
: blow-sparks ( -- )
    spark-bone dup 0<  effect-key? 0= or if  drop exit  then
    my-model @ swap bone-pos at-bone vec!
    7 0 do
        rnd 6e f* f>s 12 +
        i 3 * floats spark-offsets +  dup f@ at-bone v.x sf@ f+  dup float+ f@ at-bone v.y sf@ f+  2 floats + f@ at-bone v.z sf@ f+
        1e  0.5e rnd 0.5e f* f+  1.1e rnd 0.05e f* f+  spark
    loop ;

\ ---- his frame (Lorenzo2_Update): his "rage" from the panic's first stage, out of it at 0; his
\ reaches by it; his blows' effects ----
: lorenzo2-frame ( -- )
    slam-dust  blow-sparks
    her-stage @ composed = if  0 raging !
    else  raging @ 0=  reeling @ 0= and  my-mode @ after-her = and if  -1 raging !  then  then
    raging @ if  18e 12e  else  hard-mode? if  40e 30e  else  70e 60e  then  then  reach-k f!  attack-range f! ;
