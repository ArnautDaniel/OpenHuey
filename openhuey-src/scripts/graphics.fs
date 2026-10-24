\ graphics.fs - the settings menu (F1; brought over from new-src): the look of the picture, its
\ presets, fullscreen, and the controls. The picture's settings are C's (render.h RenderSettings),
\ reached through the gfx fields: flags and counts are 32-bit (l@ l!), the rest floats (sf@ sf!);
\ saved to graphics.fs in the player's directory when asked, read back at start-up. The controls
\ (platform.h: the keys bound to each of the game's actions) and fullscreen are saved to
\ controls.fs there as they change.
IN: graphics
USING: engine ;

variable menu-open  0 menu-open !

<PRIVATE
\ one string built at a time:  str-reset  s" a" +str  s" b" +str  str ( -- addr len )
create (str) 1024 allot
variable (str-len)
: str-reset  0 (str-len) ! ;
: +str ( addr len -- )  dup >r  (str) (str-len) @ +  swap move  r> (str-len) +! ;
: str ( -- addr len )  (str) (str-len) @ ;
\ a string copied into the dictionary, to last
: keep ( addr len -- addr' len )  dup >r here dup >r swap move r> r> dup allot align ;
PRIVATE>

\ ---- presets ----
<PRIVATE
: on  ( field -- ) gfx swap execute 1 swap l! ;
: off ( field -- ) gfx swap execute 0 swap l! ;
: int! ( n field -- ) gfx swap execute l! ;
: float! ( field -- ) ( F: x -- ) gfx swap execute sf! ;
PRIVATE>

\ as the PS2 showed it: a 4:3 picture, the room's own fog, tint, bloom and depth of
\ field (the original's), the characters lit as the game lit them (src/game/model.c light_rgba: no
\ boost, no rim) - and none of OpenHuey's own effects
: look-original
    1 ['] gfx.msaa int!  1 ['] gfx.aspect int!
    ['] gfx.room-lights on  ['] gfx.shadow-maps on   \ (the game lit and shadowed its characters too)
    1 ['] gfx.ps2-lighting int!  1e ['] gfx.character-light float!  0e ['] gfx.rim float!
    ['] gfx.room-fog on  ['] gfx.room-tint on  ['] gfx.room-bloom on  ['] gfx.room-dof on
    ['] gfx.ssao off  ['] gfx.bloom off  ['] gfx.fog off  ['] gfx.shadows off
    0 ['] gfx.tonemap int!  0e ['] gfx.vignette float!  0e ['] gfx.grain float!
    1e ['] gfx.saturation float!  1e ['] gfx.contrast float!  1e ['] gfx.exposure float! ;

\ the original's look with modern polish, the default: antialiased, the wide picture, the characters
\ lit by the PS2's formula per pixel (smooth, but as bright against the room as the game had them -
\ OpenHuey's own point lights at twice the strength lit them brighter than the room, chests blown
\ white), a faint rim only as lit as they are (in the dark, no glowing outline), ambient occlusion,
\ a soft glow on what is past white, highlights eased, a light vignette (0.2: the old 0.35 sank the
\ corners of rooms dark already). No haze: the rooms have their own fog
: look-enhanced
    4 ['] gfx.msaa int!  0 ['] gfx.aspect int!
    ['] gfx.room-lights on  ['] gfx.shadow-maps on
    2 ['] gfx.ps2-lighting int!  1e ['] gfx.character-light float!  0.2e ['] gfx.rim float!
    ['] gfx.room-fog on  ['] gfx.room-tint on  ['] gfx.room-bloom on  ['] gfx.room-dof on
    ['] gfx.ssao on  0.9e ['] gfx.ssao-strength float!  20e ['] gfx.ssao-radius float!
    ['] gfx.bloom on  0.9e ['] gfx.bloom-threshold float!  0.12e ['] gfx.bloom-strength float!
    ['] gfx.fog off  0.0012e ['] gfx.fog-density float!  150e ['] gfx.fog-start float!   \ (the rooms have their own)
    ['] gfx.shadows on
    1 ['] gfx.tonemap int!  0.2e ['] gfx.vignette float!  0e ['] gfx.grain float!   \ (no grain: in a game this dark it reads as dither)
    1e ['] gfx.saturation float!  1e ['] gfx.contrast float!  1e ['] gfx.exposure float! ;

\ ---- the menu ----
\ An item is a record: label (address, length), kind, the field's word, and for numbers
\ min, max, step - in thousandths, so all of it fits in cells.

<PRIVATE
\ the kinds: on / off; a count, min .. max by 1; doubling, 1 2 4 8 ...; a float, min .. max by
\ step (thousandths); a count shown by name (step: the names); runs a word (the field: its word);
\ a control (the field: the action's number)
enum: item-kind  flag count doubling number choice action control ;

begin-structure /item
  field: i.label  field: i.len  field: i.field  field: i.kind  field: i.lo  field: i.hi  field: i.step
end-structure
72 constant most-items
create items most-items /item * allot
variable #items

: rec ( i -- addr )  /item * items + ;
: item ( label-addr label-len field-xt kind min max step -- )
    {: a n xt k lo hi st | r :}  #items @ rec to r
    a n keep r i.len !  r i.label !  xt r i.field !  k r i.kind !  lo r i.lo !  hi r i.hi !  st r i.step !
    1 #items +! ;

: label ( rec -- addr len )  dup i.label @ swap i.len @ ;
: kind ( rec -- k )  i.kind @ ;
: slot ( rec -- x )  i.field @ ;                 \ the field word (an action: its word)
: field ( rec -- addr ) slot gfx swap execute ;
: lo ( rec -- n )  i.lo @ ;
: hi ( rec -- n )  i.hi @ ;
: step ( rec -- n )  i.step @ ;

variable tbl
\ names for a choice: `s" a" s" b" s" c" 3 names` gives a table: count, then (addr, len) pairs
: names ( a1 n1 .. ak nk k -- table )
    here tbl !  dup 2* 1+ cells allot  dup tbl @ !
    0 ?do  keep  tbl @ dup @ i - 1- 2* 1+ cells + 2!  loop  tbl @ ;
: name ( i table -- addr len )  swap 2* 1+ cells + 2@ ;

\ ---- a control's keys as text: "X / Space"; waiting for one, says so ----
create keys-text 80 allot   variable keys-n
: +keys ( addr len -- )  80 keys-n @ - min  keys-text keys-n @ + swap dup keys-n +! move ;
: control-text ( i -- addr len )
    dup control-waiting = if  drop s" press a key (Escape: keep it)"  exit  then
    0 keys-n !  dup 0 control-binding key-name +keys
    1 control-binding ?dup if  s"  / " +keys  key-name +keys  then
    keys-text keys-n @ ;

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
        control of  slot control-text endof
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
        control of  swap 0> if  slot control-bind-next  else  0 swap slot 1 control-bind  then endof   \ (right: a new key; left: its second off)
    endcase ;

\ ---- saving: the settings written out as Forth, read back at start-up ----
: settings-file ( -- addr len )
    str-reset  user-dir +str  s" graphics.fs" +str  str ;

: save-item ( rec -- )
    dup kind action =  over kind control = or if  drop exit  then
    dup kind number = if  dup field sf@ 6 f>s$ type  else  dup field l@ n>s type  then
    ."  gfx " dup slot xt>name type
    kind number = if  ."  sf!"  else  ."  l!"  then  cr ;

\ the controls and fullscreen, as they change (not in a test: nothing kept)
: controls-file ( -- addr len )
    str-reset  user-dir +str  s" controls.fs" +str  str ;
: save-controls ( -- )
    controls-file to-file
    ." \ the controls and fullscreen, saved as they change (the settings menu, F1)" cr
    ." USING: engine ;" cr
    ." controls-reset" cr
    controls 0 ?do
        i 0 control-binding n>s type space  i n>s type  ."  0 control-bind" cr
        i 1 control-binding n>s type space  i n>s type  ."  1 control-bind" cr
    loop
    gfx gfx.fullscreen l@ n>s type  ."  gfx gfx.fullscreen l!" cr
    end-file ;
\ what was last saved: each action's two keys, then fullscreen
create controls-kept 64 cells allot
: control-now ( i -- x )   \ (i: action i/2's key i mod 2; the last, fullscreen)
    dup controls 2* = if  drop gfx gfx.fullscreen l@ exit  then  2 /mod swap control-binding ;
: controls-changed? ( -- flag )
    controls 2* 1+ 0 ?do  i control-now  controls-kept i cells + @ <> if  true unloop exit  then  loop  false ;
: keep-controls ( -- )  controls 2* 1+ 0 ?do  i control-now  controls-kept i cells + !  loop ;
: watch-controls ( -- )
    control-waiting 0< controls-changed? and if  keep-controls  testing? 0= if  save-controls  then  then ;

defer save-graphics
: (save-graphics)
    settings-file to-file
    ." \ graphics settings, saved from the menu (F1)" cr
    ." USING: engine ;" cr   \ (a file read starts with only the core's words)
    #items @ 0 ?do  i rec save-item  loop
    end-file  ." saved " settings-file type cr ;
' (save-graphics) is save-graphics

PRIVATE>

\ ---- the items ----
s" Preset: as on the PS2"  ' look-original  action 0 0 0 item
s" Preset: enhanced"       ' look-enhanced  action 0 0 0 item
s" Fullscreen (F11)" ' gfx.fullscreen flag 0 1 0 item
s" Picture"         ' gfx.aspect choice 0 1   s" fill the screen" s" 4:3" 2 names item
s" Antialiasing"    ' gfx.msaa doubling 1 8 0 item
s" Resolution scale" ' gfx.scale number 500 2000 250 item
s" Texture filter"  ' gfx.anisotropy number 1000 16000 1000 item
s" Room's own fog"   ' gfx.room-fog flag 0 1 0 item
s" Room's own tint"  ' gfx.room-tint flag 0 1 0 item
s" Room's own bloom" ' gfx.room-bloom flag 0 1 0 item
s" The game's own font" ' gfx.game-font flag 0 1 0 item
s" Room's depth of field" ' gfx.room-dof flag 0 1 0 item
s" Ambient occlusion" ' gfx.ssao flag 0 1 0 item
s"   strength"      ' gfx.ssao-strength number 0 1000 100 item
s"   radius"        ' gfx.ssao-radius number 4000 60000 2000 item
s" Bloom"           ' gfx.bloom flag 0 1 0 item
s"   threshold"     ' gfx.bloom-threshold number 100 3000 100 item
s"   strength"      ' gfx.bloom-strength number 0 1000 20 item
s" Haze"            ' gfx.fog flag 0 1 0 item
s"   density"       ' gfx.fog-density number 0 10 1 item
s"   start"         ' gfx.fog-start number 0 1000000 25000 item
s" Characters lit by the room" ' gfx.room-lights flag 0 1 0 item
s"   how"           ' gfx.ps2-lighting choice 0 2   s" OpenHuey's" s" as the PS2" s" as the PS2, smooth" 3 names item
s"   strength"      ' gfx.character-light number 500 4000 100 item
s" Shadows from room lights" ' gfx.shadow-maps flag 0 1 0 item
s"   strength"      ' gfx.shadow-strength number 0 1000 50 item
s" Contact blobs"   ' gfx.shadows flag 0 1 0 item
s" Tone mapping"    ' gfx.tonemap choice 0 2   s" clip" s" soft" s" filmic" 3 names item
s" Exposure"        ' gfx.exposure number 250 4000 50 item
s" Saturation"      ' gfx.saturation number 0 2000 50 item
s" Contrast"        ' gfx.contrast number 500 2000 25 item
s" Vignette"        ' gfx.vignette number 0 1000 50 item
s" Film grain"      ' gfx.grain number 0 100 5 item
s" Rim light"       ' gfx.rim number 0 1500 50 item
s" Save as my default" ' save-graphics action 0 0 0 item
s" Show a buffer"   ' gfx.debug choice 0 4   s" no" s" occlusion" s" bloom" s" depth" s" bloom mask" 5 names item

\ the controls: an action a row, by its number (platform.c kActions); saved as they change
: control-item ( addr len i -- )  control 0 0 0 item ;
s" CONTROLS  (right: a new key, left: the second off)" ' noop action 0 0 0 item
s" Move forward"   0 control-item   s" Move back"   1 control-item
s" Move left"      2 control-item   s" Move right"  3 control-item
s" Act (circle)"   4 control-item   s" Run (cross)" 5 control-item
s" Kick / shove (square)"  6 control-item   s" Throw (triangle)" 7 control-item
s" Crouch (L1)"    8 control-item   s" Flee (R1)"   9 control-item
s" Pause (start)" 10 control-item   s" Items (select)" 11 control-item
s" Hewie: go / attack" 12 control-item   s" Hewie: come back" 13 control-item
s" Hewie: praise"  14 control-item   s" Hewie: scold" 15 control-item
s" Hewie: stay / come (R3)" 16 control-item
s" Controls as they were" ' controls-reset action 0 0 0 item

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
\ as many rows as the screen holds, scrolled to keep the one selected in the middle
: shown-rows ( -- n )  screen-size nip 48 - row-h @ /  2 -  #items @ min ;
: first-row ( -- i )  selected @ shown-rows 2/ -  #items @ shown-rows -  min  0 max ;
: draw-menu
    menu-open @ 0= if  exit  then
    screen-size nip 900 >= if 3 else 2 then pen-scale
    char-size row-h ! col-w !
    $0A0C10D8 pen-color  16 16  col-w @ 56 *  shown-rows 2 + row-h @ * 16 +  draw-rect
    $FFE070FF pen-color  s" Settings   (arrows, F1 to close)" 28 24 draw-text
    first-row {: top :}
    shown-rows 0 ?do
        i top + selected @ = if  $FFE070FF  else  $C8D0D8FF  then pen-color
        i top + rec label  28  i 2 + row-h @ * 16 +  draw-text
        i top + rec value-text  28 col-w @ 26 * +  i 2 + row-h @ * 16 +  draw-text
    loop ;

' f1-key on-tick
' menu-keys on-tick
' watch-controls on-tick
' draw-menu on-draw

\ a fresh start: enhanced; a player's saved settings over it
look-enhanced
: load-settings  settings-file file-exists? if  settings-file included  then ;
' load-settings catch drop
: load-controls  controls-file file-exists? if  controls-file included  then ;
' load-controls catch drop  keep-controls
