// hgview: model viewer for converted Haunting Ground characters (.glb from tools/hg_model.py).
// Pose the skeleton by hand, or play the animations stored in the .glb.
//
//   build:  cc -O2 -o hgview tools/viewer/hgview.c -lraylib -lm
//   run:    ./hgview build/models/hewie_costume0.glb
//
// Controls (also shown on screen):
//   mouse drag (left) orbit, wheel zoom, right drag pan
//   TAB          pose mode <-> animation mode
//   pose:        [ / ] or click a joint: select bone   Q/A W/S E/D: rotate X/Y/Z (shift = fine)
//                R: reset bone   BACKSPACE: reset all
//   animation:   LEFT/RIGHT: previous/next animation   SPACE: pause   , / .: step frame
//                UP/DOWN: speed
//   B skeleton   N bone names   G grid   F1 help
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    Model model;
    int boneCount;
    Transform *bindLocal;      // bind pose, relative to the parent bone
    Quaternion *poseRot;       // user rotation on top of the bind pose (pose mode)
    ModelAnimation manual;     // one-keyframe animation built from the user's pose
    ModelAnimation *anims;
    int animCount;
} Viewer;

// world = parent * local (raylib poses are in model space)
static Transform TransformCombine(Transform parent, Transform local) {
    Transform w;
    w.rotation = QuaternionNormalize(QuaternionMultiply(parent.rotation, local.rotation));
    w.scale = Vector3Multiply(parent.scale, local.scale);
    w.translation = Vector3Add(parent.translation,
                               Vector3RotateByQuaternion(Vector3Multiply(parent.scale, local.translation), parent.rotation));
    return w;
}

static Transform TransformRelative(Transform parent, Transform world) {
    Transform l;
    Quaternion inv = QuaternionInvert(parent.rotation);
    l.rotation = QuaternionNormalize(QuaternionMultiply(inv, world.rotation));
    l.scale = Vector3Divide(world.scale, parent.scale);
    l.translation = Vector3Divide(Vector3RotateByQuaternion(Vector3Subtract(world.translation, parent.translation), inv),
                                  parent.scale);
    return l;
}

static void BuildManualPose(Viewer *v) {
    ModelSkeleton *sk = &v->model.skeleton;
    Transform *out = v->manual.keyframePoses[0];
    for (int i = 0; i < v->boneCount; i++) {   // parents come before children in our files
        Transform l = v->bindLocal[i];
        l.rotation = QuaternionNormalize(QuaternionMultiply(l.rotation, v->poseRot[i]));
        int p = sk->bones[i].parent;
        out[i] = (p >= 0) ? TransformCombine(out[p], l) : l;
    }
}

// raylib 6 crashes in LoadModelAnimations on a .glb without animations: look first
static bool GlbHasAnimations(const char *path) {
    int size = 0;
    unsigned char *d = LoadFileData(path, &size);
    bool has = false;
    if (d && size > 20) {
        unsigned int jsonLen = d[12] | d[13] << 8 | d[14] << 16 | (unsigned int)d[15] << 24;
        const char *key = "\"animations\"";
        size_t kl = strlen(key);
        for (unsigned int i = 20; i + kl <= 20 + jsonLen && i + kl <= (unsigned int)size; i++)
            if (memcmp(d + i, key, kl) == 0) { has = true; break; }
    }
    UnloadFileData(d);
    return has;
}

static void ResetPose(Viewer *v) {
    for (int i = 0; i < v->boneCount; i++) v->poseRot[i] = QuaternionIdentity();
}

