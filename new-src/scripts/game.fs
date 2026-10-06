\ game.fs - where the game starts (after prelude.fs). Everything the game does is decided here
\ and in the scripts it loads; C only provides the words.
\
\ For now the game is a room viewer: a free camera, PageUp / PageDown through the rooms.

\ `script name.fs` loads another script from this folder
create path 512 allot  variable path-len
: path+ ( addr len -- ) dup >r  path path-len @ +  swap move  r> path-len +! ;
: script ( "name" -- )
    0 path-len !  scripts-dir path+  s" /" path+  parse-name path+
    path path-len @ included ;

script vectors.fs
script freecam.fs
script rooms.fs
script views.fs

0.06e 0.06e 0.08e clear-color
first-room
