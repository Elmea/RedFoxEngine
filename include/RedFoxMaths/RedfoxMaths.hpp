#ifndef REDFOXMATHS_HPP
#define REDFOXMATHS_HPP

#ifndef PI
#define PI 3.14159265f
#endif

#define DEG2RAD PI/180.0f
#define RAD2DEG 180/PI

#define FLOATCOMPACCURATE 0.0001f

#include <math.h>
#include <cstdarg>

namespace RedFoxMaths
{
    // ----------------- [Vector] -----------------
#pragma region Vector
    
    class Float2
    {
        public:
        float x;
        float y;
        
        float CrossProduct(Float2 pOther) const;
        
        float DotProduct(const Float2& pOther) const;
        
        void PlanRotation(float pAngle);
        float Magnitude();
        void Normalize();
        Float2 GetNormalized();
        static Float2 Lerp(const Float2& a, const Float2& b, const float& pTime);
        
        Float2 operator+(const Float2& pOther);
        Float2 operator-(const Float2& pOther);
        Float2 operator*(const int& pMultiplicator);
        Float2 operator*(const float& pMultiplicator);
        Float2 operator/(const float& pDivider);
        Float2 operator-();
        friend Float2 operator+(const Float2& pLeft, const Float2& pRight);
        friend Float2 operator-(const Float2& pLeft, const Float2& pRight);
        friend Float2 operator*(const Float2& pLeft, const float pMultiplier);
        friend Float2 operator/(const Float2& pLeft, const float pDivider);
        
        friend Float2& operator+=(Float2& pLeft, const Float2& pRight);
        friend Float2& operator-=(Float2& pLeft, const Float2& pRight);
        friend Float2& operator*=(Float2& pLeft, const float pMultiplier);
        friend Float2& operator/=(Float2& pLeft, const float pDivider);
        
        friend bool operator<(const Float2& pLeft, const Float2& pRight);
        friend bool operator<=(const Float2& pLeft, const Float2& pRight);
        friend bool operator>(const Float2& pLeft, const Float2& pRight);
        friend bool operator>=(const Float2& pLeft, const Float2& pRight);
        
        bool operator==(const Float2& pOther);
    };
    
    class Float3
    {
        public:
        float x;
        float y;
        float z;
        
        Float3 CrossProduct(const Float3& pOther) const;
        
        float DotProduct(const Float3& vec3) const;
        
        Float3 operator+(const Float3& pOther);
        Float3 operator-(const Float3& pOther);
        Float3 operator-();
        Float3 operator*(const float& pMultiplicator);
        Float3 operator*(const int& pMultiplicator);
        friend Float3 operator+(const Float3& pLeft, const Float3& pRight);
        friend Float3 operator-(const Float3& pLeft, const Float3& pRight);
        friend Float3 operator*(const Float3& pLeft, const float pMultiplier);
        friend Float3 operator/(const Float3& pLeft, const float pDivider);
        
        friend Float3& operator+=(Float3& pLeft, const Float3& pRight);
        friend Float3& operator-=(Float3& pLeft, const Float3& pRight);
        friend Float3& operator*=(Float3& pLeft, const float pMultiplier);
        friend Float3& operator/=(Float3& pLeft, const float pDivider);
        
        friend bool operator<(const Float3& pLeft, const Float3& pRight);
        friend bool operator<=(const Float3& pLeft, const Float3& pRight);
        friend bool operator>(const Float3& pLeft, const Float3& pRight);
        friend bool operator>=(const Float3& pLeft, const Float3& pRight);
        
        
        Float3(float x = 0, float y = 0, float z = 0);
        
        float Magnitude();
        void Normalize();
        
        Float3 GetNormalized();
        
        bool operator==(const Float3& pOther)
        {
            return (x <= pOther.x + FLOATCOMPACCURATE && x >= pOther.x - FLOATCOMPACCURATE) && (y <= pOther.y +
                                                                                                FLOATCOMPACCURATE && y >= pOther.y - FLOATCOMPACCURATE) && (z <= pOther.z + FLOATCOMPACCURATE && z >=
                                                                                                                                                            pOther.z - FLOATCOMPACCURATE);
        }
        
        
        static Float3 GetSphericalCoords(float r, float pTheta, float pPhi);
        static Float3 Lerp(const Float3& a, const Float3& b, const float& pTime);
    };
    
    inline bool operator==(const RedFoxMaths::Float3& f1, const RedFoxMaths::Float3& f2)
    {
        return ((f1.x == f2.x) && (f1.y == f2.y) && (f1.z == f2.z));
    }
    
    class Float4
    {
        public:
        float x;
        float y;
        float z;
        float w;
        
        float DotProduct(const Float4& pOther) const;
        
        Float4 operator+(const Float4& pOther);
        Float4 operator-(const Float4& pOther);
        Float4 operator-();
        Float4 operator*(const float& pMultiplicator);
        Float4 operator*(const int& pMultiplicator);
        friend Float4 operator+(const Float4& pLeft, const Float4& pRight);
        friend Float4 operator-(const Float4& pLeft, const Float4& pRight);
        friend Float4 operator*(const Float4& pLeft, const float pMultiplier);
        friend Float4 operator/(const Float4& pLeft, const float pDivider);
        
        friend Float4& operator+=(Float4& pLeft, const Float4& pRight);
        friend Float4& operator-=(Float4& pLeft, const Float4& pRight);
        friend Float4& operator*=(Float4& pLeft, const float pMultiplier);
        friend Float4& operator/=(Float4& pLeft, const float pDivider);
        
        friend bool operator<(const Float4& pLeft, const Float4& pRight);
        friend bool operator<=(const Float4& pLeft, const Float4& pRight);
        friend bool operator>(const Float4& pLeft, const Float4& pRight);
        friend bool operator>=(const Float4& pLeft, const Float4& pRight);
        
        Float4(const Float3& f3, float w = 1.0f);
        Float4(float x = 0, float y = 0, float z = 0, float w = 1.0f);
        
        float Magnitude();
        
        void Normalize();
        Float4 GetNormalized();
        
        void Homogenize();
        Float4 GetHomogenized();
        Float3 GetXYZF3();
        
        inline void operator=(const Float3& f3)
        {
            x = f3.x;
            y = f3.y;
            z = f3.z;
            w = 1;
        }
        
        bool operator==(const Float4& pOther)
        {
            return (x <= pOther.x + FLOATCOMPACCURATE && x >= pOther.x - FLOATCOMPACCURATE) && 
            (y <= pOther.y + FLOATCOMPACCURATE && y >= pOther.y - FLOATCOMPACCURATE) && 
            (z <= pOther.z + FLOATCOMPACCURATE && z >=pOther.z - FLOATCOMPACCURATE) && 
            (w <= pOther.w + FLOATCOMPACCURATE && w >=pOther.w - FLOATCOMPACCURATE);
        }
    };
    
#pragma endregion Vector
    
