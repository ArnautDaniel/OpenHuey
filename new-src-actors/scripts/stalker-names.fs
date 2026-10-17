\ stalker-names.fs - the stalkers' actors by name: one list for everyone who looks for "the
\ stalker" (one is in the game at a time - the story's character slot 2)
IN: stalker-names
USING: engine actors ;

\ the stalkers by kind (their character's +0x153C: the cutscenes' cast ids): Debilitas (and his
\ figures in the scenes), Daniella, Riccardo, Lorenzo's other class, Lorenzo, kind 12, the
\ TintStalker, Daniella's later kinds, Riccardo's other class, Lorenzo's other class again
enum: stalker-kind#  2 debilitas-kind daniella-kind riccardo-kind 6 debilitas-6-kind debilitas-7-kind
    10 lorenzo2-kind lorenzo-kind kind12-kind 23 tintstalker-kind 27 debilitas-27-kind
    34 kind34-kind kind35-kind kind36-kind kind37-kind 39 kind39-kind ;
12 constant #stalkers
strings: stalker-name  debilitas daniella riccardo lorenzo lorenzo2 kind39 kind37 tintstalker kind12
    kind34 kind35 kind36 ;
: stalker# ( i -- id | -1 )   \ the i-th stalker's actor (-1 not spawned)
    stalker-name dup if  actor-named  else  2drop -1  then ;
: stalker-in-game ( -- id | -1 )   \ the one with a body (in the game)
    #stalkers 0 do  i stalker# dup 0< 0= if  dup body? if  unloop exit  then  then  drop  loop  -1 ;
