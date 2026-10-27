\ story/rooms/old-mansion-1f-15.fs - the event scripts of room old-mansion-1f-15 ($54; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-15
USING: engine actors messages room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant shelves-moving              \ (bit 0) the shelves moving back
1 constant books-remarked              \ (bit 1) its first remark made: "The books have been arranged by author. The books are all wr"
2 constant hewie-zone3-noticed         \ (bit 2) Hewie has reacted to zone 3 (once a visit)
3 constant hewie-zone4-noticed         \ (bit 3) Hewie has reacted to zone 4 (once a visit)
4 constant hewie-away                  \ (bit 4) Hewie not here (or sent off) as the scene ends
5 constant hewie-zone4-noticed-2       \ (bit 5) Hewie has reacted to zone 4 (once a visit)
0 constant sound-step-var              \ (var 0) which of its sounds next (0..3)
1 constant sound-frames-var            \ (var 1) frames to the next sound (30)
3 constant ambient1-sound              \ (snd 3) its ambient sounds in turn (every 30 frames): the first
4 constant ambient2-sound              \ (snd 4) its ambient sounds in turn (every 30 frames): the second
5 constant ambient3-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the third
6 constant ambient4-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the fourth
8 constant ambient12-sound            \ (snd 8) its ambient sounds in turn (every 30 frames): the first
9 constant ambient22-sound            \ (snd 9) its ambient sounds in turn (every 30 frames): the second
10 constant ambient32-sound           \ (snd 10) its ambient sounds in turn (every 30 frames): the third
11 constant ambient42-sound           \ (snd 11) its ambient sounds in turn (every 30 frames): the fourth
13 constant shelves-stop-sound         \ (snd 13) the shelves coming to rest
14 constant place-books-sound          \ (snd 14) her placing the books
12 constant shelves-sound              \ (snd 12) the shelves moving

\ the moving bookshelves (Room54_GroupStep): group g's four (the colour's book and its three
\ shelves) - op 0 at their stop; 1 / 2 back 0.25 a frame in x / z to it (event bit 0 cleared
\ there); 3 / 4 their sound $C at the first (4 the kept-on kind); 5 the things lying on a
\ triangle of nav group g gone. (The dust they raise: not drawn yet)
: shelf ( i -- addr len )
    case
        0 of  s" movebook_blue"  endof    1 of  s" hondana_a0"  endof   2 of  s" hondana_a1"  endof   3 of  s" hondana_a2"  endof
        4 of  s" movebook_yellow"  endof  5 of  s" hondana_b0"  endof   6 of  s" hondana_b1"  endof   7 of  s" hondana_b2"  endof
        8 of  s" movebook_red"  endof     9 of  s" hondana_c0"  endof  10 of  s" hondana_c1"  endof  11 of  s" hondana_c2"  endof
        >r s" " r>
    endcase ;
create shelf-stops  0e f, 0e f,  39e f, 21e f,  0e f, -52.5e f,   \ (x, z by group)
: shelf-stop ( g off -- ) ( F: -- v )  place-z = 1 and  swap 2* +  floats shelf-stops + f@ ;
: shelves-back ( g off -- )
    {: g off | a n :}
    4 0 do
        g 4 * i + shelf to n to a
        a n object-here? if
            -0.25e a n off object+!
            a n off object@  g off shelf-stop  f< if  g off shelf-stop a n off object!  shelves-moving ebit-clear  then
        then
    loop ;
: shelves-at-stop ( g -- )
    {: g | a n :}
    4 0 do
        g 4 * i + shelf to n to a
        a n object-here? if  g place-x shelf-stop a n place-x object!  g place-z shelf-stop a n place-z object!  then
    loop ;