    // ----------------- [Matrix] -----------------
#pragma region Matrix
    
    class Quaternion;
    
    class Mat4
    {
        private:
        Mat4 GetSubmat(int l, int c);
        
        public:
        Mat4();
        Mat4(float matrix[4][4]);
        //TODO: mat16 constructor
        //TODO: ptr constructor
        
        static Mat4 GetRotationX(const float& pAngle);
        static Mat4 GetRotationY(const float& pAngle);
        static Mat4 GetRotationZ(const float& pAngle);
        static Mat4 GetRotation(const float& pYaw, const float& pPitch, const float& pRoll);
        
        static Mat4 GetTranslation(const Float3& pTranslation);
        static Mat4 GetScale(const Float3& pScale);
        
        union
        {
            float mat[4][4] = {
                {1.f, 0.f, 0.f, 0.f},
                {0.f, 1.f, 0.f, 0.f},
                {0.f, 0.f, 1.f, 0.f},
                {0.f, 0.f, 0.f, 1.f}
            };
            
            float mat16[16];
        };
        
        void operator=(const Mat4& pOther);
        void operator=(const float matrix[4][4]);
        
        static Mat4 CreateTransformMatrix(const Float3& position, const Float3& rotationDEG, const Float3& pScale);
        static Mat4 CreateTransformMatrix(const Float3& position, const Quaternion& rotation, const Float3& pScale);
        
        static Mat4 GetOrthographicMatrix(float pRight, float pLeft, float pTop, float pBottom, float pFar, float pNear);
        static Mat4 GetPerspectiveMatrix(float aspect, float FOV, float pFar, float pNear);
        
        Mat4 operator*(const Mat4& pOther) const;
        Float4 operator*(const Float4& ft4);
        Mat4 operator*(const float& pMult);
        Mat4 operator*(const int& pMult);
        
        const float* AsPtr() const;
        
        Mat4 GetTransposedMatrix();
        
        float GetDeterminent();
        Mat4 GetComplementaryMat();
        Mat4 GetCoMatrix();
        Mat4 GetInverseMatrix();
        static Mat4 GetIdentityMatrix();
        
        Float4 GetMatLine(int index);
        Quaternion ToQuaternion();

        bool operator==(const Mat4& pOther);
    };
    
    Mat4 operator*(const float& pMult, const Mat4& matrix);
    Float4 operator*(const Float4& ft4, const Mat4& matrix);
    
#pragma endregion Matrix
    
    // --------------- [Quaternion] ---------------
#pragma region Quaternion
    
    class Quaternion
    {
        public:
        float a, b, c, d;
        
        Quaternion();
        Quaternion(const float& _a, const float& _b, const float& _c, const float& _d);
        Quaternion(const float& pRoll, const float& pPitch, const float& pYaw);
        Quaternion(const Float3& pEulerAngles);
        
        void Normalize();
        Quaternion GetNormalized() const;
        
        float Modulus() const;
        float SquaredModulus() const;
        
        //Return corresponding rotation matrix
        Mat4 GetRotationMatrix() const;
        
        static Quaternion Hamilton(const Quaternion& pRight, const Quaternion& pLeft);
        
        //Return a Quaternion from corresponding Euler pAngles
        static Quaternion FromEuler(const float& pRoll, const float& pPitch, const float& pYaw);
        //Return a Quaternion from corresponding Euler pAngles
        static Quaternion FromEuler(const Float3& pEulerAngles);
        //Return a Float3 as pRoll pPitch pYaw from corresponding quaternion
        static Float3 ToEuler(const Quaternion& quaternion);
        Float3 ToEuler();
        
        //Return a Quaternion from corresponding axis and radian pAngle
        static Quaternion AngleAxis(Float3& pAxis, const float& pAngle);
        
        static Quaternion SLerp(const Quaternion& pFirst, const Quaternion& pSecond, float t);
        static Quaternion NLerp(const Quaternion& pFirst, const Quaternion& pSecond, float t);
        
        static float DotProduct(const Quaternion& pFirst, const Quaternion& pSecond);
        
        Quaternion operator*(const Quaternion& pOther) const;
        Quaternion operator*(const float& pOther) const;
        Quaternion operator+(const Quaternion& pOther) const;
        
        bool operator==(const Quaternion& pOther)
        {
            return (a <= pOther.a + FLOATCOMPACCURATE && a >= pOther.a - FLOATCOMPACCURATE) && 
            (b <= pOther.b + FLOATCOMPACCURATE && b >= pOther.b - FLOATCOMPACCURATE) && 
            (c <= pOther.c + FLOATCOMPACCURATE && c >=pOther.c - FLOATCOMPACCURATE) && 
            (d <= pOther.d + FLOATCOMPACCURATE && d >=pOther.d - FLOATCOMPACCURATE);
        }
    };
    
#pragma endregion
    
    // ------------------ [Misc] ------------------
#pragma region Misc
    namespace Misc
    {
        class Circle
        {
            public:
            Float2 center;
            float radius;
        };
        
        typedef struct Triangle
        {
            Float2 vertex[3];
        } Triangle;
        
        template <typename T>
            T Min(const T& a, const T& b)
        {
            return a < b ? a : b;
        }
        
        template <typename T>
            T Max(const T& a, const T& b)
        {
            return a > b ? a : b;
        }
        
        template <typename T>
            T Abs(const T& a)
        {
            return a < 0 ? -a : a;
        }
        
        float Lerp(const float& t, const float& a, const float& b);
        
        float Pythagoreantheorem(int nb_values, ...);
        
        float getPointYByLineEquation(Float2 line, Float2 point);
        
        Float2 Barrycentre(int nb_values, ...);
        
        Float3 BarrycentreF3(Float3 point1, Float3 point2, Float3 point3);
        
        Float3 CalcNormal(const Float3& p1, const Float3& p2, const Float3& p3);
    }
#pragma endregion
}
#endif
    // ----------------------------------------------------------- [Implementation] -----------------------------------------------------------
#ifdef REDFOXMATHS_IMPLEMENTATION

namespace RedFoxMaths
{
#pragma region Float2
    
    float Float2::CrossProduct(Float2 pOther) const
    {
        return this->x * pOther.y - this->y * pOther.x;
    }
    
    float Float2::DotProduct(const Float2& pOther) const
    {
        return (this->x * pOther.x) + (this->y * pOther.y);
    }
    
