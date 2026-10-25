\ story/rooms/old-mansion-1f-22.fs - the event scripts of room old-mansion-1f-22 ($6E; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-22
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant first-time                  \ (bit 0) the story flag not yet set (the first time here)
1 constant hiding-left                 \ (bit 1) she has come out of hiding (act04)
2 constant hewie-along                 \ (bit 2) Hewie came along with her (near, commandable)
3 constant hewie-in-place              \ (bit 3) Hewie at his place for the scene
4 constant came-while-hidden           \ (bit 4) the stalker came in while she was hiding
0 constant stage-var                   \ (var 0) ? its scene's stage (2: hiding over)
1 constant held-var                    \ (var 1) ? she is held (1)
2 constant search-result-var           \ (var 2) his search's outcome by chance: 0, 1 or 2
3 constant searched-var                \ (var 3) ? his search done (0 / 1)
0 constant hide-open-sound             \ (snd 0) the hiding place opened
1 constant hide-shut-sound             \ (snd 1) the hiding place closed on her
5 constant grab-sound                  \ (snd 5) him grabbing her

: old-mansion-1f-22.enter ( -- )   \ 0043A120
    room-sounds
    0 keeps-hewie-off floor-on
    0 keeps-stalkers-off floor-on
    om1f-22-flag1 story-flag? if
        0 part-shown
        0 barred-floor floor-on
        0 1 object-hidden
        1 1 object-hidden
    else
        0 part-hidden
        0 barred-floor floor-off
        0 0 object-hidden
        1 0 object-hidden
    then
    om1f-22-refined-antimony story-flag? not if
        0 147.17 11.0 86.89 flicker-sprite
    then
    1 -36.7 19.1 -34.8 1 flame-in
    2 -36.7 19.1 34.8 1 flame-in
    1 $2300 sound-volume
;

: old-mansion-1f-22.act06 ( -- )   \ 0043A630
    came-while-hidden ebit-set
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
    then
    stalker-who camera-follow
    search-result-var 0 var-set
    $10 0 pvar? if
        $A chance? if
            search-result-var 2 var-set
        then
    else $10 1 pvar? if
        $19 chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    else $10 2 pvar? if
        $32 chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    else $10 3 pvar? if
        $4B chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    then then then then
    stalker-who 2 stalker-command
    3 self-is? $22 self-is? or if
    else search-result-var 1 var? if
        search-result-var 2 var-set
    then then
    2 creature-action? if
        search-result-var 2 var-set
    then
    search-result-var 0 var? if
        stalker-who 0 stalker-search-delay
        $78 1 summoner-wait
    else search-result-var 1 var? if
        stalker-who 150 stalker-search-delay
        0 stalker-who 7 action
    else search-result-var 2 var? if
        stalker-who 300 stalker-search-delay
        0 stalker-who 8 action
    then then then
    searched-var 0 var-set
    $10 pvar-inc
    exit
;

: old-mansion-1f-22.char-enter ( -- )   \ 0043A1A0
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
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            old-mansion-1f-22.act06
        else
            came-while-hidden ebit-clear
        then
    then
;

: old-mansion-1f-22.phase1 ( -- )   \ 0043A230
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 2 char-entered-area? if
        $1C 3 0 char-load
    then
    fiona-who 3 char-entered-area? if
        3 0 char-remove
    then
;

: old-mansion-1f-22.phase2 ( -- )   \ 0043A280
    fiona-who 6 char-in-area? fiona-who 45 $3C char-heading? and if
        stalker-who char-here? not if
            1 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    om1f-22-refined-antimony story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            9 calm offer-action
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
    [: stage-var 2 var? ;] wait-until
    counter-inc
    2 avoid-prompt
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    3 room-motion 0 blend-out
    0 self-noclip
    om1f-22-flag1 story-flag-set
    0 barred-floor floor-on
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-22.act03 ( -- )   \ 0043A510
    1 self-scripted
    counter-inc
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    0 2 object-anim
    1 2 object-anim
    2 room-motion 0 self-anim-blend
    $10 wait-frames
    fiona-who hide-open-sound 6 char-sound
    $38 wait-frames
    fiona-who hide-shut-sound 6 char-sound
    self-wait-anim
    0 self-noclip
    stalker-who char-here? if
        stalker-who self-look-at
        yield
        stalker-who self-turn-to
        self-wait-done
    then
    om1f-22-flag1 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-22.act04 ( -- )   \ 0043A560
    1 self-scripted
    counter-inc
    hiding-left ebit-set
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? stage-var 0 var? and if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    4 room-motion 0 self-anim-blend
    7 wait-frames
    fiona-who hide-open-sound 6 char-sound
    self-wait-anim
    0 self-noclip
    stalker-who char-here? if
        stalker-who self-look-at
        yield
        stalker-who self-turn-to
        self-wait-done
    then
    om1f-22-flag1 story-flag-set
    0 barred-floor floor-on
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-22.act01 ( -- )   \ 0043A350
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    self-wait-done
    stage-var 0 var-set
    held-var 0 var-set
    om1f-22-flag1 story-flag? not if
        first-time ebit-set
    else
        first-time ebit-clear
    then
    0 counter-set
    hewie-along ebit-clear
    hewie-in-place ebit-clear
    hiding-left ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who 5 action-force
        hewie-along ebit-set
        hewie-who 3 char-file-load
    then
    fiona-who 2 char-file-load
    stalker-who 4 char-file-load
    $47 144.89 65.089 90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    hewie-along ebit? if
        hewie-who self-look-at
        yield
        [: hewie-in-place ebit? ;] wait-until
    then
    counter-inc
    1 self-noclip
    self-who self-look-at
    yield
    first-time ebit? not if
        0 room-motion 5 self-anim-blend8
        5 wait-frames
        0 part-hidden
        0 0 object-hidden
        1 0 object-hidden
        0 0 object-anim
        1 0 object-anim
        $28 wait-frames
        fiona-who hide-shut-sound 6 char-sound
        self-wait-anim
    else
        1 room-motion 5 self-anim-blend8
        5 wait-frames
        0 part-hidden
        0 0 object-hidden
        1 0 object-hidden
        0 1 object-anim
        1 1 object-anim
        $E wait-frames
        fiona-who hide-open-sound 6 char-sound
        $3C wait-frames
        fiona-who hide-shut-sound 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 barred-floor floor-off
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not held-var 1 var? or if
            stage-var 1 var? if
                held-var 0 var-set
                fiona-who seized-sound 5 char-sound
                stalker-who char-here? if
                    ['] old-mansion-1f-22.act02 goto
                else
                    ['] old-mansion-1f-22.act03 goto
                then
            else in-panic? if
                ['] old-mansion-1f-22.act04 goto
            then then
            2 panic-calm-in
            6 fiona-calm
            $1E fiona-recovery-lower
            came-while-hidden ebit? if
                stalker-who char-here? not if
                    came-while-hidden ebit-clear
                    1 avoid-prompt
                then
            then
            yield
        else
            stalker-who char-here? fiona-who stalker-who 50 chars-within? and if
                ['] old-mansion-1f-22.act04 goto
            then
            counter-inc
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            stalker-who char-here? if
                stalker-who 0 stalker-command
                stalker-who action-end
            then
            1 self-noclip
            0 2 object-anim
            1 2 object-anim
            2 room-motion 0 self-anim-blend
            $10 wait-frames
            fiona-who hide-open-sound 6 char-sound
            $38 wait-frames
            fiona-who hide-shut-sound 6 char-sound
            self-wait-anim
            0 self-noclip
            stalker-who char-here? if
                stalker-who self-look-at
                yield
                stalker-who self-turn-to
                self-wait-done
            then
            om1f-22-flag1 story-flag-clear
            0 barred-floor floor-off
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
    hewie-who char-file-use
    hewie-in-place ebit-set
    1 wait-counter
    1 self-noclip
    first-time ebit? not if
        0 room-motion 5 self-anim-blend8
        self-wait-anim
    else
        1 room-motion 5 self-anim-blend8
        self-wait-anim
    then
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        fiona-who char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                hiding-left ebit? if
                    4 room-motion 0 self-anim-blend
                    7 wait-frames
                    fiona-who hide-open-sound 6 char-sound
                    self-wait-anim
                else stage-var 0 var? if
                    2 room-motion 0 blend-out
                else
                    3 room-motion 0 blend-out
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            hiding-left ebit? if
                4 room-motion 0 self-anim-blend
                7 wait-frames
                fiona-who hide-open-sound 6 char-sound
                self-wait-anim
            else stage-var 0 var? if
                2 room-motion 0 blend-out
            else
                3 room-motion 0 blend-out
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-1f-22.act07 ( -- )   \ 0043A700
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $47 144.89 65.089 90 $FFFF 5 self-move-to
    self-wait-done
    searched-var 1 var-set
    1 self-scripted
    held-var 1 var-set
    2 room-motion $A self-anim-blend
    $19 wait-frames
    stalker-who grab-sound 6 char-sound
    $28 threat-raise
    9 wait-frames
    stalker-who grab-sound 6 char-sound
    $14 threat-raise
    self-wait-anim
    held-var 0 var-set
    searched-var 0 var-set
    step-back-anim play-out
    0 self-scripted
    $78 1 summoner-wait
    self-idle-or-end
;

: old-mansion-1f-22.act08 ( -- )   \ 0043A770
    self-wait-done
    stalker-who 2 char-heading-for? 2 exit-door-open? not and if
        2 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        searched-var 0 var? if
            $47 144.89 65.089 90 $FFFF 5 self-move-to
            self-wait-done
        then
        searched-var 0 var-set
    else
        $4F 134.56 61.52 80 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    stage-var 1 var-set
    3 self-is? $22 self-is? or if
        0 3 object-anim
        1 3 object-anim
        1 room-motion $A self-anim-blend
        $10 wait-frames
        stage-var 2 var-set
        $A wait-frames
        stalker-who hide-open-sound 6 char-sound
        self-wait-anim
        step-back-anim play-out
        0 self-scripted
    else
        there-anim play-out
        fiona-hidden state-flag? if
            2 avoid-prompt
            panic-eased
            0 fiona-who 4 action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-1f-22.act09 ( -- )   \ 0043A820
    self-wait-done
    147.17 86.89 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-22-refined-antimony story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-22 register-scripts   \ (its phases and actions, by their names)
