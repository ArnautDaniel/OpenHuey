\ The sub screen, the in-game menu (docs/subsystems/items.md; src/game/subscreen.c): Select opens
\ it and holds the world; its pages; an item's actions - used, thrown away, worn; closing; the
\ tab the story slides in with an item. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/items/test_subscreen.fs
IN: test-subscreen
USING: tester engine game-state actors messages keys facts flag-names story-names game items items.names subscreen subscreen.state fiona.state debug saves room-names ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: menu@ ( xt -- x )  game:subscreen enter  execute  leave-actor ;
: stage@ ( -- n )  [: stage @ ;] menu@ ;
: mode@ ( -- n )  [: mode @ ;] menu@ ;
: under-cursor ( -- id )  [: page @  dup cells cursors + @  item-at ;] menu@ ;
: give ( id n -- )  game:items send give-item  deliver ;
fvariable x0  fvariable z0
: mark ( -- )  game:fiona body-pos z0 f! fdrop x0 f! ;
: moved ( F: -- d )  game:fiona body-pos z0 f@ f- fsq fswap fdrop fswap x0 f@ f- fsq f+ fsqrt ;

100 frames
camomile 3 give  feather-boots 1 give  lavender 2 give
testing Select opens it (the world held: she doesn't walk), its first list
mark
key: Tab tap  10 frames
T{ stage@  [: page @ ;] menu@ -> 2 0 }T
key: W true key-hold  20 frames  key: W false key-hold
T{ moved 0.01e f< -> -1 }T

testing R1 turns to the next list; the cross opens the item's actions (on its first: use)
key: E tap
T{ [: page @ ;] menu@  under-cursor -> 1 camomile }T
key: X tap
T{ mode@  [: action @ ;] menu@ -> 1 0 }T
testing used: the items actor does it, "Used" said; one spent
key: X tap  2 frames
T{ mode@  [: shown @ ;] menu@  camomile items-of -> 3 $4055 2 }T
key: X tap
T{ mode@ -> 0 }T

testing thrown away: asked first, then one gone
key: X tap  key: Down tap  key: X tap
T{ mode@  [: shown @ ;] menu@ -> 3 $4056 }T
key: X tap  2 frames
T{ camomile items-of  [: shown @ ;] menu@ -> 1 $4057 }T
key: X tap

testing worn: the boots (their first action: wear), in her boots' slot
key: Down tap  key: X tap
T{ under-cursor  mode@ -> feather-boots 1 }T
key: X tap  2 frames
T{ 0 equipped  mode@ -> feather-boots 0 }T

testing the pages round (the original's order): L1 from the first list the options, then the files, the map
key: Q tap  key: Q tap
T{ [: page @ ;] menu@ -> 5 }T
key: Q tap
T{ [: page @ ;] menu@ -> 3 }T
key: Q tap
T{ [: page @ ;] menu@ -> 4 }T
testing the map: none held - nothing to turn to; her room found on its page (the front garden)
T{ [: map-at @  her-map @  her-pg @ ;] menu@ -> 0 0 0 }T
castle-map-held story-flag-set  key: Right tap
T{ [: map-at @ map-pg @ ;] menu@ -> 0 1 }T
key: Left tap  key: Left tap
T{ [: map-at @ map-pg @ ;] menu@ -> 0 4 }T
testing the options: the cross edits a row, the arrows change it; the square puts them back
key: E tap  key: E tap
T{ [: page @ ;] menu@ -> 5 }T
key: Down tap  key: Down tap  key: Down tap  key: X tap  key: Left tap  key: Left tap  key: X tap
T{ [: opt-row @ volume @ opt-editing @ ;] menu@ -> 3 96 0 }T
key: Z tap  key: X tap
T{ [: volume @ ;] menu@ -> 128 }T
key: E tap
testing triangle closes it: the world goes on
key: V tap  10 frames
T{ stage@ -> 0 }T
key: W true key-hold  20 frames  key: W false key-hold
T{ moved 0.5e f> -> -1 }T

testing locked, Select does nothing
subscreen-locked state-flag-set  key: Tab tap  5 frames
T{ stage@ -> 0 }T
subscreen-locked state-flag-clear

testing the tab: the item's, sliding in (told when in), shown, out (told when out)
state: asker-state  cell field told  end-state
behaviour asking
  on tab-done ( -- )  1 told +! ;
end-behaviour
asking asker-state s" asker" spawn constant asker
: ask ( op item -- )  asker enter  game:subscreen send tab-op  leave-actor  deliver ;
: told@ ( -- n )  asker enter  told @  leave-actor ;
0 camomile ask  1 frames
T{ told@ -> 1 }T
2 camomile ask  5 frames
T{ told@  [: tab @ ;] menu@ -> 1 2 }T
20 frames
T{ told@ -> 2 }T
4 camomile ask  25 frames
T{ told@  [: tab @ ;] menu@ -> 3 0 }T

testing the word plate (the scripts' 0x6D, mode 2): letters typed, one taken back, done makes the plate
: plate-open ( -- )  2 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 frames ;
: key-to ( at -- )  [: key-at ! ;] menu@ ;
plate-open
T{ stage@ mode@ -> 2 4 }T
$15 key-to key: X tap  $02 key-to key: X tap  $02 key-to key: X tap
T{ [: word-len @  word c@  word 1+ c@ ;] menu@ -> 3 'H' 'E' }T
key: V tap
T{ [: word-len @ ;] menu@ -> 2 }T
key: Right tap
T{ [: key-at @ ;] menu@ -> 3 }T
$27 key-to key: X tap  20 frames
T{ stage@  plates  plate-made story-flag?  subscreen-wanted state-flag? -> 0 1 -1 0 }T
T{ 0 0 listed 1- held-at letters c@  0 0 listed 1- held-at letters 1+ c@ -> 'H' 'E' }T

testing the slot machine (mode 3): none to put in - says so, closes
3 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 frames
T{ stage@ mode@ [: slot-state @ ;] menu@ -> 2 5 1 }T
key: X tap  20 frames
T{ stage@ -> 0 }T
testing ... a medallion put in (one spent), the rows rolled - the cross stops each - something forged
medallion 2 give
3 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 frames
T{ [: slot-state @  stock @ ;] menu@ -> 0 2 }T
key: X tap  2 frames
T{ [: slot-state @ medal @ ;] menu@  medallion items-of -> 2 0 1 }T
: stop-rows ( -- )  60 0 do  [: slot-state @ ;] menu@ 2 <> if  leave  then  key: X tap  8 frames  loop ;
stop-rows
T{ [: slot-state @  row-at @ ;] menu@ -> 3 9 }T
T{ [: true  10 0 do  i cells rows + @ 0< if  drop false  then  loop ;] menu@ -> -1 }T
variable had  variable forged-id  variable forged-n
[: result @ ;] menu@ drop  [: forged @ ;] menu@ forged-id !  [: amount @ ;] menu@ forged-n !
: held-of ( id -- n )  dup 0< if  drop 0  else  items-of  then ;
forged-id @ held-of had !
40 frames  key: X tap  5 frames
T{ [: slot-state @ ;] menu@ -> 0 }T
: forged-given? ( -- flag )   \ (nothing forged, or the amount forged added)
    [: result @ ;] menu@ 0< if  true  else  forged-id @ items-of  had @ forged-n @ + 99 min =  then ;
T{ forged-given? -> -1 }T
key: V tap  20 frames
testing a save point (the scripts' 0x6D, mode 1): the slots listed; the cross saves in the second - its place, the time played; "Saved", then closed
: saving-open ( -- )  1 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 frames ;
: step@ ( -- n )  [: save-step @ ;] menu@ ;
saving-open
T{ stage@  mode@  [: save-at @ ;] menu@ -> 2 on-saving 0 }T
key: Down tap  key: X tap  5 frames
T{ step@  1 slot-used?  1 slot-path file-exists?  1 slot-frames@ progress pr.play-frames l@ = -> 3 -1 -1 -1 }T
key: X tap  20 frames
T{ stage@  subscreen-wanted state-flag? -> 0 0 }T
testing a used slot: asked first (no picked first) - no leaves it be, yes writes over it; the triangle leaves
saving-open  key: Down tap  key: X tap
T{ step@  [: save-yes @ ;] menu@ -> 1 0 }T
key: X tap
T{ step@ -> 0 }T
key: X tap  key: Right tap  key: X tap  5 frames
T{ step@ -> 3 }T
key: X tap  20 frames
saving-open  key: V tap  20 frames
T{ stage@ -> 0 }T
testing loaded back from the slot: the room it was saved in
room-id constant saved-room
front-garden-2 -1 game:rooms send go-to-room  20 frames
1 game:saves send load-from-slot  deliver  40 frames
T{ room-id saved-room =  stage@ -> -1 0 }T   \ (not the save screen again)

testing the wardrobe (the scripts' 0x6D, mode 7, in castle-2f-3): her costumes - the locked one (7, until the first ending) passed by; one picked and worn at Exit (variable $26; what she wore in $28)
castle-2f-3 -1 game:rooms send go-to-room  20 frames
: pvar ( n -- v )  progress pr.vars + c@ ;
0 progress pr.vars $26 + c!
7 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 frames
T{ mode@  [: cost-row @  cost-her-worn @  cost-his-worn @ ;] menu@ -> on-costumes 0 0 -1 }T
key: X tap  key: Down tap  key: Down tap
T{ [: cost-open @  cost-her-at @ ;] menu@ -> 1 2 }T
key: Down tap
T{ [: cost-her-at @ ;] menu@ -> 4 }T   \ (past 7, locked)
key: Up tap  key: X tap
T{ [: cost-open @  cost-her-worn @ ;] menu@ -> 0 2 }T
key: Down tap
T{ [: cost-row @ ;] menu@ -> 2 }T   \ (no Hewie row: Exit)
key: X tap  20 frames
T{ stage@  $26 pvar  $28 pvar -> 0 3 0 }T

test-summary