    void Float2::PlanRotation(float pAngle)
    {
        float tmpx = this->x;
        this->x = this->x * cosf(DEG2RAD * pAngle) - this->y * sinf(DEG2RAD * pAngle);
        this->y = tmpx * sinf(DEG2RAD * pAngle) + this->y * cosf(DEG2RAD * pAngle);
    }
    
    float Float2::Magnitude()
    {
        return sqrtf(x * x + y * y);
    }
    
    void Float2::Normalize()
    {
        float mag = Magnitude();
        if (mag == 0.f) return;
        x = x / mag;
        y = y / mag;
    }
    
    Float2 Float2::GetNormalized()
    {
        float mag = Magnitude();
        if (mag == 0.f) return Float2();
        return { x / mag, y / mag };
    }
    
    Float2 Float2::Lerp(const Float2& a, const Float2& b, const float& pTime)
    {
        return { pTime * a.x + (1 - pTime) * b.x, pTime * a.y + (1 - pTime) * b.y };
    }
    
#pragma region operators
    bool Float2::operator==(const Float2& pOther)
    {
        return (x <= pOther.x + FLOATCOMPACCURATE && x >= pOther.x - FLOATCOMPACCURATE) &&
        (y <= pOther.y + FLOATCOMPACCURATE && y >= pOther.y - FLOATCOMPACCURATE);
    }
    
    Float2 Float2::operator+(const Float2& pOther)
    {
        Float2 result;
        result.x = x + pOther.x;
        result.y = y + pOther.y;
        return result;
    }
    
    Float2 operator+(const Float2& pLeft, const Float2& pRight)
    {
        return { pLeft.x + pRight.x, pLeft.y + pRight.y };
    }
    
    Float2 Float2::operator-(const Float2& pOther)
    {
        Float2 result;
        result.x = x - pOther.x;
        result.y = y - pOther.y;
        return result;
    }
    
    Float2 Float2::operator-()
    {
        return { -x, -y };
    }
    
    Float2 operator-(const Float2& pLeft, const Float2& pRight)
    {
        return { pLeft.x - pRight.x, pLeft.y - pRight.y };
    }
    
    Float2 Float2::operator*(const float& pMultiplicator)
    {
        return { x * pMultiplicator, y * pMultiplicator };
    }
    
    Float2 operator*(const Float2& pLeft, const float pMultiplier)
    {
        return { pLeft.x * pMultiplier, pLeft.y * pMultiplier };
    }
    
    Float2 Float2::operator/(const float& pDivider)
    {
        return { x / pDivider, y / pDivider };
    }
    
    Float2 operator/(const Float2& pLeft, const float pDivider)
    {
        return { pLeft.x / pDivider, pLeft.y / pDivider };
    }
    
    Float2& operator+=(Float2& pLeft, const Float2& pRight)
    {
        pLeft = pLeft + pRight;
        return pLeft;
    }
    
    Float2& operator-=(Float2& pLeft, const Float2& pRight)
    {
        pLeft = pLeft - pRight;
        return pLeft;
    }
    
    Float2& operator*=(Float2& pLeft, const float pMultiplier)
    {
        pLeft = pLeft * pMultiplier;
        return pLeft;
    }
    
    Float2& operator/=(Float2& pLeft, const float pDivider)
    {
        pLeft = pLeft / pDivider;
        return pLeft;
    }
    
    Float2 Float2::operator*(const int& pMultiplicator)
    {
        return { x * pMultiplicator, y * pMultiplicator };
    }
    
    bool operator<(const Float2& pLeft, const Float2& pRight)
    {
        if (pLeft.x < pRight.x && pLeft.y < pRight.y)
            return true;
        else
            return false;
    }
    
    bool operator<=(const Float2& pLeft, const Float2& pRight)
    {
        if (pLeft.x <= pRight.x && pLeft.y <= pRight.y)
            return true;
        else
            return false;
    }
    
    bool operator>(const Float2& pLeft, const Float2& pRight)
    {
        if (pLeft.x > pRight.x && pLeft.y > pRight.y)
            return true;
        else
            return false;
    }
    
    bool operator>=(const Float2& pLeft, const Float2& pRight)
    {
        if (pLeft.x >= pRight.x && pLeft.y >= pRight.y)
            return true;
        else
            return false;
    }
#pragma endregion 
    
#pragma endregion
    
    // ----------------------------[Float 3]----------------------------
#pragma region Float3
    
    Float3::Float3(const float inx, const float iny, const float inz)
    {
        x = inx;
        y = iny;
        z = inz;
    }
    
    Float3 Float3::CrossProduct(const Float3& pOther) const
    {
        Float3 result;
        
        result.x = y * pOther.z - z * pOther.y;
        result.y = z * pOther.x - x * pOther.z;
        result.z = x * pOther.y - y * pOther.x;
        
        return result;
    }
    
    float Float3::DotProduct(const Float3& vec3) const
    {
        return x * vec3.x + y * vec3.y + z * vec3.z;
    }
    
    float Float3::Magnitude()
    {
        return sqrtf(x * x + y * y + z * z);
    }
    
    void Float3::Normalize()
    {
        float mag = Magnitude();
        if (mag == 0.f) return;
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }
    
    Float3 Float3::GetNormalized()
    {
        float mag = Magnitude();
        if (mag == 0.f) return Float3();
        return { x / mag, y / mag, z / mag };
    }
    
    Float3 Float3::GetSphericalCoords(float r, float pTheta, float pPhi)
    {
        return {
            r * sinf(pTheta) * cosf(pPhi),
            r * cosf(pTheta),
            r * sinf(pTheta) * sinf(pPhi)
        };
    }
    
    Float3 Float3::Lerp(const Float3& a, const Float3& b, const float& pTime)
    {
        return Float3(pTime * a.x + (1 - pTime) * b.x, pTime * a.y + (1 - pTime) * b.y, pTime * a.z + (1 - pTime) * b.z);
    }
    
#pragma region operators
    Float3 Float3::operator+(const Float3& pOther)
    {
        Float3 result;
        result.x = x + pOther.x;
        result.y = y + pOther.y;
        result.z = z + pOther.z;
        return result;
    }
    
    Float3 operator+(const Float3& pLeft, const Float3& pRight)
    {
        return { pLeft.x + pRight.x, pLeft.y + pRight.y, pLeft.z + pRight.z };
    }
    
    Float3 Float3::operator-(const Float3& pOther)
    {
        Float3 result;
        result.x = x - pOther.x;
        result.y = y - pOther.y;
        result.z = z - pOther.z;
        return result;
    }
    
