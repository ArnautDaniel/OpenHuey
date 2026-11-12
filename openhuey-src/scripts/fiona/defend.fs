\ fiona/defend.fs - Fiona defending herself (F4; src/game/fiona.c Fiona_StateShoveStart /
\ StateStrike / StateShove - despite the decomp's names, her kick - and Fiona_StateKickStart /
\ StateKick - her shove; Progress_PlayerButtons' square, Fiona_StateBlock 8). The square button,
\ while she's free and not past the panic's stage 3: standing, walking, resting, or just set off
\ running, a kick ($E00: her left foot swings out, bone 9, damage 1); running, or stepping back,
\ a shove ($E01: her arms and body, damage 5, and it may make him stumble). A blow is a `blow` to
\ whom it reaches; struck home she's held still 5 frames. Hewie is told when her kick meets a wall
\ or her shove ends, without him being struck (Fiona_HewieReact 10 / 11).
IN: fiona.defend
USING: engine vectors game-state actors messages common keys facts flag-names rumble items items.names fiona.state fiona.anims fiona.sounds fiona.model fiona.moving fiona.slam fiona.fear fiona.controls fiona.looks ;

\ the kick's steps move her every frame - held still by a blow that landed, its frozen frame's
\ step goes on (Fiona_StateShoveStart / StateStrike / StateShove: Character_RootMoveMasked
\ unconditionally; the shove stops instead)
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

\ ---- the kick (Fiona_StateStrike): in its key window, from her foot - or in the two special
\ costumes (progress variable $26: 6, 7) their own strike - a blow to whom it reaches. Her foot
\ reaches 2 round, damage 1; costume 6's strike 6 ahead of its bone $21, reach 3, damage 5; costume
\ 7's 10 to the side of its bone $23, reach 10, a heavy blow of 10. Her boots (the equipment's slot
\ 0): metal (1 in 10 a stumble, damage 2), silver (a heavy blow of 5, 1 in 5 a stumble), burst
\ shoes (1 in 10 a heavy blow of 50 - the special costumes' 1 in 10: 50 / 100 - with a burst on
\ whom it meets); in a special costume 1 in 5 a stumble too. The damage by Fortis's boost (x3).
\ Into a wall (not the special strikes) it ends: blended into standing ----
create blow-spot /vec allot
\ her blow, as worked out (blow-power): its kind, damage, how (a stumble), a big one (a burst at
\ them), its reach
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
    form cowgirl-outfit = if  heavy-blow blow-kind !  10 blow-dmg !
        boots burst-shoes = 0.1e chance? and if  100 blow-dmg !  -1 blow-big !  then  0.2e chance? if  stumbles  then  exit  then
    form illegal-outfit = if  light-blow blow-kind !  5 blow-dmg !
        boots burst-shoes = 0.1e chance? and if  50 blow-dmg !  -1 blow-big !  then  0.2e chance? if  stumbles  then  exit  then
    light-blow blow-kind !  1 blow-dmg !
    boots case
        burst-shoes of  0.1e chance? if  heavy-blow blow-kind !  50 blow-dmg !  -1 blow-big !  then  endof
        silver-boots of  0.2e chance? if  stumbles  then  heavy-blow blow-kind !  5 blow-dmg !  endof
        metal-boots of  0.1e chance? if  stumbles  then  2 blow-dmg !  endof
    endcase ;
: at-bone ( bone -- ) ( F: ox oz -- )   \ (an offset in the bone's own axes: the original's bone matrix applied)
    {: bone F: ox oz :}  her-model @ bone  ox 0e oz  bone-point  blow-spot vec! ;
: blow-point ( -- )
    form cowgirl-outfit = if  -10e 0e hand-bone at-bone  10e blow-reach f! exit  then
    form illegal-outfit = if  0e 6e hand-bone at-bone  3e blow-reach f! exit  then
    her-model @ 9 bone-pos blow-spot vec!  2e blow-reach f! ;
