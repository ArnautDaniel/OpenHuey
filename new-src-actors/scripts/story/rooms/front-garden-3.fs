\ story/rooms/front-garden-3.fs - the event scripts of room front-garden-3 ($2A; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-3
USING: room-names story.words story.shared flag-names ;

\ the box and the grate, by byte 3: 0 the box's +0x28 on by 0.4; the grate's +0x14 (an angle) 1
\ back 1.5 degrees, 2 on 0.5, 3 back 0.5
: front-garden-3.cmd00 ( b0 -- )
    case
        0 of  0.4e s" kibako" place-z object+!  endof
        1 of  1.5e deg>rad fnegate s" a_koushi" turn-y object+!  endof
        2 of  0.5e deg>rad s" a_koushi" turn-y object+!  endof
        3 of  0.5e deg>rad fnegate s" a_koushi" turn-y object+!  endof
    endcase ;
\ Room2A_Cmd01
variable hs-left   \ (D_0047B24C: the frames left of the handler's step)
: front-garden-3.cmd01 ( b0 -- )   \ (Room2A_HandlerStep: 0 set going, 90 frames; else each frame the
    \ event character $F up 0.5 and on 2 along z till they are up)
    0= if  90 hs-left !  exit  then
    begin  -1 hs-left +!  hs-left @ 0> while
        $F 0e 0.5e 2e char-shift  yield
    repeat ;
\ room 0x2A: its effect (Room2AWisps_vtable) started at (220, 0, -100)
: front-garden-3.cmd02 ( -- )  effects:leaves ;   \ (leaves blowing across)
\ the 0x14-byte effect (Effect78BC0_vtable) started with the command's parameters (from byte 3)
: front-garden-3.cmd03 ( b0 -- )  drop  $F $80808080  65.6387e 6.1e 70.9417e  39.8e 0.5e 0.1e  effects:caustic drop ;   \ (light rippling: Effect78BC0 - b0 its glow's threshold, not drawn)

: front-garden-3.enter ( -- )   \ 00404740
    room-sounds
    1 1 8 nav-group
    $C story-flag? not if
        0 resident-flag? if
            $AE story-flag-set
        then
        fiona-who $275 -90 char-to-tri-facing
        2 1 player-camera
        hewie-who action-end
        hewie-who char-done
        subscreen-locked state-flag-clear
        hewie-commandable state-flag-clear
        hewie-no-attack state-flag-clear
        force-calm state-flag-set
        hewie-no-dodge state-flag-set
        hewie-timid state-flag-set
        no-flee state-flag-set
        7 door-lock
        9 door-lock
        $A door-lock
        $B door-lock
        $13 door-lock
        $22 door-lock
        $26 door-lock
        $12 door-lock
        $2F door-lock
        2 door-lock
        0 door-lock
        3 door-lock
        6 door-lock
        8 door-lock
        $14 door-lock
        $15 door-lock
        $19 door-lock
        $1E door-lock
        $34 door-lock
        $35 door-close-off-lock
        $18 door-close-off-lock
        $1C door-close-off-lock
        $1D door-close-off-lock
        1 door-close-off-lock
        $D door-close-off-lock
        exits-rebuild
        0 pvar-inc
        0 pvar-inc
        0 pvar-inc
        1 pvar-inc
        1 pvar-inc
        1 pvar-inc
        world-held state-flag-set
        0 fiona-who 1 action
    else
        $16 1.0 0 bgm
    then
    3 0 $14 door-bits
    4 0 $14 door-bits
    2 story-flag? if
        3 story-flag? not if
            0 30 var-set
            $2A 0 360 hewie-to-room
            0 hewie-who 3 action
            3 1 $14 door-bits
        else
            4 1 $14 door-bits
        then
    then
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
        front-garden-3.cmd02
        0 front-garden-3.cmd03
    else
        $A 128.0 56.7 13.0 $E $80 $80 $80 $40 specks
        $10 152.0 56.7 13.0 $C $80 $80 $80 $40 specks
        1 front-garden-3.cmd03
    then
    $33 story-flag? not if
        1 2 $20000 nav-group
    else
        2 1 object-hidden
        3 1 object-hidden
        2 1 $14 door-bits
    then
    0 $A 0.812 0.687 0.187 0.312 zone-rect
    $214 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $229 story-flag? not if
            0 185.7 0.9 -35.4 flicker-sprite
        then
    then
;

: front-garden-3.char-enter ( -- )   \ 004048E0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 0 player-camera
    then then
    0 1 0 area-camera
    $D story-flag? not if
        hewie-controlled? not if
            fiona-who self-is? 1 exit-taken? and if
                fiona-who 1 char-to-exit
                2 1 player-camera
            then
        else hewie-who self-is? 1 exit-taken? and if
            hewie-who 1 char-to-exit
            2 1 player-camera
        then then
        1 2 1 area-camera
    else
        hewie-controlled? not if
            fiona-who self-is? 1 exit-taken? and if
                fiona-who 1 char-to-exit
                6 5 player-camera
            then
        else hewie-who self-is? 1 exit-taken? and if
            hewie-who 1 char-to-exit
            6 5 player-camera
        then then
        1 6 5 area-camera
    then
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            5 4 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        5 4 player-camera
    then then
    2 5 4 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            3 2 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        3 2 player-camera
    then then
    3 3 2 area-camera
;

: front-garden-3.phase1 ( -- )   \ 00404A20
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
    hewie-controlled? not if
        fiona-who 4 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 4 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    fiona-who 5 char-in-area? fiona-who char-busy? not and if
        $1A 3 3 char-load
        3 exit-check
    then
    $B 0 -1 1 chars-area-camera
    $D story-flag? not if
        $C 2 1 1 chars-area-camera
    else
        $C 6 5 1 chars-area-camera
    then
    $D 0 -1 1 chars-area-camera
    $E 1 0 1 chars-area-camera
    $F 1 0 1 chars-area-camera
    $10 3 2 1 chars-area-camera
    $14 4 3 1 chars-area-camera
    $15 0 -1 1 chars-area-camera
    $1B 1 0 1 chars-area-camera
    $1C 5 4 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 3 char-entered-area? fiona-who 7 char-entered-area? or fiona-who 8 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        $1A 3 3 char-load
        3 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 6 char-left-area? if
        3 0 char-remove
    then
    fiona-who 0 char-action? $FF panic-stage? not and fiona-who $16 char-entered-area? and if
        0 fiona-who 7 action
    then
    hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
        35 fiona-started? if
            fiona-who $19 char-in-area? hewie-who $1A char-in-area? and fiona-who -16 137 $32 char-faces-xz? and $33 story-flag? not and stalker-who char-here? not and if
                hewie-may-break-off? if
                    0 hewie-who $B action
                then
            else 238 0 -135 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and fiona-who 238 -135 $32 char-faces-xz? and $231 story-flag? not and if
                hewie-may-break-off? if
                    0 fiona-who $10 action
                then
            then then
        then
    then
    9 ebit? hewie-who char-busy? not and if
        1 ebit-clear
        stalkers-stay state-flag-clear
    then
    5 ebit? 4 ebit? not and if
        2 225.0 0.0 -44.0 $19 10 0 zone
        fiona-who 2 2 char-zone-bits? if
            4 ebit-set
            $F3 action-end
            0 $F3 $14 action
        then
    then
    4 ebit? 7 ebit? not and if
        fiona-who $B char-entered-area? if
            7 ebit-clear
            3 0 char-remove
            4 0 char-remove
            $F1 action-end
            $F3 action-end
        then
    then
    2 story-flag? if
        3 story-flag? not if
            0 0 var? if
                $1E chance? if
                    hewie-who 1 6 char-sound
                    0 180 var-set
                else
                    0 60 var-set
                then
            else
                0 var-dec
            then
        then
    then
    1 244.4 11.0 82.19 $2C 13 0 zone
    3 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    3 ebit-set
                    $32 chance? if
                        $1F 1 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    1 244.4 11.0 82.19 $2C 13 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $33 story-flag? not if
        hewie-who $1A char-in-area? if
            3 -23.45 -10.0 138.0 $28 20 0 zone
            6 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 3 9 char-zone-bits? if
                            6 ebit-set
                            $64 chance? if
                                $1F 3 var-set
                                0 hewie-who $88 action
                            then
                        then
                    then
                then
            then
        then
    then
    $231 story-flag? not if
        4 252.44 0.0 -156.9 $30 20 0 zone
        8 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
        4 252.44 0.0 -156.9 $30 20 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    $1F 4 var-set
                    $1F 0.0 hewie-look-zone
                then
            then
        then
    then
;

: front-garden-3.phase2 ( -- )   \ 00404CE0
    2 story-flag? if
        3 story-flag? not if
            fiona-who hewie-who $A 0 $5A char-touching-facing? if
                5 0 0 scene-change
            then
        else
            5 228.1 8.0 77.57 $A 0 0 zone
            fiona-who 5 2 char-zone-bits? fiona-who 228 77 $5A char-faces-xz? and if
                5 $15 0 scene-change
            then
        then
    then
    fiona-who $A $3C char-faces-area? if
        5 2 0 scene-change
    then
    $33 story-flag? not if
        fiona-who $17 char-in-area? fiona-who -45 $32 char-heading? and if
            5 9 0 scene-change
        then
    then
    fiona-who $18 char-in-area? fiona-who -45 $32 char-heading? and if
        5 $A 0 scene-change
    then
    fiona-who $11 char-in-area? $29 story-flag? not and if
        5 4 0 scene-change
    then
    fiona-who $12 char-in-area? $2A story-flag? not and if
        5 5 0 scene-change
    then
    fiona-who $13 $32 char-faces-area? if
        5 6 0 scene-change
    then
    $214 story-flag? not if
        0 185.7 -0.1 -35.4 5 8 1 zone
        self-who 0 char-in-zone? if
            $214 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            185.7 -0.1 -35.4 0 -2144325584 0 0.0 shards-at
            $88 5 185.7 -0.1 -35.4 0 0 sound
            $40 $2CF noise
            0 185.7 0.9 -35.4 flicker-sprite
        then
    else $229 story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then then
;

: front-garden-3.phase5 ( -- )   \ 00404E00
    2 story-flag? 3 story-flag? not and if
        hewie-who action-end
        hewie-who char-done
    then
    $29 story-flag-set
    $2A story-flag-set
    1 ebit? if
        stalkers-stay state-flag-clear
    then
    hewie-who char-busy? if
        hewie-who 3 char-in-nav-group? if
            hewie-who action-end
            hewie-who $282 235.0 -126.0 0 char-to-xz
            1 3 8 nav-group
            stalkers-stay state-flag-clear
        then
    then
;

: front-garden-3.act00 ( -- )   \ 00404E50
    stalkers-stay state-flag-set
    1 self-scripted
    3 story-flag-set
    self-wait-done
    228.1 77.57 self-turn-to-xz
    self-wait-done
    $C0A $A self-anim-blend
    self-wait-anim
    $C message   \ "Unforgivable! A wire is bound to the poor animal. It is digging deeply into the dog's..."
    wait-message
    $C0B $A self-anim-blend
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    3 0 $14 door-bits
    4 1 $14 door-bits
    panic-eased
    0 message-prepare   \ "Go, now. And don't get caught this time. Go..."
    1 result? if
        scene-play
    then
    fiona-who $167 218.95 74.59 166 char-to-xz
    hewie-who action-end
    hewie-who char-done
    camera-restart
    $16 1.0 0 bgm
    $F $51 fade
    wait-fade
    $B message   \ "I can't believe there are people out there that would do this to a poor helpless anima..."
    wait-message
    $205 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: front-garden-3.act01 ( -- )   \ 00404F30
    2 partner-load
    $E 3 $FF char-load
    $F 4 figure-load
    stalker-who 3 1 room-doors-state
    stalker-who 5 1 room-doors-state
    stalker-who 6 1 room-doors-state
    stalker-who $17 1 room-doors-state
    stalker-who $B 1 room-doors-state
    stalker-who $C 1 room-doors-state
    stalker-who $15 1 room-doors-state
    stalker-who $24 1 room-doors-state
    stalker-who $12 1 room-doors-state
    hewie-who $17 1 room-doors-state
    2 char-start
    0 music-stage
    2 0 0 music
    4 0 0 music
    5 ebit-set
    0 $F1 $12 action
    0 $F3 $13 action
    $C story-flag-set
    $16 1.0 0 bgm
    in-play state-flag-clear
    $F 1 fade
    wait-fade
    $200 item-give
    self-idle-or-end
;

: front-garden-3.act02 ( -- )   \ 00404FA0
    self-wait-done
    250.0 87.0 self-turn-to-xz
    self-wait-done
    2 story-flag? not if
        $A00 self-anim
        self-frames-reset
        $1E self-wait-frames
        0 ebit? not if
            3 message   \ "A large tree is sprawled out here."
            wait-message
            0 ebit-set
        else
            4 message   \ "The branches are well kempt. Looks like someone has put a lot of time and energ..."
            wait-message
        then
        self-wait-anim
    else
        5 message   \ "There is a thick wire encircling the trunk of the tree."
        wait-message
    then
    self-idle-or-end
;

: front-garden-3.act03 ( -- )   \ 00404FD0
    self-wait-done
    hewie-who $168 228.1 77.57 0 char-to-xz
    1 self-scripted
    $1004 0 self-anim-blend
    begin
        yield
    again
;

: front-garden-3.act04 ( -- )   \ 00404FF0
    self-wait-done
    1 self-anim
    self-wait-anim
    -1 self-idle-anim
    6 message   \ "Where am I?"
    self-frames-reset
    7 self-wait-frames
    wait-message
    $29 story-flag-set
    self-idle-or-end
;

: front-garden-3.act05 ( -- )   \ 00405008
    self-wait-done
    7 message   \ "It's kind of cold... The air smells... funny."
    wait-message
    $2A story-flag-set
    self-idle-or-end
;

: front-garden-3.act06 ( -- )   \ 00405020
    self-wait-done
    65.64 70.94 self-turn-to-xz
    self-wait-done
    8 message   \ "A fountain... Looks like it is quite the antique."
    wait-message
    self-idle-or-end
;

: front-garden-3.act07 ( -- )   \ 00405030
    self-wait-done
    120 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-idle-anim
    2 message   \ "I just want to put the terrible things I saw back there behind me. I am not going back."
    wait-message
    self-idle-or-end
;

: front-garden-3.act08 ( -- )   \ 00405050
    self-wait-done
    185.7 -35.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $229 story-flag-set
            0 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: front-garden-3.act0E ( -- )   \ 00405230
    1 self-scripted
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    counter-inc
    -90 self-turn-angle
    self-wait-done
    2 wait-counter
    self-frames-reset
    8 self-wait-frames
    hewie-who 0 6 char-sound
    begin
        3 counter? not while
        0 front-garden-3.cmd00
        yield
    repeat
    4 wait-counter
    fiona-who 2 6 char-sound
    begin
        5 counter? not while
        1 front-garden-3.cmd00
        yield
    repeat
    begin
        6 counter? not while
        2 front-garden-3.cmd00
        yield
    repeat
    begin
        7 counter? not while
        3 front-garden-3.cmd00
        yield
    repeat
    8 wait-counter
    $A ebit-clear
    $8275 item-give
    0 self-scripted
    self-idle-or-end
;

: front-garden-3.act09 ( -- )   \ 004050B0
    self-wait-done
    $A ebit? if
        0 counter-set
        2 ebit-set
        ['] front-garden-3.act0E goto
    then
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $2A7 -25.0 168.0 -80 char-to-xz
    1 13.0 20.0 0.0 0.0 event-camera
    self-frames-reset
    self-wait-16
    9 message   \ "This door is being blocked by that wooden box. There's got to be something I can do..."
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    fiona-who $21D -21.0 168.0 -90 char-to-xz
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: front-garden-3.act0A ( -- )   \ 00405120
    self-wait-done
    $DC -12.99 136.933 -90 $FFFF 5 self-move-to
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    $A message   \ "There is a hole here, but it's far too small for me."
    wait-message
    $802 self-anim
    self-wait-anim
    -1 self-idle-anim
    self-idle-or-end
;

: front-garden-3.act0C ( -- )   \ 004051D0
    hewie-bark
    self-wait-done
    $346 -35.0 137.0 90 $FFFF 5 self-move-to
    self-wait-done
    $DC -12.99 136.933 90 $204 5 self-move-to
    self-wait-done
    0 self-noclip
    -1 self-idle-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    1 ebit-clear
    self-idle-or-end
;

: front-garden-3.act0F ( -- )   \ 00405290
    1 wait-counter
    0 self-doorway-fade
    hewie-who $80808080 1 char-tint
    hewie-who $346 -35.0 137.0 0 char-to-xz
    self-who 7 -1 char-camera
    1 self-noclip
    $345 -33.792 160.0 0 $FFFF 5 self-move-to
    self-wait-done
    counter-inc
    $194 -33.905 165.0 0 $204 5 self-move-to
    self-wait-done
    self-frames-reset
    $A self-wait-frames
    counter-inc
    $34D $FFFF 6 self-move-tri
    self-wait-done
    counter-inc
    self-frames-reset
    $40 self-wait-frames
    counter-inc
    self-frames-reset
    8 self-wait-frames
    counter-inc
    self-frames-reset
    $10 self-wait-frames
    counter-inc
    self-frames-reset
    8 self-wait-frames
    0 2 $20000 nav-group
    $2F door-unlock
    5 4 player-camera
    stalkers-stay state-flag-clear
    1 ebit-clear
    $33 story-flag-set
    counter-inc
    1 self-doorway-fade
    30 hewie-trust
    $34E $FFFF 6 self-move-tri
    self-wait-done
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: front-garden-3.act0B ( -- )   \ 00405150
    1 self-scripted
    9 ebit-set
    1 ebit-set
    2 ebit-clear
    stalkers-stay state-flag-set
    self-wait-done
    hewie-bark
    self-wait-done
    $DC -12.99 136.933 -90 $FFFF $A self-move-to
    self-wait-done
    9 ebit-clear
    $A ebit-set
    1 self-noclip
    $359 -30.0 137.0 -90 $204 5 self-move-to
    self-wait-done
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-anim
    begin
        fiona-who $19 char-in-area? if
            44 fiona-started? if
                $A ebit-clear
                ['] front-garden-3.act0C goto
            then
        then
        fiona-who $17 char-in-area? if
            44 fiona-started? 35 fiona-started? or 45 fiona-started? or if
                0 counter-set
                0 $F2 $D action
                ['] front-garden-3.act0F goto
            then
            2 ebit? if
                ['] front-garden-3.act0F goto
            then
        then
        yield
    again
;

: front-garden-3.act0D ( -- )   \ 00405210
    begin
        fiona-free? not while
        yield
    repeat
    panic-eased
    0 fiona-who $E action-force
    self-idle-or-end
;

: front-garden-3.act10 ( -- )   \ 00405330
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-who char-busy? not if
        0 hewie-who $11 action-force
        $C00 self-anim
        fiona-who 1 char-wait-motion
        fiona-who $2E 5 char-sound
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
    else
        stalkers-stay state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: front-garden-3.act11 ( -- )   \ 00405360
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $1B3 232.55 -97.32 173 $FFFF $A self-move-to
    self-wait-done
    0 3 8 nav-group
    1 3 $30 nav-group
    $1A1 238.0 -135.0 1.0 35 hewie-go-to
    self-wait-done
    $231 story-flag? not if
        $37 $FFFF 6 self-move-tri
        self-wait-done
        $1C03 self-anim
        self-wait-anim
        fiona-half-hidden state-flag? fiona-who char-busy? not or if
            $95 $63 item-count? not if
                10 hewie-trust
            then
            $95 message-param-room
            $95 $63 item-count? if
                $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
                wait-message
            else
                $231 story-flag-set
                $95 1 item-give-count
                0 $95 item-tab
                item-fanfare
                $8011 message   \ "You've obtained ."
                wait-message
            then
        then
    then
    238.0 -135.0 self-turn-to-xz
    self-wait-done
    $1A1 238.0 -135.0 1.0 35 hewie-go-to
    self-wait-done
    1 3 8 nav-group
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: front-garden-3.act12 ( -- )   \ 00405420
    3 char-start
    $E char-activate
    $E 266.9 75.5 -138.7 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: front-garden-3.act13 ( -- )   \ 00405450
    4 3 char-hand-over
    $F char-activate
    $F 229.0 38.3 -55.3 0 char-to-xyz
    begin
        $32 chance? if
            $F $9000 1 0 char-anim-hold
        else
            $F $9001 1 0 char-anim-hold
        then
        $F wait-char-anim
    again
;

: front-garden-3.act14 ( -- )   \ 00405480
    $F $A 6 char-sound
    $F $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 front-garden-3.cmd01
    1 front-garden-3.cmd01
    self-idle-or-end
;

: front-garden-3.act15 ( -- )   \ 004054A0
    self-wait-done
    228.1 77.57 self-turn-to-xz
    self-wait-done
    $800 $A self-anim-blend
    self-wait-anim
    $D message   \ "The wire looks like it got snagged on something. So many wounds... So much blood... Yo..."
    wait-message
    $802 $A self-anim-blend
    self-wait-anim
    self-idle-or-end
;

: front-garden-3.act16 ( -- )   \ 004054C0
    self-wait-done
    $A 128.0 56.7 13.0 $E $80 $80 $80 $40 specks
    $10 152.0 56.7 13.0 $C $80 $80 $80 $40 specks
    1 front-garden-3.cmd03
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    4 1 $14 door-bits
    panic-eased
    0 message-prepare   \ "Go, now. And don't get caught this time. Go..."
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
' front-garden-3.enter front-garden-3 0 room-script!
' front-garden-3.char-enter front-garden-3 6 room-script!
' front-garden-3.phase1 front-garden-3 1 room-script!
' front-garden-3.phase2 front-garden-3 2 room-script!
' front-garden-3.phase5 front-garden-3 5 room-script!
' front-garden-3.act00 front-garden-3 $00 action-script!
' front-garden-3.act01 front-garden-3 $01 action-script!
' front-garden-3.act02 front-garden-3 $02 action-script!
' front-garden-3.act03 front-garden-3 $03 action-script!
' front-garden-3.act04 front-garden-3 $04 action-script!
' front-garden-3.act05 front-garden-3 $05 action-script!
' front-garden-3.act06 front-garden-3 $06 action-script!
' front-garden-3.act07 front-garden-3 $07 action-script!
' front-garden-3.act08 front-garden-3 $08 action-script!
' front-garden-3.act09 front-garden-3 $09 action-script!
' front-garden-3.act0A front-garden-3 $0A action-script!
' front-garden-3.act0B front-garden-3 $0B action-script!
' front-garden-3.act0C front-garden-3 $0C action-script!
' front-garden-3.act0D front-garden-3 $0D action-script!
' front-garden-3.act0E front-garden-3 $0E action-script!
' front-garden-3.act0F front-garden-3 $0F action-script!
' front-garden-3.act10 front-garden-3 $10 action-script!
' front-garden-3.act11 front-garden-3 $11 action-script!
' front-garden-3.act12 front-garden-3 $12 action-script!
' front-garden-3.act13 front-garden-3 $13 action-script!
' front-garden-3.act14 front-garden-3 $14 action-script!
' front-garden-3.act15 front-garden-3 $15 action-script!
' front-garden-3.act16 front-garden-3 $16 action-script!
