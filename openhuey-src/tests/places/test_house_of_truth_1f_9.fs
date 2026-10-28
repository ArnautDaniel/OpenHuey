\ house-of-truth-1f-9's place: coming in - the flames, the debris hidden; the hole's remarks (the
\ branch, the smell); with Hewie by - her remark, his sniffing it out (file 255); her gesture sending
\ him; the file found - her word; his bark at the hole; the Cane of Caduceus set in - the scene (Lorenzo
\ in his last form, the door locked, the fans, the debris falling); "Oh no! It's locked!"; the debris
\ falling through - the traps; a corner's drone, its sparks' blows, its plate; Lorenzo brought down -
\ the scene (the house freed: file 256, the door unlocked); the hole's and the pit's remarks after;
\ the corners' sounds; brought in by the forced way - its scene, out again; the replays. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_1f_9.fs
IN: test-house-of-truth-1f-9
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-1f-9 ;
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
: room-afresh ( -- )  house-of-truth-1f-9 room!  60 run-frames ;
: replays ( n -- flag )
    replay-var var!  events-held state-flag-set  house-of-truth-1f-9 -1 rooms send go-to-room  5 run-frames
    [: cutscene-active? movie-status 1 = or ;] 600 soon  [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon and ;
: of-kind ( k -- n )  0  128 0 do  i thing-used? if  i thing-kind 2 pick = if  1+  then  then  loop  nip ;
: fan-speed@ ( -- F: v )  [: 0 fan$ fan-speed object@ ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
ht1f-9-flag1 story-flag-clear  ht1f-9-flag2 story-flag-clear  found-file-255 story-flag-clear  over-music-f story-flag-clear
tremor-warned story-flag-clear
room-afresh
testing coming in: the flames lit, the debris hidden, the cane not yet set in
T{ [: flame-0-fx effect @ 0<>  flame-1-fx effect @ 0<>  cane-part part? ;] in-place  [: 0 keri$ placed-shown? ;] in-place -> -1 -1 0 0 }T
testing the hole, Hewie not by: the branch, then the smell
T{ hole-area 0 at  said  said -> branch-msg smell-msg }T
testing the corners before the cane: a sound as someone steps in, again once all are out
T{ left-area 0 at  [: left-on @ ;] in-place  0 0e 0e -40e put  [: left-on @ ;] in-place -> -1 0 }T
testing with Hewie by: her remark - he sniffs it out (file 255)
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona  60 run-frames
room-afresh
T{ hole-area 0 at  circle press  shown-soon -> inserted-msg }T
T{ [: window-shown 0< 0= if  cross press  then  found-file-255 story-flag? ;] 2000 soon -> -1 }T
200 play-on  [: doing-busy? 0= ;] 600 soon drop
T{ $255 filed? -> -1 }T
testing the file found: her word to him
T{ hole-area 0 at  said -> sniff-msg }T
testing her gesture sending him: he sniffs it out
found-file-255 story-flag-clear  room-afresh
T{ [: ['] sends-him fiona start-holding drop ;] in-place  [: window-shown 0< 0= if  cross press  then  found-file-255 story-flag? ;] 2000 soon -> -1 }T
200 play-on  [: doing-busy? 0= ;] 600 soon drop
testing Hewie by the hole (once a visit): his bark
found-file-255 story-flag-clear  room-afresh
house-of-truth-1f-9 1e 0e 50e nav-tri 0 game:hewie send hewie-place  30 run-frames
T{ [: [: hole-noticed @ ;] in-place ;] 600 soon -> -1 }T
found-file-255 story-flag-set  [: doing-busy? 0= ;] 600 soon drop
testing the Cane of Caduceus set in: the scene - Lorenzo's last form, the door locked, the fans, the debris
cane-of-caduceus 1 give-item  10 run-frames  cane-of-caduceus items:items-of before !
T{ 4 use-on-place  [: movie-skipped state-flag-set  $4D resident-flag? ;] 3000 soon  ht1f-9-flag1 story-flag? -> -1 -1 }T
T{ partner-kind  $ED doors:door-locked?  scene-5-pending state-flag?  [: cane-part part? ;] in-place -> kind39-kind -1 -1 -1 }T
T{ [: ['] fans running?  ['] falling running? ;] in-place  cane-of-caduceus items:items-of -> -1 -1 before @ 1- }T
[: doing-busy? 0= ;] 600 soon drop
testing "Oh no! It's locked!" (her try at the door)
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-xt @ ;] in-place  ' locked-now = -> -1 }T
testing the debris falling (six at most), each carrying a "keri"; two fallen through - counted back, the traps
T{ [: 0 debris-out !  debris-falls  debris-out @ ;] in-place  3 run-frames  9 of-kind = -> -1 }T
T{ [: 0 keri$ placed-shown? ;] in-place -> -1 }T
[: debris-out @ ;] in-place before !
T{ [: 2 fell-now ! ;] in-place  2 run-frames  [: debris-out @  trap-set @  ['] traps running? ;] in-place -> before @ 2 - -1 -1 }T
T{ [: 10 of-kind 0> ;] 400 soon  [: [: trap-set @ 0= ;] in-place ;] 400 soon -> -1 -1 }T
T{ things-id send story-things-off  3 run-frames  9 of-kind 10 of-kind -> 0 0 }T
testing a corner stepped into: its drone, the fan racing, its sparks - their blow - then running down; its plate up
T{ left-area 0 at  [: left-on @  ['] left-corner running?  left-pace @ ;] in-place -> -1 -1 racing }T
T{ [: [: left-armed @ ;] in-place ;] 200 soon  [: left-sparks-fx effect @ 0<> ;] in-place -> -1 -1 }T
T{ [: s" fumi0" place-y object@ ;] in-place 0e f= -> -1 }T
T{ 7 0 at  [: [: fiona-mode-now @ own-mode <> ;] in-place ;] 30 soon -> -1 }T
T{ [: [: left-armed @ 0= ;] in-place ;] 200 soon  [: left-pace @ ;] in-place -> -1 stopping }T
T{ [: [: left-on @ 0= ;] in-place ;] 1200 soon  [: left-pace @ ;] in-place -> -1 steadying }T
testing Lorenzo brought down: the scene - the house freed (file 256), the door unlocked, the drone
T{ [: down-cond stalker send set-cond ;] in-place  [: movie-skipped state-flag-set  $4E resident-flag? ;] 3000 soon  ht1f-9-flag2 story-flag? -> -1 -1 }T
T{ $256 filed?  $ED doors:door-locked?  partner-kind  scene-5-pending state-flag? -> -1 0 kind12-kind 0 }T
T{ [: drone-on @  ['] falling running? ;] in-place -> -1 0 }T
[: doing-busy? 0= ;] 600 soon drop
testing after: the hole - the cane's remark, then "get out"; the pit's remark, then "get out"
room-afresh
T{ hole-area 0 at  said  said -> moaning-msg get-out-msg }T
T{ 0 0e 0e -21e put  said  said -> toast-msg get-out-msg }T
testing brought in by the forced way: its scene, out again by it
T{ house-of-truth-1f-9 forced-exit rooms send go-to-room  5 run-frames  [: movie-skipped state-flag-set  $4F resident-flag? ;] 3000 soon -> -1 }T
T{ [: room-id house-of-truth-1f-9 <> ;] 600 soon -> -1 }T
testing the movies replayed (the extras)
T{ 74 replays  75 replays  82 replays -> -1 -1 -1 }T
test-summary
