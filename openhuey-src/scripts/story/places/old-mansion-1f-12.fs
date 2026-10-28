\ story/places/old-mansion-1f-12.fs - the three-headed dragon's hall of the old mansion's 1F: a word
\ plate into each of the low heads' mouths (the right element: its jaw shut; else "Nothing
\ happened"); the highest one out of her reach - with Hewie by her the plate goes in (its scene;
\ right, the fire pillar frozen to ice: Hewie's scene, his trust up); the ice pillar climbed up or
\ down; the box by the fire opened as a spirit is put away by it (the Film Reel in it); the statue
\ with its lamp; the door locked (its symbol; the Venus Key); the Torn Letter in a mouth; the ivy;
\ the Rubrum Medallion Hewie sniffs out; Hewie tense at the fire, his looks; the replays. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-12.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-12
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit fiona.anims scenes ;

own-reset

0 constant dragon-msg     \ "A 3-headed monster. Is this some kind of dragon? Its wide-open mouth is very un..."
1 constant one-more-msg   \ "The highest dragon head's mouth is still open. I'll need to insert one more plate."
2 constant solved-msg     \ "All 3 mouths are now tightly shut. Looks like I've completely solved this puzzle..."
3 constant nothing-msg    \ "Nothing happened. This must not be the correct plate..."
4 constant too-short-msg  \ "Darn it! I'm just not tall enough. If only Hewie were here, I bet it would be doable."
5 constant flame-msg      \ "A pillar of seething flame is spewing forth from the floor. There is something up a..."
6 constant ice-msg        \ "The flame has been frozen in place, creating a crystalline ice pillar. This could b..."
8 constant locked-msg     \ "The door is locked..."
9 constant symbol-msg     \ "A strange symbol is written on the door. I wonder what it could mean?"
$A constant climb-down-msg \ "I can use this to get down. Climb down the ice pillar? YES NO"
$B constant statue-msg    \ "A statue grasping firmly onto an oil lamp. I know I've seen this somewhere bef..."
$C constant trick-msg     \ "I knew there was some trick to solving it!"
$D constant mouth-msg     \ "It's holding something in its mouth."
$E constant ivy-msg       \ "Ivy has grown in through the collapsed area where the floor used to be... Looks like i..."
own: locked-remarked  own: box-seen  own: fire-noticed  own: sounds-on  own: plate-right
0 constant letter-fx   1 constant medallion-fx   2 constant smoke-fx
0 constant box-sound   1 constant lever-sound   2 constant plate-sound   3 constant key-sound
4 constant fire-sound   5 constant climb-sound   $609 constant key-turn-anim   $6C constant marked-door
4 constant high-movie   3 constant high-cutscene   6 constant ice-movie   5 constant ice-cutscene
5 constant high-plate   6 constant right-plate   7 constant left-plate   $A constant out-of-reach   1 constant venus-key-used

