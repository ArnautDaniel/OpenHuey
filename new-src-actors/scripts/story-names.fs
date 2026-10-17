\ story-names.fs - the story flags (the scenario's, the progress' +0x1C; ~0x36B of them) by
\ what they mean, named as they are worked out from the room scripts. Use the names.
IN: story-names

$C   constant opening-done        \ the cage room's opening has played (its entering script plays it otherwise)
$33  constant grate-open          \ the cage room's grate is open (its way out unblocked, the grate's parts hidden)
$AF  constant items-40-as-70      \ ? the scripts' items / rooms 0x40 and 0x41 count as 0x70 (Events_ScriptRoom)
\ ---- the game over's music (GameOver's faded-out sequence picks its track by these: the
\ latest set wins - $A with none). What they mean in the story: not yet worked out ----
$41  constant over-music-b        \ ? the game over's track $B (+0x24 bit 2)
$82  constant over-music-c        \ ? its track $C (+0x2C bit 4)
$94  constant over-music-d        \ ? its track $D (+0x2C bit 0x100000)
$95  constant over-music-e        \ ? its track $E (+0x2C bit 0x200000)
$A4  constant over-music-f        \ ? its track $F (+0x30 bit 0x10)
\ ---- the items (src/game/items.c, story_chars.c: their uses test and set these) ----
$7   constant flowers-laid        \ ? the Fresh Flowers can't be used any more (their use's spot is done)
$37  constant garden-door-done    \ ? the Garden Key's door (room $25's door 1) no longer takes it
$42  constant saturnus-hint-on    \ ? the Saturnus Key used elsewhere adds its hint ($232)
$66  constant books-placed        \ ..$6E: book b (0..2: items $15..$17) on lectern p (0..2: room $54's
                                  \ spots $D, $11, $14) is flag books-placed + p * 3 + b
