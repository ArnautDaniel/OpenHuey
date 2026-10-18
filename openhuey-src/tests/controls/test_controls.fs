\ The controls (platform.c, the settings menu's CONTROLS: scripts/graphics.fs): each of the
\ game's actions is the key the scripts read, held or pressed through the keys bound to it - by
\ default itself, and Space for cross (X), Backspace for circle (C). A key bound elsewhere is
\ taken from where it was; a key no action is reaches the scripts as itself.
IN: test-controls
USING: tester engine keys ;

5 constant run   4 constant act   10 constant pause

testing the defaults (whatever the player's controls.fs set)
controls-reset
T{ run control-key  run 0 control-binding  run 1 control-binding -> key: X key: X key: Space }T
T{ act 1 control-binding  pause 1 control-binding -> key: Backspace 0 }T
T{ key: X key-name s" X" compare -> 0 }T
testing a key pressed: the actions bound to it
T{ key: Space key-push  cross button-pressed?  key: Space key-pressed? -> -1 -1 }T   \ (Space: no action's own key)
T{ key: Backspace key-push  circle button-pressed? -> -1 }T
testing a new key for pause: its old one second; the key taken from run
T{ key: P pause 0 control-bind  pause 0 control-binding  pause 1 control-binding -> key: P key: Return }T
T{ key: Space pause 0 control-bind  run 0 control-binding run 1 control-binding -> key: X 0 }T
T{ key: Space key-push  start-button button-pressed?  cross button-pressed? -> -1 0 }T
testing run moved to R: X pressed is nothing
T{ key: R run 0 control-bind  run 0 control-binding run 1 control-binding -> key: R key: X }T
T{ 0 run 1 control-bind  run 1 control-binding -> 0 }T
T{ key: X key-push  cross button-pressed? -> 0 }T
T{ key: R key-push  cross button-pressed? -> -1 }T
testing the defaults back
T{ controls-reset  run 0 control-binding run 1 control-binding  pause 0 control-binding -> key: X key: Space key: Return }T

test-summary
