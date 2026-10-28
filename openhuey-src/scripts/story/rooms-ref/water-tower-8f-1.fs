\ story/rooms/water-tower-8f-1.fs - the event scripts of room water-tower-8f-1 ($C7; Water Tower: 8F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-8f-1
USING: engine room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant lying-down                  \ (bit 0) she lies down (act00)
1 constant stalker-near                \ (bit 1) the stalker came into area 8 while she lay
2 constant got-up                      \ (bit 2) she got up (her command)
3 constant hewie-clear                 \ (bit 3) Hewie off the floor: its way changed
4 constant spot-chosen                 \ (bit 4) a spot chosen with the cursor (confirmed)
5 constant second-model                \ (bit 5) the second model's turn (act04)
6 constant model-set                   \ (bit 6) the model set in place: Lorenzo comes
7 constant drop-remarked               \ (bit 7) its first remark made: "If I fell from here... it would be all over. Come on, Fiona."
8 constant hewie-zone3-noticed         \ (bit 8) Hewie has reacted to zone 3 (once a visit)
9 constant final-scene-on              \ (bit 9) the final scene (act01) under way
0 constant spot-var                    \ (var 0) where she lay (0..2)
1 constant tint-var                    \ (var 1) Hewie's tint (fading in)
2 constant model1-var                  \ (var 2) the first model's stage (3 .. 0)
3 constant model2-var                  \ (var 3) the second model's stage (3 .. 0)
4 constant model3-var                  \ (var 4) the third model's stage (3 .. 0)
5 constant model-var                   \ (var 5) the model being set (0..2)
7 constant cursor-x-var                \ (var 7) the cursor's x
8 constant cursor-y-var                \ (var 8) the cursor's y
0 constant model-set-sound             \ (snd 0) the model set in place
7 constant model-sound                 \ (snd 7) the model (dust)
8 constant model2-sound                \ (snd 8) the model (dust)
1 constant cant-set-sound              \ (snd 1) not a spot the model can be set in (heard plainly)

\ the screen fade (renderer +0x70) by script variable 1 with Hewie's light 0xF: byte 3 0 clear
\ (0x808080), 2 full (0x80808080); 1 fades in by 0x10 a call and 3 back out, waiting (2),
\ Hewie's +0xE4 set once there
: water-tower-8f-1.cmd00 ( b0 -- )   \ (Hewie drawn in a colour fading by event variable 1 ($RRGGBBAA as the GS has
    \ it, alpha on top): b0 0 clear, 2 full; 1 fading in by $10 a frame, 3 back out - then drawn as
    \ he is; the colour: char-tint's, as the original's renderer +0x70 with his layer $F)
    case
        0 of  tint-var $808080 var-set  1 $808080 true char-tint  endof
        2 of  tint-var $80808080 var-set  1 $80808080 true char-tint  endof
        1 of  begin  tint-var var @  1 over true char-tint  $80808080 <> while
                  1  tint-var var @ $10000000 +  var-set  yield
              repeat  1 0 false char-tint  endof
        3 of  begin  tint-var var @  1 over true char-tint  $808080 <> while
                  1  tint-var var @ $10000000 -  var-set  yield
              repeat  1 0 false char-tint  endof
    endcase ;
\ (as Room0C_Cmd01, the player only)
: water-tower-8f-1.cmd01 ( b0 -- )  var-down-by-hit ;
\ a cursor effect (RoomC7Cursor_vtable) at (x, y) kept in script variables 7 / 8, its slot in 6:
\ byte 3 0 puts it at (246, 242); 1 moves it 4 a frame by the stick or the d-pad (x 0..492, y
\ 0..420), waiting (2) until confirm (event 4 +0x5C) or cancel (+0x60); 2 ends it
: water-tower-8f-1.cmd02 ( b0 -- )   \ (the aiming cursor at variables 7 / 8: b0 0 shown at (246, 242); 1 moved 4 a
    \ frame by the arrows till confirm (event bit 4 set) or cancel (cleared); 2 gone)
    case
        0 of  cursor-x-var 246 var-set  cursor-y-var 242 var-set  cursor-draw  endof
        1 of  begin
                  cursor-step  cursor-draw
                  menu-confirm? if  spot-chosen ebit-set  exit  then
                  menu-cancel? if  spot-chosen ebit-clear  exit  then
                  yield
              again  endof
        2 of  cursor-off  endof
    endcase ;
\ (as RoomC0_Cmd01) byte 3 2 up from frame 1268 (2.79 a frame, light 0x23) and 3 from frame 25
\ (4.27, light 0xF), held at 0x80; 1 the fade fully on with light 0xA, else off with light 0x11
\ (+0x64)
: water-tower-8f-1.cmd03 ( b0 -- )   \ (the stalker drawn in a colour: b0 2 its alpha up 2.79 a frame from the scene's
    \ frame 1268, 3 up 4.27 a frame from frame 25 - held at $80, full before them, as the
    \ original's unsigned count gives; 1 full; else as he is. The colour: char-tint's, for the
    \ original's renderer +0x70 / +0x64 and his layers $23 / $F / $A / $11)
    case
        2 of  cutscene-frame dup 1268 < if  drop $80  else  1268 - s>f 2.79e f* f>s $80 min  then
              24 lshift $808080 or  $FE swap true char-tint  endof
        3 of  cutscene-frame dup 25 < if  drop $80  else  25 - s>f 4.27e f* f>s $80 min  then
              24 lshift $808080 or  $FE swap true char-tint  endof
        1 of  $FE $FF808080 true char-tint  endof
        >r  $FE 0 false char-tint  r>
    endcase ;
\ (as Room2A_Cmd03) the 0xD40-byte effect Debris_vtable started with byte 3
: water-tower-8f-1.cmd04 ( b0 -- )  effects:debris ;   \ (debris falling from up the tower: b0 the spot, 3..5 in small pieces)
\ (as Room23_Cmd00) the kind-0xB character's model +0xCC8: 0 (byte 3 1) or -0.02
: water-tower-8f-1.cmd05 ( b0 -- )  drop ;   \ (character $B's model +0xCC8: 0 (b0 1) or -0.02 - its springs: not simulated here yet)
\ (as Room48_Cmd02) byte 3 0 starts the effect BackdropModel_vtable (its slot in event variable
\ 9); else that one is ended (EffectMgr_Remove)
: water-light ( -- id )  0 $40404040  0e 30.1e 0e  3750e 0.05e 0.01e  effects:caustic ;   \ (BackdropModel)
: water-tower-8f-1.cmd06 ( b0 -- )  9 ['] water-light effect-in-var ;   \ (the light on the water below: b0 0 made - kept in variable 9 - else gone)
\ script variables 7 / 8 (the player's spot) in 151..269 / 171..219
: water-tower-8f-1.cond00? ( -- flag )   \ (vars 7 / 8, the player's spot, in 151..269 / 171..219)
    cursor-x-var var @ $FFFF and $97 $10E within  cursor-y-var var @ $FFFF and $AB $DC within and ;
\ a room callback: the pursuer's Pursuer_GrabHewieBehind
: water-tower-8f-1.cond01? ( -- flag )  grab-hewie-behind? ;   \ (the stalker's Pursuer_GrabHewieBehind)

: water-tower-8f-1.enter ( -- )   \ 0042F500
    room-sounds
    $1B 1.0 0 bgm
    c7-second-event story-flag? not if
        0 part-hidden
        1 part-hidden
        8 1 object-hidden
        9 1 object-hidden
        $A 1 object-hidden
    else
        0 part-shown
        1 part-shown
    then
    wt8f-1-flag1 story-flag? not if
        model1-var 3 var-set
        2 part-shown
        3 part-hidden
    else
        2 part-hidden
        3 part-shown
    then
    wt8f-1-flag3 story-flag? not if
        model2-var 3 var-set
        4 part-shown
        5 part-hidden
    else
        4 part-hidden
        5 part-shown
    then
    wt8f-1-flag4 story-flag? not if
        model3-var 3 var-set
        6 part-shown
        7 part-hidden
    else
        6 part-hidden
        7 part-shown
    then
    8 part-shown
    0 water-tower-8f-1.cmd06
;

: water-tower-8f-1.char-enter ( -- )   \ 0042F590
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
;

: water-tower-8f-1.phase1 ( -- )   \ 0042F5D0
    fg-3-flag3 story-flag? c7-second-event story-flag? and wt8f-1-flag2 story-flag? not and if
        fiona-who 0 char-entered-area? if
            wt8f-1-flag2 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who $A action-force
            else
                1 fiona-who $A action-force
            then
        then
    else 0 exit-usable? if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 0 0 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 1 -1 1 chars-area-camera
    0 20000.0 camera-value
    stalker-near ebit? not got-up ebit? not and if
        lying-down ebit? not if
            0 control-action? if
                fiona-who 5 char-in-area? if
                    fiona-who -6 -12 $3C char-faces-xz? if
                        hewie-may-break-off? if
                            spot-var 0 var-set
                            0 hewie-who 0 action
                        then
                    then
                else fiona-who 6 char-in-area? if
                    fiona-who -9 10 $3C char-faces-xz? if
                        hewie-may-break-off? if
                            spot-var 1 var-set
                            0 hewie-who 0 action
                        then
                    then
                else fiona-who 7 char-in-area? if
                    fiona-who 12 7 $3C char-faces-xz? if
                        hewie-may-break-off? if
                            spot-var 2 var-set
                            0 hewie-who 0 action
                        then
                    then
                then then then
            then
        else 45 fiona-started? if
            stalker-who 2 char-cond? not if
                stalker-free? if
                    spot-var 0 var? if
                        stalker-who 8 char-in-area? if
                            water-tower-8f-1.cond01? if
                                stalker-near ebit-set
                                wt8f-1-flag1 story-flag? if
                                    panic-eased
                                    0 fiona-who 8 action-force
                                then
                            then
                        then
                    else spot-var 1 var? if
                        stalker-who 9 char-in-area? if
                            water-tower-8f-1.cond01? if
                                stalker-near ebit-set
                            then
                        then
                    else spot-var 2 var? if
                        stalker-who $A char-in-area? if
                            water-tower-8f-1.cond01? if
                                stalker-near ebit-set
                            then
                        then
                    then then then
                then
            then
        else 44 fiona-started? if
            got-up ebit-set
        then then then
    then
    wt8f-1-flag1 story-flag? not if
        0 3.0 620.0 79.0 $A 8 0 zone
        fiona-who $C char-in-area? fiona-who 0 char-in-zone? and if
            model-var 0 var-set
            0 1 track 6 action
        then
    then
    wt8f-1-flag3 story-flag? not if
        1 -58.0 620.0 54.0 $A 8 0 zone
        fiona-who $D char-in-area? fiona-who 1 char-in-zone? and if
            model-var 1 var-set
            0 2 track 6 action
        then
    then
    wt8f-1-flag4 story-flag? not if
        2 -5.0 620.0 -78.0 $A 8 0 zone
        fiona-who $E char-in-area? fiona-who 2 char-in-zone? and if
            model-var 2 var-set
            0 3 track 6 action
        then
    then
    c7-second-event story-flag? not if
        3 0.0 620.0 41.0 $11 10 0 zone
        hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        hewie-zone3-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 3 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
;

: water-tower-8f-1.phase2 ( -- )   \ 0042F7B0
    scene-5-pending state-flag? if
        -2147483646 scene-request? if
            9 always offer-action
        then
    then
    fiona-who $B char-in-area? fiona-who 90 $3C char-heading? and if
        2 unhurried offer-action
    then
    wt8f-1-flag3 story-flag? not if
        fiona-who $D char-in-area? fiona-who -58 54 $32 char-faces-xz? and if
            $E unhurried offer-action
        then
    then
    wt8f-1-flag4 story-flag? not if
        fiona-who $E char-in-area? fiona-who -5 -78 $32 char-faces-xz? and if
            $E unhurried offer-action
        then
    then
    wt8f-1-flag1 story-flag? not if
        fiona-who $C char-in-area? fiona-who 3 79 $32 char-faces-xz? and if
            $E unhurried offer-action
        then
    else c7-second-event story-flag? if
        fiona-who $C char-in-area? fiona-who 3 79 $32 char-faces-xz? and if
            $F unhurried offer-action
        then
    then then
    fiona-who $F char-in-area? fiona-who -6 -12 $3C char-faces-xz? and if
        $12 unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who -9 10 $3C char-faces-xz? and if
        $12 unhurried offer-action
    then
    fiona-who $11 char-in-area? fiona-who 12 7 $3C char-faces-xz? and if
        $12 unhurried offer-action
    then
;

: water-tower-8f-1.phase3 ( -- )   \ 0042F860
    final-scene-on ebit? not if
        17.4 630.0 89.8 -59.9 630.0 69.0 17.4 580.0 89.8 -59.9 580.0 69.0 lights-doorway
    then
    -51.2 630.0 75.7 -91.2 630.0 6.5 -51.2 580.0 75.7 -91.2 580.0 6.5 lights-doorway
    -23.7 617.0 -26.3 -33.3 617.0 12.9 -55.3 617.0 -57.8 -74.5 617.0 29.0 lights-doorway
    6.0 617.0 -35.0 -29.8 617.0 -19.9 15.6 617.0 -78.5 -66.5 617.0 -44.5 lights-doorway
    27.8 617.0 -23.1 6.0 617.0 -35.0 66.5 617.0 -44.4 15.6 617.0 -78.5 lights-doorway
    7.0 617.0 40.4 35.1 617.0 6.9 15.6 617.0 78.5 78.5 617.0 15.6 lights-doorway
    35.1 617.0 6.9 27.8 617.0 -23.0 78.5 617.0 15.6 66.5 617.0 -44.4 lights-doorway
;

: water-tower-8f-1.phase5 ( -- )   \ 0042F9C0
    hewie-who 0 char-in-nav-group? if
        hewie-who action-end
        spot-var 0 var? if
            hewie-who $33 2.82 68.25 14 char-to-xz
        else spot-var 1 var? if
            hewie-who $38 -64.059 -24.05 -107 char-to-xz
        else spot-var 2 var? if
            hewie-who $4C 58.05 -31.28 121 char-to-xz
        then then then
    then
;

: water-tower-8f-1.act00 ( -- )   \ 0042FA10
    self-wait-done
    hewie-bark
    self-wait-done
    spot-var 0 var? if
        $91 -15.35 -32.75 20 $FFFF $A self-move-to
        self-wait-done
    else spot-var 1 var? if
        $C7 -22.68 28.06 140 $FFFF $A self-move-to
        self-wait-done
    else spot-var 2 var? if
        $14 31.63 18.64 -120 $FFFF $A self-move-to
        self-wait-done
    then then then
    1 self-scripted
    1 self-noclip
    0 solid-floor floor-off
    0 keeps-fiona-off keeps-pursuers-off or floor-on
    spot-var 0 var? if
        $C4 -5.62 -11.6 20 $204 5 self-move-to
        self-wait-done
    else spot-var 1 var? if
        $C0 -8.96 10.23 140 $204 5 self-move-to
        self-wait-done
    else spot-var 2 var? if
        $BE 11.74 6.8 -120 $204 5 self-move-to
        self-wait-done
    then then then
    2 water-tower-8f-1.cmd00
    3 water-tower-8f-1.cmd00
    spot-var 0 var? if
        hewie-who $D0 -0.12 8.95 5 char-to-xz
    else spot-var 1 var? if
        hewie-who $FF -7.73 -4.22 -125 char-to-xz
    else spot-var 2 var? if
        hewie-who $F9 6.6 -4.24 117 char-to-xz
    then then then
    1 self-scripted
    0 self-noclip
    $204 0 hewie-anim-root
    0 water-tower-8f-1.cmd00
    1 water-tower-8f-1.cmd00
    hewie.anims:stand-anim 8 self-anim-blend
    8 wait-frames
    lie-down-anim play-out
    lie-anim $A self-anim-blend
    $A wait-frames
    lying-down ebit-set
    begin
        stalker-near ebit? not got-up ebit? not and while
        spot-var 0 var? if
            stalker-who 8 char-in-area? if
                stalker-who self-look-at
                yield
            else fiona-who 8 char-in-area? if
                fiona-who self-look-at
                yield
            else
                self-who self-look-at
                yield
            then then
        else spot-var 1 var? if
            stalker-who 9 char-in-area? if
                stalker-who self-look-at
                yield
            else fiona-who 9 char-in-area? if
                fiona-who self-look-at
                yield
            else
                self-who self-look-at
                yield
            then then
        else spot-var 2 var? if
            stalker-who $A char-in-area? if
                stalker-who self-look-at
                yield
            else fiona-who $A char-in-area? if
                fiona-who self-look-at
                yield
            else
                self-who self-look-at
                yield
            then then
        then then then
        yield
    repeat
    self-who self-look-at
    yield
    stalker-near ebit? if
        $75 0 hewie-do
        hewie-clear ebit-clear
        begin
            hewie-clear ebit? not if
                hewie-who $38 char-on-nav-flags? not if
                    hewie-clear ebit-set
                    0 keeps-fiona-off keeps-pursuers-off or floor-off
                    0 solid-floor floor-on
                then
            then
            117 hewie-action? while
            yield
        repeat
        hewie-who $38 char-on-nav-flags? if
            spot-var 0 var? if
                $33 2.39 70.98 1.0 100 hewie-go-to
                self-wait-done
            else spot-var 1 var? if
                $38 -61.02 -37.59 1.0 100 hewie-go-to
                self-wait-done
            else spot-var 2 var? if
                $4C 61.9 -34.57 1.0 100 hewie-go-to
                self-wait-done
            then then then
            0 keeps-fiona-off keeps-pursuers-off or floor-off
            0 solid-floor floor-on
        then
    else
        hewie-bark
        self-wait-done
        spot-var 0 var? if
            $33 2.39 70.98 1.0 100 hewie-go-to
            self-wait-done
        else spot-var 1 var? if
            $38 -61.02 -37.59 1.0 100 hewie-go-to
            self-wait-done
        else spot-var 2 var? if
            $4C 61.9 -34.57 1.0 100 hewie-go-to
            self-wait-done
        then then then
        0 keeps-fiona-off keeps-pursuers-off or floor-off
        0 solid-floor floor-on
    then
    lying-down ebit-clear
    stalker-near ebit-clear
    got-up ebit-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-8f-1.act01 ( -- )   \ 0042FCA0
    $FF 1.0 0 bgm
    stalkers-stay state-flag-set
    1 self-scripted
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $F 4 fade
    wait-fade
    stalker-who action-end
    stalker-who char-done
    hewie-who action-end
    hewie-who char-done
    0 cutscene-track $C action
    final-scene-on ebit-set
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    final-scene-on ebit-clear
    1 water-tower-8f-1.cmd03
    scene-5-pending state-flag-clear
    no-stalker-camera state-flag-clear
    stalker-who action-end
    stalker-who char-done
    hewie-who action-end
    hewie-who char-done
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $D action
    panic-eased
    0 message-prepare   \ "Fiona!"
    1 result? if
        scene-play
    then
    $40 resident-flag-set
    50 hewie-trust
    stalker-who action-end
    stalker-who char-done
    c7-second-event story-flag-set
    fiona-who $33 -1.38 70.2 0 char-to-xz
    hewie-who $33 5.0 67.76 0 char-to-xz
    fg-3-flag3 story-flag? not if
        2 0 char-remove
    then
    camera-restart
    $103 door-unlock
    $100 door-unlock
    $E0 door-lock
    stalker-who $C5 0 room-doors-state
    stalker-who $C6 0 room-doors-state
    stalker-who $C7 0 room-doors-state
    0 solid-floor floor-on
    lying-down ebit-clear
    stalker-near ebit-clear
    got-up ebit-clear
    $1B 1.0 0 bgm
    $F $41 fade
    wait-fade
    $41 resident-flag-set
    $24D item-give
    $828B item-give
    shared.act9C
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-8f-1.act02 ( -- )   \ 0042FE40
    self-wait-done
    c7-second-event story-flag? not if
        1 self-scripted
        1 self-noclip
        8 wait-frames
        world-frozen state-flag-set
        fiona-who $36 0.0 33.0 180 char-to-xz
        1 10.0 60.0 0.0 5.0 event-camera
        8 wait-frames
        4 say   \ "Wow. I've never seen anything like this before... Looks like some kind of diorama o..."
        8 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 self-scripted
        0 self-noclip
        fiona-who $9D 0.0 41.0 180 char-to-xz
    else $A4 1 item-count? if
        1 self-scripted
        1 self-noclip
        8 wait-frames
        world-frozen state-flag-set
        fiona-who $D2 -0.95 33.0 180 char-to-xz
        1 2.0 50.0 0.0 5.0 event-camera
        8 wait-frames
        $A say   \ "So... the 'magisterium' listed in the memo... was actually referring to this minia..."
        8 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 self-scripted
        0 self-noclip
        fiona-who $9D 0.0 41.0 180 char-to-xz
    else
        $B say   \ "So the miniature was the activation switch?"
    then then
    self-idle-or-end
;

: water-tower-8f-1.act07 ( -- )   \ 00430110
    $F $44 fade
    $FF 1.0 0 bgm
    wait-fade
    8 part-hidden
    1 water-tower-8f-1.cmd06
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    stalkers-stay state-flag-set
    hewie-who action-end
    hewie-who char-done
    2 char-start
    summoner-on state-flag-clear
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $B action
    $10 $50 movie-param
    panic-eased
    3 message-prepare   \ "Fiona! Why must you defy me? Why do you run? Let me into your womb!"
    1 result? if
        scene-play
    then
    1 part-shown
    8 part-shown
    0 water-tower-8f-1.cmd06
    $100 door-open-clear
    $100 door-lock
    doors-room-in
    camera-restart
    fiona-who $33 6.38 70.12 180 char-to-xz
    hewie-who char-activate
    $C7 0 41 hewie-to-room
    hewie-who $29 -17.04 65.97 -160 char-to-xz
    stalker-who char-activate
    $FE $C7 89 2 stalker-to-room
    stalker-who $59 -58.86 10.32 0 char-to-xz
    0 water-tower-8f-1.cmd03
    0 solid-floor floor-on
    lying-down ebit-clear
    stalker-near ebit-clear
    got-up ebit-clear
    scene-5-pending state-flag-set
    9 1.0 0 bgm
    no-stalker-camera state-flag-set
    $F 1 fade
    wait-fade
    $3F resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-8f-1.act05 ( -- )   \ 0042FF20
    self-wait-done
    1 self-scripted
    1 self-noclip
    8 wait-frames
    world-frozen state-flag-set
    fiona-who $36 0.0 33.0 180 char-to-xz
    1 10.0 60.0 0.0 5.0 event-camera
    8 wait-frames
    0 water-tower-8f-1.cmd02
    5 show-message   \ "Choose a location to set the model in. ?:USE ?:CANCEL"
    begin
        1 water-tower-8f-1.cmd02
        spot-chosen ebit? if
            second-model ebit? not water-tower-8f-1.cond00? not or if
                cant-set-sound $86 0.0 0.0 0.0 0 0 sound
                yield
            else
                model-set-sound 6 -1.0 632.0 31.0 0 0 sound
                0 part-shown
                $23 item-use
                model-set ebit-set
                2 0 char-remove
                $25 partner-load
                5 message-close
                2 water-tower-8f-1.cmd02
                $1E wait-frames
                model-set ebit? if
                    ['] water-tower-8f-1.act07 goto
                then
                0 0.0 0.0 0.0 0.0 event-camera
                world-frozen state-flag-clear
                0 self-scripted
                0 self-noclip
                fiona-who $9D 0.0 41.0 180 char-to-xz
                self-idle-or-end
            then
        else
            $2C $85 0.0 0.0 0.0 0 0 sound
            5 message-close
            2 water-tower-8f-1.cmd02
            $1E wait-frames
            model-set ebit? if
                ['] water-tower-8f-1.act07 goto
            then
            0 0.0 0.0 0.0 0.0 event-camera
            world-frozen state-flag-clear
            0 self-scripted
            0 self-noclip
            fiona-who $9D 0.0 41.0 180 char-to-xz
            self-idle-or-end
        then
    again
;

: water-tower-8f-1.act03 ( -- )   \ 0047ADD0
    second-model ebit-clear
    ['] water-tower-8f-1.act05 goto
;

: water-tower-8f-1.act04 ( -- )   \ 0047ADD8
    second-model ebit-set
    ['] water-tower-8f-1.act05 goto
;

: water-tower-8f-1.act06 ( -- )   \ 00430000
    fiona-who model-sound 6 char-sound
    model-var 0 var? if
        2 water-tower-8f-1.cmd01
        0 -1.0 623.0 77.0 0 0 0 0 dust
        0 4.0 623.0 77.0 0 0 0 0 dust
        model1-var 0 var? if
            fiona-who model2-sound 6 char-sound
            0 water-tower-8f-1.cmd04
            3 water-tower-8f-1.cmd04
            2 part-hidden
            3 part-shown
            wt8f-1-flag1 story-flag-set
        then
    else model-var 1 var? if
        3 water-tower-8f-1.cmd01
        0 -59.0 623.0 50.0 0 0 0 0 dust
        0 -55.0 623.0 54.0 0 0 0 0 dust
        model2-var 0 var? if
            fiona-who model2-sound 6 char-sound
            1 water-tower-8f-1.cmd04
            4 water-tower-8f-1.cmd04
            4 part-hidden
            5 part-shown
            wt8f-1-flag3 story-flag-set
        then
    else model-var 2 var? if
        4 water-tower-8f-1.cmd01
        0 -8.0 623.0 -77.0 0 0 0 0 dust
        0 -2.0 623.0 -77.0 0 0 0 0 dust
        model3-var 0 var? if
            fiona-who model2-sound 6 char-sound
            2 water-tower-8f-1.cmd04
            5 water-tower-8f-1.cmd04
            6 part-hidden
            7 part-shown
            wt8f-1-flag4 story-flag-set
        then
    then then then
    $1E wait-frames
    self-idle-or-end
;

: water-tower-8f-1.act08 ( -- )   \ 00430250
    1 self-scripted
    self-wait-done
    stalker-who self-look-at
    yield
    go-for-it-gesture-anim self-anim
    fiona-who 1 char-wait-motion
    fiona-who $2F 5 char-sound
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    ['] water-tower-8f-1.act01 goto
;

: water-tower-8f-1.act09 ( -- )   \ 0047ADE0
    2 show-message   \ "It's locked!"
    self-idle-or-end
;

: water-tower-8f-1.act0A ( -- )   \ 00430270
    $F $54 fade
    $FF 1.0 0 bgm
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    wait-fade
    self-who 1 char-visible
    $B 3 $FF char-load
    $B $1E char-layer
    stalker-who $A char-layer
    3 char-start
    hewie-who action-end
    hewie-who char-done
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    $2D $41 movie-param
    0 cutscene-track $11 action
    panic-eased
    1 message-prepare   \ "Riccardo... You were always... incomplete. But now, I will have your Azoth. You..."
    1 result? if
        scene-play
    then
    1 water-tower-8f-1.cmd05
    2 0 char-remove
    3 0 char-remove
    world-held state-flag-set
    0 exit-prepare
    wt8f-1-flag2 story-flag-set
    $42 resident-flag-set
    $81 exit-check
    self-idle-or-end
;

: water-tower-8f-1.act0B ( -- )   \ 00430330
    0 water-tower-8f-1.cmd03
    [: $4F5 cutscene-cue-reached? ;] wait-until
    begin
        $523 cutscene-cue-reached? not while
        2 water-tower-8f-1.cmd03
        yield
    repeat
    1 water-tower-8f-1.cmd03
    self-idle-or-end
;

: water-tower-8f-1.act0C ( -- )   \ 00430350
    0 water-tower-8f-1.cmd03
    [: $1A cutscene-cue-reached? ;] wait-until
    begin
        $38 cutscene-cue-reached? not while
        3 water-tower-8f-1.cmd03
        yield
    repeat
    1 water-tower-8f-1.cmd03
    self-idle-or-end
;

: water-tower-8f-1.act0D ( -- )   \ 00430370
    [: 0 cutscene-shot? ;] wait-until
    begin
        $E8 cutscene-cue-reached? not while
        $3E sprites-additive
        yield
    repeat
    [: $E9 cutscene-cue-reached? ;] wait-until
    begin
        $11C cutscene-cue-reached? not while
        $29 sprites-additive
        yield
    repeat
    self-idle-or-end
;

: water-tower-8f-1.act0E ( -- )   \ 0047ADE8
    self-wait-done
    6 say   \ "This wall is in really bad shape. It looks like it might break if a strong force impact..."
    self-idle-or-end
;

: water-tower-8f-1.act0F ( -- )   \ 004303A0
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    drop-remarked ebit? not if
        drop-remarked ebit-set
        7 say   \ "If I fell from here... it would be all over. Come on, Fiona... Best to just keep it..."
    else
        drop-remarked ebit-clear
        8 say   \ "A path has appeared on the surface of the lake. If I can just cross over that path..."
    then
    self-wait-anim
    self-idle-or-end
;

: water-tower-8f-1.act10 ( -- )   \ 0047ADF0
    self-wait-done
    $C say   \ "If I try this out again and seal off my escape route I'd have no one to blame but myse..."
    self-idle-or-end
;

: water-tower-8f-1.act11 ( -- )   \ 004303C0
    [: 4 cutscene-shot? ;] wait-until
    0 water-tower-8f-1.cmd05
    self-idle-or-end
;

: water-tower-8f-1.act12 ( -- )   \ 004303D0
    self-wait-done
    fiona-who $F char-in-area? if
        -6.0 -12.0 self-turn-to-xz
        self-wait-done
    else fiona-who $10 char-in-area? if
        -9.0 10.0 self-turn-to-xz
        self-wait-done
    else fiona-who $11 char-in-area? if
        12.0 7.0 self-turn-to-xz
        self-wait-done
    then then then
    look-low-anim self-anim
    16 wait-frames
    9 say   \ "There is a small hole... It looks just big enough for Hewie to fit in."
    self-wait-anim
    self-idle-or-end
;

: water-tower-8f-1.act13 ( -- )   \ 00430410
    self-wait-done
    0 part-shown
    2 part-shown
    4 part-shown
    6 part-shown
    8 part-shown
    0 20000.0 camera-value
    $25 partner-load
    2 char-start
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $B action
    $10 $50 movie-param
    panic-eased
    3 message-prepare   \ "Fiona! Why must you defy me? Why do you run? Let me into your womb!"
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

: water-tower-8f-1.act14 ( -- )   \ 004304C0
    self-wait-done
    0 part-shown
    1 part-shown
    3 part-shown
    5 part-shown
    7 part-shown
    8 part-shown
    0 20000.0 camera-value
    $25 partner-load
    2 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $C action
    final-scene-on ebit-set
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    final-scene-on ebit-clear
    1 water-tower-8f-1.cmd03
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: water-tower-8f-1.act15 ( -- )   \ 00430580
    self-wait-done
    0 part-shown
    1 part-shown
    3 part-shown
    5 part-shown
    7 part-shown
    8 part-shown
    0 20000.0 camera-value
    $17 3 $FF char-load
    3 char-start
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $D action
    panic-eased
    0 message-prepare   \ "Fiona!"
    1 result? if
        scene-play
    then
    hewie-who action-end
    hewie-who char-done
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: water-tower-8f-1.act16 ( -- )   \ 00430630
    self-wait-done
    0 part-shown
    1 part-shown
    3 part-shown
    5 part-shown
    7 part-shown
    8 part-shown
    0 20000.0 camera-value
    $17 3 $FF char-load
    $B 4 $FF char-load
    3 char-start
    4 char-start
    self-who 1 char-visible
    $B $1E char-layer
    stalker-who $A char-layer
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    $2D $41 movie-param
    0 cutscene-track $11 action
    panic-eased
    1 message-prepare   \ "Riccardo... You were always... incomplete. But now, I will have your Azoth. You..."
    1 result? if
        scene-play
    then
    1 water-tower-8f-1.cmd05
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
water-tower-8f-1 register-scripts   \ (its phases and actions, by their names)
