\ The inventory (docs/subsystems/items.md; src/game/items.c): giving, stacks, the lists, spending
\ and removing, what she wears, sorting, the files; using items (their rules), the counters and
\ the timed effects; the story's words. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/items/test_items.fs
IN: test-items
USING: tester engine game-state actors messages game room-names items items.names items.tables fiona.state story-names story.state story.words flag-names fiona.moving ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: give ( id n -- )  game:items send give-item  deliver ;
: ids ( list -- a b c )  dup 0 item-at  over 1 item-at  rot 2 item-at ;
\ a stand-in for the menu: what came back from a use
variable used-id  variable used-result
state: menu-state  cell field dummy  end-state
behaviour stand-in-menu
  on item-used ( id result -- )  used-result !  used-id ! ;
end-behaviour
stand-in-menu menu-state s" stand-in-menu" spawn constant menu
: use ( list i -- result )   \ (as the menu: its rule, answered)
    -9 used-result !  menu enter  game:items send use-item  leave-actor  deliver  used-result @ ;
: equip ( list i -- )  menu enter  game:items send equip-item  leave-actor  deliver ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;

2 frames
testing a new game: nothing carried, nothing worn
T{ 0 listed  1 listed  2 listed  0 equipped -> 0 0 0 -1 }T

testing given: into its list (ids under $40, under $A0, the rest), a stack counted, one alone
camomile 3 give  study-key 1 give  old-parchment 1 give
T{ 1 0 item-at  camomile items-of  0 0 item-at  study-key items-of  2 0 item-at  old-parchment items-of -> camomile 3 study-key 1 old-parchment 1 }T
testing ... onto its stack, at most 99; an id with no class is not given
camomile 120 give  $30 1 give
T{ camomile items-of  0 listed -> 99 1 }T
testing ... a counter (the Fresh Flowers) starts at 0, counts each frame
fresh-flowers 1 give  1 frames
T{ 0 1 held-at n@  fresh-flowers items-of -> 1 1 }T

testing spent: a stack loses one; the last one goes and the list closes up
lavender 2 give
camomile game:items send take-item  deliver
T{ camomile items-of -> 98 }T
study-key game:items send take-item  deliver
T{ 0 listed  0 0 item-at -> 1 fresh-flowers }T

testing worn: in its kind's slot (boots 0, earrings 1, the costume 2, the choker 3), told to all
feather-boots 1 give  metal-boots 1 give  alchemist-earrings 1 give
1 2 equip   \ (the list: camomile, lavender, feather boots, metal boots, earrings)
T{ 0 equipped  1 2 equip-state  1 3 equip-state  1 0 equip-state -> feather-boots 1 2 -1 }T
1 3 equip
T{ 0 equipped  1 2 equip-state -> metal-boots 2 }T
menu enter  1 3 game:items send unequip-item  leave-actor  deliver
T{ 0 equipped -> -1 }T
testing ... one worn and thrown away is off
1 4 equip  menu enter  1 4 game:items send discard-item  leave-actor  deliver
T{ 1 equipped  alchemist-earrings items-of -> -1 0 }T

testing sorted: by their places in a sort (Items_Sort)
menu enter  1 game:items send sort-items  leave-actor  deliver
T{ 1 ids -> camomile lavender feather-boots }T
testing ... out of order: each kept, in order (a swap)
: in-order? ( list -- flag )
    true  over listed 1- 0 ?do  over i item-at item-sort  2 pick i 1+ item-at item-sort > if  drop false  then  loop  nip ;
variable before   1 listed before !
onion 1 give  quies 1 give  chicken-jerky 1 give
T{ 1 in-order? -> 0 }T
menu enter  1 game:items send sort-items  leave-actor  deliver
T{ 1 in-order?  1 listed before @ -  onion items-of  quies items-of  chicken-jerky items-of -> -1 3 1 1 1 }T

testing the files: found once each, in order
$205 -1 game:items send add-file  $205 -1 game:items send add-file  $206 0 game:items send add-file  deliver
T{ files  0 file@  1 file@ -> 2 $205 $206 }T

