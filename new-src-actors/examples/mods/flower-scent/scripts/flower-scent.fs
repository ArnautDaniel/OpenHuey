\ flower-scent: the Fresh Flowers, used anywhere the game has no use for them, calm Fiona a
\ little (her fear down 10) - and stay hers, still fresh for the vase.
\
\ The sample of a hook: the game asks `item-use-rule` what using an item does. The mod sets its
\ own rule, keeps the game's (`action-of`) and asks it first: only when the game's answer is
\ "nothing" does the mod act - by the game's message to Fiona (remedy), not by touching her.
IN: flower-scent
USING: engine actors messages items items.names ;

action-of item-use-rule constant game-rule
: fiona ( -- id )  s" fiona" actor-named ;
: scent ( place -- result )
    dup game-rule execute  dup no-use <> if  nip exit  then  drop   \ (the game's use, where it has one)
    id@ fresh-flowers = if  -10 0 fiona send remedy  done-quietly  else  no-use  then ;
' scent is item-use-rule
