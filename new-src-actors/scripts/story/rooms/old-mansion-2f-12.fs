\ story/rooms/old-mansion-2f-12.fs - the event scripts of room old-mansion-2f-12 ($69; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-12
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
1 constant synthesizer-remarked        \ (bit 1) its first remark made: "Synthesizer By separating the platinum half-piece and then s"
2 constant dials-on                    \ (bit 2) the dials being set (cleared when right)
3 constant dial-turned                 \ (bit 3) a dial turned this frame
4 constant dials-right                 \ (bit 4) the dials set right
5 constant locked-remarked             \ (bit 5) its first remark made: "It's locked. I can't open it..."
6 constant dial-picked                 \ (bit 6) another dial picked this frame
0 constant picked-dial-var             \ (var 0) the dial picked
1 constant used-item-var               \ (var 1) the item she used (8 .. 11)
2 constant sound-step-var              \ (var 2) which of its sounds next (0..3)
3 constant sound-frames-var            \ (var 3) frames to the next sound (30)
0 constant dial-pick-sound             \ (snd 0) a dial picked
1 constant dial-turn-sound             \ (snd 1) a dial turned
2 constant dials-right-sound           \ (snd 2) the dials right (rumble)
3 constant unlocked-sound              \ (snd 3) the door unlocked
4 constant change-sound                \ (snd 4) it transforming
5 constant ambient1-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the first
6 constant ambient2-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the second
7 constant ambient3-sound              \ (snd 7) its ambient sounds in turn (every 30 frames): the third
8 constant ambient4-sound              \ (snd 8) its ambient sounds in turn (every 30 frames): the fourth

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
    picked-dial-var sel var-set  dial-picked ebit-set ;
: turn-dial ( sel -- )   \ (left / right: its setting 0..3 round, sound bit 3)
    {: sel | v :}  sel dial-var progress-var@ to v
    3 menu-repeat? if  v 0= if  3  else  v 1-  then  to v
    else  1 menu-repeat? if  v 3 < if  v 1+  else  0  then  to v  then  then
    v sel dial-var progress-var@ <> if  sel dial-var v progress-var!  dial-turned ebit-set  then
    dial-picked ebit-clear ;
: dials-step ( -- )   \ (2: the picked dial turning 4 degrees a frame to its setting; there, opened if 1 0 2)
    {: | sel :}  picked-dial-var var @ to sel
    sel dial object-here? 0= sel 3 + dial object-here? 0= or if  exit  then
    sel dial turn-y object@ sel dial-angle f- angle-wrap                ( F: d )
    fdup 4e deg>rad f> if  fdrop  -4e deg>rad sel dial turn-y object+!  -4e deg>rad sel 3 + dial turn-y object+!  exit  then
    -4e deg>rad f< if  4e deg>rad sel dial turn-y object+!  4e deg>rad sel 3 + dial turn-y object+!  exit  then
    sel dial-angle sel dial turn-y object!  sel dial-angle sel 3 + dial turn-y object!
    0 dial-var progress-var@ 1 =  1 dial-var progress-var@ 0= and  2 dial-var progress-var@ 2 = and if
        dials-on ebit-clear  dials-right ebit-set
    else  dial-turned ebit-clear  then ;
\ bits 2 off, 4)
: old-mansion-2f-12.cmd00 ( b0 -- )   \ (0 the dials set; 1 the player at it: up / down pick, left / right turn, cancel leaves
    \ (event flag 2 off); 2 the picked one turning to its setting)
    case
        0 of  6 0 do  i dial object-here? if  i 3 mod dial-angle i dial turn-y object!  then  loop  endof
        1 of  menu-cancel? if  dials-on ebit-clear  exit  then
              picked-dial-var var @  dup  0 menu-pressed? if  dup 0= if  drop 2  else  1-  then
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
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        3 -1 player-camera
    then
    0 3 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 -1 player-camera
    then
    1 1 -1 area-camera
;

