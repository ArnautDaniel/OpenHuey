\ keys.fs - reading the keyboard: held now, or pressed this tick (scancodes from `key: Name`).
IN: keys
USING: engine ;

: held? ( scancode -- flag ) key-down? ;
: pressed? ( scancode -- flag ) key-pressed? ;
