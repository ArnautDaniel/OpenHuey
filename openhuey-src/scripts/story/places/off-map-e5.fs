\ story/places/off-map-e5.fs - the clock run's castle ($E5, castle-b1-2's washroom again): the day's
\ light in the run's second kind; the curtain as it hangs; out by exit 0; its cameras; the curtain
\ to hide behind - Fiona crawls in at Hewie's command as he stands by it (drawn as she goes), out
\ at his next (found or panicking: hauled out); the stalker coming in while she hides finds her by
\ the odds; left behind there, she's out in front of it; the clock. A place (story/kit.fs,
\ story/places/clock-run.fs). The original's scripts, for checking: story/rooms-ref/off-map-e5.fs
\ (converted from the game's, not loaded).
IN: story.places.off-map-e5
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    fiona.sounds story.places.clock-run ;

own-reset
own: crawl      \ her crawl's frames: the curtain goes by them
own: her-held   \ Fiona behind the curtain this frame (held by it: kept for the room's end)

$2E constant run-kind-var   0 constant her-motions   0 constant curtain-sound   $32 constant search-var

\ ---- the curtain: drawn by her crawl's frames as she goes in or out ----
: curtain ( a b F: t -- )  s" Cartain" morph ;
: drawn ( lo hi fwd F: div -- )   \ ((frames - lo) / div, kept to lo..hi: forward 1 then 0, or back; its sound as it starts)
    {: lo hi fwd F: div :}  crawl @ lo = if  curtain-sound room-bank fiona sound-by  then
    fwd  fwd 0= 1 and  crawl @ lo max hi min lo - s>f div f/  curtain ;
: curtain-back ( -- )  0 1 0e curtain ;    \ (as it hangs)
: curtain-drawn ( -- )  0 1 1e curtain ;   \ (drawn across, she behind it)
: drawing ( n xt -- )   \ (n frames of her crawl, the curtain drawn by xt each)
    {: n xt :}  0 crawl !  begin  xt execute  yield  crawl @ n < while  1 crawl +!  repeat ;
: came-in ( exit -- )
    drop  room-sounds  run-kind-var var@ 1 = if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    curtain-back  1 $3FFF reverb ;

\ ---- behind the curtain ----
: out-hauled ( -- )   \ (found, or panicking, or he's here)
    found-her @ if  2 avoid-prompt  then
    fiona-hidden state-off  fiona true noclip
    fiona room-motion 3 + 5 start-blend8  5 frames  18 [: $B $12 1 7e drawn ;] drawing  curtain-back
    fiona wait-anim  fiona false noclip
    found-her @ 0= if  stalker let-go  stalker-in? if  0 stalker-order  then  then ;
: out-slipping ( -- )
    fiona-hidden state-off  fiona true noclip
    fiona room-motion 1+ 0 start-blend8  21 [: $C $15 1 9e drawn ;] drawing  curtain-back
    fiona wait-anim  fiona false noclip ;
: behind-curtain ( -- )
    stalkers-stay state!  0 came-while-hidden !  fiona her-motions motions-for  fiona wait-move
    fiona $15 180 60.918e -14.76e walk-to  fiona true noclip  fiona use-motions  fiona -1 look-at
    fiona room-motion 5 start-blend8  5 frames  54 [: $2B $36 0 11e drawn ;] drawing
    fiona wait-anim  curtain-drawn  fiona false noclip
    stalkers-stay state-off  fiona-hidden state!  0 found-her !  0 avoid-prompt
    begin
        wants-out? if  stalker-in? if  stalker let-go  out-hauled  else  out-slipping  then  exit  then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound common-bank fiona sound-by  then  out-hauled exit  then
        hidden-calm  yield
    again ;

\ ---- the stalker coming in while she hides ----
: odds ( -- pct )  search-var var@ 2 4 within if  25  else  0  then ;
: finds-her ( -- )
    stalker hold  stalker wait-move  0 door-for-him  1 door-for-him
    stalker run-move $FA -1  $FA tri-center 0e  move  stalker wait-move
    $10 180 58.883e -8.841e run-finds-her ;
: stalker-came ( -- )  came-on-her? if  odds search-var ['] finds-her run-searches  then ;

\ ---- every frame ----
create cameras  5 , 0 , 0 ,  6 , 0 , 0 ,  7 , 2 , -1 ,  8 , 3 , -1 ,  9 , 1 , -1 ,   \ (area, set, path)
: each-frame ( -- )
    0 exit-out  4 0 0 camera-out  cameras 5 cameras-at
    true 62.08e -16.33e 10e hide-cue? if  ['] behind-curtain fiona start-holding drop  then
    fiona held? her-held !  clock-shown ;
: person-in ( i -- )
    dup stalker-p = if  drop stalker-came exit  then
    player person-of = if  0 0 0 way-in  then ;
: went ( -- )  her-held @ if  $15 0 62.35e -10.65e left-hidden  then  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e5 place!