    Float3 Float3::operator-()
    {
        return { -x, -y, -z };
    }
    
    Float3 operator-(const Float3& pLeft, const Float3& pRight)
    {
        return { pLeft.x - pRight.x, pLeft.y - pRight.y, pLeft.z - pRight.z };
    }
    
    Float3 Float3::operator*(const float& pMultiplicator)
    {
        return { x * pMultiplicator, y * pMultiplicator, z * pMultiplicator };
    }
    
    Float3 operator*(const Float3& pLeft, const float pMultiplier)
    {
        return { pLeft.x * pMultiplier, pLeft.y * pMultiplier, pLeft.z * pMultiplier };
    }
    
    Float3 operator/(const Float3& pLeft, const float pDivider)
    {
        return { pLeft.x / pDivider, pLeft.y / pDivider, pLeft.z / pDivider };
    }
    
    Float3& operator+=(Float3& pLeft, const Float3& pRight)
    {
        pLeft = pLeft + pRight;
        return pLeft;
    }
    
    Float3& operator-=(Float3& pLeft, const Float3& pRight)
    {
        pLeft = pLeft - pRight;
        return pLeft;
    }
    
    Float3& operator*=(Float3& pLeft, const float pMultiplier)
    {
        pLeft = pLeft * pMultiplier;
        return pLeft;
    }
    
    Float3& operator/=(Float3& pLeft, const float pDivider)
    {
        pLeft = pLeft / pDivider;
        return pLeft;
    }
    
    Float3 Float3::operator*(const int& pMultiplicator)
    {
        return { x * pMultiplicator, y * pMultiplicator, z * pMultiplicator };
    }
    
    bool operator<(const Float3& pLeft, const Float3& pRight)
    {
        if (pLeft.x < pRight.x && pLeft.y < pRight.y && pLeft.z < pRight.z)
            return true;
        else
            return false;
    }
    
    bool operator<=(const Float3& pLeft, const Float3& pRight)
    {
        if (pLeft.x <= pRight.x && pLeft.y <= pRight.y && pLeft.z <= pRight.z)
            return true;
        else
            return false;
    }
    
    bool operator>(const Float3& pLeft, const Float3& pRight)
    {
        if (pLeft.x > pRight.x && pLeft.y > pRight.y && pLeft.z > pRight.z)
            return true;
        else
            return false;
    }
    
    bool operator>=(const Float3& pLeft, const Float3& pRight)
    {
        if (pLeft.x >= pRight.x && pLeft.y >= pRight.y && pLeft.z >= pRight.z)
            return true;
        else
            return false;
    }
#pragma endregion 
    
#pragma endregion
    
    // ----------------------------[Float 4]----------------------------
#pragma region Float4
    Float4::Float4(float inx, float iny, float inz, float inw)
    {
        x = inx;
        y = iny;
        z = inz;
        w = inw;
    }
    
    Float4::Float4(const Float3& f3, float inw)
    {
        x = f3.x;
        y = f3.y;
        z = f3.z;
        w = inw;
    }
    
    float Float4::DotProduct(const Float4& pOther) const
    {
        return x * pOther.x + y * pOther.y + z * pOther.z + w * pOther.w;
    }
    
    float Float4::Magnitude()
    {
         return sqrtf(x * x + y * y + z * z + w * w);
    }
    
    void Float4::Normalize()
    {
        float mag = Magnitude();
        if (mag == 0.f) return;
        x = x / mag;
        y = y / mag;
        z = z / mag;
        w = w / mag;
    }
    
    Float4 Float4::GetNormalized()
    {
        float mag = Magnitude();
        if (mag == 0.f) return Float4();
        return { x / mag, y / mag, z / mag, w / mag };
    }
    
    void Float4::Homogenize()
    {
        if (w != 0)
        {
            x = x / w;
            y = y / w;
            z = z / w;
            w = w / w;
        }
    }
    
    Float4 Float4::GetHomogenized()
    {
        if (w != 0)
            return { x / w, y / w, z / w, w / w };
        return { x, y, z, w };
    }
    
    Float3 Float4::GetXYZF3()
    {
        return { x, y, z };
    }
    
    
#pragma region operators
    Float4 Float4::operator+(const Float4& pOther)
    {
        Float4 result;
        result.x = x + pOther.x;
        result.y = y + pOther.y;
        result.z = z + pOther.z;
        result.w = w + pOther.w;
        return result;
    }
    
    Float4 operator+(const Float4& pLeft, const Float4& pRight)
    {
        return { pLeft.x + pRight.x, pLeft.y + pRight.y, pLeft.z + pRight.z, pLeft.w + pRight.w };
    }
    
    Float4 Float4::operator-(const Float4& pOther)
    {
        Float4 result;
        result.x = x - pOther.x;
        result.y = y - pOther.y;
        result.z = z - pOther.z;
        result.w = w - pOther.w;
        return result;
    }
    
    Float4 Float4::operator-()
    {
        return { -x, -y, -z, -w };
    }
    
    Float4 operator-(const Float4& pLeft, const Float4& pRight)
    {
        return { pLeft.x - pRight.x, pLeft.y - pRight.y, pLeft.z - pRight.z, pLeft.w - pRight.w };
    }
    
    Float4 Float4::operator*(const float& pMultiplicator)
    {
        return { x * pMultiplicator, y * pMultiplicator, z * pMultiplicator, w * pMultiplicator };
    }
    
    Float4 operator*(const Float4& pLeft, const float pMultiplier)
    {
        return { pLeft.x * pMultiplier, pLeft.y * pMultiplier, pLeft.z * pMultiplier, pLeft.w * pMultiplier };
    }
    
    Float4 operator/(const Float4& pLeft, const float pDivider)
    {
        return { pLeft.x / pDivider, pLeft.y / pDivider, pLeft.z / pDivider, pLeft.w / pDivider };
    }
    
    Float4& operator+=(Float4& pLeft, const Float4& pRight)
    {
        pLeft = pLeft + pRight;
        return pLeft;
    }
    
    Float4& operator-=(Float4& pLeft, const Float4& pRight)
    {
        pLeft = pLeft - pRight;
        return pLeft;
    }
    
    Float4& operator*=(Float4& pLeft, const float pMultiplier)
    {
        pLeft = pLeft * pMultiplier;
        return pLeft;
    }
    
    Float4& operator/=(Float4& pLeft, const float pDivider)
    {
        pLeft = pLeft / pDivider;
        return pLeft;
    }
    
    Float4 Float4::operator*(const int& pMultiplicator)
    {
        return { x * pMultiplicator, y * pMultiplicator, z * pMultiplicator, w * pMultiplicator };
    }
    
