\ story/rooms/chaos-forest-10.fs - the event scripts of room chaos-forest-10 ($108; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-10
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims hewie.sounds ;

\ byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(1, 0), then the partner's
\ message slot shows progress +0x73EDC0; else Progress_SpeechCall and the slot is closed
: chaos-forest-10.cmd00 ( b0 -- )   \ (Hewie's face: 0 / 1 loaded, 2 shown - his texture 1 - else done and his own again)
    case  0 of  s" O_HEW\HEW_201.TEX" speech-load  endof  1 of  s" O_HEW\HEW_202.TEX" speech-load  endof
          2 of  hewie-who 0 speak  hewie-who 1 speak  endof  >r speech-done  hewie-who speech-off r>  endcase ;

: chaos-forest-10.enter ( -- )   \ 00418DC0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    in-panic? if
        1 ebit-set
    then
    cf-10-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        cf-10-medallion story-flag? not if
            0 76.5 -25.5 134.5 flicker-sprite
        then
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    found-file-247 story-flag? cf-13-flag1 story-flag? not and if
        hewie-who char-activate
        hewie-who $118 4.7 54.573 180 char-to-xz
        0 hewie-who 1 action
    then
;

: chaos-forest-10.char-enter ( -- )   \ 00418E40
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
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        3 3 player-camera
    then then
    3 3 3 area-camera
;

: chaos-forest-10.phase1 ( -- )   \ 00418F40
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    8 2 2 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    fiona-who $D char-entered-area? if
        found-file-247 story-flag? cf-10-flag1 story-flag? not and if
            cf-10-flag1 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    0 ebit? not if
        1 ebit? not in-panic? and if
            0 ebit-set
            deal-things
        then
    then
    in-panic? if
        1 ebit-set
    else
        1 ebit-clear
    then
    found-file-247 story-flag? cf-13-flag1 story-flag? not and if
        0 0 var? if
            $1E chance? if
                hewie-who $40000001 6 char-sound
                0 180 var-set
            else
                0 60 var-set
            then
        else
            0 var-dec
            hewie-who $C0000001 6 char-sound
        then
    then
;

: chaos-forest-10.phase2 ( -- )   \ 00419050
    found-file-247 story-flag? cf-13-flag1 story-flag? not and if
        cf-10-flag1 story-flag? fiona-who hewie-who $A $A $2D char-touching-facing? and if
            2 unhurried offer-action
        then
    then
    cf-10-breakable-broken story-flag? not if
        0 76.5 -26.5 134.5 5 8 1 zone
        self-who 0 char-in-zone? if
            cf-10-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            76.5 -26.5 134.5 0 -2144325584 0 0.0 shards-at
            $88 5 76.5 -26.5 134.5 0 0 sound
            $40 $262 noise
            0 76.5 -25.5 134.5 flicker-sprite
        then
    else cf-10-medallion story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then then
;

: chaos-forest-10.phase5 ( -- )   \ 004190F0
    found-file-247 story-flag? cf-13-flag1 story-flag? not and if
        hewie-who action-end
        hewie-who char-done
        $27 0 pvar? if
            3 chaos-forest-10.cmd00
        then
    then
;

: chaos-forest-10.act00 ( -- )   \ 00419110
    1 self-scripted
    stalkers-stay state-flag-set
    stalker-who action-end
    stalker-who char-done
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    hewie-who action-end
    self-frames-reset
    1 self-wait-frames
    panic-eased
    0 message-prepare   \ "You've been shot. You'll be all right. This will stop the bleeding."
    1 result? if
        scene-play
    then
    30 hewie-trust
    world-held state-flag-set
    cf-10-flag1 story-flag-set
    fiona-who $9E 12.16 49.91 -7 char-to-xz
    hewie-who $118 4.7 54.573 180 char-to-xz
    $27 0 pvar? if
        3 chaos-forest-10.cmd00
    then
    0 counter-set
    0 hewie-who 1 action
    1 wait-counter
    stalker-who char-activate
    $FE $108 305 2 stalker-to-room
    stalker-who 3 3 char-camera
    stalker-who 2 stalker-command
    summoner-on state-flag-set
    summoner-own-cooldown
    camera-restart
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    $39 resident-flag-set
    $259 item-give
    $8285 item-give
    0 self-scripted
    stalkers-stay state-flag-clear
    plain-commands state-flag-clear
    hewie-commandable state-flag-clear
    self-idle-or-end
;

: chaos-forest-10.act01 ( -- )   \ 00419220
    1 self-noclip
    1 self-scripted
    self-wait-done
    $1004 0 self-anim-blend
    $27 0 pvar? if
        cf-10-flag1 story-flag? not if
            0 chaos-forest-10.cmd00
        else
            1 chaos-forest-10.cmd00
        then
        2 chaos-forest-10.cmd00
    then
    1 counter-set
    begin
        yield
    again
;

: chaos-forest-10.act02 ( -- )   \ 00419250
    self-wait-done
    crouch-down-anim self-anim
    self-wait-anim
    1 message   \ "You'll be alright, boy. Just stay still, OK?"
    wait-message
    hewie-who cowed-whine 5 char-sound
    crouch-up-anim self-anim
    self-wait-anim
    self-idle-or-end
;

: chaos-forest-10.act03 ( -- )   \ 00419270
    self-wait-done
    76.5 134.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cf-10-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: chaos-forest-10.act04 ( -- )   \ 004192D0
    self-wait-done
    $27 0 pvar? if
        0 chaos-forest-10.cmd00
        2 chaos-forest-10.cmd00
    then
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "You've been shot. You'll be all right. This will stop the bleeding."
    1 result? if
        scene-play
    then
    $27 0 pvar? if
        3 chaos-forest-10.cmd00
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
chaos-forest-10 register-scripts   \ (its phases and actions, by their names)
