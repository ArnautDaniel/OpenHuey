\ castle-1f-2's place (scripts/story/places/castle-1f-2.fs): the courtyard - coming in (the gate shut
\ or open: the cage, the lever), the remarks (the flowers and the fence in turn, the flowers by the
\ gate, the fire and the doll in turn, the hole), the lever pulled (the cage rising), in by the
\ gate's way (no going back), the vases (the Viola Medallion, the Onion), the Rubrum Medallion
\ (sniffed out; picked up), the drum can struck, a splash, Hewie's barks and his wary stand, Hewie
\ under the gate (back; the Fresh Flowers fetched), the hiding place (in, out; found), the stalker
\ searching, Debilitas's shoves, the hole's voices (the Scrawled Memo), the machine's hum, the map's
\ page, the replay. Headless:
\   build/openhuey --test tests/places/test_castle_1f_2.fs
IN: test-castle-1f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-1f-2 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-1f-2 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: hewie-here ( F: x y z -- )  {: F: x y z :}  castle-1f-2 x y z nav-tri 0 game:hewie send hewie-place  30 run-frames  x y z hewie-spot ;
: map-page@ ( -- page )  game:subscreen enter  subscreen.state:her-pg @  leave-actor ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: struck ( xt F: x y z -- )  blow-at  in-place  [: body-point-off ;] as-her  10 run-frames ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: cage-y ( F: -- y )  s" ori" place-y object@ ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: started ( cmd -- )  [: started-now ! ;] in-place ;   \ (her command to Hewie, this frame - as she gives it)
: replays ( v -- )   \ (the extras' theatre: that movie, here)
    $2A var!  events-held state-flag-set  castle-1f-2 -1 rooms send go-to-room  5 run-frames ;
: replayed? ( -- flag )
    [: cutscene-active? movie-status 1 = or ;] 200 soon
    [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon and ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in, the gate shut: the cage down, the lever up; the vases whole; the embers, the moths
c1f-2-flag1 story-flag-clear  c1f-2-breakable1-broken story-flag-clear  c1f-2-breakable2-broken story-flag-clear  room-afresh
T{ cage-y 11e f-  fabs 0.01e f<  gate-open-part part?  1 part?  3 part?  [: embers-fx effect @ 0<>  moths-fx effect @ 0<> ;] in-place
   -> -1 0 -1 -1 -1 -1 }T
testing ... the gate open: the cage up
c1f-2-flag1 story-flag-set  room-afresh
T{ cage-y cage-top f-  fabs 0.01e f<  gate-open-part part? -> -1 -1 }T

testing the cage set low, raised 0.25 a step; the drum can struck, wobbling back to rest (6 frames)
c1f-2-flag1 story-flag-clear  room-afresh
T{ [: cage-set  cage place-y object@ 11e ~=  0.25e cage-by  cage place-y object@ 11.25e ~= ;] in-place -> -1 -1 }T
: wobbles ( -- n )  [: drum-still  drum-struck!  0  begin  1+  drum-bobs  drum drum-strength object@ f0=  over 50 > or  until ;] in-place ;
T{ wobbles  [: drum place-y object@ drum drum-rest object@ ~= ;] in-place -> 6 -1 }T   \ (1 less 0.2 five times: just above 0 in single floats)
c1f-2-flag1 story-flag-set  room-afresh

testing the remarks: the flowers and the fence in turn, the flowers by the gate, the fire in turn, the lever pulled
T{ fence-area -45 at  said  fence-area -45 at  said -> flowers-msg fence-msg }T
T{ gate-far-area 90 at  said -> flowers-msg }T
T{ 180 276.5e 0e -137e put  said  180 276.5e 0e -137e put  said -> fire-msg doll-msg }T
T{ lever-area 0 at  said -> pulled-msg }T
testing ... the hole: the breeze; once her hand has been in, never again
got-study-key story-flag-clear  got-scrawled-memo story-flag-clear
T{ hole-area 0 at  said -> breeze-msg }T
got-scrawled-memo story-flag-set
T{ hole-area 0 at  said -> never-again-msg }T
got-scrawled-memo story-flag-clear

testing the lever pulled (yes): the gate door unlocked, the cage rising to the top
c1f-2-flag1 story-flag-clear  gate-door s" doors" actor-named send lock  got-saturnus-key story-flag-set  room-afresh
lever-area 0 at  circle press  [: window-shown lever-msg = ;] 300 soon drop
[: window-shown 0< 0= if  cross press  then  c1f-2-flag1 story-flag? ;] 600 soon drop
T{ c1f-2-flag1 story-flag?  gate-door doors:door-locked? -> -1 0 }T
T{ [: cage-y cage-top f- fabs 0.01e f< ;] 900 soon -> -1 }T
200 run-frames

testing the vases broken: the Viola Medallion and the Onion showing; picked up
c1f-2-viola-medallion story-flag-clear  got-onion story-flag-clear  room-afresh
' spots 215.5e 1e 2.6e struck  ' spots 290.5e 1e -99.5e struck
T{ c1f-2-breakable1-broken story-flag?  c1f-2-breakable2-broken story-flag?  [: viola-fx effect @ 0<>  onion-fx effect @ 0<> ;] in-place -> -1 -1 -1 -1 }T
viola-medallion items:items-of before !
215.5e 0e 2.6e picked
T{ c1f-2-viola-medallion story-flag?  viola-medallion items:items-of -> -1 before @ 1+ }T
onion items:items-of before !
290.5e 0e -99.5e picked
T{ got-onion story-flag?  onion items:items-of -> -1 before @ 1+ }T

testing the drum can struck (a blow at it): once, till she is no longer on guard
room-afresh  276.4e 1e -145.7e blow-at
T{ [: by-the-fire drum-struck @ ;] in-place  [: body-point-off ;] as-her  2 run-frames  [: drum-struck @ ;] in-place -> -1 0 }T   \ (not on guard: let go)

testing a splash in the pond (once a visit)
room-afresh  0 28e 0e 60e put  0 28e 0e 72e put
T{ [: splash-1-made @ ;] in-place -> -1 }T

testing the Rubrum Medallion Hewie sniffs out: found, it shows; picked up
c1f-2-rubrum-medallion-found story-flag-clear  c1f-2-rubrum-medallion story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  292.2e -1e -56e hewie-here
[: ['] rubrum-sniffed start drop ;] in-place
T{ [: c1f-2-rubrum-medallion-found story-flag? ;] 600 soon  [: rubrum-fx effect @ 0<> ;] in-place -> -1 -1 }T
200 read-on
rubrum-medallion items:items-of before !
292.2e -1e -61.8e picked
T{ c1f-2-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> -1 before @ 1+ }T

testing Hewie: a bark at the shut gate, at the pond; wary by the far wall (the voices due)
c1f-2-flag1 story-flag-clear  room-afresh  50e -10e 160e hewie-here
T{ [: [: gate-noticed @ ;] in-place ;] 60 soon -> -1 }T
200 run-frames  5.346e -9.5e 80.917e hewie-spot
T{ [: [: pond-noticed @ ;] in-place ;] 60 soon -> -1 }T
200 run-frames  got-study-key story-flag-set  got-scrawled-memo story-flag-clear  c1f-12-flag1 story-flag-clear
-142e 0e 175e hewie-spot
T{ [: [: wary-noticed @ ;] in-place ;] 60 soon -> -1 }T
300 run-frames

testing Hewie under the gate: sent to the gap, through it, sitting; called back
c1f-2-flag1 story-flag-clear  got-study-key story-flag-clear  room-afresh  60e -10e 110e hewie-here
-90 65e -10e 70e put  go-there-cmd started
T{ [: [: bark-scene @ ;] in-place ;] 30 soon -> -1 }T
T{ [: room-flag-2d state-flag? ;] 600 soon -> -1 }T
300 run-frames  come-back-cmd started
T{ [: [: bark-scene @ 0= ;] in-place ;] 900 soon -> -1 }T
testing ... sent on: he fetches the Fresh Flowers back to her
withered-flowers 1 items send give-item  10 run-frames  fresh-flowers items:items-of before !
room-afresh  60e -10e 110e hewie-here  -90 65e -10e 70e put  go-there-cmd started
T{ [: room-flag-2d state-flag? ;] 600 soon -> -1 }T
300 run-frames  gate-far-area 90 at  go-there-cmd started   \ (from by the flowers: sent on, whatever he is doing)
T{ [: window-shown 0< 0= if  cross press  then  fresh-flowers items:items-of before @ > ;] 1500 soon  withered-flowers items:items-of -> -1 0 }T
300 read-on  game:hewie send part-from-fiona

testing the hiding place: in (hidden), out by the button
room-afresh  hiding-area 0 at  circle press  [: hidden? ;] 600 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden? -> 0 }T
testing ... found: hauled out
hiding-area 0 at  circle press  [: hidden? ;] 600 soon drop
[: -1 found-her ! ;] in-place  [: doing-busy? 0= ;] 600 soon drop
T{ hidden? -> 0 }T

testing the stalker coming while she hides: his comings counted
2 story.words:partner-load  5 run-frames
hiding-area 0 at  circle press  [: hidden? ;] 600 soon drop
search-var var@ before !
castle-1f-2 $132 game:debilitas send stalker-in  10 run-frames   \ (whether he finds her: by chance, and by the camera)
T{ [: came-while-hidden @  searched @ ;] in-place  search-var var@ -> -1 -1 before @ 1+ }T
game:debilitas send stalker-out  300 run-frames  circle press  200 run-frames

testing Debilitas's shoves: by the hall (then done this time); far off (then gone)
room-afresh  castle-1f-2 $11 game:debilitas send stalker-in  30 run-frames
[: ['] shoves-by-hall start drop ;] in-place
T{ [: [: his-shove-done @ ;] in-place ;] 900 soon -> -1 }T
[: ['] shoves-far start drop ;] in-place
T{ [: [: his-far-shove @ 0= ;] in-place ;] 1500 soon -> -1 }T
game:debilitas send stalker-out  60 run-frames

testing in by the gate's way: Debilitas knocked down beyond it, the file; no going back
castle-1f-2 $81 rooms send go-to-room  5 run-frames
T{ [: doing-busy? 0= ;] 600 soon  through-file filed?  [: through-gate @ ;] in-place  game:debilitas body-room -> -1 -1 -1 castle-1f-10 }T
0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ circle press  [: window-shown no-way-back-msg = ;] 300 soon -> -1 }T
300 read-on  game:debilitas send stalker-out  60 run-frames

testing the machine's hum (the voices due): stepping out of the far way
got-study-key story-flag-set  got-scrawled-memo story-flag-clear  c1f-12-flag1 story-flag-clear  room-afresh
leaving-3-area 1 across  leaving-3-area -1 across
T{ [: [: hum-playing @ ;] in-place ;] 30 soon -> -1 }T
100 run-frames

testing the hole's voices (yes): her hand in it - the scene, the Scrawled Memo
scrawled-memo items:items-of before !
hole-area 0 at  circle press  [: window-shown voices-msg = ;] 300 soon drop
[: window-shown 0< 0= if  cross press  then  movie-skipped state-flag-set  got-scrawled-memo story-flag? ;] 1500 soon drop
300 read-on
T{ got-scrawled-memo story-flag?  scrawled-memo items:items-of  voices-seen resident-flag? -> -1 before @ 1+ -1 }T

testing coming in by exit 1: the map's page
2 [: subscreen.state:her-pg ! ;] game:subscreen enter execute leave-actor
castle-1f-2 1 rooms send go-to-room  30 run-frames
T{ map-page@ -> 3 }T

testing the movie replayed (the extras): the voices
T{ 31 replays  replayed? -> -1 }T
test-summary
