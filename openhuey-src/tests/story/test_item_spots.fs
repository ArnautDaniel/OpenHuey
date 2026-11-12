\ Items lying in the rooms told to mods (mod API 1.5; story/kit.fs item-spots, docs/modding.md "Items
\ lying about"): every room's table sound; castle-2f-12's Refined Antimony - its sparkle up and
\ told (item-placed, where it lies), taken by a mod as her action button would (take-item-at: the
\ room's own doing, the item given, item-cleared), not drawn while a mod shows the items itself,
\ not taken where there's none. Headless:
\   build/openhuey --test tests/story/test_item_spots.fs
IN: test-item-spots
USING: tester engine game-state actors messages room-names game facts story-names story.kit
    story.places.castle-2f-12 items.names mods debug ;

: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: place-id ( -- id )  room-id room-name actor-named ;

\ ---- a mod's ear: what the places tell ----
variable placed-k  variable placed-item  variable cleared-k  variable take-ok  variable answered
fvariable px  fvariable pz
state: spy-state  cell field spy-unused  end-state
behaviour spy
  on spawned ( -- )  self subscribe item-placed  self subscribe item-cleared ;
  on item-placed ( room k item x y z -- )  cell>f pz f!  drop  cell>f px f!  placed-item !  placed-k !  drop ;
  on item-cleared ( room k -- )  cleared-k !  drop ;
  on take-answer ( k started? -- )  take-ok !  drop  -1 answered ! ;
end-behaviour
spy spy-state s" item-spy" spawn constant the-spy
: forget-told ( -- )  -1 placed-k !  -1 placed-item !  -1 cleared-k !  0 take-ok !  0 answered ! ;
: ask-take ( k -- )  0 answered !  place-id the-spy enter  send take-item-at  leave-actor ;
: answered? ( -- flag )  answered @ 0<> ;

testing every room's table: items, effects 0..31, -1 ending (140 and more of them)
: spots-in ( t -- n ok? )
    0 true {: t n ok :}
    begin  t @ -1 <> while
        t @ 0 $100 within  t 2 cells + @ 0 #room-effects within and  ok and to ok
        n 1+ to n  t 3 cells + to t
    repeat  n ok ;
: all-spots ( -- n ok? )
    0 true {: n ok :}
    #place-rooms 0 do
        i place-of ?dup if  d.items @ ?dup if  spots-in  ok and to ok  n + to n  then  then
    loop  n ok ;
T{ all-spots  swap 140 >= -> -1 -1 }T

full-suite   \ (on: rooms played, frames run - the full run's)
testing castle-2f-12: its Refined Antimony sparkling where it lies (Hewie found it) - told, at (22.9, -9.7)
free-play  front-garden-2 room!  20 run-frames
forget-told
c2f-12-refined-antimony-found story-flag-set  c2f-12-refined-antimony story-flag-clear
castle-2f-12 room!  60 run-frames
T{ placed-k @  placed-item @  px f@ 22.9e f- fabs 0.01e f<  pz f@ -9.7e f- fabs 0.01e f< -> antimony-fx refined-antimony -1 -1 }T
T{ [: antimony-fx effect @ 0<> ;] in-place -> -1 }T
testing ... none at the vase's Medallion (not broken): not taken
T{ medallion-fx ask-take  ' answered? 10 soon  take-ok @ -> -1 0 }T
testing ... taken by a mod: her picking it up started, the item given, the sparkle gone - told
variable before  refined-antimony items:items-of before !
T{ antimony-fx ask-take  ' answered? 10 soon  take-ok @ -> -1 -1 }T
T{ [: cleared-k @ antimony-fx = ;] 600 soon  c2f-12-refined-antimony story-flag?  refined-antimony items:items-of -> -1 -1 before @ 1+ }T
testing ... not again: it's gone
T{ antimony-fx ask-take  ' answered? 10 soon  take-ok @ -> -1 0 }T
testing a mod showing the items itself: told all the same, the sparkle not drawn
false item-sparkles!  forget-told
c2f-12-refined-antimony story-flag-clear  front-garden-2 room!  20 run-frames  castle-2f-12 room!  60 run-frames
T{ placed-k @  [: antimony-fx effect @ ;] in-place -> antimony-fx 0 }T
testing ... leaving the room: cleared
forget-told  front-garden-2 room!  20 run-frames
T{ cleared-k @ -> antimony-fx }T
true item-sparkles!

test-summary
