/* libpad2 (PS2 0x001EFA38..0x001F0250) on PC: input will come from SDL. */

/* scePad2Init */
int func_001EF990(int mode) { (void)mode; return 1; }

/* scePad2CreateSocket(port, dma buffer): socket id */
int func_001EFA38(int port, void *buffer) { (void)buffer; return port; }
