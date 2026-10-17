\ story/rooms/old-mansion-b1-5.fs - the event scripts of room old-mansion-b1-5 ($51; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-5
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ room 0x51 (Room51_Cmd00_ptmf): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves
: old-mansion-b1-5.cmd00 ( b0 -- )  0= if  0 1 door-play  else  0 door-wait  then ;   \ (door 0: b0 0 swung, else waited for)
: old-mansion-b1-5.cmd01 ( -- )  $650 $238 file-once-flag ;

: old-mansion-b1-5.enter ( -- )   \ 0040CF40
    room-sounds
    0 keeps-hewie-off floor-on
    0 keeps-stalkers-off floor-on
    omb1-5-flag4 story-flag? if
        0 part-shown
        0 barred-floor floor-on
        8 1 object-hidden
        9 1 object-hidden
    else
        0 part-hidden
        0 barred-floor floor-off
        8 0 object-hidden
        9 0 object-hidden
    then
    omb1-5-flag5 story-flag? not omb1-5-flag4 story-flag? not and if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                om1f-13-flag2 story-flag? not white-mandragora-used story-flag? or if
                    7 ebit-set
                    force-followed state-flag-set
                then
            then
        then
    then
    0 -2.8 31.7 -5.1 0 flame-in
    1 -5.7 31.4 -0.3 0 flame-in
    2 -3.3 31.4 4.3 0 flame-in
    3 2.7 31.7 -5.0 0 flame-in
    4 5.6 31.5 -0.5 0 flame-in
    5 2.9 31.2 4.4 0 flame-in
    old-mansion-b1-5.cmd01
;

: old-mansion-b1-5.act0E ( -- )   \ 0040DC70
    4 ebit-set
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
    then
    stalker-who camera-follow
    2 0 var-set
    $12 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $12 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $12 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $12 3 pvar? if
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
        0 stalker-who $F action
    else 2 2 var? if
        stalker-who 300 stalker-search-delay
        0 stalker-who $10 action
    then then then
    3 0 var-set
    $12 pvar-inc
    exit
;

: old-mansion-b1-5.char-enter ( -- )   \ 0040D000
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
        7 ebit-clear
        force-followed state-flag-clear
        fiona-hidden state-flag? if
            4 ebit-set
            old-mansion-b1-5.act0E
        else
            4 ebit-clear
        then
    then
;

: old-mansion-b1-5.phase1 ( -- )   \ 0040D060
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    omb1-5-flag5 story-flag? not omb1-5-flag4 story-flag? not and if
        0 17.96 0.0 -27.69 $F 18 0 zone
        5 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            hewie-zone-var 0 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
        0 17.96 0.0 -27.69 $F 18 0 zone
        hewie-who char-here? 2 game-mode? not and if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    hewie-zone-var 0 var-set
                    hewie-zone-var 18.0 hewie-look-zone
                then
            then
        then
    then
    1 1.58 0.0 -9.28 $10 8 0 zone
    6 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    6 ebit-set
                    $64 chance? if
                        hewie-zone-var 1 var-set
                        0 hewie-who $92 action
                    then
                then
            then
        then
    then
    2 17.16 0.0 30.67 $16 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
    3 -33.99 0.0 32.91 $16 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
    4 15.24 0.0 -0.28 $11 14 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 14.0 hewie-look-zone
            then
        then
    then
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000003 6 -32.0 0.0 21.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000003 6 -32.0 0.0 21.0 0 0 sound
    then
;

: old-mansion-b1-5.phase2 ( -- )   \ 0040D1F0
    fiona-who 5 char-in-area? fiona-who 0 $32 char-heading? and if
        0 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who 45 $32 char-heading? and if
        1 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who -45 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who -45 $32 char-heading? and if
        4 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who -45 $32 char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 0 $32 char-heading? and if
        6 unhurried offer-action
    then
    -2147483646 scene-request? if
        7 always offer-action
    then
    fiona-who $C char-in-area? fiona-who 90 $3C char-heading? and if
        stalker-who char-here? not if
            7 ebit? $52 door-locked? not and if
                $11 calm offer-action
            else
                9 calm offer-action
            then
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
;

: old-mansion-b1-5.act00 ( -- )   \ 0040D290
    omb1-5-flag1 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        world-frozen state-flag-set
        fiona-who $5F 18.0 25.5 0 char-to-xz
        1 15.0 10.0 0.0 0.0 event-camera
        self-frames-reset
        self-wait-16
        2 message   \ "A pair of creepy wax figures. These things aren't going to come to life like in the..."
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
            1 0 movie-play
            0 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            world-frozen state-flag-clear
            hewie-who char-here? if
                9 ebit-clear
                hewie-who 2 char-cond? if
                    0 hewie-who $86 action
                else
                    hewie-who action-end
                    hewie-who char-done
                then
            else
                9 ebit-set
            then
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            wait-fade
            things-clear
            0 self-idle-anim
            fiona-who $27 15.929 21.234 -2 char-to-xz
            camera-restart
            9 ebit? not if
                hewie-who 2 char-cond? if
                    hewie-who 0 char-visible
                    hewie-who action-end
                else
                    hewie-who char-activate
                    $51 0 86 hewie-to-room
                    hewie-who $56 1.98 5.93 -2 char-to-xz
                then
            then
            $4D $68 door-copy
            $68 door-close-off-lock
            $4E $69 door-copy
            $69 door-close-off-lock
            $50 $6A door-copy
            $6A door-close-off-lock
            $53 $6B door-copy
            $6B door-close-off-lock
            exits-rebuild
            hewie-who $4D char-in-room? if
                $68 0 -1 hewie-to-room
            then
            stalker-who char-here? not stalker-active? and stalker-who 2 char-cond? not and if
                $FE $4B -1 2 stalker-to-room
            then
            omb1-5-flag1 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $F $41 fade
            wait-fade
            1 message   \ "Did something just move?"
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            world-frozen state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message   \ "I've already searched this. No need to do it again."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-b1-5.act01 ( -- )   \ 0040D450
    omb1-5-flag2 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        world-frozen state-flag-set
        fiona-who $1E 12.5 3.0 90 char-to-xz
        2 2 player-camera
        1 1.0 45.0 0.0 5.0 event-camera
        self-frames-reset
        self-wait-16
        3 message   \ "A chess board? Here of all places? Guess they used this to take their mind off thin..."
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
            3 0 movie-play
            2 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            hewie-who char-here? hewie-who 2 char-cond? and if
                0 hewie-who $86 action
            then
            world-frozen state-flag-clear
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            wait-fade
            0 self-idle-anim
            fiona-who $55 10.203 -1.688 57 char-to-xz
            0 0 player-camera
            camera-restart
            hewie-who 0 char-visible
            hewie-who action-end
            $58 $82 door-copy
            $82 door-close-off-lock
            $5A $83 door-copy
            $83 door-close-off-lock
            exits-rebuild
            hewie-who $5F 0 room-doors-state
            stalker-who $5F 0 room-doors-state
            omb1-5-flag2 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $2B resident-flag-set
            $F $41 fade
            wait-fade
            1 message   \ "Did something just move?"
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            world-frozen state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message   \ "I've already searched this. No need to do it again."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-b1-5.act02 ( -- )   \ 0040D5E0
    omb1-5-flag3 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        world-frozen state-flag-set
        fiona-who 6 -30.0 33.5 -90 char-to-xz
        1 10.0 5.0 0.0 2.0 event-camera
        self-frames-reset
        self-wait-16
        4 message   \ "A massive hourglass... Turning this over would be quite a task. Search more thoroug..."
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
            5 1 movie-play
            4 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            hewie-who char-here? hewie-who 2 char-cond? and if
                0 hewie-who $86 action
            then
            world-frozen state-flag-clear
            things-clear
            $10 $78 movie-param
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            wait-fade
            0 self-idle-anim
            fiona-who 6 -24.369 32.751 -92 char-to-xz
            camera-restart
            hewie-who 0 char-visible
            hewie-who action-end
            $78 door-unlock
            $5C $88 door-copy
            $5C door-reopen-unlock
            $88 door-close-off-lock
            exits-rebuild
            stalker-who char-here? not stalker-active? and stalker-who 2 char-cond? not and if
                $FE $4B -1 2 stalker-to-room
            then
            omb1-5-flag3 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $F $41 fade
            wait-fade
            1 message   \ "Did something just move?"
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            world-frozen state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message   \ "I've already searched this. No need to do it again."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-b1-5.act08 ( -- )   \ 0040D8E0
    self-frames-reset
    8 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 answer? if
        reach-out-anim self-anim
        0 exit-door-open? if
            0 old-mansion-b1-5.cmd00
            self-wait-anim
            0.0 10.0 -40.0 self-look-at-point
            yield
            reach-back-anim self-anim
            1 old-mansion-b1-5.cmd00
            self-frames-reset
            self-wait-16
        else
            self-wait-anim
            0.0 10.0 -40.0 self-look-at-point
            yield
            reach-back-anim self-anim
        then
        $72 5 0.0 0.0 -40.0 0 0 sound
        $52 door-open-clear
        doors-room-in
        $52 door-lock
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        self-wait-16
        self-who self-look-at
        yield
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act03 ( -- )   \ 0040D750
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    fiona-who $71 -13.0 -21.0 -90 char-to-xz
    1 15.0 -5.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    5 message   \ "There are models and their accompanying sketches. I haven't seen any of these bugs befo..."
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act04 ( -- )   \ 0040D790
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    fiona-who $55 -1.0 -3.0 -90 char-to-xz
    0 0 player-camera
    1 5.0 20.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    6 message   \ "There is a flask... I wonder what kind of liquid is inside? Search more thoroughly? YES NO"
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act05 ( -- )   \ 0040D7E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    fiona-who 7 -28.0 20.0 -90 char-to-xz
    1 15.0 10.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    7 message   \ "An industrial-sized kiln... It burns fiery hot. Search more thoroughly? YES NO"
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act06 ( -- )   \ 0040D820
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-set
    fiona-who $19 -2.0 24.0 0 char-to-xz
    1 20.0 20.0 0.0 1.0 event-camera
    self-frames-reset
    self-wait-16
    8 message   \ "A device for distilling things... Search more thoroughly? YES NO"
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act07 ( -- )   \ 0040D860
    capture-no-end state-flag-set
    no-pause state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $52 self-through-door
    self-wait-done
    $F $44 fade
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    0 $F9 $13 action
    $10 $28 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        movie-loop
        scene-play-through
    then
    $2C resident-flag-set
    fiona-who 0 char-no-shadow
    0 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: old-mansion-b1-5.act0A ( -- )   \ 0040DB20
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
    3 room-motion 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    omb1-5-flag4 story-flag-set
    0 barred-floor floor-on
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act0B ( -- )   \ 0040DB50
    1 self-scripted
    counter-inc
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    8 2 object-anim
    9 2 object-anim
    2 room-motion 0 self-anim-blend
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
    omb1-5-flag4 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act0C ( -- )   \ 0040DBA0
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
    8 4 object-anim
    9 4 object-anim
    4 room-motion 0 self-anim-blend
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
    omb1-5-flag4 story-flag-set
    0 barred-floor floor-on
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act09 ( -- )   \ 0040D970
    stalkers-stay state-flag-set
    4 ebit-clear
    7 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    omb1-5-flag4 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who $D action-force
        2 ebit-set
        hewie-who $B char-file-load
    then
    fiona-who $A char-file-load
    stalker-who $C char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
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
        0 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 part-hidden
        8 0 object-hidden
        9 0 object-hidden
        8 0 object-anim
        9 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    else
        1 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 part-hidden
        8 0 object-hidden
        9 0 object-hidden
        8 1 object-anim
        9 1 object-anim
        self-frames-reset
        $E self-wait-frames
        fiona-who 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 barred-floor floor-off
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
                fiona-who seized-sound 5 char-sound
                stalker-who char-here? if
                    ['] old-mansion-b1-5.act0A goto
                else
                    ['] old-mansion-b1-5.act0B goto
                then
            else in-panic? if
                ['] old-mansion-b1-5.act0C goto
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
                ['] old-mansion-b1-5.act0C goto
            then
            counter-inc
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            stalker-who char-here? if
                stalker-who 0 stalker-command
                stalker-who action-end
            then
            1 self-noclip
            8 2 object-anim
            9 2 object-anim
            2 room-motion 0 self-anim-blend
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
            omb1-5-flag4 story-flag-clear
            0 barred-floor floor-off
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-b1-5.act0D ( -- )   \ 0040DBF0
    1 self-scripted
    self-wait-done
    $7D 11.91 -21.5 180 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
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
                1 ebit? if
                    4 room-motion 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    fiona-who 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    2 room-motion 0 self-anim-blend
                    self-wait-anim
                else
                    3 room-motion 0 self-anim-blend
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
                4 room-motion 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                fiona-who 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                2 room-motion 0 self-anim-blend
                self-wait-anim
            else
                3 room-motion 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-b1-5.act0F ( -- )   \ 0040DD40
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $7B 14.14 -24.14 180 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    2 room-motion $A self-anim-blend
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
    step-back-anim self-anim
    self-wait-anim
    0 self-scripted
    $78 1 summoner-wait
    self-idle-or-end
;

: old-mansion-b1-5.act10 ( -- )   \ 0040DDB0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $7B 14.14 -24.14 180 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $1D 9.84 -11.27 153 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    0 1 var-set
    3 self-is? $22 self-is? or if
        8 3 object-anim
        9 3 object-anim
        1 room-motion $A self-anim-blend
        self-frames-reset
        $10 self-wait-frames
        0 2 var-set
        self-frames-reset
        $A self-wait-frames
        stalker-who 0 6 char-sound
        self-wait-anim
        step-back-anim self-anim
        self-wait-anim
        0 self-scripted
    else
        there-anim self-anim
        self-wait-anim
        fiona-hidden state-flag? if
            2 avoid-prompt
            panic-eased
            0 fiona-who $C action-force
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

: old-mansion-b1-5.act11 ( -- )   \ 0040DE60
    1 self-scripted
    stalkers-stay state-flag-set
    force-followed state-flag-set
    self-wait-done
    fiona-who $D char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-activate
    $FE $51 7 2 stalker-to-room
    stalker-who 0 0 char-camera
    summoner-own-cooldown
    stalker-who 1 char-visible
    stalker-who 1 char-silent
    fiona-who char-file-use
    0 counter-set
    0 stalker-who $12 action
    1 wait-counter
    fiona-who seized-sound 5 char-sound
    0 room-motion $A self-anim-blend
    self-wait-anim
    3 room-motion $A self-anim-blend
    self-wait-anim
    counter-inc
    stalkers-stay state-flag-clear
    force-followed state-flag-clear
    7 ebit-clear
    omb1-5-flag5 story-flag-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act12 ( -- )   \ 0040DED0
    1 self-noclip
    1 self-scripted
    self-wait-done
    stalker-who $77 15.58 -33.18 0 char-to-xz
    1 self-noclip
    1 self-scripted
    stalker-who $C char-file-load
    stalker-who 0 char-visible
    stalker-who 0 char-silent
    0 part-hidden
    stalker-who char-file-use
    omb1-5-flag4 story-flag-set
    0 barred-floor floor-on
    counter-inc
    8 0 object-hidden
    9 0 object-hidden
    8 6 object-anim
    9 6 object-anim
    3 room-motion self-anim
    self-frames-reset
    7 self-wait-frames
    fiona-who 0 6 char-sound
    self-wait-anim
    2 wait-counter
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act13 ( -- )   \ 0040DF30
    begin
        3 cutscene-shot? not while
        yield
    repeat
    fiona-who 1 char-no-shadow
    begin
        4 cutscene-shot? not while
        yield
    repeat
    fiona-who 0 char-no-shadow
    self-idle-or-end
;

: old-mansion-b1-5.act14 ( -- )   \ 0040DF50
    self-wait-done
    0 -2.8 31.7 -5.1 0 flame-in
    1 -5.7 31.4 -0.3 0 flame-in
    2 -3.3 31.4 4.3 0 flame-in
    3 2.7 31.7 -5.0 0 flame-in
    4 5.6 31.5 -0.4 0 flame-in
    5 2.9 31.2 4.4 0 flame-in
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-b1-5.act15 ( -- )   \ 0040E030
    self-wait-done
    0 -2.8 31.7 -5.1 0 flame-in
    1 -5.7 31.4 -0.3 0 flame-in
    2 -3.3 31.4 4.3 0 flame-in
    3 2.7 31.7 -5.0 0 flame-in
    4 5.6 31.5 -0.4 0 flame-in
    5 2.9 31.2 4.4 0 flame-in
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-b1-5.act16 ( -- )   \ 0040E110
    self-wait-done
    0 -2.8 31.7 -5.1 0 flame-in
    1 -5.7 31.4 -0.3 0 flame-in
    2 -3.3 31.4 4.3 0 flame-in
    3 2.7 31.7 -5.0 0 flame-in
    4 5.6 31.5 -0.4 0 flame-in
    5 2.9 31.2 4.4 0 flame-in
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $78 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-b1-5.act17 ( -- )   \ 0040E1F0
    self-wait-done
    0 -2.8 31.7 -5.1 0 flame-in
    1 -5.7 31.4 -0.3 0 flame-in
    2 -3.3 31.4 4.3 0 flame-in
    3 2.7 31.7 -5.0 0 flame-in
    4 5.6 31.5 -0.4 0 flame-in
    5 2.9 31.2 4.4 0 flame-in
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $13 action
    $10 $28 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-b1-5.phase5 ( -- )   \ 0047AB98
    force-followed state-flag-clear
;

\ ---- registered ----
old-mansion-b1-5 register-scripts   \ (its phases and actions, by their names)
