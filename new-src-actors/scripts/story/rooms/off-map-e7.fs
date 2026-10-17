\ story/rooms/off-map-e7.fs - the event scripts of room off-map-e7 ($E7; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e7
USING: room-names story.words story.shared flag-names story.clock floor-flags fiona.sounds stalker.anims ;

\ room 0xE7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e7.cmd00 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e7.cond00? ( -- flag )  true ;

: off-map-e7.enter ( -- )   \ 00449AD0
    0 34.2 18.3 14.2 1 flame-in
    1 34.2 18.3 -14.2 1 flame-in
    2 -34.2 18.3 -14.2 1 flame-in
    3 -34.2 18.3 14.2 1 flame-in
    0 part-shown
    1 $2300 sound-volume
;

: off-map-e7.act02 ( -- )   \ 00449E00
    1 ebit-clear
    $33 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $33 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $33 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $33 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
    then
    $33 pvar-inc
    exit
;

: off-map-e7.char-enter ( -- )   \ 00449B20
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 0 player-camera
    then then
    1 1 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            2 ebit-set
            off-map-e7.act02
        else
            2 ebit-clear
        then
    then
;

: off-map-e7.phase1 ( -- )   \ 00449BB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 0 1 chars-area-camera
    5 0 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 0 -1 chars-area-camera
    hewie-who 2 char-entered-area? if
        1 exit-prepare
    then
    hewie-who 3 char-entered-area? if
        0 exit-prepare
    then
    4 -0.56 0.0 -21.78 $A 5 0 zone
    hewie-who 4 8 char-zone-bits? if
        fiona-who char-here? if
            fiona-who char-busy? not in-panic? not and fiona-free? and if
                -1 control-action? not if
                    stalker-who char-here? not if
                        0 fiona-who 0 action
                    then
                then
            then
        then
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
;

: off-map-e7.phase3 ( -- )   \ 00449CE0
    off-map-e7.cond00? not if
        3 ebit-set
    else 3 ebit? if
        3 ebit-clear
    else
        off-map-e7.cmd00
    then then
;

: off-map-e7.phase5 ( -- )   \ 00449D00
    fiona-who char-busy? if
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-clear
        fiona-who action-end
        fiona-who $141 -0.1 -18.54 0 char-to-xz
    then
;

: off-map-e7.act01 ( -- )   \ 00449DD0
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e7.act00 ( -- )   \ 00449D20
    stalkers-stay state-flag-set
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    $141 -0.12 -19.4 180 $FFFF $A self-move-to
    self-wait-done
    2 ebit-clear
    fiona-who char-file-use
    $141 $8004 5 -0.803 -15.664 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who slide-sound 5 char-sound
    5 room-motion $A self-anim-blend8
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            in-panic? 0 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] off-map-e7.act01 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? stalker-who char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] off-map-e7.act01 goto
            then
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: off-map-e7.act03 ( -- )   \ 00449E50
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $B2 0.866 0.417 180 $FFFF 5 self-move-to
    self-wait-done
    there-anim self-anim
    self-wait-anim
    0 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: off-map-e7.phase2 ( -- )   \ 0047B118
;

\ ---- registered ----
off-map-e7 register-scripts   \ (its phases and actions, by their names)
