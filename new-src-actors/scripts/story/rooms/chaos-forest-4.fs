\ story/rooms/chaos-forest-4.fs - the event scripts of room chaos-forest-4 ($102; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-4
USING: room-names story.words story.shared flag-names floor-flags ;

\ room 0x102: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: chaos-forest-4.cmd00 ( b0 -- )  swing-three ;

: chaos-forest-4.enter ( -- )   \ 004174B0
    room-sounds
    $19 1.0 0 bgm
    0 chaos-forest-4.cmd00
;

: chaos-forest-4.act02 ( -- )   \ 00417780
    stalker-who camera-follow
    1 ebit-clear
    $1B 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1B 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1B 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1B 3 pvar? if
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
    $1B pvar-inc
    exit
;

: chaos-forest-4.char-enter ( -- )   \ 004174C0
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
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
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
        fiona-hidden state-flag? stalker-who 2 char-heading-for? and if
            2 ebit-set
            chaos-forest-4.act02
        else
            2 ebit-clear
        then
    then
;

: chaos-forest-4.phase1 ( -- )   \ 004175D0
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
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    in-panic? not if
        fiona-who char-busy? not 2 2 pad? and if
            0 -61.3 9.0 0.24 5 10 0 zone
            fiona-who 0 8 char-zone-bits? if
                stalker-who char-here? not if
                    0 fiona-who 0 action
                else stalker-who 2 char-heading-for? not if
                    0 fiona-who 0 action
                then then
            then
        then
    then
    2 chaos-forest-4.cmd00
;

: chaos-forest-4.phase2 ( -- )   \ 00417690
    0 -61.3 9.0 0.24 5 10 0 zone
    fiona-who 0 8 char-zone-bits? if
        1 chaos-forest-4.cmd00
        in-panic? not if
            stalker-who char-here? not if
                0 calm offer-action
            else stalker-who 2 char-heading-for? if
                $8016 examine-message   \ "I have to get away! I don't have time for this!"
            else
                0 calm offer-action
            then then
        then
    then
;

: chaos-forest-4.act01 ( -- )   \ 00417750
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

: chaos-forest-4.act00 ( -- )   \ 004176D0
    stalkers-stay state-flag-set
    0 solid-floor floor-on
    2 ebit-clear
    1 self-scripted
    1 self-noclip
    0 ebit-clear
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    fiona-hidden state-flag-set
    stalkers-stay state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            in-panic? 0 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] chaos-forest-4.act01 goto
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
                ['] chaos-forest-4.act01 goto
            then
            stalkers-stay state-flag-set
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            0 solid-floor floor-off
            self-idle-or-end
        then
    again
;

: chaos-forest-4.act03 ( -- )   \ 004177D0
    self-wait-done
    $E0 -83.04 1.02 90 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
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
chaos-forest-4 register-scripts   \ (its phases and actions, by their names)
