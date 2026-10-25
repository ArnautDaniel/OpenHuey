\ Bug reports (debug.fs): `bug <what happened>` keeps the moment - a folder of its own with the
\ report, the note and a save (the screenshot too, but a test draws nothing), all zipped beside it
\ (again with each note) - and the save loads back as it was. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/debug/test_bug_report.fs
IN: test-bug-report
USING: tester engine game-state actors messages room-names game debug ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: her-x ( -- n )  game:fiona body-pos fdrop fdrop fround f>s ;
: there? ( name len -- flag )  in-bug-dir file-exists? ;
60 frames

testing a report: the folder, its report and note, then its save
her-x constant reported-x
bug Hewie walked through a wall
T{ 3 frames  s" report.txt" there?  s" note.txt" there?  s" save.fs" there? -> -1 -1 -1 }T
T{ bug-step @ -> 0 }T
testing ... zipped beside the folder, each file under the folder's name
create in-zip 300 allot  variable in-zip-len
: zipped? ( name len -- flag )   \ (<folder>/name in the report's zip)
    {: a n :}  bug-folder {: f fn :}
    f in-zip fn move  [char] / in-zip fn + c!  a in-zip fn 1+ + n move  fn 1+ n + in-zip-len !
    in-zip in-zip-len @  bug-zip-path zip-has? ;
T{ bug-zip-path file-exists? -> -1 }T
T{ s" report.txt" zipped?  s" note.txt" zipped?  s" save.fs" zipped? -> -1 -1 -1 }T
testing ... a report without a note (F12's), then one added: zipped again with it
1 bug-step !  3 frames
T{ s" report.txt" zipped?  s" note.txt" zipped? -> -1 0 }T
bug-note it happened again
T{ s" note.txt" zipped? -> -1 }T

testing the save loaded back: she is where she was
10e 0e 0e tp  5 frames
T{ her-x reported-x = -> 0 }T
s" save.fs" in-bug-dir bug-load  10 frames
T{ her-x -> reported-x }T

testing every actor's state, by its fields
T{ game:fiona alive? -> -1 }T
game:fiona .state

test-summary