\ ---- the heads (the room's objects 0..2: atama_1..3), their jaws shut ----
: head ( k -- addr len )  this-room @ story.strings:room-string ;
: shut ( k -- )  head turn-x -0.65e object! ;
: shuts ( k -- )   \ (lowered 0.02 a frame - not while a movie plays - till shut)
    {: k :}
    begin
        movie-state 0< if  k head turn-x -0.02e object+!  then
        k head turn-x object@ -0.65e f> while  yield
    repeat  k shut ;
: flames ( lit? -- )   \ (the pillar of flame, or frozen to ice)
    if  5 show-part  6 hide-part  2 show-part  effects:spiral-smoke smoke-fx effect!
    else  5 hide-part  6 show-part  1 show-part  smoke-fx effect-off  effects:dust-shaft  then ;
: came-in ( exit -- )
    drop  room-sounds
    om1f-12-flag3 fact? if  0 shut  then  om1f-12-flag1 fact? if  1 shut  then  om1f-12-flag2 fact? if  2 shut  then
    om1f-12-flag4 fact? if  0 show-part  3 show-part  8 object-hide  else  0 hide-part  3 hide-part  8 object-show  then
    got-film-reel fact? if  4 hide-part  else  4 show-part  then
    om1f-12-flag1 fact? 0= flames
    got-torn-letter fact? 0=  om1f-12-flag2 fact? 0= and if  letter-fx 12e 13e -24e flicker-at  then
    om1f-12-rubrum-medallion-found fact?  om1f-12-rubrum-medallion fact? 0= and if  medallion-fx 52.04e 1e 5.68e flicker-at  then ;

\ ---- a plate in a low head's mouth ----
: in-the-mouth ( F: x z -- )   \ (turned to it, the plate pushed in)
    {: F: x z :}  stalkers-stay state!  fiona hold  fiona wait-move  fiona x z turn-to-xz
    fiona reach-out-anim play  plate-sound room-bank x 10e z sound-there  16 frames  fiona reach-back-anim play ;
: low-head ( k fact F: x z -- )
    {: k f F: x z :}  word-right-now @ plate-right !  x z in-the-mouth
    plate-right @ if
        k 2 = if  letter-fx effect-off  then
        lever-sound room-bank x 10e z sound-there  k shuts  f fact!
    else  30 frames  nothing-msg say  then
    stalkers-stay state-off ;
: right-head ( -- )  2 om1f-12-flag2 12e -24e low-head ;
: left-head ( -- )  0 om1f-12-flag3 -12e -24e low-head ;

\ ---- the highest head: out of her reach; with Hewie by her, in it goes ----
: too-short ( -- )  fiona hold  fiona wait-move  fiona 0e -27e turn-to-xz  fiona look-at-anim start-anim  16 frames  too-short-msg say  fiona wait-anim ;
: high-shuts ( -- )   \ (at the scene's cue, the highest jaw shuts - the scene's own: not if it's cut short)
    begin  cutscene-frame dup 0< 0=  swap $44C < and while  yield  repeat
    cutscene-frame 0< if  exit  then  lever-sound room-bank 0e 30e -27e sound-there  1 shuts ;
: high-scene ( -- )
    high-movie scenes:keyed-movie high-cutscene load-scene  wait-fade  plate-right @ if  ['] high-shuts start drop  then
    $10 $C8 movie-key  panic-eased  if  -1 run-scene  then  ['] high-shuts stop-doing ;
: ice-scene ( -- )  ice-movie scenes:keyed-movie ice-cutscene load-scene  wait-fade  $10 $C8 movie-key  panic-eased  if  -1 run-scene  then ;
: frozen ( -- )   \ (the third right: the fire frozen - Hewie's scene)
    1 shut  5 hide-part  hewie let-go  ice-scene  wait-fade
    hewie activate  old-mansion-1f-12 163 0 hewie-to  hewie $A3 -45 -22.21e -11.73e place-at  50 trust+
    false flames  fire-sound room-bank stop-sound  om1f-12-flag1 fact! ;
: with-hewie ( -- )
    word-right-now @ plate-right !  stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade
    2 hide-part  smoke-fx effect-off  high-scene  wait-fade
    world-held state!  fiona 0 idle  fiona $11E 180 -5.7e 17e place-at  hewie $A3 -45 -22.21e -11.73e place-at  camera-restart
    plate-right @ if  frozen  else  2 show-part  5 show-part  effects:spiral-smoke smoke-fx effect!  then
    15 fade-in
    plate-right @ 0= if  16 frames  nothing-msg say  then
    $827D give-file  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    case
        high-plate of  ['] with-hewie  endof
        right-plate of  ['] right-head  endof
        left-plate of  ['] left-head  endof
        out-of-reach of  ['] too-short  endof
        venus-key-used of  [: 0 marked-door venus-key key-turn-anim key-sound room-bank key-turns ;]  endof
        >r false r>  endcase
    dup if  start drop  true  then ;

\ ---- the box by the fire: a spirit put away beside it, it opens ----
: box-opens ( -- )
    16 frames  0 show-part  box-sound room-bank -38e 5e -8e sound-there
    begin  s" sikakebox" turn-y -0.025e object+!  s" sikakebox" turn-y object@ -0.78e f> while  yield  repeat
    s" sikakebox" turn-y -0.78e object! ;
: box-due ( -- )
    box-seen @  om1f-12-flag4 fact? or  cam-set@ 1 > or if  exit  then
    -35.7e -7.5e 4e spirit-put-away-near? if  -1 box-seen !  om1f-12-flag4 fact!  ['] box-opens start drop  then ;

\ ---- what she may do or say here ----
: the-door ( -- )   \ (locked; then its symbol)
    locked-remarked @ 0=  chased? or  in-panic? or if  0 false locked-msg tries-locked  -1 locked-remarked !  exit  then
    fiona wait-move  4 frames  fiona $150 -90 -60e 15e place-at  world-frozen state!  stalkers-stay state!  fiona hold
    1 20e 5e 0e 4e shot  4 frames  symbol-msg say  4 frames  shot-off
    fiona $150 -90 -60.3e 16.25e place-at  world-frozen state-off  stalkers-stay state-off ;
: the-statue ( -- )   \ (once the box is open: the Film Reel in the lamp; else its remark)
    fiona wait-move
    om1f-12-flag4 fact?  got-film-reel fact? 0= and if
        fiona $14E -130 -33.036e -2.992e walk-to  fiona pick-up-anim play  4 hide-part
        film-reel got-film-reel -1 takes-the  fiona up-from-pick-anim play  exit
    then
    world-frozen state!  fiona hold  fiona $14E 180 -38e -2.5e place-at  1 10e -10e 0e 0e shot  4 frames
    om1f-12-flag4 fact? if  trick-msg  else  statue-msg  then  say  shot-off  world-frozen state-off ;
: the-pillar ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    om1f-12-flag1 fact? 0= if  fiona 38e -10e turn-to-xz  fiona turn-round-anim play  flame-msg say  stalkers-stay state-off exit  then
    ice-msg yes? if
        15 screen:to-black-2 fade wait-fade  climb-sound room-bank fiona sound-by  fiona $10C 180 place-on-tri
        0 0 player-camera  camera-restart  2 prepare-exit  90 frames  2 map-page  15 screen:from-black fade wait-fade
    then  stalkers-stay state-off ;
: climbs-down ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    climb-down-msg yes? if
        15 screen:to-black-2 fade wait-fade  climb-sound room-bank fiona sound-by  fiona $141 0 place-on-tri
        1 1 player-camera  camera-restart  1 prepare-exit  90 frames  1 map-page  15 screen:from-black fade wait-fade
    then  stalkers-stay state-off ;
: the-dragon ( -- )
    fiona wait-move
    om1f-12-flag1 fact? if  solved-msg  else  om1f-12-flag3 fact? 0=  om1f-12-flag2 fact? 0= or if  dragon-msg  else  one-more-msg  then  then  say ;
: the-letter ( -- )
    fiona wait-move  fiona 12e -24e turn-to-xz  mouth-msg say
    fiona reach-out-anim play  4 frames  torn-letter got-torn-letter letter-fx takes-the  fiona reach-back-anim play ;
: the-ivy ( -- )  fiona wait-move  fiona -63e -10e turn-to-xz  fiona look-low-anim start-anim  40 frames  ivy-msg say  fiona wait-anim ;
: take-medallion ( -- )  rubrum-medallion om1f-12-rubrum-medallion medallion-fx 52.04e 5.68e picks-up ;
: medallion-sniffed ( -- )  om1f-12-rubrum-medallion-found om1f-12-rubrum-medallion medallion-fx  52.04e 1e 5.68e  hewie-finds ;
: tense ( -- )  0.32e 0e -28.08e hewie-barks-tense ;

\ ---- the replays (the extras) ----
: replay-high ( -- )
    fiona hold  room-sounds  begin  room-sounds-in? 0= while  yield  repeat
    0 shut  2 shut  5 show-part  -1 plate-right !  high-scene  replay-over ;
: replay-ice ( -- )  fiona hold  0 shut  1 shut  2 shut  ice-scene  replay-over ;

create entries  1 , 1 ,   1 , 1 ,   0 , 0 ,   \ (by exit 0..2: the camera)
create ahead  3 , 0 ,   4 , 1 ,   \ (area, exit loaded ahead)
create far-looks  -12.29e 0e -26.2e  24e 15e 15e look,   0.32e 0e -28.08e  25e 31e 31e look,   12.19e 0e -23.96e  24e 15e 15e look,
create calm-looks  38.13e 0e -9.78e  30e 30e 30e look,   -35.94e 0e -7.54e  16e 11e 11e look,
: hum ( -- )
    om1f-12-flag1 fact? if  exit  then
    sounds-on @ if  fire-sound 38e 5e -10e keep-sound
    else  room-sounds-in? if  fire-sound 38e 5e -10e room-sound  -1 sounds-on !  then  then ;
: each-frame ( -- )
    hum  0 exit-out  1 exit-out  2 2 leaves-by  ahead 2 ways-out  box-due
    om1f-12-flag1 fact? 0= if  fire-noticed 100 0.32e 0e -28.08e 25e 31e notices? if  ['] tense start drop  then  then
    far-looks 3 hewie-looks-unchased  calm-looks 2 hewie-looks-at
    om1f-12-rubrum-medallion-found fact? 0=  scrambling? and  hewie 52.04e 0e 5.68e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    1 locked-request?  fiona 0 before-door? and if  ['] the-door any-time offer  then
    fiona 5 in-area?  fiona 90 50 heading? and if
        om1f-12-flag4 fact?  got-film-reel fact? 0= and if  ['] the-statue calm-only  else  ['] the-statue unhurried  then  offer
    then
    fiona 6 50 faces-area? if  ['] the-pillar unhurried offer  then
    fiona 7 in-area?  fiona 0 50 heading? and if  ['] climbs-down unhurried offer  then
    got-torn-letter fact? 0=  om1f-12-flag2 fact? 0= and if
        fiona 12e 13e -24e 4e 4e touches? if  ['] the-letter calm-only offer  then
    else  fiona 8 in-area?  fiona 90 50 heading? and if  ['] the-dragon unhurried offer  then  then
    fiona 9 in-area?  fiona 90 50 heading? and if  ['] the-dragon unhurried offer  then
    fiona $F in-area?  fiona -45 60 heading? and if  ['] the-ivy unhurried offer  then
    om1f-12-rubrum-medallion-found fact?  om1f-12-rubrum-medallion fact? 0= and  fiona 52.04e 0e 5.68e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona  replay-act $F = if  ['] replay-high  else  ['] replay-ice  then  start drop  exit
    then
    entries 3 ways-in  came-by @ 2 = if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-12 place!
' items-used old-mansion-1f-12 items-here
