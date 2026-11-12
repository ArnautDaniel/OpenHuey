\ The item-props sample mod (examples/mods/item-props: mod API 1.5): the items lying about shown as
\ props - castle-2f-12's Refined Antimony a cube where it lies, its sparkle not drawn; taken (as a
\ mod's hand would: take-item-at), given and the cube gone; the room left, none kept. Not a
\ test_*.fs: it runs with the mod on (CMakeLists.txt):
\   build/openhuey --mod examples/mods/item-props --test tests/mods/item-props.fs
IN: test-item-props
USING: tester engine actors messages room-names game debug facts story-names story.kit
    story.places.castle-2f-12 items.names item-props ;

: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: the-prop ( -- id )  item-props enter  antimony-fx spot @  leave-actor ;

testing the mod on, the game's sparkles off
2 run-frames   \ (its actor started)
T{ s" item-props" mod-on?  mods:item-sparkles? -> -1 0 }T
testing the model it falls back on: there, read, a cube 2 across
T{ s" PROPS/item.gltf" data-file?  s" PROPS/item-999.gltf" data-file? -> -1 0 }T
T{ 39 item-props:model s" PROPS/item.gltf" s= -> -1 }T

free-play  front-garden-2 room!  20 run-frames
c2f-12-refined-antimony-found story-flag-set  c2f-12-refined-antimony story-flag-clear
castle-2f-12 room!  60 run-frames
testing castle-2f-12's Refined Antimony: a prop where it lies (22.9, 2, -9.7), its sparkle not drawn
T{ the-prop 0< 0=  [: antimony-fx effect @ ;] in-place -> -1 0 }T
: placed-at? ( -- flag )  the-prop prop-at  -9.7e f- fabs 0.01e f<  2e f- fabs 0.01e f< and  22.9e f- fabs 0.01e f< and ;
T{ placed-at? -> -1 }T
: bounds-ok? ( -- flag )  the-prop prop-bounds  1e f- fabs 0.001e f<  1e f- fabs 0.001e f< and  1e f- fabs 0.001e f< and
    1e f+ fabs 0.001e f< and  1e f+ fabs 0.001e f< and  1e f+ fabs 0.001e f< and ;
T{ bounds-ok? -> -1 }T
20 run-frames   \ (drawn, turning)
testing ... taken: given, the prop gone
\ (as a mod's hand would - sent to the place by the mod's actor)
: take ( k -- )  room-id room-name actor-named  item-props enter  send take-item-at  leave-actor ;
antimony-fx take
T{ [: the-prop 0< ;] 600 soon  c2f-12-refined-antimony story-flag? -> -1 -1 }T
testing the room left with one shown: none kept
c2f-12-refined-antimony story-flag-clear  front-garden-2 room!  20 run-frames  castle-2f-12 room!  60 run-frames
T{ the-prop 0< -> 0 }T
front-garden-2 room!  20 run-frames
T{ the-prop -> -1 }T

test-summary
