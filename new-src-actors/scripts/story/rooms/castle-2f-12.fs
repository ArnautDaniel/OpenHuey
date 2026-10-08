\ story/rooms/castle-2f-12.fs - the event scripts of room castle-2f-12 ($26; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-12
USING: room-names story.words story.shared flag-names ;

\ the two doors ("left", "right") opening: byte 3 0 at once (2.25), else a step (0.075)
: castle-2f-12.cmd00 ( b0 -- )  drop s" castle-2f-12.cmd00" stub-step ;
\ the three rocking chairs ("movechair_1..3"; +0x30 the rock's phase in degrees, +0x34 its size,
\ +0x38 how fast it dies down; +0x10 the tilt), by byte 3: 0 all still; 1 a rocking step (6
\ degrees; each swing smaller, the first chair creaking at a volume by its size); 2 / 3 set
\ rocking at full size from their tilt now (swinging forward / back), with a creak
: castle-2f-12.cmd01 ( b0 -- )  drop s" castle-2f-12.cmd01" stub-step ;
\ the partner's target (+0xF35E0 on, +0xF35F0) 3 above the rocking chair (movechair_2): its seat
\ 6 ahead, tipped by its rock (90 x +0x34 x sin +0x30 degrees) and turned with it
: castle-2f-12.cmd02 ( -- )  s" castle-2f-12.cmd02" stub-step ;
\ the first rocking chair still rocking (+0x34 over 0.3)
: castle-2f-12.cond00? ( -- flag )  s" castle-2f-12.cond00?" stub-flag ;

: castle-2f-12.enter ( -- )   \ 00402D50
    room-sounds
    1 1 $1000000 nav-group
    1 1 $4000000 nav-group
    $305 story-flag? if
        0 1 $14 door-bits
        1 1 $20000 nav-group
        0 1 object-show
        1 1 object-show
    else
        0 0 $14 door-bits
        0 1 $20000 nav-group
        0 0 object-show
        1 0 object-show
    then
    0 castle-2f-12.cmd01
    0 $F1 $13 action
    $30E story-flag? not if
        2 castle-2f-12.cmd01
        $30E story-flag-set
    then
    $202 story-flag? not if
        1 0 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        $28C story-flag? not if
            1 -17.5 1.0 47.0 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    $258 story-flag? $259 story-flag? not and if
        2 22.9 1.0 -9.7 flicker-sprite
    then
    $30C story-flag? if
        0 castle-2f-12.cmd00
    then
;

: castle-2f-12.act0D ( -- )   \ 00403680
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    6 0 pvar? if
        0 chance? if
            2 2 var-set
        then
    else 6 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else 6 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else 6 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    2 self-is? 6 self-is? or 7 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    4 stalker-alert? if
        2 0 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE 4 action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE 2 action
    then then then
    3 0 var-set
    6 pvar-inc
    exit
;

: castle-2f-12.char-enter ( -- )   \ 00402E10
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
    $FE self-is? if
        fiona-hidden state-flag? if
            7 ebit-set
            2 self-is? 6 self-is? or 7 self-is? or if
                $318 story-flag? not castle-2f-12.cond00? and 2 creature-action? not and if
                    $FE camera-follow
                    0 $FE $E action
                else
                    castle-2f-12.act0D
                then
            else
                castle-2f-12.act0D
            then
        else
            7 ebit-clear
        then
    then
;

: castle-2f-12.phase1 ( -- )   \ 00402EC0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 -1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 -4.0 0.0 4.0 4 7 0 zone
    1 6.0 0.0 -7.0 3 7 0 zone
    $258 story-flag? not if
        3 22.9 0.0 -9.7 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 600 var-set
                $1A 601 var-set
                $1B 2 var-set
                $1C 22900 var-set
                $1D 1000 var-set
                $1E -9700 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    8 ebit? not if
        1 char-here? 1 2 char-C4? not and if
            4 2.09 0.0 -2.09 $14 5 0 zone
            0 game-mode? if
                hewie-can-command? castle-2f-12.cond00? and if
                    1 4 9 char-zone-bits? if
                        4 0 var-set
                        0 1 $11 action
                    then
                then
            then
        then
    then
;

: castle-2f-12.phase2 ( -- )   \ 00402FA0
    0 6 char-in-area? 0 22 $3C char-heading? and if
        $FE char-here? not if
            5 0 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 7 $3C char-faces-area? if
        5 6 0 scene-change
    then
    0 8 char-in-area? 0 75 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 9 $3C char-faces-area? if
        5 7 0 scene-change
    then
    0 $A char-in-area? 0 90 $32 char-heading? and if
        5 $A 0 scene-change
    then
    0 $B $3C char-faces-area? if
        5 $10 0 scene-change
    then
    $258 story-flag? $259 story-flag? not and if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $F 4 scene-change
        then
    then
    $202 story-flag? not if
        2 -17.5 0.0 47.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $202 story-flag-set
            0 0 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -17.5 0.0 47.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -17.5 0.0 47.0 0 0 sound
            $40 $48 noise
            1 -17.5 1.0 47.0 flicker-sprite
        then
    else $28C story-flag? not if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then then
;

: castle-2f-12.act03 ( -- )   \ 00403350
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
    $305 story-flag-set
    1 1 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act05 ( -- )   \ 00403400
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
    $305 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act09 ( -- )   \ 004034A0
    1 self-scripted
    counter-inc
    4 ebit-set
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
    $305 story-flag-set
    1 1 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act00 ( -- )   \ 004030A0
    stalkers-stay state-flag-set
    7 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $305 story-flag? not if
        3 ebit-set
    else
        3 ebit-clear
    then
    0 counter-set
    5 ebit-clear
    6 ebit-clear
    4 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 1 action-force
        5 ebit-set
        1 5 char-file-load
    then
    0 4 char-file-load
    $FE 6 char-file-load
    $B4 16.729 33.985 45 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    5 ebit? if
        1 self-look-at
        yield
        begin
            6 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    3 ebit? not if
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
    0 1 $20000 nav-group
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] castle-2f-12.act03 goto
                else
                    ['] castle-2f-12.act05 goto
                then
            else $FF panic-stage? if
                ['] castle-2f-12.act09 goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            7 ebit? $FE char-here? not and if
                7 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] castle-2f-12.act09 goto
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
            $305 story-flag-clear
            0 1 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-12.act01 ( -- )   \ 00403230
    1 self-scripted
    self-wait-done
    $B4 17.237 27.889 45 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    6 ebit-set
    1 wait-counter
    1 self-noclip
    3 ebit? not if
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
                4 ebit? if
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
            4 ebit? if
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

: castle-2f-12.act02 ( -- )   \ 004032B0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    3 0 var? if
        $B4 15.0 29.0 45 $FFFF 5 self-move-to
        self-wait-done
    then
    3 0 var-set
    1 self-scripted
    0 1 var-set
    2 self-is? 6 self-is? or 7 self-is? or if
        0 3 object-anim
        1 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $C self-wait-frames
        $FE 0 6 char-sound
        self-frames-reset
        4 self-wait-frames
        0 2 var-set
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
            0 0 9 action-force
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

: castle-2f-12.act04 ( -- )   \ 00403380
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $B4 15.0 29.0 45 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $1E self-wait-frames
    $FE $28 5 char-sound
    $1E threat-raise
    self-frames-reset
    $19 self-wait-frames
    $FE $28 5 char-sound
    $14 threat-raise
    self-frames-reset
    $17 self-wait-frames
    $FE $28 5 char-sound
    $A threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: castle-2f-12.act06 ( -- )   \ 00403450
    self-wait-done
    23.3 4.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        3 message
        wait-message
        0 ebit-set
    else
        4 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: castle-2f-12.act07 ( -- )   \ 0047AB28
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: castle-2f-12.act08 ( -- )   \ 00403480
    self-wait-done
    1 ebit? not if
        6 message
        wait-message
        1 ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-12.act0A ( -- )   \ 004034F0
    self-wait-done
    $35 0.0 -31.5 180 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 10.0 0.0 0.0 event-camera
    $30C story-flag? not if
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act0B ( -- )   \ 00403550
    stalkers-stay state-flag-set
    1 self-scripted
    $86 1 item-count? $8A 1 item-count? and if
        9 ebit-clear
    else
        9 ebit-set
    then
    self-wait-done
    0 $35 0.0 -31.5 180 char-to-xz
    1 7.0 30.0 0.0 0.0 event-camera
    world-frozen state-flag-set
    4 6 0.0 5.0 -39.0 0 0 sound
    self-frames-reset
    $10 self-wait-frames
    9 ebit? if
        0 0.0 3.0 -39.0 flicker-sprite
    then
    self-frames-reset
    0 3 6 char-sound
    begin
        $1E frames? not while
        1 castle-2f-12.cmd00
        yield
    repeat
    self-frames-reset
    self-wait-16
    9 ebit? if
        $A ebit-clear
        $86 1 item-count? not if
            $86 message-param-room
            $86 1 item-give-count
            0 $86 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
            $A ebit-set
        then
        $8A 1 item-count? not if
            $A ebit? if
                self-frames-reset
                self-wait-16
            then
            $8A message-param-room
            $8A 1 item-give-count
            0 $8A item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        0 effect-remove
    else
        $A message
        wait-message
    then
    $30C story-flag-set
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-clear
    stalkers-stay state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act0C ( -- )   \ 00403660
    self-wait-done
    4 6 0.0 5.0 -39.0 0 0 sound
    self-frames-reset
    $1E self-wait-frames
    8 message
    wait-message
    self-idle-or-end
;

: castle-2f-12.act0E ( -- )   \ 00403760
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $3A 3.123 11.435 -165 $FFFF 5 self-move-to
    self-wait-done
    $1305 self-anim
    self-wait-anim
    $FE 3 stalker-mode
    $318 story-flag-set
    self-idle-or-end
;

: castle-2f-12.act0F ( -- )   \ 004037A0
    self-wait-done
    22.9 -9.7 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $259 story-flag-set
            2 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-12.act10 ( -- )   \ 00403800
    self-wait-done
    -19.0 -19.0 self-turn-to-xz
    self-wait-done
    9 message
    wait-message
    self-idle-or-end
;

: castle-2f-12.act11 ( -- )   \ 00403810
    self-wait-done
    2.0 -2.0 self-turn-to-xz
    self-wait-done
    8 ebit-set
    begin
        castle-2f-12.cmd02
        4 var-inc
        4 180 var? not while
        yield
    repeat
    self-idle-or-end
;

: castle-2f-12.act12 ( -- )   \ 00403830
    self-wait-done
    -17.5 47.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $28C story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-12.act13 ( -- )   \ 00403890
    begin
        $FF 0 char-in-zone? if
            3 castle-2f-12.cmd01
        then
        $FF 1 char-in-zone? if
            2 castle-2f-12.cmd01
        then
        1 castle-2f-12.cmd01
        yield
    again
;

\ ---- registered ----
' castle-2f-12.enter castle-2f-12 0 room-script!
' castle-2f-12.char-enter castle-2f-12 6 room-script!
' castle-2f-12.phase1 castle-2f-12 1 room-script!
' castle-2f-12.phase2 castle-2f-12 2 room-script!
' castle-2f-12.act00 castle-2f-12 $00 action-script!
' castle-2f-12.act01 castle-2f-12 $01 action-script!
' castle-2f-12.act02 castle-2f-12 $02 action-script!
' castle-2f-12.act03 castle-2f-12 $03 action-script!
' castle-2f-12.act04 castle-2f-12 $04 action-script!
' castle-2f-12.act05 castle-2f-12 $05 action-script!
' castle-2f-12.act06 castle-2f-12 $06 action-script!
' castle-2f-12.act07 castle-2f-12 $07 action-script!
' castle-2f-12.act08 castle-2f-12 $08 action-script!
' castle-2f-12.act09 castle-2f-12 $09 action-script!
' castle-2f-12.act0A castle-2f-12 $0A action-script!
' castle-2f-12.act0B castle-2f-12 $0B action-script!
' castle-2f-12.act0C castle-2f-12 $0C action-script!
' castle-2f-12.act0D castle-2f-12 $0D action-script!
' castle-2f-12.act0E castle-2f-12 $0E action-script!
' castle-2f-12.act0F castle-2f-12 $0F action-script!
' castle-2f-12.act10 castle-2f-12 $10 action-script!
' castle-2f-12.act11 castle-2f-12 $11 action-script!
' castle-2f-12.act12 castle-2f-12 $12 action-script!
' castle-2f-12.act13 castle-2f-12 $13 action-script!
