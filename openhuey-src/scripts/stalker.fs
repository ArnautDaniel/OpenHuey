\ stalker.fs - the stalkers (docs/subsystems/stalker.md; src/game/pursuer.c, debilitas.c): one
\ actor per stalker in the game. The family's behaviour (`stalking`) and each one's over it: his
\ tables, his numbers, his model; in the house - placed, standing, growling, telling the danger
\ he is about; searching the room (stalker/search.fs: his modes, his route of stops, walking).
IN: stalker
USING: engine game-state actors messages common facts flag-names room-names paths vectors hewie-names stalker-names stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.doorway stalker.tables stalker.attack stalker.chase stalker.leads stalker.pursue stalker.grab stalker.struck stalker.bitten stalker.riccardo stalker.lorenzo2 stalker.ladder stalker.head stalker.names stalker.anims stalker.sounds stalker.scripted floor-flags ;

: him ( -- a )  my-model @ actor ;   \ (his model's fields)
: played? ( -- flag )  in-game @ if  my-room room-id =  else  false  then ;   \ (Npc_InPlayedRoom)

\ his model where his body is, shown in the room being played (Daniella on her back: her hair
\ and back by her animation - the engine's, DaniellaModel_Vt3C)
2 constant play-slot   \ (the stalker in play: his sounds in bank 7, saved)
: figure? ( -- flag )  loaded @  my-slot @ play-slot <> and ;   \ (in slots 3..5: a scene's figure)
: played-here? ( -- flag )   \ (in the room being played: the stalker in play, or a figure placed there)
    in-game @ if  played? exit  then
    figure? self body? and if  self body-room room-id =  else  false  then ;
: pose ( -- )
    my-at v.x sf@ him act.x sf!  my-at v.y sf@ him act.y sf!  my-at v.z sf@ him act.z sf!
    self body-yaw him act.yaw sf!
    played-here? under-t @ 0= and unseen @ 0= and hidden @ 0= and 0<> 1 and him act.visible l! ;   \ (under the floor: Lorenzo's other class;
    \ hidden by the story: its 0x1F - the original's +0x29)

\ his body set by his feet (Pursuer_ModelUpdate: his model's +0x40 and +0x4C, the engine's legs-fit -
\ humanlegs.c): in the room played, on the floor - but on a ladder, or a script's walk through
\ blocked floor - by his stance's points along his step, his feet planted; else at his place
: feet-on? ( -- flag )  my-move-mode @ on-ladder-mode <>  script-held @ noclipped @ and 0= and ;
: feet-way ( F: -- back front )
    anim @ walk-slow-anim = if  feet-slow-back f@ feet-slow-front f@  else  feet-back f@ feet-front f@  then ;
\ Lorenzo (LorenzoModel_BodyFrames: the same two numbers, the floor ahead and behind) tilted in
\ his chair by the floor and his head turned by his look, no feet
: fit-legs ( -- )
    played? 0= my-model @ 0< or if  exit  then
    self body-tri  feet-on?  {: tri on :}
    on if  feet-way  else  0e 0e  then
    chaired @ if  head-pitch f@ head-yaw f@  my-model @ tri my-floor @ chair-fit exit  then
    my-model @ tri my-floor @ on 1 and legs-fit ;
\ his legs (each model class's Loaded: +0x890..+0x8AC) by his model's file - Lorenzo's other class
\ (O_LRY: Lorenzo2Model) his left from $12; Lorenzo (O_LRO) not a human's model (LorenzoModel: its
\ own floor), none; the others' 2..5 and 6..9
: model-of? ( addr len dir len -- flag )  {: a n d m :}  n m < if  false exit  then  a m d m compare 0= ;   \ (its file in that folder)
: leg-bones ( addr len -- right left )
    2dup s" O_LRO" model-of? if  2drop -1 -1 exit  then
    s" O_LRY" model-of? if  2 $12  else  2 6  then ;
: feet-fit ( F: slow-back slow-front back front -- )   \ (his Setup's +0x1694.. +0x16A0)
    feet-front f!  feet-back f!  feet-slow-front f!  feet-slow-back f! ;

\ ---- in the game and out (Pursuer_Activate: waiting about, his first animation) ----
\ in the story's character slot 2 (CharLoad_Partner: loaded and started, not yet anywhere) and
\ out of it; while loaded the cutscenes find its model by its kind (their cast)
: file! ( addr len -- )  31 min dup my-file c!  my-file 1+ swap move ;
: file ( -- addr len )  my-file 1+ my-file c@ ;
\ a scene's figure (slots 3..5) wears its kind's other model (Pursuer_ModelFiles out of slot 2):
\ its _200 pack - the scenes' own motions, $9000 on - with the play model's textures (hard: the H
\ ones; the second set, _200.TEX, not yet); back in slot 2 his own again. (Debilitas 7 and 27's,
\ O_DB2/DB2_200.PCK, isn't on the disc: none)
: figure-base ( kind -- addr len | 0 0 )
    case
        debilitas-kind of  s" O_DB0/DB0"  endof    debilitas-6-kind of  s" O_DB0/DB0"  endof
        daniella-kind of  s" O_DNL/DNL"  endof     kind34-kind of  s" O_DNL/DNL"  endof
        kind35-kind of  s" O_DNL/DNL"  endof       kind36-kind of  s" O_DNL/DNL"  endof
        riccardo-kind of  s" O_RCG/RCG"  endof     tintstalker-kind of  s" O_RCT/RCT"  endof
        kind37-kind of  s" O_RCT/RCT"  endof       lorenzo-kind of  s" O_LRO/LRO"  endof
        lorenzo2-kind of  s" O_LRY/LRY"  endof     kind39-kind of  s" O_LRY/LRY"  endof
        kind12-kind of  s" O_LRF/LRF"  endof       \ (no hard textures of its own)
        >r 0 0 r>
    endcase ;
create figure-pck 32 allot   create figure-tex 32 allot
: joined ( addr len addr2 len2 buf -- addr' len' )  {: a n a2 n2 buf :}  a buf n move  a2 buf n + n2 move  buf n n2 + ;
: figure-textures ( -- addr len )   \ (its base's _000, hard: _000H)
    hard-mode?  my-kind @ kind12-kind <> and if  s" _000H"  else  s" _000"  then ;
: wear-figure ( -- )
    figure-worn @ if  exit  then
    my-kind @ figure-base dup 0= if  2drop exit  then  {: a n :}
    a n s" _200" figure-pck joined  a n figure-textures figure-tex joined  actor-load-textured {: id :}
    0 him act.visible l!  my-model @ play-model !  id my-model !  -1 figure-worn !
    my-motions 2@ dup if  id -rot motion-table  else  2drop  then
    0 him act.visible l!  1 him act.alive l!  id file leg-bones human-legs ;
: wear-own ( -- )
    figure-worn @ 0= if  exit  then
    my-model @ actor-free  play-model @ my-model !  0 figure-worn !  0 him act.visible l! ;
: load ( slot -- )   \ (slots 3..5 have no bank: Daniella as a scene's figure mid-Debilitas leaves his)
    dup my-slot !  -1 loaded !  dup play-slot = if  wear-own  else  wear-figure  then
    my-kind @ my-model @ cast-as  play-slot = if  7 file sound-bank drop  then ;
\ nothing of the story's on him (Character_Activate / Pursuer_EventReset's): not held nor posed,
\ no look of its, through no blocked floor, his sounds his own
: unheld ( -- )
    0 script-held !  0 posed !  0 take-asked !  0 look-on !  -1 sm-who !  0e 0e 0e look-pt vec!  0 door-script !
    0 noclipped !  my-floor @ body-mask! ;
: come-in ( room tri -- )
    loaded @ 0= if  play-slot load  then  forget-leads
    {: room tri :}
    room room-id = if  room tri dup tri-center body-place
    else  room -1 0e 0e 0e body-place  then                         \ (out of sight: in that room, nowhere yet)
    -1 in-game !  0 growl-t !  own-move-mode my-move-mode !  -1 anim !
    hp @ 0> 0= if  hp-max @ hp !  then
    well-cond cond !  0 reeling !  0 hits !
    route-clear  my-path path-clear  waiting-about mode!  stand-anim play-now  stand  settle  pose   \ (Pursuer_PlaceInRoom)
    -1 came-by !  -1 to-shut !  -1 at-ladder !  -1 making-for !  0 doors-n !  0 door-i !  0 away-search !  0 knock-t !  avoid-clear
    door-passed  -1 combo !  0 combo-step !  0 chase-t !  0 in-attack !   \ (Pursuer_Reset's)
    unheld  0 apart !  0 link-t !  out-over  -1 held-door !  0 entering !  0 barging !  -1 push-exit !  0 hunting-dog !  0 stun-t !  0 raging !  0 bitten !  held-by-dog bite-how !  0 dog-t !  0 at-dog !
    away? 0= if  search-start  then ;
: go-out ( -- )  let-her-go  let-door-go  out-over  forget-leads  0 in-game !  body-off  0 him act.visible l! ;

\ ---- the summoner (summoner.fs): told where he is each frame while in slot 2; brought in by it
\ out of sight (Pursuer_IntoRoomByEvent): by a noise after her, else waiting about - with a plan
\ heading for her, without one off by an exit at random ----
: status ( -- )
    in-game @ if  my-room  else  -1  then  my-mode @  in-game @ 0<>  in-game @ if  own-room?  else  false  then
    away-base @  cool-secs @  cond @ down-cond =  s" summoner" actor-named send stalker-status ;   \ (down now: +0xC4 2)
: summoned ( room exit found plan -- )
    {: room e found plan :}
    room -1 come-in  e came-by !  hp-max @ hp !   \ (a stalk afresh: his health full - beaten or worn, he was out a while)
    found if
        her-room my-room = if  search-again  else  heading-for-her mode!  avoid-clear  head-for-her  then
    else
        waiting-about mode!  plan if  avoid-clear  head-for-her  else  route-at-random  then
    then ;

\ ---- his frame (Debilitas_Update: so far the growl, his model) and his part in the danger ----
$1300 constant gesture-anims
: growl ( -- )   \ every 90 frames, standing on his own
    1 growl-t +!  growl-t @ 90 < if  exit  then
    0 growl-t !
    my-move-mode @ own-move-mode = anim @ gesture-anims <> and anim @ stand-about-anims <> and if  growl-sound 7 sound  then ;
: tell-danger ( -- )  my-room my-mode @ cond @ down-cond = danger-id send stalker-here ;   \ (SceneGame_Danger reads his mode, and whether he's down)
\ ---- held by a script of the room (the story's action for him - Pursuer_EventState's 5, only in
\ the room being played: Pursuer_BackToNormal, BackToStance): what he was doing given up - her hand
\ and a door let go, his way out over, Hewie off him -, the story's look off, then standing (his
\ action 1), done once his stance has faded in. Let go (the script over, its move 1:
\ Pursuer_EventReset, StanceByFiona, PickStep): through blocked floor no more, his sounds his own,
\ on as before. (Hewie on him is thrown off - the original has him let go of his own accord.) ----
: taken ( -- )
    let-her-go  let-door-go  out-over  0 reeling !  0 apart !  no-search search-phase !
    bitten @ if  0 bitten !  hewie send shaken-off  then
    0 look-on !  -1 sm-who !  0e 0e 0e look-pt vec!  -1 script-held !  my-path path-clear
    stand-to ;
:noname ( -- )  unheld  0 hushed !  0 sm-done !  in-game @ if  on-as-before  then ; is released
\ the scripts' 0x9D: posed in an animation (its flags - 1 loops - and the frames it fades in over),
\ shown; held there (+0xE0, +0xE2) till started afresh
: pose-in ( anim flags blend -- )
    {: a fl bl :}  -1 posed !  -1 script-held !  0 hidden !
    my-model @ a has-motion? if  a anim !  my-model @ a bl fl motion-play  then ;
\ held by a script (+0xE0: Progress_CharsFrame runs his Pursuer_Think, not his own frame): his
\ senses, the story's move his step (stalker/scripted.fs), his head, footsteps, his model - no
\ modes, no behaviour, no frame of his own; on the triangle a move takes him to, done
: held-frame ( -- )
    senses  step @ ?dup if  execute  then  on-the-tri  played? looking
    played? if  stalker.moving:footsteps  anim-sounds-play  then
    pose  fit-legs  tell-danger ;
\ posed by the scripts' 0x9D (+0xE2: Pursuer_PlaceModel): his model alone, at his place - its
\ body by its feet, not planted (BodyFrames 0 0)
: posed-fit ( -- )
    played-here? 0= my-model @ 0< or  self body? 0= or if  exit  then
    chaired @ if  0e 0e 0e 0e  my-model @ self body-tri my-floor @ chair-fit exit  then
    0e 0e  my-model @ self body-tri my-floor @ 0 legs-fit ;
: posed-frame ( -- )  pose  posed-fit ;
: take? ( -- )   \ (Pursuer_EventState's 5: asked, taken in the room played - else dropped)
    take-asked @ 0= if  exit  then  0 take-asked !
    played? script-held @ 0= and if  taken  then ;
: frame ( -- )
    take?
    posed @ if  posed-frame  tell-danger exit  then
    script-held @ if  held-frame exit  then
    away? if  away  pose  tell-danger  exit  then
    thaw  hurt-down  senses  modes  dog-temper  shut-behind  ladder-watch
    step @ ?dup if  execute  then   \ (his search waits while he reels - struck, down: the original's behaviour is
    reeling @ 0= if  search  then  played? looking   \ only picked at a step's end; then Pursuer_Think's head)
    played? growls @ and if  growl  then
    played? if  stalker.moving:footsteps  anim-sounds-play  then
    pose  fit-legs  tell-danger ;
\ what he is doing, for Hewie (the original's character fields he reads: room, move mode and
\ part, condition, health)
: move-mode-told ( -- mode )
    away? if  unreachable-mode exit  then
    reeling @ cond @ down-cond = or bitten @ or if  struck-mode exit  then
    leading @ if  engaged-mode  else  free-mode  then ;
: sub-told ( -- sub )
    bitten @ if  bitten-sub exit  then  cond @ down-cond = if  down-sub exit  then
    leading @ if  leading-sub  else  0  then ;
\ his step this frame in the room while it takes him ahead (Motion_RootMovement's z over 0), else none
: stride-ahead ( F: -- dx dz )
    away? my-model @ 0< or if  0e 0e exit  then
    root@ {: F: turn x y z :}  z f0> if  x z yaw rotate-by  else  0e 0e  then ;
: tell-stride ( -- )  stride-ahead {: F: dx dz :}  dx f>cell dz f>cell broadcast stalker-stride ;
\ what holds him, for the others (stalker-ties): a script or a scene of the room has him (+0xE0:
\ Hewie's scuffle off screen leaves him be); out of contact (+0x2A: Fiona isn't pushed out of him);
\ a joint action under way (Progress_IsLinked: his seize of her awaiting her answer; Hewie's attack
\ or tackle the frame he takes it - its entry settled the next) - the story's "free" (cond 0x65)
: linked? ( -- flag )  leading @ 1 =  answer @ no-answer = and  link-t @ 0> or ;
: tell-ties ( -- )
    script-held @ in-scene @ or 0<>  apart @ away? or 0<>  linked?  broadcast stalker-ties
    link-t @ if  -1 link-t +!  then ;
: tell-doing ( -- )
    my-room  move-mode-told  sub-told
    cond @  hp @  hp-max @  my-kind @  my-mode @  came-by @  broadcast stalker-doing
    d-fiona f@ f>cell broadcast stalker-reach
    tell-stride
    away? 0=  combo @ 0< 0= and if  mv-damage  else  -1  then  broadcast stalker-blow
    sees-fiona @ 0<>  away? 0= and  broadcast stalker-sees
    tell-ties ;
\ the family's setup (Pursuer_Setup): his sight 150 ahead, 60 degrees either side; Fiona and
\ Hewie known; listening for noises above his threshold (not his own)
: family-setup ( -- )
    -1 her-came !  0 temper !
    150e view-range f!  60e deg>rad view-half f!  -1e d-fiona f!  -1e d-hewie f!
    s" fiona" actor-named fiona-cached !  s" hewie" actor-named hewie-cached !
    hear-threshold @ stalker-noise s" acoustics" actor-named send listen
    pursuer-ladder ladder-offsets !  -1 her-ladder-on !  -1 at-ladder !
    self subscribe zone-target  self subscribe zone-targets-off  self subscribe fiona-unseen
    self subscribe hewie-doing  self subscribe scene  0 script-held !  0 in-scene !  well-cond dog-cond !
    my-kind @ head-bones!  ahead-look look-mode ! ;

\ a door slammed off screen, the one he makes for: right behind it (Pursuer_HitOffscreen kind 5) -
\ not while down or stunned already; its damage (and the damage he's had); health left, stunned
\ 60, 90 or 120 frames, crying out, after her; none, down there (his cry)
: door-cry ( id -- )   \ (at the door he would come into her room by)
    world-held state-flag? hushed @ or if  drop exit  then
    next-door dup 0< if  2drop exit  then  room-id door-exit-in dup 0< if  2drop exit  then
    1 exit-spot drop  7 bank-sound-at ;
: struck-away ( damage -- )
    cond @ down-cond =  stun-t @ 0<> or if  drop exit  then
    dup hits @ + 999 min hits !  hp @ swap - 0 max hp !
    hp @ 0> if  3e rnd f* f>s 30 * 60 + stun-t !  hurt-cry door-cry  after-her mode!
    else  down-cry door-cry  down-away  then ;
: door-slammed-away ( damage exit -- )  behind-door? if  struck-away  else  drop  then ;
\ Hewie's scuffle with him off screen (Pursuer_Hit kind 8): its damage (not below 1), held those
\ frames more; hurt, his cry from the door he would come into her room by
: scuffled ( damage frames -- )
    stun-t +!  dup hp @ swap - 1 max hp !  0> if  hurt-cry door-cry  then ;
: blow-away ( kind damage how -- )
    rot case
        door-blow of  door-slammed-away  endof
        scuffle-blow of  scuffled  endof
        >r 2drop r>
    endcase ;
\ ---- the story's orders (the scripts' 0x2F, 0x30, 0x15: Pursuer_GoForFionaStance0 /
\ ChaseFionaHere / StartSearch, Pursuer_KnockedDown, Pursuer_Activate) - not while struck,
\ bitten or leading her: then his mode only ----
: recover ( -- )  cond @ down-cond = if  well-cond cond !  0 down-t !  then ;
: busy? ( -- flag )  reeling @ bitten @ or leading @ or ;
: ordered ( mode -- )   \ (his wandering off given up: the order is his behaviour now)
    wandering-out @ if  let-door-go  out-over  then
    case
        after-her of  recover  after-her mode!  avoid-clear  busy? if  exit  then
              played? if  sees-fiona @ if  chase-begin  else  chase-start  then  else  head-for-her  then  endof
        searching of  recover  played? if  busy? if  searching mode!  else  search-again  then
              else  heading-for-her mode!  avoid-clear  head-for-her  then  endof
        waiting-about of  recover  played? if  busy? if  searching mode!  else  search-again  then
              else  waiting-about mode!  route-clear  then  endof
    endcase ;
: knocked-out ( -- )  played? 0= if  exit  then  let-door-go  out-over  0 hp !  knock-down ;
: run-temper ( -- )   \ (the run: how much of Hewie he stands - Kind34_Activate 80, TintStalker_Reset 20,
    my-kind @ case                     \ Debilitas3_Activate 16)
        kind34-kind of  80 tolerance !  endof  tintstalker-kind of  20 tolerance !  endof
        debilitas-7-kind of  16 tolerance !  endof
    endcase ;
: activated ( -- )
    hewie-played? if  run-temper  then
    forget-leads  -1 making-for !  0 down-t !  waiting-about mode!  900 mode-t !  0 raging !
    hp @ 0> 0= if  hp-max @ hp !  then ;
: unload ( -- )   \ (a scene's figure placed: out of the room too)
    0 loaded !  unheld  in-game @ if  go-out  else  body-off  my-model @ 0< 0= if  0 him act.visible l!  then  then
    my-kind @ -1 cast-as  wear-own ;
: dressed ( id addr len -- )   \ (his model loaded, its motion table by name: hidden till he comes in;
    \ he blinks, his hands posed by his motions - Pursuer_ModelUpdate's Motion_Eyes / Motion_Hands)
    {: id a n :}  id my-model !  a n my-motions 2!  id a n motion-table  0 him act.visible l!  1 him act.alive l!
    id file leg-bones human-legs
    file s" O_LRO" model-of? dup chaired !  id swap lorenzo-chair ;   \ (LorenzoModel: in his chair)
: load-model-file ( addr len -- id )  2dup file!  actor-load ;
\ ---- each one's own numbers (his Setup, by the hard setting) and ways ----
: by-hardness ( normal hard -- n )  hard-mode? if  nip  else  drop  then ;
: fby-hardness ( F: normal hard -- f )  hard-mode? if  fnip  else  fdrop  then ;
: ways ( kit pursuers? gives-up? -- ) ( F: turn-deg reach -- )   \ (his tables; the Pursuer's chase or his own; giving up)
    gives-up !  base-chase !  kit !  reach-k f!  deg>rad turn-k f! ;
: health! ( n -- )  dup hp-max !  hp ! ;
: hewie-temper ( tolerance dog-limit -- ) ( F: throw -- )  dog-limit !  tolerance !  throw-k f! ;
: no-own-rooms ( -- )  -1 own-a !  -1 own-b ! ;
: own-rooms ( room-a tri-a room-b tri-b -- )  own-b-tri !  own-b !  own-a-tri !  own-a ! ;   \ (vtable +0x314: -1 none)
: clocks ( after after-hard down down-hard -- )   \ (+0x16D4 after her, +0x16D0 down when knocked out)
    {: a ah d dh :}  a ah by-hardness after-frames !  d dh by-hardness down-frames ! ;

\ ---- Debilitas's kinds: one actor, set up as the kind he is loaded as (a new Pursuer each time in
\ the original: its Setup's numbers, health full). All turn 3 degrees a frame, reach 16 for Hewie,
\ 24 for her, Hewie's time 1800 ----
: as-debilitas ( -- )   \ (debilitas.c Debilitas_Setup: his own chase; 20 damage and he gives up,
    \ home to his lair - castle-2f-2 $20, its spot 796 - kDebilitasRoomSpots)
    3e 16e  debilitas-kit false true ways  20 give-up-hits !  8e 1.5e 12e 1.5e feet-fit
    70 110 by-hardness health!  300 540 1800 1350 clocks
    10e  20 1800 hewie-temper  4500 away-base !  90 cool-secs !  $20 796 -1 -1 own-rooms ;
: as-debilitas-6 ( -- )   \ (debilitas2.c Debilitas2_Setup: the Pursuer's chase, +0x16B4 1; 80 damage
    \ and he gives up, home to castle-2f-2 $20 (796) or castle-1f-2 $02 (794) - D_0041BE40)
    3e 16e  debilitas6-kit true true ways  80 give-up-hits !  8e 1.5e 12e 1.5e feet-fit
    90 135 by-hardness health!  300 300 1800 1800 clocks
    10e 15e fby-hardness  30 1800 hewie-temper  10800 away-base !  150 cool-secs !  $20 796 $02 794 own-rooms ;
: as-debilitas-7 ( -- )   \ (debilitas.c Debilitas3_Setup: the Pursuer's chase; never gives up
    \ (Pursuer_GivesUp), no rooms of his own (Debilitas3_PickDestination 0))
    3e 16e  debilitas7-kit true false ways  6e 1.5e 13e 0e feet-fit
    110 165 by-hardness health!  300 300 1350 1350 clocks
    12e 18e fby-hardness  40 1800 hewie-temper  9000 away-base !  150 cool-secs !  no-own-rooms ;
: as-debilitas-27 ( -- )   \ (debilitas.c Kind27_Setup: as kind 7, its own tables, Hewie's tolerance 30)
    3e 16e  kind27-kit true false ways  6e 1.5e 13e 0e feet-fit
    110 165 by-hardness health!  300 300 1350 1350 clocks
    12e 18e fby-hardness  30 1800 hewie-temper  9000 away-base !  150 cool-secs !  no-own-rooms ;
: debilitas-setup ( kind -- )
    dup my-kind !  0 give-up-hits !  case
        debilitas-6-kind of  as-debilitas-6  endof   debilitas-7-kind of  as-debilitas-7  endof
        debilitas-27-kind of  as-debilitas-27  endof  as-debilitas
    endcase ;
\ Debilitas's model by the kind he is loaded as (Pursuer_ModelFiles: Debilitas_ModelFiles O_DB0,
\ Debilitas2_ModelFiles O_DB1, Debilitas3_ / Kind27_ModelFiles O_DB2 - the same skeleton and
\ motions, the model class DebilitasModel's), its textures by the hard setting (the tables' H ones);
\ its sounds and events its own
: debilitas-file ( kind -- addr len )
    case  debilitas-6-kind of  s" O_DB1/DB1_000"  endof   debilitas-7-kind of  s" O_DB2/DB2_000"  endof
          debilitas-27-kind of  s" O_DB2/DB2_000"  endof  >r s" O_DB0/DB0_000" r>  endcase ;
create hard-tex 32 allot
: hard-textures ( addr len -- addr' len' )   \ (the name with H on)
    {: a n :}  a hard-tex n move  [char] H hard-tex n + c!  hard-tex n 1+ ;
: debilitas-model ( addr len -- id )
    2dup file!  2dup hard-mode? if  hard-textures  then  actor-load-textured ;
: debilitas-dress ( kind -- )   \ (anew when the kind's model isn't the one he has)
    wear-own  debilitas-file  2dup file compare 0= if  2drop exit  then
    my-model @ dup 0< if  drop  else  actor-free  then
    debilitas-model s" motions-debilitas" dressed ;
: debilitas-of? ( kind -- flag )   \ (the kinds the Debilitas actor is loaded as)
    dup debilitas-kind =  over debilitas-6-kind = or  over debilitas-7-kind = or  swap debilitas-27-kind = or ;
create place-at /vec allot
behaviour stalking
  on stalker-load ( slot -- )  load ;
  \ the kind he is loaded as (Debilitas's: his model and his Setup by it - debilitas-dress,
  \ debilitas-setup; his kind the cutscenes' cast finds him by)
  on stalker-as ( kind -- )
      dup debilitas-of? 0= if  drop exit  then
      dup debilitas-dress  debilitas-setup ;
  on stalker-unload ( -- )  unload ;
  on stalker-in ( room tri -- )  come-in ;
  on place ( x y z yaw -- )   \ (put there, facing so: where a cutscene left him; a scene's figure where the story puts it)
      in-game @ figure? or 0= if  2drop 2drop exit  then
      {: yaw :}  cells>xyz place-at vec!
      room-id  place-at my-floor @ v-tri-in  place-at vec@ body-place  yaw cell>f body-turn  settle ;
  on stalker-out ( -- )  go-out ;
  on write-save ( -- )   \ (saves.fs) loaded: his kind, in the game or not, his place, health and mode
      loaded @ 0=  my-slot @ play-slot <> or if  exit  then   \ (a scene's figure isn't the stalker in play)
      my-kind @ .  in-game @ 0<> .  self body? if  self body-room .  self body-tri .  else  -1 . -1 .  then
      hp @ .  my-mode @ .  ." stalker-was" cr
      \ (Pursuer_SaveState: his condition, most health, rage, temper with Hewie, the damage he's had,
      \ his mode's clock, his time down, his heading)
      cond @ .  hp-max @ .  raging @ .  temper @ .  hits @ .  mode-t @ .  down-t @ .
      self body? if  self body-yaw  else  0e  then  f>cell .  ." stalker-more" cr ;
  on stalker-restore-more ( cond hp-max rage temper hits mode-t down-t yaw -- )   \ (Pursuer_LoadState)
      cell>f  self body? if  body-turn  else  fdrop  then
      down-t !  mode-t !  hits !  temper !  raging !  hp-max !  cond !
      cond @ down-cond =  in-game @ and if   \ (down: lying as he was, his time down to run)
          lying-front-anim lie-anim !  lie-anim @ play-now  lying down-stage !  -1 reeling !  ['] downed step!
      then ;
  on stalker-restore ( hp mode -- )  mode!  hp ! ;
  on noclip ( on -- )  dup 0<> noclipped !  if  0  else  my-floor @  then  body-mask! ;   \ (the story's 0x20: through blocked floor - at doors)
  on drawn-to ( tri -- )  in-game @ if  go-for-doll  else  drop  then ;
  on stalker-hunt ( -- )  in-game @ if  heading-for-her mode!  away? 0= if  search-again  then  then ;
  on entered-room ( room exit -- )  0 still-held !  her-came !  room-changed  in-game @ if  pose  then ;
  on characters-still ( on -- )  still-held ! ;
  on tick ( -- )   \ (a scene having him - Character_Disable - his frame waits: the scene moves his model;
      \ given back he is shown, as the cast was made active: Cutscene_ReleaseActors' +0x29)
      my-model @ scene-has?  dup 0=  in-cast @ and if  0 hidden !  then  in-cast !
      in-game @ in-cast @ and if  tell-doing  loaded @ if  status  then  exit  then
      in-game @ still-held @ and if  tell-doing  loaded @ if  status  then  exit  then   \ (held still: OpenHuey's)
      in-game @ if  frame  script-held @ 0= if  own-frame @ ?dup if  execute  then  then  tell-doing  then   \ (held: his Think, not his own frame)
      figure? self body? and  in-game @ 0= and if  posed @ if  posed-frame  else  pose  then  then   \ (a scene's figure: moved only posed)
      loaded @ if  status  then ;
  \ Hewie: his attack asked, his letting go, his bites, his growls
  on dog-attack ( kind -- )  dog-attacked ;
  on dog-tackle ( -- )  dog-tackled ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      dog-trust !  2drop 2drop 2drop drop  dog-cond !  2drop 2drop ;   \ (his condition: the chance roll's; his trust)
  \ the story's: a script of the room has him (in the room being played: +0xE0) or lets go (posed:
  \ held till started afresh); its moves (stalker/scripted.fs: carried out while held - each clears
  \ his done, +0xE1); shown or hidden (0x1F: +0x29); posed (0x9D)
  on scripted ( on -- )
      if  -1 take-asked !
      else  0 take-asked !  script-held @ posed @ 0= and if  released  then  then ;
  on scripted-move ( kind a b x y z yaw -- )
      cell>f sm-yaw f!  cells>xyz sm-at vec!  sm-b !  sm-a !
      0 sm-done !  script-held @ posed @ 0= and if  start-move  else  drop  then ;
  on show ( on -- )  0= hidden ! ;
  on hold-anim ( anim flags blend -- )  loaded @ if  pose-in  else  drop 2drop  then ;
  on scene ( on -- )   \ (a cutscene has the characters; given back - Pursuer_Enable: his way forgotten)
      dup 0<> in-scene !  0= in-cast @ and if  0 in-cast !  my-path path-clear  then ;
  on shaken-off ( -- )  bitten @ bite-how @ held-by-dog = and if  dog-let-go bite-how !  then ;
  on bite ( damage hard -- )  bitten @ if  bitten-by  else  2drop  then ;
  on snap ( -- )  in-game @ if  snapped  then ;
  on grab-hewie ( -- )  in-game @ if  grab-hewie-behind  else  false  then  sender send grab-answer ;
  \ the story's orders
  on stalker-order ( mode -- )  in-game @ if  ordered  else  drop  then ;
  on stalker-clock ( frames -- )  mode-t ! ;
  on stalker-knock-out ( -- )  in-game @ if  knocked-out  then ;
  on stalker-down-over ( -- )  0 down-t ! ;
  on rage ( on -- )  0<> raging ! ;
  on activate ( -- )   \ (Pursuer_Activate: Character_Activate - not held, not posed)
      script-held @ posed @ or  unheld  0 sm-done !
      in-game @ if  activated  if  stand  then  else  drop  then ;
  on set-cond ( v -- )  cond ! ;
  on full-health ( -- )  hp-max @ hp ! ;
  on heal ( n -- )  abs hp @ + hp-max @ min hp ! ;
  on silent ( on -- )  0<> hushed ! ;
  \ his move and animation for the story's waits (the scripts' 0x7B, 0x9E): as Fiona's and Hewie's
  on frame-end ( -- )  in-game @ figure? or if  sm-done @ ended? my-model @ 0 0 1 motion-events broadcast moving  then ;
  on stalker-summoned ( room exit found plan -- )  summoned ;
  on heard ( loud room tri door source -- )  nip hear ;
  on heard-nothing ( -- )  heard-none ;
  on door-held ( room exit -- )  nip held-answer ;   \ (his to open, or the door he came in by)
  on hit-taken ( kind -- )  struck-home ;
  \ (held by a script his requests aren't taken - Pursuer_Think - and are cleared as he is let go)
  on blow-from ( kind damage how x y z -- )   \ (from a point: Progress_Noise)
      script-held @ if  2drop 2drop 2drop exit  then
      cells>xyz strike-pt vec!  away? if  blow-away exit  then  -2 striker !  struck ;
  on blow ( kind damage how -- )   \ (her shove or kick; a door she slammed: no answer)
      script-held @ if  2drop drop exit  then
      away? if  blow-away exit  then
      sender striker !  2 pick door-blow = {: door? :}  struck  door? 0= if  sender send blow-taken  then ;
  on seize-taken ( -- )   \ (her answers to his taking her; one he isn't waiting for: let go)
      leading @ if  she-came answer !  else  fiona send unhand  then ;
  on seize-refused ( -- )  she-refused answer ! ;
  on broke-free ( -- )  she-broke-free answer ! ;              \ (his blow struck her: his cry)
  on fiona-plight ( state ladder -- )  her-ladder-on !  her-plight ! ;
  on zone-targets-off ( -- )  target-on #targets cells 0 fill ;
  on zone-target ( zone x y z -- )   \ (its centre: cells of floats)
      {: zone x y z :}  zone #targets u< 0= if  exit  then
      x cell>f y cell>f z cell>f  zone 3 * floats target-at + vec!
      -1 zone cells target-on + ! ;
  on panic ( stage level -- )  drop her-stage ! ;     \ (the door he came in by: swung open)
  on fiona-unseen ( on -- )  her-unseen ! ;
  on door-refused ( room exit -- )  nip refused-answer ;
end-behaviour

\ ---- each one's own: his model, his body ----
: bodied ( floor -- ) ( F: width height -- )
    body-size  dup my-floor !  body-mask!  body-off  0 in-game !
    self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end
    self subscribe characters-still ;

\ ---- Debilitas (debilitas.c Debilitas_Setup, DebilitasModel): 70 health (hard: 110), 5 across,
\ 20 tall; his model O_DB0/DB0_000, its motion table, his sounds (bank 7); his own room (his
\ lair and its spot); loaded as kinds 6, 7 and 27 their Setups (`debilitas-setup`) ----
behaviour debilitas
  extends stalking
  on spawned ( -- )
      debilitas-kind debilitas-file debilitas-model s" motions-debilitas" dressed
      debilitas-kind debilitas-setup  -1 growls !  0 hear-threshold !  family-setup
      leaves-doors door-manner !  debilitas-ladder ladder-offsets !
      5e 20e debilitas-floor bodied ;
end-behaviour
: debilitas-spawn ( -- id )  debilitas stalker-state s" debilitas" spawn ;

\ ---- Daniella (daniella.c Daniella_Setup, DaniellaModel; kind 3): 120 health (hard: 100), 3
\ across, 17 tall; her model O_DNL/DNL_000, its textures DNL_000 (hard: DNL_000H), its motion
\ table $419E60, her sounds (bank 7); the Pursuer's floor (NPC_BlockFlags); her frame without
\ his growl ----
: daniella-motions ( -- addr len )  s" motions-daniella" ;
\ her blow's mark (Daniella_BlowEffect), as the one struck cries out or at her blow's effect key:
\ out along her hand (1.5, 3.5 for her third and fifth blows), big for her fourth and fifth; her
\ grabs on Fiona, big
$2D constant hand-bone
: blow# ( n -- anim )  $E00 + ;   \ (her blows: their animations)
: hand-mark ( heavy -- ) ( F: reach -- )
    {: heavy F: reach :}  0e 0e reach  my-model @ hand-bone bone-point  self body-tri heavy spray ;
: her-mark ( -- )
    anim @ case
        hold-on-anim of  fiona -1 0.75e mark-on  endof   carry-anim of  fiona -1 0.75e mark-on  endof
        5 blow# of  3.5e -1 hand-mark  endof         4 blow# of  1.5e -1 hand-mark  endof
        3 blow# of  3.5e 0 hand-mark  endof
        0 blow# of  1.5e 0 hand-mark  endof   2 blow# of  1.5e 0 hand-mark  endof
        6 blow# of  1.5e 0 hand-mark  endof   7 blow# of  1.5e 0 hand-mark  endof
    endcase ;
: her-frame ( -- )  effect-key? if  her-mark  then ;
: her-cry-mark ( who -- )  drop her-mark ;
: daniella-model ( addr len -- id )   \ (her model and its sounds' file; her textures by the hard setting)
    2dup file!  hard-mode? if  s" O_DNL/DNL_000H"  else  s" O_DNL/DNL_000"  then  actor-load-textured ;
: daniella-marks ( -- )  ['] her-frame own-frame !  ['] her-cry-mark own-mark ! ;
behaviour daniella
  extends stalking
  on spawned ( -- )
      4e 12e  daniella-kit true false ways  8e 1.5e 12e 1.5e feet-fit  daniella-marks
      s" O_DNL/DNL_000" daniella-model daniella-motions dressed
      120 100 by-hardness health!  daniella-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      140 100 by-hardness cool-secs !  3600 6000 by-hardness away-base !  no-own-rooms   \ (Daniella_Setup's first)
      450 450 750 300 clocks
      10e 25e fby-hardness  200 80 by-hardness 6000 hewie-temper
      shuts-doors-calm door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: daniella-spawn ( -- id )  daniella stalker-state s" daniella" spawn ;

\ ---- Daniella's later kinds (daniella.c Kind34_* / Kind35_* / Kind36_*): her chase, her look
\ (DaniellaModel; 34 and 36 the model DNL_001, 35 hers), each with its own tables (her kit's
\ shape: stalker/tables.fs) and numbers. 34 has her rage and pulls the doors she comes in by
\ shut while calm (left open on the hard setting); 35 and 36 have no rage and leave doors be ----
behaviour kind34
  extends stalking
  on spawned ( -- )
      4e 12e  kind34-kit true false ways  8e 1.5e 12e 1.5e feet-fit  daniella-marks
      s" O_DNL/DNL_001" daniella-model daniella-motions dressed
      120 200 by-hardness health!  kind34-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      180 160 by-hardness cool-secs !  4500 6000 by-hardness away-base !  no-own-rooms   \ (Kind34_Setup)
      300 360 900 1350 clocks
      25e 50e fby-hardness  200 65 by-hardness 6000 hewie-temper
      shuts-doors-calm leaves-doors by-hardness door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind34-spawn ( -- id )  kind34 stalker-state s" kind34" spawn ;

behaviour kind35
  extends stalking
  on spawned ( -- )
      4e 12e  kind35-kit true false ways  8e 1.5e 12e 1.5e feet-fit  daniella-marks
      s" O_DNL/DNL_000" daniella-model daniella-motions dressed
      65 80 by-hardness health!  kind35-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      140 100 by-hardness cool-secs !  3600 6000 by-hardness away-base !  no-own-rooms   \ (Kind35_Setup)
      600 600 450 360 clocks
      15e 10e fby-hardness  120 75 by-hardness 3000 600 by-hardness hewie-temper
      leaves-doors door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind35-spawn ( -- id )  kind35 stalker-state s" kind35" spawn ;

behaviour kind36
  extends stalking
  on spawned ( -- )
      4e 12e  kind36-kit true false ways  8e 1.5e 12e 1.5e feet-fit  daniella-marks
      s" O_DNL/DNL_001" daniella-model daniella-motions dressed
      65 80 by-hardness health!  kind36-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      180 160 by-hardness cool-secs !  4500 6000 by-hardness away-base !  no-own-rooms   \ (Kind36_Setup)
      300 600 900 360 clocks
      25e 10e fby-hardness  120 65 by-hardness 3000 600 by-hardness hewie-temper
      leaves-doors door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind36-spawn ( -- id )  kind36 stalker-state s" kind36" spawn ;

\ ---- Riccardo (riccardo.c Riccardo_Setup, RiccardoModel; kind 4): 100 health (hard: 125), 3
\ across, 17 tall; his model O_RCG/RCG_000, its motion table $41A2C0, his sounds (bank 7); the
\ Pursuer's floor; his own chase, flurry and lunge (stalker/riccardo.fs), his reach 20 ----
: riccardo-chase ( -- )  ['] ric-begin own-chase !  ['] ric-run own-run !  20e attack-range f! ;
behaviour riccardo
  extends stalking
  on spawned ( -- )
      4e 12e  riccardo-kit false false ways  8e 1.5e 12e 1.5e feet-fit
      ['] cry-hit own-mark !  riccardo-chase  riccardo-acts own-acts !  ['] riccardo-frame own-frame !
      s" O_RCG/RCG_000" load-model-file s" motions-riccardo" dressed
      100 125 by-hardness health!  riccardo-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      90 cool-secs !  9000 away-base !  no-own-rooms   \ (Riccardo_Setup)
      300 540 1200 900 clocks
      15e 10e fby-hardness  50 1800 hewie-temper  shuts-doors door-manner !  riccardo-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: riccardo-spawn ( -- id )  riccardo stalker-state s" riccardo" spawn ;

\ ---- Lorenzo (lorenzo.c Lorenzo_Setup, LorenzoModel; kind 11): 50 health (hard: 75), 5 across,
\ 20 tall; his model O_LRO/LRO_000, its motion table $4246C0; the Pursuer's chase with his
\ tables, turning 2 degrees a frame, his reach 18; no temper with Hewie in his tables ----
behaviour lorenzo
  extends stalking
  on spawned ( -- )
      2e 12e  lorenzo-kit true false ways  8e -8e 8e -8e feet-fit  18e attack-range f!
      s" O_LRO/LRO_000" load-model-file s" motions-lorenzo" dressed
      50 75 by-hardness health!  lorenzo-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      100 cool-secs !  3600 away-base !  no-own-rooms   \ (Lorenzo_Setup)
      600 450 510 360 clocks
      0e  100 0 hewie-temper  leaves-doors door-manner !
      5e 20e pursuer-floor bodied ;
end-behaviour
: lorenzo-spawn ( -- id )  lorenzo stalker-state s" lorenzo" spawn ;

\ ---- Lorenzo's other class (lorenzo.c Lorenzo2_Setup; kind 10): 250 health (hard: 400), 4 across,
\ 18 tall; his model O_LRY/LRY_000 (motion table $424490); the Pursuer's chase with his kit (his
\ moves by the hard setting), his own actions under the floor (stalker/lorenzo2.fs) ----
behaviour lorenzo2
  extends stalking
  on spawned ( -- )
      4e 60e  lorenzo2-kit lorenzo2h-kit by-hardness true false ways  8e -8e 8e -8e feet-fit
      lorenzo2-acts own-acts !  ['] lorenzo2-frame own-frame !  70e attack-range f!
      s" O_LRY/LRY_000" load-model-file s" motions-lorenzo2" dressed
      250 400 by-hardness health!  lorenzo2-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      150 cool-secs !  9000 away-base !  no-own-rooms   \ (Lorenzo2_Setup)
      900 450 0 0 clocks
      50e 25e fby-hardness  100 600 hewie-temper  leaves-doors door-manner !
      4e 18e pursuer-floor bodied ;
end-behaviour
: lorenzo2-spawn ( -- id )  lorenzo2 stalker-state s" lorenzo2" spawn ;

\ ---- kind 39 (lorenzo.c Kind39_Setup): Lorenzo's other class again - its own tables, 250 health
\ (hard: 300), no "rage" (its reach 18, for Hewie 12), its sweep at whoever it sees ----
behaviour kind39
  extends stalking
  on spawned ( -- )
      4e 12e  kind39-kit kind39h-kit by-hardness true false ways  8e -8e 8e -8e feet-fit
      lorenzo2-acts own-acts !  18e attack-range f!
      s" O_LRY/LRY_000" load-model-file s" motions-lorenzo2" dressed
      250 300 by-hardness health!  kind39-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      150 cool-secs !  9000 away-base !  no-own-rooms   \ (Kind39_Setup)
      900 900 0 0 clocks
      30e 65e fby-hardness  40 75 by-hardness 300 360 by-hardness hewie-temper
      leaves-doors door-manner !
      4e 18e pursuer-floor bodied ;
end-behaviour
: kind39-spawn ( -- id )  kind39 stalker-state s" kind39" spawn ;

\ ---- Riccardo's other class (riccardo.c Kind37_Setup; kind 37): his chase with its own tables,
\ 100 health (hard: 125), 3 across, 17 tall, its model O_RCT/RCT_000 (motion table $42AED0);
\ near him at 30; struck enough, alerted (stalker/riccardo.fs `kind37-frame`) ----
behaviour kind37
  extends stalking
  on spawned ( -- )
      4e 12e  kind37-kit false false ways  8e 1.5e 12e 1.5e feet-fit
      riccardo-chase  24e attack-range f!  ['] kind37-frame own-frame !   \ (its reach the Pursuer's: Pursuer_AttackRange)
      s" O_RCT/RCT_000" load-model-file s" motions-tintstalker" dressed
      100 125 by-hardness health!  kind37-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      45 cool-secs !  5400 away-base !  no-own-rooms   \ (Kind37_Setup)
      300 360 150 150 clocks
      10e  50 900 hewie-temper  leaves-doors door-manner !  riccardo-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind37-spawn ( -- id )  kind37 stalker-state s" kind37" spawn ;

\ ---- the TintStalker (tintstalker.c TintStalker_Setup; kind 23): the Pursuer's chase with its
\ tables, Riccardo's look (O_RCT/RCT_000, motion table $42AED0), 100 health (hard: 125); seen
\ only a while after each turn (stalker/riccardo.fs `tint-frame`) ----
behaviour tintstalker
  extends stalking
  on spawned ( -- )
      4e 12e  tintstalker-kit true false ways  8e 1.5e 12e 1.5e feet-fit
      ['] tint-frame own-frame !  -1 tint-was !  -1 unseen !
      s" O_RCT/RCT_000" load-model-file s" motions-tintstalker" dressed
      100 125 by-hardness health!  tintstalker-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      45 cool-secs !  5400 away-base !  no-own-rooms   \ (TintStalker_Setup)
      300 300 1200 1200 clocks
      15e 10e fby-hardness  50 1800 hewie-temper  shuts-doors door-manner !  riccardo-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: tintstalker-spawn ( -- id )  tintstalker stalker-state s" tintstalker" spawn ;

\ ---- kind 12 (lorenzo.c Kind12_Setup): the Pursuer's chase with its tables, 9999 health and never
\ hurt, 3.5 across, 18 tall, its model O_LRF/LRF_000 (motion table $443FB0), reach 18, for Hewie
\ 14; in her room it seizes her (stalker/riccardo.fs `kind12-frame`) ----
behaviour kind12
  extends stalking
  on spawned ( -- )
      4e 14e  kind12-kit true false ways  8e -8e 8e -8e feet-fit  18e attack-range f!
      ['] kind12-frame own-frame !  -1 mark-id !
      s" O_LRF/LRF_000" load-model-file s" motions-kind12" dressed
      9999 health!  kind12-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      999 cool-secs !  3000 away-base !  no-own-rooms   \ (Kind12_Setup)
      29970 29970 0 0 clocks
      0e  100 0 hewie-temper  leaves-doors door-manner !
      3.5e 18e pursuer-floor bodied ;
end-behaviour
: kind12-spawn ( -- id )  kind12 stalker-state s" kind12" spawn ;
