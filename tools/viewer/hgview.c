// hgview: model viewer for converted Haunting Ground characters (.glb from tools/hg_model.py /
// tools/hg_export_all.py). Browse the models, play their motions, pose the skeleton by hand.
//
//   build:  cc -O2 -o build/hgview tools/viewer/hgview.c -lraylib -lm
//   run:    build/hgview [folder or .glb]          (default: build/models)
//
// Controls (also on screen, F1 hides them):
//   left panel: models (click; mouse wheel scrolls; PAGE UP / PAGE DOWN previous / next model;
//               or drop a .glb on the window)
//   right panel: motions (click; mouse wheel scrolls; LEFT / RIGHT previous / next)
//   view: left drag orbit, right drag pan, wheel zoom, F frame the model
//   TAB: animation mode <-> pose mode
//   animation: SPACE pause, , / . step frame, UP / DOWN speed
//   pose: [ / ] or ctrl+click a joint to select a bone; Q/A W/S E/D rotate X/Y/Z (shift: fine);
//         R reset bone, BACKSPACE reset all
//   B skeleton, N bone numbers, G grid
//
// Testing: build/hgview file.glb --shot out.png [bone angle | anim N frame] renders one frame.
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdlib.h>
#include <string.h>

#define PANEL_W 230
#define ROW_H 18

// alpha test (the PS2 draws the cut-out parts with an alpha test; raylib would blend) + simple light
static const char *VS =
    "#version 330\n"
    "in vec3 vertexPosition; in vec2 vertexTexCoord; in vec3 vertexNormal; in vec4 vertexColor;\n"
    "uniform mat4 mvp; uniform mat4 matNormal;\n"
    "out vec2 uv; out vec3 nrm; out vec4 col;\n"
    "void main() { uv = vertexTexCoord; nrm = (matNormal * vec4(vertexNormal, 0.0)).xyz; col = vertexColor;\n"
    "  gl_Position = mvp * vec4(vertexPosition, 1.0); }\n";
static const char *FS =
    "#version 330\n"
    "in vec2 uv; in vec3 nrm; in vec4 col;\n"
    "uniform sampler2D texture0; uniform vec4 colDiffuse;\n"
    "out vec4 finalColor;\n"
    "void main() { vec4 t = texture(texture0, uv) * colDiffuse * col;\n"
    "  if (t.a < 0.25) discard;\n"
    "  vec3 n = normalize(nrm); float l = 0.5 + 0.5 * abs(dot(n, normalize(vec3(0.4, 0.8, 0.5))));\n"
    "  finalColor = vec4(t.rgb * l, 1.0); }\n";
static Shader gShader;

typedef struct {
    bool loaded;
    Model model;
    int boneCount;
    Transform *bindLocal;      // bind pose, relative to the parent bone
    Quaternion *poseRot;       // user rotation on top of the bind pose (pose mode)
    ModelAnimation manual;     // one-keyframe animation built from the user's pose
    ModelAnimation *anims;
    int animCount;
    BoundingBox bounds;
} Viewer;

typedef struct {
    float yaw, pitch, dist, size;
    Vector3 target;
    Camera3D cam;
} Orbit;

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

