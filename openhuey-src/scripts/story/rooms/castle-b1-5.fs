\ story/rooms/castle-b1-5.fs - the event scripts of room castle-b1-5 ($B; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-5
USING: room-names facts story.words story.shared flag-names story-names floor-flags hewie.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant wait-sound                  \ (snd 0) ?
1 constant caught-room-sound           \ (snd 1) caught (game over)

\ room 0x0B (Room0B_Cond00_ptmf): the player is 20 .. 120 from (x, z) = s16 bytes 3..4, 5..6
: castle-b1-5.cond00? ( b0 b1 b2 b3 -- flag )   \ (x, z: s16 bytes 3..4, 5..6)
    swap 8 lshift or s16>  >r  swap 8 lshift or s16>  r>
    s>f fiona-at 8 + sf@ f-  s>f fiona-at sf@ f-  fdup f* fswap fdup f* f+ fsqrt
    fdup 20e f< 0=  120e f<= and ;

: castle-b1-5.enter ( -- )   \ 003F43C0
    room-sounds
    0 keeps-hewie-off floor-on
    0 hole-floor floor-on
    fiona-who 0 char-file-load
    1 $2300 sound-volume
;

: castle-b1-5.char-enter ( -- )   \ 003F43E0
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
    hewie-who self-is? if
        cb1-5-flag1 story-flag? not hewie-who 0 char-heading-for? and if
            0 hewie-who 5 action
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
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    hewie-who char-here? if
        35 fiona-started? hewie-who 2 char-cond? not and if
            fiona-who -80 0 $2D char-faces-xz? $FF $B0 0 0 castle-b1-5.cond00? and if
                hewie-may-break-off? if
                    0 hewie-who 1 action
                then
            then
            fiona-who 10 0 $2D char-faces-xz? 0 $A 0 0 castle-b1-5.cond00? and if
                hewie-may-break-off? if
                    0 hewie-who 2 action
                then
            then
            fiona-who 110 0 $2D char-faces-xz? 0 $6E 0 0 castle-b1-5.cond00? and if
                hewie-may-break-off? if
                    0 hewie-who 3 action
                then
            then
        then
    then
    fiona-who 0 char-in-nav-group? if
        0 fiona-who 0 action
    then
;

: castle-b1-5.act00 ( -- )   \ 003F4500
    capture-no-end state-flag-set
    1 self-scripted
    [: fiona-who char-busy? ;] wait-until
    fiona-who char-file-use
    0 2 track 6 action
    0 room-motion play-out
    $3C wait-frames
    [: 6 sound-bank-loaded? ;] wait-until
    fiona-who caught-room-sound 6 char-sound
    16 wait-frames
    0 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: castle-b1-5.act01 ( -- )   \ 003F4540
    self-wait-done
    hewie-bark
    self-wait-done
    $A7 $FFFF 6 self-move-tri
    self-wait-done
    fiona-who self-turn-to
    self-wait-done
    0 1 track 4 action
    self-idle-or-end
;

: castle-b1-5.act02 ( -- )   \ 003F4560
    self-wait-done
    hewie-bark
    self-wait-done
    $AB $FFFF 6 self-move-tri
    self-wait-done
    fiona-who self-turn-to
    self-wait-done
    0 1 track 4 action
    self-idle-or-end
;

: castle-b1-5.act03 ( -- )   \ 003F4580
    self-wait-done
    hewie-bark
    self-wait-done
    $9B $FFFF 6 self-move-tri
    self-wait-done
    fiona-who self-turn-to
    self-wait-done
    0 1 track 4 action
    self-idle-or-end
;

: castle-b1-5.act04 ( -- )   \ 003F45A0
    [: hewie-who char-busy? ;] wait-while
    $1D $29 hewie-do
    self-idle-or-end
;

: castle-b1-5.act05 ( -- )   \ 003F45C0
    self-wait-done
    $A4 -55.0 0.0 90 $FFFF $A self-move-to
    self-wait-done
    stance-anim self-anim
    $3C wait-frames
    bark-tense-anim play-out
    bark-tense-anim play-out
    bark-tense-anim play-out
    bark-tense-anim play-out
    bark-tense-anim play-out
    bark-tense-anim play-out
    fiona-who self-look-at
    yield
    cb1-5-flag1 story-flag-set
    stance-anim self-anim
    $28 wait-frames
    self-who self-look-at
    yield
    self-idle-or-end
;

: castle-b1-5.act06 ( -- )   \ 003F4608
    [: 6 sound-bank-loaded? ;] wait-until
    fiona-who wait-sound 6 char-sound
    self-idle-or-end
;

\ ---- registered ----
castle-b1-5 register-scripts   \ (its phases and actions, by their names)
