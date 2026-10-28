\ story/places/castle-b1-7.fs - the storeroom behind the furniture in Belli Castle's B1 ($1D): the
\ Study Key out of her reach by a small hole - Hewie, hidden in the hole on the other side
\ (castle-1f-19), comes through it here; sent for the key he fetches it (back to the hole as she
\ leaves); the box before the door to castle-1f-18 (pushed off its square the door unlocks; her
\ pushing it locks it again) and the stalker trying that door while it's blocked; the vase and its
\ Medallion, the Medallion Hewie sniffs out, the one in the drain; the covered furniture, the flame,
\ the far view. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-b1-7.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.castle-b1-7
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims hewie.anims hewie-names ;

own-reset

\ ---- what is said here ----
0 constant key-msg        \ "Looks like a key or something is lying on the ground. I can't reach it from here..."
1 constant hole-msg       \ "There is a small hole in the wall. That could be my way to the key..."
2 constant sheets-msg     \ "The furniture is covered with freshly washed sheets."
3 constant blocked-msg    \ "All of this stuff is blocking the door and I can't get out."
4 constant drain-msg      \ "Huh? There is something in the drain."
5 constant nothing-msg    \ "There's nothing particularly interesting here."

\ ---- its own state (a visit) ----
own: key-asked     \ she sent Hewie for the key (this frame)
own: key-step      \ the fetch: 1 she's free for it, 2 he has it, 3 she has it
own: rattling      \ the stalker at the blocked door (its scene)
own: tries-left    \ his tries at it still to come

\ ---- the room's things ----
0 constant flame-fx   1 constant key-fx   2 constant medallion-fx   3 constant found-fx
0 constant the-box   $1ED constant box-square   $26 constant blocked-door   0 constant door-exit
6 constant hole-area   7 constant furniture-area   8 constant drain-area
3 constant before-area   4 constant beyond-area
1 constant try-sound   $1C03 constant fetch-anim   $827C constant fetched-file
252 constant hole-side-tri   $FC constant waiting-tri   $92 constant key-tri   \ (the first two the same: by the hole)
cb1-7-breakable-broken 0 1  31e -41e 13e 5e 8e  breakable: the-vase
the-vase $80404040 0 9 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv
the-vase 0 frees-floor  the-vase $1B0 noise-from

\ ---- as she comes in: the flame, the key, the vase, the box; Hewie through the hole ----
: medallion-shows ( -- )  cb1-7-medallion fact? 0= if  medallion-fx 31e -40e 13e flicker-at  then ;
: found-shows ( -- )  cb1-7-medallion-found fact?  cb1-7-medallion-2 fact? 0= and if  found-fx 29e 1e 21e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds  flame-fx 0 30e -27.65e -9.8e flame-at
    got-study-key fact? 0= if  key-fx -12e -40e -25e flicker-at  then
    the-vase as-it-stands  the-vase broken? if  medallion-shows  then  found-shows
    the-box 0 0 box-square $245 obstacle-in ;

