\ story/rooms/castle-2f-14.fs - the event scripts of room castle-2f-14 ($2F; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-14
USING: room-names story.words story.shared flag-names ;

\ room 0x2F (Room2F_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind
: castle-2f-14.cond00? ( b0 -- flag )  drop s" castle-2f-14.cond00?" stub-flag ;

: castle-2f-14.enter ( -- )   \ 00412950
    room-sounds
    $16 1.0 0 bgm
    $260 story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
    1 $13 $10000000 nav-tri-flags
    1 $124 $10000000 nav-tri-flags
    1 $6B $10000000 nav-tri-flags
    1 $69 $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
;

: castle-2f-14.char-enter ( -- )   \ 004129A0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    0 self-is? if
        $E story-flag? not if
            0 $4D -45 char-to-tri-facing
            1 char-activate
            1 char-here? if
                1 $120 -45 char-to-tri-facing
            then
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            0 0 0 action
        then
    then
    1 self-is? if
        118 hewie-action? not if
            1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
                1 $13C -90 char-to-tri-facing
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
    0 2 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        4 map-page
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
            44 fiona-started? if
                0 1 5 action
            else $FE 2 char-C4? not $FE 8 char-in-area? and if
                stalker-free? if
                    3 ebit? not if
                        0 0 8 nav-group
                        1 0 $30 nav-group
                        0 castle-2f-14.cond00? if
                            3 ebit-set
                            0 $F1 6 action
                        else
                            1 0 8 nav-group
                            0 0 $30 nav-group
                        then
                    then
                then
            then then
        else 2 game-mode? not if
            0 control-action? if
                -163 97 -50 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                    0 -163 -50 $32 char-faces-xz? if
                        hewie-stays? if
                            0 1 3 action
                        then
                    then
                then
            then
        then then
    then
    $260 story-flag? not 1 7 char-in-area? and if
        0 -149.92 60.0 -50.71 $36 36 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    0 -149.92 60.0 -50.71 $36 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-14.phase2 ( -- )   \ 00412B60
    $E story-flag? if
        0 9 char-in-area? if
            5 1 0 scene-change
        then
    then
    0 4 char-in-area? 0 90 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 $A char-in-area? 0 45 $32 char-heading? and if
        5 7 0 scene-change
    then
;

: castle-2f-14.phase5 ( -- )   \ 00412B90
    1 char-here? if
        118 hewie-action? if
            1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
                1 $140 -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else 1 0 char-in-nav-group? 1 $2B char-on-tri? or 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
            1 $13C char-to-tri
        then then
    then
;

: castle-2f-14.act00 ( -- )   \ 00412BE0
    yield
    camera-restart
    world-held state-flag-clear
    $F $41 fade
    $E story-flag-set
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
    2 message
    wait-message
    0 answer? if
        $F 4 fade
        wait-fade
        world-held state-flag-set
        scene-locked state-flag-set
        0 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $B4 self-wait-frames
        1 char-here? if
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
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-14.act03 ( -- )   \ 00412C70
    self-wait-done
    1 0 char-file-load
    hewie-bark
    self-wait-done
    $AE -152.54 -106.55 0 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    1 self-noclip
    $8000 5 self-anim-9
    self-wait-anim
    0 0 $30 nav-group
    0 self-noclip
    $260 story-flag? not if
        $260 story-flag? not if
            fiona-half-hidden state-flag? 0 char-busy? not or if
                $70 $63 item-count? not if
                    10 hewie-trust
                then
                $70 message-param-room
                $70 $63 item-count? if
                    $8010 message
                    wait-message
                else
                    $260 story-flag-set
                    $70 1 item-give-count
                    0 $70 item-tab
                    0 $F9 $8C action-force
                    $83 $85 0.0 0.0 0.0 0 0 sound
                    $8011 message
                    wait-message
                then
                $260 story-flag? if
                    1 effect-remove
                then
            then
        then
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 0 8 nav-group
        1 0 $30 nav-group
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        1 0 8 nav-group
        0 0 $30 nav-group
    else
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        $102 self-anim
        self-wait-anim
        2 self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: castle-2f-14.act04 ( -- )   \ 00412D60
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    3 ebit-clear
    self-idle-or-end
;

: castle-2f-14.act05 ( -- )   \ 00412DB0
    self-wait-done
    hewie-bark
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    self-idle-or-end
;

: castle-2f-14.act06 ( -- )   \ 00412E00
    $75 0 hewie-action
    begin
        117 hewie-action? while
        yield
    repeat
    1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
        0 1 4 action
    else
        1 0 8 nav-group
        0 0 $30 nav-group
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
        3 message
        wait-message
        2 ebit-set
    else
        4 message
        wait-message
        2 ebit-clear
    then
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-14.enter castle-2f-14 0 room-script!
' castle-2f-14.char-enter castle-2f-14 6 room-script!
' castle-2f-14.phase1 castle-2f-14 1 room-script!
' castle-2f-14.phase2 castle-2f-14 2 room-script!
' castle-2f-14.phase5 castle-2f-14 5 room-script!
' castle-2f-14.act00 castle-2f-14 $00 action-script!
' castle-2f-14.act01 castle-2f-14 $01 action-script!
' castle-2f-14.act02 castle-2f-14 $02 action-script!
' castle-2f-14.act03 castle-2f-14 $03 action-script!
' castle-2f-14.act04 castle-2f-14 $04 action-script!
' castle-2f-14.act05 castle-2f-14 $05 action-script!
' castle-2f-14.act06 castle-2f-14 $06 action-script!
' castle-2f-14.act07 castle-2f-14 $07 action-script!