    bool operator<(const Float4& pLeft, const Float4& pRight)
    {
        if (pLeft.x < pRight.x && pLeft.y < pRight.y && pLeft.z < pRight.z && pLeft.w < pRight.w)
            return true;
        else
            return false;
    }
    
    bool operator<=(const Float4& pLeft, const Float4& pRight)
    {
        if (pLeft.x <= pRight.x && pLeft.y <= pRight.y && pLeft.z <= pRight.z && pLeft.w <= pRight.w)
            return true;
        else
            return false;
    }
    
    bool operator>(const Float4& pLeft, const Float4& pRight)
    {
        if (pLeft.x > pRight.x && pLeft.y > pRight.y && pLeft.z > pRight.z && pLeft.w > pRight.w)
            return true;
        else
            return false;
    }
    
    bool operator>=(const Float4& pLeft, const Float4& pRight)
    {
        if (pLeft.x >= pRight.x && pLeft.y >= pRight.y && pLeft.z >= pRight.z && pLeft.w >= pRight.w)
            return true;
        else
            return false;
    }
#pragma endregion 
    
#pragma endregion
    
    // ----------------------------[Matrix]----------------------------
#pragma region Matrix
    
    Mat4::Mat4() { }
    
    Mat4::Mat4(float matrix[4][4])
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat[i][j] = matrix[i][j];
            }
        }
    }
    
#pragma region Operators
    
    void Mat4::operator=(const Mat4& pOther)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat[i][j] = pOther.mat[i][j];
            }
        }
    }
    
    void Mat4::operator=(const float matrix[4][4])
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat[i][j] = matrix[i][j];
            }
        }
    }
    
    Float4 Mat4::operator*(const Float4& ft4)
    {
        Float4 result;
        
        result.x = mat[0][0] * ft4.x + mat[0][1] * ft4.y + mat[0][2] * ft4.z + mat[0][3] * ft4.w;
        result.y = mat[1][0] * ft4.x + mat[1][1] * ft4.y + mat[1][2] * ft4.z + mat[1][3] * ft4.w;
        result.z = mat[2][0] * ft4.x + mat[2][1] * ft4.y + mat[2][2] * ft4.z + mat[2][3] * ft4.w;
        result.w = mat[3][0] * ft4.x + mat[3][1] * ft4.y + mat[3][2] * ft4.z + mat[3][3] * ft4.w;
        
        return result;
    }
    
    Mat4 Mat4::operator*(const Mat4& pOther) const
    {
        Mat4 res;
        
        res.mat16[0] = mat16[0] * pOther.mat16[0] + mat16[1] * pOther.mat16[4] + mat16[2] * pOther.mat16[8]  + mat16[3] * pOther.mat16[12];
        res.mat16[1] = mat16[0] * pOther.mat16[1] + mat16[1] * pOther.mat16[5] + mat16[2] * pOther.mat16[9]  + mat16[3] * pOther.mat16[13];
        res.mat16[2] = mat16[0] * pOther.mat16[2] + mat16[1] * pOther.mat16[6] + mat16[2] * pOther.mat16[10] + mat16[3] * pOther.mat16[14];
        res.mat16[3] = mat16[0] * pOther.mat16[3] + mat16[1] * pOther.mat16[7] + mat16[2] * pOther.mat16[11] + mat16[3] * pOther.mat16[15];
        
        res.mat16[4] = mat16[4] * pOther.mat16[0] + mat16[5] * pOther.mat16[4] + mat16[6] * pOther.mat16[8]  + mat16[7] * pOther.mat16[12];
        res.mat16[5] = mat16[4] * pOther.mat16[1] + mat16[5] * pOther.mat16[5] + mat16[6] * pOther.mat16[9]  + mat16[7] * pOther.mat16[13];
        res.mat16[6] = mat16[4] * pOther.mat16[2] + mat16[5] * pOther.mat16[6] + mat16[6] * pOther.mat16[10] + mat16[7] * pOther.mat16[14];
        res.mat16[7] = mat16[4] * pOther.mat16[3] + mat16[5] * pOther.mat16[7] + mat16[6] * pOther.mat16[11] + mat16[7] * pOther.mat16[15];

        res.mat16[8]  = mat16[8] * pOther.mat16[0] + mat16[9] * pOther.mat16[4] + mat16[10] * pOther.mat16[8]  + mat16[11] * pOther.mat16[12];
        res.mat16[9]  = mat16[8] * pOther.mat16[1] + mat16[9] * pOther.mat16[5] + mat16[10] * pOther.mat16[9]  + mat16[11] * pOther.mat16[13];
        res.mat16[10] = mat16[8] * pOther.mat16[2] + mat16[9] * pOther.mat16[6] + mat16[10] * pOther.mat16[10] + mat16[11] * pOther.mat16[14];
        res.mat16[11] = mat16[8] * pOther.mat16[3] + mat16[9] * pOther.mat16[7] + mat16[10] * pOther.mat16[11] + mat16[11] * pOther.mat16[15];

        res.mat16[12] = mat16[12] * pOther.mat16[0] + mat16[13] * pOther.mat16[4] + mat16[14] * pOther.mat16[8]  + mat16[15] * pOther.mat16[12];
        res.mat16[13] = mat16[12] * pOther.mat16[1] + mat16[13] * pOther.mat16[5] + mat16[14] * pOther.mat16[9]  + mat16[15] * pOther.mat16[13];
        res.mat16[14] = mat16[12] * pOther.mat16[2] + mat16[13] * pOther.mat16[6] + mat16[14] * pOther.mat16[10] + mat16[15] * pOther.mat16[14];
        res.mat16[15] = mat16[12] * pOther.mat16[3] + mat16[13] * pOther.mat16[7] + mat16[14] * pOther.mat16[11] + mat16[15] * pOther.mat16[15];

        /*
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                res.mat[i][j] = mat[i][0] * pOther.mat[0][j] + mat[i][1] * pOther.mat[1][j] + mat[i][2] * pOther.mat[2][j]
                    + mat[i][3] * pOther.mat[3][j];
            }
        }*/
        return res;
    }
    
    Mat4 Mat4::operator*(const float& pMult)
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = mat[i][j] * pMult;
            }
        }
        return result;
    }
    
    Mat4 operator*(const float& pMult, const Mat4& matrix)
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = matrix.mat[i][j] * pMult;
            }
        }
        return result;
    }
    
    Mat4 Mat4::operator*(const int& pMult)
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = mat[i][j] * pMult;
            }
        }
        return result;
    }
    
    Float4 operator*(const Float4& ft4, const Mat4& matrix)
    {
        Float4 result;
        
        result.x = matrix.mat[0][0] * ft4.x + matrix.mat[0][1] * ft4.y + matrix.mat[0][2] * ft4.z + matrix.mat[0][3] *
            ft4.w;
        result.y = matrix.mat[1][0] * ft4.x + matrix.mat[1][1] * ft4.y + matrix.mat[1][2] * ft4.z + matrix.mat[1][3] *
            ft4.w;
        result.z = matrix.mat[2][0] * ft4.x + matrix.mat[2][1] * ft4.y + matrix.mat[2][2] * ft4.z + matrix.mat[2][3] *
            ft4.w;
        result.w = matrix.mat[3][0] * ft4.x + matrix.mat[3][1] * ft4.y + matrix.mat[3][2] * ft4.z + matrix.mat[3][3] *
            ft4.w;
        
        return result;
    }
    
