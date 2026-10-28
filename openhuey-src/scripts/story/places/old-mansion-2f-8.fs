\ story/places/old-mansion-2f-8.fs - the gallery of the old mansion's 2F: a curtain to hide behind
\ (drawn round her; out by the button, or found - the stalker's search by the odds - or in a panic,
\ pulled back; a mandragora she carries cries out); the statue with the lamp (lit blue once the
\ way's found); a spirit put away by it the first time - the wall gives (out by the forced way to
\ the 1F); the hole (in it, out by its way; back through it); the picture; the bones; the book of
\ medicines (its recipes marked); the door behind her, water pouring in; the save clock; the
\ Camomile, the Lavender; the candle's flame; brought here the first time (Hewie by her, the doors
\ shut behind, the spirits gone, the file) or by the way behind the wall; Hewie's looks; its sounds;
\ the doorways' shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-2f-8.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-2f-8
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names keys story.kit fiona.anims fiona.sounds ;

own-reset

0 constant unlit-msg     \ "There is a statue clasping an oil lamp. The wick of the lamp is unlit."
1 constant blue-msg      \ "A mesmerizing blue flame now dances from the oil lamp's wick."
2 constant hole-msg      \ "A stale, damp gale of wind blows forth from the other side of the hole. Enter the hole?..."
3 constant insects-msg   \ "It's full of insects."
4 constant mother-msg    \ "A mother is lovingly holding a child. Why is her face painted red?"
5 constant bones-msg     \ "Discolored bones lay in a pile. I can lie to myself all I want to, but there is no deny..."
7 constant water-msg     \ "Water is flowing in. Guess there is no going back now..."
8 constant written-msg   \ "Something is written here..."
9 constant recipes-msg   \ "Recreatio Completely restores lost stamina. Sedatio Completely calms the player and ret..."
$A constant medicine-msg \ "This book sounds like it's about some kind of medicine."
own: wall-seen  own: looked-round  own: spot-noticed  own: sounds-on  own: frames-in
0 constant camomile-fx   1 constant lavender-fx   2 constant flame-fx
0 constant curtain-sound   1 constant shriek-sound   3 constant first-ambient   8 constant hum-sound   9 constant hum-2-sound
0 constant her-motions   $16 constant search-var   $5A constant wall-door   $FE constant through-hole

\ ---- the curtain round her ----
: curtain-by ( lo hi a b F: div -- )   \ ((frames - lo) / div, the frames kept to lo..hi)
    {: lo hi a b F: div :}  a b  frames-in @ lo max hi min lo - s>f div f/  s" Curtain" morph ;
: drawn ( motion last sound-at lo hi a b F: div -- )   \ (her motion, the curtain along with it)
    {: m last at lo hi a b F: div :}
    fiona true noclip  0 frames-in !  fiona m 5 start-blend8  5 frames
    begin  lo hi a b div curtain-by  yield
        frames-in @ last <  while
        frames-in @ at = if  curtain-sound room-bank fiona sound-by  then  1 frames-in +!
    repeat  fiona wait-anim  fiona false noclip ;
: curtain-in ( -- )  room-motion 56 44 $2B $38 0 1 13e drawn ;
: curtain-out ( -- )  room-motion 1+ 32 13 $C $20 1 0 20e drawn ;
: curtain-flung ( -- )  room-motion 2 + 18 6 7 $12 1 0 11e drawn ;
: pulled-back ( -- )   \ (found, or in a panic)
    found-her @ if  2 avoid-prompt  then
    fiona follow  fiona-hidden state-off  curtain-flung
    found-her @ 0= if  stalker let-go  stalker-in? if  0 stalker-order  then  then ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  fiona hold  fiona her-motions motions-for  fiona wait-move
    fiona $182 180 -67.8e -119.5e walk-to  fiona use-motions  curtain-in  fiona -1 idle
    stalkers-stay state-off  fiona-hidden state!  0 found-her !  0 avoid-prompt
    mandragora? if  shriek-sound room-bank fiona sound-by  then
    begin
        circle button-pressed? if
            stalker-in? if  pulled-back exit  then
            fiona follow  fiona-hidden state-off  curtain-out  exit
        then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound common-bank fiona sound-by  then  pulled-back exit  then
        hidden-calm  yield
    again ;

