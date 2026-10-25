\ story-names.fs - the story flags (the scenario's, the progress' +0x1C; ~0x36B of them) by
\ what they mean, named as they are worked out from the room scripts. Use the names.
IN: story-names
USING: facts ;

$C   fact:    opening-done        \ the cage room's opening has played (its entering script plays it otherwise)
$33  fact:    grate-open          \ the cage room's grate is open (its way out unblocked, the grate's parts hidden)
$AF  fact:    items-40-as-70      \ ? the scripts' items / rooms 0x40 and 0x41 count as 0x70 (Events_ScriptRoom)
\ ---- the game over's music (GameOver's faded-out sequence picks its track by these: the
\ latest set wins - $A with none). What they mean in the story: not yet worked out ----
$41  fact:    over-music-b        \ ? the game over's track $B (+0x24 bit 2)
$82  fact:    over-music-c        \ ? its track $C (+0x2C bit 4)
$94  fact:    over-music-d        \ ? its track $D (+0x2C bit 0x100000)
$95  fact:    over-music-e        \ ? its track $E (+0x2C bit 0x200000)
$A4  fact:    over-music-f        \ ? its track $F (+0x30 bit 0x10)
\ ---- the items (src/game/items.c, story_chars.c: their uses test and set these) ----
$7   fact:    flowers-laid        \ ? the Fresh Flowers can't be used any more (their use's spot is done)
$37  fact:    garden-door-done    \ ? the Garden Key's door (room $25's door 1) no longer takes it
$42  fact:    saturnus-hint-on    \ ? the Saturnus Key used elsewhere adds its hint ($232)
$66  fact:    books-placed        \ ..$6E: book b (0..2: items $15..$17) on lectern p (0..2: room $54's
                                  \ spots $D, $11, $14) is flag books-placed + p * 3 + b
