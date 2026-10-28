\ story/places/walkway.fs - the Water Tower's turning walkways (2F and 4F: the object move00): turned a
\ quarter by a lever from home to across and back, its floor (nav group 1 at home, 0 across) walked
\ where it lies; Hewie on it walks to his spot first and rides it. Each room describes its own with
\ `walkway:` (which way it turns, its height, the lever, his spots, what the room adds) and names it
\ as she comes in (`walkway-here`). The rooms' own cells follow these: `own-reset  walkway-owns owns !`.
IN: story.places.walkway
USING: engine game-state actors messages common facts flag-names floor-flags story.kit ;

own-reset

\ ---- its own state (a visit) ----
own: the-walkway           \ the room's walkway (its description)
own: turning               \ the walkway turning (the lever pulled, till it's done)
own: turning-back          \ ... this turn back home
own: hewie-clear           \ ... Hewie off its floor, or on his spot (his ride waits for it)
own: turn-step             \ its frame of the turn (to 120)
owns @ constant walkway-owns

\ ---- a walkway: which way it turns (1: to 90 degrees, -1: to -90), its height, where its edge is at
\ an angle (its creak there), the lever (her motions' file, where she stands), Hewie's spots on it
\ (going across, coming back: where he walks to, where he is put as it stops), his side then (-1 none),
\ what the room adds as it's across, and once it has stopped ----
begin-structure /walkway
  field: wk.fact  field: wk.sign  field: wk.edge  field: wk.file  field: wk.lever  field: wk.lever-face
  field: wk.out  field: wk.out-face  field: wk.home  field: wk.home-face  field: wk.out-end  field: wk.home-end
  field: wk.side  field: wk.across-xt  field: wk.stopped-xt
  ffield: wk.y  ffield: wk.lever-x  ffield: wk.lever-z  ffield: wk.out-x  ffield: wk.out-z  ffield: wk.home-x  ffield: wk.home-z
end-structure
: walkway: ( fact sign edge-xt file F: y "name" -- )   \ (edge-xt: ( F: rad -- x z ) its edge at that angle)
    create  here /walkway allot  {: f s e fl a F: y :}
    f a wk.fact !  s a wk.sign !  e a wk.edge !  fl a wk.file !  y a wk.y f!
    -1 a wk.side !  ['] noop a wk.across-xt !  ['] noop a wk.stopped-xt ! ;
: lever-at ( wk tri face F: x z -- )  {: a t fc F: x z :}  t a wk.lever !  fc a wk.lever-face !  x a wk.lever-x f!  z a wk.lever-z f! ;
: hewie-out ( wk tri face F: x z -- )  {: a t fc F: x z :}  t a wk.out !  fc a wk.out-face !  x a wk.out-x f!  z a wk.out-z f! ;
: hewie-home ( wk tri face F: x z -- )  {: a t fc F: x z :}  t a wk.home !  fc a wk.home-face !  x a wk.home-x f!  z a wk.home-z f! ;
: hewie-ends ( wk out-tri home-tri side -- )  {: a o h s :}  o a wk.out-end !  h a wk.home-end !  s a wk.side ! ;
: when-across ( wk xt -- )  swap wk.across-xt ! ;     \ (the room's, as it's across the first time each turn)
: when-stopped ( wk xt -- )  swap wk.stopped-xt ! ;   \ (the room's, once it has stopped)

\ ---- its parts, floors, objects, sounds (the same in both rooms) ----
0 constant across-floor   1 constant home-floor   2 constant walkway-floor   \ (nav groups: the floor turned across, at home; all of it)
0 constant home-part   1 constant across-part   2 constant back-object   3 constant across-object
3 constant back-motion   4 constant across-motion   0 constant lever-anim
0 constant shut-sound   1 constant open-sound   2 constant creak-sound   120 constant turn-frames
: wk ( -- a )  the-walkway @ ;
: turned? ( -- flag )  wk wk.fact @ fact? ;   \ (across)
: walkway ( -- addr len )  s" move00" ;
: blocked ( -- flags )  barred-floor hole-floor or ;
: kept-off ( -- flags )  keeps-fiona-off keeps-pursuers-off or ;
: aboard ( -- group )  turning-back @ if  across-floor  else  home-floor  then ;   \ (its floor as it lies)
: other-floor ( -- group )  turning-back @ if  home-floor  else  across-floor  then ;
: walkway-at ( across? -- )  if  wk wk.sign @ 90 *  else  0  then  s>f deg>rad walkway turn-y object! ;
: walkway-floors ( across? -- )   \ (the floor it lies on walked, the other blocked)
    if  home-floor blocked floor-on  across-floor blocked floor-off
    else  across-floor blocked floor-on  home-floor blocked floor-off  then ;
: walkway-parts ( across? -- )  if  home-part hide-part  across-part show-part  else  home-part show-part  across-part hide-part  then ;
: walkway-here ( wk -- )   \ (coming in: as its fact has it, its moving parts hidden)
    the-walkway !  2 hide-part  3 hide-part  0 object-show  1 object-hide  back-object object-hide  across-object object-hide
    turned? dup walkway-at  dup walkway-parts  walkway-floors ;

\ ---- the turn: 120 frames eased by a sine, its creak at its edge ----
: creak-at ( id F: x z -- )  {: id F: x z :}  id room-bank x wk wk.y f@ z sound-there ;
: creak ( F: rad -- )  creak-sound keep-playing or  wk wk.edge @ execute  creak-at ;   \ (kept going, moved along)
: eased ( F: -- deg )  turn-step @ s>f turn-frames s>f f/  180e f* 90e f-  deg>rad fsin 1e f+ 45e f* ;
: angle ( F: -- deg )  turning-back @ if  90e eased f-  else  eased  then  wk wk.sign @ s>f f* ;
: walkway-moves ( -- )  angle deg>rad fdup walkway turn-y object!  creak ;
: hewie-to-spot ( -- )   \ (an activity: Hewie walks to his spot on it, then waits there for the turn)
    hewie hold  hewie wait-move
    turning-back @ if  hewie wk wk.home @ wk wk.home-face @ -1 walk-path-move wk wk.home-x f@ wk wk.home-z f@ go-to
    else  hewie wk wk.out @ wk wk.out-face @ -1 walk-path-move wk wk.out-x f@ wk wk.out-z f@ go-to  then
    hewie let-go  begin  hewie-clear @ 0= while  yield  repeat ;
: turns ( -- )   \ (an activity: Hewie kept off its floor once he's off it - the original asks of its home floor either way)
    0 turn-step !
    creak-sound  turning-back @ if  90e  else  0e  then  wk wk.sign @ s>f f* deg>rad  wk wk.edge @ execute  creak-at
    begin  turn-step @ turn-frames < while
        1 turn-step +!  walkway-moves
        hewie-clear @ 0=  hewie home-floor on-group? 0= and if  aboard keeps-hewie-off floor-on  -1 hewie-clear !  then
        yield
    repeat
    hewie-in?  hewie aboard on-group? and if
        ['] hewie-to-spot stop-doing  hewie turning-back @ if  wk wk.home-end @  else  wk wk.out-end @  then  put-on-tri
        wk wk.side @ 0< 0= if  wk wk.side @ hewie-side  then
    then
    turning-back @ 0= dup walkway-floors  other-floor keeps-hewie-off floor-off
    across-floor kept-off floor-off  home-floor kept-off floor-off
    walkway-at  wk wk.stopped-xt @ execute  0 turning ! ;
: lever-pulled ( motion obj -- )   \ (her pull, the lever's object shown moving with it)
    {: m obj :}  fiona room-motion m + 5 start-blend8  5 frames
    home-part hide-part  across-part hide-part  back-object object-hide  across-object object-hide
    obj object-show  obj 1 lever-anim object ;
: pulls-lever ( -- )   \ (an activity: to the lever, pulled; the walkway turns)
    stalkers-stay state!  -1 turning !  0 hewie-clear !  turned? turning-back !
    aboard kept-off hole-floor or floor-on
    hewie-in?  hewie aboard on-group? and if  ['] hewie-to-spot start drop
    else  aboard keeps-hewie-off floor-on  -1 hewie-clear !  then
    fiona wait-move  things-clear
    fiona wk wk.file @ motions-for  fiona wk wk.lever @ wk wk.lever-face @ wk wk.lever-x f@ wk wk.lever-z f@ walk-to  fiona use-motions
    turning-back @ if  back-motion back-object  else  across-motion across-object  then  lever-pulled
    25 frames  open-sound room-bank fiona sound-by  55 frames  shut-sound room-bank fiona sound-by  fiona wait-anim
    ['] turns start drop
    turning-back @ if  wk wk.fact @ fact-off  else  wk wk.fact @ fact!  then
    back-object object-hide  across-object object-hide  turned? walkway-parts
    turning-back @ 0= if  wk wk.across-xt @ execute  then
    stalkers-stay state-off ;
: lever-offer ( area deg -- )   \ (each frame: facing the lever there, not while it turns)
    {: area deg :}  fiona area in-area?  fiona deg 50 heading? and  turning @ 0= and if  ['] pulls-lever unhurried offer  then ;
