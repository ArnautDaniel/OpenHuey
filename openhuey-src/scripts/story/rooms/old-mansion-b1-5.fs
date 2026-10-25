\ story/rooms/old-mansion-b1-5.fs - the event scripts of room old-mansion-b1-5 ($51; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-5
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant first-time                  \ (bit 0) the story flag not yet set (the first time here)
1 constant hiding-left                 \ (bit 1) she has come out of hiding (act0C)
2 constant hewie-along                 \ (bit 2) Hewie came along with her (near, commandable)
3 constant hewie-in-place              \ (bit 3) Hewie at his place for the scene
4 constant came-while-hidden           \ (bit 4) the stalker came in while she was hiding
5 constant hewie-zone0-noticed         \ (bit 5) Hewie has reacted to zone 0 (once a visit)
6 constant hewie-zone1-noticed         \ (bit 6) Hewie has reacted to zone 1 (once a visit)
7 constant stalker-waiting             \ (bit 7) the stalker here, not yet about: she is followed
8 constant sound-started               \ (bit 8) its sound started (then kept up)
9 constant hewie-away                  \ (bit 9) Hewie not here (or sent off) as the scene ends
0 constant stage-var                   \ (var 0) ? its scene's stage (2: hiding over)
1 constant held-var                    \ (var 1) ? she is held (1)
2 constant search-result-var           \ (var 2) his search's outcome by chance: 0, 1 or 2
3 constant searched-var                \ (var 3) ? his search done (0 / 1)
0 constant hide-open-sound             \ (snd 0) the hiding place opened
1 constant hide-shut-sound             \ (snd 1) the hiding place closed on her
2 constant hide-sound                  \ (snd 2) her hiding
3 constant room-loop-sound             \ (snd 3) its steady sound (started as it comes in, then kept up)
5 constant grab-sound                  \ (snd 5) him grabbing her

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
                    stalker-waiting ebit-set
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
    came-while-hidden ebit-set
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
    then
    stalker-who camera-follow
    search-result-var 0 var-set
    $12 0 pvar? if
        $A chance? if
            search-result-var 2 var-set
        then
    else $12 1 pvar? if
        $19 chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    else $12 2 pvar? if
        $32 chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    else $12 3 pvar? if
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
        0 stalker-who $F action
    else search-result-var 2 var? if
        stalker-who 300 stalker-search-delay
        0 stalker-who $10 action
    then then then
    searched-var 0 var-set
    $12 pvar-inc
    exit
;

: old-mansion-b1-5.char-enter ( -- )   \ 0040D000
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    stalker-who self-is? if
        stalker-waiting ebit-clear
        force-followed state-flag-clear
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            old-mansion-b1-5.act0E
        else
            came-while-hidden ebit-clear
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
        hewie-zone0-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        hewie-zone0-noticed ebit-set
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
    hewie-zone1-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    hewie-zone1-noticed ebit-set
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
    sound-started ebit? not if
        6 sound-bank-loaded? if
            room-loop-sound volume-scaled 6 -32.0 0.0 21.0 0 0 sound
            sound-started ebit-set
        then
    else
        room-loop-sound kept-going volume-scaled 6 -32.0 0.0 21.0 0 0 sound
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
            stalker-waiting ebit? $52 doors:door-locked? not and if
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
        4 wait-frames
        world-frozen state-flag-set
        fiona-who $5F 18.0 25.5 0 char-to-xz
        1 15.0 10.0 0.0 0.0 event-camera
        16 wait-frames
        2 say   \ "A pair of creepy wax figures. These things aren't going to come to life like in the..."
        8 wait-frames
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
                hewie-away ebit-clear
                hewie-who 2 char-cond? if
                    0 hewie-who $86 action
                else
                    hewie-who action-end
                    hewie-who char-done
                then
            else
                hewie-away ebit-set
            then
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            wait-fade
            engine:things-clear
            0 self-idle-anim
            fiona-who $27 15.929 21.234 -2 char-to-xz
            camera-restart
            hewie-away ebit? not if
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
            1 say   \ "Did something just move?"
            $52 doors:door-locked? if
                16 wait-frames
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
        0 say   \ "I've already searched this. No need to do it again."
    then
    self-idle-or-end
;

: old-mansion-b1-5.act01 ( -- )   \ 0040D450
    omb1-5-flag2 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        4 wait-frames
        world-frozen state-flag-set
        fiona-who $1E 12.5 3.0 90 char-to-xz
        2 2 player-camera
        1 1.0 45.0 0.0 5.0 event-camera
        16 wait-frames
        3 say   \ "A chess board? Here of all places? Guess they used this to take their mind off thin..."
        8 wait-frames
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
            1 say   \ "Did something just move?"
            $52 doors:door-locked? if
                16 wait-frames
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
        0 say   \ "I've already searched this. No need to do it again."
    then
    self-idle-or-end
;

: old-mansion-b1-5.act02 ( -- )   \ 0040D5E0
    omb1-5-flag3 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        4 wait-frames
        world-frozen state-flag-set
        fiona-who 6 -30.0 33.5 -90 char-to-xz
        1 10.0 5.0 0.0 2.0 event-camera
        16 wait-frames
        4 say   \ "A massive hourglass... Turning this over would be quite a task. Search more thoroug..."
        8 wait-frames
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
            engine:things-clear
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
            1 say   \ "Did something just move?"
            $52 doors:door-locked? if
                16 wait-frames
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
        0 say   \ "I've already searched this. No need to do it again."
    then
    self-idle-or-end
;

: old-mansion-b1-5.act08 ( -- )   \ 0040D8E0
    8 wait-frames
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
            16 wait-frames
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
        16 wait-frames
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
    4 wait-frames
    world-frozen state-flag-set
    fiona-who $71 -13.0 -21.0 -90 char-to-xz
    1 15.0 -5.0 0.0 5.0 event-camera
    16 wait-frames
    5 say   \ "There are models and their accompanying sketches. I haven't seen any of these bugs befo..."
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act04 ( -- )   \ 0040D790
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 wait-frames
    world-frozen state-flag-set
    fiona-who $55 -1.0 -3.0 -90 char-to-xz
    0 0 player-camera
    1 5.0 20.0 0.0 5.0 event-camera
    16 wait-frames
    6 say   \ "There is a flask... I wonder what kind of liquid is inside? Search more thoroughly? YES NO"
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act05 ( -- )   \ 0040D7E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 wait-frames
    world-frozen state-flag-set
    fiona-who 7 -28.0 20.0 -90 char-to-xz
    1 15.0 10.0 0.0 2.0 event-camera
    16 wait-frames
    7 say   \ "An industrial-sized kiln... It burns fiery hot. Search more thoroughly? YES NO"
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act06 ( -- )   \ 0040D820
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 wait-frames
    world-frozen state-flag-set
    fiona-who $19 -2.0 24.0 0 char-to-xz
    1 20.0 20.0 0.0 1.0 event-camera
    16 wait-frames
    8 say   \ "A device for distilling things... Search more thoroughly? YES NO"
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
    0 cutscene-track $13 action
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
    [: stage-var 2 var? ;] wait-until
    counter-inc
    2 avoid-prompt
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    3 room-motion 0 blend-out
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
    omb1-5-flag4 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act0C ( -- )   \ 0040DBA0
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
    8 4 object-anim
    9 4 object-anim
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
    omb1-5-flag4 story-flag-set
    0 barred-floor floor-on
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act09 ( -- )   \ 0040D970
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    stalker-waiting ebit-clear
    1 self-scripted
    self-wait-done
    stage-var 0 var-set
    held-var 0 var-set
    omb1-5-flag4 story-flag? not if
        first-time ebit-set
    else
        first-time ebit-clear
    then
    0 counter-set
    hewie-along ebit-clear
    hewie-in-place ebit-clear
    hiding-left ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who $D action-force
        hewie-along ebit-set
        hewie-who $B char-file-load
    then
    fiona-who $A char-file-load
    stalker-who $C char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
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
        8 0 object-hidden
        9 0 object-hidden
        8 0 object-anim
        9 0 object-anim
        $28 wait-frames
        fiona-who hide-shut-sound 6 char-sound
        self-wait-anim
    else
        1 room-motion 5 self-anim-blend8
        5 wait-frames
        0 part-hidden
        8 0 object-hidden
        9 0 object-hidden
        8 1 object-anim
        9 1 object-anim
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
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        fiona-who hide-sound 6 char-sound
    then
    begin
        0 2 pad? not held-var 1 var? or if
            stage-var 1 var? if
                held-var 0 var-set
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
            came-while-hidden ebit? stalker-who char-here? not and if
                came-while-hidden ebit-clear
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

: old-mansion-b1-5.act0F ( -- )   \ 0040DD40
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $7B 14.14 -24.14 180 $FFFF 5 self-move-to
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

: old-mansion-b1-5.act10 ( -- )   \ 0040DDB0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        searched-var 0 var? if
            $7B 14.14 -24.14 180 $FFFF 5 self-move-to
            self-wait-done
        then
        searched-var 0 var-set
    else
        $1D 9.84 -11.27 153 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    stage-var 1 var-set
    3 self-is? $22 self-is? or if
        8 3 object-anim
        9 3 object-anim
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
            0 fiona-who $C action-force
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
    0 room-motion $A blend-out
    3 room-motion $A blend-out
    counter-inc
    stalkers-stay state-flag-clear
    force-followed state-flag-clear
    stalker-waiting ebit-clear
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
    7 wait-frames
    fiona-who hide-open-sound 6 char-sound
    self-wait-anim
    2 wait-counter
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act13 ( -- )   \ 0040DF30
    [: 3 cutscene-shot? ;] wait-until
    fiona-who 1 char-no-shadow
    [: 4 cutscene-shot? ;] wait-until
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
    0 cutscene-track $13 action
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
