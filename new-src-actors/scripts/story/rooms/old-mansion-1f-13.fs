\ story/rooms/old-mansion-1f-13.fs - the event scripts of room old-mansion-1f-13 ($52; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-13
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

: old-mansion-1f-13.cmd00 ( -- )  effects:drips ;   \ (water dripping, rippling)
\ 1 unless the object at +0x18 exists and its byte +0x28 is 1.
: old-mansion-1f-13.cond00? ( -- flag )  6 creature-up? 0= ;

: old-mansion-1f-13.enter ( -- )   \ 0040E3B0
    room-sounds
    $35 1.0 0 bgm
    om2f-8-flag1 story-flag? not if
        old-mansion-1f-13.cond00? if
            $52 1 323 2 5 6 0 0.0 creature-place
        then
    then
    old-mansion-1f-13.cmd00
    om1f-13-flag2 story-flag? not if
        2 0 object-hidden
        3 0 object-hidden
        4 0 object-hidden
        5 0 object-hidden
        6 0 object-hidden
        7 0 object-hidden
        8 0 object-hidden
        9 0 object-hidden
        0 part-hidden
    else
        2 1 object-hidden
        3 1 object-hidden
        4 1 object-hidden
        5 1 object-hidden
        6 1 object-hidden
        7 1 object-hidden
        8 1 object-hidden
        9 1 object-hidden
        0 part-shown
    then
    $10 -54.6 28.6 -20.0 $E $80 $80 $80 $40 specks
    $10 54.6 29.0 -20.0 $E $80 $80 $80 $40 specks
    $10 -7.5 5.0 53.5 $E $80 $80 $80 $40 specks
    $10 0.0 64.8 -20.5 $E $80 $80 $80 $40 specks
    0 hole-floor floor-on
;

: old-mansion-1f-13.char-enter ( -- )   \ 0040E480
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
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    fiona-who self-is? if
        1 exit-taken? 2 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-13.phase1 ( -- )   \ 0040E550
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        2 exit-prepare
    then
    om1f-13-flag2 story-flag? not if
        fiona-who 5 char-left-area? if
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 1 action-force
            else
                1 fiona-who 1 action-force
            then
        then
    then
    0 -48.85 0.0 -48.91 $21 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    1 48.42 -6.0 28.0 $1A 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    2 2.46 -6.0 33.29 $16 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    3 -22.19 -6.0 36.99 $14 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    4 26.75 0.0 -36.99 $E 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-13.phase2 ( -- )   \ 0040E680
    fiona-who 6 $3C char-faces-area? if
        2 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 90 $3C char-heading? and if
        3 unhurried offer-action
    then
    white-mandragora-used story-flag? not if
        $18 1 item-count? not $19 1 item-count? not and $1A 1 item-count? not and $1B 1 item-count? not and $1C 1 item-count? not and if
            fiona-who $A $3C char-faces-area? if
                4 unhurried offer-action
            then
            fiona-who 9 $3C char-faces-area? if
                5 unhurried offer-action
            then
            fiona-who 8 $3C char-faces-area? if
                6 unhurried offer-action
            then
            fiona-who $B $3C char-faces-area? if
                7 unhurried offer-action
            then
            fiona-who 7 char-in-area? fiona-who 90 $3C char-heading? and if
                8 unhurried offer-action
            then
        else
            fiona-who $A $3C char-faces-area? if
                0 unhurried offer-action
            then
            fiona-who 9 $3C char-faces-area? if
                0 unhurried offer-action
            then
            fiona-who 8 $3C char-faces-area? if
                0 unhurried offer-action
            then
            fiona-who $B $3C char-faces-area? if
                0 unhurried offer-action
            then
            fiona-who 7 char-in-area? fiona-who 90 $3C char-heading? and if
                0 unhurried offer-action
            then
        then
    else
        fiona-who $A $3C char-faces-area? if
            9 unhurried offer-action
        then
        fiona-who 9 $3C char-faces-area? if
            9 unhurried offer-action
        then
        fiona-who 8 $3C char-faces-area? if
            9 unhurried offer-action
        then
        fiona-who $B $3C char-faces-area? if
            9 unhurried offer-action
        then
        fiona-who 7 char-in-area? fiona-who 90 $3C char-heading? and if
            9 unhurried offer-action
        then
    then
    fiona-who $D char-in-area? fiona-who -90 $3C char-heading? and if
        $D unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who -90 $3C char-heading? and if
        $D unhurried offer-action
    then
;

: old-mansion-1f-13.act00 ( -- )   \ 0047ABA0
    self-wait-done
    4 message   \ "You can't hold any more. You must first do something with the one you are carrying."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-13.act01 ( -- )   \ 0040E780
    om1f-13-flag2 story-flag-set
    stalker-who action-end
    stalker-who char-done
    summoner-on state-flag-clear
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    0 0 door-flag-unread
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    hewie-who char-here? 0 hewie-side? and if
        0 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        0 ebit-set
    then
    fiona-who 1 char-no-shadow
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    2 1 object-hidden
    3 1 object-hidden
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    0 part-shown
    0 ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $52 0 182 hewie-to-room
            hewie-who $B6 35.67 -1.81 99 char-to-xz
        then
    then
    fiona-who $1C 54.36 -8.26 -43 char-to-xz
    0 0 player-camera
    yield
    camera-restart
    om1f-13-flag1 story-flag? not if
        om1f-13-flag1 story-flag-set
    then
    0 1 door-flag-unread
    $F $51 fade
    $35 1.0 0 bgm
    wait-fade
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    stalker-who $6C 0 door-lock-for
    self-idle-or-end
;

: old-mansion-1f-13.act02 ( -- )   \ 0047ABA8
    self-wait-done
    0 message   \ "It's totally dried-out. I guess it can't hurt me now... I'll never look at a plant the..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-13.act03 ( -- )   \ 0040E8D0
    1 self-scripted
    stalkers-stay state-flag-set
    self-wait-done
    1 message   \ "Is this an info sheet on the plants here? There is something written here..."
    wait-message
    $F 6 fade
    wait-fade
    2 message   \ "In order to procure the large quantity of blood required to complete the grand process,..."
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act0A ( -- )   \ 0040EAD0
    om1f-13-flag2 story-flag? white-mandragora-used story-flag? not and if
        stalker-who $48 1 room-doors-state
        stalker-active? not if
            $FE $4B 217 2 stalker-to-room
            stalker-who 2 stalker-command
        then
        summoner-own-cooldown
    then
    exit
;

: old-mansion-1f-13.act04 ( -- )   \ 0040E8F0
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    self-wait-done
    3 message   \ "Full-bodied flowers sprout from the bed. Pull one out? YES NO"
    wait-message
    0 answer? if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $18 message-param-room
        $18 1 item-give-count
        0 $18 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        $3C threat-add
        fiona-who 2 6 char-sound
        reach-back-anim self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    stalkers-stay state-flag-clear
    hewie-no-attack state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act05 ( -- )   \ 0040E950
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    self-wait-done
    3 message   \ "Full-bodied flowers sprout from the bed. Pull one out? YES NO"
    wait-message
    0 answer? if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $19 message-param-room
        $19 1 item-give-count
        0 $19 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        got-yellow-mandragora story-flag-set
        $3C threat-add
        fiona-who 2 6 char-sound
        reach-back-anim self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    stalkers-stay state-flag-clear
    hewie-no-attack state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act06 ( -- )   \ 0040E9B0
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    self-wait-done
    3 message   \ "Full-bodied flowers sprout from the bed. Pull one out? YES NO"
    wait-message
    0 answer? if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1A message-param-room
        $1A 1 item-give-count
        0 $1A item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        got-red-mandragora story-flag-set
        $3C threat-add
        fiona-who 2 6 char-sound
        reach-back-anim self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    stalkers-stay state-flag-clear
    hewie-no-attack state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act07 ( -- )   \ 0040EA10
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    self-wait-done
    3 message   \ "Full-bodied flowers sprout from the bed. Pull one out? YES NO"
    wait-message
    0 answer? if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1B message-param-room
        $1B 1 item-give-count
        0 $1B item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        got-blue-mandragora story-flag-set
        $3C threat-add
        fiona-who 2 6 char-sound
        reach-back-anim self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    stalkers-stay state-flag-clear
    hewie-no-attack state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act08 ( -- )   \ 0040EA70
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    self-wait-done
    3 message   \ "Full-bodied flowers sprout from the bed. Pull one out? YES NO"
    wait-message
    0 answer? if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1C message-param-room
        $1C 1 item-give-count
        0 $1C item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        got-purple-mandragora story-flag-set
        $3C threat-add
        fiona-who 2 6 char-sound
        reach-back-anim self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    stalkers-stay state-flag-clear
    hewie-no-attack state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act09 ( -- )   \ 0047ABB0
    self-wait-done
    5 message   \ "No thank you. I've just about had my fill of these freakish things."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-13.act0B ( -- )   \ 0040EAF0
    stalkers-stay state-flag-set
    1 self-scripted
    0 counter-set
    0 hewie-who $C action
    self-wait-done
    hewie-who self-turn-to
    self-wait-done
    1 wait-counter
    $C06 self-anim
    self-wait-anim
    0 self-anim
    self-frames-reset
    1 self-wait-frames
    hewie-who self-look-at
    yield
    2 counter-set
    3 wait-counter
    0 self-scripted
    stalkers-stay state-flag-clear
    5 hewie-trust
    self-idle-or-end
;

: old-mansion-1f-13.act0C ( -- )   \ 0040EB20
    1 self-scripted
    self-wait-done
    fiona-who self-turn-to
    self-wait-done
    1 counter-set
    2 wait-counter
    hewie-bark
    self-wait-done
    hewie-who 0 0 char-camera
    hewie-who camera-follow
    $8D -18.0 29.0 0 $FFFF 5 self-move-to
    self-wait-done
    hewie-bark
    self-wait-done
    self-frames-reset
    $1E self-wait-frames
    fiona-who 0 0 char-camera
    fiona-who camera-follow
    3 counter-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act0D ( -- )   \ 0040EB60
    self-wait-done
    -180 self-turn-angle
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    6 message   \ "The flower bed below contains various flowers stretching in every direction. This must..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-13.act0E ( -- )   \ 0040EB80
    self-wait-done
    old-mansion-1f-13.cmd00
    $10 -54.6 28.6 -20.0 $E $80 $80 $80 $40 specks
    $10 54.6 29.0 -20.0 $E $80 $80 $80 $40 specks
    $10 -7.5 5.0 53.5 $E $80 $80 $80 $40 specks
    $10 0.0 64.8 -20.5 $E $80 $80 $80 $40 specks
    0 0 door-flag-unread
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    fiona-who 1 char-no-shadow
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who 0 char-no-shadow
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-13 register-scripts   \ (its phases and actions, by their names)
