/* Example for tools/difftest.py (not linked into the game yet).
 * func_002D1E40: part of the main game object's init; loads the embedded
 * cdvdman IRX and exits if it reports a problem. */
typedef int s32;
typedef float f32;

extern void func_001136E8(s32 status);          /* exit() */
extern s32 func_0037E1F0(s32 *result);          /* load embedded IRX */
extern void func_002CFA10(void *self);
extern void func_001F44D0(void *p);
extern void func_002BFB20(void *p);
extern f32 D_00414370, D_00414374, D_00414378;

typedef struct Object { void (**vtbl)(struct Object *); } Object;

void func_002D1E40(char *self) {
    s32 result;
    s32 ret;
    Object *sub = (Object *)(self + 0x69AC0);
    f32 *v;

    sub->vtbl[3](sub);
    ret = func_0037E1F0(&result);
    if (ret >= 0 && result != 0) {
        func_001136E8(0);
    }
    func_002CFA10(self);
    func_001F44D0(self + 0x14E8C90);
    func_002BFB20(self + 0x20);
    *(s32 *)(self + 0x4) = 1;
    *(s32 *)(self + 0x10) = 0;
    *(s32 *)(self + 0x14E8FBC) = 0;
    v = (f32 *)(self + 0x4009CC);
    v[0] = D_00414370;
    v[1] = D_00414374;
    v[2] = D_00414378;
}
