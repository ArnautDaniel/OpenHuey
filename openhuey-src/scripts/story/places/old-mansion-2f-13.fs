\ story/places/old-mansion-2f-13.fs - the old mansion's 2F room of the curtained mirror: she draws
\ the curtain back from it (its glass lit) or across again; Daniella, coming in while it's bare,
\ smashes it in a rage (the floor flooded since); "The glass has been shattered"; brought in by the
\ forced way, the door behind her won't do ("Only a fool..."); the scene figures loaded by its ways;
\ the Rubrum Medallion Hewie sniffs out. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-2f-13.fs in the history (converted from the game's; gone
\ with the old machinery in 0.2.20).
IN: story.places.old-mansion-2f-13
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims fiona.sounds ;

own-reset

0 constant back-msg       \ "Only a fool would even think of going back now!!"
6 constant shattered-msg  \ "The glass has been shattered. This mirror is useless."
own: came-forward   \ in by the forced way: no going back (till the stalker's here)
own: drawing        \ her curtain under way
own: smashing       \ Daniella at the mirror
own: frames-in      \ the curtain's frames
0 constant medallion-fx   $1B constant glass-fx   0 constant her-motions   1 constant daniella-motions
0 constant struggle-sound   1 constant smash-sound   5 constant smash-start-sound   $E00 constant smash-anim
2 constant curtain-object   $E constant mirror-area
$1C constant scene-figure   $21 constant other-figure

\ ---- the mirror's glass, its curtain ----
: glass ( -- )  glass-fx false $80  120e 21e 47e  120e 21e 39e  120e 4e 47e  120e 4e 39e  glass-in ;
: curtain ( a b F: t -- )  s" Curtain" morph ;
: curtain-by ( lo hi a b F: div -- )   \ ((frames - lo) / div, the frames kept to lo..hi)
    {: lo hi a b F: div :}  a b  frames-in @ lo max hi min lo - s>f div f/  curtain ;
: drawn-back ( -- )  0 1 0e curtain ;   : drawn-across ( -- )  0 1 1e curtain ;
: mirror-shown ( -- )
    om2f-13-breakable-broken fact? if  0 hide-part  1 show-part  glass-fx effect-off  0 water-floor floor-on  exit  then
    0 show-part  1 hide-part
    om2f-13-flag1 fact? if  drawn-back  glass  else  drawn-across  glass-fx effect-off  then ;
: medallion-shows ( -- )
    om2f-13-rubrum-medallion-found fact?  om2f-13-rubrum-medallion fact? 0= and if  medallion-fx 88.5e 1e 71.56e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds  curtain-object object-show  mirror-shown  medallion-shows  1 $3FFF reverb ;

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
    fiona $F8 90 114.64e 43e walk-to  fiona use-motions  begin  room-sounds-in? 0= while  yield  repeat
    om2f-13-flag1 fact? 0= if
        glass  room-motion 30 12 30 1 0 18e pulls  drawn-back  om2f-13-flag1 fact!
    else
        room-motion 1+ 28 12 28 0 1 16e pulls  drawn-across  om2f-13-flag1 fact-off  glass-fx effect-off
    then  0 drawing ! ;
: the-shards ( -- )  fiona wait-move  fiona 90 turn  shattered-msg say ;

\ ---- Daniella in while the mirror's bare: she smashes it in a rage ----
: smashes ( -- )
    stalker hold  -1 smashing !  stalker daniella-motions motions-for  stalker wait-move
    stalker $F8 90 114.64e 43e walk-to  stalker use-motions  begin  room-sounds-in? 0= while  yield  repeat
    smash-start-sound room-bank stalker sound-by  stalker room-motion 5 play-blend  stalker smash-anim 5 start-blend
    22 frames  smash-sound room-bank stalker sound-by  om2f-13-breakable-broken fact!  true stalker send rage
    0 hide-part  1 show-part  0 water-floor floor-on  glass-fx effect-off
    $38383838 4 9  120e 16e 45e  0.4e 0e 0.2e 0.5e  -15.5e shards   $38383838 4 9  120e 16e 41e  0.4e 0e 0.2e 0.5e  -15.5e shards
    $38383838 4 9  120e 10e 45e  0.4e 0e 0.2e 0.5e  -9.5e shards    $38383838 4 9  120e 10e 41e  0.4e 0e 0.2e 0.5e  -9.5e shards
    $38383838 4 9  120e 4e 45e  0.4e 0e 0.2e 0.5e  -3.5e shards     $38383838 4 9  120e 4e 41e  0.4e 0e 0.2e 0.5e  -3.5e shards
    stalker wait-anim  0 smashing ! ;
: her-kind? ( -- flag )  partner-kind dup daniella-kind = swap kind34-kind = or ;
: smash-due ( -- )
    drawing @ smashing @ or  om2f-13-breakable-broken fact? or  om2f-13-flag1 fact? 0= or  her-kind? 0= or if  exit  then
    stalker mirror-area entered?  st-mode-now @ 0= and if  ['] smashes start drop  then ;

\ ---- brought in by the forced way ----
: forward ( -- )
    fiona hold  -1 came-forward !  15 fade-in  try-door-sound common-bank fiona sound-by  fiona wait-move  stalkers-stay state-off ;

: take-medallion ( -- )  rubrum-medallion om2f-13-rubrum-medallion medallion-fx 88.5e 71.56e picks-up ;
: medallion-sniffed ( -- )  om2f-13-rubrum-medallion-found om2f-13-rubrum-medallion medallion-fx  88.5e 1e 71.56e  hewie-finds ;

create cameras  $A , 2 , 2 ,   $B , 1 , 1 ,   $C , 0 , 0 ,   $D , 1 , 1 ,   \ (area, set, path)
create entries  1 , 1 ,   1 , 1 ,   2 , 2 ,   0 , 0 ,   \ (by exit 0..3: the camera)
: the-figures ( -- )   \ (the scene figures loaded, let go, by its ways)
    fiona 4 entered? if  2 prepare-exit  then
    fiona 5 entered?  fiona 6 entered? or if  0 prepare-exit  then
    fiona 7 entered?  fiona 8 entered? or if  1 prepare-exit  then
    fiona 6 entered? if  3 figure-out  then
    fiona 7 entered? if  scene-figure 3 1 figure-in-by  then
    om3f-1-flag2 fact? 0=  om1f-6-flag1 fact? 0= and if
        fiona 8 entered? if  4 figure-out  then
        fiona 9 entered? if  other-figure 4 3 figure-in-by  then
    then
    fiona 9 entered? if  3 prepare-exit  then ;
: each-frame ( -- )
    2 exits-out  2 2 leaves-by  3 3 leaves-by  cameras 4 cameras-at  the-figures  smash-due
    om2f-13-rubrum-medallion-found fact? 0=  scrambling? and  hewie 88.5e 0e 71.56e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    came-forward @  stalker-in? and if  0 came-forward !  then
    0 exit-open? 0=  came-forward @ and  fiona 0 before-door? and if  back-msg offer-remark-now  then
    smashing @ 0=  fiona mirror-area in-area? and  fiona 90 60 heading? and if
        om2f-13-breakable-broken fact? if  ['] the-shards unhurried offer
        else  stalker-in? 0= if  ['] the-curtain calm-only offer
        else  stalker mirror-area in-area?  fiona body-at stalker body-at vec-dist 40e f<= or if  no-time-message offer-remark-now
        else  ['] the-curtain calm-only offer  then  then  then
    then
    om2f-13-rubrum-medallion-found fact?  om2f-13-rubrum-medallion fact? 0= and  fiona 88.5e 1e 71.56e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 4 ways-in
    came-by @ forced-exit = if
        fiona 0 at-exit  1 1 player-camera  ['] forward start drop
        hewie body-room old-mansion-2f-6 = if  old-mansion-2f-13 55 0 hewie-to  then
    then ;
: went ( -- )  smashing @ if  om2f-13-flag1 fact!  om2f-13-breakable-broken fact!  her-kind? if  true stalker send rage  then  then ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-13 place!
