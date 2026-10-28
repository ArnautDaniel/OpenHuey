\ story/places/castle-2f-12.fs - the nursery in Belli Castle's 2F ($26): the wardrobe to hide in
\ (Hewie, near, in with her; out by the button, or in a panic, or the stalker by it - he rattles it,
\ she shaking inside; he finds her: Debilitas drags her out, the others see her and she runs), the
\ stalker by the rocking chair; the three rocking chairs (set rocking by a blow, the first a while
\ the first time; Hewie watching it), the box (its slot - the right thing used: it opens, the
\ Inquisitor Earrings and the Jade Choker; else nothing), the caged doll and the rocking horse in
\ turn, the doll in the chair, the crib, the Refined Antimony Hewie sniffs out, a vase (the
\ Medallion). A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-2f-12.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.castle-2f-12
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects keys story.kit fiona.anims fiona.sounds stalker.anims ;

own-reset

\ ---- what is said here ----
0 constant box-msg         \ "It's a box, but I can't open it."
1 constant slot-msg        \ "There is a slot here. Don't tell me this is going to be like the last one...?"
2 constant box-open-msg    \ "The box is open."
3 constant caged-doll-msg  \ "Why is there a doll... in a cage? Maybe someone is just trying to keep one of their..."
4 constant so-real-msg     \ "The doll is so real, it looks like it could move at any second."
5 constant chair-doll-msg  \ "There is a doll slumped in the chair. Perhaps this is the nursery?"
6 constant horse-msg       \ "It's a child's rocking horse."
7 constant childhood-msg   \ "Even if I wanted to revisit my childhood with a quick ride, I don't think this will..."
8 constant nothing-msg     \ "Nothing happened. Nothing at all."
9 constant crib-msg        \ "It's a crib for a small child. The sheets have become discolored from lack of use. I gu..."
$A constant empty-msg      \ "Just my luck... it's empty."

\ ---- its own state (a visit) ----
own: box-remarked  own: doll-remarked  own: horse-remarked
own: hide-step             \ the hiding's turns: 1 she is going in (Hewie in with her), 2 she is out (him too)
own: stage                 \ found in there: 1 being opened, 2 opened
own: shaking               \ he rattles it: she can't get out
own: searched              \ his rattle done (his walk there spared after)
own: hewie-along  own: hewie-in-place  own: left-in-panic  own: first-time
own: watching              \ Hewie watching the rocking chair (once)