#pragma endregion
    
    const float* Mat4::AsPtr() const
    {
        return &mat[0][0];
    }
    
    Mat4 Mat4::GetRotationX(const float& pAngle)
    {
        Mat4 result;
        result.mat[0][0] = 1;
        
        result.mat[1][1] = cosf(pAngle);
        result.mat[1][2] = -sinf(pAngle);
        result.mat[2][1] = sinf(pAngle);
        result.mat[2][2] = cosf(pAngle);
        
        return result;
    }
    
    Mat4 Mat4::GetRotationY(const float& pAngle)
    {
        Mat4 result;
        result.mat[1][1] = 1;
        
        result.mat[0][0] = cosf(pAngle);
        result.mat[2][0] = -sinf(pAngle);
        result.mat[0][2] = sinf(pAngle);
        result.mat[2][2] = cosf(pAngle);
        
        return result;
    }
    
    Mat4 Mat4::GetRotationZ(const float& pAngle)
    {
        Mat4 result;
        result.mat[2][2] = 1;
        
        result.mat[0][0] = cosf(pAngle);
        result.mat[0][1] = -sinf(pAngle);
        result.mat[1][0] = sinf(pAngle);
        result.mat[1][1] = cosf(pAngle);
        
        return result;
    }
    
    Mat4 Mat4::GetRotation(const float& pYaw, const float& pPitch, const float& pRoll)
    {
        return GetRotationY(pPitch) * GetRotationX(pYaw) * GetRotationZ(pRoll);
    }
    
    Mat4 Mat4::GetTranslation(const Float3& pTranslation)
    {
        Mat4 result;
        
        result.mat[0][0] = 1;
        result.mat[1][1] = 1;
        result.mat[2][2] = 1;
        result.mat[3][3] = 1;
        
        result.mat[0][3] = pTranslation.x;
        result.mat[1][3] = pTranslation.y;
        result.mat[2][3] = pTranslation.z;
        
        return result;
    }
    
    Mat4 Mat4::GetScale(const Float3& pScale)
    {
        Mat4 result;
        result.mat[0][0] = pScale.x;
        result.mat[1][1] = pScale.y;
        result.mat[2][2] = pScale.z;
        result.mat[3][3] = 1;
        
        return result;
    }
    
    Mat4 Mat4::CreateTransformMatrix(const Float3& position, const Float3& rotationDEG, const Float3& pScale)
    {
        return GetTranslation(position) * GetRotationY(DEG2RAD * rotationDEG.y) * GetRotationX(DEG2RAD * rotationDEG.x)
            * GetRotationZ(DEG2RAD * rotationDEG.z) * GetScale(pScale);
    }
    
    Mat4 Mat4::CreateTransformMatrix(const Float3& position, const Quaternion& rotation, const Float3& pScale)
    {
        return GetTranslation(position) * rotation.GetRotationMatrix() * GetScale(pScale);
    }
    
    Mat4 Mat4::GetOrthographicMatrix(float pRight, float pLeft, float pTop, float pBottom, float pFar, float pNear)
    {
        float result[4][4] = {
            {2.f / (pRight - pLeft), 0.f, 0.f, -(pRight + pLeft) / (pRight - pLeft)},
            {0.f, 2.f / (pTop - pBottom), 0.f, -(pTop + pBottom) / (pTop - pBottom)},
            {0.f, 0.f, -2.f / (pFar - pNear), -(pFar + pNear) / (pFar - pNear)},
            {0.f, 0.f, 0.f, 1.f}
        };
        
        return result;
    }
    
    Mat4 Mat4::GetPerspectiveMatrix(float aspect, float FOV, float pFar, float pNear)
    {
        float a = 1.f / tanf(DEG2RAD * FOV / 2.f);
        
        float result[4][4] = {
            {a / aspect, 0.f, 0.f, 0.f},
            {0.f, a, 0.f, 0.f},
            {0.f, 0.f, -(pFar + pNear) / (pFar - pNear), -(2 * pFar * pNear) / (pFar - pNear)},
            {0.f, 0.f, -1.f, 0.f}
        };
        
        return result;
    }
    
    Mat4 Mat4::GetTransposedMatrix()
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = mat[j][i];
            }
        }
        return result;
    }
    
    float Mat4::GetDeterminent()
    {
        return mat[0][0] * (mat[1][1] * mat[2][2] * mat[3][3] //afkp
                            - mat[1][1] * mat[2][3] * mat[3][2] //aflo
                            - mat[1][2] * mat[2][1] * mat[3][3] //agjp
                            + mat[1][2] * mat[2][3] * mat[3][1] //agln
                            + mat[1][3] * mat[2][1] * mat[3][2] //ahjo
                            - mat[1][3] * mat[2][2] * mat[3][1]) //ahkn '
        
            - mat[0][1] * (mat[1][0] * mat[2][2] * mat[3][3] //bekp
                           - mat[1][0] * mat[2][3] * mat[3][2] //belo
                           - mat[1][2] * mat[2][0] * mat[3][3] //bgip
                           + mat[1][2] * mat[2][3] * mat[3][0] //bglm
                           + mat[1][3] * mat[2][0] * mat[3][2] //bhio
                           - mat[1][3] * mat[2][2] * mat[3][0]) //bhkm 
        
            + mat[0][2] * (mat[1][0] * mat[2][1] * mat[3][3] //cejp
                           - mat[1][0] * mat[2][3] * mat[3][1] //celn
                           - mat[1][1] * mat[2][0] * mat[3][3] //cfip
                           + mat[1][1] * mat[2][3] * mat[3][0] //cflm
                           + mat[1][3] * mat[2][0] * mat[3][1] //chin
                           - mat[1][3] * mat[2][1] * mat[3][0]) //chjm 
        
            - mat[0][3] * (mat[1][0] * mat[2][1] * mat[3][2] //dejo
                           - mat[1][0] * mat[2][2] * mat[3][1] //dekn
                           - mat[1][1] * mat[2][0] * mat[3][2] //dfio
                           + mat[1][1] * mat[2][2] * mat[3][0] //dfkm
                           + mat[1][2] * mat[2][0] * mat[3][1] //dgin
                           - mat[1][2] * mat[2][1] * mat[3][0]) //dgjm
            ;
    }
    
    Mat4 Mat4::GetIdentityMatrix()
    {
        Mat4 res;
        float matrice[4][4] = {
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f}
        };
        
        res = matrice;
        return res;
    }

    Quaternion Mat4::ToQuaternion()
    {
        Quaternion res;
        res.a = sqrt(1 + mat[0][0] + mat[1][1] + mat[2][2]) / 2;
        res.b = (mat[2][1] - mat[1][2]) / (4 * res.a);
        res.c = (mat[0][2] - mat[2][0]) / (4 * res.a);
        res.d = (mat[1][0] - mat[0][1]) / (4 * res.a);

        return res;
    }
    
    Mat4 Mat4::GetSubmat(int l, int c)
    {
        Mat4 sub;
        int line = 0;
        int collumn = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i != l && j != c)
                {
                    sub.mat[line][collumn] = mat[i][j];
                    collumn++;
                    if (collumn == 3)
                    {
                        collumn = 0;
                        line++;
                    }
                }
            }
        }
        return sub;
    }
    
    Mat4 Mat4::GetCoMatrix()
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if ((i + j + 1) % 2 == 0)
                    result.mat[i][j] = -GetSubmat(i, j).GetDeterminent();
                else
                    result.mat[i][j] = GetSubmat(i, j).GetDeterminent();
            }
        }
        
        return result;
    }
    
    Mat4 Mat4::GetComplementaryMat()
    {
        return GetCoMatrix().GetTransposedMatrix();
    }
    
    //If determinent is nul, return identity
    Mat4 Mat4::GetInverseMatrix()
    {
        float det = GetDeterminent();
        
        if (Misc::Abs(det) < 0.000005f)
            return GetIdentityMatrix();
        
        return GetComplementaryMat() * (1.f / det);
    }
    
    Float4 Mat4::GetMatLine(int index)
    {
        if (index < 0 && index > 3)
            return 0;
        
        return (Float4)(mat[index][0], mat[index][1], mat[index][2], mat[index][3]);
    }
    
    bool Mat4::operator==(const Mat4& pOther)
    {
        short  counter = 0;
        for (int i=0; i < 16; i += 4)
        {
            bool check =
                mat16[i]     == pOther.mat16[i]     && 
                mat16[i + 1] == pOther.mat16[i + 1] &&
                mat16[i + 2] == pOther.mat16[i + 2] &&
                mat16[i + 3] == pOther.mat16[i + 3];
            counter += check;
        }
        return counter;
    }
