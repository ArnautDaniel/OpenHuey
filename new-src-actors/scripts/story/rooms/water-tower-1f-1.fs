\ story/rooms/water-tower-1f-1.fs - the event scripts of room water-tower-1f-1 ($C1; Water Tower: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-1f-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant rested                      \ (bit 0) she has rested here (her recovery: once a visit)
1 constant sound-step-var              \ (var 1) which of its sounds next (0..3)
2 constant sound-frames-var            \ (var 2) frames to the next sound (30)

\ room 0xC1: the summoner's countdown (progress +0x764) has run out.
: water-tower-1f-1.cond00? ( -- flag )  summoner-done? ;

: water-tower-1f-1.enter ( -- )   \ 0043EC80
    room-sounds
    $17 stalker-kind? if
        no-stalker-camera state-flag-set
    then
    wt1f-1-medallion-found story-flag? wt1f-1-medallion story-flag? not and if
        0 -39.66 1.0 0.99 flicker-sprite
    then
    1 $2300 sound-volume
    $1F hole-floor tri-floor-on
    $2F hole-floor tri-floor-on
    $89 hole-floor tri-floor-on
;

: water-tower-1f-1.char-enter ( -- )   \ 0043ECC0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 -1 player-camera
    then
    0 2 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 -1 player-camera
    then
    1 0 -1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        1 1 player-camera
    then
    2 1 1 area-camera
;

: water-tower-1f-1.phase1 ( -- )   \ 0043ED80
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    7 0 -1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    4 stalker-kind? $17 stalker-kind? or if
        stalker-who char-here? stalker-who char-busy? not and stalker-active? and if
            water-tower-1f-1.cond00? stalker-who char-unseen? and if
                stalker-who action-end
                1 summon-take
            then
        then
    then
    wt1f-1-medallion-found story-flag? not if
        0 -39.66 0.0 0.99 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var wt1f-1-medallion-found var-set
                taken-flag-var wt1f-1-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var -39660 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 990 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                $40000005 6 25.0 10.0 136.5 0 0 sound
            else sound-step-var 1 var? if
                $40000006 6 25.0 10.0 136.5 0 0 sound
            else sound-step-var 2 var? if
                $40000007 6 25.0 10.0 136.5 0 0 sound
            else sound-step-var 3 var? if
                $40000008 6 25.0 10.0 136.5 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
;

: water-tower-1f-1.phase2 ( -- )   \ 0043EF00
    fiona-who $B $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    fiona-who $C char-in-area? fiona-who -22 $32 char-heading? and if
        0 unhurried offer-action
    then
    wt1f-1-medallion-found story-flag? wt1f-1-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            1 calm offer-action
        then
    then
;

: water-tower-1f-1.phase3 ( -- )   \ 0043EF40
    -83.5 3.0 -34.7 -18.0 3.0 -20.4 -83.5 -12.0 -34.7 -18.0 -12.0 -20.4 lights-doorway
;

: water-tower-1f-1.act00 ( -- )   \ 0043EF80
    rested ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        fiona-who 0 char-file-load
        $32 -64.094 52.322 -45 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        0 say   \ "I wonder if this water is safe to drink... Guess I'll never know if I don't try."
        fiona-who char-file-use
        4 room-motion $A self-anim-blend
        $16 wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        3 avoid-prompt
        rested ebit-set
        fiona-who $84 5 char-sound
        $1E wait-frames
        got-doc-recovery story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 say   \ "'Recovery Points' Search a recovery point to calm Fiona from a state of pan..."
            $F 7 fade
            wait-fade
            got-doc-recovery story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B say   \ "has been saved to your file."
            4 wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        1 say   \ "Ahh, that helped."
    then
    self-idle-or-end
;

: water-tower-1f-1.act01 ( -- )   \ 0043F040
    self-wait-done
    -39.66 0.99 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            wt1f-1-medallion story-flag-set
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

\ ---- registered ----
water-tower-1f-1 register-scripts   \ (its phases and actions, by their names)
