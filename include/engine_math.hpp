/* date = October 31st 2022 6:43 pm */

#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

#include <immintrin.h>
#include <math.h>
#include <stdint.h>
typedef int8_t       s8;
typedef uint8_t      u8;
typedef int16_t      s16;
typedef uint16_t     u16;
typedef int32_t      s32;
typedef uint32_t     u32;
typedef int64_t      s64;
typedef uint64_t     u64;

typedef float  f32;
typedef double f64;


#ifndef M_PI
#define M_PI     3.14159265358979323846f
#define M_PI_2   (M_PI / 2.0f)
#define M_PI_M_2 (M_PI * 2.0f)
#define M_TAU    6.283185307179586476925f
#endif

//TODO use or get rid of it??
static int compare_float(double f1, double f2)
{
    double precision = 0.00000000000000000001;
    if ((f1 - precision) < f2)
    {
        return -1;
    }
    else if ((f1 + precision) > f2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

typedef union
{
    f32 e[2];
    struct { f32 x; f32 y;};
    struct { f32 u; f32 v;};
} vec2;

typedef union
{
    f32 e[3];
    struct { f32 x; f32 y; f32 z; };
    struct { f32 r; f32 g; f32 b; };
    struct { vec2 xy; f32 _z;};
    struct { f32 pitch; f32 roll; f32 yaw;};
} vec3;

typedef union vec4
{
    f32 e[4];
    struct { f32 x; f32 y; f32 z; f32 w; };
    struct { f32 r; f32 g; f32 b; f32 a; };
    vec3 xyz;
    vec3 rgb;
    vec2 xy;
} vec4;

union float16
{
    f32 e[16];
};

typedef struct v4x4
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} v4x4;

/*
0  1  2  3
4  5  6  7
8  9  10 11
12 13 14 15
*/

typedef union m4x4
{
    f32 e[16];
    f32 em[4][4];
    v4x4 vec;
    vec4 c[4];
} m4x4;

static m4x4
identity()
{
    return {{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f,
    }};
}

static void PrintV4(vec4 v);
static vec3 initVec3(f32 x1, f32 y1, f32 z1);

//static f32 Dot(vec3 a, vec3 b);
static f32 GetMagnitude(vec3 vec);
static void Normalize(vec3 vec);

static vec3 CrossV3(vec3 a, vec3 b);
static void Printvector3(vec3 v);
static vec3 Reflect(vec3 v, vec3 n);
static f32 Length(vec3 a, vec3 b);
static f32 LengthSquared(vec3 a, vec3 b);

typedef struct
{
    vec3 min;
    vec3 max;
} AABB;

//static vec3 ClosestPoint(AABB aabb, vec3 point);
static char AABBAABBCollision(AABB a, AABB b);
static char AABBSphereCollision(vec3 SphereCenter, f32 radius, AABB aabb);
static f32 rayHitSphere(vec3 center, f32 radius, vec3 rayOrigin, vec3 rayDirection);
static char sphereCollison(vec4 a, f32 radiusA, vec4 b, f32 radiusB);


static void Homogenize(vec4 r);


static m4x4 IdentityMatrix();
static m4x4 TranslationMatrix(vec3 translation);
static m4x4 ScaleMatrix(vec3 scale);
static m4x4 XRotationMatrix(f32 angle);
static m4x4 YRotationMatrix(f32 angle);
static m4x4 ZRotationMatrix(f32 angle);
static m4x4 TransformMatrix(vec3 rotation, vec3 position, vec3 scale);
static m4x4 FrustrumMatrix(f64 left, f64 right, f64 bottom, f64 top, f64 myNear, f64 myFar);
static m4x4 PerspectiveMatrix(f64 fovy, f64 aspect, f64 zNear, f64 zFar);
static m4x4 OrthoMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 nearVal, f32 farVal);
static m4x4 InverseMatrix(m4x4 a);
static void TransposeMatrix(m4x4 &r);
static void PrintMatrix(m4x4 r);
static void PrintMatrixTransposed(m4x4 r);