testing used: a remedy - Camomile calms her (fear and recovery down) and is spent
[: 60e her-fear f!  900 her-recovery ! ;] her
T{ 1 0 use  [: her-fear f@ f>s her-recovery @ ;] her  camomile items-of -> 1 0 0 97 }T
testing ... Lavender: its effect for 300 frames, running down
T{ 1 1 use  calm-boost boost-on? -> 1 -1 }T
T{ 300 frames  calm-boost boost-on? -> 0 }T
testing ... a key away from its place: nothing happens, kept
smudged-key 1 give
T{ 0 1 item-at 0 1 use  smudged-key items-of -> smudged-key 0 1 }T
testing ... a sketch: its map held, spent
castle-sketch 1 give
T{ 0 2 use  castle-map-held story-flag?  castle-sketch items-of -> 2 -1 0 }T
testing ... nothing there: an empty place
T{ 0 40 use  used-id @ -> 0 -1 }T

testing the Fresh Flowers wither after 9000 frames: the Withered Flowers instead, marked
9000 0 0 held-at 4 + l!  2 frames
T{ fresh-flowers items-of  withered-flowers items-of  flowers-withered story-flag? -> 0 1 -1 }T

testing the story's words: counted and given, the count; a mark and its test; the reward
camomile 2 item-give-count  deliver
T{ camomile 98 item-count?  camomile 100 item-count? -> -1 0 }T
T{ 7 subscreen-bit?  7 subscreen-bit  deliver  7 subscreen-bit? -> 0 -1 }T
45 progress pr.hewie-mistreated l!  reward-item  deliver
T{ files 1- file@ -> $271 }T

testing a key at its place: the room's action script for her starts (the gate key: the front
\ garden's spot 5 - its event $F), and she is marked as stalkers stay away
front-garden-1 -1 game:rooms send go-to-room  5 frames
create spot 12 allot
5 area-middle drop  spot vec!
game:fiona enter  her-at spot vec-copy  her-at $28020018 v-tri-in her-tri !  spot vec@ body-place-at  leave-actor
castle-gate-key 1 give
: room-actor ( -- id )  room-id room-name actor-named ;
: index-of ( id -- i )  64 0 do  dup 0 i item-at = if  drop i unloop exit  then  loop  drop -1 ;
T{ 0 castle-gate-key index-of use  1 frames  room-actor enter  1 slot-task @ 0<>  leave-actor  stalkers-stay state-flag? -> 4 -1 -1 }T
testing the chokers (her equipment's slot 3): $8C halves a fright and doubles her calming, and spares her the recovery's reset; $8B a quarter off, half again; none: as it is
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: choker! ( item -- )  progress pr.equipped 12 + l! ;
T{ $8C choker!  [: 10e choker-fear f>s  -10e choker-fear f>s  5 her-recovery !  reset-recovery  her-recovery @ ;] hers -> 5 -20 5 }T
T{ $8B choker!  [: 10e choker-fear 10e f* f>s  -10e choker-fear f>s ;] hers -> 75 -15 }T
T{ 0 choker!  [: 10e choker-fear f>s  reset-recovery  her-recovery @ ;] hers -> 10 1800 }T

testing the word plate (item $3F): at the golem (castle-2f-8, spot 6) EMETH - its flag (26), its event ($A), the plate spent
: plate ( addr len -- )   \ (a plate bearing the word)
    {: a n :}  8 0 do  i n < if  a i + c@  else  0  then  loop  game:items send give-plate  deliver ;
: stand-at ( area -- )   \ (her at the middle of the room's area)
    area-middle drop  spot vec!
    game:fiona enter  her-at spot vec-copy  her-at $28020018 v-tri-in her-tri !  spot vec@ body-place-at  leave-actor  2 frames ;
castle-2f-8 -1 game:rooms send go-to-room  5 frames
6 stand-at  s" EMETH" plate
T{ 0 word-plate-key index-of use  26 story-flag?  word-plate-key items-of -> 6 -1 0 }T
testing ... a word it doesn't know: event 5, no flag
s" HELLO" plate  27 frames
T{ 0 word-plate-key index-of use  25 story-flag? -> 6 0 }T
testing the elements (old-mansion-1f-12, spot 8): MERCURY - its flag ($4D) and event bit 0 (the word right); at no spot: nothing
old-mansion-1f-12 -1 game:rooms send go-to-room  5 frames
s" MERCURY" plate
T{ 0 word-plate-key index-of use -> 0 }T
8 stand-at
T{ 0 word-plate-key index-of use  1 frames  $4D story-flag?  room-actor enter  0 ebit?  leave-actor -> 6 -1 -1 }T

test-summary
