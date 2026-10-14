\ stalker/lorenzo2.fs - Lorenzo's other class (P4c; kind 10, src/game/lorenzo.c Lorenzo2_*): the
\ Pursuer's chase with his tables; from the panic's first stage his "rage" (mode 2: his rage
\ tables, his reach 18 - else 70, hard 40 - for his attacks from afar). His own actions:
\ - `$1000` (Lorenzo2_StartSink): 30 or more from her over the floor - he sinks into it ($1303)
\   and comes up 10 short of her (where he is, nearer than 20), out of the floor ($1305); nearer,
\   his move 1, or (no way to her) a look about;
\ - `$1001` (Lorenzo2_StartStalkBelow): a way to her - he sinks ($1304) and comes up by her (5
\   short of her) in his sweep ($E02): at its key she and Hewie within its reach (over his floor)
\   are struck, once each, a stumble by its chance;
\ - `$1002` (Lorenzo2_StartGrab): his reach from afar ($E01): aimed where she is at its 12th
\   frame; at its key whoever is within its reach of there is struck.
\ Under the floor 15 frames, out of sight.
IN: stalker.lorenzo2
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue ;

: move# ( n -- addr )  /move * moves + ;
: key? ( dt -- flag )  my-model @ 0 rot 1 motion-events 2 and 0<> ;
\ ---- his way to her over the floor: how far (-1 none) ----
: way-to-her ( F: -- d )
    quarry body-tri dup 0< if  drop -1e exit  then  quarry body-at plan 0> if  way-left  else  -1e  then ;
: goal-here ( -- )  goal my-at vec-copy  self body-tri goal-tri ! ;
: goal-along ( F: d -- )   \ the point that far along his way (planned by way-to-her)
    my-path my-at path-ahead drop  goal ahead vec-copy  goal self body-mask v-tri-in dup 0< if  drop self body-tri  then  goal-tri ! ;
\ ---- under the floor and up again ----
\ (where he sinks away and comes up: a ring of light - LightRing)
0 constant sinking   1 constant rising
: go-under ( rise-anim rise-xt -- )   \ (Lorenzo2_Sink: out of sight, 15 frames)
    rise-xt !  rise-anim !  0 my-model @ actor act.visible l!  15 under-t !
    sinking my-at vec@ sink-ring ;
: up-again ( -- )   \ at his goal, facing her, seen again
    room-id goal-tri @ goal vec@ body-place
    quarry body-at heading-to yaw!  -1 my-model @ actor act.visible l!
    rising my-at vec@ sink-ring
    rise-anim @ play-now  0 swept !  rise-xt @ $1000 act! ;
: under ( -- )  -1 under-t +!  under-t @ 0> 0= if  up-again  then ;
: playing-out ( -- )  root-move  ended? if  -1 step-done !  then ;
\ his sweep ($E02, Lorenzo2_StateSweep): she and Hewie within move 2's reach, reached over his
\ floor, struck once each
: reached? ( id -- flag ) ( F: reach -- )
    dup here? 0= if  drop fdrop false exit  then
    my-at over body-at vec-dist fdup f0< if  fdrop fdrop drop false exit  then  f< if  drop false exit  then
    self body-tri my-at rot body-at self body-mask v-walk 0< 0= ;
