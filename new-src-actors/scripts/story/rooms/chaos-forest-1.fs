\ story/rooms/chaos-forest-1.fs - the event scripts of room chaos-forest-1 ($67; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-1
USING: room-names story.words story.shared flag-names ;

: chaos-forest-1.enter ( -- )   \ 0041DD20
    room-sounds
    0 $F2 3 action
    $1C $2F char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 22.0 -16.0 145.0 0 char-to-xyz
        $1C $9008 1 0 char-anim-hold
    then
    0 $F1 1 action
    hunted state-flag-set
    $336 story-flag? not if
        0 1 $14 door-bits
    then
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: chaos-forest-1.char-enter ( -- )   \ 0041DD90
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    0 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
;

: chaos-forest-1.phase1 ( -- )   \ 0041DE10
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    $86 story-flag? not if
        0 1 char-in-area? if
            0 0 0 action
        then
    else hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then then
    2 0 0 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 1 1 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 4 4 1 chars-area-camera
    7 5 5 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 5 -1 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 5 5 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 5 -1 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 2 char-entered-area? if
        1 exit-prepare
    then
    0 22.0 -16.0 145.0 5 10 1 zone
    $FF 0 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 0 var? not if
        2 var-inc
        2 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            2 0 var-set
        then
    then
    1 22.39 -16.0 144.46 $15 15 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $50 chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 22.39 -16.0 144.46 $15 15 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
;

: chaos-forest-1.phase5 ( -- )   \ 0041DF60
    2 ebit? not if
        $1C action-end
        $1C char-done
    then
;

: chaos-forest-1.phase2 ( -- )   \ 0041DF70
    $336 story-flag? not if
        0 $F char-in-area? 0 90 $32 char-heading? and if
            5 2 4 scene-change
        then
    then
;

: chaos-forest-1.act00 ( -- )   \ 0041DF90
    stalkers-stay state-flag-set
    1 self-scripted
    3 0 char-remove
    $F1 action-end
    2 ebit-set
    begin
        loader-done? while
        yield
    repeat
    $E 3 $FF char-load
    $F 4 char-load-2
    $F $44 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    1 exit-check
    self-idle-or-end
;

: chaos-forest-1.act01 ( -- )   \ 0041DFC0
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 22.0 -16.0 145.0 0 char-to-xyz
        $1C $9008 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            1 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9009 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9008 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9008 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: chaos-forest-1.act02 ( -- )   \ 0041E080
    self-wait-done
    -324.0 322.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $336 story-flag-set
    0 0 $14 door-bits
    $1F message-param-room
    $1F 1 item-give-count
    0 $1F item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: chaos-forest-1.act03 ( -- )   \ 0041E0D0
    begin
        3 $1C char-loaded? not while
        yield
    repeat
    $19 1.0 0 bgm
    self-idle-or-end
;

\ ---- registered ----
' chaos-forest-1.enter chaos-forest-1 0 room-script!
' chaos-forest-1.char-enter chaos-forest-1 6 room-script!
' chaos-forest-1.phase1 chaos-forest-1 1 room-script!
' chaos-forest-1.phase5 chaos-forest-1 5 room-script!
' chaos-forest-1.phase2 chaos-forest-1 2 room-script!
' chaos-forest-1.act00 chaos-forest-1 $00 action-script!
' chaos-forest-1.act01 chaos-forest-1 $01 action-script!
' chaos-forest-1.act02 chaos-forest-1 $02 action-script!
' chaos-forest-1.act03 chaos-forest-1 $03 action-script!
