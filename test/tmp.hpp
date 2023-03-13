#pragma once

#pragma region TEST_IMPLEMENTATION

static int testPassed = 0;
static int testFailed = 0;

#define TEST_START \
LARGE_INTEGER start,end,frequency; \
QueryPerformanceCounter(&start); \
QueryPerformanceFrequency(&frequency); \

#define TEST_STOP \
QueryPerformanceCounter(&end); \
long long cycles = end.QuadPart-start.QuadPart; \
double elapsedTime = (double)(cycles)/frequency.QuadPart; \
elapsedTime *= 1000.f; \

#define TEST_OUT(pExpectedFormat, ...) \
char buffer[1024]; \
memset(buffer, 0, sizeof buffer); \
std::string output(__FUNCTION__); \
if (result == pExpected) { \
output += " passed in %lf ms (%lli cycle%s)\n"; \
snprintf(buffer, 1024, output.c_str(), elapsedTime, cycles, (cycles > 1 ? "s" : "")); \
OutputDebugStringA(buffer); \
memset(buffer, 0, sizeof buffer); \
testPassed++; \
return true; \
} \
else { \
output += std::string(" (") + __FILE__ + " at line " + std::to_string(__LINE__) + \
") failed (expected " + pExpectedFormat + ")\n"; \
snprintf(buffer, 1024, output.c_str(), __VA_ARGS__); \
OutputDebugStringA(buffer); \
memset(buffer, 0, sizeof buffer); \
testFailed++; \
return false; \
} \

#define TEST_OUT_VEC2 \
TEST_OUT("\n{%f %f}\nand got\n{%f %f}", \
pExpected.x, pExpected.y, \
result.x, result.y); \

#define TEST_OUT_VEC3 \
TEST_OUT("\n{%f %f %f}\nand got\n{%f %f %f}", \
pExpected.x, pExpected.y, pExpected.z, \
result.x, result.y, result.z); \

#define TEST_OUT_VEC4 \
TEST_OUT("\n{%f %f %f %f}\nand got\n{%f %f %f %f}", \
pExpected.x, pExpected.y, pExpected.z, pExpected.w, \
result.x, result.y, result.z, result.w); \

#define TEST_OUT_QUAT \
TEST_OUT("\n{%f %f %f %f}\nand got\n{%f %f %f %f}", \
pExpected.a, pExpected.b, pExpected.c, pExpected.d, \
result.a, result.b, result.c, result.d); \

#define TEST_OUT_MAT4 \
float* r = result.mat16; \
float* e = pExpected.mat16; \
TEST_OUT("\n%f %f %f %f\n %f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\nand got\n\n%f %f %f %f\n %f %f %f %f\n%f %f %f %f\n%f %f %f %f", \
e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7], e[8], e[9], e[10], e[11], e[12], e[13], e[14], e[15], \
r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], r[9], r[10], r[11], r[12], r[13], r[14], r[15]); \

#define TEST_ERR(pCondition, pFormat, ...) \
char errBuffer[512]; \
memset(errBuffer, 0, sizeof errBuffer); \
std::string errOutput(__FUNCTION__); \
if (pCondition) { \
errOutput += std::string("ERROR from ") + __FILE__ + " at line " + \
std::to_string(__LINE__) + ": " + pFormat + "\n"; \
snprintf(errBuffer, 512, errOutput.c_str(), __VA_ARGS__); \
OutputDebugStringA(errBuffer); \
return false; \
} \
else return true;\
memset(errBuffer, 0, sizeof errBuffer); \

#define TEST_SEQ_START(pSeqName) \
testPassed = testFailed = 0; \
{ \
char buffer[512]; \
memset(buffer, 0, sizeof buffer); \
std::string output("\n#### "); \
output += pSeqName; \
output += " TEST SEQUENCE\n\n"; \
OutputDebugStringA(output.c_str()); \
} \

#define TEST_SEQ_END() \
{ \
char buffer[1024]; \
memset(buffer, 0, sizeof buffer); \
snprintf(buffer, 1024, "\n#### PASSED: %d; FAILED: %d; TOTAL: %d;\n\n", \
testPassed, testFailed, testPassed + testFailed); \
OutputDebugStringA(buffer); \
memset(buffer, 0, sizeof buffer); \
} \

#pragma endregion

#pragma region Float2Test

