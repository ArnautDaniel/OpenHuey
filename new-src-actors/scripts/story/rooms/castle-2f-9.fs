\ story/rooms/castle-2f-9.fs - the event scripts of room castle-2f-9 ($5; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-9
USING: room-names story.words story.shared flag-names ;

: castle-2f-9.enter ( -- )   \ 003F1800
    $5B story-flag? $5C story-flag? not and if
        room-sounds
        force-followed state-flag-set
        stalker-no-fear state-flag-set
        no-stalker-camera state-flag-set
        1 0 $1000010 nav-group
        $FE char-activate
        $FE 5 354 2 stalker-to-room
        0 $FE 0 char-model-op
        $FE 1 char-silent
        0 $FE 5 action
    then
    1 $2300 sound-volume
;

: castle-2f-9.char-enter ( -- )   \ 003F1840
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    0 self-is? if
        $1D story-flag? not if
            $FF char-find-tri
            $80 exit-taken? if
                hewie-controlled? not if
                    0 2 2 char-camera
                    0 camera-follow
                else
                    1 2 2 char-camera
                    1 camera-follow
                then
            else hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then then
            0 0 0 action
        then
    then
;

: castle-2f-9.phase1 ( -- )   \ 003F1930
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 4 char-entered-area? if
        0 exit-prepare
    then
    $5B story-flag? $5C story-flag? not and if
        0 -4.53 -46.0 -47.5 $1E 15 0 zone
        0 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        0 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        0 -4.53 -46.0 -47.5 $1E 15 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? $FE char-here? and if
        $FE $80000000 6 char-sound
    then
;

: castle-2f-9.phase2 ( -- )   \ 003F1A30
    $5B story-flag? $5C story-flag? not and if
        0 3 $A 0 $2D char-touching-facing? if
            $61 story-flag? not if
                5 1 0 scene-change
            else
                5 2 0 scene-change
            then
        then
    then
;

: castle-2f-9.phase5 ( -- )   \ 003F1A50
    force-followed state-flag-clear
    $5B story-flag? $5C story-flag? not and if
        1 $FE 0 char-model-op
        $FE 0 char-silent
        $FE action-end
        $FE char-done
        0 0 char-in-area? if
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
    $1D story-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-9.act01 ( -- )   \ 003F1AA0
    1 self-scripted
    hewie-no-attack state-flag-set
    0 counter-set
    $FE action-end
    self-frames-reset
    1 self-wait-frames
    0 $FE 4 action
    1 wait-counter
    0 self-scripted
    hewie-no-attack state-flag-clear
    self-idle-or-end
;

: castle-2f-9.act02 ( -- )   \ 003F1AC0
    1 self-scripted
    hewie-no-attack state-flag-set
    1 message
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
    $FE 0 6 char-sound
    0 message
    wait-message
    self-wait-anim
    $8005 7 self-anim-blend
    self-wait-anim
    $8001 $14 self-anim-blend
    self-wait-anim
    $61 story-flag-set
    1 counter-set
    ['] castle-2f-9.act03 goto
;

: castle-2f-9.act05 ( -- )   \ 003F1B20
    1 self-noclip
    1 self-scripted
    self-wait-done
    $FE char-file-use
    $FE $162 -4.85 -47.03 180 char-to-xz
    $FE 0 0 char-camera
    ['] castle-2f-9.act03 goto
;

\ ---- registered ----
' castle-2f-9.enter castle-2f-9 0 room-script!
' castle-2f-9.char-enter castle-2f-9 6 room-script!
' castle-2f-9.phase1 castle-2f-9 1 room-script!
' castle-2f-9.phase2 castle-2f-9 2 room-script!
' castle-2f-9.phase5 castle-2f-9 5 room-script!
' castle-2f-9.act00 castle-2f-9 $00 action-script!
' castle-2f-9.act01 castle-2f-9 $01 action-script!
' castle-2f-9.act02 castle-2f-9 $02 action-script!
' castle-2f-9.act03 castle-2f-9 $03 action-script!
' castle-2f-9.act04 castle-2f-9 $04 action-script!
' castle-2f-9.act05 castle-2f-9 $05 action-script!
