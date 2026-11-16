\ The languages (scripts/languages.fs, core/files.h): a European disc's library has the game's
\ words and its pictures with words in them in five languages. Here a mod gives the English
\ library a little French as such a library has it (tests/title/languages-mod): the title's
\ LANGUAGE, picking French there, the words and the art in French - English where the language has
\ no file of its own - and back. Run with --title and the mod (CMakeLists.txt):
\   build/openhuey-src/openhuey --title --mod openhuey-src/tests/title/languages-mod --test openhuey-src/tests/title/languages.fs
IN: test-languages
USING: tester engine game-state actors keys game opening languages debug ;

: of-title ( xt -- x )  game:opening enter  execute  leave-actor ;
: tap ( key -- )  dup true key-hold  1 run-frames  false key-hold  1 run-frames ;
: first-line ( id -- addr len )  message-layout drop  0 0 message-line ;
$2D constant avoid-art   \ (the avoid prompt's group: avoid.fs - SYSTEM/AVOID.TEX, loaded as the game starts)
$C001 constant sub-message   \ (MSG_SUB's: the mod has no French of it)
create english 96 allot   variable english-n
: keep ( addr len -- )  dup english-n !  english swap move ;

5 run-frames   \ (the title up, its menu's entries made, the avoid prompt's art in)
error-count constant before
testing the languages there: English, and the mod's French; none set yet
T{ 0 language-present?  1 language-present?  2 language-present? -> -1 -1 0 }T
T{ languages:languages?  language -> -1 0 }T
T{ avoid-art 0 tex-info drop -> 512 256 }T
sub-message first-line keep
testing a language the library hasn't is English
T{ 2 language!  language -> 0 }T

testing the title's LANGUAGE, after OPTIONS
T{ 3 entry -> e-language }T
T{ key: Down tap  key: Down tap  key: Down tap  [: picked @ ;] of-title -> 3 }T
T{ key: X tap  [: stage @ ;] of-title  languages:at @ -> on-language 0 }T
testing the cursor: down to French, no further (the mod has no German)
T{ key: Down tap  languages:at @ -> 1 }T
T{ key: Down tap  languages:at @ -> 1 }T
testing the cross sets it, back to the menu
T{ key: X tap  language  [: stage @ ;] of-title -> 1 on-menu }T

testing the words in French: the system messages (the font's "â" for UTF-8's), the room's
T{ $8000 first-line s" Bonjour" compare -> 0 }T
T{ $8001 first-line  swap 2 + c@  swap -> $A2 7 }T
T{ 0 first-line s" Le jardin" compare -> 0 }T
testing English where French has no file: MSG_SUB
T{ sub-message first-line english english-n @ compare -> 0 }T
testing the art in French: the avoid prompt's bank loaded again, the mod's
T{ avoid-art 0 tex-info drop -> 64 32 }T

testing back to English: its words and its art
T{ 0 language!  language -> 0 }T
T{ $8000 first-line s" Bonjour" compare 0= -> 0 }T
T{ avoid-art 0 tex-info drop -> 512 256 }T
T{ error-count before - -> 0 }T

test-summary
