\ story/rooms/chaos-forest-1.fs - the event scripts of room chaos-forest-1 ($67; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-1
USING: room-names story.words story.shared flag-names story-names ;

: chaos-forest-1.enter ( -- )   \ 0041DD20
    room-sounds
    0 $F2 3 action
    $1C $2F char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 22.0 -16.0 145.0 0 char-to-xyz
        $1C $9008 1 0 char-anim-hold
    then
    0 $F1 1 action
    hunted state-flag-set
    got-forest-map story-flag? not if
        0 1 $14 door-bits
    then
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: chaos-forest-1.char-enter ( -- )   \ 0041DD90
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        3 3 player-camera
    then then
    0 3 3 area-camera
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
;

: chaos-forest-1.phase1 ( -- )   \ 0041DE10
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    found-file-247 story-flag? not if
        fiona-who 1 char-in-area? if
            0 fiona-who 0 action
        then
    else hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
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
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 2 char-entered-area? if
        1 exit-prepare
    then
    0 22.0 -16.0 145.0 5 10 1 zone
    self-who 0 char-in-zone? if
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
    1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    1 ebit-set
                    $50 chance? if
                        $1F 1 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    1 22.39 -16.0 144.46 $15 15 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
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
    got-forest-map story-flag? not if
        fiona-who $F char-in-area? fiona-who 90 $32 char-heading? and if
            2 calm offer-action
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
    $F 4 figure-load
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
        3 char-start
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
                stalker-who 0 stalker-command
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            fiona-who $8F 5 char-sound
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
    got-forest-map story-flag-set
    0 0 $14 door-bits
    $1F message-param-room
    $1F 1 item-give-count
    0 $1F item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
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
chaos-forest-1 register-scripts   \ (its phases and actions, by their names)
