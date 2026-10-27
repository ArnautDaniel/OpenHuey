\ The marker effect (house-of-truth-1f-5's scene: Lorenzo's end): made on the figure, sized, done -
\ left to go out on its own (the place forgets it). Headless:
\   build/openhuey --test tests/story/test_marker.fs
IN: test-marker
USING: tester engine game-state actors messages common room-names game debug story.kit
  story.places.house-of-truth-1f-5 ;
: in-room ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: marker-id ( -- id )  [: marker-fx effect @ ;] in-room ;
variable was
free-play  house-of-truth-1f-5 room!  20 run-frames
end-figure 3 [: figure-in ;] in-room  5 run-frames
T{ [: marker-in ;] in-room  marker-id alive? -> -1 }T
5 run-frames
T{ [: 500 marker-to ;] in-room  5 run-frames  marker-id alive? -> -1 }T
marker-id was !
T{ [: marker-done  marker-fx effect @ ;] in-room -> 0 }T
T{ [: was @ alive? 0= ;] 120 soon -> -1 }T   \ (gone out on its own)
test-summary
