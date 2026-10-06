\ look.fs - changing a room's look while it runs (its fog, tint, bloom, depth of field: the
\ values from its file, render.h RoomLook). `room-look-reset` puts the file's back.
\ A colour is four 32-bit floats: r g b (1.0 = the PS2's 0x80, fog 0xFF) and its strength.
IN: look
USING: engine ;

\ a colour field: set it, read it
: rgba! ( addr -- ) ( F: r g b a -- )  dup 3 4 * + sf!  dup 2 4 * + sf!  dup 4 + sf!  sf! ;
: rgba@ ( addr -- ) ( F: -- r g b a )  dup sf@  dup 4 + sf@  dup 2 4 * + sf@  3 4 * + sf@ ;

\ the fog: on, its range, its colours
: fog-on   -1 room-look look.fog l! ;
: fog-off  0 room-look look.fog l! ;
: fog-range ( F: near far -- )  room-look look.fog-far sf!  room-look look.fog-near sf! ;
: fog-colors ( F: nr ng nb na  fr fg fb fa -- )
    room-look look.fog-far-color rgba!  room-look look.fog-near-color rgba! ;

\ fading the fog's range to new distances over some ticks - a task, so the caller goes on:
\   200e 600e 120 fade-fog
fvariable from-near  fvariable from-far  fvariable to-near  fvariable to-far
variable fade-ticks
fvariable t
<PRIVATE
: lerp ( F: a b -- c )  fover f- t f@ f* f+ ;
: (fade-fog)
    fade-ticks @ 0 ?do
        i 1+ s>f fade-ticks @ s>f f/ t f!
        from-near f@ to-near f@ lerp  from-far f@ to-far f@ lerp  fog-range
        yield
    loop ;
PRIVATE>
: fade-fog ( F: near far -- ) ( ticks -- )
    1 max fade-ticks !  to-far f! to-near f!
    room-look look.fog-near sf@ from-near f!  room-look look.fog-far sf@ from-far f!
    fog-on  ['] (fade-fog) spawn drop ;
