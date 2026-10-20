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

\ ---- what a save read back says: its lines run these ----
variable was-room
variable was-costume   fvariable was-x  fvariable was-y  fvariable was-z  fvariable was-yaw
variable was-hewie     fvariable hw-x   fvariable hw-y   fvariable hw-z   fvariable hw-yaw
variable was-trust  variable was-points  variable was-hp
variable hw-room  variable hw-tri  variable hw-side  create hw-mood 7 cells allot   \ (a save without them: her room, calm)
create was-skills  6 cells allot   \ (a save without them: 16 each, as he starts)
variable was-stalker   \ his kind (-1: none loaded)
variable st-in  variable st-room  variable st-tri  variable st-hp  variable st-mode
create st-more 8 cells allot   variable st-more?   \ (a save without them: as he was)

: progress-clear ( -- )  progress progress-size 0 fill ;
: progress-word! ( v i -- )  4 * progress + l! ;
: in-room ( room -- )
    dup was-room !  hw-room !  -1 hw-tri !  2 hw-side !  hw-mood 7 cells 0 fill
    false was-hewie !  -1 was-stalker !  6 0 do  16 i cells was-skills + !  loop ;
: fiona-was ( costume -- ) ( F: x y z yaw -- )  was-yaw f!  was-z f!  was-y f!  was-x f!  was-costume ! ;
: hewie-was ( trust points hp -- ) ( F: x y z yaw -- )
    hw-yaw f!  hw-z f!  hw-y f!  hw-x f!  was-hp !  was-points !  was-trust !  true was-hewie ! ;

: hewie-skills ( s0 s1 s2 s3 s4 s5 -- )  0 5 do  i cells was-skills + !  -1 +loop ;
: fiona-camera ( set path -- )  s" fiona" actor-named -rot cam-char-setup ;   \ (her camera setup: the save's +0x1A0)
: hewie-state ( room tri side mood time hide f0 f1 f2 f3 -- )
    0 6 do  i cells hw-mood + !  -1 +loop  hw-side !  hw-tri !  hw-room ! ;

: stalker-was ( kind in room tri hp mode -- )  st-mode !  st-hp !  st-tri !  st-room !  st-in !  was-stalker !  false st-more? ! ;
: stalker-more ( cond hp-max rage temper hits mode-t down-t yaw -- )  0 7 do  i cells st-more + !  -1 +loop  true st-more? ! ;

\ ---- writing ----
: .f ( F: r -- )  6 f>s$ type space ;   \ (always with a point: read back as a float)
: .at ( F: x y z -- )  {: F: x y z :}  x .f  y .f  z .f ;   \ a place, as x y z
: write-progress ( -- )
    ." progress-clear" cr
    progress-size 4 / 0 do
        i 4 * progress + l@ ?dup if  . i . ." progress-word!" cr  then
    loop ;

\ ---- loading: the progress back, the room played, then everyone put where they were ----
: fiona-id ( -- id )  s" fiona" actor-named ;
: hewie-id ( -- id )  s" hewie" actor-named ;
: place-msg ( F: x y z yaw -- ) ( -- x y z yaw )  {: F: x y z yaw :}  x f>cell y f>cell z f>cell yaw f>cell ;
: put-back ( -- )
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
    else  hewie-id send part-from-fiona  then
    was-stalker @ 0< if  slot-unload exit  then   \ (the stalker's slot: as it was)
    was-stalker @ partner-load
    st-in @ if  st-room @ st-tri @  was-stalker @ stalker-actor send stalker-in  then
    st-hp @ st-mode @  was-stalker @ stalker-actor send stalker-restore
    st-more? @ if  8 0 do  i cells st-more + @  loop  was-stalker @ stalker-actor send stalker-restore-more  then ;

enum: saves-step  idle writing entering settling ;

state: saves-state
  cell field step
  cell field wait     \ frames left
  cell field sender-asked   \ who asked for the save (told `game-saved` once it is written; -1 none)
end-state

: write-save-file ( -- )
    save-path to-file
    ." \ hga save - progress, then where things stood" cr
    ." USING: saves ;" cr
    write-progress
    room-id . ." in-room" cr
    broadcast write-save  writing step ! ;   \ (the file kept open: the others add their lines)
: saved ( -- )  end-file  ." saved " save-path type cr  idle step !  sender-asked @ dup 0< if  drop  else  send game-saved  then ;
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
: write-system ( -- )
    testing? if  progress pr.resident written resident-flags 8 / move  run-records records-written 36 move exit  then   \ (a test's unlocks aren't kept)
    system-path to-file
    ." \ hga system data - the unlocks (the resident flags), kept across games" cr
    ." USING: saves ;" cr
    resident-flags 32 / 0 do
        i 4 * progress pr.resident + l@ ?dup if  . i . ." resident-word!" cr  then
    loop
    4 0 do  3 0 do  j i record  dup c@ .  dup 1+ c@ .  2 + c@ .  j . i . ." record!" cr  loop  loop
    end-file
    progress pr.resident written resident-flags 8 / move  run-records records-written 36 move ;
: unlocks-changed? ( -- flag )
    progress pr.resident resident-flags 8 / written over compare 0<>
    run-records 36 records-written 36 compare 0<> or ;
: read-system ( -- )
    system-path file-exists?  testing? 0= and if  system-path included  then
    progress pr.resident written resident-flags 8 / move  run-records records-written 36 move ;
: keep-unlocks ( -- )  progress pr.resident unlocks resident-flags 8 / move ;
: unlocks-back ( -- )  unlocks progress pr.resident resident-flags 8 / move ;
: load-file ( -- )
    save-path file-exists? 0= if  ." no save at " save-path type cr exit  then
    keep-unlocks  save-path included  unlocks-back  0 hewie-played !   \ (SceneGame_ctor: Fiona played)
    subscreen-wanted state-flag-clear   \ (a save point's save was made with the save screen up: not again)
    broadcast game-loaded
    was-room @ -1 s" rooms" actor-named send go-to-room  entering step ! ;
: quick-load ( -- )  -1 save-path-n !  load-file ;
: load-slot ( n -- )  save-path-n !  load-file ;

behaviour saving
  on spawned ( -- )  idle step !  -1 sender-asked !  self subscribe tick  self subscribe held-tick  self subscribe entered-room  read-system  read-index ;
  on held-tick ( -- )   \ (the world held - the ending's results, a menu: the unlocks kept all the same; a save point's save written)
      unlocks-changed? if  write-system  then
      step @ writing = if  saved  then ;
  on save-to-slot ( n place -- )  sender sender-asked !  save-slot ;
  on load-from-slot ( n -- )  load-slot ;
  on save-to-file ( addr len -- )  path-named  -1 sender-asked !  write-save-file ;
  on load-from-file ( addr len -- )  path-named  load-file ;
  on tick ( -- )
      unlocks-changed? if  write-system  then
      step @ case
          idle of
              key: F5 key-pressed? if  quick-save exit  then
              key: F9 key-pressed? if  quick-load  then  endof
          writing of  saved  endof   \ (a tick later: their lines are in)
          settling of  -1 wait +!  wait @ 0> 0= if  put-back  ." loaded " save-path type cr  idle step !  then  endof
      endcase ;
  on entered-room ( room exit -- )   \ (a couple of frames for the room's own entry, then ours)
      2drop  step @ entering = if  2 wait !  settling step !  then ;
end-behaviour

: saves-spawn ( -- id )  saving saves-state s" saves" spawn ;