#pragma endregion
    
    // --------------------------[Quaternion]--------------------------
#pragma region Quaternion
    
#pragma region Constructors
    Quaternion::Quaternion() : a(0), b(0), c(0), d(0)
    {}
    
    Quaternion::Quaternion(const float& _a, const float& _b, const float& _c, const float& _d) :
    a(_a), b(_b), c(_c), d(_d)
    {}
    
    Quaternion::Quaternion(const float& pRoll, const float& pPitch, const float& pYaw)
    {
        *this = FromEuler(pRoll, pPitch, pYaw);
    }
    
    Quaternion::Quaternion(const Float3& pEulerAngles)
    {
        *this = FromEuler(pEulerAngles);
    }
#pragma endregion
    
    float Quaternion::SquaredModulus() const
    {
        return a * a + b * b + c * c + d * d;
    }
    
    float Quaternion::Modulus() const
    {
        return sqrtf(a * a + b * b + c * c + d * d);
    }
    
    void Quaternion::Normalize()
    {
        float mod = Modulus();
        if (mod == 0.f) return;
        a /= mod;
        b /= mod;
        c /= mod;
        d /= mod;
    }
    
    Mat4 Quaternion::GetRotationMatrix() const
    {
        float matrix[4][4] =
        { {2 * (a * a + b * b) - 1,    2 * (b * c - d * a),      2 * (b * d + c * a),  0},
            {2 * (b * c + d * a),         2 * (a * a + c * c) - 1,  2 * (c * d - b * a),  0},
            {2 * (b * d - c * a),         2 * (c * d + b * a),      2 * (a * a + d * d) - 1,  0},
            {0,                     0,                  0,              1.f} };
        
        return matrix;
    }
    
    Quaternion Quaternion::GetNormalized() const
    {
        float mod = Modulus();
        if (mod == 0.f) return Quaternion();
        return { a / mod, b / mod, c / mod, d / mod };
    }
    
