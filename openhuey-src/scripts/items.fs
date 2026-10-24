\ items.fs - the inventory (docs/subsystems/items.md; src/game/items.c, story_chars.c: the item
\ manager, gSubScreen +0x8). What Fiona carries, in three lists of 64 places (ids under $40 the
\ key things, under $A0 the remedies, baits and what she wears, the rest the materials and
\ documents); what she wears (four slots: boots, earrings, a costume, a choker); the files she
\ has found; the items marked as used. It is kept in the progress, a fact anyone may read (the
\ menu, the story's conditions, Fiona's equipment); only this actor changes it.
\ Using an item (`use-item`, from the menu) follows its own rule: at its place in its room an
\ action script of the room's starts (`item-event`); a remedy calms her (`remedy`, `soothe`); a
\ bait feeds Hewie (`fed`); a sketch marks its map as held. Each frame the counters count (the
\ Fresh Flowers wither after 9000 frames) and the timed effects run down.
IN: items
USING: engine game-state actors messages common facts flag-names story-names room-names items.tables items.names ;

\ ==== the inventory (facts: anyone may read these) ====
\ a place: the item's id (-1 empty), how many (a stack: 1..99; a counter: its frames), 8 letters
\ (the word plate's)
begin-structure /held  lfield: h.id  lfield: h.n  8 +field letters  end-structure
\ the three lists: the key things (ids under $40), the remedies, baits and what she wears (under
\ $A0), the materials and documents
enum: item-list  key-list kit-list material-list ;
64 constant #places
: held-at ( list i -- addr )  swap #places * + /held * progress pr.item-lists + ;
: id@ ( addr -- id )  h.id sl@ ;
: n@ ( addr -- n )  h.n sl@ ;
: empty? ( addr -- flag )  id@ 0< ;
: list-of ( id -- list )  dup $40 < if  drop key-list exit  then  $A0 < kit-list material-list ? ;

\ the items' classes (items/tables.fs)
: class-of ( id -- addr )  /item-info * item-info + ;
: item-class? ( id -- flag )  dup 0 #item-ids within if  class-of i.kind @ 0< 0=  else  drop false  then ;
: item-kind ( id -- kind )  class-of i.kind @ ;         \ (enum: item-kind)
: item-actions ( id -- bits )  class-of i.actions @ ;   \ (can-use can-equip can-discard note-changes)
: item-held ( id -- how )  class-of i.held @ ;          \ (enum: held-as)
: item-sort ( id -- key )  class-of i.sort @ ;
: stacks? ( id -- flag )  item-held stack = ;

: listed ( list -- n )   \ how many places of the list are filled (Items_FirstFree)
    #places 0 do  dup i held-at empty? if  drop i unloop exit  then  loop  drop #places ;
: item-at ( list i -- id )  held-at id@ ;
: how-many ( addr -- n )   \ (PoolEntry_Count: a stack its number, else 1; 0 an empty place)
    dup empty? if  drop 0 exit  then  dup id@ stacks? if  n@  else  drop 1  then ;
: find-item ( id -- addr | 0 )   \ (Items_Count: wherever it is)
    {: id :}
    3 0 do  #places 0 do
        j i held-at {: place :}
        place empty? if  leave  then
        place id@ id = if  place unloop unloop exit  then
    loop  loop  0 ;
: items-of ( id -- n )  find-item dup if  how-many  then ;

\ what she wears, or holds to throw: a slot per kind (her boots, earrings, what she throws, the choker)
enum: equip-slot  boots-slot earrings-slot throw-slot choker-slot ;
: equipped ( slot -- id )  4 * progress pr.equipped + sl@ ;
: kind-slot ( kind -- slot | -1 )  dup 0 10 within if  cells kind-slots + @  else  drop -1  then ;
: slot-of ( list i -- slot | -1 )  item-at dup 0< if  exit  then  item-kind kind-slot ;
-1 constant not-wearable   0 constant slot-free   1 constant worn   2 constant other-worn
: equip-state ( list i -- s )   \ not wearable, its slot free, worn, another worn there
    {: list at :}  list at slot-of {: slot :}
    slot 0< if  not-wearable exit  then
    slot equipped dup 0< if  drop slot-free exit  then
    list at item-at = worn other-worn ? ;

\ the files (key things, documents: their message ids; 128, in the order found)
: file@ ( i -- id )  2* progress pr.files + w@ ;
: files ( -- n )  128 0 do  i file@ 0= if  i unloop exit  then  loop  128 ;
: item-bit? ( n -- flag )  progress pr.item-bits bit? ;
: plates ( -- n )   \ (Items_CountItem3F: the word plates made)
    0  key-list listed 0 ?do  key-list i item-at word-plate-key = if  1+  then  loop ;

\ the timed effects (the progress' +0x9E0..: a value, frames left)
begin-structure /boost  sffield: b.value  lfield: b.left  end-structure
0 constant calm-boost       \ Lavender's: calming 0.27 a frame for 300 frames (+0x9E0, StatusTimers_Frame)
1 constant fear-boost       \ Torva's: the panic's fear inputs doubled for 1800 frames (+0x9E8, Panic_Update)
2 constant hush-boost       \ Weltigo's: everything heard faded out and back, 1800 frames (+0x9F0 the volume now, +0x9F4 left; not again while it runs)
3 constant fear-held        \ Fortis's: her fear held at 0 for 1800 frames (+0x9FC, +0xA00: StatusTimers_Frame)
4 constant blow-boost       \ Fortis's: her blows' damage x3 for 1800 frames (+0xA04, Fiona_Kick)
5 constant boost-a0c        \ Paptisma's 1800 frames (+0xA0C; nothing reads it)
6 constant bite-boost       \ Sylvesta Esca's: Hewie's bites by half again, 450 frames (+0xA10)
: boost ( k -- addr )  /boost * progress pr.boosts + ;
: boost-on? ( k -- flag )  boost b.left sl@ 0> ;
: boost-value ( k -- ) ( F: -- v )  boost b.value sf@ ;

\ what using one did (the items actor's answer, item-used): nothing, used up, a mark set, an event
\ started, she / Hewie act, the word plate's event, a hint filed (bits 1 / 2: one spent)
0 constant no-use   1 constant used-up   2 constant marked   4 constant event-started
5 constant acted   6 constant plate-used   8 constant hinted
$10 constant done-quietly   \ (a mod's: something happened, nothing spent - back to the list)

<PRIVATE
state: items-state
  cell field dog-commandable   \ (told by Hewie each frame) she may command him
  cell field dog-cond          \ his condition (dog-down: down)
  cell field her-busy          \ (told by Fiona each frame) what she does (0: free)
end-state

2 constant dog-down
: fiona-id ( -- id )  s" fiona" actor-named ;
: hewie-id ( -- id )  s" hewie" actor-named ;
: panic-id ( -- id )  s" panic" actor-named ;

\ ==== changing it (only here) ====
: n! ( n addr -- )  h.n l! ;
: equip-slot ( slot -- addr )  4 * progress pr.equipped + ;
: tell-equipment ( -- )  0 equipped 1 equipped 2 equipped 3 equipped broadcast equipment ;
: empty ( addr -- )  -1 over h.id l!  0 over n!  letters 8 0 fill ;
: list-place ( addr -- list i )  progress pr.item-lists - /held / #places /mod swap ;
\ an item taken out: off what she wears, the rest of its list moved up (Items_Remove)
: remove ( addr -- )
    {: addr :}
    4 0 do  i equipped addr id@ = if  -1 i equip-slot l!  then  loop
    addr list-place {: list at :}
    #places at 1+ ?do   \ (each after it one place up)
        list i held-at  dup empty? if  drop leave  then
        list i 1- held-at /held move
    loop
    list dup listed 1- held-at empty   \ (the last, moved up, emptied)
    tell-equipment ;
\ one of it spent: a stack of two or more loses one, else it goes (item_use_one)
: spend ( addr -- )
    dup id@ stacks?  over n@ 2 >= and if  dup n@ 1- swap n!  else  remove  then ;
\ given n (Items_Give): onto its stack if it stacks (up to 99; a full one won't take them), else
\ into its list's first free held-at - a stack of n, a counter at 0, else one
: onto-stack ( id n -- addr | 0 | -1 )   \ more of a stacking item she has (-1: she has none)
    {: id n :}
    id find-item ?dup 0= if  -1 exit  then
    dup n@ 99 < if  dup n@ n + 99 min over n!  else  drop 0  then ;
: give ( id n -- addr | 0 )   \ (Items_Give) onto its stack if it stacks, else into its list's next place
    {: id n | list place :}
    id stacks? if  id n onto-stack dup -1 <> if  exit  then  drop  then
    id list-of to list
    list listed #places =  n 0= or  id item-class? 0= or if  0 exit  then   \ (a full list, none, no such item)
    list list listed held-at to place
    id place h.id l!
    id item-held case  stack of  n 99 min  endof  counter of  0  endof  >r 1 r>  endcase  place n!   \ (a stack, a counter's frames, one)
    place letters 8 0 fill  place ;
\ a file found (SubScreen_AddFile): true if added (not already there, room for it)
: file-add ( id -- flag )
    $FFFF and {: id :}
    128 0 do
        progress pr.files i 2* +  {: at :}
        at w@ id = if  false unloop exit  then   \ (filed already)
        at w@ 0= if  id at w!  true unloop exit  then
    loop  false ;
: item-bit-set ( n -- )  progress pr.item-bits bit-set ;
: boost! ( k frames -- ) ( F: v -- )  {: k frames :}  frames k boost b.left l!  k boost b.value sf! ;
: boost-frames! ( k frames -- )  swap boost b.left l! ;
1800 constant hush-frames   30 constant hush-fade   \ (Weltigo's: its length; out and back in over 30 frames)

\ ==== using one: its own rule (the items' +0x3C uses; items.c, story_chars.c). The result: 0
\ nothing, 1 used up, 2 a mark set (used up), 4 an event started, 5 she / Hewie act, 6 the word
\ plate's event, 8 a hint (bits 1 / 2: one spent) ====
: her-at ( -- ) ( F: -- x y z )  fiona-id body-pos ;
: in-room? ( room -- flag )  room-id = ;
: at-spot? ( room spot -- flag )   \ (item_room_spot: she stands in the room's event area)
    swap in-room? 0= if  drop false exit  then  her-at area-in? ;
: at-door? ( room exit -- flag )   \ (item_door_open: before the exit's door)
    swap in-room? 0= if  drop false exit  then
    dup door-here? 0= if  drop false exit  then  0 her-at door-in-area? ;
: route-locked? ( door -- flag )  4 * progress pr.doors + l@ 8 and 0<> ;   \ (its state word's locked bit)
: event ( who ev -- )  -1 0 broadcast item-event ;   \ (item_event: the room's action script)
: event-flag ( ev -- r )  0 swap event  stalkers-stay state-flag-set  event-started ;   \ (item_event_flag)
: event-only ( ev -- r )  0 swap event  event-started ;
: offer ( id -- r )   \ laid on the altar (item_offer): script variable 1 the item, event $14
    water-tower-b1-1 $1B at-spot? 0= if  drop no-use exit  then
    {: id :}  0 $14 1 id broadcast item-event  stalkers-stay state-flag-set  event-started ;
: hint ( needs-flag? file -- r )   \ (use_sound_only: no use here - in the easy game a hint is
    swap if  saturnus-hint-on story-flag? 0= if  drop no-use exit  then  then   \ filed, with its sound)
    items-40-as-70 story-flag? if  drop no-use exit  then
    file-add if  $C 5 bank-sound  hinted  else  no-use  then ;
: mark ( flag -- r )  story-flag-set  marked ;
: remedy-her ( fear recovery -- r )  fiona-id send remedy  used-up ;
: drink ( -- r )  0 $8D event  acted ;   \ (she drinks: her event $8D)
\ a bait for Hewie: only with him at hand - hers to command, within 20, reachable in a step
: at-hand? ( -- flag )
    dog-commandable @ 0= if  false exit  then
    fiona-id body-at hewie-id body-at vec-dist 20e f< 0= if  false exit  then
    fiona-id body-tri fiona-id body-at hewie-id body-at $20008 v-walk  hewie-id body-tri = ;
: feed ( how his-event -- r )
    at-hand? 0= if  2drop no-use exit  then
    swap hewie-id send fed  0 $8E event  1 swap event  acted ;   \ (item_give_hewie)
\ the ores' and godstones' places (place_item): rows of room, spot, the script variable given
\ the item (-1 none) and her event, ended by -1; room $40's only while it still takes one
begin-structure /place-row  field: pr-room  field: pr-spot  field: pr-var  field: pr-event  end-structure
: takes-one? ( row -- flag )   \ (in its room; the medallion room's only until it is done)
    pr-room @ dup in-room?  swap old-mansion-1f-2 = medallion-40-done story-flag? and 0= and ;
: place-item ( id rows -- r )
    {: id row :}
    begin  row pr-room @ 0< 0= while
        row takes-one?  row pr-spot @ her-at area-in?  and if
            row pr-var @ 0< if  0 row pr-event @ event
            else  0 row pr-event @ row pr-var @ id broadcast item-event  then
            stalkers-stay state-flag-set  event-started exit
        then
        row /place-row + to row
    repeat  no-use ;
create ore-08   old-mansion-1f-2 , $11 , 0 , 1 ,  old-mansion-1f-4 , 5 , 1 , 1 ,  old-mansion-2f-3 , 4 , 0 , $A ,
    old-mansion-2f-4 , $D , 1 , 3 ,  old-mansion-2f-12 , $B , 1 , 4 ,  old-mansion-1f-7 , 8 , -1 , 1 ,
    water-tower-b1-1 , $1B , 1 , $14 ,  -1 ,
create ore-09   old-mansion-1f-2 , $11 , 0 , 1 ,  old-mansion-1f-4 , 5 , 1 , 1 ,  old-mansion-1f-7 , 8 , 0 , 2 ,
    old-mansion-2f-3 , 4 , 0 , $A ,  old-mansion-2f-12 , $B , 1 , 4 ,  old-mansion-2f-4 , $D , -1 , 0 ,
    water-tower-b1-1 , $1B , 1 , $14 ,  -1 ,
create ore-0a   old-mansion-1f-2 , $11 , 0 , 1 ,  old-mansion-1f-4 , 5 , 1 , 1 ,  old-mansion-1f-7 , 8 , 0 , 2 ,
    old-mansion-2f-3 , 4 , 0 , $A ,  old-mansion-2f-4 , $D , 1 , 3 ,  old-mansion-2f-12 , $B , -1 , 0 ,
    water-tower-b1-1 , $1B , 1 , $14 ,  -1 ,
create ore-0b   old-mansion-1f-7 , 8 , 0 , 2 ,  old-mansion-2f-3 , 4 , 0 , $A ,  old-mansion-2f-4 , $D , 1 , 3 ,
    old-mansion-2f-12 , $B , 1 , 4 ,  old-mansion-1f-2 , $11 , 0 , 0 ,  old-mansion-1f-4 , 5 , -1 , 0 ,
    water-tower-b1-1 , $1B , 1 , $14 ,  -1 ,
create ore-0c   old-mansion-1f-4 , 5 , 1 , 1 ,  old-mansion-1f-7 , 8 , 0 , 2 ,  old-mansion-2f-3 , 4 , 0 , $A ,
    old-mansion-2f-4 , $D , 1 , 3 ,  old-mansion-2f-12 , $B , 1 , 4 ,  old-mansion-1f-2 , $11 , 0 , 0 ,
    water-tower-b1-1 , $1B , 1 , $14 ,  -1 ,
create ash-10   old-mansion-1f-4 , 5 , 1 , 1 ,  old-mansion-1f-7 , 8 , 0 , 2 ,  old-mansion-2f-4 , $D , 1 , 3 ,
    old-mansion-2f-12 , $B , 1 , 4 ,  old-mansion-2f-3 , 4 , -1 , 9 ,  -1 ,
\ the three books on the lecterns (place_statue: room $54's spots $D, $11, $14): book b on
\ lectern p is event p * 3 + b, while neither other book lies there; else on the altar
create lecterns  $D , $11 , $14 ,
: book-blocked? ( p b -- flag )   \ another book lies on lectern p
    {: p b :}  false  3 0 do  i b <> if  p 3 * i + books-placed + story-flag? or  then  loop ;
: lay-book ( id b -- r )
    {: id b :}
    old-mansion-1f-15 in-room? if
        3 0 do
            i b book-blocked? 0=  old-mansion-1f-15 i cells lecterns + @ at-spot? and if  b i 3 * + event-flag unloop exit  then
        loop
    then  id offer ;
\ ---- the word plate (item $3F, ItemClassF430_Use): what she does with the word it bears, room by
\ room, at the spot - its event started and the plate spent (6), or 0 elsewhere:
\  castle-2f-7 spot 3: event 5. castle-2f-8 spot 6 (until story flag 27, the golem done): EMETH
\  (flag 26, event $A), METH (flag 25, event $B), SALTATIO (event $E) - each stalkers-stay; any
\  other word event 5. castle-2f-12 spot $A (until flag $30C): MAGNUS event $B (stalkers-stay),
\  else $C. castle-1f-4 spot $11, door 8 locked: REST event $14 (stalkers-stay), else $17.
\  old-mansion-1f-9 spot 2: event 1. old-mansion-b1-6: spot $14 event 9; spot $16 SALTATIO event
\  $11, else the word into script variables 0 / 1 and event $A. old-mansion-1f-12 (until flag
\  $50): the elements MERCURY / SALT... (flags $4D..$4F) - two given already, Hewie up by her
\  (within 20, or she at spot $A): event 5 (stalkers-stay) - else at spot $A event $A (4); at
\  spot 8 (until $51) event 6, spot 9 (until $52) event 7; event bit 0 whether the word was one
\  not yet given (its flag set with it). off-map-27, flag $AE, spot 3: ALCHYMIA / ADAMAS /
\  POWDER / MORGAN (each once: flags $368..$36B) into variable 1 ($88 / $8C / $83 / $89) and
\  event 8, else event 9 ----
: plate-word? ( addr word len -- flag )   \ (its letters: the word, the rest blank)
    {: addr w n :}  addr letters  8 0 do
        dup i + c@  i n < if  w i + c@  else  0  then  <> if  drop false unloop exit  then
    loop  drop true ;
: word-var ( var value -- )  broadcast item-var ;              \ (the events' +0x30: a script variable)
: word-right ( on -- )  0 swap broadcast item-event-bit ;      \ (+0x5C / +0x60: event bit 0)
: plate-event ( ev -- r )  event-only drop  plate-used ;
: plate-event-flag ( ev -- r )  event-flag drop  plate-used ;
: dog-by-her? ( -- flag )   \ (Hewie up in her room, not down; she at spot $A or within 20 of him)
    hewie-id dup 0< if  drop false exit  then  dup body? 0= if  drop false exit  then
    body-room room-id <> dog-cond @ dog-down = or if  false exit  then
    old-mansion-1f-12 $A at-spot?  fiona-id body-at hewie-id body-at vec-dist 20e f< or ;
: element ( addr flag word len -- given? element )   \ (one of the elements: given already; else this plate's, if it names it)
    {: addr flag w n :}  flag story-flag? if  true -1 exit  then
    false  addr w n plate-word? flag -1 ? ;
: elements ( addr -- done element )   \ (how many given already; the element this plate names, not yet given: its flag, -1 none)
    {: addr | done named :}  -1 to named
    addr $4D s" MERCURY" element  addr $4E s" SULFUR" element  addr $4F s" SALT" element
    3 0 do  dup 0< 0= if  to named  else  drop  then  if  done 1+ to done  then  loop
    done named ;
: element-given ( element -- )   \ (event bit 0: a new element named - its flag set)
    dup 0< if  drop false word-right  else  story-flag-set  true word-right  then ;
: element-at ( element ev -- r )   \ (at a spot: the element's flag, the event)
    over 0< 0= if  stalkers-stay state-flag-set  then  swap element-given  plate-event ;
: elements-rule ( addr -- r )
    elements {: done named :}
    done 2 = if
        dog-by-her? if  named element-given  5 plate-event-flag exit  then
        old-mansion-1f-12 $A at-spot? if  $A event-only exit  then  no-use exit
    then
    old-mansion-1f-12 8 at-spot?  $51 story-flag? 0= and if  named 6 element-at exit  then
    old-mansion-1f-12 9 at-spot?  $52 story-flag? 0= and if  named 7 element-at exit  then
    no-use ;
: once-word ( addr word len flag value -- addr done? )   \ (off-map-27: each word once)
    {: w n flag value :}  dup w n plate-word?  flag story-flag? 0= and if  1 value word-var  -1  else  0  then ;
: plate-use ( addr -- r )
    castle-2f-7 3 at-spot? if  drop 5 plate-event exit  then
    castle-2f-8 6 at-spot?  27 story-flag? 0= and if
        dup s" EMETH" plate-word? if  drop  26 story-flag-set  $A plate-event-flag exit  then
        dup s" METH" plate-word? if  drop  25 story-flag-set  $B plate-event-flag exit  then
        s" SALTATIO" plate-word? if  $E plate-event-flag  else  5 plate-event  then  exit
    then
    castle-2f-12 $A at-spot?  $30C story-flag? 0= and if
        s" MAGNUS" plate-word? if  $B plate-event-flag  else  $C plate-event  then  exit
    then
    castle-1f-4 $11 at-spot?  8 route-locked? and if
        s" REST" plate-word? if  $14 plate-event-flag  else  $17 plate-event  then  exit
    then
    old-mansion-1f-9 2 at-spot? if  drop 1 plate-event exit  then
    old-mansion-b1-6 $14 at-spot? if  drop 9 plate-event exit  then
    old-mansion-b1-6 $16 at-spot? if
        dup s" SALTATIO" plate-word? if  drop $11 plate-event exit  then
        letters  0 over l@ word-var  1 swap 4 + l@ word-var  $A plate-event exit
    then
    old-mansion-1f-12 in-room?  $50 story-flag? 0= and if  elements-rule exit  then
    off-map-27 3 at-spot?  $AE story-flag? and if
        s" ALCHYMIA" $368 $88 once-word if  drop 8 plate-event exit  then
        s" ADAMAS" $369 $8C once-word if  drop 8 plate-event exit  then
        s" POWDER" $36A $83 once-word if  drop 8 plate-event exit  then
        s" MORGAN" $36B $89 once-word if  drop 8 plate-event exit  then
        drop 9 plate-event exit
    then
    drop 0 ;

: use-rule ( addr -- r )
    dup id@ word-plate-key = if  plate-use exit  then
    id@ case
        broken-marionette of  castle-1f-4 $A at-spot? if  1 event-only  else  no-use  then  endof
        fresh-flowers of  flowers-laid story-flag? 0=  castle-1f-18 9 at-spot? and if  0 event-flag  else  no-use  then  endof
        withered-flowers of  withered-flowers offer  endof
        study-key of  castle-2f-6 5 at-spot? if  5 event-flag  else  no-use  then  endof
        lunar-refractor of  castle-1f-12 $C at-spot? if  4 event-flag  else  no-use  then  endof
        castle-sketch of  castle-map-held mark  endof
        saturnus-key of  castle-3f-1 0 at-door? if  1 event-flag  else  true $232 hint  then  endof
        smudged-key of  castle-b1-2 1 at-door? if  4 event-flag  else  smudged-key offer  then  endof
        prima-materia of  prima-materia ore-08 place-item  endof
        sulfuric-ore of  sulfuric-ore ore-09 place-item  endof
        silver-ore of  silver-ore ore-0a place-item  endof
        white-godstone of  white-godstone ore-0b place-item  endof
        red-godstone of  red-godstone ore-0c place-item  endof
        jupiter-key of
            old-mansion-b1-2 2 at-door? $51 route-locked? and if  1 event-flag
            else old-mansion-b1-2 3 at-door? $52 route-locked? and if  3 event-flag
            else  jupiter-key offer  then then  endof
        film-reel of  old-mansion-1f-14 3 at-spot? if  0 event-flag  else  film-reel offer  then  endof
        venus-key of  old-mansion-1f-12 0 at-door? if  1 event-flag  else  no-use  then  endof
        handful-of-ash of  handful-of-ash ash-10 place-item  endof
        mars-key of  old-mansion-1f-16 4 at-door? if  2 event-flag  else  no-use  then  endof
        cracked-plate-key of  castle-1f-4 $11 at-spot? 8 route-locked? and if  $16 event-only  else  cracked-plate-key offer  then  endof
        cracked-plate-key-2 of  castle-1f-4 $11 at-spot? 8 route-locked? and if  $16 event-only  else  cracked-plate-key-2 offer  then  endof
        garden-key of  garden-door-done story-flag? 0=  castle-1f-9 1 at-door? and if  4 event-flag  else  no-use  then  endof
        cathedral-secrets of  cathedral-secrets 0 lay-book  endof
        occult-philosophy of  occult-philosophy 1 lay-book  endof
        wisemans-crafts of  wisemans-crafts 2 lay-book  endof
        white-mandragora of  old-mansion-1f-8 $17 at-spot? if  0 event-only  else  no-use  then  endof
        yellow-mandragora of  old-mansion-1f-8 $17 at-spot? if  $17 event-only  else  no-use  then  endof
        red-mandragora of  old-mansion-1f-8 $17 at-spot? if  $18 event-only  else  no-use  then  endof
        blue-mandragora of  old-mansion-1f-8 $17 at-spot? if  $19 event-only  else  no-use  then  endof
        purple-mandragora of  old-mansion-1f-8 $17 at-spot? if  $1A event-only  else  no-use  then  endof
        hebels-perfume of
            old-mansion-1f-13 in-room?  dog-commandable @ and  dog-cond @ dog-down <> and  her-busy @ 0= and
            fiona-id body-at hewie-id body-at vec-dist 20e f< and if  $B event-only
            else  hebels-perfume offer  dup no-use = if  drop
                items-40-as-70 story-flag? 0= if  false $239 hint
                else  hewie-id body-room room-id =  $27F file-add and if  $C 5 bank-sound  8  else  no-use  then  then
            then then  endof
        mansion-sketch of  mansion-map-held mark  endof
        forest-map of  forest-map-held mark  endof
        tower-sketch of  tower-map-held mark  endof
        bronze-figurine of  water-tower-8f-1 $B at-spot? if  c7-second-event story-flag? if  $10  else  3  then  event-only  else  no-use  then  endof
        silver-disc of  water-tower-8f-1 $B at-spot? if  c7-second-event story-flag? if  $10  else  3  then  event-only  else  no-use  then  endof
        wooden-miniature of  water-tower-8f-1 $B at-spot? if  4 event-only  else  no-use  then  endof
        sol-key of
            house-of-truth-2f-1 in-room? if  house-of-truth-2f-1 0 at-door? $E2 route-locked? and if  2 event-flag  else  no-use  then
            else house-of-truth-1f-4 0 at-door? $E6 route-locked? and if  8 event-flag  else  no-use  then then  endof
        cane-of-caduceus of  house-of-truth-1f-9 3 at-spot? if  4 event-flag  else  no-use  then  endof
        mystery-sketch of  mystery-map-held mark  endof
        castle-gate-key of  front-garden-1 5 at-spot? if  $F event-flag  else  castle-gate-key offer  then  endof
        \ the remedies
        camomile of  -100 -1800 remedy-her  endof
        lavender of  calm-boost 300 -0.26666668e boost!  used-up  endof
        quies of  -25 -450 remedy-her  endof
        recreatio of  -100 -1800 remedy-her  endof
        mundus of  25 0 panic-id send soothe  used-up  endof
        sedetio of  100 0 panic-id send soothe  used-up  endof
        carbo of  25 0 fiona-id send remedy  drink  endof
        nigred of  100 0 fiona-id send remedy  drink  endof
        torva of  fear-boost 1800 2e boost!  drink  endof
        weltigo of  hush-boost boost-on? if  0  else  hush-boost hush-frames boost-frames!  drink  then  endof
        remedium of  -50 0 fiona-id send remedy  0 50 panic-id send soothe  drink  endof
        fortis of  -100 -1800 remedy-her drop  fear-held 1800 0e boost!  blow-boost 1800 3e boost!  used-up  endof
        paptisma of  boost-a0c 1800 0e boost!  used-up  endof
        \ the baits (to Hewie)
        chicken-jerky of  small-treat $8F feed  endof
        beef-jerky of  big-treat $8F feed  endof
        onion of  onion-bait $90 feed  endof
        esca of  small-treat $8F feed  endof
        magna-esca of  big-treat $8F feed  endof
        sylvesta-esca of  at-hand? if  bite-boost 450 0e boost!  then  fury-bait $91 feed  endof
        bennenam-esca of  rouse-bait $90 feed  endof
        folded-memo of  false $24B hint  endof
        >r no-use r>
    endcase ;

\ ---- each frame (Items_Update): the counters count - the Fresh Flowers wither after 9000
\ frames (Item01_CountUp: the Withered Flowers given, the flowers gone); the effects run down ----
9000 constant flowers-last   \ (frames: 5 minutes)
: count-up ( -- )
    {: | place :}
    3 0 do  #places 0 do
        j i held-at to place
        place empty? if  leave  then
        place id@ item-held counter = if
            place n@ flowers-last >  place id@ fresh-flowers = and if
                withered-flowers 1 give drop  flowers-withered story-flag-set  place remove  leave
            then
            place n@ 1+ place n!
        then
    loop  loop ;
\ ---- the timed effects, each frame before they run down (StatusTimers_Frame): Lavender's
\ calming (a value not negative would be a threat); Weltigo's fade of everything heard (out over
\ its first 30 frames, silent, back over its last 30: the sound, the music, the movies); Fortis's
\ hold on her fear. Not while a cutscene has the camera ----
: lavender-frame ( -- )
    calm-boost boost-on? 0= if  exit  then
    calm-boost boost-value  fdup f0< if  fabs f>cell panic-id send calm-add  else  f>cell panic-id send threat  then ;
: hush-volume ( left -- ) ( F: -- v )
    {: left :}
    left 0= if  1e exit  then
    left hush-frames hush-fade - >= if  1e  hush-frames left - s>f hush-fade s>f f/ f-  exit  then
    left hush-fade 1+ < if  1e  left s>f hush-fade s>f f/ f-  exit  then
    0e ;
: weltigo-frame ( -- )   \ (told only as it changes)
    hush-boost boost b.left sl@ hush-volume  0e fmax 1e fmin {: F: v :}
    v hush-boost boost-value f= if  exit  then
    v hush-boost boost b.value sf!  v world-volume! ;
: fortis-frame ( -- )
    fear-held boost-on? if  fear-held boost-value f>s 0 max 100 min fiona-id send fear-to  then ;
: timed-effects ( -- )  lavender-frame  weltigo-frame  fortis-frame ;
: run-down ( -- )  8 0 do  i boost b.left  dup sl@ dup 0> if  1- swap l!  else  2drop  then  loop ;
create swap-buf /held allot
: swap-places ( a b -- )  {: a b :}  a swap-buf /held move  b a /held move  swap-buf b /held move ;   \ (move: from, to)
PRIVATE>

\ ---- the hook (docs/library-and-mods.md): what using the item at a place does - the game's
\ rule (Items_Use), unless a mod set another (`' its-rule is item-use-rule`, calling what it
\ replaced - `action-of item-use-rule` - for the rest). Its answer as the rules': bits 1 / 2
\ spend one ----
defer item-use-rule ( place -- result )   ' use-rule is item-use-rule

behaviour keeping-items
  on spawned ( -- )
      3 0 do  #places 0 do  j i held-at empty  loop  loop
      4 0 do  -1 i equip-slot l!  loop
      progress pr.boosts 8 /boost * 0 fill
      self subscribe tick  self subscribe hewie-doing  self subscribe fiona-doing  self subscribe leaving-room ;
  on tick ( -- )  count-up  cam-scene? 0= if  timed-effects run-down  then ;
  \ (SceneGame's room change: everything heard full again - Weltigo's fade, if silent, only back
  \ as it fades in; and at the game's end)
  on leaving-room ( room exit -- )  2drop  1e world-volume! ;
  on killed ( -- )  1e world-volume! ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      {: here action mode sub cond mood group commandable near reachable breakable way trust :}
      commandable dog-commandable !  cond dog-cond ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  her-busy !  2drop 2drop ;
  on give-item ( id n -- )  give drop ;
  on take-item ( id -- )  find-item ?dup if  spend  then ;
  on add-file ( id sound? -- )  swap file-add and if  $C 5 bank-sound  then ;
  on use-item ( list held-at -- )   \ (Items_Use: its rule; bits 1 / 2 spend one)
      held-at {: place :}
      place empty? if  -1 no-use sender send item-used exit  then
      place id@ {: id :}  place item-use-rule {: r :}
      r 3 and  place id@ id = and if  place spend  then   \ (still there: a rule may have moved it)
      id r sender send item-used ;
  on discard-item ( list held-at -- )  held-at dup id@ 0< if  drop  else  spend  then ;
  on equip-item ( list held-at -- )
      {: list at :}  list at slot-of {: slot :}
      slot 0< if  exit  then  list at item-at slot equip-slot l!  tell-equipment ;
  on unequip-item ( list held-at -- )
      2dup equip-state worn = if  slot-of equip-slot -1 swap l!  tell-equipment  else  2drop  then ;
  on sort-items ( list -- )   \ (Items_Sort: by their places in a sort)
      {: list :}  list listed {: n :}  n 2 < if  exit  then
      n 1- 0 ?do   \ (a bubble sort, as the original)
          n 1- i - 0 ?do
              list i held-at  list i 1+ held-at {: a b :}
              a id@ item-sort  b id@ item-sort > if  a b swap-places  then
          loop
      loop ;
  on item-bit ( n -- )  item-bit-set ;
  on give-plate ( c0 c1 c2 c3 c4 c5 c6 c7 -- )   \ (Items_NewItem3F: a word plate, its letters)
      word-plate-key 1 give {: plate :}
      plate 0= if  8 0 do  drop  loop  exit  then
      8 0 do  plate letters 7 i - + c!  loop ;
end-behaviour

: items-spawn ( -- id )  keeping-items items-state s" items" spawn ;
