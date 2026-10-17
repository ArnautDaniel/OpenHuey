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
: stalker# ( i -- id | -1 )   \ the i-th stalker's actor (-1 not spawned)
    case
        0 of  s" debilitas"  endof    1 of  s" daniella"  endof     2 of  s" riccardo"  endof
        3 of  s" lorenzo"  endof      4 of  s" lorenzo2"  endof     5 of  s" kind39"  endof
        6 of  s" kind37"  endof       7 of  s" tintstalker"  endof  8 of  s" kind12"  endof
        9 of  s" kind34"  endof       10 of  s" kind35"  endof      11 of  s" kind36"  endof
        >r s" none" r>
    endcase  actor-named ;
: stalker-in-game ( -- id | -1 )   \ the one with a body (in the game)
    #stalkers 0 do  i stalker# dup 0< 0= if  dup body? if  unloop exit  then  then  drop  loop  -1 ;
