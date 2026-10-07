\ fiona.fs - Fiona in the game: her model's motion table and size, and her own frame each tick
\ (fiona.brain - the original's, src/game/fiona.c) in place of player.fs's walking.
\   W A S D  the stick (camera relative)     Shift  run (the cross button)
IN: fiona
USING: engine state game-state events.core chars relations player fiona.core fiona.moves fiona.brain fiona.commands fiona.doors fiona.panic fiona.kick fiona.ladder fiona.react fiona.grab ;

variable fiona-ready  -1 fiona-ready !   \ the model she was set up with
: prepare ( -- )
    fiona @ dup fiona-ready @ = if  drop exit  then  dup fiona-ready !
    dup 0< if  drop exit  then
    dup 0 character char.actor l!
    $3D5CC0 motion-table                  \ (CharModel_SecondaryMotion: her fades and flags)
    me 2e 15e c-size!                     \ (Fiona_Reset: radius 2, height 15)
    f-reset-fields  me c-find-tri me c-tri!  to-idle ;
: fiona-control ( -- )
    relation-changes resolve   \ (the progress' relations, each frame before the characters)
    panic-update   \ (the panic's frame: SceneGame's, before the characters')
    prepare  fiona-frame
    f-cmd @ partner.core:fiona-cmd ! ;   \ (what Hewie sees of her controls)
' fiona-control is control-fiona
\ (the camera-cut rule of her controls: the director's setup changed)
' cam-changed? is camera-cut?
