\ story/rooms/water-tower-4f-1.fs - the event scripts of room water-tower-4f-1 ($C3; Water Tower: 4F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-4f-1
USING: engine actors messages room-names story.words story.shared flag-names ;

\ (as RoomC2_Cmd00, the room object named RoomC3_ObjectNames, opening to -pi/2)
: water-tower-4f-1.cmd00 ( b0 -- )   \ (as water-tower-2f-1's, opening the other way: -90 degrees)
    {: b0 :}  s" move00" object-here? 0= if  exit  then
    b0 0= if  0e s" move00" turn-y object!  exit  then
    b0 1 = if  pi f2/ fnegate s" move00" turn-y object!  exit  then
    b0 2 = if  -45e var0-ease f*  else  b0 3 = if  -90e  45e var0-ease f*  f+  else  exit  then  then
    deg>rad fdup s" move00" turn-y object!
    $80000002 6 0 0  fdup fsin 45e f*  142e  frot fcos 45e f*  sound-at ;
\ the player (active, +0xE0 clear) put in action 0xB / 0x21 / 0xFF unless held (7); then
\ progress +0x7B8 gets 50
: water-tower-4f-1.cmd01 ( -- )  fiona-id send flinch  50 threat-raise ;
\ sound 3 (bank 6) at the room object named pstr_doramukan_2[0]
: water-tower-4f-1.cmd02 ( -- )   \ (sound 3 at "doramukan")
    s" doramukan" object-here? 0= if  exit  then
    3 6 0 0  s" doramukan" place-x object@  s" doramukan" place-y object@  s" doramukan" place-z object@  sound ;
\ room 0xC3: the summoner's countdown (progress +0x764) has run out.
: water-tower-4f-1.cond00? ( -- flag )  summoner-done? ;

: water-tower-4f-1.enter ( -- )   \ 0043F8F0
    room-sounds
    0 exit-taken? if
        $342 story-flag? not if
            $FF door-lock
            $101 door-lock
        else
            $FF door-unlock
            $101 door-unlock
        then
    else 1 exit-taken? if
        $342 story-flag-set
        $FF door-unlock
        $101 door-unlock
    then then
    2 0 $14 door-bits
    3 0 $14 door-bits
    0 0 object-hidden
    1 1 object-hidden
    2 1 object-hidden
    3 1 object-hidden
    $342 story-flag? not if
        0 water-tower-4f-1.cmd00
        0 1 $14 door-bits
        1 0 $14 door-bits
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
    else
        1 water-tower-4f-1.cmd00
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 1 $10020000 nav-group
        0 0 $10020000 nav-group
    then
    $33A story-flag? if
        4 1 object-hidden
    else
        4 0 object-hidden
    then
    $17 stalker-kind? if
        no-stalker-camera state-flag-set
    then
    summoner-on state-flag? if
        4 ebit-set
        summoner-on state-flag-clear
    then
    stalker-who action-end
    1 summon-take
    $2DF story-flag? not if
        0 -71.41 143.0 -7.67 flicker-sprite
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    $2F8 story-flag? not if
        5 1 $14 door-bits
        6 0 $14 door-bits
    else
        5 0 $14 door-bits
        6 1 $14 door-bits
        $2F9 story-flag? not if
            1 -86.45 47.0 -8.0 flicker-sprite
        then
    then
    1 $2300 sound-volume
;

: water-tower-4f-1.char-enter ( -- )   \ 0043F9F0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 3 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 3 player-camera
    then then
    0 0 3 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            3 2 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        3 2 player-camera
    then then
    1 3 2 area-camera
    fiona-who self-is? if
        1 exit-taken? if
            7 map-page
        then
    then
;

: water-tower-4f-1.phase1 ( -- )   \ 0043FA80
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 0 3 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 0 3 1 chars-area-camera
    7 2 1 1 chars-area-camera
    8 0 3 1 chars-area-camera
    9 2 1 1 chars-area-camera
    $A 2 1 1 chars-area-camera
    $B 3 2 1 chars-area-camera
    $14 1 0 1 chars-area-camera
    $13 6 5 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who $15 char-entered-area? if
        4 map-page
    then
    fiona-who $15 char-left-area? fiona-who $16 char-entered-area? or if
        5 map-page
    then
    fiona-who $16 char-left-area? fiona-who $17 char-entered-area? or if
        6 map-page
    then
    fiona-who $17 char-left-area? if
        7 map-page
    then
    fiona-who $10 char-entered-area? fiona-who $11 char-entered-area? or fiona-who $12 char-entered-area? or if
        fiona-who 0 3 char-camera
    then
    $8D story-flag? $8E story-flag? not and if
        4 stalker-kind? $17 stalker-kind? or stalker-who char-here? not and if
            $33A story-flag? not if
                fiona-who $F char-entered-area? if
                    fiona-who 5 4 char-camera
                then
                fiona-who $D char-entered-area? fiona-who $E char-entered-area? or if
                    $33A story-flag-set
                    0 $F1 1 action
                then
            else $8C story-flag? if
                5 ebit? not if
                    1 1800 var-set
                    stalker-active? not if
                        5 ebit-set
                    then
                else stalker-active? if
                    5 ebit-clear
                else 1 0 var? not if
                    1 var-dec
                else 1 ebit? not stalkers-stay state-flag? not and if
                    $342 story-flag? not if
                        $FE $C3 249 2 stalker-to-room
                        stalker-who 0 stalker-command
                        summoner-own-cooldown
                        5 ebit-clear
                    else
                        $FE $C3 944 2 stalker-to-room
                        stalker-who 0 stalker-command
                        summoner-own-cooldown
                        5 ebit-clear
                    then
                then then then then
            then then
        then
    then
    4 stalker-kind? $17 stalker-kind? or if
        stalker-who char-busy? not stalker-active? and if
            water-tower-4f-1.cond00? if
                stalker-who char-unseen? if
                    stalker-who action-end
                    1 summon-take
                then
            then
        then
    then
    $34D story-flag? not if
        1 -47.23 142.0 1.87 $26 10 0 zone
        6 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        6 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
;

: water-tower-4f-1.phase2 ( -- )   \ 0043FC10
    fiona-who $C char-in-area? fiona-who 90 $32 char-heading? and if
        1 ebit? not if
            5 0 0 scene-change
        then
    then
    $2DF story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $2F8 story-flag? not if
        2 -86.45 46.0 -8.0 5 8 1 zone
        self-who 2 char-in-zone? if
            $2F8 story-flag-set
            5 0 $14 door-bits
            6 1 $14 door-bits
            -86.45 46.0 -8.0 0 -2145378272 0 0.0 shards-at
            $88 5 -86.45 46.0 -8.0 0 0 sound
            $40 $131 noise
            1 -86.45 47.0 -8.0 flicker-sprite
        then
    else $2F9 story-flag? not if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
;

: water-tower-4f-1.phase5 ( -- )   \ 0043FCC0
    no-stalker-camera state-flag-clear
    4 ebit? if
        summoner-on state-flag-set
    then
;

: water-tower-4f-1.act00 ( -- )   \ 0043FCD0
    stalkers-stay state-flag-set
    1 self-scripted
    1 ebit-set
    3 ebit-clear
    $342 story-flag? not if
        2 ebit-clear
        1 1 $10000030 nav-group
        hewie-who char-here? hewie-who 1 char-in-nav-group? and if
            0 hewie-who 2 action-force
        else
            1 1 $1000000 nav-group
            3 ebit-set
        then
    else
        2 ebit-set
        1 0 $10000030 nav-group
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            0 hewie-who 2 action-force
        else
            1 0 $1000000 nav-group
            3 ebit-set
        then
    then
    self-wait-done
    things-clear
    fiona-who 5 char-file-load
    $166 73.23 -0.48 180 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    $342 story-flag? not if
        $8004 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        1 0 $14 door-bits
        2 1 object-hidden
        3 0 object-hidden
        3 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        fiona-who 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        fiona-who 0 6 char-sound
        self-wait-anim
        0 $F3 3 action
        $342 story-flag-set
        2 1 object-hidden
        3 1 object-hidden
        0 0 $14 door-bits
        1 1 $14 door-bits
        $C3 0 1055 $A 4 -1 0 0.0 creature-place
        $C3 0 1053 $A 6 -1 0 0.0 creature-place
        $C3 0 618 $A 7 -1 0 0.0 creature-place
        $C3 0 1051 $A 7 -1 0 0.0 creature-place
        $C3 0 607 $A 4 -1 0 0.0 creature-place
        $C3 0 615 $A 6 -1 0 0.0 creature-place
        $34D story-flag-set
    else
        $8003 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        1 0 $14 door-bits
        2 0 object-hidden
        3 1 object-hidden
        2 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        fiona-who 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        fiona-who 0 6 char-sound
        self-wait-anim
        0 $F3 3 action
        $342 story-flag-clear
        2 1 object-hidden
        3 1 object-hidden
        0 1 $14 door-bits
        1 0 $14 door-bits
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: water-tower-4f-1.act01 ( -- )   \ 0043FE40
    4 0 object-anim
    self-frames-reset
    $11 self-wait-frames
    1 $FF 8 rumble
    water-tower-4f-1.cmd01
    water-tower-4f-1.cmd02
    self-frames-reset
    $3C self-wait-frames
    $8C story-flag? not if
        $13 7 54.0 112.0 -56.0 0 0 sound
        $FE $C3 830 2 stalker-to-room
        stalker-who 0 stalker-command
        summoner-own-cooldown
        $8C story-flag-set
    then
    self-idle-or-end
;

: water-tower-4f-1.act02 ( -- )   \ 0043FE80
    1 self-scripted
    self-wait-done
    2 ebit? not if
        $260 9.39 -18.29 180 $FFFF $A self-move-to
        self-wait-done
    else
        $260 9.39 -18.29 180 $FFFF $A self-move-to
        self-wait-done
    then
    0 self-scripted
    begin
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: water-tower-4f-1.act03 ( -- )   \ 0043FEC0
    0 0 var-set
    2 ebit? not if
        2 6 0.0 142.0 45.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            2 water-tower-4f-1.cmd00
            3 ebit? not if
                hewie-who 1 char-in-nav-group? not if
                    1 1 $1000000 nav-group
                    3 ebit-set
                then
            then
            yield
        repeat
        hewie-who char-here? hewie-who 1 char-in-nav-group? and if
            hewie-who action-end
            hewie-who $260 char-to-tri
        then
        1 1 $10020000 nav-group
        0 0 $10020000 nav-group
        0 0 $1000000 nav-group
        0 0 $30 nav-group
        0 1 $30 nav-group
        1 water-tower-4f-1.cmd00
    else
        2 6 -45.0 142.0 0.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            3 water-tower-4f-1.cmd00
            3 ebit? not if
                hewie-who 1 char-in-nav-group? not if
                    1 0 $1000000 nav-group
                    3 ebit-set
                then
            then
            yield
        repeat
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            hewie-who action-end
            hewie-who $260 char-to-tri
        then
        1 0 $10020000 nav-group
        0 1 $10020000 nav-group
        0 1 $1000000 nav-group
        0 0 $30 nav-group
        0 1 $30 nav-group
        0 water-tower-4f-1.cmd00
    then
    $342 story-flag? not if
        $FF door-lock
        $101 door-lock
    else
        $FF door-unlock
        $101 door-unlock
    then
    1 ebit-clear
    self-idle-or-end
;

: water-tower-4f-1.act04 ( -- )   \ 0043FFC0
    self-wait-done
    -71.41 -7.67 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $2DF story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: water-tower-4f-1.act05 ( -- )   \ 00440020
    self-wait-done
    -86.45 -8.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $2F9 story-flag-set
            1 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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
' water-tower-4f-1.enter water-tower-4f-1 0 room-script!
' water-tower-4f-1.char-enter water-tower-4f-1 6 room-script!
' water-tower-4f-1.phase1 water-tower-4f-1 1 room-script!
' water-tower-4f-1.phase2 water-tower-4f-1 2 room-script!
' water-tower-4f-1.phase5 water-tower-4f-1 5 room-script!
' water-tower-4f-1.act00 water-tower-4f-1 $00 action-script!
' water-tower-4f-1.act01 water-tower-4f-1 $01 action-script!
' water-tower-4f-1.act02 water-tower-4f-1 $02 action-script!
' water-tower-4f-1.act03 water-tower-4f-1 $03 action-script!
' water-tower-4f-1.act04 water-tower-4f-1 $04 action-script!
' water-tower-4f-1.act05 water-tower-4f-1 $05 action-script!
