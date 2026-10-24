# Items and the sub screen

## Purpose

What Fiona carries and wears, the files she finds, and using them: the original's item manager
(`src/game/items.c`, its classes; `story_chars.c`, some items' uses) and the sub screen, the
in-game menu (`src/game/subscreen.c`). Items open the way on (keys at their places, the ores on
their pedestals, the books on their lecterns), calm her (the remedies), feed Hewie (the baits),
and change what she wears (boots, earrings, chokers, costumes).

## Phases

| Phase | What |
|---|---|
| **I1** | The inventory (`scripts/items.fs`): the three lists, stacks and counters, what she wears, the files, the marks; each item's use; the counters and timed effects; the story's item words | 
| **I2** | The sub screen: opening it, the item pages (the grid, use / equip / discard / examine, the questions), the files page, the "got an item" tab |
| **I3** | The map page, the options, the word plate, the synthesizer (the slot machine and the pot), saving |

## API (I1)

Declared in `scripts/messages.fs`.

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `give-item` | `( id n -- )` | in | n of item id (Items_Give): onto its stack (99 at most) or into its list's first free place. |
| `take-item` | `( id -- )` | in | One of it spent, wherever it is (Items_UseId; the scripts' 0x0C). |
| `add-file` | `( id sound? -- )` | in | A file found (message ids: key things, documents), with the pickup sound if new. |
| `use-item` | `( list place -- )` | in (the menu) | The item there used by its rule; answered with `item-used`. |
| `item-used` | `( id result -- )` | out | 0 nothing, 1 used up, 2 a mark set, 4 an event started (the menu closes on it), 5 she / Hewie act, 8 a hint only. |
| `discard-item`, `equip-item`, `unequip-item` | `( list place -- )` | in (the menu) | Thrown away (one); worn in its kind's slot; taken off. |
| `sort-items` | `( list -- )` | in (the menu) | Put in order (Items_Sort). |
| `item-bit` | `( n -- )` | in | Item n marked as used (its note changes). |
| `equipment` | `( boots earrings costume choker -- )` | broadcast | What she wears changed. |
| `item-event` | `( who ev var value -- )` | broadcast → the room | An item used here: character slot who's action script ev (a script variable given the item first). |
| `remedy` | `( fear recovery -- )` | → Fiona | Her fear and recovery's count changed (item_meters). |
| `soothe` | `( calm fear -- )` | → the panic | Calming at once, and fear (the progress' +0x7E0 / +0x7D8). |
| `fed` | `( how -- )` | → Hewie | A bait: 0 a jerky, 1 a big one, 2 an onion, 3 a calming bait, 4 one that gets him up. |

**Facts** (the progress, read by anyone; only the items actor changes them): `held-at ( list
i -- addr )` and `id@ n@`, `item-at`, `listed`, `items-of ( id -- n )`, `find-item`,
`equipped ( slot -- id )`, `equip-state`, `files`, `file@`, `item-bit?`, `boost-on?` /
`boost-value`; the classes (`items/tables.fs`, from the executable by `tools/itemtables.py`):
`item-kind`, `item-actions`, `item-held`, `item-sort`, `item-picture`. Items by name:
`items/names.fs` (their names in the game's text).

## Rules (I1)

1. **The lists**: ids under $40, under $A0, the rest; 64 places each, filled from the front;
   a removed item's list closes up (Items_Remove).
2. **Held** three ways (the class's +0x18): one (a key; given again it is there twice), a stack
   (1..99; a full one takes no more), a counter (its frames: the Fresh Flowers wither after 9000
   into the Withered Flowers, `flowers-withered` set).
3. **Worn**: a slot per kind (kItemKindSlot): 4 boots, 5 earrings, 6 the costume, 9 the choker.
   One spent or thrown away is off.
4. **Using** (each class's +0x3C): the rules in `items.fs` `use-rule`, from items.c and
   story_chars.c. A key, an ore, a book at its place starts the room's action script for her
   (most also set `stalkers-stay`); away from its place one on the altar (the water tower's spot
   $1B: script variable 1 the item, event $14); some file a hint instead (with its sound). The
   remedies change her fear and recovery, calm the panic, or start a timed effect. A bait feeds
   Hewie only at hand (hers to command, within 20, reachable in a step): her event $8E, his
   $8F / $90 / $91. Results 1 and 2 spend one.
5. **Timed effects** (the progress' +0x9E0..; they run, and run down, only while no cutscene has
   the camera - StatusTimers_Frame): Lavender calms the panic 0.27 a frame for 300 frames (into
   its weighed calming, `calm-add`), Torva doubles the panic's fear inputs, Fortis holds her fear
   at 0 for 1800 frames (`fear-to` each frame) and triples her blows' damage, Weltigo fades
   everything heard (the sound, the music, the movies: `world-volume!`, under the options'
   volume) out over 30 frames, silent, back in over the last 30 of its 1800 - told only as it
   changes, so a room change (everything full again) is heard until it fades back in -
   Sylvesta Esca Hewie's bites; the earrings scale the panic's calming (the
   Inquisitor's and the Philosopher's 1.25, the Alchemist's 2).
6. **The story's words**: `item-give` (0x0D: a file with its sound; in the other game - story
   flag $AF - only ids with bit $8000), `item-add` (0x0E), `item-give-count` (0x83),
   `item-use` (0x0C), `item-count?` (cond 0x39), `subscreen-bit?` (cond 0x5E), `reward-item`
   (0xD8: a hint by how Hewie has been treated), `stalker-gift` (0xB6: the stalker's row). A
   give lands at the next delivery.

## API (I2: the sub screen, `scripts/subscreen.fs`)

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `menu-mode` | `( mode -- )` | in (the story's 0x6D) | The mode the scripts open it in (with `subscreen-wanted`): 0 the menu, 1 a save point's save slots, 5 the options alone (the title's), 2 the word plate, 3 the slot machine, 7 the wardrobe; 9 the clear-data save shuts at once (hg writes only the system data then - OpenHuey keeps it written as it changes); the extras' (8 the movies, $B..$F the galleries, $C a challenge's results) not yet; 4 (the pot) no script asks for. |
| `game-saved` | `( -- )` | in, from saves | The save point's save is written (`save-to-slot ( n place )` asked it). |
| `tab-op` | `( op item -- )` | in (the story's 0xBE) | The tab: 0 the item handed her (its picture loaded), 1 loaded, 2 slide in, 3 shown, 4 slide out. |
| `tab-done` | `( -- )` | out, to who asked | That step is done (2 / 4 once the tab is in / out; the others at once). |
| `use-item` ... | | out → the items | (I1's) |

**Reads** the buttons (`keys.fs`; the original's menu buttons, Pads_FaceButtons): cross confirms,
triangle backs out, square sorts, L1 / R1 turn the pages, the arrows (or WASD) move - held, again
after 12 frames then every 2 - and Select (Tab) opens and closes it. **Facts**: the inventory,
the state flags (`subscreen-locked`, `world-held`, `subscreen-wanted`). **Draws** with the
original's art (`subscreen/tables.fs` from `tools/subscreentables.py`: SUBBASE's parts in the
cache's group $18, SUBBACK's panels in $19, the item's picture ITEMnn/ITEM_xxx in $27) on UI
layers 10 and 11, the PS2's 512 x 448 in a 4:3 frame. The engine's `tex-bank` loads a bank into a
group; `ui-image` draws a part of one of its textures, `ui-blend` blends what was drawn last as the
GS's ALPHA does with a fixed alpha (FIX): subtracted (Cd - Cs x FIX), mixed, added. Blended as the
original: the panels mixed by their alpha (SubScreen_DrawPanel, 0x64), each part by its table
entry's last column (SubScreen_DrawPart's kAlpha: by its own alpha, subtracted - the item's picture,
dark on the paper -, mixed - the page arrows -, added - the corners, the lit key); the words by the
colour byte (`art.fs` `inked`, Task_DrawGlyph): its low 6 bits the font's palette, $80 subtracted
(palettes 4..6 by their alpha), $40 by their alpha, else added - the menu's own words $80 (dark on
the paper; the chosen $82), the boxes' words 0 (light on the shade), the wardrobe's, the slot
machine's and the extras' 0 / 1 / 2 added, the cursor's hand 9; drawn in three quarters of the
glyph's colour (its vertex colour $60). The box's lines honour 0x12 / 0x14 (centred in, or to the
right of, the page's widest line), as the message window does.

## Rules (I2)

1. **Opening** (SceneGame_SubPlay): Select while the menu isn't locked, the world isn't held, no
   scene runs, the panic is below 4 and Fiona is free; or the scripts' 0x6D. The world is held
   (`pause!`) and the menu has the held frames: it fades in over 8 frames, out over 4.
   The sound goes down with it and back up as it closes (sub_set_volume: 1 - its fade / 255 -
   the effects' master, the stream by `music-duck`, the stage music; about half while open).
2. **Its pages**: the three item lists (16 to a page, columns of 8), then the files (two to a
   page); L1 / R1 turn them round. (The original's map and options pages come between: I3.)
3. **An item** (SubScreen_StateItemActions): its picture slides in with its name, count and
   note (the note of one not yet used, for those whose note changes: message $13D); its actions
   are those its class allows - use, wear / take off, throw away - and back. Throwing away asks
   first; wearing where another is worn asks first, naming it; a document is read (its message)
   before it is used.
4. **Using** (item_use): a key thing (kinds 0, 1) can't be used in a chase or with the stalker in
   her room ($54). What came of it: nothing ($52, the buzzer - or no buzzer for a hint); an event
   - marked used, the menu closes quietly onto it; used up (a remedy, or a mark set) - marked,
   "Used [it]" ($55); otherwise back to the list.
5. **The tab** (SubScreen_DrawTab): slides in from the left by 4 a frame to 64, stays, slides out;
   the tab and the item's picture, fading with the slide.

## I3: the map, the options, the word plate, the slot machine

Files: `scripts/subscreen.fs` (the actor: opening, closing, which page has the keys, the tab) and
`scripts/subscreen/`: `state.fs` (its fields, named stages / modes / pages), `art.fs` (drawing:
panels, parts, text, in the 4:3 frame), `input.fs` (keys, sounds, turning pages), `box.fs` (the
message box), `lists.fs` (the item lists, an item's actions, the files), `map.fs`, `options.fs`,
`plate.fs`, `slots.fs`; `tables.fs` generated by `tools/subscreentables.py`.

1. **The map** (Map_*): the maps she holds - a sketch used for each (story flags
   `castle-map-held` ..) - page by page with left / right, past the ends to the next map she
   holds; it opens on her room's page with an arrow where she stands, turned to her heading
   (Map_HereArrow: base texture 0 at $1C0, $60, palette 6, on four corners - `ui-image4` - at a
   quarter's alpha, $20); its title centred on $A6 by its width, its top at $48; in the
   forest's maze rooms ($100..$105) placed by the room's place in the dealt row
   (Events_ThingPlace: one of three spots of their own, none while no rows are dealt). The page
   she is on is kept in a save (its +0x18F0) and turned to on loading. Each room entered is found on the maps (`arrived`); the story's 0xD1 (`map-turn-to`)
   shows her room on another page of her map.
2. **The options** (Options_*): the controller type (its button names; the keyboard here),
   vibration, the sound out, the volume, the screen's position - the cross edits a row, the square
   resets them. Kept in the system data (saves.fs: `options-changed` to saves at each change,
   `options-restore` back as the game starts - the original's +0x30); the vibration turns the pad's rumble on or off (on: a buzz)
   and the volume scales everything heard (SubScreen_ApplyOptions); the sound out and the
   position have nothing here to change.
3. **The word plate** (the scripts' mode 2, SubScreen_StateWordMake): a word of up to 8 letters on
   a QWERTY plate; done makes a plate bearing it (item $3F, `give-plate`; at most 10) and sets
   `plate-made`; in the golem rooms quietly, with the machine's sounds.
4. **The slot machine** (the scripts' mode 3, SlotMachine_*): a medallion put in, ten rows rolled
   (the top by the medallion's odds, each other's reel picked by them; the cross stops a spinning
   reel, which stops itself at full speed); the 22 cells score where two rows match (5 wild); the
   symbols' counts give a result, the result an item (the boots, earrings and chokers only once;
   nothing left: the Magist Medallion) in an amount by how many matched.

## Status

**I1 built** (`scripts/items.fs`, `tests/items/test_items.fs` (40)). What she wears: her boots
(slot 0) - $80 her steps softer and heard less (`fiona/moving.fs`), $81..$83 her kicks
(`fiona/defend.fs`); her choker (slot 3, `fiona/moving.fs` choker-fear / reset-recovery) - $8A
a quarter off each fright and the recovery's reset three times in four; $8B a quarter off, her
calming half again, the reset half the time; $8C half off, her calming doubled, never the reset,
nor running's fright; **I2, I3 built**
(`scripts/subscreen.fs`, `tests/items/test_subscreen.fs` (62)). Not yet: the words' fades by the task's alpha (the gallery bars' and
page counts' `0x80 - fade`: drawn at the menu's). **The galleries** (`subscreen/galleries.fs`, the extras
room's modes; their tables generated from the executable by `tools/gallerytables.py` into
`subscreen/gallery-tables.fs`; `tests/items/test_galleries.fs` (35)): eight a page, "???" until the
system data unlocks each (the resident flags: the original's +0x24 bits on), the extras room's
own messages, each gallery's .TEX in the background group. The movies (8, SubScreen_StateMovies):
each one's thumbnail (SUBSCR/THUMBNAIL.TXS, `thumbnail`); one seen picked into the story's $2A,
the cursor into $2B - the room's script replays it. The models ($B, Gallery_*Model*): a figure
unlocked loaded (its model, textures and markers, its kind's motion table) and drawn alone on the
original's backdrop (`alone`; the room's look plain, `look-plain`) in an ambient light and one
from the camera's way, 15 degrees up and 30 round; ✕ its motion again, R1 / L1 its next /
previous, W / S nearer / further, A / D round, I / K up / down (the sticks), select the bars,
start the help. The music ($D, Gallery_StateMusic): a track played (the music stopped, the track
started once the stream is free - the music actor's background track now steps while the menu
holds the world); △ stops it, or leaves. The art ($E, Gallery_StateArt*): a picture unpacked
(SUBSCR/ARTnn.LEN's runs, `art-picture` - data/artpic.c) and looked over: the arrows a pixel, WASD
eight, F (R3) whole size, the circle back to the middle (the original's L3: no key for it), ✕ the
moves turned round. The TYPE ranks ($F, SubScreen_StateEntries): each one's message in the box,
then - unlocked - the one after it. The dogs idle with Hewie's mouth, ears and tail overlays ($1F00, $2000, $2102); the costumes,
Daniella and the rest hang their cloth as in play (picked by their model). *Not kept*: the
variants' per-part draw settings (Daniella's and the tint stalker's entries 11 / 12, 15 / 16;
costumes 7 / 6 at $E00) - as the cast's, none are kept; the second texture sets (`*_200`) no
figure shows without. **The mini-game** (`subscreen/extras.fs`): its menu
(mode $A: the introduction, challenges A..D - C and D once A and B are cleared, resident flags
$52 / $53 - with their three best times, exit; the pick into variable $2E) and its results (mode
$C: the unlock, the time placed among the challenge's best, shown a step every 30 frames with the
avoid prompt's pictures); the best times kept with the unlocks in the system data (`facts.fs`
`run-records`, saves.fs `record!`). Drawn with the original's art (SUBSCR/SUBMG in the
background group: the parts' textures 1..3 resolve to it). In the run the menu opens to the options alone, with Hewie's button names. **The wardrobe** (mode 7,
`subscreen/costumes.fs`, castle-2f-3 / castle-2f-5): Fiona's six costumes (7 locked until the
first ending, 8 until the third), Hewie's three (when he is here: his third locked until a hard
game cleared with the TYPE Top Breeder or Dog Lover); chosen ones into variables $26 / $27, what
was worn kept in $28 / $29 - the room's script dresses them. Its names and notes are the
wardrobe room's own messages. A loaded save no longer reopens the save screen (the flag it was
made with is cleared). **The save points** (mode 1, `subscreen/saving.fs`; the
story's shared actions $84 / $85 ask first): eight slots, each a save file of its own
(`hga-save-<n>.fs`, as the quick save's) and an index of their places and times
(`hga-saves.fs`); the place is the original's (D_00412800: her room and map, a menu message
$300..); a used slot asked before it is written over; "Saved", then the menu closes. The title
has LOAD GAME: the same slots, one in use loaded (`load-from-slot`). The timed effects: Torva's (her fear doubled, Panic_Update), Fortis's (her strikes and her kick
on Hewie and the stalker x3 - the creatures' kick stays 5, as the original's), Sylvesta Esca's
(Hewie's bites half again), Lavender's (calming), Fortis's hold on her fear and Weltigo's
silence (StatusTimers_Frame, on SceneGame +0xA20 - the progress' +0x9E0). Paptisma's field is
written by its use and only counted down. (hg's reset leaves the blows' factor +0xA04 at 0,
which multiplied as it stands would make every kick and strike do nothing; OpenHuey reads it as a
boost - the base damage, x3 while Fortis lasts.) **The word plate** (item $3F, ItemClassF430_Use,
`plate-use`): the word it bears, by room and spot - the golem's EMETH / METH / SALTATIO
(castle-2f-8), MAGNUS (castle-2f-12), REST (castle-1f-4), the elements MERCURY / SULFUR / SALT
(old-mansion-1f-12: event bit 0 whether the word was one not yet given), the word itself into
the script's variables (old-mansion-b1-6), ALCHYMIA / ADAMAS / POWDER / MORGAN (off-map-27);
its event started and the plate spent. The room takes `item-var` and `item-event-bit` for it.
(The inventory is the progress's: every save keeps it.) I2 and I3.
