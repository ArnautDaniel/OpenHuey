/* Metrowerks MSL C++ runtime (PS2 0x00100230..0x0010BDC0) as far as the game needs it on PC. */

/* iostream init objects (std::ios_base::Init and a sibling): the game doesn't use iostreams. */
void func_00102D80(void *obj) { (void)obj; }
void func_001032F0(void *obj) { (void)obj; }

/* __register_global_object(obj, dtor, link): global destructors to run at exit. The process
 * exit frees everything on PC, so nothing is kept. */
void func_00100AB0(void *obj, void (*dtor)(void), void *link) {
    (void)obj;
    (void)dtor;
    (void)link;
}

/* __construct_array(array, ctor, dtor, size, count): construct `count` elements. */
void func_00100340(void *array, void *(*ctor)(void *), void (*dtor)(void *, int), unsigned size,
                   unsigned count) {
    unsigned i;

    (void)dtor;
    for (i = 0; i < count; i++) {
        ctor((char *)array + i * size);
    }
}
