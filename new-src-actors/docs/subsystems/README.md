# Subsystem pages

One page per subsystem: its living document. It records the API agreed so far and the design
notes, and is updated whenever the subsystem changes. Every page has these sections:

- **Purpose**: what it is for, in game terms.
- **API**: the messages it takes and sends, each with its stack comment and meaning. Facts it
  reads (C words). This is the contract; change it here first.
- **State**: what it owns.
- **Rules**: how it behaves, written from the original with references (`src/game/...`,
  function names). Quirks of the original we keep are marked *quirk*.
- **Design notes**: decisions, alternatives, open questions.
- **Status**: spec / built / checked, and what is left.
