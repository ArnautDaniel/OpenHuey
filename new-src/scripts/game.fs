\ game.fs - where the game starts (after prelude.fs). Everything the game does is decided by
\ the vocabularies it uses (each is scripts/<name>.fs); C only provides the words (`engine`).
\
\ For now the game is a room viewer with Fiona and Hewie: a free camera, PageUp / PageDown
\ through the rooms, Tab to play, Space at exits, F1 for the graphics. `labels` (console): debug labels.
IN: game
USING: engine freecam views rooms player doors hewie fiona stalker graphics look events.play title debug ;

0.06e 0.06e 0.08e clear-color
show-title
