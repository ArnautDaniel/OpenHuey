\ story/places/house-of-truth-b1-1.fs - the House of Truth's B1 room where Lorenzo waits ($84): brought
\ here by the forced way (through B1-2's door the first time), his scene - "Daddy?" (the file; her
\ clothes and Hewie's coat as they were, the door to B1-2 open, Lorenzo let loose in the B1, Hewie
\ in the next room); its things; his slam's shake; the replay. A place (story/kit.fs). The original's
\ scripts, for checking: story/rooms-ref/house-of-truth-b1-1.fs (converted from the game's, not
\ loaded).
IN: story.places.house-of-truth-b1-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    story.kit story.places.house-of-truth scenes ;

own-reset  truth-owns owns !

0 constant daddy-msg       \ "Daddy? Ah, Fiona... My dear holder of Azoth! What's going on? You're not my......" (the scene's)
0 constant lorenzo-part   $2A constant brought-tri   9 constant scene-figure   $A constant scene-figure-2
4 constant daddy-movie   3 constant daddy-cutscene   $EF constant b1-2-door   $253 constant lorenzo-file
$4B constant lorenzo-seen
: room-set ( -- )  lorenzo-part show-part  3 0 do  i object-hide  loop ;
: came-in ( exit -- )  drop  room-set ;
: daddy-scene ( -- )
    daddy-movie scenes:unseen-movie daddy-cutscene load-scene drop  3 figure-on  panic-eased  daddy-msg run-scene ;
: lorenzo-meets ( -- )
    stalkers-stay state!  fiona hold  stalker let-go  0 summon-take  fiona wait-move
    scene-figure 3 figure-in  yield  lorenzo-part hide-part
    daddy-scene  3 figure-out  stalker let-go  0 summon-take
    fiona $25 0 -1.96e -22.63e place-at  0 0 player-camera  camera-restart
    over-music-e fact!  1 0 0 music send music-op
    b1-2-door unlock  b1-2-door true open-mark
    costume-var var@ dress  steps-in  coat-var var@ coat  yield
    partner activate  this-room @ 44 stalker-to  stalker $2C -140 place-on-tri  stalker 0 0 camera-set
    summoner-on state!  his-own-time
    hewie-in? if  hewie let-go  then  house-of-truth-b1-2 82 0 hewie-to
    room-set  15 fade-in
    lorenzo-seen resident-flag-set  lorenzo-file give-file  stalkers-stay state-off ;
: replayed ( -- )
    fiona hold  scene-figure 3 figure-in  scene-figure-2 4 figure-in  yield
    daddy-scene  3 figure-out  4 figure-out  replay-over ;
: each-frame ( -- )  0 exit-out  slam-shake ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    0 0 0 way-in
    came-by @ forced-exit = if
        fiona brought-tri 0 place-on-tri  world-held state!  0e sounds-scaled  panic-eased
        ['] lorenzo-meets fiona start-holding drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-1 place!
