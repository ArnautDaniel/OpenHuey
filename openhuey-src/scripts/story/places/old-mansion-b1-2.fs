\ story/places/old-mansion-b1-2.fs - the long hall of the old mansion's B1: the curtained mirror
\ (she draws the curtain back from it, or across; Daniella, coming to it while it's bare, smashes it
\ in a rage - the floor flooded since; "The glass has been shattered"); the plaque on the two paths;
\ two doors locked with odd symbols (the Jupiter Key turns in either; spent once both are open); the
\ door unlocked from this side; the book ("Glowing Sun..." - read twice, the file); two dolls; two
\ vases (the Viola and Rubrum Medallions in them); the Refined Magnesia; the story's spirit about;
\ four candles; Hewie wary, sitting down by the bare mirror, his looks; the map's pages; the
\ doorways' shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-b1-2.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-b1-2
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims ;

own-reset

0 constant paths-msg     \ "The right path leads to the truth, whilst the left path leads to deception. When the sh..."
1 constant locked-msg    \ "The door is locked..."
2 constant symbol-msg    \ "A strange symbol is written on the door. I wonder what it could mean?"
3 constant shattered-msg \ "The glass has been shattered. This mirror is useless."
4 constant doll-msg      \ "There is a doll here as well. It feels like they are watching me or something... It's r..."
5 constant title-msg     \ "'Glowing Sun, Radiant Star, Luminescent Moonlight'"
6 constant words-msg     \ "Glowing SUN, radiant STAR, MOONLIT sky... The words from the title are highlighted in red."
own: locked-remarked  own: locked-2-remarked  own: drawing  own: smashing  own: frames-in  own: reads
own: mirror-noticed  own: noticed-4  own: noticed-5  own: noticed-6
0 constant viola-fx   1 constant rubrum-fx   2 constant magnesia-fx   $1B constant glass-fx
0 constant struggle-sound   1 constant smash-sound   2 constant key-sound   5 constant smash-start-sound
0 constant her-motions   1 constant daniella-motions   $E00 constant smash-anim   $1A constant mirror-area   $609 constant key-turn-anim
$51 constant left-door   $52 constant right-door   1 constant left-key   3 constant right-key   \ (the items' events here: items.fs)
omb1-2-breakable1-broken 0 1  13.5e -10e -75e 5e 8e  breakable: viola-vase
viola-vase $80202020 0 4 shard-look  viola-vase 0.5e 0.5e 0.25e 0.5e shard-uv  viola-vase 0 frees-floor  viola-vase $23A noise-from
omb1-2-breakable2-broken 2 3  13.5e -10e -59.5e 5e 8e  breakable: rubrum-vase
rubrum-vase $80101010 0 4 shard-look  rubrum-vase 0.5e 0.5e 0.25e 0.5e shard-uv  rubrum-vase 1 frees-floor  rubrum-vase $234 noise-from

\ ---- the mirror's glass, its curtain ----
: glass ( -- )  glass-fx false $80  -63.65e 21e 104.5e  -55.65e 21e 104.5e  -63.65e 4e 104.5e  -55.65e 4e 104.5e  glass-in ;
: curtain ( a b F: t -- )  s" Curtain" morph ;
: curtain-by ( lo hi a b F: div -- )   \ ((frames - lo) / div, the frames kept to lo..hi)
    {: lo hi a b F: div :}  a b  frames-in @ lo max hi min lo - s>f div f/  curtain ;
: drawn-back ( -- )  0 1 0e curtain ;   : drawn-across ( -- )  0 1 1e curtain ;
: mirror-shown ( -- )
    omb1-2-breakable3-broken fact? if  4 show-part  5 hide-part  glass-fx effect-off  2 water-floor floor-on  exit  then
    4 hide-part  5 show-part
    omb1-2-flag1 fact? if  drawn-back  glass  else  drawn-across  glass-fx effect-off  then ;
: pulls ( motion last lo hi a b F: div -- )   \ (her motion, its frames lo..hi the curtain's)
    {: m last lo hi a b F: div :}
    fiona m 5 start-blend8  5 frames  0 frames-in !
    begin  lo hi a b div curtain-by  yield
        frames-in @ 12 = if  struggle-sound room-bank fiona sound-by  then
        frames-in @ last <  while  1 frames-in +!
    repeat  fiona wait-anim ;
: the-curtain ( -- )
    fiona hold  -1 drawing !  fiona her-motions motions-for  fiona wait-move
    fiona $21D 0 -59.65e 99.14e walk-to  fiona use-motions  begin  room-sounds-in? 0= while  yield  repeat
    omb1-2-flag1 fact? 0= if
        glass  room-motion 30 12 30 1 0 18e pulls  drawn-back  omb1-2-flag1 fact!
    else
        room-motion 1+ 28 12 28 0 1 16e pulls  drawn-across  omb1-2-flag1 fact-off  glass-fx effect-off
    then  0 drawing ! ;
: the-shards ( -- )  fiona wait-move  fiona 0 turn  shattered-msg say ;

\ ---- Daniella at the bare mirror: she smashes it in a rage ----
: her-kind? ( -- flag )  partner-kind dup daniella-kind = swap kind34-kind = or ;
: shards-from ( F: x y z floor -- )  {: F: x y z fl :}  $18181818 4 $B  x y z  0.4e 0e 0.2e 0.5e  fl shards ;
: smashes ( -- )
    stalker hold  -1 smashing !  stalker daniella-motions motions-for  stalker wait-move
    stalker $21D 0 -59.65e 99.14e walk-to  stalker use-motions  begin  room-sounds-in? 0= while  yield  repeat
    smash-start-sound room-bank stalker sound-by  stalker room-motion 5 play-blend  stalker smash-anim 5 start-blend
    22 frames  omb1-2-breakable3-broken fact!  smash-sound room-bank stalker sound-by  true stalker send rage
    4 show-part  5 hide-part  2 water-floor floor-on  glass-fx effect-off
    -57e 16e 104.5e -15.5e shards-from  -61e 16e 104.5e -15.5e shards-from
    -57e 10e 104.5e -9.5e shards-from   -61e 10e 104.5e -9.5e shards-from
    -57e 4e 104.5e -3.5e shards-from    -61e 4e 104.5e -3.5e shards-from
    stalker wait-anim  0 smashing ! ;
: smash-due ( -- )
    drawing @  omb1-2-breakable3-broken fact? or  omb1-2-flag1 fact? 0= or  her-kind? 0= or if  exit  then
    stalker mirror-area entered?  st-mode-now @ 0= and if  ['] smashes start drop  then ;
: smash-given-up ( -- )  her-kind?  stalker held? and 0= if  0 smashing !  then ;

\ ---- as she comes in ----
: viola-shows ( -- )  omb1-2-viola-medallion fact? 0= if  viola-fx 13.5e -9e -75e flicker-at  then ;
: rubrum-shows ( -- )  omb1-2-rubrum-medallion fact? 0= if  rubrum-fx 13.5e -9e -59.5e flicker-at  then ;
create flames  132.4e f, -16.6e f, -76.8e f,   85.7e f, -5.3e f, -16.8e f,   85.7e f, -5.3e f, 16.8e f,   132.4e f, -16.6e f, 76.8e f,
: came-in ( exit -- )
    drop  room-sounds
    om1f-12-flag4 fact? 0=  story-spirit creature-up? 0= and if  old-mansion-b1-2 0 422 2 8 story-spirit-in  then
    viola-vase as-it-stands  viola-vase broken? if  viola-shows  then
    rubrum-vase as-it-stands  rubrum-vase broken? if  rubrum-shows  then
    mirror-shown
    omb1-2-refined-magnesia fact? 0= if  magnesia-fx -109.83e 12e 72.62e flicker-at  then
    4 0 do  i 3 +  1  flames i 3 * floats +  dup f@  dup float+ f@  2 floats + f@  flame-at  loop
    1 $2300 reverb ;

\ ---- the doors with symbols: locked, then the symbol; the Jupiter Key (spent once both are open) ----
: symbol-door ( exit addr tri deg F: x z x' z' -- )   \ (locked - or, said once, its symbol)
    {: ex rem tri deg F: x z x2 z2 :}
    rem @ 0=  chased? or  in-panic? or if  ex false locked-msg tries-locked  -1 rem !  exit  then
    fiona ex to-door  4 frames  fiona tri deg x z place-at  world-frozen state!  1 20e 5e 0e 3e shot  4 frames
    symbol-msg say  4 frames  shot-off  fiona tri deg x2 z2 place-at  world-frozen state-off ;
: left-symbol ( -- )  2 locked-remarked $8F 180 145e -75e 143.75e -75.3e symbol-door ;
: right-symbol ( -- )  3 locked-2-remarked $29 0 145e 75e 146.25e 75.3e symbol-door ;
: key-in ( exit door other -- )
    {: ex door other :}  fiona hold  fiona ex to-door  fiona key-turn-anim start-anim  20 frames
    key-sound room-bank fiona sound-by  jupiter-key item-named  other doors:door-locked? 0= if  jupiter-key spend  then
    door unlock  fiona 20 idle  20 frames  used-msg say  stalkers-stay state-off ;
: left-key-in ( -- )  2 left-door right-door key-in ;
: right-key-in ( -- )  3 right-door left-door key-in ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    case  left-key of  ['] left-key-in  endof  right-key of  ['] right-key-in  endof  >r false r>  endcase
    dup if  start drop  true  then ;
: unlocks-it ( -- )  0 unlocks ;

\ ---- what she may do or say here ----
: the-plaque ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona 90e 0e turn-to-xz
    15 screen:to-dim fade wait-fade  paths-msg say  15 screen:from-dim fade wait-fade  stalkers-stay state-off ;
: a-doll ( F: x z -- )  fiona wait-move  fiona turn-to-xz  doll-msg say ;
: near-doll ( -- )  -29.58e 75.55e a-doll ;   : far-doll ( -- )  -133.09e 71.35e a-doll ;
: the-book ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  4 frames
    fiona $F4 -90 -53e -16e place-at  world-frozen state!  1 12e 0e 0e 5e shot  title-msg say
    world-frozen state-off  shot-off  4 frames
    fiona hand-to-mouth-anim 10 start-blend  words-msg say  fiona wait-anim
    1 reads +!  reads @ 2 = if  $236 give-file  then  stalkers-stay state-off ;
: take-magnesia ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -109.83e 72.62e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  refined-magnesia omb1-2-refined-magnesia magnesia-fx takes-up  fiona reach-back-anim play ;
: take-viola ( -- )  viola-medallion omb1-2-viola-medallion viola-fx 13.5e -75e picks-up ;
: take-rubrum ( -- )  rubrum-medallion omb1-2-rubrum-medallion rubrum-fx 13.5e -59.5e picks-up ;
: refuses ( -- )  -60.85e 0e 104.12e hewie-sits-refusing ;
: wary-4 ( -- )  -101.62e 0e 100.21e hewie-stands-by ;   : wary-5 ( -- )  -132.75e 0e 71.15e hewie-stands-by ;
: wary-6 ( -- )  -29.87e 0e 75.83e hewie-stands-by ;

create cameras  $D , 0 , -1 ,   $E , 1 , 0 ,   $F , 1 , 0 ,   $10 , 2 , 1 ,   $11 , 1 , 0 ,   $12 , 3 , -1 ,
    $13 , 3 , -1 ,   $14 , 4 , 2 ,   $15 , 3 , -1 ,   $16 , 5 , 3 ,   $17 , 1 , 0 ,   $18 , 6 , 4 ,   \ (area, set, path)
create entries  0 , -1 ,   2 , 1 ,   5 , 3 ,   4 , 2 ,   1 , 0 ,   6 , 4 ,   \ (by exit 0..5: the camera)
create ahead  6 , 0 ,   7 , 1 ,   8 , 2 ,   9 , 3 ,   $A , 4 ,   $B , 4 ,   $C , 5 ,   \ (area, exit loaded ahead)
create calm-look  85.21e -24e 1.06e  20e 19e 19e look,
create far-looks  -101.62e 0e 100.21e  20e 6e 6e look,   -132.75e 0e 71.15e  20e 12e 12e look,   -29.87e 0e 75.83e  20e 12e 12e look,
: hewie-about ( -- )
    calm-look 1 hewie-looks-at  far-looks 3 hewie-looks-unchased
    omb1-2-breakable3-broken fact? 0=  omb1-2-flag1 fact? and if
        mirror-noticed 50 -60.85e 0e 104.12e 16e 6e notices? if  ['] refuses start drop  then
    then
    noticed-4 30 -101.62e 0e 100.21e 20e 6e notices? if  ['] wary-4 start drop  then
    noticed-5 30 -132.75e 0e 71.15e 20e 12e notices? if  ['] wary-5 start drop  then
    noticed-6 30 -29.87e 0e 75.83e 20e 12e notices? if  ['] wary-6 start drop  then ;
: by-the-mirror ( -- )
    smashing @ if  smash-given-up  exit  then
    fiona mirror-area in-area?  fiona 0 60 heading? and 0= if  exit  then
    omb1-2-breakable3-broken fact? if  ['] the-shards unhurried offer  exit  then
    stalker-in? 0= if  ['] the-curtain calm-only offer  exit  then
    stalker mirror-area in-area?  fiona body-at stalker body-at vec-dist 40e f<= or if  no-time-message offer-remark-now  exit  then
    ['] the-curtain calm-only offer ;
: locked-doors ( -- )
    6 locked-request? 0= if  exit  then
    fiona 0 before-door? if  ['] unlocks-it any-time offer  then
    fiona 2 before-door? if  ['] left-symbol any-time offer  then
    fiona 3 before-door? if  ['] right-symbol any-time offer  then ;
: each-frame ( -- )
    6 exits-out  cameras $C cameras-at  ahead 7 ways-out
    fiona $12 entered? if  0 map-page  then  fiona $12 left? if  1 map-page  then
    drawing @ 0= if  smash-due  then  hewie-about
    locked-doors
    fiona $19 in-area?  fiona 45 50 heading? and if  ['] the-plaque unhurried offer  then
    by-the-mirror
    omb1-2-refined-magnesia fact? 0=  fiona -109.83e 12e 72.62e 5e 5e touches? and if  ['] take-magnesia calm-only offer  then
    fiona -29.58e 0e 75.55e 7e 10e touches?  fiona 60 -30e 76e faces-point? and if  ['] near-doll unhurried offer  then
    fiona -133.09e 0e 71.35e 7e 10e touches?  fiona 60 -133e 71e faces-point? and if  ['] far-doll unhurried offer  then
    fiona $1D in-area?  fiona -45 50 heading? and if  ['] the-book unhurried offer  then
    viola-vase broken? if
        omb1-2-viola-medallion fact? 0=  fiona 13.5e -10e -75e 5e 5e touches? and if  ['] take-viola calm-only offer  then
    else  viola-vase breaks? if  viola-shows  then  then
    rubrum-vase broken? if
        omb1-2-rubrum-medallion fact? 0=  fiona 13.5e -10e -59.5e 5e 5e touches? and if  ['] take-rubrum calm-only offer  then
    else  rubrum-vase breaks? if  rubrum-shows  then  then
    58e 5e 19.5e  58e 5e 15e  58e -24e 19.5e  58e -24e 15e  light-doorway
    58e 5e -15e  58e 5e -19.5e  58e -24e -15e  58e -24e -19.5e  light-doorway ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 6 ways-in  came-by @ dup 0 =  over 1 = or  over 4 = or  swap 5 = or if  1 map-page  then ;
: went ( -- )  smashing @ if  omb1-2-flag1 fact!  omb1-2-breakable3-broken fact!  her-kind? if  true stalker send rage  then  then ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-2 place!
' items-used old-mansion-b1-2 items-here
