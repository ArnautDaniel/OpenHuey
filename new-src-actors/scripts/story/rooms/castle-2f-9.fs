\ story/rooms/castle-2f-9.fs - the event scripts of room castle-2f-9 ($5; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-9
USING: room-names story.words story.shared flag-names story-names floor-flags ;

: castle-2f-9.enter ( -- )   \ 003F1800
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        room-sounds
        force-followed state-flag-set
        stalker-no-fear state-flag-set
        no-stalker-camera state-flag-set
        0 keeps-fiona-off keeps-hewie-off or floor-on
        stalker-who char-activate
        $FE 5 354 2 stalker-to-room
        0 stalker-who 0 char-model-op
        stalker-who 1 char-silent
        0 stalker-who 5 action
    then
    1 $2300 sound-volume
;

: castle-2f-9.char-enter ( -- )   \ 003F1840
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        2 2 player-camera
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    fiona-who self-is? if
        c2f-9-flag1 story-flag? not if
            self-who char-find-tri
            $80 exit-taken? if
                2 2 player-camera
            else 1 1 player-camera then
            0 fiona-who 0 action
        then
    then
;

: castle-2f-9.phase1 ( -- )   \ 003F1930
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
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        0 -4.53 -46.0 -47.5 $1E 15 0 zone
        0 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        0 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
        0 -4.53 -46.0 -47.5 $1E 15 0 zone
        hewie-who char-here? 1 game-mode? and if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? stalker-who char-here? and if
        stalker-who $80000000 6 char-sound
    then
;

: castle-2f-9.phase2 ( -- )   \ 003F1A30
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        fiona-who 3 $A 0 $2D char-touching-facing? if
            c2f-9-flag2 story-flag? not if
                1 unhurried offer-action
            else
                2 unhurried offer-action
            then
        then
    then
;

: castle-2f-9.phase5 ( -- )   \ 003F1A50
    force-followed state-flag-clear
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        1 stalker-who 0 char-model-op
        stalker-who 0 char-silent
        stalker-who action-end
        stalker-who char-done
        fiona-who 0 char-in-area? if
            stalker-no-fear state-flag-clear
            no-stalker-camera state-flag-clear
        then
    then
;

: castle-2f-9.act00 ( -- )   \ 003F1A80
    yield
    camera-restart
    1 exit-prepare
    self-frames-reset
    self-wait-16
    world-held state-flag-clear
    $F $41 fade
    c2f-9-flag1 story-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-9.act01 ( -- )   \ 003F1AA0
    1 self-scripted
    hewie-no-attack state-flag-set
    0 counter-set
    stalker-who action-end
    self-frames-reset
    1 self-wait-frames
    0 stalker-who 4 action
    1 wait-counter
    0 self-scripted
    hewie-no-attack state-flag-clear
    self-idle-or-end
;

: castle-2f-9.act02 ( -- )   \ 003F1AC0
    1 self-scripted
    hewie-no-attack state-flag-set
    1 message   \ "What?! Y-You just attacked me. How can you be so nonchalant? Could it have been... some..."
    wait-message
    0 self-scripted
    hewie-no-attack state-flag-clear
    self-idle-or-end
;

: castle-2f-9.act03 ( -- )   \ 0047A998
    begin
        $8001 self-anim
        self-wait-anim
    again
;

: castle-2f-9.act04 ( -- )   \ 003F1AE0
    1 self-noclip
    1 self-scripted
    self-wait-done
    $8004 $14 self-anim-blend
    self-wait-anim
    $8003 7 self-anim-blend
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    stalker-who 0 6 char-sound
    0 message   \ "Miss... It's cleaning time now."
    wait-message
    self-wait-anim
    $8005 7 self-anim-blend
    self-wait-anim
    $8001 $14 self-anim-blend
    self-wait-anim
    c2f-9-flag2 story-flag-set
    1 counter-set
    ['] castle-2f-9.act03 goto
;

: castle-2f-9.act05 ( -- )   \ 003F1B20
    1 self-noclip
    1 self-scripted
    self-wait-done
    stalker-who char-file-use
    stalker-who $162 -4.85 -47.03 180 char-to-xz
    stalker-who 0 0 char-camera
    ['] castle-2f-9.act03 goto
;

\ ---- registered ----
castle-2f-9 register-scripts   \ (its phases and actions, by their names)
