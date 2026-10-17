\ hewie.fs - Hewie, Fiona's dog (docs/subsystems/hewie.md). Phase H1: his body and model, his
\ own mind (the danger's moods, what he does next by his trust), keeping with Fiona, roaming,
\ sniffing, tricks, barking, his upkeep, his sounds and steps, and following her from room to
\ room (off screen by the doors).
\
\ He is spawned with the game but comes in only when told (`join-fiona`: the story, or the
\ console); `part-from-fiona` sends him off.
IN: hewie
USING: engine actors messages common paths vectors saves hewie-names hewie.state hewie.body hewie.anims hewie.model hewie.moving hewie.states hewie.offscreen hewie.fight hewie.actions hewie.commands hewie.mind hewie.scripted hewie.fetch floor-flags ;

\ at heel: behind her and a little to her side (where she is when that isn't floor), her way
create heel-at /vec allot
: heel ( -- )
    her body-yaw {: F: a :}  fiona-at vec@ {: F: x y z :}
    x  a fsin -14e f*  a fcos 8e f* f+  f+   y   z  a fcos -14e f*  a fsin -8e f* f+  f+  heel-at vec!
    heel-at hewie-floor v-tri-in dup 0< if  drop  heel-at fiona-at vec-copy  her body-tri  then  {: tri :}
    room-id tri heel-at vec@ body-place  a his-yaw! ;
: along? ( -- flag )  self body? ;   \ in the game (his body in the house)
: join ( -- )
    her active? 0= if  -1 his-joining !  exit  then   \ (she isn't in yet: when she arrives)
    0 his-joining !
    1 him-model act.visible l!  heel  0 away!  fresh ;
: part ( -- )
    body-off  0 him-model act.visible l!  model 0 dog-legs ;

\ his coat (the story's 0xB7: DogModel / DogModelA / DogModelB) - kinds 0..2 are HEW_000..002, all
\ with HEW_000's textures; his place, his showing and who he is to the scenes kept
: coat-file ( kind -- addr len )
    case  1 of  s" O_HEW/HEW_001"  endof  2 of  s" O_HEW/HEW_002"  endof  >r s" O_HEW/HEW_000" r>  endcase ;
: dress ( kind -- )
    him-model act.visible l@ >r
    model actor-free
    coat-file s" O_HEW/HEW_000" actor-load-textured  dup his-model !  $3D5F90 motion-table
    r> him-model act.visible l!
    along? if  1 model cast-as  then ;

: place-on ( room tri -- )   \ (Hewie_PlaceInRoom: triangle -1 is that room, on none)
    dup 0< if  0e 0e 0e  else  dup tri-center  then  body-place ;
behaviour hewie-own
  on spawned ( -- )
      6 0 do  16 i cells his-skill + !  loop   \ (Hewie_LoadState: his skills at fetching, middling)
      s" O_HEW/HEW_000" actor-load dup his-model !  $3D5F90 motion-table   \ (his motion table: fades and flags)
      0 him-model act.visible l!
      2.5e 5e body-size  hewie-floor body-mask!  body-off
      s" fiona" actor-named his-fiona !
      self subscribe tick  self subscribe leaving-room  self subscribe arrived  self subscribe write-save
      self subscribe danger  self subscribe panic  self subscribe fiona-doing  self subscribe frame-end
      self subscribe stalker-doing  self subscribe creature-doing  self subscribe scene  -1 st-id !  100 st-age !  -1 his-grip !
      #creatures 0 do  -1 i cells cr-ids + !  100 i cells cr-ages + !  loop ;
  on join-fiona ( -- )  s" fiona" actor-named his-fiona !  room-id fiona-room !  join  1 his-model @ cast-as ;
  on part-from-fiona ( -- )  part  1 -1 cast-as ;
  on hewie-costume ( kind -- )  dress ;
  on hewie-yelp ( time anim -- )  his-yelp-anim !  dup 0< if  drop  else  his-yelp !  then ;   \ (the story's 0x77: Hewie_SetAnim)
  on hewie-play ( anim -- )  model swap 0 0 motion-play ;                                      \ (the story's 0xD7: Motion_Play)
  on hewie-hold ( on -- )  5 his-wait !  0<> his-stay ! ;                                      \ (the story's 0xBB)
  on write-save ( -- )   \ (saves.fs) with her: how he trusts her, his health, his place
      along? 0= if  exit  then
      his-trust @ .  his-trust-points @ .  his-hp @ .  self body-pos .at  self body-yaw .f  ." hewie-was" cr
      6 0 do  i cells his-skill + @ .  loop  ." hewie-skills" cr
      \ (Hewie_SaveState: his room and the half of it he is on, his mood and its time, staying put,
      \ his feelings about each stalker)
      self body-room .  self body-tri .  his-way @ .  his-mood @ .  his-mood-time @ .  his-hide @ .
      4 0 do  i cells his-feel + @ .  loop  ." hewie-state" cr ;
  on hewie-mood-restore ( mood time hide f0 f1 f2 f3 -- )
      3 cells his-feel + !  2 cells his-feel + !  1 cells his-feel + !  his-feel !
      his-hide !  his-mood-time !  his-mood ! ;
  on hewie-restore ( trust points hp -- )  his-hp !  his-trust-points !  his-trust ! ;
  on full-health ( -- )  100 hp! ;                       \ (the story's 0x48)
  on heal ( n -- )  abs his-hp @ + 100 min hp! ;          \ (0xB8)
  on set-hp ( n -- )  0 max 100 min hp! ;                 \ (a room's command: +0x14C8 put back)
  on hewie-skills ( s0 s1 s2 s3 s4 s5 -- )  0 5 do  i cells his-skill + !  -1 +loop ;
  on place ( x y z yaw -- )   \ (the story puts him)
      along? 0= if  2drop 2drop exit  then
      >r cells>xyz  his-place  r> cell>f his-yaw! ;
  on leaving-room ( room exit -- )   \ (the old room still in)
      2dup room-exit-leads drop fiona-room !  nip  along? if  fiona-left  else  drop  then ;
  on arrived ( room exit -- )  drop fiona-room !
      his-joining @ if  s" fiona" actor-named his-fiona !  join  then ;   \ (he comes in by himself: Hewie_Arrive, the doors off screen)
  \ a scene starting (the scripts' 0x61: Hewie_LeftBehind) - out of the room after an attack
  \ (0x38): his default; his mood back to normal
  on scene ( on -- )
      0= if  exit  then
      his-away @  his-action @ after-attack-act = and if  to-default  then  normal-mood -1 set-mood ;
  on danger ( level -- )  his-danger ! ;
  on panic ( stage level -- )  drop his-panic ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  drop fiona-cmd !  fiona-cond !  fiona-sub !  fiona-mode ! ;
  \ the story's (H4: EventCmd_Hewie) - his action, trust, mood, a point to look to, his place and
  \ half of the room, a wait; through blocked floor, left alone
  on do-action ( action arg -- )  along? if  set-action  else  2drop  then ;
  on trust+ ( n -- )  add-trust ;
  \ the items' baits (items.c Item60..66_Use): a jerky heals 20 (full after it: treated a little
  \ better), a big one 100; an onion hurts him 50; a calming bait angers him 450 frames; one gets
  \ him up (health 1). How each counts in his treatment (the progress' +0xFB6)
  on fed ( how -- )
      case
          small-treat of  his-hp @ 20 + 100 min hp!  his-hp @ 100 = if  -1 mistreated+  then  endof
          big-treat of  his-hp @ 100 + 100 min hp!  -1 mistreated+  endof
          onion-bait of  50 hurt-by  3 mistreated+  endof
          fury-bait of  angry-mood 450 set-mood  20 mistreated+  endof
          rouse-bait of  1 hp!  3 mistreated+  endof
      endcase ;
  on mood! ( mood -- )  along? if  -1 set-mood  else  drop  then ;
  on scent-at ( x y z -- )
      his-smells @ along? 0= or if  2drop drop exit  then
      cells>xyz  his-scent vec!  1 his-smells ! ;
  on hold-spot ( tri x y z yaw -- )   \ (Pursuer_GrabHewieBehind: where he is held - +0x110, +0x10C, +0x104[1])
      cell>f his-to-yaw f!  cells>xyz his-to vec!  his-to-tri ! ;
  on scent-now ( x y z -- )   \ (a room's command: his point to look to, whatever he had - Room26_Cmd02)
      along? 0= if  2drop drop exit  then
      cells>xyz  his-scent vec!  1 his-smells ! ;
  on hewie-place ( room tri side -- )   \ (his +0x64, Hewie_PlaceInRoom)
      his-way !  over room-id = >r  place-on  r> if  to-default  then ;
  on hewie-side! ( side -- )  dup 3 u< 0= if  drop -1  then  his-way ! ;
  on hewie-stay ( -- )  300 his-pet-time ! ;
  on noclip ( on -- )  0<> 1 and his-2b ! ;
  on aside ( on -- )  0<> 1 and his-2d ! ;
  on activate ( -- )  0 his-2b !  0 his-2d !  100 hp! ;   \ (the story's 0x15, Hewie_Activate: afresh, at full health)
  on tick ( -- )  along? if  frame  else  tell-doing  then  -1 his-heard ! ;
  \ her commands (H2)
  on command ( code tri yaw x z -- )   \ (Hewie_StateBlock's 13)
      along? 0= if  2drop 2drop drop exit  then
      cells>xyz                                                ( code tri ) ( F: yaw x z )
      over go-there-cmd = if  showed  else  drop fdrop fdrop fdrop  then
      fiona-command drop  0 his-broke !  tell-doing ;
  on reaction ( n -- )  along? if  react  else  drop  then ;
  on meet-me ( type -- )
      along? if  meet-ask  else  drop -1  then
      dup 0< if  drop  sender send meet-refused exit  then
      fs-at v.x sf@ f>cell  fs-at v.y sf@ f>cell  fs-at v.z sf@ f>cell  his-to-yaw f@ pi f+ angle-wrap f>cell   \ (facing him)
      sender send meet-at  tell-doing ;   \ (what he does now, before her next frame)
  on meet-now ( type -- )
      along? if  meet-second?  else  drop false  then
      if  sender send meet-on  tell-doing  else  sender send meet-refused  then ;
  on meet-off ( -- )  -1 his-meet ! ;
  \ the story's moves (H4: Hewie_Requests)
  on scripted ( on -- )  along? if  his-busy !  else  drop  then ;
  on scripted-move ( kind a b x y z yaw -- )
      along? 0= if  2drop 2drop 2drop drop exit  then
      cell>f his-to-yaw f!  cells>xyz his-to vec!
      his-to-anim !  his-to-tri !  0 his-done !
      dup look-move = if  drop his-to-tri @ his-look-char !  0 his-look-pt? !  done! exit  then
      dup look-point-move = if  drop his-look-pt his-to vec-copy  1 his-look-pt? !  done! exit  then
      dup stand-move = if  full-stop  then
      move-action dup 0< if  drop done! exit  then  0 want ;
  on hold-anim ( anim flags blend -- )  nip  along? 0= if  2drop exit  then  his-to-anim !  his-to-tri !  story-blend-act 0 want ;
  on show ( on -- )  along? 0= if  drop exit  then  0<> 1 and him-model act.visible l! ;
  on frame-end ( -- )
      along? if  his-done @  anim-done?  his-model @ 0 0 1 motion-events  broadcast moving  then
      st-age @ 100 < if  1 st-age +!  then
      #creatures 0 do  i cells cr-ages + dup @ 100 < if  1 swap +!  else  drop  then  loop ;
  \ the stalker (H3: hewie/fight.fs): what he is doing; his answers to the attack; thrown off;
  \ struck - by him, by her, by a door (answered `blow-taken` when it struck)
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      drop st-alert !  st-kind !  st-hp-max !  st-hp !  st-cond !  st-sub !  st-mode !  st-room !  sender st-id !  0 st-age ! ;
  on dog-grip ( tri x y z yaw -- )
      along? 0= if  2drop 2drop drop exit  then
      his-act @ ['] st-await-grip <> if  2drop 2drop drop  sender send shaken-off exit  then   \ (no longer at it)
      {: x y z yaw :}  x cell>f  y cell>f  z cell>f  yaw cell>f  leap ;   \ (tri; x y z yaw)
  on dog-refused ( -- )  along? if  his-act @ ['] st-await-grip = if  harry-stalker-act start  then  then ;
  on blow-taken ( -- )   \ (his bite struck home: on a homunculus, bite-landed)
      along? sender creature-slot 0< 0= and if  bite-landed  then ;
  on hit-taken ( kind -- )  drop ;
  on silent ( on -- )  0<> his-hushed ! ;   \ (the scripts' 0x3D)
  on creature-doing ( slot room mode up -- )   \ (each remembered; one up in his room: hostile - what-next)
      {: slot room mode up :}
      slot #creatures u< if  sender slot cells cr-ids + !  mode slot cells cr-modes + !  up slot cells cr-ups + !  0 slot cells cr-ages + !  then
      along? up and if  room self body-room = if  2 his-creature-t !  then  then ;
  on fetch-this ( thing -- )   \ (Fiona_StateLetGo: her ball thrown, while all's calm)
      along? his-danger @ calm = and his-action @ fetch-act <> and if  his-follows !  else  drop  then ;
  on shaken-off ( -- )  along? if  his-act @ ['] st-bite-hold = if  thrown-off his-biting !  then  then ;   \ (thrown off: only while he holds on)
  on blow-from ( kind damage how x y z -- )   \ (from a point: Progress_Noise)
      cells>xyz his-blow-pt vec!  along? 0= if  2drop drop exit  then  -2 take-blow drop ;
  on blow ( kind damage how -- )
      along? 0= if  2drop drop exit  then
      sender take-blow  sender her = and if  sender send blow-taken  then ;
end-behaviour

: hewie-spawn ( -- id )  hewie-own hewie-state s" hewie" spawn ;
