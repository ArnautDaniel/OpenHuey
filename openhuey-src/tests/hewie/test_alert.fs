\ Hewie: what alerts him (Hewie_Alert: an exit into the stalker's room, the stalker heard), what
\ he hears off screen (Hewie_HiddenFrame), his feelings and leaps by the stalker's kind
\ (Hewie_Feeling, Hewie_CountEncounter, Hewie_Dares), struck (Hewie_TakeBlow), near her by his way
\ (Hewie_FionaNearCommand), coming in (Hewie_Arrive: through the exit with her, letting go of the
\ stalker) - docs/subsystems/hewie.md. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_alert.fs
IN: test-alert
USING: tester engine game-state actors messages common facts vectors room-names game debug flag-names stalker-names hewie-names hewie.state hewie.body hewie.anims hewie.model hewie.states hewie.offscreen hewie.fight hewie.mind ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  game:hewie enter  execute  leave-actor ;
: his! ( xt -- )  game:hewie enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: blind ( on -- )  if  stalkers-blind state-flag-set  else  stalkers-blind state-flag-clear  then ;
1 seed !

free-play  front-garden-2 room!  20 frames
140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames

testing his feelings by the stalker's kind: Daniella's later kinds, the TintStalker and kind 37 Riccardo's, kind 39 Lorenzo's, none for others
: feel-of ( kind -- i | -1 )  [: st-kind !  feel dup if  his-feel - cell /  else  drop -1  then ;] his@ ;
T{ debilitas-kind feel-of  debilitas-27-kind feel-of  kind34-kind feel-of  kind36-kind feel-of -> 0 0 1 1 }T
T{ tintstalker-kind feel-of  kind37-kind feel-of  riccardo-kind feel-of  kind39-kind feel-of  lorenzo-kind feel-of -> 3 3 3 2 2 }T
T{ 99 feel-of  [: 99 st-kind !  5 feel+  feeling ;] his@ -> -1 0 }T

testing how often he leapt at each (Hewie_CountEncounter): counted to 100, kept in the save
T{ [: his-met 4 cells 0 fill  daniella-kind st-kind !  met+ met+  his-met cell+ @ ;] his@ -> 2 }T
T{ [: 99 his-met cell+ !  met+ met+  his-met cell+ @ ;] his@ -> 100 }T
T{ [: 0 his-trust !  dares? ;] his@ -> 0 }T
[: his-met 4 cells 0 fill  -1 st-kind ! ;] his!

testing struck while angry: whoever struck him calls him (her 0, another 2); her newly making him angry - no call
: struck-by ( from -- call )
    [: {: from :}  angry-mood 450 set-mood  no-call his-call !  0 cond!  free-mode his-mode !  0 his-sub !  100 hp!
       shove-blow 1 0 from take-blow drop  his-call @ ;] his@ ;
T{ game:fiona struck-by  game:hewie struck-by -> 0 2 }T   \ (another: anyone with a body but her)
T{ [: normal-mood -1 set-mood  0 his-hits !  0 0 set-action ;] his! -> }T
testing struck by another (not her): her hits held against him forgiven
T{ [: 3 his-hits !  free-mode his-mode !  0 his-sub !  shove-blow 1 0 game:hewie take-blow drop  his-hits @ ;] his@ -> 0 }T
testing down: a blow has him go limp ($74), the stalker's grab (6) does not
T{ [: 0 0 set-action  0 hp!  down-cond cond!  ignored-blow 5 0 game:debilitas take-blow  his-action @ go-limp-act = ;] his@ -> 0 0 }T
T{ [: shove-blow 1 0 game:debilitas take-blow  his-action @ ;] his@ -> -1 $74 }T
[: 100 hp!  well-cond cond!  0 0 set-action ;] his!  10 frames

testing near her for a command (Hewie_FionaNearCommand): by his way over the floor - never shorter than straight
T{ [: way-to-her  her-dist f-  -0.01e f> ;] his@ -> -1 }T

testing what he hears (the acoustics' heard): its source and room; nothing heard
T{ [: 0 his-heard-room ! ;] his!  $80 room-id -1 -1 stalker-noise game:acoustics send noise  game-tick
   [: his-heard-src @  his-heard-room @ ;] his@ -> stalker-noise room-id }T
T{ game-tick  [: his-heard-src @ ;] his@ -> -1 }T

testing coming in through the exit with her ($88): at its spot through it, on its half, himself again
create spot /vec allot
T{ 0 2 exit-spot  spot vec!  0< -> 0 }T
T{ [: 0 his-hit-t !  0 his-door !  through-act his-action !  arrive  his-action @  his-at spot vec-dist 5e f< ;] his@ -> -1 0 -1 }T
testing coming in mid-grip (after-attack-act, at a flank): letting go of him ($2202), to its end ($73)
T{ [: near-grip his-grip !  at-flank-act his-attack-was !  after-attack-act his-action !  1 his-away !  arrive  his-action @  anim@ ;] his@ -> -1 $73 $2202 }T
[: no-grip his-grip !  0 0 set-action ;] his!  10 frames

testing she leaves while he is at the stalker (Hewie_Vt34): only leaping or biting this frame
T{ [: at-flank-act his-action !  1 his-on-him !  at-stalker?  0 his-on-him !  at-stalker?  sniff-act his-action !  1 his-on-him !  at-stalker? ;] his@ -> -1 0 0 }T
[: 0 his-on-him !  0 0 set-action ;] his!
testing she leaves while he is down: up again (hurt) only in a hard game, or where he may not lie
: left-down ( -- hp )  [: 0 hp!  down-cond cond!  knocked-down-act his-action !  -1 fiona-left  his-hp @ ;] his@ ;
T{ [: floor-free? ;] his@  left-down -> 0 1 }T   \ (at heel, by door 0 of front-garden-2)
: free-spot ( -- )   \ (the first triangle away from doors and ladders)
    [: nav-tris 0 do  room-id i dup tri-center body-place  floor-free? if  unloop exit  then  loop ;] his! ;
free-spot
T{ [: floor-free? ;] his@  left-down -> -1 0 }T
T{ $AF story-flag-set  left-down  $AF progress pr.story bit-clear -> 1 }T
[: room-id s" fiona" actor-named body-tri dup tri-center body-place ;] his!
[: 100 hp!  well-cond cond!  0 his-hide !  0 0 set-action ;] his!  10 frames

testing off screen, a noise from another room (Hewie_HiddenFrame, on-noise): he makes for it ($33) by chance
front-garden-3 room!  2 frames   \ (she goes on: he is left in front-garden-2)
: noise-there ( -- )  $80 front-garden-3 -1 -1 fiona-noise game:acoustics send noise ;
[: wait-off-act 0 want  -1 his-noise-room ! ;] his!
T{ [: noise-there  [: his-noise-room @ front-garden-3 = ;] his@ ;] 60 soon -> -1 }T

testing the exit into the stalker's room (Hewie_Alert 2): told, the exit his target ($8 faces it)
front-garden-2 room!  10 frames  140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
variable next   variable by
: an-exit ( -- )  8 0 do  room-id i room-exit-leads drop dup 0< 0=  over room-id <> and if  next !  i by !  unloop exit  then  drop  loop ;
an-exit
: first-exit ( -- e )  8 0 do  room-id i room-exit-leads drop next @ = if  i unloop exit  then  loop  -1 ;
true blind  next @ 0 game:debilitas send stalker-in  3 frames
T{ [: [: his-alert @ ;] his@ exit-alert = ;] 10 soon  [: his-alert-what @ ;] his@ -> -1 first-exit }T
T{ [: -1 his-target !  target-tri 0< 0= ;] his@ -> -1 }T
debug:debilitas-out  5 frames
T{ [: [: his-alert @ ;] his@ no-alert = ;] 10 soon -> -1 }T
false blind
testing she leaves by an exit: nearer its door than she is (Hewie_Vt34's Actor_NearerRoom - then through it with her, $88)
T{ [: 0 1 exit-spot drop his-place  0 nearer-door? ;] his@ -> -1 }T
T{ [: fiona-at vec@ his-place  0 nearer-door? ;] his@ -> 0 }T
[: room-id s" fiona" actor-named body-tri dup tri-center body-place ;] his!
test-summary
