\ story/rooms/castle-1f-11.fs - the event scripts of room castle-1f-11 ($3; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-11
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant stalker-scene-on            \ (bit 0) his scripted scene under way here (stalker): at the room's end he is put back
0 constant clamp-hits-var              \ (var 0) the clamp struck so far (the wire loosens at 2)
1 constant shove-frames-var            \ (var 1) his shove's frames (dust at 89)
0 constant clamp-hit-sound             \ (snd 0) her striking the clamp
11 constant shove-thud-sound           \ (snd 11) his shove landing (dust)

\ room 0x03 (Room03_Cmd00_ptmf): three objects turned (-60, -60 degrees about x; -90 about z)
: castle-1f-11.cmd00 ( -- )   \ (the drawbridge's two halves -60 degrees about x, the lever -90 about z)
    -60e deg>rad s" hanebasi1" turn-x object!  -60e deg>rad s" hanebasi2" turn-x object!
    -90e deg>rad s" kanagu" turn-z object! ;
\ a lever (pstr_kanagu, tilt +0x18 between -10 and 0 degrees): byte 3 0 back 2 degrees, 1 pulled
\ (-10) with a puff of grey dust at it
: castle-1f-11.cmd01 ( b0 -- )   \ (the lever's tilt, -10..0 degrees: 0 back 2; 1 pulled to -10 with a grey puff)
    s" kanagu" object-here? 0= if  drop exit  then
    s" kanagu" turn-z object@ rad>deg                                  ( b0 ) ( F: deg )
    dup 0= if  2e f+  then  dup 1 = if  fdrop -10e  then
    0e fmin -10e fmax deg>rad s" kanagu" turn-z object!
    1 = if  0 $80 $80 $80 1  s" kanagu" place-x object@ s" kanagu" place-y object@ s" kanagu" place-z object@  dust  then ;

: castle-1f-11.enter ( -- )   \ 003F0450
    room-sounds
    $16 1.0 0 bgm
    c1f-11-flag1 story-flag? if
        castle-1f-11.cmd00
    then
    c1f-11-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        c1f-11-beef-jerky story-flag? not if
            0 72.5 49.0 8.5 flicker-sprite
        then
    then
    c1f-11-breakable2-broken story-flag? not if
        2 keeps-people-off floor-on
        3 part-shown
        2 part-hidden
    else
        2 keeps-people-off floor-off
        3 part-hidden
        2 part-shown
        c1f-11-magnesia story-flag? not if
            1 53.0 114.0 -113.5 flicker-sprite
        then
    then
    c1f-11-breakable3-broken story-flag? not if
        1 keeps-people-off floor-on
        5 part-shown
        4 part-hidden
    else
        1 keeps-people-off floor-off
        5 part-hidden
        4 part-shown
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    $A 3.5 30.7 49.9 $10 $80 $80 $80 $40 scatter-specks
    $C 16.0 129.0 35.7 $10 $80 $80 $80 $60 scatter-specks
    c1f-11-medallion story-flag? not if
        2 9.646 0.163 27.266 flicker-sprite
    then
;

: castle-1f-11.char-enter ( -- )   \ 003F0540
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 2 player-camera
    then
    0 2 2 area-camera
;

: castle-1f-11.phase1 ( -- )   \ 003F0580
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    c1f-11-flag1 story-flag? not if
        3 1 1 1 chars-area-camera
        4 1 1 1 chars-area-camera
        5 0 0 1 chars-area-camera
        6 0 0 1 chars-area-camera
    then
    7 2 2 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    fiona-who 7 char-entered-area? if
        2 map-page
    then
    fiona-who 8 char-entered-area? if
        3 map-page
    then
    fiona-who 9 char-entered-area? if
        4 map-page
    then
    fiona-who $A char-entered-area? if
        3 map-page
    then
    c1f-11-flag1 story-flag? not if
        0 castle-1f-11.cmd01
        3 -11.0 113.0 -87.0 5 12 0 zone
        fiona-who 3 char-in-zone? if
            1 castle-1f-11.cmd01
            1 $FF 4 rumble-pad
            clamp-hits-var 2 var? if
                clamp-hits-var var-inc
                0 2 track $A action
            else
                0 1 track 3 action
            then
        then
    then
    c1f-11-medallion story-flag? not if
        hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
            2 game-mode? not if
                fiona-who $100000 char-on-nav-flags? not if
                    0 control-action? if
                        4 0 45 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                            fiona-who 4 45 $32 char-faces-xz? if
                                hewie-may-break-off? if
                                    0 fiona-who 6 action
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    c1f-11-medallion story-flag? not if
        4 9.646 0.163 27.266 $1E 6 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    hewie-zone-var 4 var-set
                    hewie-zone-var 6.0 hewie-look-zone
                then
            then
        then
    then
;

: castle-1f-11.phase2 ( -- )   \ 003F06A0
    c1f-11-breakable1-broken story-flag? not if
        0 72.5 48.0 8.5 5 8 1 zone
        self-who 0 char-in-zone? if
            c1f-11-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            72.5 48.0 8.5 0 -2142220208 0 0.0 shards-at
            $88 5 72.5 48.0 8.5 0 0 sound
            $40 $111 noise
            0 72.5 49.0 8.5 flicker-sprite
        then
    else c1f-11-beef-jerky story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            8 calm offer-action
        then
    then then
    c1f-11-breakable2-broken story-flag? not if
        1 53.0 113.0 -113.5 5 8 1 zone
        self-who 1 char-in-zone? if
            c1f-11-breakable2-broken story-flag-set
            2 keeps-people-off floor-off
            3 part-hidden
            2 part-shown
            53.0 113.0 -113.5 0 -2143272896 0 0.0 shards-at
            $88 5 53.0 113.0 -113.5 0 0 sound
            $40 $122 noise
            1 53.0 114.0 -113.5 flicker-sprite
        then
    else c1f-11-magnesia story-flag? not if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            4 calm offer-action
        then
    then then
    c1f-11-breakable3-broken story-flag? not if
        2 53.0 113.0 -107.0 5 8 1 zone
        self-who 2 char-in-zone? if
            c1f-11-breakable3-broken story-flag-set
            1 keeps-people-off floor-off
            5 part-hidden
            4 part-shown
            53.0 113.0 -107.0 0 -2143272896 0 0.0 shards-at
            $88 5 53.0 113.0 -107.0 0 0 sound
            $40 $123 noise
        then
    then
    fiona-who 2 char-in-area? fiona-who 90 $32 char-heading? and if
        2 unhurried offer-action
    then
;

: castle-1f-11.phase3 ( -- )   \ 003F0820
    78.0 127.0 -100.0 78.0 127.0 -19.0 78.0 90.0 -100.0 78.0 90.0 -19.0 lights-doorway
;

: castle-1f-11.phase5 ( -- )   \ 003F0860
    hewie-who char-busy? if
        hewie-who action-end
        hewie-who $CC 32.509 56.979 60 char-to-xz
        stalkers-stay state-flag-clear
    then
    stalker-who char-busy? stalker-scene-on ebit? and if
        stalker-who $8F 2.4 -76.445 90 char-to-xz
        stalker-scene-on ebit-clear
    then
;

: castle-1f-11.act00 ( -- )   \ 003F0890
    hewie-who char-busy? if
        hewie-who action-end
        hewie-who $CC 32.509 56.979 60 char-to-xz
        stalkers-stay state-flag-clear
    then
    1 self-scripted
    stalker-who action-end
    stalker-who char-done
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    $F $44 fade
    $FF 1.0 0 bgm
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    stalker-who 2 char-file-load
    0 self-idle-anim
    fiona-who $8C 1.471 -112.581 -161 char-to-xz
    0 0 player-camera
    yield
    camera-restart
    castle-1f-11.cmd00
    c1f-11-flag1 story-flag-set
    $18 $35 door-copy
    $35 door-close-off-lock
    exits-rebuild
    hewie-who $2F char-in-room? if
        $30 0 -1 hewie-to-room
    then
    stalker-who char-activate
    $FE 3 153 2 stalker-to-room
    summoner-own-cooldown
    stalker-who -1 -1 char-camera
    stalker-who 1 char-visible
    stalker-who 1 char-silent
    0 stalker-who 5 action
    $16 1.0 0 bgm
    $F $41 fade
    wait-fade
    $22B item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    stalker-who 3 0 room-doors-state
    stalker-scene-on ebit-clear
    self-idle-or-end
;

: castle-1f-11.act01 ( -- )   \ 003F09C0
    self-wait-done
    c1f-11-flag1 story-flag? not if
        3 say
    else
        2 say   \ "The clamp has already come off..."
    then
    self-idle-or-end
;

: castle-1f-11.act02 ( -- )   \ 003F09D0
    self-wait-done
    c1f-11-flag1 story-flag? not if
        world-frozen state-flag-set
        1 self-scripted
        fiona-who $8E -6.625 -82.651 -144 char-to-xz
        1 10.0 15.0 0.0 0.0 event-camera
        clamp-hits-var 0 var? if
            0 say   \ "A clamp is fixed to a wire winding device. It's rusted solid but if I hit it wi..."
        else
            1 say   \ "The wire seems to have loosened a bit."
        then
        16 wait-frames
        world-frozen state-flag-clear
        0 self-scripted
    else
        2 say   \ "The clamp has already come off..."
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-11.act03 ( -- )   \ 003F0A30
    fiona-who clamp-hit-sound 6 char-sound
    [: fiona-free? ;] wait-until
    clamp-hits-var var-inc
    self-idle-or-end
;

: castle-1f-11.act04 ( -- )   \ 003F0A40
    self-wait-done
    53.0 -113.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-11-magnesia story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-1f-11.act05 ( -- )   \ 003F0AA0
    stalker-scene-on ebit-set
    self-wait-done
    stalker-who char-file-use
    1 self-noclip
    1 self-scripted
    yield
    stalker-who $132 -37.43 44.367 180 char-to-xz
    stalker-who 0 0 char-camera
    1 self-noclip
    1 self-scripted
    stalker-who 0 char-visible
    stalker-who 0 char-silent
    $13C -45.739 -73.201 90 $FFFF $A self-move-to
    self-wait-done
    0 room-motion 5 self-anim-blend8
    shove-frames-var 0 var-set
    begin
        fiona-who self-touching? if
            stalker-who 0 fiona-thrown
        then
        shove-frames-var 89 var? if
            0 5.0 115.0 -70.0 0 0 0 0 dust
            0 5.0 115.0 -76.0 0 0 0 0 dust
            stalker-who shove-thud-sound 6 char-sound
        then
        shove-frames-var var-inc
        self-at-motion-event? not while
        yield
    repeat
    stalker-who 0 stalker-command
    0 self-noclip
    0 self-scripted
    stalker-scene-on ebit-clear
    summoner-own-cooldown
    self-idle-or-end
;

: castle-1f-11.act06 ( -- )   \ 003F0B30
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-who char-busy? not if
        0 hewie-who 7 action-force
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

: castle-1f-11.act07 ( -- )   \ 003F0B60
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $13 53.585 39.199 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $148 0.0 43.21 1.5 40 hewie-go-to
    self-wait-done
    $145 9.804 32.761 180 $FFFF 5 self-move-to
    self-wait-done
    sit-a-while-anim $A blend-out
    c1f-11-medallion story-flag? not if
        fiona-half-hidden state-flag? fiona-who char-busy? not or if
            $70 $63 item-count? not if
                10 hewie-trust
            then
            $70 message-param-room
            $70 $63 item-count? if
                $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
            else
                c1f-11-medallion story-flag-set
                $70 1 item-give-count
                0 $70 item-tab
                item-fanfare
                $8011 say   \ "You've obtained ."
            then
            c1f-11-medallion story-flag? if
                2 effect-remove
            then
        then
    then
    $148 -7.96 39.28 66 $FFFF 5 self-move-to
    self-wait-done
    $CC 32.509 56.979 1.5 50 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-11.act08 ( -- )   \ 003F0C20
    self-wait-done
    72.5 8.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-11-beef-jerky story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-1f-11.act09 ( -- )   \ 003F0C80
    self-wait-done
    $A 3.5 30.7 49.9 $10 $80 $80 $80 $40 scatter-specks
    $C 16.0 129.0 35.7 $10 $80 $80 $80 $60 scatter-specks
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
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

: castle-1f-11.act0A ( -- )   \ 003F0D30
    stalkers-stay state-flag-set
    fiona-who clamp-hit-sound 6 char-sound
    [: fiona-free? ;] wait-until
    panic-eased
    fiona-who 0 char-action? if
        0 fiona-who 0 action-force
    else
        1 fiona-who 0 action-force
    then
    self-idle-or-end
;

\ ---- registered ----
castle-1f-11 register-scripts   \ (its phases and actions, by their names)
