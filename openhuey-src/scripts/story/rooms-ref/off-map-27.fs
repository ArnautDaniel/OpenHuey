\ story/rooms/off-map-27.fs - the event scripts of room off-map-27 ($27; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-27
USING: actors messages items room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant gave-first                  \ (bit 0) the first item ($91) given
1 constant gave-second                 \ (bit 1) the second item ($92) given
2 constant nothing-left                \ (bit 2) taken: "There is nothing here." from now on
3 constant something-lacking           \ (bit 3) she lacks one of the items (to be given)
0 constant lacking-var                 \ (var 0) the items she lacks (each a byte)
1 constant used-item-var               \ (var 1) the item she used
0 constant wrong-sound                 \ (snd 0) not right (her answer)
4 constant reach-sound                 \ (snd 4) her reaching out
5 constant reach-back-sound            \ (snd 5) her drawing back

\ the items 0x91 / 0x92, by byte 3: 0 which of them Fiona lacks (one each) kept in event var 0
\ (0x91 low byte, 0x92 the next), and event +0x5C(3) when any; 1 they are given back, event
\ +0x5C(0) / (1)
: lacking ( item -- n )  1 swap items-of - 0 max ;   \ (short of one)
: lacks-kept ( -- )   \ (those she lacks into variable 0: $91 the low byte, $92 the next; event bit 3)
    $91 lacking  $92 lacking 8 lshift or  ?dup if  lacking-var swap var-set  something-lacking ebit-set  then ;
: lacks-given ( -- )   \ (and given: event bits 0 / 1)
    lacking-var var @
    dup $FF and ?dup if  $91 swap items-id send give-item  gave-first ebit-set  then
    8 rshift $FF and ?dup if  $92 swap items-id send give-item  gave-second ebit-set  then ;
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
        lacking-var 0 var? not if
            0 -27.75 10.0 5.68 flicker-sprite
        then
    then
    fg-3-flag3 story-flag? if
        0 part-shown
    then
;

: off-map-27.phase1 ( -- )   \ 0041C3E0
    0 0.699 0.0 -17.321 5 10 0 zone
    1 -16.2 0.0 -5.68 5 10 0 zone
    2 -21.456 0.0 6.443 5 10 0 zone
;

: off-map-27.phase2 ( -- )   \ 0041C420
    fiona-who 0 char-in-area? fiona-who 45 $32 char-heading? and if
        1 unhurried offer-action
    then
    fiona-who 1 char-in-area? fiona-who -90 $37 char-heading? and if
        5 unhurried offer-action
    then
    fiona-who 2 char-in-area? fiona-who 90 $37 char-heading? and if
        fg-3-flag3 story-flag? not if
            3 unhurried offer-action
        else
            6 unhurried offer-action
        then
    then
    fiona-who 1 2 char-zone-bits? if
        fiona-who -50 $3C char-heading? if
            2 unhurried offer-action
        then
    then
    fiona-who 2 2 char-zone-bits? if
        fiona-who -45 $3C char-heading? if
            4 unhurried offer-action
        then
    then
    fiona-who 4 char-in-area? fiona-who 45 $32 char-heading? and if
        $A unhurried offer-action
    then
    fg-3-flag3 story-flag? if
        fiona-who 3 char-in-area? fiona-who 90 $37 char-heading? and if
            7 unhurried offer-action
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
            $40AB say   \ "'Costumes' You are now able to change costumes. Change into alternate costu..."
            $F 7 fade
            wait-fade
            got-doc-action story-flag-set
            $AB message-param-room
            $AB 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B say   \ "has been saved to your file."
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
    [: subscreen-wanted state-flag? ;] wait-while
    self-idle-or-end
;

: off-map-27.act03 ( -- )   \ 0041C510
    self-wait-done
    180 self-turn-angle
    self-wait-done
    got-doc-action story-flag? not if
        $40 say   \ "A chest used to store clothes."
    else $26 3 pvar? if
        $57 say   \ "Is this the outfit I was wearing while in the castle? Change into this outfit? YE..."
        0 answer? if
            $F $44 fade
            wait-fade
            world-held state-flag-set
            fiona-who wrong-sound 6 char-sound
            $3C wait-frames
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
        $58 say   \ "The 'Under the Scalpel' outfit that I just removed is here. Change into this outf..."
        0 answer? if
            $F $44 fade
            wait-fade
            world-held state-flag-set
            fiona-who wrong-sound 6 char-sound
            $3C wait-frames
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
    something-lacking ebit? if
        nothing-left ebit? not if
            reach-out-anim play-out
            4 wait-frames
            gave-first ebit-clear
            gave-second ebit-clear
            1 off-map-27.cmd00
            0 effect-remove
            gave-first ebit? if
                $91 message-param-room
                0 $91 item-tab
                item-fanfare
                $8011 say   \ "You've obtained ."
            then
            gave-second ebit? if
                16 wait-frames
                $92 message-param-room
                0 $92 item-tab
                item-fanfare
                $8011 say   \ "You've obtained ."
            then
            nothing-left ebit-set
            reach-back-anim play-out
        else
            7 say   \ "There is nothing here."
        then
    else
        6 say   \ "What a strange machine... I wonder what they use it for?"
    then
    self-idle-or-end
;

: off-map-27.act05 ( -- )   \ 0041C640
    1 self-scripted
    self-wait-done
    8 say   \ "There is something written here."
    $F 6 fade
    wait-fade
    9 say   \ "'Variatio Machina' By using a medallions, one can actually transform items into new thi..."
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
    $41 say   \ "A chest used to store clothes. Change clothes? YES NO"
    0 answer? if
        $FF 1.0 0 bgm
        7 subscreen-open
        [: subscreen-wanted state-flag? ;] wait-while
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            fiona-who wrong-sound 6 char-sound
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
            7 wait-frames
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
        $10 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    self-idle-or-end
;

: off-map-27.act07 ( -- )   \ 0041C7B0
    self-wait-done
    -12.0 -22.0 self-turn-to-xz
    self-wait-done
    x-27-magist-medallion story-flag? not x-27-magist-medallion-2 story-flag? not and x-27-magist-medallion-3 story-flag? not and x-27-magist-medallion-4 story-flag? not and if
        hand-to-mouth-anim self-anim
        $A say   \ "What on earth could this machine be for? There's a slot on it..."
        self-wait-anim
    else
        $B say   \ "This must be a machine that dispenses something if a certain Plate-key is inserted."
    then
    self-idle-or-end
;

: off-map-27.act08 ( -- )   \ 0041C7E0
    self-wait-done
    $46 -9.0 -13.0 -160 $FFFF 5 self-move-to
    self-wait-done
    reach-out-anim play-out
    fiona-who reach-sound 6 char-sound
    4 wait-frames
    reach-back-anim play-out
    fiona-who reach-back-sound 6 char-sound
    1 -11.0 3.0 -17.0 flicker-sprite
    16 wait-frames
    $1E wait-frames
    pick-up-anim play-out
    1 effect-remove
    used-item-var 136 var? if
        $88 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            $88 message-param-room
            $88 1 item-give-count
            0 $88 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        x-27-magist-medallion story-flag-set
    else used-item-var 140 var? if
        $8C 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            $8C message-param-room
            $8C 1 item-give-count
            0 $8C item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        x-27-magist-medallion-2 story-flag-set
    else used-item-var 131 var? if
        $83 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            $83 message-param-room
            $83 1 item-give-count
            0 $83 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        x-27-magist-medallion-3 story-flag-set
    else used-item-var 137 var? if
        $89 1 item-count? if
            $75 message-param-room
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        else
            $89 message-param-room
            $89 1 item-give-count
            0 $89 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        x-27-magist-medallion-4 story-flag-set
    then then then then
    up-from-pick-anim play-out
    self-idle-or-end
;

: off-map-27.act09 ( -- )   \ 0041C9B0
    self-wait-done
    $46 -9.0 -13.0 -160 $FFFF 5 self-move-to
    self-wait-done
    reach-out-anim play-out
    fiona-who reach-sound 6 char-sound
    4 wait-frames
    reach-back-anim play-out
    $1E wait-frames
    hand-to-mouth-anim self-anim
    $C say   \ "Nothing happened. Nothing at all."
    self-wait-anim
    self-idle-or-end
;

: off-map-27.act0A ( -- )   \ 0041C9F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    8.0 -4.0 self-turn-to-xz
    self-wait-done
    $D say   \ "The book looks almost archaic in nature. Wait, something is written here..."
    $F 6 fade
    wait-fade
    $E say   \ "Quies Restores a small amount of stamina. Mundus Calms the nerves of those who use it."
    $F 7 fade
    wait-fade
    $42 subscreen-bit? not $44 subscreen-bit? not or if
        hand-to-mouth-anim self-anim
        $F say   \ "I wonder what these combinations are supposed to mean?"
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
off-map-27 register-scripts   \ (its phases and actions, by their names)
