/* The music sequences: what the PS2's sound driver does for the stage music (Sony's modmidi and
 * modhsyn under Capcom's SNDDRV; the decomp's native/platform/seq.c and spu2.c). Four ports, each
 * playing an SQ file (MIDI with running status; a set top bit on an event's last data byte: no
 * delta time follows; one-byte note-offs; NRPN 20 / 30 loop marks) on a bank (NAME.HD programs,
 * splits and samples; NAME.BD the PS-ADPCM samples). Notes become voices with the SPU's pitch,
 * ADSR envelope and loop points, mixed into the sound output.
 *
 * The stage music director (Forth) drives the ports: their sequence volume (0..127), their
 * volume (0..255), MIDI messages to their channels (volume, pan, bend) and muting. */
#ifndef SEQ_H
#define SEQ_H

/* the bank (NAME.HD / NAME.BD in the data folder); 0 if it can't be read */
int seq_bank(const char *name);
/* port k's sequence (an SQ file); 0 if it can't be read */
int seq_load(int k, const char *path);
/* play from the start / stop (its voices off) */
void seq_play(int k, int on);
int seq_playing(int k);
/* the sequence volume (0..255, 128 as written), the port's volume (0..255) and a channel's
 * volume as the director sets it (0..127, on top of the sequence's own) */
void seq_volume(int k, int v);
void seq_port_volume(int k, int v);
void seq_chan_volume(int k, int ch, int v);
/* a MIDI channel message (status with channel, two data bytes) */
void seq_midi(int k, int status, int d1, int d2);
/* everything stopped, the bank and sequences dropped */
void seq_reset(void);

#endif
