\ hewie/anims.fs - Hewie's animations by name (the motion's id in his banks: its set in the high
\ byte). Named for what he does in them; the original's states that play them in hewie.c. His
\ overlays' (head, ears, tail) are in hewie/mind.fs.
IN: hewie.anims

\ ---- still ----
0 constant stand-anim
1 constant sit-anim
2 constant lie-anim
3 constant alert-anim            \ standing, alert
4 constant stance-anim           \ the stance: tense, angry
5 constant stance-upset-anim     \ ... mistreated
6 constant stand-hurt-anim
7 constant lie-hurt-anim
8 constant lie-low-anim
9 constant crouch-anim           \ low, readying to spring
\ ---- between the poses ----
$100 constant sit-down-anim
$101 constant up-from-sitting-anim
$102 constant lie-down-anim
$103 constant up-from-lying-anim
$104 constant lie-from-sitting-anim
$105 constant sit-from-lying-anim
$106 constant get-low-anim
$107 constant up-from-low-anim
\ ---- on the move ----
$200 constant walk-anim
$201 constant trot-anim
$202 constant run-anim
$203 constant on-side-anim       \ lying on his side
$204 constant dart-anim          \ darting about
$205 constant walk-wary-anim     \ walking while followed
$206 constant limp-anim
$300 constant roll-on-side-anim
$301 constant roll-off-side-anim
\ ---- struck, down ----
$1000 constant yelp-anim
$1001 constant go-down-anim
$1002 constant down-anim
$1003 constant get-up-anim
\ ---- turning, a scene's pose ----
$1300 constant turn-on-spot-anim
$1301 constant scene-pose-anim
\ ---- barking (Hewie_Bark: by his pose when calm, else by his mood) ----
$1B00 constant bark-anim
$1B01 constant bark-sitting-anim
$1B02 constant bark-lying-anim
$1B03 constant bark-tense-anim
$1B04 constant bark-upset-anim
$1B05 constant bark-limping-anim
$1B06 constant barks-end         \ (past the last of them)
\ ---- his tricks, his ears, her praise ----
$1C00 constant sit-trick-2-anim
$1C01 constant refuse-anim       \ sitting, his head turned away
$1C02 constant ears-up-anim      \ stopped, his ears pricked
$1C03 constant sit-a-while-anim  \ (the story's)
$1C04 constant ears-down-anim    \ sitting, his ears down (scolded)
$1C05 constant patted-anim
$1C06 constant sit-trick-anim
$1C07 constant sit-trick-lying-anim
$1C08 constant stand-trick-anim
$1C09 constant stand-trick-2-anim
$1D00 constant petted-start-anim
$1D01 constant petted-anim
$1D02 constant petted-end-anim
\ ---- leaps ----
$1E01 constant tackle-anim
$1E03 constant land-anim
$1E04 constant leap-up-anim      \ (the story's leap)
$1E05 constant leap-fall-anim
$1E06 constant leap-land-anim
\ ---- his fight's animations: the leap, the bite, letting go - at the near flank, the far one,
\ head on, his leg; thrown off at a flank (hanging on), held on again or flung, at the top of
\ the arc and falling; landing ----
$2200 constant leap-near-anim   $2209 constant leap-far-anim   $2214 constant leap-head-on-anim   $2217 constant leap-leg-anim
$2201 constant bite-near-anim   $220A constant bite-far-anim   $2215 constant bite-head-on-anim   $2218 constant bite-leg-anim
$2202 constant let-go-near-anim   $220B constant let-go-far-anim   $2216 constant let-go-head-on-anim   $2219 constant let-go-leg-anim
$2203 constant hanging-near-anim   $220C constant hanging-far-anim
$2204 constant hold-again-near-anim   $220D constant hold-again-far-anim
$2207 constant flung-near-anim   $2210 constant flung-far-anim
$2205 constant top-held-near-anim   $220E constant top-held-far-anim
$2208 constant top-flung-near-anim   $2211 constant top-flung-far-anim
$2206 constant falling-held-near-anim   $220F constant falling-held-far-anim
$2212 constant land-held-anim   $2213 constant flung-anim   \ (landed: held on; flung - struck down too)
