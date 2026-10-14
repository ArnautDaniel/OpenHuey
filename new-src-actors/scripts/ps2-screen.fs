\ ps2-screen.fs - the PS2's 512 x 448 screen, fitted to a 4:3 frame in the middle of the window:
\ the original's pictures and words are placed in its units and drawn where they fall in ours.
IN: ps2-screen
USING: engine ;

variable fw  variable fh  variable fx
: frame! ( -- )  screen-size fh !  fh @ 4 * 3 / over min fw !  fw @ - 2/ fx ! ;   \ (each frame: the window may change)
: sx ( n -- x )  fw @ * 512 / fx @ + ;   \ a screen x, in the window
: sy ( n -- y )  fh @ * 448 / ;          \ a screen y (or height)
: sw ( n -- w )  fw @ * 512 / ;          \ a width
