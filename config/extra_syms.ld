/* Hand-written symbol definitions that must follow their section when it moves. */
cdvdman_irx_end = cdvdman_irx + 0x359; /* exact IRX size; asset is padded to 0x360 */

/* VU microcode block (DMA-tagged MPG uploads), bin segment vu_microcode @ 0x003A1B80. */
D_003A1B80 = vu_microcode + 0x0;
D_003A1FF0 = vu_microcode + 0x470;
D_003A2EF0 = vu_microcode + 0x1370;
D_003A38C0 = vu_microcode + 0x1D40;
D_003A3900 = vu_microcode + 0x1D80;
D_003A4390 = vu_microcode + 0x2810;
D_003A4F40 = vu_microcode + 0x33C0;
D_003A58A0 = vu_microcode + 0x3D20;
D_003A5C60 = vu_microcode + 0x40E0;
D_003A60F0 = vu_microcode + 0x4570;
D_003A6C10 = vu_microcode + 0x5090;
D_003A7530 = vu_microcode + 0x59B0;
D_003A77F0 = vu_microcode + 0x5C70;
D_003A80C0 = vu_microcode + 0x6540;
D_003A8A70 = vu_microcode + 0x6EF0;
D_003A9520 = vu_microcode + 0x79A0;
D_003AA050 = vu_microcode + 0x84D0;
D_003AAE00 = vu_microcode + 0x9280;
D_003AB3A0 = vu_microcode + 0x9820;
D_003ABCF0 = vu_microcode + 0xA170;
D_003AC3F0 = vu_microcode + 0xA870;

/* Small-data base: 0x4828F0 = start of small data (0x47A8F0) + 0x8000, the usual _gp convention. */
_gp = D_0047A8F0 + 0x8000;
