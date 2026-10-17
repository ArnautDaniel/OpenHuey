\ story/rooms/old-mansion-2f-12.fs - the event scripts of room old-mansion-2f-12 ($69; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-12
USING: engine room-names story.words story.shared flag-names story-names floor-flags ;

\ room 0x69 (D_004386F8): the three-dial lock by byte 3 - 0 the dials (and their lit twins) set
\ to their settings; 1 the player at it: up / down pick the dial (event variable 0; its lit twin
\ shown), left / right turn it (its setting, sound bit 3), cancel leaves (event bit 2); 2 the
\ picked dial turns 4 degrees a frame to its setting, then - 1, 0, 2 - the lock opens (event
\ ---- the three-dial lock (Room69): dials 0..2 and their lit twins 3..5, on
\ progress variables $18..$1A, 90 degrees a notch; 1, 0, 2 opens it ----
: dial-var ( k -- pv )  $18 + ;
: dial-angle ( k -- ) ( F: -- a )  dial-var progress-var@ 90 * s>f deg>rad angle-wrap ;
: dial ( k -- addr len )   \ (kDialNames: "dial0".."dial5" - the code's own table, not the room's strings)
    case  0 of  s" dial0"  endof  1 of  s" dial1"  endof  2 of  s" dial2"  endof
          3 of  s" dial3"  endof  4 of  s" dial4"  endof  >r s" dial5" r>  endcase ;
: pick-dial ( old sel -- )   \ (the lit twin of the picked one shown instead)
    {: old sel :}  0 old dial object-hidden!  1 old 3 + dial object-hidden!
    1 sel dial object-hidden!  0 sel 3 + dial object-hidden!
    0 sel var-set  6 ebit-set ;
: turn-dial ( sel -- )   \ (left / right: its setting 0..3 round, sound bit 3)
    {: sel | v :}  sel dial-var progress-var@ to v
    3 menu-repeat? if  v 0= if  3  else  v 1-  then  to v
    else  1 menu-repeat? if  v 3 < if  v 1+  else  0  then  to v  then  then
    v sel dial-var progress-var@ <> if  sel dial-var v progress-var!  3 ebit-set  then
    6 ebit-clear ;
: dials-step ( -- )   \ (2: the picked dial turning 4 degrees a frame to its setting; there, opened if 1 0 2)
    {: | sel :}  0 var @ to sel
    sel dial object-here? 0= sel 3 + dial object-here? 0= or if  exit  then
    sel dial turn-y object@ sel dial-angle f- angle-wrap                ( F: d )
    fdup 4e deg>rad f> if  fdrop  -4e deg>rad sel dial turn-y object+!  -4e deg>rad sel 3 + dial turn-y object+!  exit  then
    -4e deg>rad f< if  4e deg>rad sel dial turn-y object+!  4e deg>rad sel 3 + dial turn-y object+!  exit  then
    sel dial-angle sel dial turn-y object!  sel dial-angle sel 3 + dial turn-y object!
    0 dial-var progress-var@ 1 =  1 dial-var progress-var@ 0= and  2 dial-var progress-var@ 2 = and if
        2 ebit-clear  4 ebit-set
    else  3 ebit-clear  then ;
\ bits 2 off, 4)
: old-mansion-2f-12.cmd00 ( b0 -- )   \ (0 the dials set; 1 the player at it: up / down pick, left / right turn, cancel leaves
    \ (event flag 2 off); 2 the picked one turning to its setting)
    case
        0 of  6 0 do  i dial object-here? if  i 3 mod dial-angle i dial turn-y object!  then  loop  endof
        1 of  menu-cancel? if  2 ebit-clear  exit  then
              0 var @  dup  0 menu-pressed? if  dup 0= if  drop 2  else  1-  then
              else  2 menu-pressed? if  dup 2 < if  1+  else  drop 0  then  then  then   ( old sel )
              2dup <> if  pick-dial  else  nip turn-dial  then  endof
        2 of  dials-step  endof
    endcase ;

: old-mansion-2f-12.enter ( -- )   \ 00437E50
    room-sounds
    0 old-mansion-2f-12.cmd00
    3 1 object-hidden
    4 1 object-hidden
    5 1 object-hidden
    om2f-12-antimony-powder-found story-flag? om2f-12-antimony-powder story-flag? not and if
        0 82.73 31.0 -85.2 flicker-sprite
    then
    1 -31.0 51.9 -132.6 1 flame-in
    2 0.0 51.9 -142.7 1 flame-in
    3 30.9 51.8 -132.6 1 flame-in
    4 50.0 51.9 -106.3 1 flame-in
    5 -50.0 51.9 -106.3 1 flame-in
    6 82.0 51.9 -62.7 1 flame-in
    7 160.0 51.9 -62.7 1 flame-in
    8 215.7 21.9 -62.6 1 flame-in
    om2f-12-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    om2f-12-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
    then
    0 $C 0.812 0.687 0.187 0.312 zone-rect
    1 $3FFF sound-volume
;

: old-mansion-2f-12.char-enter ( -- )   \ 00437F50
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            3 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        3 -1 player-camera
    then then
    0 3 -1 area-camera
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
;

: old-mansion-2f-12.phase1 ( -- )   \ 00437FD0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 2 1 1 chars-area-camera
    5 3 -1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 1 1 chars-area-camera
    8 0 0 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    om2f-12-antimony-powder-found story-flag? not if
        2 82.73 30.0 -85.2 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om2f-12-antimony-powder-found var-set
                $1A om2f-12-antimony-powder var-set
                $1B 0 var-set
                $1C 82730 var-set
                $1D 31000 var-set
                $1E -85200 var-set
                $1F 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    3 -103.36 30.0 -72.17 $19 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 288.99 0.0 -98.66 $19 16 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    om2f-12-breakable1-broken story-flag? not if
        0 -114.0 30.0 -48.0 5 8 1 zone
        self-who 0 char-in-zone? if
            om2f-12-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -114.0 30.0 -48.0 0 -2144325584 0 0.0 shards-at
            $88 5 -114.0 30.0 -48.0 0 0 sound
            $40 $DD noise
        then
    then
    om2f-12-breakable2-broken story-flag? not if
        1 -114.0 30.0 -42.0 5 8 1 zone
        self-who 1 char-in-zone? if
            om2f-12-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            -114.0 30.0 -42.0 0 -2144325584 0 0.0 shards-at
            $88 5 -114.0 30.0 -42.0 0 0 sound
            $40 $DF noise
        then
    then
    6 sound-bank-loaded? if
        3 var-inc
        3 30 var? if
            3 0 var-set
            2 0 var? if
                $40000005 6 74.0 30.0 -110.0 0 0 sound
            else 2 1 var? if
                $40000006 6 74.0 30.0 -110.0 0 0 sound
            else 2 2 var? if
                $40000007 6 74.0 30.0 -110.0 0 0 sound
            else 2 3 var? if
                $40000008 6 74.0 30.0 -110.0 0 0 sound
            then then then then
            2 var-inc
            2 4 var? if
                2 0 var-set
            then
        then
    then
;

: old-mansion-2f-12.phase2 ( -- )   \ 00438230
    fiona-who $A $32 char-faces-area? if
        2 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 22 $3C char-heading? and if
        6 unhurried offer-action
    then
    -2147483646 scene-request? fiona-who 0 char-before-door? and if
        1 always offer-action
    then
    fiona-who $D char-in-area? fiona-who 50 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    om2f-12-antimony-powder-found story-flag? om2f-12-antimony-powder story-flag? not and if
        2 0 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 calm offer-action
        then
    then
;

: old-mansion-2f-12.phase3 ( -- )   \ 00438290
    251.4 20.0 -115.0 251.4 20.0 -90.0 251.4 0.0 -115.0 251.4 0.0 -90.0 lights-doorway
;

: old-mansion-2f-12.act00 ( -- )   \ 004382D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 4 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message   \ "Its composition has changed!"
    wait-message
    $902 self-anim
    self-wait-anim
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act01 ( -- )   \ 00438340
    self-wait-done
    0 self-through-exit
    self-wait-done
    5 ebit? not 2 game-mode? or in-panic? or if
        in-panic? 2 game-mode? or if
            $60A self-anim
            self-wait-anim
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
        else
            $608 self-anim
            self-wait-anim
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
            9 message   \ "It's locked. I can't open it... Now how am I going to get this open?"
            wait-message
        then
        5 ebit-set
    else
        $800 self-anim
        self-wait-anim
        $801 self-anim
        $A message   \ "Water is spilling out from the edge of the door frame. The room on the other side..."
        wait-message
        $802 self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        5 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-12.act02 ( -- )   \ 00438390
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    om2f-12-flag1 story-flag? not if
        1 message   \ "There's a dial connected to what looks like a water pipe... Operate the dial? YES NO"
        wait-message
        0 answer? if
            fiona-who $4B 293.136 -95.0 180 char-to-xz
            world-frozen state-flag-set
            1 5.0 0.0 0.0 2.0 event-camera
            2 message   \ "Push the left analog stick, up or down to change the dial setting. right or lef..."
            2 ebit-set
            0 0 var-set
            0 1 object-hidden
            3 0 object-hidden
            begin
                3 ebit? not if
                    1 old-mansion-2f-12.cmd00
                    3 ebit? if
                        1 6 293.0 12.0 -102.0 0 0 sound
                    then
                    6 ebit? if
                        0 6 293.0 12.0 -102.0 0 0 sound
                    then
                else
                    2 old-mansion-2f-12.cmd00
                then
                2 ebit? while
                yield
            repeat
            2 message-close
            world-frozen state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
            0 0 object-hidden
            1 0 object-hidden
            2 0 object-hidden
            3 1 object-hidden
            4 1 object-hidden
            5 1 object-hidden
            4 ebit? if
                2 6 318.0 12.0 -80.0 0 0 sound
                1 $FF $D2 rumble
                1 self-anim
                self-wait-anim
                0 self-anim
                self-frames-reset
                $32 self-wait-frames
                318.0 10.0 -80.0 self-look-at-point
                yield
                $72 5 318.0 12.0 -80.0 0 0 sound
                $74 door-unlock
                $62 door-unlock
                om2f-12-flag1 story-flag-set
                self-frames-reset
                $1E self-wait-frames
                fiona-who 3 6 char-sound
                self-frames-reset
                $5A self-wait-frames
                self-who self-look-at
                yield
            else om1f-14-flag1 story-flag? if
                $23B item-give
            then then
        else om1f-14-flag1 story-flag? if
            $23B item-give
        then then
    else
        7 message   \ "There's a device for draining water here."
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act03 ( -- )   \ 004384E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 ebit? not if
        $F 6 fade
        wait-fade
        3 message   \ "Synthesizer By separating the platinum half-piece and then synthesizing it with the..."
        wait-message
        1 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        4 message   \ "A synthesizer? What a peculiar device. It appears you can insert an item here..."
        wait-message
        self-wait-anim
        1 ebit-clear
    then
    $244 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act04 ( -- )   \ 00438530
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 4 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 9 var? if
        9 message-param-room
        0 9 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 11 var? if
        $B message-param-room
        0 $B item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    5 message   \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act05 ( -- )   \ 00438650
    self-wait-done
    82.73 -85.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-12-antimony-powder story-flag-set
            0 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-12.act06 ( -- )   \ 004386B0
    self-wait-done
    -69.5 -46.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message   \ "There are books spread out on the table. They seem to pertain to complex formulas, but..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-12.phase5 ( -- )   \ 0047AE7C
;

\ ---- registered ----
old-mansion-2f-12 register-scripts   \ (its phases and actions, by their names)
