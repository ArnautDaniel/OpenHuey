\ story/rooms/castle-3f-2.fs - the event scripts of room castle-3f-2 ($2C; Belli Castle: 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-3f-2
USING: room-names facts story.words story.shared flag-names story-names fiona.anims stalker.anims ;

: castle-3f-2.char-enter ( -- )   \ 0041D140
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
;

: castle-3f-2.phase1 ( -- )   \ 0041D1C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    fiona-who 2 char-entered-area? if
        c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
            stalker-who 0 char-file-load
        then
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        then
        1 exit-prepare
    then
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        fiona-who 4 char-left-area? if
            panic-eased
            0 fiona-who 0 action-force
        then
    then
    c3f-2-flag1 story-flag? found-file-233 story-flag? not and if
        0 stalker-alert? not if
            stalker-who 0 stalker-command
        then
    then
;

: castle-3f-2.enter ( -- )   \ 0047ACB4
;

: castle-3f-2.act00 ( -- )   \ 0041D220
    c3f-2-flag1 story-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    0 exit-door-open? not if
        0 1 self-door-knock
        $10 wait-frames
    then
    0 0 self-door-knock
    self-wait-done
    0 counter-set
    stalker-who char-activate
    $FE $2C 46 2 stalker-to-room
    summoner-own-cooldown
    stalker-who $2E 0.0 192.0 -180 char-to-xz
    0 stalker-who 1 action
    uneasy-2-anim play-out
    -1 self-idle-anim
    7 wait-frames
    1 wait-counter
    fiona-who 1 char-visible
    stalker-who 0 0 char-camera
    stalker-who camera-follow
    1 20.0 10.0 180.0 0.0 event-camera
    0 exit-door-open? if
        $34 door-open-clear
    then
    doors-room-in
    $34 door-lock
    $3C wait-frames
    hewie-who char-here? not hewie-who 0 char-in-area? or if
        $2C 0 43 hewie-to-room
        hewie-who $2B 9.584 178.014 0 char-to-xz
    then
    0 0.0 0.0 0.0 0.0 event-camera
    fiona-who 0 char-visible
    fiona-who camera-follow
    counter-inc
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-3f-2.act01 ( -- )   \ 0041D2D0
    1 self-scripted
    self-wait-done
    stalker.anims:stand-anim self-anim
    counter-inc
    2 wait-counter
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
castle-3f-2 register-scripts   \ (its phases and actions, by their names)
