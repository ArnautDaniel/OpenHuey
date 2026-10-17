\ story/rooms/castle-2f-14.fs - the event scripts of room castle-2f-14 ($2F; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-14
USING: room-names story.words story.shared flag-names story-names floor-flags hewie.anims ;

\ room 0x2F (Room2F_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind
: castle-2f-14.cond00? ( b0 -- flag )  drop grab-hewie-behind? ;   \ (the stalker's Pursuer_GrabHewieBehind)

: castle-2f-14.enter ( -- )   \ 00412950
    room-sounds
    $16 1.0 0 bgm
    c2f-13-medallion story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
    $13 hole-floor tri-floor-on
    $124 hole-floor tri-floor-on
    $6B hole-floor tri-floor-on
    $69 hole-floor tri-floor-on
    $54 hole-floor tri-floor-on
;

: castle-2f-14.char-enter ( -- )   \ 004129A0
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
    fiona-who self-is? if
        found-file-227 story-flag? not if
            fiona-who $4D -45 char-to-tri-facing
            hewie-who char-activate
            hewie-who char-here? if
                hewie-who $120 -45 char-to-tri-facing
            then
            0 0 player-camera
            camera-restart
            0 fiona-who 0 action
        then
    then
    hewie-who self-is? if
        118 hewie-action? not if
            hewie-who $2B char-on-tri? hewie-who $140 char-on-tri? or hewie-who $1DF char-on-tri? or if
                hewie-who $13C -90 char-to-tri-facing
            then
        then
    then
;

: castle-2f-14.phase1 ( -- )   \ 00412A30
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
    hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
        hewie-who $2B char-on-tri? hewie-who $140 char-on-tri? or hewie-who $1DF char-on-tri? or if
            44 fiona-started? if
                0 hewie-who 5 action
            else stalker-who 2 char-cond? not stalker-who 8 char-in-area? and if
                stalker-free? if
                    3 ebit? not if
                        0 solid-floor floor-off
                        0 keeps-fiona-off keeps-pursuers-off or floor-on
                        0 castle-2f-14.cond00? if
                            3 ebit-set
                            0 $F1 6 action
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
                            0 hewie-who 3 action
                        then
                    then
                then
            then
        then then
    then
    c2f-13-medallion story-flag? not hewie-who 7 char-in-area? and if
        0 -149.92 60.0 -50.71 $36 36 0 zone
        1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    0 -149.92 60.0 -50.71 $36 36 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-14.phase2 ( -- )   \ 00412B60
    found-file-227 story-flag? if
        fiona-who 9 char-in-area? if
            1 unhurried offer-action
        then
    then
    fiona-who 4 char-in-area? fiona-who 90 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 45 $32 char-heading? and if
        7 unhurried offer-action
    then
;

: castle-2f-14.phase5 ( -- )   \ 00412B90
    hewie-who char-here? if
        118 hewie-action? if
            hewie-who $2B char-on-tri? hewie-who $140 char-on-tri? or hewie-who $1DF char-on-tri? or if
                hewie-who $140 -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else hewie-who 0 char-in-nav-group? hewie-who $2B char-on-tri? or hewie-who $140 char-on-tri? or hewie-who $1DF char-on-tri? or if
            hewie-who $13C char-to-tri
        then then
    then
;

: castle-2f-14.act00 ( -- )   \ 00412BE0
    yield
    camera-restart
    world-held state-flag-clear
    $F $41 fade
    found-file-227 story-flag-set
    0 exit-prepare
    wait-fade
    $227 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-14.act01 ( -- )   \ 00412C00
    stalkers-stay state-flag-set
    1 self-scripted
    8 room-preload
    self-wait-done
    2 message   \ "If I choose my footing carefully, I should be able to get down this way... Climb down?..."
    wait-message
    0 answer? if
        $F 4 fade
        wait-fade
        world-held state-flag-set
        scene-locked state-flag-set
        0 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $B4 self-wait-frames
        hewie-who char-here? if
            $2F 0 288 hewie-to-room
        then
        $82 exit-check
    else
        0 exit-prepare
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: castle-2f-14.act02 ( -- )   \ 00412C50
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

: castle-2f-14.act03 ( -- )   \ 00412C70
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
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 solid-floor floor-off
        0 keeps-fiona-off keeps-pursuers-off or floor-on
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        0 solid-floor floor-on
        0 keeps-fiona-off keeps-pursuers-off or floor-off
    else
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        lie-down-anim self-anim
        self-wait-anim
        lie-anim self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: castle-2f-14.act04 ( -- )   \ 00412D60
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 solid-floor floor-off
    0 keeps-fiona-off keeps-pursuers-off or floor-on
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    0 solid-floor floor-on
    0 keeps-fiona-off keeps-pursuers-off or floor-off
    3 ebit-clear
    self-idle-or-end
;

: castle-2f-14.act05 ( -- )   \ 00412DB0
    self-wait-done
    hewie-bark
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 solid-floor floor-off
    0 keeps-fiona-off keeps-pursuers-off or floor-on
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    0 solid-floor floor-on
    0 keeps-fiona-off keeps-pursuers-off or floor-off
    self-idle-or-end
;

: castle-2f-14.act06 ( -- )   \ 00412E00
    $75 0 hewie-do
    begin
        117 hewie-action? while
        yield
    repeat
    hewie-who $2B char-on-tri? hewie-who $140 char-on-tri? or hewie-who $1DF char-on-tri? or if
        0 hewie-who 4 action
    else
        0 solid-floor floor-on
        0 keeps-fiona-off keeps-pursuers-off or floor-off
        3 ebit-clear
        5 hewie-trust
    then
    self-idle-or-end
;

: castle-2f-14.act07 ( -- )   \ 00412E40
    self-wait-done
    90 self-turn-angle
    self-wait-done
    2 ebit? not if
        3 message   \ "The body of a beast and the head of a bird... What a strange-looking statue."
        wait-message
        2 ebit-set
    else
        4 message   \ "Some ancient castles used to line their walls with gargoyles, as it was believed to..."
        wait-message
        2 ebit-clear
    then
    self-idle-or-end
;

\ ---- registered ----
castle-2f-14 register-scripts   \ (its phases and actions, by their names)
