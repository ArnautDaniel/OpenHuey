\ house-of-truth-1f-8's place: brought here by the forced way, the first tremor (her crouch, its facts,
\ the track); the snagged door - her try and its remark, worn through by her blows, Hewie's leap through
\ it; the Magist Medallion Hewie sniffs out; two creatures let out as she goes on; the house coming down -
\ the drone, the tremors, her first step in taking her on; the clock asked (no). Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_8.fs
IN: test-house-of-truth-1f-8
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-8 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  house-of-truth-1f-8 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
ht1f-9-flag2 story-flag-clear  over-music-f story-flag-clear  tremor-warned story-flag-clear  tremor-warned-2 story-flag-clear
ht1f-8-flag1 story-flag-clear  ht1f-8-magist-medallion-found story-flag-clear  ht1f-8-magist-medallion story-flag-clear
testing brought here by the forced way: the first tremor - she crouches; its facts, the track; Lorenzo sent ahead
lorenzo2-kind story.words:partner-load  5 run-frames
house-of-truth-1f-8 forced-exit rooms send go-to-room  5 run-frames
T{ [: [: ['] first-tremor running? ;] in-place ;] 120 soon  [: window-shown 0< 0= if  cross press  then  tremor-warned-2 story-flag? ;] 900 soon -> -1 -1 }T
T{ over-music-f story-flag?  tremor-warned story-flag?  [: snag-var var@ ;] in-place -> -1 -1 5 }T
300 read-on  1e sound-scale!  game:lorenzo2 send stalker-out  10 run-frames
testing the snagged door: her try at it, its remark
snagged-door [: lock ;] in-place  room-afresh  0 by-door
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-xt @ ;] in-place  ' tries-snagged = -> -1 }T
testing her blows at it: worn down (5 a full blow), through at 0
T{ [: 6 snag-var var!  ;] in-place  -37e 5e 177e blow-at  [: -37e 0e 177e 7e 20e her-blow-in? ;] in-place -> -1 }T
[: body-point-off ;] her
T{ [: ['] struck start drop ;] in-place  20 run-frames  [: snag-var var@ ;] in-place  snagged-door doors:door-locked? -> 1 -1 }T
T{ [: ['] struck start drop ;] in-place  20 run-frames  [: snag-var var@ ;] in-place  snagged-door doors:door-locked? -> 0 0 }T
testing Hewie leaping in at it: through
snagged-door [: lock  snagged-door false open-mark ;] in-place
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
T{ [: ['] leaps-in start drop ;] in-place  [: snagged-door doors:door-locked? 0= ;] 900 soon -> -1 }T
300 run-frames
testing the Magist Medallion: Hewie sniffs it out; taken
[: ['] magist-sniffed start drop ;] in-place
T{ [: ht1f-8-magist-medallion-found story-flag? ;] 600 soon -> -1 }T
400 run-frames  magist-medallion items:items-of before !
T{ 0 86e 1e 68e put  circle press  300 read-on  ht1f-8-magist-medallion story-flag?  magist-medallion items:items-of -> -1 before @ 1+ }T
testing two creatures let out as she crosses on (once)
0 17e 0e -65e put
T{ 0 17e 0e -45e put  ht1f-8-flag1 story-flag? -> -1 }T
testing the clock: the save asked (no)
T{ clock-area -22 at  circle press  shown-soon -> clock-msg }T
[: key: Right tap  picked 1 = dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press  300 read-on
testing the house coming down: the drone, the tremors (slight - the stalker dealt with)
ht1f-9-flag2 story-flag-set  over-music-f story-flag-set  found-file-257 story-flag-set  room-afresh
T{ [: drone-on @  ['] slight-tremors running? ;] in-place -> -1 -1 }T
testing ... and the violent ones while he isn't
found-file-257 story-flag-clear  room-afresh
T{ [: ['] violent-tremors running? ;] in-place -> -1 }T
testing ... her first step in (her track not yet on) takes her on
over-music-f story-flag-clear  room-afresh  [: 2 prepare-exit ;] in-place  0 40e 0e 90e put
T{ 0 20e 0e 90e put  [: room-id house-of-truth-1f-8 <> world-held state-flag? or ;] 600 soon -> -1 }T
600 play-on  ht1f-9-flag2 story-flag-clear  over-music-f story-flag-clear
test-summary
