\ fiona/anims.fs - Fiona's animations by name (the motion's id in her FIN / FIS banks: its set
\ in the high byte). Named for what she does in them; the original's states that play them in
\ fiona.c.
IN: fiona.anims

\ ---- standing (Fiona_IdleAnim: the base and its variants) ----
0 constant stand-anim             \ standing
1 constant rest-anim              \ resting (calm a while, followed)
2 constant attack-anim            \ the panic attack
5 constant chased-stand-anim      \ standing while chased
\ ---- about: walking and running, as she feels (Fiona_MoveLook's sets) ----
$200 constant walk-anim            \ walking
$201 constant walk-shaken-anim     \ walking, shaken
$204 constant walk-scared-anim     \ walking, scared
$208 constant walk-chased-anim     \ walking while chased
$202 constant run-anim             \ running
$203 constant run-shaken-anim      \ running, shaken
$205 constant run-scared-anim      \ running, scared
$206 constant panic-run-anim       \ running blindly, panicking
$207 constant out-of-breath-anim   \ bent over, out of breath
$400 constant turn-left-anim       \ turning on the spot
$401 constant turn-right-anim
$402 constant turn-round-anim
$403 constant flee-turn-anim       \ turning to flee (Fiona_StateFleeTurn)
\ ---- doors ----
$600 constant through-door-anim            \ through a door: $600 + its kind (FIN_D000.MTN)
$608 constant rattle-front-anim    \ trying a locked door, from in front
$609 constant rattle-behind-anim   \ ... from behind
$60A constant rattle-front-slow-anim   \ ... slowly (chased or panicking)
$60B constant rattle-behind-slow-anim
\ ---- ladders (Fiona_StateOnLadder) ----
$700 constant on-ladder-top-anim   \ getting on from the top
$701 constant rung-down-anim       \ a rung down
$702 constant rung-down-end-anim   \ ... its end
$703 constant off-ladder-foot-anim \ getting off at the foot
$704 constant on-ladder-foot-anim  \ getting on from the foot
$705 constant rung-up-anim         \ a rung up
$706 constant rung-up-end-anim     \ ... its end
$707 constant off-ladder-top-anim  \ getting off at the top
$708 constant ladder-hold-anim     \ holding on
$709 constant ladder-hold-2-anim
$70A constant ladder-slip-anim     \ losing her hold
$70B constant ladder-fall-anim     \ falling off
$70C constant ladder-fallen-anim   \ down at its foot
\ ---- falling, getting up ----
$B00 constant fall-anim            \ falling (a fright)
$B01 constant lying-anim           \ down
$B02 constant get-up-anim          \ getting up
$B03 constant held-down-anim       \ held down by him
$B04 constant flung-down-anim      \ flung down, panicking
\ ---- letting go of a thing (the throw, the aim, the set down) ----
$D00 constant throw-anim
$D01 constant aimed-throw-anim
$D02 constant set-down-anim
\ ---- her gestures to Hewie (named for the commands that play them) ----
$C00 constant go-for-it-gesture-anim
$C02 constant call-gesture-anim      \ (from afar: praise, scold, come back)
$C03 constant stay-gesture-anim      \ (stay, come)
$C04 constant go-there-gesture-anim
$C06 constant rouse-gesture-anim
$C07 constant pat-start-anim   $C08 constant pat-anim   $C09 constant pat-end-anim
$C0A constant tend-start-anim  $C0B constant tend-lead-in-anim  $C0C constant tend-anim
$C0D constant scold-near-anim
$C0E constant look-around-anim  $C0F constant look-around-end-anim   \ (attack now)
\ ---- defending ----
$E00 constant shove-anim           \ her hand
$E01 constant kick-anim            \ her body
$F02 constant pull-free-anim       \ pulling free as he lets go
\ ---- struck ----
$1001 constant stumble-anim
$1008 constant thrown-forward-anim \ thrown, her back to him
$100A constant up-from-forward-anim
$100B constant thrown-back-anim    \ thrown, facing him
$100D constant up-from-back-anim
$100E constant knocked-anim
$100F constant fall-through-anim   \ down through a doorway
$1100 constant held-anim           \ held by him
$1101 constant pulled-back-anim    \ pulled back by him
$1503 constant held-front-anim
\ ---- pushing (Fiona_StatePush) ----
$1200 constant lean-anim           \ leaning on it
$1201 constant push-anim
$1202 constant let-go-push-anim
$1203 constant strain-anim
\ ---- led away, held by homunculi ----
$1400 constant led-by-hand-anim
$1401 constant dragged-anim
$1403 constant shake-free-anim
$1404 constant dragged-off-anim    \ (its end: the game is over)
$1405 constant held-under-anim     \ down under the homunculi
$1406 constant struggling-anim
$1407 constant up-from-under-anim
$1500 constant led-walking-anim
\ ---- the story's own (the rooms' scripts play them; named from how they look when played -
\ hga's renders - so ? where a guess) ----
4 constant hug-self-anim           \ ? hugging herself, cold
$10 constant hug-self-2-anim       \ ? the same
$800 constant crouch-down-anim     \ crouching down (in a shadow: shared.act81)
$801 constant crouching-anim
$802 constant crouch-up-anim
$900 constant pick-up-anim         \ squatting to something on the floor (an item taken)
$901 constant up-from-pick-anim
$902 constant reach-out-anim       \ reaching out at chest height
$903 constant reach-back-anim
$904 constant reach-up-anim        \ reaching up high
$905 constant reach-down-anim
$A00 constant look-at-anim         \ ? examining: looking at it
$A01 constant look-low-anim        \ ? bending forward to look low
$A02 constant look-close-anim      \ ? leaning in to look closely
$F00 constant startled-anim        \ recoiling, arms up
$F01 constant look-behind-anim     \ turning to look behind her
$F03 constant uneasy-anim          \ ? hands drawn up in front
$F04 constant uneasy-2-anim        \ ? the same, a hand to her face
$1002 constant stagger-anim        \ ? staggering
$1003 constant doubled-over-anim   \ ? doubled over, recovering
$1D00 constant bent-hand-mouth-anim   \ ? bent over, a hand to her mouth
$1D01 constant hand-to-mouth-anim  \ ? a hand to her mouth
$1D02 constant hands-clasped-anim  \ ? hands clasped in front, head bowed
