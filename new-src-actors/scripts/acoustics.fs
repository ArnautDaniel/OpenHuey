\ acoustics.fs - noises and who hears them. One actor: it keeps the loudest noise of each
\ source made this frame, and at the frame's end tells each listener what it heard. The rules
\ and their sources in the original: docs/subsystems/acoustics.md.
IN: acoustics
USING: engine actors messages ;

state: acoustics-state
  #noise-sources cells field noise-louds      \ this frame's noise from each source (0: none)
  #noise-sources cells field noise-rooms
  #noise-sources cells field noise-tris
  #noise-sources cells field noise-doors
  cell field setting                    \ 0..3
  cell field listeners                  \ a list: id threshold source, three cells each
end-state

: nth-of ( s field-addr -- addr )  swap cells + ;
: forget-noises ( -- )
    #noise-sources 0 do  0 i noise-louds nth-of !  -1 i noise-rooms nth-of !  -1 i noise-tris nth-of !  -1 i noise-doors nth-of !  loop ;

\ ---- the listeners ----
: listener# ( id -- i | -1 )   \ its place in the list
    listeners @ length 0 ?do  listeners @ i swap nth over = if  drop i unloop exit  then  3 +loop  drop -1 ;
: drop-listener ( id -- )
    listener# dup 0< if  drop exit  then
    listeners @ length 3 - over ?do  listeners @ i 3 + swap nth  listeners @ i swap nth!  loop
    drop  3 0 do  listeners @ pop drop  loop ;

\ ---- hearing one noise (Character_Hearing's loop body). `exit-was` carries across the sources,
\ as the original's `exit` does (quirk: step 7) ----
variable who   variable src   variable base   variable exit-was
: n@ ( field-addr -- x )  src @ swap nth-of @ ;
: setting-loud ( -- loud )
    noise-louds n@  dup $80 < if
        setting @ case  1 of  $1F -  endof  2 of  $3F -  endof  3 of  $5F -  endof  endcase
    then ;
: none>ff ( exit -- exit' )  dup 0< if  drop $FF  then ;
\ in the listener's room, or at a door into it
: reaches? ( -- flag )
    who @ body-room noise-rooms n@ = if  true exit  then
    noise-doors n@ 0< if  false exit  then
    noise-doors n@ noise-rooms n@ door-exit-in none>ff dup exit-was !
    dup $FF = if  drop true exit  then
    noise-rooms n@ swap room-exit-leads drop  who @ body-room = ;
\ where it came from: its triangle's centre, else where its door stands, else the listener
fvariable sx  fvariable sy  fvariable sz
: source-at ( -- )
    noise-tris n@ dup 0< 0= if  tri-center sz f! sy f! sx f! exit  then  drop
    who @ body-pos sz f! sy f! sx f!
    noise-doors n@ 0< if  exit  then
    noise-doors n@ who @ body-room door-exit-in dup 0< if  drop exit  then
    exit-stand if  sz f! sy f! sx f!  then ;
: by-distance ( loud -- loud' )   \ a tenth of the level distance plus three times the rise
    source-at  who @ body-pos                                    ( F: x y z )
    sz f@ f- fsq  fswap sy f@ f- fabs 3e f*  frot sx f@ f- fsq  frot f+ fsqrt  f+
    0.1e f* f>s - ;
: route-end ( n -- door | -1 )  dup 0> if  1- route-door  else  drop -1  then ;
: by-route ( loud -- loud' )   \ through at most 2 noise-doors, for a walker (kind 1)
    who @ body-room noise-rooms n@ 1 2 route                           ( loud n )
    noise-doors n@ 0< 0=  over 0> and if
        dup route-end noise-rooms n@ door-exit-in none>ff  exit-was @ = if  1-  then
    then
    dup -1 = if  drop  base @ $60 < if  drop 0  then  exit  then
    base @ over 5 lshift < if  2drop 0 exit  then
    base @ $41 < if  route-end door-open? 0= if  drop 0  then  else  drop  then ;
: heard? ( threshold -- flag )   \ (base, src set)
    base @ reaches? if  who @ body-room room-id = if  by-distance  then  else  by-route  then
    < ;
: tell ( id threshold source -- )   \ the first source it hears, or nothing
    rot who !  0 exit-was !
    #noise-sources 0 do
        i over <> if
            i src !  setting-loud dup base !  0> if
                over heard? if
                    2drop  noise-louds n@ noise-rooms n@ noise-tris n@ noise-doors n@ src @  who @ send heard  unloop exit
                then
            then
        then
    loop  2drop  who @ send heard-nothing ;

behaviour hearing
  on spawned ( -- )  forget-noises  list listeners !  self subscribe frame-end ;
  on noise ( loud room tri door source -- )
      dup 0 #noise-sources within 0= if  2drop 2drop drop exit  then   >r
      3 pick 0=  3 pick -1 = or  4 pick $FF and r@ noise-louds nth-of @ < or if  r> drop 2drop 2drop exit  then
      dup 0< 0= if  nip -1 swap  then                        \ (at a door: no triangle)
      r@ noise-doors nth-of !  r@ noise-tris nth-of !  r@ noise-rooms nth-of !  $FF and r> noise-louds nth-of ! ;
  on listen ( threshold source -- )
      sender drop-listener  sender listeners @ push  swap listeners @ push  listeners @ push ;
  on stop-listening ( -- )  sender drop-listener ;
  on noise-setting ( n -- )  setting ! ;
  on frame-end ( -- )
      listeners @ length 0 ?do
          listeners @ i swap nth  dup body? if
              listeners @ i 1+ swap nth  listeners @ i 2 + swap nth  tell
          else  drop  then
      3 +loop
      forget-noises ;
end-behaviour

: acoustics-spawn ( -- id )  hearing acoustics-state s" acoustics" spawn ;
