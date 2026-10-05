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

#endif
