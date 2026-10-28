\ story/places/castle-1f-2.fs - the courtyard of Belli Castle's 1F ($2): the gate's lever (pulled,
\ the cage rising, the way through), Hewie under the gate - sent through the gap, he fetches the
\ Fresh Flowers - and back; the hole in the wall (the voices: a scene, the Scrawled Memo); the
\ fireplace (its embers in the draught, the drum can struck, the doll inside in turn); a place to
\ hide (the stalker searching - finding her more likely each time he comes, or seeing her); in by
\ the gate's way (Debilitas knocked down beyond it, no going back); Debilitas shoving the rubble
\ down here and there; two vases (the Viola Medallion, the Onion), the Rubrum Medallion Hewie sniffs
\ out; the flowers and the fence in turn, the splashes in the pond, what Hewie barks at, stands wary
\ of and looks up at, the moths, the specks, the steady sounds, the walls' shade, the map's pages. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-2.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-2
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects keys story.kit fiona.anims fiona.sounds hewie.anims stalker.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant lever-msg        \ "Looks like this is used to open the gate. Pull the lever? YES NO"
1 constant pulled-msg       \ "The lever has already been pulled."
2 constant breeze-msg       \ "There's a breeze coming from the other side. I wonder where it leads..."
3 constant never-again-msg  \ "That is the last time I put my hand in there."
4 constant flowers-msg      \ "There are beautiful flowers blooming here."
5 constant fence-msg        \ "A section of the fence is broken. There is a hole..."
6 constant voices-msg       \ "Maybe I am hearing things, but I'd swear there are voices coming from the hole. Sea..."
7 constant no-way-back-msg  \ "There is no way I am going back there now!"
8 constant fire-msg         \ "A fire is crisply burning."
9 constant doll-msg         \ "Something's inside. It's a... ripped apart doll?!"