int main(int argc, char **argv) {
    const char *path = argc > 1 ? argv[1] : "build/models/hewie_costume0.glb";
    // --shot out.png [bone angleX]: render one frame (optionally with one bone bent) and exit
    const char *shot = (argc > 3 && strcmp(argv[2], "--shot") == 0) ? argv[3] : NULL;
    int shotBone = argc > 5 ? atoi(argv[4]) : -1;
    float shotAngle = argc > 5 ? (float)atof(argv[5]) : 0.0f;
    int frameNo = 0;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 800, "hgview");
    SetTargetFPS(60);

    Viewer v = {0};
    v.model = LoadModel(path);
    if (v.model.meshCount == 0) {
        TraceLog(LOG_ERROR, "could not load %s", path);
        CloseWindow();
        return 1;
    }
    ModelSkeleton *sk = &v.model.skeleton;
    v.boneCount = sk->boneCount;
    v.bindLocal = calloc(v.boneCount, sizeof(Transform));
    v.poseRot = calloc(v.boneCount, sizeof(Quaternion));
    for (int i = 0; i < v.boneCount; i++) {
        int p = sk->bones[i].parent;
        v.bindLocal[i] = (p >= 0) ? TransformRelative(sk->bindPose[p], sk->bindPose[i]) : sk->bindPose[i];
    }
    ResetPose(&v);
    if (shotBone >= 0 && shotBone < v.boneCount)
        v.poseRot[shotBone] = QuaternionFromAxisAngle((Vector3){1, 0, 0}, shotAngle * DEG2RAD);
    v.manual.boneCount = v.boneCount;
    v.manual.keyframeCount = 1;
    v.manual.keyframePoses = calloc(1, sizeof(ModelAnimPose));
    v.manual.keyframePoses[0] = calloc(v.boneCount, sizeof(Transform));
    strcpy(v.manual.name, "manual pose");
    if (GlbHasAnimations(path)) v.anims = LoadModelAnimations(path, &v.animCount);

    // frame the model
    BoundingBox bb = GetModelBoundingBox(v.model);
    Vector3 center = Vector3Scale(Vector3Add(bb.min, bb.max), 0.5f);
    float size = Vector3Length(Vector3Subtract(bb.max, bb.min));
    float yaw = 0.6f, pitch = 0.25f, dist = size * 1.1f;
    Vector3 target = center;
    Camera3D cam = {0};
    cam.up = (Vector3){0, 1, 0};
    cam.fovy = 40.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    bool poseMode = v.animCount == 0 || shot, paused = false, showSkel = true, showNames = false, showGrid = true, help = true;
    int sel = 0, anim = 0;
    float frame = 0.0f, speed = 1.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        // camera
        Vector2 md = GetMouseDelta();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_CONTROL)) {
            yaw -= md.x * 0.008f;
            pitch = Clamp(pitch + md.y * 0.008f, -1.5f, 1.5f);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector3 fwd = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(fwd, cam.up));
            Vector3 up = Vector3CrossProduct(right, fwd);
            float k = dist * 0.0015f;
            target = Vector3Add(target, Vector3Add(Vector3Scale(right, -md.x * k), Vector3Scale(up, md.y * k)));
        }
        dist = Clamp(dist * (1.0f - GetMouseWheelMove() * 0.1f), size * 0.05f, size * 10.0f);
        cam.target = target;
        cam.position = Vector3Add(target, (Vector3){dist * cosf(pitch) * sinf(yaw), dist * sinf(pitch),
                                                     dist * cosf(pitch) * cosf(yaw)});

        // input
        if (IsKeyPressed(KEY_TAB) && v.animCount > 0) poseMode = !poseMode;
        if (IsKeyPressed(KEY_B)) showSkel = !showSkel;
        if (IsKeyPressed(KEY_N)) showNames = !showNames;
        if (IsKeyPressed(KEY_G)) showGrid = !showGrid;
        if (IsKeyPressed(KEY_F1)) help = !help;

        if (poseMode) {
            if (IsKeyPressed(KEY_RIGHT_BRACKET)) sel = (sel + 1) % v.boneCount;
            if (IsKeyPressed(KEY_LEFT_BRACKET)) sel = (sel + v.boneCount - 1) % v.boneCount;
            float step = (IsKeyDown(KEY_LEFT_SHIFT) ? 0.3f : 1.5f) * dt;
            Vector3 axis = {0};
            float a = 0.0f;
            if (IsKeyDown(KEY_Q)) { axis = (Vector3){1, 0, 0}; a = step; }
            if (IsKeyDown(KEY_A)) { axis = (Vector3){1, 0, 0}; a = -step; }
            if (IsKeyDown(KEY_W)) { axis = (Vector3){0, 1, 0}; a = step; }
            if (IsKeyDown(KEY_S)) { axis = (Vector3){0, 1, 0}; a = -step; }
            if (IsKeyDown(KEY_E)) { axis = (Vector3){0, 0, 1}; a = step; }
            if (IsKeyDown(KEY_D)) { axis = (Vector3){0, 0, 1}; a = -step; }
            if (a != 0.0f)
                v.poseRot[sel] = QuaternionNormalize(QuaternionMultiply(v.poseRot[sel], QuaternionFromAxisAngle(axis, a)));
            if (IsKeyPressed(KEY_R)) v.poseRot[sel] = QuaternionIdentity();
            if (IsKeyPressed(KEY_BACKSPACE)) ResetPose(&v);
            BuildManualPose(&v);
            UpdateModelAnimation(v.model, v.manual, 0.0f);
        } else {
            if (IsKeyPressed(KEY_RIGHT)) { anim = (anim + 1) % v.animCount; frame = 0; }
            if (IsKeyPressed(KEY_LEFT)) { anim = (anim + v.animCount - 1) % v.animCount; frame = 0; }
            if (IsKeyPressed(KEY_SPACE)) paused = !paused;
            if (IsKeyPressed(KEY_UP)) speed *= 1.5f;
            if (IsKeyPressed(KEY_DOWN)) speed /= 1.5f;
            int n = v.anims[anim].keyframeCount;
            if (IsKeyPressed(KEY_PERIOD)) frame = (float)(((int)frame + 1) % n);
            if (IsKeyPressed(KEY_COMMA)) frame = (float)(((int)frame + n - 1) % n);
            if (!paused) frame += dt * 60.0f * speed;   // the game runs its motions at 60 fps
            while (frame >= n) frame -= n;
            UpdateModelAnimation(v.model, v.anims[anim], frame);
        }

        // current bone positions (for drawing and picking)
        ModelAnimPose pose = poseMode ? v.manual.keyframePoses[0] : v.model.currentPose;

        BeginDrawing();
        ClearBackground((Color){38, 40, 48, 255});
        BeginMode3D(cam);
        if (showGrid) DrawGrid(20, size / 10.0f);
        DrawModel(v.model, (Vector3){0, 0, 0}, 1.0f, WHITE);
        if (showSkel && pose) {
            rlDisableDepthTest();
            for (int i = 0; i < v.boneCount; i++) {
                int p = sk->bones[i].parent;
                Color c = (i == sel && poseMode) ? YELLOW : (Color){90, 200, 255, 255};
                if (p >= 0) DrawLine3D(pose[p].translation, pose[i].translation, (Color){90, 200, 255, 160});
                DrawSphere(pose[i].translation, size * ((i == sel && poseMode) ? 0.008f : 0.004f), c);
            }
            rlEnableDepthTest();
        }
        EndMode3D();

        // bone picking / names
        if (poseMode && pose && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)) {
            Vector2 m = GetMousePosition();
            float best = 400.0f;
            for (int i = 0; i < v.boneCount; i++) {
                Vector2 s = GetWorldToScreen(pose[i].translation, cam);
                float d2 = Vector2DistanceSqr(s, m);
                if (d2 < best) { best = d2; sel = i; }
            }
        }
        if (showNames && pose) {
            for (int i = 0; i < v.boneCount; i++) {
                Vector2 s = GetWorldToScreen(pose[i].translation, cam);
                DrawText(TextFormat("%d", i), (int)s.x + 4, (int)s.y - 4, 10, (i == sel) ? YELLOW : LIGHTGRAY);
            }
        }

        // HUD
        int y = 10;
        DrawText(GetFileName(path), 10, y, 20, RAYWHITE); y += 26;
        if (poseMode) {
            int p = sk->bones[sel].parent;
            DrawText(TextFormat("POSE MODE   bone %d (%s)  parent %d", sel, sk->bones[sel].name, p), 10, y, 18, YELLOW);
        } else {
            DrawText(TextFormat("ANIMATION %d/%d  %s   frame %d/%d   speed %.2fx%s", anim + 1, v.animCount,
                                v.anims[anim].name, (int)frame, v.anims[anim].keyframeCount, speed, paused ? "  (paused)" : ""),
                     10, y, 18, GREEN);
        }
        y += 24;
        if (help) {
            const char *lines[] = {
                "mouse: left drag orbit, right drag pan, wheel zoom",
                v.animCount ? "TAB: pose mode / animation mode" : "(no animations in this file: pose mode only)",
                "pose: [ ] select bone (or ctrl+click a joint), Q/A W/S E/D rotate X/Y/Z (shift: fine)",
                "      R reset bone, BACKSPACE reset all",
                "anim: LEFT/RIGHT animation, SPACE pause, , . step, UP/DOWN speed",
                "B skeleton  N bone numbers  G grid  F1 hide help",
            };
            for (int i = 0; i < 6; i++) { DrawText(lines[i], 10, y, 16, LIGHTGRAY); y += 20; }
        }
        DrawFPS(GetScreenWidth() - 90, 10);
        EndDrawing();
        if (shot && ++frameNo == 3) {
            TakeScreenshot(shot);
            break;
        }
    }

    if (v.anims) UnloadModelAnimations(v.anims, v.animCount);
    UnloadModel(v.model);
    CloseWindow();
    return 0;
}
