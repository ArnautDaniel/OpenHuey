/* Controller input on PC: keyboard and SDL gamepads, as DualShock 2 data (scePad2Read format:
 * two bytes of buttons, active low, then 16 analog bytes: right stick X/Y, left stick X/Y and
 * the pressures of right, left, up, down, triangle, circle, cross, square, L1, R1, L2, R2).
 *
 * Keyboard: arrows d-pad, X / Space cross, C / Backspace circle, Z square, V triangle, Q / E L1 / R1,
 * 1 / 3 L2 / R2, Enter start, Tab select, WASD left stick.
 * HG_AUTOCROSS=1 (tests without a window): tap cross every 2 seconds.
 * HG_INPUT="frame:button,..." (tests): press button (up down left right cross circle square
 * triangle start select l1 r1 l2 r2) for 6 video frames from frame `frame`. */
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

enum {
    B_SELECT = 0, B_L3, B_R3, B_START, B_UP, B_RIGHT, B_DOWN, B_LEFT,
    B_L2, B_R2, B_L1, B_R1, B_TRIANGLE, B_CIRCLE, B_CROSS, B_SQUARE
};

/* HG_INPUT: the buttons the script holds at input frame `f` */
static unsigned scripted(unsigned f) {
    static const char *const names[16] = {
        "select", "l3", "r3", "start", "up", "right", "down", "left",
        "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square"};
    const char *s = getenv("HG_INPUT");
    unsigned held = 0;

    while (s != NULL && *s) {
        char *end;
        unsigned at = (unsigned)strtoul(s, &end, 10);
        int i;

        if (*end != ':') {
            break;
        }
        s = end + 1;
        for (i = 0; i < 16; i++) {
            size_t n = strlen(names[i]);

            if (strncmp(s, names[i], n) == 0 && (s[n] == ',' || s[n] == 0)) {
                if (f >= at && f < at + 6) {
                    held |= 1u << i;
                }
                break;
            }
        }
        s = strchr(s, ',');
        s = s ? s + 1 : NULL;
    }
    return held;
}

/* analog byte (from 4) of each button's pressure, -1 none */
static const signed char sPressure[16] = {
    -1, -1, -1, -1, 6, 4, 7, 5, 14, 15, 12, 13, 8, 9, 10, 11
};

static SDL_Gamepad *sPad;
static unsigned sFrames;
extern unsigned hg_video_frame;   /* video.c */

static void add_key(unsigned *b, const bool *k, SDL_Scancode sc, int button) {
    if (k[sc]) {
        *b |= 1u << button;
    }
}

void hg_input_read(unsigned char *data) {
    unsigned b = 0;
    int lx = 0x80, ly = 0x80, rx = 0x80, ry = 0x80, i;

    sFrames++;
    if (getenv("HG_AUTOCROSS") && sFrames % 120 < 4) {
        b |= 1u << B_CROSS;
    }
    b |= scripted(hg_video_frame);
    if (SDL_WasInit(SDL_INIT_VIDEO)) {
        const bool *k = SDL_GetKeyboardState(NULL);

        add_key(&b, k, SDL_SCANCODE_UP, B_UP);
        add_key(&b, k, SDL_SCANCODE_DOWN, B_DOWN);
        add_key(&b, k, SDL_SCANCODE_LEFT, B_LEFT);
        add_key(&b, k, SDL_SCANCODE_RIGHT, B_RIGHT);
        add_key(&b, k, SDL_SCANCODE_X, B_CROSS);
        add_key(&b, k, SDL_SCANCODE_SPACE, B_CROSS);
        add_key(&b, k, SDL_SCANCODE_C, B_CIRCLE);
        add_key(&b, k, SDL_SCANCODE_BACKSPACE, B_CIRCLE);
        add_key(&b, k, SDL_SCANCODE_Z, B_SQUARE);
        add_key(&b, k, SDL_SCANCODE_V, B_TRIANGLE);
        add_key(&b, k, SDL_SCANCODE_Q, B_L1);
        add_key(&b, k, SDL_SCANCODE_E, B_R1);
        add_key(&b, k, SDL_SCANCODE_1, B_L2);
        add_key(&b, k, SDL_SCANCODE_3, B_R2);
        add_key(&b, k, SDL_SCANCODE_RETURN, B_START);
        add_key(&b, k, SDL_SCANCODE_TAB, B_SELECT);
        if (k[SDL_SCANCODE_A]) lx = 0x00;
        if (k[SDL_SCANCODE_D]) lx = 0xFF;
        if (k[SDL_SCANCODE_W]) ly = 0x00;
        if (k[SDL_SCANCODE_S]) ly = 0xFF;

        if (sPad == NULL) {
            int n = 0;
            SDL_JoystickID *ids = SDL_GetGamepads(&n);

            if (ids != NULL && n > 0) {
                sPad = SDL_OpenGamepad(ids[0]);
            }
            SDL_free(ids);
        }
        if (sPad != NULL) {
            static const struct { SDL_GamepadButton s; int b; } map[] = {
                {SDL_GAMEPAD_BUTTON_SOUTH, B_CROSS}, {SDL_GAMEPAD_BUTTON_EAST, B_CIRCLE},
                {SDL_GAMEPAD_BUTTON_WEST, B_SQUARE}, {SDL_GAMEPAD_BUTTON_NORTH, B_TRIANGLE},
                {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, B_L1}, {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, B_R1},
                {SDL_GAMEPAD_BUTTON_START, B_START}, {SDL_GAMEPAD_BUTTON_BACK, B_SELECT},
                {SDL_GAMEPAD_BUTTON_LEFT_STICK, B_L3}, {SDL_GAMEPAD_BUTTON_RIGHT_STICK, B_R3},
                {SDL_GAMEPAD_BUTTON_DPAD_UP, B_UP}, {SDL_GAMEPAD_BUTTON_DPAD_DOWN, B_DOWN},
                {SDL_GAMEPAD_BUTTON_DPAD_LEFT, B_LEFT}, {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, B_RIGHT},
            };
            unsigned j;

            for (j = 0; j < sizeof(map) / sizeof(map[0]); j++) {
                if (SDL_GetGamepadButton(sPad, map[j].s)) {
                    b |= 1u << map[j].b;
                }
            }
            if (SDL_GetGamepadAxis(sPad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000) b |= 1u << B_L2;
            if (SDL_GetGamepadAxis(sPad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > 16000) b |= 1u << B_R2;
            lx = (SDL_GetGamepadAxis(sPad, SDL_GAMEPAD_AXIS_LEFTX) + 32768) >> 8;
            ly = (SDL_GetGamepadAxis(sPad, SDL_GAMEPAD_AXIS_LEFTY) + 32768) >> 8;
            rx = (SDL_GetGamepadAxis(sPad, SDL_GAMEPAD_AXIS_RIGHTX) + 32768) >> 8;
            ry = (SDL_GetGamepadAxis(sPad, SDL_GAMEPAD_AXIS_RIGHTY) + 32768) >> 8;
        }
    }
    data[0] = (unsigned char)~(b & 0xFF);
    data[1] = (unsigned char)~(b >> 8);
    data[2] = (unsigned char)rx;
    data[3] = (unsigned char)ry;
    data[4] = (unsigned char)lx;
    data[5] = (unsigned char)ly;
    for (i = 0; i < 16; i++) {
        if (sPressure[i] >= 0 && (b & (1u << i))) {
            data[2 + sPressure[i]] = 0xFF;
        }
    }
}
