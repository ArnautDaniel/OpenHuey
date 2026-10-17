\ hewie-names.fs - Hewie's actions by name (the original's numbers: Hewie_SetAction's cases;
\ the story's `hewie-action?` and its orders give them by number). hewie/actions.fs sets each
\ one up; the comments say what he does in it.
IN: hewie-names

enum: hewie-action
    choose-act            \ 0  nothing on: into a low pose and choose (under a script: standing)
    lie-by-her-act        \ 1  lying low by her
    stay-act              \ 2  staying by her, for his trust's time (her "stay")
    stay-a-while-act      \ 3  by her, for a given time
    hold-low-act          \ 4  a low pose, looking about
    settle-act            \ 5  sitting, lying or standing about, looking round
    sniff-act             \ 6  sniffing toward his target or a scent
    watch-act             \ 7  standing alert, his head on his target
    face-target-act       \ 8  turned to his target, in the stance
    wait-for-her-act      \ 9  turned to her, in the stance
    bark-at-target-act    \ $A
    bark-at-her-act       \ $B
    come-to-her-act       \ $C  calm
    come-back-act         \ $D  her "come back"
    hurry-to-her-act      \ $E  followed or chased
    sulk-to-her-act       \ $F  upset, chased
    keep-off-her-act      \ $10
    keep-off-target-act   \ $11
    flank-act             \ $12 round to his target's flank
    scramble-act          \ $13 darting about, then sitting
    prick-ears-act        \ $14 stops and pricks his ears
    roam-act              \ $15
    come-to-place-act     \ $16 to his place by her
    trick-act             \ $17 one of his tricks, at random
    sit-trick-act         \ $18
    sit-trick-2-act       \ $19
    stand-trick-act       \ $1A
    stand-trick-2-act     \ $1B
    lie-down-trick-act    \ $1C sits, lies down and stays a while
    heard-her-act         \ $1D a bark that he heard her, then what she asked
    wont-act              \ $1E sits and turns his head away
    at-flank-act          \ $1F going for the stalker: a flank
    at-stalker-act        \ $20 going for the stalker
    at-leg-act            \ $21 going for the stalker's leg (charged, head on)
    at-leg-2-act          \ $22
    close-on-act          \ $23 harrying: at his target and off again
    away-from-her-act     \ $24 out by an exit away from her
    away-act              \ $25 out by any exit
    stand-for-her-act     \ $26 standing by her, then lying by her (or settling)
    stand-then-stay-act   \ $27 ... then staying (her "stay")
    sit-for-her-act       \ $28 sitting by her ("praise me")
    sit-then-stay-act     \ $29
    lie-for-her-act       \ $2A lying, a roll over
    lie-then-stay-act     \ $2B
    \ off screen (out of the room being played)
    to-her-room-act       \ $2C
    back-to-her-act       \ $2D her "come back"
    wait-off-act          \ $2E waiting, followed
    wait-a-while-act      \ $2F waiting a while (hurt; by her as she left)
    wait-scuffle-act      \ $30 after a scuffle with the stalker
    wait-after-flight-act \ $31 after drawing the stalker off
    out-random-act        \ $32 out by an open exit at random
    to-noise-act          \ $33 toward a room he heard something in
    loud-noise-act        \ $34 barking, heard all about
    out-random-away-act   \ $35 out at random, not to her room
    hide-act              \ $36 staying hidden
    stop-off-act          \ $37
    after-attack-act      \ $38 waiting off to the side
    to-her-cry-act        \ $39 her cry for help
    $3B
    \ the story's moves (Hewie_Requests)
    story-anim-act        \ $3B an animation, moved by its root motion
    story-blend-act       \ $3C ... blended in
    story-blend8-act      \ $3D ... blended, no time of its own
    story-stand-act       \ $3E a standing animation
    story-walk-act        \ $3F to the spot, walking
    story-run-act         \ $40 ... at a gait by how far
    story-path-act        \ $41 along a way to a triangle, walking
    story-path-run-act    \ $42 ... running
    story-turn-act        \ $43 turning on the spot
    story-turn-2-act      \ $44
    story-bark-act        \ $45
    story-spot-act        \ $46 to a spot, milling about it
    story-anim-still-act  \ $47 an animation, not moved
    \ by her side, her commands
    turn-with-her-act     \ $48 turning with her as she gestures, then sitting
    scolded-close-act     \ $49
    petted-act            \ $4A
    patted-act            \ $4B
    sniff-about-act       \ $4C
    show-off-act          \ $4D "praise me", lying, or a roll over
    after-coming-out-act  \ $4E her "go" after coming out
    answer-call-act       \ $4F at whoever called him
    answer-call-2-act     \ $50
    random-idle-act       \ $51 lying by her, standing by or settling
    knocked-down-act      \ $52 down till he has health again
    near-target-act       \ $53 keeping near his target
    near-her-act          \ $54 keeping near her
    close-by-her-act      \ $55
    near-spot-act         \ $56
    close-by-spot-act     \ $57
    lie-low-act           \ $58
    harry-act             \ $59 harrying his target
    harry-stalker-act     \ $5A ... the stalker, three times
    at-cast-3-act         \ $5B at one of the cast (slots 3, 4, 5; the odd ones longer)
    at-cast-3-long-act    \ $5C
    at-cast-4-act         \ $5D
    at-cast-4-long-act    \ $5E
    at-cast-5-act         \ $5F
    at-cast-5-long-act    \ $60
    harry-her-act         \ $61
    harry-her-3-act       \ $62
    go-there-act          \ $63 to the spot she showed
    scramble-there-act    \ $64 darting about it
    squeeze-out-act       \ $65 out past a door in his way
    walk-out-act          \ $66 out by the door he stands at
    step-off-her-act      \ $67 too close to her
    pushed-back-act       \ $68
    yelp-act              \ $69 struck: his yelp, pushed back
    off-edge-act          \ $6A his nose over an edge
    door-yelp-act         \ $6B a door slammed on him
    pushed-out-act        \ $6C out by a door
    rush-act              \ $6D running at his target and biting
    off-slope-act         \ $6E
    run-to-her-act        \ $6F a happy whine, lying low by her
    dash-act              \ $70
    scolded-afar-act      \ $71
    keep-behind-act       \ $72 behind her, facing away (the story's)
    anim-then-act         \ $73 the animation to its end
    go-limp-act           \ $74 struck down
    tackle-act            \ $75 the story's: at the stalker
    yelp-anim-act         \ $76 his yelp's animation
    hidden-act            \ $77 hidden away (the story's time)
    fetch-act             \ $78
    scene-pose-act        \ $79
    ready-act             \ $7A waiting low, readying
    draw-off-act          \ $7B drawing the stalker off while she hides
    near-target-2-act     \ $7C
    whine-act             \ $7D
    whine-praise-act      \ $7E due a praise
    story-leap-act        \ $7F
    sit-then-act          \ $80 the story's: sitting a while, then on
    smell-act             \ $81 a scent: his head on it
    step-off-target-act   \ $82
    steered-act           \ $83 the Hewie-played sections (not built)
    steered-2-act         \ $84
    bark-act              \ $85
    back-on-floor-act     \ $86
    run-up-jump-act       \ $87 (not built)
;

\ his move mode (his-mode; the characters' move modes): his own; out of reach (on a ladder, off
\ screen - never his); struck; off screen; engaged (going for someone, keeping near); a scene's
\ pose; with her (a meeting by her side)
enum: hewie-mode  free-mode 3 unreachable-mode struck-mode 6 away-mode 8 engaged-mode $B posed-mode $C meeting-mode ;
\ his mood (his-mood): normal; pleased (praised from afar); upset (scolded too often); angry
\ (hit by her)
enum: hewie-mood  normal-mood pleased-mood upset-mood angry-mood ;
\ what he is doing, for the story (his-sub: Hewie_MoveSubMode, and his fights)
enum: hewie-sub  standing-sub walking-sub running-sub sitting-sub lying-sub
    $A holding-on-sub  $17 travelling-sub going-for-sub leaping-sub ;
\ his animations' groups (Hewie_AnimGroup): still in a pose, getting into one, on the move, down
enum: hewie-group  in-standing in-sitting in-lying in-low
    in-standing-up in-sitting-down in-lying-down in-getting-low
    in-running in-trotting in-walking in-on-side in-rolling-over in-down in-going-down in-other ;
\ where he is, as he tells (hewie-doing's first): not in the game, in another room, with her
enum: dog-whereabouts  dog-absent dog-elsewhere dog-with-her ;
