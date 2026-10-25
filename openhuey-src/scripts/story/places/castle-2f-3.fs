\ story/places/castle-2f-3.fs - the bedroom in Belli Castle's 2F ($21), where Fiona wakes in a
\ sheet: Daniella brings her clothes, she changes, the stalker comes, she hides under the bed;
\ at night she hears the dog; later Hewie learns to attack here. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms/castle-2f-3.fs (converted from the game's).
IN: story.places.castle-2f-3
USING: engine game-state actors messages common vectors facts flag-names story-names room-names
    story.kit fiona.anims ;

own-reset

\ ---- what is said here (the room's messages) ----
$C  constant sheets-msg          \ "I can't just walk around all day holding these sheets around me..."
$1F constant change-clothes-msg  \ "There is a change of clothes that maid has apparently laid out. Change clothes? YES NO"
$12 constant now-what-msg        \ "That worked... but now what should I do? If I could just find a map of this place..."
$29 constant tv-hope-msg         \ "I hope this TV works..."
$28 constant tv-broken-msg       \ "It won't turn on. Maybe it's broken?"
$C  constant dog-alright-msg     \ "I wonder if that dog will be alright?" (night)
$22 constant cant-sleep-msg      \ "I can't fall asleep. I am too worried about that dog."
$D  constant dog-night-msg       \ "If I just leave it like this, it may not make it through the night."
$23 constant fire-msg            \ "The fire burns, emitting a red glow. The room is heated to a comfortable temperature."
$24 constant portrait-1-msg      \ "I wonder whose portrait this is?"
$25 constant portrait-1-again    \ "This sounds really weird, but for some reason, I feel like I've seen this person..."
$2D constant portrait-2-msg      \ "The man pictured here has a noble air about him. I wonder how long ago this portrait..."
$2E constant portrait-2-again    \ "In this painting, the man looks quite young. There is no mistaking that all the p..."
$2B constant portrait-3-msg      \ "What a powerful piece of art. The eyes really feel like they are peering deep int..."
$2C constant portrait-3-again    \ "Every painting features the same person. Perhaps it's the lord of this castle?"
$26 constant sheets-pressed-msg  \ "Freshly starched and pressed sheets and bed covers now line the bed."
$27 constant boogieman-msg       \ "Oh come on, Fiona... It's not like you are going to find the Boogieman under there."
1   constant doll-msg            \ "There's an antique doll lying here."
2   constant doll-again          \ "They say that bisque dolls are supposedly designed with a specific model in mind..."
3   constant birdcage-msg        \ "A birdcage... Right now there is nothing in it."
4   constant scales-msg          \ "There is a pair of scales lying here. It looks like their purpose is more one of decora..."
$40 constant chest-msg           \ "A chest used to store clothes."
$41 constant chest-change-msg    \ "A chest used to store clothes. Change clothes? YES NO"
6   constant sunset-msg          \ "The sun is starting to set... And with it will come night..."
7   constant daylight-msg        \ "I should cover as much of this castle as I can while there is still some light left."
$31 constant pitch-black-msg     \ "The sun has completely set... It's pitch black out now. I can't see 10 feet out the w..."
$11 constant hide-now-msg        \ "I have to find a place to hide!"
$F  constant what-now-msg        \ "Oh no! What should I do?! If I don't do something, that... thing will get me!"
$8016 constant no-time-msg       \ "I have to get away! I don't have time for this!"
$8010 constant cant-carry-msg    \ "It's a . Unfortunately you can't carry anymore items."
$8011 constant obtained-msg      \ "You've obtained ."

\ ---- its own state ----
own: portrait-1-remarked   own: portrait-2-remarked   own: portrait-3-remarked
own: doll-remarked         own: sunset-remarked
own: sounds-on             \ its steady sounds started
own: ambient-next          \ which ambient sound next (0..3)
own: ambient-frames        \ frames to it (every 30)
own: whine-frames          \ frames to Hewie's next whine (caught in the wire)
own: reaction-made         \ her start at seeing the maid, made this visit

\ ---- its places (the room's areas, by what is there) ----
2 constant to-exit-0-area   3 constant to-exit-1-area
4 constant tv-area   5 constant bed-area   6 constant portrait-3-area   7 constant clock-area
$B constant fire-area   $C constant portrait-1-area   $D constant window-area   $16 constant window-2-area
$E constant maid-area   $10 constant clothes-area   $11 constant portrait-2-area   $12 constant doll-area
$13 constant birdcage-area   $14 constant scales-area   $15 constant chest-area
$3C constant hall-door   $3D constant west-door   \ (its doors)

\ ---- the room's things ----
0 constant tv-fx   1 constant sparkle-fx   2 constant window-1   3 constant window-2   \ (the place's effects)
0 constant tv-off-part   1 constant clothes-part   \ (its parts: the TV's dark screen, the clothes laid out)
0 constant tv-on-sound   1 constant tv-static-sound   2 constant tv-off-sound
5 constant first-ambient   9 constant room-loop-sound   15 constant whine-sound   16 constant wrong-sound

: tv-on? ( -- flag )  c2f-3-tv-examined fact? ;
: show-tv ( -- )   \ its screen lit and its part away while it's on
    tv-on? if  tv-off-part hide-part  1 tv-fx tell-effect  else  tv-off-part show-part  0 tv-fx tell-effect  then ;
: windows ( strength -- )   \ the two windows reflecting the room
    {: str :}
    window-1 str  0.491e 22.0e 69.502e   9.502e 22.0e 60.491e   0.491e 0.0e 69.502e   9.502e 0.0e 60.491e  window-in
    window-2 str  40.491e 30.0e -10.498e  49.502e 30.0e -19.509e  40.491e 1.5e -10.498e  49.502e 1.5e -19.509e  window-in ;
$40 constant window-strength
: daylight-tint ( -- )  $84 $60 $3C $50  $24 $2A $2A $46  room-tint ;   \ (the afternoon's light: until she has the castle sketch)

\ ---- entering ----
: came-in ( exit -- )
    drop  room-sounds
    0e 0e 0e 0 effects:tv-screen tv-fx effect!  show-tv  tv-on? if  1 s" acoustics" actor-named send noise-setting  then
    c2f-3-medallion-found fact?  c2f-3-medallion fact? 0= and if  -9e 1e -11e 0 effects:flicker sparkle-fx effect!  then
    found-file-202 fact? 0= if  west-door lock  then
    c2f-3-flag1 fact?  hewie-found-in-wire fact? 0= and if  west-door lock  then
    found-file-201 fact?  found-file-202 fact? 0= and  clothes-part swap part
    0 ambient-next !  0 ambient-frames !  30 whine-frames !
    got-castle-sketch fact? 0= if  daylight-tint  then
    window-strength windows ;

\ ---- each frame: what goes on by itself ----
: ambience ( -- )
    sounds-on @ 0= if
        room-sounds-in? 0= if  exit  then
        tv-on? if  tv-static-sound  0e 3e 7e  room-sound  then
        room-loop-sound  -42e 4e -63e  room-sound  -1 sounds-on !  exit
    then
    1 ambient-frames +!  ambient-frames @ 30 = if
        0 ambient-frames !  first-ambient ambient-next @ +  -47e 12e -28e  room-sound
        ambient-next @ 1+ 4 mod ambient-next !
    then
    tv-on? if  tv-static-sound  0e 3e 7e  keep-sound  then
    room-loop-sound  -42e 4e -63e  keep-sound ;
: fan ( -- )  s" fan" 0.0200713e object-turn ;   \ (1.15 degrees a frame, but while a movie plays)
: cameras ( -- )   \ (whoever walks into these: their camera)
    $17 0 0 area-camera   $18 1 1 area-camera   $19 0 0 area-camera   $1A 2 2 area-camera ;
: ways-out ( -- )
    0 exit-out  1 exit-out
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    fiona to-exit-1-area entered? if  1 prepare-exit  then ;
: doorway-light ( -- )  -2e 7.6e -47e  -2e 7.6e -67e  -2e 0e -47e  -2e 0e -67e  light-doorway ;

\ ---- her doings here (activities: she is held for each) ----
: at-spot ( tri face F: x z -- )  {: tri face F: x z :}  fiona wait-move  fiona tri face x z walk-to ;
: looking ( F: dist pitch yaw rise -- )  1 shot ;   \ (the story's shot of her, close)
: look-done ( -- )  shot-off ;

: remark-sheets ( -- )  fiona wait-move  sheets-msg say ;
: remark-fire ( -- )  fiona wait-move  fiona -40e -60e turn-to-xz  fire-msg say ;
: portrait ( msg tri face F: x z -- )   \ (looked at closely, a remark said, back)
    {: msg tri face F: x z :}  tri face x z at-spot
    20e -10e 0e 10e looking  fiona look-at-anim start-anim  40 frames
    msg say  fiona wait-anim  16 frames  look-done ;
: portrait-1 ( -- )
    c2f-3-portrait-1-examined fact!
    portrait-1-remarked @ if  portrait-1-again  else  -1 portrait-1-remarked !  portrait-1-msg  then
    $B5 90 43.571e -39.929e portrait ;
: portrait-2 ( -- )
    c2f-3-portrait-2-examined fact!
    portrait-2-remarked @ 0= if  -1 portrait-2-remarked !  portrait-2-msg
    else  c2f-3-portrait-1-examined fact? c2f-3-portrait-3-examined fact? or if  portrait-2-again  else  portrait-2-msg  then  then
    $9F 0 -37e 63e portrait ;
: portrait-3 ( -- )
    c2f-3-portrait-3-examined fact!
    portrait-3-remarked @ 0= if  -1 portrait-3-remarked !  portrait-3-msg
    else  c2f-3-portrait-1-examined fact? c2f-3-portrait-2-examined fact? and if  portrait-3-again  else  portrait-3-msg  then  then
    9 -90 -40.418e -39.428e portrait ;
: bed ( -- )   \ (a look under it, before the night)
    fiona wait-move  sheets-pressed-msg say
    fiona turn-round-anim play  fiona -1 idle  7 frames
    fiona crouch-down-anim play  fiona crouching-anim start-anim  16 frames
    fiona crouch-up-anim play  fiona -1 idle  boogieman-msg say ;
: close-look ( tri face F: x z dist pitch yaw rise -- )   \ (she is set there, the story's shot on her)
    {: tri face F: x z d p y r :}  fiona wait-move  4 frames
    fiona tri face x z place-at  d p y r looking  4 frames ;
: doll ( -- )
    $E8 -5 -25e 63e 20e 10e 40e 0e close-look
    doll-remarked @ if  doll-again  else  -1 doll-remarked !  doll-msg  then  say
    4 frames  look-done ;
: birdcage ( -- )  fiona wait-move  fiona 5e 40.5e turn-to-xz  birdcage-msg say ;
: scales ( -- )  $55 -90 -40e 12e 30e 20e -60e 0e close-look  scales-msg say  4 frames  look-done ;
: chest ( -- )  fiona wait-move  fiona 180 turn  chest-msg say ;
: window-day ( -- )
    fiona wait-move  fiona 45 turn
    sunset-remarked @ if  daylight-msg  else  -1 sunset-remarked !  sunset-msg  then  say ;
: window-night ( -- )  fiona wait-move  fiona 45 turn  pitch-black-msg say ;
: remark-dog ( -- )  fiona wait-move  dog-alright-msg say ;
: remark-sleep ( -- )  fiona wait-move  cant-sleep-msg say ;
: window-dog ( -- )
    $80 45 -1.546e 59.728e at-spot  fiona reach-out-anim play  dog-night-msg say  fiona reach-back-anim play ;
: remark-hide ( -- )  fiona wait-move  hide-now-msg say ;

\ the TV: on and off, once it works (the story's variable 5: the stalker hasn't broken it)
: tv-works? ( -- flag )  progress pr.vars 5 + c@ 2 < ;
: tv ( -- )
    $6A 160 -1e 13.5e at-spot  20e 10e 0e 0e looking
    c2f-3-tv-examined-2 fact? 0= if  c2f-3-tv-examined-2 fact!  tv-hope-msg say  then
    fiona reach-out-anim play
    tv-works? if
        tv-on? if
            c2f-3-tv-examined fact-off  tv-off-sound room-bank fiona sound-by  tv-static-sound room-bank stop-sound
            0 s" acoustics" actor-named send noise-setting
        else
            c2f-3-tv-examined fact!  tv-on-sound room-bank fiona sound-by  tv-static-sound 0e 3e 7e room-sound
            $40 this-room @ $6A -1 world-noise s" acoustics" actor-named send noise
            1 s" acoustics" actor-named send noise-setting
        then
        fiona reach-back-anim play
    else
        fiona reach-back-anim play  tv-broken-msg say
    then
    look-done ;

\ the medallion Hewie sniffed out, lying by the wall
$70 constant medallion-item
: medallion ( -- )
    fiona wait-move  fiona -9e -11e turn-to-xz
    in-panic? if  exit  then
    fiona pick-up-anim play  4 frames
    medallion-item item-named
    medallion-item item-full? if  cant-carry-msg say
    else
        c2f-3-medallion fact!  sparkle-fx effect-off  medallion-item 1 give-item
        medallion-item show-item-tab  fanfare  obtained-msg say
    then
    fiona up-from-pick-anim play ;

\ ---- what she may do here: offered by where she stands and which way she faces (the first
\ offered wins; in the sheet every thing only reminds her of it) ----
own: hiding-offered   \ the stalker coming: told to hide (her locked doors say so)
: change-clothes ( -- ) ;   : hide-under-bed ( -- ) ;   : chest-costumes ( -- ) ;   \ (below)
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;
: clothed? ( -- flag )  found-file-202 fact? ;
: or-sheets ( xt -- xt' )  clothed? 0= if  drop ['] remark-sheets  then ;   \ (in the sheet: that, not this)
: night? ( -- flag )  c2f-3-flag1 fact?  hewie-found-in-wire fact? 0= and ;   \ (the dog heard, not yet found)
: by-the-bed ( -- )
    bed-area 180 60 facing? 0= if  exit  then
    c2f-2-flag1 fact? 0= if  ['] bed unhurried offer exit  then
    stalker-in? if  no-time-msg offer-remark-now  else  ['] hide-under-bed calm-only offer  then ;
: spots ( -- )
    tv-area 180 90 facing? if  ['] tv or-sheets unhurried offer  then
    clothed? 0= if
        found-file-201 fact?  clothes-area 180 50 facing? and if  ['] change-clothes unhurried offer  then
        fiona at-locked-exit? if  ['] remark-sheets any-time offer  then
    else night? if
        fiona at-locked-exit? if  ['] remark-dog any-time offer  then
        bed-area 180 60 facing? if  ['] remark-sleep unhurried offer  then
        window-area 44 60 facing? if  ['] window-dog unhurried offer  then
    else  by-the-bed  then then
    fire-area -134 60 facing? if  ['] remark-fire unhurried offer  then
    portrait-1-area 90 60 facing? if  ['] portrait-1 or-sheets unhurried offer  then
    portrait-2-area 0 60 facing? if  ['] portrait-2 or-sheets unhurried offer  then
    portrait-3-area -90 60 facing? if  ['] portrait-3 or-sheets unhurried offer  then
    doll-area 0 50 facing? if  ['] doll or-sheets unhurried offer  then
    fiona birdcage-area 50 faces-area? if  ['] birdcage or-sheets unhurried offer  then
    scales-area -90 50 facing? if  ['] scales or-sheets unhurried offer  then
    chest-area 180 50 facing? if  fg-3-flag3 fact? if  ['] chest-costumes  else  ['] chest  then  or-sheets unhurried offer  then
    window-area 44 60 facing?  window-2-area 44 60 facing? or if
        c2f-3-flag1 fact? if  ['] window-night  else  ['] window-day  then  or-sheets unhurried offer
    then
    found-file-201 fact?  clock-area -90 50 facing? and if  ['] save-clock or-sheets calm-hewie-up offer  then
    hiding-offered @  fiona at-locked-exit? and if  ['] remark-hide any-time offer  then
    c2f-3-medallion-found fact?  c2f-3-medallion fact? 0= and  fiona -9e 1e -11e 5e 5e touches? and if
        ['] medallion calm-only offer
    then ;

: each-frame ( -- )
    ambience  fan  ways-out  cameras  show-tv  spots  doorway-light ;
: person-in ( i -- )  drop ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-3 place!