\ ---- its own state (a visit) ----
own: flowers-remarked  own: fire-remarked
own: bark-scene            \ Hewie sent under the gate: the stalkers held off till he is back
own: hewie-going           \ ... his way there under way
own: at-gap                \ ... sitting there beyond the gate (her command awaited)
own: fetch-step            \ the fetch's turns: 1 she is free, 2 he is back with it, 3 given
own: through-gate          \ she came in by the gate's way: no going back (till the stalker comes)
own: searched              \ the stalker has searched her hiding place (once a visit, till she hides anew)
own: search-on             \ his search begun this visit (the doors on his way opened only by them)
own: his-shove             \ Debilitas shoving the rubble by the hall (at the room's end he is put back)
own: his-shove-done        \ ... played (this time he came)
own: his-far-shove         \ Debilitas shoving the rubble by the fireplace (then gone)
own: drum-struck           \ someone at the drum can: struck once till she is no longer on guard
own: hum-playing           \ the machine's hum playing out (75 frames)
own: sounds-started  own: splash-1-made  own: splash-2-made
own: wary-noticed  own: pond-noticed  own: gate-noticed   \ (Hewie: once a visit each)

\ ---- the room's things ----
0 constant viola-fx   1 constant rubrum-fx   2 constant onion-fx   3 constant moths-fx   5 constant embers-fx
1 constant memo-object   2 constant lever-object   6 constant gate-open-part   5 constant flowers-part
0 constant gap-floor   3 constant gate-floor   4 constant cage-floor
4 constant her-hide-motions   5 constant his-look-motions   6 constant lever-motions   8 constant his-shove-motions
0 constant gate-sound   1 constant hum-sound   2 constant crank-sound   3 constant vase-sound   4 constant machine-sound
5 constant lever-sound   6 constant fire-sound   7 constant drum-sound   11 constant thud-sound
$C constant search-var   $14 constant gate-door   $B constant voices-kind   3 constant figure-slot
3 constant voices-movie   0 constant voices-cutscene   $24 constant voices-seen
$827B constant fetched-file   $22C constant through-file   $1C03 constant hand-over-anim   $204 constant crawl-anim
4 constant leaving-0-area   5 constant leaving-1-area   6 constant leaving-2-area   7 constant leaving-2b-area
8 constant leaving-3-area   $16 constant far-side-area   $11 constant hall-area   $13 constant gate-area
$14 constant gate-far-area   $17 constant hewie-gate-area   $19 constant pond-area   $1A constant far-rubble-area
$12 constant lever-area   $15 constant hole-area   $A constant fence-area   $18 constant hiding-area
: gate-open? ( -- flag )  c1f-2-flag1 fact? ;
: voices-due? ( -- flag )  got-study-key fact?  got-scrawled-memo fact? 0= and  c1f-12-flag1 fact? 0= and ;
: seated-ahead? ( -- flag )  found-file-20b fact?  c1f-10-flag1 fact? 0= and ;   \ (castle-1f-10's seated Debilitas)
: fire-at ( F: -- x y z )  276e 5e -146e ;
: hum-at ( F: -- x y z )  110e -10e 142e ;
c1f-2-breakable1-broken 1 2  215.5e -1e 2.6e 5e 8e  breakable: vase-1
vase-1 $80202020 0 $C shard-look  vase-1 0.562e 0.5e 0.25e 0.5e shard-uv
vase-1 1 frees-floor  vase-1 $377 noise-from  vase-1 vase-sound room-bank shatters-with
c1f-2-breakable2-broken 3 4  290.5e -1e -99.5e 5e 8e  breakable: vase-2
vase-2 $80202020 0 $C shard-look  vase-2 0.812e 0.687e 0.187e 0.312e shard-uv
vase-2 2 frees-floor  vase-2 $380 noise-from

\ ---- the cage on the gate ("ori"): up once the lever is pulled, else down ----
: cage ( -- addr len )  s" ori" ;
37.978e fconstant cage-top
: cage-set ( -- )  gate-open? if  cage-top  else  11e  then  cage place-y object! ;
: cage-by ( F: d -- )  cage place-y object+! ;
: cage-rises ( -- )   \ (the lever pulled: its hum, 64 frames up, its floor freed, on to the top; the crank, a jolt)
    hum-sound hum-at room-sound
    64 0 do  0.25e cage-by  yield  loop
    cage-floor solid-floor floor-off
    begin  cage place-y object@ cage-top f<  while  0.25e cage-by  yield  repeat  cage-top cage place-y object!
    crank-sound hum-at room-sound
    3 0 do  0.25e cage-by  4 frames  -0.25e cage-by  i 2 < if  4 frames  then  loop ;

\ ---- the drum can ("doramukan"): struck, it bobs - 0.2 x its strength x sin(phase), the strength
\ down 0.2 a frame, the phase on 90 degrees ----
: drum ( -- addr len )  s" doramukan" ;
$30 constant drum-phase   $34 constant drum-strength   $38 constant drum-rest
: drum-still ( -- )  0e drum drum-strength object!  0e drum drum-phase object!  drum place-y object@ drum drum-rest object! ;
: drum-struck! ( -- )  0e drum drum-phase object!  1e drum drum-strength object! ;
: drum-bobs ( -- )
    drum placed-here? 0= if  exit  then
    -0.2e drum drum-strength object+!
    drum drum-strength object@ f0> 0= if
        0e drum drum-strength object!  0e drum drum-phase object!  drum drum-rest object@ drum place-y object!  exit
    then
    90e drum drum-phase object+!
    drum drum-phase object@ 360e f< 0= if  -360e drum drum-phase object+!  then
    drum drum-strength object@  drum drum-phase object@ deg>rad fsin f*  0.2e f*  drum drum-rest object@ f+  drum place-y object! ;

\ ---- the fireplace's embers: lit, blown back in a draught, a lot flung away ----
: embers-lit ( -- )  -1 effects:embers embers-fx effect! ;
: embers-gust ( -- )  embers-fx live-effect if  0 swap send effect-command  else  drop  then ;
: embers-flung ( -- )  1 effects:embers drop ;

\ ---- as she comes in ----
: specks-about ( -- )
    8 $B $80 $80 $80 $30 rgba 229.9e 29e -63.9e specks   $A $B $80 $80 $80 $30 rgba 111.4e 25e -58.5e specks
    $E $E $80 $80 $80 $40 rgba 122.8e 19e 103.2e specks   $C $10 $80 $80 $80 $40 rgba 63.3e 19e 151.6e specks
    8 $C $80 $80 $80 $40 rgba 34.2e 19e 92.1e specks ;
create holes  $38B , $1B9 , $38C , $38D , $386 , $38E ,   \ (things lying about fall through)
: came-in ( exit -- )
    drop  room-sounds  0 show-part  5 show-part  gate-floor solid-floor keeps-fiona-off or keeps-pursuers-off or floor-on
    cage-set
    gate-open? if  lever-object object-hide  gate-open-part show-part
    else  cage-floor solid-floor floor-on  lever-object object-show  gate-open-part hide-part  then
    vase-1 as-it-stands  vase-1 broken?  c1f-2-viola-medallion fact? 0= and if  viola-fx 215.5e 0e 2.6e flicker-at  then
    vase-2 as-it-stands  vase-2 broken?  got-onion fact? 0= and if  onion-fx 290.5e 0e -99.5e flicker-at  then
    c1f-2-rubrum-medallion-found fact?  c1f-2-rubrum-medallion fact? 0= and if  rubrum-fx 292.2e 0e -61.8e flicker-at  then
    embers-lit  moths-fx 1 4e -4e 80e moths-at  drum-still  specks-about
    6 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the steady sounds: the fire's begun once the room's sounds are in; the cage's kept up ----
: sounds ( -- )
    sounds-started @ 0= if  room-sounds-in? if  fire-sound fire-at room-sound  -1 sounds-started !  then  exit  then
    hum-sound hum-at keep-sound  fire-sound fire-at keep-sound ;
: hum-plays ( -- )  -1 hum-playing !  machine-sound -142e 12e 179e room-sound  75 frames  0 hum-playing ! ;

\ ---- the lever: the gate opened (the cage rising; the door beyond unlocked; the creatures out) ----
: pulls-it ( -- )
    lever-object 1 0 object  fiona room-motion 10 start-blend  32 frames
    lever-sound 50e -5e 171e room-sound  62 frames  gate-sound 50e -5e 171e room-sound  fiona wait-anim
    gate-door unlock  fiona -1 idle  fiona wait-anim
    c1f-2-flag1 fact!  $5F $1F9 room-noise  ['] cage-rises start drop
    got-saturnus-key fact? 0= if  castle-1f-1 0 443 4 0 creature-in  castle-1f-9 0 217 4 0 creature-in  then ;
: lever ( -- )
    gate-open? if  fiona wait-move  pulled-msg say exit  then
    stalkers-stay state!  fiona wait-move  fiona lever-motions motions-for
    fiona $8C 0 -1 walk-move 49.9e 164e go-to  1 20e 20e 0e 0e shot
    lever-msg ask  fiona use-motions  begin  room-sounds-in? 0= while  yield  repeat  16 frames  shot-off
    0= if  pulls-it  then
    bark-scene @ 0= if  stalkers-stay state-off  then ;

\ ---- Hewie under the gate: sent to the gap he crawls through and sits; sent on, he fetches the
\ Fresh Flowers back to her; called, he crawls back ----
: hewie-done ( -- )  0 hewie-going !  0 bark-scene !  stalkers-stay state-off ;
: crawls-back ( -- )
    hewie hold  hewie $353 90 -1 walk-path-move 37e 69e go-to  gap-floor solid-floor floor-off
    hewie $267 90 crawl-anim walk-move 52e 69e go-to  gate-floor solid-floor floor-on
    hewie -1 idle  hewie let-go  room-flag-2d state-off  0 bark-scene ! ;
: her-flowers ( -- )   \ (once she is free: he gives them over)
    begin  fiona-free? 0= while  yield  repeat
    1 fetch-step !  panic-eased
    fiona hold  begin  fetch-step @ 2 < while  yield  repeat
    fresh-flowers spend  withered-flowers spend  10 trust+
    fresh-flowers item-named  fresh-flowers 1 give-item  fresh-flowers show-item-tab  fanfare  obtained-2-msg say
    fetched-file give-file  3 fetch-step ! ;
: fetches ( -- )
    hewie hold  0 fetch-step !  ['] her-flowers start drop
    begin  fetch-step @ 1 < while  yield  repeat
    hewie-bark  hewie wait-move  hewie run-move $1DF -1  $1DF tri-center 0e  move  hewie wait-move
    hewie hand-over-anim play  2 fetch-step !
    begin  fetch-step @ 3 < while  yield  repeat  hewie let-go ;
: sits-at-gap ( -- )   \ (barking, to the gap, through it; sitting there till she sends him on or calls him)
    stalkers-stay state!  -1 bark-scene !  -1 hewie-going !
    hewie hold  hewie-bark  hewie wait-move  hewie $267 -90 -1 walk-path-move 52e 69e go-to
    room-flag-2d state!  0 hewie-going !
    gate-floor solid-floor floor-off  hewie $353 -90 crawl-anim walk-move 40e 69e go-to
    gap-floor solid-floor floor-on  hewie sit-down-anim play  hewie sit-anim play
    -1 at-gap !  begin  at-gap @ while  yield  repeat ;
: sent? ( -- flag )  started-now @ dup go-there-cmd = swap go-for-it-cmd = or ;
: called? ( -- flag )  started-now @ come-back-cmd = ;
: from-the-gap ( -- )   \ (her commands as he sits there)
    at-gap @ 0= if  exit  then
    fiona gate-area in-area? if
        called? if  0 at-gap !  ['] crawls-back start drop exit  then
        sent?  hewie-breakable-now @ and if  0 at-gap !  ['] fetches start drop exit  then
    then
    fiona gate-far-area in-area?  sent? called? or and if  0 at-gap !  ['] fetches start drop  then ;
: under-the-gate ( -- )
    from-the-gap
    sent?  hewie-cond-now @ down-cond <> and  stalker-in? 0= and
    fiona gate-area in-area? and  hewie hewie-gate-area in-area? and  fiona 50 45e 69e faces-point? and  bark-scene @ 0= and if
        hewie-breakable-now @ if  ['] sits-at-gap start drop  then
    then
    called?  fiona gate-area in-area? and  hewie 3 on-group? and  at-gap @ 0= and  bark-scene @ and if  ['] crawls-back start drop  then
    hewie-going @  hewie held? 0= and if  hewie-done  then ;

\ ---- the hole in the wall: the voices - her hand in it (a scene: the Scrawled Memo) ----
: voices-shots ( shot -- )   \ (her shadow off from the second shot to the eighth)
    dup 2 = if  0 true no-shadow  then  8 = if  0 false no-shadow  then ;
: voices-scene ( movie? -- )
    ['] voices-shots at-shots  voices-kind true no-shadow  0.9e near-clip
    panic-eased  if  -1 run-scene  then  1e near-clip ;
: hand-in ( -- )
    15 screen:to-black-2 with-music or fade  voices-kind figure-slot figure-in  yield  figure-slot figure-on  fiona wait-move
    voices-movie scenes:unseen-movie voices-cutscene load-scene  wait-fade
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie hold  hewie false shown  then
    voices-scene
    fiona 0 idle  fiona $EE -2 -141.823e 160.482e place-at  camera-restart
    0 false no-shadow  voices-kind false no-shadow  hewie true shown  hewie let-go
    figure-slot figure-out  memo-object object-hide  80 fright-up-to
    15 screen:from-black with-music or fade  wait-fade
    got-scrawled-memo fact!  voices-seen resident-flag-set
    scrawled-memo item-named  scrawled-memo 1 give-item  scrawled-memo show-item-tab  fanfare  obtained-2-msg say ;
: the-hole ( -- )
    voices-due? if
        stalkers-stay state!  fiona wait-move  voices-msg ask 0= if  hand-in  then
        bark-scene @ 0= if  stalkers-stay state-off  then  exit
    then
    fiona wait-move
    got-scrawled-memo fact? 0= if  breeze-msg say exit  then
    fiona hands-clasped-anim 10 start-blend  never-again-msg say  fiona wait-anim ;

\ ---- the hiding place: in, lying low, calming; out by the button - or found, and hauled out ----
: hauled ( -- )   \ (found, in a panic, or him in: out, the stalker let go unless he found her or has a scene)
    found-her @ if  2 avoid-prompt  then
    in-panic? 0= if  seized-sound common-bank fiona sound-by  then
    fiona follow  fiona-hidden state-off  fiona true noclip  slide-sound common-bank fiona sound-by
    fiona 3 hid-motion 10 play-blend  fiona false noclip
    stalker held?  his-shove @ 0= and  his-far-shove @ 0= and  found-her @ 0= and if  stalker let-go  0 stalker-order  then ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  0 searched !
    fiona her-hide-motions motions-for  fiona wait-move  fiona -1 look-at  yield  fiona use-motions
    fiona walk-anim-move 4 hid-motion 5  $132 240e -17.59e floor-at  0 deg>yaw  move  fiona wait-move
    fiona true noclip  slide-sound common-bank fiona sound-by  fiona 5 hid-motion 10 start-blend8  fiona wait-anim
    fiona 1 hid-motion 10 start-blend  fiona false noclip
    bark-scene @ 0= if  stalkers-stay state-off  then
    fiona-hidden state!  0 found-her !  0 avoid-prompt
    begin
        circle button-pressed? if
            stalker-in? if  hauled exit  then
            fiona follow  fiona-hidden state-off  fiona true noclip
            fiona 2 hid-motion 5 start-blend8  5 frames  slide-sound common-bank fiona sound-by  fiona wait-anim
            fiona false noclip  exit
        then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound common-bank fiona sound-by  then  hauled exit  then
        hidden-calm  yield
    again ;

\ ---- the stalker while she hides: his search (seeing her, he finds her) ----
: his-camera? ( -- flag )   \ (him about, on the move his way - the camera on someone: hers, else his)
    stalker-told?  st-mode-now @ dup 0= over 1 = or swap 5 = or 0= and  cam-followed 0< 0= and ;
: door-on-his-way ( exit -- )   \ (come by it, its door shut: opened - in his search, only from its area)
    {: ex :}  ex stalker-came-by?  ex exit-open? 0= and  search-on @ 0=  stalker ex exit-area in-area? or and if
        stalker ex open-door
    then ;
: finds-her ( -- )
    stalker hold  stalker wait-move  0 door-on-his-way  1 door-on-his-way  0 search-on !
    debilitas? if  stalker his-look-motions motions-for  then
    $132 0 237.262e -39.055e finds-her-at ;
: search-odds ( -- pct )  search-var var@ case  0 of 25 endof  1 of 50 endof  2 of 75 endof  3 of 75 endof  >r 0 r>  endcase ;
: searches-for-her ( -- )
    -1 searched !  -1 came-while-hidden !
    search-odds chance?  stalker unseen? 0= or  2 creature-moving? or  4 stalker-alert? 0= and
    his-camera? 0= if  stalker follow  then
    if  ['] finds-her start drop  else  120 summoner-sooner  0 search-on !  then
    0 stalker-alert? if  2 stalker-order  then
    search-var var@ 1+ search-var var! ;
: search-due ( -- )   \ (him in, not down, her hidden, unsearched: his search)
    st-cond-now @ down-cond =  fiona-hidden state? 0= or  searched @ or  stalker-in? 0= or  stalker held? or if  exit  then
    -1 search-on !  searches-for-her ;

\ ---- Debilitas's shoves: the rubble by the hall (from the hall's way, she down on the floor), and
\ the far rubble (on guard there) - dust, a thud, her thrown if she is by him ----
: shoves ( motion frame F: x0 y0 z0 x1 y1 z1 -- )   \ (the dust thrown up at its frame at the two points)
    {: m at F: x0 y0 z0 x1 y1 z1 :}  stalker use-motions  stalker true noclip  stalker room-motion m + 5 start-blend8
    0  begin
        stalker fiona 0 close-to? if  stalker throws-her  then
        dup at = if  x0 y0 z0 dust  x1 y1 z1 dust  thud-sound room-bank stalker sound-by  then
        1+  stalker went-round? 0= while  yield
    repeat  drop ;
: shoves-by-hall ( -- )
    stalker hold  -1 his-shove !  stalker wait-move  stalker his-shove-motions motions-for
    stalker $11 180 151.535e -15.151e walk-to
    0 49 153e -3.7e -51e 148e -4e -51e shoves
    stalker 0 0 camera-set  stalker stand-anim 5 play-blend  stalker false noclip
    0 his-shove !  0 searched !  -1 his-shove-done ! ;
: shoves-far ( -- )
    stalker hold  -1 his-far-shove !  stalker wait-move  stalker his-shove-motions motions-for
    stalker $381 90 -1 walk-path-move 279.332e -83.7e go-to
    1 54 306e 23e -79e 306e 23e -85e shoves
    stalker $395 90 -1 walk-path-move 341.992e -29.2e go-to  stalker false noclip
    0 his-far-shove !  stalker let-go  0 summon-take ;
: his-kind? ( -- flag )  debilitas? ;
: shove-due ( -- )
    st-cond-now @ down-cond = if  exit  then
    stalker leaving-1-area in-area?  fiona body-pos fdrop f0> 0= fdrop and  his-shove-done @ 0= and  his-kind? and if
        0 stalker-alert?  2 stalker-alert? or if
            stalker held?  ['] finds-her running? and  fiona-hidden state? and  his-shove @ 0= and if  stalker let-go  then
            stalker held? 0= if  ['] shoves-by-hall start drop  then
        then
    then
    stalker far-rubble-area in-area?  4 stalker-alert? and  his-kind? and  stalker held? 0= and if  ['] shoves-far start drop  then ;

\ ---- in by the gate's way: Debilitas knocked down beyond it (a file); Hewie with her, by the gate ----
: through-the-gate ( -- )
    fiona hold  force-followed state!  16 frames  force-followed state-off
    $28 common-bank fiona sound-by  world-held state-off
    partner dup 0< 0= if  dup >r castle-1f-10 129 r> send stalker-in  send stalker-knock-out  else  drop  then
    15 screen:from-black with-music or fade  wait-fade  through-file give-file ;

\ ---- what she may do or say here ----
: fence ( -- )   \ (in turn: the flowers, the fence's hole)
    fiona wait-move
    flowers-remarked @ if  fiona look-low-anim 10 start-blend  fence-msg  else  -1 flowers-remarked !  fiona look-close-anim 10 start-blend  flowers-msg  then
    say  fiona wait-anim ;
: flowers ( -- )
    fiona $7D 180 -1 walk-move 6.874e 90e go-to  world-frozen state!  1 10e 20e 0e 0e shot
    flowers-part hide-part  flowers-msg say  16 frames  world-frozen state-off  shot-off  flowers-part show-part ;
: fireplace ( -- )   \ (in turn: the fire, the doll inside)
    fiona wait-move  fiona 276.5e -145.9e turn-to-xz
    fire-remarked @ if  doll-msg  else  -1 fire-remarked !  fire-msg  then  say ;
: pick-rubrum ( -- )  rubrum-medallion c1f-2-rubrum-medallion rubrum-fx 292.2e -61.8e picks-up ;
: pick-viola ( -- )  viola-medallion c1f-2-viola-medallion viola-fx 215.5e 2.6e picks-up ;
: pick-onion ( -- )  onion got-onion onion-fx 290.5e -99.5e picks-up ;
: spots ( -- )
    fiona hiding-area in-area?  fiona 0 60 heading? and if
        stalker-in? 0=  stalker unseen? or if  ['] hides calm-only offer  else  no-time-message offer-remark-now  then
    then
    fiona lever-area in-area?  fiona 0 50 heading? and if  ['] lever unhurried offer  then
    fiona hole-area in-area?  fiona 0 50 heading? and if  ['] the-hole unhurried offer  then
    fiona fence-area in-area?  fiona -90 50 heading? and if  ['] fence unhurried offer  then
    fiona gate-far-area in-area?  fiona 180 50 heading? and if  ['] flowers unhurried offer  then
    fiona 276.5e -145.9e 7e reaches-across?  fiona 50 276e -146e faces-point? and if  ['] fireplace unhurried offer  then
    0 exit-open? 0=  through-gate @ and  fiona 0 before-door? and if  no-way-back-msg offer-remark-now  then
    c1f-2-rubrum-medallion-found fact?  c1f-2-rubrum-medallion fact? 0= and  fiona 292.2e 0e -61.8e 5e 5e touches? and if
        ['] pick-rubrum calm-only offer  then
    vase-1 broken? if
        c1f-2-viola-medallion fact? 0=  fiona 215.5e 0e 2.6e 5e 5e touches? and if  ['] pick-viola calm-only offer  then
    else  vase-1 breaks? if  viola-fx 215.5e 0e 2.6e flicker-at  then  then
    vase-2 broken? if
        got-onion fact? 0=  fiona 290.5e 0e -99.5e 5e 5e touches? and if  ['] pick-onion calm-only offer  then
    else  vase-2 breaks? if  onion-fx 290.5e 0e -99.5e flicker-at  then  then ;

\ ---- the fireplace and the drum can: a step into the draught blows the embers back; someone at
\ the drum strikes it (its sound, the embers flung) - once till she is no longer on guard ----
: by-the-fire ( -- )
    fiona 276.5e -1e -145.9e 14e 4e steps-into?  stalker 276.5e -1e -145.9e 14e 4e steps-into? or if  embers-gust  then
    drum-bobs
    drum-struck @ 0= if
        276.4e -1e -145.7e 5e 10e struck-in? if   \ (a blow there)
            -1 drum-struck !  drum-sound fire-at room-sound  drum-struck!  embers-gust  embers-flung embers-flung embers-flung
        then
    else  fiona-mode-now @ defend-mode <> if  0 drum-struck !  then  then ;

\ ---- the pond: a splash as someone first steps in at each end (once a visit) ----
: splashes ( -- )
    splash-1-made @ 0= if
        fiona 28e -9e 72e 16e 4e steps-into? if  fiona 5 28e -9e 72e splash-from  -1 splash-1-made !  then
        hewie 28e -9e 72e 16e 4e steps-into? if  hewie 10 28e -9e 72e splash-from  -1 splash-1-made !  then
        stalker 28e -9e 72e 16e 4e steps-into? if  stalker 5 28e -9e 72e splash-from  -1 splash-1-made !  then
    then
    splash-2-made @ 0= if
        fiona 8e -9e 70e 16e 4e steps-into? if  fiona 5 8e -9e 70e splash-from  -1 splash-2-made !  then
        hewie 8e -9e 70e 16e 4e steps-into? if  hewie 10 8e -9e 70e splash-from  -1 splash-2-made !  then
        stalker 8e -9e 70e 16e 4e steps-into? if  stalker 5 8e -9e 70e splash-from  -1 splash-2-made !  then
    then ;

\ ---- Hewie: wary by the far wall (the voices due), a bark at the pond and at the closed gate (once
\ a visit each), his look up at the fireplace, the Rubrum Medallion he sniffs out ----
create looks  274.93e -1e -145.73e  20e 11e 11e look,
: wary-by-wall ( -- )  -142.16e 0e 179e hewie-stands-by ;
: barks-at-pond ( -- )  5.346e -9.5e 80.917e hewie-barks-at ;
: barks-at-gate ( -- )  50e -10e 164.549e hewie-barks-at ;
: at-his-command? ( -- flag )  hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and ;
: hewie-notices ( -- )
    at-his-command? 0= if  exit  then
    voices-due?  wary-noticed @ 0= and  hewie -142.16e 0e 179e 75e 20e centred-in? and if
        -1 wary-noticed !  ['] wary-by-wall start drop
    then
    bark-scene @ 0=  hewie pond-area in-area? and  pond-noticed @ 0= and  hewie 5.346e -9.5e 80.917e 40e 20e centred-in? and if
        -1 pond-noticed !  30 chance? if  ['] barks-at-pond start drop  then
    then
    gate-open? 0=  gate-noticed @ 0= and  hewie 50e -10e 164.549e 30e 20e centred-in? and if
        -1 gate-noticed !  ['] barks-at-gate start drop
    then ;
: rubrum-sniffed ( -- )  c1f-2-rubrum-medallion-found c1f-2-rubrum-medallion rubrum-fx  292.2e 0e -61.8e hewie-finds ;
: hewie-sniffs ( -- )
    c1f-2-rubrum-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie 292.2e -1e -61.8e 10e 5e touches? 0= if  exit  then
    ['] rubrum-sniffed start-only ;

\ ---- the walls' shade ----
: walls-shade ( -- )
    263.8e 8.9e -109.2e  263.8e 8.9e -145.2e  259.799e -39.1e -119.2e  258.799e -39.1e -147.2e  light-doorway
    264.8e 9.1e -68.9e  264.8e 9.1e -104.9e  261.799e -30.9e -79.9e  261.799e -30.9e -90.3e  light-doorway
    210e 8e -71e  267e 8e -71e  210e -40e -75e  266e -40e -75e  light-doorway
    153e 10e -71e  211e 10e -71e  153e -38e -75e  210e -38e -75e  light-doorway
    60e 5e 57e  90e 5e 57e  60e -40e 53e  89e -39e 53e  light-doorway
    89e 8e 58e  99e 5e 46e  91.3e -40e 53e  99e -40e 42e  light-doorway
    -162e 25.5e 122.8e  -162e 25.5e 160.5e  -162e -10e 123e  -162e -10e 160e  light-doorway ;

\ ---- the movie replayed (the extras): the voices ----
: replayed ( -- )
    fiona hold  voices-kind figure-slot figure-in  yield
    voices-movie scenes:unseen-movie voices-cutscene load-scene  voices-scene
    0 false no-shadow  figure-slot figure-out  replay-over ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    5 , 1 , 1 ,   $B , 0 , 0 ,   $C , 0 , 0 ,   $D , 3 , 3 ,   $E , 0 , 0 ,   $F , 2 , 2 ,   $10 , 3 , 3 ,
    $11 , 4 , -1 ,   $16 , 5 , 4 ,
create entries  0 , 0 ,   5 , 4 ,   0 , 0 ,   3 , 3 ,   \ (by exit 0..3: the camera)
: seated-figure ( -- )   \ (castle-1f-10's seated Debilitas, loaded ahead on the way there)
    debilitas-kind figure-slot figure-in ;
: ways ( -- )
    seated-ahead?  0 exit-open? and  fiona 0 exit-area in-area? and  figure-slot figure 0< and if  seated-figure  then
    2 exits-out  2 2 leaves-by  3 3 leaves-by  cameras 9 cameras-at
    fiona leaving-0-area entered? if  seated-ahead? if  seated-figure  then  0 prepare-exit  then
    fiona leaving-1-area entered?  fiona leaving-2-area entered? or  seated-ahead? and if  figure-slot figure-out  then
    fiona leaving-1-area entered? if  1 prepare-exit  2 map-page  then
    fiona leaving-2-area entered?  fiona leaving-2b-area entered? or if  2 prepare-exit  then
    fiona leaving-3-area entered? if  3 prepare-exit  then
    fiona far-side-area entered? if  3 map-page  then
    voices-due?  hum-playing @ 0= and if
        fiona leaving-2b-area left?  fiona leaving-3-area left? or  fiona hall-area left? or if  ['] hum-plays start drop  then
    then
    through-gate @  stalker-in? and if  0 through-gate !  then ;
: each-frame ( -- )
    sounds  events-held state? if  exit  then
    ways  hewie-sniffs  by-the-fire  search-due  shove-due  hewie-notices  looks 1 hewie-looks-at  splashes
    fiona-hidden state? if  his-camera? if  fiona follow  else  stalker follow  then  then
    under-the-gate  spots  walls-shade ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
            entries 4 ways-in
            $81 forced-way? if
                -1 through-gate !  fiona $324 180 place-on-tri  fiona 0 at-exit  0 0 player-camera  camera-restart
                ['] through-the-gate start drop
                came-by @ forced-exit = if  hewie activate  castle-1f-2 691 0 hewie-to  hewie $2B3 50 place-on-tri  then
            then
            saturnus-hint-on fact?  c1f-14-flag2 fact? 0= and if  c1f-14-flag2 fact!  stalker let-go  0 summon-take  then
            came-by @ 1 = if  3 map-page  then  endof
        stalker-p of
            0 his-shove-done !
            came-on-her? if  searches-for-her  then  endof
    endcase ;
: went ( -- )
    room-flag-2d state-off
    hewie-in?  hewie 3 on-group? and if  hewie $101 put-on-tri  stalkers-stay state-off  then
    bark-scene @ if  stalkers-stay state-off  then
    through-gate @  stalker-in? 0= and  st-cond-now @ down-cond = and if  stalker let-go  2 summon-take  then
    stalker held? if
        his-shove @ if  stalker let-go  stalker $192 180 place-on-tri  0 his-shove !
        else  his-far-shove @ if  stalker let-go  0 summon-take  then  then
    then ;

' came-in ' person-in ' each-frame ' went castle-1f-2 place!
