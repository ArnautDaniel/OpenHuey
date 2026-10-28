\ front-garden-3's place (scripts/story/places/front-garden-3.fs, docs/rooms-design.md): the cage's
\ garden as the original plays it - the opening, the remarks, the breakable and its magnesia, the
\ box and the grate with Hewie, at night Hewie freed from the wire - and what every room does
\ (story/common.fs: crouching, a drink). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_front_garden_3.fs
IN: test-front-garden-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    fiona.state fiona.model fiona.anims story.kit story.places.front-garden-3 ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] as-her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: by ( deg F: x z -- )  {: F: x z :}  x 0e z put ;
: said ( -- msg )   \ (the remark offered there: taken, its message, read on, done with)
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: play-on ( n -- )   \ (its messages read on, its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;

testing a new game: the opening - the doors locked, the figures in the trees, the first file, then she is free
60 run-frames
T{ room-id  opening-done story-flag?  $200 filed? -> front-garden-3 -1 -1 }T
T{ 3 story.base:slot-actor @ 0< 0=  4 story.base:slot-actor @ 0< 0= -> -1 -1 }T
T{ 0 doors:door-locked?  free? -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
0 game:fiona send costume  10 run-frames

testing what she says as she looks about: "Where am I?", the cold, the fountain, the tree (once, then another)
T{ waking-area 0 at  said  fg-3-flag1 story-flag? -> where-am-i-msg -1 }T
T{ cold-area 0 at  said  fg-3-flag2 story-flag? -> cold-msg -1 }T
T{ fountain-area 0 at  said -> fountain-msg }T
T{ tree-area 0 at  said -> tree-msg }T
T{ tree-area 0 at  said -> tree-again }T
testing ... the hole, too small; the box against the grate
T{ hole-area -90 at  said -> hole-msg }T
T{ box-area -45 at  said  world-frozen state-flag? -> box-msg 0 }T
testing ... nearing the figures: the second cries out and rises, and past them they are gone
200 225e -44e by  60 run-frames
T{ [: figure-cried @ ;] in-place -> -1 }T

no-flee state-flag-clear   \ (the opening's hold on her - no fleeing, no blows, no crouching - over, as later in the story)
testing the breakable (story/kit.fs): whole, a blow elsewhere misses it; one in its place breaks it - in pieces, once
fg-3-breakable-broken story-flag-clear  front-garden-3 room!  30 run-frames
T{ whole-part part?  broken-part part? -> -1 0 }T
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )   \ (her blow's point there)
    blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
T{ 150e 1e -35.4e blow-at  [: the-box breaks? ;] in-place -> 0 }T
T{ 185.7e 1e -35.4e blow-at  [: the-box breaks? ;] in-place  [: the-box breaks? ;] in-place -> -1 0 }T
T{ fg-3-breakable-broken story-flag?  whole-part part?  broken-part part? -> -1 0 -1 }T
[: body-point-off ;] as-her
testing ... broken, as she comes in: in pieces, the magnesia's sparkle; she picks it up
fg-3-breakable-broken story-flag-set  front-garden-3 room!  30 run-frames
T{ [: magnesia-fx effect @ 0<> ;] in-place -> -1 }T
variable before  magnesia items:items-of before !
0 185.7e -40.4e by  circle press  300 play-on
T{ fg-3-magnesia story-flag?  magnesia items:items-of -> -1 before @ 1+ }T

testing what every room does: L1 held, she crouches (half hidden); let go, up again
key: Q true key-hold  60 run-frames
T{ fiona-half-hidden state-flag?  doing-busy? -> -1 -1 }T
key: Q false key-hold  60 run-frames
T{ fiona-half-hidden state-flag?  free? -> 0 -1 }T
testing ... a drink (her shared $8D): her hand to her mouth, then free
0 $8D -1 0 broadcast item-event  10 run-frames
T{ [: fiona.model:anim@ ;] as-her -> bent-hand-mouth-anim }T
120 run-frames  T{ free? -> -1 }T

testing the box and the grate with Hewie: told "go there" by the box, he goes and sits by it; she pushes, he goes through the hole and pushes from the other side - the grate open, the door unlocked
hewie-freed story-flag-set  hewie-commandable state-flag-set  front-garden-3 room!  30 run-frames
game:hewie send join-fiona  60 run-frames
: ready ( -- )  game:hewie enter  1000 hewie.state:his-trust-points !  leave-actor ;   \ (trusting her)
ready
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
-90 0e 138e by  40e -10e 150e hewie-spot   \ (she by the box facing the hole, he in his part of the yard)
key: I true key-hold  game-tick  key: I false key-hold  60 run-frames   \ (the stick up: "go there")
T{ [: hewie held? ;] in-place -> -1 }T
400 run-frames
T{ [: hewie-at-box @ ;] in-place -> -1 }T
testing ... called back from the box ("come back"): he comes back; sent again
[: come-back-cmd started-now ! ;] in-place
T{ [: [: hewie-at-box @ 0= ;] in-place ;] 30 soon  [: [: hewie held? 0= ;] in-place ;] 600 soon -> -1 -1 }T
-90 0e 138e by  key: I true key-hold  game-tick  key: I false key-hold  460 run-frames
T{ [: hewie-at-box @ ;] in-place -> -1 }T
box-area -45 at  circle press  900 play-on
T{ grate-open story-flag?  grate-door doors:door-locked?  $275 filed?  free? -> -1 0 0 -1 }T   \ ($8275: filed in the other game only)

testing at night, back from the castle: Hewie caught in the wire, whining; she frees him - the scene, and he runs off (out of the game)
c2f-3-flag1 story-flag-set  castle-2f-3 room!  30 run-frames  front-garden-3 room!  60 run-frames
T{ [: hewie-in? hewie held? ;] in-place -> -1 -1 }T
0 228.1e 72e by
T{ circle press  1200 play-on  hewie-found-in-wire story-flag?  $205 filed?  free? -> -1 -1 -1 }T
T{ game:hewie body? -> 0 }T   \ (not left lying by the wire)
testing ... back in later: the wire alone, its remark
front-garden-3 room!  60 run-frames
T{ game:hewie body?  0 228.1e 70e by  said -> 0 wire-snagged-msg }T
test-summary
