\ story/rooms/lake-pathway-2.fs - the event scripts of room lake-pathway-2 ($80; Lake Pathway).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.lake-pathway-2
USING: room-names story.words story.shared story-names floor-flags ;

\ (as Room2A_Cmd03) the 0x14-byte effect BackdropModel2_vtable started with byte 3 as a word
: lake-pathway-2.cmd00 ( b0 -- )   \ (the water's light: BackdropModel2 - b0 nonzero 500 across at (0, 50) turning, else 1500 at (775, -420))
    >r 0 $40404040 r> if  0e 27.1e 50e 500e 0.1e 0.05e  else  775e 27.1e -420e 1500e 0.1e 0e  then  effects:caustic drop ;

: lake-pathway-2.enter ( -- )   \ 0043EA90
    $18 1.0 0 bgm
    $10 22.8 95.4 53.7 $E $80 $80 $80 $40 specks
    $10 -23.1 95.4 53.7 $E $80 $80 $80 $40 specks
    over-music-d story-flag? not if
        0 part-shown
        0 lake-pathway-2.cmd00
    else
        0 barred-floor floor-on
        1 lake-pathway-2.cmd00
    then
;

: lake-pathway-2.char-enter ( -- )   \ 0043EAE0
    over-music-d story-flag? not if
        hewie-controlled? not if
            fiona-who self-is? 0 exit-taken? and if
                fiona-who 0 char-to-exit
                1 1 player-camera
            then
        else hewie-who self-is? 0 exit-taken? and if
            hewie-who 0 char-to-exit
            1 1 player-camera
        then then
        0 1 1 area-camera
    else
        hewie-controlled? not if
            fiona-who self-is? 0 exit-taken? and if
                fiona-who 0 char-to-exit
                2 2 player-camera
            then
        else hewie-who self-is? 0 exit-taken? and if
            hewie-who 0 char-to-exit
            2 2 player-camera
        then then
        0 2 2 area-camera
    then
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

: lake-pathway-2.phase1 ( -- )   \ 0043EBA0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 1 1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    over-music-d story-flag? if
        0 -2.26 38.0 -25.57 $1E 15 0 zone
        1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        1 ebit-set
                        $1E chance? if
                            $1F 0 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
;

: lake-pathway-2.phase2 ( -- )   \ 0043EC20
    over-music-d story-flag? if
        fiona-who 6 char-in-area? fiona-who 90 $32 char-heading? and if
            0 unhurried offer-action
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
        0 message   \ "What!? The bridge is gone! It looks like it has disappeared!"
        wait-message
        self-wait-anim
        0 ebit-set
    else
        $1D02 self-anim
        self-frames-reset
        self-wait-16
        1 message   \ "There's no going back now. I'll need to find a new way out now."
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
lake-pathway-2 register-scripts   \ (its phases and actions, by their names)
