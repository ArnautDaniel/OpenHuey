\ story/places/old-mansion-1f-9.fs - a room of the old mansion's 1F caged in steel: the plate
\ pressing machine (a plate pressed while fewer than ten are made; a plate given back); the window -
\ the first time she asks, Daniella's scene (the cage's windows opened, her after her, the file),
\ then only the dark outside; the water to drink (a recovery point - its document the first time);
\ the tub; "The Three Spiritual Substances"; the Refined Antimony, the Refined Magnesia Hewie sniffs
\ out; his looks; the holes things fall through. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-1f-9.fs in the history (converted from the game's; gone with
\ the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-9
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims fiona.sounds scenes ;

own-reset

0 constant cage-msg         \ "I wonder what's going on outside of this steel cage I seemed to have landed myself..."
1 constant pitch-black-msg  \ "It's pitch black outside. I can't see anything. Who could possibly fathom that this..."
2 constant water-msg        \ "Is this water safe to drink?"
3 constant refreshed-msg    \ "Ah, I feel much better now."
4 constant tub-msg          \ "The tub is full of stagnating reddish water. It's probably just rust discoloration. Unl..."
5 constant blinds-msg       \ "Who opened these blinds? It's too dark out to see anything."
6 constant title-msg        \ "'The Three Spiritual Substances'"
7 constant jumble-msg       \ "A strange jumble of letters and numbers... I wonder if this title holds some deeper, un..."
$F constant no-plates-msg   \ "There's a plate pressing machine here. There are no remaining plates currently so..."
$10 constant returned-msg   \ "Returned a plate to the plate pressing machine."
$11 constant press-msg      \ "It's a plate pressing machine."
$12 constant none-left-msg  \ "There are no plates remaining. I'll have to get rid of some plate-keys before I c..."
own: press-remarked  own: rested  own: haze-on  own: hewie-sent
0 constant antimony-fx   1 constant magnesia-fx   2 constant haze-fx   $1B constant window-fx
3 constant calm-sound   6 constant her-motions   2 constant press-screen
1 constant cage-movie   0 constant cage-cutscene   $13 constant cage-figure   3 constant figure-slot
$235 constant cage-file   $2A constant cage-seen
create holes  $78 , $41 , $79 , $42 ,

\ ---- the window's glass; the cage's windows opened ----
: glass ( -- )  window-fx false $40  -43e 30.053e -15.124e  -43e 30.053e 5.068e  -43e 10.053e -15.124e  -43e 10.053e 5.068e  glass-in ;
: windows-open ( -- )  6 2 do  i object-hide  loop  0 show-part ;   \ (its panes, the room's objects 2..5: mado0..3)
: came-in ( exit -- )
    drop  room-sounds  glass
    om1f-9-refined-antimony fact? 0= if  antimony-fx -7.97e 12.42e 21.3e flicker-at  then
    om1f-9-refined-magnesia-found fact?  om1f-9-refined-magnesia fact? 0= and if  magnesia-fx 7.72e 1e 15.08e flicker-at  then
    4 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the plate pressing machine; a plate given back ----
: the-press ( -- )
    stalkers-stay state!  fiona wait-move
    plate-pressable? if
        press-remarked @ 0= if  press-msg say  -1 press-remarked !  then
        plate-made fact-off  press-screen open-subscreen  yield
        plate-made fact? if  word-plate-key item-named  word-plate-key show-item-tab  fanfare  obtained-2-msg say  then
        plate-made fact!
    else  press-remarked @ if  none-left-msg  else  -1 press-remarked !  no-plates-msg  then  say  then
    stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )   \ (a plate given back: her word)
    2drop  swap 0<> if  drop false exit  then
    1 = if  [: fiona wait-move  returned-msg say ;] start drop  true  else  false  then ;

\ ---- the cage's window: the first time, Daniella's scene ----
: hazy ( on -- )   \ (the scene's haze between its cues)
    dup haze-on !  if  effects:haze haze-fx effect!  else  haze-fx effect-off  then ;
: till-shot ( n -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: till-cue ( frame -- )  begin  cutscene-frame over < while  yield  repeat  drop ;
: opening ( -- )   \ (its depth range opening from frame 1156: the far two on by 1 a frame up to 80 / 140)
    cutscene-frame 1156 - s>f {: F: d :}  1e 1e  d 40e f+ 80e fmin  d 100e f+ 140e fmin  depth-range ;
: scene-looks ( -- )   \ (along the scene's shots and cues: her layer, the haze, the fog, the depth)
    depth-range-off  1 till-shot  0 before-reflections-layer layer  3 till-shot  0 usual-layer layer
    true hazy  5 till-shot  false hazy  6 till-shot
    begin  cutscene-frame $446 < while  $80808000 $404A6038 14e 420.732e fog  yield  repeat
    $44E till-cue
    begin  cutscene-frame $484 < while  1e 1e 40e 100e depth-range  yield  repeat
    begin  cutscene-frame $4B0 < while  opening  yield  repeat
    depth-range-off  begin  cutscene-frame 0< 0= while  yield  repeat  wait-fade ;
: cage-scene ( -- )   \ (the figure loaded; her scene - its looks along it)
    cage-figure figure-slot figure-in  yield
    cage-movie scenes:unseen-movie cage-cutscene load-scene  wait-fade
    ['] scene-looks start drop  0 haze-on !
    4 cutscene-keep-model  figure-slot figure-on
    panic-eased  if  -1 run-scene  then ;
: after-scene ( -- )   \ (back in the cage, its windows opened)
    fiona $74 -107 -32.419e -4.78e place-at  camera-restart  figure-slot figure-out  0 usual-layer layer
    windows-open  haze-on @ if  false hazy  then ;
: daniella-outside ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    cage-msg yes? if
        om1f-13-flag1 fact!  om1f-9-flag1 fact!  15 screen:to-black-2 with-music or fade
        hewie-in? hewie-sent !  hewie-sent @ if  hewie let-go  then
        cage-scene  world-held state!  after-scene
        hewie-sent @ if  hewie activate  old-mansion-1f-9 88 0 hewie-to  then
        old-mansion-1f-9 41 stalker-to  stalker $29 -91 33.75e -4.38e place-at  stalker 0 -1 camera-set  his-own-time
        15 fade-in  cage-file give-file  $50 threat  cage-seen resident-flag-set
    then
    stalkers-stay state-off ;
: the-window ( -- )
    om1f-13-flag1 fact? 0= if  daniella-outside exit  then
    fiona wait-move
    om1f-9-flag1 fact? if  pitch-black-msg say exit  then
    fiona -90 turn  fiona look-close-anim start-anim  40 frames  blinds-msg say  fiona wait-anim ;
: replayed ( -- )
    fiona hold  daniella-kind stalker-ready  yield  cage-scene  after-scene  stalker-unloaded  replay-over ;

\ ---- what she may do or say here ----
: the-water ( -- )   \ (a recovery point: once a visit it calms her)
    rested @ if  fiona wait-move  refreshed-msg say exit  then
    -1 rested !  water-msg her-motions calm-sound $22 30 18.764e 15.099e recovery-point ;
: the-tub ( -- )  fiona wait-move  fiona -6e -17e turn-to-xz  tub-msg say ;
: the-title ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  4 frames
    fiona $3A 0 -19e 21e place-at  world-frozen state!  1 12e -10e 0e 7e shot  title-msg say
    world-frozen state-off  shot-off  4 frames
    fiona hand-to-mouth-anim 10 start-blend  jumble-msg say  fiona wait-anim  stalkers-stay state-off ;
: take-antimony ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -7.97e 21.3e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  refined-antimony om1f-9-refined-antimony antimony-fx takes-up  fiona reach-back-anim play ;
: take-magnesia ( -- )  refined-magnesia om1f-9-refined-magnesia magnesia-fx 7.72e 15.08e picks-up ;
: magnesia-sniffed ( -- )  om1f-9-refined-magnesia-found om1f-9-refined-magnesia magnesia-fx  7.72e 1e 15.08e  hewie-finds ;

create looks  21.03e 0e -17.8e  20e 12e 12e look,   -41.22e 4.5e -4.36e  28e 12e 12e look,
: each-frame ( -- )
    0 exit-out  looks 2 hewie-looks-at
    om1f-9-refined-magnesia-found fact? 0=  scrambling? and  hewie 7.72e 0e 15.08e 10e 5e touches? and if  ['] magnesia-sniffed start-only  then
    fiona 2 50 faces-area? if  ['] the-press unhurried offer  then
    fiona 1 in-area?  fiona -45 50 heading? and if  ['] the-window unhurried offer  then
    fiona 3 in-area?  fiona 15 50 heading? and if  ['] the-water unhurried offer  then
    fiona 4 in-area?  fiona 50 -6e -22e faces-point? and if  ['] the-tub unhurried offer  then
    fiona 5 in-area?  fiona 0 50 heading? and if  ['] the-title unhurried offer  then
    om1f-9-refined-antimony fact? 0=  fiona -7.97e 12.42e 21.3e 5e 5e touches? and if  ['] take-antimony calm-only offer  then
    om1f-9-refined-magnesia-found fact?  om1f-9-refined-magnesia fact? 0= and  fiona 7.72e 0e 15.08e 5e 5e touches? and if
        ['] take-magnesia calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    0 0 -1 way-in
    om1f-13-flag1 fact? if  windows-open  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-9 place!
' items-used old-mansion-1f-9 items-here
