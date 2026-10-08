\ story/rooms/old-mansion-1f-22.fs - the event scripts of room old-mansion-1f-22 ($6E; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-22
USING: room-names story.words story.shared flag-names ;

: old-mansion-1f-22.enter ( -- )   \ 0043A120
    room-sounds
    1 0 $1000000 nav-group
    1 0 $4000000 nav-group
    $325 story-flag? if
        0 1 $14 door-bits
        1 0 $20000 nav-group
        0 1 object-show
        1 1 object-show
    else
        0 0 $14 door-bits
        0 0 $20000 nav-group
        0 0 object-show
        1 0 object-show
    then
    $27F story-flag? not if
        0 147.17 11.0 86.89 flicker-sprite
    then
    1 -36.7 19.1 -34.8 1 effect-86
    2 -36.7 19.1 34.8 1 effect-86
    1 $2300 sound-volume
;

: old-mansion-1f-22.act06 ( -- )   \ 0043A630
    4 ebit-set
    3 self-is? $22 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    $10 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $10 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $10 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $10 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    3 self-is? $22 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE 7 action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE 8 action
    then then then
    3 0 var-set
    $10 pvar-inc
    exit
;

: old-mansion-1f-22.char-enter ( -- )   \ 0043A1A0
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
    $FE self-is? if
        fiona-hidden state-flag? if
            4 ebit-set
            old-mansion-1f-22.act06
        else
            4 ebit-clear
        then
    then
;

: old-mansion-1f-22.phase1 ( -- )   \ 0043A230
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 2 char-entered-area? if
        $1C 3 0 char-load
    then
    0 3 char-entered-area? if
        3 0 char-remove
    then
;

: old-mansion-1f-22.phase2 ( -- )   \ 0043A280
    0 6 char-in-area? 0 45 $3C char-heading? and if
        $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $27F story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
;

: old-mansion-1f-22.phase3 ( -- )   \ 0043A2B0
    -4.5 15.0 110.0 -4.5 15.0 79.8 -4.5 -15.0 110.0 -4.5 -15.0 79.8 lights-doorway
    -3.0 3.0 92.5 -33.6 3.0 86.4 -3.0 -20.0 92.5 -33.6 -20.0 86.4 lights-doorway
    149.4 25.0 59.1 159.4 25.0 59.1 149.4 0.0 59.1 159.4 0.0 59.1 lights-doorway
;

: old-mansion-1f-22.act00 ( -- )   \ 0047AEB8
    self-idle-or-end
;

: old-mansion-1f-22.act02 ( -- )   \ 0043A4E0
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $325 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-22.act03 ( -- )   \ 0043A510
    1 self-scripted
    counter-inc
    0 camera-follow
    fiona-hidden state-flag-clear
    $FE char-here? if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 2 object-anim
    1 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 1 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $325 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-22.act04 ( -- )   \ 0043A560
    1 self-scripted
    counter-inc
    1 ebit-set
    0 camera-follow
    fiona-hidden state-flag-clear
    $FE char-here? 0 0 var? and if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $325 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-22.act01 ( -- )   \ 0043A350
    stalkers-stay state-flag-set
    4 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $325 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 5 action-force
        2 ebit-set
        1 3 char-file-load
    then
    0 2 char-file-load
    $FE 4 char-file-load
    $47 144.89 65.089 90 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    2 ebit? if
        1 self-look-at
        yield
        begin
            3 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    0 ebit? not if
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 0 object-anim
        1 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 1 object-anim
        1 1 object-anim
        self-frames-reset
        $E self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 0 $20000 nav-group
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] old-mansion-1f-22.act02 goto
                else
                    ['] old-mansion-1f-22.act03 goto
                then
            else $FF panic-stage? if
                ['] old-mansion-1f-22.act04 goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            4 ebit? if
                $FE char-here? not if
                    4 ebit-clear
                    1 avoid-prompt
                then
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] old-mansion-1f-22.act04 goto
            then
            counter-inc
            0 camera-follow
            fiona-hidden state-flag-clear
            $FE char-here? if
                $FE 0 stalker-mode
                $FE action-end
            then
            1 self-noclip
            0 2 object-anim
            1 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            0 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            0 1 6 char-sound
            self-wait-anim
            0 self-noclip
            $FE char-here? if
                $FE self-look-at
                yield
                $FE self-turn-to
                self-wait-done
            then
            $325 story-flag-clear
            0 0 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-1f-22.act05 ( -- )   \ 0043A5B0
    1 self-scripted
    self-wait-done
    $47 140.97 60.42 90 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
        $8000 5 self-anim-9
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-wait-anim
    then
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                1 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    0 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            1 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                0 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-1f-22.act07 ( -- )   \ 0043A700
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $47 144.89 65.089 90 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $19 self-wait-frames
    $FE 5 6 char-sound
    $28 threat-raise
    self-frames-reset
    9 self-wait-frames
    $FE 5 6 char-sound
    $14 threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: old-mansion-1f-22.act08 ( -- )   \ 0043A770
    self-wait-done
    $FE 2 char-heading-for? 2 exit-door-open? not and if
        2 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $47 144.89 65.089 90 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $4F 134.56 61.52 80 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    0 1 var-set
    3 self-is? $22 self-is? or if
        0 3 object-anim
        1 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $10 self-wait-frames
        0 2 var-set
        self-frames-reset
        $A self-wait-frames
        $FE 0 6 char-sound
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        fiona-hidden state-flag? if
            2 avoid-prompt
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-1f-22.act09 ( -- )   \ 0043A820
    self-wait-done
    147.17 86.89 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $27F story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-22.enter old-mansion-1f-22 0 room-script!
' old-mansion-1f-22.char-enter old-mansion-1f-22 6 room-script!
' old-mansion-1f-22.phase1 old-mansion-1f-22 1 room-script!
' old-mansion-1f-22.phase2 old-mansion-1f-22 2 room-script!
' old-mansion-1f-22.phase3 old-mansion-1f-22 3 room-script!
' old-mansion-1f-22.act00 old-mansion-1f-22 $00 action-script!
' old-mansion-1f-22.act01 old-mansion-1f-22 $01 action-script!
' old-mansion-1f-22.act02 old-mansion-1f-22 $02 action-script!
' old-mansion-1f-22.act03 old-mansion-1f-22 $03 action-script!
' old-mansion-1f-22.act04 old-mansion-1f-22 $04 action-script!
' old-mansion-1f-22.act05 old-mansion-1f-22 $05 action-script!
' old-mansion-1f-22.act06 old-mansion-1f-22 $06 action-script!
' old-mansion-1f-22.act07 old-mansion-1f-22 $07 action-script!
' old-mansion-1f-22.act08 old-mansion-1f-22 $08 action-script!
' old-mansion-1f-22.act09 old-mansion-1f-22 $09 action-script!
