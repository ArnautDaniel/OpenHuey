\ The title's UPDATE (scripts/updater.fs): there for a build that updates itself (here: given
\ OPENHUEY_UPDATE_URL, CMakeLists.txt - a release's always is), before QUIT; its page drawn as
\ the update goes, which ends saying why when there is no release where it looks (that URL: no
\ such folder). The update itself: tests/update/run.sh.
IN: test-update-title
USING: tester engine game-state opening updater ;

testing the entry, before QUIT
T{ updater? -> -1 }T
T{ entries!  #items 1- entry  #items 2 - entry -> e-quit e-update }T
T{ #items 2 - item s" UPDATE" compare -> 0 }T
testing the page: not begun, then begun - drawn at each - and failing, saying why
T{ update-state -> not-begun }T
T{ menu-layer updater:draw  update-state -> not-begun }T
T{ updater:open  updater:busy? update-state failed = or -> -1 }T
: settled ( -- state )  begin  update-state  dup looking < over installing > or 0= while  drop  repeat ;
T{ settled -> failed }T
T{ menu-layer updater:draw  update-text nip 0> -> -1 }T
T{ update-text drop 33  s" the newest version can't be found" compare -> 0 }T
testing a failed one can be tried again; the cross does nothing until it has updated
T{ updater:open  settled -> failed }T
T{ updater:keys drop  update-state -> failed }T

test-summary
