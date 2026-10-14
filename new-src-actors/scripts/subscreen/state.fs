\ subscreen/state.fs - the sub screen's state (docs/subsystems/items.md): one actor's fields,
\ and names for its stages, modes and pages.
IN: subscreen.state
USING: engine actors ;

\ how far it is open
0 constant shut   1 constant opening   2 constant up   3 constant closing
\ what the keys work on
0 constant on-page        \ a page (the lists, the files, the map, the options)
1 constant on-actions     \ an item's actions
2 constant awaiting-use   \ an item handed to the items actor to use; its answer awaited
3 constant on-box         \ a message or a question in the box
4 constant on-plate       \ the word plate (the scripts' mode 2)
5 constant on-slots       \ the slot machine (the scripts' mode 3)
6 constant on-saving      \ the save slots (the scripts' mode 1: a save point)
7 constant on-costumes    \ the wardrobe (the scripts' mode 7)
\ the pages, in the order L1 / R1 turn them: the three item lists, the map, the files, the options
0 constant pg-special   1 constant pg-items   2 constant pg-other
3 constant pg-files     4 constant pg-map     5 constant pg-options

state: subscreen-state
  cell field stage        \ shut, opening, up, closing
  cell field openness     \ 0..128 how far faded in (the PS2's 128: opaque)
  cell field quiet        \ closing without its sound
  cell field opened-as    \ the mode the scripts asked for (0 the menu)
  cell field options-only \ (the title's options, mode 5) the options page alone: no turning to the others
  cell field mode         \ what the keys work on
  cell field page
  cell field frames       \ (the arrows' blink)
  4 cells field held-for  \ each arrow held: frames (their repeat)
  \ facts told by the others
  cell field her-doing    cell field her-mode   cell field the-danger  cell field the-panic
  cell field stalker-room
  \ the item lists and an item's actions
  3 cells field cursors   \ each list's cursor (16 a page, columns of 8)
  cell field action       \ the cursor among the item's actions
  cell field shown-picture   \ the item whose picture is loaded (-1 none)
  cell field slide        \ its picture sliding in (0..64)
  cell field file-page
  \ the box
  cell field shown        \ its message (-1 none)
  cell field shown-page   cell field picked
  cell field after        \ what to do once it closes (an xt, given the option picked)
  \ the map
  cell field map-at       cell field map-pg       \ the map and page shown (-1 none)
  cell field her-map      cell field her-pg       \ where her room is on the maps (-1 nowhere)
  cell field her-room     cell field map-loaded   \ (the page whose picture is in: map * 16 + page)
  \ the options
  cell field opt-row      cell field opt-editing
  cell field vibration    cell field sound-out    cell field volume   \ (volume 0..128)
  cell field pos-x        cell field pos-y        cell field layout
  \ the word plate
  8 field word            cell field word-len     cell field key-at   \ (key: row * 16 + column)
  cell field art-loaded   \ the menu's own art (SUBBASE) is in
  cell field back-bank    \ which art is in the background group: 0 the menu's, 1 the plate's, 2 the slots'
  \ the slot machine
  cell field slot-state   \ 0 choosing a medallion, 1 none to put in, 2 rolling, 3 what came out
  6 cells field stock     \ how many of each medallion she has
  cell field slot-cursor  cell field medal
  10 cells field rows     \ each row's symbol (-1 not rolled yet)
  cell field row-at       \ the row last stopped (-1: none yet)
  cell field reel  cell field reel-pos  cell field reel-tick  cell field reel-time  cell field reel-speed
  cell field reel-turning  cell field stopping  cell field all-rolled
  22 cells field matches  \ each cell's symbol (-1 no match)
  5 cells field matched   \ how many cells each symbol matched
  cell field result  cell field amount  cell field forged  cell field found-t  cell field found-heard
  cell field blink-t
  \ the save slots
  cell field save-at      \ the slot chosen
  cell field save-step    \ 0 choosing, 1 asked to write over it, 2 writing, 3 saved
  cell field save-yes     \ (asked) yes picked
  cell field save-t       \ (saved) frames shown
  \ the wardrobe
  cell field cost-row     \ 0 Fiona, 1 Hewie, 2 Exit
  cell field cost-open    \ the row's list is open
  cell field cost-her-at  cell field cost-her-worn   \ (places in her list) the cursor, the one chosen (-1: none known)
  cell field cost-his-at  cell field cost-his-worn   \ (his) - cost-his-worn -1: he isn't here
  \ the tab (an item the story hands her)
  cell field tab          \ 0 none, 2 sliding in, 3 shown, 4 sliding out
  cell field tab-slide    cell field tab-asker
end-state

\ the moves every page has
: close-menu ( -- )  closing stage ! ;
: back-to-page ( -- )  on-page mode ! ;
