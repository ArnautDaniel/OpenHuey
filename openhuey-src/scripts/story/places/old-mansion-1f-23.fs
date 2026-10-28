\ story/places/old-mansion-1f-23.fs - a room of the old mansion's 1F with a hiding place (the kit's;
\ a mandragora she carries cries out as she settles; the stalker's search - he finds her by the
\ odds); the save clock; the Mansion Sketch; the Viride Medallion and the Medallion Hewie sniffs
\ out; the holes things fall through; its ambient sounds. A place (story/kit.fs). The original's
\ scripts, for checking: story/rooms-ref/old-mansion-1f-23.fs in the history (converted from the
\ game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-23
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims ;

own-reset

0 constant viride-fx   1 constant medallion-fx   1 constant shriek-sound   3 constant first-ambient
0 constant her-motions   1 constant daniella-motions   $14 constant search-var   $C constant hide-area
create holes  $9D , $8D , $2B , $90 , $8A , $C3 , $87 , $7C , $BA , $9E , $8F , $2E , $92 , $8C , $C6 , $88 ,
    $C2 , $86 , $B8 , $7A , $BE , $80 , $C0 , $84 , $B3 , $1E , $9F , $42 , $BD , $7F , $BB , $7D ,
32 constant #holes

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  0 ambient-step !  0 ambient-frames !
    got-mansion-sketch fact? 0= if  0 show-part  then
    om1f-23-viride-medallion-found fact?  om1f-23-viride-medallion fact? 0= and if  viride-fx 0.03e 1e -85.54e flicker-at  then
    om1f-23-medallion-found fact?  om1f-23-medallion fact? 0= and if  medallion-fx 109.8e -9e -94.58e flicker-at  then
    1 $2300 reverb
    #holes 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the hiding place (the kit's); the stalker's search ----
: hides ( -- )  shriek-sound hide-shriek !  her-motions $20 -90 3 105.64e -103.65e hides-in ;
: her-looker? ( -- flag )  st-kind-now @ dup daniella-kind = swap kind34-kind = or ;
: looks-in ( -- )
    stalker hold  stalker wait-move  0 door-for-him
    her-looker? if  stalker daniella-motions motions-for  then
    stalker $20 -90 113e -103.65e walk-to
    her-looker? if  stalker use-motions  1 her-back  stalker room-motion play  0 her-back
    else  stalker stalker.anims:there-anim play  then
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    search-var mansion-odds chance?  2 creature-moving? or if  ['] looks-in start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;

\ ---- what she may take here ----
: take-sketch ( -- )
    fiona wait-move  fiona 127e -125e turn-to-xz  fiona reach-out-anim play  4 frames
    0 hide-part  mansion-sketch got-mansion-sketch -1 takes-the  fiona reach-back-anim play ;
: take-viride ( -- )  viride-medallion om1f-23-viride-medallion viride-fx 0.03e -85.54e picks-up ;
: take-medallion ( -- )  medallion om1f-23-medallion medallion-fx 109.8e -94.58e picks-up ;
: viride-sniffed ( -- )  om1f-23-viride-medallion-found om1f-23-viride-medallion viride-fx  0.03e 1e -85.54e  hewie-finds ;
: medallion-sniffed ( -- )  om1f-23-medallion-found om1f-23-medallion medallion-fx  109.8e -9e -94.58e  hewie-finds ;

create cameras  7 , 0 , 0 ,   8 , 1 , 1 ,   9 , 1 , 1 ,   $A , 2 , 2 ,   \ (area, set, path)
create entries  0 , 0 ,   2 , 2 ,   0 , 0 ,   \ (by exit 0..2: the camera)
create ahead  3 , 0 ,   5 , 0 ,   4 , 1 ,   6 , 2 ,   \ (area, exit loaded ahead)
: sniffed ( -- )
    scrambling? 0= if  exit  then
    om1f-23-viride-medallion-found fact? 0=  hewie 0.03e 0e -85.54e 10e 5e touches? and if  ['] viride-sniffed start-only  then
    om1f-23-medallion-found fact? 0=  hewie 109.8e -10e -94.58e 10e 5e touches? and if  ['] medallion-sniffed start-only  then ;
: each-frame ( -- )
    2 exits-out  2 2 leaves-by  cameras 4 cameras-at  ahead 4 ways-out  sniffed
    room-sounds-in? if  first-ambient 79.45e 0e -93.49e ambient-sounds  then
    fiona $B in-area?  fiona 0 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    fiona hide-area in-area?  fiona -45 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    got-mansion-sketch fact? 0=  fiona $D in-area? and  fiona 45 50 heading? and if  ['] take-sketch calm-only offer  then
    om1f-23-viride-medallion-found fact?  om1f-23-viride-medallion fact? 0= and  fiona 0.03e 0e -85.54e 5e 5e touches? and if
        ['] take-viride calm-only offer
    then
    om1f-23-medallion-found fact?  om1f-23-medallion fact? 0= and  fiona 109.8e -10e -94.58e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then ;
: person-in ( i -- )
    case
        fiona-p of  entries 3 ways-in  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-23 place!
