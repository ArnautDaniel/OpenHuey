/* The emulated IOP side on PC: its memory (2 MB, where the EE DMAs to) and the sound memory
 * (SPU2 RAM, 2 MB), and the IOP modules' RPC entry points. */
#ifndef HG_IOP_MEM_H
#define HG_IOP_MEM_H

#define IOP_RAM_SIZE 0x200000
#define SPU_RAM_SIZE 0x200000

extern unsigned char iop_ram[IOP_RAM_SIZE];
extern unsigned char spu_ram[SPU_RAM_SIZE];

/* a pointer to `n` bytes of IOP memory at `addr` (NULL if out of range) */
void *iop_ptr(unsigned addr, unsigned n);
void iop_write(unsigned addr, const void *src, unsigned n);
unsigned iop_alloc(unsigned size);

/* the sound driver SNDDRV.IRX (snddrv.c): its servers 0x77777777 / 0x77777778 */
void *snddrv_rpc(unsigned fno, void *args, int size);
void *snddrv_rpc2(unsigned fno, void *args, int size);

/* libsd (spu2.c) */
void sceSdSetParam(unsigned short entry, unsigned short value);
unsigned short sceSdGetParam(unsigned short entry);
void sceSdSetSwitch(unsigned short entry, unsigned value);
unsigned sceSdGetSwitch(unsigned short entry);
void sceSdSetAddr(unsigned short entry, unsigned value);
unsigned sceSdGetAddr(unsigned short entry);
unsigned short sceSdNote2Pitch(unsigned short cnote, unsigned short cfine, unsigned short note, short fine);
void spu_reset(void);
void snddrv_report(unsigned off, unsigned value);   /* the EE's state block */

/* the sequencer (seq.c) */
void seq_tick(double sec);
void seq_reset(void);
void seq_load_bank(int k, unsigned hd, unsigned spu);
int seq_load(int k, unsigned addr);
void seq_play(int k, int on);
void seq_locate(int k, unsigned pos);
int seq_playing(int k);
void seq_midi_in(int k, unsigned msg);
void seq_set_volume(int k, int v);
int seq_volume(int k);
void seq_set_synth_volume(int k, int v);
void seq_set_tempo(int k, int t);
int seq_tempo(int k);

#endif
