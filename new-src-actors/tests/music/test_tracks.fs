\ The streamed tracks (platform/music.c, data/musicsrc.c): a track opens and plays - from the
\ library's FLAC when there is a library (ADX00\AD_01.flac for ADX00\AD_01.ADX), else the ADX;
\ one that isn't there doesn't. (Their samples, FLAC against ADX: music-check.) Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/music/test_tracks.fs
IN: test-tracks
USING: tester engine ;

testing a track plays, looped or not; stopped, it doesn't
T{ s" ADX00\AD_01.ADX" -1 0 music-play  music-playing? -> -1 -1 }T
music-stop
T{ music-playing? -> 0 }T
T{ s" ADX02/AD_60.ADX" 0 0 music-play  music-playing? -> -1 -1 }T
music-stop
testing one that isn't there
T{ s" ADX00\AD_99.ADX" -1 0 music-play  music-playing? -> 0 0 }T

test-summary
