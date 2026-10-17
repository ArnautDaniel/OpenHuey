\ story/rooms/castle-2f-8.fs - the event scripts of room castle-2f-8 ($24; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-8
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant earth-doll-remarked         \ (bit 0) its first remark made: "A huge doll made of earth blocks the door. There is a slot-s"
1 constant sound-started               \ (bit 1) its sound started (then kept up)
2 constant hewie-zone2-noticed         \ (bit 2) Hewie has reacted to zone 2 (once a visit)
3 constant calmed-remarked             \ (bit 3) its first remark made: "...... That got my mind off things. I feel a little better n"
0 constant figure-var                  \ (var 0) ? the figure's dance (cmd02)

\ Room24_Cmd00
: castle-2f-8.cmd00 ( b0 -- )   \ (the kiln's glow - "jimen", "kama", "sumi" - pulsing: their blend 0.5 + 0.5 cos(phase),
    \ the phase on 12.3 degrees a call; b0 0 at its start, -pi)
    {: b0 | a n :}
    3 0 do
        i case  0 of  s" jimen"  endof  1 of  s" kama"  endof  >r s" sumi" r>  endcase  to n to a
        a n object-here? if
            b0 0= if  pi fnegate a n $30 object!  0e 0 1 a n morph!
            else
                a n $30 object@ fcos 0.5e f* 0.5e f+  0 1 a n morph!
                a n $30 object@ 0.20944e f+  fdup pi f> if  2e pi f* f-  then  a n $30 object!
            then
        then
    loop ;
\ Room24_Cmd01
: castle-2f-8.cmd01 ( -- )  effects:rising-smoke ;
\ Room24_Cmd02
: castle-2f-8.cmd02 ( -- )  0 $50 $24 $2A $2A $46 colour-pulse ;

\ the golem (objects 4..9, "Golem_off_00".."05") and the plate in him ($A) are kept 50 above
\ the room, out of sight under its ceiling: hga stands him on the floor, before the kiln where
\ act00 has Fiona look up at him (the plate scenes key his parts themselves). Up there, what
\ the plate scenes hid stayed out of sight; down here it stays hidden: once a plate has gone in
\ ($1B) the plate, and after act0B's ending (not act0A's, $1A) all of him
: golem-down ( -- )
    $B 4 do  50e i object-lowered  loop
    c2f-8-flag3 story-flag? 0= if  exit  then
    $A 1 object-hidden
    c2f-8-flag2 story-flag? if  exit  then
    $A 4 do  i 1 object-hidden  loop ;

: castle-2f-8.enter ( -- )   \ 00401890
    golem-down
    room-sounds
    c2f-8-flag3 story-flag? not if
        0 barred-floor floor-on
        5 pushable floor-on
        1 keeps-stalkers-off floor-on
        0 part-shown
    else
        0 part-hidden
        c2f-8-flag2 story-flag? if
            2 barred-floor floor-on
            6 pushable floor-on
            3 keeps-stalkers-off floor-on
            1 part-shown
        else c2f-8-flag1 story-flag? if
            2 part-shown
        then then
    then
    got-old-parchment story-flag? not if
        5 part-shown
    then
    castle-2f-8.cmd01
    c2f-8-breakable-broken story-flag? not if
        4 keeps-people-off floor-on
        3 part-shown
        4 part-hidden
    else
        4 keeps-people-off floor-off
        3 part-hidden
        4 part-shown
    then
    0 7 0.812 0.687 0.187 0.312 zone-rect
    c2f-8-flag1 story-flag? got-obsidian-choker story-flag? not and if
        0 10.0 1.0 -25.0 flicker-sprite
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    2 2 1.5 light
;

: castle-2f-8.char-enter ( -- )   \ 00401950
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 -1 player-camera
    then
    0 1 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 -1 player-camera
    then
    1 0 -1 area-camera
    0 1 track 4 action
;

