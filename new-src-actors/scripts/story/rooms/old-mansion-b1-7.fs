\ story/rooms/old-mansion-b1-7.fs - the event scripts of room old-mansion-b1-7 ($61; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-7
USING: engine common actors messages room-names story.words story.shared story-names floor-flags fiona.anims ;

\ one of the fish swimming its way (swim path k), on and on
: swim ( id k -- )
    {: id k :}
    begin
        id who>cs dup 0< 0= if
            dup char-here if  dup char-at vec@  dup char-actor body-yaw  k swim-step  place-char  else  drop  then
        else  drop  then
        yield
    again ;
\ room 0x61 (byte 3): 0 / 2 / 4 set up the paths of characters 0x14 (2 x 3 x 7 cells from (25,
\ 0, 40), 480 frames a stretch; with the glint Glint_vtable) / 0x15 (2 x 3 x 5 from (30, 0, 50),
\ 240) / 0x16 (the same box, 280); 1 / 3 / 5 move them a frame (returning 2: again next frame)
: old-mansion-b1-7.cmd00 ( b0 -- )   \ (the fish $14 / $15 / $16 in the pool: b0 0 / 2 / 4 their ways set up - the glint on
    \ the first; 1 / 3 / 5 one swims on)
    case
        0 of  0 2 3 7 480  25e 0e 40e  swim-path  $14 char-model dup 0< if  drop  else  effects:glint  then  endof
        2 of  1 2 3 5 240  30e 0e 50e  swim-path  endof
        4 of  2 2 3 5 280  30e 0e 50e  swim-path  endof
        1 of  $14 0 swim  endof
        3 of  $15 1 swim  endof
        5 of  $16 2 swim  endof
    endcase ;
\ room 0x61: the light shaft: byte 3 0 starts it (LightShaft, with its motes from (30, 0, 70)),
\ its slot in event variable 3; 1 its haze on; 2 off.
: old-mansion-b1-7.cmd01 ( b0 -- )   \ (the beam of light: b0 0 made - kept in variable 3; 1 its haze on, 2 off)
    case
        0 of  3 effects:light-shaft var-set  endof
        1 of  3 var @ dup alive? if  1 swap send effect-command  else  drop  then  endof
        2 of  3 var @ dup alive? if  2 swap send effect-command  else  drop  then  endof
    endcase ;

: old-mansion-b1-7.enter ( -- )   \ 004291C0
    0 1 track 0 action
    0 2 track 1 action
    0 3 track 2 action
    0 0 var-set
    1 20 var-set
    2 90 var-set
    omb1-7-medallion-found story-flag? omb1-7-medallion story-flag? not and if
        0 8.95 1.0 11.89 flicker-sprite
    then
    1 $2300 sound-volume
    0 old-mansion-b1-7.cmd01
    2 hole-floor floor-on
;

: old-mansion-b1-7.char-enter ( -- )   \ 00429210
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
    fiona-who self-is? if
        0 exit-taken? if
            1 map-page
        then
    then
;

: old-mansion-b1-7.phase1 ( -- )   \ 004292A0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who $B char-entered-area? if
        1 map-page
    then
    fiona-who $B char-left-area? if
        0 map-page
    then
    0 3 var? if
        0 game-mode? if
            2 var-dec
            2 0 var? if
                2 90 var-set
                1 20 var? if
                    1 21 var-set
                else 1 21 var? if
                    1 22 var-set
                else 1 22 var? if
                    1 20 var-set
                then then then
            then
            1 20 var? if
                hewie-who char-here? if
                    $14 hewie-who 100 chars-within? if
                        $14 0.0 hewie-look-char
                    then
                then
            else 1 21 var? if
                hewie-who char-here? if
                    $15 hewie-who 100 chars-within? if
                        $15 0.0 hewie-look-char
                    then
                then
            else 1 22 var? if
                hewie-who char-here? if
                    $16 hewie-who 100 chars-within? if
                        $16 0.0 hewie-look-char
                    then
                then
            then then then
        then
    then
    omb1-7-medallion-found story-flag? not if
        0 8.95 0.0 11.89 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var omb1-7-medallion-found var-set
                taken-flag-var omb1-7-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 8950 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 11890 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: old-mansion-b1-7.phase2 ( -- )   \ 004293E0
    fiona-who $A char-in-area? if
        4 unhurried offer-action
    then
    omb1-7-medallion-found story-flag? omb1-7-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then
;

: old-mansion-b1-7.phase3 ( -- )   \ 00429408
    2 camera-mode? if
        1 old-mansion-b1-7.cmd01
    else
        2 old-mansion-b1-7.cmd01
    then
;

: old-mansion-b1-7.phase5 ( -- )   \ 00429418
    3 action-end
    3 char-done
    4 action-end
    4 char-done
    5 action-end
    5 char-done
;

: old-mansion-b1-7.act00 ( -- )   \ 00429430
    $14 3 $FF char-load
    3 char-start
    $14 char-activate
    $14 $9000 1 0 char-anim-hold
    $14 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    0 old-mansion-b1-7.cmd00
    1 old-mansion-b1-7.cmd00
    self-idle-or-end
;

: old-mansion-b1-7.act01 ( -- )   \ 00429460
    $15 4 $FF char-load
    4 char-start
    $15 char-activate
    $15 $9000 1 0 char-anim-hold
    $15 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    2 old-mansion-b1-7.cmd00
    3 old-mansion-b1-7.cmd00
    self-idle-or-end
;

: old-mansion-b1-7.act02 ( -- )   \ 00429490
    $16 5 $FF char-load
    5 char-start
    self-frames-reset
    $A self-wait-frames
    $16 char-activate
    $16 $9000 1 0 char-anim-hold
    $16 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    4 old-mansion-b1-7.cmd00
    5 old-mansion-b1-7.cmd00
    self-idle-or-end
;

: old-mansion-b1-7.act03 ( -- )   \ 004294C0
    self-wait-done
    8.95 11.89 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            omb1-7-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-b1-7.act04 ( -- )   \ 00429520
    self-wait-done
    rest-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    0 message   \ "An aquarium? Schools of strange fish glide through the water."
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

\ ---- registered ----
old-mansion-b1-7 register-scripts   \ (its phases and actions, by their names)
