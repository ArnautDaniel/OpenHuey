\ story/rooms/off-map-d6.fs - the event scripts of room off-map-d6 ($D6; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d6
USING: room-names story.words story.shared flag-names story.clock floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits and script variables ----
0 constant found-her                   \ (bit 0) he has found her hiding: she must flee (the avoid prompt)
1 constant will-find-her               \ (bit 1) he will find her hiding (by chance, as he searches: more likely by the progress variable)
2 constant came-while-hidden           \ (bit 2) the stalker came in while she was hiding, heading for the exit
4 constant cond-was-off                \ (bit 4) the room's condition was false last frame: its hook waits a frame once it holds

\ (as Room105_Cmd00) pushed by the character slot byte 4 names
: off-map-d6.cmd00 ( b0 b1 -- )  swing-three-by ;   \ (as chaos-forest-7's, pushed by character slot b1)
\ room 0xD6: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d6.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d6.cond00? ( -- flag )  true ;

: off-map-d6.act02 ( -- )   \ 00447990
    will-find-her ebit-clear
    $2F 0 pvar? if
        $A chance? if
            will-find-her ebit-set
        then
    else $2F 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $2F 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $2F 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
    then
    $2F pvar-inc
    exit
;

: off-map-d6.char-enter ( -- )   \ 00447680
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? stalker-who 1 char-heading-for? and if
            came-while-hidden ebit-set
            off-map-d6.act02
        else
            came-while-hidden ebit-clear
        then
    then
;

: off-map-d6.phase1 ( -- )   \ 00447790
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    hewie-who 4 char-entered-area? if
        0 exit-prepare
    then
    hewie-who 6 char-entered-area? if
        2 exit-prepare
    then
    hewie-who 7 char-entered-area? if
        3 exit-prepare
    then
    1 70.5 5.0 -5.5 $14 10 0 zone
    hewie-who 1 8 char-zone-bits? if
        fiona-who char-here? fiona-who 1 char-heading-for? and if
            fiona-who char-busy? not in-panic? not and fiona-free? and if
                -1 control-action? not if
                    stalker-who char-here? not if
                        0 fiona-who 0 action
                    else stalker-who 1 char-heading-for? not if
                        0 fiona-who 0 action
                    then then
                then
            then
        then
    then
    0 70.5 5.0 -5.5 5 10 0 zone
    fiona-who 0 8 char-zone-bits? if
        1 0 off-map-d6.cmd00
    else hewie-who 0 8 char-zone-bits? if
        1 1 off-map-d6.cmd00
    else stalker-who 0 8 char-zone-bits? if
        1 2 off-map-d6.cmd00
    then then then
    2 0 off-map-d6.cmd00
;

: off-map-d6.phase3 ( -- )   \ 00447890
    off-map-d6.cond00? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-d6.cmd01
    then then
;

: off-map-d6.phase5 ( -- )   \ 004478B0
    fiona-who char-busy? if
        fiona-hidden state-flag-clear
        stalkers-stay state-flag-clear
        fiona-who action-end
        fiona-who $E7 90.99 -3.17 90 char-to-xz
    then
;

: off-map-d6.act01 ( -- )   \ 00447960
    found-her ebit? if
        2 avoid-prompt
    then
    fiona-hidden state-flag-clear
    stalkers-stay state-flag-set
    stalker-who self-look-at
    yield
    4 $14 blend-out
    self-who self-look-at
    yield
    0 self-scripted
    0 self-noclip
    stalkers-stay state-flag-clear
    0 floor-flag-40000 floor-off
    self-idle-or-end
;

: off-map-d6.act00 ( -- )   \ 004478D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $19A 70.5 -5.5 90 $FFFF $A self-move-to
    self-wait-done
    0 floor-flag-40000 floor-on
    came-while-hidden ebit-clear
    1 self-noclip
    found-her ebit-clear
    crouch-down-anim play-out
    crouching-anim self-anim
    fiona-hidden state-flag-set
    stalkers-stay state-flag-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] off-map-d6.act01 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                came-while-hidden ebit? stalker-who char-here? not and if
                    came-while-hidden ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            fiona-hidden state-flag-clear
            stalker-who action-end
            2 game-mode? if
                ['] off-map-d6.act01 goto
            then
            stalkers-stay state-flag-set
            crouch-up-anim play-out
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            0 floor-flag-40000 floor-off
            self-idle-or-end
        then
    again
;

: off-map-d6.act03 ( -- )   \ 004479E0
    self-wait-done
    $E7 90.5 -4.5 -90 $FFFF 5 self-move-to
    self-wait-done
    there-anim play-out
    found-her ebit-set
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: off-map-d6.enter ( -- )   \ 0047B070
    room-sounds
    0 0 off-map-d6.cmd00
;

: off-map-d6.phase2 ( -- )   \ 0047B078
;

\ ---- registered ----
off-map-d6 register-scripts   \ (its phases and actions, by their names)
