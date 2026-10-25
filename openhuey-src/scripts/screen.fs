\ screen.fs - the screen's fade (docs/subsystems/screen.md; the original's event +0x20, Events_Fade):
\ black over the picture, under the message window. The room's scripts fade it in or out over
\ some frames and wait for it. The fade goes on by its step script (the scripts' 0x5C starts one
\ for the kind in the slot $FA: D_003D6A40), each frame a step (0x5D: ScreenFade_Step), which also
\ sets the panic's overlay level and tells it a fade's frame is on (Events_ScenePlaying).
IN: screen
USING: engine actors messages ;

1 constant fade-layer   \ (the UI layers: 0 the movie, 1 the fade, 2 the message window, 3 the prompt)

\ a fade over `frames` frames, by kind (ScreenFade_Step; the kind's bits $C0 / $30 fade the music
\ and the volume with it: with the music). The even kinds go out, the odd come in.
enum: fade-kind  to-black from-black to-white from-white to-black-2 from-black-2 to-dim from-dim ;
\ the step script's parts: its first steps (one, or from-black-2's 16 - 15 held, then its loop's
\ first), stepping while the steps haven't passed the frames, to-black-2's 15 steps more, and after
\ it the out kinds' scripts going on (a fade's frame each frame, not stepping: $B8 / $BC) where the
\ in kinds' end ($B9 / $BD)
enum: fade-script  no-script leading stepping trailing lingering ;

state: screen-state
  cell field level        \ drawn: no-fade .. full-fade
  cell field from-level  cell field to-level  cell field frames  cell field t
  cell field asked-by     \ who waits for it (-1 none)
  cell field white        \ in white (kinds 2 and 3), not black
  cell field kind         \ (+0x11F1)
  cell field running      \ the fade on (+0x11F2: what the room's fading? asks)
  cell field steps        \ its steps so far (+0x20)
  cell field rate         \ its alpha a step (+0x18: 128 / frames, a whole number)
  cell field script       \ its step script's part (fade-script)
  cell field left         \ steps of that part left
end-state

0 constant no-fade   500 constant half-fade   1000 constant full-fade   \ (its level)
begin-structure /fade  field: fade-from  field: fade-to  end-structure
create fades
    no-fade , full-fade ,   full-fade , no-fade ,   no-fade , full-fade ,   full-fade , no-fade ,
    no-fade , full-fade ,   full-fade , no-fade ,   no-fade , half-fade ,   half-fade , no-fade ,
: fade-of ( kind -- addr )  /fade * fades + ;
: white-kind? ( kind -- flag )  dup to-white = swap from-white = or ;
: coming-in? ( -- flag )  kind @ 1 and 0<> ;
128 constant opaque   \ (the PS2's alpha: 128 full; the dim kinds go half way)
16 constant held-in   15 constant held-out   \ (from-black-2's first steps, to-black-2's last)
: start ( frames kind -- )
    $F and  dup from-dim > if  drop to-black-2  then        \ (8..15: no such kind - as out)
    dup kind !  dup white-kind? white !
    fade-of  dup fade-from @ from-level !  fade-to @ to-level !
    0 max frames !  0 t !
    frames @ 0= if  to-level @ level !  then
    \ (0 frames divide by 0 in the original: here at once)
    frames @ if  opaque frames @ /  else  opaque  then  rate !
    0 steps !  -1 running !
    leading script !  kind @ from-black-2 = if  held-in  else  1  then  left ! ;

\ ---- the panic's overlay level (ScreenFade_Level, the panic's +0x34) by each step ----
: fade-frame ( -- )   \ (a fade's frame: its level not set back to full this frame)
    panic-id dup 0< if  drop exit  then  send overlay-held ;
: alpha ( -- a )   \ this step's (ScreenFade_Step): by the steps so far, out or in
    rate @ steps @ *  kind @ to-dim < if  opaque  else  opaque 2/  then  {: n top :}
    coming-in? if  top n - 0 max  else  n top min  then ;
\ the level by it (0 .. 1000, the panic clamps): 0.5 going out, 1 coming in, less 0.005 a point of
\ alpha past 16 - full a little before a fade in is through. The dim kinds leave it.
: overlay-level ( -- thousandths )  alpha 16 - 5 *  coming-in? if  full-fade  else  half-fade  then  swap - ;
: tell-overlay ( -- )
    kind @ to-dim < 0= if  exit  then
    panic-id dup 0< if  drop exit  then  overlay-level swap send panic-overlay ;

\ ---- a step (0x5D, ScreenFade_Step): while the fade is on it goes a frame on; a fade's frame
\ either way ----
: drawn-step ( -- )   \ (drawn: in a straight line over its frames)
    t @ frames @ < 0= if  exit  then
    1 t +!  to-level @ from-level @ - t @ * frames @ / from-level @ + level ! ;
: step ( -- )
    running @ if  tell-overlay  drawn-step  1 steps +!  then  fade-frame ;
: fade-over ( -- )   \ (0x5E) the fade counts as over: who asked told
    running @ 0= if  exit  then  0 running !
    asked-by @ dup 0< if  drop exit  then  -1 asked-by !
    dup alive? if  send fade-done  else  drop  then ;   \ (who asked may be gone - the story stopped)
: script-done ( -- )   \ its fade over; the out kinds' scripts go on
    fade-over  coming-in? if  no-script script !  else  lingering script !  fade-frame  then ;
: part ( script left -- )  left !  script ! ;
: steps-done ( -- )   \ the steps past its frames: to-black-2 has 15 more
    kind @ to-black-2 = if  trailing held-out part  step  -1 left +!  else  script-done  then ;
: script-frame ( -- )   \ a frame of the fade's step script
    script @ case
        leading of  step  -1 left +!  left @ 0= if  stepping script !  then  endof
        stepping of  steps @ frames @ > if  steps-done  else  step  then  endof
        trailing of  left @ if  step  -1 left +!  else  script-done  then  endof
        lingering of  fade-frame  endof
    endcase ;

: opacity ( -- 0..255 )  level @ 255 * full-fade / 0 max 255 min ;
: draw ( -- )   \ black or white, that much opaque
    fade-layer ui-clear
    level @ 0> 0= if  exit  then
    opacity  white @ if  $FFFFFF00 or  then  {: rgba :}
    fade-layer 0 0 screen-size rgba ui-rect ;

behaviour fading
  on spawned ( -- )  -1 asked-by !  no-script script !  self subscribe tick  self subscribe frame-end  self subscribe arrived ;
  \ (its first frame at once: the original's script runs in the frame it is started)
  on fade ( frames kind -- )  start  sender asked-by !  script-frame ;
  on fade-step ( -- )  step ;   \ (a room's own 0x5D)
  on fade-stop ( -- )  fade-over ;   \ (a room's own 0x5E: where it is)
  \ (a room entered: its slots cleared, the step script with them - a fade not over stays where it is)
  on arrived ( room exit -- )  2drop  no-script script ! ;
  on tick ( -- )  script-frame ;
  on frame-end ( -- )  draw ;
end-behaviour

: screen-spawn ( -- id )  fading screen-state s" screen" spawn ;
