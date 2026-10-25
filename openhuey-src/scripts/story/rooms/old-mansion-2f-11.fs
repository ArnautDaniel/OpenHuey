\ story/rooms/old-mansion-2f-11.fs - the event scripts of room old-mansion-2f-11 ($62; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-11
USING: engine actors messages vectors story.state room-names facts story.words story.shared flag-names story-names fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant ladder-remarked             \ (bit 0) its first remark made: "There is a rusty stepladder here..."
1 constant hewie-zone4-noticed         \ (bit 1) Hewie has reacted to zone 4 (once a visit)
0 constant tries-var                   \ (var 0) its tries (3: on)
1 constant books-looks-var             \ (var 1) her remarks on the books in turn
2 constant sound-step-var              \ (var 2) which of its sounds next (0..3)
3 constant sound-frames-var            \ (var 3) frames to the next sound (30)
3 constant ambient1-sound              \ (snd 3) its ambient sounds in turn (every 30 frames): the first
4 constant ambient2-sound              \ (snd 4) its ambient sounds in turn (every 30 frames): the second
5 constant ambient3-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the third
6 constant ambient4-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the fourth

\ room 0x62 (Room62_Cmd00_ptmf): object byte 4 swings: byte 3 0 starts it (phase +0x30 0, size
\ +0x34 0.01; object 0 with sound 7), else a step (+0x10 = size x sin(phase), phase on 60
\ degrees, the size down 0.001); 2 until it is still
: old-mansion-2f-11.cmd00 ( b0 b1 -- )   \ (object b1 - the room's string - swinging: b0 0 starts it (size 0.01; the first with its
    \ sound 7), else a step each frame till it is still)
    {: op k | a n :}  k room-object to n to a  a n object-here? 0= if  exit  then
    op 0= if
        0e a n $30 object!  0.01e a n $34 object!
        k 0= if  7 6 0 0  a n place-x object@ a n place-y object@ a n place-z object@  sound  then  exit
    then
    begin
        a n $34 object@ a n $30 object@ deg>rad fsin f* a n turn-x object!
        60e a n $30 object+!  -0.001e a n $34 object+!
        a n $34 object@ f0> while  yield
    repeat ;
