\ story/rooms/old-mansion-1f-14.fs - the event scripts of room old-mansion-1f-14 ($53; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-14
USING: room-names story.words story.shared flag-names story-names floor-flags ;

: old-mansion-1f-14.enter ( -- )   \ 004268B0
    room-sounds
    film-reel-used-om1f-14 story-flag? not if
        0 part-shown
        1 part-hidden
    else
        0 part-hidden
        1 part-shown
    then
    8 0 object-hidden
    9 0 object-hidden
    om1f-14-chicken-jerky story-flag? not if
        0 15.82 1.0 20.96 flicker-sprite
    then
;

: old-mansion-1f-14.char-enter ( -- )   \ 004268F0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
;

: old-mansion-1f-14.phase1 ( -- )   \ 00426930
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
;

: old-mansion-1f-14.phase2 ( -- )   \ 00426950
    fiona-who 3 $32 char-faces-area? if
        1 unhurried offer-action
    then
    fiona-who 4 char-in-area? fiona-who 90 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 5 char-in-area? fiona-who -45 $3C char-heading? and if
        4 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who -45 $3C char-heading? and if
        4 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 45 $3C char-heading? and if
        4 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who -45 $3C char-heading? and if
        5 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who -45 $3C char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 45 $3C char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $B $3C char-faces-area? if
        6 unhurried offer-action
    then
    om1f-14-chicken-jerky story-flag? not if
        0 0 5 -8 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then
;

: old-mansion-1f-14.act00 ( -- )   \ 004269E0
    1 self-scripted
    3 stalker-kind? $22 stalker-kind? or if
        stalker-who action-end
        stalker-who char-done
    then
    self-wait-done
    self-frames-reset
    self-wait-16
    $F $44 fade
    wait-fade
    0 5 movie-play
    yield
    yield
    movie-restart-op cutscene-control
    1 result? if
        1.0 movie-volume
        yield
        world-held state-flag-set
        begin
            movie-state-op cutscene-control
            -1 result? not if
                6 2 pad? not if
                    yield
                    false
                else
                    true
                then
            else
                true
            then
        until
        movie-stop-op cutscene-control
        begin
            movie-playing? while
            yield
        repeat
        yield
        world-held state-flag-clear
    then
    film-reel-used-om1f-14-2 story-flag-set
    world-held state-flag-set
    $E item-use
    3 stalker-kind? $22 stalker-kind? or if
        0 part-hidden
        1 part-shown
        film-reel-used-om1f-14 story-flag-set
        yield
        yield
        7 0 movie-play
        6 cutscene-start
        yield
        yield
        scene-wait-ready
        3 4 0 char-model-op
        $FE $53 43 2 stalker-to-room
        stalker-who 0 0 char-camera
        0 $F9 7 action
        panic-eased
        0 message-prepare   \ "What is this? I was being filmed. Who's there? Why are you after me? What d..."
        1 result? if
            scene-play
        then
        world-held state-flag-set
        stalker-who $53 0 room-doors-state
        stalker-who $75 0 door-lock-for
        $FE $53 43 2 stalker-to-room
        stalker-who char-activate
        stalker-who $2B 7.18 33.88 -160 char-to-xz
        summoner-own-cooldown
        1 self-scripted
        om1f-14-flag1 story-flag-set
        stalkers-stay state-flag-clear
        fiona-who $16 -1.36 20.03 0 char-to-xz
        0 0 player-camera
        yield
        camera-restart
    else
        fiona-who $76 -6.79 9.91 168 char-to-xz
        0 0 player-camera
        yield
        camera-restart
        stalkers-stay state-flag-clear
    then
    8 0 object-hidden
    9 0 object-hidden
    $F $41 fade
    wait-fade
    $33 resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-14.act01 ( -- )   \ 00426B60
    self-wait-done
    film-reel-used-om1f-14-2 story-flag? not if
        0 ebit? not if
            1 message   \ "There's a projector set up here. Its looks rather old, but perhaps it still works."
            wait-message
            0 ebit-set
        else
            2 message   \ "The screen is already set up. All I need now is a film to play."
            wait-message
        then
    else
        4 message   \ "The film seems to have totally played out. I've seen everything there is to see."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-14.act02 ( -- )   \ 00426B80
    self-wait-done
    film-reel-used-om1f-14 story-flag? not if
        3 message   \ "A screen... Has someone been watching films here recently?"
        wait-message
    else
        5 message   \ "A slash runs along the surface of the screen. I'd best forget it. It's definitely u..."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-14.act03 ( -- )   \ 00426B90
    self-wait-done
    15.82 20.96 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-14-chicken-jerky story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-14.act04 ( -- )   \ 0047AD38
    self-wait-done
    6 message   \ "Just a lifeless mannequin."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-14.act05 ( -- )   \ 00426BF0
    self-wait-done
    fiona-who 8 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    else fiona-who 9 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    else fiona-who $A char-in-area? if
        90 self-turn-angle
        self-wait-done
    then then then
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    7 message   \ "This shelf is packed full of old books, discolored from years of disuse. A majority of..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-14.act06 ( -- )   \ 00426C20
    self-wait-done
    19.0 33.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message   \ "Scattered mannequin parts are strewn about. When assembled correctly, these parts will..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-14.act07 ( -- )   \ 00426C40
    begin
        $8E8 cutscene-cue-reached? not while
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        $2D sprites-additive
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: old-mansion-1f-14.act08 ( -- )   \ 00426C60
    self-wait-done
    8 0 object-hidden
    9 0 object-hidden
    3 partner-load
    2 char-start
    0 part-hidden
    1 part-shown
    yield
    yield
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    3 4 0 char-model-op
    $FE $53 43 2 stalker-to-room
    0 $F9 7 action
    panic-eased
    0 message-prepare   \ "What is this? I was being filmed. Who's there? Why are you after me? What do yo..."
    1 result? if
        scene-play
    then
    8 0 object-hidden
    9 0 object-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-14 register-scripts   \ (its phases and actions, by their names)
