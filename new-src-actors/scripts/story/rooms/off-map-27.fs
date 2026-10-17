\ story/rooms/off-map-27.fs - the event scripts of room off-map-27 ($27; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-27
USING: actors messages items room-names story.words story.shared flag-names story-names ;

\ the items 0x91 / 0x92, by byte 3: 0 which of them Fiona lacks (one each) kept in event var 0
\ (0x91 low byte, 0x92 the next), and event +0x5C(3) when any; 1 they are given back, event
\ +0x5C(0) / (1)
: lacking ( item -- n )  1 swap items-of - 0 max ;   \ (short of one)
: lacks-kept ( -- )   \ (those she lacks into variable 0: $91 the low byte, $92 the next; event bit 3)
    $91 lacking  $92 lacking 8 lshift or  ?dup if  0 swap var-set  3 ebit-set  then ;
: lacks-given ( -- )   \ (and given: event bits 0 / 1)
    0 var @
    dup $FF and ?dup if  $91 swap items-id send give-item  0 ebit-set  then
    8 rshift $FF and ?dup if  $92 swap items-id send give-item  1 ebit-set  then ;
: off-map-27.cmd00 ( b0 -- )   \ (items $91 / $92 topped up to one each: b0 0 those she lacks kept, 1 given)
    case  0 of  lacks-kept  endof  1 of  lacks-given  endof  endcase ;

: off-map-27.enter ( -- )   \ 0041C390
    room-sounds
    $36 1.0 0 bgm
    $FE exit-taken? if
        fiona-who $60 -90 char-to-tri-facing
        0 0 player-camera
        0 fiona-who 0 action
        0 off-map-27.cmd00
        0 0 var? not if
            0 -27.75 10.0 5.68 flicker-sprite
        then
    then
    fg-3-flag3 story-flag? if
        0 1 $14 door-bits
    then
;

: off-map-27.phase1 ( -- )   \ 0041C3E0
    0 0.699 0.0 -17.321 5 10 0 zone
    1 -16.2 0.0 -5.68 5 10 0 zone
    2 -21.456 0.0 6.443 5 10 0 zone
;

: off-map-27.phase2 ( -- )   \ 0041C420
    fiona-who 0 char-in-area? fiona-who 45 $32 char-heading? and if
        5 1 0 scene-change
    then
    fiona-who 1 char-in-area? fiona-who -90 $37 char-heading? and if
        5 5 0 scene-change
    then
    fiona-who 2 char-in-area? fiona-who 90 $37 char-heading? and if
        fg-3-flag3 story-flag? not if
            5 3 0 scene-change
        else
            5 6 0 scene-change
        then
    then
    fiona-who 1 2 char-zone-bits? if
        fiona-who -50 $3C char-heading? if
            5 2 0 scene-change
        then
    then
    fiona-who 2 2 char-zone-bits? if
        fiona-who -45 $3C char-heading? if
            5 4 0 scene-change
        then
    then
    fiona-who 4 char-in-area? fiona-who 45 $32 char-heading? and if
        5 $A 0 scene-change
    then
    fg-3-flag3 story-flag? if
        fiona-who 3 char-in-area? fiona-who 90 $37 char-heading? and if
            5 7 0 scene-change
        then
    then
;

: off-map-27.act00 ( -- )   \ 0041C4A0
    world-held state-flag-clear
    camera-restart
    $F 1 fade
    wait-fade
    fg-3-flag3 story-flag? not if
        x-37-flag2 story-flag? got-doc-action story-flag? not and if
            $F 6 fade
            wait-fade
            $40AB message   \ "'Costumes' You are now able to change costumes. Change into alternate costu..."
            wait-message
            $F 7 fade
            wait-fade
            got-doc-action story-flag-set
            $AB message-param-room
            $AB 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message   \ "has been saved to your file."
            wait-message
        then
    then
    self-idle-or-end
;

: off-map-27.act01 ( -- )   \ 0041C4E8
    self-wait-done
    $F 0 fade
    wait-fade
    world-held state-flag-set
    $FE exit-check
    self-idle-or-end
;

: off-map-27.act02 ( -- )   \ 0041C500
    self-wait-done
    -100 self-turn-angle
    self-wait-done
    3 subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    self-idle-or-end
;

: off-map-27.act03 ( -- )   \ 0041C510
    self-wait-done
    180 self-turn-angle
    self-wait-done
    got-doc-action story-flag? not if
        $40 message   \ "A chest used to store clothes."
        wait-message
    else $26 3 pvar? if
        $57 message   \ "Is this the outfit I was wearing while in the castle? Change into this outfit? YE..."
        wait-message
        0 answer? if
            $F $44 fade
            wait-fade
            world-held state-flag-set
            fiona-who 0 6 char-sound
            self-frames-reset
            $3C self-wait-frames
            new-game-sounds state-flag-set
            1 sound-set
            2 fiona-costume
            $26 2 pvar-set
            effects-arena-flip
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            fiona-who 3 -0.912 -13.497 180 char-to-xz
            camera-restart
            $F $41 fade
            wait-fade
        then
    else
        $58 message   \ "The 'Under the Scalpel' outfit that I just removed is here. Change into this outf..."
        wait-message
        0 answer? if
            $F $44 fade
            wait-fade
            world-held state-flag-set
            fiona-who 0 6 char-sound
            self-frames-reset
            $3C self-wait-frames
            new-game-sounds state-flag-set
            1 sound-set
            3 fiona-costume
            $26 3 pvar-set
            effects-arena-flip
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            fiona-who 3 -0.912 -13.497 180 char-to-xz
            camera-restart
            $F $41 fade
            wait-fade
        then
    then then
    self-idle-or-end
;

: off-map-27.act04 ( -- )   \ 0041C5B0
    self-wait-done
    -28.0 6.0 self-turn-to-xz
    self-wait-done
    3 ebit? if
        2 ebit? not if
            $902 self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            0 ebit-clear
            1 ebit-clear
            1 off-map-27.cmd00
            0 effect-remove
            0 ebit? if
                $91 message-param-room
                0 $91 item-tab
                item-fanfare
                $8011 message   \ "You've obtained ."
                wait-message
            then
            1 ebit? if
                self-frames-reset
                self-wait-16
                $92 message-param-room
                0 $92 item-tab
                item-fanfare
                $8011 message   \ "You've obtained ."
                wait-message
            then
            2 ebit-set
            $903 self-anim
            self-wait-anim
        else
            7 message   \ "There is nothing here."
            wait-message
        then
    else
        6 message   \ "What a strange machine... I wonder what they use it for?"
        wait-message
    then
    self-idle-or-end
;

: off-map-27.act05 ( -- )   \ 0041C640
    1 self-scripted
    self-wait-done
    8 message   \ "There is something written here."
    wait-message
    $F 6 fade
    wait-fade
    9 message   \ "'Variatio Machina' By using a medallions, one can actually transform items into new thi..."
    wait-message
    $F 7 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: off-map-27.act06 ( -- )   \ 0041C660
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message   \ "A chest used to store clothes. Change clothes? YES NO"
    wait-message
    0 answer? if
        $FF 1.0 0 bgm
        7 subscreen-open
        begin
            subscreen-wanted state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            fiona-who 0 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            hewie-who char-here? if
                hewie-who 2 char-cond? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $36 1.0 0 bgm
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    self-idle-or-end
;

: off-map-27.act07 ( -- )   \ 0041C7B0
    self-wait-done
    -12.0 -22.0 self-turn-to-xz
    self-wait-done
    x-27-magist-medallion story-flag? not x-27-magist-medallion-2 story-flag? not and x-27-magist-medallion-3 story-flag? not and x-27-magist-medallion-4 story-flag? not and if
        $1D01 self-anim
        $A message   \ "What on earth could this machine be for? There's a slot on it..."
        wait-message
        self-wait-anim
    else
        $B message   \ "This must be a machine that dispenses something if a certain Plate-key is inserted."
        wait-message
    then
    self-idle-or-end
;

: off-map-27.act08 ( -- )   \ 0041C7E0
    self-wait-done
    $46 -9.0 -13.0 -160 $FFFF 5 self-move-to
    self-wait-done
    $902 self-anim
    self-wait-anim
    fiona-who 4 6 char-sound
    self-frames-reset
    4 self-wait-frames
    $903 self-anim
    self-wait-anim
    fiona-who 5 6 char-sound
    1 -11.0 3.0 -17.0 flicker-sprite
    self-frames-reset
    self-wait-16
    self-frames-reset
    $1E self-wait-frames
    $900 self-anim
    self-wait-anim
    1 effect-remove
    1 136 var? if
        $88 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            $88 message-param-room
            $88 1 item-give-count
            0 $88 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        x-27-magist-medallion story-flag-set
    else 1 140 var? if
        $8C 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            $8C message-param-room
            $8C 1 item-give-count
            0 $8C item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        x-27-magist-medallion-2 story-flag-set
    else 1 131 var? if
        $83 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            $83 message-param-room
            $83 1 item-give-count
            0 $83 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        x-27-magist-medallion-3 story-flag-set
    else 1 137 var? if
        $89 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        else
            $89 message-param-room
            $89 1 item-give-count
            0 $89 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        x-27-magist-medallion-4 story-flag-set
    then then then then
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: off-map-27.act09 ( -- )   \ 0041C9B0
    self-wait-done
    $46 -9.0 -13.0 -160 $FFFF 5 self-move-to
    self-wait-done
    $902 self-anim
    self-wait-anim
    fiona-who 4 6 char-sound
    self-frames-reset
    4 self-wait-frames
    $903 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    $1D01 self-anim
    $C message   \ "Nothing happened. Nothing at all."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: off-map-27.act0A ( -- )   \ 0041C9F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    8.0 -4.0 self-turn-to-xz
    self-wait-done
    $D message   \ "The book looks almost archaic in nature. Wait, something is written here..."
    wait-message
    $F 6 fade
    wait-fade
    $E message   \ "Quies Restores a small amount of stamina. Mundus Calms the nerves of those who use it."
    wait-message
    $F 7 fade
    wait-fade
    $42 subscreen-bit? not $44 subscreen-bit? not or if
        $1D01 self-anim
        $F message   \ "I wonder what these combinations are supposed to mean?"
        wait-message
        self-wait-anim
        $42 subscreen-bit
        $44 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-27.char-enter ( -- )   \ 0047ACB0
;

\ ---- registered ----
' off-map-27.enter off-map-27 0 room-script!
' off-map-27.phase1 off-map-27 1 room-script!
' off-map-27.phase2 off-map-27 2 room-script!
' off-map-27.act00 off-map-27 $00 action-script!
' off-map-27.act01 off-map-27 $01 action-script!
' off-map-27.act02 off-map-27 $02 action-script!
' off-map-27.act03 off-map-27 $03 action-script!
' off-map-27.act04 off-map-27 $04 action-script!
' off-map-27.act05 off-map-27 $05 action-script!
' off-map-27.act06 off-map-27 $06 action-script!
' off-map-27.act07 off-map-27 $07 action-script!
' off-map-27.act08 off-map-27 $08 action-script!
' off-map-27.act09 off-map-27 $09 action-script!
' off-map-27.act0A off-map-27 $0A action-script!
' off-map-27.char-enter off-map-27 6 room-script!
