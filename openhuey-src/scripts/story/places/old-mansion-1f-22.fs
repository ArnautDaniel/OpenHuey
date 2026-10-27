\ story/places/old-mansion-1f-22.fs - a room of the old mansion's 1F with a cabinet to hide in (its
\ own, not the kit's: Hewie, near and at her command, squeezes in beside her; out by the button -
\ or bursting out in a panic, or the stalker by her). The stalker coming in while she hides: by the
\ odds he passes by, or Daniella grabs at her through it, or he opens it - Daniella hauls her out,
\ the others see her there (she bursts out). The scene's figure kept by its near way; two candles;
\ the Refined Antimony; the doorways' shade. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-1f-22.fs (converted from the game's, not loaded).
IN: story.places.old-mansion-1f-22
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names keys story.kit fiona.anims fiona.sounds ;

own-reset

own: stage        \ his opening it: 0 not, 1 at it, 2 open
own: held         \ Daniella grabbing at her through it
own: closed-at-first   \ it was shut as she went in
own: burst-out    \ she burst out
own: hewie-along  own: hewie-in-place
own: steps        \ hers: 1 in, 2 out (Hewie follows them)
own: searched     \ his grab under way
0 constant antimony-fx   1 constant flame-fx   2 constant flame-2-fx
0 constant open-sound   1 constant shut-sound   5 constant grab-sound
2 constant her-motions   3 constant his-motions   4 constant daniella-motions   $10 constant search-var
$47 constant cabinet-tri   6 constant cabinet-area   $1C constant scene-figure   3 constant figure-slot

\ ---- the cabinet: its two doors, shut (the floor before it barred) or open ----
: doors ( anim -- )  {: a :}  0 1 a object  1 1 a object ;
: cabinet-shut ( -- )  om1f-22-flag1 fact!  0 barred-floor floor-on ;
: shown-shut ( -- )   \ (as she comes in)
    om1f-22-flag1 fact? if  0 show-part  0 barred-floor floor-on  0 object-hide  1 object-hide
    else  0 hide-part  0 barred-floor floor-off  0 object-show  1 object-show  then ;
: came-in ( exit -- )
    drop  room-sounds  0 keeps-hewie-off floor-on  0 keeps-stalkers-off floor-on  shown-shut
    om1f-22-refined-antimony fact? 0= if  antimony-fx 147.17e 11e 86.89e flicker-at  then
    flame-fx 1 -36.7e 19.1e -34.8e flame-at  flame-2-fx 1 -36.7e 19.1e 34.8e flame-at
    1 $2300 reverb ;
: near-her? ( id F: d -- flag )  {: id F: d :}  id here? 0= if  false exit  then  fiona body-at id body-at vec-dist d f<= ;
: her-looker? ( -- flag )  st-kind-now @ dup daniella-kind = swap kind34-kind = or ;

\ ---- out of it: stepping out, bursting out, hauled out ----
: out ( -- )  2 steps !  fiona follow  fiona-hidden state-off ;
: faces-him ( -- )   \ (out, the stalker here: her eyes on him, turned to him)
    stalker-in? 0= if  exit  then  fiona stalker look-at  yield  fiona stalker body-pos fswap fdrop turn-to-xz ;
: him-off ( -- )  stalker-in? if  0 stalker-order  stalker let-go  then ;
: steps-out ( -- )
    out  him-off  fiona true noclip  2 doors  fiona room-motion 2 + 0 start-blend
    16 frames  open-sound room-bank fiona sound-by  56 frames  shut-sound room-bank fiona sound-by
    fiona wait-anim  fiona false noclip  faces-him  om1f-22-flag1 fact-off ;
: bursts-out ( -- )
    out  -1 burst-out !  stage @ 0= if  him-off  then
    fiona true noclip  4 doors  fiona room-motion 4 + 0 start-blend  7 frames  open-sound room-bank fiona sound-by
    fiona wait-anim  fiona false noclip  faces-him  cabinet-shut ;
: forced-out ( -- )  fiona hold  bursts-out ;   \ (seen there: out of it, at once)
: pulled-out ( -- )
    begin  stage @ 2 = 0= while  yield  repeat
    out  2 avoid-prompt  fiona true noclip  fiona room-motion 3 + 0 play-blend  fiona false noclip  cabinet-shut ;

