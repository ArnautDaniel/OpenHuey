\ story/places/castle-2f-5.fs - the bedroom in Belli Castle's 2F ($32) once the chapter has turned:
\ Daniella the stalker, Fiona weak ("I don't feel so hot") till she rests on the bed - the mirror's
\ scene ("I am not complete.": it cracks, shatters, Daniella in its fragments, her face), well
\ again, Hewie back from under the other bed, Daniella left in the room; the bed to hide under
\ (Daniella looking under it), the chicken jerky Hewie sniffs out by the wall, the windows after
\ dark, the movie replayed (the bedroom's own things: story/places/bedroom.fs). A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-2f-5.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-2f-5
USING: engine game-state actors messages common facts flag-names story-names room-names stalker-names
    items.names story.kit fiona.anims scenes story.places.bedroom ;

own-reset  bedroom-owns owns !

\ ---- what is said here ----
$21 constant rest-msg         \ "Fresh sheets have been placed on the bed. Rest on the bed? YES NO"
$1A constant incomplete-msg   \ "I am not complete."
$2F constant not-hot-msg      \ "I... I don't feel so hot. It's like I have no energy all of a sudden."
$30 constant get-rest-msg     \ "I should try to get some rest."
$32 constant sharp-glass-msg  \ "The glass looks sharp enough to really cause some serious injuries. I probably sh..."

\ ---- its own state (a visit) ----
own: tired-remarked   \ her first "I don't feel so hot" made (the next: rest)
own: rest-step        \ the rest's end: Hewie lying under the other bed (1), coming out (2)

\ ---- the room's things ----
$D constant mirror-movie   $C constant mirror-cutscene   $231 constant rest-file
12 constant tired-sound   $6A constant hewie-out-sound   \ (the room's; the common bank's)
$A constant looker-motions   \ (her look under the bed: Daniella's)
\ the mirror's parts (1..6, as bits from 1): whole, cracking, shattered (5 with 6, or 5 alone)
$01 constant whole-mirror   $02 constant cracking-1   $04 constant cracking-2   $08 constant cracking-3
$30 constant broken-mirror  $10 constant broken-mirror-2   \ ? (6 off a while late in the scene)
: mirror-as ( bits -- )  6 0 do  i 1+  over 1 i lshift and 0<>  part  loop  drop ;
: mirror-now ( -- )  saturnus-hint-on fact? if  broken-mirror  else  whole-mirror  then  mirror-as ;
$E constant first-fragment   11 constant #fragments   \ (its objects a_fragment0..10, hidden but in the scene)
: fragments ( shown? -- )
    #fragments 0 do  first-fragment i +  over if  object-show  else  object-hide  then  loop  drop ;
: fragment-glint ( -- )   \ (Daniella in the first fragment, its plane's normal (1.8 -0.1 1))
    door-fx $FF partner-kind cast-model s" a_fragment0" 1.8e -0.1e 1e object-mirror ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  tv-in  medallion-sparkle
    glass  mirror-now  false fragments ;

\ ---- she weak (till she has rested): reaching to act, she flags ----
: tired ( -- )
    fiona hold  fiona wait-move  fiona knocked-anim start-anim  tired-sound room-bank fiona sound-by  20 frames
    tired-remarked @ if  0 tired-remarked !  get-rest-msg  else  -1 tired-remarked !  not-hot-msg  then  show
    fiona attack-anim 10 start-blend  wait-closed  fiona wait-anim ;

\ ---- the mirror's scene: as it goes, the mirror cracks and shatters, its fragments flash, the
\ door's glass over the whole picture a while, Daniella in a fragment; her face at the end ----
: shot-wait ( shot -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: cue-wait ( frame -- )  begin  cutscene-frame over < while  yield  repeat  drop ;
: mirror-shots ( -- )
    whole-mirror mirror-as  false fragments
    $A shot-wait  true door-glass
    $A03 cue-wait  cracking-1 mirror-as
    $B shot-wait  false door-glass
    $A67 cue-wait  false fragments
    $A70 cue-wait  cracking-2 mirror-as
    $A89 cue-wait  cracking-3 mirror-as
    $AB7 cue-wait  broken-mirror mirror-as  true fragments
    $AE7 cue-wait  false fragments  true door-glass
    $BF3 cue-wait  broken-mirror-2 mirror-as  false door-glass
    $CD2 cue-wait  broken-mirror mirror-as
    begin  cutscene-frame cutscene-shot-at $10 <> while  fragment-glint  yield  repeat
    false door-glass
    $11 shot-wait  daniella-kind 0 s" O_DNL\DNL_202.TEX" face-on ;
: the-mirror ( movie? -- )   \ (Daniella in for it, before the reflections; back as she was)
    castle-2f-5 212 stalker-to  stalker 1 -1 camera-set  partner-kind before-reflections-layer layer
    ['] mirror-shots start drop  panic-eased
    if  incomplete-msg run-scene  then
    ['] mirror-shots stop-doing  false door-glass  partner-kind usual-layer layer ;

\ ---- resting on the bed: the mirror's scene; she well again, Hewie out from under the other bed,
\ Daniella left by the door, the doors the chapter locked open, the file ----
: hewie-comes-out ( -- )
    hewie hold  hewie true noclip  hewie wait-move  hewie $25 -90 13.928e -57.528e place-at
    hewie hewie-hiding-motions motions-for  hewie use-motions  hewie 1 hid-motion play  1 rest-step !
    begin  rest-step @ 2 < while  yield  repeat
    hewie-out-sound common-bank hewie sound-by  hewie 2 hid-motion 5 start-blend8  hewie wait-anim  hewie false noclip ;
: rests ( -- )
    fiona wait-move
    rest-msg yes? 0= if  exit  then
    15 screen:to-black-2 with-music or fade
    mirror-movie scenes:unseen-movie mirror-cutscene load-scene  wait-fade
    c2f-3-tv-examined fact-off  tv-static-sound room-bank stop-sound  0 s" acoustics" actor-named send noise-setting
    hewie let-go
    the-mirror  wait-fade  world-held state!  0 rest-step !
    hewie activate  castle-2f-5 39 0 hewie-to  ['] hewie-comes-out start drop
    stalker $D4 -113 25.182e -33.041e place-at  his-own-time
    saturnus-hint-on fact!  fiona send calm-reset
    fiona $21 49 4.683e -40e place-at  1 -1 player-camera  camera-restart
    1 unlock  3 unlock  $C unlock  no-flee state-off  no-running state-off
    begin  rest-step @ 1 < while  yield  repeat  2 rest-step !
    force-calm state-off  broken-mirror mirror-as  subscreen-locked state-off
    rest-file give-file  15 screen:from-black with-music or fade  wait-fade  his-own-time ;

\ ---- the movie replayed (the extras): Daniella brought in for it ----
: replay-mirror ( -- )
    fiona hold  glass  whole-mirror mirror-as  false fragments
    daniella-kind stalker-ready  yield
    mirror-movie scenes:unseen-movie mirror-cutscene load-scene  the-mirror
    stalker-unloaded  replay-over ;

\ ---- hiding under the bed (the bedroom's); Daniella coming in while she hides: she searches ----
: hides ( -- )  ['] noop ['] noop hide-under-bed ;
: her-looker? ( -- flag )  st-kind-now @ dup daniella-kind = swap kind34-kind = or ;   \ (Daniella, as she comes)
: looks-under ( -- )   \ (Daniella bends down to look under it - her hair over her back - the others see her)
    stalker-to-bed
    her-looker? if  stalker use-motions  1 her-back  stalker room-motion play  0 her-back
    else  stalker stalker.anims:there-anim play  then
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: search-odds ( -- pct )   \ (by the story's variable 2: how often she has come)
    2 var@ case  2 of 25 endof  3 of 50 endof  >r 0 r>  endcase ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    her-looker? if  stalker looker-motions motions-for  then
    search-odds chance?  2 creature-moving? or if  ['] looks-under start drop  else  120 summoner-sooner  then
    2 var@ 1+ 2 var! ;

\ ---- what she may do or say here ----
: window-2 ( -- )
    fiona wait-move  fiona 45 turn  saturnus-hint-on fact? if  sharp-glass-msg  else  pitch-black-msg  then  say ;
: jerky ( -- )  chicken-jerky c2f-3-medallion sparkle-fx -9e -11e picks-up ;
: by-the-bed ( -- )
    bed-area 180 60 facing? 0= if  exit  then
    weak? if  ['] rests unhurried offer exit  then
    stalker-in? if  no-time-msg offer-remark-now  else  ['] hides calm-only offer  then ;
: spots ( -- )
    tv-area 180 90 facing? if  ['] tv unhurried offer  then
    by-the-bed
    fire-area -134 60 facing? if  ['] remark-fire unhurried offer  then
    portrait-1-area 90 60 facing? if  ['] portrait-1 unhurried offer  then
    portrait-2-area 0 60 facing? if  ['] portrait-2 unhurried offer  then
    portrait-3-area -90 60 facing? if  ['] portrait-3 unhurried offer  then
    doll-area 0 50 facing? if  ['] doll unhurried offer  then
    fiona birdcage-area 50 faces-area? if  ['] birdcage unhurried offer  then
    scales-area -90 50 facing? if  ['] scales unhurried offer  then
    chest-area 180 50 facing? if  fg-3-flag3 fact? if  ['] chest-costumes  else  ['] chest  then  unhurried offer  then
    window-area 44 60 facing? if  ['] window-night unhurried offer  then
    window-2-area 44 60 facing? if  ['] window-2 unhurried offer  then
    clock-area -90 50 facing? if  ['] save-clock calm-hewie-up offer  then
    weak?  2 locked-request? and if  ['] tired any-time offer  then
    c2f-3-medallion-found fact?  c2f-3-medallion fact? 0= and  fiona -9e 1e -11e 5e 5e touches? and if
        weak? if  ['] tired  else  ['] jerky  then  calm-only offer
    then ;

\ ---- every frame ----
create cameras  3 , 0 , 0 ,   8 , 1 , -1 ,   9 , 2 , -1 ,   $A , 2 , -1 ,   \ (area, set, path)
create entries  2 , -1 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    fan  events-held state? if  exit  then   \ (a replay: only the fan)
    ambience  sounds-on @ if  tired-sound keep-playing or room-bank fiona sound-by  then
    2 exits-out  ahead 2 ways-out  cameras 4 cameras-at  show-tv
    ['] tired weakness  hewie-sniffs  spots  hewie-looks ;
: fiona-came ( -- )
    events-held state? if  hewie send part-from-fiona  ['] replay-mirror start drop exit  then   \ (the extras)
    entries 2 ways-in ;
: person-in ( i -- )
    case  fiona-p of  fiona-came  endof  stalker-p of  stalker-came  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-5 place!