\ Room54_Cmd00
: old-mansion-1f-15.cmd00 ( b0 b1 -- )   \ (group b0, op b1: as the shelves' words above)
    {: g op | a n :}
    op case
        0 of  g 3 < if  g shelves-at-stop  then  endof
        1 of  g 3 < if  g place-x shelves-back  then  endof
        2 of  g 3 < if  g place-z shelves-back  then  endof
        3 of  4 g * shelf to n to a  a n object-here? if  $C 6 0 0  a n place-x object@ a n place-y object@ a n place-z object@  sound-at  then  endof
        4 of  4 g * shelf to n to a  a n object-here? if  $8000000C 6 0 0  a n place-x object@ a n place-y object@ a n place-z object@  sound-at  then  endof
        5 of  g things-off-group  endof
    endcase ;
\ Room54_Cmd01
: old-mansion-1f-15.cmd01 ( -- )  doors-id send rebuild-exits ;   \ (NavMesh_FindDoorRegions: the doorways found again, the shelves moved)

: old-mansion-1f-15.enter ( -- )   \ 00426D80
    room-sounds
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    got-cathedral-secrets story-flag? not if
        1 part-shown
    then
    got-occult-philosophy story-flag? not if
        2 part-shown
    then
    got-wisemans-crafts story-flag? not if
        0 part-shown
    then
    books-placed story-flag? not if
        4 barred-floor floor-on
        5 keeps-hewie-off floor-on
        8 1 object-hidden
    else
        5 barred-floor floor-on
        2 0 old-mansion-1f-15.cmd00
    then
    lectern-0-book-1 story-flag? if
        $B part-shown
    then
    lectern-0-book-2 story-flag? if
        $A part-shown
    then
    lectern-1-book-0 story-flag? if
        9 part-shown
    then
    lectern-1-book-1 story-flag? not if
        2 barred-floor floor-on
        3 keeps-hewie-off floor-on
        $18A hole-floor tri-floor-on
        $1D3 hole-floor tri-floor-on
        4 1 object-hidden
    else
        3 barred-floor floor-on
        $187 hole-floor tri-floor-on
        $1D0 hole-floor tri-floor-on
        $188 hole-floor tri-floor-on
        $1D1 hole-floor tri-floor-on
        1 0 old-mansion-1f-15.cmd00
    then
    lectern-1-book-2 story-flag? if
        8 part-shown
    then
    lectern-2-book-0 story-flag? if
        6 part-shown
    then
    lectern-2-book-1 story-flag? if
        7 part-shown
    then
    lectern-2-book-2 story-flag? not if
        0 barred-floor floor-on
        1 keeps-hewie-off floor-on
        0 1 object-hidden
        6 floor-flag-200 floor-off
        old-mansion-1f-15.cmd01
    else
        1 barred-floor floor-on
        0 0 old-mansion-1f-15.cmd00
    then
    om1f-15-flag1 story-flag? not if
        0 0 0 $69 $12A obstacle-place
    else
        0 0 0 $87 $148 obstacle-place
    then
    7 hole-floor floor-on
    om1f-15-refined-antimony story-flag? not if
        0 36.86 51.0 -27.91 flicker-sprite
    then
    om1f-15-antimony-tube-found story-flag? om1f-15-antimony-tube story-flag? not and if
        1 39.16 1.0 -38.76 flicker-sprite
    then
    om1f-15-rubrum-medallion-found story-flag? om1f-15-rubrum-medallion story-flag? not and if
        2 -23.61 51.0 -61.37 flicker-sprite
    then
;

: old-mansion-1f-15.char-enter ( -- )   \ 00426EF0
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 -1 player-camera
    then
    1 1 -1 area-camera
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 -1 player-camera
    then
    2 0 -1 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 -1 player-camera
    then
    3 0 -1 area-camera
    fiona-who self-is? if
        2 exit-taken? 3 exit-taken? or if
            3 map-page
        then
    then
;

: old-mansion-1f-15.phase1 ( -- )   \ 00426FF0
    1 exit-usable? if
        1 exit-check
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    $24 0 -1 1 chars-area-camera
    $25 1 -1 1 chars-area-camera
    $26 1 -1 -1 chars-area-camera
    $27 2 0 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    0 $148 obstacle-on? if
        om1f-15-flag1 story-flag-set
    else
        om1f-15-flag1 story-flag-clear
    then
    om1f-15-antimony-tube-found story-flag? not if
        1 39.16 0.0 -38.76 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-15-antimony-tube-found var-set
                taken-flag-var om1f-15-antimony-tube var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var 39160 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -38760 var-set
                hewie-zone-var 1 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-15-rubrum-medallion-found story-flag? not if
        2 -23.61 50.0 -61.37 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-15-rubrum-medallion-found var-set
                taken-flag-var om1f-15-rubrum-medallion var-set
                sparkle-fx-var 2 var-set
                sparkle-x-var -23610 var-set
                sparkle-y-var 51000 var-set
                sparkle-z-var -61370 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    got-occult-philosophy story-flag? not if
        3 -31.67 0.0 -62.5 $B 10 0 zone
        hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        hewie-zone3-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 3 var-set
                            0 hewie-who $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        hewie-zone4-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        hewie-zone4-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 4 var-set
                            0 hewie-who $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        hewie-zone4-noticed-2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        hewie-zone4-noticed-2 ebit-set
                        $64 chance? if
                            hewie-zone-var 4 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 24.15 0.0 76.38 0 0 sound
                ambient12-sound volume-scaled 6 -44.0 70.0 22.0 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 24.15 0.0 76.38 0 0 sound
                ambient22-sound volume-scaled 6 -44.0 70.0 22.0 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 24.15 0.0 76.38 0 0 sound
                ambient32-sound volume-scaled 6 -44.0 70.0 22.0 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 24.15 0.0 76.38 0 0 sound
                ambient42-sound volume-scaled 6 -44.0 70.0 22.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
;

: old-mansion-1f-15.phase2 ( -- )   \ 004272A0
    got-cathedral-secrets story-flag? not if
        fiona-who 9 $32 char-faces-area? if
            9 calm offer-action
        then
    then
    got-occult-philosophy story-flag? not if
        fiona-who $B char-in-area? om1f-15-flag1 story-flag? and if
            $A calm offer-action
        then
    then
    got-wisemans-crafts story-flag? not if
        fiona-who $A char-in-area? fiona-who 90 $32 char-heading? and if
            $B calm offer-action
        then
    then
    books-placed story-flag? not if
        fiona-who $D char-in-area? fiona-who -45 $32 char-heading? and if
            lectern-0-book-1 story-flag? lectern-0-book-2 story-flag? or if
                $C calm offer-action
            else
                $C unhurried offer-action
            then
        then
        fiona-who $C char-in-area? fiona-who 45 $32 char-heading? and if
            $10 unhurried offer-action
        then
    else
        fiona-who $10 char-in-area? fiona-who -45 $32 char-heading? and if
            $F unhurried offer-action
        then
        fiona-who $F char-in-area? fiona-who 45 $32 char-heading? and if
            $10 unhurried offer-action
        then
    then
    lectern-1-book-1 story-flag? not if
        fiona-who $11 char-in-area? fiona-who 45 $32 char-heading? and if
            lectern-1-book-0 story-flag? lectern-1-book-2 story-flag? or if
                $D calm offer-action
            else
                $D unhurried offer-action
            then
        then
    else fiona-who $13 char-in-area? fiona-who 45 $32 char-heading? and if
        $11 unhurried offer-action
    then then
    lectern-2-book-2 story-flag? not if
        fiona-who $14 char-in-area? fiona-who 90 $32 char-heading? and if
            lectern-2-book-0 story-flag? lectern-2-book-1 story-flag? or if
                $E calm offer-action
            else
                $E unhurried offer-action
            then
        then
        fiona-who $15 char-in-area? fiona-who 0 $32 char-heading? and if
            $13 unhurried offer-action
        then
    else
        fiona-who $17 char-in-area? fiona-who 90 $32 char-heading? and if
            $12 unhurried offer-action
        then
        fiona-who $18 char-in-area? fiona-who 0 $32 char-heading? and if
            $13 unhurried offer-action
        then
    then
    fiona-who $19 char-in-area? fiona-who 90 $32 char-heading? and if
        $12 unhurried offer-action
    then
    fiona-who $1B char-in-area? fiona-who 0 $32 char-heading? and if
        $13 unhurried offer-action
    then
    fiona-who $1C char-in-area? fiona-who 90 $32 char-heading? and if
        $14 unhurried offer-action
    then
    fiona-who $1D char-in-area? fiona-who 0 $32 char-heading? and if
        $15 unhurried offer-action
    then
    fiona-who $20 char-in-area? fiona-who -45 $32 char-heading? and if
        $16 unhurried offer-action
    then
    fiona-who $21 char-in-area? fiona-who $22 char-in-area? or fiona-who 45 $32 char-heading? and if
        $17 unhurried offer-action
    then
    fiona-who $1E char-in-area? fiona-who 90 $32 char-heading? and if
        $18 unhurried offer-action
    then
    fiona-who $1F char-in-area? fiona-who 0 $32 char-heading? and if
        $19 unhurried offer-action
    then
    books-placed story-flag? not if
        fiona-who $1A char-in-area? fiona-who 90 $32 char-heading? and if
            $1A unhurried offer-action
        then
    then
    fiona-who 8 char-in-area? fiona-who 22 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    fiona-who $23 $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    om1f-15-refined-antimony story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            $1B calm offer-action
        then
    then
    om1f-15-antimony-tube-found story-flag? om1f-15-antimony-tube story-flag? not and if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $1C calm offer-action
        then
    then
    om1f-15-rubrum-medallion-found story-flag? om1f-15-rubrum-medallion story-flag? not and if
        2 2 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            $1D calm offer-action
        then
    then
;

: old-mansion-1f-15.act00 ( -- )   \ 00427490
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    hewie-who char-here? 0 hewie-side? and if
        hewie-away ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        hewie-away ebit-set
    then
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    8 0 object-hidden
    books-placed story-flag-set
    $15 item-use
    reach-down-anim play-out
    shelves-moving ebit-set
    2 3 old-mansion-1f-15.cmd00
    [: 2 1 old-mansion-1f-15.cmd00 2 4 old-mansion-1f-15.cmd00 shelves-moving ebit? ;] wait-while
    shelves-sound 6 sound-stop
    shelves-stop-sound 6 0.0 -52.5 10.0 0 0 sound
    4 barred-floor floor-off
    5 barred-floor floor-on
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    hewie-away ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $54 0 221 hewie-to-room
            hewie-who $DD -27.0 37.0 60 char-to-xz
        then
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act01 ( -- )   \ 00427570
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    $B part-shown
    lectern-0-book-1 story-flag-set
    $16 item-use
    reach-down-anim play-out
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act02 ( -- )   \ 004275E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    $A part-shown
    lectern-0-book-2 story-flag-set
    $17 item-use
    reach-down-anim play-out
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act03 ( -- )   \ 00427650
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    9 part-shown
    lectern-1-book-0 story-flag-set
    $15 item-use
    reach-down-anim play-out
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act04 ( -- )   \ 004276C0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    3 5 old-mansion-1f-15.cmd00
    $18A hole-floor tri-floor-off
    $1D3 hole-floor tri-floor-off
    $187 hole-floor tri-floor-on
    $1D0 hole-floor tri-floor-on
    $188 hole-floor tri-floor-on
    $1D1 hole-floor tri-floor-on
    1 25.0 20.0 50.0 2.0 event-camera
    hewie-who char-here? 0 hewie-side? and if
        hewie-away ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        hewie-away ebit-set
    then
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    4 0 object-hidden
    lectern-1-book-1 story-flag-set
    $16 item-use
    reach-down-anim play-out
    40.0 15.0 25.0 self-look-at-point
    yield
    shelves-moving ebit-set
    1 3 old-mansion-1f-15.cmd00
    [: 1 2 old-mansion-1f-15.cmd00 1 4 old-mansion-1f-15.cmd00 shelves-moving ebit? ;] wait-while
    shelves-sound 6 sound-stop
    shelves-stop-sound 6 39.0 21.0 10.0 0 0 sound
    2 barred-floor floor-off
    3 barred-floor floor-on
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    hewie-away ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $54 0 22 hewie-to-room
            hewie-who $16 -41.0 -4.0 -100 char-to-xz
        then
    then
    self-who self-look-at
    yield
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act05 ( -- )   \ 004277F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    8 part-shown
    lectern-1-book-2 story-flag-set
    $17 item-use
    reach-down-anim play-out
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act06 ( -- )   \ 00427860
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    6 part-shown
    lectern-2-book-0 story-flag-set
    $15 item-use
    reach-down-anim play-out
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act07 ( -- )   \ 004278D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    7 part-shown
    lectern-2-book-1 story-flag-set
    $16 item-use
    reach-down-anim play-out
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act08 ( -- )   \ 00427940
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    hewie-who char-here? 0 hewie-side? and if
        hewie-away ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        hewie-away ebit-set
    then
    reach-up-anim play-out
    fiona-who place-books-sound 6 char-sound
    0 0 object-hidden
    lectern-2-book-2 story-flag-set
    $17 item-use
    reach-down-anim play-out
    shelves-moving ebit-set
    0 3 old-mansion-1f-15.cmd00
    [: 0 2 old-mansion-1f-15.cmd00 0 4 old-mansion-1f-15.cmd00 shelves-moving ebit? ;] wait-while
    shelves-sound 6 sound-stop
    shelves-stop-sound 6 0.0 0.0 10.0 0 0 sound
    0 barred-floor floor-off
    1 barred-floor floor-on
    6 floor-flag-200 floor-on
    old-mansion-1f-15.cmd01
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    hewie-away ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $54 0 255 hewie-to-room
            hewie-who $FF 38.0 -20.0 90 char-to-xz
        then
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act09 ( -- )   \ 00427A30
    self-wait-done
    -35.0 66.0 self-turn-to-xz
    self-wait-done
    0 say   \ "There is a book lying on the table."
    reach-out-anim play-out
    4 wait-frames
    1 part-hidden
    got-cathedral-secrets story-flag-set
    $15 message-param-room
    $15 1 item-give-count
    0 $15 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: old-mansion-1f-15.act0A ( -- )   \ 00427A80
    self-wait-done
    -25.0 -60.0 self-turn-to-xz
    self-wait-done
    1 say   \ "There is a book below the stand..."
    pick-up-anim play-out
    4 wait-frames
    2 part-hidden
    got-occult-philosophy story-flag-set
    $16 message-param-room
    $16 1 item-give-count
    0 $16 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    up-from-pick-anim play-out
    self-idle-or-end
;

: old-mansion-1f-15.act0B ( -- )   \ 00427AD0
    self-wait-done
    1.0 -30.0 self-turn-to-xz
    self-wait-done
    2 say   \ "One of the books stands out from the rest."
    reach-out-anim play-out
    4 wait-frames
    0 part-hidden
    got-wisemans-crafts story-flag-set
    $17 message-param-room
    $17 1 item-give-count
    0 $17 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: old-mansion-1f-15.act0C ( -- )   \ 00427B20
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    lectern-0-book-1 story-flag? lectern-0-book-2 story-flag? or if
        reach-up-anim play-out
        lectern-0-book-1 story-flag? if
            $B part-hidden
            lectern-0-book-1 story-flag-clear
            reach-down-anim self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            $A part-hidden
            lectern-0-book-2 story-flag-clear
            reach-down-anim self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        16 wait-frames
        16 wait-frames
        self-wait-anim
    else
        $A say   \ "The books have been arranged by author. The shelf is full of books written by some..."
        4 wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act0D ( -- )   \ 00427BF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    lectern-1-book-0 story-flag? lectern-1-book-2 story-flag? or if
        reach-up-anim play-out
        lectern-1-book-0 story-flag? if
            9 part-hidden
            lectern-1-book-0 story-flag-clear
            reach-down-anim self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            8 part-hidden
            lectern-1-book-2 story-flag-clear
            reach-down-anim self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        16 wait-frames
        16 wait-frames
        self-wait-anim
    else
        $B say   \ "The books have been arranged by author. The books are all written by a person name..."
        4 wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act0E ( -- )   \ 00427CC0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    lectern-2-book-0 story-flag? lectern-2-book-1 story-flag? or if
        reach-up-anim play-out
        lectern-2-book-0 story-flag? if
            6 part-hidden
            lectern-2-book-0 story-flag-clear
            reach-down-anim self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            7 part-hidden
            lectern-2-book-1 story-flag-clear
            reach-down-anim self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        16 wait-frames
        16 wait-frames
        self-wait-anim
    else
        $C say   \ "The books have been arranged by author. The shelf seems to contain books by Nichol..."
        4 wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act0F ( -- )   \ 00427D88
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    3 say   \ "The books have been arranged by author. This shelf is full of books written by someone..."
    self-idle-or-end
;

: old-mansion-1f-15.act10 ( -- )   \ 00427D98
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 say   \ "The books have been arranged by author. This shelf is full of books written by someone..."
    self-idle-or-end
;

: old-mansion-1f-15.act11 ( -- )   \ 00427DA8
    self-wait-done
    90 self-turn-angle
    self-wait-done
    4 say   \ "The books have been arranged by author. These books are all written by a person named A..."
    self-idle-or-end
;

: old-mansion-1f-15.act12 ( -- )   \ 00427DB8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    5 say   \ "The books have been arranged by author. This shelf seems to contain books by Nicholai F..."
    self-idle-or-end
;

: old-mansion-1f-15.act13 ( -- )   \ 00427DC8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    5 say   \ "The books have been arranged by author. This shelf seems to contain books by Nicholai F..."
    self-idle-or-end
;

: old-mansion-1f-15.act14 ( -- )   \ 00427DD8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    6 say   \ "The books have been arranged by author. Sangerman is the author of these books."
    self-idle-or-end
;

: old-mansion-1f-15.act15 ( -- )   \ 00427DE8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    6 say   \ "The books have been arranged by author. Sangerman is the author of these books."
    self-idle-or-end
;

: old-mansion-1f-15.act16 ( -- )   \ 00427DF8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    7 say   \ "The books have been arranged by author. These books are authored by Paraculeus."
    self-idle-or-end
;

: old-mansion-1f-15.act17 ( -- )   \ 00427E08
    self-wait-done
    90 self-turn-angle
    self-wait-done
    7 say   \ "The books have been arranged by author. These books are authored by Paraculeus."
    self-idle-or-end
;

: old-mansion-1f-15.act18 ( -- )   \ 00427E18
    self-wait-done
    180 self-turn-angle
    self-wait-done
    7 say   \ "The books have been arranged by author. These books are authored by Paraculeus."
    self-idle-or-end
;

: old-mansion-1f-15.act19 ( -- )   \ 00427E28
    self-wait-done
    0 self-turn-angle
    self-wait-done
    7 say   \ "The books have been arranged by author. These books are authored by Paraculeus."
    self-idle-or-end
;

: old-mansion-1f-15.act1A ( -- )   \ 00427E40
    self-wait-done
    books-remarked ebit? not if
        180 self-turn-angle
        self-wait-done
        8 say   \ "The books have been arranged by author. The books are all written by a person named..."
        books-remarked ebit-set
    else
        0.0 -80.0 self-turn-to-xz
        self-wait-done
        look-at-anim self-anim
        16 wait-frames
        9 say   \ "There is a door atop this bookshelf... Why would anyone build a door of all things..."
        books-remarked ebit-clear
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1B ( -- )   \ 00427E70
    self-wait-done
    36.86 -27.91 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-15-refined-antimony story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1C ( -- )   \ 00427ED0
    self-wait-done
    39.16 -38.76 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $97 message-param-room
        $97 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-15-antimony-tube story-flag-set
            1 effect-remove
            $97 1 item-give-count
            0 $97 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1D ( -- )   \ 00427F30
    self-wait-done
    -23.61 -61.37 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-15-rubrum-medallion story-flag-set
            2 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-15 register-scripts   \ (its phases and actions, by their names)
