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
16 constant /held
: held-at ( list i -- addr )  swap 64 * + /held * progress pr.item-lists + ;
: id@ ( addr -- id )  sl@ ;
: n@ ( addr -- n )  4 + sl@ ;
: letters ( addr -- addr' )  8 + ;
: list-of ( id -- list )  dup $40 < if  drop 0 exit  then  $A0 < if  1  else  2  then ;

\ the items' classes (items/tables.fs)
: class-of ( id -- addr )  /item-info * item-info + ;
: item-class? ( id -- flag )  dup 0 #item-ids within if  class-of @ 0< 0=  else  drop false  then ;
: item-kind ( id -- kind )  class-of @ ;            \ 0 a key thing, 2 a remedy, 3 a bait, 4.. worn, 7 a document, 8 a material
: item-actions ( id -- bits )  class-of cell+ @ ;   \ 1 use, 2 equip, 4 discard; $80000000 its note changes once used
: item-held ( id -- how )  class-of 2 cells + @ ;   \ 0 one, 1 a stack, 2 a counter
: item-sort ( id -- key )  class-of 3 cells + @ ;

: listed ( list -- n )   \ how many places of the list are filled (Items_FirstFree)
    64 0 do  dup i held-at id@ 0< if  drop i unloop exit  then  loop  drop 64 ;
: item-at ( list i -- id )  held-at id@ ;
: how-many ( addr -- n )   \ (PoolEntry_Count: a stack its number, else 1; 0 an empty held-at)
    dup id@ 0< if  drop 0 exit  then  dup id@ item-held 1 = if  n@  else  drop 1  then ;
: find-item ( id -- addr | 0 )   \ (Items_Count: wherever it is)
    3 0 do  64 0 do
        j i held-at dup id@ 0< if  drop leave  then
        dup id@ 2 pick = if  nip unloop unloop exit  then  drop
    loop  loop  drop 0 ;
: items-of ( id -- n )  find-item dup if  how-many  then ;

\ what she wears: a slot per kind (boots, earrings, the costume, the choker)
: equipped ( slot -- id )  4 * progress pr.equipped + sl@ ;
: kind-slot ( kind -- slot | -1 )  dup 0 10 within if  cells kind-slots + @  else  drop -1  then ;
: slot-of ( list i -- slot | -1 )  item-at dup 0< if  exit  then  item-kind kind-slot ;
: equip-state ( list i -- s )   \ -1 not worn at all, 0 its slot empty, 1 worn, 2 another worn there
    2dup slot-of dup 0< if  nip nip exit  then
    equipped dup 0< if  drop 2drop 0 exit  then  -rot item-at = if  1  else  2  then ;

\ the files (key things, documents: their message ids; 128, in the order found)
: file@ ( i -- id )  2* progress pr.files + w@ ;
: files ( -- n )  128 0 do  i file@ 0= if  i unloop exit  then  loop  128 ;
: item-bit? ( n -- flag )  progress pr.item-bits bit? ;
: plates ( -- n )   \ (Items_CountItem3F: the word plates made)
    0  0 listed 0 ?do  0 i item-at word-plate-key = if  1+  then  loop ;

\ the timed effects (the progress' +0x9E0..: a value, frames left)
0 constant calm-boost       \ Lavender's -0.27 for 300 frames (+0x9E0; nothing in the original reads it)
1 constant fear-boost       \ Torva's: the panic's fear inputs doubled for 1800 frames (+0x9E8, Panic_Update)
2 constant boost-9f4        \ Weltigo's 1800 frames (+0x9F4; not again while it runs; nothing reads it)
3 constant boost-9fc        \ Fortis's other (+0x9FC 0 for 1800 frames; nothing reads it)
4 constant blow-boost       \ Fortis's: her blows' damage x3 for 1800 frames (+0xA04, Fiona_Kick)
5 constant boost-a0c        \ Paptisma's 1800 frames (+0xA0C; nothing reads it)
6 constant bite-boost       \ Sylvesta Esca's: Hewie's bites by half again, 450 frames (+0xA10)
: boost ( k -- addr )  8 * progress pr.boosts + ;
: boost-on? ( k -- flag )  boost 4 + sl@ 0> ;
: boost-value ( k -- ) ( F: -- v )  boost sf@ ;

<PRIVATE
state: items-state
  cell field dog-commandable   \ (told by Hewie each frame) she may command him
  cell field dog-cond          \ his condition (2: down)
  cell field her-busy          \ (told by Fiona each frame) what she does (0: free)
end-state

: fiona-id ( -- id )  s" fiona" actor-named ;
: hewie-id ( -- id )  s" hewie" actor-named ;
: panic-id ( -- id )  s" panic" actor-named ;

\ ==== changing it (only here) ====
: n! ( n addr -- )  4 + l! ;
: equip-slot ( slot -- addr )  4 * progress pr.equipped + ;
: tell-equipment ( -- )  0 equipped 1 equipped 2 equipped 3 equipped broadcast equipment ;
: empty ( addr -- )  -1 over l!  0 over n!  letters 8 0 fill ;
: list-place ( addr -- list i )  progress pr.item-lists - /held / 64 /mod swap ;
\ an item taken out: off what she wears, the rest of its list moved up (Items_Remove)
: remove ( addr -- )
    dup id@  4 0 do  i equipped over = if  -1 i equip-slot l!  then  loop  drop
    dup list-place                                       ( addr list i )
    64 swap 1+ ?do                                       ( addr list )
        dup i held-at  dup id@ 0< if  drop leave  then
        over i 1- held-at /held move
    loop                                                 ( addr list )
    dup listed 1- held-at empty  drop                   \ (the last, moved up, emptied)
    tell-equipment ;
\ one of it spent: a stack of two or more loses one, else it goes (item_use_one)
: spend ( addr -- )
    dup id@ item-held 1 =  over n@ 2 >= and if  dup n@ 1- swap n!  else  remove  then ;
\ given n (Items_Give): onto its stack if it stacks (up to 99; a full one won't take them), else
\ into its list's first free held-at - a stack of n, a counter at 0, else one
: onto-stack ( id n -- addr | 0 | -1 )   \ more of a stacking item she has (-1: she has none)
    {: id n :}
    id find-item ?dup 0= if  -1 exit  then
    dup n@ 99 < if  dup n@ n + 99 min over n!  else  drop 0  then ;
: give ( id n -- addr | 0 )   \ (Items_Give) onto its stack if it stacks, else into its list's next place
    {: id n | list place :}
    id item-held 1 = if  id n onto-stack dup -1 <> if  exit  then  drop  then
    id list-of to list
    list listed 64 =  n 0= or  id item-class? 0= or if  0 exit  then   \ (a full list, none, no such item)
    list list listed held-at to place
    id place l!
    id item-held case  1 of  n 99 min  endof  2 of  0  endof  >r 1 r>  endcase  place n!   \ (a stack, a counter's frames, one)
    place letters 8 0 fill  place ;
\ a file found (SubScreen_AddFile): true if added (not already there, room for it)
: file-add ( id -- flag )
    $FFFF and  128 0 do
        progress pr.files i 2* + dup w@                  ( id addr v )
        dup 3 pick = if  2drop drop false unloop exit  then
        0= if  w! true unloop exit  then  drop
    loop  drop false ;
: bit-set ( n -- )  dup 5 rshift 4 * progress pr.item-bits +  swap 31 and 1 swap lshift  over l@ or swap l! ;
: boost! ( k frames -- ) ( F: v -- )  swap boost  tuck 4 + l!  sf! ;

\ ==== using one: its own rule (the items' +0x3C uses; items.c, story_chars.c). The result: 0
\ nothing, 1 used up, 2 a mark set (used up), 4 an event started, 5 she / Hewie act, 8 a hint ====
: her-at ( -- ) ( F: -- x y z )  fiona-id body-pos ;
: in-room? ( room -- flag )  room-id = ;
: at-spot? ( room spot -- flag )   \ (item_room_spot: she stands in the room's event area)
    swap in-room? 0= if  drop false exit  then  her-at area-in? ;
: at-door? ( room exit -- flag )   \ (item_door_open: before the exit's door)
    swap in-room? 0= if  drop false exit  then
    dup door-here? 0= if  drop false exit  then  0 her-at door-in-area? ;
: route-locked? ( door -- flag )  4 * progress pr.doors + l@ 8 and 0<> ;
: event ( who ev -- )  -1 0 broadcast item-event ;   \ (item_event: the room's action script)
: event-flag ( ev -- 4 )  0 swap event  stalkers-stay state-flag-set  4 ;   \ (item_event_flag)
: event-only ( ev -- 4 )  0 swap event  4 ;
: offer ( id -- r )   \ laid on the altar (item_offer): script variable 1 the item, event $14
    water-tower-b1-1 $1B at-spot? 0= if  drop 0 exit  then
    >r 0 $14 1 r> broadcast item-event  stalkers-stay state-flag-set  4 ;
: hint ( needs-flag? file -- r )   \ (use_sound_only: no use here - in the easy game a hint is
    swap if  saturnus-hint-on story-flag? 0= if  drop 0 exit  then  then   \ filed, with its sound)
    items-40-as-70 story-flag? if  drop 0 exit  then
    file-add if  $C 5 bank-sound  8  else  0  then ;
: mark ( flag -- 2 )  story-flag-set  2 ;
: remedy-her ( fear recovery -- 1 )  fiona-id send remedy  1 ;
: drink ( -- 5 )  0 $8D event  5 ;   \ (she drinks: her event $8D)
\ a bait for Hewie: only with him at hand - hers to command, within 20, reachable in a step
: at-hand? ( -- flag )
    dog-commandable @ 0= if  false exit  then
    fiona-id body-at hewie-id body-at vec-dist 20e f< 0= if  false exit  then
    fiona-id body-tri fiona-id body-at hewie-id body-at $20008 v-walk  hewie-id body-tri = ;
: feed ( how his-event -- r )
    at-hand? 0= if  2drop 0 exit  then
    swap hewie-id send fed  0 $8E event  1 swap event  5 ;   \ (item_give_hewie)
\ the ores' and godstones' places (place_item): rows of room, spot, the script variable given
\ the item (-1 none) and her event, ended by -1; room $40's only while it still takes one
variable pl-id
: place-item ( id rows -- r )
    swap pl-id !
    begin  dup @ 0< 0= while
        dup @ in-room?  over @ old-mansion-1f-2 = medallion-40-done story-flag? and 0= and if
            dup cell+ @ her-at area-in? if
                dup 2 cells + @ dup 0< if  drop  0 over 3 cells + @ event
                else  0 2 pick 3 cells + @ rot pl-id @ broadcast item-event  then
                drop  stalkers-stay state-flag-set  4 exit
            then
        then  4 cells +
    repeat  drop 0 ;
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
    false  3 0 do  i 2 pick <> if  2 pick 3 * i + books-placed + story-flag? or  then  loop  nip nip ;
: lay-book ( id b -- r )
    old-mansion-1f-15 in-room? if
        3 0 do
            i over book-blocked? 0= if
                old-mansion-1f-15 i cells lecterns + @ at-spot? if  nip i 3 * + event-flag unloop exit  then
            then
        loop
    then  drop offer ;
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
: plate-event ( ev -- 6 )  event-only drop  6 ;
: plate-event-flag ( ev -- 6 )  event-flag drop  6 ;
: dog-by-her? ( -- flag )   \ (Hewie up in her room, not down; she at spot $A or within 20 of him)
    hewie-id dup 0< if  drop false exit  then  dup body? 0= if  drop false exit  then
    body-room room-id <> dog-cond @ 2 = or if  false exit  then
    old-mansion-1f-12 $A at-spot?  fiona-id body-at hewie-id body-at vec-dist 20e f< or ;
variable element   \ (the element the plate names, not yet given: its flag; -1 none)
: elements ( addr -- done )   \ (how many given already; element: this plate's, if not)
    -1 element !  0 swap
    $4D story-flag? if  swap 1+ swap  else  dup s" MERCURY" plate-word? if  $4D element !  then  then
    $4E story-flag? if  swap 1+ swap  else  dup s" SULFUR" plate-word? if  $4E element !  then  then
    $4F story-flag? if  swap 1+ swap  else  dup s" SALT" plate-word? if  $4F element !  then  then
    drop ;
: element-given ( -- )   \ (event bit 0: a new element named - its flag set)
    element @ 0< if  false word-right  else  element @ story-flag-set  true word-right  then ;
: elements-rule ( addr -- r )
    elements 2 = if
        dog-by-her? if  element-given  5 plate-event-flag exit  then
        old-mansion-1f-12 $A at-spot? if  $A event-only exit  then  0 exit
    then
    old-mansion-1f-12 8 at-spot?  $51 story-flag? 0= and if
        element @ 0< 0= if  stalkers-stay state-flag-set  then  element-given  6 plate-event exit
    then
    old-mansion-1f-12 9 at-spot?  $52 story-flag? 0= and if
        element @ 0< 0= if  stalkers-stay state-flag-set  then  element-given  7 plate-event exit
    then  0 ;
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
        broken-marionette of  castle-1f-4 $A at-spot? if  1 event-only  else  0  then  endof
        fresh-flowers of  flowers-laid story-flag? 0=  castle-1f-18 9 at-spot? and if  0 event-flag  else  0  then  endof
        withered-flowers of  withered-flowers offer  endof
        study-key of  castle-2f-6 5 at-spot? if  5 event-flag  else  0  then  endof
        lunar-refractor of  castle-1f-12 $C at-spot? if  4 event-flag  else  0  then  endof
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
        venus-key of  old-mansion-1f-12 0 at-door? if  1 event-flag  else  0  then  endof
        handful-of-ash of  handful-of-ash ash-10 place-item  endof
        mars-key of  old-mansion-1f-16 4 at-door? if  2 event-flag  else  0  then  endof
        cracked-plate-key of  castle-1f-4 $11 at-spot? 8 route-locked? and if  $16 event-only  else  cracked-plate-key offer  then  endof
        cracked-plate-key-2 of  castle-1f-4 $11 at-spot? 8 route-locked? and if  $16 event-only  else  cracked-plate-key-2 offer  then  endof
        garden-key of  garden-door-done story-flag? 0=  castle-1f-9 1 at-door? and if  4 event-flag  else  0  then  endof
        cathedral-secrets of  cathedral-secrets 0 lay-book  endof
        occult-philosophy of  occult-philosophy 1 lay-book  endof
        wisemans-crafts of  wisemans-crafts 2 lay-book  endof
        white-mandragora of  old-mansion-1f-8 $17 at-spot? if  0 event-only  else  0  then  endof
        yellow-mandragora of  old-mansion-1f-8 $17 at-spot? if  $17 event-only  else  0  then  endof
        red-mandragora of  old-mansion-1f-8 $17 at-spot? if  $18 event-only  else  0  then  endof
        blue-mandragora of  old-mansion-1f-8 $17 at-spot? if  $19 event-only  else  0  then  endof
        purple-mandragora of  old-mansion-1f-8 $17 at-spot? if  $1A event-only  else  0  then  endof
        hebels-perfume of
            old-mansion-1f-13 in-room?  dog-commandable @ and  dog-cond @ 2 <> and  her-busy @ 0= and
            fiona-id body-at hewie-id body-at vec-dist 20e f< and if  $B event-only
            else  hebels-perfume offer  dup 0= if  drop
                items-40-as-70 story-flag? 0= if  false $239 hint
                else  hewie-id body-room room-id =  $27F file-add and if  $C 5 bank-sound  8  else  0  then  then
            then then  endof
        mansion-sketch of  mansion-map-held mark  endof
        forest-map of  forest-map-held mark  endof
        tower-sketch of  tower-map-held mark  endof
        bronze-figurine of  water-tower-8f-1 $B at-spot? if  c7-second-event story-flag? if  $10  else  3  then  event-only  else  0  then  endof
        silver-disc of  water-tower-8f-1 $B at-spot? if  c7-second-event story-flag? if  $10  else  3  then  event-only  else  0  then  endof
        wooden-miniature of  water-tower-8f-1 $B at-spot? if  4 event-only  else  0  then  endof
        sol-key of
            house-of-truth-2f-1 in-room? if  house-of-truth-2f-1 0 at-door? $E2 route-locked? and if  2 event-flag  else  0  then
            else house-of-truth-1f-4 0 at-door? $E6 route-locked? and if  8 event-flag  else  0  then then  endof
        cane-of-caduceus of  house-of-truth-1f-9 3 at-spot? if  4 event-flag  else  0  then  endof
        mystery-sketch of  mystery-map-held mark  endof
        castle-gate-key of  front-garden-1 5 at-spot? if  $F event-flag  else  castle-gate-key offer  then  endof
        \ the remedies
        camomile of  -100 -1800 remedy-her  endof
        lavender of  calm-boost 300 -0.26666668e boost!  1  endof
        quies of  -25 -450 remedy-her  endof
        recreatio of  -100 -1800 remedy-her  endof
        mundus of  25 0 panic-id send soothe  1  endof
        sedetio of  100 0 panic-id send soothe  1  endof
        carbo of  25 0 fiona-id send remedy  drink  endof
        nigred of  100 0 fiona-id send remedy  drink  endof
        torva of  fear-boost 1800 2e boost!  drink  endof
        weltigo of  boost-9f4 boost-on? if  0  else  boost-9f4 1800 0e boost!  drink  then  endof
        remedium of  -50 0 fiona-id send remedy  0 50 panic-id send soothe  drink  endof
        fortis of  -100 -1800 remedy-her drop  boost-9fc 1800 0e boost!  blow-boost 1800 3e boost!  1  endof
        paptisma of  boost-a0c 1800 0e boost!  1  endof
        \ the baits (to Hewie)
        chicken-jerky of  0 $8F feed  endof
        beef-jerky of  1 $8F feed  endof
        onion of  2 $90 feed  endof
        esca of  0 $8F feed  endof
        magna-esca of  1 $8F feed  endof
        sylvesta-esca of  at-hand? if  bite-boost 450 0e boost!  then  3 $91 feed  endof
        bennenam-esca of  4 $90 feed  endof
        folded-memo of  false $24B hint  endof
        >r 0 r>
    endcase ;

\ ---- each frame (Items_Update): the counters count - the Fresh Flowers wither after 9000
\ frames (Item01_CountUp: the Withered Flowers given, the flowers gone); the effects run down ----
: count-up ( -- )
    3 0 do  64 0 do
        j i held-at dup id@ 0< if  drop leave  then
        dup id@ item-held 2 = if
            dup n@ 9000 > over id@ fresh-flowers = and if
                withered-flowers 1 give drop  flowers-withered story-flag-set  remove  leave
            then
            dup n@ 1+ swap n!
        else  drop  then
    loop  loop ;
: run-down ( -- )  8 0 do  i boost 4 + dup sl@ dup 0> if  1- swap l!  else  2drop  then  loop ;
create swap-buf /held allot
PRIVATE>

behaviour keeping-items
  on spawned ( -- )
      3 0 do  64 0 do  j i held-at empty  loop  loop
      4 0 do  -1 i equip-slot l!  loop
      progress pr.boosts 8 8 * 0 fill
      self subscribe tick  self subscribe hewie-doing  self subscribe fiona-doing ;
  on tick ( -- )  count-up  run-down ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      2drop 2drop drop  dog-commandable !  drop  drop  dog-cond !  2drop 2drop ;
  on fiona-doing ( mode sub cond cmd doing -- )  her-busy !  2drop 2drop ;
  on give-item ( id n -- )  give drop ;
  on take-item ( id -- )  find-item ?dup if  spend  then ;
  on add-file ( id sound? -- )  swap file-add and if  $C 5 bank-sound  then ;
  on use-item ( list held-at -- )   \ (Items_Use: its rule; bits 1 / 2 spend one)
      held-at dup id@ 0< if  drop -1 0 sender send item-used exit  then
      dup id@ swap  dup use-rule                           ( id addr r )
      dup 3 and if  over id@ 3 pick = if  swap spend  else  nip  then  else  nip  then
      sender send item-used ;
  on discard-item ( list held-at -- )  held-at dup id@ 0< if  drop  else  spend  then ;
  on equip-item ( list held-at -- )
      2dup slot-of dup 0< if  drop 2drop exit  then  >r item-at r> equip-slot l!  tell-equipment ;
  on unequip-item ( list held-at -- )
      2dup equip-state 1 = if  slot-of equip-slot -1 swap l!  tell-equipment  else  2drop  then ;
  on sort-items ( list -- )   \ (Items_Sort: by their places in a sort)
      dup listed  dup 2 < if  2drop exit  then
      1- 0 ?do
          dup listed 1- i - 0 ?do
              dup i held-at  over i 1+ held-at                 ( list a b )
              over id@ item-sort  over id@ item-sort > if
                  swap-buf 2 pick /held move  over swap /held move  swap-buf swap /held move
              else  2drop  then
          loop
      loop  drop ;
  on item-bit ( n -- )  bit-set ;
  on give-plate ( c0 c1 c2 c3 c4 c5 c6 c7 -- )   \ (Items_NewItem3F: a word plate, its letters)
      word-plate-key 1 give ?dup 0= if  8 0 do  drop  loop  exit  then
      letters  8 0 do  tuck 7 i - + c!  loop  drop ;
end-behaviour

: items-spawn ( -- id )  keeping-items items-state s" items" spawn ;
