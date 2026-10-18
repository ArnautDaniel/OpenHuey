\ story/rooms/chaos-forest-9.fs - the event scripts of room chaos-forest-9 ($107; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-9
USING: room-names story.words story.shared story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant maze-dealt                  \ (bit 0) the maze's rooms dealt (once a visit, not in a panic)
1 constant was-panicking               \ (bit 1) she was in a panic (as she came in, then each frame)
2 constant figure-scene-on             \ (bit 2) the figure's scene begun (act00)
3 constant lake-remarked               \ (bit 3) its first remark made: "It's... a lake, I think. Looks like there some kind of a... "
10 constant figure-cry-sound           \ (snd 10) the figure's cry as it appears

\ room 0x107: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 2 a frame (room_nudge).
: chaos-forest-9.cmd00 ( b0 -- )  2e room-nudge ;
\ room 0x107: starts the slowly turning backdrop model (Effect7A3D0: model 0x30 far off at
\ (-225, -64, -1125)).
: chaos-forest-9.cmd01 ( -- )  $C $80808080  -225e -63.62e -1125e  1400e 0.5e 0.1e  effects:caustic drop ;   \ (light rippling far off: Effect7A3D0)
\ room 0x107: a noise of loudness 0x20 or more was made in this room last frame (the progress'
\ noise requests kept at +0x10D4).
: chaos-forest-9.cond00? ( -- flag )  loud-here? ;

: chaos-forest-9.enter ( -- )   \ 00418AF0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    in-panic? if
        was-panicking ebit-set
    then
    cf-9-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        cf-9-refined-antimony story-flag? not if
            0 -298.4 72.6 -365.4 flicker-sprite
        then
    then
    0 $B 0.812 0.687 0.187 0.312 zone-rect
    cf-9-flag1 story-flag? not if
        $E 5 $FF char-load
        0 1 track 0 action
    then
    chaos-forest-9.cmd01
;

: chaos-forest-9.char-enter ( -- )   \ 00418B60
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
;

: chaos-forest-9.phase1 ( -- )   \ 00418BA0
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    maze-dealt ebit? not if
        was-panicking ebit? not in-panic? and if
            maze-dealt ebit-set
            deal-things
        then
    then
    in-panic? if
        was-panicking ebit-set
    else
        was-panicking ebit-clear
    then
    figure-scene-on ebit? cf-9-flag1 story-flag? not and if
        chaos-forest-9.cond00? if
            cf-9-flag1 story-flag-set
            1 track action-end
            0 1 track 2 action
        then
    then
;

: chaos-forest-9.phase2 ( -- )   \ 00418BF0
    cf-9-breakable-broken story-flag? not if
        0 -298.4 71.6 -365.4 5 8 1 zone
        self-who 0 char-in-zone? if
            cf-9-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -298.4 71.6 -365.4 0 -2144325584 0 0.0 shards-at
            $88 5 -298.4 71.6 -365.4 0 0 sound
            $40 $BA noise
            0 -298.4 72.6 -365.4 flicker-sprite
        then
    else cf-9-refined-antimony story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            1 calm offer-action
        then
    then then
    fiona-who 1 char-in-area? fiona-who 90 $2D char-heading? and if
        3 unhurried offer-action
    then
;

: chaos-forest-9.act00 ( -- )   \ 00418C90
    5 char-start
    figure-scene-on ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -339.86 125.95 -391.22 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: chaos-forest-9.act01 ( -- )   \ 00418CD0
    self-wait-done
    -298.4 -365.4 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            cf-9-refined-antimony story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: chaos-forest-9.act02 ( -- )   \ 00418D30
    $E figure-cry-sound 6 char-sound
    $E $9002 1 0 char-anim-hold
    8 wait-frames
    0 chaos-forest-9.cmd00
    1 chaos-forest-9.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

: chaos-forest-9.act03 ( -- )   \ 00418D50
    self-wait-done
    lake-remarked ebit? not if
        look-close-anim 5 blend-out
        0 say   \ "It's... a lake, I think. Looks like there some kind of a... tower erected in the ce..."
        lake-remarked ebit-set
    else
        hands-clasped-anim 5 blend-out
        1 say   \ "I'm not going to be able to go any further this way. Better go back and find a way..."
    then
    self-idle-or-end
;

: chaos-forest-9.phase5 ( -- )   \ 0047AC78
    $E action-end
    $E char-done
;

\ ---- registered ----
chaos-forest-9 register-scripts   \ (its phases and actions, by their names)
