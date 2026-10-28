\ stalker/anims.fs - the stalkers' animations by name: the family's (each stalker's motion table
\ has them; Pursuer_PlayAnim and the states that play them). Each one's own (Debilitas's turn and
\ lunge, Riccardo's gun, Lorenzo's sinking) are in their files.
IN: stalker.anims

\ ---- standing (vtable +0x320 Pursuer_WalkAnim) ----
0 constant stand-anim
1 constant stand-alt-anim         \ (Riccardo's)
2 constant stand-hurt-anim
3 constant stand-rage-anim
\ ---- walking (vtable +0x324 / +0x328) ----
$200 constant walk-anim
$201 constant walk-slow-anim      \ searching, heading for her room
$202 constant walk-hurt-anim
$203 constant walk-slow-hurt-anim \ (also held off)
$204 constant walk-rage-anim
$205 constant walk-slow-rage-anim
$206 constant walk-after-anim     \ after her, slowly
\ ---- turning on the spot (Pursuer_TurnOnSpot: + the way - left, right, round left, round right; hurt +5) ----
$400 constant turn-anims
$405 constant turn-hurt-anims
$404 constant step-back-anim
\ ---- a door (+ how he uses it: 0 from behind it, 2 in front) ----
$600 constant door-anims
\ ---- a ladder ----
$700 constant on-top-anim
$701 constant rung-down-anim   $702 constant rung-down-2-anim
$703 constant off-foot-anim
$704 constant on-foot-anim
$705 constant rung-up-anim     $706 constant rung-up-2-anim
$707 constant off-top-anim
\ ---- struck (Pursuer_StateFlinch: in front, behind, low; at a door) ----
$1000 constant flinch-behind-anim
$1001 constant flinch-front-anim
$1002 constant flinch-door-anim
$1003 constant flinch-low-behind-anim
$1004 constant flinch-low-anim     \ (also knocked through a door, stunned)
$1005 constant flinch-door-low-anim   \ (also staggered back from a door)
$1006 constant powdered-anim       \ his hands to his face
\ ---- searching (Pursuer_StateWalkGesture) ----
$1302 constant look-about-anim   $1303 constant look-about-2-anim   $1305 constant look-about-3-anim
$1A00 constant glance-round-anim     \ closing on her and she out of reach: a look round (Pursuer_StateLookAround)
\ ---- standing about ($1600 ..) ----
$1600 constant stand-about-anims
$1601 constant there-anim          \ seeing her anew
$1602 constant taunt-anim
\ ---- bitten by Hewie (+1 back on his feet, +2 throwing him off) ----
$1700 constant bitten-arm-anim   $1703 constant bitten-flank-anim   $1706 constant bitten-head-on-anim
$1708 constant bitten-leg-anim
$1709 constant leg-fall-anim       \ Hewie let go of his leg: down on his front
\ ---- down (the fall, then lying: -1 a twitch, +1 up) ----
$1800 constant fall-back-anim   $1802 constant lying-back-anim
$1804 constant fall-front-anim  $1806 constant lying-front-anim
\ ---- taking her ----
$1900 constant hold-hand-anim
$1901 constant taunt-hold-anim
$1903 constant let-go-hand-anim
$1904 constant hold-on-anim       \ (the sixth: she is dragged off)
$1A01 constant carry-anim