\ ---- in it (Hewie beside her, if he's near and at her command) ----
: hewie-hides ( -- )
    hewie hold  hewie his-motions motions-for  hewie cabinet-tri 90 -1 walk-path-move 140.97e 60.42e go-to
    hewie use-motions  -1 hewie-in-place !  begin  steps @ 0= while  yield  repeat
    hewie true noclip  hewie room-motion closed-at-first @ 0= if  1+  then  5 start-blend8  hewie wait-anim  hewie false noclip
    hewie-hidden state!
    begin  fiona held?  steps @ 2 <> and while  yield  repeat
    hewie-hidden state-off  hewie true noclip
    burst-out @ if  hewie room-motion 4 + 0 start-blend  7 frames  open-sound room-bank fiona sound-by  hewie wait-anim
    else  hewie room-motion  stage @ 0= if  2 +  else  3 +  then  0 play-blend  then
    hewie false noclip ;
: goes-in ( -- )
    om1f-22-flag1 fact? if
        fiona room-motion 5 start-blend8  5 frames  0 hide-part  0 object-show  1 object-show  0 doors
        40 frames  shut-sound room-bank fiona sound-by  fiona wait-anim
    else
        fiona room-motion 1+ 5 start-blend8  5 frames  0 hide-part  0 object-show  1 object-show  1 doors
        14 frames  open-sound room-bank fiona sound-by  60 frames  shut-sound room-bank fiona sound-by  fiona wait-anim
    then ;
: lies-low ( -- )   \ (till she leaves, or is hauled or bursts out)
    begin
        circle button-pressed? 0=  held @ or if
            stage @ 1 = if
                0 held !  seized-sound common-bank fiona sound-by
                stalker-in? if  pulled-out  else  steps-out  then  exit
            then
            in-panic? if  bursts-out exit  then
            hidden-calm  yield
        else
            stalker 50e near-her? if  bursts-out exit  then
            steps-out  0 barred-floor floor-off  exit
        then
    again ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  fiona hold  fiona wait-move
    0 stage !  0 held !  om1f-22-flag1 fact? closed-at-first !  0 steps !  0 hewie-along !  0 hewie-in-place !  0 burst-out !
    hewie 30e near-her?  hewie held? 0= and  hewie-near-now @ and if
        -1 hewie-along !  ['] hewie-hides start drop
    then
    fiona her-motions motions-for  her-looker? if  stalker daniella-motions motions-for  then
    fiona cabinet-tri 90 144.89e 65.089e walk-to  fiona use-motions
    hewie-along @ if  fiona hewie look-at  yield  begin  hewie-in-place @ 0= while  yield  repeat  then
    1 steps !  fiona true noclip  fiona -1 look-at  yield  goes-in  fiona false noclip
    0 barred-floor floor-off  stalkers-stay state-off  fiona-hidden state!  0 avoid-prompt
    lies-low ;

\ ---- the stalker in while she hides: he passes, grabs at her through it, or opens it ----
: grabs ( -- )   \ (Daniella: her hands through it - her fright up twice)
    stalker hold  stalker wait-move  0 door-for-him  stalker cabinet-tri 90 144.89e 65.089e walk-to
    -1 searched !  -1 held !  stalker room-motion 2 + 10 start-blend
    25 frames  grab-sound room-bank stalker sound-by  40 threat  9 frames  grab-sound room-bank stalker sound-by  20 threat
    stalker wait-anim  0 held !  0 searched !  stalker stalker.anims:step-back-anim play  120 summoner-sooner ;
: opens ( -- )
    stalker hold  stalker wait-move  2 door-for-him
    her-looker? if  searched @ 0= if  stalker cabinet-tri 90 144.89e 65.089e walk-to  then  0 searched !
    else  stalker $4F 80 134.56e 61.52e walk-to  then
    1 stage !
    her-looker? if
        3 doors  stalker room-motion 1+ 10 start-blend  16 frames  2 stage !  10 frames  open-sound room-bank stalker sound-by
        stalker wait-anim  stalker stalker.anims:step-back-anim play
    else
        stalker stalker.anims:there-anim play
        fiona-hidden state? if  2 avoid-prompt  panic-eased  ['] forced-out start drop  then
    then
    stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: finding ( -- result )   \ (0 he passes, 1 a grab, 2 he opens it)
    search-var var@ case
        0 of  10 chance? if  2  else  0  then  endof
        1 of  25 chance? if  2  else  1  then  endof
        2 of  50 chance? if  2  else  1  then  endof
        3 of  75 chance? if  2  else  1  then  endof
        >r 0 r>
    endcase
    her-looker? 0=  over 1 = and if  drop 2  then
    2 creature-moving? if  drop 2  then ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    stalker follow  finding  2 stalker-order
    case
        0 of  0 stalker-clock  120 summoner-sooner  endof
        1 of  150 stalker-clock  ['] grabs start drop  endof
        2 of  300 stalker-clock  ['] opens start drop  endof
    endcase
    0 searched !  search-var var@ 1+ search-var var! ;

\ ---- what she may take here ----
: take-antimony ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 147.17e 86.89e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  refined-antimony om1f-22-refined-antimony antimony-fx takes-up  fiona reach-back-anim play ;

create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 2 cameras-at
    fiona 2 entered? if  0 prepare-exit  scene-figure figure-slot 0 figure-in-by  then
    fiona 3 entered? if  1 prepare-exit  figure-slot figure-out  then
    fiona cabinet-area in-area?  fiona 45 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    om1f-22-refined-antimony fact? 0=  fiona 147.17e 11e 86.89e 5e 5e touches? and if  ['] take-antimony calm-only offer  then
    -4.5e 15e 110e  -4.5e 15e 79.8e  -4.5e -15e 110e  -4.5e -15e 79.8e  light-doorway
    -3e 3e 92.5e  -33.6e 3e 86.4e  -3e -20e 92.5e  -33.6e -20e 86.4e  light-doorway
    149.4e 25e 59.1e  159.4e 25e 59.1e  149.4e 0e 59.1e  159.4e 0e 59.1e  light-doorway ;
: person-in ( i -- )
    case
        fiona-p of  entries 2 ways-in  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-22 place!