: stumble? ( e -- how )  7 cells + f@ rnd 100e f* f< 0= if  $8000  else  0  then ;
\ (kind 39's: whoever it sees - Lorenzo2b_Sweep, Npc_WhoSeen)
: sweep-reaches? ( e id -- flag )
    my-kind @ 39 = if  nip fiona = if  sees-fiona  else  sees-hewie  then  @ 0<> exit  then
    swap 3 cells + f@ reached? ;
: sweep-hits ( e -- )
    dup fiona sweep-reaches?  swept @ 1 and 0= and if
        dup 4 cells + @  over stumble?  2 pick 6 cells + f@ f>cell  fiona send hit  1 swept @ or swept !
    then
    dup hewie sweep-reaches?  swept @ 2 and 0= and if
        dup 4 cells + @  over 5 cells + @  2 pick stumble?  hewie send blow  2 swept @ or swept !
    then  drop ;
: sweeping ( -- )  root-move  0 key? if  2 move# sweep-hits  then  ended? if  -1 step-done !  then ;

\ ---- $1000: sinking to come up by her (Lorenzo2_StartSink / StateSink) ----
: sinking-behind ( -- )
    root-move  ended? 0= if  exit  then
    way-to-her fdup 20e f< if  fdrop goal-here  else  10e f- goal-along  then
    $1305 ['] playing-out go-under  ['] under $1000 act! ;
: sink-start ( -- )
    way-to-her fdup 30e f< if
        f0< if  stand-watch  else  1 combo-start  then  exit
    then  fdrop
    $1303 play-now  ['] sinking-behind $1000 act! ;
\ ---- $1001: stalking from below (Lorenzo2_StartStalkBelow / StateApproach / StateRise) ----
: sinking-to-her ( -- )
    root-move  ended? 0= if  exit  then
    way-to-her fdup f0< if  fdrop goal-here  else  5e f- 0e fmax goal-along  then
    $E02 ['] sweeping go-under  ['] under $1000 act! ;
: below-start ( -- )
    way-to-her f0< if  stand-watch exit  then
    $1304 play-now  ['] sinking-to-her $1000 act! ;
\ ---- $1002: his reach from afar (Lorenzo2_StartGrab / StateGrab) ----
: grabbing ( -- )
    root-move
    12 key? if  goal quarry body-at vec-copy  then
    0 key? if
        3 move# >r  r@ 3 cells + f@ goal reaches? if
            r@ 4 cells + @  r@ stumble?  r@ 6 cells + f@ f>cell  fiona send hit
        then
        r@ 3 cells + f@ goal reaches-dog? if  r@ 4 cells + @  r@ 5 cells + @  r@ stumble?  hewie send blow  then
        r> drop
        0  goal vec@ 0e 1e 0e mag-burst-new particles-engines   \ (its burst there: BurstA)
    then
    ended? if  -1 step-done !  then ;
: grab-start ( -- )  goal quarry body-at vec-copy  $E01 play-now  ['] grabbing $1000 act! ;
create lorenzo2-acts  ' sink-start , ' below-start , ' grab-start ,

\ ---- his blows' effects (Lorenzo2_SlamDust, Lorenzo2_BlowSparks): his slam ($2301) at its key,
\ eight grey dust clouds at his hand (bone $32), 320..640 big; his blows ($1904 / $E00 / $E04 /
\ $E05 at his hand, $E06 at bone $28) at their effect key ($20), seven sparks about it ----
$32 constant his-hand   $28 constant his-arm
create spark-offsets  0.866e f, 0.866e f, 0e f,   -0.866e f, -0.866e f, 0e f,   0.866e f, -0.866e f, 0e f,
    -0.866e f, 0.866e f, 0e f,   0e f, 0e f, 1e f,   0e f, 0e f, -1e f,   0e f, 0e f, 0e f,
create at-bone 12 allot
: slam-dust ( -- )
    anim @ $2301 <>  0 key? 0= or if  exit  then
    my-model @ his-hand bone-pos at-bone vec!
    8 0 do  2 $50 $50 $50  rnd 320e f* f>s 320 +  at-bone vec@ burst-new particles-engines  loop ;
: spark-bone ( -- bone | -1 )
    anim @ case
        $1904 of  his-hand  endof   $E00 of  his-hand  endof   $E04 of  his-hand  endof
        $E05 of  his-hand  endof    $E06 of  his-arm  endof
        >r -1 r>
    endcase ;
: blow-sparks ( -- )
    spark-bone dup 0<  effect-key? 0= or if  drop exit  then
    my-model @ swap bone-pos at-bone vec!
    7 0 do
        rnd 6e f* f>s 12 +
        i 3 * floats spark-offsets +  dup f@ at-bone sf@ f+  dup float+ f@ at-bone 4 + sf@ f+  2 floats + f@ at-bone 8 + sf@ f+
        1e  0.5e rnd 0.5e f* f+  1.1e rnd 0.05e f* f+  spark
    loop ;

\ ---- his frame (Lorenzo2_Update): his "rage" from the panic's first stage, out of it at 0; his
\ reaches by it; his blows' effects ----
: lorenzo2-frame ( -- )
    slam-dust  blow-sparks
    her-stage @ 0= if  0 raging !
    else  raging @ 0=  reeling @ 0= and  my-mode @ 0= and if  -1 raging !  then  then
    raging @ if  18e 12e  else  hard-mode? if  40e 30e  else  70e 60e  then  then  reach-k f!  attack-range f! ;
