\ graphics.fs - the look of the picture: presets, and a settings menu (F1).
\ The settings are C's (render.h RenderSettings), reached through the gfx fields: flags and
\ counts are 32-bit (l@ l!), the rest floats (sf@ sf!).
IN: graphics
USING: engine strings state ;

\ ---- presets ----
<PRIVATE
: on  ( field -- ) gfx swap execute 1 swap l! ;
: off ( field -- ) gfx swap execute 0 swap l! ;
: int! ( n field -- ) gfx swap execute l! ;
: float! ( field -- ) ( F: x -- ) gfx swap execute sf! ;
PRIVATE>

\ as the PS2 showed it: no effects, a 4:3 picture
: look-original
    1 ['] gfx.msaa int!  1 ['] gfx.aspect int!
    ['] gfx.room-fog on  ['] gfx.room-tint on  ['] gfx.room-bloom on
    ['] gfx.ssao off  ['] gfx.bloom off  ['] gfx.fog off  ['] gfx.shadows off
    0 ['] gfx.tonemap int!  0e ['] gfx.vignette float!  0e ['] gfx.grain float!
    1e ['] gfx.saturation float!  1e ['] gfx.contrast float!  1e ['] gfx.exposure float! ;

\ the original's look, cleaned up and deepened: the default
: look-enhanced
    4 ['] gfx.msaa int!  0 ['] gfx.aspect int!
    ['] gfx.room-fog on  ['] gfx.room-tint on  ['] gfx.room-bloom on
    ['] gfx.ssao on  0.9e ['] gfx.ssao-strength float!  20e ['] gfx.ssao-radius float!
    ['] gfx.bloom on  0.9e ['] gfx.bloom-threshold float!  0.12e ['] gfx.bloom-strength float!
    ['] gfx.fog off  0.0012e ['] gfx.fog-density float!  150e ['] gfx.fog-start float!   \ (the rooms have their own)
    ['] gfx.shadows on
    1 ['] gfx.tonemap int!  0.35e ['] gfx.vignette float!  0.025e ['] gfx.grain float!
    1e ['] gfx.saturation float!  1e ['] gfx.contrast float!  1e ['] gfx.exposure float! ;

\ darker and moodier: film curve, heavier fog, glow, grain and vignette, colours drained
: look-cinematic
    look-enhanced
    2 ['] gfx.tonemap int!  1.25e ['] gfx.exposure float!
    0.6e ['] gfx.bloom-threshold float!  0.25e ['] gfx.bloom-strength float!
    ['] gfx.fog on  0.0025e ['] gfx.fog-density float!  80e ['] gfx.fog-start float!
    0.55e ['] gfx.vignette float!  0.045e ['] gfx.grain float!
    0.8e ['] gfx.saturation float!  1.08e ['] gfx.contrast float! ;

\ ---- the menu ----
\ An item is a record: label (address, length), kind, the field's word, and for numbers
\ min, max, step - in thousandths, so all of it fits in cells.

<PRIVATE
0 constant flag        \ on / off
1 constant count       \ min .. max by 1
2 constant doubling    \ 1 2 4 8 ...
3 constant number      \ a float, min .. max by step (thousandths)
4 constant choice      \ a count shown by name (step: the names)
5 constant action      \ runs a word (the field: its execution token)

7 constant item-cells
create items 48 item-cells * cells allot
variable #items

: item ( label-addr label-len field-xt kind min max step -- )
    #items @ item-cells * cells items + >r
    r@ 6 cells + !  r@ 5 cells + !  r@ 4 cells + !  r@ 3 cells + !  r@ 2 cells + !
    keep r@ cell+ !  r> !
    1 #items +! ;

: rec ( i -- addr ) item-cells * cells items + ;
: label ( rec -- addr len )  dup @ swap cell+ @ ;
: kind ( rec -- k ) 3 cells + @ ;
: slot ( rec -- x ) 2 cells + @ ;                 \ the field word (an action: its word)
: field ( rec -- addr ) slot gfx swap execute ;
: lo ( rec -- n ) 4 cells + @ ;
: hi ( rec -- n ) 5 cells + @ ;
: step ( rec -- n ) 6 cells + @ ;

variable tbl
\ names for a choice: `s" a" s" b" s" c" 3 names` gives a table: count, then (addr, len) pairs
: names ( a1 n1 .. ak nk k -- table )
    here tbl !  dup 2* 1+ cells allot  dup tbl @ !
    0 ?do  keep  tbl @ dup @ i - 1- 2* 1+ cells + 2!  loop  tbl @ ;
: name ( i table -- addr len )  swap 2* 1+ cells + 2@ ;

\ ---- the value as text ----
: thousandths ( n -- ) ( F: -- x ) s>f 1000e f/ ;
: value-text ( rec -- addr len )
    dup kind case
        flag of     field l@ if s" on" else s" off" then endof
        count of    field l@ n>s endof
        doubling of field l@ dup 1 = if drop s" off" else n>s then endof
        number of   dup field sf@  step 10 < if 4 else 2 then f>s$ endof
        choice of   dup field l@ swap step name endof
        action of   drop s" " endof
    endcase ;

\ ---- changing a value (dir: -1 or 1) ----
: clamp-count ( n rec -- n' ) dup lo swap hi clamp ;
: change ( dir rec -- )
    dup kind case
        flag of     nip field dup l@ 0= 1 and swap l! endof
        count of    tuck field l@ + over clamp-count swap field l! endof
        choice of   tuck field l@ + over clamp-count swap field l! endof
        doubling of swap 0> if  dup field l@ 2*  else  dup field l@ 2/  then
                    over clamp-count swap field l! endof
        number of   >r s>f r@ step thousandths f* r@ field sf@ f+
                    r@ lo thousandths r@ hi thousandths fclamp r> field sf! endof
        action of   nip slot execute endof
    endcase ;

\ ---- saving: the settings written out as Forth, read back at start-up ----
: settings-file ( -- addr len )
    str-reset  user-dir +str  s" graphics.fs" +str  str ;

: save-item ( rec -- )
    dup kind action = if  drop exit  then
    dup kind number = if  dup field sf@ 6 f>s$ type  else  dup field l@ n>s type  then
    ."  gfx " dup slot xt>name type
    kind number = if  ."  sf!"  else  ."  l!"  then  cr ;

defer save-graphics
: (save-graphics)
    settings-file to-file
    ." \ graphics settings, saved from the menu (F1)" cr
    #items @ 0 ?do  i rec save-item  loop
    end-file  ." saved " settings-file type cr ;
' (save-graphics) is save-graphics

PRIVATE>

\ ---- the items ----
s" Preset: as on the PS2"  ' look-original  action 0 0 0 item
s" Preset: enhanced"       ' look-enhanced  action 0 0 0 item
s" Preset: cinematic"      ' look-cinematic action 0 0 0 item
s" Picture"         ' gfx.aspect choice 0 1   s" fill the screen" s" 4:3" 2 names item
s" Antialiasing"    ' gfx.msaa doubling 1 8 0 item
s" Resolution scale" ' gfx.scale number 500 2000 250 item
s" Texture filter"  ' gfx.anisotropy number 1000 16000 1000 item
s" Room's own fog"   ' gfx.room-fog flag 0 1 0 item
s" Room's own tint"  ' gfx.room-tint flag 0 1 0 item
s" Room's own bloom" ' gfx.room-bloom flag 0 1 0 item
s" Ambient occlusion" ' gfx.ssao flag 0 1 0 item
s"   strength"      ' gfx.ssao-strength number 0 1000 100 item
s"   radius"        ' gfx.ssao-radius number 4000 60000 2000 item
s" Bloom"           ' gfx.bloom flag 0 1 0 item
s"   threshold"     ' gfx.bloom-threshold number 100 3000 100 item
s"   strength"      ' gfx.bloom-strength number 0 1000 20 item
s" Haze"            ' gfx.fog flag 0 1 0 item
s"   density"       ' gfx.fog-density number 0 10 1 item
s"   start"         ' gfx.fog-start number 0 1000000 25000 item
s" Contact shadows" ' gfx.shadows flag 0 1 0 item
s" Tone mapping"    ' gfx.tonemap choice 0 2   s" clip" s" soft" s" filmic" 3 names item
s" Exposure"        ' gfx.exposure number 250 4000 50 item
s" Saturation"      ' gfx.saturation number 0 2000 50 item
s" Contrast"        ' gfx.contrast number 500 2000 25 item
s" Vignette"        ' gfx.vignette number 0 1000 50 item
s" Film grain"      ' gfx.grain number 0 100 5 item
s" Rim light"       ' gfx.rim number 0 1500 50 item
s" Save as my default" ' save-graphics action 0 0 0 item
s" Show a buffer"   ' gfx.debug choice 0 4   s" no" s" occlusion" s" bloom" s" depth" s" bloom mask" 5 names item

\ ---- showing it ----
variable selected  0 selected !

: menu-keys
    menu-open @ 0= if  exit  then
    key: Down key-pressed? if  selected @ 1+ #items @ mod selected !  then
    key: Up key-pressed?   if  selected @ 1- #items @ + #items @ mod selected !  then
    key: Right key-pressed? key: Return key-pressed? or if  1 selected @ rec change  then
    key: Left key-pressed?  if  -1 selected @ rec change  then ;

: f1-key  key: F1 key-pressed? if  menu-open @ 0= menu-open !  then ;

variable row-h  variable col-w
: draw-menu
    menu-open @ 0= if  exit  then
    screen-size nip 900 >= if 3 else 2 then pen-scale
    char-size row-h ! col-w !
    $0A0C10D8 pen-color  16 16  col-w @ 48 *  #items @ 2 + row-h @ * 16 +  draw-rect
    $FFE070FF pen-color  s" Graphics   (arrows, F1 to close)" 28 24 draw-text
    #items @ 0 ?do
        i selected @ = if  $FFE070FF  else  $C8D0D8FF  then pen-color
        i rec label  28  i 2 + row-h @ * 16 +  draw-text
        i rec value-text  28 col-w @ 26 * +  i 2 + row-h @ * 16 +  draw-text
    loop ;

' f1-key on-tick
' menu-keys on-tick
' draw-menu on-draw

look-enhanced
: load-settings  settings-file file-exists? if  settings-file included  then ;
' load-settings catch drop
