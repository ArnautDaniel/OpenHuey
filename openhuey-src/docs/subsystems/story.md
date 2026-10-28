# The story

## Purpose

What happens in each room: the characters placed as they come in, the camera's setups as they
move through a room's parts, ways opened and closed, exits taken, scenes started, messages shown,
items handed out, the characters' scripted actions. The original runs them from bytecode
(`event.c`). new-src converted all 170 rooms' scripts to Forth once (`tools/events2forth.py`),
and OpenHuey ran those for a while; since 0.2.20 every room is a **place** written anew in the
kit's words (below), and the converted scripts and their ~460 opcode words are gone (they are in
the history, `scripts/story/rooms-ref/` and `scripts/story/words.fs`, for checking a place
against the original).

## Structure

| Actor | What |
|---|---|
| `story` (one) | Spawns the place of each room as it comes in (`arrived`), tells it when it is being left (`leaving-room`) and when the next room is in (`room-left`), and retires it once it reports done. A room with no place has no actor. |
| a place (named for its room: `front-garden-2`) | The played room's doings, as one actor: its description (`came-in`, `person-in`, `each-frame`, `went`), its own cells, its activities. |

Files: `scripts/story.fs` (the story actor), `scripts/story/kit.fs` (the words a place is
written in), `scripts/story/places/<room>.fs` (each place; shared pieces of an area in their own
files there - forest.fs, house-of-truth.fs, walkway.fs, bedroom.fs, bridge.fs, clock-run.fs),
`scripts/story/places.fs` (all loaded), `scripts/story/common.fs` (what every room does),
`scripts/story/base.fs` (what the story kept of the old machinery: the character slots - the
stalker the story loaded, the scenes' figures - the stalkers' gifts, the maze's deal, the screen
filled black for a frame, and a few pieces as the original has them), `scripts/story/clock.fs`
(the clock run's clock), `scripts/story/strings.fs` (the rooms' strings and the music's files).

## Places: rooms written anew

A room listed in `scripts/story/places.fs` is a **place** (docs/rooms-design.md): `story` spawns
it, and it says what happens in the room as a program. Every room with anything of the story's
has one.

- **The kit** (`scripts/story/kit.fs`, vocab `story.kit`): a place's state (`place-state`, its
  own cells by `own:`), its description (`' came-in ' person-in ' each-frame ' went room place!`),
  and the words a place is written in - facts, holds and moves, waits, messages, fades by name
  (`screen:to-black-2 with-music or`), areas, cameras, exits and doors, parts, sounds, scenes,
  figures, items, offers. No opcode words: a place sends messages and uses these.
- **Activities**: a place's longer doings are tasks it resumes each frame (`start`,
  `start-only`). Whoever an activity holds (`hold`: `scripted` to the character) is let go when
  it ends or is stopped (the room left, `room-left`); a move is sent only for a character the
  running activity holds, and `wait-move` waits only on a move it asked for.
- **Offers**: what she may do where she stands - `offer` (an activity, Fiona held for it),
  `offer-remark` - the first offered in a frame wins; `unhurried`, `calm-only`,
  `calm-hewie-up`, `any-time` as the original's kinds.
- **Who is in**: the place keeps where each person was at the last frame's end (areas crossed)
  and what they last told (`fiona-doing`, `hewie-doing`, `stalker-doing`, `creature-doing`).
  `story` keeps the stalker's last report and gives it to a new place before `room-enter` (the
  original reads him directly as a character enters); the stalker counts as in once he has told
  the place what he is.
- **Story facts** by name: `fact:` in `story-names.fs`; saves write `fact <name>`.

**Scenes** are the `scenes` actor's (`scripts/scenes.fs`): `scene-load ( room movie class cutscene )`
→ `scene-ready ( movie? )`; `scene-run ( subtitles )` → `scene-shot ( shot )` each new shot →
`scene-over` (played through or skipped: the world held, the camera back); `scene-run-through
( subtitles )` the game overs' (never skipped; at its end the movie's last frame left up, the
director as it is, for the game over to hold: Scene_PlayThrough); `scene-stop` (the place gone).
The kit's `load-scene`, `run-scene`, `run-through`, `at-shots`. (Headless runs go by the game
clock, so a movie ends with its video there, not waiting for its sound to drain in real time.)

