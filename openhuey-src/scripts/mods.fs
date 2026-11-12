\ mods.fs - what the game gives mods besides its own vocabularies (docs/library-and-mods.md).
\ A mod's scripts: `USING: mods` and any of the game's.
IN: mods
USING: engine ;

18 constant mod-layer   \ the UI layer for mods, over the game's (a mod using it clears it and
                        \ draws its own each frame-end; mods sharing it draw in load order)

\ the items lying in the rooms (API 1.5): their sparkles drawn by the game, or not - a mod showing
\ them its own way (props). Told either way: item-placed / item-cleared (scripts/messages.fs)
variable sparkles-off
: item-sparkles! ( on -- )  0= sparkles-off ! ;
: item-sparkles? ( -- flag )  sparkles-off @ 0= ;