\ ---- the Study Key: Hewie, come through the hole, waits by it; sent for the key he fetches it ----
: back-to-hole ( -- )   \ (left on the hole's side: back in it, beyond the wall)
    room-flag-2d state-off
    hewie-in?  hewie-way-now @ 1 = and if  castle-1f-19 -1 0 hewie-to  1800 2 hewie send hewie-yelp  then ;
: takes-key ( -- )
    fiona hold  begin  key-step @ 2 < while  yield  repeat
    got-study-key fact!  key-fx effect-off  10 trust+
    study-key item-named  study-key 1 give-item  study-key show-item-tab  fanfare  obtained-2-msg say
    40 trust+  fetched-file give-file  3 key-step ! ;
: key-handed ( -- )  with-fiona-free  1 key-step !  panic-eased  ['] takes-key start drop ;
: fetches ( -- )
    begin  key-step @ 1 < while  yield  repeat
    hewie-bark  hewie wait-move  hewie key-tri 45 -1 walk-move -17e -30e go-to
    hewie fetch-anim play  2 key-step !
    begin  key-step @ 3 < while  yield  repeat ;
: waits-by-hole ( -- )
    hewie hold  hewie waiting-tri 0 place-on-tri  hewie sit-anim play  hewie fiona look-at  yield
    begin  key-asked @ 0= while  yield  repeat
    0 key-step !  ['] key-handed start drop  fetches ;
: through-hole ( -- )   \ (a moment on, as Hewie tells where he is: hidden in the hole beyond - here)
    2 frames
    hewie body-room castle-1f-19 =  hewie-act-now @ hidden-act = and if
        room-flag-2d state!  castle-b1-7 hole-side-tri 1 hewie-to
        got-study-key fact? 0= if  ['] waits-by-hole start drop  then
    else  back-to-hole  then ;
: faces-key? ( -- flag )  fiona hole-area in-area?  fiona 50 -12e -25e faces-point? and ;
: key-asked? ( -- flag )   \ (her "go there" or "come back" toward it, not chased)
    started-now @ dup go-there-cmd =  swap come-back-cmd = or  chased? 0= and  faces-key? and ;

\ ---- the box before the door: off its square the door gives; her pushing it locks it again ----
: door-locked? ( -- flag )  blocked-door doors:door-locked? ;
: doorway? ( -- flag )  door-exit exit-door dup 0< if  drop false  else  door-flags 1 and 0<>  then ;
: pushing-shut? ( -- flag )   \ (pushing - not straining at it - the door there shut)
    fiona-sub-now @ 5 =  straining-now @ 0= and  doorway? 0= and  door-exit exit-open? 0= and ;
: the-box-door ( -- )
    the-box box-square obstacle-on-tri?  door-locked? and if  blocked-door unlock  then
    pushing-shut?  door-locked? 0= and if  blocked-door lock  then ;

\ ---- the stalker in castle-1f-18 at the blocked door: tries it, a crack, now and then ----
: door-crack ( open? -- )  if  -0.08e rad>deg  else  0e  then  door-exit true door-swing ;
: tries-it ( -- )  try-sound -40e 0e -20e room-sound  true door-crack  yield  false door-crack ;
: tries-door ( -- )
    stalkers-stay state!  60 frames  4 random 2 + tries-left !
    begin
        tries-it  25 chance? if  16 frames  then  16 frames
        tries-it  50 chance? if  32 frames  then  60 frames
        tries-left @ while  -1 tries-left +!  yield
    repeat
    door-locked? if
        cb1-7-flag1 fact? 0= if  cb1-7-flag1 fact!  else  75 chance? if  cb1-7-flag2 fact!  then  then
        stalker let-go
    else  1 stalker-order  then
    stalkers-stay state-off  0 rattling ! ;
: at-the-door ( -- )
    rattling @ 0= if
        door-locked?  st-room-now @ castle-1f-18 = and  stalker-told? and if  -1 rattling !  ['] tries-door start drop  then
        exit
    then
    door-locked? 0= if
        ['] tries-door stop-doing  false door-crack  1 stalker-order  stalkers-stay state-off  0 rattling !
    then ;

\ ---- what she may do or say here ----
: the-hole ( -- )
    fiona wait-move  fiona look-low-anim 10 play-blend  key-msg say  8 frames
    4 -1 fixed-camera  8 frames  hole-msg say  8 frames  3 1 player-camera  camera-restart ;
: furniture ( -- )  fiona wait-move  sheets-msg say ;
: blocked ( -- )  fiona wait-move  fiona -90 turn  blocked-msg say ;
: the-drain ( -- )
    fiona wait-move
    cb1-7-medallion-3 fact? if  nothing-msg say exit  then
    fiona 0 turn  drain-msg say  medallion item-named
    medallion item-full? if  cant-carry-msg say exit  then
    cb1-7-medallion-3 fact!  medallion 1 give-item  medallion show-item-tab  fanfare  obtained-msg say ;
: vase-medallion ( -- )  medallion cb1-7-medallion medallion-fx 31e 13e picks-up ;
: found-medallion ( -- )  medallion cb1-7-medallion-2 found-fx 29e 21e picks-up ;
: hewie-hole-side? ( -- flag )  hewie-in?  hewie-way-now @ 1 = and ;
: spots ( -- )
    the-vase broken? if
        cb1-7-medallion fact? 0=  fiona 31e -40e 13e 5e 5e touches? and if  ['] vase-medallion calm-only offer  then
    else  the-vase breaks? if  medallion-shows  then  then
    fiona furniture-area 60 faces-area? if  ['] furniture unhurried offer  then
    hewie-hole-side? 0=  got-study-key fact? 0= and  faces-key? and if  ['] the-hole unhurried offer  then
    1 locked-request? if  ['] blocked any-time offer  then
    fiona drain-area in-area?  fiona 0 50 heading? and if  ['] the-drain unhurried offer  then
    cb1-7-medallion-found fact?  cb1-7-medallion-2 fact? 0= and  fiona 29e 1e 21e 5e 5e touches? and if
        ['] found-medallion calm-only offer
    then ;

\ ---- Hewie scrambling about by the wall: the Medallion he sniffs out ----
: medallion-sniffed ( -- )  cb1-7-medallion-found cb1-7-medallion-2 found-fx  29e 1e 21e  hewie-finds ;
: hewie-sniffs ( -- )
    cb1-7-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie 29e 0e 21e 10e 5e touches? if  ['] medallion-sniffed start-only  then ;

\ ---- every frame ----
create cameras  1 , 0 , -1 ,   2 , 1 , 0 ,   3 , 1 , 0 ,   4 , 2 , -1 ,   5 , 3 , 1 ,   \ (area, set, path)
create entries  0 , -1 ,   \ (by exit 0: the camera)
: far-view ( -- )  cam-changed? 0= if  exit  then  cam-set@ 1 = if  20e 30e 2000e 2000e depth-range  else  depth-range-off  then ;
: each-frame ( -- )
    key-asked? key-asked !
    0 exit-out  cameras 5 cameras-at
    fiona before-area entered? if  2 map-page  then   fiona beyond-area entered? if  1 map-page  then
    flame-fx 30e -27.65e -9.8e 8e -4e flame-stirred
    the-box-door  at-the-door  hewie-sniffs  spots  far-view ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 1 ways-in  came-by @ 0 = if  2 map-page  then  ['] through-hole start drop ;
: went ( -- )  back-to-hole ;

' came-in ' person-in ' each-frame ' went castle-b1-7 place!