#pragma  region StaticsMethods
    Quaternion Quaternion::Hamilton(const Quaternion& pRight, const Quaternion& pLeft)
    {
        Quaternion result;
        
        result.a = pRight.a * pLeft.a - pRight.b * pLeft.b - pRight.c * pLeft.c - pRight.d * pLeft.d;
        result.b = pRight.a * pLeft.b + pRight.b * pLeft.a + pRight.c * pLeft.d - pRight.d * pLeft.c;
        result.c = pRight.a * pLeft.c - pRight.b * pLeft.d + pRight.c * pLeft.a + pRight.d * pLeft.b;
        result.d = pRight.a * pLeft.d + pRight.b * pLeft.c - pRight.c * pLeft.b + pRight.d * pLeft.a;
        
        return result;
    }
    
    Quaternion Quaternion::FromEuler(const float& pRoll, const float& pPitch, const float& pYaw)
    {
        Quaternion result;
        
        const float cr = cosf(pRoll * 0.5f);
        const float sr = sinf(pRoll * 0.5f);
        const float cp = cosf(pPitch * 0.5f);
        const float sp = sinf(pPitch * 0.5f);
        const float cy = cosf(pYaw * 0.5f);
        const float sy = sinf(pYaw * 0.5f);
        
        result.a = cr * cp * cy + sr * sp * sy;
        result.b = sr * cp * cy - cr * sp * sy;
        result.c = cr * sp * cy + sr * cp * sy;
        result.d = cr * cp * sy - sr * sp * cy;
        
        return result;
    }
    
    Quaternion Quaternion::FromEuler(const Float3& EulerpAngles)
    {
        Quaternion result;
        
        const float cr = cosf(EulerpAngles.x * 0.5f);
        const float sr = sinf(EulerpAngles.x * 0.5f);
        const float cp = cosf(EulerpAngles.y * 0.5f);
        const float sp = sinf(EulerpAngles.y * 0.5f);
        const float cy = cosf(EulerpAngles.z * 0.5f);
        const float sy = sinf(EulerpAngles.z * 0.5f);
        
        result.a = cr * cp * cy + sr * sp * sy;
        result.b = sr * cp * cy - cr * sp * sy;
        result.c = cr * sp * cy + sr * cp * sy;
        result.d = cr * cp * sy - sr * sp * cy;

        return result;
    }
    
    Float3 Quaternion::ToEuler(const Quaternion& quaternion)
    {
        float sinp = 2 * (quaternion.a * quaternion.c - quaternion.d * quaternion.b);
        float pitch = 0;
        if (fabs(sinp) >= 1)
        {
            pitch = copysignf(PI / 2, sinp);
        }
        else
        {
            pitch = asinf(sinp);
        }

        return
        {
            atan2f(2 * (quaternion.a * quaternion.b + quaternion.c * quaternion.d), 1 - 2 * (quaternion.b * quaternion.b + quaternion.c * quaternion.c)),
            pitch,
            atan2f(2 * (quaternion.a * quaternion.d + quaternion.b * quaternion.c), 1 - 2 * (quaternion.c * quaternion.c + quaternion.d * quaternion.d))
        };
    }
    
    Float3 Quaternion::ToEuler()
    {
        float sinp = 2 * (a * c - d * b);
        float pitch = 0;
        if (fabs(sinp) >= 1)
        {
            pitch = copysignf(PI / 2, sinp);
        }
        else
        {
            pitch = asinf(sinp);
        }

        return
        {
            atan2f(2 * (a * b + c * d), 1 - 2 * (b * b + c * c)),
            pitch,
            atan2f(2 * (a * d + b * c), 1 - 2 * (c * c + d * d))
        };
    }
    
    Quaternion Quaternion::AngleAxis(Float3& pAxis, const float& pAngle)
    {
        Quaternion result;
        Float3 axis = pAxis.GetNormalized();
        float sinTetha = sin(pAngle / 2);
        
        result.a = cosf(pAngle / 2);
        result.b = sinTetha * axis.x;
        result.c = sinTetha * axis.y;
        result.d = sinTetha * axis.z;
        
        return result;
    }
    
    float Quaternion::DotProduct(const Quaternion& pFirst, const Quaternion& pSecond)
    {
        return  pFirst.a * pSecond.a + pFirst.b * pSecond.b + pFirst.c * pSecond.c + pFirst.d * pSecond.d;
    }
    
    Quaternion Quaternion::SLerp(const Quaternion& pFirst, const Quaternion& pSecond, float t)
    {
        float alpha = (float) acos(DotProduct(pFirst, pSecond));

        if (alpha < 0.0f) alpha = -alpha;
        if (alpha == 0.0f || isnan(alpha)) return pFirst;

        const float sinAlpha = sinf(alpha);
        t = t / 2;

        return (pFirst * (sinf((1 - t) * alpha) / sinAlpha) + pSecond * (sinf(t * alpha) / sinAlpha)).GetNormalized();
    }
    
    Quaternion Quaternion::NLerp(const Quaternion& pFirst, const Quaternion& pSecond, float t)
    {
        return (pFirst * (1 - t) + pSecond * t).GetNormalized();
    }
#pragma endregion
    
#pragma region Operators
    Quaternion Quaternion::operator*(const Quaternion& pOther) const
    {
        return Hamilton(*this, pOther);
    }
    
    Quaternion Quaternion::operator+(const Quaternion& pOther) const
    {
        return { this->a + pOther.a, this->b + pOther.b, this->c + pOther.c, this->d + pOther.d };
    }
    
    Quaternion Quaternion::operator*(const float& pOther) const
    {
        return { a * pOther, b * pOther, c * pOther, d * pOther };
    }
#pragma endregion
    
#pragma endregion
    
    // -----------------------------------------------------------------
#pragma region Misc
    namespace Misc
    {
        float Lerp(const float& t, const float& a, const float& b)
        {
            return t * a + (1 - t) * b;
        }
        
        float Clamp(const float value, const float min, const float max)
        {
            if (value < min)
                return min;
            if (value > max)
                return max;
            return value;
        }
        
        float Pythagoreantheorem(int nb_values, ...)
        {
            va_list args;
            float nextValue;
            float result;
            
            va_start(args, nb_values);
            
            result = 0;
            for (int i = 0; i < nb_values; i++)
            {
                nextValue = va_arg(args, double);
                result += nextValue * nextValue;
            }
            va_end(args);
            
            return sqrt(result);
        }
        
        float getPointYByLineEquation(Float2 line, Float2 point)
        {
            return line.x * point.x + line.y;
        }
        
        Float2 Barrycentre(int nb_values, ...)
        {
            va_list args;
            Float2 nextValue;
            Float2 result = { 0, 0 };
            
            va_start(args, nb_values);
            
            for (int i = 0; i < nb_values; i++)
            {
                nextValue = va_arg(args, Float2);
                result.x += nextValue.x;
                result.y += nextValue.y;
            }
            va_end(args);
            
            result.x = result.x / nb_values;
            result.y = result.y / nb_values;
            
            return result;
        }
        
        Float3 BarrycentreF3(Float3 point1, Float3 point2, Float3 point3)
        {
            Float3 result = { 0, 0, 0 };
            
            result.x += point1.x + point2.x + point3.x;
            result.y += point1.y + point2.y + point3.y;
            result.z += point1.z + point2.z + point3.z;
            
            result.x = result.x / 3;
            result.y = result.y / 3;
            result.z = result.z / 3;
            
            return result;
        }
        
        Float3 CalcNormal(const Float3& p1, const Float3& p2, const Float3& p3)
        {
            Float3 result{ 0 };
            // Calculate vectors
            const float var1x = p2.x - p1.x;
            const float var1y = p2.y - p1.y;
            const float var1z = p2.z - p1.z;
            
            const float var2x = p3.x - p1.x;
            const float var2y = p3.y - p1.y;
            const float var2z = p3.z - p1.z;
            
            // Get cross product of vectors
            result.x = (var1y * var2z) - (var2y * var1z);
            result.y = (var1z * var2x) - (var2z * var1x);
            result.z = (var1x * var2y) - (var2x * var1y);
            
            // Normalise final vector
            const float vLen = sqrtf((result.x * result.x) + (result.y * result.y) + (result.z * result.z));
            
            result.x = result.x / vLen;
            result.y = result.y / vLen;
            result.z = result.z / vLen;
            
            return result;
        }
    }
#pragma endregion
}
#endif
