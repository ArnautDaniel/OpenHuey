\ fiona/defend.fs - Fiona defending herself (F4; src/game/fiona.c Fiona_StateShoveStart /
\ StateStrike / StateShove, Fiona_StateKickStart / StateKick; Progress_PlayerButtons'
\ square, Fiona_StateBlock 8): the square button, while she's free and not past the panic's
\ stage 3 - standing, walking, resting, or just set off running, a shove ($E00: her hand, damage
\ 1); running or turning to flee, a kick ($E01: her body, damage 5, and it may make him
\ stumble). A blow is a `blow` to whom it reaches; struck home she's held still 5 frames.
\ Hewie is told when her shove meets a wall or her kick ends, without him being struck
\ (Fiona_HewieReact 10 / 11).
IN: fiona.defend
USING: engine vectors game-state actors messages common keys facts flag-names rumble items items.names fiona.state fiona.anims fiona.sounds fiona.model fiona.moving fiona.slam fiona.fear fiona.controls fiona.looks ;

\ the shove's steps move her every frame - held still by a blow that landed, its frozen frame's
\ step goes on (Fiona_StateShoveStart / StateStrike / StateShove: Character_RootMoveMasked
\ unconditionally; the kick stops instead)
: moving ( -- )  root-move ;

\ ---- whom a blow at a point reaches (Progress_CharNear: within their height and radius,
\ widened by the margin) ----
: near-point? ( v id -- flag ) ( F: margin -- )
    {: v id F: m :}  id 0< if  false exit  then
    v v.y sf@  id body-at v.y sf@ m f-  f> 0= if  false exit  then                               \ (above their feet)
    v v.y sf@  id body-at v.y sf@  id body-dims fswap fdrop f+ m f+  f< 0= if  false exit  then   \ (below their top)
    v id body-at vec-dist-xz  id body-dims fdrop m f+  f< ;                                      \ (within their radius)
: touching? ( id -- flag )  dup 0< if  drop false exit  then  self swap 0e 0e bodies-touching? ;

\ a blow sent: to the stalker (1) and Hewie (2) it reaches, each once a blow
: blow-at ( damage -- ) ( F: x y z reach -- )   \ to the homunculi a blow reaches there (reach -1: those touching her)
    {: damage F: x y z reach :}  her-blow-n @  damage  x f>cell y f>cell z f>cell reach f>cell  broadcast fiona-blow ;
: strike ( kind damage how mask -- )   \ (each struck once a blow: 1 the stalker, 2 Hewie)
    her-struck @ invert and  dup her-struck @ or her-struck !  {: kind damage how mask :}
    mask 1 and if  kind damage how stalker-in-room send blow  then
    mask 2 and if  kind damage how dog-in-room send blow  then ;