$20  fact:    plate-made          \ ? a word plate has been made (SubScreen_StateWordMake)
$85  fact:    medallion-40-done   \ ? room $40's medallion spot no longer takes one
$8E  fact:    c7-second-event     \ ? the books' spot in room $C7 starts event $10, not 3
\ the maps held (Castle / Mansion / Forest / Tower / Mystery sketch used: the sub screen's map
\ pages; Progress_MapsHeld) and the flowers withered (the Fresh Flowers' 9000 frames)
$356 fact:    castle-map-held
$357 fact:    mansion-map-held
$358 fact:    forest-map-held
$359 fact:    tower-map-held
$35A fact:    mystery-map-held
$35F fact:    flowers-withered

\ ---- the story flags (progress +0x24..), named from the rooms' scripts: an item or document
\ picked up (got-, found-file-, <room>-<item>), Hewie's finds (-sniffed-out: his shared script $8B
\ sets them), a breakable broken, the lecterns' books, a thing looked at; the rest by the room that
\ sets (or tests) them and a number, with what is known - ? an unknown. The rooms by short name:
\ fg-4 front-garden-4, c1f-2 castle-1f-2 (c2f-, c3f-, cb1-), om1f-23 old-mansion-1f-23 (om2f-, om3f-,
\ omb1-), ht1f-2 house-of-truth (ht2f-, htb1-), wt2f-1 water-tower, cf-8 chaos-forest, lp-1 the lake
\ path, x-37 off the map ----
$0    fact:    found-file-202                \ (castle-2f-3)
$1    fact:    got-castle-sketch             \ (castle-b1-1)
$2    fact:    c2f-3-flag1                   \ ?
$3    fact:    hewie-found-in-wire           \ "Unforgivable! A wire is bound to the poor animal..." (front-garden-3)
$4    fact:    got-broken-marionette         \ "You've obtained ."
$5    fact:    c1f-8-flag1                   \ ?
$6    fact:    found-file-209                \ (castle-2f-2, castle-b1-6)
$8    fact:    got-study-key                 \ (castle-b1-7)
$9    fact:    c1f-11-flag1                  \ ? after a scene
$A    fact:    got-lunar-refractor           \ (castle-2f-6)
$B    fact:    hewie-freed                   \ "Thank you. You saved me... What's your name? Hewie?" (castle-2f-3)
$D    fact:    c2f-2-flag1                   \ ?
$E    fact:    found-file-227                \ (castle-1f-13, castle-2f-14)
$F    fact:    found-file-222                \ (front-garden-1)
$10   fact:    found-file-223                \ (front-garden-1)
$11   fact:    broken-marionette-used        \ (castle-1f-4)
$12   fact:    c1f-2-flag1                   \ ?
$13   fact:    c1f-4-flag1                   \ ? set on entering
$14   fact:    breaker-lowered               \ the breaker box in castle-1f-7: its handle lowered
$15   fact:    cb1-6-flag1                   \ ?
$16   fact:    cb1-6-flag2                   \ ?
$17   fact:    found-file-201                \ (castle-2f-3)
$18   fact:    c2f-3-flag2                   \ ?
$19   fact:    c2f-8-flag1                   \ ? never set by a room
$1A   fact:    c2f-8-flag2                   \ ? never set by a room
$1B   fact:    c2f-8-flag3                   \ ? after a scene
$1C   fact:    c2f-2-flag2                   \ ?
$1D   fact:    c2f-9-flag1                   \ ?
$1E   fact:    got-old-parchment             \ (castle-2f-8)
$1F   fact:    c2f-7-flag1                   \ ? after a scene; "It's not a computer, that's for sure... It has alphabetic k
$21   fact:    c1f-10-flag1                  \ ? never set by a room
$22   fact:    c1f-10-flag2                  \ ? after a scene
$23   fact:    got-saturnus-key              \ (castle-b1-6)
$24   fact:    c1f-1-flag1                   \ ? set on entering
$25   fact:    got-doc-luminessants          \ (castle-1f-9, castle-b1-6)
$26   fact:    c1f-12-flag1                  \ ? after a scene
$27   fact:    got-scrawled-memo             \ (castle-1f-2)
$28   fact:    fg-2-flag1                    \ ? after a scene
$29   fact:    fg-3-flag1                    \ ? "Where am I?"
$2A   fact:    fg-3-flag2                    \ ? "It's kind of cold... The air smells... funny."
$2B   fact:    c1f-9-flag1                   \ ?
$2C   fact:    cb1-6-flag3                   \ ? set on entering
$2D   fact:    found-file-227-2              \ (castle-1f-13, castle-b1-6)
$2E   fact:    c2f-2-flag3                   \ ? set on entering; after a scene
$2F   fact:    c2f-1-flag1                   \ ? set on entering; after a scene
$30   fact:    lunar-refractor-used          \ (castle-1f-12)
$31   fact:    found-file-208                \ (castle-1f-8)
$32   fact:    cb1-1-flag1                   \ ?
$34   fact:    found-file-20b                \ (castle-b1-6)
$35   fact:    cb1-6-flag4                   \ ?
$36   fact:    cb1-6-flag5                   \ ?
$38   fact:    c1f-13-flag1                  \ ? set on entering; after a scene
$39   fact:    c1f-6-flag1                   \ ? after a scene; "My creator said he made me the perfect woman... But I canno
$3A   fact:    c2f-3-flag3                   \ ?
$3B   fact:    c1f-3-flag1                   \ ? after a scene
$3C   fact:    got-garden-key                \ (castle-1f-3)
$3D   fact:    c1f-3-bookshelf-hint          \ "That bookshelf... I wonder if I can hide behind it?"
$3E   fact:    got-castle-gate-key           \ (castle-b1-3)
$3F   fact:    c1f-14-flag1                  \ ? set on entering
$40   fact:    c1f-15-flag1                  \ ? set on entering
$43   fact:    omb1-5-flag1                  \ ? "Did something just move?"
$44   fact:    omb1-5-flag2                  \ ? "Did something just move?"
$45   fact:    omb1-5-flag3                  \ ? "Did something just move?"
$46   fact:    om3f-2-flag1                  \ ? after a scene
$47   fact:    omb1-3-flag1                  \ ?
$48   fact:    omb1-3-flag2                  \ ?
$49   fact:    got-venus-key                 \ (old-mansion-b1-3)
$4A   fact:    found-file-234                \ (old-mansion-2f-8)
$4B   fact:    om2f-8-flag1                  \ ?
$4C   fact:    got-film-reel                 \ (old-mansion-1f-12)
$50   fact:    om1f-12-flag1                 \ ? after a scene
$51   fact:    om1f-12-flag2                 \ ? "Nothing happened. This must not be the correct plate..."
$52   fact:    om1f-12-flag3                 \ ? "Nothing happened. This must not be the correct plate..."
$53   fact:    got-jupiter-key               \ (old-mansion-2f-11)
$54   fact:    om2f-12-flag1                 \ ?
$55   fact:    om3f-1-flag1                  \ ?
$56   fact:    om3f-1-flag2                  \ ?
$57   fact:    armor-button-pushed           \ the button on the armour's back pushed (old-mansion-3f-2)
$58   fact:    om1f-13-flag1                 \ ? "I wonder what's going on outside of this steel cage I seeme
$59   fact:    om1f-9-flag1                  \ ? "I wonder what's going on outside of this steel cage I seeme
$5A   fact:    omb1-6-flag1                  \ ? after a scene
$5B   fact:    c1f-14-flag2                  \ ?
$5C   fact:    c3f-2-flag1                   \ ?
$5D   fact:    omb1-6-flag2                  \ ?
$5E   fact:    omb1-6-flag3                  \ ?
$5F   fact:    omb1-6-flag4                  \ ?
$60   fact:    omb1-6-flag5                  \ ?
$61   fact:    c2f-9-flag2                   \ ? "Miss... It's cleaning time now."
$62   fact:    om1f-15-flag1                 \ ?
$63   fact:    got-cathedral-secrets         \ (old-mansion-1f-15)
$64   fact:    got-occult-philosophy         \ (old-mansion-1f-15)
$65   fact:    got-wisemans-crafts           \ (old-mansion-1f-15)
$67   fact:    lectern-0-book-1              \ (old-mansion-1f-15)
$68   fact:    lectern-0-book-2              \ (old-mansion-1f-15)
$69   fact:    lectern-1-book-0              \ (old-mansion-1f-15)
$6A   fact:    lectern-1-book-1              \ (old-mansion-1f-15)
$6B   fact:    lectern-1-book-2              \ (old-mansion-1f-15)
$6C   fact:    lectern-2-book-0              \ (old-mansion-1f-15)
$6D   fact:    lectern-2-book-1              \ (old-mansion-1f-15)
$6E   fact:    lectern-2-book-2              \ (old-mansion-1f-15)
$6F   fact:    x-37-flag1                    \ ?
$70   fact:    om1f-12-flag4                 \ ?
$71   fact:    found-file-233                \ (old-mansion-1f-1)
$72   fact:    om2f-11-flag1                 \ ?
$73   fact:    got-torn-letter               \ (old-mansion-1f-12)
$74   fact:    film-reel-used-om1f-14        \ (old-mansion-1f-14)
$75   fact:    om1f-13-flag2                 \ ?
$76   fact:    white-mandragora-used         \ (old-mansion-1f-8)
$77   fact:    om1f-4-flag1                  \ ?
$78   fact:    om2f-5-flag1                  \ ? set on entering
$79   fact:    om1f-8-flag1                  \ ? set on entering
$7A   fact:    om2f-10-flag1                 \ ? set on entering
$7B   fact:    om1f-6-flag1                  \ ? set on entering
$7C   fact:    om1f-19-flag1                 \ ? set on entering
$7D   fact:    om1f-21-flag1                 \ ? set on entering
$7E   fact:    om1f-20-breakable-broken      \
$7F   fact:    got-mars-key                  \ (old-mansion-3f-1)
$80   fact:    om1f-8-flag2                  \ ? after a scene; "It doesn't seem too terribly dangerous... But I get the fee
$81   fact:    om2f-11-flag2                 \ ? set on entering
$84   fact:    om2f-2-flag1                  \ ? after a scene
$86   fact:    found-file-247                \ (chaos-forest-8)
$87   fact:    cf-10-flag1                   \ ? after a scene; "You've been shot. You'll be all right. This will stop the b
$88   fact:    cf-13-flag1                   \ ?
$89   fact:    x-37-flag2                    \ ?
$8A   fact:    cf-11-flag1                   \ ?
$8B   fact:    wt2f-1-flag1                  \ ?
$8C   fact:    wt4f-1-flag1                  \ ?
$8D   fact:    wt2f-1-flag2                  \ ? never set by a room
$91   fact:    wt8f-1-flag1                  \ ?
$93   fact:    wt8f-1-flag2                  \ ? after a scene; "Riccardo... You were always... incomplete. But now, I will
$96   fact:    found-file-257                \ (house-of-truth-1f-5)
$97   fact:    ht1f-3-flag1                  \ ?
$98   fact:    found-file-252                \ (house-of-truth-b1-2)
$99   fact:    ht1f-1-flag1                  \ ?
$9A   fact:    found-file-250                \ (house-of-truth-1f-4)
$9B   fact:    ht1f-4-flag1                  \ ? after a scene
$9C   fact:    got-sol-key                   \ (house-of-truth-1f-4)
$9D   fact:    got-golden-candlestick        \ (house-of-truth-1f-3)
$9E   fact:    got-cane-of-caduceus          \ (house-of-truth-1f-7)
$9F   fact:    ht1f-7-flag1                  \ ? after a scene
$A0   fact:    ht1f-7-flag2                  \ ? after a scene
$A1   fact:    found-file-255                \ (house-of-truth-1f-9)
$A2   fact:    ht1f-9-flag1                  \ ?
$A3   fact:    ht1f-9-flag2                  \ ?
$A5   fact:    ht1f-7-flag3                  \ ?
$A6   fact:    ht1f-8-flag1                  \ ?
$A7   fact:    htb1-3-flag1                  \ ? set on entering
$A8   fact:    htb1-10-flag1                 \ ?
$A9   fact:    htb1-11-flag1                 \ ?
$AA   fact:    htb1-12-flag1                 \ ?
$AB   fact:    htb1-11-flag2                 \ ?
$AC   fact:    ht1f-1-flag2                  \ ? set on entering
$AE   fact:    fg-3-flag3                    \ ? set on entering
$200  fact:    c1f-5-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$201  fact:    cb1-7-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$202  fact:    c2f-12-breakable-broken       \ broken by a blow in its zone: shards, its model off, the way open
$203  fact:    cb1-1-breakable1-broken       \ broken by a blow in its zone: shards, its model off, the way open
$204  fact:    c1f-11-breakable1-broken      \ broken by a blow in its zone: shards, its model off, the way open
$205  fact:    c1f-11-breakable2-broken      \ broken by a blow in its zone: shards, its model off, the way open
$206  fact:    c1f-11-breakable3-broken      \ broken by a blow in its zone: shards, its model off, the way open
$207  fact:    c1f-12-breakable-broken       \ broken by a blow in its zone: shards, its model off, the way open
$209  fact:    c2f-8-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$20A  fact:    c1f-2-breakable1-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20B  fact:    c1f-2-breakable2-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20C  fact:    c1f-9-breakable1-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20D  fact:    c1f-9-breakable2-broken       \ broken by a blow in its zone: shards, its model off, the way open
$20E  fact:    fg-1-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$20F  fact:    fg-2-breakable1-broken        \ broken by a blow in its zone: shards, its model off, the way open
$210  fact:    fg-2-breakable2-broken        \ broken by a blow in its zone: shards, its model off, the way open
$211  fact:    fg-2-breakable3-broken        \ broken by a blow in its zone: shards, its model off, the way open
$212  fact:    fg-5-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$213  fact:    fg-4-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$214  fact:    fg-3-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$215  fact:    cf-8-breakable1-broken        \ broken by a blow in its zone: shards, its model off, the way open
$216  fact:    cf-8-breakable2-broken        \ broken by a blow in its zone: shards, its model off, the way open
$217  fact:    cf-8-breakable3-broken        \ broken by a blow in its zone: shards, its model off, the way open
$218  fact:    cf-8-breakable4-broken        \ broken by a blow in its zone: shards, its model off, the way open
$219  fact:    cf-8-breakable5-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21A  fact:    cf-8-breakable6-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21B  fact:    cf-8-breakable7-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21C  fact:    cf-9-breakable-broken         \ broken by a blow in its zone: shards, its model off, the way open
$21D  fact:    cf-10-breakable-broken        \ broken by a blow in its zone: shards, its model off, the way open
$21E  fact:    om1f-21-breakable1-broken     \
$21F  fact:    c1f-14-breakable1-broken      \ broken by a blow in its zone: shards, its model off, the way open
$220  fact:    c1f-14-breakable2-broken      \ broken by a blow in its zone: shards, its model off, the way open
$221  fact:    c1f-14-breakable3-broken      \ broken by a blow in its zone: shards, its model off, the way open
$222  fact:    fg-4-camomile                 \ (front-garden-4)
$223  fact:    c1f-3-camomile                \ (castle-1f-3)
$225  fact:    cb1-2-camomile                \ (castle-b1-2)
$226  fact:    c1f-16-magnesia               \ (castle-1f-16)
$227  fact:    c1f-17-lavender               \ (castle-1f-17)
$228  fact:    c1f-10-beef-jerky             \ (castle-1f-10)
$229  fact:    fg-3-magnesia                 \ (front-garden-3)
$22A  fact:    c1f-11-magnesia               \ (castle-1f-11)
$22B  fact:    c1f-14-chicken-jerky          \ (castle-1f-14)
$22C  fact:    cb1-1-chicken-jerky           \ (castle-b1-1)
$22D  fact:    cb1-7-medallion               \ (castle-b1-7)
$22E  fact:    om1f-17-breakable1-broken     \
$22F  fact:    om1f-17-breakable2-broken     \
$230  fact:    c1f-9-album-medallion         \ (castle-1f-9)
$231  fact:    fg-3-refined-magnesia         \ (front-garden-3)
$232  fact:    c2f-13-flag1                  \ ? never set by a room
$233  fact:    om2f-10-breakable-broken      \
$234  fact:    c1f-1-refined-magnesia        \ (castle-1f-1)
$235  fact:    c1f-11-medallion              \ (castle-1f-11)
$236  fact:    c1f-19-rubrum-medallion       \ (castle-1f-19)
$237  fact:    omb1-2-breakable1-broken      \
$238  fact:    omb1-2-breakable2-broken      \
$239  fact:    c1f-4-refined-antimony        \ (castle-1f-4)
$23A  fact:    fg-5-flag1                    \ ?
$23B  fact:    fg-5-viola-medallion          \ (front-garden-5)
$23C  fact:    fg-1-medallion-found          \ Hewie sniffs it out (his shared script $8B sets this); taking it: fg-1-medallion
$23D  fact:    fg-1-medallion                \ (front-garden-1)
$23E  fact:    fg-5-medallion-found          \ Hewie sniffs it out (his shared script $8B sets this); taking it: fg-5-medallion
$23F  fact:    fg-5-medallion                \ (front-garden-5)
$240  fact:    fg-4-album-medallion-found    \ Hewie sniffs it out (his shared script $8B sets this); taking it: fg-4-album-medallion
$241  fact:    fg-4-album-medallion          \ (front-garden-4)
$242  fact:    c1f-6-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-6-medallion
$243  fact:    c1f-6-medallion               \ (castle-1f-6)
$244  fact:    cb1-1-lavender-found          \ Hewie sniffs it out (his shared script $8B sets this); taking it: cb1-1-lavender
$245  fact:    cb1-1-lavender                \ (castle-b1-1)
$246  fact:    cb1-2-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: cb1-2-medallion
$247  fact:    cb1-2-medallion               \ (castle-b1-2)
$248  fact:    c1f-9-rubrum-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-9-rubrum-medallion
$249  fact:    c1f-9-rubrum-medallion        \ (castle-1f-9)
$24A  fact:    c2f-2-album-medallion-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-2-album-medallion
$24B  fact:    c2f-2-album-medallion         \ (castle-2f-2)
$24C  fact:    c2f-1-refined-magnesia-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-1-refined-magnesia
$24D  fact:    c2f-1-refined-magnesia        \ (castle-2f-1)
$24E  fact:    c1f-2-rubrum-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-2-rubrum-medallion
$24F  fact:    c1f-2-rubrum-medallion        \ (castle-1f-2)
$250  fact:    c1f-13-antimony-powder-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-13-antimony-powder
$251  fact:    c1f-13-antimony-powder        \ (castle-1f-13)
$252  fact:    cb1-7-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: cb1-7-medallion
$253  fact:    cb1-7-medallion-2             \ (castle-b1-7)
$254  fact:    c1f-15-camomile-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-15-camomile
$255  fact:    c1f-15-camomile               \ (castle-1f-15)
$256  fact:    c1f-15-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-15-medallion
$257  fact:    c1f-15-medallion              \ (castle-1f-15)
$258  fact:    c2f-12-refined-antimony-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-12-refined-antimony
$259  fact:    c2f-12-refined-antimony       \ (castle-2f-12)
$25A  fact:    c1f-14-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-14-viride-medallion
$25B  fact:    c1f-14-viride-medallion       \ (castle-1f-14)
$25C  fact:    c1f-8-album-medallion-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: c1f-8-album-medallion
$25D  fact:    c1f-8-album-medallion         \ (castle-1f-8)
$25E  fact:    c2f-3-medallion-found         \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-3-medallion
$25F  fact:    c2f-3-medallion               \ (castle-2f-3, castle-2f-5)
$260  fact:    c2f-13-medallion              \ (castle-2f-13, castle-2f-14, castle-2f-15)
$261  fact:    om2f-5-breakable1-broken      \
$262  fact:    om2f-5-breakable2-broken      \
$263  fact:    om1f-24-breakable-broken      \
$264  fact:    omb1-1-breakable-broken       \
$265  fact:    om2f-8-camomile               \ (old-mansion-2f-8)
$266  fact:    om2f-8-lavender               \ (old-mansion-2f-8)
$267  fact:    om1f-17-chicken-jerky         \ (old-mansion-1f-17)
$268  fact:    om1f-9-refined-antimony       \ (old-mansion-1f-9)
$269  fact:    omb1-2-refined-magnesia       \ (old-mansion-b1-2)
$26A  fact:    omb1-3-camomile               \ (old-mansion-b1-3)
$26B  fact:    got-sylvesta-esca             \ (old-mansion-2f-9)
$26C  fact:    om2f-11-magnesia              \ (old-mansion-2f-11)
$26D  fact:    om1f-14-chicken-jerky         \ (old-mansion-1f-14)
$26E  fact:    om1f-8-beef-jerky             \ (old-mansion-1f-8)
$26F  fact:    om1f-5-lavender               \ (old-mansion-1f-5, old-mansion-1f-8)
$270  fact:    om1f-8-album-medallion        \ (old-mansion-1f-8)
$271  fact:    omb1-6-medallion              \ (old-mansion-b1-6)
$272  fact:    om1f-16-refined-magnesia      \ (old-mansion-1f-16)
$273  fact:    om1f-15-refined-antimony      \ (old-mansion-1f-15)
$274  fact:    om2f-10-weeping-stone         \ (old-mansion-2f-10)
$275  fact:    om1f-6-medallion              \ (old-mansion-1f-6)
$276  fact:    om2f-6-lavender               \ (old-mansion-2f-6)
$277  fact:    om1f-3-medallion              \ (old-mansion-1f-3)
$278  fact:    om1f-4-chicken-jerky          \ (old-mansion-1f-4)
$27A  fact:    om1f-2-camomile               \ (old-mansion-1f-2)
$27C  fact:    om2f-4-viride-medallion       \ (old-mansion-2f-4)
$27D  fact:    om1f-24-rubrum-medallion      \ (old-mansion-1f-24)
$27E  fact:    om1f-20-weeping-stone         \ (old-mansion-1f-20)
$27F  fact:    om1f-22-refined-antimony      \ (old-mansion-1f-22)
$280  fact:    fg-1-medallion-2              \ (front-garden-1)
$281  fact:    cb1-1-chicken-jerky-2         \ (castle-b1-1)
$282  fact:    c2f-1-lavender                \ (castle-2f-1)
$283  fact:    fg-2-medallion                \ (front-garden-2)
$284  fact:    fg-5-beef-jerky               \ (front-garden-5)
$285  fact:    c1f-2-viola-medallion         \ (castle-1f-2)
$286  fact:    got-onion                     \ (castle-1f-2)
$287  fact:    c1f-11-beef-jerky             \ (castle-1f-11)
$288  fact:    c1f-9-magnesia                \ (castle-1f-9)
$289  fact:    c1f-12-medallion              \ (castle-1f-12)
$28A  fact:    c1f-14-antimony-powder        \ (castle-1f-14)
$28B  fact:    c1f-5-antimony-powder         \ (castle-1f-5)
$28C  fact:    c2f-12-medallion              \ (castle-2f-12)
$28D  fact:    om2f-5-medallion              \ (old-mansion-2f-5)
$28E  fact:    om1f-21-album-medallion       \ (old-mansion-1f-21)
$28F  fact:    omb1-1-chicken-jerky          \ (old-mansion-b1-1)
$290  fact:    omb1-2-viola-medallion        \ (old-mansion-b1-2)
$291  fact:    omb1-2-rubrum-medallion       \ (old-mansion-b1-2)
$292  fact:    om1f-24-beef-jerky            \ (old-mansion-1f-24)
$293  fact:    om1f-17-magnesia              \ (old-mansion-1f-17)
$294  fact:    c1f-13-chicken-jerky          \ (castle-1f-13)
$295  fact:    c1f-8-magnesia                \ (castle-1f-8)
$296  fact:    c2f-6-lavender                \ (castle-2f-6)
$297  fact:    ht2f-1-breakable1-broken      \
$298  fact:    ht1f-2-breakable-broken       \
$299  fact:    om1f-11-album-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-11-album-medallion
$29A  fact:    om1f-11-album-medallion       \ (old-mansion-1f-11)
$29B  fact:    om1f-17-medallion-found       \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-17-medallion
$29C  fact:    om1f-17-medallion             \ (old-mansion-1f-17)
$29D  fact:    om1f-9-refined-magnesia-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-9-refined-magnesia
$29E  fact:    om1f-9-refined-magnesia       \ (old-mansion-1f-9)
$29F  fact:    om1f-23-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-23-viride-medallion
$2A0  fact:    om1f-23-viride-medallion      \ (old-mansion-1f-23)
$2A1  fact:    om1f-23-medallion-found       \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-23-medallion
$2A2  fact:    om1f-23-medallion             \ (old-mansion-1f-23)
$2A3  fact:    om1f-12-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-12-rubrum-medallion
$2A4  fact:    om1f-12-rubrum-medallion      \ (old-mansion-1f-12)
$2A5  fact:    omb1-7-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: omb1-7-medallion
$2A6  fact:    omb1-7-medallion              \ (old-mansion-b1-7)
$2A7  fact:    om1f-15-antimony-tube-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-15-antimony-tube
$2A8  fact:    om1f-15-antimony-tube         \ (old-mansion-1f-15)
$2A9  fact:    omb1-6-viola-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: omb1-6-viola-medallion
$2AA  fact:    omb1-6-viola-medallion        \ (old-mansion-b1-6)
$2AB  fact:    om1f-5-refined-magnesia-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-5-refined-magnesia
$2AC  fact:    om1f-5-refined-magnesia       \ (old-mansion-1f-5)
$2AD  fact:    om2f-13-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-13-rubrum-medallion
$2AE  fact:    om2f-13-rubrum-medallion      \ (old-mansion-2f-13)
$2AF  fact:    om2f-12-antimony-powder-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-12-antimony-powder
$2B0  fact:    om2f-12-antimony-powder       \ (old-mansion-2f-12)
$2B1  fact:    om1f-15-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-15-rubrum-medallion
$2B2  fact:    om1f-15-rubrum-medallion      \ (old-mansion-1f-15)
$2B3  fact:    om1f-20-medallion-found       \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-20-medallion
$2B4  fact:    om1f-20-medallion             \ (old-mansion-1f-20)
$2B5  fact:    om1f-20-medallion-sniffed-2   \
$2B6  fact:    om1f-20-medallion-2           \ (old-mansion-1f-20)
$2B7  fact:    om1f-20-medallion-sniffed-3   \
$2B8  fact:    om1f-20-medallion-3           \ (old-mansion-1f-20)
$2B9  fact:    om1f-20-prima-magnesia-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-20-prima-magnesia
$2BA  fact:    om1f-20-prima-magnesia        \ (old-mansion-1f-20)
$2BB  fact:    om1f-7-album-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-7-album-medallion
$2BC  fact:    om1f-7-album-medallion        \ (old-mansion-1f-7)
$2BD  fact:    om2f-2-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-2-medallion
$2BE  fact:    om2f-2-medallion              \ (old-mansion-2f-2)
$2BF  fact:    om1f-3-refined-antimony-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om1f-3-refined-antimony
$2C0  fact:    om1f-3-refined-antimony       \ (old-mansion-1f-3)
$2C1  fact:    om2f-5-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: om2f-5-viride-medallion
$2C2  fact:    om2f-5-viride-medallion       \ (old-mansion-2f-5)
$2C3  fact:    omb1-6-magist-medallion       \ (old-mansion-b1-6)
$2C4  fact:    om1f-8-antimony-tube          \ (old-mansion-1f-8)
$2C5  fact:    om2f-3-album-medallion        \ (old-mansion-2f-3)
$2C6  fact:    omb1-1-album-medallion        \ (old-mansion-b1-1)
$2C7  fact:    cf-12-camomile                \ (chaos-forest-12)
$2C8  fact:    cf-8-medallion                \ (chaos-forest-8)
$2C9  fact:    cf-8-chicken-jerky            \ (chaos-forest-8)
$2CA  fact:    cf-10-medallion               \ (chaos-forest-10)
$2CB  fact:    cf-9-refined-antimony         \ (chaos-forest-9)
$2CC  fact:    om2f-12-breakable1-broken     \
$2CD  fact:    om2f-12-breakable2-broken     \
$2CE  fact:    om1f-21-breakable2-broken     \
$2CF  fact:    om1f-21-breakable3-broken     \
$2D0  fact:    om1f-21-breakable4-broken     \
$2D3  fact:    got-metal-boots               \ (castle-b1-3)
$2D6  fact:    wt1f-1-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: wt1f-1-medallion
$2D7  fact:    wt1f-1-medallion              \ (water-tower-1f-1)
$2D8  fact:    wt2f-1-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: wt2f-1-medallion
$2D9  fact:    wt2f-1-medallion              \ (water-tower-2f-1)
$2DA  fact:    wt7f-2-rubrum-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: wt7f-2-rubrum-medallion
$2DB  fact:    wt7f-2-rubrum-medallion       \ (water-tower-7f-2)
$2DE  fact:    wt2f-1-refined-antimony       \ (water-tower-2f-1)
$2DF  fact:    wt4f-1-medallion              \ (water-tower-4f-1)
$2E0  fact:    lp-1-lavender                 \ (lake-pathway-1)
$2E1  fact:    ht1f-10-camomile              \ (house-of-truth-1f-10)
$2E2  fact:    ht1f-3-rubrum-medallion       \ (house-of-truth-1f-3)
$2E3  fact:    htb1-2-lavender               \ (house-of-truth-b1-2)
$2E4  fact:    htb1-5-camomile               \ (house-of-truth-b1-5)
$2E5  fact:    htb1-7-beef-jerky             \ (house-of-truth-b1-7)
$2E6  fact:    ht1f-2-refined-magnesia       \ (house-of-truth-1f-2)
$2E7  fact:    ht1f-7-antimony-tube          \ (house-of-truth-1f-7)
$2E8  fact:    ht1f-2-refined-antimony       \ (house-of-truth-1f-2)
$2E9  fact:    ht1f-1-album-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-1-album-medallion
$2EA  fact:    ht1f-1-album-medallion        \ (house-of-truth-1f-1)
$2EB  fact:    ht2f-1-medallion-found        \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht2f-1-medallion
$2EC  fact:    ht2f-1-medallion              \ (house-of-truth-2f-1)
$2ED  fact:    ht1f-3-viola-medallion-found  \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-3-viola-medallion
$2EE  fact:    ht1f-3-viola-medallion        \ (house-of-truth-1f-3)
$2EF  fact:    ht2f-2-viride-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht2f-2-viride-medallion
$2F0  fact:    ht2f-2-viride-medallion       \ (house-of-truth-2f-2)
$2F1  fact:    ht1f-2-prima-magnesia-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-2-prima-magnesia
$2F2  fact:    ht1f-2-prima-magnesia         \ (house-of-truth-1f-2)
$2F3  fact:    ht1f-8-magist-medallion-found \ Hewie sniffs it out (his shared script $8B sets this); taking it: ht1f-8-magist-medallion
$2F4  fact:    ht1f-8-magist-medallion       \ (house-of-truth-1f-8)
$2F5  fact:    got-ball                      \ (front-garden-5)
$2F6  fact:    ht2f-2-rubrum-medallion       \ (house-of-truth-2f-2)
$2F7  fact:    wt2f-1-breakable-broken       \
$2F8  fact:    wt4f-1-breakable-broken       \
$2F9  fact:    wt4f-1-camomile               \ (water-tower-4f-1)
$2FA  fact:    ht2f-1-breakable2-broken      \
$2FB  fact:    ht2f-1-breakable3-broken      \
$2FC  fact:    ht2f-1-viola-medallion        \ (house-of-truth-2f-1)
$2FD  fact:    cb1-7-medallion-3             \ (castle-b1-7)
$2FE  fact:    c2f-15-prima-magnesia-found   \ Hewie sniffs it out (his shared script $8B sets this); taking it: c2f-15-prima-magnesia
$2FF  fact:    c2f-15-prima-magnesia         \ (castle-2f-15)
$300  fact:    c2f-3-tv-examined             \ "I hope this TV works..."
$301  fact:    c1f-3-bookshelf-examined      \ "A book shelf of fine design..."
$302  fact:    c1f-1-flag2                   \ ?
$303  fact:    c1f-3-flag2                   \ ? after a scene
$304  fact:    c2f-2-flag4                   \ ? "Is this... blood?"
$305  fact:    c2f-12-flag1                  \ ?
$306  fact:    c2f-3-tv-examined-2           \ "I hope this TV works..." (the other)
$307  fact:    c2f-7-flag2                   \ ? "It's a plate pressing machine."
$308  fact:    c1f-5-flag1                   \ ? set on entering
$309  fact:    c1f-16-flag1                  \ ?
$30A  fact:    c1f-16-flag2                  \ ?
$30B  fact:    fg-4-flag1                    \ ?
$30C  fact:    got-jade-choker               \ (castle-2f-12)
$30E  fact:    c2f-12-flag2                  \ ? set on entering
$310  fact:    c1f-4-flag2                   \ ?
$311  fact:    c1f-4-flag3                   \ ?
$312  fact:    c2f-3-portrait-1-examined     \ "I wonder whose portrait this is?"
$313  fact:    c2f-3-portrait-3-examined     \ "What a powerful piece of art..."
$314  fact:    c2f-3-portrait-2-examined     \ "The man pictured here has a noble air..."
$315  fact:    got-obsidian-choker           \ (castle-2f-8)
$316  fact:    c1f-7-flag1                   \ ?
$317  fact:    cb1-5-flag1                   \ ?
$318  fact:    c2f-12-flag3                  \ ?
$319  fact:    c1f-4-flag4                   \ ?
$31A  fact:    cb1-7-flag1                   \ ?
$31B  fact:    cb1-7-flag2                   \ ?
$31C  fact:    c2f-2-flag5                   \ ?
$31D  fact:    c1f-10-flag3                  \ ? after a scene
$31E  fact:    got-doc-recovery              \ (castle-b1-2, house-of-truth-1f-1, house-of-truth-1f-2, old-mansion-1f-9, old-mansion-2f-6, water-tower-1f-1)
$31F  fact:    got-cracked-plate-key         \ (castle-1f-4)
$320  fact:    got-cracked-plate-key-2       \ (castle-1f-6)
$321  fact:    cb1-1-breakable2-broken       \ broken by a blow in its zone: shards, its model off, the way open
$322  fact:    om2f-3-flag1                  \ ?
$323  fact:    om2f-6-flag1                  \ ?
$324  fact:    om2f-6-flag2                  \ ?
$325  fact:    om1f-22-flag1                 \ ?
$326  fact:    omb1-4-flag1                  \ ?
$327  fact:    omb1-5-flag4                  \ ?
$328  fact:    omb1-5-flag5                  \ ?
$329  fact:    omb1-2-breakable3-broken      \
$32A  fact:    omb1-2-flag1                  \ ?
$32B  fact:    om1f-11-breakable-broken      \
$32C  fact:    om1f-11-flag1                 \ ?
$32D  fact:    om2f-13-breakable-broken      \
$32E  fact:    om2f-13-flag1                 \ ?
$330  fact:    omb1-3-flag3                  \ ?
$331  fact:    journals-examined             \ "Mountains of books have been piled up here..." (old-mansion-2f-11)
$332  fact:    red-painting-examined         \ "Why would someone intentionally paint this completely red?" (old-mansion-1f-18)
$333  fact:    om1f-14-flag1                 \ ? after a scene; "What is this? I was being filmed. Who's there? Why are you
$334  fact:    sundial-examined              \ "This is a sundial..." (chaos-forest-8)
$335  fact:    got-mansion-sketch            \ (old-mansion-1f-23)
$336  fact:    got-forest-map                \ (chaos-forest-1)
$337  fact:    film-reel-used-om1f-14-2      \ (old-mansion-1f-14)
$338  fact:    om3f-1-flag3                  \ ?
$339  fact:    mummy-examined                \ "This... isn't a doll! It's a mummy!" (old-mansion-1f-17)
$33A  fact:    wt4f-1-flag2                  \ ?
$33E  fact:    wt8f-1-flag3                  \ ?
$33F  fact:    wt8f-1-flag4                  \ ?
$340  fact:    got-doc-crouch-evasion        \ (castle-1f-1)
$341  fact:    wt2f-1-flag3                  \ ? set on entering
$342  fact:    wt4f-1-flag3                  \ ? set on entering
$343  fact:    got-mystery-sketch            \ (house-of-truth-2f-2)
$344  fact:    cf-12-flag1                   \ ?
$345  fact:    htb1-5-flag1                  \ ? set on entering
$346  fact:    htb1-6-flag1                  \ ? set on entering
$347  fact:    htb1-7-flag1                  \ ? set on entering
$348  fact:    htb1-8-flag1                  \ ? set on entering
$349  fact:    got-bisque-doll               \ (castle-2f-2)
$34A  fact:    wt7f-1-flag1                  \ ? set on entering
$34B  fact:    got-doc-action                \ (castle-2f-3, off-map-27)
$34C  fact:    wt2f-1-flag4                  \ ?
$34D  fact:    wt4f-1-flag4                  \ ?
$34E  fact:    tremor-warned                 \ "Whoa! What was that? It felt like a tremor..." (house-of-truth-1f-8)
$34F  fact:    tremor-warned-2               \ "Whoa! What was that?..." (house-of-truth-1f-8, the other)
$350  fact:    found-file-254                \ (house-of-truth-b1-4)
$351  fact:    shared-flag1                  \ ?
$352  fact:    shared-flag2                  \ ?
$353  fact:    shared-flag3                  \ ?
$354  fact:    shared-flag4                  \ ?
$35B  fact:    x-d5-flag1                    \ ?
$35C  fact:    x-d8-flag1                    \ ?
$35D  fact:    cf-9-flag1                    \ ?
$35E  fact:    x-d7-flag1                    \ ?
$360  fact:    got-red-godstone              \ (old-mansion-1f-4)
$361  fact:    got-handful-of-ash            \ (old-mansion-1f-2)
$362  fact:    got-yellow-mandragora         \ (old-mansion-1f-13)
$363  fact:    got-red-mandragora            \ (old-mansion-1f-13)
$364  fact:    got-blue-mandragora           \ (old-mansion-1f-13)
$365  fact:    got-purple-mandragora         \ (old-mansion-1f-13)
$366  fact:    got-hebels-perfume            \ (old-mansion-1f-24)
$368  fact:    x-27-magist-medallion         \ (off-map-27)
$369  fact:    x-27-magist-medallion-2       \ (off-map-27)
$36A  fact:    x-27-magist-medallion-3       \ (off-map-27)
$36B  fact:    x-27-magist-medallion-4       \ (off-map-27)

