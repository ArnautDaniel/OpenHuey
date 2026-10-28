\ story/places/old-mansion-1f-11.fs - a room of the old mansion's 1F with a curtained mirror on its
\ upper floor: she draws the curtain back from it (its glass lit) or across again; Daniella, coming
\ to it while it's bare, smashes it in a rage (the floor flooded since); "The glass has been
\ shattered"; the way blocked by water ("I can see a door on the other side" after); once B1's floor
\ gave way, the collapsed floor; Hewie sitting down, refusing, by the bare mirror; the Album
\ Medallion he sniffs out; the scene figure kept by its far way; light rippling. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-11.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-11
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims ;

own-reset

0 constant collapsed-msg   \ "The floor has collapsed! But I literally just crossed through here seconds ago..."
1 constant water-msg       \ "The path is obstructed by water. I can't get through..."
2 constant far-door-msg    \ "I can see a door on the other side. Maybe that is the way out of this hellish maze..."
3 constant shattered-msg   \ "The glass has been shattered. This mirror is useless."
own: water-remarked  own: drawing  own: smashing  own: frames-in  own: mirror-noticed
0 constant medallion-fx   $1B constant glass-fx   0 constant her-motions   1 constant daniella-motions
0 constant struggle-sound   1 constant smash-sound   5 constant smash-start-sound   $E00 constant smash-anim
$13 constant mirror-area   $26 constant scene-figure   3 constant figure-slot

\ ---- the mirror's glass, its curtain ----
: glass ( -- )  glass-fx false $80  -44e 71e 60e  -36e 71e 60e  -44e 54e 60e  -36e 54e 60e  glass-in ;
: curtain ( a b F: t -- )  s" Curtain" morph ;
: curtain-by ( lo hi a b F: div -- )   \ ((frames - lo) / div, the frames kept to lo..hi)
    {: lo hi a b F: div :}  a b  frames-in @ lo max hi min lo - s>f div f/  curtain ;
: drawn-back ( -- )  0 1 0e curtain ;   : drawn-across ( -- )  0 1 1e curtain ;
: mirror-shown ( -- )
    om1f-11-breakable-broken fact? if  2 show-part  3 hide-part  glass-fx effect-off  4 water-floor floor-on  exit  then
    2 hide-part  3 show-part
    om1f-11-flag1 fact? if  drawn-back  glass  else  drawn-across  glass-fx effect-off  then ;
