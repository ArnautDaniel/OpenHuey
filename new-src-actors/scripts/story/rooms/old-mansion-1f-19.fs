\ story/rooms/old-mansion-1f-19.fs - the event scripts of room old-mansion-1f-19 ($6B; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-19
USING: room-names story.words story.shared flag-names story-names fiona.anims fiona.sounds stalker.anims ;

: old-mansion-1f-19.enter ( -- )   \ 00438D30
    room-sounds
    om3f-1-flag2 story-flag? if
        om1f-19-flag1 story-flag? not if
            om1f-19-flag1 story-flag-set
            $6B 0 109 $80 5 -1 $B4 0.0 creature-place
        then
    then
    1 $2300 sound-volume
;

: old-mansion-1f-19.act06 ( -- )   \ 00439080
    5 ebit-set
    4 ebit-clear
    $13 0 pvar? if
        $A chance? if
            4 ebit-set
        then
    else $13 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $13 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $13 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    2 creature-action? if
        4 ebit-set
    then
    4 ebit? if
        0 stalker-who 7 action
    else
        $78 1 summoner-wait
    then
    $13 pvar-inc
    exit
;

: old-mansion-1f-19.char-enter ( -- )   \ 00438D60
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
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
    fiona-who self-is? if
        $FE exit-taken? if
            fiona-who $14 0.0 -110.0 0 char-to-xz
            0 0 player-camera
            0 fiona-who 1 action
        then
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            5 ebit-set
            old-mansion-1f-19.act06
        else
            5 ebit-clear
        then
    then
;

: old-mansion-1f-19.phase1 ( -- )   \ 00438E20
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        $1C 3 1 char-load
    then
    fiona-who 2 char-entered-area? if
        3 0 char-remove
    then
    2 0.75 0.0 -116.99 $14 10 0 zone
    6 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 hewie-who $88 action
                    then
                then
            then
        then
    then
    2 0.75 0.0 -116.99 $14 10 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 10.0 hewie-look-zone
            then
        then
    then
    7 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 0.0 9.0 -125.0 0 0 sound
            7 ebit-set
        then
    else
        $C0000008 6 0.0 9.0 -125.0 0 0 sound
    then
;

: old-mansion-1f-19.phase2 ( -- )   \ 00438F10
    fiona-who 7 char-in-area? fiona-who 90 $32 char-heading? and if
        0 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who 90 $3C char-heading? and if
        stalker-who char-here? not if
            4 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
;

: old-mansion-1f-19.act00 ( -- )   \ 00438F40
    stalkers-stay state-flag-set
    1 self-scripted
    $27 room-preload
    3 0 char-remove
    self-wait-done
    180 self-turn-angle
    self-wait-done
    look-close-anim $A self-anim-blend
    self-wait-anim
    0 message   \ "A miasma of mold and corrosion wafts from within the hole. Enter the hole? YES NO"
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        fiona-who slide-sound 5 char-sound
        world-held state-flag-set
        $FE exit-check
    else
        1 exit-prepare
        $1C 3 1 char-load
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-19.act01 ( -- )   \ 00438F80
    1 self-scripted
    1 exit-prepare
    $1C 3 1 char-load
    self-wait-done
    camera-restart
    fiona-who slide-sound 5 char-sound
    $F 1 fade
    wait-fade
    world-held state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-19.act02 ( -- )   \ 0047AE88
    self-idle-or-end
;

: old-mansion-1f-19.act03 ( -- )   \ 0047AE8C
    self-idle-or-end
;

: old-mansion-1f-19.act05 ( -- )   \ 00439050
    3 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    self-who 0 char-visible
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A self-anim-blend
    self-wait-anim
    0 self-noclip
    3 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-19.act04 ( -- )   \ 00438FA0
    stalkers-stay state-flag-set
    5 ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $31 $8004 5 82.92 -88.12 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who slide-sound 5 char-sound
    5 room-motion $A self-anim-blend8
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    3 ebit-clear
    0 avoid-prompt
    self-who 2 -1 char-camera
    self-who 1 char-visible
    begin
        0 2 pad? not if
            in-panic? 3 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] old-mansion-1f-19.act05 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? if
                    stalker-who char-here? not if
                        5 ebit-clear
                        1 avoid-prompt
                    then
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] old-mansion-1f-19.act05 goto
            then
            fiona-who camera-follow
            self-who 0 char-visible
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-1f-19.act07 ( -- )   \ 004390D0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        stalker-who 1 char-file-load
    then
    $4F 83.66 -76.9 180 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
        4 stalker-who 1 char-model-op
        0 room-motion self-anim
        self-wait-anim
        4 stalker-who 0 char-model-op
    else
        there-anim self-anim
        self-wait-anim
    then
    3 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-1f-19.phase5 ( -- )   \ 0047AE84
;

\ ---- registered ----
old-mansion-1f-19 register-scripts   \ (its phases and actions, by their names)