\ ---- the shove (Fiona_StateStrike): at its key, from her hand - or in the two special costumes
\ (progress variable $26: 6, 7) their own strike - a blow to whom it reaches. Her hand reaches 2
\ round, damage 1; costume 6's strike 6 ahead of its bone $21, reach 3, damage 5; costume 7's 10
\ to the side of its bone $23, reach 10, a hard blow of 10. Her boots (the equipment's slot 0):
\ metal (1 in 10 a stumble, damage 2), silver (a hard blow of 5, 1 in 5 a stumble), burst shoes
\ (1 in 10 a hard blow of 50 - the special costumes' 1 in 10: 50 / 100 - with a burst on whom it
\ meets); in a special costume 1 in 5 a stumble too. The damage by Fortis's boost (x3). Into
\ a wall (not the special strikes) it ends: blended into standing ----
create hand /vec allot
\ her blow, as worked out (blow-power): its kind, damage, how (a stumble), a big one (a burst at
\ them), its reach - the shove's working values, read again as it lands
variable blow-kind  variable blow-dmg  variable blow-how  variable blow-big  fvariable blow-reach
$8000 constant stumble-blow
: form ( -- n )  her-costume @ ;
: special? ( -- flag )  form dup illegal-outfit = swap cowgirl-outfit = or ;
: boots ( -- id )  progress pr.equipped sl@ ;
: chance? ( F: p -- flag )  rnd f> ;
: stumbles ( -- )  stumble-blow blow-how ! ;
: boosted ( n -- n' )   \ (Fortis's: her blows x3 while it lasts - items.fs)
    blow-boost boost-on? if  s>f  blow-boost boost-value f* f>s  then ;
: blow-power ( -- )
    0 blow-how !  0 blow-big !
    form cowgirl-outfit = if  kick-blow blow-kind !  10 blow-dmg !
        boots burst-shoes = 0.1e chance? and if  100 blow-dmg !  -1 blow-big !  then  0.2e chance? if  stumbles  then  exit  then
    form illegal-outfit = if  shove-blow blow-kind !  5 blow-dmg !
        boots burst-shoes = 0.1e chance? and if  50 blow-dmg !  -1 blow-big !  then  0.2e chance? if  stumbles  then  exit  then
    shove-blow blow-kind !  1 blow-dmg !
    boots case
        burst-shoes of  0.1e chance? if  kick-blow blow-kind !  50 blow-dmg !  -1 blow-big !  then  endof
        silver-boots of  0.2e chance? if  stumbles  then  kick-blow blow-kind !  5 blow-dmg !  endof
        metal-boots of  0.1e chance? if  stumbles  then  2 blow-dmg !  endof
    endcase ;
: at-bone ( bone -- ) ( F: ox oz -- )   \ (an offset in the bone's own axes: the original's bone matrix applied)
    {: bone F: ox oz :}  her-model @ bone  ox 0e oz  bone-point  hand vec! ;
: blow-point ( -- )
    form cowgirl-outfit = if  -10e 0e hand-bone at-bone  10e blow-reach f! exit  then
    form illegal-outfit = if  0e 6e hand-bone at-bone  3e blow-reach f! exit  then
    her-model @ 9 bone-pos hand vec!  2e blow-reach f! ;
: shove-done ( -- )  0 her-run-t !  to-idle ;
\ into a wall (Fiona_StateShove): blended into standing over 10 (Motion_PlayWith: flags $101 -
\ looping, feet planted - once the shove has faded in); once that has faded in Hewie is told
\ (if not struck) and she stands
: wall-end ( -- )  stand-anim -1 10 $101 start ;
: shove-end ( -- )
    recoil-down
    settled? if
        anim@ shove-anim = if  wall-end
        else  her-struck @ 2 and 0= if  shove-reaction hewie-id send reaction  then  shove-done exit  then
    then
    moving ;
\ ---- (OpenHuey's, not the original's - the user's call, 2026-10-28) the shove reaches: her
\ shove animation has next to no root step, so a step back left her short of him. She lunges 12
\ ahead over its first 30 frames (at its pace - slower when frightened), until a blow lands; and
\ the stalker is struck the moment her hand reaches him or she touches him, not only at its key.
\ Hewie is struck at its key alone, as before ----
12e fconstant lunge-length   30 constant lunge-frames
variable lunge-t
fvariable shove-pace
: lunge ( -- )
    her-struck @  her-recoil @ or  lunge-t @ lunge-frames >= or if  exit  then
    1 lunge-t +!
    0e  lunge-length lunge-frames s>f f/ shove-pace f@ f*  her-yaw f@ rotate-by move-by ;
: reaches-him? ( -- flag )   \ (her hand, or herself: 1 margin round, to his height)
    blow-point  hand stalker-in-room blow-reach f@ near-point?
    stalker-in-room dup 0< if  drop false  else  self swap 1e 0e bodies-touching?  then  or ;
: blow-lands ( mask -- )   \ (her blow on whom it reaches, worked out afresh)
    blow-power  blow-dmg @ boosted blow-dmg !
    blow-kind @ blow-dmg @ blow-how @ rot strike ;
\ met before its key, her shove goes on from there: the rest of it as after a blow at its key,
\ no longer open to him than the original's (its key at frame 19 - Motion_EventFlags dt -1; the
\ special costumes' strikes on as they are)
18e fconstant shove-key-frame
: to-key ( -- )  special? if  exit  then  her act.frame sf@ shove-key-frame f< if  shove-key-frame her act.frame sf!  then ;
: meet-him ( -- )   \ (the stalker met before or after her shove's key)
    her-struck @ 1 and  reaches-him? 0= or if  exit  then
    to-key  1 blow-lands ;
: shoving ( -- )   \ (Fiona_StateStrike: at its end she stands)
    recoil-down
    ended? if  shove-done exit  then
    moving  lunge
    events-last 2 and 0= if  meet-him exit  then   \ (Fiona_StateStrike: dt -1)
    blow-point
    hand true slam-door if  exit  then                     \ (a door there slammed)
    special? 0=  hand $20018 body-tri-to 0< and if  settled? if  wall-end  then  ['] shove-end her-act ! exit  then   \ (a wall before her hand)
    0  hand stalker-in-room blow-reach f@ near-point? reaches-him? or 1 and or  hand dog-in-room blow-reach f@ near-point? 2 and or  {: mask :}
    mask if  mask blow-lands  then
    blow-dmg @ hand vec@ blow-reach f@ blow-at ;
\ the swing begun (Fiona_StateShoveStart, once what she was doing has faded out - or at once,
\ the stalker met as she sets off)
: swing ( -- )
    special? if  1e shove-pace f!  shove-anim -1 play-table  else   \ (the special costumes: their own, at its pace)
        1e  her-recovery @ $1C3 < 0= if  fdrop 3150 her-recovery @ - s>f 1800e f/ 1.5e f*  then
        1e  her-fear f@ 40e f> if  fdrop 160e her-fear f@ f- 120e f/  then
        fmin shove-pace f!
        shove-anim 2 play-table  shove-pace f@ weight!   \ (slower by her fright: blended with motion 2, kept in step)
    then
    1 her-blow-n +!  ['] shoving her-act ! ;
: shove-start ( -- )
    moving  lunge
    her-struck @ 1 and 0=  reaches-him? and if  swing meet-him exit  then
    settled? if  swing  then ;

\ ---- the kick: a step ahead while it fades in; while its key is clear it meets whom it
\ touches (a kick, damage 5 - x3 with Fortis's - he may stumble); at its end her fear up 10 ----
create ahead-pt /vec allot   \ (her radius ahead)
: ahead-pt! ( -- )  her-yaw f@ self body-dims fdrop ahead-pt her-at vec-ahead ;
: kicking ( -- )
    recoil-down
    her-recoil @ 0= if  settled? 0= if  0e 1.0756e her-yaw f@ rotate-by move-by  else  root-move  then  then
    ended? if
        her-struck @ 2 and 0= if  kick-reaction hewie-id send reaction  then
        10e add-fear  0 her-run-t !  to-idle exit
    then
    events-now 2 and if  exit  then
    ahead-pt! ahead-pt true slam-door if  exit  then                 \ (a door ahead slammed)
    0  stalker-in-room touching? 1 and or  dog-in-room touching? 2 and or
    {: mask :}  mask if  kick-blow 5 boosted 1 mask strike  then   \ (Fortis's x3 too: Fiona_StateKick's 5 x +0xA04)
    5 her-at vec@ -1e blow-at ;   \ (the creatures 5 as it is)
: kick-start ( -- )
    root-move
    settled? 0= if  exit  then
    kick-sound 0 her-sound  kick-anim -1 play-table  1 her-blow-n +!  0 her-struck !  ['] kicking her-act ! ;

\ ---- the square button (Progress_PlayerButtons, Fiona_StateBlock 8) ----
: shove-sub ( -- n )  $1A ;   : kick-sub ( -- n )  $1B ;   \ (her sub defending)
: start-kick ( -- )  kick-doing her-doing !  defend-mode her-mode !  kick-sub her-sub !  ['] kick-start her-act ! ;
: start-shove ( -- )  0 her-struck !  0 lunge-t !  1e shove-pace f!  shove-doing her-doing !  defend-mode her-mode !  shove-sub her-sub !  ['] shove-start her-act ! ;
\ a shove or a kick by her pose (Fiona_StateBlock 8): fleeing, a kick; standing, walking, resting -
\ or just set off running, or running still frightened - a shove; running, a kick
: defend-by-pose ( -- )
    her-doing @ flee-doing = if  start-kick exit  then
    group {: g :}
    g resting-group =  g walking-group = or  g standing-group = or
    g running-group =  her-recovery @ 0<> her-run-t @ $3D < or  and  or if  start-shove exit  then
    g running-group = if  start-kick  then ;
: try-defend ( -- )
    square button-pressed? 0= if  exit  then
    hands-off?  her-mode @ own-mode <> or  still? or  her-panic-stage @ panicking >= or  no-flee state-flag? or if  exit  then
    defend-by-pose ;   \ (still?: Fiona_IsIdle - not turning to push, in a panic attack or out of breath)
\ her blow struck home (`blow-taken`): held still; her sound ($90 a kick, $8F a shove; Hewie
\ counted as kicked - with his trust, H3)
\ her shove's: costume 6 its own ($22), costume 7 none - its sparks instead (a blood spray at
\ one of four of their bones); a big blow a burst at them (Fiona_ShoveEffect)
: spray-on ( id -- )   \ (the original picks one of four of their bones; here a height on them, at random)
    {: id :}  id body? 0= if  exit  then
    id body-tri 0  id body-at vec@  {: F: x y z :}
    x  y  id body-dims fswap fdrop  0.4e rnd 0.5e f* f+ f* f+  z  spray ;
: by-dog? ( -- flag )  sender hewie-id = ;
: dog-mistreated ( -- )   \ (her blow on Hewie: 3 more in his treatment - the progress' +0xFB6)
    progress pr.hewie-mistreated dup l@ 3 + 0 max 10000 min swap l! ;
: struck-home ( -- )
    by-dog? if  dog-mistreated  then
    her-sub @ kick-sub = if   \ (her kick landed: the rumble only on the stalker or a creature)
        by-dog? 0= if  $C0 12 fiona-jolt  then  kick-hit-sound 0 her-sound  recoil exit
    then
    form illegal-outfit = if  illegal-hit-sound 0 her-sound  else  form cowgirl-outfit <> if  shove-hit-sound 0 her-sound  then  then
    form cowgirl-outfit = if  sender spray-on  then
    blow-big @ if  sender body-at vec@ puffs  then
    recoil ;

\ ---- her point of interest (Fiona_PointOfInterest: the story's zones test it, cond 0x37): her
\ kick outside its key window, 10 up and her radius ahead; her shove at its key - her hand,
\ costume 6's strike 6 ahead of its bone (at 2.5 up), costume 7's 10 ahead (2.5 up); else none ----
create poi /vec allot
: point-ahead ( F: up ahead -- )   \ (from her place, turned by her heading)
    {: F: up ahead :}  0e ahead her-yaw f@ rotate-by  {: F: dx dz :}
    her-at v.x sf@ dx f+ poi v.x sf!  her-at v.y sf@ up f+ poi v.y sf!  her-at v.z sf@ dz f+ poi v.z sf! ;
: tell-point ( -- )
    her-mode @ defend-mode <> if  body-point-off exit  then
    her-sub @ kick-sub = if
        events-now 2 and if  body-point-off exit  then
        10e self body-dims fdrop point-ahead  poi vec@ body-point! exit
    then
    her-sub @ shove-sub <>  events-last 2 and 0= or if  body-point-off exit  then   \ (dt -1, as her shove's)
    form cowgirl-outfit = if  2.5e 10e point-ahead  poi vec@ body-point! exit  then
    form illegal-outfit = if  0e 6e hand-bone at-bone  hand vec@ fdrop her-at v.y sf@ 2.5e f+ fswap body-point! exit  then
    her-model @ 9 bone-pos body-point! ;