static vec3 getBarycentricCoordonates(vec2 a, vec2 b, vec2 c, vec2 p, f32 *areaABCp);
static f32 barycentricInterpolationf32(vec3 barycentricCoordinates, f32 *value);
static vec2 barycentricInterpolationv2(vec3 barycentricCoordinates, vec2 *value);


static f32 Minf32(f32 a, f32 b);
static f32 Maxf32(f32 a, f32 b);

static vec2 Minv2(vec2 a, vec2 b);
static vec2 Maxv2(vec2 a, vec2 b);

static vec3 Minv3(vec3 a, vec3 b);
static vec3 Maxv3(vec3 a, vec3 b);

static f32 Clamp(f32 value, f32 lower, f32 upper);
static f32 Lerp(f32 a, f32 b, f32 t);
static f32 InverseLerp(f32 x, f32 y, f32 value);
static vec3 GetPosition(m4x4 * matrix);
static void SetPosition(m4x4 * matrix, vec3 position);
static void SetScale(m4x4 * matrix, vec3 scale);

static vec3 getNormalFromTriangle(vec3 v0, vec3 v1, vec3 v2);
static vec3 GetSphericalCoords(f32 r, f32 theta, f32 phi);

static m4x4 mulMat4(m4x4 a, m4x4 b);
static vec4 Cross(vec4 a, vec4 b);
static vec3 subVec3(vec3 v0, vec3 v1);
static vec3 NormalizeV3(vec3 vec);

static vec3 operator-(vec3& a, vec3& b);

#endif //ENGINE_MATH_H

#ifdef MATHS_HPP_IMPLEMENTATION

#include <stdio.h>

static void
PrintV4(vec4 v)
{
    char OutputStringDebug[254];
    snprintf(OutputStringDebug, 254, "x.%f y.%f z.%f w.%f\n", v.x, v.y, v.z, v.w);
    OutputDebugStringA(OutputStringDebug);
}

static vec3
initVec3(f32 x1, f32 y1, f32 z1){
    vec3 result = {0};

    result.x = x1; result.y = y1; result.z = z1;
    return (result);
}

static m4x4
mulMat4(m4x4 a, m4x4 b)
{
    m4x4 result = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.em[i][j] += a.em[i][k] * b.em[k][j];
            }
        }
    }
    return (result);
}

static vec3
mulMat4Vec3(m4x4 a, vec3 v)
{
    vec3 result;
    v4x4 *m = (v4x4 *)&a;

    result.x = v.x * m->x.x + v.y * m->y.x + v.z * m->z.x + 1 * m->w.x;
    result.y = v.x * m->x.y + v.y * m->y.y + v.z * m->z.y + 1 * m->w.y;
    result.z = v.x * m->x.z + v.y * m->y.z + v.z * m->z.z + 1 * m->w.z;
    return (result);
}

static vec4
mulMat4Vec4(m4x4 a, vec4 v)
{
    vec4 result;
    v4x4 *m = (v4x4 *)&a;

    result.x = v.x * m->x.x + v.y * m->y.x + v.z * m->z.x + v.w * m->w.x;
    result.y = v.x * m->x.y + v.y * m->y.y + v.z * m->z.y + v.w * m->w.y;
    result.z = v.x * m->x.z + v.y * m->y.z + v.z * m->z.z + v.w * m->w.z;
    result.w = v.x * m->x.w + v.y * m->y.w + v.z * m->z.w + v.w * m->w.w;
    return (result);
}

static vec4
addVec4(vec4 v0, vec4 v1)
{
    vec4 resVal;

    resVal.x = v0.x + v1.x;
    resVal.y = v0.y + v1.y;
    resVal.z = v0.z + v1.z;
    resVal.w = v0.w + v1.w;
    return resVal;
}

static vec4
minusVec4(vec4 v0, vec4 v1)
{
    vec4 resVal;

    resVal.x = v0.x - v1.x;
    resVal.y = v0.y - v1.y;
    resVal.z = v0.z - v1.z;
    resVal.w = v0.w - v1.w;
    return resVal;

}

