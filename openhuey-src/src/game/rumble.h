/* The pad's rumble (src/game/pad.c Rumble_*): a few channels - who asks - each holding the small motor
 * on and the large one at a strength for some frames; each tick the strongest of them goes to the pad.
 * The last channel plays a sequence of steps one after another (the panic's beat with her breath). */
#ifndef RUMBLE_H
#define RUMBLE_H

enum { RUMBLE_FIONA, RUMBLE_STORY, RUMBLE_SCENE, RUMBLE_STALKER, RUMBLE_SEQUENCE, RUMBLE_CHANNELS };

/* the large motor at `strength` (0..255) for `frames` frames */
void rumble_jolt(int channel, int strength, int frames);
/* the small motor on (or off) for `frames` frames */
void rumble_buzz(int channel, int on, int frames);
/* the large motor going from one strength to another over `frames` frames */
void rumble_swell(int channel, int from, int to, int frames);
/* the sequence: emptied (stopping it), and a step added - the small motor, the large one going from
 * one strength to another, over `frames` frames - played once the steps before it are done */
void rumble_sequence_clear(void);
void rumble_sequence_add(int small_on, int from, int to, int frames);
/* all of it stopped, or let go on again (the pause, the menus) */
void rumble_enable(int on);
/* the vibration allowed at all (the options), whatever the pause says */
void rumble_allow(int on);
void rumble_clear(void);
/* a tick: the channels run, the strongest sent to the pad */
void rumble_tick(void);
/* (tests) the strengths it sends now: the small motor 0 / 1, the large 0..255 */
void rumble_now(int *small_on, int *large);

#endif