$20  constant plate-made          \ ? a word plate has been made (SubScreen_StateWordMake)
$85  constant medallion-40-done   \ ? room $40's medallion spot no longer takes one
$8E  constant c7-second-event     \ ? the books' spot in room $C7 starts event $10, not 3
\ the maps held (Castle / Mansion / Forest / Tower / Mystery sketch used: the sub screen's map
\ pages; Progress_MapsHeld) and the flowers withered (the Fresh Flowers' 9000 frames)
$356 constant castle-map-held
$357 constant mansion-map-held
$358 constant forest-map-held
$359 constant tower-map-held
$35A constant mystery-map-held
$35F constant flowers-withered

\ ---- the story flags (progress +0x24..), named from the rooms' scripts: an item or document
\ picked up (got-, found-file-, <room>-<item>), Hewie's finds (-sniffed-out: his shared script $8B
\ sets them), a breakable broken, the lecterns' books, a thing looked at; the rest by the room that
\ sets (or tests) them and a number, with what is known - ? an unknown. The rooms by short name:
\ fg-4 front-garden-4, c1f-2 castle-1f-2 (c2f-, c3f-, cb1-), om1f-23 old-mansion-1f-23 (om2f-, om3f-,
\ omb1-), ht1f-2 house-of-truth (ht2f-, htb1-), wt2f-1 water-tower, cf-8 chaos-forest, lp-1 the lake
\ path, x-37 off the map ----
$0    constant found-file-202                \ (castle-2f-3)
$1    constant got-castle-sketch             \ (castle-b1-1)
$2    constant c2f-3-flag1                   \ ?
$3    constant hewie-found-in-wire           \ "Unforgivable! A wire is bound to the poor animal..." (front-garden-3)
$4    constant got-broken-marionette         \ "You've obtained ."
$5    constant c1f-8-flag1                   \ ?
$6    constant found-file-209                \ (castle-2f-2, castle-b1-6)
$8    constant got-study-key                 \ (castle-b1-7)
$9    constant c1f-11-flag1                  \ ? after a scene
$A    constant got-lunar-refractor           \ (castle-2f-6)
$B    constant hewie-freed                   \ "Thank you. You saved me... What's your name? Hewie?" (castle-2f-3)
$D    constant c2f-2-flag1                   \ ?
$E    constant found-file-227                \ (castle-1f-13, castle-2f-14)
$F    constant found-file-222                \ (front-garden-1)
$10   constant found-file-223                \ (front-garden-1)
$11   constant broken-marionette-used        \ (castle-1f-4)
$12   constant c1f-2-flag1                   \ ?
$13   constant c1f-4-flag1                   \ ? set on entering
$14   constant breaker-lowered               \ the breaker box in castle-1f-7: its handle lowered
$15   constant cb1-6-flag1                   \ ?
$16   constant cb1-6-flag2                   \ ?
$17   constant found-file-201                \ (castle-2f-3)
$18   constant c2f-3-flag2                   \ ?
$19   constant c2f-8-flag1                   \ ? never set by a room
$1A   constant c2f-8-flag2                   \ ? never set by a room
$1B   constant c2f-8-flag3                   \ ? after a scene
$1C   constant c2f-2-flag2                   \ ?
$1D   constant c2f-9-flag1                   \ ?
$1E   constant got-old-parchment             \ (castle-2f-8)
$1F   constant c2f-7-flag1                   \ ? after a scene; "It's not a computer, that's for sure... It has alphabetic k
$21   constant c1f-10-flag1                  \ ? never set by a room
$22   constant c1f-10-flag2                  \ ? after a scene
$23   constant got-saturnus-key              \ (castle-b1-6)
$24   constant c1f-1-flag1                   \ ? set on entering
$25   constant got-doc-luminessants          \ (castle-1f-9, castle-b1-6)
$26   constant c1f-12-flag1                  \ ? after a scene
$27   constant got-scrawled-memo             \ (castle-1f-2)
$28   constant fg-2-flag1                    \ ? after a scene
$29   constant fg-3-flag1                    \ ? "Where am I?"
$2A   constant fg-3-flag2                    \ ? "It's kind of cold... The air smells... funny."
$2B   constant c1f-9-flag1                   \ ?
$2C   constant cb1-6-flag3                   \ ? set on entering
$2D   constant found-file-227-2              \ (castle-1f-13, castle-b1-6)
$2E   constant c2f-2-flag3                   \ ? set on entering; after a scene
$2F   constant c2f-1-flag1                   \ ? set on entering; after a scene
$30   constant lunar-refractor-used          \ (castle-1f-12)
$31   constant found-file-208                \ (castle-1f-8)
$32   constant cb1-1-flag1                   \ ?
$34   constant found-file-20b                \ (castle-b1-6)
$35   constant cb1-6-flag4                   \ ?
$36   constant cb1-6-flag5                   \ ?
$38   constant c1f-13-flag1                  \ ? set on entering; after a scene
$39   constant c1f-6-flag1                   \ ? after a scene; "My creator said he made me the perfect woman... But I canno
$3A   constant c2f-3-flag3                   \ ?
$3B   constant c1f-3-flag1                   \ ? after a scene
$3C   constant got-garden-key                \ (castle-1f-3)
$3D   constant c1f-3-bookshelf-hint          \ "That bookshelf... I wonder if I can hide behind it?"
$3E   constant got-castle-gate-key           \ (castle-b1-3)
$3F   constant c1f-14-flag1                  \ ? set on entering
$40   constant c1f-15-flag1                  \ ? set on entering
$43   constant omb1-5-flag1                  \ ? "Did something just move?"
$44   constant omb1-5-flag2                  \ ? "Did something just move?"
$45   constant omb1-5-flag3                  \ ? "Did something just move?"
$46   constant om3f-2-flag1                  \ ? after a scene
$47   constant omb1-3-flag1                  \ ?
$48   constant omb1-3-flag2                  \ ?
$49   constant got-venus-key                 \ (old-mansion-b1-3)
$4A   constant found-file-234                \ (old-mansion-2f-8)
$4B   constant om2f-8-flag1                  \ ?
$4C   constant got-film-reel                 \ (old-mansion-1f-12)
$50   constant om1f-12-flag1                 \ ? after a scene
$51   constant om1f-12-flag2                 \ ? "Nothing happened. This must not be the correct plate..."
$52   constant om1f-12-flag3                 \ ? "Nothing happened. This must not be the correct plate..."
$53   constant got-jupiter-key               \ (old-mansion-2f-11)
$54   constant om2f-12-flag1                 \ ?
$55   constant om3f-1-flag1                  \ ?
$56   constant om3f-1-flag2                  \ ?
$57   constant armor-button-pushed           \ the button on the armour's back pushed (old-mansion-3f-2)
$58   constant om1f-13-flag1                 \ ? "I wonder what's going on outside of this steel cage I seeme
$59   constant om1f-9-flag1                  \ ? "I wonder what's going on outside of this steel cage I seeme
$5A   constant omb1-6-flag1                  \ ? after a scene
$5B   constant c1f-14-flag2                  \ ?
$5C   constant c3f-2-flag1                   \ ?
$5D   constant omb1-6-flag2                  \ ?
$5E   constant omb1-6-flag3                  \ ?
$5F   constant omb1-6-flag4                  \ ?
$60   constant omb1-6-flag5                  \ ?
$61   constant c2f-9-flag2                   \ ? "Miss... It's cleaning time now."
$62   constant om1f-15-flag1                 \ ?
$63   constant got-cathedral-secrets         \ (old-mansion-1f-15)
$64   constant got-occult-philosophy         \ (old-mansion-1f-15)
$65   constant got-wisemans-crafts           \ (old-mansion-1f-15)
$67   constant lectern-0-book-1              \ (old-mansion-1f-15)
$68   constant lectern-0-book-2              \ (old-mansion-1f-15)
$69   constant lectern-1-book-0              \ (old-mansion-1f-15)
$6A   constant lectern-1-book-1              \ (old-mansion-1f-15)
$6B   constant lectern-1-book-2              \ (old-mansion-1f-15)
$6C   constant lectern-2-book-0              \ (old-mansion-1f-15)
$6D   constant lectern-2-book-1              \ (old-mansion-1f-15)
$6E   constant lectern-2-book-2              \ (old-mansion-1f-15)
$6F   constant x-37-flag1                    \ ?
$70   constant om1f-12-flag4                 \ ?
$71   constant found-file-233                \ (old-mansion-1f-1)
$72   constant om2f-11-flag1                 \ ?
$73   constant got-torn-letter               \ (old-mansion-1f-12)
$74   constant film-reel-used-om1f-14        \ (old-mansion-1f-14)
$75   constant om1f-13-flag2                 \ ?
$76   constant white-mandragora-used         \ (old-mansion-1f-8)
$77   constant om1f-4-flag1                  \ ?
$78   constant om2f-5-flag1                  \ ? set on entering
$79   constant om1f-8-flag1                  \ ? set on entering
$7A   constant om2f-10-flag1                 \ ? set on entering
$7B   constant om1f-6-flag1                  \ ? set on entering
$7C   constant om1f-19-flag1                 \ ? set on entering
$7D   constant om1f-21-flag1                 \ ? set on entering
$7E   constant om1f-20-breakable-broken      \
$7F   constant got-mars-key                  \ (old-mansion-3f-1)
$80   constant om1f-8-flag2                  \ ? after a scene; "It doesn't seem too terribly dangerous... But I get the fee
$81   constant om2f-11-flag2                 \ ? set on entering
$84   constant om2f-2-flag1                  \ ? after a scene
$86   constant found-file-247                \ (chaos-forest-8)
$87   constant cf-10-flag1                   \ ? after a scene; "You've been shot. You'll be all right. This will stop the b
$88   constant cf-13-flag1                   \ ?
$89   constant x-37-flag2                    \ ?
$8A   constant cf-11-flag1                   \ ?
$8B   constant wt2f-1-flag1                  \ ?
$8C   constant wt4f-1-flag1                  \ ?
$8D   constant wt2f-1-flag2                  \ ? never set by a room
$91   constant wt8f-1-flag1                  \ ?
$93   constant wt8f-1-flag2                  \ ? after a scene; "Riccardo... You were always... incomplete. But now, I will
$96   constant found-file-257                \ (house-of-truth-1f-5)
$97   constant ht1f-3-flag1                  \ ?
$98   constant found-file-252                \ (house-of-truth-b1-2)
$99   constant ht1f-1-flag1                  \ ?
$9A   constant found-file-250                \ (house-of-truth-1f-4)
$9B   constant ht1f-4-flag1                  \ ? after a scene
$9C   constant got-sol-key                   \ (house-of-truth-1f-4)
$9D   constant got-golden-candlestick        \ (house-of-truth-1f-3)
$9E   constant got-cane-of-caduceus          \ (house-of-truth-1f-7)
$9F   constant ht1f-7-flag1                  \ ? after a scene
$A0   constant ht1f-7-flag2                  \ ? after a scene
$A1   constant found-file-255                \ (house-of-truth-1f-9)
$A2   constant ht1f-9-flag1                  \ ?
$A3   constant ht1f-9-flag2                  \ ?
$A5   constant ht1f-7-flag3                  \ ?
$A6   constant ht1f-8-flag1                  \ ?
$A7   constant htb1-3-flag1                  \ ? set on entering
$A8   constant htb1-10-flag1                 \ ?
$A9   constant htb1-11-flag1                 \ ?
$AA   constant htb1-12-flag1                 \ ?
$AB   constant htb1-11-flag2                 \ ?
$AC   constant ht1f-1-flag2                  \ ? set on entering
$AE   constant fg-3-flag3                    \ ? set on entering
$200  constant c1f-5-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$201  constant cb1-7-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$202  constant c2f-12-breakable-broken       \ broken by a blow in its zone: shards, its model off, the way open
$203  constant cb1-1-breakable1-broken       \ broken by a blow in its zone: shards, its model off, the way open
$204  constant c1f-11-breakable1-broken      \ broken by a blow in its zone: shards, its model off, the way open
$205  constant c1f-11-breakable2-broken      \ broken by a blow in its zone: shards, its model off, the way open
$206  constant c1f-11-breakable3-broken      \ broken by a blow in its zone: shards, its model off, the way open
$207  constant c1f-12-breakable-broken       \ broken by a blow in its zone: shards, its model off, the way open
$209  constant c2f-8-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$20A  constant c1f-2-breakable1-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20B  constant c1f-2-breakable2-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20C  constant c1f-9-breakable1-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20D  constant c1f-9-breakable2-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20E  constant fg-1-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$20F  constant fg-2-breakable1-broken        \ broken by a blow in its zone: shards, its model off, the way open
$210  constant fg-2-breakable2-broken        \ broken by a blow in its zone: shards, its model off, the way open
$211  constant fg-2-breakable3-broken        \ broken by a blow in its zone: shards, its model off, the way open
$212  constant fg-5-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$213  constant fg-4-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$214  constant fg-3-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$215  constant cf-8-breakable1-broken        \ broken by a blow in its zone: shards, its model off, the way open
$216  constant cf-8-breakable2-broken        \ broken by a blow in its zone: shards, its model off, the way open
$217  constant cf-8-breakable3-broken        \ broken by a blow in its zone: shards, its model off, the way open
$218  constant cf-8-breakable4-broken        \ broken by a blow in its zone: shards, its model off, the way open
$219  constant cf-8-breakable5-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21A  constant cf-8-breakable6-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21B  constant cf-8-breakable7-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21C  constant cf-9-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$21D  constant cf-10-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21E  constant om1f-21-breakable1-broken     \
$21F  constant c1f-14-breakable1-broken      \ broken by a blow in its zone: shards, its model off, the way open
$220  constant c1f-14-breakable2-broken      \ broken by a blow in its zone: shards, its model off, the way open
$221  constant c1f-14-breakable3-broken      \ broken by a blow in its zone: shards, its model off, the way open
$222  constant fg-4-camomile                 \ (front-garden-4)
$223  constant c1f-3-camomile                \ (castle-1f-3)
$225  constant cb1-2-camomile                \ (castle-b1-2)
$226  constant c1f-16-magnesia               \ (castle-1f-16)
$227  constant c1f-17-lavender               \ (castle-1f-17)
$228  constant c1f-10-beef-jerky             \ (castle-1f-10)
$229  constant fg-3-magnesia                 \ (front-garden-3)
$22A  constant c1f-11-magnesia               \ (castle-1f-11)
$22B  constant c1f-14-chicken-jerky          \ (castle-1f-14)
$22C  constant cb1-1-chicken-jerky           \ (castle-b1-1)
$22D  constant cb1-7-medallion               \ (castle-b1-7)
$22E  constant om1f-17-breakable1-broken     \
$22F  constant om1f-17-breakable2-broken     \
$230  constant c1f-9-album-medallion         \ (castle-1f-9)
$231  constant fg-3-refined-magnesia         \ (front-garden-3)
$232  constant c2f-13-flag1                  \ ? never set by a room
$233  constant om2f-10-breakable-broken      \
$234  constant c1f-1-refined-magnesia        \ (castle-1f-1)
$235  constant c1f-11-medallion              \ (castle-1f-11)
$236  constant c1f-19-rubrum-medallion       \ (castle-1f-19)
$237  constant omb1-2-breakable1-broken      \
$238  constant omb1-2-breakable2-broken      \
$239  constant c1f-4-refined-antimony        \ (castle-1f-4)
$23A  constant fg-5-flag1                    \ ?
$23B  constant fg-5-viola-medallion          \ (front-garden-5)
$23C  constant fg-1-medallion-found          \ Hewie sniffs it out (his shared script $8B sets this); taking it: fg-1-medallion
$23D  constant fg-1-medallion                \ (front-garden-1)
$23E  constant fg-5-medallion-found          \ Hewie sniffs it out (his shared script $8B sets this); taking it: fg-5-medallion
$23F  constant fg-5-medallion                \ (front-garden-5)
$240  constant fg-4-album-medallion-found    \ Hewie sniffs it out (his shared script $8B sets this); taking it: fg-4-album-medallion
$241  constant fg-4-album-medallion          \ (front-garden-4)
$242  constant c1f-6-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-6-medallion
$243  constant c1f-6-medallion               \ (castle-1f-6)
$244  constant cb1-1-lavender-found          \ Hewie sniffs it out (his shared script $8B sets this); taking it: cb1-1-lavender
$245  constant cb1-1-lavender                \ (castle-b1-1)
$246  constant cb1-2-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: cb1-2-medallion
$247  constant cb1-2-medallion               \ (castle-b1-2)
$248  constant c1f-9-rubrum-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-9-rubrum-medallion
$249  constant c1f-9-rubrum-medallion        \ (castle-1f-9)
$24A  constant c2f-2-album-medallion-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-2-album-medallion
$24B  constant c2f-2-album-medallion         \ (castle-2f-2)
$24C  constant c2f-1-refined-magnesia-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-1-refined-magnesia
$24D  constant c2f-1-refined-magnesia        \ (castle-2f-1)
$24E  constant c1f-2-rubrum-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-2-rubrum-medallion
$24F  constant c1f-2-rubrum-medallion        \ (castle-1f-2)
$250  constant c1f-13-antimony-powder-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-13-antimony-powder
$251  constant c1f-13-antimony-powder        \ (castle-1f-13)
$252  constant cb1-7-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: cb1-7-medallion
$253  constant cb1-7-medallion-2             \ (castle-b1-7)
$254  constant c1f-15-camomile-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-15-camomile
$255  constant c1f-15-camomile               \ (castle-1f-15)
$256  constant c1f-15-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-15-medallion
$257  constant c1f-15-medallion              \ (castle-1f-15)
$258  constant c2f-12-refined-antimony-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-12-refined-antimony
$259  constant c2f-12-refined-antimony       \ (castle-2f-12)
$25A  constant c1f-14-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-14-viride-medallion
$25B  constant c1f-14-viride-medallion       \ (castle-1f-14)
$25C  constant c1f-8-album-medallion-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-8-album-medallion
$25D  constant c1f-8-album-medallion         \ (castle-1f-8)
$25E  constant c2f-3-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-3-medallion
$25F  constant c2f-3-medallion               \ (castle-2f-3, castle-2f-5)
$260  constant c2f-13-medallion              \ (castle-2f-13, castle-2f-14, castle-2f-15)
$261  constant om2f-5-breakable1-broken      \
$262  constant om2f-5-breakable2-broken      \
$263  constant om1f-24-breakable-broken      \
$264  constant omb1-1-breakable-broken       \
$265  constant om2f-8-camomile               \ (old-mansion-2f-8)
$266  constant om2f-8-lavender               \ (old-mansion-2f-8)
$267  constant om1f-17-chicken-jerky         \ (old-mansion-1f-17)
$268  constant om1f-9-refined-antimony       \ (old-mansion-1f-9)
$269  constant omb1-2-refined-magnesia       \ (old-mansion-b1-2)
$26A  constant omb1-3-camomile               \ (old-mansion-b1-3)
$26B  constant got-sylvesta-esca             \ (old-mansion-2f-9)
$26C  constant om2f-11-magnesia              \ (old-mansion-2f-11)
$26D  constant om1f-14-chicken-jerky         \ (old-mansion-1f-14)
$26E  constant om1f-8-beef-jerky             \ (old-mansion-1f-8)
$26F  constant om1f-5-lavender               \ (old-mansion-1f-5, old-mansion-1f-8)
$270  constant om1f-8-album-medallion        \ (old-mansion-1f-8)
$271  constant omb1-6-medallion              \ (old-mansion-b1-6)
$272  constant om1f-16-refined-magnesia      \ (old-mansion-1f-16)
$273  constant om1f-15-refined-antimony      \ (old-mansion-1f-15)
$274  constant om2f-10-weeping-stone         \ (old-mansion-2f-10)
$275  constant om1f-6-medallion              \ (old-mansion-1f-6)
$276  constant om2f-6-lavender               \ (old-mansion-2f-6)
$277  constant om1f-3-medallion              \ (old-mansion-1f-3)
$278  constant om1f-4-chicken-jerky          \ (old-mansion-1f-4)
$27A  constant om1f-2-camomile               \ (old-mansion-1f-2)
$27C  constant om2f-4-viride-medallion       \ (old-mansion-2f-4)
$27D  constant om1f-24-rubrum-medallion      \ (old-mansion-1f-24)
$27E  constant om1f-20-weeping-stone         \ (old-mansion-1f-20)
$27F  constant om1f-22-refined-antimony      \ (old-mansion-1f-22)
$280  constant fg-1-medallion-2              \ (front-garden-1)
$281  constant cb1-1-chicken-jerky-2         \ (castle-b1-1)
$282  constant c2f-1-lavender                \ (castle-2f-1)
$283  constant fg-2-medallion                \ (front-garden-2)
$284  constant fg-5-beef-jerky               \ (front-garden-5)
$285  constant c1f-2-viola-medallion         \ (castle-1f-2)
$286  constant got-onion                     \ (castle-1f-2)
$287  constant c1f-11-beef-jerky             \ (castle-1f-11)
$288  constant c1f-9-magnesia                \ (castle-1f-9)
$289  constant c1f-12-medallion              \ (castle-1f-12)
$28A  constant c1f-14-antimony-powder        \ (castle-1f-14)
$28B  constant c1f-5-antimony-powder         \ (castle-1f-5)
$28C  constant c2f-12-medallion              \ (castle-2f-12)
$28D  constant om2f-5-medallion              \ (old-mansion-2f-5)
$28E  constant om1f-21-album-medallion       \ (old-mansion-1f-21)
$28F  constant omb1-1-chicken-jerky          \ (old-mansion-b1-1)
$290  constant omb1-2-viola-medallion        \ (old-mansion-b1-2)
$291  constant omb1-2-rubrum-medallion       \ (old-mansion-b1-2)
$292  constant om1f-24-beef-jerky            \ (old-mansion-1f-24)
$293  constant om1f-17-magnesia              \ (old-mansion-1f-17)
$294  constant c1f-13-chicken-jerky          \ (castle-1f-13)
$295  constant c1f-8-magnesia                \ (castle-1f-8)
$296  constant c2f-6-lavender                \ (castle-2f-6)
$297  constant ht2f-1-breakable1-broken      \
$298  constant ht1f-2-breakable-broken       \
$299  constant om1f-11-album-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-11-album-medallion
$29A  constant om1f-11-album-medallion       \ (old-mansion-1f-11)
$29B  constant om1f-17-medallion-found       \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-17-medallion
$29C  constant om1f-17-medallion             \ (old-mansion-1f-17)
$29D  constant om1f-9-refined-magnesia-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-9-refined-magnesia
$29E  constant om1f-9-refined-magnesia       \ (old-mansion-1f-9)
$29F  constant om1f-23-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-23-viride-medallion
$2A0  constant om1f-23-viride-medallion      \ (old-mansion-1f-23)
$2A1  constant om1f-23-medallion-found       \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-23-medallion
$2A2  constant om1f-23-medallion             \ (old-mansion-1f-23)
$2A3  constant om1f-12-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-12-rubrum-medallion
$2A4  constant om1f-12-rubrum-medallion      \ (old-mansion-1f-12)
$2A5  constant omb1-7-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: omb1-7-medallion
$2A6  constant omb1-7-medallion              \ (old-mansion-b1-7)
$2A7  constant om1f-15-antimony-tube-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-15-antimony-tube
$2A8  constant om1f-15-antimony-tube         \ (old-mansion-1f-15)
$2A9  constant omb1-6-viola-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: omb1-6-viola-medallion
$2AA  constant omb1-6-viola-medallion        \ (old-mansion-b1-6)
$2AB  constant om1f-5-refined-magnesia-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-5-refined-magnesia
$2AC  constant om1f-5-refined-magnesia       \ (old-mansion-1f-5)
$2AD  constant om2f-13-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-13-rubrum-medallion
$2AE  constant om2f-13-rubrum-medallion      \ (old-mansion-2f-13)
$2AF  constant om2f-12-antimony-powder-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-12-antimony-powder
$2B0  constant om2f-12-antimony-powder       \ (old-mansion-2f-12)
$2B1  constant om1f-15-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-15-rubrum-medallion
$2B2  constant om1f-15-rubrum-medallion      \ (old-mansion-1f-15)
$2B3  constant om1f-20-medallion-found       \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-20-medallion
$2B4  constant om1f-20-medallion             \ (old-mansion-1f-20)
$2B5  constant om1f-20-medallion-sniffed-2   \
$2B6  constant om1f-20-medallion-2           \ (old-mansion-1f-20)
$2B7  constant om1f-20-medallion-sniffed-3   \
$2B8  constant om1f-20-medallion-3           \ (old-mansion-1f-20)
$2B9  constant om1f-20-prima-magnesia-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-20-prima-magnesia
$2BA  constant om1f-20-prima-magnesia        \ (old-mansion-1f-20)
$2BB  constant om1f-7-album-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-7-album-medallion
$2BC  constant om1f-7-album-medallion        \ (old-mansion-1f-7)
$2BD  constant om2f-2-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-2-medallion
$2BE  constant om2f-2-medallion              \ (old-mansion-2f-2)
$2BF  constant om1f-3-refined-antimony-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-3-refined-antimony
$2C0  constant om1f-3-refined-antimony       \ (old-mansion-1f-3)
$2C1  constant om2f-5-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-5-viride-medallion
$2C2  constant om2f-5-viride-medallion       \ (old-mansion-2f-5)
$2C3  constant omb1-6-magist-medallion       \ (old-mansion-b1-6)
$2C4  constant om1f-8-antimony-tube          \ (old-mansion-1f-8)
$2C5  constant om2f-3-album-medallion        \ (old-mansion-2f-3)
$2C6  constant omb1-1-album-medallion        \ (old-mansion-b1-1)
$2C7  constant cf-12-camomile                \ (chaos-forest-12)
$2C8  constant cf-8-medallion                \ (chaos-forest-8)
$2C9  constant cf-8-chicken-jerky            \ (chaos-forest-8)
$2CA  constant cf-10-medallion               \ (chaos-forest-10)
$2CB  constant cf-9-refined-antimony         \ (chaos-forest-9)
$2CC  constant om2f-12-breakable1-broken     \
$2CD  constant om2f-12-breakable2-broken     \
$2CE  constant om1f-21-breakable2-broken     \
$2CF  constant om1f-21-breakable3-broken     \
$2D0  constant om1f-21-breakable4-broken     \
$2D3  constant got-metal-boots               \ (castle-b1-3)
$2D6  constant wt1f-1-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: wt1f-1-medallion
$2D7  constant wt1f-1-medallion              \ (water-tower-1f-1)
$2D8  constant wt2f-1-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: wt2f-1-medallion
$2D9  constant wt2f-1-medallion              \ (water-tower-2f-1)
$2DA  constant wt7f-2-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: wt7f-2-rubrum-medallion
$2DB  constant wt7f-2-rubrum-medallion       \ (water-tower-7f-2)
$2DE  constant wt2f-1-refined-antimony       \ (water-tower-2f-1)
$2DF  constant wt4f-1-medallion              \ (water-tower-4f-1)
$2E0  constant lp-1-lavender                 \ (lake-pathway-1)
$2E1  constant ht1f-10-camomile              \ (house-of-truth-1f-10)
$2E2  constant ht1f-3-rubrum-medallion       \ (house-of-truth-1f-3)
$2E3  constant htb1-2-lavender               \ (house-of-truth-b1-2)
$2E4  constant htb1-5-camomile               \ (house-of-truth-b1-5)
$2E5  constant htb1-7-beef-jerky             \ (house-of-truth-b1-7)
$2E6  constant ht1f-2-refined-magnesia       \ (house-of-truth-1f-2)
$2E7  constant ht1f-7-antimony-tube          \ (house-of-truth-1f-7)
$2E8  constant ht1f-2-refined-antimony       \ (house-of-truth-1f-2)
$2E9  constant ht1f-1-album-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-1-album-medallion
$2EA  constant ht1f-1-album-medallion        \ (house-of-truth-1f-1)
$2EB  constant ht2f-1-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht2f-1-medallion
$2EC  constant ht2f-1-medallion              \ (house-of-truth-2f-1)
$2ED  constant ht1f-3-viola-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-3-viola-medallion
$2EE  constant ht1f-3-viola-medallion        \ (house-of-truth-1f-3)
$2EF  constant ht2f-2-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht2f-2-viride-medallion
$2F0  constant ht2f-2-viride-medallion       \ (house-of-truth-2f-2)
$2F1  constant ht1f-2-prima-magnesia-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-2-prima-magnesia
$2F2  constant ht1f-2-prima-magnesia         \ (house-of-truth-1f-2)
$2F3  constant ht1f-8-magist-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-8-magist-medallion
$2F4  constant ht1f-8-magist-medallion       \ (house-of-truth-1f-8)
$2F5  constant got-ball                      \ (front-garden-5)
$2F6  constant ht2f-2-rubrum-medallion       \ (house-of-truth-2f-2)
$2F7  constant wt2f-1-breakable-broken       \
$2F8  constant wt4f-1-breakable-broken       \
$2F9  constant wt4f-1-camomile               \ (water-tower-4f-1)
$2FA  constant ht2f-1-breakable2-broken      \
$2FB  constant ht2f-1-breakable3-broken      \
$2FC  constant ht2f-1-viola-medallion        \ (house-of-truth-2f-1)
$2FD  constant cb1-7-medallion-3             \ (castle-b1-7)
$2FE  constant c2f-15-prima-magnesia-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-15-prima-magnesia
$2FF  constant c2f-15-prima-magnesia         \ (castle-2f-15)
$300  constant c2f-3-tv-examined             \ "I hope this TV works..."
$301  constant c1f-3-bookshelf-examined      \ "A book shelf of fine design..."
$302  constant c1f-1-flag2                   \ ?
$303  constant c1f-3-flag2                   \ ? after a scene
$304  constant c2f-2-flag4                   \ ? "Is this... blood?"
$305  constant c2f-12-flag1                  \ ?
$306  constant c2f-3-tv-examined-2           \ "I hope this TV works..." (the other)
$307  constant c2f-7-flag2                   \ ? "It's a plate pressing machine."
$308  constant c1f-5-flag1                   \ ? set on entering
$309  constant c1f-16-flag1                  \ ?
$30A  constant c1f-16-flag2                  \ ?
$30B  constant fg-4-flag1                    \ ?
$30C  constant got-jade-choker               \ (castle-2f-12)
$30E  constant c2f-12-flag2                  \ ? set on entering
$310  constant c1f-4-flag2                   \ ?
$311  constant c1f-4-flag3                   \ ?
$312  constant c2f-3-portrait-1-examined     \ "I wonder whose portrait this is?"
$313  constant c2f-3-portrait-3-examined     \ "What a powerful piece of art..."
$314  constant c2f-3-portrait-2-examined     \ "The man pictured here has a noble air..."
$315  constant got-obsidian-choker           \ (castle-2f-8)
$316  constant c1f-7-flag1                   \ ?
$317  constant cb1-5-flag1                   \ ?
$318  constant c2f-12-flag3                  \ ?
$319  constant c1f-4-flag4                   \ ?
$31A  constant cb1-7-flag1                   \ ?
$31B  constant cb1-7-flag2                   \ ?
$31C  constant c2f-2-flag5                   \ ?
$31D  constant c1f-10-flag3                  \ ? after a scene
$31E  constant got-doc-recovery              \ (castle-b1-2, house-of-truth-1f-1, house-of-truth-1f-2, old-mansion-1f-9, old-mansion-2f-6, water-tower-1f-1)
$31F  constant got-cracked-plate-key         \ (castle-1f-4)
$320  constant got-cracked-plate-key-2       \ (castle-1f-6)
$321  constant cb1-1-breakable2-broken       \ broken by a blow in its zone: shards, its model off, the way open
$322  constant om2f-3-flag1                  \ ?
$323  constant om2f-6-flag1                  \ ?
$324  constant om2f-6-flag2                  \ ?
$325  constant om1f-22-flag1                 \ ?
$326  constant omb1-4-flag1                  \ ?
$327  constant omb1-5-flag4                  \ ?
$328  constant omb1-5-flag5                  \ ?
$329  constant omb1-2-breakable3-broken      \
$32A  constant omb1-2-flag1                  \ ?
$32B  constant om1f-11-breakable-broken      \
$32C  constant om1f-11-flag1                 \ ?
$32D  constant om2f-13-breakable-broken      \
$32E  constant om2f-13-flag1                 \ ?
$330  constant omb1-3-flag3                  \ ?
$331  constant journals-examined             \ "Mountains of books have been piled up here..." (old-mansion-2f-11)
$332  constant red-painting-examined         \ "Why would someone intentionally paint this completely red?" (old-mansion-1f-18)
$333  constant om1f-14-flag1                 \ ? after a scene; "What is this? I was being filmed. Who's there? Why are you
$334  constant sundial-examined              \ "This is a sundial..." (chaos-forest-8)
$335  constant got-mansion-sketch            \ (old-mansion-1f-23)
$336  constant got-forest-map                \ (chaos-forest-1)
$337  constant film-reel-used-om1f-14-2      \ (old-mansion-1f-14)
$338  constant om3f-1-flag3                  \ ?
$339  constant mummy-examined                \ "This... isn't a doll! It's a mummy!" (old-mansion-1f-17)
$33A  constant wt4f-1-flag2                  \ ?
$33E  constant wt8f-1-flag3                  \ ?
$33F  constant wt8f-1-flag4                  \ ?
$340  constant got-doc-crouch-evasion        \ (castle-1f-1)
$341  constant wt2f-1-flag3                  \ ? set on entering
$342  constant wt4f-1-flag3                  \ ? set on entering
$343  constant got-mystery-sketch            \ (house-of-truth-2f-2)
$344  constant cf-12-flag1                   \ ?
$345  constant htb1-5-flag1                  \ ? set on entering
$346  constant htb1-6-flag1                  \ ? set on entering
$347  constant htb1-7-flag1                  \ ? set on entering
$348  constant htb1-8-flag1                  \ ? set on entering
$349  constant got-bisque-doll               \ (castle-2f-2)
$34A  constant wt7f-1-flag1                  \ ? set on entering
$34B  constant got-doc-action                \ (castle-2f-3, off-map-27)
$34C  constant wt2f-1-flag4                  \ ?
$34D  constant wt4f-1-flag4                  \ ?
$34E  constant tremor-warned                 \ "Whoa! What was that? It felt like a tremor..." (house-of-truth-1f-8)
$34F  constant tremor-warned-2               \ "Whoa! What was that?..." (house-of-truth-1f-8, the other)
$350  constant found-file-254                \ (house-of-truth-b1-4)
$351  constant shared-flag1                  \ ?
$352  constant shared-flag2                  \ ?
$353  constant shared-flag3                  \ ?
$354  constant shared-flag4                  \ ?
$35B  constant x-d5-flag1                    \ ?
$35C  constant x-d8-flag1                    \ ?
$35D  constant cf-9-flag1                    \ ?
$35E  constant x-d7-flag1                    \ ?
$360  constant got-red-godstone              \ (old-mansion-1f-4)
$361  constant got-handful-of-ash            \ (old-mansion-1f-2)
$362  constant got-yellow-mandragora         \ (old-mansion-1f-13)
$363  constant got-red-mandragora            \ (old-mansion-1f-13)
$364  constant got-blue-mandragora           \ (old-mansion-1f-13)
$365  constant got-purple-mandragora         \ (old-mansion-1f-13)
$366  constant got-hebels-perfume            \ (old-mansion-1f-24)
$368  constant x-27-magist-medallion         \ (off-map-27)
$369  constant x-27-magist-medallion-2       \ (off-map-27)
$36A  constant x-27-magist-medallion-3       \ (off-map-27)
$36B  constant x-27-magist-medallion-4       \ (off-map-27)