: floor-shown ( -- )   \ (B1's floor given way: this one too)
    omb1-5-flag3 fact? 0= if  0 show-part  0 barred-floor hole-floor or floor-on  exit  then
    1 show-part  1 barred-floor hole-floor or floor-on  2 both-halves floor-on
    hewie-in?  hewie 1 on-group?  hewie 2 on-group? or and if  hewie $D put-on-tri  then ;
: medallion-shows ( -- )
    om1f-11-album-medallion-found fact?  om1f-11-album-medallion fact? 0= and if  medallion-fx 13.96e 51e -5.34e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds  $11 1e background  floor-shown  mirror-shown  medallion-shows
    8 $80808080  -8.7654e -7.4e -0.0072e  120e 0.5e 0.1e  effects:caustic drop
    1 $2300 reverb ;

\ ---- her curtain: drawn back from the mirror, or across ----
: pulls ( motion last lo hi a b F: div -- )   \ (her motion, its frames lo..hi the curtain's)
    {: m last lo hi a b F: div :}
    fiona m 5 start-blend8  5 frames  0 frames-in !
    begin  lo hi a b div curtain-by  yield
        frames-in @ 12 = if  struggle-sound room-bank fiona sound-by  then
        frames-in @ last <  while  1 frames-in +!
    repeat  fiona wait-anim ;
: the-curtain ( -- )
    fiona hold  -1 drawing !  fiona her-motions motions-for  fiona wait-move
    fiona $56 0 -40e 54.64e walk-to  fiona use-motions  begin  room-sounds-in? 0= while  yield  repeat
    om1f-11-flag1 fact? 0= if
        glass  room-motion 30 12 30 1 0 18e pulls  drawn-back  om1f-11-flag1 fact!
    else
        room-motion 1+ 28 12 28 0 1 16e pulls  drawn-across  om1f-11-flag1 fact-off  glass-fx effect-off
    then  0 drawing ! ;
: the-shards ( -- )  fiona wait-move  fiona 0 turn  shattered-msg say ;

\ ---- Daniella at the bare mirror: she smashes it in a rage ----
: her-kind? ( -- flag )  partner-kind dup daniella-kind = swap kind34-kind = or ;
: shards-from ( F: x y z floor -- )  {: F: x y z fl :}  $20202020 4 7  x y z  0.4e 0e 0.2e 0.5e  fl shards ;
: smashes ( -- )
    stalker hold  -1 smashing !  stalker daniella-motions motions-for  stalker wait-move
    stalker $56 0 -40e 54.64e walk-to  stalker use-motions  begin  room-sounds-in? 0= while  yield  repeat
    smash-start-sound room-bank stalker sound-by  stalker room-motion 5 play-blend  stalker smash-anim 5 start-blend
    22 frames  om1f-11-breakable-broken fact!  smash-sound room-bank stalker sound-by  true stalker send rage
    2 show-part  3 hide-part  4 water-floor floor-on  glass-fx effect-off
    -37e 66e 60e -15.5e shards-from  -41e 66e 60e -15.5e shards-from
    -37e 60e 60e -9.5e shards-from   -41e 60e 60e -9.5e shards-from
    -37e 54e 60e -3.5e shards-from   -41e 54e 60e -3.5e shards-from
    stalker wait-anim  0 smashing ! ;
: smash-due ( -- )
    drawing @  om1f-11-breakable-broken fact? or  om1f-11-flag1 fact? 0= or  her-kind? 0= or if  exit  then
    stalker mirror-area entered?  st-mode-now @ 0= and if  ['] smashes start drop  then ;
: smash-given-up ( -- )   \ (her smash cut short: the mirror hers again)
    her-kind?  stalker held? and 0= if  0 smashing !  then ;

\ ---- what she may do here ----
: collapsed ( deg -- )  fiona wait-move  fiona swap turn  collapsed-msg say ;
: floor-ahead ( -- )  0 collapsed ;   : floor-behind ( -- )  180 collapsed ;
: the-water ( -- )
    water-remarked @ 0= if  fiona wait-move  fiona -90 turn  water-msg say  -1 water-remarked !  exit  then
    stalkers-stay state!  fiona hold  fiona wait-move  fiona -90 turn
    4 frames  4 4 player-camera  far-door-msg say  4 frames  1 1 player-camera  stalkers-stay state-off ;
: take-medallion ( -- )  album-medallion om1f-11-album-medallion medallion-fx 13.96e -5.34e picks-up ;
: medallion-sniffed ( -- )  om1f-11-album-medallion-found om1f-11-album-medallion medallion-fx  13.96e 51e -5.34e  hewie-finds ;
: refuses ( -- )  -39.93e 50e 59.59e hewie-sits-refusing ;

create cameras  8 , 0 , 0 ,   9 , 2 , 2 ,   $A , 2 , 2 ,   $B , 3 , 3 ,   \ (area, set, path)
create entries  2 , 2 ,   0 , 0 ,   1 , 1 ,   1 , 1 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   5 , 1 ,   \ (area, exit loaded ahead)
: the-figure ( -- )   \ (the scene figure kept by the far way - not after the White Mandragora)
    fiona 6 entered? if  2 prepare-exit  white-mandragora-used fact? 0= if  figure-slot figure-out  then  then
    fiona 7 entered? if  white-mandragora-used fact? 0= if  scene-figure figure-slot figure-in  then  3 prepare-exit  then ;
: by-the-mirror ( -- )
    smashing @ if  smash-given-up  exit  then
    fiona mirror-area in-area?  fiona 0 60 heading? and 0= if  exit  then
    om1f-11-breakable-broken fact? if  ['] the-shards unhurried offer  exit  then
    stalker-in? 0= if  ['] the-curtain calm-only offer  exit  then
    stalker mirror-area in-area?  fiona body-at stalker body-at vec-dist 40e f<= or if  no-time-message offer-remark-now  exit  then
    ['] the-curtain calm-only offer ;
: each-frame ( -- )
    4 exits-out  cameras 4 cameras-at  ahead 2 ways-out  the-figure
    om1f-11-breakable-broken fact? 0=  om1f-11-flag1 fact? and if
        smash-due
        mirror-noticed 50 -39.93e 50e 59.59e 12e 6e notices? if  ['] refuses start drop  then
    then
    om1f-11-album-medallion-found fact? 0=  scrambling? and  hewie 13.96e 50e -5.34e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    omb1-5-flag3 fact? if
        fiona $10 in-area?  fiona 0 50 heading? and if  ['] floor-ahead unhurried offer  then
        fiona $11 in-area?  fiona 90 50 heading? and if  ['] floor-behind unhurried offer  then
    else
        fiona $12 in-area?  fiona -45 50 heading? and if  ['] the-water unhurried offer  then
    then
    by-the-mirror
    om1f-11-album-medallion-found fact?  om1f-11-album-medallion fact? 0= and  fiona 13.96e 50e -5.34e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 4 ways-in  came-by @ 2 < if  2 map-page  then ;
: went ( -- )
    smashing @ if  om1f-11-flag1 fact!  om1f-11-breakable-broken fact!  her-kind? if  true stalker send rage  then  then
    hewie-in?  hewie-cond-now @ down-cond = and  hewie 1 on-group? and if  1 hewie send heal  well-cond hewie send set-cond  then ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-11 place!
