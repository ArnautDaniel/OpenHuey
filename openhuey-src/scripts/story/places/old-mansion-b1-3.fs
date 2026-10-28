\ story/places/old-mansion-b1-3.fs - the torture chamber of the old mansion's B1: four floor plates
\ (pressed in by whoever stands on them - Hewie sits on one when he's sent there) - two at once put
\ out one of the four flames (2 and 3, 1 and 4, 2 and 4, 1 and 3), side by side (1 and 2, 3 and 4)
\ they all light again; all four out, the statue's scene (the figure drained); the corpse in its
\ chair - the first time a scene, then reaching for it she is held (struggling free - her shakes,
\ her panic, or Hewie at it), after the statue the Venus Key from it, then its remarks; the engraved
\ tool and its text; the iron maiden (its spikes, lowered by the lever - then a place to hide: the
\ stalker's search; Daniella shuts it on her - the game over); the oven, the birdcage, the
\ stretcher, the sabers, the breaker, the instruments; the Camomile; Hewie tense, wary, his looks;
\ its hum; the replays. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-b1-3.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-b1-3
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names keys story.kit fiona.anims fiona.sounds scenes ;

own-reset

0 constant corpse-msg     \ "A corpse. There are traces that suggest this person was heavily tortured... There i..."
1 constant drained-msg    \ "Any remaining life energy has been totally drained away. Did the statue suck its li..."
2 constant peaceful-msg   \ "Strange, but the expression on its face looks somehow... peaceful. Maybe it was her..."
3 constant tool-msg       \ "Is this a tool used to torture innocent victims? Tiny letters are engraved..."
4 constant fire-msg       \ "'Hotness and dryness form the element of fire' What could it mean?"
9 constant engraved-msg   \ "Fire is essentially composed of hotness and dryness. Air differs only from fire in that..."
$A constant oven-msg      \ "The oven burns an angry red. I'd rather not even think about the kind of things they b..."
$B constant birdcage-msg  \ "Is this a birdcage? It just seems too large to be a cage for birds."
$C constant person-msg    \ "Could it have housed a person? No. No one could fit inside it."
$D constant stretcher-msg \ "There's a handle that winds the attached rope up. Looks like this was used as a stretc..."
$E constant sabers-msg    \ "Thin-bladed sabers line the wall. Several of them appear to be coated with rust and ca..."
$F constant breaker-msg   \ "A circuit breaker."
$10 constant chair-msg    \ "The end of the cord connects to that chair... Which can mean only one thing. This..."
$12 constant tools-msg    \ "Restraints, a whip, and various surgical instruments. Dried blood covers all of the s..."
$13 constant spikes-msg   \ "Huge spikes jut out from inside the iron maiden, rendering the device unusable as..."
own: plates-on  own: pairs-held  own: flames-out  own: dog-plates  own: hum-on
own: drained-remarked  own: birdcage-remarked  own: breaker-remarked
own: holding  own: hewie-came  own: free  own: near-panic  own: cried  own: cry-frames  own: threat-frames
own: noticed-5  own: noticed-6  own: dog-step
5 constant camomile-fx   0 constant shut-sound   1 constant open-sound   2 constant climb-sound   3 constant shriek-sound
4 constant hum-sound   5 constant step-sound   6 constant press-sound   7 constant shut-2-sound   8 constant relit-sound   9 constant out-sound
$11 constant corpse   3 constant corpse-slot   $17 constant search-var   5 constant maiden-area

\ ---- the flames: lit (kind 2), or past the puzzle (kind 3) ----
create flames  -0.5e f, 18.5e f, -35.5e f,   9.5e f, 18.5e f, -35e f,   4.8e f, 25.5e f, -37.5e f,   4.8e f, 6.2e f, -33.4e f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;
: flames-lit ( kind -- )  {: kind :}  4 0 do  i kind  i flame@  flame-at  loop ;

\ ---- the plates: who stands on them; pressed in, or up ----
create plate-tris  $89 , $B1 ,  $90 , $B2 ,  $97 , $B3 ,  $83 , $B0 ,
: on-tri? ( id tri -- flag )  over here? 0= if  2drop false exit  then  swap body-tri = ;
: on-plate? ( id p -- flag )  {: id p | a :}  p 2* cells plate-tris + to a  id a @ on-tri?  id a cell+ @ on-tri? or ;
: plate-held? ( p -- flag )  {: p :}  fiona p on-plate?  hewie p on-plate? or  stalker p on-plate? or ;
: plate ( p -- addr len )  this-room @ story.strings:room-string ;
: plate-moves ( p -- )   \ (down 0.2 a frame to -0.7 while held - a sound as it starts - else up to 0)
    {: p :}  p plate placed-here? 0= if  exit  then
    plates-on @ p rshift 1 and if
        p plate place-y object@ f0= if  press-sound room-bank  p plate place-x object@  p plate place-y object@  p plate place-z object@  sound-there  then
        p plate place-y -0.2e object+!  p plate place-y object@ -0.7e fmax  p plate place-y object!
    else
        p plate place-y 0.2e object+!  p plate place-y object@ 0e fmin  p plate place-y object!
    then ;
: plates ( -- )  0  4 0 do  i plate-held? if  1 i lshift or  then  loop  plates-on !  4 0 do  i plate-moves  loop ;
: on? ( p -- flag )  plates-on @ swap rshift 1 and ;
: pair ( a b -- flag )  on? swap on? and ;
: newly ( bit flag -- new? )   \ (a pair held: true the frame it's first held)
    {: bit on :}  pairs-held @ bit and 0=  on and  on if  pairs-held @ bit or  else  pairs-held @ bit invert and  then  pairs-held ! ;
: put-out ( k -- )  {: k :}  flames-out @ 1 k lshift and if  exit  then
    1 k lshift flames-out @ or flames-out !  out-sound room-bank  k flame@  sound-there  k effect-off ;
: all-lit ( -- )   \ (side by side: the flames all lit again)
    flames-out @ if  relit-sound room-bank  3 flame@  sound-there  then  0 flames-out !  2 flames-lit ;
: flames-puzzle ( -- )
    1 1 2 pair newly if  0 put-out  then   2 0 3 pair newly if  1 put-out  then
    4 1 3 pair newly if  2 put-out  then   8 0 2 pair newly if  3 put-out  then
    16 0 1 pair newly if  all-lit  then    32 2 3 pair newly if  all-lit  then ;

\ ---- the corpse in its chair (figure $11): the Venus Key on it till she takes it ----
: corpse-part ( part on -- )  corpse cast-model dup 0< if  drop 2drop exit  then  -rot actor-part-show ;
: key-shown ( on -- )  dup 5 swap corpse-part  dup 6 swap corpse-part  10 swap corpse-part ;
: corpse-seated ( anim -- )  corpse-slot swap 0 0 figure-play  corpse-slot 9.97e 0e -0.59e 0e figure-at ;
: corpse-in ( -- )
    corpse-slot figure 0< if  corpse corpse-slot figure-in  yield  then
    corpse-slot figure-on  got-venus-key fact? 0= key-shown  $9005 corpse-seated ;

\ ---- the iron maiden: its spikes (by the lever) ----
: spikes ( down? -- )   \ (lever and spikes: objects $A/$B and $F/$10)
    if  $A object-show  $B object-hide  $F object-show  $10 object-hide
    else  $A object-hide  $B object-show  $F object-hide  $10 object-show  then ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  4 0 do  i plate placed-here? if  i plate place-y 0e object!  then  loop
    omb1-3-flag2 fact? if  3  else  2  then  flames-lit
    ['] corpse-in start drop  omb1-3-flag3 fact? spikes  2 show-part  $E object-hide  5 hide-part
    omb1-3-camomile fact? 0= if  camomile-fx -20.96e 8.5e 5.57e flicker-at  then  1 $2300 reverb ;

\ ---- all four out: the statue's scene ----
: hewie-hidden-by ( -- )  hewie hold  hewie false shown  1 dog-step !  begin  dog-step @ 2 < while  yield  repeat  hewie true shown ;
: statue-scene ( -- )   \ (her shadow off the corpse till shot 2)
    corpse cast-model 0< 0= if  corpse true no-shadow  then
    begin  cutscene-frame cutscene-shot-at 2 <> while  yield  repeat  corpse false no-shadow ;
: all-out ( -- )
    0 dog-step !  stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade
    7 scenes:keyed-movie 6 load-scene  wait-fade  ['] statue-scene start drop  fiona false shown
    hewie-in? if  ['] hewie-hidden-by start drop  begin  dog-step @ 1 < while  yield  repeat  then
    $10 $FF movie-key  panic-eased  if  -1 run-scene  then  wait-fade
    50 trust+  fiona 0 idle  camera-restart  fiona true shown  2 dog-step !
    3 flames-lit  $9005 corpse-seated  corpse false no-shadow  old-mansion-b1-3 stalker-slot 0 doors-into
    15 fade-in  $827E give-file  stalkers-stay state-off ;

\ ---- the corpse: its scene the first time; reaching for it, she is held - struggling free ----
: held-pose ( -- )   \ (by her panic: the corpse's grip and her struggle)
    panic-stage @ 2 < if  corpse-slot $9001 0 5 figure-play  fiona room-motion 3 + 5 play-blend  then
    panic-stage @ 2 4 within if  corpse-slot $9002 0 5 figure-play  fiona room-motion 4 + 5 play-blend  then ;
: struggling ( -- )
    -1 holding !  0 hewie-came !  0 free !  0 near-panic !  0 cry-frames !  0 threat-frames !
    0 shakes-now !  true fiona send count-shakes
    begin
        hewie-cmdable-now @ if  $61 0 hewie-do  then
        fiona-cmd-now @ 0 2 within  cried @ 0= and if  -1 cried !  $38 common-bank fiona sound-by  then
        cried @ if  1 cry-frames +!  cry-frames @ 60 = if  0 cry-frames !  0 cried !  then  then
        shakes-now @ 35 >= if  -1 free !  false fiona send count-shakes  then
        1 threat-frames +!  threat-frames @ 12 = if  10 fright+  0 threat-frames !  then
        panic-level-now @ 98 >= if  -1 free !  -1 near-panic !  then
        free @ 0=  fiona went-round? and if  held-pose  then
        hewie-came @  fiona went-round?  free @ and or  0= while  yield
    repeat
    0 holding !  shot-off  hewie-cmdable-now @ if  0 0 hewie-do  then
    corpse-slot $9003 0 5 figure-play  fiona room-motion 5 + 3 play-blend  $9005 corpse-seated
    near-panic @ if  4 panic-to  then  omb1-3-flag1 fact!  $2F resident-flag-set ;
: the-corpse ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    omb1-3-flag1 fact? 0= if
        fiona $8D 180 9e 10e walk-to
        corpse-msg yes? 0= if  stalkers-stay state-off exit  then
        15 screen:to-black-2 with-music or fade  5 scenes:unseen-movie 4 load-scene  wait-fade
        hewie-in?  hewie-cond-now @ down-cond = and if  ['] hewie-hidden-by start drop  then
        panic-eased  if  -1 run-scene  then  wait-fade
        fiona 0 idle  fiona $2F 180 10.8e 14.07e place-at  $9005 corpse-seated  camera-restart  2 dog-step !
        world-held state!  fiona 8 motions-for  fiona use-motions  world-held state-off  15 screen:from-black fade
        fiona hold  1 27e 10e -40e 0e shot  fiona $2F 180 12.34e 13.19e place-at  $9001 corpse-seated
        fiona room-motion 3 + start-anim
    else
        corpse-msg yes? 0= if  stalkers-stay state-off exit  then
        fiona 8 motions-for  fiona use-motions  1 27e 10e -40e 0e shot  fiona $2F 180 10.8e 14.07e place-at
        $9000 corpse-seated  fiona hold  fiona room-motion 2 + 5 start-blend  23 frames  step-sound room-bank fiona sound-by  fiona wait-anim
    then
    struggling  stalkers-stay state-off ;
: hewie-at-it? ( -- )   \ (Hewie at the corpse while she's held: she's freed)
    holding @ 0=  hewie-came @ or if  exit  then
    10.8e 0e 14.07e 10e 200e  hewie blow-in? if  -1 hewie-came !  then ;
: drained ( -- )
    fiona wait-move  fiona 10e 0e turn-to-xz
    drained-remarked @ if  0 drained-remarked !  peaceful-msg  else  -1 drained-remarked !  drained-msg  then  say ;
: take-key ( -- )
    fiona wait-move  fiona 10e 0e turn-to-xz  fiona reach-out-anim play  4 frames
    got-venus-key fact!  false key-shown  venus-key got-venus-key -1 takes-the  fiona reach-back-anim play ;
: the-tool ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  world-frozen state!  1 20e -15e 0e 0e shot
    fiona $97 180 5e -27e place-at  4 frames  tool-msg say
    15 screen:to-dim fade wait-fade  engraved-msg say  15 screen:from-dim fade wait-fade
    shot-off  world-frozen state-off  fire-msg say  stalkers-stay state-off ;

\ ---- the iron maiden: the lever lowering its spikes; inside it, hidden ----
: the-lever ( -- )
    stalkers-stay state!  fiona hold  fiona 9 motions-for  fiona wait-move  fiona 5 0 9.83e 27.45e walk-to  fiona use-motions
    omb1-3-flag3 fact? 0= if
        fiona room-motion 4 + 5 start-blend8  5 frames  false spikes  $B 1 0 object  $10 1 0 object
        25 frames  open-sound room-bank fiona sound-by  55 frames  shut-sound room-bank fiona sound-by  fiona wait-anim  omb1-3-flag3 fact!
    else
        fiona room-motion 3 + 5 start-blend8  5 frames  true spikes  $A 1 0 object  $F 1 0 object
        25 frames  open-sound room-bank fiona sound-by  55 frames  shut-sound room-bank fiona sound-by
        5 frames  shut-2-sound room-bank fiona sound-by  fiona wait-anim  omb1-3-flag3 fact-off
    then  stalkers-stay state-off ;
: maiden-out ( -- )   \ (found, in a panic, or him here: out of it)
    found-her @ if  2 avoid-prompt  then
    fiona follow  fiona-hidden state-off  fiona true noclip  fiona room-motion 2 + 5 start-blend8  5 frames
    climb-sound room-bank fiona sound-by  $E 1 2 object  fiona wait-anim  fiona false noclip
    found-her @ 0= if  stalker let-go  stalker-in? if  0 stalker-order  then  then ;
: the-maiden ( -- )
    omb1-3-flag3 fact? 0= if  fiona wait-move  fiona $3C 90 32e 20.63e walk-to  spikes-msg say exit  then
    stalkers-stay state!  0 came-while-hidden !  fiona hold  fiona 9 motions-for  fiona wait-move
    fiona $3C 90 32e 20.63e walk-to  fiona true noclip  fiona use-motions  fiona room-motion 5 start-blend8  5 frames
    2 hide-part  $E object-show  $E 1 0 object  36 frames  climb-sound room-bank fiona sound-by  fiona wait-anim
    fiona -1 idle  fiona false noclip  stalkers-stay state-off  fiona-hidden state!  0 found-her !  0 avoid-prompt
    mandragora? if  shriek-sound room-bank fiona sound-by  then  3 -1 fixed-camera
    begin
        circle button-pressed? if
            stalker-in? if  maiden-out exit  then
            fiona follow  fiona-hidden state-off  fiona true noclip  fiona room-motion 1+ 5 start-blend8  5 frames
            climb-sound room-bank fiona sound-by  $E 1 1 object  fiona wait-anim  fiona false noclip  exit
        then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound common-bank fiona sound-by  then  maiden-out exit  then
        hidden-calm  yield
    again ;

\ ---- the stalker in while she hides: Daniella shuts it on her (the game over); the others see her ----
: impaled ( -- )
    capture-no-end state!  no-pause state!  fiona hold  begin  dog-step @ 1 < while  yield  repeat
    15 screen:to-black-2 with-music or fade  $12 scenes:keyed-movie $11 load-scene  wait-fade
    hewie let-go  $A object-hide  $B object-hide
    partner-kind dup daniella-kind = swap kind34-kind = or if  4 cutscene-keep-model  then
    $10 $50 movie-key  fiona false shown  panic-eased  if  -1 run-through  then
    $2E resident-flag-set  gameover:movie-death game-over ;
: shuts-it ( -- )
    stalker hold  things-clear  0 dog-step !  panic-eased  ['] impaled start drop
    stalker wait-move  0 door-for-him  stalker $3C 90 32e 20.63e walk-to  1 dog-step !  begin  yield  again ;
: sees-her ( -- )
    stalker hold  stalker wait-move  0 door-for-him  stalker 7 118 26.16e 27.36e walk-to  stalker stalker.anims:there-anim play
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: search-odds ( -- pct )  search-var var@ case  0 of 0 endof  1 of 50 endof  2 of 50 endof  3 of 50 endof  >r 0 r>  endcase ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    search-odds chance?  2 creature-moving? or if
        partner-kind dup daniella-kind = swap kind34-kind = or if  ['] shuts-it  else  ['] sees-her  then  start drop
    else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;

\ ---- what she may look at here ----
: turned-say ( deg msg -- )  {: deg msg :}  fiona wait-move  fiona deg turn  msg say ;
: the-oven ( -- )  110 oven-msg turned-say ;
: the-birdcage ( -- )
    birdcage-remarked @ if  0 birdcage-remarked !  90 person-msg  else  -1 birdcage-remarked !  90 birdcage-msg  then  turned-say ;
: the-stretcher ( -- )  0 stretcher-msg turned-say ;
: the-sabers ( -- )  -45 sabers-msg turned-say ;
: the-breaker ( -- )
    breaker-remarked @ 0= if  -1 breaker-remarked !  -90 breaker-msg turned-say exit  then
    fiona wait-move  fiona -90 turn  fiona look-point-move 0 0  10e 10e 0e 0e  move  yield  chair-msg say  fiona -1 look-at  yield ;
: the-tools ( -- )  fiona wait-move  fiona -90 turn  fiona look-low-anim start-anim  16 frames  tools-msg say  fiona wait-anim ;
: take-camomile ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -20.96e 5.57e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  camomile omb1-3-camomile camomile-fx takes-up  fiona reach-back-anim play ;

\ ---- Hewie: on a plate he's sent to he stays; tense, wary; his looks ----
create dog-spots  23e f, -22.5e f,  11e f, -22.5e f,  -1e f, -22.5e f,  -13e f, -22.5e f,
: dog-on-plates ( -- )
    hewie-in?  hewie-cond-now @ well-cond = and 0= if  exit  then
    4 0 do
        dog-plates @ i rshift 1 and if
            hewie  dog-spots i 2* floats + dup f@ 0e float+ f@  7e 5e centred-in? 0= if  dog-plates @ 1 i lshift invert and dog-plates !  then
        else
            chased? 0=  hewie-cmdable-now @ and  hewie  dog-spots i 2* floats + dup f@ 0e float+ f@  5e 5e centred-in? and if
                dog-plates @ 1 i lshift or dog-plates !  $80 0 hewie-do
            then
        then
    loop ;
: tense ( -- )  37.99e 0e 24.16e hewie-barks-tense ;
: wary ( -- )  11.81e 0e -0.03e hewie-stands-by ;
create far-look  37.99e 0e 24.16e  20e 14e 14e look,
create corpse-look  11.81e 0e -0.03e  20e 6e 6e look,
: hewie-about ( -- )
    dog-on-plates  hewie-at-it?
    holding @ if  exit  then
    noticed-5 50 37.99e 0e 24.16e 20e 14e notices? if  ['] tense start drop  then
    far-look 1 hewie-looks-unchased
    omb1-3-flag2 fact? if  exit  then
    noticed-6 100 11.81e 0e -0.03e 20e 6e notices? if  ['] wary start drop  then
    corpse-look 1 danger-now @ followed = hewie-looks-if ;

\ ---- the replays (the extras) ----
: replay-maiden ( -- )
    fiona hold  daniella-kind stalker-ready  yield  $12 scenes:keyed-movie $11 load-scene
    $A object-hide  $B object-hide  $F object-show  $10 object-hide  $E object-hide  4 cutscene-keep-model
    $10 $50 movie-key  fiona false shown  panic-eased  if  -1 run-scene  then  fiona true shown  stalker-unloaded  replay-over ;
: replay-corpse ( -- )
    fiona hold  true spikes  $E object-hide  corpse-in  5 scenes:unseen-movie 4 load-scene
    panic-eased  if  -1 run-scene  then  3 figure-out  replay-over ;
: replay-statue ( -- )
    fiona hold  true spikes  $E object-hide  corpse-in  7 scenes:keyed-movie 6 load-scene  ['] statue-scene start drop
    fiona false shown  $10 $FF movie-key  panic-eased  if  -1 run-scene  then  fiona true shown  3 figure-out  replay-over ;

\ ---- its hum ----
: hums ( -- )
    hum-on @ if  hum-sound 40e 5e -26e keep-sound
    else  room-sounds-in? if  hum-sound 40e 5e -26e room-sound  -1 hum-on !  then  then ;

create cameras  1 , 0 , -1 ,   2 , 1 , -1 ,   \ (area, set, path)
: facing? ( area deg -- flag )  {: a d :}  fiona a in-area?  fiona d 50 heading? and ;
: each-frame ( -- )
    hums  0 exit-out  cameras 2 cameras-at  plates
    omb1-3-flag2 fact? 0= if
        flames-puzzle
        flames-out @ $F =  fiona held? 0= and if  omb1-3-flag2 fact!  panic-eased  ['] all-out start drop  then
    then
    hewie-about
    3 180 facing? if  ['] the-tool unhurried offer  then
    fiona 4 in-area?  fiona 50 10e 0e faces-point? and if
        omb1-3-flag2 fact? 0= if  ['] the-corpse unhurried
        else  got-venus-key fact? 0= if  ['] take-key calm-only  else  ['] drained unhurried  then  then  offer
    then
    maiden-area 90 facing? if
        omb1-3-flag3 fact? 0= if  ['] the-maiden unhurried offer
        else  stalker-in? if  no-time-message offer-remark-now  else  ['] the-maiden calm-only offer  then  then
    then
    6 0 facing? if  ['] the-lever unhurried offer  then
    7 110 facing? if  ['] the-oven unhurried offer  then
    8 90 facing? if  ['] the-birdcage unhurried offer  then
    9 0 facing? if  ['] the-stretcher unhurried offer  then
    $A -44 facing? if  ['] the-sabers unhurried offer  then
    $B -90 facing? if  ['] the-breaker unhurried offer  then
    $C -90 facing? if  ['] the-tools unhurried offer  then
    omb1-3-camomile fact? 0=  fiona -20.96e 8.5e 5.57e 5e 5e touches? and if  ['] take-camomile calm-only offer  then ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if
                hewie send part-from-fiona
                replay-act case  $14 of  ['] replay-maiden  endof  $15 of  ['] replay-corpse  endof  >r ['] replay-statue r>  endcase
                start drop  exit
            then
            0 0 -1 way-in
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )  false fiona send count-shakes ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-3 place!
