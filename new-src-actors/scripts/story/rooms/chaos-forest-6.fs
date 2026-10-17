\ story/rooms/chaos-forest-6.fs - the event scripts of room chaos-forest-6 ($104; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-6
USING: room-names story.words story.shared flag-names ;

\ room 0x104: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: chaos-forest-6.cmd00 ( b0 -- )  swing-three ;

: chaos-forest-6.act02 ( -- )   \ 00417E90
    stalker-who camera-follow
    1 ebit-clear
    $1D 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1D 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1D 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1D 3 pvar? if
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
    $1D pvar-inc
    exit
;

: chaos-forest-6.char-enter ( -- )   \ 00417BD0
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
            1 1 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        1 1 player-camera
    then then
    3 1 1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? stalker-who 3 char-heading-for? and if
            2 ebit-set
            chaos-forest-6.act02
        else
            2 ebit-clear
        then
    then
;

: chaos-forest-6.phase1 ( -- )   \ 00417CE0
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
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    $FF panic-stage? not if
        fiona-who char-busy? not 2 2 pad? and if
            0 3.4 9.0 62.55 5 10 0 zone
            fiona-who 0 8 char-zone-bits? if
                stalker-who char-here? not if
                    0 fiona-who 0 action
                else stalker-who 3 char-heading-for? not if
                    0 fiona-who 0 action
                then then
            then
        then
    then
    2 chaos-forest-6.cmd00
;

: chaos-forest-6.phase2 ( -- )   \ 00417DA0
    0 3.4 9.0 62.55 5 10 0 zone
    fiona-who 0 8 char-zone-bits? if
        1 chaos-forest-6.cmd00
        $FF panic-stage? not if
            stalker-who char-here? not if
                5 0 5 scene-change
            else stalker-who 3 char-heading-for? if
                $8016 examine-message   \ "I have to get away! I don't have time for this!"
            else
                5 0 5 scene-change
            then then
        then
    then
;

: chaos-forest-6.act01 ( -- )   \ 00417E60
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
    0 0 8 nav-group
    self-idle-or-end
;

: chaos-forest-6.act00 ( -- )   \ 00417DE0
    stalkers-stay state-flag-set
    1 0 8 nav-group
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
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    fiona-who $43 5 char-sound
                then
                ['] chaos-forest-6.act01 goto
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
                ['] chaos-forest-6.act01 goto
            then
            stalkers-stay state-flag-set
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            0 0 8 nav-group
            self-idle-or-end
        then
    again
;

: chaos-forest-6.act03 ( -- )   \ 00417EE0
    self-wait-done
    $E8 3.84 83.43 -180 $FFFF 5 self-move-to
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

: chaos-forest-6.enter ( -- )   \ 0047AC60
    room-sounds
    0 chaos-forest-6.cmd00
;

\ ---- registered ----
' chaos-forest-6.char-enter chaos-forest-6 6 room-script!
' chaos-forest-6.phase1 chaos-forest-6 1 room-script!
' chaos-forest-6.phase2 chaos-forest-6 2 room-script!
' chaos-forest-6.act00 chaos-forest-6 $00 action-script!
' chaos-forest-6.act01 chaos-forest-6 $01 action-script!
' chaos-forest-6.act02 chaos-forest-6 $02 action-script!
' chaos-forest-6.act03 chaos-forest-6 $03 action-script!
' chaos-forest-6.enter chaos-forest-6 0 room-script!
