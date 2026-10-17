\ story/rooms/castle-2f-5.fs - the event scripts of room castle-2f-5 ($32; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-5
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ room 0x32: room effect slot 1 made anew as the first TV screen (TvScreenA).
: castle-2f-5.cmd00 ( -- )  1 0 tv-in ;   \ (room effect 1 the TV showing the screen)
\ room 0x32 (D_0042C2A8): the fan turns, except while a movie plays
: castle-2f-5.cmd01 ( -- )  fan-turn ;
\ room 0x32 (D_0042C2B8)
: castle-2f-5.cmd02 ( b0 -- )  $1B $10000040  0.491e 22.0e 69.502e  9.502e 22.0e 60.491e  0.491e 0.0e 69.502e  9.502e 0.0e 60.491e  lit-quad ;   \ (a reflecting window: b0 1 removed, 2 over the whole picture)
\ a room callback: byte 3 0 a progress name, 1 wait for character 3 (2 while not), else done
: castle-2f-5.cmd03 ( b0 -- )   \ (Daniella's face: 0 loaded, 1 shown, else done)
    case  0 of  s" O_DNL\DNL_202.TEX" speech-load  endof  1 of  3 0 speak  endof  >r speech-done r>  endcase ;
\ room 0x32 (D_0042C2D8): byte 3 0..3 the lit quad (room effect 0x1A) as room 0x21's; 4 and up
\ the mirror fragment's reflection (room effect 0x1A, MirrorFragment_vtable) on the object
\ "a_fragment0": 1.8 across, -0.1 down, strength 1, kind 2, alpha 0xFF
: castle-2f-5.cmd04 ( b0 -- )   \ (b0 0..3 the door's reflecting quad, as castle-2f-3's; 4 and up the fragment reflecting)
    dup 4 < if  $1A $20000040  40.491e 30.0e -10.498e  49.502e 30.0e -19.509e  40.491e 1.5e -10.498e  49.502e 1.5e -19.509e  lit-quad exit  then
    drop  0 $1A s" a_fragment0" $FF 2  1.8e -0.1e 1e  mirror-fragment ;
\ the player's model tint: white (byte 3 0) or blue halved
: castle-2f-5.cmd05 ( b0 -- )  drop ;   \ (the player model tint (+0xC0: CharModel_SetPoint, empty))

: castle-2f-5.enter ( -- )   \ 0042B0B0
    room-sounds
    castle-2f-5.cmd00
    c2f-3-tv-examined story-flag? not if
        0 part-shown
        1 0 1 effect-string
    else
        0 part-hidden
        1 1 1 effect-string
        1 noise-level
    then
    0 $F1 $12 action
    c2f-3-medallion-found story-flag? c2f-3-medallion story-flag? not and if
        2 -9.0 1.0 -11.0 flicker-sprite
    then
    0 0 var-set
    1 0 var-set
    0 castle-2f-5.cmd02
    0 castle-2f-5.cmd04
    saturnus-hint-on story-flag? not if
        1 part-shown
        2 part-hidden
        3 part-hidden
        4 part-hidden
        5 part-hidden
        6 part-hidden
    else
        1 part-hidden
        2 part-hidden
        3 part-hidden
        4 part-hidden
        5 part-shown
        6 part-shown
    then
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
;

: castle-2f-5.act04 ( -- )   \ 0042B850
    5 ebit-clear
    2 0 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 1 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 2 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else 2 3 pvar? if
        $32 chance? if
            5 ebit-set
        then
    then then then then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 stalker-who $B action
    else
        $78 1 summoner-wait
    then
    2 pvar-inc
    exit
;

: castle-2f-5.char-enter ( -- )   \ 0042B160
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 -1 player-camera
    then then
    0 2 -1 area-camera
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
    then
    hewie-who self-is? if
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            $B ebit-set
            c2f-3-tv-examined story-flag? if
                castle-2f-5.act04
            else
                castle-2f-5.act04
            then
        else
            $B ebit-clear
        then
    then
;

: castle-2f-5.phase1 ( -- )   \ 0042B200
    3 ebit? not if
        6 sound-bank-loaded? if
            c2f-3-tv-examined story-flag? if
                $40000001 6 0.0 3.0 7.0 0 0 sound
            then
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            3 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000001 6 0.0 3.0 7.0 0 0 sound
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
        fiona-who $8000000C 6 char-sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    3 0 0 1 chars-area-camera
    8 1 -1 1 chars-area-camera
    9 2 -1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    c2f-3-tv-examined story-flag? not if
        0 part-shown
        1 0 1 effect-string
    else
        0 part-hidden
        1 1 1 effect-string
    then
    over-music-b story-flag? saturnus-hint-on story-flag? not and if
        1 2 pad? 2 2 pad? or 3 2 pad? or 0 control-action? or 1 control-action? or 2 control-action? or 3 control-action? or 4 control-action? or fiona-free? and if
            0 fiona-who $19 action
        then
        99.0 fiona-fear
    then
    c2f-3-medallion-found story-flag? not if
        7 -9.0 0.0 -11.0 $A 5 0 zone
        hewie-who 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 c2f-3-medallion-found var-set
                $1A c2f-3-medallion var-set
                $1B 2 var-set
                $1C -9000 var-set
                $1D 1000 var-set
                $1E -11000 var-set
                $1F 7 var-set
                0 hewie-who $8B action
            then
        then
    then
    0 -1.41 -8.0 8.99 $D 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 -46.87 0.0 -38.5 $14 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    2 -39.23 0.0 -59.47 $14 6 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    3 -37.71 -8.0 63.24 $F 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 46.35 0.0 -40.12 $F 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    5 39.18 0.0 -19.5 $10 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    6 -0.22 -8.0 59.41 $10 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-5.phase2 ( -- )   \ 0042B520
    fiona-who 4 char-in-area? fiona-who 90 $5A char-heading? and if
        0 unhurried offer-action
    then
    fiona-who 5 char-in-area? fiona-who 90 $3C char-heading? and if
        over-music-b story-flag? saturnus-hint-on story-flag? not and if
            $1A unhurried offer-action
        else stalker-who char-here? not if
            1 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then then
    then
    fiona-who $B char-in-area? fiona-who -67 $3C char-heading? and if
        $14 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 45 $3C char-heading? and if
        $15 unhurried offer-action
    then
    fiona-who $11 char-in-area? fiona-who 0 $3C char-heading? and if
        $17 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who -45 $3C char-heading? and if
        $18 unhurried offer-action
    then
    fiona-who $12 char-in-area? fiona-who 0 $32 char-heading? and if
        $1C unhurried offer-action
    then
    fiona-who $13 $32 char-faces-area? if
        $1D unhurried offer-action
    then
    fiona-who $14 char-in-area? fiona-who -45 $32 char-heading? and if
        $1E unhurried offer-action
    then
    fiona-who $15 char-in-area? fiona-who 90 $32 char-heading? and if
        fg-3-flag3 story-flag? not if
            $1F unhurried offer-action
        else
            $20 unhurried offer-action
        then
    then
    fiona-who $D char-in-area? fiona-who 22 $3C char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $16 char-in-area? fiona-who 22 $3C char-heading? and if
        8 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who -45 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    over-music-b story-flag? saturnus-hint-on story-flag? not and if
        -2147483646 scene-request? 2 scene-request? or if
            $19 always offer-action
        then
    then
    c2f-3-medallion-found story-flag? c2f-3-medallion story-flag? not and if
        7 2 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            over-music-b story-flag? saturnus-hint-on story-flag? not and if
                $19 calm offer-action
            else
                $A calm offer-action
            then
        then
    then
;

: castle-2f-5.act00 ( -- )   \ 0042B640
    self-wait-done
    $6A -1.0 13.5 160 $FFFF 5 self-move-to
    self-wait-done
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    c2f-3-tv-examined-2 story-flag? not if
        c2f-3-tv-examined-2 story-flag-set
        $29 message   \ "I hope this TV works..."
        wait-message
    then
    reach-out-anim self-anim
    self-wait-anim
    5 0 pvar? 5 1 pvar? or if
        c2f-3-tv-examined story-flag? not if
            c2f-3-tv-examined story-flag-set
            fiona-who 0 6 char-sound
            $40000001 6 0.0 3.0 7.0 0 0 sound
            $40 $6A noise
            1 noise-level
        else
            c2f-3-tv-examined story-flag-clear
            fiona-who 2 6 char-sound
            1 6 sound-stop
            0 noise-level
        then
        reach-back-anim self-anim
        self-wait-anim
    else
        reach-back-anim self-anim
        self-wait-anim
        $28 message   \ "It won't turn on. Maybe it's broken?"
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act03 ( -- )   \ 0042B820
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    counter-inc
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-2f-5.act01 ( -- )   \ 0042B6E0
    stalkers-stay state-flag-set
    $B ebit-clear
    1 self-scripted
    0 counter-set
    6 ebit-clear
    7 ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 50 chars-within? and if
        0 hewie-who 2 action-force
        6 ebit-set
    then
    fiona-who 8 char-file-load
    self-wait-done
    6 ebit? if
        $27 14.5 -37.5 180 $FFFF 5 self-move-to
        self-wait-done
        fiona-who char-file-use
        hewie-who self-look-at
        yield
        begin
            7 ebit? not while
            yield
        repeat
        counter-inc
        1 self-noclip
        self-who self-look-at
        yield
        0 room-motion 5 self-anim-blend8
        self-frames-reset
        $1C self-wait-frames
        fiona-who slide-sound 5 char-sound
        self-wait-anim
    else
        fiona-who char-file-use
        $27 $8004 5 14.5 -37.5 180 self-walk-anim
        self-wait-done
        counter-inc
        1 self-noclip
        fiona-who slide-sound 5 char-sound
        5 room-motion $A self-anim-blend8
        self-wait-anim
    then
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    4 ebit-clear
    0 avoid-prompt
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? 4 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] castle-2f-5.act03 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                $B ebit? stalker-who char-here? not and if
                    $B ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            counter-inc
            stalker-who char-here? if
                ['] castle-2f-5.act03 goto
            then
            fiona-who camera-follow
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

: castle-2f-5.act02 ( -- )   \ 0042B7D0
    1 self-scripted
    hewie-who 9 char-file-load
    self-wait-done
    $88 18.5 -37.5 180 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    7 ebit-set
    1 wait-counter
    1 self-noclip
    0 room-motion 5 self-anim-blend
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        fiona-who char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                2 room-motion 5 self-anim-blend8
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-5.act05 ( -- )   \ 0047AD78
    self-wait-done
    -1 self-idle-anim
    self-wait-anim
    self-idle
    self-wait-done
    self-idle-or-end
;

: castle-2f-5.act06 ( -- )   \ 0042B8A0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $6A -4.0 13.0 135 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    stalker-who 2 6 char-sound
    1 6 sound-stop
    0 noise-level
    c2f-3-tv-examined story-flag-clear
    5 pvar-inc
    stalker-who 3 stalker-command
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: castle-2f-5.act07 ( -- )   \ 0042B8F0
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $31 message   \ "The sun has completely set... It's pitch black out now. I can't see 10 feet out the w..."
    wait-message
    self-idle-or-end
;

: castle-2f-5.act08 ( -- )   \ 0042B900
    self-wait-done
    45 self-turn-angle
    self-wait-done
    saturnus-hint-on story-flag? not if
        $31 message   \ "The sun has completely set... It's pitch black out now. I can't see 10 feet out t..."
        wait-message
    else
        $32 message   \ "The glass looks sharp enough to really cause some serious injuries. I probably sh..."
        wait-message
    then
    self-idle-or-end
;

: castle-2f-5.act09 ( -- )   \ 0042B920
    1 part-shown
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    1 castle-2f-5.cmd05
    begin
        2 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 castle-2f-5.cmd05
    begin
        $A cutscene-shot? not while
        yield
    repeat
    2 castle-2f-5.cmd04
    begin
        $A03 cutscene-cue-reached? not while
        yield
    repeat
    1 part-hidden
    2 part-shown
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    begin
        $B cutscene-shot? not while
        yield
    repeat
    0 castle-2f-5.cmd04
    begin
        $A67 cutscene-cue-reached? not while
        yield
    repeat
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    begin
        $A70 cutscene-cue-reached? not while
        yield
    repeat
    1 part-hidden
    2 part-hidden
    3 part-shown
    4 part-hidden
    5 part-hidden
    6 part-hidden
    begin
        $A89 cutscene-cue-reached? not while
        yield
    repeat
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-shown
    5 part-hidden
    6 part-hidden
    begin
        $AB7 cutscene-cue-reached? not while
        yield
    repeat
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-shown
    6 part-shown
    $E 0 object-hidden
    $F 0 object-hidden
    $10 0 object-hidden
    $11 0 object-hidden
    $12 0 object-hidden
    $13 0 object-hidden
    $14 0 object-hidden
    $15 0 object-hidden
    $16 0 object-hidden
    $17 0 object-hidden
    $18 0 object-hidden
    begin
        $AE7 cutscene-cue-reached? not while
        yield
    repeat
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    2 castle-2f-5.cmd04
    begin
        $BF3 cutscene-cue-reached? not while
        yield
    repeat
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-shown
    6 part-hidden
    0 castle-2f-5.cmd04
    begin
        $CD2 cutscene-cue-reached? not while
        yield
    repeat
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-shown
    6 part-shown
    begin
        $10 cutscene-shot? not while
        4 castle-2f-5.cmd04
        yield
    repeat
    0 castle-2f-5.cmd04
    begin
        $11 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    1 castle-2f-5.cmd03
    self-idle-or-end
;

: castle-2f-5.act0A ( -- )   \ 0042BB10
    self-wait-done
    -9.0 -11.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-3-medallion story-flag-set
            2 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-5.act0B ( -- )   \ 0042BB70
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        stalker-who $A char-file-load
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
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
    4 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-2f-5.act0C ( -- )   \ 0047AD80
    self-idle-or-end
;

: castle-2f-5.act0D ( -- )   \ 0047AD84
    self-idle-or-end
;

: castle-2f-5.act0E ( -- )   \ 0047AD88
    self-idle-or-end
;

: castle-2f-5.act0F ( -- )   \ 0047AD8C
    self-idle-or-end
;

: castle-2f-5.act10 ( -- )   \ 0047AD90
    self-idle-or-end
;

: castle-2f-5.act11 ( -- )   \ 0047AD94
    self-idle-or-end
;

: castle-2f-5.act12 ( -- )   \ 0047AD98
    begin
        castle-2f-5.cmd01
        yield
    again
;

: castle-2f-5.act13 ( -- )   \ 0047ADA0
    self-idle-or-end
;

: castle-2f-5.act14 ( -- )   \ 0042BBD0
    self-wait-done
    -40.0 -60.0 self-turn-to-xz
    self-wait-done
    $23 message   \ "The fire burns, emitting a red glow. The room is heated to a comfortable temperature."
    wait-message
    self-idle-or-end
;

: castle-2f-5.act15 ( -- )   \ 0042BBE0
    self-wait-done
    $B5 43.571 -39.929 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 10.0 event-camera
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    c2f-3-portrait-1-examined story-flag-set
    0 ebit? not if
        $24 message   \ "I wonder whose portrait this is?"
        wait-message
        0 ebit-set
    else
        $25 message   \ "This sounds really weird, but for some reason, I feel like I've seen this person..."
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act16 ( -- )   \ 0047ADA4
    self-idle-or-end
;

: castle-2f-5.act17 ( -- )   \ 0042BC40
    self-wait-done
    $9F -37.0 63.0 0 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    c2f-3-portrait-2-examined story-flag-set
    1 ebit? not if
        $2D message   \ "The man pictured here has a noble air about him. I wonder how long ago this portr..."
        wait-message
        1 ebit-set
    else c2f-3-portrait-1-examined story-flag? c2f-3-portrait-3-examined story-flag? or if
        $2E message   \ "In this painting the man looks quite young. There is no mistaking that all the pa..."
        wait-message
    else
        $2D message   \ "The man pictured here has a noble air about him. I wonder how long ago this portr..."
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act18 ( -- )   \ 0042BCB0
    self-wait-done
    9 -40.418 -39.428 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    c2f-3-portrait-3-examined story-flag-set
    2 ebit? not if
        $2B message   \ "What a powerful piece of art. The eyes really feel like they are peering deep int..."
        wait-message
        2 ebit-set
    else c2f-3-portrait-1-examined story-flag? c2f-3-portrait-2-examined story-flag? and if
        $2C message   \ "Every painting features the same person. Perhaps it's the lord of this castle?"
        wait-message
    else
        $2B message   \ "What a powerful piece of art. The eyes really feel like they are peering deep int..."
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act19 ( -- )   \ 0042BD20
    self-wait-done
    knocked-anim self-anim
    fiona-who $C 6 char-sound
    self-frames-reset
    $14 self-wait-frames
    8 ebit? not if
        $2F message   \ "I... I don't feel so hot. It's like I have no energy all of a sudden."
        8 ebit-set
    else
        $30 message   \ "I should try to get some rest."
        8 ebit-clear
    then
    attack-anim $A self-anim-blend
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-5.act1A ( -- )   \ 0042BD50
    self-wait-done
    $21 message   \ "Fresh sheets have been placed on the bed. Rest on the bed? YES NO"
    wait-message
    1 answer? if
        self-idle-or-end
    then
    $F $54 fade
    $D 0 movie-play
    $C cutscene-start
    yield
    yield
    scene-wait-ready
    0 castle-2f-5.cmd03
    wait-fade
    c2f-3-tv-examined story-flag-clear
    1 6 sound-stop
    0 noise-level
    hewie-who action-end
    hewie-who char-done
    $FE $32 212 2 stalker-to-room
    stalker-who 1 -1 char-camera
    stalker-who $80808080 1 char-tint
    stalker-who $1E char-layer
    0 $F9 9 action
    panic-eased
    $1A message-prepare   \ "I am not complete."
    1 result? if
        scene-play
    then
    0 castle-2f-5.cmd04
    stalker-who 0 0 char-tint
    stalker-who $A char-layer
    wait-fade
    world-held state-flag-set
    0 counter-set
    2 castle-2f-5.cmd03
    0 castle-2f-5.cmd05
    hewie-who char-activate
    $32 0 39 hewie-to-room
    0 hewie-who $22 action
    stalker-who $D4 25.182 -33.041 -113 char-to-xz
    summoner-own-cooldown
    saturnus-hint-on story-flag-set
    fiona-calm-reset
    fiona-who $21 4.683 -40.0 49 char-to-xz
    1 -1 player-camera
    camera-restart
    1 door-unlock
    3 door-unlock
    $C door-unlock
    no-flee state-flag-clear
    no-running state-flag-clear
    1 wait-counter
    2 counter-set
    force-calm state-flag-clear
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-shown
    6 part-shown
    subscreen-locked state-flag-clear
    $231 item-give
    $F $51 fade
    wait-fade
    summoner-own-cooldown
    self-idle-or-end
;

: castle-2f-5.act1B ( -- )   \ 0047ADA8
    self-idle-or-end
;

: castle-2f-5.act1C ( -- )   \ 0042BEA0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $E8 -25.0 63.0 -5 char-to-xz
    1 20.0 10.0 40.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 ebit? not if
        1 message   \ "There's an antique doll lying here."
        wait-message
        9 ebit-set
    else
        2 message   \ "They say that bisque dolls are supposedly designed with a specific model in mind......"
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act1D ( -- )   \ 0042BEF0
    self-wait-done
    5.0 40.5 self-turn-to-xz
    self-wait-done
    3 message   \ "A birdcage... Right now there is nothing in it."
    wait-message
    self-idle-or-end
;

: castle-2f-5.act1E ( -- )   \ 0042BF00
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $55 -40.0 12.0 -90 char-to-xz
    1 30.0 20.0 -60.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 message   \ "There is a pair of scales lying here. It looks like their purpose is more one of decora..."
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-5.act1F ( -- )   \ 0042BF48
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $40 message   \ "A chest used to store clothes."
    wait-message
    self-idle-or-end
;

: castle-2f-5.act20 ( -- )   \ 0042BF60
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message   \ "A chest used to store clothes. Change clothes? YES NO"
    wait-message
    0 answer? if
        7 subscreen-open
        begin
            subscreen-wanted state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            fiona-who $10 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            hewie-who char-here? if
                hewie-who 2 char-cond? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-2f-5.act21 ( -- )   \ 0047ADAC
    self-idle-or-end
;

: castle-2f-5.act22 ( -- )   \ 0042C0B0
    1 self-scripted
    1 self-noclip
    self-wait-done
    hewie-who $25 13.928 -57.528 -90 char-to-xz
    hewie-who 9 char-file-load
    hewie-who char-file-use
    1 room-motion self-anim
    self-wait-anim
    1 counter-set
    2 wait-counter
    hewie-who $6A 5 char-sound
    2 room-motion 5 self-anim-blend8
    self-wait-anim
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: castle-2f-5.act23 ( -- )   \ 0042C0F0
    self-wait-done
    0 $F1 $12 action
    0 castle-2f-5.cmd02
    0 castle-2f-5.cmd04
    1 part-shown
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-hidden
    6 part-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    $17 1 object-hidden
    $18 1 object-hidden
    3 partner-load
    2 char-start
    $D 0 movie-play
    $C cutscene-start
    yield
    yield
    scene-wait-ready
    0 castle-2f-5.cmd03
    $FE $32 212 2 stalker-to-room
    stalker-who $80808080 1 char-tint
    stalker-who $1E char-layer
    0 $F9 9 action
    panic-eased
    $1A message-prepare   \ "I am not complete."
    1 result? if
        scene-play
    then
    0 castle-2f-5.cmd04
    stalker-who 0 0 char-tint
    stalker-who $A char-layer
    2 castle-2f-5.cmd03
    0 castle-2f-5.cmd05
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-2f-5 register-scripts   \ (its phases and actions, by their names)