**What most rooms have** (the kit's components, each room's data): a breakable (`breakable:`
names it - its fact, whole and broken parts, the cylinder a blow breaks it in; `shard-look`,
`shard-uv`, `frees-floor`, `noise-from`; `as-it-stands` as she comes in, `breaks?` each frame -
the parts swapped, its floor freed, shards, the shatter $88, a noise); the ways in (`ways-in`: a
camera set and path per exit - placed at the one she came by - or `way-in` for one), the ways out
(`exits-out`, `ways-out`: an area and the exit loaded ahead), the area cameras (`cameras-at`: area,
set, path; `camera-out` for one taken crossing out of its area); what Hewie looks up at (`look,` builds a table of zones - x y z, radius, height, how
high - and `hewie-looks-at` runs it, while he is calm and at her command: the rooms' 0xC5); a
replayed movie over (`replay-over`: back to the extras by the forced way); an item found
(`picks-up`: turned to it, bent down, taken - `takes-up` the taking: named, full or given, its
tab and fanfare); out by an area (`leaves-by`); Hewie's whine from the wire (`whine-due?`); an
animal figure sent off and waited for (`figure-goes`); anything's centre in a zone across
(`centre-across?`, the rooms' zone bit 8 - `here?` and the zone words know only the people); her
own blow in a zone (`her-blow-in?`); hung things - the chimes she brushes walking by (`chimes`,
`chimes-rest`: hangers_swing) and one struck swaying away from who struck it till still
(`sway-start`, `sways`); a hiding place - the rooms' usual cupboard (`hides-in`: her motions'
file, the spot she slips in from, the camera while in; the room's motions 4 the walk there, 5 in,
1 lying low, 2 out, 3 hauled out; the room's camera while she's in, following nobody; out by the circle button, `slips-out`, or found, panicking or
him in, `hauled-out`; each frame `hidden-calm`), its stalker (`came-on-her?` as he comes in;
`searches`: the odds by a story variable - `finding-odds` 25, 50, 75 - and `will-find?`, a creature
moving about, his alert 4; `door-for-him` opens a shut door he came by; `finds-her-at`: to the
spot, Debilitas looking in, the others seeing her; the place's `found-her`, `came-while-hidden`);
a placed object back as it was set (`object-reset`); the push-boxes (`obstacle-in`,
`obstacle-fixed`, `obstacle-on-tri?`, `obstacle-kept`, `obstacle-keep`); a reflecting quad (`glass-in`: over the whole picture or on the quad, its strength's top bits side
panels - `window-panels`, `door-panels`; `window-in`, `door-glass-in`, `window-over-all`), a scene's
lit quad over the picture (`lit-over-all`); she weak since the chapter turned, till she rests
(`weak?`; `reaching-to-act?`: a button, a command; `weakness` starts the room's flagging as she
does, her fear kept at 99); Daniella's back for her hair (`her-back`: 0 standing, 1 flat, 2 bent); an exit's door held (`exit-held?`: none, not); Daniella's held things away or back
(`hands-empty`, `hands-back`); "Used ..." (`used-msg`); a door tried from beyond (`door-tried`);
another word plate to press (`plate-pressable?`); a thing wandering about its place (`wanderer-at`);
a face's texture from a file (`face-on`); the stalker free (`stalker-free?`: here, not struck, held
or linked, not leading her, not in a doorway - cond 0x65); his grab of Hewie from behind asked
(`grabs-hewie?`, his yes a frame on); Hewie's look on a point now (`hewie-eyes-on`); dust of a kind and colour (`dust-of`); an activity
stopped by its word (`stop-doing`); by one of the forced ways
(`forced-way?`: $80 .. the last given - not a room entered by none, -1); one triangle's floor flags
(`floor-tri-on`); before an exit's door (`before-door?`); within reach across, whatever the height
(`reaches-across?`, the zone bit 2); the camera's near clip (`near-clip`); the rooms' four ambient sounds in turn
(`ambient-sounds`); her request at a locked door (`locked-request?`), a locked door tried
(`tries-locked`), a key turned in one (`key-turns`); the boxes' squares kept and models put back
(`obstacle-kept-at`, `obstacle-model-home`); an object put somewhere (`object-to`); a thing there is
one of taken (`takes-the`); the exit Hewie came in by (`hewie-came-by?`, the `hewie-came` message);
Fiona's move part (`fiona-sub-now`); a blow in a cylinder by anyone (`blow-in?`) and the stalker's
blow's strength (`his-blow`); the other game's file for Hewie's trust (`trust-file`); the screen
darkened a frame (`screen-dark`); the spirits away (`spirits-gone`); the panic over (`panic-over`);
her fright raised up to
a level (`fright-up-to`: Panic_FrightRaw by what she is short of it); the stalker's move mode
(`st-mode-now`); an animation come round this time
(`went-round?`: its motion's $20 key, the original's cond 0x48); a scene's signal counted past
its nth this frame (`signal-passed?`, the original's cond 0x3B); the stalker's sound bank
(`stalker-bank`); a breakable's own shatter
(`shatters-with`); moths (`moths-at`: a swarm in an effect slot, the rooms' 0x35;
`moths-to` sends it off for another point, their 0x33; `moths-sound` keeps a sound where it
wanders); Hewie's wary stand at a point (`hewie-stands-by`, the shared $8A); an item used up
(`spend`); the map's page (`map-page`); a scare (`fright+`, her fright raised by so much); put on a triangle, its heading kept (`put-on-tri`); on a triangle with these floor flags (`on-floor-of?`); she thrown down
by someone (`throws-her`); a door taking on another's state (`door-as`); the summoner's wait his
kind's own (`his-own-time`); dials (`dial-turning`: the stick's message up, each number turned to, the rest sound and
the right one's click and glint, till she is done; `dial-set`, `dial-turns`, `dial-asked`: a number 0..6 in a progress
variable, 30 degrees each, the stick's repeat); an object catching the light (`catches-light`);
one character reflected in an object (`object-mirror`: MirrorFragment); who struck her (`struck-by-now`), Hewie's
side of a split room (`hewie-way-now`); the story's stalker in its slot, in the game or not (`partner`,
`partner-kind`); doors locked against a kind (`door-for`), back again (`reopened`), their opened
mark (`open-mark`); a creature let out turned (`creature-facing`); the
game over (`game-over`, its kind; `ending-var` which ending caught her). A place has the original's
32 effect slots. A
whole breakable is a target a stalker's blow may strike instead of her (`zone-target`, cleared as
each frame starts: Events_NearestPoint, as the original's zones of kind bit 1).

The old mansion's pieces: the hopping figure (`hopper:` - its hops, sounds, place; `hopper-in`,
`hopper-struck?`, `hops-back`, `hops-about`, `hopper-calls`, `hopper-gone`); the cabinet to hide in
(`cabinet:`, `cabinet-doors`, `cabinet-shown`, `hides-in-cabinet`, `cabinet-stalker-came`: Hewie
beside her, the stalker's grab or opening); a recovery point (`recovery-point`: its message, motions,
sound, its document the first time); a mandragora carried shrieking as she hides (`hide-shriek`,
`mandragora?`, `shrieks`); the mansion's finding odds (`mansion-odds`); doors closed off for good
(`closed-off?`); one triangle's floor flags off (`floor-tri-off`); fog (`fog`, `fog-off`); an arrow
pressed this frame (`arrow-pressed?`); the creatures - the homunculi on a nav group gone
(`homunculi-off`), the story's spirit let out (`story-spirit-in`), one in the game (`creature-up?`),
a body within reach across (`within-xz?`), a spirit put away there (`spirit-put-away-near?`), the
ones here all on level 0 (`creatures-grounded`); Hewie sitting with his head turned away
(`hewie-sits-refusing`, the shared $93); the word plate's answer and letters (`word-right-now`,
`word-vars`), her shakes counted (`shakes-now`); hangers pushed by anyone's step (`step-of`,
`chime-push`, `chime-swing`: Room48's are pushed by Hewie). An effect's slot: `live-effect` (0 is
none), `tell-effect`, and `dies-down` for one left to burn out on its own (forgotten, so a later
clear-up never kills an actor that took its id). A point on the screen: `point-seen?` - the rooms'
`point-on-camera? not`, their cond 0x4F being the camera's out-of-view test (+0xD4, as cond
0x45's); `point-unseen?` the other way.
- the old mansion (every room a place): its 1F (the scales, the transmuter, the dragon, the
  library's shelves, the garden of mandragoras, the graveyard - the graves, the mausoleum, the
  thicket Hewie lies hidden in and Daniella's ambush, the hand at the back fed the mandragoras,
  Hewie's errands for the glow and the Antimony Tube, the back door), its 2F (the laboratory's
  lockers, the gallery, the dial lock), its 3F (the boxes, Daniella's death; the armour's
  corridor), its B1 (the mirror and symbol doors, Daniella at the kiln, the trap workshop, the
  torture chamber, the golem race).
- the Chaos Forest (every room a place; `story/places/forest.fs` what they share - the maze dealt
  anew, as she comes in to a room never dealt and as her panic sets in; the bushes of -4 .. -7 to
  crouch in, the stalker's search of them by the odds, their leaves; the wisp startled by a noise or
  someone near; the rabbit): its edge (the hopper, the Forest Map), the clearing (Hewie found, seven
  crates, the sundial), the lake, the crossing, the way to its heart, Hewie shot (his hurt face, the
  bandage scene), the heart (Riccardo; dressed anew, on to the water tower). Kit: `start-holding`
  (an activity that holds someone from the frame it starts - nothing else takes them first),
  `loud-here?` (a loud noise made here last frame), `face-off`.
