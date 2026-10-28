\ story/places/off-map-35.fs - room $35 (on no map page), the scenes with Hewie at the end: by the
\ first forced way - "Thanks, Hewie. Come on, boy!" (the room's look for it: its tint, the blend,
\ the fog; their shadows off), on to room $E; by the second - with Hewie's file found, "Thanks,
\ Hewie. Hewie!" (Debilitas's figure, her shadow off for two shots), on to room $36; else "... Wait
\ up, boy!" and the game over of the story's end (the ending B); each replayed from the extras, on
\ to the next room's replay or back. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-35.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.off-map-35
USING: engine game-state actors messages common flag-names facts room-names story-names stalker-names
    story.kit scenes ;

0 constant hewie-msg   1 constant wait-up-msg   2 constant come-on-msg
1 constant come-on-movie   0 constant come-on-cutscene   3 constant hewie-movie   2 constant hewie-cutscene
5 constant wait-up-movie   4 constant wait-up-cutscene   $81 constant second-way
3 constant come-on-seen   1 constant hewie-seen   2 constant wait-up-seen
3 constant figure-slot   1 constant ending-b   2 constant story-over   $2C constant replaying-var
$E constant come-on-room   $36 constant hewie-room
: came-in ( exit -- )  drop ;
: no-pause-unless ( flag -- )  resident-flag? 0= if  no-pause state!  then ;
: shadows ( on -- )  {: on :}  0 on no-shadow  1 on no-shadow ;   \ (Fiona's and Hewie's off, or back)

\ ---- "Come on, boy!": the room's look for it ----
: come-on-scene ( -- )
    come-on-movie scenes:unseen-movie come-on-cutscene load-scene
    true shadows  $44 $4E $6C $64 $66 $6C $60 $46 room-tint  $B090C8BA 0 screen-blend
    $40424C74 $4244489C 8.54e 25.61e fog  wait-fade
    panic-eased  if  come-on-msg run-scene  then ;
: come-on ( -- )
    come-on-seen no-pause-unless  world-held state!  stalkers-stay state!  hewie activate
    come-on-scene  come-on-room preload  false shadows  forced-exit take-exit  stalkers-stay state-off ;

\ ---- "Hewie!": Debilitas's figure; her shadow off for two shots ----
: her-shadow ( -- )   \ (off at its shot $11, back at $12)
    begin  $11 scene-shot? 0= while  yield  repeat  0 true no-shadow
    begin  $12 scene-shot? 0= while  yield  repeat  0 false no-shadow ;
: hewie-scene ( -- )
    debilitas-kind figure-slot figure-in  yield
    hewie-movie scenes:unseen-movie hewie-cutscene load-scene  wait-fade  ['] her-shadow start drop
    panic-eased  if  hewie-msg run-scene  then
    hewie-room preload  0 false no-shadow ;
: hewie-in ( -- )
    hewie-seen no-pause-unless  world-held state!  hewie activate  hewie-scene  forced-exit take-exit ;

\ ---- "Wait up, boy!": the story's end ----
: wait-up-scene ( -- )
    wait-up-movie scenes:unseen-movie wait-up-cutscene load-scene  wait-fade
    panic-eased  if  wait-up-msg run-scene  then ;
: wait-up ( -- )
    wait-up-seen no-pause-unless  world-held state!  hewie activate  wait-up-scene
    ending-b ending-var var!  story-over game-over ;

\ ---- replayed from the extras: on to the next room's replay (the first two), or back ----
: replay-on ( -- )  forced-exit take-exit  1 replaying-var var! ;
: come-on-replayed ( -- )  fiona hold  fiona wait-move  come-on-scene  come-on-room preload  false shadows  replay-on ;
: hewie-replayed ( -- )  fiona hold  fiona wait-move  hewie-scene  replay-on ;
: wait-up-replayed ( -- )  fiona hold  fiona wait-move  wait-up-scene  replay-over ;

\ ---- every frame ----
: each-frame ( -- ) ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case
            4 of  ['] hewie-replayed  endof
            5 of  ['] wait-up-replayed  endof
            >r ['] come-on-replayed r>
        endcase  start drop exit
    then
    came-by @ forced-exit = if  ['] come-on fiona start-holding drop  then
    came-by @ second-way = if
        found-file-20b fact? if  ['] hewie-in  else  ['] wait-up  then  fiona start-holding drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went off-map-35 place!