fvariable drop-v   \ (the falling sprite's speed: gRoom62FallSpeed)
\ room 0x62 (D_00422318): the room's effect 0 dropped by byte 3 - 0 at rest (speed 0), 1 raised
\ by 0.5; else it falls (gravity 0.5 a frame, turning 0.16) and bounces off 0.7 losing 70% (a
\ sound each bounce) until slower than 0.2 (+0x74 set: landed; 1), else still going (2)
: old-mansion-2f-11.cmd01 ( b0 -- )   \ (room effect 0, the flickering light, dropped: b0 0 at rest, 1 raised 0.5;
    \ else it falls - 0.5 a frame faster, drifting 0.16 - and bounces off 0.7 keeping 30% (its sound
    \ 0 each bounce) till slower than 0.2, unresting as it goes and resting a frame there)
    {: b0 | e :}  0 fx-place to e
    b0 0= if  0e drop-v f!  exit  then
    b0 1 = if  0.5e e v.z field+!  0 effect-moved  exit  then
    begin
        0 fx-id @ dup alive? if  0 swap send effect-rest  else  drop  then
        0.16e e v.z field+!
        drop-v f@ 0.5e f- drop-v f!
        drop-v f@ e v.y field+!
        e v.y sf@ 0.7e f< if
            0.7e e v.y sf!  drop-v f@ -0.3e f* drop-v f!
            0 6 0 0  e vec@  sound-at
            drop-v f@ 0.2e f< if
                0 effect-moved  0 fx-id @ dup alive? if  1 swap send effect-rest  else  drop  then  exit
            then
        then
        0 effect-moved  yield
    again ;

: old-mansion-2f-11.enter ( -- )   \ 00421C60
    room-sounds
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    om2f-11-flag1 story-flag? not if
        0 5.19 35.3 -34.0 flicker-sprite
        0 old-mansion-2f-11.cmd01
    else got-jupiter-key story-flag? not if
        0 7.3 0.7 -28.3 flicker-sprite
    then then
    om2f-11-magnesia story-flag? not if
        1 -6.82 1.0 -10.25 flicker-sprite
    then
    items-40-as-70 story-flag? if
        om2f-11-flag2 story-flag? not if
            om2f-11-flag2 story-flag-set
            $62 0 15 $80 8 -1 $5A 0.0 creature-place
        then
    then
;

: old-mansion-2f-11.char-enter ( -- )   \ 00421CD0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
;

: old-mansion-2f-11.phase1 ( -- )   \ 00421D50
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 1 1 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    0 3.0 0.0 -35.5 9 15 0 zone
    1 -5.75 0.0 -33.25 9 15 0 zone
    2 -14.5 0.0 -31.0 9 15 0 zone
    om2f-11-flag1 story-flag? not if
        fiona-who 0 char-in-zone? fiona-who 1 char-in-zone? or fiona-who 2 char-in-zone? or if
            0 1 track $B action
        then
    then
    4 21.19 0.0 -20.13 $C 10 0 zone
    hewie-zone4-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    hewie-zone4-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 4 var-set
                        0 hewie-who $89 action
                    then
                then
            then
        then
    then
    5 19.29 0.0 -1.64 $14 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                hewie-zone-var 5 var-set
                hewie-zone-var 22.0 hewie-look-zone
            then
        then
    then
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 37.86 0.0 -38.87 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 37.86 0.0 -38.87 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 37.86 0.0 -38.87 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 37.86 0.0 -38.87 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
;

: old-mansion-2f-11.phase2 ( -- )   \ 00421EF0
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            0 always offer-action
        then
    then
    om2f-11-flag1 story-flag? got-jupiter-key story-flag? not and if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 2 char-zone-bits? if
            1 calm offer-action
        then
    then
    fiona-who 8 char-in-area? fiona-who 90 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    fiona-who 9 char-in-area? fiona-who 60 $32 char-heading? and if
        4 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who -10 $32 char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 90 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 0 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $D char-in-area? fiona-who -80 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who 90 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who -45 $32 char-heading? and if
        7 unhurried offer-action
    then
    om2f-11-magnesia story-flag? not if
        6 1 5 5 0 zone-at-effect
        fiona-who 6 3 char-zone-bits? if
            6 calm offer-action
        then
    then
    fiona-who $11 char-in-area? fiona-who -45 $32 char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $12 char-in-area? fiona-who -45 $32 char-heading? and if
        $A unhurried offer-action
    then
;

: old-mansion-2f-11.phase3 ( -- )   \ 00421FC0
    -7.0 30.0 42.0 -10.8 30.0 29.2 -7.0 0.0 42.0 -10.8 0.0 29.2 lights-doorway
;

: old-mansion-2f-11.act00 ( -- )   \ 00422000
    self-wait-done
    stalker-who self-touching? not if
        $67 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $67 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                $14 wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $67 door-unlock
                    $8008 show-message   \ "You unlocked the door."
                    20 self-idle-anim
                    $14 wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: old-mansion-2f-11.act01 ( -- )   \ 00422040
    self-wait-done
    7.3 -28.3 self-turn-to-xz
    self-wait-done
    pick-up-anim play-out
    4 wait-frames
    got-jupiter-key story-flag-set
    0 effect-remove
    $D message-param-room
    $D 1 item-give-count
    0 $D item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    up-from-pick-anim play-out
    self-idle-or-end
;

: old-mansion-2f-11.act02 ( -- )   \ 00422090
    om2f-11-flag1 story-flag-set
    2 old-mansion-2f-11.cmd01
    0 7.3 0.7 -28.3 flicker-sprite
    self-idle-or-end
;

: old-mansion-2f-11.act03 ( -- )   \ 004220A8
    0 0 old-mansion-2f-11.cmd00
    1 0 old-mansion-2f-11.cmd00
    self-idle-or-end
;

: old-mansion-2f-11.act04 ( -- )   \ 004220C0
    self-wait-done
    5.19 -34.0 self-turn-to-xz
    self-wait-done
    ladder-remarked ebit? not om2f-11-flag1 story-flag? or if
        0 say   \ "There is a rusty stepladder here. It really is in bad shape. If I were to try climb..."
        ladder-remarked ebit-set
    else
        look-at-anim play-out
        1 say   \ "There is something on top of the stepladder... I bet if I shake it a bit, I can get..."
        ladder-remarked ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-11.act05 ( -- )   \ 004220F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -20 self-turn-angle
    self-wait-done
    journals-examined story-flag? not if
        2 say   \ "Mountains of books have been piled up here. Are these journals?"
        journals-examined story-flag-set
    else
        3 say   \ "The dates on these journals are just too spread out for the same man to have writte..."
    then
    $F 6 fade
    wait-fade
    4 say   \ "February 8th, 1784 I have decided to summon that renowned master alchemist to the castl..."
    $F 7 fade
    wait-fade
    $237 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-11.act06 ( -- )   \ 00422130
    self-wait-done
    -6.82 -10.25 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-11-magnesia story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-11.act07 ( -- )   \ 00422190
    self-wait-done
    fiona-who $B char-in-area? if
        180 self-turn-angle
        self-wait-done
    else fiona-who $C char-in-area? if
        0 self-turn-angle
        self-wait-done
    else fiona-who $D char-in-area? if
        160 self-turn-angle
        self-wait-done
    else fiona-who $E char-in-area? if
        180 self-turn-angle
        self-wait-done
    else fiona-who $F char-in-area? if
        0 self-turn-angle
        self-wait-done
    else fiona-who $10 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    then then then then then then
    look-close-anim self-anim
    $28 wait-frames
    books-looks-var 0 var? if
        5 say   \ "What an incredible number of books."
        books-looks-var var-inc
    else books-looks-var 1 var? if
        6 say   \ "They are written in various languages. Whomever read all of these must have had a s..."
        books-looks-var var-inc
    else books-looks-var 2 var? if
        7 say   \ "Hmpf! What am I doing? It's not like I can even read the titles or anything."
        books-looks-var 0 var-set
    then then then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-11.act08 ( -- )   \ 00422208
    0 1 old-mansion-2f-11.cmd00
    1 1 old-mansion-2f-11.cmd00
    self-idle-or-end
;

: old-mansion-2f-11.act09 ( -- )   \ 00422220
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    8 say   \ "Something is written here..."
    $F 6 fade
    wait-fade
    $A say   \ "Some combinations can be used to aid animals as well. Here are a few: Esca Slightly re..."
    $F 7 fade
    wait-fade
    $63 subscreen-bit? not $65 subscreen-bit? not or if
        hand-to-mouth-anim self-anim
        $B say   \ "I wonder what these combinations are supposed to mean?"
        self-wait-anim
        $63 subscreen-bit
        $65 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-11.act0A ( -- )   \ 00422260
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    8 say   \ "Something is written here..."
    $F 6 fade
    wait-fade
    9 say   \ "The grand refining process can also yield several precarious items as well. I have list..."
    $F 7 fade
    wait-fade
    $46 subscreen-bit? not $47 subscreen-bit? not or if
        hand-to-mouth-anim self-anim
        $B say   \ "I wonder what these combinations are supposed to mean?"
        self-wait-anim
        $46 subscreen-bit
        $47 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-11.act0B ( -- )   \ 004222A0
    0 2 track 3 action
    0 4 track 8 action
    tries-var var-inc
    tries-var 3 var? if
        0 3 track 2 action
    else
        1 old-mansion-2f-11.cmd01
    then
    [: fiona-free? ;] wait-until
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-11 register-scripts   \ (its phases and actions, by their names)
