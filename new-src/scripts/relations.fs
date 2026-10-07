\ relations.fs - the characters' dealings with one another, as the original's progress keeps
\ them (src/game/progress.c): the three character slots' commands to each other (+0x10B0: a
\ joint action one asks of another - Fiona asking Hewie to sit by her, Hewie asking her to
\ turn - started or cancelled by the one asked), and their requests (+0x1014: one asking for
\ others - a stalker for Fiona - each taken by one asker and accepted or not by the one asked,
\ its vtable +0x68). Each frame (SceneGame's play): the changes, then the requests resolved.
\
\ A character's request block (+0x14E8, char.req: 8 words) is what it is asked to do; +0x1508
\ (char.req2) the one kept for when a joint action starts.
IN: relations
USING: engine game-state events.core chars ;

\ ---- the request blocks ----
: req# ( cs -- addr )  character char.req ;
: req2# ( cs -- addr )  character char.req2 ;
\ char_set_action: [0] state, [1] a, [2] b, [3] c, [4] d, [5] e (float), [6] f, [7]
: w# ( addr i -- addr' )  4 * + ;   \ (the blocks' words are 32-bit)
: req-set ( state a b c d cs -- ) ( F: e -- )
    req# >r  r@ 4 w# l!  r@ 3 w# l!  r@ 2 w# l!  r@ 1 w# l!  r@ l!
    r@ 5 w# sf!  0 r@ 6 w# l!  0 r> 7 w# l! ;
: req-copy ( from to -- )  8 0 do  over i w# l@  over i w# l!  loop  2drop ;
: req-of ( cs -- n )  req# sl@ ;
: req-word-of ( cs i -- n )  swap req# swap w# sl@ ;

\ ---- the commands (+0x10B0: kind, sub (1 started, 2 cancelled), the other's slot, arg, n, f) ----
create cmds 3 6 * cells allot
: cmd# ( k -- addr )  6 cells * cmds + ;
: cmd-reset ( k -- )  cmd# >r  0 r@ !  0 r@ cell+ !  $FF r@ 2 cells + !  $FF r@ 3 cells + !
    0 r@ 4 cells + !  0 r> 5 cells + ! ;
: cmds-reset ( -- )  3 0 do  i cmd-reset  loop ;
cmds-reset
: cmd-kind ( k -- n )  cmd# @ ;               \ SlotCmd_Kind (0: none)
: cmd-sub ( k -- n )  cmd# cell+ @ ;
: cmd-other ( k -- n )  cmd# 2 cells + @ ;    \ SlotCmd_Target
: cmd-arg ( k -- n )  cmd# 3 cells + @ ;      \ SlotCmd_Arg
: cmd? ( k -- flag )  cmd-kind 0<> ;          \ Progress_HasRelationCmd
: cmd-start ( k -- )  1 swap cmd# cell+ ! ;   \ SlotCmd_Start
: cmd-cancel ( k -- )  2 swap cmd# cell+ ! ;  \ SlotCmd_Cancel
\ cmd_busy: it has a command, or is one's other
: cmd-busy? ( k -- flag )
    dup cmd? if  drop true exit  then
    3 0 do  i cmd-other over = if  drop true unloop exit  then  loop  drop false ;
\ SlotCmd_Give: slot `slot` asked to do `kind` (arg, n) for `other`, unless either is busy
<PRIVATE
variable cg-k  variable cg-a  variable cg-o  variable cg-s  variable cg-n
PRIVATE>
: cmd-give ( kind arg other slot n -- flag )
    cg-n !  cg-s !  cg-o !  cg-a !  cg-k !
    cg-o @ cmd-busy? cg-s @ cmd-busy? or if  false exit  then
    cg-s @ cmd# >r  cg-k @ r@ !  0 r@ cell+ !  cg-o @ r@ 2 cells + !  cg-a @ r@ 3 cells + !
    cg-n @ r@ 4 cells + !  0 r> 5 cells + !  true ;
\ Progress_IsLinked: k has a command, or is one's other
: linked? ( k -- flag )  cmd-busy? ;

\ ---- the requests (+0x1014: whom (a mask), kind, a, b, f; accepted by (a mask)) ----
create asks 3 6 * cells allot
: ask# ( k -- addr )  6 cells * asks + ;
: ask-reset ( k -- )  ask# 6 cells 0 fill ;
: asks-reset ( -- )  3 0 do  i ask-reset  loop ;
asks-reset
\ a request from slot k (Relation_Request): of the characters in `mask`, kind, a, b, f (who
\ accepted the last one, +0xC, stays until the next resolve)
: ask ( mask kind a b k -- ) ( F: f -- )
    ask# >r  r@ 3 cells + !  r@ 2 cells + !  r@ cell+ !  r@ !  r> 4 cells + sf! ;
: ask-accepted ( k -- mask )  ask# 5 cells + @ ;
: asks-clear ( -- )  3 0 do  i ask# 5 cells 0 fill  loop ;   \ (the requests, not who took them)

\ Character_Held: asked to react (4) or let go (5), or in a command
: held? ( cs -- flag )
    dup 3 < 0= if  drop false exit  then
    dup req-of dup 4 = swap 5 = or if  drop true exit  then  linked? ;

\ a character's answer to a request (its vtable +0x68: kind, the asker, b): each character's own
\ (vtable +0x68: each slot's own word, set by its character's code; none: no)
create accepters  ' false , ' false , ' false ,
: accepts! ( xt cs -- )  cells accepters + ! ;
:noname 2drop 2drop false ;  dup 0 accepts!  dup 1 accepts!  2 accepts!
: accepts? ( cs kind asker b -- flag )  3 pick cells accepters + @ execute ;
: in-play? ( cs -- flag )  dup c-ok? 0= if  drop false exit  then  character char.present sl@ 0<> ;
variable no-hewie   \ (progress +0xC bit 0x2000: Hewie takes no requests)

\ Progress_SlotAfresh: k starts afresh - its own command marked cancelled, the others' with it
\ and the requests involving it reset
: afresh ( k -- )
    3 0 do
        i over = if  i cmd-cancel  else  i cmd-other over = if  i cmd-reset  then  then
    loop
    1 over lshift  3 0 do  i 2 pick = over i ask# @ and or if  i ask-reset  then  loop  2drop ;

\ ---- Progress_RelationChanges: a cancelled command holds the other (request 7); a started
\ kind 1 becomes a request (kind 10) on the other; a started kind 2 lets its own go with the
\ request it kept ----
<PRIVATE
: hold ( cs -- )  dup req-of 7 = if  drop exit  then  >r 7 0 0 0 0 0e r> req-set ;
PRIVATE>
: relation-changes ( -- )
    3 0 do
        i cmd-kind 2 = i cmd-sub 1 = and if
            i req2# sl@ 0<> i req-of 7 <> and if  i req2# i req# req-copy  then
            i cmd-reset
        else i cmd-kind 0<> i cmd-sub 2 = and if
            i cmd-other dup 3 < if  hold  else  drop  then  i cmd-reset
        else i cmd-kind 1 = i cmd-sub 1 = and if
            1 i lshift  10  i cmd# 4 cells + @  i cmd-arg  i cmd-other  i cmd# 5 cells + sf@ ask
            i cmd-reset
        then then then
    loop ;

\ ---- Progress_ResolveRelations: each asked character goes to one asker (the last slot first;
\ kind 6 after the others; one already asking is not taken; one in a corner (+0x2D) not but
\ for kind 5; Hewie not while he takes none); asked, those accepting are marked, and take
\ request 4 (kind, the asker, a, b, f) - or for kind 9 a command for both; askers of kind
\ 9 / 10 nobody took are held. All requests cleared. ----
\ a character in a corner of its own (the actor's +0x2D: its own code says)
defer corner? ( cs -- flag )   :noname drop false ; is corner?
<PRIVATE
create owner 3 cells allot
: owner@ ( k -- j )  cells owner + @ ;
: owner! ( j k -- )  cells owner + ! ;
variable taken
: taken? ( k -- flag )  1 swap lshift taken @ and 0<> ;
: take ( k -- )  1 swap lshift taken @ or taken ! ;
: claim ( j six? -- )   \ asker j claims the characters it asks for (its kind 6 or not)
    over ask# @ 0= if  2drop exit  then
    over ask# cell+ @ 6 = <> if  drop exit  then
    3 0 do
        dup ask# @ 1 i lshift and 0<>  i in-play? and  i taken? 0= and
        i 1 = no-hewie @ and 0= and if
            dup ask# cell+ @ 5 = if  i character char.scripted sl@ 0<> i corner? and 0=
            else  i corner? 0=  then
            if  dup i owner!  i take  then
        then
    loop  drop ;
variable rs-k  variable rs-r  variable rs-c  variable rs-o
: rs-word ( i -- n )  cells rs-r @ + @ ;
PRIVATE>
: resolve ( -- )
    3 0 do  $FF i owner!  0 i ask# 5 cells + !  loop  0 taken !
    3 0 do  2 i -  dup in-play? over taken? 0= and if  false claim  else  drop  then  loop
    3 0 do  2 i -  dup in-play? over taken? 0= and if  true claim  else  drop  then  loop
    3 0 do
        i taken? if
            i rs-k !  i owner@ ask# rs-r !
            i held?  i 1 rs-word i owner@ 3 rs-word accepts? 0= or if
                $FF i owner!
            else
                5 rs-word 1 i lshift or rs-r @ 5 cells + !
                1 rs-word 9 = if
                    i owner@ cmd-busy? i cmd-busy? or if  $FF i owner!
                    else
                        i cmd# rs-c !  i owner@ rs-o !   \ (no >r here: `i` reads the return stack)
                        1 rs-c @ !  0 rs-c @ cell+ !  rs-o @ rs-c @ 2 cells + !
                        3 rs-word rs-c @ 3 cells + !  2 rs-word rs-c @ 4 cells + !  4 rs-word rs-c @ 5 cells + !
                    then
                else
                    i req-of 7 <> if
                        rs-r @ 4 cells + sf@  4  1 rs-word  i owner@  2 rs-word  3 rs-word  i req-set
                    then
                then
            then
        then
    loop
    3 0 do
        i ask# cell+ @ dup 9 = swap 10 = or if
            true  3 0 do  i owner@ j = if  drop false  then  loop
            i req-of 7 <> and if  0e  7 i ask# cell+ @ i 0 0 i req-set  then
        then
    loop
    asks-clear ;
