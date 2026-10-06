\ vocabularies: IN:, USING:, USE:, <PRIVATE PRIVATE>, vocab:name, ambiguity, cycles
IN: test-vocab
USING: tester vocabs.va ;

testing a used vocabulary
T{ greet tell -> 1 99 }T

testing private words stay private
: try-secret s" secret" evaluate ;
T{ ' try-secret catch -> -1 }T
T{ vocabs.va.private:secret -> 99 }T

testing a vocabulary loads once
USE: vocabs.vb
T{ greet2 loads @ -> 2 1 }T

testing qualified names
T{ vocabs.vb:shared vocabs.va:shared -> 20 10 }T

testing two vocabularies with one name
: try-shared s" shared" evaluate ;
T{ ' try-shared catch -> -1 }T
T{ error-message drop 6 s" shared" s= -> true }T

testing our own definitions win
: shared 30 ;
T{ shared -> 30 }T

testing vocabularies using each other
: try-circle s" USING: vocabs.vc ;" evaluate ;
T{ ' try-circle catch -> -1 }T

testing a missing vocabulary
: try-missing s" USING: no.such.thing ;" evaluate ;
T{ ' try-missing catch -> -1 }T

test-summary
