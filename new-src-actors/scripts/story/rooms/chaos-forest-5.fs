\ story/rooms/chaos-forest-5.fs - the event scripts of room chaos-forest-5 ($103; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-5
USING: room-names story.words story.shared flag-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ room 0x103: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: chaos-forest-5.cmd00 ( b0 -- )  swing-three ;

: chaos-forest-5.enter ( -- )   \ 00417840
    room-sounds
    $19 1.0 0 bgm
    0 chaos-forest-5.cmd00
;

: chaos-forest-5.act02 ( -- )   \ 00417B10
    stalker-who camera-follow
    1 ebit-clear
    $1C 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1C 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1C 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1C 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
    then
    $1C pvar-inc
    exit
;

: chaos-forest-5.char-enter ( -- )   \ 00417850
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
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
    then then
    3 0 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? stalker-who 1 char-heading-for? and if
            2 ebit-set
            chaos-forest-5.act02
        else
            2 ebit-clear
        then
    then
;

: chaos-forest-5.phase1 ( -- )   \ 00417960
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
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    in-panic? not if
        fiona-who char-busy? not 2 2 pad? and if
            0 70.5 9.0 -5.5 5 10 0 zone
            fiona-who 0 8 char-zone-bits? if
                stalker-who char-here? not if
                    0 fiona-who 0 action
                else stalker-who 1 char-heading-for? not if
                    0 fiona-who 0 action
                then then
            then
        then
    then
    2 chaos-forest-5.cmd00
;

: chaos-forest-5.phase2 ( -- )   \ 00417A20
    0 70.5 9.0 -5.5 5 10 0 zone
    fiona-who 0 8 char-zone-bits? if
        1 chaos-forest-5.cmd00
        in-panic? not if
            stalker-who char-here? not if
                0 calm offer-action
            else stalker-who 1 char-heading-for? if
                $8016 examine-message   \ "I have to get away! I don't have time for this!"
            else
                0 calm offer-action
            then then
        then
    then
;

: chaos-forest-5.act01 ( -- )   \ 00417AE0
    0 ebit? if
        2 avoid-prompt
    then
    fiona-hidden state-flag-clear
    fiona-who camera-follow
    stalkers-stay state-flag-set
    stalker-who self-look-at
    yield
    4 $14 self-anim-blend
    self-wait-anim
    self-who self-look-at
    yield
    0 self-scripted
    0 self-noclip
    stalkers-stay state-flag-clear
    0 solid-floor floor-off
    self-idle-or-end
;

: chaos-forest-5.act00 ( -- )   \ 00417A60
    stalkers-stay state-flag-set
    0 solid-floor floor-on
    2 ebit-clear
    1 self-scripted
    1 self-noclip
    0 ebit-clear
    self-wait-done
    crouch-down-anim self-anim
    self-wait-anim
    crouching-anim self-anim
    fiona-hidden state-flag-set
    stalkers-stay state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            in-panic? 0 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] chaos-forest-5.act01 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? stalker-who char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            fiona-hidden state-flag-clear
            fiona-who camera-follow
            stalker-who action-end
            2 game-mode? if
                ['] chaos-forest-5.act01 goto
            then
            stalkers-stay state-flag-set
            crouch-up-anim self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            0 solid-floor floor-off
            self-idle-or-end
        then
    again
;

: chaos-forest-5.act03 ( -- )   \ 00417B60
    self-wait-done
    $E7 90.5 -4.5 -90 $FFFF 5 self-move-to
    self-wait-done
    there-anim self-anim
    self-wait-anim
    0 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

\ ---- registered ----
chaos-forest-5 register-scripts   \ (its phases and actions, by their names)
