\ story/rooms/old-mansion-b1-4.fs - the event scripts of room old-mansion-b1-4 ($50; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-4
USING: room-names story.words story.shared flag-names ;

: old-mansion-b1-4.cmd00 ( -- )  $651 $238 file-once-flag ;

: old-mansion-b1-4.enter ( -- )   \ 0040C190
    room-sounds
    1 0 $1000000 nav-group
    1 0 $4000000 nav-group
    $326 story-flag? if
        0 1 $14 door-bits
        1 0 $20000 nav-group
        0 1 object-hidden
        1 1 object-hidden
    else
        0 0 $14 door-bits
        0 0 $20000 nav-group
        0 0 object-hidden
        1 0 object-hidden
    then
    $6F story-flag? not if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                $75 story-flag? not $76 story-flag? or if
                    6 ebit-set
                then
            then
        then
    then
    0 1 $20000 nav-group
    6 ebit? if
        stalkers-stay state-flag-set
        force-followed state-flag-set
        stalker-no-fear state-flag-set
        no-stalker-camera state-flag-set
        stalker-who char-activate
        $FE $50 7 2 stalker-to-room
        stalker-who 2 2 char-camera
        0 stalker-who $F action
    then
    0 -3.3 31.4 -4.3 0 flame-in
    1 -5.7 31.4 0.3 0 flame-in
    2 -2.8 31.7 5.0 0 flame-in
    3 2.9 31.1 -4.4 0 flame-in
    4 5.6 31.5 0.5 0 flame-in
    5 2.7 31.7 5.0 0 flame-in
    old-mansion-b1-4.cmd00
;

: old-mansion-b1-4.act0C ( -- )   \ 0040CA20
    4 ebit-set
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
    then
    stalker-who camera-follow
    2 0 var-set
    $11 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $11 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $11 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $11 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    stalker-who 2 stalker-command
    3 self-is? $22 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    2 0 var? if
        stalker-who 0 stalker-search-delay
        $78 1 summoner-wait
    else 2 1 var? if
        stalker-who 150 stalker-search-delay
        0 stalker-who $D action
    else 2 2 var? if
        stalker-who 300 stalker-search-delay
        0 stalker-who $E action
    then then then
    3 0 var-set
    $11 pvar-inc
    exit
;

: old-mansion-b1-4.char-enter ( -- )   \ 0040C270
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
    stalker-who self-is? if
        fiona-hidden state-flag? if
            4 ebit-set
            old-mansion-b1-4.act0C
        else
            4 ebit-clear
        then
    then
;

: old-mansion-b1-4.phase1 ( -- )   \ 0040C2C0
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    6 ebit? if
        1 -27.99 0.0 -19.65 $1E 8 0 zone
        7 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        7 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 hewie-who $92 action
                        then
                    then
                then
            then
        then
        1 -27.99 0.0 -19.65 $1E 8 0 zone
        hewie-who char-here? 1 game-mode? and if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    $1F 1 var-set
                    $1F 8.0 hewie-look-zone
                then
            then
        then
        6 sound-bank-loaded? 3 stalker-kind? and if
            3 $C0000004 6 char-sound
        then
    then
    2 19.24 0.0 -32.32 $16 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    3 -32.619 0.0 -33.47 $16 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 19.13 0.0 -1.45 $11 14 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000003 6 -32.0 0.0 -21.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000003 6 -32.0 0.0 -21.0 0 0 sound
    then
;

: old-mansion-b1-4.phase2 ( -- )   \ 0040C420
    fiona-who 5 char-in-area? fiona-who 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    fiona-who 6 char-in-area? fiona-who 45 $32 char-heading? and if
        5 1 0 scene-change
    then
    fiona-who 7 char-in-area? fiona-who -45 $32 char-heading? and if
        5 2 0 scene-change
    then
    fiona-who 8 char-in-area? fiona-who -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    fiona-who 9 char-in-area? fiona-who -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    6 ebit? not if
        fiona-who $A char-in-area? fiona-who -45 $32 char-heading? and if
            5 5 0 scene-change
        then
    then
    fiona-who $B char-in-area? fiona-who 90 $32 char-heading? and if
        5 6 0 scene-change
    then
    fiona-who $C char-in-area? fiona-who 0 $3C char-heading? and if
        6 ebit? if
            5 $12 0 scene-change
        else stalker-who char-here? not if
            5 7 5 scene-change
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then then
    then
    6 ebit? if
        fiona-who 3 $14 0 $2D char-touching-facing? if
            5 $10 0 scene-change
        then
    then
;

: old-mansion-b1-4.phase5 ( -- )   \ 0040C4C0
    6 ebit? if
        2 stalker-who 0 char-model-op
        stalker-who action-end
        stalker-who char-done
        stalkers-stay state-flag-clear
        no-stalker-camera state-flag-clear
        stalker-no-fear state-flag-clear
        force-followed state-flag-clear
    then
;

: old-mansion-b1-4.act00 ( -- )   \ 0040C4E0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $5F 18.0 -25.5 180 char-to-xz
    1 15.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    self-wait-16
    2 message   \ "A pair of creepy wax figures. These things aren't going to come to life like in the mov..."
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act01 ( -- )   \ 0040C530
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $1E 12.5 -3.0 90 char-to-xz
    1 1 player-camera
    1 1.0 45.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    3 message   \ "A chess board? Here of all places? Guess they used this to take their mind off things."
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act02 ( -- )   \ 0040C590
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who 6 -30.0 -33.5 -90 char-to-xz
    1 10.0 5.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    4 message   \ "A massive hourglass... Turning this over would be quite a task."
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act03 ( -- )   \ 0040C5E0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $71 -13.0 21.0 -90 char-to-xz
    1 15.0 -5.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    5 message   \ "There are models and their accompanying sketches. I haven't seen any of these bugs befo..."
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act04 ( -- )   \ 0040C630
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $55 -1.0 3.0 -90 char-to-xz
    0 0 player-camera
    1 5.0 20.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    6 message   \ "There is a flask... I wonder what kind of liquid is inside?"
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act05 ( -- )   \ 0040C690
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who 7 -28.0 -20.0 -90 char-to-xz
    1 15.0 10.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    7 message   \ "An industrial-sized kiln... It burns fiery hot."
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act06 ( -- )   \ 0040C6E0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $19 -2.0 -24.0 180 char-to-xz
    1 20.0 20.0 0.0 1.0 event-camera
    self-frames-reset
    self-wait-16
    8 message   \ "A device for distilling things..."
    wait-message
    self-frames-reset
    8 self-wait-frames
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act08 ( -- )   \ 0040C8D0
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $326 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act09 ( -- )   \ 0040C900
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
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    fiona-who 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    fiona-who 1 6 char-sound
    self-wait-anim
    0 self-noclip
    stalker-who char-here? if
        stalker-who self-look-at
        yield
        stalker-who self-turn-to
        self-wait-done
    then
    $326 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act0A ( -- )   \ 0040C950
    1 self-scripted
    counter-inc
    1 ebit-set
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? 0 0 var? and if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    fiona-who 0 6 char-sound
    self-wait-anim
    0 self-noclip
    stalker-who char-here? if
        stalker-who self-look-at
        yield
        stalker-who self-turn-to
        self-wait-done
    then
    $326 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-4.act07 ( -- )   \ 0040C730
    stalkers-stay state-flag-set
    4 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $326 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who $B action-force
        2 ebit-set
        hewie-who 3 char-file-load
    then
    fiona-who 2 char-file-load
    stalker-who 4 char-file-load
    $7B 19.42 25.02 0 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    2 ebit? if
        hewie-who self-look-at
        yield
        begin
            3 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    self-who self-look-at
    yield
    0 ebit? not if
        $8000 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-hidden
        1 0 object-hidden
        0 0 object-anim
        1 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-hidden
        1 0 object-hidden
        0 1 object-anim
        1 1 object-anim
        self-frames-reset
        $E self-wait-frames
        fiona-who 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 0 $20000 nav-group
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        fiona-who 2 6 char-sound
    then
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                fiona-who $43 5 char-sound
                stalker-who char-here? if
                    ['] old-mansion-b1-4.act08 goto
                else
                    ['] old-mansion-b1-4.act09 goto
                then
            else $FF panic-stage? if
                ['] old-mansion-b1-4.act0A goto
            then then
            2 panic-calm-in
            6 fiona-calm
            $1E fiona-recovery-lower
            4 ebit? stalker-who char-here? not and if
                4 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            stalker-who char-here? fiona-who stalker-who 50 chars-within? and if
                ['] old-mansion-b1-4.act0A goto
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
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            fiona-who 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            fiona-who 1 6 char-sound
            self-wait-anim
            0 self-noclip
            stalker-who char-here? if
                stalker-who self-look-at
                yield
                stalker-who self-turn-to
                self-wait-done
            then
            $326 story-flag-clear
            0 0 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-b1-4.act0B ( -- )   \ 0040C9A0
    1 self-scripted
    self-wait-done
    $23 24.09 21.5 0 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
        $8000 5 self-anim-blend8
        self-wait-anim
    else
        $8001 5 self-anim-blend8
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
                1 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    fiona-who 0 6 char-sound
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
                fiona-who 0 6 char-sound
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

: old-mansion-b1-4.act0D ( -- )   \ 0040CAF0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $23 21.86 24.14 0 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $19 self-wait-frames
    stalker-who 5 6 char-sound
    $28 threat-raise
    self-frames-reset
    9 self-wait-frames
    stalker-who 5 6 char-sound
    $14 threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 summoner-wait
    self-idle-or-end
;

: old-mansion-b1-4.act0E ( -- )   \ 0040CB60
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $23 21.86 24.14 0 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $3C 14.4 16.6 28 $FFFF 5 self-move-to
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
        stalker-who 0 6 char-sound
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        fiona-hidden state-flag? if
            2 avoid-prompt
            panic-eased
            0 fiona-who $A action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-b1-4.act0F ( -- )   \ 0040CC10
    self-wait-done
    1 self-noclip
    1 self-scripted
    stalker-who 7 -25.05 -20.33 -90 char-to-xz
    1 1 $20000 nav-group
    stalker-who 5 char-file-load
    stalker-who char-file-use
    2 stalker-who 1 char-model-op
    begin
        3 $40000004 6 char-sound
        $8000 self-anim
        self-wait-anim
    again
;

: old-mansion-b1-4.act10 ( -- )   \ 0040CC50
    $6F story-flag-set
    6 ebit-clear
    1 self-scripted
    self-wait-done
    $F $54 fade
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    stalker-who action-end
    3 4 0 char-model-op
    hewie-who char-here? if
        5 ebit-set
        hewie-who action-end
        hewie-who char-done
    then
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    stalker-who action-end
    stalker-who char-done
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    0 char-in
    fiona-who $57 -1.12 -15.98 -108 char-to-xz
    camera-restart
    world-held state-flag-set
    0 1 $20000 nav-group
    2 0 char-remove
    $22 partner-load
    2 char-start
    stalker-who char-activate
    $FE $50 7 2 stalker-to-room
    0 stalker-who $11 action
    world-held state-flag-clear
    5 ebit? if
        hewie-who char-activate
        $50 0 30 hewie-to-room
    then
    $F $51 fade
    wait-fade
    $2D resident-flag-set
    0 self-scripted
    force-followed state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-4.act11 ( -- )   \ 0040CD80
    stalkers-stay state-flag-clear
    force-followed state-flag-clear
    stalker-no-fear state-flag-clear
    no-stalker-camera state-flag-clear
    stalker-who 7 -24.52 -20.6 -68 char-to-xz
    stalker-who 2 2 char-camera
    self-idle-or-end
;

: old-mansion-b1-4.act12 ( -- )   \ 0040CDA8
    self-wait-done
    stalker-who self-look-at
    yield
    9 message   \ "She's right there, stupid. What good is hiding in the closet going to do now?"
    wait-message
    self-idle-or-end
;

: old-mansion-b1-4.act13 ( -- )   \ 0040CDC0
    0 -3.3 31.4 -4.3 0 flame-in
    1 -5.7 31.4 0.3 0 flame-in
    2 -2.8 31.7 5.0 0 flame-in
    3 2.9 31.1 -4.4 0 flame-in
    4 5.6 31.5 0.4 0 flame-in
    5 2.7 31.7 5.0 0 flame-in
    self-wait-done
    3 partner-load
    2 char-start
    2 stalker-who 1 char-model-op
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    3 4 0 char-model-op
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-b1-4.enter old-mansion-b1-4 0 room-script!
' old-mansion-b1-4.char-enter old-mansion-b1-4 6 room-script!
' old-mansion-b1-4.phase1 old-mansion-b1-4 1 room-script!
' old-mansion-b1-4.phase2 old-mansion-b1-4 2 room-script!
' old-mansion-b1-4.phase5 old-mansion-b1-4 5 room-script!
' old-mansion-b1-4.act00 old-mansion-b1-4 $00 action-script!
' old-mansion-b1-4.act01 old-mansion-b1-4 $01 action-script!
' old-mansion-b1-4.act02 old-mansion-b1-4 $02 action-script!
' old-mansion-b1-4.act03 old-mansion-b1-4 $03 action-script!
' old-mansion-b1-4.act04 old-mansion-b1-4 $04 action-script!
' old-mansion-b1-4.act05 old-mansion-b1-4 $05 action-script!
' old-mansion-b1-4.act06 old-mansion-b1-4 $06 action-script!
' old-mansion-b1-4.act07 old-mansion-b1-4 $07 action-script!
' old-mansion-b1-4.act08 old-mansion-b1-4 $08 action-script!
' old-mansion-b1-4.act09 old-mansion-b1-4 $09 action-script!
' old-mansion-b1-4.act0A old-mansion-b1-4 $0A action-script!
' old-mansion-b1-4.act0B old-mansion-b1-4 $0B action-script!
' old-mansion-b1-4.act0C old-mansion-b1-4 $0C action-script!
' old-mansion-b1-4.act0D old-mansion-b1-4 $0D action-script!
' old-mansion-b1-4.act0E old-mansion-b1-4 $0E action-script!
' old-mansion-b1-4.act0F old-mansion-b1-4 $0F action-script!
' old-mansion-b1-4.act10 old-mansion-b1-4 $10 action-script!
' old-mansion-b1-4.act11 old-mansion-b1-4 $11 action-script!
' old-mansion-b1-4.act12 old-mansion-b1-4 $12 action-script!
' old-mansion-b1-4.act13 old-mansion-b1-4 $13 action-script!
