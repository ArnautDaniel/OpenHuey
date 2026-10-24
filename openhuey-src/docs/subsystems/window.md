# Messages on screen (the window)

## Purpose

The message window: the rooms' texts (what Fiona sees and says as she examines things), page by
page, and the choices some of them ask; and the action prompt, shown while something here can be
examined. The original's text Task (`src/game/text.c`) and `SceneGame_ActionPrompt`.

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `show-text` | `( msg -- )` | in (a room's scripts) | Show message `msg` (bit 15 a system message, 14 the second table). |
| `close-text` | `( msg -- )` | in | Close it if it shows `msg` (`$FFFF`: whatever it shows). |
| `text-closed` | `( msg answer -- )` | out, to who showed it | Closed; the option chosen (-1: no choice). |
| `text-shown` | `( on -- )` | broadcast | Open / closed: Fiona stands while it is up. |
| `prompt` | `( kind -- )` | in (the room, each frame) | Something can be examined here: the prompt (gone when not offered for 2 frames). |

**Facts:** the messages laid out by C (`game/messages.c`: pages of lines, the options, each
character's colour and the frame it is typed on - `message-run`, `message-typed`, `message-skip`,
`message-close-after`, `message-place`, `message-align`; the parameters `message-param!`), the pad's buttons (`keys.fs`), the arrows.
**Sounds:** the menu's (bank 5): $2B an answer, $2C cancelled to the last option, $2A the cursor.
**Draws** on the retained UI layer (`ui-clear`, `ui-rect`, `ui-text`: C keeps what an actor put
there and draws it each frame), layers 2 (the window) and 3 (the prompt). The layers, in drawing
order: 0 a scene's movie, 1 the fade, 2 the window, 3 the prompt, 4 the subtitles, 5 the pause.

## Rules

1. A page at a time, typed out (Task_StateType): a glyph then waits `Text_GlyphFrames[speed]`
   frames and one more (speeds 0..2: 0, 1, 4; 0x0C n sets it, 0x0D back to 0, a choice to 0);
   0x09 n pauses n frames and one. The triangle (cancel) types at once up to the next pause or the
   page's end (not during a pause). Typed, any of Select, L1, R1 and the face buttons (the pad's
   0xFC01; not Start) turns it; on the last it closes.
2. A choice (0x0E): on its last page the cross answers (the message the option leads to, same
   table, or closed; the answer is the last option chosen); the triangle goes to the last option
   when the choice allows it (its flags' bit 0 clear); up / down go round the options, left /
   right round those on the same line (one alone there: as down / up). Task_StateChoice.
3. 0x10 n: typing stops; the message closes n frames later, no button needed (0: left up until
   the scripts close it).
4. Colours: 0x03 n draws what follows in the font's palette n (`ui-text-in`), 0x04 back.
5. The place (0x0A n, Task_Layout / set_position, D_0044B010): the text's centre at line 370 of
   the PS2's 448 (n 0, the default), 224 (1: the notes on the walls, the tutorials) or 330 (2: the
   memory card's), the panel 6 lines lower; n's bit 7: no box, the words alone, centred across
   the screen. It holds from the page it is on to the message's end; a new message starts low and
   boxed, but the one a choice's answer opens stays where the choice was, boxed (Task_StateChoice
   keeps x, y; Task_OpenDefault sets the box back on). A page taller than the screen would be drawn
   smaller (none in the game).
6. 0x12 centres a line in the text's width, 0x14 puts it to its right (Task_DrawPage; on this disc
   only at a line's start: the YES / NO options, signatures). An option (0x0F) takes no room: the
   hand (the font's $60, palette 9) is drawn 20 of the PS2's pixels before it (Task_CollectChoices).

## Status

**Built** (`scripts/window.fs`), with the story's S2. The prompt is the circle button's glyph and
"look", in the game's own font (`draw-prompt`). Checked by `tests/window/test_window.fs`.

OpenHuey's own: the panel's look and width (the original's box is a soft-edged shade hugging the
text, Task_DrawBox), the page mark. Not needed on this disc: the colour's blend bit (0x80: the
glyphs subtracted) never reaches the window - Task_OpenDefault's base colour is 0 and no
message's 0x03 n has it (the sub screen's own words do: items.md); furigana (0x16 base 0x18
reading 0x17) is in none of the USA tables (the reading would be dropped); nor is 0x19 (the child
task: nothing in the game gives a Task a child), skipped with its two bytes.