static vec4
divideV4f32(vec4 v, f32 a)
{
    vec4 result = {0};

    if (a < 0.001f)
        a = 1;
    result.x = v.x / a;
    result.y = v.y / a;
    result.z = v.z / a;
    result.w = v.w / a;
    return (result);
}

static vec4
mulitplyV4f32(vec4 v, f32 s)
{
    vec4 resVal = {0};
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    resVal.z = v.z * s;
    resVal.w = v.w * s;
    return resVal;

}

static vec3
addVec3(vec3 v0, vec3 v1)
{
    vec3 resVal;

    resVal.x = v0.x + v1.x;
    resVal.y = v0.y + v1.y;
    resVal.z = v0.z + v1.z;
    return resVal;
}

static vec3
subVec3(vec3 v0, vec3 v1)
{
    vec3 resVal;

    resVal.x = v0.x - v1.x;
    resVal.y = v0.y - v1.y;
    resVal.z = v0.z - v1.z;
    return resVal;
}

static vec3
mulitplyV3f32(vec3 v, f32 s)
{
    vec3 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    resVal.z = v.z * s;
    return resVal;

}

static vec3
divideV3f32(vec3 v, f32 a)
{
    vec3 result = {0};

    if (a < 0.001f)
        a = 1;
    result.x = v.x / a;
    result.y = v.y / a;
    result.z = v.z / a;
    return (result);
}

#if __cplusplus
static vec2
operator-(vec2& a, vec2&  b)
{
    vec2 resVal;
    resVal.x = a.x - b.x;
    resVal.y = a.y - b.y;
    return resVal;

}

static vec2 operator+(vec2& a,vec2& b)
{
    vec2 resVal;
    resVal.x = a.x + b.x;
    resVal.y = a.y + b.y;
    return resVal;

}

static vec2 operator*(f32& s, vec2& v)
{
    vec2 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    return resVal;
}

static vec2 operator*(vec2& v, f32& s)
{
    vec2 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    return resVal;
}

static vec3 operator+(vec3 a, vec3 b)
{
    vec3 result = {};

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return (result);
}

static vec3 operator*(f32 s, vec3& v)
{
    return {v.x * s, v.y * s, v.z * s};
}

static vec3 operator*(vec3 a, f32 scale)
{
    vec3 result;

    result.x = a.x * scale;
    result.y = a.y * scale;
    result.z = a.z * scale;
    return (result);
}

static void operator+=(vec3 &r, vec3 a)
{
    r.x = r.x + a.x;
    r.y = r.y + a.y;
    r.z = r.z + a.z;
}

static void operator-=(vec3 &r, vec3 a)
{
    r.x = r.x - a.x;
    r.y = r.y - a.y;
    r.z = r.z - a.z;
}

static vec3 operator-(vec3& a, vec3& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static vec3 operator-(vec3 r)
{
    return {-r.x, -r.y, -r.z};
}

static void operator*=(vec3 &r, f32 scale)
{
    r.x = r.x * scale;
    r.y = r.y * scale;
    r.z = r.z * scale;
}

static m4x4 operator*=(m4x4 a, m4x4 b)
{
    m4x4 result = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.em[i][j] += a.em[i][k] * b.em[k][j];
            }
        }
    }
    return (result);
}

static m4x4 operator*(m4x4 a, m4x4 b)
{
    m4x4 result = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.em[i][j] += a.em[i][k] * b.em[k][j];
            }
        }
    }
    return (result);
}

#endif

static f32
GetMagnitudeV3(vec3 vec)
{
    __m128 value = {0};
    f32 *result = (f32 *)&value;
    *result = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
    value = _mm_sqrt_ps(value);
    return (*result);
}

static f32
DotV3(vec3 a, vec3 b)
{
    return ((f32)(a.x * b.x + a.y * b.y + a.z * b.z));
}

static vec3
NormalizeV3(vec3 vec)
{
    f32 length = GetMagnitudeV3(vec);

    if (length == 0.0f)
        length = 1.0;
    vec.x = vec.x / length;
    vec.y = vec.y / length;
    vec.z = vec.z / length;
    return (vec);
}

