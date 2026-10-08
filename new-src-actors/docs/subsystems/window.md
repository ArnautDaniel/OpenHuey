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

**Facts:** the messages laid out by C (`game/messages.c`: pages of lines, the options; the
parameters `message-param!`), the action button (`circle` in `keys.fs`), the arrows.
**Draws** on the retained UI layer (`ui-clear`, `ui-rect`, `ui-text`: C keeps what an actor put
there and draws it each frame), layers 1 (the window) and 2 (the prompt).

## Rules

1. A page at a time; the action button turns it; on the last it closes.
2. A choice (0x0E): on its last page the arrows pick an option, the button answers it; the
   window then shows the message the option leads to (same table), or closes. The answer is the
   last option chosen.

## Status

**Built** (`scripts/window.fs`), with the story's S2. The prompt says "Space: look" for now (the
original's icon: with the HUD art).
