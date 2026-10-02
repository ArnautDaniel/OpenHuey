/* Sony libvu0 (VU0 macro-mode vector / matrix helpers) in plain C. Matrices are row-major
 * float[4][4] with the PS2 convention v' = v * M (rows are the basis vectors, row 3 the
 * translation). Semantics as tools/difftest.py's vu0_hle. */
#include <math.h>

typedef float Vec[4];
typedef float Mat[4][4];

void sceVu0CopyVector(Vec d, const Vec s) {
    int i;

    for (i = 0; i < 4; i++) {
        d[i] = s[i];
    }
}

void sceVu0CopyMatrix(Mat d, const Mat s) {
    int i, k;

    for (i = 0; i < 4; i++) {
        for (k = 0; k < 4; k++) {
            d[i][k] = s[i][k];
        }
    }
}

void sceVu0AddVector(Vec d, const Vec a, const Vec b) { int i; for (i = 0; i < 4; i++) d[i] = a[i] + b[i]; }
void sceVu0SubVector(Vec d, const Vec a, const Vec b) { int i; for (i = 0; i < 4; i++) d[i] = a[i] - b[i]; }
void sceVu0MulVector(Vec d, const Vec a, const Vec b) { int i; for (i = 0; i < 4; i++) d[i] = a[i] * b[i]; }
void sceVu0ScaleVector(Vec d, const Vec a, float s) { int i; for (i = 0; i < 4; i++) d[i] = a[i] * s; }

/* scale x, y, z; keep w (PS2 0x0010E640) */
void func_0010E640(Vec d, const Vec a, float s) {
    d[0] = a[0] * s;
    d[1] = a[1] * s;
    d[2] = a[2] * s;
    d[3] = a[3];
}

float sceVu0InnerProduct(const Vec a, const Vec b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

void sceVu0OuterProduct(Vec d, const Vec a, const Vec b) {
    Vec r = {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0], 0.0f};

    sceVu0CopyVector(d, r);
}

/* x, y, z to unit length (a zero vector stays zero); w kept */
void sceVu0Normalize(Vec d, const Vec a) {
    float n = sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    float k = n != 0.0f ? 1.0f / n : 0.0f;
    Vec r = {a[0] * k, a[1] * k, a[2] * k, a[3]};

    sceVu0CopyVector(d, r);
}

void sceVu0UnitMatrix(Mat m) {
    int i, k;

    for (i = 0; i < 4; i++) {
        for (k = 0; k < 4; k++) {
            m[i][k] = i == k ? 1.0f : 0.0f;
        }
    }
}

/* d = v * M */
void sceVu0ApplyMatrix(Vec d, const Mat m, const Vec v) {
    Vec r;
    int k;

    for (k = 0; k < 4; k++) {
        r[k] = m[0][k] * v[0] + m[1][k] * v[1] + m[2][k] * v[2] + m[3][k] * v[3];
    }
    sceVu0CopyVector(d, r);
}

/* d = b * a (apply a after b, the libvu0 order) */
void sceVu0MulMatrix(Mat d, const Mat a, const Mat b) {
    Mat r;
    int i;

    for (i = 0; i < 4; i++) {
        sceVu0ApplyMatrix(r[i], a, b[i]);
    }
    sceVu0CopyMatrix(d, r);
}

void sceVu0TransposeMatrix(Mat d, const Mat m) {
    Mat r;
    int i, k;

    for (i = 0; i < 4; i++) {
        for (k = 0; k < 4; k++) {
            r[i][k] = m[k][i];
        }
    }
    sceVu0CopyMatrix(d, r);
}

static void rot(Mat d, const Mat m, int axis, float t) {
    float c = cosf(t), s = sinf(t);
    Mat r;

    sceVu0UnitMatrix(r);
    if (axis == 0) {
        r[1][1] = c; r[1][2] = s; r[2][1] = -s; r[2][2] = c;
    } else if (axis == 1) {
        r[0][0] = c; r[0][2] = -s; r[2][0] = s; r[2][2] = c;
    } else {
        r[0][0] = c; r[0][1] = s; r[1][0] = -s; r[1][1] = c;
    }
    sceVu0MulMatrix(d, r, m);
}

void sceVu0RotMatrixX(Mat d, const Mat m, float t) { rot(d, m, 0, t); }
void sceVu0RotMatrixY(Mat d, const Mat m, float t) { rot(d, m, 1, t); }
void sceVu0RotMatrixZ(Mat d, const Mat m, float t) { rot(d, m, 2, t); }

void sceVu0TransMatrix(Mat d, const Mat m, const Vec v) {
    sceVu0CopyMatrix(d, m);
    d[3][0] += v[0];
    d[3][1] += v[1];
    d[3][2] += v[2];
}

/* the view-to-screen (perspective) matrix: x, y scaled by scrz * a and centred on c, z mapped
 * from near..far to zmin..zmax (after the divide by w = z) */
void sceVu0ViewScreenMatrix(Mat m, float scrz, float ax, float ay, float cx, float cy,
                            float zmin, float zmax, float nearz, float farz) {
    float cz = (-zmax * nearz + zmin * farz) / (-nearz + farz);
    float az = farz * nearz * (-zmin + zmax) / (-nearz + farz);
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = 0.0f;
        }
    }
    m[0][0] = ax * scrz;
    m[1][1] = ay * scrz;
    m[2][0] = cx;
    m[2][1] = cy;
    m[2][2] = cz;
    m[2][3] = 1.0f;
    m[3][2] = az;
}

/* the world-to-camera matrix for a camera at p looking along zd with up yd: the inverse of the
 * camera's frame (x = yd x zd, z = zd, y = z x x; all unit), a rigid transform */
void sceVu0CameraMatrix(Mat m, const Vec p, const Vec zd, const Vec yd) {
    Vec x, y, z, t;
    int i;

    sceVu0OuterProduct(t, yd, zd);
    sceVu0Normalize(x, t);
    sceVu0Normalize(z, zd);
    sceVu0OuterProduct(y, z, x);
    for (i = 0; i < 3; i++) {
        m[i][0] = x[i];
        m[i][1] = y[i];
        m[i][2] = z[i];
        m[i][3] = 0.0f;
    }
    m[3][0] = -(x[0] * p[0] + x[1] * p[1] + x[2] * p[2]);
    m[3][1] = -(y[0] * p[0] + y[1] * p[1] + y[2] * p[2]);
    m[3][2] = -(z[0] * p[0] + z[1] * p[1] + z[2] * p[2]);
    m[3][3] = 1.0f;
}

/* rotate about x, then y, then z by rot[0..2] */
void sceVu0RotMatrix(Mat d, const Mat m, const Vec r) {
    Mat t;

    sceVu0RotMatrixX(t, m, r[0]);
    sceVu0RotMatrixY(t, t, r[1]);
    sceVu0RotMatrixZ(d, t, r[2]);
}
