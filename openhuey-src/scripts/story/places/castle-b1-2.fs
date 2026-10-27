\ story/places/castle-b1-2.fs - the washroom in Belli Castle's B1 ($14): the shower behind its
\ curtain to hide in (the curtain drawn as she slips in and out; found - hauled out, or Debilitas
\ dragging her out); the water that calms her (a recovery point, its document the first time), the
\ toilets, the door locked with its grimy knob (the Smudged Key opens it), the Chamomile, the
\ Medallion Hewie sniffs out. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-b1-2.fs (converted from the game's, not loaded).
IN: story.places.castle-b1-2
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    items.names story.kit fiona.anims fiona.sounds keys ;

own-reset

\ ---- what is said here ----
0 constant toilets-msg     \ "The toilets in the castle are... communal? Guess I'll try holding it for now."
1 constant water-msg       \ "Wonder if this water is drinkable?"
2 constant refreshed-msg   \ "Ah, that was quite refreshing."
3 constant locked-msg      \ "The door is locked..."
4 constant handprints-msg  \ "There are lots of grimy handprints littering the knob."

\ ---- its own state (a visit) ----
own: rested   own: locked-remarked
own: dragged       \ Debilitas has found her in it: he drags her out
own: drag-step     \ his drag and her motion in step (1 he's at it, 2 done)
own: crawl         \ her crawl's frames: the curtain goes by them

\ ---- the room's things ----
0 constant camomile-fx   1 constant found-fx   0 constant her-motions   1 constant his-motions
0 constant curtain-sound   1 constant calm-sound   2 constant key-sound
1 constant locked-exit   $A constant smudged-door   $608 constant key-turn-anim
$A constant toilets-area   $B constant water-area   $C constant shower-area
$F constant search-var
: daylight ( -- )  c2f-3-flag1 fact? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;   \ (the day's light)

\ ---- the curtain: drawn by her crawl's frames as she goes in or out ----
: curtain ( a b F: t -- )  s" Cartain" morph ;
: drawn ( lo hi fwd F: div -- )   \ ((frames - lo) / div, the frames kept to lo..hi: forward 1 then 0, or back; its sound as it starts)
    {: lo hi fwd F: div :}  crawl @ lo = if  curtain-sound room-bank fiona sound-by  then
    fwd  fwd 0= 1 and  crawl @ lo max hi min lo - s>f div f/  curtain ;
: curtain-back ( -- )  0 1 0e curtain ;    \ (as it hangs)
: curtain-drawn ( -- )  0 1 1e curtain ;   \ (drawn across, she behind it)
: drawing ( n xt -- )   \ (n frames of her crawl, the curtain drawn by xt each)
    {: n xt :}  0 crawl !  begin  xt execute  yield  crawl @ n < while  1 crawl +!  repeat ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  daylight
    cb1-2-camomile fact? 0= if  camomile-fx 78e 6.5e 20e flicker-at  then
    cb1-2-medallion-found fact?  cb1-2-medallion fact? 0= and if  found-fx -44.9e -7e -12.9e flicker-at  then
    curtain-back  1 $3FFF reverb ;

\ ---- hiding behind the curtain: in, lying low and calming; out by the button; found - hauled out,
\ or Debilitas dragging her out ----
: out-hauled ( -- )
    found-her @ if  2 avoid-prompt  then
    fiona follow  fiona-hidden state-off  fiona true noclip
    fiona room-motion 3 + 5 start-blend8  5 frames  18 [: $B $12 1 7e drawn ;] drawing  curtain-back
    fiona wait-anim  fiona false noclip
    found-her @ 0= if  stalker let-go  stalker-in? if  0 stalker-order  then  then ;
: out-dragged ( -- )
    2 avoid-prompt  fiona follow  fiona-hidden state-off  fiona true noclip
    dragged @ if  1 drag-step +!  then
    fiona room-motion 2 + 5 start-blend8  5 frames  28 [: $10 $1C 1 12e drawn ;] drawing  curtain-back
    in-panic? 0= if  seized-sound room-bank fiona sound-by  then
    fiona wait-anim  fiona false noclip
    dragged @ if  begin  drag-step @ 2 < while  yield  repeat  then ;
: out-slipping ( -- )
    fiona follow  fiona-hidden state-off  fiona true noclip
    fiona room-motion 1+ 0 start-blend8  21 [: $C $15 1 9e drawn ;] drawing  curtain-back
    fiona wait-anim  fiona false noclip ;
: behind-curtain ( -- )
    stalkers-stay state!  0 came-while-hidden !  fiona her-motions motions-for  fiona wait-move
    fiona true noclip  fiona $15 180 60.918e -14.76e walk-to  fiona use-motions  fiona -1 look-at
    fiona room-motion 5 start-blend8  5 frames  54 [: $2B $36 0 11e drawn ;] drawing
    fiona wait-anim  curtain-drawn  fiona false noclip
    stalkers-stay state-off  fiona-hidden state!  0 found-her !  0 dragged !  0 drag-step !  0 avoid-prompt
    begin
        dragged @ if  out-dragged exit  then
        circle button-pressed? if
            stalker-in? if  stalker let-go  out-hauled  else  out-slipping  then  exit
        then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound room-bank fiona sound-by  then  out-hauled exit  then
        hidden-calm  yield
    again ;

\ ---- the stalker coming in while she hides: he searches - Debilitas drags her out, the others see her ----
: search-odds ( -- pct )  search-var var@ 2 4 within if  25  else  0  then ;
: finds-her ( -- )
    stalker hold  stalker wait-move  0 door-for-him  1 door-for-him
    stalker run-move $FA -1  $FA tri-center 0e  move  stalker wait-move
    stalker $10 180 58.883e -8.841e walk-to
    debilitas? if
        -1 dragged !  yield  stalker use-motions
        begin  drag-step @ 1 < while  yield  repeat
        stalker room-motion 5 start-blend8  stalker wait-anim  1 drag-step +!
    else  stalker stalker.anims:there-anim play  -1 found-her !  then
    stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    debilitas? if  stalker his-motions motions-for  then  stalker follow
    search-odds chance?  2 creature-moving? or  4 stalker-alert? 0= and if  ['] finds-her start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;

\ ---- what she may do or say here ----
: toilets ( -- )  fiona wait-move  toilets-msg say ;
: the-water ( -- )   \ (a recovery point: once a visit it calms her)
    rested @ if  fiona wait-move  refreshed-msg say exit  then
    -1 rested !  water-msg her-motions calm-sound $89 180 -9.659e 4.88e recovery-point ;
: tries-door ( -- )
    locked-remarked @ 0=  chased? or  in-panic? or if  locked-exit true locked-msg tries-locked  -1 locked-remarked !  exit  then
    fiona wait-move  fiona $95 -105 -58.5e 9.5e walk-to
    fiona look-low-anim start-anim  30 frames  handprints-msg say  fiona wait-anim  fiona -1 idle  7 frames ;
: key-used ( -- )  locked-exit smudged-door smudged-key key-turn-anim key-sound room-bank key-turns ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    4 = if  ['] key-used start drop  true  else  false  then ;
: take-camomile ( -- )   \ (reached for on the shelf, not in a panic)
    fiona wait-move  fiona 78e 20e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  camomile cb1-2-camomile camomile-fx takes-up  fiona reach-back-anim play ;
: found-medallion ( -- )  medallion cb1-2-medallion found-fx -44.9e -12.9e picks-up ;
: spots ( -- )
    fiona toilets-area in-area?  fiona 180 45 heading? and if  ['] toilets unhurried offer  then
    fiona water-area in-area?  fiona 180 45 heading? and if  ['] the-water unhurried offer  then
    2 locked-request? if  ['] tries-door any-time offer  then
    fiona shower-area in-area?  fiona 180 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] behind-curtain calm-only offer  then
    then
    cb1-2-camomile fact? 0=  fiona 78e 6.5e 20e 5e 5e touches? and if  ['] take-camomile calm-only offer  then
    cb1-2-medallion-found fact?  cb1-2-medallion fact? 0= and  fiona -44.9e -7e -12.9e 5e 5e touches? and if
        ['] found-medallion calm-only offer
    then ;

\ ---- Hewie scrambling about: the Medallion he sniffs out ----
: medallion-sniffed ( -- )  cb1-2-medallion-found cb1-2-medallion found-fx  -44.9e -7e -12.9e  hewie-finds ;
: hewie-sniffs ( -- )
    cb1-2-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -44.9e -8e -12.9e 10e 5e touches? if  ['] medallion-sniffed start-only  then ;

\ ---- every frame ----
create cameras  5 , 0 , 0 ,   6 , 0 , 0 ,   7 , 2 , -1 ,   8 , 3 , -1 ,   9 , 1 , -1 ,   \ (area, set, path)
create entries  0 , 0 ,   2 , -1 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    2 exits-out  4 0 0 camera-out  cameras 5 cameras-at
    fiona 2 left? if  0 prepare-exit  then   fiona 3 entered? if  1 prepare-exit  then
    hewie-sniffs  spots ;
: person-in ( i -- )
    case  fiona-p of  entries 2 ways-in  endof  stalker-p of  stalker-came  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-b1-2 place!
' items-used castle-b1-2 items-here