: old-mansion-2f-12.phase1 ( -- )   \ 00437FD0
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
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
                found-flag-var om2f-12-antimony-powder-found var-set
                taken-flag-var om2f-12-antimony-powder var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 82730 var-set
                sparkle-y-var 31000 var-set
                sparkle-z-var -85200 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    3 -103.36 30.0 -72.17 $19 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
    4 288.99 0.0 -98.66 $19 16 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 16.0 hewie-look-zone
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
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 74.0 30.0 -110.0 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 74.0 30.0 -110.0 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 74.0 30.0 -110.0 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 74.0 30.0 -110.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
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
    fiona-who change-sound 6 char-sound
    $78 wait-frames
    look-close-anim play-out
    6 say   \ "Its composition has changed!"
    reach-out-anim play-out
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act01 ( -- )   \ 00438340
    self-wait-done
    0 self-through-exit
    self-wait-done
    locked-remarked ebit? not 2 game-mode? or in-panic? or if
        in-panic? 2 game-mode? or if
            rattle-front-slow-anim play-out
            -1 self-idle-anim
            7 wait-frames
        else
            rattle-front-anim play-out
            -1 self-idle-anim
            7 wait-frames
            9 say   \ "It's locked. I can't open it... Now how am I going to get this open?"
        then
        locked-remarked ebit-set
    else
        crouch-down-anim play-out
        crouching-anim self-anim
        $A say   \ "Water is spilling out from the edge of the door frame. The room on the other side..."
        crouch-up-anim play-out
        -1 self-idle-anim
        7 wait-frames
        locked-remarked ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-12.act02 ( -- )   \ 00438390
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    om2f-12-flag1 story-flag? not if
        1 say   \ "There's a dial connected to what looks like a water pipe... Operate the dial? YES NO"
        0 answer? if
            fiona-who $4B 293.136 -95.0 180 char-to-xz
            world-frozen state-flag-set
            1 5.0 0.0 0.0 2.0 event-camera
            2 message   \ "Push the left analog stick, up or down to change the dial setting. right or lef..."
            dials-on ebit-set
            picked-dial-var 0 var-set
            0 1 object-hidden
            3 0 object-hidden
            begin
                dial-turned ebit? not if
                    1 old-mansion-2f-12.cmd00
                    dial-turned ebit? if
                        dial-turn-sound 6 293.0 12.0 -102.0 0 0 sound
                    then
                    dial-picked ebit? if
                        dial-pick-sound 6 293.0 12.0 -102.0 0 0 sound
                    then
                else
                    2 old-mansion-2f-12.cmd00
                then
                dials-on ebit? while
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
            dials-right ebit? if
                dials-right-sound 6 318.0 12.0 -80.0 0 0 sound
                1 $FF $D2 rumble
                rest-anim play-out
                fiona.anims:stand-anim self-anim
                $32 wait-frames
                318.0 10.0 -80.0 self-look-at-point
                yield
                $72 5 318.0 12.0 -80.0 0 0 sound
                $74 door-unlock
                $62 door-unlock
                om2f-12-flag1 story-flag-set
                $1E wait-frames
                fiona-who unlocked-sound 6 char-sound
                $5A wait-frames
                self-who self-look-at
                yield
            else om1f-14-flag1 story-flag? if
                $23B item-give
            then then
        else om1f-14-flag1 story-flag? if
            $23B item-give
        then then
    else
        7 say   \ "There's a device for draining water here."
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
    synthesizer-remarked ebit? not if
        $F 6 fade
        wait-fade
        3 say   \ "Synthesizer By separating the platinum half-piece and then synthesizing it with the..."
        synthesizer-remarked ebit-set
        $F 7 fade
        wait-fade
    else
        hand-to-mouth-anim $A self-anim-blend
        4 say   \ "A synthesizer? What a peculiar device. It appears you can insert an item here..."
        self-wait-anim
        synthesizer-remarked ebit-clear
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
    fiona-who change-sound 6 char-sound
    $78 wait-frames
    look-close-anim play-out
    reach-out-anim play-out
    used-item-var 8 var? if
        8 message-param-room
        0 8 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 9 var? if
        9 message-param-room
        0 9 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 11 var? if
        $B message-param-room
        0 $B item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 12 var? if
        $C message-param-room
        0 $C item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 16 var? if
        $10 message-param-room
        0 $10 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    then then then then then
    reach-back-anim play-out
    5 say   \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act05 ( -- )   \ 00438650
    self-wait-done
    82.73 -85.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-12-antimony-powder story-flag-set
            0 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-12.act06 ( -- )   \ 004386B0
    self-wait-done
    -69.5 -46.5 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    $28 wait-frames
    8 say   \ "There are books spread out on the table. They seem to pertain to complex formulas, but..."
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-12.phase5 ( -- )   \ 0047AE7C
;

\ ---- registered ----
old-mansion-2f-12 register-scripts   \ (its phases and actions, by their names)
