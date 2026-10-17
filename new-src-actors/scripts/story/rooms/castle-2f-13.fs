\ story/rooms/castle-2f-13.fs - the event scripts of room castle-2f-13 ($28; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-13
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ the pursuer's Pursuer_GrabHewieBehind
: castle-2f-13.cond00? ( b0 -- flag )  drop grab-hewie-behind? ;   \ (the stalker's Pursuer_GrabHewieBehind)

: castle-2f-13.enter ( -- )   \ 00403980
    $16 1.0 0 bgm
    c2f-13-flag1 story-flag? not if
        0 -231.0 61.0 24.0 flicker-sprite
    then
    c2f-13-medallion story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
;

: castle-2f-13.char-enter ( -- )   \ 004039B0
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
    hewie-who self-is? if
        118 hewie-action? not if
            hewie-who $1C3 char-on-tri? hewie-who $1D0 char-on-tri? or hewie-who $1EA char-on-tri? or if
                hewie-who $13C -90 char-to-tri-facing
            then
        then
    then
;

: castle-2f-13.phase1 ( -- )   \ 00403A10
    0 exit-usable? if
        0 exit-check
    then
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        3 map-page
    then
    fiona-who 3 char-entered-area? if
        4 map-page
    then
    found-file-227 story-flag? not 1 ebit? not and if
        fiona-who 6 char-in-area? if
            0 var-inc
            0 30 var? if
                1 ebit-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 0 action-force
                else
                    1 fiona-who 0 action-force
                then
                fiona-who 0 char-action? if
                    6 ebit-set
                then
            then
        else
            0 0 var-set
        then
        stalker-who 6 char-in-area? if
            1 var-inc
            1 30 var? if
                1 ebit-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 1 action-force
                else
                    1 fiona-who 1 action-force
                then
            then
        else
            1 0 var-set
        then
    then
    hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
        hewie-who $1C3 char-on-tri? hewie-who $1D0 char-on-tri? or hewie-who $1EA char-on-tri? or if
            44 fiona-started? if
                0 hewie-who 7 action
            else stalker-who 2 char-cond? not stalker-who 8 char-in-area? and if
                stalker-free? if
                    5 ebit? not if
                        0 solid-floor floor-off
                        0 keeps-fiona-off keeps-pursuers-off or floor-on
                        0 castle-2f-13.cond00? if
                            5 ebit-set
                            0 $F1 8 action
                        else
                            0 solid-floor floor-on
                            0 keeps-fiona-off keeps-pursuers-off or floor-off
                        then
                    then
                then
            then then
        else 2 game-mode? not if
            0 control-action? if
                -163 97 -50 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                    fiona-who -163 -50 $32 char-faces-xz? if
                        hewie-may-break-off? if
                            0 hewie-who 5 action
                        then
                    then
                then
            then
        then then
    then
    2 -231.29 60.0 30.58 $32 20 0 zone
    3 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    3 ebit-set
                    $64 chance? if
                        hewie-zone-var 2 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    c2f-13-medallion story-flag? not hewie-who 7 char-in-area? and if
        1 -149.92 60.0 -50.71 $36 36 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            hewie-zone-var 1 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    1 -149.92 60.0 -50.71 $36 36 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 36.0 hewie-look-zone
            then
        then
    then
    3 -228.31 60.0 25.69 $23 10 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 0.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-13.phase2 ( -- )   \ 00403C10
    fiona-who 4 char-in-area? fiona-who 90 $32 char-heading? and if
        4 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 45 $32 char-heading? and if
        9 unhurried offer-action
    then
;

: castle-2f-13.phase5 ( -- )   \ 00403C30
    hewie-who char-here? if
        118 hewie-action? if
            hewie-who $1C3 char-on-tri? hewie-who $1D0 char-on-tri? or hewie-who $1EA char-on-tri? or if
                hewie-who $1EA -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else hewie-who 0 char-in-nav-group? hewie-who $1C3 char-on-tri? or hewie-who $1D0 char-on-tri? or hewie-who $1EA char-on-tri? or if
            hewie-who $13C char-to-tri
        then then
    then
;

: castle-2f-13.act02 ( -- )   \ 00403D00
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $F $44 fade
    wait-fade
    world-held state-flag-set
    stalker-who char-here? if
        0 stalker-who 3 action
    then
    $35 $36 door-copy
    $36 door-close-off-lock
    exits-rebuild
    exit
;

: castle-2f-13.act00 ( -- )   \ 00403C80
    stalkers-stay state-flag-set
    stalkers-blind state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    8 room-preload
    $E 3 $FF char-load
    $F 4 figure-load
    self-wait-done
    6 ebit? if
        rest-anim self-anim
    then
    castle-2f-13.act02
    hewie-who char-here? if
        8 0 -1 hewie-to-room
    then
    stalkers-blind state-flag-clear
    hewie-no-attack state-flag-clear
    $80 exit-check
    self-idle-or-end
;

: castle-2f-13.act01 ( -- )   \ 00403CC0
    stalkers-stay state-flag-set
    stalkers-blind state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    stalker-who 0 0 char-camera
    stalker-who camera-follow
    8 room-preload
    $E 3 $FF char-load
    $F 4 figure-load
    self-wait-done
    castle-2f-13.act02
    hewie-who char-here? if
        $2F 0 -1 hewie-to-room
    then
    stalkers-blind state-flag-clear
    hewie-no-attack state-flag-clear
    $81 exit-check
    self-idle-or-end
;

: castle-2f-13.act03 ( -- )   \ 0047AB30
    begin
        yield
    again
;

: castle-2f-13.act04 ( -- )   \ 00403D30
    self-wait-done
    0 ebit? not if
        0 message   \ "It's a suspended bridge."
        wait-message
        0 ebit-set
    else
        1 message   \ "If I lower the bridge somehow, I can use it to cross."
        wait-message
    then
    self-idle-or-end
;

: castle-2f-13.act05 ( -- )   \ 00403D50
    self-wait-done
    hewie-who 0 char-file-load
    hewie-bark
    self-wait-done
    $AE -152.54 -106.55 0 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    1 self-noclip
    0 room-motion 5 self-anim-blend8
    self-wait-anim
    0 keeps-fiona-off keeps-pursuers-off or floor-off
    0 self-noclip
    c2f-13-medallion story-flag? not if
        c2f-13-medallion story-flag? not if
            fiona-half-hidden state-flag? fiona-who char-busy? not or if
                $70 $63 item-count? not if
                    10 hewie-trust
                then
                $70 message-param-room
                $70 $63 item-count? if
                    $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
                    wait-message
                else
                    c2f-13-medallion story-flag-set
                    $70 1 item-give-count
                    0 $70 item-tab
                    item-fanfare
                    $8011 message   \ "You've obtained ."
                    wait-message
                then
                c2f-13-medallion story-flag? if
                    1 effect-remove
                then
            then
        then
        1 counter-set
        $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 solid-floor floor-off
        0 keeps-fiona-off keeps-pursuers-off or floor-on
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        0 solid-floor floor-on
        0 keeps-fiona-off keeps-pursuers-off or floor-off
    else
        1 counter-set
        $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        lie-down-anim self-anim
        self-wait-anim
        lie-anim self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: castle-2f-13.act06 ( -- )   \ 00403E40
    self-wait-done
    $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 solid-floor floor-off
    0 keeps-fiona-off keeps-pursuers-off or floor-on
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    0 solid-floor floor-on
    0 keeps-fiona-off keeps-pursuers-off or floor-off
    5 ebit-clear
    self-idle-or-end
;

: castle-2f-13.act07 ( -- )   \ 00403E90
    self-wait-done
    hewie-bark
    self-wait-done
    $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 solid-floor floor-off
    0 keeps-fiona-off keeps-pursuers-off or floor-on
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    0 solid-floor floor-on
    0 keeps-fiona-off keeps-pursuers-off or floor-off
    self-idle-or-end
;

: castle-2f-13.act08 ( -- )   \ 00403EE0
    $75 0 hewie-do
    begin
        117 hewie-action? while
        yield
    repeat
    hewie-who $1C3 char-on-tri? hewie-who $1D0 char-on-tri? or hewie-who $1EA char-on-tri? or if
        0 hewie-who 6 action
    else
        0 solid-floor floor-on
        0 keeps-fiona-off keeps-pursuers-off or floor-off
        5 ebit-clear
        5 hewie-trust
    then
    self-idle-or-end
;

: castle-2f-13.act09 ( -- )   \ 00403F20
    self-wait-done
    90 self-turn-angle
    self-wait-done
    4 ebit? not if
        3 message   \ "The body of a beast and the head of a bird... What a strange-looking statue."
        wait-message
        4 ebit-set
    else
        4 message   \ "Some ancient castles used to line their walls with gargoyles, as it was believed to..."
        wait-message
        4 ebit-clear
    then
    self-idle-or-end
;

\ ---- registered ----
castle-2f-13 register-scripts   \ (its phases and actions, by their names)