static void ResetPose(Viewer *v) {
    for (int i = 0; i < v->boneCount; i++) v->poseRot[i] = QuaternionIdentity();
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

static void UnloadViewer(Viewer *v) {
    if (!v->loaded) return;
    for (int i = 0; i < v->model.materialCount; i++) {   // (UnloadModel would unload our shader)
        v->model.materials[i].shader.id = rlGetShaderIdDefault();
        v->model.materials[i].shader.locs = rlGetShaderLocsDefault();
    }
    if (v->anims) UnloadModelAnimations(v->anims, v->animCount);
    UnloadModel(v->model);
    free(v->bindLocal);
    free(v->poseRot);
    free(v->manual.keyframePoses[0]);
    free(v->manual.keyframePoses);
    memset(v, 0, sizeof(*v));
}

static bool LoadViewer(Viewer *v, const char *path) {
    UnloadViewer(v);
    v->model = LoadModel(path);
    if (v->model.meshCount == 0) {
        TraceLog(LOG_WARNING, "could not load %s", path);
        return false;
    }
    v->loaded = true;
    if (IsShaderValid(gShader))
        for (int i = 0; i < v->model.materialCount; i++) v->model.materials[i].shader = gShader;
    ModelSkeleton *sk = &v->model.skeleton;
    v->boneCount = sk->boneCount;
    v->bindLocal = calloc(v->boneCount + 1, sizeof(Transform));
    v->poseRot = calloc(v->boneCount + 1, sizeof(Quaternion));
    for (int i = 0; i < v->boneCount; i++) {
        int p = sk->bones[i].parent;
        v->bindLocal[i] = (p >= 0) ? TransformRelative(sk->bindPose[p], sk->bindPose[i]) : sk->bindPose[i];
    }
    ResetPose(v);
    v->manual.boneCount = v->boneCount;
    v->manual.keyframeCount = 1;
    v->manual.keyframePoses = calloc(1, sizeof(ModelAnimPose));
    v->manual.keyframePoses[0] = calloc(v->boneCount + 1, sizeof(Transform));
    strcpy(v->manual.name, "manual pose");
    if (GlbHasAnimations(path)) v->anims = LoadModelAnimations(path, &v->animCount);
    v->bounds = GetModelBoundingBox(v->model);
    return true;
}

static void FrameModel(Orbit *o, BoundingBox bb) {
    o->target = Vector3Scale(Vector3Add(bb.min, bb.max), 0.5f);
    o->size = Vector3Length(Vector3Subtract(bb.max, bb.min));
    if (o->size < 0.001f) o->size = 1.0f;
    o->dist = o->size * 1.2f;
}

static bool HasGlbExt(const char *p) { return IsFileExtension(p, ".glb"); }

static int CompareStr(const void *a, const void *b) { return strcmp(*(char *const *)a, *(char *const *)b); }

// a scrollable list; returns the clicked row or -1
static int ListPanel(Rectangle r, const char *title, const char **items, int count, int current, float *scroll) {
    DrawRectangleRec(r, Fade((Color){20, 22, 28, 255}, 0.85f));
    DrawText(title, (int)r.x + 6, (int)r.y + 4, 16, RAYWHITE);
    Rectangle list = {r.x, r.y + 24, r.width, r.height - 24};
    int visible = (int)(list.height / ROW_H);
    Vector2 m = GetMousePosition();
    bool hover = CheckCollisionPointRec(m, list);
    if (hover) *scroll -= GetMouseWheelMove() * 3.0f;
    float maxScroll = (float)(count - visible);
    if (maxScroll < 0) maxScroll = 0;
    *scroll = Clamp(*scroll, 0.0f, maxScroll);
    int first = (int)*scroll, clicked = -1;
    BeginScissorMode((int)list.x, (int)list.y, (int)list.width, (int)list.height);
    for (int i = first; i < count && i < first + visible + 1; i++) {
        Rectangle row = {list.x, list.y + (float)(i - first) * ROW_H, list.width, ROW_H};
        bool over = hover && CheckCollisionPointRec(m, row);
        if (i == current) DrawRectangleRec(row, (Color){60, 90, 150, 255});
        else if (over) DrawRectangleRec(row, (Color){50, 54, 66, 255});
        DrawText(items[i], (int)row.x + 6, (int)row.y + 3, 14, (i == current) ? WHITE : LIGHTGRAY);
        if (over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) clicked = i;
    }
    EndScissorMode();
    return clicked;
}

int main(int argc, char **argv) {
    const char *arg = argc > 1 ? argv[1] : "build/models";
    // --shot out.png [bone angleX | anim N F]: render one frame and exit (testing)
    const char *shot = (argc > 3 && strcmp(argv[2], "--shot") == 0) ? argv[3] : NULL;
    bool shotAnim = shot && argc > 6 && strcmp(argv[4], "anim") == 0;
    int shotBone = (shot && argc > 5 && !shotAnim) ? atoi(argv[4]) : -1;
    float shotAngle = (shot && argc > 5 && !shotAnim) ? (float)atof(argv[5]) : 0.0f;
    int frameNo = 0;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1400, 860, "hgview");
    SetTargetFPS(60);
    gShader = LoadShaderFromMemory(VS, FS);

    // the model list: every .glb under the folder (or under the given file's folder)
    char *files[4096];
    int fileCount = 0, cur = 0;
    char baseDir[1024];
    strncpy(baseDir, DirectoryExists(arg) ? arg : GetDirectoryPath(arg), sizeof(baseDir) - 1);
    baseDir[sizeof(baseDir) - 1] = 0;
    FilePathList fl = LoadDirectoryFilesEx(baseDir, ".glb", true);
    for (unsigned int i = 0; i < fl.count && fileCount < 4096; i++) files[fileCount++] = strdup(fl.paths[i]);
    UnloadDirectoryFiles(fl);
    qsort(files, fileCount, sizeof(char *), CompareStr);
    if (HasGlbExt(arg)) {
        cur = -1;
        for (int i = 0; i < fileCount; i++)
            if (strcmp(GetFileName(files[i]), GetFileName(arg)) == 0) cur = i;
        if (cur < 0 && fileCount < 4096) { files[fileCount] = strdup(arg); cur = fileCount++; }
    }
    const char **fileNames = calloc(4097, sizeof(char *));
    size_t baseLen = strlen(baseDir);
    for (int i = 0; i < fileCount; i++) {
        const char *n = files[i];
        if (strncmp(n, baseDir, baseLen) == 0) n += baseLen;
        while (*n == '/') n++;
        fileNames[i] = n;
    }

    Viewer v = {0};
    Orbit o = {.yaw = 0.6f, .pitch = 0.25f, .size = 1.0f, .dist = 2.0f};
    o.cam.up = (Vector3){0, 1, 0};
    o.cam.fovy = 40.0f;
    o.cam.projection = CAMERA_PERSPECTIVE;
    bool paused = false, showSkel = true, showNames = false, showGrid = true, help = !shot, poseMode = false;
    int sel = 0, anim = 0, loadedIndex = -1;
    float frame = 0.0f, speed = 1.0f, fileScroll = 0.0f, animScroll = 0.0f;
    const char **animNames = NULL;

    while (!WindowShouldClose()) {
        // (re)load the current model
        if (cur != loadedIndex && cur >= 0 && cur < fileCount) {
            LoadViewer(&v, files[cur]);
            loadedIndex = cur;
            free(animNames);
            animNames = calloc(v.animCount + 1, sizeof(char *));
            for (int i = 0; i < v.animCount; i++) animNames[i] = v.anims[i].name;
            anim = 0; frame = 0; sel = 0; animScroll = 0;
            poseMode = v.animCount == 0;
            if (v.loaded) FrameModel(&o, v.bounds);
            if (shot) {
                if (shotBone >= 0 && shotBone < v.boneCount)
                    v.poseRot[shotBone] = QuaternionFromAxisAngle((Vector3){1, 0, 0}, shotAngle * DEG2RAD);
                poseMode = !shotAnim || v.animCount == 0;
                if (shotAnim && v.animCount) { anim = atoi(argv[5]) % v.animCount; frame = (float)atof(argv[6]); paused = true; }
            }
            SetWindowTitle(TextFormat("hgview - %s", fileNames[cur]));
        }
        if (IsFileDropped()) {
            FilePathList dropped = LoadDroppedFiles();
            if (dropped.count > 0 && HasGlbExt(dropped.paths[0]) && fileCount < 4096) {
                files[fileCount] = strdup(dropped.paths[0]);
                fileNames[fileCount] = GetFileName(files[fileCount]);
                cur = fileCount++;
            }
            UnloadDroppedFiles(dropped);
        }

        float dt = GetFrameTime();
        int sw = GetScreenWidth(), sh = GetScreenHeight();
        Rectangle leftPanel = {0, 0, PANEL_W, (float)sh};
        Rectangle rightPanel = {(float)(sw - PANEL_W), 0, PANEL_W, (float)sh};
        Vector2 mouse = GetMousePosition();
        bool overUI = !shot && (CheckCollisionPointRec(mouse, leftPanel) ||
                                (v.animCount > 0 && CheckCollisionPointRec(mouse, rightPanel)));

        // camera
        Vector2 md = GetMouseDelta();
        if (!overUI && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_CONTROL)) {
            o.yaw -= md.x * 0.008f;
            o.pitch = Clamp(o.pitch + md.y * 0.008f, -1.5f, 1.5f);
        }
        if (!overUI && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector3 fwd = Vector3Normalize(Vector3Subtract(o.cam.target, o.cam.position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(fwd, o.cam.up));
            Vector3 up = Vector3CrossProduct(right, fwd);
            float k = o.dist * 0.0015f;
            o.target = Vector3Add(o.target, Vector3Add(Vector3Scale(right, -md.x * k), Vector3Scale(up, md.y * k)));
        }
        if (!overUI) o.dist = Clamp(o.dist * (1.0f - GetMouseWheelMove() * 0.1f), o.size * 0.02f, o.size * 20.0f);
        o.cam.target = o.target;
        o.cam.position = Vector3Add(o.target, (Vector3){o.dist * cosf(o.pitch) * sinf(o.yaw), o.dist * sinf(o.pitch),
                                                         o.dist * cosf(o.pitch) * cosf(o.yaw)});

        // keys
        if (IsKeyPressed(KEY_PAGE_DOWN) && fileCount) cur = (cur + 1) % fileCount;
        if (IsKeyPressed(KEY_PAGE_UP) && fileCount) cur = (cur + fileCount - 1) % fileCount;
        if (IsKeyPressed(KEY_TAB) && v.animCount > 0) poseMode = !poseMode;
        if (IsKeyPressed(KEY_B)) showSkel = !showSkel;
        if (IsKeyPressed(KEY_N)) showNames = !showNames;
        if (IsKeyPressed(KEY_G)) showGrid = !showGrid;
        if (IsKeyPressed(KEY_F1)) help = !help;
        if (IsKeyPressed(KEY_F) && v.loaded) FrameModel(&o, v.bounds);

        if (v.loaded && poseMode && v.boneCount > 0) {
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
        } else if (v.loaded && v.animCount > 0) {
            if (IsKeyPressed(KEY_RIGHT)) { anim = (anim + 1) % v.animCount; frame = 0; }
            if (IsKeyPressed(KEY_LEFT)) { anim = (anim + v.animCount - 1) % v.animCount; frame = 0; }
            if (IsKeyPressed(KEY_SPACE)) paused = !paused;
            if (IsKeyPressed(KEY_UP)) speed *= 1.5f;
            if (IsKeyPressed(KEY_DOWN)) speed /= 1.5f;
            int n = v.anims[anim].keyframeCount;
            if (IsKeyPressed(KEY_PERIOD)) frame = (float)(((int)frame + 1) % n);
            if (IsKeyPressed(KEY_COMMA)) frame = (float)(((int)frame + n - 1) % n);
            if (!paused) frame += dt * 60.0f * speed;   // raylib bakes glTF animations at 60 keyframes/s
            while (frame >= n) frame -= n;
            UpdateModelAnimation(v.model, v.anims[anim], frame);
        }
        ModelAnimPose pose = !v.loaded ? NULL : (poseMode ? v.manual.keyframePoses[0] : v.model.currentPose);

        BeginDrawing();
        ClearBackground((Color){38, 40, 48, 255});
        if (v.loaded) {
            BeginMode3D(o.cam);
            if (showGrid) DrawGrid(20, o.size / 10.0f);
            DrawModel(v.model, (Vector3){0, 0, 0}, 1.0f, WHITE);
            if (showSkel && pose && v.boneCount > 1) {
                rlDisableDepthTest();
                for (int i = 0; i < v.boneCount; i++) {
                    int p = v.model.skeleton.bones[i].parent;
                    bool s = (i == sel && poseMode);
                    if (p >= 0) DrawLine3D(pose[p].translation, pose[i].translation, (Color){90, 200, 255, 160});
                    DrawSphere(pose[i].translation, o.size * (s ? 0.008f : 0.004f), s ? YELLOW : (Color){90, 200, 255, 255});
                }
                rlEnableDepthTest();
            }
            EndMode3D();

            if (poseMode && pose && !overUI && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)) {
                float best = 400.0f;
                for (int i = 0; i < v.boneCount; i++) {
                    float d2 = Vector2DistanceSqr(GetWorldToScreen(pose[i].translation, o.cam), mouse);
                    if (d2 < best) { best = d2; sel = i; }
                }
            }
            if (showNames && pose) {
                for (int i = 0; i < v.boneCount; i++) {
                    Vector2 s = GetWorldToScreen(pose[i].translation, o.cam);
                    DrawText(TextFormat("%d", i), (int)s.x + 4, (int)s.y - 4, 10, (i == sel) ? YELLOW : LIGHTGRAY);
                }
            }
        }

        // panels
        if (!shot) {
            int c = ListPanel(leftPanel, TextFormat("Models (%d)", fileCount), fileNames, fileCount, cur, &fileScroll);
            if (c >= 0) cur = c;
            if (v.animCount > 0) {
                int a2 = ListPanel(rightPanel, TextFormat("Motions (%d)", v.animCount), animNames, v.animCount,
                                   poseMode ? -1 : anim, &animScroll);
                if (a2 >= 0) { anim = a2; frame = 0; poseMode = false; }
            }
        }

        // HUD
        int x = shot ? 10 : PANEL_W + 10, y = 10;
        DrawText(cur >= 0 && cur < fileCount ? fileNames[cur] : "(no model: give a folder or drop a .glb)", x, y, 20, RAYWHITE);
        y += 26;
        if (v.loaded) {
            DrawText(TextFormat("%d bones, %d meshes", v.boneCount, v.model.meshCount), x, y, 14, GRAY);
            y += 20;
            if (poseMode) {
                DrawText(TextFormat("POSE MODE   bone %d (%s)  parent %d", sel, v.boneCount ? v.model.skeleton.bones[sel].name : "-",
                                    v.boneCount ? v.model.skeleton.bones[sel].parent : -1), x, y, 18, YELLOW);
            } else if (v.animCount > 0) {
                DrawText(TextFormat("MOTION %d/%d  %s   frame %d/%d   speed %.2fx%s", anim + 1, v.animCount, v.anims[anim].name,
                                    (int)frame, v.anims[anim].keyframeCount, speed, paused ? "  (paused)" : ""), x, y, 18, GREEN);
            }
            y += 24;
        }
        if (help) {
            const char *lines[] = {
                "models: click in the left list, PAGE UP/DOWN, or drop a .glb",
                "motions: click in the right list, LEFT/RIGHT; SPACE pause, , . step, UP/DOWN speed",
                "view: left drag orbit, right drag pan, wheel zoom, F frame",
                "TAB: pose mode - [ ] or ctrl+click select bone, Q/A W/S E/D rotate (shift fine), R / BACKSPACE reset",
                "B skeleton  N bone numbers  G grid  F1 hide help",
            };
            for (int i = 0; i < 5; i++) { DrawText(lines[i], x, y, 14, LIGHTGRAY); y += 18; }
        }
        EndDrawing();
        if (shot && ++frameNo == 3) {
            TakeScreenshot(shot);
            break;
        }
    }

    UnloadViewer(&v);
    UnloadShader(gShader);
    for (int i = 0; i < fileCount; i++) free(files[i]);
    free(fileNames);
    free(animNames);
    CloseWindow();
    return 0;
}