bool Float2CrossProductTest
(Float2 pA, Float2 pB, float pExpected) {
    TEST_START;
    float result = pA.CrossProduct(pB);
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float2DotProductTest
(Float2 pA, Float2 pB, float pExpected) {
    TEST_START;
    float result = pA.DotProduct(pB);
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float2PlanRotationTest
(Float2 pA, float pAngle, Float2 pExpected) {
    TEST_START;
    Float2 result = pA;
    result.PlanRotation(pAngle);
    TEST_STOP;
    TEST_OUT_VEC2;
}

bool Float2MagnitudeTest(Float2 pA, float pExpected) {
    TEST_START;
    float result = pA.Magnitude();
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float2NormalizeTest
(Float2 pA, Float2 pExpected) {
    TEST_START;
    Float2 result = pA.GetNormalized();
    TEST_STOP;
    TEST_OUT_VEC2;
}

bool Float2LerpTest
(Float2 pFrom, Float2 pTo, float pTime, Float2 pExpected) {
    TEST_START;
    Float2 result = Float2::Lerp(pFrom, pTo, pTime);
    TEST_STOP;
    TEST_OUT_VEC2;
}

bool Float2AddTest
(Float2 pA, Float2 pB, Float2 pExpected) {
    TEST_START;
    Float2 result = pA + pB;
    TEST_STOP;
    TEST_OUT_VEC2;
}

bool Float2SubTest
(Float2 pA, Float2 pB, Float2 pExpected) {
    TEST_START;
    Float2 result = pA - pB;
    TEST_STOP;
    TEST_OUT_VEC2;
}

bool Float2MulSTest
(Float2 pA, float pB, Float2 pExpected) {
    TEST_START;
    Float2 result = pA * pB;
    TEST_STOP;
    TEST_OUT_VEC2;
}

bool Float2DivSTest
(Float2 pA, float pB, Float2 pExpected) {
    TEST_ERR((pB == 0.f), "TEST FAILED (ZERO DIVIDER) !");
    TEST_START;
    Float2 result = pA / pB;
    TEST_STOP;
    TEST_OUT_VEC2;
}

#pragma endregion

#pragma region Float3Test

bool Float3CrossProductTest
(Float3 pA, Float3 pB, Float3 pExpected) {
    TEST_START;
    Float3 result = pA.CrossProduct(pB);
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool Float3DotProductTest
(Float3 pA, Float3 pB, float pExpected) {
    TEST_START;
    float result = pA.DotProduct(pB);
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float3MagnitudeTest
(Float3 pA, float pExpected) {
    TEST_START;
    float result = pA.Magnitude();
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float3NormalizeTest
(Float3 pA, Float3 pExpected) {
    TEST_START;
    Float3 result = pA.GetNormalized();
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool Float3LerpTest
(Float3 pFrom, Float3 pTo, float pTime, Float3 pExpected) {
    TEST_START;
    Float3 result = Float3::Lerp(pFrom, pTo, pTime);
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool Float3AddTest
(Float3 pA, Float3 pB, Float3 pExpected) {
    TEST_START;
    Float3 result = pA + pB;
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool Float3SubTest
(Float3 pA, Float3 pB, Float3 pExpected) {
    TEST_START;
    Float3 result = pA - pB;
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool Float3MulSTest
(Float3 pA, float pB, Float3 pExpected) {
    TEST_START;
    Float3 result = pA * pB;
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool Float3DivSTest
(Float3 pA, float pB, Float3 pExpected) {
    TEST_ERR((pB == 0.f), "TEST FAILED (ZERO DIVIDER) !");
    TEST_START;
    Float3 result = pA / pB;
    TEST_STOP;
    TEST_OUT_VEC3;
}

#pragma endregion

#pragma region Float4Test

bool Float4DotProductTest
(Float4 pA, Float4 pB, float pExpected) {
    TEST_START;
    float result = pA.DotProduct(pB);
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float4MagnitudeTest
(Float4 pA, float pExpected) {
    TEST_START;
    float result = pA.Magnitude();
    TEST_STOP;
    TEST_OUT("%f and got %f", pExpected, result);
}

bool Float4NormalizeTest
(Float4 pA, Float4 pExpected) {
    TEST_START;
    Float4 result = pA.GetNormalized();
    TEST_STOP;
    TEST_OUT_VEC4;
}

bool Float4AddTest
(Float4 pA, Float4 pB, Float4 pExpected) {
    TEST_START;
    Float4 result = pA + pB;
    TEST_STOP;
    TEST_OUT_VEC4;
}

bool Float4SubTest
(Float4 pA, Float4 pB, Float4 pExpected) {
    TEST_START;
    Float4 result = pA - pB;
    TEST_STOP;
    TEST_OUT_VEC4;
}

bool Float4MulSTest
(Float4 pA, float pB, Float4 pExpected) {
    TEST_START;
    Float4 result = pA * pB;
    TEST_STOP;
    TEST_OUT_VEC4;
}

bool Float4DivSTest
(Float4 pA, float pB, Float4 pExpected) {
    TEST_ERR((pB == 0.f), "TEST FAILED (ZERO DIVIDER) !");
    TEST_START;
    Float4 result = pA / pB;
    TEST_STOP;
    TEST_OUT_VEC4;
}

#pragma endregion

#pragma region Mat4Test

bool Mat4GetRotationXTest
(float pAngle, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetRotationX(pAngle);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4GetRotationYTest
(float pAngle, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetRotationX(pAngle);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4GetRotationZTest
(float pAngle, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetRotationZ(pAngle);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4GetRotationTest
(float pYaw, float pPitch, float pRoll, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetRotation(pYaw, pPitch, pRoll);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4GetTranslationTest
(Float3 pTranslation, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetTranslation(pTranslation);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4GetScaleTest
(Float3 pScale, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetScale(pScale);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4TransformTest
(Float3 pPosition, Float3 pRotation, Float3 pScale, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::CreateTransformMatrix(pPosition, pRotation, pScale);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4TransformQuatTest
(Float3 pPosition, Quaternion pRotation, Float3 pScale, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::CreateTransformMatrix(pPosition, pRotation, pScale);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4OrthographicTest
(float pRight, float pLeft, float pTop, float pBottom, float pFar, float pNear, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetOrthographicMatrix(pRight, pLeft, pTop, pBottom, pFar, pNear);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4PerspectiveTest
(float pAspect, float pFOV, float pFar, float pNear, Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetPerspectiveMatrix(pAspect, pFOV, pFar, pNear);
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4TransposeTest
(Mat4 pIn, Mat4 pExpected) {
    TEST_START;
    Mat4 result = pIn.GetTransposedMatrix();
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4InverseTest
(Mat4 pIn, Mat4 pExpected) {
    TEST_START;
    Mat4 result = pIn.GetInverseMatrix();
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4IdentityTest
(Mat4 pExpected) {
    TEST_START;
    Mat4 result = Mat4::GetIdentityMatrix();
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4MulTest
(Mat4 pIn, Mat4 pOther, Mat4 pExpected) {
    TEST_START;
    Mat4 result = pIn * pOther;
    result = result.GetTransposedMatrix();
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4MulSTest
(Mat4 pIn, float pOther, Mat4 pExpected) {
    TEST_START;
    Mat4 result = pIn * pOther;
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool Mat4MulVTest
(Float4 pVec, Mat4 pIn, Float4 pExpected) {
    TEST_START;
    Float4 result = pVec * pIn;
    TEST_STOP;
    TEST_OUT_VEC4;
}

#pragma endregion

#pragma region QuatTest

bool QuatNormalizeTest
(Quaternion pIn, Quaternion pExpected) {
    TEST_START;
    pIn.Normalize();
    Quaternion result = pIn;
    TEST_STOP;
    TEST_OUT_QUAT;
}

bool QuatModulusTest
(Quaternion pIn, float pExpected) {
    TEST_START;
    float result = pIn.Modulus();
    TEST_STOP;
    TEST_OUT("%f got %f", result, pExpected);
}

bool QuatSqrModulusTest
(Quaternion pIn, float pExpected) {
    TEST_START;
    float result = pIn.SquaredModulus();
    TEST_STOP;
    TEST_OUT("%f got %f", result, pExpected);
    
}

bool QuatToMatTest
(Quaternion pIn, Mat4 pExpected) {
    TEST_START;
    Mat4 result = pIn.GetRotationMatrix();
    TEST_STOP;
    TEST_OUT_MAT4;
}

bool QuatHamiltonTest
(Quaternion pRight, Quaternion pLeft, Quaternion pExpected) {
    TEST_START;
    Quaternion result = Quaternion::Hamilton(pRight, pLeft);
    TEST_STOP;
    TEST_OUT_QUAT;
}

bool QuatFromEulerTest
(Float3 pEulerAngles, Quaternion pExpected) {
    TEST_START;
    Quaternion result = Quaternion::FromEuler(pEulerAngles);
    Float3 f3 = Quaternion::ToEuler(result);
    TEST_STOP;
    TEST_OUT_QUAT;
}

bool QuatToEulerTest
(Quaternion pQuat, Float3 pExpected) {
    TEST_START;
    Float3 result = pQuat.ToEuler();
    TEST_STOP;
    TEST_OUT_VEC3;
}

bool QuatAngleAxisTest
(Float3 pAxis, float pAngle, Quaternion pExpected) {
    TEST_START;
    Quaternion result = Quaternion::AngleAxis(pAxis, pAngle);
    TEST_STOP;
    TEST_OUT_QUAT;
}

bool QuatSLerpTest
(Quaternion pFirst, Quaternion pSecond, float pTime, Quaternion pExpected) {
    TEST_START;
    Quaternion result = Quaternion::SLerp(pFirst, pSecond, pTime);
    TEST_STOP;
    TEST_OUT_QUAT;
}

bool QuatNLerpTest
(Quaternion pFirst, Quaternion pSecond, float pTime, Quaternion pExpected) {
    TEST_START;
    Quaternion result = Quaternion::NLerp(pFirst, pSecond, pTime);
    TEST_STOP;
    TEST_OUT_QUAT;
}

bool QuatDotProductTest
(Quaternion pFirst, Quaternion pSecond, float pExpected) {
    TEST_START;
    float result = Quaternion::DotProduct(pFirst, pSecond);
    TEST_STOP;
    TEST_OUT("%f got %f", result, pExpected);
}

bool QuatAddTest
(Quaternion pIn, Quaternion pOther, Quaternion pExpected) {
    TEST_START;
    Quaternion result = pIn + pOther;
    TEST_STOP;
    TEST_OUT_QUAT;
}

#pragma endregion