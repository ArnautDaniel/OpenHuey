\ story/places/castle-1f-3.fs - the library in Belli Castle's 1F ($9): the first time at the Garden
\ Key, the scene of her awakening (the voice, the thing on the table); the key taken after; chased
\ by Debilitas while he is elsewhere - the hint ("hide behind it?"), and in behind the bookshelf she
\ knocks it down on him (a scene: he is knocked out under it, she runs out); pushing it before then,
\ her thought; the books, the harp, the creepy thing in turn, the Camomile, what Hewie looks up at
\ and stands wary of, the walls' shade. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/castle-1f-3.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.castle-1f-3
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    stalker-names items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant shelf-msg        \ "A book shelf of fine design. It has a rather sturdy look to it."
1 constant knock-msg        \ "If I just apply enough force, I might be able to knock it over..."
2 constant books-msg        \ "A wealth of information, right before your fingertips... A lot of the books seem to..."
3 constant books-2-msg      \ "Maybe they wouldn't mind if I took a few of these books... Nah, I'm sure they would..."
4 constant harp-msg         \ "This harp is so large and bulky... It's hard to imagine people actually play these..."
5 constant harp-2-msg       \ "Trying to play that for long stints of time would probably give even the most exper..."
6 constant hint-msg         \ "That bookshelf... I wonder if I can hide behind it?"
7 constant awakened-msg     \ "Fiona. I see you've finally awakened... There's something that I've wanted to s..."
8 constant snooping-msg     \ "What... is this? I probably shouldn't go snooping around things I know nothing about."
9 constant creepy-msg       \ "W-What is this thing? It is really giving me the creeps. Why would anyone in their..."
$A constant creepy-2-msg    \ "I think I'll refrain from touching it... Heck, even looking at it makes my skin cr..."

\ ---- its own state (a visit) ----
own: books-remarked  own: harp-remarked  own: creepy-remarked
own: chase-scene           \ chased, told of the shelf: the stalkers kept off a while (let go as she leaves)
own: hint-timer            \ that while's activity (k+1; 0 none)
own: key-scene             \ the awakening (or the key taken) under way: the while waits for it
own: hewie-wary            \ Hewie has stood wary by the table once this visit

\ ---- the room's things ----
0 constant camomile-fx   0 constant fallen-part   1 constant table-part   2 constant thing-part   3 constant key-part
2 constant shelf-object
0 constant shelf-floor   1 constant shelf-stalker-floor   2 constant fallen-floor   3 constant fallen-stalker-floor
4 constant hewie-off-floor
8 constant behind-shelf-area   9 constant shelf-area   $A constant books-area   $B constant harp-area   $C constant thing-area
4 constant first-track   4 constant awakened-movie   3 constant awakened-cutscene
6 constant shelf-movie   5 constant shelf-cutscene   1 constant shelf-night-movie   0 constant shelf-night-cutscene
$18 constant shelf-seen   $19 constant shelf-night-seen   $E constant shelf-door   3 constant figure-slot
4 constant fall-signal   $28 constant crash-sound   $13 constant his-cry
: night? ( -- flag )  c2f-3-flag1 fact? ;
: shelf-down? ( -- flag )  c1f-3-bookshelf-examined fact? ;
: awakened? ( -- flag )  c1f-3-flag1 fact? ;
: he-hunts-elsewhere? ( -- flag )   \ (Debilitas in the game, not down, not here)
    debilitas?  st-cond-now @ down-cond <> and  st-room-now @ castle-1f-3 <> and ;
