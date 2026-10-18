\ stalker/names.fs - a stalker's actions, his tables' situations and where his head looks, by
\ name (the original's numbers: kPursuerSteps' entries, Pursuer_PickFromTable's situations).
IN: stalker.names

\ his actions (chase-act; his tables' rows name them too): standing; a gesture; turned on the
\ spot to her; to where she is; walking at her; stalking her; setting off on his way; a door
\ opened to get at her; to a ladder; a combo; taking her by the hand, walking her off; a look
\ about; his taunts; round her; a step back; flinching; down; hurt; struck at a door; a door
\ ahead; bitten; a special animation (powdered). $1000 on: his own (own-acts)
enum: pursuer-act  1 watch-act gesture-act turn-act after-her-act walk-at-act stalk-act
    8 set-off-act 9 act-9 $B door-for-her-act $D act-d $12 ladder-act combo-act take-by-hand-act
    take-walking-act look-around-act taunt-act taunt-2-act taunt-3-act round-act round-2-act
    $1D back-off-act flinch-act knocked-down-act hurt-act hit-at-door-act door-ahead-act bitten-act
    special-anim-act
    $1000 own-act-1 own-act-2 own-act-3 own-act-4 ;
\ his tables' situations (Pursuer_PickFromTable): at her by how she stands (her plight, 0..5:
\ the attack tables); his attack before the panic's height, at it; striking back in front,
\ behind; a lunge; at Hewie; after a combo that struck her, that missed
enum: situation  6 attack-calm attack-panic strike-back-front strike-back-behind lunge-situation
    at-hewie-situation struck-her missed-her ;
\ where his head looks (an action's: kPursuerSteps +0x19): as it was; at her; at Hewie; at his goal; ahead
enum: head-look  as-was-look her-look hewie-look goal-look ahead-look ;
