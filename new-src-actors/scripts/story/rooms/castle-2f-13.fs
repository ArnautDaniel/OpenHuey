\ story/rooms/castle-2f-13.fs - the event scripts of room castle-2f-13 ($28; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-13
USING: room-names story.words story.shared ;

\ the pursuer's Pursuer_GrabHewieBehind
: castle-2f-13.cond00? ( b0 -- flag )  drop s" castle-2f-13.cond00?" stub-flag ;

: castle-2f-13.enter ( -- )   \ 00403980
    $16 1.0 0 bgm
    $232 story-flag? not if
        0 -231.0 61.0 24.0 flicker-sprite
    then
    $260 story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
;

: castle-2f-13.char-enter ( -- )   \ 004039B0
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
    1 self-is? if
        118 hewie-action? not if
            1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
                1 $13C -90 char-to-tri-facing
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
    0 2 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        4 map-page
    then
    $E story-flag? not 1 ebit? not and if
        0 6 char-in-area? if
            0 var-inc
            0 30 var? if
                1 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 0 action-force
                else
                    1 0 0 action-force
                then
                0 0 char-action? if
                    6 ebit-set
                then
            then
        else
            0 0 var-set
        then
        $FE 6 char-in-area? if
            1 var-inc
            1 30 var? if
                1 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 1 action-force
                else
                    1 0 1 action-force
                then
            then
        else
            1 0 var-set
        then
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
            44 fiona-started? if
                0 1 7 action
            else $FE 2 char-C4? not $FE 8 char-in-area? and if
                stalker-free? if
                    5 ebit? not if
                        0 0 8 nav-group
                        1 0 $30 nav-group
                        0 castle-2f-13.cond00? if
                            5 ebit-set
                            0 $F1 8 action
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
                            0 1 5 action
                        then
                    then
                then
            then
        then then
    then
    2 -231.29 60.0 30.58 $32 20 0 zone
    3 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    3 ebit-set
                    $64 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    $260 story-flag? not 1 7 char-in-area? and if
        1 -149.92 60.0 -50.71 $36 36 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    1 -149.92 60.0 -50.71 $36 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
    3 -228.31 60.0 25.69 $23 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-13.phase2 ( -- )   \ 00403C10
    0 4 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 45 $32 char-heading? and if
        5 9 0 scene-change
    then
;

: castle-2f-13.phase5 ( -- )   \ 00403C30
    1 char-here? if
        118 hewie-action? if
            1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
                1 $1EA -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else 1 0 char-in-nav-group? 1 $1C3 char-on-tri? or 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
            1 $13C char-to-tri
        then then
    then
;

: castle-2f-13.act02 ( -- )   \ 00403D00
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $F $44 fade
    wait-fade
    8 state-flag-set
    $FE char-here? if
        0 $FE 3 action
    then
    $35 $36 door-copy
    $36 door-close-off-lock
    exits-rebuild
    exit
;

: castle-2f-13.act00 ( -- )   \ 00403C80
    $18 state-flag-set
    $E state-flag-set
    $13 state-flag-set
    1 self-scripted
    8 room-preload
    $E 3 $FF char-load
    $F 4 char-load-2
    self-wait-done
    6 ebit? if
        1 self-anim
    then
    castle-2f-13.act02
    1 char-here? if
        8 0 -1 hewie-to-room
    then
    $E state-flag-clear
    $13 state-flag-clear
    $80 exit-check
    self-idle-or-end
;

: castle-2f-13.act01 ( -- )   \ 00403CC0
    $18 state-flag-set
    $E state-flag-set
    $13 state-flag-set
    1 self-scripted
    $FE 0 0 char-camera
    $FE camera-follow
    8 room-preload
    $E 3 $FF char-load
    $F 4 char-load-2
    self-wait-done
    castle-2f-13.act02
    1 char-here? if
        $2F 0 -1 hewie-to-room
    then
    $E state-flag-clear
    $13 state-flag-clear
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
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-13.act05 ( -- )   \ 00403D50
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
            $A state-flag? 0 char-busy? not or if
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
        $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 0 8 nav-group
        1 0 $30 nav-group
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        1 0 8 nav-group
        0 0 $30 nav-group
    else
        1 counter-set
        $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        $102 self-anim
        self-wait-anim
        2 self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: castle-2f-13.act06 ( -- )   \ 00403E40
    self-wait-done
    $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    5 ebit-clear
    self-idle-or-end
;

: castle-2f-13.act07 ( -- )   \ 00403E90
    self-wait-done
    hewie-bark
    self-wait-done
    $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    self-idle-or-end
;

: castle-2f-13.act08 ( -- )   \ 00403EE0
    $75 0 hewie-action
    begin
        117 hewie-action? while
        yield
    repeat
    1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
        0 1 6 action
    else
        1 0 8 nav-group
        0 0 $30 nav-group
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
        3 message
        wait-message
        4 ebit-set
    else
        4 message
        wait-message
        4 ebit-clear
    then
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-13.enter castle-2f-13 0 room-script!
' castle-2f-13.char-enter castle-2f-13 6 room-script!
' castle-2f-13.phase1 castle-2f-13 1 room-script!
' castle-2f-13.phase2 castle-2f-13 2 room-script!
' castle-2f-13.phase5 castle-2f-13 5 room-script!
' castle-2f-13.act00 castle-2f-13 $00 action-script!
' castle-2f-13.act01 castle-2f-13 $01 action-script!
' castle-2f-13.act02 castle-2f-13 $02 action-script!
' castle-2f-13.act03 castle-2f-13 $03 action-script!
' castle-2f-13.act04 castle-2f-13 $04 action-script!
' castle-2f-13.act05 castle-2f-13 $05 action-script!
' castle-2f-13.act06 castle-2f-13 $06 action-script!
' castle-2f-13.act07 castle-2f-13 $07 action-script!
' castle-2f-13.act08 castle-2f-13 $08 action-script!
' castle-2f-13.act09 castle-2f-13 $09 action-script!
