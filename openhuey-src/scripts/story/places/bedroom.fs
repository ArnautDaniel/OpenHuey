\ story/places/bedroom.fs - the bedroom in Belli Castle's 2F as both its rooms have it (castle-2f-3,
\ where she wakes, and castle-2f-5, the same room once the chapter has turned): the TV, the glass of
\ the window and the door, its sounds and its fan; the fire, the portraits, the doll, the birdcage, the
\ scales, the chest (her costumes in it, once she has them); the bed to hide under - Hewie under the
\ other one if he's with her - lying low till she gets out or is found; what Hewie sniffs out by the
\ wall and looks up at. The rooms' own cells follow these: `own-reset  bedroom-owns owns !`.
IN: story.places.bedroom
USING: engine game-state actors messages common vectors facts flag-names story-names room-names stalker-names
    story.kit fiona.anims fiona.sounds hewie.anims keys ;

own-reset

\ ---- its own state (a visit) ----
own: portrait-1-remarked   own: portrait-2-remarked   own: portrait-3-remarked   own: doll-remarked
own: sounds-on             \ its steady sounds started
own: hiding-step           \ how far the hiding is: Hewie goes by it (0 going in, 1 under, 2 out by herself)
own: hewie-along           \ he came to hide too
own: hewie-in-place        \ he's at the other bed, ready
owns @ constant bedroom-owns

\ ---- what is said here (the same in both) ----
$29 constant tv-hope-msg         \ "I hope this TV works..."
$28 constant tv-broken-msg       \ "It won't turn on. Maybe it's broken?"
$23 constant fire-msg            \ "The fire burns, emitting a red glow. The room is heated to a comfortable temperature."
$24 constant portrait-1-msg      \ "I wonder whose portrait this is?"
$25 constant portrait-1-again    \ "This sounds really weird, but for some reason, I feel like I've seen this person..."
$2D constant portrait-2-msg      \ "The man pictured here has a noble air about him. I wonder how long ago this portrait..."
$2E constant portrait-2-again    \ "In this painting, the man looks quite young. There is no mistaking that all the p..."
$2B constant portrait-3-msg      \ "What a powerful piece of art. The eyes really feel like they are peering deep int..."
$2C constant portrait-3-again    \ "Every painting features the same person. Perhaps it's the lord of this castle?"
1   constant doll-msg            \ "There's an antique doll lying here."
2   constant doll-again          \ "They say that bisque dolls are supposedly designed with a specific model in mind..."
3   constant birdcage-msg        \ "A birdcage... Right now there is nothing in it."
4   constant scales-msg          \ "There is a pair of scales lying here. It looks like their purpose is more one of decora..."
$40 constant chest-msg           \ "A chest used to store clothes."
$41 constant chest-change-msg    \ "A chest used to store clothes. Change clothes? YES NO"
$31 constant pitch-black-msg     \ "The sun has completely set... It's pitch black out now. I can't see 10 feet out the w..."
$8016 constant no-time-msg       \ "I have to get away! I don't have time for this!"

