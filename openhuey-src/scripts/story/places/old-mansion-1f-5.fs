\ story/places/old-mansion-1f-5.fs - the old mansion's kitchen: the big fire and its sound; the pot
\ (burnt meat; then it moves - she starts, panicking; then a child...); the Lavender; the jerky and
\ the Album Medallion sparkling (the kitchen's later room takes them), the ores' place while she has
\ none; the Refined Magnesia Hewie sniffs out. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-1f-5.fs in the history (converted from the game's; gone with
\ the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-5
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags items.names effects
    story.kit fiona.anims fiona.sounds ;

own-reset

$11 constant meat-msg      \ "Smells like burnt meat. A dreadful odor indeed... They must be cooking an animal'..."
$12 constant alive-msg     \ "Wait a second! It's still moving in there! It's being cooked alive!"
$13 constant child-msg     \ "The more I look, the more it seems to resemble a human child... But certainly suc..."
own: sounds-on
0 constant jerky-fx  1 constant lavender-fx  2 constant medallion-fx  3 constant ores-fx  4 constant magnesia-fx
0 constant room-loop-sound   1 constant sizzle-sound   $21 constant pot-var   $19 constant pot-area

: no-ores? ( -- flag )  0  $D 8 do  i items:items-of +  loop  0= ;
: magnesia-shows ( -- )  om1f-5-refined-magnesia-found fact?  om1f-5-refined-magnesia fact? 0= and if  magnesia-fx -9.11e 1e -21.78e flicker-at  then ;
: came-in ( exit -- )
    drop  room-sounds  $19 1e background  1 both-halves floor-on
    om1f-8-beef-jerky fact? 0= if  jerky-fx 122.09e 8.28e 10.26e flicker-at  then
    om1f-5-lavender fact? 0= if  lavender-fx -107.57e 1e 10.1e flicker-at  then
    om1f-8-album-medallion fact? 0= if  medallion-fx -140.37e 16e -25.97e flicker-at  then
    no-ores? if  ores-fx -111e 17e -115e flicker-at  then
    magnesia-shows  effects:big-fire ;   \ (a scene effect: gone with the room)

: the-pot ( -- )
    fiona wait-move  fiona -136e 12e turn-to-xz  fiona look-close-anim 10 play-blend
    pot-var var@ dup 1+ pot-var var!
    case
        0 of  meat-msg say  endof
        1 of  sizzle-sound -138e 5e 5e room-sound  16 frames  seized-sound common-bank fiona sound-by
              fiona startled-anim start-anim  in-panic? 0= if  3 panic-to  then  alive-msg say  endof
        >r child-msg say r>
    endcase ;
: take-lavender ( -- )  lavender om1f-5-lavender lavender-fx -107.57e 10.1e picks-up ;
: take-magnesia ( -- )  refined-magnesia om1f-5-refined-magnesia magnesia-fx -9.11e -21.78e picks-up ;
: magnesia-sniffed ( -- )  om1f-5-refined-magnesia-found om1f-5-refined-magnesia magnesia-fx  -9.11e 1e -21.78e  hewie-finds ;

: each-frame ( -- )
    sounds-on @ if  room-loop-sound -138e 11e 11e keep-sound
    else  room-sounds-in? if  room-loop-sound -138e 11e 11e room-sound  -1 sounds-on !  then  then
    3 exit-out  4 4 leaves-by
    om1f-5-refined-magnesia-found fact? 0=  scrambling? and  hewie -9.11e 0e -21.78e 10e 5e touches? and if
        ['] magnesia-sniffed start-only
    then
    fiona pot-area in-area?  fiona -90 50 heading? and if  ['] the-pot unhurried offer  then
    om1f-5-lavender fact? 0=  fiona -107.57e 1e 10.1e 5e 5e touches? and if  ['] take-lavender calm-only offer  then
    om1f-5-refined-magnesia-found fact?  om1f-5-refined-magnesia fact? 0= and  fiona -9.11e 1e -21.78e 5e 5e touches? and if
        ['] take-magnesia calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    3 4 4 way-in  4 2 2 way-in  came-by @ 3 = if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-5 place!
