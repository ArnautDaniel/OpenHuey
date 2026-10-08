\ story/rooms/castle-2f-15.fs - the event scripts of room castle-2f-15 ($30; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-15
USING: room-names story.words story.shared flag-names ;

\ room 0x30 (Room30_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind
: castle-2f-15.cond00? ( b0 -- flag )  drop s" castle-2f-15.cond00?" stub-flag ;

: castle-2f-15.enter ( -- )   \ 00412EA0
    room-sounds
    $16 1.0 0 bgm
    $260 story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
    $2FE story-flag? $2FF story-flag? not and if
        0 -105.0 135.0 23.0 flicker-sprite
    then
    1 $13 $10000000 nav-tri-flags
    1 $124 $10000000 nav-tri-flags
    1 $6B $10000000 nav-tri-flags
    1 $69 $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
;

: castle-2f-15.char-enter ( -- )   \ 00412F00
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
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
    0 self-is? if
        1 exit-taken? if
            4 map-page
        then
    then
    1 self-is? if
        118 hewie-action? not if
            1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
                1 $13C -90 char-to-tri-facing
            then
        then
    then
;

: castle-2f-15.phase1 ( -- )   \ 00412FB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        0 exit-prepare
    then
    0 3 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
            $FE 1 char-file-load
        then
        1 exit-prepare
    then
    0 2 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        4 map-page
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and 0 hewie-side? and if
        1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
            44 fiona-started? if
                0 1 3 action
            else $FE 2 char-C4? not $FE 8 char-in-area? and if
                stalker-free? if
                    2 ebit? not if
                        0 0 8 nav-group
                        1 0 $30 nav-group
                        0 castle-2f-15.cond00? if
                            2 ebit-set
                            0 $F1 4 action
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
                            0 1 1 action
                        then
                    then
                then
            then
        then then
    then
    $2FE story-flag? not if
        1 -105.0 134.0 23.0 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 766 var-set
                $1A 767 var-set
                $1B 0 var-set
                $1C -105000 var-set
                $1D 135000 var-set
                $1E 23000 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    $260 story-flag? not 1 7 char-in-area? and if
        0 -149.92 60.0 -50.71 $36 36 0 zone
        0 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        0 ebit-set
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

: castle-2f-15.phase2 ( -- )   \ 00413160
    $E story-flag? if
        0 9 char-in-area? if
            5 0 0 scene-change
        then
    then
    0 $A char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    $2FE story-flag? $2FF story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: castle-2f-15.phase5 ( -- )   \ 004131A0
    1 char-here? if
        118 hewie-action? if
            1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
                1 $140 -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else 1 0 char-in-nav-group? 1 $2B char-on-tri? or 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
            1 $13C char-to-tri
        then then
    then
;

: castle-2f-15.act00 ( -- )   \ 004131F0
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
            $30 0 288 hewie-to-room
        then
        $82 exit-check
    else
        0 exit-prepare
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: castle-2f-15.act01 ( -- )   \ 00413240
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

: castle-2f-15.act02 ( -- )   \ 00413330
    self-wait-done
    -105.0 23.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $98 message-param-room
        $98 $63 item-count? if
            $8010 message
            wait-message
        else
            $2FF story-flag-set
            0 effect-remove
            $98 1 item-give-count
            0 $98 item-tab
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

: castle-2f-15.act03 ( -- )   \ 00413390
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

: castle-2f-15.act04 ( -- )   \ 004133E0
    $75 0 hewie-action
    begin
        117 hewie-action? while
        yield
    repeat
    1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
        0 1 6 action
    else
        1 0 8 nav-group
        0 0 $30 nav-group
        2 ebit-clear
        5 hewie-trust
    then
    self-idle-or-end
;

: castle-2f-15.act05 ( -- )   \ 00413420
    self-wait-done
    90 self-turn-angle
    self-wait-done
    1 ebit? not if
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: castle-2f-15.act06 ( -- )   \ 00413440
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    2 ebit-clear
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-15.enter castle-2f-15 0 room-script!
' castle-2f-15.char-enter castle-2f-15 6 room-script!
' castle-2f-15.phase1 castle-2f-15 1 room-script!
' castle-2f-15.phase2 castle-2f-15 2 room-script!
' castle-2f-15.phase5 castle-2f-15 5 room-script!
' castle-2f-15.act00 castle-2f-15 $00 action-script!
' castle-2f-15.act01 castle-2f-15 $01 action-script!
' castle-2f-15.act02 castle-2f-15 $02 action-script!
' castle-2f-15.act03 castle-2f-15 $03 action-script!
' castle-2f-15.act04 castle-2f-15 $04 action-script!
' castle-2f-15.act05 castle-2f-15 $05 action-script!
' castle-2f-15.act06 castle-2f-15 $06 action-script!
