\ The sub screen, the in-game menu (docs/subsystems/items.md; src/game/subscreen.c): Select opens
\ it and holds the world; its pages; an item's actions - used, thrown away, worn; closing; the
\ tab the story slides in with an item. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/items/test_subscreen.fs
IN: test-subscreen
USING: tester engine game-state actors messages keys facts flag-names story-names game items items.names subscreen subscreen.state fiona.state debug saves room-names music ;

: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: menu@ ( xt -- x )  game:subscreen enter  execute  leave-actor ;
: stage@ ( -- n )  [: stage @ ;] menu@ ;
: mode@ ( -- n )  [: mode @ ;] menu@ ;
: under-cursor ( -- id )  [: page @  dup cells cursors + @  item-at ;] menu@ ;
: give ( id n -- )  game:items send give-item  deliver ;
fvariable x0  fvariable z0
: mark ( -- )  game:fiona body-pos z0 f! fdrop x0 f! ;
: moved ( F: -- d )  game:fiona body-pos z0 f@ f- fsq fswap fdrop fswap x0 f@ f- fsq f+ fsqrt ;

100 run-frames
camomile 3 give  feather-boots 1 give  lavender 2 give
testing Select opens it (the world held: she doesn't walk), its first list
mark
key: Tab tap  10 run-frames
T{ stage@  [: page @ ;] menu@ -> 2 0 }T
key: W true key-hold  20 run-frames  key: W false key-hold
T{ moved 0.01e f< -> -1 }T
testing the stream turned down with it (sub_set_volume: 1 - 128 / 255)
: ducked ( -- n )  game:music enter  music:bgm-duck f@ 1000e f* fround f>s  leave-actor ;
T{ ducked -> 498 }T

testing R1 turns to the next list; the cross opens the item's actions (on its first: use)
key: E tap
T{ [: page @ ;] menu@  under-cursor -> 1 camomile }T
key: X tap
T{ mode@  [: action @ ;] menu@ -> 1 0 }T
testing used: the items actor does it, "Used" said; one spent
key: X tap  2 run-frames
T{ mode@  [: shown @ ;] menu@  camomile items-of -> 3 $4055 2 }T
key: X tap
T{ mode@ -> 0 }T

testing thrown away: asked first, then one gone
key: X tap  key: Down tap  key: X tap
T{ mode@  [: shown @ ;] menu@ -> 3 $4056 }T
key: X tap  2 run-frames
T{ camomile items-of  [: shown @ ;] menu@ -> 1 $4057 }T
key: X tap

testing worn: the boots (their first action: wear), in her boots' slot
key: Down tap  key: X tap
T{ under-cursor  mode@ -> feather-boots 1 }T
key: X tap  2 run-frames
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
testing the arrow in the forest's maze: by the room's place in the dealt row (Events_ThingPlace)
: var! ( v n -- )  progress pr.vars + c! ;
: row-room ( i -- room )  s" deal-rows" exe-table drop  5 3 * + + c@ $100 + ;   \ (row 5's)
0 $1F var!  T{ 0 row-room subscreen.map:maze-place -> -1 }T
1 $1F var!  5 $20 var!
T{ 0 row-room subscreen.map:maze-place  2 row-room subscreen.map:maze-place  $10B subscreen.map:maze-place -> 0 2 -1 }T
0 $1F var!  0 $20 var!
testing the map's arrow at a quarter (Map_HereArrow's $20808080); its title on $A6, $48 (Map_PageFrame)
T{ subscreen.map:arrow-alpha  subscreen.map:title-x  subscreen.map:title-y -> $20 $A6 $48 }T
testing the options: the cross edits a row, the arrows change it; the square puts them back
key: E tap  key: E tap
T{ [: page @ ;] menu@ -> 5 }T
key: Down tap  key: Down tap  key: Down tap  key: X tap  key: Left tap  key: Left tap  key: X tap
T{ [: opt-row @ volume @ opt-editing @ ;] menu@ -> 3 96 0 }T
key: Z tap  key: X tap
T{ [: volume @ ;] menu@ -> 128 }T
key: E tap
testing triangle closes it: the world goes on
key: V tap  10 run-frames
T{ stage@ ducked -> 0 1000 }T
key: W true key-hold  20 run-frames  key: W false key-hold
T{ moved 0.5e f> -> -1 }T

testing locked, Select does nothing
subscreen-locked state-flag-set  key: Tab tap  5 run-frames
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
0 camomile ask  1 run-frames
T{ told@ -> 1 }T
2 camomile ask  5 run-frames
T{ told@  [: tab @ ;] menu@ -> 1 2 }T
20 run-frames
T{ told@ -> 2 }T
4 camomile ask  25 run-frames
T{ told@  [: tab @ ;] menu@ -> 3 0 }T

testing the word plate (the scripts' 0x6D, mode 2): letters typed, one taken back, done makes the plate
: plate-open ( -- )  2 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 run-frames ;
: key-to ( at -- )  [: key-at ! ;] menu@ ;
plate-open
T{ stage@ mode@ -> 2 4 }T
$15 key-to key: X tap  $02 key-to key: X tap  $02 key-to key: X tap
T{ [: word-len @  word c@  word 1+ c@ ;] menu@ -> 3 'H' 'E' }T
key: V tap
T{ [: word-len @ ;] menu@ -> 2 }T
key: Right tap
T{ [: key-at @ ;] menu@ -> 3 }T
$27 key-to key: X tap  20 run-frames
T{ stage@  plates  plate-made story-flag?  subscreen-wanted state-flag? -> 0 1 -1 0 }T
T{ 0 0 listed 1- held-at letters c@  0 0 listed 1- held-at letters 1+ c@ -> 'H' 'E' }T

testing the slot machine (mode 3): none to put in - says so, closes
3 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 run-frames
T{ stage@ mode@ [: slot-state @ ;] menu@ -> 2 5 1 }T
key: X tap  20 run-frames
T{ stage@ -> 0 }T
testing ... a medallion put in (one spent), the rows rolled - the cross stops each - something forged
medallion 2 give
3 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 run-frames
T{ [: slot-state @  stock @ ;] menu@ -> 0 2 }T
key: X tap  2 run-frames
T{ [: slot-state @ medal @ ;] menu@  medallion items-of -> 2 0 1 }T
: stop-rows ( -- )  60 0 do  [: slot-state @ ;] menu@ 2 <> if  leave  then  key: X tap  8 run-frames  loop ;
stop-rows
T{ [: slot-state @  row-at @ ;] menu@ -> 3 9 }T
T{ [: true  10 0 do  i cells rows + @ 0< if  drop false  then  loop ;] menu@ -> -1 }T
variable had  variable forged-id  variable forged-n
[: result @ ;] menu@ drop  [: forged @ ;] menu@ forged-id !  [: amount @ ;] menu@ forged-n !
: held-of ( id -- n )  dup 0< if  drop 0  else  items-of  then ;
forged-id @ held-of had !
40 run-frames  key: X tap  5 run-frames
T{ [: slot-state @ ;] menu@ -> 0 }T
: forged-given? ( -- flag )   \ (nothing forged, or the amount forged added)
    [: result @ ;] menu@ 0< if  true  else  forged-id @ items-of  had @ forged-n @ + 99 min =  then ;
T{ forged-given? -> -1 }T
key: V tap  20 run-frames
testing a save point (the scripts' 0x6D, mode 1): the slots listed; the cross saves in the second - its place, the time played; "Saved", then closed
: saving-open ( -- )  1 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 run-frames ;
: step@ ( -- n )  [: save-step @ ;] menu@ ;
saving-open
T{ stage@  mode@  [: save-at @ ;] menu@ -> 2 on-saving 0 }T
key: Down tap  key: X tap  5 run-frames
T{ step@  1 slot-used?  1 slot-path file-exists?  1 slot-frames@ progress pr.play-frames l@ = -> 3 -1 -1 -1 }T
key: X tap  20 run-frames
T{ stage@  subscreen-wanted state-flag? -> 0 0 }T
testing a used slot: asked first (no picked first) - no leaves it be, yes writes over it; the triangle leaves
saving-open  key: Down tap  key: X tap
T{ step@  [: save-yes @ ;] menu@ -> 1 0 }T
key: X tap
T{ step@ -> 0 }T
key: X tap  key: Right tap  key: X tap  5 run-frames
T{ step@ -> 3 }T
key: X tap  20 run-frames
saving-open  key: V tap  20 run-frames
T{ stage@ -> 0 }T
testing loaded back from the slot: the room it was saved in
room-id constant saved-room
front-garden-2 -1 game:rooms send go-to-room  20 run-frames
1 game:saves send load-from-slot  deliver  40 run-frames
T{ room-id saved-room =  stage@ -> -1 0 }T   \ (not the save screen again)

testing the wardrobe (the scripts' 0x6D, mode 7, in castle-2f-3): her costumes - the locked one (7, until the first ending) passed by; one picked and worn at Exit (variable $26; what she wore in $28)
castle-2f-3 -1 game:rooms send go-to-room  20 run-frames
: pvar ( n -- v )  progress pr.vars + c@ ;
0 progress pr.vars $26 + c!
7 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 run-frames
T{ mode@  [: cost-row @  cost-her-worn @  cost-his-worn @ ;] menu@ -> on-costumes 0 0 -1 }T
key: X tap  key: Down tap  key: Down tap
T{ [: cost-open @  cost-her-at @ ;] menu@ -> 1 2 }T
key: Down tap
T{ [: cost-her-at @ ;] menu@ -> 4 }T   \ (past 7, locked)
key: Up tap  key: X tap
T{ [: cost-open @  cost-her-worn @ ;] menu@ -> 0 2 }T
key: Down tap
T{ [: cost-row @ ;] menu@ -> 2 }T   \ (no Hewie row: Exit)
key: X tap  20 run-frames
T{ stage@  $26 pvar  $28 pvar -> 0 3 0 }T

testing the words' blending by the colour byte (Task_DrawGlyph): $80 subtracted (palettes 4..6 by alpha), $40 by alpha, else added
USING: subscreen.art subscreen.tables ;
T{ menu-ink ink-blend  chosen-ink ink-blend  $85 ink-blend  $40 ink-blend  box-ink ink-blend  cursor-ink ink-blend
   -> subtracted subtracted by-alpha by-alpha added added }T
testing the parts' blending (SubScreen_DrawPart's kAlpha by the table's last column): the item picture subtracted, the arrows mixed, the corners added
: part-blend ( i -- n )  /part * parts + 8 cells + @ ;
T{ 11 part-blend  $1A part-blend  $15 part-blend  $A part-blend -> subtracted mixed added by-alpha }T
testing the box's lines: one after {right} (0x14) to the right of the page's widest (Task_DrawPage)
: box-x ( line -- x )  [: $403D shown !  0 shown-page !  subscreen.box:line-x ;] menu@ ;
: box-w ( line -- w )  [: subscreen.box:line-width ;] menu@ ;
: box-widest ( -- w )  [: subscreen.box:widest ;] menu@ ;
$403D message-layout drop  T{ 0 1 message-align  0 0 message-align -> 2 0 }T
: right-spare ( -- n )  1 box-x 0 box-x -  box-widest 1 box-w -  - ;   \ (0: the line's right at the widest's)
T{ right-spare -> 0 }T
[: -1 shown ! ;] menu@

test-summary
