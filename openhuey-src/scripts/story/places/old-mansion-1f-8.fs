\ story/places/old-mansion-1f-8.fs - the old mansion's graveyard ($48): six graves of Aureolus Belli
\ (read in turn: once two are read, a grave read again says they all bear his name); the mausoleum's
\ words; the thicket by the pond Hewie can lie hidden in at her "go there" (out again at her "come
\ back") - Daniella passing it sets him on her: the ambush, a scene; the hand in the dark at the
\ back (what she sees there the first time; a mandragora fed to it - the White Mandragora lays it to
\ rest, opening the way out, the others only rouse it); the glow at the back Hewie fetches at her
\ "go for it" (the Prima Materia), the Antimony Tube across the wall; the Beef Jerky, the Album
\ Medallion; the back door unlocked from this side (the stalker sent on); the hangers Hewie brushes,
\ its steady sound, the splashes in the pond, his barks and looks; the replays. Never offered in the
\ original: its act03 (the Lavender here - its sparkle shows). A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/old-mansion-1f-8.fs in the history (converted
\ from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-8
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims fiona.sounds hewie.anims hewie-names scenes ;

own-reset

\ ---- what is said here ----
0 constant glow-msg        \ "There is something glowing from the rear of the graveyard... I just can't make it out f..."
7 constant same-name-msg   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
8 constant mausoleum-msg   \ "Looks to be a mausoleum... There are words written here."
9 constant words-msg       \ "Dead spirit craving for life, crawling back from the underworld... Sip of the fresh..."
$A constant spirits-msg    \ "Spirits? Life-blood? No matter how you cut it, these don't sound like the words of..."
$B constant opening-msg    \ "There is an opening in the thicket... It looks just big enough for an animal to ge..."
$C constant eyes-msg       \ "Hewie's eyes flash from inside the thicket."
$D constant not-dangerous-msg   \ "It doesn't seem too terribly dangerous... But I get the feeling that if I..."
$E constant wants-msg      \ "It's almost as if... it wants something..."
$F constant traces-msg     \ "There are traces as if something has recently been ripped to shreds here...."
$10 constant substitute-msg     \ "Mandragora... So, this is used as a substitute for life-blood."
$14 constant too-dark-msg  \ "It's too dark to see... It looks like there could be something in the back. F..." (YES / NO)
\ (the graves' names: messages 1 .. 6 - "Aureolus Belli (1698-1763)" ...)

\ ---- its own state (a visit) ----
own: glow-shown            \ she lacks what glows at the back (the Prima Materia and the rest): its sparkle there
own: graves-read           \ the graves read (a bit each)
own: graves-count          \ how many read
own: mausoleum-read        \ the mausoleum's words read (in turn with its second remark)
own: back-looks            \ her remarks at the back in turn (0 .. 2)
own: splashed              \ each pond spot's splash made (a bit each)
own: hewie-noticed         \ Hewie has barked at the glow (1) / the tube (2), once a visit
own: errand                \ Hewie's errand under way: 1 into the thicket, 2 the glow, 3 the tube
own: fetched               \ the glow's find handed over (her doing waits for it)
own: rung                  \ the first hanger last rang "on" (they ring on and off in turn)
own: hewie-gone            \ Hewie was here as a scene began: out of it till it ends
own: stalker-leaving       \ the back door unlocked: the stalker to be sent on
own: steady-on             \ its steady sound started (then kept up)

\ ---- the room's things ----
0 constant jerky-fx   1 constant lavender-fx   2 constant medallion-fx   3 constant glow-fx   4 constant shadow-fx
0 constant halves-group   2 constant thicket-group   3 constant back-group   4 constant door-group
0 constant yard-sound   1 constant back-door-area   $F constant glow-area   $17 constant back-area   $15 constant mausoleum-area
$16 constant thicket-area   6 constant map-area
$7D constant back-door   $7C constant hand-door   $79 constant leaving-door   $89 constant leaving-from
2 constant first-hanger   4 constant creak-on   5 constant creak-off
3 constant hand-slot   38 constant hand-kind   $9000 constant hand-lunge   $9001 constant hand-lurk
8 constant her-motions   $19 constant yard-track   $2E constant go-for-it-call   0 constant go-for-it-gesture
5 constant rest-movie   4 constant rest-cutscene   7 constant fed-movie   6 constant fed-cutscene
1 constant ambush-movie   0 constant ambush-cutscene
$8284 constant glow-file   $23A constant ambush-file   $8280 constant ambush-other-file
$31 constant ambush-seen   $32 constant fed-seen
8 constant thicket-tri   $175 constant before-thicket-tri   $19F constant after-scene-tri
: hiding? ( -- flag )  yelp-anim-act hewie-doing? ;   \ (in the thicket, lying low)
: thicket-in ( -- )   \ (Hewie in the thicket: she and the stalkers kept off its floor)
    thicket-group solid-floor floor-off  thicket-group keeps-fiona-off keeps-pursuers-off or floor-edge or floor-on ;
: thicket-out ( -- )
    thicket-group keeps-fiona-off keeps-pursuers-off or floor-off  thicket-group solid-floor floor-edge or floor-on ;
: specks-about ( -- )
    $10 $E $80 $80 $80 $40 rgba -140e 65.6e -130e specks   $10 $E $80 $80 $80 $40 rgba 42e 45.2e -118e specks
    $10 $E $80 $80 $80 $40 rgba -100e 53.3e -25e specks    $10 $E $80 $80 $80 $40 rgba -42e 53.2e -118e specks
    $10 $E $80 $80 $80 $40 rgba 149.5e 38e 61e specks ;
: shadow-cast ( on -- )  if  effects:wall-shadow shadow-fx effect!  else  shadow-fx effect-off  then ;
: hand ( -- id )  hand-slot figure ;
: hand-lurks ( -- )   \ (the hand at the back, reaching about)
    hand-slot figure 0< if  yield  then  hand-slot figure-on
    hand-slot 127.567e 0e -123.393e -90e figure-at  hand false doorway-fades  hand-slot hand-lurk 1 0 figure-play ;
: hand-lunges ( -- )  hand-slot hand-lunge 0 0 figure-play  hand-slot figure-wait  hand-slot hand-lurk 1 0 figure-play ;
: glow-wanted? ( -- flag )   \ (none of what glows there carried)
    prima-materia 1 has-items?  sulfuric-ore 1 has-items? or  silver-ore 1 has-items? or
    white-godstone 1 has-items? or  red-godstone 1 has-items? or  handful-of-ash 1 has-items? or 0= ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  yard-track 1e background
    halves-group both-halves floor-on  thicket-out
    back-door doors:door-locked? if  door-group other-half floor-off  door-group one-half floor-on  then
    om1f-8-beef-jerky fact? 0= if  jerky-fx 122.09e 8.28e 10.26e flicker-at  then
    om1f-5-lavender fact? 0= if  lavender-fx -131.47e 1e 9.32e flicker-at  then
    om1f-8-album-medallion fact? 0= if  medallion-fx -140.37e 16e -25.97e flicker-at  then
    glow-wanted? if  -1 glow-shown !  glow-fx -111e 17e -115e flicker-at  then
    om1f-12-flag1 fact?  om1f-8-flag1 fact? 0= and if
        om1f-8-flag1 fact!  old-mansion-1f-8 0 97 4 8 creature-in  old-mansion-1f-8 0 488 4 8 creature-in
    then
    first-hanger 2 chimes-rest  specks-about
    om1f-13-flag2 fact?  white-mandragora-used fact? 0= and if  0 summon-take  then   \ (the mandragoras' time: he is kept out)
    white-mandragora-used fact? 0= if
        back-group barred-floor floor-on  hand-kind hand-slot figure-in
        om1f-8-flag2 fact? if  ['] hand-lurks start drop  then
        true shadow-cast
    then ;
: hewie-came ( -- )   \ (hidden in the thicket: there still, her and the stalkers kept off it; else not left on its floor)
    hidden-act hewie-doing? if
        thicket-in
        hewie body-tri thicket-tri <> if  hewie thicket-tri 90 76.218e -13.075e place-at  1800 2 hewie send hewie-yelp  then
    else  hewie thicket-group on-group? if  hewie before-thicket-tri put-on-tri  then  then ;

\ ---- the back door: unlocked from this side; the stalker sent on ----
: unlocks-back ( -- )
    fiona 1 to-door
    fiona fiona body-pos 1 0 door-in-area? if  rattle-front-anim  else  rattle-behind-anim  then  start-anim
    20 frames  unlock-sound common-bank fiona sound-by  back-door unlock  -1 stalker-leaving !
    fiona 20 idle  20 frames  unlocked-msg say ;
: sent-on ( -- )   \ (the stalker's way out of here another; he goes on to where he was bound)
    0 stalker-leaving !
    leaving-door leaving-from door-as  leaving-door reopened  exits-redone
    door-group one-half floor-off  door-group other-half floor-on
    hewie-in? if  0 hewie send hewie-side!  then
    stalker-in? if  0 stalker-order
    else  stalker-told?  st-cond-now @ down-cond <> and if
        stalker body? if  stalker body-room  else  this-room @  then  -1 stalker-to
    then  then
    creatures-grounded ;

\ ---- the hand at the back: what she sees there; the mandragoras ----
: hewie-out ( -- )  hewie-in? dup hewie-gone !  if  hewie let-go  hewie false shown  then ;   \ (out of the scene)
: hewie-back ( -- )
    hewie-gone @ 0= if  exit  then  0 hewie-gone !
    hewie true shown  hewie activate  old-mansion-1f-8 after-scene-tri 140 hewie-to ;
: seized ( -- )   \ (the first time: grabbed at - her fright)
    fiona use-motions  hand-slot figure-on  hand-slot 127.567e 0e -123.393e -90e figure-at  hand false doorway-fades
    fiona look-close-anim play  ['] hand-lunges start drop  15 frames
    seized-sound common-bank fiona sound-by  fiona 0 room-motion + play
    panic-stage @ 3 <> if  3 panic-to  then
    fiona 1 room-motion + play  fiona 3 room-motion + play  om1f-8-flag2 fact! ;
: back-remark ( -- )   \ (the hand seen: her remarks in turn)
    back-looks @ case
        0 of  not-dangerous-msg say  endof
        1 of  wants-msg say  endof
        2 of  fiona crouch-down-anim play  traces-msg say  fiona crouch-up-anim play  endof
    endcase
    back-looks @ 1+ 3 mod back-looks ! ;
: the-back ( -- )
    fiona $1A2 90 -1 walk-move 113e -122e go-to  1 25e 20e 30e 0e shot
    white-mandragora-used fact? if
        fiona crouch-down-anim play  substitute-msg say  fiona crouch-up-anim play
    else  om1f-8-flag2 fact? if  back-remark  else
        stalkers-stay state!  fiona her-motions motions-for
        too-dark-msg yes? if  seized  then
        stalkers-stay state-off
    then  then
    shot-off ;
: scene-start ( movie cutscene -- )
    {: movie cut :}  stalkers-stay state!  fiona hold  15 screen:to-black-2 with-music or fade  no-track 1e background
    movie scenes:unseen-movie cut load-scene drop  wait-fade  world-held state!  hewie-out ;
: laid-to-rest ( -- )   \ (the White Mandragora given: the hand at rest, the way on open)
    stalkers-stay state!  fiona hold  fiona wait-move  white-mandragora spend  hand-door unlock  white-mandragora-used fact!  back-group barred-floor floor-off
    ['] hand-lunges stop-doing  ['] hand-lurks stop-doing
    rest-movie rest-cutscene scene-start  hand-slot figure-on  panic-eased  -1 run-scene
    15 screen:from-black with-music or fade  yard-track 1e background  hewie-back
    hand-slot figure-out  false shadow-cast  wait-fade
    stalkers-stay state-off  summoner-on state!  old-mansion-1f-8 stalker-slot 0 doors-into  old-mansion-b1-6 stalker-slot 1 doors-into ;
: fed ( item -- )   \ (another mandragora given: the hand roused, her fright)
    stalkers-stay state!  fiona hold  fiona wait-move  spend
    ['] hand-lunges stop-doing  ['] hand-lurks stop-doing
    fed-movie fed-cutscene scene-start  hand-slot figure-on  panic-eased  -1 run-scene
    fiona $1A2 90 108e -122e place-at  hewie-back  om1f-8-flag2 fact!  ['] hand-lurks start drop
    fiona her-motions motions-for  fiona use-motions  fiona 3 room-motion + start-anim
    panic-stage @ 3 <> if  3 panic-to  then
    15 screen:from-black with-music or fade  yard-track 1e background  wait-fade  fiona wait-anim
    fed-seen resident-flag-set  stalkers-stay state-off ;
: feed-yellow ( -- )  yellow-mandragora fed ;
: feed-red ( -- )  red-mandragora fed ;
: feed-blue ( -- )  blue-mandragora fed ;
: feed-purple ( -- )  purple-mandragora fed ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    case
        0 of  ['] laid-to-rest  endof
        $17 of  ['] feed-yellow  endof   $18 of  ['] feed-red  endof
        $19 of  ['] feed-blue  endof     $1A of  ['] feed-purple  endof
        >r false r>  endcase
    ?dup if  start drop  true  else  false  then ;

\ ---- Hewie into the thicket (her "go there" at it) and out (her "come back") ----
: into-thicket ( -- )
    hewie hold  1 errand !  hewie-bark  hewie wait-move
    hewie before-thicket-tri -90 -1 walk-path-move 92.855e -13.871e go-to
    thicket-group solid-floor floor-off  thicket-group keeps-fiona-off keeps-pursuers-off or floor-on
    hewie thicket-tri -90 $204 walk-move 76.218e -13.075e go-to  hewie 90 turn
    hewie lie-down-anim 10 play-blend  3600 2 hewie send hewie-yelp  0 errand ! ;
: out-of-thicket ( -- )
    hewie hold  1 errand !  0 splashed !
    hewie before-thicket-tri 90 $204 walk-move 92.855e -13.871e go-to
    hewie -1 idle  hewie wait-anim  0 0 hewie send hewie-yelp  0 0 hewie-do
    thicket-out  0 errand ! ;

\ ---- the ambush: Daniella by the thicket, Hewie in it - he springs out at her (a scene) ----
: ambush-looks ( -- )  ambush-movie scenes:unseen-movie ambush-cutscene load-scene drop  4 cutscene-keep-model ;
: ambush ( -- )   \ (her springs through it - Room48's Cmd03 - not simulated)
    fiona hold  hewie hold  stalker hold  stalkers-stay state!  fiona true noclip
    15 screen:to-black-2 with-music or fade  no-track 1e background  ambush-looks  wait-fade
    force-calm state!  yield  force-calm state-off  0 true no-shadow
    panic-eased  -1 run-scene
    stalker let-go  3 summon-take  5 trust+  world-held state!
    fiona $1BB 32 112.81e 45.98e place-at  3 3 player-camera  0 false no-shadow
    yield  15 screen:from-black with-music or fade  yard-track 1e background  wait-fade
    ambush-file give-file  ambush-other-file give-file  ambush-seen resident-flag-set
    0 0 hewie send hewie-yelp  0 0 hewie-do  hewie $1DE -120 100.17e -23.46e place-at  hewie-no-dodge state-off  hewie let-go
    fiona false noclip  stalkers-stay state-off ;
: ambush? ( -- flag )   \ (Daniella up, the thicket, she and he on the screen; she free or half hidden, not panicking)
    in-panic? if  false exit  then
    fiona held? 0=  fiona-half-hidden state? or 0= if  false exit  then
    stalker-in?  partner-kind dup daniella-kind = swap kind34-kind = or and 0= if  false exit  then
    st-cond-now @ down-cond = if  false exit  then
    75e 7e -9e point-seen?  fiona unseen? 0= and  stalker unseen? 0= and ;

\ ---- her "go for it": Hewie fetches the glow at the back, the Antimony Tube across the wall ----
: hands-over ( -- )   \ (the tube, by her - not while she is busy - with his trust)
    fiona-half-hidden state?  fiona held? 0= or 0= if  exit  then
    antimony-tube item-full? 0= if  10 trust+  then  antimony-tube om1f-8-antimony-tube -1 takes-up ;
: fetches-glow ( -- )
    hewie hold  2 errand !  hewie-bark  hewie wait-move
    hewie $130 -120 -1 walk-path-move -83e -83e go-to  hewie true noclip
    hewie $133 180 $204 walk-move -111e -108e go-to  hewie 180 turn
    hewie sit-a-while-anim 10 play-blend
    glow-fx effect-off  0 glow-shown !  10 trust+
    prima-materia item-named  prima-materia 1 give-item  prima-materia show-item-tab  fanfare  obtained-2-msg say
    -1 fetched !  hewie 0 turn
    hewie $130 60 -1 walk-path-move -83e -83e go-to
    items-40-as-70 fact? if  creatures-on state!  5 0 s" creatures" actor-named send creature-count  then
    hewie false noclip  stalkers-stay state-off  0 errand ! ;
: gesture ( -- )   \ (her "go for it" at him)
    fiona go-for-it-gesture-anim start-anim  fiona 1 wait-mark  go-for-it-call common-bank fiona sound-by
    fiona wait-anim  fiona -1 idle  7 frames ;
: sends-for-glow ( -- )
    fiona hold  stalkers-stay state!  0 fetched !  ['] fetches-glow start drop  fiona wait-move
    gesture  begin  fetched @ 0= while  yield  repeat  32 frames  glow-file give-file ;
: fetches-tube ( -- )
    hewie hold  3 errand !  hewie-bark  hewie wait-move
    hewie $67 64 -1 walk-path-move 149e 91e go-to  hewie -84 turn
    hewie true noclip  $66 10 129.619e 1.7e 91.48e hewie-leap  hewie wait-move
    hewie sit-a-while-anim 10 play-blend
    om1f-8-antimony-tube fact? 0= if  hands-over  then
    hewie $1ED -90 -1 walk-path-move 43.7e 110.48e go-to  hewie 110 turn
    $1EE 5 76.12e 0.5e 104.76e hewie-leap  hewie wait-move
    hewie false noclip  stalkers-stay state-off  0 errand ! ;
: sends-for-tube ( -- )
    fiona hold  stalkers-stay state!  fiona wait-move
    hewie held? if  stalkers-stay state-off exit  then
    ['] fetches-tube start drop  gesture ;

\ ---- Hewie here: the thicket, the ambush, his errands, his barks and looks ----
: sent? ( F: x y z -- flag )   \ (her "go for it" at a point on the screen, both on it too, she facing it)
    {: F: x y z :}  fiona-cmd-now @ go-for-it-gesture <> if  false exit  then
    x y z point-seen?  fiona unseen? 0= and  hewie unseen? 0= and  fiona 50 x z faces-point? and
    hewie-breakable-now @ and ;
: errands ( -- )
    hewie-cond-now @ down-cond =  hewie held? or  chased? or if  exit  then
    glow-shown @  stalker-in? 0= and  -111e 17e -115e sent? and if  ['] sends-for-glow start drop exit  then
    om1f-8-antimony-tube fact? 0=  75e 7e 95e sent? and if  ['] sends-for-tube start drop  then ;
: thicket ( -- )
    hiding? if
        started-now @ come-back-cmd = if  ['] out-of-thicket start drop  then
        ambush? if  ['] ambush start drop  then
        exit
    then
    started-now @ go-there-cmd =  hewie-cond-now @ down-cond <> and 0= if  exit  then
    75e 7e -9e point-seen?  fiona unseen? 0= and  hewie unseen? 0= and  stalker-in? 0= and
    fiona 50 75e -9e faces-point? and  hewie-breakable-now @ and if  ['] into-thicket start drop  then ;
: glow-bark ( -- )  -111.73e 15e -115.37e hewie-barks-at ;
: tube-bark ( -- )  77.32e 0e 95.28e hewie-barks-at ;
create glow-look  -111.73e 15e -115.37e  60e 6e 0e look,
create tube-look  77.32e 0e 95.28e  60e 10e 8e look,
variable noticed   \ (notices?'s cell: one of hewie-noticed's bits)
: notices-once? ( bit F: x y z r h -- flag )
    {: bit F: x y z r h :}  hewie-noticed @ bit and 0<> noticed !
    noticed 100 x y z r h notices?  noticed @ if  hewie-noticed @ bit or hewie-noticed !  then ;
: barks-and-looks ( -- )
    glow-shown @ if
        1 -111.73e 15e -115.37e 60e 6e notices-once? if  ['] glow-bark start drop  then
        glow-look 1 hewie-looks-at
    then
    om1f-8-antimony-tube fact? 0= if
        2 77.32e 0e 95.28e 60e 10e notices-once? if  ['] tube-bark start drop  then
        tube-look 1 hewie-looks-at
    then ;
: brush ( k a n -- )   \ (a hanger: Hewie's step, squared, pushes it while it swings slowly; then a frame of its swing)
    {: k a n :}  a n placed-here? 0= if  exit  then
    hewie here?  a n swing-left object@ 5e f< and if  k a n rung creak-on creak-off  hewie step-of fdup f*  1e chime-push  then
    a n chime-swing ;
: hangers ( -- )   \ (as Hewie goes by them)
    hewie 75.003e -9.266e 14e centre-across? 0= if  exit  then
    2 0 do  i  first-hanger i + hanging  brush  loop ;
: hewie-here ( -- )
    errand @  hewie held? 0= and if  0 errand !  then   \ (an errand broken off)
    hewie-in? 0= if  exit  then
    hangers  thicket  errands  barks-and-looks ;

\ ---- the pond: a splash as someone steps in (each spot once a visit) ----
: splash-by ( bit F: x z -- )
    {: bit F: x z :}  splashed @ bit and if  exit  then
    fiona x 7e z 16e 4e steps-into? if  fiona 5 x 7e z splash-from  bit splashed +!  exit  then
    hewie x 7e z 16e 4e steps-into? if  hewie $A x 7e z splash-from  bit splashed +!  exit  then
    stalker x 7e z 16e 4e steps-into? if  stalker 5 x 7e z splash-from  bit splashed +!  then ;
: splashes ( -- )  1 80.25e -6e splash-by  2 67.4e -6.7e splash-by ;

\ ---- what she may do or say here ----
: grave ( k msg F: x z -- )   \ (its name; once three are read, one read again: all the same name)
    {: k msg F: x z :}  fiona wait-move  fiona x z turn-to-xz
    fiona k 1 = if  look-close-anim  else  look-low-anim  then  start-anim  40 frames
    graves-read @ 1 k lshift and  graves-count @ 2 < 0= and if  same-name-msg  else  msg  then  say
    graves-read @ 1 k lshift and 0= if  1 graves-count +!  then
    graves-read @ 1 k lshift or graves-read ! ;
: grave-1 ( -- )  0 1 -154e -28e grave ;
: grave-2 ( -- )  1 2 -86.29e -39.06e grave ;
: grave-3 ( -- )  2 3 40.56e -52.11e grave ;
: grave-4 ( -- )  3 4 77.34e 6.2e grave ;
: grave-5 ( -- )  4 5 127.66e -13.38e grave ;
: grave-6 ( -- )  5 6 144.96e 39.88e grave ;
: the-mausoleum ( -- )
    fiona $47 180 -1 walk-move 138e -98e go-to  fiona look-low-anim start-anim  40 frames
    mausoleum-read @ if  spirits-msg say  0 mausoleum-read !  exit  then
    mausoleum-msg say  15 screen:to-dim fade wait-fade  words-msg say  15 screen:from-dim fade wait-fade
    -1 mausoleum-read ! ;
: the-thicket ( -- )
    fiona wait-move  fiona 73.11e -9.81e turn-to-xz  fiona look-low-anim start-anim  40 frames
    hiding?  hewie body-tri thicket-tri = and if  eyes-msg  else  opening-msg  then  say ;
: the-glow ( -- )
    fiona wait-move  fiona -112.26e -123.08e turn-to-xz  fiona look-close-anim start-anim  40 frames  glow-msg say ;
: pick-jerky ( -- )  beef-jerky om1f-8-beef-jerky jerky-fx 122.09e 10.26e picks-up ;
: pick-medallion ( -- )  album-medallion om1f-8-album-medallion medallion-fx -140.37e -25.97e picks-up ;
: unlocks-it ( -- )  unlocks-back ;
: spots ( -- )
    1 before-locked?  fiona back-door-area in-area? and if  ['] unlocks-it any-time offer  then
    glow-shown @  fiona glow-area in-area? and  fiona -60 60 heading? and if  ['] the-glow unhurried offer  then
    om1f-8-beef-jerky fact? 0=  fiona 122.09e 8.28e 10.26e 5e 5e touches? and if  ['] pick-jerky calm-only offer  then
    om1f-8-album-medallion fact? 0=  fiona -140.37e 16e -25.97e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    fiona $18 in-area?  fiona -40 60 heading? and if  ['] grave-1 unhurried offer  then
    fiona $10 in-area?  fiona 0 60 heading? and if  ['] grave-2 unhurried offer  then
    fiona $11 in-area?  fiona -40 60 heading? and if  ['] grave-3 unhurried offer  then
    fiona $12 in-area?  fiona -60 60 heading? and if  ['] grave-4 unhurried offer  then
    fiona $13 in-area?  fiona 45 60 heading? and if  ['] grave-5 unhurried offer  then
    fiona $14 in-area?  fiona 62 60 heading? and if  ['] grave-6 unhurried offer  then
    fiona mausoleum-area in-area?  fiona 90 60 heading? and if  ['] the-mausoleum unhurried offer  then
    fiona thicket-area in-area?  fiona -30 60 heading? and if  ['] the-thicket unhurried offer  then
    fiona back-area in-area?  fiona 45 60 heading? and if  ['] the-back unhurried offer  then ;

\ ---- the movies replayed (the extras) ----
: replay-ambush ( -- )
    fiona hold  specks-about  daniella-kind stalker-ready  yield
    ambush-looks  0 true no-shadow  panic-eased  -1 run-scene
    0 false no-shadow  stalker-unloaded  replay-over ;
: replay-hand ( movie cutscene -- )
    {: movie cut :}  fiona hold  specks-about  true shadow-cast  hand-kind hand-slot figure-in  yield
    movie scenes:unseen-movie cut load-scene drop  hand-slot figure-on  panic-eased  -1 run-scene
    hand-slot figure-out  replay-over ;
: replay-rest ( -- )  rest-movie rest-cutscene replay-hand ;
: replay-fed ( -- )  fed-movie fed-cutscene replay-hand ;
: replayed ( -- )
    hewie send part-from-fiona
    replay-act case
        $20 of  ['] replay-ambush start drop  endof
        $21 of  ['] replay-rest start drop  endof
        $22 of  ['] replay-fed start drop  endof
    endcase ;

\ ---- every frame ----
create cameras  6 , 0 , 0 ,   7 , 1 , 1 ,   8 , 1 , 1 ,   9 , 3 , 3 ,   $A , 3 , 3 ,   $B , 5 , 5 ,   \ (area, set, path)
create ahead  $C , 0 ,   $D , 2 ,   $E , 5 ,   \ (area, exit loaded ahead)
: ways ( -- )
    0 exit-out  2 exit-out  5 5 leaves-by  cameras 6 cameras-at  ahead 3 ways-out
    fiona $C entered? if  hopper-kind hopper-slot figure-in  then
    fiona $C left? if  hopper-slot figure-out  then
    fiona map-area entered? if  2 map-page  then
    fiona map-area left? if  1 map-page  then ;
: each-frame ( -- )
    steady-on yard-sound -138e 8e 12e steady
    ways  hewie-here  splashes  spots
    stalker-leaving @  0 exit-held? 0= and if  sent-on  then ;
: person-in ( i -- )
    dup hewie-p = if  drop hewie-came exit  then
    fiona-p <> if  exit  then
    events-held state? if  replayed exit  then
    0 0 0 way-in  2 3 3 way-in  5 5 5 way-in
    came-by @ 0= if  2 map-page  then ;
: went ( -- )   \ (leaving: Hewie, held on an errand, let go where it would have left him; the stalker sent on)
    hewie held? if
        hewie let-go
        errand @ case
            1 of  hewie $1E7 90 94.1e -5.96e place-at  endof
            2 of  hewie $BC put-on-tri  stalkers-stay state-off  endof
            3 of  hewie $64 put-on-tri  stalkers-stay state-off  endof
        endcase
    then
    stalker-leaving @ if  sent-on  then ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-8 place!
' items-used old-mansion-1f-8 items-here
