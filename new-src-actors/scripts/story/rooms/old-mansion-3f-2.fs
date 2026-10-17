\ story/rooms/old-mansion-3f-2.fs - the event scripts of room old-mansion-3f-2 ($63; Belli Castle: Old Mansion 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-3f-2
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ room 0x63: the script's character walks to Fiona's nav triangle (character move 6, with
\ 0x204).
: old-mansion-3f-2.cmd00 ( -- )  fiona-who char-tri dup 0< if  drop exit  then  $204 6 self-move-tri ;   \ (to Fiona's triangle)

defer old-mansion-3f-2.act05
defer old-mansion-3f-2.act06
defer old-mansion-3f-2.act07
: old-mansion-3f-2.enter ( -- )   \ 00421130
    room-sounds
    om3f-2-flag1 story-flag? not if
        panic-eased
        0 fiona-who 0 action-force
    then
    armor-button-pushed story-flag? not if
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            0 exit-taken? if
                hewie-who $187 char-to-tri
            else
                hewie-who $195 char-to-tri
            then
        then
        0 keeps-hewie-off floor-on
        1 keeps-hewie-off floor-on
        2 keeps-hewie-off floor-on
        3 keeps-hewie-off floor-on
    then
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
    1 $2300 sound-volume
;

: old-mansion-3f-2.char-enter ( -- )   \ 004211D0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 -1 player-camera
    then then
    1 0 -1 area-camera
;

: old-mansion-3f-2.phase1 ( -- )   \ 00421250
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    armor-button-pushed story-flag? not if
        hewie-who 2 char-cond? not stalker-who char-here? not and hewie-who char-here? and if
            35 fiona-started? 45 fiona-started? or if
                fiona-who 0 -55 $32 char-faces-xz? fiona-who 0 -100 $32 char-faces-xz? and if
                    hewie-may-break-off? if
                        0 hewie-who 5 action
                    then
                then
                fiona-who 0 55 $32 char-faces-xz? fiona-who 0 110 $32 char-faces-xz? and if
                    hewie-may-break-off? if
                        0 hewie-who 6 action
                    then
                then
            then
            44 fiona-started? if
                hewie-may-break-off? if
                    0 hewie-who 8 action
                then
            then
        then
        fiona-who 1 char-in-nav-group? if
            1 1 var? not if
                0 ebit-set
            then
            1 1 var-set
        else fiona-who 2 char-in-nav-group? if
            1 2 var? not if
                0 ebit-set
            then
            1 2 var-set
        else fiona-who 3 char-in-nav-group? if
            1 3 var? not if
                0 ebit-set
            then
            1 3 var-set
        else
            1 0 var-set
        then then then
        0 ebit? if
            4 ebit? not if
                fiona-who 1 6 char-sound
                0 0 var? if
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 1 action-force
                    else
                        1 fiona-who 1 action-force
                    then
                else 0 1 var? if
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 2 action-force
                    else
                        1 fiona-who 2 action-force
                    then
                else 0 2 var? if
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 3 action-force
                    else
                        1 fiona-who 3 action-force
                    then
                then then then
                0 var-inc
                4 ebit-set
            then
            0 ebit-clear
        then
    then
    armor-button-pushed story-flag? not if
        5 ebit? if
            $23C item-give
            $8281 item-give
        then
        0 -0.27 0.0 10.71 $51 8 0 zone
        5 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 hewie-who $92 action
                        then
                    then
                then
            then
        then
    then
    1 0.0 0.0 -68.2 $12 19 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-3f-2.phase2 ( -- )   \ 00421400
    fiona-who 6 char-in-area? fiona-who 0 -68 $41 char-faces-xz? and if
        9 unhurried offer-action
    then
;

: old-mansion-3f-2.phase5 ( -- )   \ 00421420
    armor-button-pushed story-flag? not if
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            fiona-who 0 char-in-area? if
                hewie-who $187 char-to-tri
            else
                hewie-who $195 char-to-tri
            then
        then
    then
;

: old-mansion-3f-2.act00 ( -- )   \ 00421440
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    0 part-hidden
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    0 self-idle-anim
    fiona-who $135 0.064 100.888 171 char-to-xz
    camera-restart
    hewie-who 0 char-visible
    hewie-who action-end
    om3f-2-flag1 story-flag-set
    0 self-scripted
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-3f-2.act04 ( -- )   \ 00421700
    8 1 object-hidden
    9 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    0 part-shown
    exit
;

: old-mansion-3f-2.act01 ( -- )   \ 00421500
    stalkers-stay state-flag-set
    1 self-scripted
    4 ebit-set
    $F $44 fade
    self-wait-done
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-busy? not if
        hewie-who char-here? hewie-who 2 char-cond? and if
            0 hewie-who $86 action
        then
    then
    self-who 1 char-visible
    0 part-hidden
    $14 $78 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        hewie-who 0 char-visible
        hewie-who action-end
    then
    self-who 0 char-visible
    old-mansion-3f-2.act04
    $F $41 fade
    wait-fade
    4 ebit-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-3f-2.act02 ( -- )   \ 004215C0
    stalkers-stay state-flag-set
    1 self-scripted
    4 ebit-set
    $F $44 fade
    self-wait-done
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-busy? not if
        hewie-who char-here? hewie-who 2 char-cond? and if
            0 hewie-who $86 action
        then
    then
    self-who 1 char-visible
    0 part-hidden
    $14 $78 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        hewie-who 0 char-visible
        hewie-who action-end
    then
    self-who 0 char-visible
    old-mansion-3f-2.act04
    $F $41 fade
    wait-fade
    4 ebit-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-3f-2.act03 ( -- )   \ 00421680
    capture-no-end state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    no-pause state-flag-set
    4 ebit-set
    $F $44 fade
    self-wait-done
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    self-who 1 char-visible
    0 part-hidden
    $10 $46 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        movie-loop
        scene-play-through
    then
    $30 resident-flag-set
    0 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: old-mansion-3f-2.act08 ( -- )   \ 00421840
    self-wait-done
    hewie-bark
    self-wait-done
    0 keeps-hewie-off floor-off
    old-mansion-3f-2.cmd00
    begin
        self-done? not if
            35 fiona-started? 45 fiona-started? or if
                fiona-who 0 -55 $32 char-faces-xz? fiona-who 0 -100 $32 char-faces-xz? and if
                    self-idle
                    self-wait-done
                    ['] old-mansion-3f-2.act05 goto
                then
                fiona-who 0 55 $32 char-faces-xz? fiona-who 0 110 $32 char-faces-xz? and if
                    self-idle
                    self-wait-done
                    ['] old-mansion-3f-2.act06 goto
                then
            then
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act07 goto
            then
            yield
        else
            hewie-who 0 char-in-nav-group? if
                ['] old-mansion-3f-2.act07 goto
            else
                0 keeps-hewie-off floor-on
            then
            self-idle-or-end
        then
    again
;

:noname   \ old-mansion-3f-2.act06 (00421790; deferred: used before it is defined)
    self-wait-done
    hewie-bark
    self-wait-done
    0 keeps-hewie-off floor-off
    $11F $204 6 self-move-tri
    begin
        self-done? not if
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act07 goto
            then
            44 fiona-started? if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act08 goto
            then
            yield
        else
            0 keeps-hewie-off floor-on
            self-idle-or-end
        then
    again
; is old-mansion-3f-2.act06

:noname   \ old-mansion-3f-2.act07 (004217D0; deferred: used before it is defined)
    self-wait-done
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-done
    fiona-who self-look-at
    yield
    begin
        35 fiona-started? 45 fiona-started? or if
            fiona-who 0 -55 $32 char-faces-xz? fiona-who 0 -100 $32 char-faces-xz? and if
                self-who self-look-at
                yield
                $101 self-anim
                self-wait-anim
                -1 self-idle-anim
                ['] old-mansion-3f-2.act05 goto
            then
            fiona-who 0 55 $32 char-faces-xz? fiona-who 0 110 $32 char-faces-xz? and if
                self-who self-look-at
                yield
                $101 self-anim
                self-wait-anim
                -1 self-idle-anim
                ['] old-mansion-3f-2.act06 goto
            then
        then
        44 fiona-started? if
            self-who self-look-at
            yield
            $101 self-anim
            self-wait-anim
            -1 self-idle-anim
            ['] old-mansion-3f-2.act08 goto
        then
        yield
    again
; is old-mansion-3f-2.act07

:noname   \ old-mansion-3f-2.act05 (00421750; deferred: used before it is defined)
    self-wait-done
    hewie-bark
    self-wait-done
    0 keeps-hewie-off floor-off
    $187 $204 6 self-move-tri
    begin
        self-done? not if
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act07 goto
            then
            44 fiona-started? if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act08 goto
            then
            yield
        else
            0 keeps-hewie-off floor-on
            self-idle-or-end
        then
    again
; is old-mansion-3f-2.act05

: old-mansion-3f-2.act09 ( -- )   \ 004218B0
    self-wait-done
    0.0 -68.0 self-turn-to-xz
    self-wait-done
    armor-button-pushed story-flag? not if
        look-at-anim self-anim
        self-frames-reset
        self-wait-16
        0 message   \ "There is a button located on the back of the armor. Push the button? YES NO"
        wait-message
        self-wait-anim
        0 answer? if
            reach-up-anim self-anim
            self-wait-anim
            fiona-who 2 6 char-sound
            armor-button-pushed story-flag-set
            30 hewie-trust
            0 keeps-hewie-off floor-off
            1 keeps-hewie-off floor-off
            2 keeps-hewie-off floor-off
            3 keeps-hewie-off floor-off
            reach-down-anim self-anim
            self-wait-anim
        then
    else
        1 message   \ "Looks like the button was the activation switch for a booby-trap."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-3f-2.act0A ( -- )   \ 00421910
    self-wait-done
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
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

: old-mansion-3f-2.act0B ( -- )   \ 004219E0
    self-wait-done
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
    self-who 1 char-visible
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    $14 $78 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $14 $78 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $46 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-3f-2 register-scripts   \ (its phases and actions, by their names)
