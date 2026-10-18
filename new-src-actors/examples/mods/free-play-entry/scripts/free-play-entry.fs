\ free-play-entry: FREE PLAY on the title - the house to roam, past the opening, Hewie at her side
\ (the game's own `free-play`: the opening done, the grate open, Hewie hers to command).
\
\ The sample of the title's hook: the title asks `title-menu-hook` for the mods' entries. The mod
\ keeps the hook it replaces (other mods' entries: `action-of`), calls it, then adds its own.
IN: free-play-entry
USING: engine opening game ;

action-of title-menu-hook constant others
: roam ( -- )  title-gone  free-play ;   \ (picked: the title away, then the game's free play)
: entries ( -- )  others execute  s" FREE PLAY" ['] roam title-entry ;
' entries is title-menu-hook
