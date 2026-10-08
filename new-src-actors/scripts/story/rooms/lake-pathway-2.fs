\ story/rooms/lake-pathway-2.fs - the event scripts of room lake-pathway-2 ($80; Lake Pathway).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.lake-pathway-2
USING: room-names story.words story.shared ;

\ (as Room2A_Cmd03) the 0x14-byte effect BackdropModel2_vtable started with byte 3 as a word
: lake-pathway-2.cmd00 ( b0 -- )  drop s" lake-pathway-2.cmd00" stub-step ;

: lake-pathway-2.enter ( -- )   \ 0043EA90
    $18 1.0 0 bgm
    $10 22.8 95.4 53.7 $E $80 $80 $80 $40 specks
    $10 -23.1 95.4 53.7 $E $80 $80 $80 $40 specks
    $94 story-flag? not if
        0 1 $14 door-bits
        0 lake-pathway-2.cmd00
    else
        1 0 $20000 nav-group
        1 lake-pathway-2.cmd00
    then
;

: lake-pathway-2.char-enter ( -- )   \ 0043EAE0
    $94 story-flag? not if
        hewie-controlled? not if
            0 self-is? 0 exit-taken? and if
                0 0 char-to-exit
                hewie-controlled? not if
                    0 1 1 char-camera
                    0 camera-follow
                else
                    1 1 1 char-camera
                    1 camera-follow
                then
            then
        else 1 self-is? 0 exit-taken? and if
            1 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then then
        0 1 1 area-camera
    else
        hewie-controlled? not if
            0 self-is? 0 exit-taken? and if
                0 0 char-to-exit
                hewie-controlled? not if
                    0 2 2 char-camera
                    0 camera-follow
                else
                    1 2 2 char-camera
                    1 camera-follow
                then
            then
        else 1 self-is? 0 exit-taken? and if
            1 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then then
        0 2 2 area-camera
    then
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

: lake-pathway-2.phase1 ( -- )   \ 0043EBA0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 1 1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $94 story-flag? if
        0 -2.26 38.0 -25.57 $1E 15 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        1 ebit-set
                        $1E chance? if
                            $1F 0 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: lake-pathway-2.phase2 ( -- )   \ 0043EC20
    $94 story-flag? if
        0 6 char-in-area? 0 90 $32 char-heading? and if
            5 0 0 scene-change
        then
    then
;

: lake-pathway-2.act00 ( -- )   \ 0043EC40
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 ebit? not if
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        0 message
        wait-message
        self-wait-anim
        0 ebit-set
    else
        $1D02 self-anim
        self-frames-reset
        self-wait-16
        1 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' lake-pathway-2.enter lake-pathway-2 0 room-script!
' lake-pathway-2.char-enter lake-pathway-2 6 room-script!
' lake-pathway-2.phase1 lake-pathway-2 1 room-script!
' lake-pathway-2.phase2 lake-pathway-2 2 room-script!
' lake-pathway-2.act00 lake-pathway-2 $00 action-script!
