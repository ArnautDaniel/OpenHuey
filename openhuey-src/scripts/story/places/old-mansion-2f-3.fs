\ story/places/old-mansion-2f-3.fs - the decomposer's room of the old mansion's 2F: the decomposer
\ (its plaque, its file; the Handful of Ash put in, the Prima Materia out - anything else, nothing);
\ the weak floor (it creaks underfoot; the stalker crossing it once the chapter has turned, it gives
\ way under him - he falls, gone); the crawl space Hewie squeezes into at her "go there" (out at
\ "come back") till the furnace room is reached; the notes on the dog; the research gear (the Album
\ Medallion inside); Hewie barking at a spot, his looks; the doorway's shade. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-2f-3.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20). Its act 1 - her
\ dropping through to the room below - nothing starts.
IN: story.places.old-mansion-2f-3
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names hewie-names story.kit fiona.anims ;

own-reset

0 constant nothing-msg      \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
1 constant transformed-msg  \ "Oh my! It actually transformed..."
2 constant fell-out-msg     \ "Oh my! The floor fell out! Why is this room in such a state of disrepair? The other roo..."
3 constant plaque-msg       \ "Decomposer The initial step in this grand process begins with a manipulation of the..."
4 constant decomposer-msg   \ "A decomposer? More unusual machines... It appears you can insert an item here..."
6 constant draught-msg      \ "I can distinctly feel air coming from inside. I wonder where this leads?"
7 constant too-tight-msg    \ "It's too tight of a squeeze for me."
$A constant notes-msg       \ "Some notes have been jotted down."
$B constant dog-notes-msg   \ "That foul beast of a dog! The damned beast ran off with the Prima Materia that I was r..."
$C constant a-dog-msg       \ "A dog... Maybe it is referring to Hewie?"
$D constant furnace-msg     \ "The furnace has been lit!? Who did this?"
$E constant gear-msg        \ "Shoddy-looking research equipment has been left here."
$F constant gear-2-msg      \ "I've seen similar equipment before. It looks almost archaic in nature."
$10 constant inside-msg     \ "What's this? There is something inside..."
$11 constant nothing-unusual-msg   \ "Nothing unusual."
$12 constant weak-floor-msg \ "The floor looks like it could give way at any second... I'm hardly overweight but I'd..."
own: plaque-read  own: draught-remarked  own: gear-remarked  own: spot-noticed  own: put-in
0 constant creak-sound   1 constant fall-sound   3 constant change-sound
$241 constant decomposer-file   $240 constant notes-file
$A8 constant before-crawl-tri   $96 constant crawl-tri   1 constant crawl-group
9 constant transforms   $A constant nothing-from-it   \ (the items' events here: items.fs)

\ ---- the weak floor ----
: weak-floor? ( id -- flag )  35e 0e 0e 8e 4e touches? ;
: stepped-on? ( id -- flag )  35e 0e 0e 8e 4e steps-into? ;
: floor-gone ( -- )  0 hide-part  0 barred-floor hole-floor or floor-on ;
: crawl-floor ( -- )   \ (the crawl space: Hewie's only - till the furnace room is reached)
    om2f-2-flag1 fact? if  1 show-part  crawl-group solid-floor floor-edge or floor-on  exit  then
    hidden-act hewie-doing? if
        hewie crawl-tri 90 -78e 0e place-at  18000 2 hewie send hewie-yelp
        crawl-group keeps-fiona-off keeps-pursuers-off or floor-edge or floor-on
    else  crawl-group solid-floor keeps-fiona-off or keeps-pursuers-off or floor-edge or floor-on  then ;
: came-in ( exit -- )
    drop  room-sounds
    om2f-3-flag1 fact? if  floor-gone  else  0 show-part  0 keeps-hewie-off floor-on  then
    hewie-in?  hewie 1 on-group? and if  hewie before-crawl-tri put-on-tri  then
    crawl-floor ;

\ ---- the stalker on the weak floor: it gives way under him ----
: his-fall ( -- )
    stalkers-stay state!  stalker hold  fiona hold  creak-sound room-bank stalker sound-by
    15 screen:to-black-2 with-music or fade  wait-fade  things-clear  text-gone
    0 homunculi-off  fall-sound room-bank stalker sound-by  stalker let-go  3 summon-take
    60 frames  fiona 0 on-group? if  fiona $68 160 place-on-tri  then
    floor-gone  fiona -1 idle  7 frames  camera-restart  15 fade-in  stalkers-stay state-off ;
: gives-way ( -- )  om2f-3-flag1 fact!  panic-eased  ['] his-fall start drop ;

\ ---- the decomposer: its plaque; the Handful of Ash put in, the Prima Materia out; else nothing ----
: at-decomposer ( -- )  stalkers-stay state!  fiona hold  fiona wait-move  fiona $A0 0 29.5e 34.5e walk-to ;
: the-decomposer ( -- )
    at-decomposer
    plaque-read @ if
        fiona hand-to-mouth-anim 10 start-blend  decomposer-msg say  fiona wait-anim  0 plaque-read !
    else
        15 screen:to-dim fade wait-fade  plaque-msg say  -1 plaque-read !  15 screen:from-dim fade wait-fade
    then
    decomposer-file give-file  stalkers-stay state-off ;
: working ( -- )  change-sound room-bank fiona sound-by  120 frames  fiona look-close-anim play ;
: transforms-it ( -- )
    at-decomposer  working  transformed-msg say
    fiona reach-out-anim play  handful-of-ash spend  prima-materia item-named  prima-materia 1 give-item
    prima-materia show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play  stalkers-stay state-off ;
: nothing-comes ( -- )
    at-decomposer  working  fiona reach-out-anim play
    put-in @ dup item-named  show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play
    nothing-msg say  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    {: who ev var it :}  who 0<> if  false exit  then
    ev transforms = if  ['] transforms-it start drop  true exit  then
    ev nothing-from-it = if  it put-in !  ['] nothing-comes start drop  true exit  then
    false ;

\ ---- Hewie into the crawl space (her "go there" at it) and out (her "come back") ----
: crawls-in ( -- )
    hewie hold  hewie-bark  hewie wait-move
    hewie before-crawl-tri -90 -1 walk-path-move -30e 0e go-to
    crawl-group solid-floor floor-off
    hewie crawl-tri -90 $204 walk-move -78e 0e go-to
    18000 2 hewie send hewie-yelp ;
: crawls-out ( -- )
    hewie hold  hewie before-crawl-tri 90 $204 walk-move -30e 0e go-to
    hewie -1 idle  hewie wait-anim  0 0 hewie send hewie-yelp  0 0 hewie-do
    crawl-group solid-floor floor-on ;
: sent-in? ( -- flag )
    started-now @ go-there-cmd =  hewie-cond-now @ down-cond <> and  stalker-in? 0= and  cam-set@ 1 = and
    fiona unseen? 0= and  hewie unseen? 0= and  fiona 50 -50e 0e faces-point? and  hewie-breakable-now @ and ;
: the-crawl ( -- )
    om2f-2-flag1 fact?  hewie-in? 0= or if  exit  then
    yelp-anim-act hewie-doing? if
        started-now @ come-back-cmd = if  ['] crawls-out start drop  then
    else  sent-in? if  ['] crawls-in start drop  then  then ;

\ ---- what she may do or say here ----
: the-crawl-space ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona -50e 0e turn-to-xz
    om2f-2-flag1 fact? if  furnace-msg
    else  draught-remarked @ if  too-tight-msg  else  -1 draught-remarked !  draught-msg  then  then
    say  stalkers-stay state-off ;
: the-notes ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona 180 turn  notes-msg say
    15 screen:to-dim fade wait-fade  dog-notes-msg say  15 screen:from-dim fade wait-fade  a-dog-msg say
    notes-file give-file  stalkers-stay state-off ;
: looks-down ( msg -- )  fiona look-low-anim start-anim  40 frames  say  fiona wait-anim ;
: fell-out ( -- )  fiona wait-move  fell-out-msg looks-down ;
: weak-floor ( -- )  stalkers-stay state!  fiona hold  fiona wait-move  weak-floor-msg looks-down  stalkers-stay state-off ;
: the-gear ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona -3.17e 12.09e turn-to-xz
    gear-remarked @ if  gear-2-msg  else  -1 gear-remarked !  gear-msg  then  looks-down  stalkers-stay state-off ;
: inside-the-gear ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona 5.3e 37.5e turn-to-xz
    fiona look-low-anim start-anim  40 frames
    om2f-3-album-medallion fact? if  nothing-unusual-msg say
    else  inside-msg say  album-medallion om2f-3-album-medallion -1 takes-up  then
    stalkers-stay state-off ;
: barks-at-spot ( -- )  -42.06e 0e -0.81e hewie-barks-at ;

create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   \ (area, set, path)
create looks  32.38e 0e -0.63e  22e 10e 0e look,   31.11e 0e 48.2e  30e 15e 18e look,
: each-frame ( -- )
    0 0 leaves-by  cameras 2 cameras-at
    om2f-3-flag1 fact? 0= if
        fiona stepped-on? if  creak-sound room-bank fiona sound-by  then
        over-music-c fact?  stalker stepped-on? and if  gives-way  then
    then
    the-crawl
    spot-noticed 50 -42.06e 0e -0.81e 26e 5e notices? if  ['] barks-at-spot start drop  then
    looks 2 hewie-looks-at
    fiona 4 in-area?  fiona 0 50 heading? and if  ['] the-decomposer unhurried offer  then
    fiona 5 in-area?  fiona -45 50 heading? and if  ['] the-crawl-space unhurried offer  then
    fiona 6 in-area?  fiona 90 50 heading? and if  ['] the-notes unhurried offer  then
    om2f-3-flag1 fact? if  fiona 3 50 faces-area? if  ['] fell-out unhurried offer  then
    else  fiona weak-floor? if  ['] weak-floor unhurried offer  then  then
    fiona 7 60 faces-area? if  ['] the-gear unhurried offer  then
    fiona 8 in-area?  fiona 50 5e 37e faces-point? and if  ['] inside-the-gear unhurried offer  then
    45e -4e -20e  65e -4e -20e  45e -4e 20e  65e -4e 20e  light-doorway ;
: person-in ( i -- )  fiona-p = if  0 0 0 way-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-3 place!
' items-used old-mansion-2f-3 items-here