: castle-2f-8.phase1 ( -- )   \ 004019D0
    sound-started ebit? not if
        6 sound-bank-loaded? if
            $40000009 6 57.0 0.0 -2.0 0 0 sound
            sound-started ebit-set
        then
    else
        $C0000009 6 57.0 0.0 -2.0 0 0 sound
    then
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
    c2f-8-breakable-broken story-flag? not if
        0 34.0 0.0 29.0 5 8 1 zone
        self-who 0 char-in-zone? if
            c2f-8-breakable-broken story-flag-set
            4 keeps-people-off floor-off
            3 part-hidden
            4 part-shown
            34.0 0.0 29.0 0 -2144325584 0 0.0 shards-at
            $88 5 34.0 0.0 29.0 0 0 sound
            $40 $FF noise
        then
    then
    1 0 4 4 0 zone-at-effect
    c2f-8-flag3 story-flag? if
        c2f-8-flag2 story-flag? if
            2 43.743 0.0 -29.309 $F 20 0 zone
            hewie-zone2-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 2 9 char-zone-bits? if
                            hewie-zone2-noticed ebit-set
                            $1E chance? if
                                hewie-zone-var 2 var-set
                                0 hewie-who $89 action
                            then
                        then
                    then
                then
            then
        else c2f-8-flag1 story-flag? if
            hewie-who 0 char-in-area? not if
                2 10.0 0.0 -25.0 $A 20 0 zone
                hewie-zone2-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                    2 game-mode? not if
                        hewie-can-command? if
                            hewie-who 2 9 char-zone-bits? if
                                hewie-zone2-noticed ebit-set
                                $1E chance? if
                                    hewie-zone-var 2 var-set
                                    0 hewie-who $89 action
                                then
                            then
                        then
                    then
                then
            then
        then then
    then
    3 49.99 0.0 -0.54 $12 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-8.phase2 ( -- )   \ 00401B50
    -2147483644 scene-request? if
        0 fiona-who 8 action
    else c2f-8-flag3 story-flag? not if
        fiona-who 6 $32 char-faces-area? if
            0 unhurried offer-action
        then
    else fiona-who $B $32 char-faces-area? c2f-8-flag2 story-flag? and if
        9 unhurried offer-action
    then then then
    fiona-who $C char-in-area? got-old-parchment story-flag? not and if
        fiona-who 0 $32 char-heading? if
            6 calm offer-action
        then
    else fiona-who 7 $3C char-faces-area? if
        1 unhurried offer-action
    then then
    fiona-who 8 char-in-area? fiona-who 45 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who $A char-in-area? or fiona-who 90 $2D char-heading? and if
        3 unhurried offer-action
    then
    fiona-who 1 2 char-zone-bits? if
        7 calm offer-action
    then
;

: castle-2f-8.act00 ( -- )   \ 00401BD0
    self-wait-done
    $F5 10.0 -28.0 180 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 10.0 -40.0 0.0 0.0 event-camera
    earth-doll-remarked ebit? not if
        0 say   \ "A huge doll made of earth blocks the door. There is a slot-shaped hole in its chest..."
        earth-doll-remarked ebit-set
    else
        3 say   \ "It looks like there is a recess that something thin and flat will fit into."
    then
    16 wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.act01 ( -- )   \ 00401C30
    self-wait-done
    6.0 4.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    $28 wait-frames
    6 say   \ "What kind of equipment is this? It looks old, but someone has taken painstakingly good..."
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act02 ( -- )   \ 00401C50
    self-wait-done
    56.58 -1.5 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    7 say   \ "Such tremendous heat... With heat like this, I'm sure it's used for refining."
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act03 ( -- )   \ 00401C68
    self-wait-done
    look-at-anim self-anim
    $28 wait-frames
    8 say   \ "What a strange mark... There is something about it that makes me feel uncomfortable."
    self-wait-anim
    self-idle-or-end
;

: castle-2f-8.act04 ( -- )   \ 00401C80
    0 castle-2f-8.cmd00
    begin
        1 castle-2f-8.cmd00
        1 2 1.1 light
        2 wait-frames
        1 castle-2f-8.cmd00
        0 2 0.0 light
        2 2 1.5 light
        yield
    again
;

: castle-2f-8.act05 ( -- )   \ 00401CB0
    fiona-who 0 6 char-sound
    self-wait-done
    $1E wait-frames
    9 say   \ "Nothing happened."
    self-idle-or-end
;

: castle-2f-8.act06 ( -- )   \ 00401CD0
    self-wait-done
    $D6 7.826 -7.168 0 $FFFF 5 self-move-to
    self-wait-done
    $A say   \ "There is some kind of file here."
    reach-out-anim play-out
    4 wait-frames
    5 part-hidden
    got-old-parchment story-flag-set
    $A0 message-param-room
    $A0 1 item-give-count
    0 $A0 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: castle-2f-8.act07 ( -- )   \ 00401D30
    self-wait-done
    10.0 -25.0 self-turn-to-xz
    self-wait-done
    pick-up-anim play-out
    4 wait-frames
    $8D 1 item-count? not if
        0 effect-remove
        got-obsidian-choker story-flag-set
        $8D message-param-room
        $8D 1 item-give-count
        0 $8D item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else
        $75 message-param-room
        $75 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            got-obsidian-choker story-flag-set
            0 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
    then
    up-from-pick-anim play-out
    wait-message
    self-idle-or-end
