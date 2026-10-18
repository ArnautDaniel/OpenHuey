\ mods.fs - what the game gives mods besides its own vocabularies (docs/library-and-mods.md).
\ A mod's scripts: `USING: mods` and any of the game's.
IN: mods
USING: engine ;

18 constant mod-layer   \ the UI layer for mods, over the game's (a mod using it clears it and
                        \ draws its own each frame-end; mods sharing it draw in load order)
