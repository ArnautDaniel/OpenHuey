\ story/common.fs - what happens in every room played as a place: the original's shared scripts
\ that run whatever the room - after each room's own phases (shared.after-phase1: crouching in a
\ shadow, the camera on the stalker while she is half hidden, the mandragora's spell, the other
\ game's rewards) and as items are used (her drink, $8D; Hewie fed: her $8E, his $8F / $90 / $91).
\ The kit runs `every-room` each frame and hands item uses to `item-used` (story/kit.fs).
IN: story.common
USING: engine game-state actors messages common vectors facts flag-names story-names floor-flags keys
    story.kit fiona.anims fiona.sounds hewie.sounds ;

\ ---- where they are to each other ----
: touching? ( a b -- flag )   \ close by b's own margins (its radius across, its height up)
    {: a b :}  a here? b here? and 0= if  false exit  then
    b body-dims {: F: r h :}  a b r f>s close-to? ;
: on-flags? ( id flags -- flag )   \ standing on a triangle with any of these flags
    over here? 0= if  2drop false exit  then  swap body-tri dup 0< if  2drop false exit  then  nav-flags and 0<> ;
: face-toward ( id who -- )  dup here? 0= if  2drop exit  then  body-pos fswap fdrop  turn-to-xz ;
: head-on ( who frames -- )   \ her head on who for so many frames
    over 0< if  2drop exit  then  fiona send look-hold ;

