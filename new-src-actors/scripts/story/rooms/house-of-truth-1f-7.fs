\ story/rooms/house-of-truth-1f-7.fs - the event scripts of room house-of-truth-1f-7 ($8F; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-7
USING: engine common actors messages room-names story.words story.shared flag-names story-names ;

\ a struggle: byte 3 0 resets Fiona's shake tracking (+0x1AD710 / +0x1AD714); 1 adds her shakes
\ to script variable byte 4, with a grunt (voice 0x3D or 0x45 at random) when the cool-down
\ variable byte 6 is out (it then runs 45 / 60), and at 100 the event byte 5 (+0x5C)
: house-of-truth-1f-7.cmd00 ( bytes.. n -- )   \ (her struggle: b0 0 her shakes counted from now; 1 this frame's added
    \ to variable b1 - with a grunt ($3D or $45 at random) when variable b3, the cool-down, is out:
    \ it then runs 45 / 60 - and at 100 event bit b2)
    dup 4 <> if  1- 0 ?do  drop  loop  0 0 0  else  drop  then
    {: b0 v bit cool | n :}
    b0 0= if  shakes-count  exit  then
    b0 1 <> if  exit  then
    shakes-new to n
    n 0<>  cool var @ 0= and if
        2 random if  fiona-who $3D 5 char-sound  cool 45 var-set  else  fiona-who $45 5 char-sound  cool 60 var-set  then
    then
    v var @ n +  dup  v swap var-set  100 >= if  bit ebit-set  then ;
\ byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(0, 0); else
\ Progress_SpeechCall
: house-of-truth-1f-7.cmd01 ( b0 -- )   \ (Fiona's face: 0 / 1 loaded - her two costumes' - 2 shown, else done)
    case  0 of  s" O_FIO\FIO_201.TEX" speech-load  endof  1 of  s" O_FIH\FIH_201.TEX" speech-load  endof
          2 of  fiona-who 0 speak  endof  >r speech-done r>  endcase ;
\ room 0x8F: three grey smoke effects (Effect79B00, size 50) at the room's spots 0, 1, 6
\ (grey_three).
: house-of-truth-1f-7.cmd02 ( -- )   \ (the ceiling coming down: stones of three kinds falling from 50)
    0 50e 0e 0e 1e stones-falling  1 50e 0e 0e 1e stones-falling  6 50e 0e 0e 1e stones-falling ;
\ room 0x8F: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-7.cmd03 ( -- )  slam-shake ;
\ the pursuer is active and out of view: state 3 (+0xE8), or the camera's on-screen test (+0xD4)
\ fails
: house-of-truth-1f-7.cond00? ( -- flag )  stalker-about? 0= if  false exit  then  stalker-move-mode 3 = stalker-in-view? or ;

: house-of-truth-1f-7.enter ( -- )   \ 00435990
    room-sounds
    got-cane-of-caduceus story-flag? not if
        0 1 $14 door-bits
    else
        0 0 $14 door-bits
    then
    ht1f-7-flag1 story-flag? not if
        2 0 object-hidden
        3 0 object-hidden
        1 0 8 nav-group
    else
        2 1 object-hidden
        3 1 object-hidden
    then
    4 0 object-hidden
    over-music-f story-flag? not if
        1 0 $14 door-bits
        2 1 $14 door-bits
    else
        1 1 8 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    then
    0 -59.1 18.5 -16.0 1 flame-in
    1 -10.4 17.5 -26.3 1 flame-in
    2 84.1 19.8 -2.3 1 flame-in
    3 -57.4 20.0 24.4 1 flame-in
    4 115.9 19.5 33.2 1 flame-in
    tremor-warned story-flag? if
        5 1.0 0 bgm
    then
    shared.act95
    ht1f-9-flag2 story-flag? if
        house-of-truth-1f-7.cmd02
    then
    ht1f-7-antimony-tube story-flag? not if
        5 13.05 6.54 108.47 flicker-sprite
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-7.act08 ( -- )   \ 00436180
    5 ebit-set
    6 ebit-set
    4 ebit-clear
    $25 0 pvar? if
        $A chance? if
            4 ebit-set
        then
    else $25 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $25 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $25 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    stalker-who char-unseen? not if
        4 ebit-set
    then
    2 creature-action? if
        4 ebit-set
    then
    4 ebit? if
        0 stalker-who 9 action
    else
        $78 1 summoner-wait
        8 ebit-clear
    then
    0 stalker-alert? if
        stalker-who 2 stalker-command
    then
    $25 pvar-inc
    exit
;

: house-of-truth-1f-7.char-enter ( -- )   \ 00435A60
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
            4 4 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        4 4 player-camera
    then then
    1 4 4 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            house-of-truth-1f-7.act08
        else
            6 ebit-clear
        then
    then
;

: house-of-truth-1f-7.phase1 ( -- )   \ 00435AF0
    9 ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    7 camera-mode? if
        $15 2 2 1 chars-area-camera
        $16 2 2 1 chars-area-camera
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    $C 4 4 1 chars-area-camera
    $D 5 5 1 chars-area-camera
    $E 5 5 1 chars-area-camera
    $F 0 0 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    found-file-255 story-flag? ht1f-7-flag1 story-flag? not and if
        hewie-who $10 char-in-area? if
            0 ebit? not if
                2 game-mode? not if
                    hewie-who 2 char-cond? if
                        hewie-who 1 char-heal
                    else
                        0 hewie-who 1 action
                    then
                then
            then
        then
    then
    0 ebit? hewie-who char-busy? not and hewie-who $10 char-in-area? not and if
        0 ebit-clear
    then
    hewie-who 2 char-cond? if
        0 ebit-clear
    then
    over-music-f story-flag? if
        ht1f-7-flag2 story-flag? not if
            fiona-who $11 char-entered-area? if
                ht1f-7-flag2 story-flag-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 3 action-force
                else
                    1 fiona-who 3 action-force
                then
            then
        then
    then
    ht1f-9-flag2 story-flag? not if
        ht1f-7-flag3 story-flag? not if
            fiona-who $13 char-entered-area? if
                ht1f-7-flag3 story-flag-set
                $8F 0 235 $88 0 -1 0 0.0 creature-place
            then
        then
    then
    stalker-who 2 char-cond? not if
        fiona-hidden state-flag? 5 ebit? not and stalker-who char-here? and if
            stalker-who char-busy? not if
                8 ebit-set
                house-of-truth-1f-7.act08
            then
        then
    then
    7 ebit? stalker-who $17 char-in-area? and if
        0 stalker-who $D action
    then
    house-of-truth-1f-7.cmd03
;

: house-of-truth-1f-7.phase2 ( -- )   \ 00435C20
    ht1f-7-flag3 story-flag? if
        fiona-who $14 char-in-area? fiona-who 90 $3C char-heading? and if
            stalker-who char-here? not if
                6 calm offer-action
            else house-of-truth-1f-7.cond00? not if
                6 calm offer-action
            else
                $8016 examine-message   \ "I have to get away! I don't have time for this!"
            then then
        then
    then
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            5 always offer-action
        then
    then
    ht1f-7-flag1 story-flag? not if
        fiona-who $12 char-in-area? fiona-who -45 $32 char-heading? and if
            0 ebit? if
                0 unhurried offer-action
            then
        then
    else got-cane-of-caduceus story-flag? not if
        fiona-who $12 char-in-area? fiona-who -45 $32 char-heading? and if
            2 unhurried offer-action
        then
    then then
    ht1f-7-antimony-tube story-flag? not if
        0 5 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            $A calm offer-action
        then
    then
;

: house-of-truth-1f-7.phase3 ( -- )   \ 00435CA0
    -72.3 18.0 56.0 -85.0 18.0 56.4 -72.3 -2.0 51.4 -85.0 -2.0 51.4 lights-doorway
    72.0 16.0 -0.1 91.2 16.0 -0.1 72.0 0.0 -2.0 91.2 0.0 -2.0 lights-doorway
    ht1f-9-flag2 story-flag? 9 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        9 ebit-set
    then
;

: house-of-truth-1f-7.act00 ( -- )   \ 00435D30
    stalkers-stay state-flag-set
    1 self-scripted
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    self-wait-done
    -89.0 75.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    self-wait-16
    0 message   \ "Hewie's barking... Is he trying to tell me something about this wall?"
    wait-message
    self-wait-anim
    $F $44 fade
    wait-fade
    $26 3 pvar? if
        0 house-of-truth-1f-7.cmd01
    else $26 2 pvar? if
        1 house-of-truth-1f-7.cmd01
    then then
    hewie-who action-end
    hewie-who char-done
    $14 $64 movie-param
    0 $F9 $B action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 1 object-hidden
    3 1 object-hidden
    0 0 8 nav-group
    ht1f-7-flag1 story-flag-set
    fiona-who $B3 -74.86 74.73 -91 char-to-xz
    hewie-who char-activate
    hewie-who $B5 -51.58 76.38 -80 char-to-xz
    50 hewie-trust
    1 1 player-camera
    camera-restart
    $26 3 pvar? $26 2 pvar? or if
        3 house-of-truth-1f-7.cmd01
    then
    $F $41 fade
    wait-fade
    $4C resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-7.act01 ( -- )   \ 00435E50
    0 self-scripted
    0 ebit-set
    self-wait-done
    -89.0 75.0 self-turn-to-xz
    self-wait-done
    begin
        hewie-bark
        self-wait-done
        2 game-mode? until
    0 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-7.act02 ( -- )   \ 00435E70
    self-wait-done
    -89.0 75.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 message   \ "This cane... I don't know how or why, but for some reason I feel like I've seen it some..."
    wait-message
    self-wait-anim
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0 $14 door-bits
    got-cane-of-caduceus story-flag-set
    $26 message-param-room
    $26 1 item-give-count
    0 $26 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-7.act03 ( -- )   \ 00435ED0
    stalkers-stay state-flag-set
    1 self-scripted
    stalkers-blind state-flag-set
    stalker-who char-here? if
        0 stalker-who 4 action-force
    then
    hewie-who char-here? if
        0 hewie-who 4 action-force
    then
    $F8 action-end
    $F $14 fade
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    stalker-who 1 char-visible
    hewie-who 1 char-visible
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $50 resident-flag-set
    4 0 object-hidden
    1 1 8 nav-group
    ht1f-7-flag2 story-flag-set
    fiona-who 5 char-file-load
    fiona-who char-file-use
    fiona-who $192 -37.91 65.69 180 char-to-xz
    7 -1 player-camera
    camera-restart
    stalkers-stay state-flag-clear
    0 1 house-of-truth-1f-7.cmd00
    $8000 self-anim
    4 0 object-anim
    $40000000 6 -37.0 0.0 63.0 0 0 sound
    stalker-who char-here? if
        stalker-who action-end
    then
    hewie-who char-here? if
        hewie-who action-end
    then
    stalker-who 0 char-visible
    hewie-who 0 char-visible
    stalker-who 1 char-in-nav-group? if
        stalker-who $194 char-to-tri
    then
    hewie-who 1 char-in-nav-group? if
        hewie-who $18B char-to-tri
    then
    $F $11 fade
    shared.act95
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    7 ebit-set
    begin
        self-at-motion-event? if
            $8000 self-anim
            4 0 object-anim
        then
        1 0 var? not if
            1 var-dec
        then
        1 0 1 1 4 house-of-truth-1f-7.cmd00
        self-frames-reset
        1 self-wait-frames
        1 ebit? until
    self-wait-anim
    fiona-who $45 5 char-sound
    $8001 self-anim
    4 1 object-anim
    self-wait-anim
    stalker-who char-busy? if
        stalker-who action-end
    then
    7 ebit-clear
    stalkers-blind state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-7.act04 ( -- )   \ 0047AE60
    begin
        self-frames-reset
        1 self-wait-frames
    again
;

: house-of-truth-1f-7.act05 ( -- )   \ 00436050
    self-wait-done
    stalker-who self-touching? not if
        $EA self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $EA char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $EA door-unlock
                    $8008 message   \ "You unlocked the door."
                    20 self-idle-anim
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    $E6 door-lock
    $E9 door-lock
    self-idle-or-end
;

: house-of-truth-1f-7.act07 ( -- )   \ 00436150
    3 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-who 0 char-visible
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    3 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-7.act06 ( -- )   \ 004360A0
    stalkers-stay state-flag-set
    6 ebit-clear
    1 self-scripted
    5 ebit-clear
    fiona-who 8 char-file-load
    self-wait-done
    self-who self-look-at
    yield
    fiona-who char-file-use
    $FE $8004 5 50.28 39.2 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    3 ebit-clear
    0 avoid-prompt
    self-who 6 -1 char-camera
    self-who 1 char-visible
    begin
        0 2 pad? not if
            in-panic? 3 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] house-of-truth-1f-7.act07 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                6 ebit? stalker-who char-here? not and if
                    6 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] house-of-truth-1f-7.act07 goto
            then
            self-who 0 char-visible
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: house-of-truth-1f-7.act09 ( -- )   \ 004361E0
    self-wait-done
    8 ebit? not if
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    else
        stalker-who 0 char-heading-for? 0 exit-door-open? not and stalker-who 0 char-in-area? and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and stalker-who 1 char-in-area? and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    then
    8 ebit-clear
    $FE 45.21 54.9 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    3 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: house-of-truth-1f-7.act0A ( -- )   \ 00436250
    self-wait-done
    13.05 108.47 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $97 message-param-room
        $97 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            ht1f-7-antimony-tube story-flag-set
            5 effect-remove
            $97 1 item-give-count
            0 $97 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: house-of-truth-1f-7.act0B ( -- )   \ 004362B0
    begin
        2 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    $26 3 pvar? $26 2 pvar? or if
        2 house-of-truth-1f-7.cmd01
    then
    self-idle-or-end
