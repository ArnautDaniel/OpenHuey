\ story/rooms/front-garden-2.fs - the event scripts of room front-garden-2 ($13; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-2
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ room 0x13: the grate ("kousi"): byte 3 0 shut (turn 0), else swung open a quarter turn.
: front-garden-2.cmd00 ( b0 -- )  if  pi f2/ fnegate  else  0e  then  s" kousi" turn-y object! ;

: front-garden-2.enter ( -- )   \ 003F91A0
    room-sounds
    c2f-3-flag1 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $10 152.0 92.2 -60.0 $A $80 $80 $80 $30 specks
        $E 128.0 92.2 -60.0 $C $80 $80 $80 $30 specks
        $10 152.0 56.7 12.9 $A $80 $80 $80 $30 specks
        $A 128.0 56.7 12.9 $A $80 $80 $80 $30 specks
    then
    fg-2-flag1 story-flag? not if
        0 front-garden-2.cmd00
        1 barred-floor floor-off
        0 barred-floor floor-on
    else
        1 front-garden-2.cmd00
        0 barred-floor floor-off
        1 barred-floor floor-on
    then
    saturnus-hint-on story-flag? not if
        6 part-shown
    else
        7 part-shown
    then
    c2f-3-flag1 story-flag? hewie-found-in-wire story-flag? not and if
        0 30 var-set
        0 1 track 6 action
    then
    fg-2-breakable1-broken story-flag? not if
        2 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        2 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    fg-2-breakable2-broken story-flag? not if
        3 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        3 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
        fg-2-medallion story-flag? not if
            0 130.0 61.0 -237.0 flicker-sprite
        then
    then
    fg-2-breakable3-broken story-flag? not if
        4 keeps-people-off floor-on
        4 part-shown
        5 part-hidden
    else
        4 keeps-people-off floor-off
        4 part-hidden
        5 part-shown
    then
    0 6 0.0 1.0 0.187 0.312 zone-rect
;

: front-garden-2.char-enter ( -- )   \ 003F92E0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 2 player-camera
    then
    0 2 2 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        3 -1 player-camera
    then
    2 3 -1 area-camera
    stalker-who self-is? if
    then
;

: front-garden-2.phase1 ( -- )   \ 003F93A0
    c2f-3-flag2 story-flag? not hewie-found-in-wire story-flag? and hewie-freed story-flag? not and if
    else 0 exit-usable? if
        0 exit-check
    then then
    1 exit-usable? if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 -1 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    hewie-found-in-wire story-flag? fg-2-flag1 story-flag? not and if
        fiona-who $F char-entered-area? fiona-who $F char-left-area? or stalker-who char-here? not and if
            fg-2-flag1 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 1 action-force
            else
                1 fiona-who 1 action-force
            then
        then
    then
    c2f-3-flag2 story-flag? not hewie-found-in-wire story-flag? and hewie-freed story-flag? not and if
        fiona-who 0 char-entered-area? 0 ebit? not and if
            0 ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 2 action-force
            else
                1 fiona-who 2 action-force
            then
        then
    then
    fg-2-breakable1-broken story-flag? not if
        0 131.0 60.0 -244.0 5 8 1 zone
        self-who 0 char-in-zone? if
            fg-2-breakable1-broken story-flag-set
            2 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            131.0 60.0 -244.0 0 -2144325584 0 0.0 shards-at
            $88 5 131.0 60.0 -244.0 0 0 sound
            $40 $1E4 noise
        then
    then
    fg-2-breakable3-broken story-flag? not if
        2 137.0 60.0 -240.0 5 8 1 zone
        self-who 2 char-in-zone? if
            fg-2-breakable3-broken story-flag-set
            4 keeps-people-off floor-off
            4 part-hidden
            5 part-shown
            137.0 60.0 -240.0 0 -2144325584 0 0.0 shards-at
            $88 5 137.0 60.0 -240.0 0 0 sound
            $40 $1E4 noise
        then
    then
;

: front-garden-2.phase2 ( -- )   \ 003F9530
    fg-2-flag1 story-flag? not fiona-who $E char-in-area? and fiona-who -45 $32 char-heading? and if
        0 always offer-action
    then
    fiona-who $10 char-in-area? fiona-who 90 $3C char-heading? and if
        3 unhurried offer-action
    then
    fiona-who $11 char-in-area? fiona-who -45 $3C char-heading? and if
        4 unhurried offer-action
    then
    fg-2-breakable2-broken story-flag? not if
        1 130.0 60.0 -237.0 5 8 1 zone
        self-who 1 char-in-zone? if
            fg-2-breakable2-broken story-flag-set
            3 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            130.0 60.0 -237.0 0 -2144325584 0 0.0 shards-at
            $88 5 130.0 60.0 -237.0 0 0 sound
            $40 $1E4 noise
            0 130.0 61.0 -237.0 flicker-sprite
        then
    else fg-2-medallion story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then then
;

: front-garden-2.phase3 ( -- )   \ 003F95F0
    151.9 66.3 -63.6 151.9 66.3 -43.5 151.9 44.1 -63.6 151.9 44.1 -43.8 lights-doorway
    148.0 97.0 -176.0 175.0 97.0 -176.0 148.0 97.0 -150.0 175.0 97.0 -150.0 lights-doorway
    109.0 124.0 -176.0 148.0 97.0 -176.0 109.0 124.0 -150.0 148.0 97.0 -150.0 lights-doorway
    148.0 101.0 -176.0 175.0 101.0 -176.0 148.0 97.0 -176.0 175.0 97.0 -176.0 lights-doorway
    109.0 128.0 -176.0 148.0 101.0 -176.0 109.0 124.0 -176.0 148.0 97.0 -176.0 lights-doorway
;

: front-garden-2.act00 ( -- )   \ 003F96E8
    self-wait-done
    hands-clasped-anim $A self-anim-blend
    0 say   \ "It is bolted shut from the other side. I'm not going to be able to open this..."
    self-wait-anim
    self-idle-or-end
;

: front-garden-2.act01 ( -- )   \ 003F9700
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    self-wait-done
    wait-fade
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    fiona-who $20A 133.34 -76.62 -35 char-to-xz
    0 self-idle-anim
    camera-restart
    fg-2-flag1 story-flag-set
    $B door-unlock
    1 front-garden-2.cmd00
    0 barred-floor floor-off
    1 barred-floor floor-on
    stalkers-stay state-flag-clear
    0 self-scripted
    $F $41 fade
    self-idle-or-end
;

: front-garden-2.act02 ( -- )   \ 003F97C0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    scene-locked state-flag-set
    0 exit-prepare
    stalkers-stay state-flag-clear
    0 self-scripted
    $81 exit-check
    0.0 sound-volume-scale
    self-idle-or-end
;

: front-garden-2.act03 ( -- )   \ 003F97F0
    self-wait-done
    70.0 -160.0 self-turn-to-xz
    self-wait-done
    1 say   \ "I wonder what kind of a room is on the other side? I can see the light flicker from can..."
    self-idle-or-end
;

: front-garden-2.act04 ( -- )   \ 003F9800
    self-wait-done
    100.0 -210.0 self-turn-to-xz
    self-wait-done
    1 say   \ "I wonder what kind of a room is on the other side? I can see the light flicker from can..."
    self-idle-or-end
;

: front-garden-2.act05 ( -- )   \ 003F9810
    self-wait-done
    130.0 -237.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            fg-2-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: front-garden-2.act06 ( -- )   \ 003F9870
    begin
        yield
        0 var-dec
        0 0 var? if
            $1E chance? if
                $F 6 228.0 5.0 78.0 0 0 sound
                0 180 var-set
            else
                0 60 var-set
            then
        then
    again
;

: front-garden-2.act07 ( -- )   \ 003F98B0
    self-wait-done
    $10 152.0 92.2 -60.0 $A $80 $80 $80 $30 specks
    $E 128.0 92.2 -60.0 $C $80 $80 $80 $30 specks
    $10 152.0 56.7 12.9 $A $80 $80 $80 $30 specks
    $A 128.0 56.7 12.9 $A $80 $80 $80 $30 specks
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
front-garden-2 register-scripts   \ (its phases and actions, by their names)