\ ---- crouching (shared $81 / $82 / $87) ----
1 constant low-floor   \ (a triangle's flag: too low to stay crouched - she ducks and comes up: $87)
$23 constant drink-sound   $2D constant crouch-recovery
: crouch-held? ( -- flag )  shook-now @ 0=  l1 button-down? and ;   \ (crouched till she shakes or L1 is let go)
: near-homunculus? ( -- flag )   \ (one touching her here)
    #creatures first-homunculus do
        i cells cr-age + @ 2 <  i cells cr-room + @ this-room @ = and if  true unloop exit  then
    loop  false ;
: found-out? ( -- flag )   \ (crouched, but found: the stalker at her, a homunculus, her panic)
    unseen-now @ if
        fiona stalker touching?  st-cond-now @ down-cond <> and  near-homunculus? or  in-panic? or exit
    then
    chased?  stalker-in? and  70e noticed-near? and  in-panic? or ;   \ (found: he has her in sight or reach)
: startled ( -- )   \ ($82: up, seized, turned to him)
    fiona-half-hidden state-off  fiona follow  16 frames
    fiona stalker look-at  yield
    fiona crouch-up-anim 5 start-blend  8 frames  seized-sound common-bank fiona sound-by
    fiona uneasy-anim 10 play-blend  1 $7F $F rumble
    fiona stalker face-toward ;
: crouches ( -- )   \ ($81: down in the shadow, calmer as she stays)
    fiona hold  fiona wait-move  fiona crouch-down-anim play
    fiona-half-hidden state!  story-reaction hewie send reaction
    fiona crouching-anim start-anim
    begin  crouch-held?  while
        found-out? if  startled exit  then
        1 ease-panic  chased? if  4  else  5  then  calm-her  crouch-recovery recover  yield
    repeat
    fiona-half-hidden state-off  fiona follow  fiona crouch-up-anim play ;
: ducks ( -- )   \ ($87: too low here - down and up)
    fiona hold  fiona wait-move  fiona crouch-down-anim start-anim  5 frames  fiona crouch-up-anim 10 play-blend ;
: may-crouch? ( -- flag )   \ (L1 pressed, she free, the screen still, the world going on)
    hewie-played? 0=  no-flee state? 0= and  request-now @ spirit-request <> and
    l1 button-pressed? and  fiona-free? and
    fading @ 0= and  world-held state? 0= and  world-frozen state? 0= and ;
: crouching ( -- )
    may-crouch? 0= if  exit  then
    chased?  tremor-warned-2 fact? 0= and  70e noticed-near? and if  stalker 30 head-on exit  then   \ (found and near him: she only looks)
    fiona low-floor on-flags? if  ['] ducks  else  ['] crouches  then  start drop ;

\ ---- the rest of what every room does each frame ----
: stalker-watched ( -- )   \ (half hidden: the camera on him)
    fiona-half-hidden state?  no-stalker-camera state? 0= and  tremor-warned-2 fact? 0= and if  stalker follow  then ;
$18 constant first-mandragora   5 constant #mandragoras
: mandragora-carried? ( -- flag )
    #mandragoras 0 do  first-mandragora i + 1 has-items? if  true unloop exit  then  loop  false ;
: mandragora-spell ( -- )   \ (the old mansion's: one carried, her panic up - hiding, terrified; else he is kept out of two rooms)
    om1f-13-flag2 fact?  white-mandragora-used fact? 0= and 0= if  exit  then
    mandragora-carried? if
        $3C s" panic" actor-named send panic-reach
        in-panic? 0=  fiona-hidden state? and if  4 s" panic" actor-named send panic-stage!  then
    else
        $48 stalker-slot 0 doors-into  $66 stalker-slot 1 doors-into
    then ;
: rewards ( -- )   \ (the other game: Hewie's hints for how he has been treated)
    items-40-as-70 fact? 0= if  exit  then
    fiona held?  hewie held? or  if  exit  then  hewie-act-now @ 0= if  story.base:reward-item  then ;
\ ---- the stalker down on his front: searched (shared $80, offered as the shared after-phase 2
\ has it) - she bends to him and finds the next of his kind's gifts (story/base.fs, 0xB6) ----
: lying-on-front? ( -- flag )   \ (down, lying on his front or twitching so: Pursuer_InStance2Anim)
    stalker-in?  fiona here? and  st-cond-now @ down-cond = and 0= if  false exit  then
    stalker model-of dup 0< if  drop false exit  then  motion@
    dup stalker.anims:lying-front-anim =  swap stalker.anims:lying-front-anim 1- = or ;
: over-him? ( -- flag )   \ (close - 2 more than their sizes across - and facing him within 50 degrees)
    fiona stalker 2 0 story.base:touching? 0= if  false exit  then
    fiona stalker story.base:heading-between  fiona body-yaw f- angle-wrap fabs rad>deg  50e f<= ;
: found-on-him ( -- )   \ (his gift, if one is due: given and named, or none left)
    story.base:gift-due? 0= if  stalker send stalker-down-over exit  then   \ (none: he comes round)
    st-kind-now @ story.base:next-gift  dup 0< if  drop story.base:no-gift-message show exit  then
    dup item-named  dup 1 items send give-item  show-item-tab  fanfare  story.base:gift-message show ;
: search-him ( -- )
    fiona hold  fiona wait-move  fiona stalker face-toward  fiona wait-move
    fiona pick-up-anim play  4 frames  found-on-him  8 frames  fiona up-from-pick-anim play  wait-closed ;
: searching ( -- )  lying-on-front? 0= if  exit  then  over-him? if  ['] search-him calm-only offer  then ;

:noname ( -- )  crouching  stalker-watched  mandragora-spell  rewards  searching ; is every-room

\ ---- items used: her drink, Hewie fed (her part and his kept in step) ----
: drinks ( -- )   \ ($8D)
    fiona hold  fiona wait-move  fiona bent-hand-mouth-anim 10 start-blend
    drink-sound common-bank fiona sound-by
    fiona 2 wait-mark  1 $60 8 rumble  yield  fiona 2 wait-mark  1 $60 8 rumble
    fiona wait-anim  fiona 10 idle  9 frames ;
variable feed-step   \ (0 none yet, 1 he is ready, 2 she has put it down, 3 he is done)
: gives ( -- )   \ ($8E: she puts it down for him)
    fiona hold  0 feed-step !  fiona wait-move
    begin  hewie held?  feed-step @ 0= and  while  yield  repeat
    fiona hewie face-toward  fiona pick-up-anim play  2 feed-step !  fiona up-from-pick-anim play
    fiona -1 idle  7 frames  fiona hewie look-at  yield
    begin  hewie held?  feed-step @ 2 = and  while  yield  repeat
    fiona -1 look-at ;
: hewie-down? ( -- flag )  hewie-cond-now @ down-cond = ;
: hewie-full? ( -- flag )  hewie-in?  hewie-hp-now @ 100 = and ;
: hewie-hurt? ( -- flag )  hewie-cond-now @ hurt-cond = ;
: waits-for-her ( -- )  begin  fiona held?  feed-step @ 1 = and  while  yield  repeat ;
: gets-up ( -- )   \ (down: a treat brings him round)
    1 feed-step !  waits-for-her  bark-sound common-bank hewie sound-by
    hewie hewie.anims:get-up-anim play  hewie 10 idle  10 frames
    hewie-full? if  1 hewie send mood!  then  3 feed-step ! ;
: eats ( -- )   \ (up: to her, eats, a bark or a limping one)
    1 feed-step !  hewie fiona face-toward  waits-for-her
    hewie hewie.anims:sit-a-while-anim play  hewie fiona look-at  yield
    hewie-hurt? if  hewie.anims:bark-limping-anim  else  hewie.anims:bark-anim  then  hewie swap play ;
: treat ( -- )   \ ($8F: a treat)
    hewie hold  hewie wait-move
    hewie-down? if  gets-up exit  then
    eats  hewie-full? if  1 hewie send mood!  then  3 feed-step !  hewie -1 look-at ;
: flung ( -- )   \ (down: struck through - a yelp, flung)
    1 feed-step !  waits-for-her  yelp-sound common-bank hewie sound-by
    hewie still-anim-move hewie.anims:flung-anim 0  0e 0e 0e 0e  move  hewie wait-anim
    hewie hewie.anims:down-anim 5 start-blend  5 frames  3 feed-step ! ;
: tri-free? ( id -- flag )   \ (a floor one may lie on: no step, no doorway)
    dup here? 0= if  drop false exit  then  dup body-tri dup 0< if  2drop false exit  then
    nav-flags $80001 and if  drop false exit  then
    8 0 do  i door-here? if  i 0 2 pick body-pos door-in-area?  i 1 3 pick body-pos door-in-area? or if  drop false unloop exit  then  then  loop
    drop true ;
: bait ( -- )   \ ($90: an onion or the rousing bait - hurt, or knocked down if it takes the last of him)
    hewie hold  hewie wait-move
    hewie-down? if  flung exit  then
    1 feed-step !  hewie fiona face-toward
    hewie hewie.anims:sit-a-while-anim play  hewie fiona look-at  yield
    hewie-in?  hewie-hp-now @ 0= and  hewie tri-free? and if
        hewie hewie.anims:go-down-anim play  down-cond hewie send set-cond
    else
        shoved-yelp common-bank hewie sound-by  hewie hewie.anims:yelp-anim play
    then
    3 feed-step !  hewie -1 look-at ;
: fury ( -- )   \ ($91: the fury bait)
    hewie hold  hewie wait-move
    hewie-down? if  flung exit  then
    eats  3 feed-step !  hewie -1 look-at ;
: her-event ( ev -- )  case  $8D of  ['] drinks start drop  endof  $8E of  ['] gives start drop  endof  endcase ;
: his-event ( ev -- )
    case  $8F of  ['] treat start drop  endof  $90 of  ['] bait start drop  endof  $91 of  ['] fury start drop  endof  endcase ;
:noname ( who ev var value -- )   \ (the original's item_event: whose shared script)
    2drop  swap case  fiona-slot of  her-event  endof  hewie-slot of  his-event  endof  nip  endcase ; is item-used
