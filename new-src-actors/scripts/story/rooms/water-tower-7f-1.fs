\ story/rooms/water-tower-7f-1.fs - the event scripts of room water-tower-7f-1 ($C5; Water Tower: 7F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-7f-1
USING: actors messages room-names story.words story.shared flag-names story-names fiona.anims ;

\ the 0x10-byte effect ObjectGlow_vtable on room object k + 1 (byte 4 = k, 1..8; script variable
\ 11 - k keeps its slot): made on first use when byte 3 is set, then sent (on byte 3, index 8 -
\ k, the variable, the object), with sound 1 at the object when on and the camera director's
\ +0x38 is clear
: glow-of ( k -- id | -1 )   \ (the glow under planet k, kept in variable 11 - k; none alive: -1)
    11 swap - var @  dup alive? if  dup actor-name s" object-glow" compare 0= if  exit  then  then  drop -1 ;
: water-tower-7f-1.cmd00 ( b0 b1 -- )   \ (the glow under planet b1 (1..8: "earth".."saturn"): b0 1 on - made the first time,
    \ kept in variable 11 - b1 - with its sound 1; 0 off, fading)
    {: on k | a n :}
    k 1+ room-object to n to a  a n object-here? 0= if  exit  then
    k glow-of 0< if
        on 0= if  exit  then
        11 k -  8 k -  a n place-x object@ 0e a n place-z object@  effects:object-glow  var-set
    then
    on  k glow-of  send effect-command
    on if  1 6 0 0  a n place-x object@ a n place-y object@ a n place-z object@  sound  then ;

: water-tower-7f-1.enter ( -- )   \ 004401B0
    room-sounds
    hunted state-flag-set
    $30 1.0 0 bgm
    2 8 var-set
    3 -1 var-set
    4 -1 var-set
    5 -1 var-set
    6 -1 var-set
    7 -1 var-set
    8 -1 var-set
    9 -1 var-set
    $A -1 var-set
    1 8 water-tower-7f-1.cmd00
    stalker-who action-end
    1 summon-take
    wt7f-1-flag1 story-flag? not if
        wt7f-1-flag1 story-flag-set
        1 creatures-clear
    then
    1 $2300 sound-volume
;

: water-tower-7f-1.char-enter ( -- )   \ 00440210
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 -1 player-camera
    then
    1 0 -1 area-camera
;

: water-tower-7f-1.phase1 ( -- )   \ 00440250
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    6 sound-bank-loaded? if
        $C var-inc
        $C 30 var? if
            $C 0 var-set
            $B 0 var? if
                $40000005 6 -127.0 1.7 10.0 0 0 sound
            else $B 1 var? if
                $40000006 6 -127.0 1.7 10.0 0 0 sound
            else $B 2 var? if
                $40000007 6 -127.0 1.7 10.0 0 0 sound
            else $B 3 var? if
                $40000008 6 -127.0 1.7 10.0 0 0 sound
            then then then then
            $B var-inc
            $B 4 var? if
                $B 0 var-set
            then
        then
    then
    8 ebit? not if
        0 0 var? if
            9 ebit-set
        else
            9 ebit-clear
        then
        1 0 var? if
            $A ebit-set
        else
            $A ebit-clear
        then
        0 0 var-set
        1 0 var-set
        0 0.0 0.0 0.0 $A 5 0 zone
        fiona-who 0 8 char-zone-bits? if
            0 1 var-set
        then
        hewie-who 0 8 char-zone-bits? if
            1 1 var-set
        then
        1 -22.0 0.0 0.0 5 5 0 zone
        fiona-who 1 8 char-zone-bits? if
            0 2 var-set
        then
        hewie-who 1 8 char-zone-bits? if
            1 2 var-set
        then
        2 8.0 0.0 -30.0 5 5 0 zone
        0 0 var? if
            fiona-who 2 8 char-zone-bits? if
                0 3 var-set
            then
        then
        1 0 var? if
            hewie-who 2 8 char-zone-bits? if
                1 3 var-set
            then
        then
        3 34.65 0.0 20.0 5 5 0 zone
        0 0 var? if
            fiona-who 3 8 char-zone-bits? if
                0 4 var-set
            then
        then
        1 0 var? if
            hewie-who 3 8 char-zone-bits? if
                1 4 var-set
            then
        then
        4 -25.0 0.0 43.31 5 5 0 zone
        0 0 var? if
            fiona-who 4 8 char-zone-bits? if
                0 5 var-set
            then
        then
        1 0 var? if
            hewie-who 4 8 char-zone-bits? if
                1 5 var-set
            then
        then
        5 -40.37 0.0 -40.38 5 5 0 zone
        0 0 var? if
            fiona-who 5 8 char-zone-bits? if
                0 6 var-set
            then
        then
        1 0 var? if
            hewie-who 5 8 char-zone-bits? if
                1 6 var-set
            then
        then
        6 48.58 0.0 -48.58 5 5 0 zone
        0 0 var? if
            fiona-who 6 8 char-zone-bits? if
                0 7 var-set
            then
        then
        1 0 var? if
            hewie-who 6 8 char-zone-bits? if
                1 7 var-set
            then
        then
        7 57.25 0.0 57.25 5 5 0 zone
        0 0 var? if
            fiona-who 7 8 char-zone-bits? if
                0 8 var-set
            then
        then
        1 0 var? if
            hewie-who 7 8 char-zone-bits? if
                1 8 var-set
            then
        then
        9 ebit? 0 0 var? not and if
            0 1 var? if
                fiona-who $11 6 char-sound
            else 0 2 var? if
                fiona-who $10 6 char-sound
            else 0 3 var? if
                fiona-who $F 6 char-sound
            else 0 4 var? if
                fiona-who $E 6 char-sound
            else 0 5 var? if
                fiona-who $D 6 char-sound
            else 0 6 var? if
                fiona-who $C 6 char-sound
            else 0 7 var? if
                fiona-who $B 6 char-sound
            else 0 8 var? if
                fiona-who $A 6 char-sound
            then then then then then then then then
        else $A ebit? 1 0 var? not and if
            1 1 var? if
                hewie-who $11 6 char-sound
            else 1 2 var? if
                hewie-who $10 6 char-sound
            else 1 3 var? if
                hewie-who $F 6 char-sound
            else 1 4 var? if
                hewie-who $E 6 char-sound
            else 1 5 var? if
                hewie-who $D 6 char-sound
            else 1 6 var? if
                hewie-who $C 6 char-sound
            else 1 7 var? if
                hewie-who $B 6 char-sound
            else 1 8 var? if
                hewie-who $A 6 char-sound
            then then then then then then then then
        then then
        9 ebit? not 0 0 var? and if
        else $A ebit? not 1 0 var? and if
        then then
        2 8 var? if
            0 8 var? 1 8 var? or if
                2 7 var-set
                1 7 water-tower-7f-1.cmd00
            then
        else 2 7 var? if
            0 8 var? 1 8 var? or if
                0 7 var? 1 7 var? or if
                    2 6 var-set
                    1 6 water-tower-7f-1.cmd00
                then
            else
                2 8 var-set
                0 7 water-tower-7f-1.cmd00
                0 ebit-clear
            then
        else 2 6 var? if
            0 7 var? 1 7 var? or if
                0 6 var? 1 6 var? or if
                    2 5 var-set
                    1 5 water-tower-7f-1.cmd00
                then
            else
                0 6 water-tower-7f-1.cmd00
                0 8 var? 1 8 var? or if
                    2 7 var-set
                    1 ebit-clear
                else
                    2 8 var-set
                    0 7 water-tower-7f-1.cmd00
                    1 ebit-clear
                    0 ebit-clear
                then
            then
        else 2 5 var? if
            0 6 var? 1 6 var? or if
                0 5 var? 1 5 var? or if
                    2 4 var-set
                    1 4 water-tower-7f-1.cmd00
                then
            else
                0 5 water-tower-7f-1.cmd00
                0 7 var? 1 7 var? or if
                    2 6 var-set
                    2 ebit-clear
                else
                    0 6 water-tower-7f-1.cmd00
                    0 8 var? 1 8 var? or if
                        2 7 var-set
                        1 ebit-clear
                    else
                        2 8 var-set
                        0 7 water-tower-7f-1.cmd00
                        1 ebit-clear
                        0 ebit-clear
                    then
                then
            then
        else 2 4 var? if
            0 5 var? 1 5 var? or if
                0 4 var? 1 4 var? or if
                    2 3 var-set
                    1 3 water-tower-7f-1.cmd00
                then
            else
                0 4 water-tower-7f-1.cmd00
                0 6 var? 1 6 var? or if
                    2 5 var-set
                    3 ebit-clear
                else
                    0 5 water-tower-7f-1.cmd00
                    0 7 var? 1 7 var? or if
                        2 6 var-set
                        2 ebit-clear
                    else
                        0 6 water-tower-7f-1.cmd00
                        0 8 var? 1 8 var? or if
                            2 7 var-set
                            1 ebit-clear
                        else
                            2 8 var-set
                            0 7 water-tower-7f-1.cmd00
                            1 ebit-clear
                            0 ebit-clear
                        then
                    then
                then
            then
        else 2 3 var? if
            0 4 var? 1 4 var? or if
                0 3 var? 1 3 var? or if
                    2 2 var-set
                    1 2 water-tower-7f-1.cmd00
                then
            else
                0 3 water-tower-7f-1.cmd00
                0 5 var? 1 5 var? or if
                    2 4 var-set
                    4 ebit-clear
                else
                    0 4 water-tower-7f-1.cmd00
                    0 6 var? 1 6 var? or if
                        2 5 var-set
                        3 ebit-clear
                    else
                        0 5 water-tower-7f-1.cmd00
                        0 7 var? 1 7 var? or if
                            2 6 var-set
                            2 ebit-clear
                        else
                            0 6 water-tower-7f-1.cmd00
                            0 8 var? 1 8 var? or if
                                2 7 var-set
                                1 ebit-clear
                            else
                                2 8 var-set
                                0 7 water-tower-7f-1.cmd00
                                1 ebit-clear
                                0 ebit-clear
                            then
                        then
                    then
                then
            then
        else 2 2 var? if
            0 3 var? 1 3 var? or if
                0 2 var? 1 2 var? or if
                    2 1 var-set
                    1 1 water-tower-7f-1.cmd00
                then
            else
                0 2 water-tower-7f-1.cmd00
                0 4 var? 1 4 var? or if
                    2 3 var-set
                    5 ebit-clear
                else
                    0 3 water-tower-7f-1.cmd00
                    0 5 var? 1 5 var? or if
                        2 4 var-set
                        4 ebit-clear
                    else
                        0 4 water-tower-7f-1.cmd00
                        0 6 var? 1 6 var? or if
                            2 5 var-set
                            3 ebit-clear
                        else
                            0 5 water-tower-7f-1.cmd00
                            0 7 var? 1 7 var? or if
                                2 6 var-set
                                2 ebit-clear
                            else
                                0 6 water-tower-7f-1.cmd00
                                0 8 var? 1 8 var? or if
                                    2 7 var-set
                                    1 ebit-clear
                                else
                                    2 8 var-set
                                    0 7 water-tower-7f-1.cmd00
                                    1 ebit-clear
                                    0 ebit-clear
                                then
                            then
                        then
                    then
                then
            then
        else 2 1 var? if
            0 2 var? 1 2 var? or if
                0 1 var? 1 1 var? or if
                    8 ebit-set
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 0 action-force
                    else
                        1 fiona-who 0 action-force
                    then
                then
            else
                0 1 water-tower-7f-1.cmd00
                0 3 var? 1 3 var? or if
                    2 2 var-set
                    6 ebit-clear
                else
                    0 2 water-tower-7f-1.cmd00
                    0 4 var? 1 4 var? or if
                        2 3 var-set
                        5 ebit-clear
                    else
                        0 3 water-tower-7f-1.cmd00
                        0 5 var? 1 5 var? or if
                            2 4 var-set
                            4 ebit-clear
                        else
                            0 4 water-tower-7f-1.cmd00
                            0 6 var? 1 6 var? or if
                                2 5 var-set
                                3 ebit-clear
                            else
                                0 5 water-tower-7f-1.cmd00
                                0 7 var? 1 7 var? or if
                                    2 6 var-set
                                    2 ebit-clear
                                else
                                    0 6 water-tower-7f-1.cmd00
                                    0 8 var? 1 8 var? or if
                                        2 7 var-set
                                        1 ebit-clear
                                    else
                                        2 8 var-set
                                        0 7 water-tower-7f-1.cmd00
                                        1 ebit-clear
                                        0 ebit-clear
                                    then
                                then
                            then
                        then
                    then
                then
            then
        then then then then then then then then
        35 fiona-started? if
            2 8 var? 0 8 var? not and if
                fiona-who 57 57 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 7 var? 0 7 var? not and if
                fiona-who 49 -49 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 6 var? 0 6 var? not and if
                fiona-who -40 -40 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 5 var? 0 5 var? not and if
                fiona-who -25 43 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 4 var? 0 4 var? not and if
                fiona-who 35 20 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 3 var? 0 3 var? not and if
                fiona-who 8 -30 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 2 var? 0 2 var? not and if
                fiona-who -22 0 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
            2 1 var? if
                fiona-who 0 0 $32 char-faces-xz? if
                    hewie-may-break-off? if
                        0 hewie-who 1 action
                    then
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            7 ebit? not if
                0 0.0 0.0 0.0 $A 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 0 9 char-zone-bits? if
                            7 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                0 0.0 0.0 0.0 $C 5 0 zone
                hewie-who 0 9 char-zone-bits? not if
                    7 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            6 ebit? not if
                1 -22.0 0.0 0.0 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 1 9 char-zone-bits? if
                            6 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                1 -22.0 0.0 0.0 6 5 0 zone
                hewie-who 1 9 char-zone-bits? not if
                    6 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            5 ebit? not if
                2 8.0 0.0 -30.0 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 2 9 char-zone-bits? if
                            5 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                2 8.0 0.0 -30.0 6 5 0 zone
                hewie-who 2 9 char-zone-bits? not if
                    5 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            4 ebit? not if
                3 34.65 0.0 20.0 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 3 9 char-zone-bits? if
                            4 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                3 34.65 0.0 20.0 7 5 0 zone
                hewie-who 3 9 char-zone-bits? not if
                    4 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            3 ebit? not if
                4 -25.0 0.0 43.31 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 4 9 char-zone-bits? if
                            3 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                4 -25.0 0.0 43.31 7 5 0 zone
                hewie-who 4 9 char-zone-bits? not if
                    3 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            2 ebit? not if
                5 -40.37 0.0 -40.38 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 5 9 char-zone-bits? if
                            2 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                5 -40.37 0.0 -40.38 6 5 0 zone
                hewie-who 5 9 char-zone-bits? not if
                    2 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            1 ebit? not if
                6 48.58 0.0 -48.58 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 6 9 char-zone-bits? if
                            1 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                6 48.58 0.0 -48.58 6 5 0 zone
                hewie-who 6 9 char-zone-bits? not if
                    1 ebit-clear
                then
            then
        then
        hewie-who char-here? hewie-who 0 char-cond? and if
            0 ebit? not if
                7 57.25 0.0 57.25 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 7 9 char-zone-bits? if
                            0 ebit-set
                            $80 0 hewie-do
                        then
                    then
                then
            else
                7 57.25 0.0 57.25 7 5 0 zone
                hewie-who 7 9 char-zone-bits? not if
                    0 ebit-clear
                then
            then
        then
    then
;

: water-tower-7f-1.phase2 ( -- )   \ 00440E00
    fiona-who $D $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    fiona-who $E $32 char-faces-area? if
        3 unhurried offer-action
    then
;

: water-tower-7f-1.act00 ( -- )   \ 00440E20
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $FF 1.0 0 bgm
    $F $54 fade
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    things-clear
    -1 self-idle-anim
    $C6 room-preload
    $10 $FF movie-param
    0 cutscene-track 2 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    50 hewie-trust
    world-held state-flag-set
    $FF door-reopen-unlock
    $101 door-close-off-lock
    exits-rebuild
    $C6 0 -1 hewie-to-room
    $3E resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: water-tower-7f-1.act01 ( -- )   \ 00440EE0
    self-wait-done
    hewie-bark
    self-wait-done
    2 8 var? if
        $37 57.25 57.25 -175 $204 5 self-move-to
        self-wait-done
    else 2 7 var? if
        $5C 48.58 -48.58 -84 $204 5 self-move-to
        self-wait-done
    else 2 6 var? if
        $FA -40.37 -40.38 10 $204 5 self-move-to
        self-wait-done
    else 2 5 var? if
        $1B -25.0 43.31 111 $204 5 self-move-to
        self-wait-done
    else 2 4 var? if
        $3F 34.65 20.0 -151 $204 5 self-move-to
        self-wait-done
    else 2 3 var? if
        $6A 8.0 -30.0 -44 $204 5 self-move-to
        self-wait-done
    else 2 2 var? if
        4 -22.0 0.0 89 $204 5 self-move-to
        self-wait-done
    else 2 1 var? if
        $D8 0.0 0.0 89 $204 5 self-move-to
        self-wait-done
    then then then then then then then then
    0 $A hewie-anim-root
    $A wait-frames
    self-idle-or-end
;

: water-tower-7f-1.act02 ( -- )   \ 00440FC0
    [: $33C cutscene-cue-reached? ;] wait-until
    0 8 water-tower-7f-1.cmd00
    0 7 water-tower-7f-1.cmd00
    0 6 water-tower-7f-1.cmd00
    0 5 water-tower-7f-1.cmd00
    0 4 water-tower-7f-1.cmd00
    0 3 water-tower-7f-1.cmd00
    0 2 water-tower-7f-1.cmd00
    0 1 water-tower-7f-1.cmd00
    self-idle-or-end
;

: water-tower-7f-1.act03 ( -- )   \ 00441000
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0.0 -105.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    $F 6 fade
    wait-fade
    0 say   \ "All life on the surface of the planet stretches out below the stars' radiant light. All..."
    $F 7 fade
    wait-fade
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-1.act04 ( -- )   \ 00441030
    self-wait-done
    3 -1 var-set
    4 -1 var-set
    5 -1 var-set
    6 -1 var-set
    7 -1 var-set
    8 -1 var-set
    9 -1 var-set
    $A -1 var-set
    1 8 water-tower-7f-1.cmd00
    1 7 water-tower-7f-1.cmd00
    1 6 water-tower-7f-1.cmd00
    1 5 water-tower-7f-1.cmd00
    1 4 water-tower-7f-1.cmd00
    1 3 water-tower-7f-1.cmd00
    1 2 water-tower-7f-1.cmd00
    1 1 water-tower-7f-1.cmd00
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
    0 cutscene-track 2 action
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

: water-tower-7f-1.phase5 ( -- )   \ 0047AF20
    hunted state-flag-clear
;

\ ---- registered ----
water-tower-7f-1 register-scripts   \ (its phases and actions, by their names)
