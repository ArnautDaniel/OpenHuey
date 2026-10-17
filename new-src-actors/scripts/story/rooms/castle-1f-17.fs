\ story/rooms/castle-1f-17.fs - the event scripts of room castle-1f-17 ($1B; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-17
USING: room-names story.words story.shared story-names fiona.anims ;

\ five pendulums (Room1B_ObjectNames[byte 3]) of their own periods and swings: byte 4 0 still at
\ a phase offset (+0x34) 60 x the index, 1 swinging on (+0x30, +0x14)
: castle-1f-17.cmd00 ( b0 b1 -- )   \ (pendulum b0 - the room's string b0 - by b1: 0 still at its phase offset (+$34: 60 x b0);
    \ 1 a frame on: its period (frames) and swing (degrees) its own)
    {: k op | a n :}  k room-object to n to a  a n object-here? 0= if  exit  then
    k case  0 of  180e 5e  endof  1 of  210e 5e  endof  3 of  180e 10e  endof
            4 of  240e 15e  endof  >r 360e 15e r>  endcase                ( F: period swing )
    op 0= if  fdrop fdrop  0e a n $30 object!  60e k s>f f* a n $34 object!  exit  then
    op 1 <> if  fdrop fdrop exit  then
    fswap 360e fswap f/ a n $30 object+!                               ( F: swing )
    a n $30 object@ a n $34 object@ f+ 360e f< 0= if  -360e a n $30 object+!  then
    a n $30 object@ a n $34 object@ f+ deg>rad fsin f* deg>rad
    fdup pi f> if  2e pi f* f-  then  a n turn-y object! ;

: castle-1f-17.enter ( -- )   \ 003FC6A0
    $14 1.0 0 bgm
    c1f-17-lavender story-flag? not if
        0 -6.0 8.5 -5.0 flicker-sprite
    then
    0 0 castle-1f-17.cmd00
    1 0 castle-1f-17.cmd00
    3 0 castle-1f-17.cmd00
    4 0 castle-1f-17.cmd00
    0 1 castle-1f-17.cmd00
    1 1 castle-1f-17.cmd00
    3 1 castle-1f-17.cmd00
    4 1 castle-1f-17.cmd00
    0 1 track 6 action
;

: castle-1f-17.char-enter ( -- )   \ 003FC6F0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 -1 player-camera
    then
    1 0 -1 area-camera
;

: castle-1f-17.phase1 ( -- )   \ 003FC770
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    1 20.5 0.0 49.99 $14 17 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 17.0 hewie-look-zone
            then
        then
    then
    2 20.64 0.0 11.01 9 10 0 zone
    2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    2 ebit-set
                    $1E chance? if
                        hewie-zone-var 2 var-set
                        0 hewie-who $89 action
                    then
                then
            then
        then
    then
    3 -22.87 0.0 38.05 $14 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    4 28.49 0.0 -29.82 $14 13 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 13.0 hewie-look-zone
            then
        then
    then
    5 1.39 0.0 -30.06 $14 13 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                hewie-zone-var 5 var-set
                hewie-zone-var 13.0 hewie-look-zone
            then
        then
    then
    6 -29.89 0.0 -30.85 $14 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 6 9 char-zone-bits? if
                hewie-zone-var 6 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    7 -29.03 0.0 -7.64 $F 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 7 9 char-zone-bits? if
                hewie-zone-var 7 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
;

: castle-1f-17.phase2 ( -- )   \ 003FC8F0
    -2147483646 scene-request? if
        0 always offer-action
    then
    fiona-who 6 char-in-area? if
        1 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 0 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who 90 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 90 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 45 $32 char-heading? and if
        3 unhurried offer-action
    then
    c1f-17-lavender story-flag? not if
        0 0 8 4 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    fiona-who $C $32 char-faces-area? if
        4 unhurried offer-action
    then
;

: castle-1f-17.act00 ( -- )   \ 003FC970
    self-wait-done
    stalker-who self-touching? not if
        $19 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $19 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                $14 wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $19 door-unlock
                    $8008 message   \ "You unlocked the door."
                    20 self-idle-anim
                    $14 wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: castle-1f-17.act01 ( -- )   \ 003FC9B0
    self-wait-done
    look-low-anim $A self-anim-blend
    0 say   \ "Is this real? Poor little thing. Being stepped and walked on all day..."
    self-wait-anim
    self-idle-or-end
;

: castle-1f-17.act02 ( -- )   \ 003FC9C0
    self-wait-done
    0 self-turn-angle
    self-wait-done
    1 40.0 20.0 0.0 0.0 event-camera
    look-at-anim $A self-anim-blend
    1 say   \ "It's an anatomically correct model of the human body. It looks so real. Almost as if it..."
    self-wait-anim
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-17.act03 ( -- )   \ 003FCA00
    self-wait-done
    0 ebit? not if
        2 say   \ "There are miniature insect models here, too."
        0 ebit-set
    else
        3 say   \ "What type of a person would decorate a room with these things?"
    then
    self-idle-or-end
;

: castle-1f-17.act04 ( -- )   \ 003FCA20
    self-wait-done
    1 ebit? not if
        4 say   \ "Looks like an ordinary pool table."
        rest-anim play-out
        -1 self-idle-anim
        6 message   \ "Why would someone put something like this in here?"
        7 wait-frames
        wait-message
        1 ebit-set
    else
        5 say   \ "So maybe whoever lives here likes to play pool while looking at all of these gross..."
        1 ebit-clear
    then
    self-idle-or-end
;

: castle-1f-17.act05 ( -- )   \ 003FCA50
    self-wait-done
    -6.0 -5.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-17-lavender story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: castle-1f-17.act06 ( -- )   \ 003FCAB0
    begin
        yield
        0 1 castle-1f-17.cmd00
        1 1 castle-1f-17.cmd00
        3 1 castle-1f-17.cmd00
        4 1 castle-1f-17.cmd00
    again
;

\ ---- registered ----
castle-1f-17 register-scripts   \ (its phases and actions, by their names)
