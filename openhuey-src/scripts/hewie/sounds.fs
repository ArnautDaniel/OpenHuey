\ hewie/sounds.fs - Hewie's sounds by name (his bank: named for where the original plays them,
\ src/game/hewie.c Hewie_MakeSound / Hewie_AnimSounds). Made with `make-sound` (hewie/model.fs:
\ not too soon after the last).
IN: hewie.sounds

$1E constant splash-sound          \ into water (bank 6)
$58 constant upset-whine           \ in the upset stance
$59 constant hurt-whimper          \ standing, lying or limping hurt
$5A constant bark-sound
$5C constant cowed-whine           \ ears down, scolded
$5D constant growl-sound           \ barking, tense
$5E constant upset-growl           \ barking, upset
$5F constant limping-bark
$60 constant whine-sound           \ to her; refusing
$65 constant shoved-yelp           \ (loud: heard about)
$66 constant yelp-sound            \ (loud: heard about)
$68 constant leap-snarl            \ springing at someone
$69 constant scuffle-yelp          \ heard from next door, not hurt
$6B constant bite-sound            \ his teeth in the stalker
$6C constant snap-sound            \ a bite landed on a creature
$6F constant stance-growl
$70 constant ready-growl           \ crouched to spring
