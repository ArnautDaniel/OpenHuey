\ story/rooms/front-garden-3.fs - the event scripts of room front-garden-3 ($2A; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-3
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant tree-remarked               \ (bit 0) its first remark made: "A large tree is sprawled out here."
1 constant stalkers-held               \ (bit 1) the stalkers held while Hewie's scene runs
2 constant next-scene                  \ (bit 2) ? on to act0E (the scene after Hewie is in place)
3 constant hewie-zone1-noticed         \ (bit 3) Hewie has reacted to zone 1 (once a visit)
4 constant zone2-reached               \ (bit 4) she reached zone 2 (the opening's next part)
5 constant opening-started             \ (bit 5) the opening begun (act01: its music, its scripts)
6 constant hewie-zone3-noticed         \ (bit 6) Hewie has reacted to zone 3 (once a visit)
7 constant figures-kept                \ (bit 7) ? the opening's figures kept while unset
8 constant hewie-zone4-noticed         \ (bit 8) Hewie has reacted to zone 4 (once a visit)
9 constant hewie-scene-on              \ (bit 9) his scripted scene under way here (hewie): at the room's end he is put back
10 constant hewie-in-place             \ (bit 10) Hewie at his place (act0B)
0 constant whine-frames-var            \ (var 0) frames to Hewie's next whine (from 30, then 180)
0 constant hewie-effort-sound          \ (snd 0) Hewie at work?
1 constant hewie-whine-sound           \ (snd 1) Hewie's whine (now and then, caught in the wire)
2 constant effort-room-sound           \ (snd 2) her at work?
10 constant figure-cry-sound           \ (snd 10) the figure's cry as it appears

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
    1 solid-floor floor-on
    opening-done story-flag? not if
        0 resident-flag? if
            fg-3-flag3 story-flag-set
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
    3 part-hidden
    4 part-hidden
    c2f-3-flag1 story-flag? if
        hewie-found-in-wire story-flag? not if
            whine-frames-var 30 var-set
            $2A 0 360 hewie-to-room
            0 hewie-who 3 action
            3 part-shown
        else
            4 part-shown
        then
    then
    c2f-3-flag1 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
        front-garden-3.cmd02
        0 front-garden-3.cmd03
    else
        $A 128.0 56.7 13.0 $E $80 $80 $80 $40 scatter-specks
        $10 152.0 56.7 13.0 $C $80 $80 $80 $40 scatter-specks
        1 front-garden-3.cmd03
    then
    grate-open story-flag? not if
        2 barred-floor floor-on
    else
        2 1 object-hidden
        3 1 object-hidden
        2 part-shown
    then
    0 $A 0.812 0.687 0.187 0.312 zone-rect
    fg-3-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        fg-3-magnesia story-flag? not if
            0 185.7 0.9 -35.4 flicker-sprite
        then
    then
;

: front-garden-3.char-enter ( -- )   \ 004048E0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 0 player-camera
    then
    0 1 0 area-camera
    c2f-2-flag1 story-flag? not if
        player-who self-is? 1 exit-taken? and if
            player-who 1 char-to-exit
            2 1 player-camera
        then
        1 2 1 area-camera
    else
        player-who self-is? 1 exit-taken? and if
            player-who 1 char-to-exit
            6 5 player-camera
        then
        1 6 5 area-camera
    then
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        5 4 player-camera
    then
    2 5 4 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        3 2 player-camera
    then
    3 3 2 area-camera
;

: front-garden-3.phase1 ( -- )   \ 00404A20
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    player-who 4 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    fiona-who 5 char-in-area? fiona-who char-busy? not and if
        $1A 3 3 char-load
        3 exit-check
    then
    $B 0 -1 1 chars-area-camera
    c2f-2-flag1 story-flag? not if
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
    fiona-who 0 char-action? in-panic? not and fiona-who $16 char-entered-area? and if
        0 fiona-who 7 action
    then
    hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
        35 fiona-started? if
            fiona-who $19 char-in-area? hewie-who $1A char-in-area? and fiona-who -16 137 $32 char-faces-xz? and grate-open story-flag? not and stalker-who char-here? not and if
                hewie-may-break-off? if
                    0 hewie-who $B action
                then
            else 238 0 -135 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and fiona-who 238 -135 $32 char-faces-xz? and fg-3-refined-magnesia story-flag? not and if
                hewie-may-break-off? if
                    0 fiona-who $10 action
                then
            then then
        then
    then
    hewie-scene-on ebit? hewie-who char-busy? not and if
        stalkers-held ebit-clear
        stalkers-stay state-flag-clear
    then
    opening-started ebit? zone2-reached ebit? not and if
        2 225.0 0.0 -44.0 $19 10 0 zone
        fiona-who 2 2 char-zone-bits? if
            zone2-reached ebit-set
            3 track action-end
            0 3 track $14 action
        then
    then
    zone2-reached ebit? figures-kept ebit? not and if
        fiona-who $B char-entered-area? if
            figures-kept ebit-clear
            3 0 char-remove
            4 0 char-remove
            1 track action-end
            3 track action-end
        then
    then
    c2f-3-flag1 story-flag? if
        hewie-found-in-wire story-flag? not if
            whine-frames-var 0 var? if
                $1E chance? if
                    hewie-who hewie-whine-sound 6 char-sound
                    whine-frames-var 180 var-set
                else
                    whine-frames-var 60 var-set
                then
            else
                whine-frames-var var-dec
            then
        then
    then
    1 244.4 11.0 82.19 $2C 13 0 zone
    hewie-zone1-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    hewie-zone1-noticed ebit-set
                    $32 chance? if
                        hewie-zone-var 1 var-set
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
                hewie-zone-var 1 var-set
                hewie-zone-var 16.0 hewie-look-zone
            then
        then
    then
    grate-open story-flag? not if
        hewie-who $1A char-in-area? if
            3 -23.45 -10.0 138.0 $28 20 0 zone
            hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 3 9 char-zone-bits? if
                            hewie-zone3-noticed ebit-set
                            $64 chance? if
                                hewie-zone-var 3 var-set
                                0 hewie-who $88 action
                            then
                        then
                    then
                then
            then
        then
    then
    fg-3-refined-magnesia story-flag? not if
        4 252.44 0.0 -156.9 $30 20 0 zone
        hewie-zone4-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        hewie-zone4-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 4 var-set
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
                    hewie-zone-var 4 var-set
                    hewie-zone-var 0.0 hewie-look-zone
                then
            then
        then
    then
;

: front-garden-3.phase2 ( -- )   \ 00404CE0
    c2f-3-flag1 story-flag? if
        hewie-found-in-wire story-flag? not if
            fiona-who hewie-who $A 0 $5A char-touching-facing? if
                0 unhurried offer-action
            then
        else
            5 228.1 8.0 77.57 $A 0 0 zone
            fiona-who 5 2 char-zone-bits? fiona-who 228 77 $5A char-faces-xz? and if
                $15 unhurried offer-action
            then
        then
    then
    fiona-who $A $3C char-faces-area? if
        2 unhurried offer-action
    then
    grate-open story-flag? not if
        fiona-who $17 char-in-area? fiona-who -45 $32 char-heading? and if
            9 unhurried offer-action
        then
    then
    fiona-who $18 char-in-area? fiona-who -45 $32 char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $11 char-in-area? fg-3-flag1 story-flag? not and if
        4 unhurried offer-action
    then
    fiona-who $12 char-in-area? fg-3-flag2 story-flag? not and if
        5 unhurried offer-action
    then
    fiona-who $13 $32 char-faces-area? if
        6 unhurried offer-action
    then
    fg-3-breakable-broken story-flag? not if
        0 185.7 -0.1 -35.4 5 8 1 zone
        self-who 0 char-in-zone? if
            fg-3-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            185.7 -0.1 -35.4 0 -2144325584 0 0.0 shards-at
            $88 5 185.7 -0.1 -35.4 0 0 sound
            $40 $2CF noise
            0 185.7 0.9 -35.4 flicker-sprite
        then
    else fg-3-magnesia story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            8 calm offer-action
        then
    then then
;

: front-garden-3.phase5 ( -- )   \ 00404E00
    c2f-3-flag1 story-flag? hewie-found-in-wire story-flag? not and if
        hewie-who action-end
        hewie-who char-done
    then
    fg-3-flag1 story-flag-set
    fg-3-flag2 story-flag-set
    stalkers-held ebit? if
        stalkers-stay state-flag-clear
    then
    hewie-who char-busy? if
        hewie-who 3 char-in-nav-group? if
            hewie-who action-end
            hewie-who $282 235.0 -126.0 0 char-to-xz
            3 solid-floor floor-on
            stalkers-stay state-flag-clear
        then
    then
;

: front-garden-3.act00 ( -- )   \ 00404E50
    stalkers-stay state-flag-set
    1 self-scripted
    hewie-found-in-wire story-flag-set
    self-wait-done
    228.1 77.57 self-turn-to-xz
    self-wait-done
    tend-start-anim $A blend-out
    $C say   \ "Unforgivable! A wire is bound to the poor animal. It is digging deeply into the dog's..."
    tend-lead-in-anim $A self-anim-blend
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    3 part-hidden
    4 part-shown
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
    $B say   \ "I can't believe there are people out there that would do this to a poor helpless anima..."
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
    opening-started ebit-set
    0 1 track $12 action
    0 3 track $13 action
    opening-done story-flag-set
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
    c2f-3-flag1 story-flag? not if
        look-at-anim self-anim
        $1E wait-frames
        tree-remarked ebit? not if
            3 say   \ "A large tree is sprawled out here."
            tree-remarked ebit-set
        else
            4 say   \ "The branches are well kempt. Looks like someone has put a lot of time and energ..."
        then
        self-wait-anim
    else
        5 say   \ "There is a thick wire encircling the trunk of the tree."
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
    rest-anim play-out
    -1 self-idle-anim
    6 show-message   \ "Where am I?"
    7 wait-frames
    wait-message
    fg-3-flag1 story-flag-set
    self-idle-or-end
;

: front-garden-3.act05 ( -- )   \ 00405008
    self-wait-done
    7 say   \ "It's kind of cold... The air smells... funny."
    fg-3-flag2 story-flag-set
    self-idle-or-end
;

: front-garden-3.act06 ( -- )   \ 00405020
    self-wait-done
    65.64 70.94 self-turn-to-xz
    self-wait-done
    8 say   \ "A fountain... Looks like it is quite the antique."
    self-idle-or-end
;

: front-garden-3.act07 ( -- )   \ 00405030
    self-wait-done
    120 self-turn-angle
    self-wait-done
    turn-round-anim play-out
    -1 self-idle-anim
    2 say   \ "I just want to put the terrible things I saw back there behind me. I am not going back."
    self-idle-or-end
;

: front-garden-3.act08 ( -- )   \ 00405050
    self-wait-done
    185.7 -35.4 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            fg-3-magnesia story-flag-set
            0 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: front-garden-3.act0E ( -- )   \ 00405230
    1 self-scripted
    [: 6 sound-bank-loaded? ;] wait-until
    counter-inc
    -90 self-turn-angle
    self-wait-done
    2 wait-counter
    8 wait-frames
    hewie-who hewie-effort-sound 6 char-sound
    begin
        3 counter? not while
        0 front-garden-3.cmd00
        yield
    repeat
    4 wait-counter
    fiona-who effort-room-sound 6 char-sound
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
    hewie-in-place ebit-clear
    $8275 item-give
    0 self-scripted
    self-idle-or-end
;

: front-garden-3.act09 ( -- )   \ 004050B0
    self-wait-done
    hewie-in-place ebit? if
        0 counter-set
        next-scene ebit-set
        ['] front-garden-3.act0E goto
    then
    4 wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $2A7 -25.0 168.0 -80 char-to-xz
    1 13.0 20.0 0.0 0.0 event-camera
    16 wait-frames
    9 say   \ "This door is being blocked by that wooden box. There's got to be something I can do..."
    4 wait-frames
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
    crouch-down-anim play-out
    crouching-anim self-anim
    $A say   \ "There is a hole here, but it's far too small for me."
    crouch-up-anim play-out
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
    stalkers-held ebit-clear
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
    $A wait-frames
    counter-inc
    $34D $FFFF 6 self-move-tri
    self-wait-done
    counter-inc
    $40 wait-frames
    counter-inc
    8 wait-frames
    counter-inc
    $10 wait-frames
    counter-inc
    8 wait-frames
    2 barred-floor floor-off
    $2F door-unlock
    5 4 player-camera
    stalkers-stay state-flag-clear
    stalkers-held ebit-clear
    grate-open story-flag-set
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
    hewie-scene-on ebit-set
    stalkers-held ebit-set
    next-scene ebit-clear
    stalkers-stay state-flag-set
    self-wait-done
    hewie-bark
    self-wait-done
    $DC -12.99 136.933 -90 $FFFF $A self-move-to
    self-wait-done
    hewie-scene-on ebit-clear
    hewie-in-place ebit-set
    1 self-noclip
    $359 -30.0 137.0 -90 $204 5 self-move-to
    self-wait-done
    sit-down-anim play-out
    sit-anim play-out
    begin
        fiona-who $19 char-in-area? if
            44 fiona-started? if
                hewie-in-place ebit-clear
                ['] front-garden-3.act0C goto
            then
        then
        fiona-who $17 char-in-area? if
            44 fiona-started? 35 fiona-started? or 45 fiona-started? or if
                0 counter-set
                0 2 track $D action
                ['] front-garden-3.act0F goto
            then
            next-scene ebit? if
                ['] front-garden-3.act0F goto
            then
        then
        yield
    again
;

: front-garden-3.act0D ( -- )   \ 00405210
    [: fiona-free? ;] wait-until
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
        go-for-it-gesture-anim self-anim
        fiona-who 1 char-wait-motion
        fiona-who $2E 5 char-sound
        self-wait-anim
        -1 self-idle-anim
        7 wait-frames
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
    3 solid-floor floor-off
    3 keeps-fiona-off keeps-pursuers-off or floor-on
    $1A1 238.0 -135.0 1.0 35 hewie-go-to
    self-wait-done
    fg-3-refined-magnesia story-flag? not if
        $37 $FFFF 6 self-move-tri
        self-wait-done
        sit-a-while-anim play-out
        fiona-half-hidden state-flag? fiona-who char-busy? not or if
            $95 $63 item-count? not if
                10 hewie-trust
            then
            $95 message-param-room
            $95 $63 item-count? if
                $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
            else
                fg-3-refined-magnesia story-flag-set
                $95 1 item-give-count
                0 $95 item-tab
                item-fanfare
                $8011 say   \ "You've obtained ."
            then
        then
    then
    238.0 -135.0 self-turn-to-xz
    self-wait-done
    $1A1 238.0 -135.0 1.0 35 hewie-go-to
    self-wait-done
    3 solid-floor floor-on
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
    $F figure-cry-sound 6 char-sound
    $F $9002 1 0 char-anim-hold
    8 wait-frames
    0 front-garden-3.cmd01
    1 front-garden-3.cmd01
    self-idle-or-end
;

: front-garden-3.act15 ( -- )   \ 004054A0
    self-wait-done
    228.1 77.57 self-turn-to-xz
    self-wait-done
    crouch-down-anim $A blend-out
    $D say   \ "The wire looks like it got snagged on something. So many wounds... So much blood... Yo..."
    crouch-up-anim $A blend-out
    self-idle-or-end
;

: front-garden-3.act16 ( -- )   \ 004054C0
    self-wait-done
    $A 128.0 56.7 13.0 $E $80 $80 $80 $40 scatter-specks
    $10 152.0 56.7 13.0 $C $80 $80 $80 $40 scatter-specks
    1 front-garden-3.cmd03
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    4 part-shown
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
front-garden-3 register-scripts   \ (its phases and actions, by their names)
