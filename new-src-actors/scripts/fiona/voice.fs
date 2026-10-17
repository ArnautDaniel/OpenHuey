\ fiona/voice.fs - her voice and the sounds of her motions on their key frames (Fiona_Voice,
\ Fiona_MotionSounds). Her gesture lines are the commands' (fiona/commands.fs command-sounds).
IN: fiona.voice
USING: engine vectors game-state actors messages common fiona.state fiona.anims fiona.model fiona.fear fiona.commands ;

\ Fiona_Voice: her cry, muffled ($1C, bank 6) when her mouth is under water - a triangle with
\ the water's flags ($2008000) between her feet and her head
$2008000 constant water
create mouth 12 allot
: under-water? ( -- flag )
    her-tri @ 0< if  false exit  then
    self body-head mouth 12 move   \ (her head, as she was posed: looks.fs tell-head)
    her-tri @ her-at mouth water nav-crosses? ;
: voice ( id -- )
    under-water? if  drop  $1C 6 0 0 her-pos sound-at  flooded? if  1 her-at water-step  then  exit  then
    0 her-sound ;

\ ---- Fiona_MotionSounds: the key frames (event 1, event $10) ----
: special-form ( -- n )  progress pr.vars $26 + c@ ;   \ (6 / 7: the two special strikes' forms)
: on-key ( -- )   \ (event 1)
    anim@ case
        $D00 of  $3C 0 her-sound  endof   $D01 of  $3C 0 her-sound  endof   \ (letting go of a thing)
        $E00 of   \ (her shove: a grunt but in the special forms; heard)
            special-form dup 6 <> swap 7 <> and if  $3C 0 her-sound  then  $1F noise-here  endof
    endcase ;
: on-mark ( -- )   \ (event $10)
    anim@ case
        $403 of  $F 0 her-sound  endof                       \ (the flee turn)
        $F02 of  $7D voice  endof   $1404 of  $7D voice  endof   \ (pulling free, dragged, breaking away, carried)
        $1403 of  $7D voice  endof   $1500 of  $7D voice  endof
        $B01 of  $7E 0 her-sound  endof                      \ (sliding down after a panic fall)
        $100B of  $80 voice  endof   $1008 of  $80 voice  endof   $B00 of  $80 voice  endof   \ (falling)
        $609 of  $71 0 her-sound  endof   $608 of  $71 0 her-sound  endof   \ (a locked door's rattle)
        $E00 of  special-form case  6 of  $21 0 her-sound  endof  7 of  $20 0 her-sound  endof  endcase  endof
    endcase ;
: motion-sounds ( -- )
    her-model @ 0< if  exit  then
    motion-bits dup 1 and if  on-key  then  $10 and if  on-mark  then ;
