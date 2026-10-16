\ party.fs - a dance party, for fun and for showing the project off (not the game's): a row of
\ golems doing their dance from the golem room (O_GLM's $1300, the SALTATIO scene), its music
\ (ADX $37), and Fiona and Hewie in front of them going through some of their own animations.
\ At the console, in any room: `dance-party` (game.fs). They face the camera as it is then; F3
\ frees the camera to fly about (the arrows, Page Up / Down, the right mouse button to look).
\ Hewie is brought in, Fiona put in her regular clothes for it. The story is stopped meanwhile;
\ `party-over` puts the golems away, her own costume back on, and lets Fiona and Hewie go.
IN: party
USING: engine game-state actors messages common vectors story.words ;

\ ---- what they do (edit these lists: each played through, then round again) ----
create her-dances  $1D00 , $1D01 , $1D02 , $A00 , $A01 , $A02 , $905 ,
7 constant #her-dances
create his-dances  $1B00 , $1C00 , $1B03 , $1C01 , $1C03 , $1B04 , $1C05 , $1C06 ,
8 constant #his-dances
5 constant #golems
14e fconstant golem-gap      \ (between the golems, side to side)
26e fconstant golem-back     \ (how far behind her the row stands)
9e fconstant hewie-side      \ (Hewie by her side)
$1300 constant golem-dance   $37 constant dance-music

\ ---- the line-up: she where she is, facing the camera; the golems in a row behind her ----
create here 12 allot   create eye 12 allot   create spot 12 allot
fvariable facing   \ (toward the camera)
fvariable ob-back  fvariable ob-side  fvariable ob-s  fvariable ob-c
: fiona-id ( -- id )  s" fiona" actor-named ;
: hewie-id ( -- id )  s" hewie" actor-named ;
: line-up ( -- )
    here fiona-id body-at vec-copy
    camera cam.x sf@ eye sf!  here 4 + sf@ eye 4 + sf!  camera cam.z sf@ eye 8 + sf!
    here eye vec-heading facing f! ;
: out-by ( F: back side -- )   \ (spot = her place moved `back` away from the camera, `side` across)
    ob-side f!  ob-back f!  facing f@ fsin ob-s f!  facing f@ fcos ob-c f!
    here sf@  ob-s f@ ob-back f@ f* f-  ob-c f@ ob-side f@ f* f+  spot sf!
    here 8 + sf@  ob-c f@ ob-back f@ f* f-  ob-s f@ ob-side f@ f* f-  spot 8 + sf!
    spot sf@  here 4 + sf@  spot 8 + sf@  nav-floor drop  spot 4 + sf! ;
: place-msg ( F: x y z yaw -- ) ( -- x y z yaw )  xyzw>cells ;

state: party-state
  #golems cells field golems   \ their models
  cell field her-at  cell field his-at   \ (the dances' places in the lists)
  cell field her-model  cell field his-model
  cell field her-t  cell field his-t   \ (frames in the present one)
  cell field her-last  cell field his-last   \ (its frame last time)
  cell field was-wearing   \ (her costume before: progress variable $26)
  cell field settle-t      \ frames before the models are taken (her costume's model loading)
end-state
: costume-var ( -- addr )  progress pr.vars $26 + ;
: dress ( n -- )  dup costume-var c!  fiona-id send costume ;

: golem ( i -- addr )  cells golems + ;
variable ga   \ (the golem being set up)
: golems-in ( -- )
    #golems 0 do
        s" O_GLM/GLM_000" actor-load  dup i golem !  actor ga !
        golem-back  i #golems 1- 2/ - s>f golem-gap f*  out-by
        spot sf@ ga @ act.x sf!  spot 4 + sf@ ga @ act.y sf!  spot 8 + sf@ ga @ act.z sf!  facing f@ ga @ act.yaw sf!
        1 ga @ act.visible l!  1 ga @ act.loop l!
        i golem @ golem-dance motion!
        i 30 * s>f  ga @ act.frame sf!   \ (each a little behind the one before)
    loop ;
: golems-out ( -- )  #golems 0 do  i golem @ dup 0< 0= if  actor-free  else  drop  then  -1 i golem !  loop ;

\ ---- Fiona and Hewie: each animation held to its end, then the next ----
: next-in ( model list n at -- anim )   \ (the next of the list the model has; -1 none)
    {: model list n at :}
    n 0 do
        at @ cells list + @  at @ 1+ n mod at !
        dup model swap has-motion? if  unloop exit  then  drop
    loop  -1 ;
: her-next ( -- )
    her-model @ her-dances #her-dances her-at next-in  dup 0< if  drop exit  then
    0 -1 fiona-id send hold-anim  0 her-t !  0 her-last ! ;
: his-next ( -- )
    his-model @ his-dances #his-dances his-at next-in  dup 0< if  drop exit  then
    0 -1 hewie-id send hold-anim  0 his-t !  0 his-last ! ;
: came-round? ( model last -- flag )   \ (done, or a looped one back at its start: its frame went down)
    {: model last :}  model motion-done?
    model actor act.frame sf@ f>s  dup last @ <  swap last !  or ;
: dancing ( -- )   \ (their models read afresh: hers changes with her costume)
    0 char-model her-model !  1 char-model his-model !
    1 her-t +!  1 his-t +!   \ (and, whatever, 10 s each)
    her-model @ her-last came-round?  her-t @ 300 > or if  her-next  then
    his-model @ his-last came-round?  his-t @ 300 > or if  his-next  then ;

behaviour partying
  on spawned ( -- )
      #golems 0 do  -1 i golem !  loop  0 her-at !  0 his-at !  -1 her-model !  -1 his-model !
      s" story" actor-named send story-stop
      costume-var c@ was-wearing !  0 dress   \ (her regular clothes)
      hewie-id send join-fiona
      line-up  golems-in
      here vec@ facing f@ place-msg  fiona-id send place
      0e hewie-side out-by  spot vec@ facing f@ place-msg  hewie-id send place
      -1 fiona-id send scripted  -1 hewie-id send scripted
      dance-music 1 1e bgm  10 settle-t !
      self subscribe tick ;
  on tick ( -- )
      settle-t @ if  -1 settle-t +!  exit  then
      her-model @ 0< if   \ (their models, once they are placed: the story's slots 0 and 1)
          0 char-model her-model !  1 char-model his-model !
          her-next  his-next  exit
      then
      dancing ;
  on killed ( -- )
      golems-out  0 fiona-id send scripted  0 hewie-id send scripted
      was-wearing @ dress ;
end-behaviour

: party-spawn ( -- )  s" party" actor-named 0< 0= if  exit  then  partying party-state s" party" spawn drop ;
: party-over ( -- )  s" party" actor-named dup 0< if  drop  else  kill  then ;
