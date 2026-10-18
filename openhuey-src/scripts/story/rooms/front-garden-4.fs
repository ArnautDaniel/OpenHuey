\ story/rooms/front-garden-4.fs - the event scripts of room front-garden-4 ($2B; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-4
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant stalker-scene-on            \ (bit 0) his scene under way: at the room's end he is let go
1 constant cages-remarked              \ (bit 1) its first remark made: "Empty cages litter the area. Maybe this was some kind of ken"
2 constant figure-in-zone3             \ (bit 2) the figure ($1A) in zone 3: its sound made
0 constant push-room-sound             \ (snd 0) her pushing
3 constant figure-zone-sound           \ (snd 3) the figure reaching zone 3

\ character kind 0x1A: byte 3 0 starts Kind26_MoveToB(2, -290, 42); else waits (2) until
\ Kind26_MoveDoneB says done
: front-garden-4.cmd00 ( b0 -- )
    if  $1A char-wait-done  else  $1A 2 -290e 42e char-animal-go  then ;

: front-garden-4.enter ( -- )   \ 00405640
    room-sounds
    $16 1.0 0 bgm
    stalker-scene-on ebit-clear
    c2f-3-flag1 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    then
    fg-4-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    0 5 0.812 0.687 0.187 0.312 zone-rect
    fg-4-camomile story-flag? not if
        0 -202.89 1.0 -6.76 flicker-sprite
    then
    fg-4-album-medallion-found story-flag? fg-4-album-medallion story-flag? not and if
        1 -283.8 1.0 15.5 flicker-sprite
    then
    $1E chance? if
        0 1 track 6 action
    else
        $1A action-end
        $1A char-done
    then
;

: front-garden-4.char-enter ( -- )   \ 004056D0
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    stalker-who self-is? if
        fiona-half-hidden state-flag? fiona-who 3 char-in-area? and fg-4-flag1 story-flag? not and $37 exit-door-open? and if
            0 stalker-who 0 action
        then
    then
;

: front-garden-4.phase1 ( -- )   \ 00405730
    player-who 0 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    stalker-who char-busy? if
        fiona-half-hidden state-flag? not if
            stalker-who action-end
            stalker-scene-on ebit-clear
        then
    then
    fg-4-breakable-broken story-flag? not if
        0 -120.0 0.0 -13.0 5 8 1 zone
        self-who 0 char-in-zone? if
            fg-4-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -120.0 0.0 -13.0 0 -2143272896 0 0.0 shards-at
            $88 5 -120.0 0.0 -13.0 0 0 sound
            $40 $124 noise
        then
    then
    fg-4-album-medallion-found story-flag? not if
        2 -283.8 0.0 15.5 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var fg-4-album-medallion-found var-set
                taken-flag-var fg-4-album-medallion var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var -283800 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 15500 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    $1A char-here? 0 game-mode? and if
        hewie-who char-here? if
            $1A hewie-who 70 chars-within? if
                $1A 0.0 hewie-look-char
            then
        then
    then
    figure-in-zone3 ebit? not if
        3 -304.86 0.0 49.5 $14 5 0 zone
        $1A 3 8 char-zone-bits? if
            figure-in-zone3 ebit-set
            $1A figure-zone-sound 6 char-sound
        then
    then
;

: front-garden-4.phase2 ( -- )   \ 00405850
    fiona-who 4 char-in-area? fiona-who -140 0 $3C char-faces-xz? and if
        3 unhurried offer-action
    then
    fiona-who 5 char-in-area? fiona-who 90 $3C char-heading? and if
        4 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who 0 $3C char-heading? and if
        5 unhurried offer-action
    then
    fg-4-camomile story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            1 calm offer-action
        then
    then
    fg-4-album-medallion-found story-flag? fg-4-album-medallion story-flag? not and if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            2 calm offer-action
        then
    then
;

: front-garden-4.act00 ( -- )   \ 004058B0
    self-wait-done
    $A8 -178.0 43.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 exit-door-open? if
        $1F -212.0 8.0 -180 $FFFF 5 self-move-to
        self-wait-done
        stand-about-anims play-out
        step-back-anim play-out
        step-back-anim play-out
        step-back-anim play-out
        step-back-anim play-out
        fiona-who 4 self-move-slot
        self-wait-done
        stalker-who 3 stalker-command
        fg-4-flag1 story-flag-set
    then
    self-idle-or-end
;

: front-garden-4.act01 ( -- )   \ 00405900
    self-wait-done
    -202.89 -6.76 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            fg-4-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: front-garden-4.act02 ( -- )   \ 00405960
    self-wait-done
    -283.8 15.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            fg-4-album-medallion story-flag-set
            1 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: front-garden-4.act03 ( -- )   \ 004059C0
    self-wait-done
    -140.0 0.0 self-turn-to-xz
    self-wait-done
    cages-remarked ebit? not if
        0 say   \ "Empty cages litter the area. Maybe this was some kind of kennel for keeping animals."
        cages-remarked ebit-set
    else
        1 say   \ "There are feathers and fur in the cage. For there to be this much, it must have bee..."
    then
    self-idle-or-end
;

: front-garden-4.act04 ( -- )   \ 004059E0
    self-wait-done
    180 self-turn-angle
    self-wait-done
    2 say   \ "This door has been boarded up."
    self-idle-or-end
;

: front-garden-4.act05 ( -- )   \ 004059F0
    self-wait-done
    found-file-202 story-flag? not if
        0 self-turn-angle
        self-wait-done
        4 say   \ "Such a large door... What could be on the other side?"
    else
        $1C -190.323 61.999 0 $FFFF 5 self-move-to
        self-wait-done
        4 wait-frames
        lean-anim play-out
        fiona-who push-room-sound 6 char-sound
        strain-anim self-anim
        16 wait-frames
        let-go-push-anim play-out
        -1 self-idle-anim
        7 wait-frames
        3 say   \ "The hinge is rusted solid. Try as I may, I can't open this door."
    then
    self-idle-or-end
;

: front-garden-4.act06 ( -- )   \ 00405A40
    3 char-start
    $1A char-activate
    $1A -240.0 0.0 43.0 90 char-to-xyz
    $1A $9000 1 0 char-anim-hold
    0 front-garden-4.cmd00
    1 front-garden-4.cmd00
    3 0 char-remove
    self-idle-or-end
;

\ ---- registered ----
front-garden-4 register-scripts   \ (its phases and actions, by their names)
