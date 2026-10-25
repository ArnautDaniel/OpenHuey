\ saves.fs - saving and loading the game (F5 a quick save, F9 back to it). As the original
\ (SceneGame_Save), a save is the progress - the story, the items, the doors - and where things
\ stand: the room, Fiona's place and costume, Hewie with her or not. Loading puts the progress
\ back and plays the room again, then puts them where they were. Anything else (a scene playing,
\ a stalker mid-chase, the room's own script state) starts afresh, as on loading in the original.
\
\ A save is a Forth file (in user-dir), read back by running it: each line says how something
\ was. This actor writes the progress, then asks the others (`write-save`) for their own lines -
\ Fiona's, Hewie's - and closes it. So a save can be read, sent along, or edited by hand.
IN: saves
USING: engine game-state actors messages keys facts flag-names story.words ;

\ ---- the files ----
create path-buf 256 allot   variable path-len
: path+ ( addr len -- )   \ added to the path being made
    {: a n :}  a  path-buf path-len @ +  n move  n path-len +! ;
: in-user-dir ( name len -- addr len )   \ <user-dir>name
    0 path-len !  user-dir path+  path+  path-buf path-len @ ;
: quick-path ( -- addr len )   \ (under --test: hga-test-quick.fs, the player's own left be)
    testing? if  s" hga-test-quick.fs"  else  s" hga-quick-save.fs"  then  in-user-dir ;
\ ---- the save slots (the original's memory card saves): each its own file, and an index of
\ what each holds - where it was made (a place's name) and the time played - for the lists ----
8 constant #slots
: slot-path ( n -- addr len )   \ hga-save-<n>.fs (under --test: hga-test-<n>.fs, the player's own left be)
    {: n :}  0 path-len !  user-dir path+
    testing? if  s" hga-test-"  else  s" hga-save-"  then  path+  n n>s path+  s" .fs" path+
    path-buf path-len @ ;
: index-path ( -- addr len )  s" hga-saves.fs" in-user-dir ;
create slot-used  #slots cells allot   create slot-place  #slots cells allot
create slot-frames  #slots cells allot
: slot-was ( n place frames -- )   \ (the index's lines run this)
    {: n place frames :}  frames n cells slot-frames + !  place n cells slot-place + !  true n cells slot-used + ! ;
: slot-used? ( n -- flag )  cells slot-used + @ ;
: slot-place@ ( n -- msg )  cells slot-place + @ ;
: slot-frames@ ( n -- frames )  cells slot-frames + @ ;
: read-index ( -- )
    slot-used #slots cells 0 fill
    index-path file-exists?  testing? 0= and if  index-path included  then ;
: write-index ( -- )
    testing? if  exit  then
    index-path to-file
    ." \ hga's save slots: each one's place (a message) and time played (frames)" cr
    ." USING: saves ;" cr
    #slots 0 do  i slot-used? if  i . i slot-place@ . i slot-frames@ . ." slot-was" cr  then  loop
    end-file ;
variable save-path-n   \ (the file being written or read: -1 the quick save, -2 one named: file-path)
create file-path 256 allot   variable file-path-len
: save-path ( -- addr len )
    save-path-n @ -2 = if  file-path file-path-len @ exit  then
    save-path-n @ 0< if  quick-path  else  save-path-n @ slot-path  then ;
: path-named ( addr len -- )  255 min  dup file-path-len !  file-path swap move  -2 save-path-n ! ;
: system-path ( -- addr len )  s" hga-system.fs" in-user-dir ;   \ (the system data: the unlocks)

\ ---- the options (the system data's +0x30: kept across runs, as the unlocks): the sub screen's
\ options page tells them as they change (`options-changed`); read back, they go to it ----
6 constant #options   \ (the vibration, the sound out, the volume, the screen's x and y, the controller type)
create options  #options cells allot   variable options?   \ (none read yet: the sub screen's defaults)
create options-written  #options cells allot
: options-were ( vib out vol x y layout -- )   \ (the system data's line runs this)
    0 #options 1- do  i cells options + !  -1 +loop  true options? ! ;
: options>stack ( -- vib out vol x y layout )  #options 0 do  i cells options + @  loop ;

\ ---- what a save read back says: its lines run these ----
variable was-room
variable was-costume   fvariable was-x  fvariable was-y  fvariable was-z  fvariable was-yaw
variable was-hewie     fvariable hw-x   fvariable hw-y   fvariable hw-z   fvariable hw-yaw
variable was-trust  variable was-points  variable was-hp
variable hw-room  variable hw-tri  variable hw-side  create hw-mood 7 cells allot   \ (a save without them: her room, calm)
create was-skills  6 cells allot   \ (a save without them: 16 each, as he starts)
create was-met  4 cells allot      \ (a save without them: none)
variable was-stalker   \ his kind (-1: none loaded)
variable sw-in  variable sw-room  variable sw-tri  variable sw-hp  variable sw-mode
create sw-more 8 cells allot   variable sw-more?   \ (a save without them: as he was)
create sm-was 7 cells allot   variable sm-was?        \ the summoner's record (progress +0x764)
variable was-map-page                                \ the map's page she was on (the save's +0x18F0; -1 none)

: progress-clear ( -- )  progress progress-size 0 fill ;
: progress-word! ( v i -- )  4 * progress + l! ;
: fact ( "name" -- )   \ a story fact set (a save's line)
    parse-name 2dup fact-named dup 0< if  drop ." save: no fact named " type cr exit  then  nip nip story-flag-set ;
: fact-number ( n -- )  story-flag-set ;   \ (one not named yet)
: in-room ( room -- )
    dup was-room !  hw-room !  -1 hw-tri !  2 hw-side !  hw-mood 7 cells 0 fill
    false was-hewie !  -1 was-stalker !  6 0 do  16 i cells was-skills + !  loop  was-met 4 cells 0 fill
    false sm-was? !  -1 was-map-page ! ;
: fiona-was ( costume -- ) ( F: x y z yaw -- )  was-yaw f!  was-z f!  was-y f!  was-x f!  was-costume ! ;
: hewie-was ( trust points hp -- ) ( F: x y z yaw -- )
    hw-yaw f!  hw-z f!  hw-y f!  hw-x f!  was-hp !  was-points !  was-trust !  true was-hewie ! ;

: hewie-skills ( s0 s1 s2 s3 s4 s5 -- )  0 5 do  i cells was-skills + !  -1 +loop ;
: hewie-met ( m0 m1 m2 m3 -- )  0 3 do  i cells was-met + !  -1 +loop ;
: fiona-camera ( set path -- )  s" fiona" actor-named -rot cam-char-setup ;   \ (her camera setup: the save's +0x1A0)
: hewie-state ( room tri side mood time hide f0 f1 f2 f3 -- )
    0 6 do  i cells hw-mood + !  -1 +loop  hw-side !  hw-tri !  hw-room ! ;

: stalker-was ( kind in room tri hp mode -- )  sw-mode !  sw-hp !  sw-tri !  sw-room !  sw-in !  was-stalker !  false sw-more? ! ;
: stalker-more ( cond hp-max rage temper hits mode-t down-t yaw -- )  0 7 do  i cells sw-more + !  -1 +loop  true sw-more? ! ;
: summoner-was ( away called cooldown room waited mode kind -- )  0 6 do  i cells sm-was + !  -1 +loop  true sm-was? ! ;
: map-page-was ( page -- )  was-map-page ! ;

\ ---- writing ----
: .f ( F: r -- )  6 f>s$ type space ;   \ (always with a point: read back as a float)
: .at ( F: x y z -- )  {: F: x y z :}  x .f  y .f  z .f ;   \ a place, as x y z
\ the doors' held bits aren't kept: nobody holds a door in a save (Progress_CopyState clears
\ bit 0 of the 400 door words)
: story-word? ( i -- flag )  progress pr.story progress - 4 /  {: i first :}  i first  first #story-flags 32 / +  within ;
: door-words ( -- first n )  progress pr.doors progress - 4 /  400 ;
: door-word? ( i -- flag )  door-words over + within ;
: unheld ( v i -- v' )  door-word? if  doors:held-bit invert and  then ;
: doors-unheld ( -- )  door-words 0 do  dup i + 4 * progress +  dup l@ doors:held-bit invert and  swap l!  loop  drop ;
: write-progress ( -- )   \ (all of it but the story's flags: those are facts, by name)
    ." progress-clear" cr
    progress-size 4 / 0 do
        i story-word? 0= if  i 4 * progress + l@ i unheld ?dup if  . i . ." progress-word!" cr  then  then
    loop ;
: write-facts ( -- )   \ the story's facts set: `fact hewie-freed` (one not named yet: its number)
    #story-flags 0 do
        i story-flag? if  i fact-name dup if  ." fact " type cr  else  2drop  i . ." fact-number" cr  then  then
    loop ;

\ ---- loading: the progress back, the room played, then everyone put where they were ----
: place-msg ( F: x y z yaw -- ) ( -- x y z yaw )  {: F: x y z yaw :}  x f>cell y f>cell z f>cell yaw f>cell ;
: summoner-back ( -- )   \ (its record, as the save's progress keeps it; a save without it: its time away rolled anew)
    sm-was? @ 0= if  summoner-timing state-flag-clear exit  then
    7 0 do  i cells sm-was + @  loop  s" summoner" actor-named send summoner-restore ;
\ he is in the game, out of her room, and down or held off: the summoner takes him (SceneGame_EnterRoom)
: takes-him? ( -- flag )
    was-stalker @ 0< sw-in @ 0= or  sw-room @ was-room @ = or if  false exit  then
    sw-mode @ held-off =  sw-more? @ sw-more @ down-cond = and  or ;
: put-back ( -- )
    was-map-page @ 0< 0= if  was-map-page @ s" subscreen" actor-named send map-turn-to  then   \ (SceneGame_RoomIn: Map_TurnTo)
    was-costume @ fiona-id send costume
    was-x f@ was-y f@ was-z f@ was-yaw f@ place-msg fiona-id send place
    was-hewie @ if
        hewie-id send join-fiona
        hw-room @ was-room @ <> hw-tri @ 0< 0= and if   \ (in another room: put there - Hewie_LoadState)
            hw-room @ hw-tri @ hw-side @ hewie-id send hewie-place
        else  hw-x f@ hw-y f@ hw-z f@ hw-yaw f@ place-msg hewie-id send place  then
        7 0 do  i cells hw-mood + @  loop  hewie-id send hewie-mood-restore
        was-trust @ was-points @ was-hp @ hewie-id send hewie-restore
        6 0 do  i cells was-skills + @  loop  hewie-id send hewie-skills
        4 0 do  i cells was-met + @  loop  hewie-id send hewie-met
    else  hewie-id send part-from-fiona  then
    was-stalker @ 0< if  slot-unload summoner-back exit  then   \ (the stalker's slot: as it was)
    was-stalker @ partner-load   \ (the summoner afresh, then as it was)
    summoner-back
    sw-in @ if  sw-room @ sw-tri @  was-stalker @ stalker-actor send stalker-in  then
    sw-hp @ sw-mode @  was-stalker @ stalker-actor send stalker-restore
    sw-more? @ if  8 0 do  i cells sw-more + @  loop  was-stalker @ stalker-actor send stalker-restore-more  then ;

enum: saves-step  not-saving writing entering settling taking ;

state: saves-state
  cell field step
  cell field wait-t     \ frames left
  cell field sender-asked   \ who asked for the save (told `game-saved` once it is written; -1 none)
end-state

: write-save-file ( -- )
    save-path to-file
    ." \ hga save - progress, then where things stood" cr
    ." USING: saves ;" cr
    write-progress  write-facts
    room-id . ." in-room" cr
    broadcast write-save  writing step ! ;   \ (the file kept open: the others add their lines)
: saved ( -- )  end-file  ." saved " save-path type cr  not-saving step !  sender-asked @ dup 0< if  drop  else  send game-saved  then ;
: quick-save ( -- )  -1 save-path-n !  -1 sender-asked !  write-save-file ;
: save-slot ( n place -- )   \ (a save point's: into slot n, made at that place)
    over save-path-n !  progress pr.play-frames l@  slot-was  write-index  write-save-file ;
\ the resident flags are the system data's (the unlocks), not the save's: a load leaves them be
create unlocks resident-flags 8 / allot
\ ---- the system data (the original's memory card system file): the unlocks, kept across runs -
\ written whenever they change, read back as the game starts ----
create written resident-flags 8 / allot   \ (the unlocks as last written)
create records-written 36 allot          \ (and the mini-game's best times)
: resident-word! ( v i -- )  4 * progress pr.resident + l! ;
: record! ( h m s challenge i -- )  record {: a :}  a 2 + c!  a 1+ c!  a c! ;
: system-lines ( -- )   \ (what the system data says, printed)
    ." \ hga system data - the unlocks (the resident flags), the best times, the options - kept across games" cr
    ." USING: saves ;" cr
    resident-flags 32 / 0 do
        i 4 * progress pr.resident + l@ ?dup if  . i . ." resident-word!" cr  then
    loop
    4 0 do  3 0 do  j i record  dup c@ .  dup 1+ c@ .  2 + c@ .  j . i . ." record!" cr  loop  loop
    options? @ if  #options 0 do  i cells options + @ .  loop  ." options-were" cr  then ;
: system-written ( -- )   \ (as last written)
    progress pr.resident written resident-flags 8 / move  run-records records-written 36 move
    options options-written #options cells move ;
: write-system ( -- )
    testing? 0= if  system-path to-file  system-lines  end-file  then   \ (a test's aren't kept)
    system-written ;
: unlocks-changed? ( -- flag )
    progress pr.resident resident-flags 8 / written over compare 0<>
    run-records 36 records-written 36 compare 0<> or
    options #options cells options-written over compare 0<> or ;
: options-out ( -- )   \ (to the sub screen's options page)
    options? @ 0= if  exit  then  options>stack  s" subscreen" actor-named send options-restore ;
: read-system ( -- )
    system-path file-exists?  testing? 0= and if  system-path included  then
    system-written  options-out ;
: keep-unlocks ( -- )  progress pr.resident unlocks resident-flags 8 / move ;
: unlocks-back ( -- )  unlocks progress pr.resident resident-flags 8 / move ;
: load-file ( -- )
    save-path file-exists? 0= if  ." no save at " save-path type cr exit  then
    keep-unlocks  save-path included  unlocks-back  0 hewie-played !   \ (SceneGame_ctor: Fiona played)
    doors-unheld   \ (a save made before they were left out)
    subscreen-wanted state-flag-clear   \ (a save point's save was made with the save screen up: not again)
    new-game-sounds state-flag? 1 and sound-set!   \ (SceneGame_StateEntry: bank 4's set by the flag - bare feet or boots)
    broadcast game-loaded
    was-room @ -1 s" rooms" actor-named send go-to-room  entering step ! ;
: quick-load ( -- )  -1 save-path-n !  load-file ;
: load-slot ( n -- )  save-path-n !  load-file ;

behaviour saving
  on spawned ( -- )  not-saving step !  -1 sender-asked !  self subscribe tick  self subscribe held-tick  self subscribe entered-room  read-system  read-index ;
  on held-tick ( -- )   \ (the world held - the ending's results, a menu: the unlocks kept all the same; a save point's save written)
      unlocks-changed? if  write-system  then
      step @ writing = if  saved  then ;
  on save-to-slot ( n place -- )  sender sender-asked !  save-slot ;
  on load-from-slot ( n -- )  load-slot ;
  on save-to-file ( addr len -- )  path-named  sender sender-asked !  write-save-file ;
  on load-from-file ( addr len -- )  path-named  load-file ;
  on options-changed ( vib out vol x y layout -- )  options-were ;   \ (written with the unlocks, below)
  on tick ( -- )
      unlocks-changed? if  write-system  then
      step @ case
          not-saving of
              key: F5 key-pressed? if  quick-save exit  then
              key: F9 key-pressed? if  quick-load  then  endof
          writing of  saved  endof   \ (a tick later: their lines are in)
          settling of  -1 wait-t +!  wait-t @ 0> 0= if
                  put-back  ." loaded " save-path type cr  not-saving step !
                  takes-him? if  2 wait-t !  taking step !  then   \ (once he has told the summoner he is in)
              then  endof
          taking of  -1 wait-t +!  wait-t @ 0> 0= if  0 s" summoner" actor-named send summoner-take  not-saving step !  then  endof
      endcase ;
  on entered-room ( room exit -- )   \ (a couple of frames for the room's own entry, then ours)
      2drop  step @ entering = if  2 wait-t !  settling step !  then ;
end-behaviour

: saves-spawn ( -- id )  saving saves-state s" saves" spawn ;
