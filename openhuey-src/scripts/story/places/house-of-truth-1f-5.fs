\ story/places/house-of-truth-1f-5.fs - the House of Truth's 1F hall of the door ($8D): with her track on
\ and the file not yet found, the stalkers held till she steps further in - then the scene (Lorenzo's
\ end: the marker shining between its cues; the door shut behind her, the file); after it her remarks
\ ("I have to get out of here now!"), his body, and the door of mystical power (open: by the forced way
\ to off-map-35, the sounds stopped, the music changed); before it, the door welded shut, the doors
\ locked from this side, the clock; the tremors and the stones, the drone; its sounds; the slam's
\ shake; the replay. A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/
\ house-of-truth-1f-5.fs (converted from the game's, not loaded).
IN: story.places.house-of-truth-1f-5
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    effects story.kit story.places.house-of-truth fiona.anims scenes ;

own-reset  truth-owns owns !

1 constant welded-msg      \ "This door is closed so tightly you'd think it was welded shut. It almost as if it's sea..."
2 constant mystic-msg      \ "You feel a mystical power flowing all throughout the door. Open the door? YES NO"
3 constant dead-msg        \ "This time... you really are dead. You bastard."
4 constant locked-msg      \ "It's locked and won't open. How am I going to get this open?"
5 constant get-out-msg     \ "I have to get out of here now!"
own: held-back             \ the stalkers held till she steps further in (the scene to come)
own: steady-on             \ its steady sound started
own: music-changed         \ through the mystical door: its sounds stopped
own: drone-on              \ the drone going
own: shrink                \ the marker shrinking by it (from 334)
$A constant clock-area   $B constant further-area   $C constant door-area   0 constant marker-fx
0 constant steady-sound   3 constant first-ambient   $30 constant drone-sound   $47 constant drone-bank   7 constant drone-stop-bank
1 constant end-movie   0 constant end-cutscene   $1E constant end-figure   $1F constant end-figure-2
$EA constant hall-door   $257 constant lorenzo-file   $828E constant end-other-file   $51 constant end-seen
$1C constant door-track   $81 constant mystic-way
: stones-fall ( -- )   \ (the ceiling coming down: stones of three kinds falling from 60)
    7 60e 0e 0.5e 0.5e story.words:stones-falling  4 60e 0e 0e 0.5e story.words:stones-falling
    3 60e 0e 0e 1e story.words:stones-falling ;
: came-in ( exit -- )
    drop  room-sounds
    over-music-f fact?  found-file-257 fact? 0= and  dup held-back !  if  stalkers-stay state!  then
    over-music-f fact? if  0 barred-floor floor-on  0 show-part  else  1 show-part  then
    found-file-257 fact? if  2 show-part  then
    tremor-warned fact? if  5 1e background  then
    tremors  ht1f-9-flag2 fact? if  stones-fall  then
    1 $2300 reverb ;

\ ---- the scene: Lorenzo's end - the marker shining between its cues ----
: marker-to ( v -- )  marker-fx tell-effect ;   \ (-2 a pause, else its size x 1000)
: marker-done ( -- )  marker-fx dies-down ;   \ (-1: left to go out on its own)
: marker-in ( -- )   \ (made: the figure and she shining in its colour, it lit on the figure)
    end-figure $14 layer  0 $14 layer
    end-figure cast-model  0 cast-model  1 1e effects:marker  marker-fx effect! ;
: till-shot ( n -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: till-cue ( frame -- )  begin  cutscene-frame over < while  yield  repeat  drop ;
: marker-cues ( -- )   \ (an activity, along the scene)
    0 till-shot  marker-in  2 till-shot  marker-done  3 till-shot  marker-in  4 till-shot  -2 marker-to
    $1E0 till-cue  1000 marker-to  7 till-shot  marker-done  8 till-shot  marker-in  9 till-shot  marker-done
    $A till-shot  marker-in  $B till-shot  marker-done  $C till-shot  marker-in  500 marker-to
    $58C till-cue  1000 marker-to  $D till-shot  marker-done  $E till-shot  marker-in
    $65E till-cue  334 shrink !
    begin  cutscene-frame 0< 0=  while  shrink @ 1- 0 max dup shrink !  1000 334 */ marker-to  yield  repeat ;
: end-scene ( -- )
    end-movie scenes:unseen-movie end-cutscene load-scene drop  3 figure-on  4 figure-on
    0 show-part  ['] marker-cues start drop  panic-eased  -1 run-scene  ['] marker-cues stop-doing  marker-done ;
: lorenzo-ends ( -- )
    fiona hold  tremor-warned fact-off  no-track 1e background
    ['] slight-tremors stop-doing  ['] violent-tremors stop-doing
    15 screen:to-black-2 fade  fiona wait-move  wait-fade
    hewie-in? if  hewie let-go  hewie false shown  then  stalker let-go  0 summon-take
    end-figure 3 figure-in  end-figure-2 4 figure-in  force-followed state!  yield
    end-scene  3 figure-out  4 figure-out
    fiona $63 0 -0.61e 47.71e place-at  1 1 player-camera  camera-restart
    hewie true shown  hewie activate  this-room @ 123 0 hewie-to  hewie $7B -20 place-on-tri  hewie-no-attack state-off
    2 show-part  hall-door false open-mark  hall-door lock  doors-redone
    3 $80 $80 $80 0e 0e 65e dust-of
    15 screen:from-black fade  wait-fade
    end-seen resident-flag-set  lorenzo-file give-file  end-other-file give-file
    stalkers-stay state-off  found-file-257 fact!  0 held-back !  tremors
    drone-sound story-volume or drone-bank bank-sound ;
: replayed ( -- )
    fiona hold  stones-fall  end-figure 3 figure-in  end-figure-2 4 figure-in  yield
    end-scene  3 figure-out  4 figure-out  replay-over ;

\ ---- the doors ----
: mystic-door ( -- )   \ (asked: open - by the forced way, the music changed, its sounds stopped)
    stalkers-stay state!  fiona wait-move
    mystic-msg yes? if
        15 screen:to-black-2 with-music or fade  off-map-35 preload  door-track 1 1e f>cell music send bgm-want
        wait-fade  -1 music-changed !
        drone-sound drone-stop-bank stop-sound  steady-sound room-bank stop-sound
        4 0 do  first-ambient i + room-bank stop-sound  loop
        1e sounds-scaled  music send bgm-resume  mystic-way take-exit
    then  stalkers-stay state-off ;
: welded ( -- )
    fiona $CA 0 -1 walk-move 0e 96.99e go-to  4 frames
    fiona lean-anim play  fiona strain-anim start-anim  16 frames  fiona let-go-push-anim play
    fiona -1 idle  7 frames  welded-msg say ;
: tries-front ( -- )  0 true locked-msg tries-locked ;
: tries-behind ( -- )  1 false locked-msg tries-locked ;

\ ---- what she may do or say here ----
: get-out ( -- )  fiona wait-move  fiona rest-anim start-anim  get-out-msg say  fiona wait-anim ;
: his-body ( -- )  fiona wait-move  fiona look-low-anim start-anim  16 frames  dead-msg say  fiona wait-anim ;
: spots-by ( done -- )
    {: done :}
    fiona clock-area in-area?  fiona 67 50 heading? and if
        done if  ['] get-out unhurried offer  else  ['] save-clock calm-hewie-up offer  then
    then
    fiona door-area in-area?  fiona 0 50 heading? and if
        done if  ['] mystic-door unhurried offer  else  ['] welded unhurried offer  then
    then
    8 locked-request? if
        fiona 0 before-door? if  done if  ['] get-out unhurried  else  ['] tries-front any-time  then  offer  then
        fiona 1 before-door? if  done if  ['] get-out unhurried  else  ['] tries-behind any-time  then  offer  then
        fiona 2 before-door?  done and if  ['] get-out unhurried offer  then
    then
    done  fiona 0e 74.25e 8e centre-across? and if  ['] his-body unhurried offer  then ;
: spots ( -- )  found-file-257 fact? spots-by ;

\ ---- every frame ----
create cameras  8 , 0 , 0 ,  9 , 1 , 1 ,   \ (area, set, path)
create ahead  5 , 0 ,  6 , 1 ,  7 , 2 ,   \ (area, exit loaded ahead)
: steady-sounds ( -- )   \ (her track on: its steady sound - started at one spot, kept up at another, as the original has it)
    music-changed @  over-music-f fact? 0= or if  exit  then
    steady-on @ if  steady-sound -60e 20e 50e keep-sound exit  then
    room-sounds-in? if  steady-sound 0e 20e 100e room-sound  -1 steady-on !  then ;
: each-frame ( -- )
    steady-sounds
    drone-on @ if  drone-sound story-volume or keep-playing or drone-bank bank-sound  then
    3 exits-out  cameras 2 cameras-at  ahead 3 ways-out
    fiona further-area entered?  held-back @ and if
        0 held-back !  panic-eased  ['] lorenzo-ends fiona start-holding drop
    then
    slam-shake
    music-changed @ 0=  room-sounds-in? and if  first-ambient 28e 20e -71e ambient-sounds  then
    ht1f-9-flag2 fact?  drone-on @ 0= and if  drone-sound story-volume or drone-bank bank-sound  -1 drone-on !  then
    spots ;
create entries  0 , 0 ,  0 , 0 ,  0 , 0 ,   \ (by exit 0 .. 2: the camera)
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 3 ways-in ;
: went ( -- )  held-back @ if  stalkers-stay state-off  then ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-5 place!