- the House of Truth (every room a place; `story/places/house-of-truth.fs` what they share - Lorenzo's
  slam shaking the camera (`slam-shake`, `slam-shake-of` a kind), the B1's voices, the colour rooms
  (the way on decided by the colours done, the inscription, the dark between them), Lorenzo coming in
  ahead (`appears`), the tremors once the house begins to come down (`tremors`: the violent ones with
  her quake, else slight), the holes in the walls (`hole-offer`, `out-of-hole`), the Sol Key turned
  (`sol-turned`)): its B1 (the colour rooms, the shower, the gas), its 1F (the blasted wall, the
  marker, the demolisher, the furnace, the trap, the pit - the cane set in, the house coming down,
  Lorenzo's fall), its 2F (the crates, the study). Kit: `steady` (a room's steady sound: started once
  its sounds are in, then kept up), `kept-by` (a sound kept on a moving character), `fell-now` (the
  debris fallen through: the things' `keri-fell`), `hewie-mode-now` (his move mode).
- the Water Tower (every room a place; `story/places/walkway.fs` what 2F and 4F share - the walkway
  turned by its lever, a `walkway:` each: which way, its height, the lever, Hewie's spots on it, what
  the room adds as it's across and once it has stopped): its 1F (the water, the clock), 2F (Riccardo
  heard prowling above, then come for her; back 1800 frames after he left), 4F (the drum falling, the
  doors open only while the walkway is across), the hall of the planets (the plates lit in turn while
  the one before is stood on, Hewie sitting on the ones he reaches and sent to the next), 7F-2, the
  top (the models worn by her blows, Hewie lying in wait in a hole - his grab of Riccardo, the final
  scenes; the diorama and its aiming cursor - the miniature brings Lorenzo; the Azoth scene). Kit:
  `summoner-due?` (its countdown run out: `summoner-secs-now`), `partner-is?`, `riccardo?`,
  `time-up-leaves` (Riccardo taken off when his time is up out of her sight), `creature-slot-in`,
  `hewie-side`, `grab-ready?` / `grab-forget` (his last answer to the grab from behind, kept),
  `far-clip`, `gs-tint` (the rooms' char-tint colour).
- the off-map rooms (every room a place): the clock run - `story/places/clock-run.fs` what its
  twenty rooms share ($D9 .. $D0 the forest's, $E0 .. $E9 the castle's: their rooms again, Hewie the
  one played, Fiona following): the clock shown each frame (`story/clock.fs` - its saved time kept
  by it now, `clock-draw-saved` given whether to show it), Hewie's ways ahead (`his-ways`,
  `his-way-back`), exits by their areas (`by-areas`), the run set out in $D9 / $E0 (`run-begins`,
  `music-taken`, `run-stage`), its goal in $D0 / $E9 (`at-goal`, `run-over`: the time kept, the
  results' menus, out to the extras room), the castle's hiding places in the run (`hide-cue?`:
  Fiona slips in at Hewie's command as he stands by one; `run-searches`, `run-finds-her`; left
  behind, `left-hidden`). The forest's copies use `story/places/forest.fs` (its wisp, rabbit and
  bush - the bush's floor flag now `bush-floor`, the leaves pushed by anyone: `leaves-pushed-by`).
  The ending scenes ($33 the ending D, $34 Lorenzo's, $35 the three with Hewie, $36 the ending A,
  each with its replays), the garden's hidden room ($27: the topping-up machine, the slot machine's
  jewellery, the book, the chests of clothes), the extras room ($37: the corners named on the floor
  - `show-text` straight to the window, not the place's message, so the action still works - the
  theatre, the mini-game's challenges with their first rooms, the galleries, the changing area, the
  exit; in from the title, back from a replay, back from the run), and $0D, $65 (its data has no
  exits), $64, $C4 and $5A (no data on the disc: never entered). Kit: exits by the one played
  (`player-free?`, `own-pace?`, `player-kind`, `passable?`, `exit-checked` - the rooms'
  exit-check: not locked, passable from their side, at their own pace; `exit-out` and `leaves-by`
  now go by it), Hewie's gesture while played (`gesture-now`, `played-gesture`; `wants-out?` - a
  hiding place left at his next command), `fiona-idle?` (Fiona_IsIdle), `stands-on?`,
  `scene-shot?` / `scene-at?` / `scene-near-end?` (a scene's shot, cue, end), `clothes-changed`
  (the costumes' menu closed: what she chose put on, his coat posed - `hewie-recoated`, from the
  bedroom).

**What every room does** (`scripts/story/common.fs`): the shared scripts the converted rooms ran
after their phases and for item uses - crouching ($81 / $82 / $87), the camera on the stalker
while she is half hidden, the mandragora's spell, the other game's rewards, searching the
stalker ($80, offered by the shared after-phase 2: down on his front - $1806 or its twitch
$1805, Pursuer_InStance2Anim, the converted rooms' cond 0x4B - she close, 2 more than their
sizes, and facing him within 50 degrees; she bends to him and finds the next of his kind's
gifts, 0xB6 - the first always, after it 1 in 10; none due, his time down is over and he gets
up), her drink ($8D), Hewie fed ($8E-$91). A place also starts as the converted rooms did (the room visited, no track
wanted till asked, the camera started for the room), its doings wait out her panic's onset, it
shows her locked door's message, and it takes its own item uses (`items-here`: the plates'
words, a marionette) before the shared ones. A doing whose character another doing takes ends,
as the original's action replaced. What she commanded Hewie this frame (`started-now`) and a
creature's request of her (`request-now`) are gone by the time the doings run: the frame's words
read them (and tell a doing, as castle-1f-2's gap and front-garden-3's box do).

Places, each with its test in `tests/places/`:
- castle-2f-1, the music room ($A): the third moon dial (turned by the stick; all three right, the
  moonlight's scene and down to castle-1f-12 by the forced way; brought back up set right, the moon
  door unlocked and the creatures loose), the moonlight's scene as she comes up with castle-1f-12's
  dials right, the door unlocked from this side, the piano, the speakers, the Refined Magnesia Hewie
  sniffs out, the Lavender, his looks up, the walls' shade, the replay (on to castle-1f-12's).
- castle-2f-2, the chandelier hall ($20): her first steps by the chandeliers (Debilitas's scene: he
  comes in and looks about, the file), the box barring the way (pushed home: fixed, the door beyond
  unlocked), the Bisque Doll knocked down by her blow, falling and landing (on open floor, picked
  up), after the telephone's warning the ticking beyond the door and the door tried (seized: the
  forced way; brought back pulled free, she flees - the file; its locked door then), the blood (a
  fright the first time), the floor, the angel painting, the dark corner, the Album Medallion Hewie
  sniffs out, his wary stand and sitting by, his looks up, the chandeliers swinging, two flames,
  the wheel's sounds, Debilitas's far shove, the creatures, the walls' shade, the replay.
- castle-2f-4, the little TV room ($31): brought in by the forced way from the bedroom's TV, her
  nightmare (a scene: the lit quad over the picture and her layer and shadow by its shots), out by
  the way loaded ahead; the TV, the fan, the replay.
- castle-2f-6, the study ($22): the desk locked (her blows on it), the Study Key used at it (the
  drawer, the Lunar Refractor; the stalker stronger, the summoner on), the board (the Luminessants),
  the shelves, the massive book and the desktop, the Lavender, Hewie's bark at the desk and his
  looks up, the far view's depth.
- castle-2f-7, the plate press room ($23): the plate pressing machine (the first time its scene and
  the press; a word plate made - its scene, the plate shown her, the file; again while fewer than ten
  are made, else none left; a plate given back), once the golem is done someone trying the door from
  beyond (a fright), brought in by the forced way Daniella's scene (her face as it turns to her), the
  birdcage in turn, the ruin, the thing wandering, Hewie's looks, the replays.
- castle-2f-9, the stair landing ($5): while the chapter has Daniella the maid, her cleaning here (on
  her knees scrubbing; spoken to she stands - "It's cleaning time now" - then her strangeness; taken
  as she leaves), her steady sound, Hewie wary of her and his look; the first time in, the screen
  back.
- castle-2f-10, the room of the four boxes ($17): the boxes pushed onto their squares (each fixed;
  all four, the scene - the way opened, the doors set anew - and out by the forced way), where they
  stand kept as she leaves, the replay.
- castle-2f-11, the corridor of the slanted crest ($1F): the doll and the swing rocking, the crest,
  the specks, Hewie's look up.
- the suspended bridge's hall (castle-2f-13, -14, -15: the same hall at three times) shares its
  bridge in `story/places/bridge.fs` (vocab story.places.bridge): the bridge and the statue in
  turn, Hewie's fetch of the Medallion across the gap and his calling back, the stalker's grab on
  the plank, Hewie's bark and look across, climbing down; each room names its far side's triangles
  as it comes in (`far-side!`).
- castle-2f-15, the hall with its far way open ($30): climbing down, the statue, the Prima Magnesia
  Hewie sniffs out, Daniella's motions loaded on the way out while she is the maid, the bridge's
  Hewie (on his side of the hall), the map's pages.
- castle-2f-12, the nursery ($26): the wardrobe to hide in (Hewie, near, in with her; out by the
  button, in a panic, or found - the stalker rattling it with her shaking inside, Debilitas
  dragging her out, the others seeing her and she runs), Debilitas by the rocking chair; the three
  rocking chairs (set rocking by a blow, the first a while the first time; Hewie watching it), the
  box and its slot (the right thing: it opens, the Inquisitor Earrings and the Jade Choker; else
  nothing), the caged doll and the rocking horse in turn, the doll in the chair, the crib, the
  Refined Antimony Hewie sniffs out, the vase's Medallion.
- castle-2f-13, the suspended bridge's hall before the great hall's balcony fell ($28): out on the
  balcony a while (she, or the stalker after her), it gives - to black, the doors set anew, down to
  the great hall by the forced way (her way the balcony's fall, his her seizing), Hewie sent there
  or kept here; the bridge, Hewie's fetch of the Medallion and his calling back, the stalker's grab
  on the plank, the statue as castle-2f-14; Hewie wary on the balcony, the sparkles.
- castle-2f-14, the suspended bridge's hall ($2F): the first time in, the screen back (the crows'
  file); the raised bridge in turn; Hewie pointed across the gap leaps over to the Medallion and
  brings it back (it had, he lies down over there); called back he leaps back; the stalker grabbing
  him from behind on the plank; climbing down to the great hall (Hewie left here); the statue turn
  and turn about; his bark across and his looks; the map's pages.
