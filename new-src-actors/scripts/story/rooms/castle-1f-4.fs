\ story/rooms/castle-1f-4.fs - the event scripts of room castle-1f-4 ($F; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-4
USING: room-names story.words story.shared flag-names story-names ;

\ room 0x0F: the floor plate ("fumi_yuka") fading: its +0x24 toward 1 while event variable 0 is
\ unset, toward 0 once set; byte 3 0 at once, else by 0.2 a step (var_fade).
: castle-1f-4.cmd00 ( b0 -- )  s" fumi_yuka" 0 var-fade ;   \ (the floor plate fading with event variable 0)
\ room 0x0F (Room0F_Cmd01_ptmf): three objects' +0x14 back to 0
: castle-1f-4.cmd01 ( -- )   \ (the hook, the puppet and its door turned back)
    0 s" hook" turn-y object-n!  0 s" a_Puppet" turn-y object-n!  0 s" Puppetdoor" turn-y object-n! ;
\ room 0x0F (Room0F_Cmd02_ptmf): an effect (DriftingFlecks_vtable, 0x840 bytes) with its box
: castle-1f-4.cmd02 ( -- )  140e -9.5e 90e  18e 110e  0e -90e 0e  effects:flecks ;   \ (flecks drifting through the hall)
\ room 0x0F (Room0F_Cmd03_ptmf): the player's model +0xBC (1, 0.25) or (0, 0) by byte 3
: castle-1f-4.cmd03 ( b0 -- )  drop ;   \ (the model hooks it calls are empty on PC and PS2 alike: CharModel_SetFlag / _VtCC / _VtD0)
\ room 0x0F (Room0F_Cmd04_ptmf): the player's model +0xD0 (0, 1.5, -2 / -1.5 by byte 3) and
\ +0xCC
: castle-1f-4.cmd04 ( b0 -- )  drop ;   \ (the model hooks it calls are empty on PC and PS2 alike: CharModel_SetFlag / _VtCC / _VtD0)

: castle-1f-4.enter ( -- )   \ 003F5EC0
    room-sounds
    broken-marionette-used story-flag? if
        0 0 $14 door-bits
        2 0 object-hidden
        3 0 object-hidden
        4 0 object-hidden
    else
        0 1 $14 door-bits
        2 0 object-hidden
        3 0 object-hidden
        4 0 object-hidden
    then
    7 door-locked? if
        1 0 $800008 nav-group
    then
    $80 exit-taken? if
        c1f-4-flag1 story-flag-set
        0 1 var-set
        1 24 var-set
    else
        3 exit-taken? if
        else
            c1f-4-flag2 story-flag-clear
            c1f-4-flag1 story-flag-clear
        then
        c1f-4-flag2 story-flag? if
            c1f-4-flag1 story-flag-set
            0 1 var-set
            1 12 var-set
        else c1f-4-flag1 story-flag? not if
            0 0 var-set
            1 0 var-set
        else
            0 1 var-set
            1 24 var-set
        then then
    then
    0 castle-1f-4.cmd00
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 flame-in
    c1f-4-flag4 story-flag? not if
        1 1 $14 door-bits
        3 ebit-clear
    else
        2 1 $14 door-bits
        c1f-4-refined-antimony story-flag? not if
            1 140.0 -9.0 94.0 flicker-sprite
        then
        3 ebit-set
    then
    got-cracked-plate-key story-flag? not if
        4 1 $14 door-bits
    else
        4 0 $14 door-bits
    then
    8 door-locked? if
        5 0 $14 door-bits
    else
        5 1 $14 door-bits
    then
    3 0 var-set
    4 0 var-set
    1 $2300 sound-volume
;

: castle-1f-4.char-enter ( -- )   \ 003F5FC0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
    then then
    3 0 0 area-camera
    fiona-who self-is? if
        got-castle-sketch story-flag? c2f-3-flag1 story-flag? not and if
            0.0 sound-volume-scale
            world-held state-flag-set
            0 fiona-who 0 action
            3 0 $14 door-bits
            7 0 object-hidden
            8 0 object-hidden
        else
            3 1 $14 door-bits
            7 1 object-hidden
            8 1 object-hidden
        then
        $80 exit-taken? if
            fiona-who $221 -90 char-to-tri-facing
            1 1 player-camera
            world-frozen state-flag-clear
            c1f-4-flag3 story-flag-set
            0 fiona-who $B action
        then
        saturnus-hint-on story-flag? c1f-14-flag2 story-flag? not and if
            c1f-14-flag2 story-flag-set
            stalker-who action-end
            stalker-who char-done
        then
    then
;

: castle-1f-4.phase1 ( -- )   \ 003F6120
    6 sound-bank-loaded? if
        broken-marionette-used story-flag? not if
            $C0000003 6 -72.0 10.0 15.0 0 0 sound
        else
            $C0000001 6 -72.0 10.0 15.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
        1 exit-usable? if
            3 3 1 char-load
            1 exit-check
        then
    else 1 exit-usable? if
        1 exit-check
    then then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
        fiona-who 6 char-entered-area? fiona-who 9 char-left-area? or if
            3 0 char-remove
        then
    then
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-left-area? fiona-who 6 char-entered-area? or if
        3 exit-prepare
    then
    fiona-who 7 char-left-area? fiona-who 8 char-entered-area? or if
        hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
            3 3 1 char-load
        then
        1 exit-prepare
    then
    fiona-who 9 char-left-area? if
        2 exit-prepare
    then
    0 0 var? if
        hewie-who $D char-in-area? if
            c1f-4-flag1 story-flag-set
            0 1 var-set
            hewie-who 0 6 char-sound
            0 $F1 $D action
        else fiona-who $D char-in-area? if
            c1f-4-flag1 story-flag-set
            0 1 var-set
            fiona-who 0 6 char-sound
            broken-marionette-used story-flag? c1f-4-flag3 story-flag? not and breaker-lowered story-flag? not and if
                0 fiona-who $A action
            else
                0 $F1 $D action
            then
        else stalker-who $D char-in-area? if
            c1f-4-flag1 story-flag-set
            0 1 var-set
            stalker-who 0 6 char-sound
            0 $F1 $D action
        then then then
    else hewie-who $D char-in-area? not fiona-who $D char-in-area? not and stalker-who $D char-in-area? not and if
        c1f-4-flag1 story-flag-clear
        0 0 var-set
        0 $F2 $D action
    then then
    1 castle-1f-4.cmd00
    broken-marionette-used story-flag? not if
        0 -28.0 0.0 15.0 $A 15 0 zone
        fiona-who 0 char-in-zone? if
            0 $F5 $1C action
        then
    then
    2 140.0 -10.0 94.0 6 5 0 zone
    c1f-4-flag4 story-flag? not if
        6 ebit? not if
            fiona-who 2 char-in-zone? if
                2 2 var? if
                    0 $F4 $1B action
                else
                    0 $F3 8 action
                then
            then
        then
    then
    6 sound-bank-loaded? if
        4 var-inc
        4 30 var? if
            4 0 var-set
            3 0 var? if
                $40000005 6 -17.0 22.0 -9.0 0 0 sound
            else 3 1 var? if
                $40000006 6 -17.0 22.0 -9.0 0 0 sound
            else 3 2 var? if
                $40000007 6 -17.0 22.0 -9.0 0 0 sound
            else 3 3 var? if
                $40000008 6 -17.0 22.0 -9.0 0 0 sound
            then then then then
            3 var-inc
            3 4 var? if
                3 0 var-set
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    c1f-4-flag4 story-flag? not if
        3 140.0 -10.0 94.0 $1E 20 0 zone
        4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        4 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        5 ebit? not if
            5 -38.5 0.0 -65.0 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 5 9 char-zone-bits? if
                        5 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            5 -38.5 0.0 -65.0 7 5 0 zone
            hewie-who 5 9 char-zone-bits? not if
                5 ebit-clear
            then
        then
    then
;

: castle-1f-4.phase2 ( -- )   \ 003F6420
    fiona-who $A char-in-area? if
        -2147483644 scene-request? if
            0 fiona-who 6 action
        else broken-marionette-used story-flag? not fiona-who -45 $2D char-heading? and if
            5 2 0 scene-change
        then then
    then
    fiona-who $E char-in-area? fiona-who 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    fiona-who $10 char-in-area? fiona-who 0 $2D char-heading? and if
        5 $E 0 scene-change
    then
    fiona-who $D char-in-area? if
        5 4 0 scene-change
    then
    fiona-who $F char-in-area? fiona-who -45 $32 char-heading? and if
        5 $84 3 scene-change
    then
    3 ebit? c1f-4-refined-antimony story-flag? not and if
        c1f-4-refined-antimony story-flag? not if
            2 1 5 5 0 zone-at-effect
            fiona-who 2 3 char-zone-bits? if
                5 $10 4 scene-change
            then
        then
    else fiona-who 2 2 char-zone-bits? fiona-who 45 $32 char-heading? and if
        5 9 0 scene-change
    then then
    -2147483646 scene-request? if
        fiona-who 2 char-before-door? if
            5 $13 1 scene-change
        then
    then
    fiona-who $11 char-in-area? fiona-who -45 $32 char-heading? and if
        5 $15 0 scene-change
    then
    got-cracked-plate-key story-flag? not if
        4 131.392 -30.0 147.223 5 5 0 zone
        fiona-who 4 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
;

: castle-1f-4.act00 ( -- )   \ 003F64F0
    $FE $F 443 2 stalker-to-room
    stalker-who $1BB 0 char-to-tri-facing
    stalker-who action-end
    stalker-who char-done
    3 char-start
    4 action-end
    4 char-done
    6 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $11 action
    panic-eased
    0 message-prepare   \ "Exire Debilitas! Please excuse me, Miss. I am Riccardo, keeper of the castle. S..."
    1 result? if
        scene-play
    then
    1 castle-1f-4.cmd03
    scene-locked state-flag-clear
    fiona-who 0 char-silent
    $31 room-preload
    fiona-calm-reset
    world-held state-flag-set
    stalker-who action-end
    stalker-who char-done
    3 0 char-remove
    $80 exit-check
    self-idle-or-end
;

: castle-1f-4.act01 ( -- )   \ 003F65A0
    0 counter-set
    1 self-scripted
    stalkers-stay state-flag-set
    self-wait-done
    0 item-use
    7 door-unlock
    broken-marionette-used story-flag-set
    0 $F0 5 action
    1 wait-counter
    fiona-who $1DD -4.507 15.509 -90 char-to-xz
    0 self-idle-anim
    camera-restart
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-4.act02 ( -- )   \ 003F65E0
    self-wait-done
    $1FD -16.0 14.0 -90 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 -10.0 0.0 0.0 event-camera
    0 ebit? not if
        $F message   \ "What? There's no knob... Now what do I do? Wait... There is a hook here... Maybe i..."
        wait-message
        0 ebit-set
    else
        $10 message   \ "The words 'Pupa Cella' are written here... Puppet room... I believe that is what..."
        wait-message
    then
    self-frames-reset
    self-wait-16
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    $228 item-give
    self-idle-or-end
;

: castle-1f-4.act03 ( -- )   \ 003F6640
    self-wait-done
    2 ebit? not if
        $16 message   \ "Why in the world is there a chair here?"
        wait-message
        $C1 0.0 -84.0 0 $FFFF 5 self-move-to
        self-wait-done
        $18 message   \ "This is just a normal chair, right?"
        wait-message
        2 ebit-set
    else
        $17 message   \ "Doesn't make much sense to me... Not that anything in this place does."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act04 ( -- )   \ 003F6670
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    self-wait-16
    $802 self-anim
    self-wait-anim
    -1 self-idle-anim
    $15 message   \ "There is something strange about this part of the floor."
    wait-message
    self-idle-or-end
;

: castle-1f-4.act05 ( -- )   \ 003F6690
    $F $54 fade
    5 0 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 0 $14 door-bits
    2 0 object-hidden
    3 0 object-hidden
    4 0 object-hidden
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    0 $F9 $18 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 castle-1f-4.cmd04
    0 0 $800008 nav-group
    castle-1f-4.cmd01
    hewie-who 0 char-visible
    hewie-who action-end
    counter-inc
    $F $51 fade
    wait-fade
    self-idle-or-end
;

: castle-1f-4.act06 ( -- )   \ 003F6750
    begin
        fiona-who char-busy? not while
        yield
    repeat
    $1203 0 self-anim-blend
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    1 ebit? not if
        $11 message   \ "It won't open. Maybe I need a key or something?"
        1 ebit-set
    else
        $12 message   \ "No luck. Could this be some strange puzzle or just another weird contraption?"
    then
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: castle-1f-4.act07 ( -- )   \ 003F6780
    self-wait-done
    self-frames-reset
    $19 self-wait-frames
    $5A threat-raise
    1 $FF 8 rumble
    fiona-who $43 5 char-sound
    $F00 self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: castle-1f-4.act08 ( -- )   \ 003F67A0
    1 $FF 4 rumble
    6 ebit-set
    fiona-who 9 6 char-sound
    0 139.0 -9.0 91.5 0 0 0 0 dust
    0 139.0 -9.0 94.5 0 0 0 0 dust
    begin
        fiona-free? not while
        yield
    repeat
    2 var-inc
    6 ebit-clear
    self-idle-or-end
;

: castle-1f-4.act09 ( -- )   \ 003F67E0
    self-wait-done
    c1f-4-flag4 story-flag? not if
        $13 message   \ "The wall looks weak here. Maybe if I hit it hard enough I can break through it?"
        wait-message
    else
        $14 message   \ "Bugs? Yuck! I hate bugs! Why me?"
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act0A ( -- )   \ 003F67F0
    stalkers-stay state-flag-set
    1 self-scripted
    $15 room-preload
    self-wait-done
    $F 0 fade
    wait-fade
    world-held state-flag-set
    scene-locked state-flag-set
    world-frozen state-flag-set
    subscreen-locked state-flag-set
    panic-eased
    panic-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-1f-4.act0B ( -- )   \ 003F6820
    1 self-scripted
    self-wait-done
    world-held state-flag-clear
    camera-restart
    1 0 self-anim-blend
    panic-held state-flag-clear
    $F 1 fade
    wait-fade
    1 message   \ "What's that noise? I swear I just heard something..."
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    wait-message
    scene-locked state-flag-clear
    subscreen-locked state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-4.act0C ( -- )   \ 003F6850
    self-wait-done
    1 ebit? not if
        $11 message   \ "It won't open. Maybe I need a key or something?"
        wait-message
        1 ebit-set
    else
        $12 message   \ "No luck. Could this be some strange puzzle or just another weird contraption?"
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act0D ( -- )   \ 003F6870
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    c1f-4-flag2 story-flag-set
    broken-marionette-used story-flag? not if
        $40000003 6 -72.0 10.0 15.0 0 0 sound
    else
        $40000001 6 -72.0 10.0 15.0 0 0 sound
    then
    c1f-4-flag1 story-flag? not if
        begin
            1 0 var? not c1f-4-flag1 story-flag? not and while
            1 var-dec
            self-frames-reset
            1 self-wait-frames
        repeat
        c1f-4-flag1 story-flag? not if
            broken-marionette-used story-flag? not if
                3 6 sound-stop
                $40000004 6 -72.0 10.0 15.0 0 0 sound
            else
                1 6 sound-stop
                $40000002 6 -72.0 10.0 15.0 0 0 sound
            then
            c1f-4-flag2 story-flag-clear
        then
    else
        begin
            1 24 var? not c1f-4-flag1 story-flag? and while
            1 var-inc
            self-frames-reset
            3 self-wait-frames
        repeat
        c1f-4-flag1 story-flag? if
            broken-marionette-used story-flag? not if
                3 6 sound-stop
                $40000004 6 -72.0 10.0 15.0 0 0 sound
            else
                1 6 sound-stop
                $40000002 6 -72.0 10.0 15.0 0 0 sound
            then
            c1f-4-flag2 story-flag-clear
        then
    then
    self-idle-or-end
;

: castle-1f-4.act0E ( -- )   \ 003F6960
    self-wait-done
    65.0 107.0 self-turn-to-xz
    self-wait-done
    2 ebit? if
        2 message   \ "The design and type are identical to the previous chair. They are probably from the..."
        wait-message
    else
        3 message   \ "Your average, run-of-the-mill chair. No unique markings that differentiate this fro..."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act0F ( -- )   \ 003F6980
    self-frames-reset
    $29 self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    self-frames-reset
    $2E self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    self-frames-reset
    $25 self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    c1f-4-refined-antimony story-flag? not if
        1 140.0 -9.0 94.0 flicker-sprite
    then
    3 ebit-set
    self-idle-or-end
;

: castle-1f-4.act10 ( -- )   \ 003F69E0
    self-wait-done
    140.0 94.0 self-turn-to-xz
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
            c1f-4-refined-antimony story-flag-set
            1 effect-remove
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

: castle-1f-4.act11 ( -- )   \ 003F6A40
    begin
        $F cutscene-shot? not while
        yield
    repeat
    stalker-who 1 char-no-shadow
    begin
        $10 cutscene-shot? not while
        yield
    repeat
    stalker-who 0 char-no-shadow
    begin
        $17 cutscene-shot? if
            0 castle-1f-4.cmd03
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-1f-4.act12 ( -- )   \ 003F6A70
    self-wait-done
    131.392 147.223 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-cracked-plate-key story-flag-set
    4 0 $14 door-bits
    $12 message-param-room
    $12 1 item-give-count
    0 $12 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-1f-4.act13 ( -- )   \ 003F6AC0
    self-wait-done
    2 self-through-exit
    self-wait-done
    in-panic? 2 game-mode? or if
        $60B self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
    else
        $609 self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        6 message   \ "Darn! The door is locked. Now how I am going to get this open?"
        wait-message
    then
    self-idle-or-end
;

: castle-1f-4.act14 ( -- )   \ 003F6AF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    fiona-who $C 6 char-sound
    5 1 $14 door-bits
    self-frames-reset
    $1E self-wait-frames
    fiona-who $72 5 char-sound
    8 door-unlock
    $903 self-anim
    130.0 -20.0 170.0 self-look-at-point
    yield
    self-frames-reset
    $3C self-wait-frames
    self-who self-look-at
    yield
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-4.act15 ( -- )   \ 003F6B50
    self-wait-done
    $A6 120.0 154.0 -90 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 15.0 -10.0 0.0 0.0 event-camera
    8 door-locked? if
        4 message   \ "What could this be used for? There appears to be a slot of sorts. Could this be som..."
        wait-message
    else
        5 message   \ "The plate-key is locked into place."
        wait-message
    then
    self-frames-reset
    self-wait-16
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-4.act16 ( -- )   \ 003F6BB0
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    7 message   \ "Looks like this isn't enough... I will need to re-forge this plate-key to be able to us..."
    wait-message
    self-idle-or-end
;

: castle-1f-4.act17 ( -- )   \ 003F6BE0
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    fiona-who $C 6 char-sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    8 message   \ "Nothing happened..."
    wait-message
    self-idle-or-end
;

: castle-1f-4.act18 ( -- )   \ 003F6C10
    0 castle-1f-4.cmd04
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 castle-1f-4.cmd04
    self-idle-or-end
;

: castle-1f-4.act19 ( -- )   \ 003F6C20
    self-wait-done
    0 1 $14 door-bits
    2 0 object-hidden
    3 0 object-hidden
    4 0 object-hidden
    1 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 22.8 14.65 68.75 0 flame-in
    4 3 $FF char-load
    3 char-start
    2 partner-load
    2 char-start
    6 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $11 action
    panic-eased
    0 message-prepare   \ "Exire Debilitas! Please excuse me, Miss. I am Riccardo, keeper of the castle. S..."
    1 result? if
        scene-play
    then
    1 castle-1f-4.cmd03
    2 0 char-remove
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-4.act1A ( -- )   \ 003F6CF0
    self-wait-done
    5 0 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    0 0 $14 door-bits
    2 0 object-hidden
    3 0 object-hidden
    4 0 object-hidden
    0 22.8 14.65 68.75 0 flame-in
    0 $F9 $18 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 castle-1f-4.cmd04
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-4.act1B ( -- )   \ 003F6DA0
    1 $FF 4 rumble
    c1f-4-flag4 story-flag-set
    fiona-who 9 6 char-sound
    $4000000A 6 140.0 -10.0 92.0 0 0 sound
    1 0 $14 door-bits
    2 1 $14 door-bits
    0 139.0 -9.0 91.5 0 0 0 0 dust
    0 139.0 -9.0 94.5 0 0 0 0 dust
    0 2 0.7 0.5 0.3 0.125 zone-rect
    137.0 -10.0 90.0 0 -2145378272 1 0.0 shards-at
    137.0 -10.0 95.0 0 -2145378272 1 0.0 shards-at
    castle-1f-4.cmd02
    0 $F1 $F action
    begin
        fiona-free? not while
        yield
    repeat
    0 fiona-who 7 action
    self-idle-or-end
;

: castle-1f-4.act1C ( -- )   \ 003F6E50
    fiona-who $91 5 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    0 fiona-who $C action
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-4.enter castle-1f-4 0 room-script!
' castle-1f-4.char-enter castle-1f-4 6 room-script!
' castle-1f-4.phase1 castle-1f-4 1 room-script!
' castle-1f-4.phase2 castle-1f-4 2 room-script!
' castle-1f-4.act00 castle-1f-4 $00 action-script!
' castle-1f-4.act01 castle-1f-4 $01 action-script!
' castle-1f-4.act02 castle-1f-4 $02 action-script!
' castle-1f-4.act03 castle-1f-4 $03 action-script!
' castle-1f-4.act04 castle-1f-4 $04 action-script!
' castle-1f-4.act05 castle-1f-4 $05 action-script!
' castle-1f-4.act06 castle-1f-4 $06 action-script!
' castle-1f-4.act07 castle-1f-4 $07 action-script!
' castle-1f-4.act08 castle-1f-4 $08 action-script!
' castle-1f-4.act09 castle-1f-4 $09 action-script!
' castle-1f-4.act0A castle-1f-4 $0A action-script!
' castle-1f-4.act0B castle-1f-4 $0B action-script!
' castle-1f-4.act0C castle-1f-4 $0C action-script!
' castle-1f-4.act0D castle-1f-4 $0D action-script!
' castle-1f-4.act0E castle-1f-4 $0E action-script!
' castle-1f-4.act0F castle-1f-4 $0F action-script!
' castle-1f-4.act10 castle-1f-4 $10 action-script!
' castle-1f-4.act11 castle-1f-4 $11 action-script!
' castle-1f-4.act12 castle-1f-4 $12 action-script!
' castle-1f-4.act13 castle-1f-4 $13 action-script!
' castle-1f-4.act14 castle-1f-4 $14 action-script!
' castle-1f-4.act15 castle-1f-4 $15 action-script!
' castle-1f-4.act16 castle-1f-4 $16 action-script!
' castle-1f-4.act17 castle-1f-4 $17 action-script!
' castle-1f-4.act18 castle-1f-4 $18 action-script!
' castle-1f-4.act19 castle-1f-4 $19 action-script!
' castle-1f-4.act1A castle-1f-4 $1A action-script!
' castle-1f-4.act1B castle-1f-4 $1B action-script!
' castle-1f-4.act1C castle-1f-4 $1C action-script!