\ ---- the room's things ----
0 constant sparkle-fx   1 constant medallion-fx   2 constant antimony-fx   0 constant doors-part
0 constant left-door   1 constant right-door   0 constant vase-floor   1 constant wardrobe-floor
4 constant her-motions   5 constant his-motions   6 constant stalker-motions
6 constant wardrobe-area   7 constant cage-area   8 constant horse-area   9 constant chair-area
$A constant box-area   $B constant crib-area   6 constant search-var
0 constant open-sound   1 constant shut-sound   3 constant glint-sound   4 constant place-sound   $28 constant wardrobe-rattle
: doors-anim ( anim -- )  dup left-door 1 rot object  right-door 1 rot object ;   \ (the wardrobe's doors)
: open? ( -- flag )  c2f-12-flag1 fact? ;   \ (the wardrobe standing open)
c2f-12-breakable-broken 1 2  -17.5e 0e 47e 5e 8e  breakable: the-vase
the-vase $80404040 0 4 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv
the-vase vase-floor frees-floor  the-vase $48 noise-from

\ ---- the box's lid ("left", "right"): at once open, or opening a step ----
: lid-by ( F: d -- )  fdup fnegate s" left" turn-y object+!  s" right" turn-y object+! ;

\ ---- the three rocking chairs ("movechair_1..3": +$30 the rock's phase in degrees, +$34 its size,
\ +$38 how fast it dies down; the tilt about x) ----
$30 constant rock-phase   $34 constant rock-size   $38 constant rock-fade   $3C constant rock-word
: chair ( k -- addr len )  case  0 of  s" movechair_1"  endof  1 of  s" movechair_2"  endof  >r s" movechair_3" r>  endcase ;
: creak ( id vol -- )  >r room-bank r> 0  2.0866e 0.3e -2.0861e  sound-at ;   \ (at the first chair)
: chair-still ( k -- )
    chair {: a n :}  a n placed-here? 0= if  exit  then
    0 a n rock-word placed-l!  0e a n rock-fade object!  0e a n rock-size object!  0e a n rock-phase object! ;
: chair-rocks ( k -- )   \ (a step: on 6 degrees; each swing smaller, the first creaking by its size)
    dup chair {: k a n :}  a n placed-here? 0= if  exit  then
    a n rock-size object@ f0> 0= if  exit  then
    6e a n rock-phase object+!
    a n rock-phase object@ 360e f< 0= if
        -360e a n rock-phase object+!  a n rock-fade object@ fnegate a n rock-size object+!
        a n rock-size object@ f0< if  0e a n rock-size object!  then
        k 0= if  2  -100e 1e a n rock-size object@ f- f* f>s creak  then
    then
    a n rock-phase object@ deg>rad fsin a n rock-size object@ f* 10e f* deg>rad
    fdup pi f> if  2e pi f* f-  then  a n turn-x object! ;
: chair-set ( back? k -- )   \ (rocking at full size from its tilt now, forward or back - a creak)
    chair {: back a n :}  a n placed-here? 0= if  exit  then
    a n turn-x object@ rad>deg 10e f/ deg>rad fasin rad>deg
    back if  180e fswap f-  then  a n rock-phase object!
    1e a n rock-size object!  0.05e a n rock-fade object!  0 a n rock-word placed-l!  5 0 creak ;
: chairs-set ( back? -- )  3 0 do  dup i chair-set  loop  drop ;
: rocking ( -- )   \ (a blow by them sets them rocking - back by the first, on by the second)
    begin
        -4e 0e 4e 4e 7e struck-in? if  true chairs-set  then
        6e 0e -7e 3e 7e struck-in? if  false chairs-set  then
        3 0 do  i chair-rocks  loop  yield
    again ;
: still-rocking? ( -- flag )  s" movechair_1" rock-size object@ 0.3e f> ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  wardrobe-floor keeps-hewie-off floor-on  wardrobe-floor keeps-stalkers-off floor-on
    open? if  doors-part show-part  wardrobe-floor barred-floor floor-on  left-door object-hide  right-door object-hide
    else  doors-part hide-part  wardrobe-floor barred-floor floor-off  left-door object-show  right-door object-show  then
    3 0 do  i chair-still  loop  ['] rocking start drop
    c2f-12-flag2 fact? 0= if  false chairs-set  c2f-12-flag2 fact!  then
    the-vase as-it-stands  the-vase broken?  c2f-12-medallion fact? 0= and if  medallion-fx -17.5e 1e 47e flicker-at  then
    c2f-12-refined-antimony-found fact?  c2f-12-refined-antimony fact? 0= and if  antimony-fx 22.9e 1e -9.7e flicker-at  then
    got-jade-choker fact? if  2.25e lid-by  then ;

\ ---- the wardrobe: in (Hewie with her, near), lying low; out - by the button, in a panic, or found ----
: turns-to-him ( -- )  stalker-in? if  fiona stalker look-at  yield  fiona stalker body-pos fswap fdrop turn-to-xz  then ;
: out-quietly ( -- )   \ (her own way out: the doors shut behind her; the wardrobe shut)
    1 hide-step +!  fiona follow  fiona-hidden state-off
    stalker-in? if  0 stalker-order  stalker let-go  then
    fiona true noclip  2 doors-anim  fiona room-motion 2 + 0 start-blend
    16 frames  open-sound room-bank fiona sound-by  56 frames  shut-sound room-bank fiona sound-by  fiona wait-anim
    fiona false noclip  turns-to-him  c2f-12-flag1 fact-off ;
: out-in-panic ( -- )   \ (bursting out: the doors left open)
    fiona hold  1 hide-step +!  -1 left-in-panic !  fiona follow  fiona-hidden state-off
    stalker-in?  stage @ 0= and if  0 stalker-order  stalker let-go  then
    fiona true noclip  4 doors-anim  fiona room-motion 4 + 0 start-blend  7 frames  open-sound room-bank fiona sound-by
    fiona wait-anim  fiona false noclip  turns-to-him  c2f-12-flag1 fact!  wardrobe-floor barred-floor floor-on ;
: dragged-out ( -- )   \ (Debilitas has the doors open: out she comes)
    begin  stage @ 2 < while  yield  repeat
    1 hide-step +!  2 avoid-prompt  fiona follow  fiona-hidden state-off
    fiona true noclip  fiona room-motion 3 + 0 play-blend  fiona false noclip
    c2f-12-flag1 fact!  wardrobe-floor barred-floor floor-on ;
: goes-in ( -- )   \ (the doors open - the first time shut - in, and shut)
    fiona true noclip  fiona -1 look-at  yield
    open? if
        fiona room-motion 5 start-blend8  5 frames  doors-part hide-part  left-door object-show  right-door object-show
        0 doors-anim  40 frames  shut-sound room-bank fiona sound-by  fiona wait-anim
    else
        fiona room-motion 1 + 5 start-blend8  5 frames  doors-part hide-part  left-door object-show  right-door object-show
        1 doors-anim  14 frames  open-sound room-bank fiona sound-by  60 frames  shut-sound room-bank fiona sound-by  fiona wait-anim
    then
    fiona false noclip  wardrobe-floor barred-floor floor-off ;
: hewie-hides ( -- )   \ (in with her; out after her)
    hewie hold  hewie $B4 45 -1 walk-path-move 17.237e 27.889e go-to  hewie use-motions  -1 hewie-in-place !
    begin  hide-step @ 1 < while  yield  repeat
    hewie true noclip  first-time @ if  hewie room-motion 1 + 5 start-blend8  else  hewie room-motion 5 start-blend8  then
    hewie wait-anim  hewie false noclip  hewie-hidden state!
    begin  hide-step @ 2 <  fiona held? and while  yield  repeat
    hewie-hidden state-off  hewie true noclip
    left-in-panic @ if  hewie room-motion 4 + 0 start-blend  7 frames  open-sound room-bank fiona sound-by  hewie wait-anim
    else  stage @ 0= if  hewie room-motion 2 + 0 play-blend  else  hewie room-motion 3 + 0 play-blend  then  then
    hewie false noclip ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  fiona wait-move
    0 stage !  0 shaking !  open? 0= first-time !  0 hide-step !  0 hewie-along !  0 hewie-in-place !  0 left-in-panic !
    hewie-in? if   \ (Hewie, free and near her: in with her)
        hewie held? 0=  hewie-near-now @ and  fiona body-at hewie body-at vec-dist 30e f<= and if
            hewie his-motions motions-for  ['] hewie-hides start drop  -1 hewie-along !
        then
    then
    fiona her-motions motions-for  stalker stalker-motions motions-for
    fiona $B4 45 -1 walk-move 16.729e 33.985e go-to  fiona use-motions
    hewie-along @ if  fiona hewie look-at  yield  begin  hewie-in-place @ 0= while  yield  repeat  then
    1 hide-step !  goes-in
    stalkers-stay state-off  fiona-hidden state!  0 avoid-prompt
    begin
        circle button-pressed? 0=  shaking @ or if
            stage @ 1 = if
                0 shaking !  seized-sound common-bank fiona sound-by
                stalker-in? if  dragged-out  else  out-quietly  then  exit
            then
            in-panic? if  out-in-panic exit  then
            hidden-calm  yield
        else
            stalker-in? if  fiona body-at stalker body-at vec-dist 50e f<= if  out-in-panic exit  then  then
            out-quietly  wardrobe-floor barred-floor floor-off  exit
        then
    again ;

\ ---- the stalker in while she hides: by the rocking chair (Debilitas, the first time it still
\ rocks), else his search - he rattles it, or finds her ----
: door-for-him-too ( -- )  0 door-for-him  1 door-for-him ;
: looks-at-chair ( -- )
    stalker hold  stalker wait-move  door-for-him-too  stalker $3A -165 3.123e 11.435e walk-to
    stalker look-about-3-anim play  3 stalker-order  c2f-12-flag3 fact! ;
: rattles-it ( -- )   \ (her shaking inside, frightened the more)
    stalker hold  stalker wait-move  door-for-him-too  stalker $B4 45 15e 29e walk-to  -1 searched !
    -1 shaking !  stalker room-motion 2 + 10 start-blend
    30 frames  wardrobe-rattle common-bank stalker sound-by  30 threat
    25 frames  wardrobe-rattle common-bank stalker sound-by  20 threat
    23 frames  wardrobe-rattle common-bank stalker sound-by  10 threat
    stalker wait-anim  0 shaking !  0 searched !  stalker step-back-anim play  stalker let-go  120 summoner-sooner ;
: finds-her ( -- )
    stalker hold  stalker wait-move  door-for-him-too
    searched @ 0= if  stalker $B4 45 15e 29e walk-to  then  0 searched !
    1 stage !
    debilitas? if
        3 doors-anim  stalker room-motion 1 + 10 start-blend  12 frames  open-sound room-bank stalker sound-by
        4 frames  2 stage !  stalker wait-anim  stalker step-back-anim play
    else
        stalker there-anim play
        fiona-hidden state? if  2 avoid-prompt  panic-eased  ['] out-in-panic start drop  then
    then
    stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: search-outcome ( -- n )   \ (0 nothing, 1 he rattles it, 2 he finds her - by how often he has come)
    search-var var@ case
        0 of  0 chance? if  2  else  0  then  endof
        1 of  25 chance? if  2  else  1  then  endof
        2 of  50 chance? if  2  else  1  then  endof
        3 of  75 chance? if  2  else  1  then  endof
        >r 0 r>
    endcase
    debilitas? 0=  over 1 = and if  drop 2  then
    2 creature-moving? if  drop 2  then
    4 stalker-alert? if  drop 0  then ;
: searches-wardrobe ( -- )
    debilitas? if  stalker use-motions  then  stalker follow
    search-outcome  2 stalker-order
    case
        0 of  0 stalker-clock  120 summoner-sooner  endof
        1 of  150 stalker-clock  ['] rattles-it start drop  endof
        2 of  300 stalker-clock  ['] finds-her start drop  endof
    endcase
    0 searched !  search-var var@ 1+ search-var var! ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    debilitas?  c2f-12-flag3 fact? 0= and  still-rocking? and  2 creature-moving? 0= and if
        stalker follow  ['] looks-at-chair start drop
    else  searches-wardrobe  then ;

\ ---- the box: its slot - the right thing used, it opens; else nothing ----
: box-opens ( -- )
    {: | has-room :}  stalkers-stay state!  fiona hold
    inquisitor-earrings items:items-of 0>  jade-choker items:items-of 0> and 0= to has-room   \ (not both had: the box holds them)
    fiona wait-move  fiona $35 180 0e -31.5e place-at  1 7e 30e 0e 0e shot  world-frozen state!
    place-sound 0e 5e -39e room-sound  16 frames
    has-room if  sparkle-fx 0e 3e -39e flicker-at  then
    glint-sound room-bank fiona sound-by  30 0 do  0.075e lid-by  yield  loop  16 frames
    has-room if
        inquisitor-earrings items:items-of 0= if
            inquisitor-earrings item-named  inquisitor-earrings 1 give-item  inquisitor-earrings show-item-tab  fanfare  obtained-2-msg say
        then
        jade-choker items:items-of 0= if
            16 frames  jade-choker item-named  jade-choker 1 give-item  jade-choker show-item-tab  fanfare  obtained-2-msg say
        then
        sparkle-fx effect-off
    else  empty-msg say  then
    got-jade-choker fact!  4 frames  world-frozen state-off  stalkers-stay state-off  shot-off ;
: nothing-happens ( -- )  fiona hold  fiona wait-move  place-sound 0e 5e -39e room-sound  30 frames  nothing-msg say ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    case
        $B of  ['] box-opens start drop  true  endof
        $C of  ['] nothing-happens start drop  true  endof
        >r false r>
    endcase ;

\ ---- what she may do or say here ----
: caged-doll ( -- )
    fiona wait-move  fiona 23.3e 4e turn-to-xz  fiona look-close-anim start-anim  40 frames
    doll-remarked @ if  so-real-msg  else  -1 doll-remarked !  caged-doll-msg  then  say  fiona wait-anim ;
: chair-doll ( -- )  fiona wait-move  chair-doll-msg say ;
: horse ( -- )  fiona wait-move  horse-remarked @ if  childhood-msg  else  -1 horse-remarked !  horse-msg  then  say ;
: the-box ( -- )
    fiona $35 180 -1 walk-move 0e -31.5e go-to  world-frozen state!  1 20e 10e 0e 0e shot
    got-jade-choker fact? if  box-open-msg  else  box-remarked @ if  slot-msg  else  -1 box-remarked !  box-msg  then  then  say
    shot-off  world-frozen state-off ;
: crib ( -- )  fiona wait-move  fiona -19e -19e turn-to-xz  crib-msg say ;
: pick-antimony ( -- )  refined-antimony c2f-12-refined-antimony antimony-fx 22.9e -9.7e picks-up ;
: pick-medallion ( -- )  medallion c2f-12-medallion medallion-fx -17.5e 47e picks-up ;
: spots ( -- )
    fiona wardrobe-area in-area?  fiona 22 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    fiona cage-area 60 faces-area? if  ['] caged-doll unhurried offer  then
    fiona horse-area in-area?  fiona 75 50 heading? and if  ['] horse unhurried offer  then
    fiona chair-area 60 faces-area? if  ['] chair-doll unhurried offer  then
    fiona box-area in-area?  fiona 90 50 heading? and if  ['] the-box unhurried offer  then
    fiona crib-area 60 faces-area? if  ['] crib unhurried offer  then
    c2f-12-refined-antimony-found fact?  c2f-12-refined-antimony fact? 0= and  fiona 22.9e 1e -9.7e 5e 5e touches? and if
        ['] pick-antimony calm-only offer  then
    the-vase broken? if
        c2f-12-medallion fact? 0=  fiona -17.5e 1e 47e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    else  the-vase breaks? if  medallion-fx -17.5e 1e 47e flicker-at  then  then ;

\ ---- Hewie: watching the rocking chair (calm, at her command, it still rocking), the Refined
\ Antimony he sniffs out ----
: chair-seat ( F: -- x y z )   \ (3 above the second chair's seat: 6 up its back, tipped by its rock, turned with it)
    {: | F: tip F: yaw F: out :}
    s" movechair_2" rock-phase object@ deg>rad fsin  s" movechair_2" rock-size object@ f*  90e f* deg>rad to tip
    s" movechair_2" turn-y object@ to yaw  tip fsin 6e f* to out
    s" movechair_2" place-x object@  out yaw fsin f* f+
    s" movechair_2" place-y object@  tip fcos 6e f* f+ 3e f+
    s" movechair_2" place-z object@  out yaw fcos f* f+ ;
: watches-chair ( -- )
    hewie hold  hewie wait-move  hewie 2e -2e turn-to-xz  -1 watching !
    180 0 do  chair-seat hewie-eyes-on  yield  loop ;
: hewie-watch ( -- )
    watching @  hewie-in? 0= or  hewie-cond-now @ down-cond = or  danger-now @ calm <> or if  exit  then
    hewie-cmdable-now @  still-rocking? and  hewie 2.09e 0e -2.09e 20e 5e centred-in? and  hewie held? 0= and if
        ['] watches-chair start drop
    then ;
: antimony-sniffed ( -- )  c2f-12-refined-antimony-found c2f-12-refined-antimony antimony-fx  22.9e 1e -9.7e hewie-finds ;
: hewie-sniffs ( -- )
    c2f-12-refined-antimony-found fact?  scrambling? 0= or if  exit  then
    hewie 22.9e 0e -9.7e 10e 5e touches? 0= if  exit  then
    ['] antimony-sniffed start-only ;

\ ---- every frame ----
create cameras  4 , 1 , -1 ,   5 , 0 , 0 ,   \ (area, set, path)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit)
create entries  1 , -1 ,   1 , -1 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )  2 exits-out  cameras 2 cameras-at  ahead 2 ways-out  hewie-sniffs  hewie-watch  spots ;
: person-in ( i -- )
    case
        fiona-p of  entries 2 ways-in  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-12 place!
' items-used castle-2f-12 items-here