\ ---- its places (the room's areas, by what is there) ----
2 constant to-exit-0-area   3 constant to-exit-1-area
4 constant tv-area   5 constant bed-area   6 constant portrait-3-area   7 constant clock-area
$B constant fire-area   $C constant portrait-1-area   $D constant window-area   $16 constant window-2-area
$11 constant portrait-2-area   $12 constant doll-area   $13 constant birdcage-area   $14 constant scales-area
$15 constant chest-area

\ ---- the room's things ----
1 constant tv-fx   2 constant sparkle-fx   $1B constant window-fx   $1A constant door-fx   \ (its effects)
0 constant tv-off-part   \ (the TV's dark screen)
0 constant tv-on-sound   1 constant tv-static-sound   2 constant tv-off-sound
5 constant first-ambient   9 constant room-loop-sound   16 constant wrong-sound
create ahead  to-exit-0-area , 0 ,   to-exit-1-area , 1 ,   \ (area, exit loaded ahead)

: tv-on? ( -- flag )  c2f-3-tv-examined fact? ;
: show-tv ( -- )   \ its screen lit and its part away while it's on
    tv-on? if  tv-off-part hide-part  1 tv-fx tell-effect  else  tv-off-part show-part  0 tv-fx tell-effect  then ;
: tv-in ( -- )   \ (as she comes in: the screen, the static's noise while it's on)
    0e 0e 0e 0 effects:tv-screen tv-fx effect!  show-tv  tv-on? if  1 s" acoustics" actor-named send noise-setting  then ;
\ the glass of the window and of the door to the hall, reflecting the room
$40 constant glass-strength
: window-glass ( -- )
    window-fx glass-strength  0.491e 22e 69.502e  9.502e 22e 60.491e  0.491e 0e 69.502e  9.502e 0e 60.491e  window-in ;
: door-glass ( over-all? -- )   \ (a scene's: over the whole picture)
    door-fx swap glass-strength  40.491e 30e -10.498e  49.502e 30e -19.509e  40.491e 1.5e -10.498e  49.502e 1.5e -19.509e
    door-glass-in ;
: glass ( -- )  window-glass  false door-glass ;
: glass-off ( -- )  window-fx effect-off  door-fx effect-off ;   \ (not over a scene)
: medallion-sparkle ( -- )   \ (what Hewie sniffed out by the wall, still lying there)
    c2f-3-medallion-found fact?  c2f-3-medallion fact? 0= and if  sparkle-fx -9e 1e -11e flicker-at  then ;

\ ---- each frame: what goes on by itself ----
: ambience ( -- )
    sounds-on @ 0= if
        room-sounds-in? 0= if  exit  then
        tv-on? if  tv-static-sound  0e 3e 7e  room-sound  then
        room-loop-sound  -42e 4e -63e  room-sound  -1 sounds-on !  exit
    then
    first-ambient -47e 12e -28e ambient-sounds
    tv-on? if  tv-static-sound  0e 3e 7e  keep-sound  then
    room-loop-sound  -42e 4e -63e  keep-sound ;
: fan ( -- )  s" fan" 0.0200713e object-turn ;   \ (1.15 degrees a frame, but while a movie plays)

\ ---- her doings here (activities: she is held for each) ----
: at-spot ( tri face F: x z -- )  {: tri face F: x z :}  fiona wait-move  fiona tri face x z walk-to ;
: looking ( F: dist pitch yaw rise -- )  1 shot ;   \ (the story's shot of her, close)
: look-done ( -- )  shot-off ;
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;

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
: window-night ( -- )  fiona wait-move  fiona 45 turn  pitch-black-msg say ;

\ the TV: on and off, once it works (the story's variable 5: the stalker hasn't broken it)
: tv-works? ( -- flag )  5 var@ 2 < ;
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

\ the chest of clothes: her costumes (and his coats), once she has them (the front garden's $AE)
7 constant costume-menu
$2100 constant coat-pose-1   $1F00 constant coat-pose-2   $2000 constant coat-pose-3   \ ? his poses played as his coat changes
: hewie-recoated ( -- )   \ (his new coat posed: lying hurt, or as he stands)
    hewie-in? 0= if  exit  then
    hewie-cond-now @ down-cond = if  hewie.anims:down-anim hewie send hewie-play exit  then
    hewie.anims:sit-anim hewie send hewie-play  coat-pose-1 hewie send hewie-play
    coat-pose-2 hewie send hewie-play  coat-pose-3 hewie send hewie-play ;
: chest-costumes ( -- )
    stalkers-stay state!  fiona wait-move  fiona 180 turn  20e 10e 0e 0e looking
    chest-change-msg yes? if
        costume-menu open-subscreen  look-done
        costume-var costume-worn-var var-same? 0=  coat-var coat-worn-var var-same? 0= or if  wrong-sound room-bank fiona sound-by  then
        costume-var costume-worn-var var-same? 0= if  costume-var var@ dress  steps-in  fiona -1 idle  7 frames  then
        coat-var coat-worn-var var-same? 0= if  coat-var var@ coat  hewie-recoated  then
        15 screen:from-black with-music or fade wait-fade
    else  16 frames  look-done  then
    stalkers-stay state-off ;

\ ---- hiding under the bed: she crawls under (Hewie under the other one, if he's with her), lies
\ there calming while the stalker searches - out with the action button, or found and hauled out
\ (the room's motions, her file: 0 in with Hewie, 1 lying low, 2 out, 3 hauled out, 4 the walk to
\ it, 5 in alone - the kit's hiding place's; his file: 0 in, 1 lying low, 2 out) ----
8 constant hiding-motions   9 constant hewie-hiding-motions
3 constant under-bed-camera
: hewie-hides ( -- )   \ (Hewie under the other bed while she hides)
    hewie hold  hewie hewie-hiding-motions motions-for  hewie wait-move
    hewie run-move $88 -1  $88 18.5e -37.5e floor-at  180 deg>yaw  move  hewie wait-move
    hewie use-motions  -1 hewie-in-place !
    begin  hiding-step @ 1 < while  yield  repeat
    hewie true noclip  hewie 0 hid-motion 5 play-blend  hewie 1 hid-motion 10 start-blend  hewie false noclip  hewie-hidden state!
    begin  fiona held?  hiding-step @ 2 <> and while  yield  repeat
    hewie-hidden state-off  hewie true noclip  hewie 2 hid-motion 5 start-blend8  hewie wait-anim  hewie false noclip ;
: crawl-under ( -- )
    fiona hiding-motions motions-for  fiona wait-move
    hewie-along @ if
        fiona $27 180 14.5e -37.5e walk-to  fiona use-motions  fiona hewie look-at
        begin  hewie-in-place @ 0= while  yield  repeat
        1 hiding-step !  fiona true noclip  fiona -1 look-at
        fiona 0 hid-motion 5 start-blend8  28 frames  slide-sound room-bank fiona sound-by  fiona wait-anim
    else
        fiona use-motions
        fiona walk-anim-move 4 hid-motion 5  $27 14.5e -37.5e floor-at  180 deg>yaw  move  fiona wait-move
        1 hiding-step !  fiona true noclip  slide-sound room-bank fiona sound-by
        fiona 5 hid-motion 10 start-blend8  fiona wait-anim
    then
    fiona 1 hid-motion 10 start-blend  fiona false noclip
    stalkers-stay state-off  fiona-hidden state!  0 found-her !  0 avoid-prompt ;
: out-caught ( -- )  3 hiding-step !  hauled-out ;   \ (pulled out from under it: found, or the stalker here)
: lying-hidden ( out-xt -- )   \ (calming down, until she gets out - out-xt run first - or is found)
    {: out-xt :}
    begin
        circle button-pressed? if  out-xt execute  2 hiding-step !  stalker-in? if  out-caught  else  slips-out  then  exit  then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound room-bank fiona sound-by  then  out-caught exit  then
        hidden-calm  yield
    again ;
: hide-under-bed ( under-xt out-xt -- )   \ (under-xt run once she's under it)
    {: under-xt out-xt :}
    stalkers-stay state!  0 came-while-hidden !  0 hiding-step !  0 hewie-along !  0 hewie-in-place !
    hewie-in? if   \ (Hewie, free and near her: under the other bed)
        hewie held? 0=  hewie-near-now @ and  fiona body-at hewie body-at vec-dist 50e f<= and if
            ['] hewie-hides start drop  -1 hewie-along !
        then
    then
    crawl-under  under-xt execute  under-bed-camera -1 fixed-camera  out-xt lying-hidden ;
: stalker-to-bed ( -- )  stalker hold  stalker wait-move  0 door-for-him  1 door-for-him  stalker $1C 180 -0.663e -24.791e walk-to ;

\ ---- Hewie about the room: what he sniffs out by the wall, what he looks up at ----
: hewie-sniffs ( -- )   \ (scrambling about by the wall, it not found yet)
    c2f-3-medallion-found fact? if  exit  then
    scrambling? 0= if  exit  then
    hewie  -9e 0e -11e 10e 5e  touches? 0= if  exit  then
    [: c2f-3-medallion-found c2f-3-medallion sparkle-fx  -9e 1e -11e  hewie-finds ;] start-only ;
\ the things he looks up at as he passes (the original's zones 0-6: x y z, radius, height - their tops)
create looks
    -1.41e  -8e   8.99e  13e  11e  11e look,   \ the TV
   -46.87e   0e -38.5e   20e  22e  22e look,   \ the third portrait
   -39.23e   0e -59.47e  20e   6e   6e look,
   -37.71e  -8e  63.24e  15e  22e  22e look,
    46.35e   0e -40.12e  15e  22e  22e look,
    39.18e   0e -19.5e   16e  11e  11e look,   \ by the second window
    -0.22e  -8e  59.41e  16e  11e  11e look,   \ by the first window
: hewie-looks ( -- )  looks 7 hewie-looks-at ;