\ into a wall (Fiona_StateShove): blended into standing over 10 (Motion_PlayWith: flags $101 -
\ looping, feet planted - once the kick has faded in); once that has faded in Hewie is told
\ (if not struck) and she stands
: kick-done ( -- )  to-idle ;
: wall-end ( -- )  stand-anim -1 10 $101 start ;
: kick-end ( -- )
    settled? if
        anim@ kick-anim = if  wall-end
        else  her-struck @ 2 and 0= if  kick-reaction hewie-id send reaction  then  kick-done  then
    then
    moving ;
: boosted-power ( -- )  blow-power  blow-dmg @ boosted blow-dmg ! ;
\ costume 7's revolver fires, once a swing (StrikeMark: its flash along the barrel from her bone
\ $23, 3.5 out and 1 out; then smoke)
variable fired
: fire ( -- )
    form cowgirl-outfit <>  fired @ or if  exit  then
    -1 fired !
    her-model @ hand-bone -3.5e 0e 1e  her-model @ hand-bone -1e 0e 1e bone-point  strike-mark ;
: kicking ( -- )   \ (Fiona_StateStrike: its root step, at its end she stands; its key a frame behind)
    moving
    ended? if  kick-done exit  then
    events-last 2 and 0= if  exit  then
    fire
    blow-point
    blow-spot true slam-door {: slammed :}         \ (a door there slammed; a wall before it comes first)
    special? 0=  blow-spot $20018 body-tri-to 0< and if  settled? if  wall-end  then  ['] kick-end her-act ! exit  then
    slammed if  exit  then
    0  blow-spot stalker-in-room blow-reach f@ near-point? 1 and or  blow-spot dog-in-room blow-reach f@ near-point? 2 and or  {: mask :}
    boosted-power
    mask if  blow-kind @ blow-dmg @ blow-how @ mask strike  then
    blow-dmg @ blow-spot vec@ blow-reach f@ blow-at ;
\ the kick begun (Fiona_StateShoveStart), once what she was doing has faded out: slower by her
\ fright - blended with motion 2, kept in step
: swing ( -- )
    special? if  kick-anim -1 play-table  else
        1e  her-recovery @ $1C3 < 0= if  fdrop 3150 her-recovery @ - s>f 1800e f/ 1.5e f*  then
        1e  her-fear f@ 40e f> if  fdrop 160e her-fear f@ f- 120e f/  then
        fmin  kick-anim 2 play-table  weight!
    then
    0 her-struck !  0 fired !  1 her-blow-n +!  ['] kicking her-act ! ;
: kick-start ( -- )  settled? if  swing  then  moving ;
\ costumes 6 and 7's weapons, in her hand only for her swing (their models' +0x30 as it starts,
\ +0x2C as it ends or anything else takes her; hidden as they load): part 25, part 35
: weapon-part ( -- part | -1 )
    form case  illegal-outfit of  25  endof  cowgirl-outfit of  35  endof  >r -1 r>  endcase ;
: weapon-shown ( -- )
    weapon-part {: part :}  part 0<  her-model @ 0< or if  exit  then
    her-model @ part  her-act @ dup ['] kicking = swap ['] kick-end = or  actor-part-show ;

\ ---- the shove (Fiona_StateKickStart / StateKick): once what she was doing has faded out, her cry,
\ its animation held at its first frame while it fades in (Motion_PlayOwnBlend) - she carries on
\ 1.0756 a frame, then its root step; while its key is clear it meets whom it touches (damage 5 -
\ x3 with Fortis's - he may stumble); at its end her fear up 10 ----
create ahead-pt /vec allot   \ (her radius ahead)
: ahead-pt! ( -- )  her-yaw f@ self body-dims fdrop ahead-pt her-at vec-ahead ;
: shoving ( -- )
    her-recoil @ 0= if  settled? 0= if  0e 1.0756e her-yaw f@ rotate-by move-by  else  root-move  then  then
    ended? if
        her-struck @ 2 and 0= if  shove-reaction hewie-id send reaction  then
        10e add-fear  0 her-run-t !  to-idle exit
    then
    events-now 2 and if  exit  then
    ahead-pt! ahead-pt true slam-door if  exit  then                 \ (a door ahead slammed)
    0  stalker-in-room touching? 1 and or  dog-in-room touching? 2 and or
    {: mask :}  mask if  heavy-blow 5 boosted 1 mask strike  then   \ (Fortis's x3 too: Fiona_StateKick's 5 x +0xA04)
    5 her-at vec@ -1e blow-at ;   \ (the creatures 5 as it is)
: shove-start ( -- )
    root-move
    settled? 0= if  exit  then
    shove-sound 0 her-sound  shove-anim play-own  1 her-blow-n +!  0 her-struck !  ['] shoving her-act ! ;

\ ---- the square button (Progress_PlayerButtons, Fiona_StateBlock 8) ----
: kick-sub ( -- n )  $1A ;   : shove-sub ( -- n )  $1B ;   \ (her sub defending)
: start-shove ( -- )  shove-doing her-doing !  defend-mode her-mode !  shove-sub her-sub !  ['] shove-start her-act ! ;
: start-kick ( -- )  kick-doing her-doing !  defend-mode her-mode !  kick-sub her-sub !  ['] kick-start her-act ! ;
\ a kick or a shove by her pose (Fiona_StateBlock 8): stepping back, a shove; standing, walking,
\ resting - or just set off running, or running still frightened - a kick; running, a shove
: defend-by-pose ( -- )
    her-doing @ backstep-doing = if  start-shove exit  then
    group {: g :}
    g resting-group =  g walking-group = or  g standing-group = or
    g running-group =  her-recovery @ 0<> her-run-t @ $3D < or  and  or if  start-kick exit  then
    g running-group = if  start-shove  then ;
: try-defend ( -- )
    square button-pressed? 0= if  exit  then
    hands-off?  free-to-act? 0= or  her-panic-stage @ panicking >= or  no-flee state-flag? or if  exit  then
    defend-by-pose ;
\ her blow struck home (`blow-taken`): held still; her sound ($90 a shove, $8F a kick; Hewie
\ counted as mistreated - with his trust, H3)
\ her kick's: costume 6 its own ($22), costume 7 none - its sparks instead (a blood spray at
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
    her-sub @ shove-sub = if   \ (her shove landed: the rumble only on the stalker or a creature)
        by-dog? 0= if  $C0 12 fiona-jolt  then  shove-hit-sound 0 her-sound  recoil exit
    then
    form illegal-outfit = if  illegal-hit-sound 0 her-sound  else  form cowgirl-outfit <> if  kick-hit-sound 0 her-sound  then  then
    form cowgirl-outfit = if  sender spray-on  then
    blow-big @ if  sender body-at vec@ puffs  then
    recoil ;

\ ---- her point of interest (Fiona_PointOfInterest: the story's zones test it, cond 0x37): her
\ shove outside its key window, 10 up and her radius ahead; her kick at its key - her foot,
\ costume 6's strike 6 ahead of its bone (at 2.5 up), costume 7's 10 ahead (2.5 up); else none ----
create poi /vec allot
: point-ahead ( F: up ahead -- )   \ (from her place, turned by her heading)
    {: F: up ahead :}  0e ahead her-yaw f@ rotate-by  {: F: dx dz :}
    her-at v.x sf@ dx f+ poi v.x sf!  her-at v.y sf@ up f+ poi v.y sf!  her-at v.z sf@ dz f+ poi v.z sf! ;
: tell-point ( -- )
    her-mode @ defend-mode <> if  body-point-off exit  then
    her-sub @ shove-sub = if
        events-now 2 and if  body-point-off exit  then
        10e self body-dims fdrop point-ahead  poi vec@ body-point! exit
    then
    her-sub @ kick-sub <>  events-last 2 and 0= or if  body-point-off exit  then   \ (dt -1, as her kick's)
    form cowgirl-outfit = if  2.5e 10e point-ahead  poi vec@ body-point! exit  then
    form illegal-outfit = if  0e 6e hand-bone at-bone  blow-spot vec@ fdrop her-at v.y sf@ 2.5e f+ fswap body-point! exit  then
    her-model @ 9 bone-pos body-point! ;
