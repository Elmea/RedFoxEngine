#include "pch.h"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedFoxMaths.hpp"
using namespace RedFoxMaths;

#pragma region Float2Test

TEST(Float2, CrossProductTest) {
  float expected = glm_vec2_cross(new(vec2){ 2.f, 3.f }, new(vec2){ 4.f, 5.f });
  Float2 a = { 2.f, 3.f }, b = { 4.f, 5.f };
  float result = a.CrossProduct(b);
  EXPECT_EQ(result, expected);
  EXPECT_TRUE(true);
}

TEST(Float2, DotProductTest) {
  float expected = glm_vec2_dot(new(vec2){ 2.f, 3.f }, new(vec2){ 4.f, 5.f });
  Float2 a ={ 2.f, 3.f, }, b = { 4.f, 5.f };
  float result = a.DotProduct(b);
  EXPECT_EQ(result, expected);
  EXPECT_TRUE(true);
}

TEST(Float2, PlanRotationTest) {
  vec2s expected;
  glm_vec2_rotate(new(vec2){ 2.f, 3.f }, PI/2, expected.raw);
  float angle = 90.f;
  Float2 result ={ 2.f, 3.f, };
  result.PlanRotation(angle);
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

TEST(Float2, MagnitudeTest) {
  float expected = glm_vec2_norm(new(vec2){ 2.f, 3.f });
  Float2 a ={ 2.f, 3.f, };
  float result = a.Magnitude();
  EXPECT_EQ(result, expected);
  EXPECT_TRUE(true);
}

TEST(Float2, NormalizeTest) {
  vec2s expected = { 2.f,3.f };
  glm_vec2_normalize(expected.raw);
  Float2 a ={ 2.f, 3.f, };
  Float2 result = a.GetNormalized();
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

TEST(Float2, LerpTest) {
  vec2s expected = {0};
  glm_vec2_lerp(new(vec2){ 2.f, 3.f }, new(vec2){ 4.f, 5.f }, 0.5f, expected.raw);
  Float2 result = Float2::Lerp({ 2.f, 3.f }, { 4.f, 5.f }, 0.5f);
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

TEST(Float2, AddTest) {
  vec2s expected = {0};
  glm_vec2_add(new(vec2){ 2.f, 3.f }, new(vec2){ 4.f, 5.f }, expected.raw);
  Float2 a ={ 2.f, 3.f, }, b = { 4.f, 5.f };
  Float2 result = a + b;
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}


TEST(Float2, SubTest) {
  vec2s expected = {0};
  glm_vec2_sub(new(vec2){ 2.f, 3.f }, new(vec2){ 4.f, 5.f }, expected.raw);
  Float2 a ={ 2.f, 3.f, }, b = { 4.f, 5.f };
  Float2 result = a - b;
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}


TEST(Float2, ScaleTest) {
  vec2s expected = {0};
  glm_vec2_scale(new(vec2){ 2.f, 3.f }, 4.f, expected.raw);
  Float2 a = { 2.f,3.f };
  Float2 result = a * 4.f;
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

TEST(Float2, DivTest) {
  vec2s expected = {0};
  glm_vec2_divs(new(vec2){ 2.f, 3.f }, 4.f, expected.raw);
  Float2 a = { 2.f,3.f };
  Float2 result = a / 4.f;
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

#pragma endregion

#pragma region Float3Test

TEST(Float3, CrossProductTest) {
    vec3s expected = { 0 };
    glm_vec3_cross(new(vec3){ 2.f, 3.f, 4.f }, new(vec3){ 5.f, 6.f, 7.f }, expected.raw);
    Float3 a = { 2.f, 3.f, 4.f }, b = { 5.f, 6.f, 7.f };
    Float3 result = a.CrossProduct(b);
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_TRUE(true);
}

TEST(Float3, DotProductTest) {
    float expected = glm_vec3_dot(new(vec3){ 2.f, 3.f, 4.f }, new(vec3){ 5.f, 6.f, 7.f });
    Float3 a = { 2.f, 3.f, 4.f }, b = { 5.f, 6.f, 7.f };
    float result = a.DotProduct(b);
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(true);
}

TEST(Float3, NormalizeTest) {
    float expected = glm_vec3_norm(new(vec3){ 2.f, 3.f, 4.f });
    Float3 a = { 2.f, 3.f, 4.f };
    float result = a.Magnitude();
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(true);
}

TEST(Float3, LerpTest) {
    vec3s expected = { 0 };
    glm_vec3_lerp(new(vec3){ 2.f, 3.f, 4.f }, new(vec3){ 5.f, 6.f, 7.f }, 0.5f, expected.raw);
    Float3 a = { 2.f, 3.f, 4.f }, b = { 5.f, 6.f, 7.f };
    Float3 result = Float3::Lerp(a, b, 0.5f);
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_TRUE(true);
}

TEST(Float3, AddTest) {
    vec3s expected = { 0 };
    glm_vec3_add(new(vec3){ 2.f, 3.f, 4.f }, new(vec3){ 5.f, 6.f, 7.f }, expected.raw);
    Float3 a = { 2.f, 3.f, 4.f }, b = { 5.f, 6.f, 7.f };
    Float3 result = a + b;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_TRUE(true);
}

TEST(Float3, SubTest) {
    vec3s expected = { 0 };
    glm_vec3_sub(new(vec3){ 2.f, 3.f, 4.f }, new(vec3){ 5.f, 6.f, 7.f }, expected.raw);
    Float3 a = { 2.f, 3.f, 4.f }, b = { 5.f, 6.f, 7.f };
    Float3 result = a - b;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_TRUE(true);
}

TEST(Float3, MulTest) {
    vec3s expected = { 0 };
    glm_vec3_scale(new(vec3){ 2.f, 3.f, 4.f }, 4.f, expected.raw);
    Float3 a = { 2.f, 3.f, 4.f };
    Float3 result = a * 4.f;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_TRUE(true);
}

TEST(Float3, DivTest) {
    vec3s expected = { 0 };
    glm_vec3_divs(new(vec3){ 2.f, 3.f, 4.f }, 4.f, expected.raw);
    Float3 a = { 2.f, 3.f, 4.f };
    Float3 result = a / 4.f;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_TRUE(true);
}

#pragma endregion

#pragma region Float4Test

TEST(Float4, DotProductTest) {
    float expected = glm_vec4_dot(new(vec4){ 2.f, 3.f, 4.f, 5.f }, new(vec4){ 6.f, 7.f, 8.f, 9.f });
    Float4 a = { 2.f, 3.f, 4.f , 5.f }, b = { 6.f, 7.f, 8.f, 9.f };
    float result = a.DotProduct(b);
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(true);
}

TEST(Float4, MagnitudeTest) {
    float expected = glm_vec4_norm(new(vec4){ 2.f, 3.f, 4.f, 5.f });
    Float4 a = { 2.f, 3.f, 4.f , 5.f };
    float result = a.Magnitude();
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(true);
}

TEST(Float4, NormalizeTest) {
    vec4s expected = { 2.f, 3.f, 4.f, 5.f };
    glm_vec4_normalize(expected.raw);
    Float4 a = { 2.f, 3.f, 4.f, 5.f };
    Float4 result = a.GetNormalized();
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_EQ(result.w, expected.w);
    EXPECT_TRUE(true);
}

TEST(Float4, AddTest) {
    vec4s expected = { 0 };
    glm_vec4_add(new(vec4){ 2.f, 3.f, 4.f, 5.f }, new(vec4){ 6.f, 7.f, 8.f, 9.f }, expected.raw);
    Float4 a = { 2.f, 3.f, 4.f, 5.f }, b = { 6.f, 7.f, 8.f, 9.f };
    Float4 result = a + b;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_EQ(result.w, expected.w);
    EXPECT_TRUE(true);
}

TEST(Float4, SubTest) {
    vec4s expected = { 0 };
    glm_vec4_sub(new(vec4){ 2.f, 3.f, 4.f, 5.f }, new(vec4){ 6.f, 7.f, 8.f, 9.f }, expected.raw);
    Float4 a = { 2.f, 3.f, 4.f, 5.f }, b = { 6.f, 7.f, 8.f, 9.f };
    Float4 result = a - b;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_EQ(result.w, expected.w);
    EXPECT_TRUE(true);
}

TEST(Float4, MulTest) {
    vec4s expected = { 0 };
    glm_vec4_scale(new(vec4){ 2.f, 3.f, 4.f, 5.f }, 6.f, expected.raw);
    Float4 a = { 2.f, 3.f, 4.f, 5.f };
    Float4 result = a * 6.f;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_EQ(result.w, expected.w);
    EXPECT_TRUE(true);
}

TEST(Float4, DivTest) {
    vec4s expected = { 0 };
    glm_vec4_divs(new(vec4){ 2.f, 3.f, 4.f, 5.f }, 6.f, expected.raw);
    Float4 a = { 2.f, 3.f, 4.f, 5.f };
    Float4 result = a / 6.f;
    EXPECT_EQ(result.x, expected.x);
    EXPECT_EQ(result.y, expected.y);
    EXPECT_EQ(result.z, expected.z);
    EXPECT_EQ(result.w, expected.w);
    EXPECT_TRUE(true);
}

#pragma endregion


#pragma region Mat4Test

TEST(Mat4, RotationXTest) {
    mat4 id, ex;
    glm_mat4_identity(id);
    glm_rotate_x(id, PI / 2.f, ex);
    Mat4 expected;
    float* expectedPtr = expected.mat16;
    expectedPtr = (float*)ex;
    Mat4 result = Mat4::GetRotationX(PI / 2.f);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, RotationYTest) {
    mat4 id, ex;
    glm_mat4_identity(id);
    glm_rotate_y(id, PI / 2.f, ex);
    Mat4 expected;
    float* expectedPtr = expected.mat16;
    expectedPtr = (float*)ex;
    Mat4 result = Mat4::GetRotationY(PI / 2.f);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, RotationZTest) {
    mat4 id, ex;
    glm_mat4_identity(id);
    glm_rotate_z(id, PI / 2.f, ex);
    Mat4 expected;
    float* expectedPtr = expected.mat16;
    expectedPtr = (float*)ex;
    Mat4 result = Mat4::GetRotationZ(PI / 2.f);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, RotationTest) {
    mat4 id;
    glm_mat4_identity(id);
    glm_rotate(id, PI / 2, new(vec3){ 1.f, 1.f, 1.f });
    Mat4 expected;
    memcpy(expected.mat16, &id[0], sizeof(float) * 16);
    Mat4 result = Mat4::GetRotation(PI / 2, PI / 2, PI / 2);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, TranslationTest) {
    mat4 ex;
    glm_mat4_identity(ex);
    glm_translate(ex, new(vec3){ 2.f, 3.f, 4.f });
    glm_mat4_transpose(ex);
    Mat4 expected;
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = Mat4::GetTranslation({ 2.f, 3.f, 4.f });
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, TransformTest) {
    mat4 ex;
    glm_mat4_identity(ex);
    glm_translate(ex, new(vec3){ 2.f, 3.f, 4.f });
    glm_rotate(ex, PI / 2, new(vec3){ 1.f, 1.f, 1.f });
    glm_scale(ex, new(vec3){ 1.f, 1.f, 1.f });
    glm_mat4_transpose(ex);
    Mat4 expected;
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Float3 position = { 2.f, 3.f, 4.f };
    Float3 rotation = { 2.f, 3.f, 4.f };
    Float3 scale    = { 2.f, 3.f, 4.f };
    Mat4 result = Mat4::CreateTransformMatrix(position, rotation, scale);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, TransformQuatTest) {
    mat4 ex; versor q;
    glm_mat4_identity(ex);
    glm_translate(ex, new(vec3){ 2.f, 3.f, 4.f });
    glm_quat_init(q, 3.f, 4.f, 5.f, 2.f);
    glm_quat_rotate(ex, q, ex);
    glm_scale(ex, new(vec3){ 1.f,1.f,1.f });
    glm_mat4_transpose(ex);
    Mat4 expected;
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Float3 position = { 2.f, 3.f, 4.f };
    Quaternion rotation = { 2.f, 3.f, 4.f, 5.f };
    Float3 scale = { 2.f, 3.f, 4.f };
    Mat4 result = Mat4::CreateTransformMatrix(position, rotation, scale);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, OrthographicTest) {
    mat4 ex;
    glm_mat4_identity(ex);
    glm_ortho(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, ex);
    glm_mat4_transpose(ex);
    Mat4 expected;
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = Mat4::GetOrthographicMatrix(2.f, 1.f, 4.f, 3.f, 6.f, 5.f);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, PerspectiveTest) {
    mat4 ex;
    glm_mat4_identity(ex);
    glm_perspective(90.f, 1.f, 0.1f, 100.f, ex);
    glm_mat4_transpose(ex);
    Mat4 expected;
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = Mat4::GetPerspectiveMatrix(1.f, 90.f, 100.f, 0.1f);
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, TransposeTest) {
    vec4 l0 = { 1.f,2.f,3.f,4.f };
    vec4 l1 = { 5.f,6.f,7.f,8.f };
    vec4 l2 = { 9.f,10.f,11.f,12.f };
    vec4 l3 = { 13.f,14.f,15.f,16.f };
    mat4 m1, ex;
    glm_vec4_copy(l0, m1[0]);
    glm_vec4_copy(l1, m1[1]);
    glm_vec4_copy(l2, m1[2]);
    glm_vec4_copy(l3, m1[3]);
    glm_mat4_copy(m1, ex);
    glm_mat4_transpose(m1);
    glm_mat4_transpose(ex);
    Mat4 mat1, expected;
    memcpy(mat1.mat16, &m1[0], sizeof(float) * 16);
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    EXPECT_TRUE(mat1 == expected);
}

TEST(Mat4, InverseTest) {
    vec4 l0 = { 2.f,0.f,1.f,3.f };
    vec4 l1 = { 1.f,1.f,-1.f,0.f };
    vec4 l2 = { 0.f,-1.f,2.f,-1.f };
    vec4 l3 = { 1.f,0.f,3.f,-2.f };
    mat4 m1, ex;
    glm_vec4_copy(l0, m1[0]);
    glm_vec4_copy(l1, m1[1]);
    glm_vec4_copy(l2, m1[2]);
    glm_vec4_copy(l3, m1[3]);
    glm_mat4_inv(m1, ex);
    glm_mat4_transpose(m1);
    glm_mat4_transpose(ex);
    Mat4 mat1, expected;
    memcpy(mat1.mat16, &m1[0], sizeof(float) * 16);
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = mat1.GetInverseMatrix();
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, IdentityTest) {
    mat4 ex;
    glm_mat4_identity(ex);
    glm_mat4_transpose(ex);
    Mat4 expected;
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = Mat4::GetIdentityMatrix();
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, MulTest) {
    vec4 l0 = {  1.f,  2.f,  3.f,  4.f };
    vec4 l1 = {  5.f,  6.f,  7.f,  8.f };
    vec4 l2 = {  9.f, 10.f, 11.f, 12.f };
    vec4 l3 = { 13.f, 14.f, 15.f, 16.f };
    mat4 ex, m1, m2;
    glm_vec4_copy(l0, m1[0]);
    glm_vec4_copy(l1, m1[1]);
    glm_vec4_copy(l2, m1[2]);
    glm_vec4_copy(l3, m1[3]);
    glm_vec4_copy(l3, m2[3]);
    glm_vec4_copy(l2, m2[2]);
    glm_vec4_copy(l1, m2[1]);
    glm_vec4_copy(l0, m2[0]);
    glm_mat4_mul(m1, m2, ex);
    Mat4 mat1, mat2, expected;
    memcpy(mat1.mat16, &m1[0], sizeof(float) * 16);
    memcpy(mat2.mat16, &m2[0], sizeof(float) * 16);
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = mat1 * mat2;
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, MulSTest) {
    vec4 l0 = { 1.f,  2.f,  3.f,  4.f };
    vec4 l1 = { 5.f,  6.f,  7.f,  8.f };
    vec4 l2 = { 9.f, 10.f, 11.f, 12.f };
    vec4 l3 = { 13.f, 14.f, 15.f, 16.f };
    mat4 ex, m1;
    glm_vec4_copy(l0, m1[0]);
    glm_vec4_copy(l1, m1[1]);
    glm_vec4_copy(l2, m1[2]);
    glm_vec4_copy(l3, m1[3]);
    glm_mat4_copy(m1, ex);
    glm_mat4_scale(ex, 5.f);
    glm_mat4_transpose(ex);
    glm_mat4_transpose(m1);
    Mat4 mat1, expected;
    memcpy(mat1.mat16, &m1[0], sizeof(float) * 16);
    memcpy(expected.mat16, &ex[0], sizeof(float) * 16);
    Mat4 result = mat1 * 5.f;
    EXPECT_TRUE(result == expected);
}

TEST(Mat4, MulVTest) {
    vec4 s = { 111.f, 222.f, 333.f, 444.f };
    vec4 l0 = { 1.f,2.f,3.f,4.f };
    vec4 l1 = { 5.f,6.f,7.f,8.f };
    vec4 l2 = { 9.f,10.f,11.f,12.f };
    vec4 l3 = { 13.f,14.f,15.f,16.f };
    mat4 m1; vec4 ex;
    glm_vec4_copy(l0, m1[0]);
    glm_vec4_copy(l1, m1[1]);
    glm_vec4_copy(l2, m1[2]);
    glm_vec4_copy(l3, m1[3]);
    glm_mat4_mulv(m1, s, ex);
    glm_mat4_transpose(m1);
    Float4 scale({ 111.f, 222.f, 333.f, 444.f });
    Mat4 mat1;
    memcpy(mat1.mat16, &m1[0], sizeof(float) * 16);
    Float4 expected;
    expected.x = ex[0];
    expected.y = ex[1];
    expected.z = ex[2];
    expected.w = ex[3];
    Float4 result = scale * mat1;
    EXPECT_TRUE(result  == expected);
}

#pragma endregion

#pragma region QuaternionTest

TEST(Quaternion, NormalizeTest) {
    
    versors q = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    q = glms_quat_normalize(q);
    Quaternion result = { 4.f, 1.f, 2.f, 3.f };
    result.Normalize();
    Quaternion expected = { q.w,q.x,q.y,q.z };
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, ModulusTest) {

    versors q = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    float expected = sqrtf(glms_quat_dot(q, q));
    Quaternion quat = { 4.f,1.f,2.f,3.f };
    float result = quat.Modulus();
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, SqrModulusTest) {
    versors q = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    float expected = glms_quat_dot(q, q);
    Quaternion quat = { 4.f,1.f,2.f,3.f };
    float result = quat.SquaredModulus();
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, RotationMatrixTest) {
    versors q = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    mat4s m = glms_quat_mat4(q);
    Mat4 expected;
    float* expectedPtr = expected.mat16;
    expectedPtr = (float*)m.raw;
    Quaternion quat = { 4.f,1.f,2.f,3.f };
    Mat4 result = quat.GetRotationMatrix();
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, HamiltonTest) {
    versors p = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    versors q = glms_quat_init(5.f, 6.f, 7.f, 8.f);
    versors e = glms_quat_mul(p, q);
    Quaternion expected(e.w, e.x, e.y, e.z);
    Quaternion r = { 4.f, 1.f, 2.f, 3.f };
    Quaternion l = { 8.f, 5.f, 6.f, 7.f };
    Quaternion result = Quaternion::Hamilton(r, l);
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, FromEulerTest) {
    vec3 angles = { PI / 2.f,PI / 4.f,PI / 8.f };
    mat4s mat = glms_mat4_identity();
    glm_euler_xyz(angles, mat.raw);
    versors e = glms_mat4_quat(mat);
    Quaternion expected(e.x, e.w, e.z, -e.y);
    Quaternion result = Quaternion::FromEuler({ PI / 2.f, PI / 4.f, PI / 8.f });
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, SLerpTest) {
    versors p = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    versors q = glms_quat_init(5.f, 6.f, 7.f, 8.f);
    versors e = glms_quat_slerp(p, q, 0.5f);
    Quaternion first(p.w, p.x, p.y, p.z);
    Quaternion second(q.w, q.x, q.y, q.z);
    Quaternion expected(e.w, e.x, e.y, e.z);
    Quaternion result = Quaternion::SLerp(first, second, 0.5f);
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, NLerpTest) {
    versors p = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    versors q = glms_quat_init(5.f, 6.f, 7.f, 8.f);
    versors e = glms_quat_nlerp(p, q, 0.5f);
    Quaternion first(p.w, p.x, p.y, p.z);
    Quaternion second(q.w, q.x, q.y, q.z);
    Quaternion expected(e.w, e.x, e.y, e.z);
    Quaternion result = Quaternion::NLerp(first, second, 0.5f);
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, DotProductTest) {

    versors p = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    versors q = glms_quat_init(5.f, 6.f, 7.f, 8.f);
    float expected = glms_quat_dot(p, q);
    Quaternion first(p.w, p.x, p.y, p.z);
    Quaternion second(q.w, q.x, q.y, q.z);
    float result = Quaternion::DotProduct(first, second);
    EXPECT_TRUE(result == expected);
}

TEST(Quaternion, AddTest) {
    versors p = glms_quat_init(1.f, 2.f, 3.f, 4.f);
    versors q = glms_quat_init(5.f, 6.f, 7.f, 8.f);
    versors e = glms_quat_add(p, q);
    Quaternion first(p.w, p.x, p.y, p.z);
    Quaternion second(q.w, q.x, q.y, q.z);
    Quaternion expected(e.w, e.x, e.y, e.z);
    Quaternion result = first + second;
    EXPECT_TRUE(result == expected);
}

#pragma endregion