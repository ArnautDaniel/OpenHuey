\ story/rooms/castle-b1-5.fs - the event scripts of room castle-b1-5 ($B; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-5
USING: room-names story.words story.shared ;

\ room 0x0B (Room0B_Cond00_ptmf): the player is 20 .. 120 from (x, z) = s16 bytes 3..4, 5..6
: castle-b1-5.cond00? ( b0 b1 b2 b3 -- flag )  drop drop drop drop s" castle-b1-5.cond00?" stub-flag ;

: castle-b1-5.enter ( -- )   \ 003F43C0
    room-sounds
    1 0 $1000000 nav-group
    1 0 $10000000 nav-group
    0 0 char-file-load
    1 $2300 sound-volume
;

: castle-b1-5.char-enter ( -- )   \ 003F43E0
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
    1 self-is? if
        $317 story-flag? not 1 0 char-heading-for? and if
            0 1 5 action
        then
    then
;

: castle-b1-5.phase1 ( -- )   \ 003F4470
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    1 char-here? if
        35 fiona-started? 1 2 char-C4? not and if
            0 -80 0 $2D char-faces-xz? $FF $B0 0 0 castle-b1-5.cond00? and if
                hewie-stays? if
                    0 1 1 action
                then
            then
            0 10 0 $2D char-faces-xz? 0 $A 0 0 castle-b1-5.cond00? and if
                hewie-stays? if
                    0 1 2 action
                then
            then
            0 110 0 $2D char-faces-xz? 0 $6E 0 0 castle-b1-5.cond00? and if
                hewie-stays? if
                    0 1 3 action
                then
            then
        then
    then
    0 0 char-in-nav-group? if
        0 0 0 action
    then
;

: castle-b1-5.act00 ( -- )   \ 003F4500
    $2C state-flag-set
    1 self-scripted
    begin
        0 char-busy? not while
        yield
    repeat
    0 char-file-use
    0 $F2 6 action
    $8000 self-anim
    self-wait-anim
    self-frames-reset
    $3C self-wait-frames
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    0 1 6 char-sound
    self-frames-reset
    self-wait-16
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: castle-b1-5.act01 ( -- )   \ 003F4540
    self-wait-done
    hewie-bark
    self-wait-done
    $A7 $FFFF 6 self-move-tri
    self-wait-done
    0 self-turn-to
    self-wait-done
    0 $F1 4 action
    self-idle-or-end
;

: castle-b1-5.act02 ( -- )   \ 003F4560
    self-wait-done
    hewie-bark
    self-wait-done
    $AB $FFFF 6 self-move-tri
    self-wait-done
    0 self-turn-to
    self-wait-done
    0 $F1 4 action
    self-idle-or-end
;

: castle-b1-5.act03 ( -- )   \ 003F4580
    self-wait-done
    hewie-bark
    self-wait-done
    $9B $FFFF 6 self-move-tri
    self-wait-done
    0 self-turn-to
    self-wait-done
    0 $F1 4 action
    self-idle-or-end
;

: castle-b1-5.act04 ( -- )   \ 003F45A0
    begin
        1 char-busy? while
        yield
    repeat
    $1D $29 hewie-action
    self-idle-or-end
;

: castle-b1-5.act05 ( -- )   \ 003F45C0
    self-wait-done
    $A4 -55.0 0.0 90 $FFFF $A self-move-to
    self-wait-done
    4 self-anim
    self-frames-reset
    $3C self-wait-frames
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    0 self-look-at
    yield
    $317 story-flag-set
    4 self-anim
    self-frames-reset
    $28 self-wait-frames
    $FF self-look-at
    yield
    self-idle-or-end
;

: castle-b1-5.act06 ( -- )   \ 003F4608
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    0 0 6 char-sound
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-5.enter castle-b1-5 0 room-script!
' castle-b1-5.char-enter castle-b1-5 6 room-script!
' castle-b1-5.phase1 castle-b1-5 1 room-script!
' castle-b1-5.act00 castle-b1-5 $00 action-script!
' castle-b1-5.act01 castle-b1-5 $01 action-script!
' castle-b1-5.act02 castle-b1-5 $02 action-script!
' castle-b1-5.act03 castle-b1-5 $03 action-script!
' castle-b1-5.act04 castle-b1-5 $04 action-script!
' castle-b1-5.act05 castle-b1-5 $05 action-script!
' castle-b1-5.act06 castle-b1-5 $06 action-script!
