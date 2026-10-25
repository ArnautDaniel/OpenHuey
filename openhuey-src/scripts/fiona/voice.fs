\ fiona/voice.fs - her voice and the sounds of her motions on their key frames (Fiona_Voice,
\ Fiona_MotionSounds). Her gesture lines are the commands' (fiona/commands.fs command-sounds).
IN: fiona.voice
USING: engine vectors game-state actors messages common fiona.state fiona.anims fiona.sounds fiona.model fiona.fear fiona.commands ;

\ Fiona_Voice: her cry, muffled ($1C, bank 6) when her mouth is under water - a triangle with
\ the water's flags ($2008000) between her feet and her head
$2008000 constant water
create head-spot /vec allot
: under-water? ( -- flag )
    her-tri @ 0< if  false exit  then
    head-spot self body-head vec-copy   \ (her head, as she was posed: looks.fs tell-head)
    her-tri @ her-at head-spot water nav-crosses? ;
: voice ( id -- )
    under-water? if  drop  muffled-sound 6 0 0 her-pos sound-at  flooded? if  1 her-at water-step  then  exit  then
    0 her-sound ;

\ ---- Fiona_MotionSounds: the key frames (event 1, event $10) ----

: on-key ( -- )   \ (event 1)
    anim@ case
        throw-anim of  effort-sound 0 her-sound  endof   aimed-throw-anim of  effort-sound 0 her-sound  endof   \ (letting go of a thing)
        shove-anim of   \ (her shove: a grunt but in the special costumes; heard)
            costume dup illegal-outfit <> swap cowgirl-outfit <> and if  effort-sound 0 her-sound  then  $1F noise-here  endof
    endcase ;
: on-mark ( -- )   \ (event $10)
    anim@ case
        flee-turn-anim of  flee-turn-sound 0 her-sound  endof
        pull-free-anim of  pull-free-cry voice  endof   dragged-off-anim of  pull-free-cry voice  endof   \ (pulling free, dragged, breaking away, carried)
        shake-free-anim of  pull-free-cry voice  endof   led-walking-anim of  pull-free-cry voice  endof
        lying-anim of  slide-sound 0 her-sound  endof                       \ (sliding down after a panic fall)
        thrown-back-anim of  falling-cry voice  endof   thrown-forward-anim of  falling-cry voice  endof   fall-anim of  falling-cry voice  endof
        rattle-behind-anim of  rattle-sound 0 her-sound  endof   rattle-front-anim of  rattle-sound 0 her-sound  endof
        shove-anim of  costume case  illegal-outfit of  illegal-blow-sound 0 her-sound  endof  cowgirl-outfit of  cowgirl-blow-sound 0 her-sound  endof  endcase  endof
    endcase ;
: motion-sounds ( -- )
    her-model @ 0< if  exit  then
    motion-bits dup 1 and if  on-key  then  $10 and if  on-mark  then ;
