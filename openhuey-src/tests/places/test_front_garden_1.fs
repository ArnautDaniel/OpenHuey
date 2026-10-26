\ front-garden-1's place (scripts/story/places/front-garden-1.fs): the gate and the drive - the
\ gate's scene and remark, the tree and the marionette, Hewie fetching it, the lamps at night, the
\ crate, the Medallions, the spirits, the castle gate key. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_front_garden_1.fs
IN: test-front-garden-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.front-garden-1 items.names ;

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
: room-afresh ( -- )  front-garden-1 room!  60 run-frames ;
: facing ( area F: px pz -- )   \ (in the area's middle, facing the point)
    {: area F: px pz :}  area area-middle drop {: F: x y z :}
    px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)

: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: at-tree ( -- )   \ (in the tree's area, short of its middle, facing it)
    tree-area area-middle drop {: F: x y z :}  0 x y z 5e f- put ;

free-play  10 run-frames  0 game:fiona send costume
testing the gate, at first: the opening's scene there - file $222 - then only the locked gate
found-file-222 story-flag-clear  room-afresh
gate-area 0 at  circle press  900 play-on
T{ found-file-222 story-flag?  gate-file filed?  [: fiona held? ;] in-place -> -1 -1 0 }T
T{ gate-area 0 at  said -> gate-locked-msg }T

testing the tree, the marionette up in it: before her clothes, something there; dressed, the scene (file $223); then too high
got-broken-marionette story-flag-clear  found-file-202 story-flag-clear  found-file-223 story-flag-clear  room-afresh
T{ marionette-part part?  at-tree  said -> -1 in-the-tree-msg }T
found-file-202 story-flag-set
at-tree  circle press  900 play-on
T{ found-file-223 story-flag?  tree-file filed?  [: fiona held? ;] in-place -> -1 -1 0 }T
T{ at-tree  said -> too-high-msg }T

testing Hewie sent up for it ("go there" by the tree): he gets it - the marionette hers, gone from the tree - she watches
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
game:hewie send join-fiona  60 run-frames
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
variable before  broken-marionette items:items-of before !
by-tree-area -204e -117e facing  hewie-tree-area area-middle drop hewie-spot
[: go-there-cmd started-now !  sent-up ;] in-place
1500 play-on
T{ got-broken-marionette story-flag?  broken-marionette items:items-of  marionette-part part?  fetched-file filed? -> -1 before @ 1+ 0 0 }T   \ ($8276: the other game's file, not this one's)

testing the lamps at night: lit, their glows; one flickers and is lit again
c2f-3-flag1 story-flag-set  room-afresh
T{ first-lamp-part part?  [: first-glow effect @ 0<> ;] in-place -> -1 -1 }T
[: 0 flicker-lamp ;] in-place  120 run-frames
T{ first-lamp-part part?  [: first-glow effect @ 0<> ;] in-place -> -1 -1 }T
c2f-3-flag1 story-flag-clear

testing the crate: a blow breaks it (once)
fg-1-breakable-broken story-flag-clear  room-afresh
T{ -284e 61e -97e blow-at  [: the-crate breaks? ;] in-place  [: the-crate breaks? ;] in-place -> -1 0 }T
[: body-point-off ;] as-her

testing the Medallions: the one Hewie found, picked up; the one on the ledge, reached
fg-1-medallion-found story-flag-set  fg-1-medallion story-flag-clear  fg-1-medallion-2 story-flag-clear  room-afresh
medallion items:items-of before !
180 -331.4e 60e 235.2e put  circle press  300 read-on   \ (facing off the gate, so its remark isn't offered first)
T{ fg-1-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
-366e 60e 26e picked
T{ fg-1-medallion-2 story-flag?  medallion items:items-of -> -1 before @ 2 + }T

testing the spirits: let out here once, the way open
cb1-6-flag3 story-flag-clear  0 s" doors" actor-named send unlock  room-afresh
T{ cb1-6-flag3 story-flag? -> -1 }T

testing the castle gate key at the gate: into the castle by the forced way
room-afresh  gate-area 0 at
0 15 -1 0 broadcast item-event   \ (the key's event: $F, by her)
T{ [: room-id front-garden-1 <> ;] 300 soon -> -1 }T
test-summary
