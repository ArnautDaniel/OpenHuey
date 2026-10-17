\ story/rooms/water-tower-2f-1.fs - the event scripts of room water-tower-2f-1 ($C2; Water Tower: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-2f-1
USING: engine actors messages room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ byte 3 0 / 1: shut / open (pi/2); 2 / 3 opening / shutting by script variable 0 (0..120
\ frames, eased by a sine)
: water-tower-2f-1.cmd00 ( b0 -- )   \ (the hatch "move00" turned about y by event variable 0 - 0..120 frames eased by
    \ a sine: b0 0 / 1 shut / open (90 degrees), 2 opening, 3 shutting; its sound 2 where its edge is)
    {: b0 :}  s" move00" object-here? 0= if  exit  then
    b0 0= if  0e s" move00" turn-y object!  exit  then
    b0 1 = if  pi f2/ s" move00" turn-y object!  exit  then
    b0 2 = if  45e var0-ease f*  else  b0 3 = if  90e  45e var0-ease f*  f-  else  exit  then  then
    deg>rad fdup s" move00" turn-y object!
    $80000002 6 0 0  fdup fcos -45e f*  152e  frot fsin 45e f*  sound-at ;
\ byte 3 to the player's Character_ChooseExit while she's active
: water-tower-2f-1.cmd01 ( b0 -- )  0 swap char-to-exit ;   \ (Fiona to that exit: Character_ChooseExit)
\ room 0xC2: the summoner's countdown (progress +0x764) has run out.
: water-tower-2f-1.cond00? ( -- flag )  summoner-done? ;

: water-tower-2f-1.enter ( -- )   \ 0043F0C0
    room-sounds
    0 exit-taken? if
        wt2f-1-flag3 story-flag-clear
    else 1 exit-taken? if
        wt2f-1-flag3 story-flag-set
    then then
    2 part-hidden
    3 part-hidden
    0 0 object-hidden
    1 1 object-hidden
    2 1 object-hidden
    3 1 object-hidden
    2 barred-floor floor-off
    wt2f-1-flag3 story-flag? not if
        0 water-tower-2f-1.cmd00
        0 part-shown
        1 part-hidden
        0 barred-floor hole-floor or floor-on
        1 barred-floor hole-floor or floor-off
    else
        1 water-tower-2f-1.cmd00
        0 part-hidden
        1 part-shown
        1 barred-floor hole-floor or floor-on
        0 barred-floor hole-floor or floor-off
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
    wt2f-1-medallion-found story-flag? wt2f-1-medallion story-flag? not and if
        0 4.12 105.0 -18.51 flicker-sprite
    then
    wt2f-1-refined-antimony story-flag? not if
        1 -19.9 153.0 74.61 flicker-sprite
    then
    wt2f-1-breakable-broken story-flag? not if
        4 part-shown
        5 part-hidden
    else
        4 part-hidden
        5 part-shown
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    1 $2300 sound-volume
;

: water-tower-2f-1.char-enter ( -- )   \ 0043F1A0
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
            4 4 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        4 4 player-camera
    then then
    1 4 4 area-camera
    fiona-who self-is? if
        1 exit-taken? if
            4 map-page
        then
    then
;

: water-tower-2f-1.phase1 ( -- )   \ 0043F230
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
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    hewie-who $A char-entered-area? if
        1 hewie-side
    then
    hewie-who $A char-left-area? if
        0 hewie-side
    then
    fiona-who $F char-entered-area? if
        2 map-page
    then
    fiona-who $F char-left-area? fiona-who $10 char-entered-area? or if
        3 map-page
    then
    fiona-who $10 char-left-area? if
        4 map-page
    then
    wt2f-1-flag2 story-flag? c7-second-event story-flag? not and if
        4 stalker-kind? $17 stalker-kind? or stalker-who char-here? not and if
            wt2f-1-flag1 story-flag? not if
                0 ebit? not if
                    fiona-who $D char-left-area? if
                        0 ebit-set
                        0 $F2 1 action
                    then
                then
                fiona-who 5 char-entered-area? if
                    $F2 action-end
                then
                fiona-who $E char-entered-area? if
                    $FE $C2 825 1 stalker-to-room
                    stalker-who 0 stalker-command
                    summoner-own-cooldown
                    wt2f-1-flag1 story-flag-set
                then
                fiona-who 7 char-entered-area? if
                    $FE $C2 816 1 stalker-to-room
                    stalker-who 0 stalker-command
                    summoner-own-cooldown
                    wt2f-1-flag1 story-flag-set
                then
            else 5 ebit? not if
                1 1800 var-set
                stalker-active? not if
                    5 ebit-set
                then
            else stalker-active? if
                5 ebit-clear
            else 1 0 var? not if
                1 var-dec
            else 1 ebit? not stalkers-stay state-flag? not and if
                wt2f-1-flag3 story-flag? not if
                    $FE $C2 473 1 stalker-to-room
                    stalker-who 0 stalker-command
                    summoner-own-cooldown
                    5 ebit-clear
                else
                    $FE $C2 888 0 stalker-to-room
                    stalker-who 0 stalker-command
                    summoner-own-cooldown
                    5 ebit-clear
                then
            then then then then then
        then
    then
    4 stalker-kind? $17 stalker-kind? or if
        stalker-who char-busy? not stalker-active? and if
            water-tower-2f-1.cond00? if
                stalker-who char-unseen? if
                    stalker-who action-end
                    1 summon-take
                then
            then
        then
    then
    wt2f-1-medallion-found story-flag? not if
        0 4.12 104.0 -18.51 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 wt2f-1-medallion-found var-set
                $1A wt2f-1-medallion var-set
                $1B 0 var-set
                $1C 4120 var-set
                $1D 105000 var-set
                $1E -18510 var-set
                $1F 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    wt2f-1-breakable-broken story-flag? not if
        3 -16.5 152.0 -47.5 5 8 1 zone
        self-who 3 char-in-zone? if
            wt2f-1-breakable-broken story-flag-set
            4 part-hidden
            5 part-shown
            -16.5 152.0 -47.5 0 -2145378272 0 0.0 shards-at
            $88 5 -16.5 152.0 -47.5 0 0 sound
            $40 $2B9 noise
            $C2 1 291 $24 5 6 0 0.0 creature-place
        then
    then
    wt2f-1-flag4 story-flag? not if
        2 1.97 152.0 46.65 $26 10 0 zone
        6 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 2 9 char-zone-bits? if
                        6 ebit-set
                        $64 chance? if
                            $1F 2 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
;

: water-tower-2f-1.phase2 ( -- )   \ 0043F480
    fiona-who $C char-in-area? fiona-who -45 $32 char-heading? and if
        1 ebit? not if
            0 unhurried offer-action
        then
    then
    wt2f-1-medallion-found story-flag? wt2f-1-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    wt2f-1-refined-antimony story-flag? not if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then
;

: water-tower-2f-1.phase5 ( -- )   \ 0043F4D0
    hewie-who char-here? if
        hewie-who 2 char-in-nav-group? if
            wt2f-1-flag3 story-flag? not if
                $C2 1 874 hewie-to-room
            else
                $C2 0 887 hewie-to-room
            then
        then
    then
    no-stalker-camera state-flag-clear
    4 ebit? if
        summoner-on state-flag-set
    then
;

: water-tower-2f-1.act00 ( -- )   \ 0043F500
    stalkers-stay state-flag-set
    1 self-scripted
    1 ebit-set
    3 ebit-clear
    wt2f-1-flag3 story-flag? not if
        2 ebit-clear
        1 keeps-fiona-off keeps-pursuers-off or hole-floor or floor-on
        hewie-who char-here? hewie-who 1 char-in-nav-group? and if
            0 hewie-who 2 action-force
        else
            1 keeps-hewie-off floor-on
            3 ebit-set
        then
    else
        2 ebit-set
        0 keeps-fiona-off keeps-pursuers-off or hole-floor or floor-on
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            0 hewie-who 2 action-force
        else
            0 keeps-hewie-off floor-on
            3 ebit-set
        then
    then
    self-wait-done
    things-clear
    fiona-who 4 char-file-load
    $13A -0.47 39.8 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    wt2f-1-flag3 story-flag? not if
        1 water-tower-2f-1.cmd01
        4 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 part-hidden
        1 part-hidden
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
        wt2f-1-flag3 story-flag-set
        2 1 object-hidden
        3 1 object-hidden
        0 part-hidden
        1 part-shown
        wt2f-1-flag4 story-flag-set
    else
        0 water-tower-2f-1.cmd01
        3 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 part-hidden
        1 part-hidden
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
        wt2f-1-flag3 story-flag-clear
        2 1 object-hidden
        3 1 object-hidden
        0 part-shown
        1 part-hidden
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: water-tower-2f-1.act01 ( -- )   \ 0043F620
    $27 7 54.0 112.0 -56.0 0 0 sound
    self-frames-reset
    $3C self-wait-frames
    begin
        4 stalker-kind-here? not $17 stalker-kind-here? not and if
            0 7 54.0 112.0 -56.0 0 0 sound
        then
        self-frames-reset
        $1E self-wait-frames
        4 stalker-kind-here? not $17 stalker-kind-here? not and if
            1 7 54.0 112.0 -56.0 0 0 sound
        then
        self-frames-reset
        $1E self-wait-frames
    again
;

: water-tower-2f-1.act02 ( -- )   \ 0043F680
    1 self-scripted
    self-wait-done
    2 ebit? not if
        $3B6 -22.18 0.43 120 $FFFF $A self-move-to
        self-wait-done
    else
        $3C3 2.56 19.55 -160 $FFFF $A self-move-to
        self-wait-done
    then
    0 self-scripted
    begin
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: water-tower-2f-1.act03 ( -- )   \ 0043F6C0
    0 0 var-set
    2 ebit? not if
        2 6 -45.0 152.0 0.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            2 water-tower-2f-1.cmd00
            3 ebit? not if
                hewie-who 1 char-in-nav-group? not if
                    1 keeps-hewie-off floor-on
                    3 ebit-set
                then
            then
            yield
        repeat
        hewie-who char-here? hewie-who 1 char-in-nav-group? and if
            hewie-who action-end
            hewie-who $3B6 char-to-tri
            1 hewie-side
        then
        1 barred-floor hole-floor or floor-on
        0 barred-floor hole-floor or floor-off
        0 keeps-hewie-off floor-off
        0 keeps-fiona-off keeps-pursuers-off or floor-off
        1 keeps-fiona-off keeps-pursuers-off or floor-off
        1 water-tower-2f-1.cmd00
        1 water-tower-2f-1.cmd01
    else
        2 6 0.0 152.0 45.0 0 0 sound
        begin
            0 120 var? not while
            0 var-inc
            3 water-tower-2f-1.cmd00
            3 ebit? not if
                hewie-who 1 char-in-nav-group? not if
                    0 keeps-hewie-off floor-on
                    3 ebit-set
                then
            then
            yield
        repeat
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            hewie-who action-end
            hewie-who $3BF char-to-tri
            1 hewie-side
        then
        0 barred-floor hole-floor or floor-on
        1 barred-floor hole-floor or floor-off
        1 keeps-hewie-off floor-off
        0 keeps-fiona-off keeps-pursuers-off or floor-off
        1 keeps-fiona-off keeps-pursuers-off or floor-off
        0 water-tower-2f-1.cmd00
        0 water-tower-2f-1.cmd01
    then
    1 ebit-clear
    self-idle-or-end
;

: water-tower-2f-1.act04 ( -- )   \ 0043F7C0
    self-wait-done
    4.12 -18.51 self-turn-to-xz
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
            wt2f-1-medallion story-flag-set
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

: water-tower-2f-1.act05 ( -- )   \ 0043F820
    self-wait-done
    -19.9 74.61 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            wt2f-1-refined-antimony story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
water-tower-2f-1 register-scripts   \ (its phases and actions, by their names)