;

: house-of-truth-1f-7.act0C ( -- )   \ 0047AE68
    self-idle-or-end
;

: house-of-truth-1f-7.act0D ( -- )   \ 004362D0
    self-wait-done
    7 ebit? not if
        self-idle-or-end
    then
    $18E -25.136 68.404 -102 $FFFF 5 self-move-to
    self-wait-done
    7 ebit? not if
        self-idle-or-end
    then
    $E04 self-anim
    self-frames-reset
    $14 self-wait-frames
    panic-eased
    1 fiona-who $E action-force
    4 2 object-anim
    self-frames-reset
    $1B self-wait-frames
    $40000001 6 -37.0 0.0 63.0 0 0 sound
    begin
        yield
    again
;

: house-of-truth-1f-7.act0E ( -- )   \ 00436320
    capture-no-end state-flag-set
    self-wait-done
    fiona-who $41 5 char-sound
    $1100 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    1 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: house-of-truth-1f-7.act0F ( -- )   \ 00436340
    self-wait-done
    0 1 $14 door-bits
    2 1 $14 door-bits
    0 -59.1 18.5 -16.0 1 flame-in
    1 -10.4 17.5 -26.3 1 flame-in
    2 84.1 19.8 -2.3 1 flame-in
    3 -57.4 20.0 24.4 1 flame-in
    4 115.9 19.5 33.2 1 flame-in
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $26 3 pvar? if
        0 house-of-truth-1f-7.cmd01
    else $26 2 pvar? if
        1 house-of-truth-1f-7.cmd01
    then then
    $14 $64 movie-param
    0 $F9 $B action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $26 3 pvar? $26 2 pvar? or if
        3 house-of-truth-1f-7.cmd01
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: house-of-truth-1f-7.act10 ( -- )   \ 00436440
    self-wait-done
    2 1 object-hidden
    3 1 object-hidden
    1 1 $14 door-bits
    0 -59.1 18.5 -16.0 1 flame-in
    1 -10.4 17.5 -26.3 1 flame-in
    2 84.1 19.8 -2.3 1 flame-in
    3 -57.4 20.0 24.4 1 flame-in
    4 115.9 19.5 33.2 1 flame-in
    house-of-truth-1f-7.cmd02
    7 0 movie-play
    6 cutscene-start
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

\ ---- registered ----
house-of-truth-1f-7 register-scripts   \ (its phases and actions, by their names)