: tinted ( -- )  night? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;
: frame-reached? ( frame -- flag )  cutscene-frame swap < 0= ;   \ (the scene's, the original's cue)
: shot? ( shot -- flag )  cutscene-frame cutscene-shot-at = ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  awakened? 0= if  first-track 1e background  then
    hewie-off-floor keeps-hewie-off floor-off
    shelf-down? if
        shelf-object object-hide  shelf-floor solid-floor floor-edge or pushable or floor-off
        shelf-stalker-floor keeps-debilitas-off floor-off  fallen-part show-part
        fallen-floor solid-floor floor-on  fallen-stalker-floor keeps-debilitas-off floor-on
    else
        shelf-floor solid-floor floor-edge or pushable or floor-on  shelf-stalker-floor keeps-debilitas-off floor-on
    then
    c1f-3-camomile fact? 0= if  camomile-fx -6e 11.5e -46e flicker-at  then
    got-garden-key fact? 0= if  key-part show-part  then
    awakened? if  thing-part show-part  else  table-part show-part  then
    tinted  1 $2300 sound-reverb! ;

\ ---- the shelf knocked down on Debilitas (the scene): his crash and cry by its signals, Hewie off
\ out of the way; after it he lies knocked out under it, and she runs out ----
: his-fall ( -- )   \ (the scene's own: by its signal's count - the door burst, his crash, his cry, Hewie off)
    begin
        fall-signal 0 signal-passed? if
            0 exit-open? 0= if  shelf-door true open-mark  doors-redone  crash-sound common-bank partner sound-by  then
        then
        fall-signal 1 signal-passed? if  his-cry stalker-bank partner sound-by  then
        fall-signal 2 signal-passed?  hewie-in? and if
            hewie hold  hewie $A1 0 -1 walk-path-move 6e 33e go-to
        then
        fall-signal 3 signal-passed? 0= while  yield
    repeat ;
