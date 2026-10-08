\ paths.fs - a way over the floor for a character to walk: planned by the engine (the nav
\ mesh), kept in the character's own state as a path record, walked a step at a time.
\   /path field my-path          (in a state: ... end-state)
\   my-path from-tri from-v to-tri to-v mask path-plan
IN: paths
USING: engine ;

64 constant path-points
2 cells path-points 12 * + constant /path   \ the record: count, index, points (3 single floats)
: path-n ( path -- n )  @ ;
: path-i ( path -- i )  cell+ @ ;
: path-i! ( i path -- )  cell+ ! ;
: path-point ( path i -- v )  12 * swap 2 cells + + ;
: path-left? ( path -- flag )  dup path-i swap path-n < ;
: path-clear ( path -- )  0 over !  0 swap path-i! ;
: path-end ( path -- )  dup path-i swap ! ;   \ nothing left to follow
\ a way from (from-tri, a) to (to-tri, b), off the mask's triangles: its turning points
: path-plan ( path from-tri a to-tri b mask -- n )
    v-path  path-points min                                           ( path n )
    dup 0 ?do  over i path-point  i nav-path-point  vec!  loop
    over !  0 over path-i!  path-n ;

\ the point `d` along the rest of the way from `at`, into `ahead`; the index of the next
\ turning point there (Character_WaypointAhead)
create ahead 12 allot  fvariable to-go
: path-ahead ( path at -- i ) ( F: d -- )
    to-go f!  ahead swap vec-copy  dup path-i                          ( path i )
    begin  2dup swap path-n < while
        2dup path-point >r  ahead r@ vec-dist-xz                         ( F: seg )
        fdup to-go f@ f<= if
            to-go f@ fswap f- to-go f!  ahead r> vec-copy  1+
        else
            fdrop  ahead r@ vec-heading  to-go f@  ahead ahead vec-ahead  r> drop
            nip exit
        then
    repeat  nip ;
\ how far is left along it from `at` (on the level)
create rest-at 12 allot
: path-rest ( path at -- ) ( F: -- d )
    0e  rest-at swap vec-copy
    dup path-n over path-i ?do
        dup i path-point  rest-at over vec-dist-xz f+  rest-at swap vec-copy
    loop  drop ;
