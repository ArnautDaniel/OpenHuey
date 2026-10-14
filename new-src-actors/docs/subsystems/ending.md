# The ending

## Purpose

When the story ends the game (one of the four endings: the story sets progress variable $2D
to 0..3 - A..D - asks the game over for kind 2 and sets `caught`), the original leaves its game
scene for scene mode 5 (`src/game/scene_ending.c`): the staff roll, then the results - the
ending's picture, the play record, the TYPE it earns - what it unlocked, and back to the title.
Here the `ending` actor does it over the held world.

The play record is kept as the game goes, in the progress (so a save keeps it):

| Value | Kept by | The original's |
|---|---|---|
| time played (`pr.play-frames`: 30 a second, 99:59:59 at most) | `ending`, each tick in play (not paused) | +0xFC4 (PlayTime_Tick) |
| panics (`pr.panics`) | `panic`: each time she panics (`panicked`, facts.fs) | +0xFC2 |
| enemies defeated (`pr.knockouts`) | the stalkers: knocked out for good (`down-for-good`) | +0xFBC |
| Hewie mistreated / pleased / critical injuries (`pr.hewie-*`) | Hewie (`hewie/states.fs`) | +0xFB6 / +0xFB8 / +0xFBA |
| Hewie's trust (`pr.hewie-trust`) | `ending`, as the game ends (his last `hewie-doing`) | +0xFB4 |
| items (of 145 story flags, `bonus-flags`, how many are set) | worked out at the end | SceneGame_BonusFlagsSet / Count |

Every count stops at 9999 (`tally`).

## API

| Message / fact | Stack | Direction | Meaning |
|---|---|---|---|
| `the-ending` | `( -- )` | in (the game over, kind 2) | The story is over: the ending starts. |
| `hewie-doing` | `( ... trust -- )` | in (Hewie) | His trust, kept for the dog level. |
| resident flags | | out (facts.fs `resident-flag-set`) | What it unlocked: the system data's, kept across games (a fresh game and a loaded save leave them). |

## The sequence

1. The world held (`world-held`, `panic-held`, `still!`); the music silenced; the screen black.
2. The staff roll, `STAFF_ROLL.SFD`, 4:3 in the middle. Start skips it once it shows.
3. Its fade: the sound and the picture to black, a 30th a frame; then the results.
4. The results' setup: `SYSTEM/ENDING_A..D.TEX` (the texture group $2E), `SUBSCR/MSG_END.BIN`
   (`message-file`: its messages are ids $2000..), the dog level and the TYPE worked out, music $33.
5. The picture fades in (2 a frame), stays 90 frames, fades to half; the record fades in (4 a
   frame): ENDING, TIME, DOG LEVEL, PANIC, ITEM, CRITICAL HEWIE INJURIES, ENEMIES DEFEATED,
   TYPE - labels at x 32, values right-aligned at 480, a line every 32 from 96. The values the
   TYPE came from are highlighted (yellow; red for the bad ones).
6. After 150 frames, a button (circle, cross or Start): the record fades out (8 a frame); the
   unlocks are worked out.
7. Anything new: the picture back up, the messages in a box - the first clear's ($1C the secret
   room, $1D hard mode), then $1E (costumes); a button takes each down.
8. Top breeder, treasure hunter, warmonger or track star: a word for the word plate ($14..$17).
9. The picture fades out, the music stops, a fresh game: the title.

### The dog level (0 A .. 4 E)

His trust, plus 1, less how he was mistreated (in 20s, at most 4), plus how he was pleased (in
10s, at most 3): 10 or more A, 9 B, 8 C, 7 D, under 7 E.

### The TYPE (the first that applies)

| | Type | Message |
|---|---|---|
| mistreated 100 or more | DOG BEATER | 8 |
| ending C | FUGITIVE | 9 |
| dog level A, no critical injuries | TOP BREEDER | $A |
| items 80% or more | TREASURE HUNTER | $B |
| 30 or more enemies defeated | WARMONGER | $C |
| under 3 hours | TRACK STAR | $D |
| dog level A or B | DOG LOVER | $E |
| 30 or more panics | SCREAM QUEEN | $F |
| under 6 hours | SPEED WALKER | $10 |
| under 20 hours | UNJUDGED | $12 |
| else | LATE BLOOMER | $11 |

### The unlocks (resident flags: the original's system data +0x24)

0 the first clear (news: the secret room and hard mode); 1 ending A, 3 ending C (news when
new); 2 ending B, 4 ending D; 5 + the TYPE. A game in hard mode (story flag $AF) also sets 86,
and 87 / 88 with ending A / B; with nothing else new, a hard game's top breeder or dog lover
is news.

## Not built

- The clear-data save (the sub screen's mode 9): the next game with the clear's extras.
- (The font: the game's own - SUBSCR/MSG_BASE.TEX, each glyph its width and drop, 16 x 21 - for all
  the UI's text (`gfx.game-font`, on); its colour codes' palettes: the first one, tinted.)
- (The resident flags - the unlocks - are kept in `hga-system.fs` in the user folder, written
  whenever they change and read as the game starts; not under `--test`.)

## Status

`tests/ending/test_ending.fs` (27): the time played (not while paused), the dog level, each
TYPE rule, the items' flags, the first clear's unlocks (once only), and the story's end through
the staff roll (Start), its fade and the results to their end.