;

: castle-2f-8.act08 ( -- )   \ 00401DC0
    [: fiona-who char-busy? ;] wait-until
    strain-anim 0 self-anim-blend
    16 wait-frames
    let-go-push-anim play-out
    1 message   \ "Well, I guess you never know until you try but I figured as much. There is no way someo..."
    -1 self-idle-anim
    7 wait-frames
    wait-message
    self-idle-or-end
;

: castle-2f-8.act09 ( -- )   \ 00401DE0
    self-wait-done
    45.0 -30.0 self-turn-to-xz
    self-wait-done
    2 say   \ "Whatever mechanism moved it seems to have run its course. I think it's here to stay..."
    self-idle-or-end
;

: castle-2f-8.act0A ( -- )   \ 00401DF0
    stalkers-stay state-flag-set
    1 self-scripted
    8 3 $FF char-load
    self-wait-done
    $F $54 fade
    3 char-start
    8 1 char-no-shadow
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    8 char-activate
    0 part-hidden
    self-who 1 char-visible
    $F $3C movie-param
    2 0 char-remove
    6 partner-load
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    2 char-start
    $A 1 object-hidden
    3 0 char-remove
    fiona-who $A8 -160 char-to-tri-facing
    camera-restart
    $13 door-unlock
    c2f-8-flag3 story-flag-set
    1 0 0 music
    0 barred-floor floor-off
    5 pushable floor-off
    1 keeps-stalkers-off floor-off
    2 barred-floor floor-on
    6 pushable floor-on
    3 keeps-stalkers-off floor-on
    1 part-shown
    self-who 0 char-visible
    $F $51 fade
    $16 resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.act0B ( -- )   \ 00401EF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 part-hidden
    self-who 1 char-visible
    2 part-shown
    0 10.0 1.0 -25.0 flicker-sprite
    $F $96 movie-param
    2 0 char-remove
    6 partner-load
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    2 char-start
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    fiona-who $A8 -160 char-to-tri-facing
    camera-restart
    $13 door-unlock
    c2f-8-flag3 story-flag-set
    1 0 0 music
    0 barred-floor floor-off
    5 pushable floor-off
    1 keeps-stalkers-off floor-off
    self-who 0 char-visible
    $F $51 fade
    $17 resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.act0C ( -- )   \ 00401FF0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    8 3 $FF char-load
    3 char-start
    8 1 char-no-shadow
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-hidden
    self-who 1 char-visible
    $F $3C movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    3 0 char-remove
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-2f-8.act0D ( -- )   \ 004020A0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-hidden
    self-who 1 char-visible
    2 part-shown
    $F $96 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-2f-8.act0E ( -- )   \ 00402150
    stalkers-stay state-flag-set
    1 self-scripted
    8 3 $FF char-load
    self-wait-done
    $F $44 fade
    3 char-start
    $37 1.0 1 bgm
    wait-fade
    no-pause state-flag-set
    panic-eased
    panic-held state-flag-set
    fiona-who $EE 10.0 -20.0 180 char-to-xz
    1 23.0 -12.0 0.0 5.0 event-camera
    self-who 1 char-visible
    0 part-hidden
    8 char-activate
    0 barred-floor floor-off
    5 pushable floor-off
    1 keeps-stalkers-off floor-off
    8 $EF 10.0 -30.0 0 char-to-xz
    0 barred-floor floor-on
    5 pushable floor-on
    1 keeps-stalkers-off floor-on
    [: 0 adx? ;] wait-until
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    figure-var 0 var-set
    castle-2f-8.cmd02
    $A wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        castle-2f-8.cmd02
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 part-shown
    8 action-end
    8 char-done
    3 0 char-remove
    self-who 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    panic-held state-flag-clear
    no-pause state-flag-clear
    $F $41 fade
    wait-fade
    calmed-remarked ebit? not if
        hand-to-mouth-anim self-anim
        $B say   \ "...... That got my mind off things. I feel a little better now."
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        3 avoid-prompt
        fiona-who $84 5 char-sound
        calmed-remarked ebit-set
        $1E wait-frames
    else
        $C say   \ "...... Just staring at this all day isn't going to get me out of this place any so..."
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-8.phase5 ( -- )   \ 0047AB10
;

\ ---- registered ----
castle-2f-8 register-scripts   \ (its phases and actions, by their names)