: hewie-aside ( -- )   \ (off to the wall, kept from the shelf's floor after)
    hewie hold  hewie $D 0 -1 walk-path-move 4e 13e go-to  hewie-off-floor keeps-hewie-off floor-on ;
: him-in ( tri -- )  partner dup 0< if  2drop exit  then  >r castle-1f-3 swap r> send stalker-in ;
: shelf-scene ( night? -- movie? )   \ (the movie, by day or by night)
    if  shelf-night-movie scenes:unseen-4-movie shelf-night-cutscene
    else  shelf-movie scenes:unseen-4-movie shelf-cutscene  then  load-scene ;
: knocks-it-down ( -- )
    chase-scene @ if  stalkers-stay state-off  hint-timer @ ?dup if  1- stop-act  0 hint-timer !  then  then
    fiona hold  c1f-3-bookshelf-examined fact!  castle-1f-3 hewie-slot true doors-into
    103 him-in  partner $67 180 place-on-tri  partner false shown  0 summon-take
    15 screen:to-black-2 with-music or fade
    night? shelf-scene  wait-fade
    ['] his-fall start drop
    hewie-in? if  ['] hewie-aside start drop  c1f-3-flag2 fact!  then
    0 true no-shadow  partner-kind true no-shadow
    panic-eased  if  -1 run-scene  then
    hewie let-go  camera-restart
    143 him-in  partner $8F 90 -34.144e 33.682e place-at  partner 0 0 camera-set  partner activate
    partner send stalker-knock-out
    night? if  shelf-night-seen  else  shelf-seen  then  resident-flag-set
    fiona let-go  world-held state!  0 false no-shadow  partner-kind false no-shadow
    shelf-door door-shut  0 prepare-exit  0 exit-door doors:door-locked? 0= if  0 take-exit  then
    castle-1f-3 hewie-slot false doors-into  c1f-3-bookshelf-examined fact!  c1f-1-flag2 fact-off ;

\ ---- chased (Debilitas elsewhere), coming in: the hint - the stalkers kept off a while ----
: hint-over ( -- )   \ (240 frames on - not while the key's scene runs - let go)
    240 frames  begin  key-scene @ while  yield  repeat
    stalkers-stay state-off  force-chased state-off  0 chase-scene !  0 hint-timer ! ;
: hint ( -- )
    stalkers-stay state!  force-chased state!  fiona hold  c1f-3-bookshelf-hint fact!  -1 chase-scene !
    fiona wait-move  fiona rest-anim 10 start-blend  hint-msg show  fiona wait-anim  fiona 10 idle  wait-closed
    fiona let-go  ['] hint-over start 1+ hint-timer ! ;
: hint-due? ( -- flag )
    shelf-down? 0=  c1f-3-bookshelf-hint fact? 0= and  chased? and  in-panic? 0= and  he-hunts-elsewhere? and
    stalker-in? 0= and ;
: hint-on-entering ( -- )   \ (as she comes in - once the stalker has told where he is: two frames)
    2 frames  hint-due? if  hint  then ;

\ ---- the awakening (the first time at the key): the voice, the thing set on the table; else the key ----
: awakening-cues ( -- )   \ (the scene's own: the music back at frame $64, the table's thing from its fourth shot)
    begin  $64 frame-reached? 0= while  yield  repeat  music send bgm-resume
    begin  4 shot? 0= while  yield  repeat  table-part hide-part  thing-part show-part ;
: awakening ( -- )
    {: | cues :}  15 screen:to-black-2 with-music or fade
    awakened-movie scenes:unseen-4-movie awakened-cutscene load-scene  wait-fade
    0 true no-shadow  ['] awakening-cues start to cues
    panic-eased  if  awakened-msg run-scene  then
    music send bgm-resume  wait-fade  0 false no-shadow  ['] awakening-cues running? if  cues stop-act  then
    table-part hide-part  thing-part show-part  shelf-object object-show
    fiona 0 idle  fiona $13C -73 23.771e -27.607e place-at  1 1 player-camera  camera-restart
    c1f-3-flag1 fact!  15 screen:from-black with-music or fade  wait-fade ;
: takes-key ( -- )
    fiona 10e -17e turn-to-xz  fiona reach-out-anim play  4 frames
    got-garden-key fact!  key-part hide-part
    garden-key item-named  garden-key 1 give-item  garden-key show-item-tab  fanfare  obtained-2-msg say
    fiona reach-back-anim play ;
: at-the-key ( -- )
    -1 key-scene !  stalkers-stay state!  fiona hold  fiona wait-move
    awakened? if  takes-key  else  awakening  then
    chase-scene @ 0= if  stalkers-stay state-off  then  0 key-scene ! ;

\ ---- what she may do or say here ----
: will-it-move ( -- )   \ (pushing at the shelf: her thought)
    fiona hold  knock-msg show  fiona -1 idle  7 frames  wait-closed ;
: shelf ( -- )  shelf-msg say ;
: behind-shelf ( -- )   \ (from behind it)
    fiona wait-move  stalkers-stay state!  world-frozen state!
    fiona $DA 0 -26.45e -13.04e place-at  1 20e -18e 8e 0e shot  shelf-msg say  shot-off
    world-frozen state-off  stalkers-stay state-off ;
: books ( -- )
    fiona wait-move  books-remarked @ if  books-2-msg  else  -1 books-remarked !  books-msg  then  say ;
: harp ( -- )
    fiona $77 90 -1 walk-move 24.585e 26.685e go-to  1 20e 60e -45e 30e shot  16 frames
    fiona look-at-anim start-anim  55 frames
    harp-remarked @ if  harp-2-msg  else  -1 harp-remarked !  harp-msg  then  say
    fiona wait-anim  16 frames  shot-off ;
: creepy-thing ( -- )
    world-frozen state!  fiona $13B -90 20.181e -28.363e place-at  1 10e 20e -30e 0e shot
    awakened? 0= if  snooping-msg  else  creepy-remarked @ if  creepy-2-msg  else  -1 creepy-remarked !  creepy-msg  then  then  say
    16 frames  world-frozen state-off  shot-off ;
: reach-camomile ( -- )
    fiona wait-move  fiona -6e -46e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  camomile c1f-3-camomile camomile-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    straining-now @  fiona held? 0= and if  ['] will-it-move start drop  exit  then
    shelf-down? 0= if
        fiona shelf-area in-area?  fiona 180 45 heading? and if  ['] shelf unhurried offer  then
        fiona behind-shelf-area in-area?  fiona 0 45 heading? and if  ['] behind-shelf unhurried offer  then
    then
    fiona books-area in-area?  fiona -134 90 heading? and if  ['] books unhurried offer  then
    fiona harp-area in-area?  fiona 90 45 heading? and if  ['] harp unhurried offer  then
    fiona thing-area in-area?  fiona -90 45 heading? and if  ['] creepy-thing unhurried offer  then
    c1f-3-camomile fact? 0=  fiona -6e 11.5e -46e 5e 5e touches? and if  ['] reach-camomile calm-only offer  then
    got-garden-key fact? 0=  fiona 10e 6e -17e 5e 5e touches? and if  ['] at-the-key calm-only offer  then ;

\ ---- in behind the shelf, chased by Debilitas elsewhere: she knocks it down ----
: knock-due ( -- )
    shelf-down?  fiona held? or  in-panic? or  chased? 0= or if  exit  then
    fiona behind-shelf-area in-area?  he-hunts-elsewhere? and if  ['] knocks-it-down start drop  then ;

\ ---- Hewie: wary by the table once a visit (one time in two), looking up there and at the shelf ----
create looks
    13.13e 0e -30.63e  28e 11e 11e look,
    26.99e 0e 27.94e  24e 22e 22e look,
: stands-by-table ( -- )  13.13e 0e -30.63e hewie-stands-by ;
: hewie-about ( -- )
    hewie-in?  hewie-cmdable-now @ and 0= if  exit  then
    danger-now @ calm = if  looks /look + looks-at  then
    chased? if  exit  then
    hewie 13.13e 0e -30.63e 28e 11e centred-in? 0= if  exit  then
    hewie-wary @ 0=  hewie-cond-now @ well-cond = and if
        -1 hewie-wary !  50 chance? if  ['] stands-by-table start drop  then
    then
    looks looks-at ;

\ ---- the walls' shade ----
: walls-shade ( -- )  -0.9e 11.8e -26.5e  -2.9e 11.7e -22e  0.2e -1e -26e  -1.9e -1e -22e  light-doorway ;

\ ---- the movies replayed (the extras): the awakening, the shelf by day, by night ----
: replay-awakening ( -- )
    fiona hold  first-track 1e background  key-part show-part  table-part show-part  tinted
    awakened-movie scenes:unseen-4-movie awakened-cutscene load-scene
    0 true no-shadow  ['] awakening-cues start drop
    panic-eased  if  awakened-msg run-scene  then
    music send bgm-resume  0 false no-shadow  replay-over ;
: replay-shelf ( night? -- )   \ (by day the room's tint too)
    {: at-night :}  fiona hold  thing-part show-part  at-night 0= if  tinted  then
    debilitas-kind figure-slot figure-in  yield
    at-night shelf-scene  0 true no-shadow  debilitas-kind true no-shadow
    panic-eased  if  -1 run-scene  then
    figure-slot figure-out  replay-over ;

\ ---- every frame ----
create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   3 , 0 , 0 ,   4 , 1 , 1 ,   \ (area, set, path)
: each-frame ( -- )
    events-held state? if  exit  then
    0 exit-out  cameras 4 cameras-at  knock-due  hewie-about  spots  walls-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case
            $10 of  ['] replay-awakening start drop  endof
            $11 of  [: false replay-shelf ;] start drop  endof
            $12 of  [: true replay-shelf ;] start drop  endof
        endcase  exit
    then
    0 0 0 way-in  ['] hint-on-entering start drop ;
: went ( -- )  chase-scene @ if  stalkers-stay state-off  force-chased state-off  then ;

' came-in ' person-in ' each-frame ' went castle-1f-3 place!
