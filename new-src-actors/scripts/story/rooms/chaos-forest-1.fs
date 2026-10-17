\ story/rooms/chaos-forest-1.fs - the event scripts of room chaos-forest-1 ($67; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ ---- this room's own event bits and script variables ----
0 constant in-zone0                    \ (bit 0) someone in zone 0
1 constant hewie-zone1-noticed         \ (bit 1) Hewie has reacted to zone 1 (once a visit)
2 constant figure-scene-on             \ (bit 2) the figure's scene begun (act00)
0 constant hops-var                    \ (var 0) the figure's hops (5: away)
1 constant still-var                   \ (var 1) the figure still (0) or hopping
2 constant call-frames-var             \ (var 2) frames to the figure's next call (60)

: chaos-forest-1.enter ( -- )   \ 0041DD20
    room-sounds
    0 2 track 3 action
    $1C $2F char-to-tri
    still-var 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 22.0 -16.0 145.0 0 char-to-xyz
        $1C $9008 1 0 char-anim-hold
    then
    0 1 track 1 action
    hunted state-flag-set
    got-forest-map story-flag? not if
        0 part-shown
    then
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: chaos-forest-1.char-enter ( -- )   \ 0041DD90
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        3 3 player-camera
    then
    0 3 3 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
;

: chaos-forest-1.phase1 ( -- )   \ 0041DE10
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    found-file-247 story-flag? not if
        fiona-who 1 char-in-area? if
            0 fiona-who 0 action
        then
    else player-who 1 char-in-area? player-who char-busy? not and if
            1 exit-check
        then then
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
        in-zone0 ebit-set
    else
        in-zone0 ebit-clear
    then
    still-var 0 var? not if
        call-frames-var var-inc
        call-frames-var 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            call-frames-var 0 var-set
        then
    then
    1 22.39 -16.0 144.46 $15 15 0 zone
    hewie-zone1-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    hewie-zone1-noticed ebit-set
                    $50 chance? if
                        hewie-zone-var 1 var-set
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
                hewie-zone-var 1 var-set
                hewie-zone-var 15.0 hewie-look-zone
            then
        then
    then
;

: chaos-forest-1.phase5 ( -- )   \ 0041DF60
    figure-scene-on ebit? not if
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
    1 track action-end
    figure-scene-on ebit-set
    [: loader-done? ;] wait-while
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
        in-zone0 ebit? if
            still-var 0 var-set
            hops-var var-inc
            hops-var 3 var? if
                $21 chance? if
                    hops-var 5 var-set
                then
            then
            hops-var 4 var? if
                $32 chance? if
                    hops-var 5 var-set
                then
            then
            hops-var 5 var? if
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
            fiona-who shove-hit-sound 5 char-sound
            $1C $9009 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9008 0 3 char-anim-hold
            still-var 0 var-set
            hops-var 5 var? if
                $1C wait-char-anim
                hops-var 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9008 0 0 char-anim-hold
            still-var 0 var-set
        else
            still-var var-inc
        then
        1 wait-frames
    again
;

: chaos-forest-1.act02 ( -- )   \ 0041E080
    self-wait-done
    -324.0 322.0 self-turn-to-xz
    self-wait-done
    reach-out-anim play-out
    4 wait-frames
    got-forest-map story-flag-set
    0 part-hidden
    $1F message-param-room
    $1F 1 item-give-count
    0 $1F item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: chaos-forest-1.act03 ( -- )   \ 0041E0D0
    [: 3 $1C char-loaded? ;] wait-until
    $19 1.0 0 bgm
    self-idle-or-end
;

\ ---- registered ----
chaos-forest-1 register-scripts   \ (its phases and actions, by their names)
