\ story/rooms/old-mansion-1f-15.fs - the event scripts of room old-mansion-1f-15 ($54; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-15
USING: engine actors messages room-names story.words story.shared flag-names story-names ;

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
            a n off object@  g off shelf-stop  f< if  g off shelf-stop a n off object!  0 ebit-clear  then
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
    0 0 var-set
    1 0 var-set
    got-cathedral-secrets story-flag? not if
        1 1 $14 door-bits
    then
    got-occult-philosophy story-flag? not if
        2 1 $14 door-bits
    then
    got-wisemans-crafts story-flag? not if
        0 1 $14 door-bits
    then
    books-placed story-flag? not if
        1 4 $20000 nav-group
        1 5 $1000000 nav-group
        8 1 object-hidden
    else
        1 5 $20000 nav-group
        2 0 old-mansion-1f-15.cmd00
    then
    lectern-0-book-1 story-flag? if
        $B 1 $14 door-bits
    then
    lectern-0-book-2 story-flag? if
        $A 1 $14 door-bits
    then
    lectern-1-book-0 story-flag? if
        9 1 $14 door-bits
    then
    lectern-1-book-1 story-flag? not if
        1 2 $20000 nav-group
        1 3 $1000000 nav-group
        1 $18A $10000000 nav-tri-flags
        1 $1D3 $10000000 nav-tri-flags
        4 1 object-hidden
    else
        1 3 $20000 nav-group
        1 $187 $10000000 nav-tri-flags
        1 $1D0 $10000000 nav-tri-flags
        1 $188 $10000000 nav-tri-flags
        1 $1D1 $10000000 nav-tri-flags
        1 0 old-mansion-1f-15.cmd00
    then
    lectern-1-book-2 story-flag? if
        8 1 $14 door-bits
    then
    lectern-2-book-0 story-flag? if
        6 1 $14 door-bits
    then
    lectern-2-book-1 story-flag? if
        7 1 $14 door-bits
    then
    lectern-2-book-2 story-flag? not if
        1 0 $20000 nav-group
        1 1 $1000000 nav-group
        0 1 object-hidden
        0 6 $200 nav-group
        old-mansion-1f-15.cmd01
    else
        1 1 $20000 nav-group
        0 0 old-mansion-1f-15.cmd00
    then
    om1f-15-flag1 story-flag? not if
        0 0 0 $69 $12A obstacle-place
    else
        0 0 0 $87 $148 obstacle-place
    then
    1 7 $10000000 nav-group
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
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 -1 player-camera
    then then
    1 1 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 -1 player-camera
    then then
    2 0 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 -1 player-camera
    then then
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
                $19 om1f-15-antimony-tube-found var-set
                $1A om1f-15-antimony-tube var-set
                $1B 1 var-set
                $1C 39160 var-set
                $1D 1000 var-set
                $1E -38760 var-set
                $1F 1 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-15-rubrum-medallion-found story-flag? not if
        2 -23.61 50.0 -61.37 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-15-rubrum-medallion-found var-set
                $1A om1f-15-rubrum-medallion var-set
                $1B 2 var-set
                $1C -23610 var-set
                $1D 51000 var-set
                $1E -61370 var-set
                $1F 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    got-occult-philosophy story-flag? not if
        3 -31.67 0.0 -62.5 $B 10 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 hewie-who $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        3 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        3 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 hewie-who $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        5 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 24.15 0.0 76.38 0 0 sound
                $40000008 6 -44.0 70.0 22.0 0 0 sound
            else 0 1 var? if
                $40000004 6 24.15 0.0 76.38 0 0 sound
                $40000009 6 -44.0 70.0 22.0 0 0 sound
            else 0 2 var? if
                $40000005 6 24.15 0.0 76.38 0 0 sound
                $4000000A 6 -44.0 70.0 22.0 0 0 sound
            else 0 3 var? if
                $40000006 6 24.15 0.0 76.38 0 0 sound
                $4000000B 6 -44.0 70.0 22.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: old-mansion-1f-15.phase2 ( -- )   \ 004272A0
    got-cathedral-secrets story-flag? not if
        fiona-who 9 $32 char-faces-area? if
            5 9 4 scene-change
        then
    then
    got-occult-philosophy story-flag? not if
        fiona-who $B char-in-area? om1f-15-flag1 story-flag? and if
            5 $A 4 scene-change
        then
    then
    got-wisemans-crafts story-flag? not if
        fiona-who $A char-in-area? fiona-who 90 $32 char-heading? and if
            5 $B 4 scene-change
        then
    then
    books-placed story-flag? not if
        fiona-who $D char-in-area? fiona-who -45 $32 char-heading? and if
            lectern-0-book-1 story-flag? lectern-0-book-2 story-flag? or if
                5 $C 4 scene-change
            else
                5 $C 0 scene-change
            then
        then
        fiona-who $C char-in-area? fiona-who 45 $32 char-heading? and if
            5 $10 0 scene-change
        then
    else
        fiona-who $10 char-in-area? fiona-who -45 $32 char-heading? and if
            5 $F 0 scene-change
        then
        fiona-who $F char-in-area? fiona-who 45 $32 char-heading? and if
            5 $10 0 scene-change
        then
    then
    lectern-1-book-1 story-flag? not if
        fiona-who $11 char-in-area? fiona-who 45 $32 char-heading? and if
            lectern-1-book-0 story-flag? lectern-1-book-2 story-flag? or if
                5 $D 4 scene-change
            else
                5 $D 0 scene-change
            then
        then
    else fiona-who $13 char-in-area? fiona-who 45 $32 char-heading? and if
        5 $11 0 scene-change
    then then
    lectern-2-book-2 story-flag? not if
        fiona-who $14 char-in-area? fiona-who 90 $32 char-heading? and if
            lectern-2-book-0 story-flag? lectern-2-book-1 story-flag? or if
                5 $E 4 scene-change
            else
                5 $E 0 scene-change
            then
        then
        fiona-who $15 char-in-area? fiona-who 0 $32 char-heading? and if
            5 $13 0 scene-change
        then
    else
        fiona-who $17 char-in-area? fiona-who 90 $32 char-heading? and if
            5 $12 0 scene-change
        then
        fiona-who $18 char-in-area? fiona-who 0 $32 char-heading? and if
            5 $13 0 scene-change
        then
    then
    fiona-who $19 char-in-area? fiona-who 90 $32 char-heading? and if
        5 $12 0 scene-change
    then
    fiona-who $1B char-in-area? fiona-who 0 $32 char-heading? and if
        5 $13 0 scene-change
    then
    fiona-who $1C char-in-area? fiona-who 90 $32 char-heading? and if
        5 $14 0 scene-change
    then
    fiona-who $1D char-in-area? fiona-who 0 $32 char-heading? and if
        5 $15 0 scene-change
    then
    fiona-who $20 char-in-area? fiona-who -45 $32 char-heading? and if
        5 $16 0 scene-change
    then
    fiona-who $21 char-in-area? fiona-who $22 char-in-area? or fiona-who 45 $32 char-heading? and if
        5 $17 0 scene-change
    then
    fiona-who $1E char-in-area? fiona-who 90 $32 char-heading? and if
        5 $18 0 scene-change
    then
    fiona-who $1F char-in-area? fiona-who 0 $32 char-heading? and if
        5 $19 0 scene-change
    then
    books-placed story-flag? not if
        fiona-who $1A char-in-area? fiona-who 90 $32 char-heading? and if
            5 $1A 0 scene-change
        then
    then
    fiona-who 8 char-in-area? fiona-who 22 $32 char-heading? and if
        5 $84 3 scene-change
    then
    fiona-who $23 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    om1f-15-refined-antimony story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 $1B 4 scene-change
        then
    then
    om1f-15-antimony-tube-found story-flag? om1f-15-antimony-tube story-flag? not and if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 $1C 4 scene-change
        then
    then
    om1f-15-rubrum-medallion-found story-flag? om1f-15-rubrum-medallion story-flag? not and if
        2 2 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 $1D 4 scene-change
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
        4 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    8 0 object-hidden
    books-placed story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    0 ebit-set
    2 3 old-mansion-1f-15.cmd00
    begin
        2 1 old-mansion-1f-15.cmd00
        2 4 old-mansion-1f-15.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 0.0 -52.5 10.0 0 0 sound
    0 4 $20000 nav-group
    1 5 $20000 nav-group
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
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
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    $B 1 $14 door-bits
    lectern-0-book-1 story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
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
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    $A 1 $14 door-bits
    lectern-0-book-2 story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
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
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    9 1 $14 door-bits
    lectern-1-book-0 story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
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
    0 $18A $10000000 nav-tri-flags
    0 $1D3 $10000000 nav-tri-flags
    1 $187 $10000000 nav-tri-flags
    1 $1D0 $10000000 nav-tri-flags
    1 $188 $10000000 nav-tri-flags
    1 $1D1 $10000000 nav-tri-flags
    1 25.0 20.0 50.0 2.0 event-camera
    hewie-who char-here? 0 hewie-side? and if
        4 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    4 0 object-hidden
    lectern-1-book-1 story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    40.0 15.0 25.0 self-look-at-point
    yield
    0 ebit-set
    1 3 old-mansion-1f-15.cmd00
    begin
        1 2 old-mansion-1f-15.cmd00
        1 4 old-mansion-1f-15.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 39.0 21.0 10.0 0 0 sound
    0 2 $20000 nav-group
    1 3 $20000 nav-group
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
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
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    8 1 $14 door-bits
    lectern-1-book-2 story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
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
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    6 1 $14 door-bits
    lectern-2-book-0 story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
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
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    7 1 $14 door-bits
    lectern-2-book-1 story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
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
        4 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    fiona-who $E 6 char-sound
    0 0 object-hidden
    lectern-2-book-2 story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    0 ebit-set
    0 3 old-mansion-1f-15.cmd00
    begin
        0 2 old-mansion-1f-15.cmd00
        0 4 old-mansion-1f-15.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 0.0 0.0 10.0 0 0 sound
    0 0 $20000 nav-group
    1 1 $20000 nav-group
    1 6 $200 nav-group
    old-mansion-1f-15.cmd01
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
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
    0 message   \ "There is a book lying on the table."
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    1 0 $14 door-bits
    got-cathedral-secrets story-flag-set
    $15 message-param-room
    $15 1 item-give-count
    0 $15 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-15.act0A ( -- )   \ 00427A80
    self-wait-done
    -25.0 -60.0 self-turn-to-xz
    self-wait-done
    1 message   \ "There is a book below the stand..."
    wait-message
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    2 0 $14 door-bits
    got-occult-philosophy story-flag-set
    $16 message-param-room
    $16 1 item-give-count
    0 $16 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-15.act0B ( -- )   \ 00427AD0
    self-wait-done
    1.0 -30.0 self-turn-to-xz
    self-wait-done
    2 message   \ "One of the books stands out from the rest."
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0 $14 door-bits
    got-wisemans-crafts story-flag-set
    $17 message-param-room
    $17 1 item-give-count
    0 $17 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
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
        $904 self-anim
        self-wait-anim
        lectern-0-book-1 story-flag? if
            $B 0 $14 door-bits
            lectern-0-book-1 story-flag-clear
            $905 self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            $A 0 $14 door-bits
            lectern-0-book-2 story-flag-clear
            $905 self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $A message   \ "The books have been arranged by author. The shelf is full of books written by some..."
        wait-message
        self-frames-reset
        4 self-wait-frames
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
        $904 self-anim
        self-wait-anim
        lectern-1-book-0 story-flag? if
            9 0 $14 door-bits
            lectern-1-book-0 story-flag-clear
            $905 self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            8 0 $14 door-bits
            lectern-1-book-2 story-flag-clear
            $905 self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $B message   \ "The books have been arranged by author. The books are all written by a person name..."
        wait-message
        self-frames-reset
        4 self-wait-frames
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
        $904 self-anim
        self-wait-anim
        lectern-2-book-0 story-flag? if
            6 0 $14 door-bits
            lectern-2-book-0 story-flag-clear
            $905 self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            7 0 $14 door-bits
            lectern-2-book-1 story-flag-clear
            $905 self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $C message   \ "The books have been arranged by author. The shelf seems to contain books by Nichol..."
        wait-message
        self-frames-reset
        4 self-wait-frames
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
    3 message   \ "The books have been arranged by author. This shelf is full of books written by someone..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act10 ( -- )   \ 00427D98
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 message   \ "The books have been arranged by author. This shelf is full of books written by someone..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act11 ( -- )   \ 00427DA8
    self-wait-done
    90 self-turn-angle
    self-wait-done
    4 message   \ "The books have been arranged by author. These books are all written by a person named A..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act12 ( -- )   \ 00427DB8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    5 message   \ "The books have been arranged by author. This shelf seems to contain books by Nicholai F..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act13 ( -- )   \ 00427DC8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    5 message   \ "The books have been arranged by author. This shelf seems to contain books by Nicholai F..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act14 ( -- )   \ 00427DD8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    6 message   \ "The books have been arranged by author. Sangerman is the author of these books."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act15 ( -- )   \ 00427DE8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    6 message   \ "The books have been arranged by author. Sangerman is the author of these books."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act16 ( -- )   \ 00427DF8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    7 message   \ "The books have been arranged by author. These books are authored by Paraculeus."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act17 ( -- )   \ 00427E08
    self-wait-done
    90 self-turn-angle
    self-wait-done
    7 message   \ "The books have been arranged by author. These books are authored by Paraculeus."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act18 ( -- )   \ 00427E18
    self-wait-done
    180 self-turn-angle
    self-wait-done
    7 message   \ "The books have been arranged by author. These books are authored by Paraculeus."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act19 ( -- )   \ 00427E28
    self-wait-done
    0 self-turn-angle
    self-wait-done
    7 message   \ "The books have been arranged by author. These books are authored by Paraculeus."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act1A ( -- )   \ 00427E40
    self-wait-done
    1 ebit? not if
        180 self-turn-angle
        self-wait-done
        8 message   \ "The books have been arranged by author. The books are all written by a person named..."
        wait-message
        1 ebit-set
    else
        0.0 -80.0 self-turn-to-xz
        self-wait-done
        $A00 self-anim
        self-frames-reset
        self-wait-16
        9 message   \ "There is a door atop this bookshelf... Why would anyone build a door of all things..."
        wait-message
        1 ebit-clear
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1B ( -- )   \ 00427E70
    self-wait-done
    36.86 -27.91 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-15-refined-antimony story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1C ( -- )   \ 00427ED0
    self-wait-done
    39.16 -38.76 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $97 message-param-room
        $97 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-15-antimony-tube story-flag-set
            1 effect-remove
            $97 1 item-give-count
            0 $97 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1D ( -- )   \ 00427F30
    self-wait-done
    -23.61 -61.37 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-15-rubrum-medallion story-flag-set
            2 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-15.enter old-mansion-1f-15 0 room-script!
' old-mansion-1f-15.char-enter old-mansion-1f-15 6 room-script!
' old-mansion-1f-15.phase1 old-mansion-1f-15 1 room-script!
' old-mansion-1f-15.phase2 old-mansion-1f-15 2 room-script!
' old-mansion-1f-15.act00 old-mansion-1f-15 $00 action-script!
' old-mansion-1f-15.act01 old-mansion-1f-15 $01 action-script!
' old-mansion-1f-15.act02 old-mansion-1f-15 $02 action-script!
' old-mansion-1f-15.act03 old-mansion-1f-15 $03 action-script!
' old-mansion-1f-15.act04 old-mansion-1f-15 $04 action-script!
' old-mansion-1f-15.act05 old-mansion-1f-15 $05 action-script!
' old-mansion-1f-15.act06 old-mansion-1f-15 $06 action-script!
' old-mansion-1f-15.act07 old-mansion-1f-15 $07 action-script!
' old-mansion-1f-15.act08 old-mansion-1f-15 $08 action-script!
' old-mansion-1f-15.act09 old-mansion-1f-15 $09 action-script!
' old-mansion-1f-15.act0A old-mansion-1f-15 $0A action-script!
' old-mansion-1f-15.act0B old-mansion-1f-15 $0B action-script!
' old-mansion-1f-15.act0C old-mansion-1f-15 $0C action-script!
' old-mansion-1f-15.act0D old-mansion-1f-15 $0D action-script!
' old-mansion-1f-15.act0E old-mansion-1f-15 $0E action-script!
' old-mansion-1f-15.act0F old-mansion-1f-15 $0F action-script!
' old-mansion-1f-15.act10 old-mansion-1f-15 $10 action-script!
' old-mansion-1f-15.act11 old-mansion-1f-15 $11 action-script!
' old-mansion-1f-15.act12 old-mansion-1f-15 $12 action-script!
' old-mansion-1f-15.act13 old-mansion-1f-15 $13 action-script!
' old-mansion-1f-15.act14 old-mansion-1f-15 $14 action-script!
' old-mansion-1f-15.act15 old-mansion-1f-15 $15 action-script!
' old-mansion-1f-15.act16 old-mansion-1f-15 $16 action-script!
' old-mansion-1f-15.act17 old-mansion-1f-15 $17 action-script!
' old-mansion-1f-15.act18 old-mansion-1f-15 $18 action-script!
' old-mansion-1f-15.act19 old-mansion-1f-15 $19 action-script!
' old-mansion-1f-15.act1A old-mansion-1f-15 $1A action-script!
' old-mansion-1f-15.act1B old-mansion-1f-15 $1B action-script!
' old-mansion-1f-15.act1C old-mansion-1f-15 $1C action-script!
' old-mansion-1f-15.act1D old-mansion-1f-15 $1D action-script!