- the bedroom (castle-2f-3, and castle-2f-5: the same room once the chapter has turned) shares its
  things in `story/places/bedroom.fs` (vocab story.places.bedroom): the TV, the window's and the
  door's glass, its sounds and fan, the fire, the portraits, the doll, the birdcage, the scales, the
  chest (her costumes), the bed to hide under (Hewie under the other; `hide-under-bed` takes what
  the room does once she's under and as she gets out), what Hewie sniffs out and looks up at; its
  own cells come first (`own-reset  bedroom-owns owns !`).
- castle-2f-3, the bedroom (44): every remark and spot, the clothes, the TV, the bed and the
  stalker's search, the maid's scene, the night, the attack lesson, the evasion tip, Hewie's
  find, the four replays.
- castle-3f-1 ($6): the boxes below, the symbol door (its file; the Saturnus Key: Daniella's
  "Dinner is served" before the chapter turns, else it unlocks), Daniella busy there after the
  rest, the replay. castle-3f-2 ($2C): Daniella ahead the first time on after the rest.
- castle-b1-1 (the kitchen: the sketch, Daniella at the stove, Riccardo's scene, the cupboard, the
  plywood door, the jerky and Lavender), castle-b1-2 (the washroom), -3 (the store room: the Metal Boots, the Castle Gate Key), -4 (the
  spiral stair), -5 (the dark passage: Hewie tense, sent ahead; the pit), -6 (the chapel: the
  statue and the Saturnus Key, Debilitas, the stands and the chandelier, the five replays), -7
  (behind the furniture: the Study Key Hewie fetches through the hole, the blocked door and the
  stalker trying it, the Medallions).
- castle-2f-5, the bedroom once the chapter has turned ($32): the mirror whole, then broken; she
  weak (flagging as she reaches to act, at a locked door, at the jerky); resting on the bed - the
  mirror's scene (it cracks and shatters as it goes, Daniella in a fragment, her face), well again,
  the file, Hewie out from under the other bed, Daniella left in the room, the chapter's doors
  open; the remarks, the TV, the windows (the glass sharp after), the chest, the clock; the bed to
  hide under and Daniella's search (she looks under it); the chicken jerky Hewie sniffs out; the
  replay.
- front-garden-3, the cage's garden (24): the opening, the remarks, the breakable, the box and
  the grate with Hewie, Hewie freed from the wire at night (and gone, out of the game), the shared
  crouch and drink.
- castle-2f-8, the kiln room (15): one golem (the room's own model), EMETH, METH, SALTATIO, any
  other word, the parchment, the choker.
- castle-1f-4, the puppet room's hall (22): the puppet door and the marionette, the floor plate,
  the weak wall's antimony, the plate-key's slot, Riccardo at night.
- front-garden-1, the gate and the drive (13): the opening's scene at the gate, the castle gate
  key, the tree and the marionette (her look, Hewie fetching it), the lamps flickering at night,
  the crate, two Medallions, the spirits let out, the far view's depth, the replays.
- front-garden-2, the yard (16): the grate bolted and its scene, three crates and a Medallion, the
  windows, the whine at night, turning back for Hewie, the replay.
- front-garden-4, the kennels (17): the cages, two doors, the crate, the camomile, the Album
  Medallion, the rabbit, the stalker shutting the door on her.
- front-garden-5, the far corner (19): Hewie's four ways over, the hung things (his snaps, her
  shoves, the drop), the chimes, the crate, three pickups, the rusted door, the hiding place.
- lake-pathway-1 (11) and lake-pathway-2 (9): the path collapsed and the bridge gone, the body,
  the lavender, Hewie's looks.
- castle-1f-1, the entrance hall (1): the gate, the pond's splashes, the Refined Magnesia Hewie
  fetches, the crouch evasion tip, the creature let out, in from the bookshelf's way.
- castle-1f-17, the specimen room (27): the pendulums, the remarks, the lavender, Hewie by the
  shelf, the door unlocked from this side.
- castle-1f-10, the doll's room (41): the remarks, the flame, the beef jerky, Debilitas sitting
  with his doll (a figure: his doll's model) - the scene and the Smudged Key, the dance - his
  chase scene, the Bisque Doll, the replays.
- castle-1f-18, the moth room (28): the moths round the lamp by the shut door (their remark and
  file, Hewie wary of them, their sound), the Fresh Flowers laid in the vase (the scene: the moths
  drawn over, the door unlocked), the torn picture, the clock, the empty vase, the replay.
- castle-1f-14, the west corridor (14): three vases broken (the Chicken Jerky and the Antimony
  Powder behind two), the Viride Medallion Hewie finds, the door unlocked from this side, the map's
  page at the stairs, the walls' shade, the other game's creature, brought here caught (Lorenzo's
  scene, the game over), the replay.
- castle-1f-7, the breaker room (21): the gate held up from castle-1f-4's plate and falling once
  let go (nobody under it), the breaker lowered (the hum, the door unlocked, the other game's
  file), the locked door tried, the stabbed doll in turn, Hewie barking at the breaker, brought
  back in by the forced way (the gate rattling, sent back), in among the wires (the game over:
  played through), the replay.
- castle-1f-16, the TV room (26): the fan, the TV on and off (the first time a fright; its noise
  heard), the hiding place (in, out, found and pulled out), the stalker coming while she hides
  (Debilitas turning the TV off - twice and it stays off - or his search, likelier each time), the
  mirror, the painting in turn, the Magnesia, the clock, the far views' depth.
- castle-1f-6, the long dining room (17): thirteen candles stirring as someone passes, the second
  cracked plate-key, the candlesticks by day and night, the tapestry, the Medallion Hewie finds, the
  door unlocked from this side, Daniella about beyond at night, brought in by the forced way
  (Daniella's scene), the replay.
- castle-1f-19, the flooded passage (30): the hole Hewie hides in at her "go there" (out at her
  "come back"), Debilitas by it - the ambush (a scene, its files), Hewie fetching the Rubrum
  Medallion across the water, splashes, his bark at the hole and look across, the hole's remarks,
  the replay.
- castle-1f-11, the drawbridge room (3): the clamp struck (the lever jolting; the third blow and
  the bridge falls - a scene, the far door closed off, Debilitas shoving the rubble down, throwing
  her in his way), three vases (the Beef Jerky and the Magnesia), the Medallion Hewie fetches
  across the gap, the clamp's remarks, the map's pages, the replay.
- castle-1f-12, the moon dials' room (4): two dials turned by the stick (clicking at a number, the
  right one's lens glinting), the Lunar Refractor set (a scene, her reflection in the lens), the
  dials right with castle-2f-1's - out there and back by the forced ways, the moon door unlocked
  and the five doors open; the vase's Medallion, the moon on the wall, the locked door, the replays.
- castle-1f-9, the hall of the half stairs (37): the stairs in turn, the sealed well, the bench, two
  vases (the Magnesia), the Rubrum Medallion Hewie sniffs out, the Album Medallion he fetches off
  the stairs at her "go there", the garden door locked - the Garden Key and the Luminessants'
  scene (their file, the creature let out), her first fright, the motes, tint and specks, the replay.
- castle-1f-5, the long gallery (16): Debilitas bursting in after her (the golem done) and his shove
  bringing the rubble down; brought in after Daniella's scene, the chapter turning (the castle's
  doors anew, Daniella the stalker, her costume and Hewie's coat afresh, the stage music) and her
  weakness after it; three flames, the vase's Antimony Powder, the box, the dim corner, the twin
  paintings, the map's pages.
- castle-1f-2, the courtyard (2): the gate's lever (the cage rising, the door beyond unlocked, the
  creatures out), Hewie sent under the gate - through the gap, sitting there; sent on, he fetches
  the Fresh Flowers back to her; called, he crawls back; the hole in the wall (the voices: a scene,
  the Scrawled Memo; else the breeze, never again), the fireplace (the embers in its draught, the
  drum can struck, the fire and the doll in turn), a place to hide (the stalker searching - likelier
  each time he comes, or seeing her), in by the gate's way (Debilitas knocked down beyond it, no
  going back), Debilitas shoving the rubble by the hall and far off, two vases (the Viola
  Medallion, the Onion), the Rubrum Medallion Hewie sniffs out, the flowers and the fence in turn,
  the pond's splashes, Hewie's barks and wary stand and his look up, the machine's hum, the moths,
  the specks, the steady sounds, the walls' shade, the map's pages, the replay.
- castle-1f-3, the library (9): the awakening at the Garden Key the first time (the voice, the thing
  set on the table; a scene), the key taken after; chased by Debilitas while he is elsewhere - the
  hint and its while, and in behind the bookshelf she knocks it down on him (a scene: he lies
  knocked out under it, she runs out); her thought pushing it, the books, the harp, the creepy thing
  in turn, the Camomile, Hewie wary by the table and his looks, the walls' shade, the replays.
- castle-1f-13, the great hall (8): brought in by the forced way - the balcony collapsing under her
  (a scene: she comes to below, Debilitas stalking along the balcony above and shoving its rubble
  down, the crows' file), or seized and carried off (a scene, out by the way loaded ahead); a place
  to crouch hidden behind (the stalker searching, likelier each time), Debilitas back from
  castle-b1-7, the fallen balcony and the false doors in turn, the Chicken Jerky, the Antimony
  Powder Hewie sniffs out, the hanging thing she sets swinging, the specks, the walls' shade, the
  creatures, the replays.
- castle-1f-15, the armour gallery (25): chased along the balcony by Debilitas, the encounter - no
  Hewie (or him down): a scene, she is left lying and frightened; with Hewie: he saves her (Debilitas
  knocked down) or, likelier the more he has, fails (she is left lying) - each a scene and the other
  game's file; the hole in the balcony floor (down it by her choice; dropped in from above, she
  lands), the diorama, the spear, the armour and the handrail in turn, the Camomile and the Medallion
  Hewie sniffs out, his bark at the high window and his looks, the steady sound, the far view's
  depth, the walls' shade, the other game's creature, the replays.
- castle-1f-8, the telephone room (24): her first steps in - uneasy, the call (the voice warning
  her of Riccardo; a scene, its file) and the stalker in after her, the door locked; the line dead
  since, the hiding place (in, out, found and hauled out), the stalker coming while she hides (his
  search, likelier each time), four flames, the chair, the ornaments, the Magnesia, the Album
  Medallion Hewie sniffs out, the walls' shade, the replay.

## API

Declared in `scripts/messages.fs`.

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `arrived` / `leaving-room` | | in (rooms broadcast) | A room comes in / is being left. |
| `room-enter` | `( room exit -- )` | story → place | The room is in, entered by `exit`: `came-in`, each person's `person-in`. |
| `room-leave` / `room-left` | | story → place | Being left / left (its doings stopped, `went`). |
| `room-done` | | place → story | Finished: it may go. |
| `story-stop` | | → story | No more places (the others' tests; the console). |
| `danger`, `panic`, `fiona-doing`, `hewie-doing`, `stalker-doing`, `creature-doing`, ... | | in (broadcasts) | What the others tell; the place keeps it (`danger-now`, `fiona-mode-now`, `st-room-now`, ...). |
| `go-through` | `( exit -- )` | → rooms | An exit taken (`take-exit`), after the frame. |
| `to-exit` | `( exit -- )` | → a character | Put yourself on the exit's outside spot, facing in (`at-exit`). |
| `cam-char-setup`, `cam-on` (C words), `camera-restart` | | → camera | `camera-set`, `area-camera`, `camera-out`, `follow`. |
| `lock`, `unlock`, `door-opened`, ... | | → doors | `lock`, `unlock`, `door-shut`, ... |
| `panic-stage!` | | → panic | `panic-eased`, `panic-to`. |

**Facts read:** the progress (story, state and resident flags, the byte variables, the visited
rooms, door states), bodies (where the characters are, their triangles and headings), the room's
event areas (crossing tests), the nav mesh. **Facts kept (the story owns them):** the story,
state and resident flags and variables the places set; the room's nav-mesh groups.

## State

A place's fields (`place-state` in story/kit.fs): its room, the exit it was entered by, the exit to
take; its activities (tasks) and who each holds; each person's place at the last frame's end (for
areas entered / left) and whether they are in; the offer this frame; what the others told; its
effects (32 slots); its own cells (`own:`). The character slots the story keeps across rooms (the
stalker it loaded, the scenes' figures in 3..5) are story/base.fs's.

## Rules

From `event.c`, `scene_game.c` (the order), `progress.c`.

1. **Characters by slot:** 0 Fiona, 1 Hewie, 2 the stalker the story loaded (`partner-load`), 3..5
   the event characters' figures (`figure-in`: docs/subsystems/cast.md).
2. **A person coming in** while the room is played runs the place's `person-in`, as the
   original's entering scripts (Events_CharEnter): Hewie arriving or placed here, Fiona back in
   view, the stalker through a door.
3. **Areas entered / left** compare where a person was at the last frame's end with where they
   are now (the original's previous position).
4. **Taking an exit** (cmd 0x00, `exit-checked`): its door not locked, passable from the one
   played's side (Fiona's bit $10, Hewie's $20), and the one played at their own pace (Fiona: mode
   0 or 0xA; Hewie: 0). **An exit usable** (cond 0x07, `exit-out`): the one played free, in the
   exit's area, its door open, and the door not held. A forced exit ($80 and up) goes to the room
   loaded ahead - the room left, unless another was loaded since.
5. **Activities** hold whom they move; a move is sent only for a character the running activity
   holds; let go as it ends or the room is left.

## Design notes

- The rooms are named (`scripts/room-names.fs`). Rooms on no page of the pause map are
  `off-map-XX`.
- No opcode words: a place sends messages and uses the kit's words. Room numbers are names.
- A room's own model parts (a breakable whole or broken, a door's leaf) are `n show-part` /
  `hide-part` (the room mesh's and the doors' groups from $15).
- castle-2f-8's golem is the room mesh's own parts (standing, moved to the door, crumbled); the
  placed golem 50 above the room is only the plate scenes' puppet, left up there as the original
  leaves it.
- The camera's nearest-set stand-in stays as a fallback for a character with no setup.

## Status

The history below runs from the converted rooms' machinery (S1 .. S5: the rooms' bytecode
scripts as Forth, their opcode words - gone in 0.2.20, every room a place since) to the places.
Tests named there that went with the machinery are gone too.

The opening from a new game (no `free-play`): `tests/story/test_opening.fs` (7) - the cage room's
entering script (Fiona's act01: the doors locked, the opening's music and figures, the fade in),
then she is the player's: the world not held, walking, the room's first offered action ("Where
am I?", area $11) taken. (`free-play` is a test and debug shortcut past it, not a stand-in.)

**S1 built**: the story and room actors, the phases, the action scripts as coroutines; the
words for flags, variables, bits, the counter, chance; the script context and its waits;
actions; characters in areas, on triangles, their headings and rooms; exits (taking, usable,
arriving at); the camera's setups and who it follows; locking doors; the nav mesh's groups;
sounds and noises. Fiona's stand-in exit check is gone: the rooms' scripts take the exits.
Checked by `tests/story/test_story.fs` (12 tests): the new game's room actor and its entering
script (Fiona's camera setup, followed); a room change swaps the actors; front-garden-2's phase 1
takes exit 2 when Fiona is in its area, she arrives and isn't sent back; a door opened with her
animation and the exit taken; an action script started as a coroutine.

Since: placing characters (`char-to-tri`, `char-to-tri-facing`, `char-to-xz`, `char-to-xyz`:
a `place` message to the character), the room's look (`effect-string`: its fog and colours to
the renderer's `look-set` - without it every room kept its file's default, darker look). A
second arrival without a leaving between (a jump) retires the room still current.

**Free play** (`openhuey --eval free-play`, or at the console): the world as the opening leaves it,
until the opening can play (it needs the scripted moves, Hewie's squeeze through the grate, and
the cutscenes): `opening-done`, `grate-open`, the world not held, Hewie with her and hers to
command. The new game stays the default start.

**S2 built** (2026-10-08): examining and messages, items, zones.
- **Examining** (`cmd_scene_change`, `Progress_PlayerButtons`): the scripts prepare an action
  each frame (`scene-change`; cleared after phase 1, as `Progress_CharRequests` does). After
  phase 2 the room offers a "scene 5" one to Fiona (`offer`) and shows the prompt; with the
  action button pressed that frame, free, she takes it (`take-offer`): her action script starts
  and she is scripted (`scripted` on) until it ends (`scripted` off) - every script that takes a
  character tells it so, and a room releases its characters as it stops or is left. Other
  scenes (changes of scene, endings): S4. The scripts' 0xA5 (`examine-message`; the old
  opcode table called it `scene-ending`, wrongly) prepares a message instead (`read-message`,
  the original's 0x80000005): taken, the message opens and she looks at character slot 2 for
  30 frames (`Events_OpenMessage`, `Fiona_SetTarget`).
- **Messages:** `show-message` (the window shows it; `text-closed` answers), `wait-message`,
  `message-close`, `answer?`, `message-prepare` (S4's subtitles), `message-param-room`.
- **Her pad:** `control-action?` (her gesture this frame, from `fiona-doing`), `pad?` (the
  buttons, `keys.fs`).
- **Facing:** `char-faces-xz?`, `char-faces-area?`; **move modes:** `char-action?`.
- **Zones:** `zone`, `zone-rect`, `char-zone-bits?`, `char-zone-bits-before?` (all off as phase
  1 starts). `char-in-zone?` is about attack points (her kick, his bite, Riccardo's shot). A zone
  of a kind with bit 1 is a target (Events_NearestPoint: the front garden's barriers, ...): told
  to the stalkers as it is set (`zone-target`; `zone-targets-off` as they all go) - Riccardo's
  shot takes the one by her.
- **Items:** `item-give` (a file, with the pickup sound), `item-add`, `item-count?`,
  `item-give-count`, `item-use`; `script-item` (Events_ScriptRoom: `items-40-as-70`). The story
  writes the inventory in the progress until the items subsystem owns it.

Checked by `tests/story/test_examine.fs` (10 tests): in front of the hole the action is offered,
the button starts her script and the window shows its text, she can't move meanwhile, the button
closes it and frees her; an answer; items given and counted; a zone against her body.

**S3 built** (2026-10-08): the characters' moves. A script's move (the original's +0xF4) is a
`scripted-move ( kind a b x y z yaw )` to the character, which carries it out itself, as the
original's do; each character broadcasts `moving ( done ended )` at each frame's end (+0xE1 and
its animation's end), which the room keeps for `self-wait-done` / `self-wait-anim` /
`wait-char-anim`. Done is true while idle (a character with no move is done). Words: `self-move-to`,
`self-move-tri`, `self-idle`, `self-anim`, `self-anim-blend`, `self-anim-blend8`, `self-idle-anim`,
`self-turn-angle`, `self-turn-to-xz`, `self-turn-to`, `self-look-at`, `self-look-at-point`,
`self-move-slot`, `self-walk-anim` (the turn to a character carries its slot as `a`, a look at one
its actor as `a` and its slot as `b` - the original's +0x100 the figures' heads read; a look at an
id nobody has sends no move, as the original's), `char-anim-hold` (`hold-anim`), `char-visible` (`show`; 0x1F's argument 1 hides, 0 shows - the
character's +0x29).
- Fiona (`scripts/fiona/moves.fs`, from Fiona_Requests and the Fiona_StateCmd* states): straight
  to a spot (her door walk), along a path turning 10 degrees a frame and stepping by her root
  motion, turning on the spot (0x400 / 0x401), animations, her idle, the walk with an
  animation arriving at its event; her looks (12 at a character, 13 at a point: `fiona/looks.fs`).
  Her scripted paths are curved as the original's (Character_WaypointsCurve; the engine's
  `v-path-curved`, `path-plan-curved`): the turning points moved 2 off the corners they turn round
  (where the way still walks), each stretch a Hermite piece of 8 steps whose tangents halve until
  every step walks straight over the floor (else the stretch straight; a way too long for the
  record stays straight). A door's use from a script (3 opening, 4 shutting: `scripted-door`).
- Hewie (`scripts/hewie/scripted.fs`, Hewie_Requests): the moves become his actions 0x3B..0x47
  and 0x7F (root-motion animations, walking a path, turning on the spot, heading for a spot and
  milling about, the scripts' bark, the leap); his looks at a character or a point. While a
  script has him (`scripted`) his frame is Hewie_Think; let go, his full stop and the default.
Checked by `tests/story/test_moves.fs` (13 tests) and `test_examine.fs` (her walk to the spot
and the kneel around the message).

**S4 built** (2026-10-08): fades, movies, cutscenes, music, motion events.
- **Fades** go to the `screen` actor (`fade`, `fade-finish`, `fade-over`); it answers `fade-done`,
  and `wait-fade` waits on the room's `fading`. (The kinds' music bits fade the stage music with it: `fade-music`.)
- **Movies** are the engine's (one at a time): `movie-play` opens the room's string paused (its
  class picks the compositing and whether it is drawn), `movie-volume` starts it, `movie-param`,
  `movie-playing?`. The room draws a drawn class's picture on UI layer 0 (`ui-movie`).
- **Cutscenes** are the C director's, cast from the characters' models: each character says
  which it is (`cast-as`: Fiona at her spawn, Hewie as he joins and parts). `cutscene-start`
  loads the room's scene and broadcasts `scene` on (Fiona is hands-off). The characters the scene
  casts (Cutscene_Cast: Character_Disable) wait - their own frames don't run (`scene-has? ( model
  -- flag )`): the scene's motion moves their models, their places following it (Fiona_FollowAnim);
  their faces cleared and drawn with all their shapes. Given back (Cutscene_ReleaseActors:
  Character_Enable) Hewie's feet are planted afresh and he is at his default action, a stalker's way
  is forgotten. The room's objects the scene animates are its groups (the script's group lists:
  each 0x10-byte entry the object's name): each shot shows the groups it moves and hides the rest
  (Cutscene_StartShot), and the scene's end, played or skipped, puts them back where they were
  defined (Cutscene_RestoreGroups - their place and turn; castle-2f-3's picture `a_nozoki` after
  the change of clothes, `tests/story/test_scene_groups.fs`). `cutscene-control`'s 13
  ops step it, its cues, the movie, the subtitles (op 12 turns the prepared message's pages -
  drawn on layer 4), and op 8 ends it (`scene` off, the camera back). The conditions:
  `cutscene-mode?`, `-cue-reached?`, `-event?`, `-passed?`, `-near-end?`, `-shot?`.
- **Music** goes to the `music` actor: `bgm`, `music`, `music-stage`, `music-stage-end`.
- **Motion events:** each character's `moving` now carries its motion's event keys
  (`moving ( done ended events )`): `char-at-motion-event?`, `self-at-motion-event?`.
- Skipping: the movie pause (`pause` actor) sets `movie-skipped`, which the scenes' scripts read.
A room left or stopped closes its movie and ends its scene.
Checked by `tests/story/test_scene.fs` (11 tests): the cage room's opening (its stage music, the
background track, faded in); a fade out and in with `fade-done`; the scene at the cage (act00) -
the movie, the cutscene, Fiona in it, its subtitles - skipped from the movie pause, and played to
its end (Fiona free, the track back). Seen on screen: the scene's camera and letterbox, the
subtitles, the pause and the skip's darkening.

**S5a built** (2026-10-08): the room's things.
- **Placed objects** by the room's string naming them (`object-hidden`, `object-anim`,
  `-anim-loop`, `-anim-reset`: the engine's `placed-op`).
- **The doors' parts:** each door's model has a group mask (`door-group!`, as Doors_SetBits sets
  door +0x120 on every present door); a batch shows while its group's bit is on (0 always).
  Six doors in five rooms have parts in a group (castle-b1-6's in 22): the scripts' `door-bits`
  shows them. The masks start empty with each room, as the original's doors are made anew.
- **Lights** (`light`): back to the room's own (`light-own`), or the intensity / the shadows'
  reach scaled (`light-scale`). `lights-doorway` (0x66) gives the lights a blocker for the
  frame (`light-blocker`): where its quad shades a light, the characters' shadows from that light
  don't fall (docs/subsystems/doors.md, "The open door's light").
- **The look:** `depth-range` / `-off` (slot $1C), `fog` ($1D), `screen-blend` ($1E).
Checked by `tests/story/test_things.fs` (8 tests).

**S5b built** (2026-10-08): the rooms' effects (`docs/subsystems/effects.md`) - an actor each,
drawn with the engine's sprites. Room effects in the room's 32 slots (`flame-in`,
`flicker-sprite`, `-var`, `release-butterflies`, `effect-remove`, `zone-at-effect`, `char-effect-moving`),
going with the room; scene effects ending by themselves or with the next room (`scatter-specks`,
`shards-at`). `effects-arena-flip` is the PS2's packet memory: nothing here.
Checked by `tests/story/test_effects.fs` (10 tests).

**The start of a game** (`scripts/opening.fs`, 2026-10-08): the title (from new-src: Hewie lying
in the warm light, NEW GAME / LOAD GAME / OPTIONS - and once a game has been cleared HARD MODE
(story flag $AF) and EXTRAS (room $37 straight) - / QUIT; a new game: he gets up and barks;
`tests/story/test_title.fs`), the opening movie
(OPENING.SFD, as the original's title plays it; the action or start button skips it), then the
new game's entry as SceneGame_StateEntry makes it for entry $2A: state flags `new-game-sounds`,
`world-held`, `in-play`; costume variable $26 = 1 (Fiona in the slip she wakes in, `O_FIS` with
her clothes' markers `O_FIN/FIN_000.MRK`), Hewie's $27 = 0; room $2A, whose scripts do the rest.
Without a window (tests, screenshots) the new game starts at once. `free-play` cuts the title
short. Fiona's model follows the costume (`costume` message: 0 her clothes, 1 the slip; the
others later); her head's bones with it. The slip's own animations are fewer: the scripts load
more with event $A6 and use them with $A7 (`char-file-use`: a second motion set) - the mechanics tests put her in her clothes.

**Debugging** (`scripts/debug.fs`, at the console and in tests): `act` (a room action script as Fiona's), `room!`, `room-by!`, `tp`,
`tp-facing`, `tp-area`, `tp-area-facing`, `tp-exit`, `tp-tri`, `walk`, `press`, `.here`.

**S5c begun** (2026-10-13): the rooms' own commands and conditions (`src/game/rooms/room_*.c`;
the stubs' `cmdNN` follow each room's command table in `src/game/sinit.c`). Every room was
entered in free play and run 300 frames: no errors, no stack left over. 96 of 311 stubs done:
- **Conditions** (34 of 43): the stalker in the game or only loaded, his kind (Debilitas: 2 / 6 /
  7), down, his mode, out of the camera's view (`out-of-view?`: Camera_OutOfView, the camera's
  +0xD4 - true when outside, whatever the decomp's comments say); a loud noise ($20+) made here
  last frame (acoustics' `noise-was`); the summoner's countdown run out (`summoner-cooldown-is`);
  who struck Fiona (`fiona-struck-by`); the things lying about; doors' states; her place; the
  script variables. The 18 off-map rooms' "progress +0x7C" is ProgressBase_Slot7C: always true.
- **Room objects by name** (`object@`, `object!`, `object+!`, `object-n@`/`!`, `object-here?`,
  `object-to-def`, `place-to`, `room-object`: the engine's `placed-f@` / `placed-f!` /
  `placed-l@` / `placed-l!` - an object's turn (+$10), place (+$20) and the rooms' own values on
  it (+$30..+$7C)): the grate, the box, the cage, the drum can, the lid, levers, dials
  (`dial-step`: the menu arrows with the pad's repeat), the floor plate (`var-fade`), pendulums,
  hanging things pushed as she or Hewie walks by (`hangers-swing`, `char-step`), rocking chairs,
  the scales, the falling doll (along the walk mesh), the swinging things of castle-b1-6.
- **Characters:** a figure moved by a room (`char-shift`); the rabbit (cast kind 26: grazing,
  alert within 30, off to its spot within 20 - `animal-go`, `char-wait-done`); Hewie's look at a
  point, forced (`scent-now`); her hair's force (`cloth-force!`: her model's +0xC8).
- **The scenes:** an actor the cast leaves be (`cutscene-leave-alone`: Cutscene_Mark82); the depth
  range following the scene's frame.
- No-ops in the original too: the model hooks +0xBC / +0xC0 / +0xCC / +0xD0 are empty.
Checked by `tests/story/test_room_conds.fs` (16), `test_room_cmds.fs` (22), `tests/cast/test_animal.fs` (4).

**S5c, the second round** (2026-10-13): 206 of 311 stubs done; what is left is the rooms'
effects and the Hewie-controlled clock rooms ($D0..$E9, 42 stubs).
- **Room objects shown and hidden - fixed:** the original draws a room object only while its +0
  byte is 0 (PlacedObjects_Draw); they start drawn and event 0x50's op 0 with 1 hides one. OpenHuey had
  it the other way round (start hidden, 1 shows) - the hanging chandelier of castle-b1-6 hidden
  and the fallen one shown before it fell. Now `placed-op` 0 takes the original's byte, op 4 shows
  (PlacedObject_ToDef), and the words say so: `object-hidden` ( obj hide? ), `object-hidden!`,
  `placed-shown?`.
- **Morphs:** the curtains (`var0-anim`, `var0-frame`) and old-mansion-2f-8's (variable 0 in a
  range); the three-dial lock of old-mansion-2f-12.
- **Speech:** the faces' textures (`speech-load`, `speak`, `speech-done`: Progress_LoadSpeech /
  Speak - a character's texture swapped for a .TEX file's first).
- **Gameplay pieces:** the floor plates (old-mansion-b1-3, house-of-truth-1f-9); her shakes
  counted (`count-shakes` to Fiona, `fiona-shakes` back: old-mansion-b1-3's 35, house-of-truth-
  1f-7's struggle to 100 with its grunts); a door swung by a variable or along its animation
  (`door-play`, `door-wait`); a file found once a flag is set (`file-once-flag`: item238_sound);
  the moving bookshelves (old-mansion-1f-15: three groups back to their stops, the things on
  their floor gone, the doorways found again); the shelf falling over, the dynamo and the belt
  machine (house-of-truth 1f-6 / 1f-4); a variable down by her hit (`var-down-by-hit`); things in
  an area aged out (`things-age-out-in`); the hatches of the water tower (`var0-ease`, with their
  sound kept on - id bit 31); the aiming cursor of water-tower-8f-1 (`cursor-step`,
  `cursor-draw`: the room's texture $C on UI layer 15); a blow from a point at a
  character (Progress_Noise: `fiona-request` kind 2 to Fiona, `blow-from` to Hewie and the stalker -
  each reacting as to a blow with no one behind it, as Progress_OwnRequests / Pursuer_Hit do); Fiona's flinch with nobody
  holding her (`flinch`: action $B / $21); the screen darkened a frame at a time (`screen-fill`,
  UI layer 16 - now 18 layers).
- **The falling debris and the traps** of house-of-truth-1f-9: kind-9 things carrying a room
  object "keri0".."keri5" (free while hidden; `keri-drop`, and `keri-fell` once one drops
  through, counted by cmd05), kind-10 traps round the middle (`trap-place`: the stalker treading
  on one is knocked down - a blow of kind 2, $28), all gone (`story-things-off`).
- **Creatures:** the homunculi on a nav group put away (`creature-clear`: Creatures_Remove); the
  creatures here on level 0 (`set-level`: CreatureA_SetSaveSlot - their floor of a split room).
- **Characters:** Hewie's health seen by the story (`hewie-health` each frame; `char-dead?` and
  `char-full-health?` for him too) and set (`set-hp`, `full-health`, `heal` - he had no handlers
  for 0x48 / 0xB8); the wading character's steps (`foot-down?`) and its sinking (old-mansion-
  b1-6); the fish of old-mansion-b1-7 swimming their ways (`swim-path` / `swim-step`:
  src/game/swimpath.c, SwimPath_*); the drift of character $E (`room-nudge`); a character turned
  by a variable (old-mansion-1f-6); walking to Fiona's triangle (old-mansion-3f-2); characters
  tinted by a fading colour (`char-tint`: water-tower-8f-1's Hewie and stalker).
- **Shared:** Lorenzo's slam shaking the camera in 13 rooms (`slam-shake`, `slam-landed?`); the
  hanging things brushed by her (`swing-three`) and pushed (`hangers-swing`); the fans
  (`fan-turn`).
- **Room strings:** chaos-forest 4..8, 10 and 13 were missing from story/strings.fs (the table
  stopped at $FF) - their hanging things and their movies (DE002A, EV0020, EV0021).
- **Not simulated here yet, kept as no-ops:** the stalker's and character $B's model springs
  (+0x9E0.., +0xCC8: the waits kept), model parts hidden by their flag 2 (old-mansion-b1-3,
  castle-2f-3), a character's doorway fade (+0xE4).
Checked by `tests/story/test_old_mansion.fs` (17), `test_house_of_truth.fs` (18 - since folded into `tests/places/test_house_of_truth_1f_9.fs`),
`test_water_tower.fs` (9 - since folded into `tests/places/test_water_tower_2f_1.fs` and `_8f_1.fs`), with `test_dial_lock.fs`, `test_curtain.fs`, `test_speech.fs`.

**S5c, the third round** (2026-10-13): 243 of 311 stubs done. The rooms' effects, as particle
classes (`docs/subsystems/effects.md`): the falling stones of the House of Truth, the water
tower's debris, the smokes, motes, embers, flecks, drips, fires and glows, the TVs showing the
screen, the reflecting windows and floor (the lit quads); the kiln's glow and the rooms' colour
pulse (the tint, slot $1F); the fish of the old mansion's pool (`swim-path`); the sprite dropped
in old-mansion-2f-11 (`effect-moved`); off-map-27's items topped up. Also fixed: a morph object's
mesh was made immutable and then refilled (old-mansion-2f-13's curtain: a GL error).

**S5c, the fourth round** (2026-10-13): 268 of 311 stubs done - all but the Hewie-controlled
clock rooms ($D0..$E9, 42 stubs) and off-map-5a (its dials' table of variables is zero in the
original: an unused room). The rest of the rooms' effects as particle classes
(`docs/subsystems/effects.md`): the old mansion 1F's big fire and haze, B1's light shaft and the
glint on the pool's fish, the caustics (the rooms' "ModelDraw" effects: water light on the lake
pathway, the front garden, the old mansion), the front garden's glow, leaves and falling arc,
the House of Truth's wisp column, spark spray, heat haze and pit embers, the water tower's
planet glows, the Chaos Forest's rain and ash; Lorenzo's marker (house-of-truth-1f-5: `marker`),
the dials' lens flashes (castle-1f-12, castle-2f-1: `lens-flash`), the mirror fragments
(castle-1f-12's lens, castle-2f-5's fragment: `mirror-fragment`) and the traps' smoke
(house-of-truth-1f-9: started by the things actor's `trap-place`).
Checked by `tests/story/test_marker.fs` (3), `test_mirror.fs` (8), `test_house_of_truth.fs` (22 - since folded into `tests/places/test_house_of_truth_1f_9.fs`).

**S5c, the renderer's part** (2026-10-13): a character's draw layer (`char-layer`, the scripts'
0xB5: `act.layer`) - $14 shines in the colour the marker gives (`post_shine`), $1E and up are
drawn after the reflections (Fiona in front of castle 2F's whole-picture reflection); the glow
pass and the effects' blooms (`docs/subsystems/effects.md`); the reflecting door's and window's
side panels (castle-2f-3, castle-2f-5: the colour word's top bits, as the original's); the
stalker's blow under way (`stalker-blow`, broadcast after `stalker-doing`; `stalker-blow?`) for
castle-b1-6's cmd01 - hg names it Pursuer_PathNodeSound, but it is his attack move's damage.
Checked by `tests/story/test_stalker_blow.fs` (5).

**The last story words** (2026-10-13): `movie-loop` (0x52: the movie held on its last frame at its
end), `char-model-op` (0xCC: 3 a cutscene actor's model left as it is; Daniella's model parts 17 /
31 by 0 / 1 - `actor-part-show`, the models' parts shown or hidden by number: skinned, then rigid,
then morphing, as the original's draw flags count them),
`self-doorway-fade` (0xAC: a character fading out through a doorway - `doorway_faded`, by its depth
past the exit area's middle; Fiona never), `self-noclip` for the stalker and the figures (their
floor mask), `char-activate` for Fiona, Hewie, the stalker and whoever is in a figure's slot (`activate`: afresh - a stalker posed as a figure freed); the hidden model parts of
old-mansion-b1-3 and castle-2f-3; the story's rumble (0x71). Checked by
`tests/story/test_finishing.fs` (23).

The cutscenes' own rumble (their marks' bits 2 and 3..4: the small motor, the large one's
strength $55 / $AA / $FF - Cutscene_Signal2 / Signals34) and op 0xCC's 3 corrected: the director's
+0x78 is Cutscene_Mark83 - the model left as it is when the scene takes it (`cutscene-keep-model`;
the cast runs the models' own hooks, Daniella's parts, as the original).

**The mini-game's run** (the clock rooms $D0..$E9): `hewie-controlled?` (cond 0x5D) is
`facts.fs` `hewie-played?`; `player-who` and `player-camera` give Hewie then; the exit words
(`exit-check`, `exit-usable?` - cond 0x07) go by him (free of scripts, at his own pace) and the
controlled one's gesture (`control-action?`, cond 0x21) is his. The clock's start hands him the
player and restarts him; its stop gives Fiona back; its save keeps the time for the results
(`run-took`). Forced exits ($80 and up: the extras room's door to a challenge, the run's way
back) go to the room `room-preload` or `exit-prepare` named (`docs/subsystems/rooms.md`).
The title's MINI-GAME starts the extras room's action 1 at once (its door 0's offer:
`mini-game-asked?`).

**Checked against event.c** (2026-10-24): the entering scripts of characters coming in while the
room is played, the script's character by id (`cs>id`, `self-is?`), exits by her mode and from
Hewie's side, the hiding places' offer while busy, `char-camera` $FF, `self-touching?` by the
other's margins, `stalker-free?` (not held by a script or struck; his floor free), the facing
conditions anywhere, `loader-busy?` (cond 0x62: the loader's busy flag - never, the waits on it
end at once), `spirit-request-pending?` (cond 0x64). `tests/story/test_story_rules.fs` (22).
Also (2026-10-24, `tests/story/test_story_fades.fs`): $FF in a condition is nobody (cond_char,
`cond-slot`: Progress_SlotOfId gives none - `char-in-zone?` takes it as anyone, Zone_HasAnyChar);
SceneGame_Busy's +0xF6CD24 bits: bit 3, her panic's start (`panic-onset` from the panic, cleared
by phase 0 and a forced action for Fiona, 0x92 for 0) is busy and holds the action scripts; bit 0
(the room's loading frames) the scripts never see, OpenHuey loading at once. The fades' 0x5D /
0x5E are a step and an over where it is (screen.md).
`stalker-free?`'s other hold (Progress_IsLinked): `stalker-linked?`, the stalker's `stalker-ties`
kept by the room (his seize of her awaiting her answer, Hewie's attack or tackle being taken).

The later stages, by how much the scripts use them (all built now):

| Stage | Words |
|---|---|
| S4 | (built) The rest: scene changes (`scene-change`'s other scenes), endings, `movie-loop`, the half-black screen (op 11). |
| S5b | (built: docs/subsystems/effects.md) Effects: a sprite batch in C, an effect as an actor per slot; the classes by use (`flame-in` flames, `flicker-sprite`, `zone-at-effect`, `char-effect-moving`, `scatter-specks`, `release-butterflies`, `shards-at`, `effects-arena-flip`). |
| S5c | (built) The rooms' own commands (`<room>.cmdNN`: their effects - wisps, smoke, drips, the mirror...). |
| S6 | (built) The stalkers' and creatures' words, with them (`docs/subsystems/stalker.md`, `creatures.md`). |