\ ---- the stalker in while she hides: his search ----
: looks-behind ( -- )
    stalker hold  stalker wait-move  5 0 do  i door-for-him  loop
    stalker $182 180 -67.8e -110e walk-to  stalker stalker.anims:there-anim play
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    stalker follow
    search-var mansion-odds chance?  2 creature-moving? or if  ['] looks-behind start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  0 ambient-step !  0 ambient-frames !
    wall-door closed-off? if  2 show-part  1 solid-floor floor-on  then
    om2f-8-flag1 fact? if  3 show-part  then
    flame-fx 0 48.8e 10.3e -50.7e flame-at  1 $2300 reverb ;
: first-time ( -- )   \ (brought here the first time: Hewie by her, the doors shut behind, the spirits gone)
    fiona hold  hewie activate  old-mansion-2f-8 302 0 hewie-to
    $62 false open-mark  doors-redone  $62 lock  creatures-on state-off  spirits-gone  found-file-234 fact!
    camera-restart  15 fade-in  $234 give-file  stalkers-stay state-off ;
: by-the-wall ( -- )   \ (brought back by the way behind the wall: it's open)
    fiona hold  3 show-part  camera-restart  world-held state-off  om2f-8-flag1 fact!  $54 unlock  2 prepare-exit
    15 fade-in  stalkers-stay state-off  summoner-on state! ;
: back-through-hole ( -- )
    fiona hold  camera-restart  slide-sound common-bank fiona sound-by  15 screen:from-black fade wait-fade  stalkers-stay state-off ;

\ ---- a spirit put away by the lamp, the first time: the wall gives - out by the forced way ----
: wall-gives ( -- )
    stalkers-stay state!  fiona hold  $5D preload  16 frames  3 show-part  8 frames
    15 screen:to-black-2 with-music or fade  wait-fade  world-held state!  forced-exit take-exit ;
: wall-due ( -- )
    wall-seen @  om2f-8-flag1 fact? or  cam-set@ 3 4 1+ within 0= or if  exit  then
    59.1e 1.43e 3e spirit-put-away-near? if  -1 wall-seen !  panic-eased  ['] wall-gives start drop  then ;

\ ---- what she may do or say here ----
: the-lamp ( -- )  fiona wait-move  fiona 60e 0e turn-to-xz  om2f-8-flag1 fact? if  blue-msg  else  unlit-msg  then  say ;
: the-hole ( -- )
    stalkers-stay state!  fiona hold  off-map-27 preload  fiona wait-move  fiona 180 turn  fiona look-close-anim 10 play-blend
    hole-msg yes? if
        15 screen:to-black fade wait-fade  slide-sound common-bank fiona sound-by  world-held state!  through-hole take-exit
    else  stalkers-stay state-off  then ;
: the-painting ( -- )
    fiona wait-move
    looked-round @ 0= if
        -1 looked-round !  fiona 138e -58e turn-to-xz  fiona look-low-anim start-anim  40 frames  insects-msg say  fiona wait-anim  exit
    then
    fiona $64 90 129e -60.5e walk-to  1 15e -10e 0e 7e shot  fiona look-at-anim start-anim  40 frames  mother-msg say
    fiona wait-anim  shot-off ;
: the-bones ( -- )  fiona wait-move  fiona 78e -70e turn-to-xz  fiona look-low-anim start-anim  40 frames  bones-msg say  fiona wait-anim ;
: the-book ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona 62e 27e turn-to-xz  written-msg say
    15 screen:to-dim fade wait-fade  recipes-msg say  15 screen:from-dim fade wait-fade
    $43 items:item-bit? 0=  $45 items:item-bit? 0= or if
        fiona hand-to-mouth-anim start-anim  medicine-msg say  fiona wait-anim
        $43 items send item-bit  $45 items send item-bit
    then  stalkers-stay state-off ;
: water-in ( -- )   \ (the door behind her: water pouring in)
    fiona 0 to-door
    in-panic? chased? or if  fiona rattle-front-slow-anim play  fiona -1 idle  7 frames  exit  then
    fiona rattle-front-anim play  fiona -1 idle  7 frames  water-msg say ;
: take-camomile ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 48e -53e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  camomile om2f-8-camomile camomile-fx takes-up  fiona reach-back-anim play ;
: take-lavender ( -- )
    fiona wait-move  fiona 53.1e -26.2e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  lavender om2f-8-lavender lavender-fx takes-up  fiona reach-back-anim play ;
: barks-at-spot ( -- )  5.52e 0e -118.99e hewie-barks-at ;

\ ---- its sounds ----
: hums ( -- )
    sounds-on @ if
        hum-sound 5e 18e -125e keep-sound  om2f-12-flag1 fact? 0= if  hum-2-sound -120e 10e -105e keep-sound  then  exit
    then
    room-sounds-in? 0= if  exit  then
    hum-sound 5e 18e -125e room-sound  om2f-12-flag1 fact? 0= if  hum-2-sound -120e 10e -105e room-sound  then  -1 sounds-on ! ;

create cameras  $C , 0 , 0 ,   $D , 1 , 1 ,   $E , 0 , 0 ,   $F , 2 , 2 ,   $10 , 0 , 0 ,
    $11 , 3 , -1 ,   $12 , 3 , -1 ,   $13 , 4 , -1 ,   $14 , 0 , 0 ,   $15 , 5 , 3 ,   \ (area, set, path)
create entries  1 , 1 ,   2 , 2 ,   4 , -1 ,   0 , 0 ,   5 , 3 ,   \ (by exit 0..4: the camera)
create ahead  5 , 0 ,   6 , 1 ,   7 , 2 ,   8 , 3 ,   9 , 3 ,   $A , 3 ,   $B , 4 ,   \ (area, exit loaded ahead)
create looks  141.32e 0e -60.6e  20e 21e 21e look,   59.37e 0e 0.93e  20e 12e 12e look,   5.52e 0e -118.99e  15e 18e 18e look,
: shades ( -- )
    40e 20e -77e  17.3e 20e -77e  40e 0e -77e  17.3e 0e -77e  light-doorway
    17.3e 20e -42e  40e 20e -42e  17.3e 0e -42e  40e 0e -42e  light-doorway
    17e 20e -77.3e  17e 20e -100e  17e 0e -77.3e  17e 0e -100e  light-doorway
    22.3e 20e -6e  22.3e 20e -30e  22.3e 0e -6e  22.3e 0e -30e  light-doorway
    20.7e 20e -30e  20.7e 20e -6e  20.7e 0e -30e  20.7e 0e -6e  light-doorway
    22.3e 20e 30e  22.3e 20e 6e  22.3e 0e 30e  22.3e 0e 6e  light-doorway
    20.7e 20e 6e  20.7e 20e 25e  20.7e 0e 6e  20.7e 0e 25e  light-doorway ;
: each-frame ( -- )
    hums  0 exit-out  1 exit-out  wall-door closed-off? 0= if  2 exit-out  then  3 exit-out  4 exit-out
    cameras $A cameras-at  ahead 7 ways-out  wall-due
    looks 3 hewie-looks-at
    spot-noticed 30 5.52e 0e -118.99e 15e 18e notices? if  ['] barks-at-spot start drop  then
    flame-fx 48.8e 10.3e -50.7e 8e -4e flame-stirred
    room-sounds-in? if  first-ambient 45.32e 0e 23.5e ambient-sounds  then
    fiona $16 50 faces-area? if  ['] the-lamp unhurried offer  then
    fiona $17 in-area?  fiona 180 50 heading? and if  ['] the-hole unhurried offer  then
    fiona $18 in-area?  fiona 180 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    fiona $19 in-area?  fiona -30 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    fiona $1A in-area?  fiona 90 60 heading? and if  ['] the-painting unhurried offer  then
    fiona $1B in-area?  fiona -180 60 heading? and if  ['] the-bones unhurried offer  then
    fiona $1C in-area?  fiona 0 60 heading? and if  ['] the-book unhurried offer  then
    1 locked-request?  fiona 0 before-door? and if  ['] water-in any-time offer  then
    om2f-8-camomile fact? 0=  fiona 48e 8e -53e 5e 5e touches? and if  ['] take-camomile calm-only offer  then
    om2f-8-lavender fact? 0=  fiona 53.1e 12e -26.2e 5e 5e touches? and if  ['] take-lavender calm-only offer  then
    shades ;
: person-in ( i -- )
    case
        fiona-p of
            entries 5 ways-in
            om2f-8-camomile fact? 0= if  camomile-fx 48e 8e -53e flicker-at  then
            om2f-8-lavender fact? 0= if  lavender-fx 53.1e 12e -26.2e flicker-at  then
            found-file-234 fact? 0= if  0e sounds-scaled  ['] first-time start drop  then
            came-by @ forced-exit = if  fiona $B 90 55e 1e place-at  3 -1 player-camera  ['] by-the-wall start drop  then
            came-by @ through-hole = if  fiona $D7 0 5.5e -116e place-at  0 0 player-camera  ['] back-through-hole start drop  then
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-8 place!
