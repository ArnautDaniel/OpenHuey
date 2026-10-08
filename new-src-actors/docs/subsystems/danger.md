# Danger: calm, followed, chased

## Purpose

How dangerous things are right now. The music, Fiona's fear and looks, the panic's calming and
Hewie all follow it. The original's "game mode" (`Progress_GameMode`, set by
`SceneGame_Danger`).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `danger-signal` | `( bit -- )` | in | Something this frame: 0 / 5 Fiona within 150 / 200 of the stalker, 1 Fiona was struck, 3 the stalker chases, 4 Hewie is alert to him, 6 hunted (the summoner). |
| `stalker-here` | `( room alert chasing -- )` | in | The stalker in play this frame (the stalkers send it). None this frame: no stalker. |
| `danger` | `( level -- )` | broadcast | Changed: 0 calm, 1 followed (tense), 2 chased. Also sent on `entered-room`. |

**Facts:** story flags 0x1B (force calm), 0x1F (force chased), 7 (force tense), 0x22; the played
room; the room graph.

## State

The level; the level before the last change; last frame's; the hold timer; this frame's signals;
this frame's stalker (room, alert, chasing).

## Rules

From `SceneGame_Danger` (`scene_game.c`). Each frame, at its end:
1. With no stalker, signal 2. A chasing stalker: signal 3.
2. Story flags force it: 0x1B calm, 0x1F chased, 7 tense (the hold timer 0).
3. Otherwise: the stalker in Fiona's room makes it chased (hold 30). Else, by the current level:
   - calm: `hunted` makes it tense (hold 450); or with the hold over, a stalker in play, a
     creature in her room or hunted (6) make it tense (hold 450);
   - tense: hunted or `hunted` add 150 to the hold (at most 450). With no stalker and no creature
     it goes calm once the hold is over (hold 30); otherwise, coming from calm the hold is 450,
     from chased 150;
   - chased: no stalker: tense if `hunted` or a creature, else calm (hold 30); the stalker next
     door with alert 3 or 4: tense when the hold is over (hold 30); otherwise the hold is 150.
   The hold counts down by one each frame.
4. The signals are the frame's: forgotten after. Struck (1) while calm signals hunted (6) for the
   next frame (*quirk:* the original sets it after clearing).
5. The camera director's event mode skips it (*with cutscenes*).

## Status

**Built** (`scripts/danger.fs`). Checked by `tests/danger/test_danger.fs` (13 tests): calm at
the start, Fiona told; the story's flags forcing it (0x1F chased, 7 tense, 0x1B calm first);
the stalker in her room (chased), next door after her (chased while the hold lasts, then
followed), gone (calm once the hold is over); `hunted` (tense, then calm after its hold).

Until the stalkers are built, tests send `stalker-here` by hand. Creatures: none yet.
