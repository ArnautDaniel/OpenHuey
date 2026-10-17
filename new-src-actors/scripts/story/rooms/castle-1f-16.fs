\ story/rooms/castle-1f-16.fs - the event scripts of room castle-1f-16 ($1A; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-16
USING: room-names story.words story.shared flag-names story-names floor-flags ;

\ room 0x1A (D_003FC660): the fan turns
: castle-1f-16.cmd00 ( -- )  fan-turn ;
\ room 0x1A: room effect slot 0 made anew as the second TV screen (TvScreenB).
: castle-1f-16.cmd01 ( -- )  0 1 tv-in ;   \ (room effect 0 the TV showing the screen)

: castle-1f-16.enter ( -- )   \ 003FC060
    room-sounds
    0 $F1 0 action
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
    1 ebit-clear
    8 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else 8 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else 8 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else 8 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    4 stalker-alert? if
        1 ebit-clear
    then
    1 ebit? if
        0 stalker-who 5 action
    else
        $78 1 summoner-wait
    then
    8 pvar-inc
    exit
;

: castle-1f-16.char-enter ( -- )   \ 003FC0A0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 -1 player-camera
    then then
    0 1 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            4 ebit-set
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
            4 ebit-clear
        then
    then
;

: castle-1f-16.phase1 ( -- )   \ 003FC110
    2 ebit? not if
        6 sound-bank-loaded? if
            c1f-16-flag2 story-flag? if
                $40000001 6 23.0 11.0 9.0 0 0 sound
            then
            2 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 27.0 15.0 -6.0 0 0 sound
            else 0 1 var? if
                $40000006 6 27.0 15.0 -6.0 0 0 sound
            else 0 2 var? if
                $40000007 6 27.0 15.0 -6.0 0 0 sound
            else 0 3 var? if
                $40000008 6 27.0 15.0 -6.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        c1f-16-flag1 story-flag? if
            $C0000001 6 23.0 11.0 9.0 0 0 sound
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
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-16-magnesia story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-16.act02 ( -- )   \ 003FC330
    self-wait-done
    $E1 18.5 13.6 120 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $902 self-anim
    self-wait-anim
    9 0 pvar? 9 1 pvar? or if
        c1f-16-flag2 story-flag? not if
            c1f-16-flag2 story-flag-set
            fiona-who 0 6 char-sound
            c1f-16-flag1 story-flag? not if
                3 $86 0.0 0.0 0.0 0 0 sound
            else
                $40000001 6 23.0 11.0 9.0 0 0 sound
            then
            $40 7 noise
            1 noise-level
        else
            fiona-who 2 6 char-sound
            1 6 sound-stop
            c1f-16-flag2 story-flag-clear
            0 noise-level
        then
        c1f-16-flag1 story-flag? not if
            1 self-scripted
            $5A threat-add
            1 $FF 8 rumble
            fiona-who $43 5 char-sound
            $F04 self-anim
            self-wait-anim
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
            $902 self-anim
            self-wait-anim
            $40000001 6 23.0 11.0 9.0 0 0 sound
            $903 self-anim
            self-wait-anim
            c1f-16-flag1 story-flag-set
            0 self-scripted
        else
            $903 self-anim
            self-wait-anim
        then
    else
        $903 self-anim
        self-wait-anim
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-16.act04 ( -- )   \ 003FC4C0
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-16.act03 ( -- )   \ 003FC420
    stalkers-stay state-flag-set
    4 ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $CC $8004 5 -4.7 -39.3 90 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 ebit-clear
    0 avoid-prompt
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? 0 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] castle-1f-16.act04 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                4 ebit? stalker-who char-here? not and if
                    4 ebit-clear
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
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
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
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        0 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        0 ebit-set
    then
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
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
    self-frames-reset
    $12 self-wait-frames
    stalker-who 2 6 char-sound
    1 6 sound-stop
    c1f-16-flag2 story-flag-clear
    0 noise-level
    9 pvar-inc
    stalker-who 3 stalker-command
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: castle-1f-16.act08 ( -- )   \ 003FC5F0
    self-wait-done
    -145 self-turn-angle
    self-wait-done
    0 message   \ "It would be nice to freshen up a bit... But there's a crack in this mirror and I can't..."
    wait-message
    self-idle-or-end
;

: castle-1f-16.act09 ( -- )   \ 003FC600
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    $A00 self-anim
    self-frames-reset
    self-wait-16
    3 ebit? not if
        1 message   \ "A landscape painting composed entirely of the color red. It seems quite out of plac..."
        wait-message
        3 ebit-set
    else
        2 message   \ "The color bears a remarkable resemblance to blood. But... surely this is just red p..."
        wait-message
        3 ebit-clear
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
castle-1f-16 register-scripts   \ (its phases and actions, by their names)
