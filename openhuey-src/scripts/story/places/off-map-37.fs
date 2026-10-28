\ story/places/off-map-37.fs - the extras room ($37, on no map page): as she comes in - Hewie at her
\ command and not to attack, the unlocks that follow from others marked, its two doors locked, the
\ fan turning; in from the title (exit 0) - placed, the "Secret Room" document the first time, the
\ costumes earned since given; back from a replay ($80) - at the theatre, its menu again; back from
\ the mini-game's run ($81) - at the door, its unlocks; the title's MINI-GAME - its menu at once. Its
\ corners, each named on the floor as she faces it (a label up while she's free): the theatre (the
\ movies: a replay - its room loaded, out by the forced way; the film - played, then back at it),
\ the mini-game (its menu: the challenge's stalker and the story as it needs, her costume and his
\ coat back on, out to the run), the changing area (the costumes' menu), the galleries (the models,
\ the music, the art, the play types), the exit (out to the title); its cameras. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/off-map-37.fs (converted
\ from the game's, not loaded).
IN: story.places.off-map-37
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    items.names story.kit fiona.anims scenes ;

own-reset
own: label   \ the corner's label up (its message; -1 none)

\ ---- the corners' labels ----
0 constant movie-label   1 constant music-label   2 constant character-label   3 constant play-type-label
$14 constant art-label   $15 constant mini-game-label   $16 constant exit-label   $40 constant changing-label
$40AC constant secret-room-msg   \ "'Secret Room' Various extras, game info, and a mini-game are contained within...."

\ ---- the room's things ----
$32 constant room-track   16 constant wrong-sound   $10C constant mini-game-door   $10D constant out-door
0 constant film-name   5 constant film-class   0 constant fan-part   $41 constant the-film
8 constant movie-menu   $A constant mini-game-menu   $B constant model-menu   $D constant music-menu
$E constant art-menu   $F constant play-type-menu
$2B constant replay-page-var   $2C constant replaying-var   $2E constant challenge-var
$FF constant none-chosen   $81 constant run-way   $10 constant secret-room-seen
: no-track! ( -- )  no-track 1e background ;
: room-track! ( -- )  room-track 1e background ;

\ ---- as she comes in ----
: fan-turning ( -- )  begin  s" fan" 0.0200713e object-turn  yield  again ;   \ (1.15 degrees a frame)
: unlocks-follow ( -- )   \ (the unlocks that follow from others: the original's resident flags)
    1 resident-flag? 2 resident-flag? and 3 resident-flag? and 4 resident-flag? and if  $15 resident-flag-set  then
    1 resident-flag? 2 resident-flag? or if  $55 resident-flag-set  then
    $57 resident-flag? $58 resident-flag? or if  $59 resident-flag-set  then ;
defer the-mini-game
: came-in ( exit -- )
    drop  -1 label !  room-sounds  hewie-commandable state!  hewie-no-attack state!  unlocks-follow
    mini-game-door lock  out-door lock  1e sounds-scaled  ['] fan-turning start drop  fan-part show-part
    mini-game-wanted @ if  0 mini-game-wanted !  ['] the-mini-game fiona start-holding drop  then ;   \ (hga's title MINI-GAME: as at door 0's offer)

\ ---- the costumes earned (the unlocks' items: given once, with their sound - after, kept) ----
: earned ( mark item ok? -- )   \ (not yet marked: ok - given, marked; marked: kept as it is)
    {: m it ok :}  m resident-flag? if  it 0 items send add-file exit  then
    ok if  it give-file  m resident-flag-set  then ;
: res ( n -- flag )  resident-flag? ;
: earned-later ( -- )   \ (the two that come of the others)
    $5C $260  $52 res $53 res and  earned
    $5D $261  $11 res $12 res and $13 res and $14 res and $5A res and $5B res and $5C res and  earned ;
: all-earned ( -- )
    $11 $25A  $56 res 1 res and  earned
    $12 $25B  8 res $C res or $56 res and 1 res and 3 res and $59 res and  earned
    $13 $25C  1 res  earned
    $14 $25D  $15 res  earned
    $5A $25E  8 res $C res or $56 res and 1 res and 3 res and  earned
    $5B $25F  true  $10 5 do  i res and  loop  earned
    earned-later ;
create replay-rooms   \ (the theatre's movie: the room that replays it - act03)
    $0 , $21 ,  $1 , $21 ,  $2 , $20 ,  $3 , $23 ,  $4 , $24 ,  $5 , $24 ,  $6 , $9 ,  $7 , $25 ,
    $8 , $9 ,  $9 , $9 ,  $A , $12 ,  $B , $F ,  $C , $31 ,  $D , $2A ,  $E , $13 ,  $F , $21 ,
    $10 , $21 ,  $11 , $0 ,  $12 , $0 ,  $13 , $12 ,  $14 , $F ,  $15 , $15 ,  $16 , $18 ,
    $17 , $19 ,  $18 , $19 ,  $19 , $19 ,  $1A , $8 ,  $1B , $8 ,  $1C , $1C ,  $1D , $29 ,
    $1E , $1E ,  $1F , $2 ,  $20 , $3 ,  $21 , $17 ,  $22 , $23 ,  $23 , $4 ,  $24 , $A ,
    $25 , $C ,  $26 , $C ,  $27 , $C ,  $28 , $C ,  $29 , $C ,  $2B , $6 ,  $2C , $11 ,  $2D , $32 ,
    $2A , $29 ,  $31 , $49 ,  $2E , $2E ,  $2F , $55 ,  $30 , $5D ,  $32 , $4F ,  $33 , $4F ,
    $34 , $51 ,  $35 , $51 ,  $36 , $51 ,  $37 , $51 ,  $38 , $50 ,  $39 , $4C ,  $3A , $4C ,
    $3B , $4C ,  $45 , $63 ,  $46 , $63 ,  $3C , $48 ,  $3D , $52 ,  $3E , $48 ,  $3F , $48 ,
    $40 , $66 ,  $42 , $53 ,  $43 , $59 ,  $44 , $59 ,  $47 , $60 ,  $48 , $60 ,  $49 , $47 ,
    $4A , $59 ,  $4B , $59 ,  $4C , $4A ,  $4D , $40 ,  $4E , $40 ,  $4F , $106 ,  $50 , $108 ,
    $51 , $10B ,  $52 , $59 ,  $53 , $C0 ,  $54 , $C0 ,  $55 , $C0 ,  $56 , $C5 ,  $57 , $C7 ,
    $58 , $C7 ,  $59 , $C7 ,  $5A , $C7 ,  $5B , $83 ,  $5C , $81 ,  $5D , $8A ,  $5E , $8C ,
    $5F , $8C ,  $60 , $8C ,  $61 , $8E ,  $62 , $85 ,  $63 , $84 ,  $64 , $8F ,  $65 , $92 ,
    $66 , $92 ,  $67 , $92 ,  $68 , $8F ,  $69 , $8D ,  $6A , $35 ,  $6B , $35 ,  $6C , $35 ,
    $6D , $34 ,
109 constant #replay-rooms
: replay-room ( -- room | -1 )
    replay-var var@  #replay-rooms 0 do  i 2* cells replay-rooms + @ over = if  drop  i 2* 1+ cells replay-rooms + @  unloop exit  then  loop  drop -1 ;

\ ---- the theatre: a movie replayed in its room, or the film here; then its menu again ----
: at-the-theatre ( -- )   \ (her by it, the camera; Hewie by her)
    fiona $11B 180 -34e 60e place-at  2 2 player-camera
    off-map-37 267 0 hewie-to  hewie $10B 0 place-on-tri  hewie 1 1 camera-set  yield  camera-restart ;
: the-theatre ( -- )
    stalkers-stay state!
    begin
        fiona wait-move
        cam-set@ 0= if  fiona $115 0 -34e 27e walk-to  1 20e 10e -20e 0e shot
        else  fiona $11B 180 -34e 60e walk-to  1 40e 10e 160e 0e shot  then
        30 frames  no-track!  movie-menu open-subscreen
        replay-var var@ none-chosen = if   \ (none chosen)
            0 replay-page-var var!  room-track!  15 screen:from-black with-music or fade  stalkers-stay state-off  exit
        then
        replay-var var@ the-film <> if   \ (a replay: its room loaded, out by the forced way - the theatre held)
            replay-room dup 0< if  drop  else  preload  then  0 replaying-var var!
            forced-exit take-exit  world-held state!  summoner-on state-off  events-held state!  stalkers-stay state-off  exit
        then
        hewie true hush  film-name film-class film  world-held state!  at-the-theatre  hewie false hush
    again ;

\ ---- the mini-game: its menu; a challenge chosen - its stalker, the story as it needs (the doors
\ and scenes by the story's flags), her costume and his coat back on, out to the run ----
create challenges   \ (each challenge's first room, loaded ahead; its stalker kind; then the story's flags it
                    \ needs set - golem-done always: over-music-b, x-37-flag1, over-music-c, cf-13-flag1, x-37-flag2, over-music-d, over-music-e)
    off-map-d9 ,  debilitas-7-kind ,  0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    off-map-e0 ,  kind34-kind ,       -1 , -1 , 0 , 0 , 0 , 0 , 0 ,
    off-map-d9 ,  kind12-kind ,       -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
    off-map-e0 ,  tintstalker-kind ,  -1 , -1 , -1 , -1 , -1 , 0 , 0 ,
: flag-as ( on fact -- )  swap if  fact!  else  fact-off  then ;
: slot@ ( row k -- v )  1+ cells + @ ;   \ (a challenge's kind (0) and flags (1..7))
: challenge ( n -- )   \ (its room ahead, its stalker in, the story's flags as it needs)
    9 * cells challenges + {: r :}
    r @ preload  r 0 slot@ stalker-ready  golem-done fact!
    r 1 slot@ over-music-b flag-as  r 2 slot@ x-37-flag1 flag-as  r 3 slot@ over-music-c flag-as
    r 4 slot@ cf-13-flag1 flag-as  r 5 slot@ x-37-flag2 flag-as  r 6 slot@ over-music-d flag-as
    r 7 slot@ over-music-e flag-as ;
: to-the-door ( -- )  hewie hold  hewie full-health  0 hewie send set-cond  hewie wait-move  hewie $76 180 -16e -82e walk-to  begin  yield  again ;   \ (Hewie's: by it, held)
: mini-game ( -- )
    none-chosen challenge-var var!  fiona wait-move  no-track!  mini-game-menu open-subscreen
    challenge-var var@ dup 4 < if  challenge  else  drop  then
    15 screen:from-black fade  wait-fade
    challenge-var var@ none-chosen = if  room-track! exit  then
    mini-game-door unlock  fiona 0 open-door  ['] to-the-door hewie start-holding drop
    fiona $76 180 -16e -82e walk-to  stalker-present  no-track!
    15 screen:to-black fade  wait-fade  fiona send calm-reset  panic-over
    ['] to-the-door stop-doing  world-held state!
    costume-var var@ dress  steps-in  coat-var var@ coat  yield
    hewie let-go  hewie-no-attack state-off  stalkers-blind state-off  forced-exit take-exit ;
' mini-game is the-mini-game

\ ---- the changing area: the costumes' menu ----
: changing ( -- )
    stalkers-stay state!  fiona wait-move  fiona 180 turn  1 20e 10e 0e 0e shot  30 frames
    no-track!  costume-menu open-subscreen  shot-off  wrong-sound clothes-changed
    room-track!  15 screen:from-black with-music or fade  wait-fade  stalkers-stay state-off ;

\ ---- the galleries: at the corner, its menu ----
: gallery ( menu -- )   \ (looked at closely, the menu, the room's track again)
    1 20e 10e 0e 0e shot  30 frames  no-track!  open-subscreen  room-track! ;
: models ( -- )  stalkers-stay state!  fiona wait-move  fiona 180 turn  model-menu gallery  stalkers-stay state-off ;
: corner ( menu tri deg F: x z -- )   \ (walked to, the menu, a moment)
    {: m tri deg F: x z :}  stalkers-stay state!  fiona wait-move  fiona tri deg x z walk-to
    m gallery  8 frames  stalkers-stay state-off ;
: music-gallery ( -- )  music-menu $121 180 0e 10.5e corner ;
: art-gallery ( -- )  art-menu 9 -90 -41e -38e corner ;
: play-types ( -- )  play-type-menu $D7 90 45e -39e corner ;

\ ---- the exit: out to the title ----
: out ( -- )
    fiona wait-move  out-door unlock  fiona 1 open-door
    15 screen:to-black fade  no-track!  fiona $7C 90 17.142e 22.877e walk-to  wait-fade
    world-held state!  30 frames  begin  quit-wanted state!  yield  again ;

\ ---- the ways in: from the title, back from a replay, back from the run ----
: from-the-title ( -- )
    fiona wait-move  0 replay-var var!  0 replay-page-var var!
    fiona $15 0 place-on-tri  1 1 player-camera
    off-map-37 267 0 hewie-to  hewie $10B 0 place-on-tri  hewie 1 1 camera-set  yield  camera-restart
    in-play state-off  15 screen:from-black fade  wait-fade
    secret-room-seen resident-flag? if  doc-secret-room 1 give-item
    else
        15 screen:to-dim fade wait-fade  secret-room-msg say  15 screen:from-dim fade wait-fade
        secret-room-seen resident-flag-set  doc-secret-room filed
    then
    all-earned ;
: from-a-replay ( -- )  fiona wait-move  at-the-theatre  the-theatre ;
: from-the-run ( -- )
    4 figure-out  5 figure-out  fiona wait-move  panic-over  fiona send calm-reset
    fiona $13 0 -16e -66.5e place-at  fiona stand-anim 0 start-blend  1 1 player-camera  -1 music send music-stage
    hewie $C4 0 -10.03e -61.53e place-at  hewie 1 1 camera-set  yield  camera-restart
    15 fade-in  earned-later ;

\ ---- every frame: the corners - each labelled while she faces it, free ----
: label-down ( -- )  label @ dup 0< if  drop exit  then  window send close-text  -1 label ! ;
: labelled ( on? msg -- )   \ (its label up while on and she is free - a name on the floor, not a message
    {: on m :}  on  fiona held? 0= and if  label @ m <> if  m window send show-text  m label !  then  exit  then   \ she waits on)
    label @ m = if  label-down  then ;
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;
: corner? ( flag msg xt -- )   \ (labelled; while she faces it, offered)
    {: on m xt :}  on m labelled  on if  xt any-time offer  then ;
: corners ( -- )
    $C 90 60 facing?  play-type-label ['] play-types corner?
    6 -90 60 facing?  art-label ['] art-gallery corner?
    $15 180 50 facing?  changing-label ['] changing corner?
    4 180 60 facing?  music-label ['] music-gallery corner?
    fiona $1B 45 faces-area?  movie-label ['] the-theatre corner?
    fiona 0 before-door?  mini-game-label ['] mini-game corner?
    fiona 1 before-door?  exit-label ['] out corner?
    5 180 60 facing?  character-label ['] models corner? ;
create cameras  $17 , 0 , 0 ,  $18 , 1 , 1 ,  $19 , 0 , 0 ,  $1A , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )  cameras 4 cameras-at  corners ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    came-by @ case
        0 of  room-track!  world-held state!  0 replay-page-var var!  panic-eased  ['] from-the-title fiona start-holding drop  endof
        forced-exit of  world-held state!  panic-eased  ['] from-a-replay fiona start-holding drop  endof
        run-way of  room-track!  world-held state!  panic-eased  ['] from-the-run fiona start-holding drop  endof
    endcase ;
: went ( -- )  label-down ;

' came-in ' person-in ' each-frame ' went off-map-37 place!
