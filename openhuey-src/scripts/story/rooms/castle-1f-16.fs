\ story/rooms/castle-1f-16.fs - the event scripts of room castle-1f-16 ($1A; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-16
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant found-her                   \ (bit 0) he has found her hiding: she must flee (the avoid prompt)
1 constant will-find-her               \ (bit 1) he will find her hiding (by chance, as he searches: more likely by the progress variable)
2 constant sound-started               \ (bit 2) its sound started (then kept up)
3 constant red-painting-remarked       \ (bit 3) its first remark made: "A landscape painting composed entirely of the color red. It "
4 constant came-while-hidden           \ (bit 4) the stalker came in while she was hiding
0 constant sound-step-var              \ (var 0) which of its sounds next (0..3)
1 constant sound-frames-var            \ (var 1) frames to the next sound (30)
0 constant switch-on-sound             \ (snd 0) her switching it on (it makes a noise the room hears)
1 constant running-sound               \ (snd 1) it running
2 constant switch-off-sound            \ (snd 2) her switching it off
5 constant ambient1-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the first
6 constant ambient2-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the second
7 constant ambient3-sound              \ (snd 7) its ambient sounds in turn (every 30 frames): the third
8 constant ambient4-sound              \ (snd 8) its ambient sounds in turn (every 30 frames): the fourth
3 constant first-on-sound              \ (snd 3) switching it on the first time (heard plainly)

\ room 0x1A (D_003FC660): the fan turns
: castle-1f-16.cmd00 ( -- )  fan-turn ;
\ room 0x1A: room effect slot 0 made anew as the second TV screen (TvScreenB).
: castle-1f-16.cmd01 ( -- )  0 1 tv-in ;   \ (room effect 0 the TV showing the screen)

: castle-1f-16.enter ( -- )   \ 003FC060
    room-sounds
    0 1 track 0 action
    castle-1f-16.cmd01
    c1f-16-flag2 story-flag? not if
        0 part-shown
        0 0 1 effect-string
    else
        0 part-hidden
        0 1 1 effect-string
        1 noise-level
    then
    c1f-16-magnesia story-flag? not if
        1 -25.0 10.0 13.0 flicker-sprite
    then
;

: castle-1f-16.act07 ( -- )   \ 003FC590
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who 1 char-file-load
    then
    will-find-her ebit-clear
    8 0 pvar? if
        0 chance? if
            will-find-her ebit-set
        then
    else 8 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else 8 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else 8 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    4 stalker-alert? if
        will-find-her ebit-clear
    then
    will-find-her ebit? if
        0 stalker-who 5 action
    else
        $78 1 summoner-wait
    then
    8 pvar-inc
    exit
;

: castle-1f-16.char-enter ( -- )   \ 003FC0A0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 -1 player-camera
    then
    0 1 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            c1f-16-flag2 story-flag? 2 creature-action? not and if
                2 self-is? 6 self-is? or 7 self-is? or if
                    $78 1 summoner-wait
                    0 stalker-who 6 action
                else
                    castle-1f-16.act07
                then
            else
                castle-1f-16.act07
            then
        else
            came-while-hidden ebit-clear
        then
    then
;

: castle-1f-16.phase1 ( -- )   \ 003FC110
    sound-started ebit? not if
        6 sound-bank-loaded? if
            c1f-16-flag2 story-flag? if
                running-sound volume-scaled 6 23.0 11.0 9.0 0 0 sound
            then
            sound-started ebit-set
        then
    else
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 27.0 15.0 -6.0 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 27.0 15.0 -6.0 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 27.0 15.0 -6.0 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 27.0 15.0 -6.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
        c1f-16-flag1 story-flag? if
            running-sound kept-going volume-scaled 6 23.0 11.0 9.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    5 2 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    c1f-16-flag2 story-flag? not if
        0 part-shown
        0 0 1 effect-string
    else
        0 part-hidden
        0 1 1 effect-string
    then
;

: castle-1f-16.phase2 ( -- )   \ 003FC220
    fiona-who 8 $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    fiona-who 9 char-in-area? fiona-who 60 $5A char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 45 $3C char-heading? and if
        stalker-who char-here? not if
            3 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    fiona-who $B char-in-area? fiona-who -72 $32 char-heading? and if
        8 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 0 $32 char-heading? and if
        9 unhurried offer-action
    then
    c1f-16-magnesia story-flag? not if
        0 1 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            1 calm offer-action
        then
    then
;

: castle-1f-16.phase3 ( -- )   \ 003FC290
    camera-setup-changed? if
        1 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else 2 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then then
    then
;

: castle-1f-16.act00 ( -- )   \ 0047AA68
    begin
        castle-1f-16.cmd00
        yield
    again
;

: castle-1f-16.act01 ( -- )   \ 003FC2D0
    self-wait-done
    -25.0 13.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-16-magnesia story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: castle-1f-16.act02 ( -- )   \ 003FC330
    self-wait-done
    $E1 18.5 13.6 120 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    reach-out-anim play-out
    9 0 pvar? 9 1 pvar? or if
        c1f-16-flag2 story-flag? not if
            c1f-16-flag2 story-flag-set
            fiona-who switch-on-sound 6 char-sound
            c1f-16-flag1 story-flag? not if
                first-on-sound $86 0.0 0.0 0.0 0 0 sound
            else
                running-sound volume-scaled 6 23.0 11.0 9.0 0 0 sound
            then
            $40 7 noise
            1 noise-level
        else
            fiona-who switch-off-sound 6 char-sound
            running-sound 6 sound-stop
            c1f-16-flag2 story-flag-clear
            0 noise-level
        then
        c1f-16-flag1 story-flag? not if
            1 self-scripted
            $5A threat-add
            1 $FF 8 rumble
            fiona-who seized-sound 5 char-sound
            uneasy-2-anim play-out
            -1 self-idle-anim
            7 wait-frames
            reach-out-anim play-out
            running-sound volume-scaled 6 23.0 11.0 9.0 0 0 sound
            reach-back-anim play-out
            c1f-16-flag1 story-flag-set
            0 self-scripted
        else
            reach-back-anim play-out
        then
    else
        reach-back-anim play-out
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-16.act04 ( -- )   \ 003FC4C0
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A blend-out
    0 self-noclip
    found-her ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-16.act03 ( -- )   \ 003FC420
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $CC $8004 5 -4.7 -39.3 90 self-walk-anim
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
    found-her ebit-clear
    0 avoid-prompt
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] castle-1f-16.act04 goto
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
            stalker-who char-here? if
                ['] castle-1f-16.act04 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            5 wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-1f-16.act05 ( -- )   \ 003FC4F0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $CE -12.0 -37.0 90 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who char-file-use
        0 room-motion $A self-anim-blend
        $1E wait-frames
        found-her ebit-set
        self-wait-anim
    else
        there-anim play-out
        found-her ebit-set
    then
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-1f-16.act06 ( -- )   \ 003FC540
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    7 14.0 18.0 120 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    $12 wait-frames
    stalker-who switch-off-sound 6 char-sound
    running-sound 6 sound-stop
    c1f-16-flag2 story-flag-clear
    0 noise-level
    9 pvar-inc
    stalker-who 3 stalker-command
    stalker.anims:stand-anim play-out
    16 wait-frames
    self-idle-or-end
;

: castle-1f-16.act08 ( -- )   \ 003FC5F0
    self-wait-done
    -145 self-turn-angle
    self-wait-done
    0 say   \ "It would be nice to freshen up a bit... But there's a crack in this mirror and I can't..."
    self-idle-or-end
;

: castle-1f-16.act09 ( -- )   \ 003FC600
    self-wait-done
    0 self-turn-angle
    self-wait-done
    turn-round-anim play-out
    -1 self-idle-anim
    7 wait-frames
    look-at-anim self-anim
    16 wait-frames
    red-painting-remarked ebit? not if
        1 say   \ "A landscape painting composed entirely of the color red. It seems quite out of plac..."
        red-painting-remarked ebit-set
    else
        2 say   \ "The color bears a remarkable resemblance to blood. But... surely this is just red p..."
        red-painting-remarked ebit-clear
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
castle-1f-16 register-scripts   \ (its phases and actions, by their names)
