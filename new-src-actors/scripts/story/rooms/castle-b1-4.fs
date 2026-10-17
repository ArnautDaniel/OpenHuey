\ story/rooms/castle-b1-4.fs - the event scripts of room castle-b1-4 ($7; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-4
USING: room-names story.words story.shared fiona.anims ;

: castle-b1-4.enter ( -- )   \ 003F21E0
    room-sounds
    0 0 var-set
    1 0 var-set
;

: castle-b1-4.char-enter ( -- )   \ 003F21F0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
    fiona-who self-is? if
        1 exit-taken? if
            2 map-page
        then
    then
    0 -11.28 26.25 0.0 0 flame-in
    1 0.0 35.2 11.3 0 flame-in
    2 11.3 59.2 0.0 0 flame-in
    3 0.0 83.2 -11.3 0 flame-in
    4 -11.28 105.2 0.0 0 flame-in
    1 $3FFF sound-volume
;

: castle-b1-4.phase1 ( -- )   \ 003F22D0
    0 exit-usable? if
        0 exit-check
    then
    player-who 0 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 map-page
    then
    fiona-who 3 char-left-area? if
        2 map-page
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
    4 4 8 -4 0 zone-at-effect
    fiona-who 4 3 char-zone-bits? fiona-who 4 3 char-zone-bits-before? not and if
        fiona-who 4 char-effect-moving
    then
    hewie-who 4 3 char-zone-bits? hewie-who 4 3 char-zone-bits-before? not and if
        hewie-who 4 char-effect-moving
    then
    stalker-who 4 3 char-zone-bits? stalker-who 4 3 char-zone-bits-before? not and if
        stalker-who 4 char-effect-moving
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -34.0 102.0 100.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -34.0 102.0 100.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -34.0 102.0 100.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -34.0 102.0 100.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: castle-b1-4.phase2 ( -- )   \ 003F24B0
    fiona-who 6 $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    fiona-who 7 char-in-area? fiona-who 0 $32 char-heading? and if
        0 unhurried offer-action
    then
;

: castle-b1-4.act00 ( -- )   \ 003F24D0
    self-wait-done
    35.0 10.0 self-turn-to-xz
    self-wait-done
    hand-to-mouth-anim $A self-anim-blend
    0 say   \ "'The path beyond is the road of darkness. Go not without a guide, lest ye lose yourself..."
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
castle-b1-4 register-scripts   \ (its phases and actions, by their names)