static vec3
CrossV3(vec3 a, vec3 b)
{
    //A x B = (a2b3  -   a3b2,     a3b1   -   a1b3,     a1b2   -   a2b1)
    vec3 result = {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
    return(result);
}

static void
PrintV3(vec3 v)
{
    char OutputStringDebug[254];
    snprintf(OutputStringDebug, 254, "x.%f y.%f z.%f\n", v.x, v.y, v.z);
    OutputDebugStringA(OutputStringDebug);
}

static vec3
minusV3(vec3 a, vec3 b)
{
    vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

static vec3
mulV3f32(vec3 r, f32 scale)
{
    vec3 result;
    result.x = r.x * scale;
    result.y = r.y * scale;
    result.z = r.z * scale;
    return (result);
}

//Reflect V about (normalized) N
static vec3
ReflectV3(vec3 v, vec3 n)
{
    return minusV3(v, mulV3f32(mulV3f32(n, DotV3(v,n)), 2.0f));
}

f32 Length(vec3 a, vec3 b)
{
    f32 x = a.x - b.x;
    f32 y = a.y - b.y;
    f32 z = a.z - b.z;

    __m128 value = {0};
    f32 *result = (f32 *)&value;
    *result = x * x + y * y + z * z;
    value = _mm_sqrt_ps(value);
    return (*result);
}

static f32 Minf(f32 a, f32 b)
{
    if (a <= b)
        return (a);
    return (b);
}

static f32 Maxf(f32 a, f32 b)
{
    if (a >= b)
        return (a);
    return (b);
}

static vec2 Minf(vec2 a, vec2 b)
{
    vec2 result;

    result.x = Minf(a.x, b.x);
    result.y = Minf(a.y, b.y);
    return (result);
}

static vec2 Maxf(vec2 a, vec2 b)
{
    vec2 result;

    result.x = Maxf(a.x, b.x);
    result.y = Maxf(a.y, b.y);
    return (result);
}

static f32 Clamp(f32 value, f32 lower,f32 upper)
{
    return Minf(upper, Maxf(value, lower));
}

#if 1

static f32 LengthSquared(vec3 a, vec3 b)
{
    f32 x = a.x - b.x;
    f32 y = a.y - b.y;
    f32 z = a.z - b.z;

    return (x * x + y * y + z * z);
}

static vec3 ClosestPoint(AABB &aabb, vec3 &point)
{
    vec3 result = {};

    if ( point.x > aabb.max.x)
        result.x = aabb.max.x;
    else if (point.x < aabb.min.x)
        result.x = aabb.min.x;
    else
        result.x = point.x;

    if (point.y > aabb.max.y)
        result.y = aabb.max.y;
    else if (point.y < aabb.min.y)
        result.y = aabb.min.y;
    else
        result.y = point.y;

    if (point.z > aabb.max.z)
        result.z = aabb.max.z;
    else if (point.z < aabb.min.z)
        result.z = aabb.min.z;
    else
        result.z = point.z;
    return (result);
}

static char AABBAABBCollision(AABB a, AABB b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

static char AABBSphereCollision(vec3 SphereCenter, f32 radius, AABB &aabb)
{
    //If the length squared of the closest point from aabb to Sphere is smaller than the radius squared
    // a collision has occured.
    return (LengthSquared(ClosestPoint(aabb, SphereCenter), SphereCenter) < (radius * radius));
}

static f32 rayHitSphere(vec3& center, f32 radius, vec3 rayOrigin, vec3 rayDirection){
    vec3 oc = rayOrigin - center;
    f32 a = DotV3(rayDirection, rayDirection);
    f32 b = 2.0 * DotV3(oc, rayDirection);
    f32 c = DotV3(oc,oc) - radius*radius;
    f32 discriminant = b*b - 4*a*c;
    if(discriminant < 0){
        return -1.0;
    }else{
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

static char sphereCollison(vec4 &a, f32 &radiusA, vec4 &b, f32 &radiusB)
{
    f32 radiusSquared = (radiusA * 2) * (radiusB * 2);

    f32 distance = LengthSquared(a.xyz, b.xyz);
    if (distance <= radiusSquared)
        return (true);
    return (0);
}

static void Homogenize(vec4 &r)
{
    r.x /= r.w;
    r.y /= r.w;
    r.z /= r.w;
    // We keep the w value for perspective interpolation later
    // We don't devide it by itself, because it becomes 1;
}



static vec3 operator*(m4x4& a, vec3 &vecs)
{
    vec3 result = {};

	result.x = a.vec.x.x*vecs.x + a.vec.y.x*vecs.y + a.vec.z.x*vecs.z + a.vec.w.x;
    result.y = a.vec.x.y*vecs.x + a.vec.y.y*vecs.y + a.vec.z.y*vecs.z + a.vec.w.y;
    result.z = a.vec.x.z*vecs.x + a.vec.y.z*vecs.y + a.vec.z.z*vecs.z + a.vec.w.z;
	return (result);
}

static vec4 operator*(m4x4 &a, vec4 vecs)
{
    vec4 result = {};

	result.x = a.vec.x.x*vecs.x + a.vec.y.x*vecs.y + a.vec.z.x*vecs.z + a.vec.w.x*vecs.w;
    result.y = a.vec.x.y*vecs.x + a.vec.y.y*vecs.y + a.vec.z.y*vecs.z + a.vec.w.y*vecs.w;
    result.z = a.vec.x.z*vecs.x + a.vec.y.z*vecs.y + a.vec.z.z*vecs.z + a.vec.w.z*vecs.w;
    result.w = a.vec.x.w*vecs.x + a.vec.y.w*vecs.y + a.vec.z.w*vecs.z + a.vec.w.w*vecs.w;
    return (result);
}

m4x4 InverseMatrix(m4x4 a)
{
    m4x4 result = {};
    int i, j, k, p;
    f32 f, g, tol;

    f = 0.f;  /* Frobenius norm of a */
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            g = a.em[i][j];
            f += g * g;
        }
    }

    f      = sqrt(f);
    tol    = (f32)(f * 2.2204460492503131e-016);
    result = IdentityMatrix();

    for (k = 0; k < 4; ++k)
    {  /* Main loop */
        f = fabs(a.em[k][k]);  /* Find pivot. */
        p = k;

        for (i = k+1; i < 4; ++i)
        {
            g = fabs(a.em[i][k]);
            if (g > f) { f = g;  p = i; }
        }

        if (f < tol) return (result);  /* Matrix is singular. */

        if (p != k)
        {  /* Swap rows. */
            for (j = k; j < 4; ++j)
            {
                f = a.em[k][j];
                a.em[k][j] = a.em[p][j];
                a.em[p][j] = f;
            }
            for (j = 0; j < 4; ++j)
            {
                f = result.em[k][j];
                result.em[k][j] = result.em[p][j];
                result.em[p][j] = f;
            }
        }

        f = 1.f / a.em[k][k];  /* Scale row so pivot is 1. */

        for (j = k; j < 4; ++j) a.em[k][j] *= f;
        for (j = 0; j < 4; ++j) result.em[k][j] *= f;

        for (i = 0; i < 4; ++i)
        {  /* Subtract to get zeros. */
            if (i == k) continue;
            f = a.em[i][k];
            for (j = k; j < 4; ++j) a.em[i][j]      -= a.em[k][j]      * f;
            for (j = 0; j < 4; ++j) result.em[i][j] -= result.em[k][j] * f;
        }
    }
    return (result);
}

//printing in a COLUMN-ROW matrix as COLUMN-MAJOR matrix
static void PrintMatrix(m4x4 &r)
{
    printf("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
           r.e[0],  r.e[1],  r.e[2],  r.e[3],
           r.e[4],  r.e[5],  r.e[6],  r.e[7],
           r.e[8],  r.e[9],  r.e[10], r.e[11],
           r.e[12], r.e[13], r.e[14], r.e[15]);
}

//printing in a COLUMN-ROW matrix as ROW-MAJOR matrix
static void PrintMatrixTransposed(m4x4 &r)
{
    printf("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
           r.e[0], r.e[4], r.e[8],  r.e[12],
           r.e[1], r.e[5], r.e[9],  r.e[13],
           r.e[2], r.e[6], r.e[10], r.e[14],
           r.e[3], r.e[7], r.e[11], r.e[15]);
}

static float orient2d(vec2& a, vec2& b, vec2& c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

static vec3 getBarycentricCoordonates(vec2 a, vec2 b, vec2 c, vec2 p,
                               f32 *areaABCp)
{
    vec3 result = {};
    f32 areaABC = orient2d(a, b, c);

    result.x = orient2d(b, c, p) / areaABC;
    result.y = orient2d(c, a, p) / areaABC;
    result.z = orient2d(a, b, p) / areaABC;

    if (areaABCp != NULL)
        *areaABCp = areaABC;
    return result;
}

static f32 barycentricInterpolation(vec3 barycentricCoordinates, f32 *value)
{
    return(value[0] + barycentricCoordinates.y * (value[1] - value[0]) +
           barycentricCoordinates.z * (value[2] - value[0]));
}

// static vec2 barycentricInterpolation(vec3 barycentricCoordinates,
//                               vec2 *value)
// {
//     return(value[0] + (barycentricCoordinates.y * (value[1] - value[0]) +
//                        barycentricCoordinates.z * (value[2] - value[0])));
// }

static vec3 Minf(vec3 a, vec3 b)
{
    vec3 result;

    result.x = Minf(a.x, b.x);
    result.y = Minf(a.y, b.y);
    result.z = Minf(a.z, b.z);
    return (result);
}

static vec3 Maxf(vec3 a, vec3 b)
{
    vec3 result;

    result.x = Maxf(a.x, b.x);
    result.y = Maxf(a.y, b.y);
    result.z = Maxf(a.z, b.z);
    return (result);
}

static f32 Lerp(f32 a,f32 b,f32 t)
{
    return (1 - t) * a + b * t;
}

static f32 InverseLerp(f32 x, f32 y, f32 value)
{
    return (value - x)/(y - x);
}

static vec3 GetPosition(m4x4 *matrix){return (matrix->c[3].xyz);}

static void SetPosition(m4x4 *matrix, vec3 position){matrix->c[3].xyz = position;}

static void SetScale(m4x4 *matrix, vec3 scale)
{
    matrix->vec.x.x = scale.x;
    matrix->vec.y.y = scale.y;
    matrix->vec.z.z = scale.z;
}

static vec3 getNormalFromTriangle(vec3 v0, vec3 v1, vec3 v2)
{
    vec3 normal;

    vec3 a = {{v1.x - v0.x, v1.y - v0.y, v1.z - v0.z}};
    vec3 b = {{v2.x - v0.x, v2.y - v0.y, v2.z - v0.z}};

    normal.x = a.y * b.z - a.z * b.y;
    normal.y = a.z * b.x - a.x * b.z;
    normal.z = a.x * b.y - a.y * b.x;
    return (normal);
}

static vec4 CrossV4(vec4 &a, vec4 &b)
{
    //A x B = (a2b3  -   a3b2,     a3b1   -   a1b3,     a1b2   -   a2b1)
	vec4 result = {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0};
    return(result);
}

#endif

m4x4 IdentityMatrix()
{
    m4x4 result = {0};
    v4x4 *vec = &result.vec;

    vec->x.x = 1;
    vec->y.y = 1;
    vec->z.z = 1;
    vec->w.w = 1;
    return (result);
};

m4x4 TranslationMatrix(vec3 translation)
{
    m4x4 result = IdentityMatrix();
    v4x4 *vec = &result.vec;

    vec->w.x = translation.x;
    vec->w.y = translation.y;
    vec->w.z = translation.z;
    return (result);
}

m4x4 ScaleMatrix(vec3 scale)
{
    m4x4 result = IdentityMatrix();;

    v4x4 *vec = &result.vec;
    vec->x.x = scale.x;
    vec->y.y = scale.y;
    vec->z.z = scale.z;
    return(result);
};

m4x4 XRotationMatrix(f32 angle)
{
    m4x4 result = IdentityMatrix();
    v4x4 *vec = &result.vec;

    vec->y.y =  cos(angle);
    vec->y.z =  sin(angle);
    vec->z.y =  -sin(angle);
    vec->z.z =  cos(angle);
    return (result);
}

m4x4 YRotationMatrix(f32 angle)
{
    m4x4 result = IdentityMatrix();

    v4x4 *vec = &result.vec;

    vec->x.x =  cos(angle);
    vec->x.z =  -sin(angle);
    vec->z.x =  sin(angle);
    vec->z.z =  cos(angle);
    return (result);
}

m4x4 ZRotationMatrix(f32 angle)
{
    m4x4 result = IdentityMatrix();
    v4x4 *vec = &result.vec;

    vec->x.x =  cos(angle);
    vec->x.y =  sin(angle);
    vec->y.x =  -sin(angle);
    vec->y.y =  cos(angle);
    return (result);
}

void TransposeMatrix(m4x4 &r)
{
    int i, j;
    m4x4 temp = {};

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            temp.em[j][i] = r.em[i][j];
    }

    for (int i = 0; i < 16; i++)
        r.e[i] = temp.e[i];
};

m4x4
TransformMatrix(vec3 rotation,
                vec3 position,
                vec3 scale)
{
    m4x4 r = {0};
    m4x4 translationMatrix = {0};
    m4x4 RotationYMatrix = {0};
    m4x4 RotationXMatrix = {0};
    m4x4 RotationZMatrix = {0};
    m4x4 scaleMatrix = {0};

    translationMatrix = TranslationMatrix(position);
    RotationYMatrix   = YRotationMatrix  (rotation.y);
    RotationXMatrix   = XRotationMatrix  (rotation.x);
    RotationZMatrix   = ZRotationMatrix  (rotation.z);
    scaleMatrix       = ScaleMatrix      (scale);

    r = scaleMatrix * RotationXMatrix * RotationZMatrix * RotationYMatrix * translationMatrix;
    return (r);
}

m4x4 FrustrumMatrix(f64 left,
                    f64 right,
                    f64 bottom,
                    f64 top,
                    f64 myNear,
                    f64 myFar)
{
    m4x4 result = {0};

    f64 a = (right + left) / (right - left);
    f64 b = (top + bottom) / (top - bottom);
    f64 c = -((myFar + myNear) / (myFar - myNear));
    f64 d = -((2 * myFar * myNear) / (myFar - myNear));

    result.em[0][0] =(f32) (2 * myNear / (right - left));

    result.em[1][1] =(f32) (2 * myNear / (top - bottom));

    result.em[2][0] = (f32)a;
    result.em[2][1] = (f32)b;
    result.em[2][2] = (f32)c;
    result.em[2][3] = -1;

    result.em[3][2] = (f32)d;

    return (result);
}

static f32
my_tanf(f32 angle)
{
    return (sin(angle)/cos(angle));
}

m4x4 PerspectiveMatrix(f64 fovy,
                       f64 aspect,
                       f64 zNear,
                       f64 zFar)
{
    m4x4 result = {0};

    f64 yMax = zNear * my_tanf((f32)(fovy * M_PI / 360.0f));
    f64 xMax = yMax * aspect;

    result = FrustrumMatrix(-xMax, xMax, -yMax, yMax, zNear, zFar);
    return (result);
}

m4x4 OrthoMatrix(f32 left, f32 right, f32 bottom, f32 top,
                 f32 nearVal, f32 farVal)
{
    m4x4 result = {0};

    result.em[0][0] = 2 / (right - left);
    result.em[1][1] = -2 / (top - bottom);
    result.em[2][2] = -2 / (farVal - nearVal);

    result.em[3][0] = -((right + left) / (right - left));
    result.em[3][1] = -((top + bottom) / (top - bottom));
    result.em[3][2] = -((farVal + nearVal) / (farVal - nearVal));
    result.em[3][3] = 1;

    return (result);
}

#undef MATHS_HPP_IMPLEMENTATION
#endif //MATHS_HPP_IMPLEMENTATION