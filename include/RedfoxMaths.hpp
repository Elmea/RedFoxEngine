#pragma once

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
#undef near
#undef far
    // ----------------- [Vector] -----------------
#pragma region Vector

    class Float2
    {
    public:
        float x;
        float y;

        float CrossProduct(Float2 other) const;

        float DotProduct(const Float2* other) const;

        void PlanRotation(float angle);
        float Magnitude();
        void Normalize();
        Float2 GetNormalized();
        static Float2 Lerp(const Float2& a, const Float2& b, const float& time);

        Float2 operator+(const Float2& other);
        Float2 operator-(const Float2& other);
        Float2 operator*(const int& multiplicator);
        Float2 operator*(const float& multiplicator);
        Float2 operator/(const float& divider);
        Float2 operator-();
        friend Float2 operator+(const Float2& left, const Float2& right);
        friend Float2 operator-(const Float2& left, const Float2& right);
        friend Float2 operator*(const Float2& left, const float multiplier);
        friend Float2 operator/(const Float2& left, const float divider);

        friend Float2& operator+=(Float2& left, const Float2& right);
        friend Float2& operator-=(Float2& left, const Float2& right);
        friend Float2& operator*=(Float2& left, const float multipler);
        friend Float2& operator/=(Float2& left, const float divider);

        friend bool operator<(const Float2& left, const Float2& right);
        friend bool operator<=(const Float2& left, const Float2& right);
        friend bool operator>(const Float2& left, const Float2& right);
        friend bool operator>=(const Float2& left, const Float2& right);

        bool operator==(Float2& other);
    };

    class Float3
    {
    public:
        float x;
        float y;
        float z;

        Float3 CrossProduct(const Float3& other) const;

        float DotProduct(const Float3 vec3) const;

        Float3 operator+(const Float3& other);
        Float3 operator-(const Float3& other);
        Float3 operator-();
        Float3 operator*(const float& multiplicator);
        Float3 operator*(const int& multiplicator);
        friend Float3 operator+(const Float3& left, const Float3& right);
        friend Float3 operator-(const Float3& left, const Float3& right);
        friend Float3 operator*(const Float3& left, const float multiplier);
        friend Float3 operator/(const Float3& left, const float divider);

        friend Float3& operator+=(Float3& left, const Float3& right);
        friend Float3& operator-=(Float3& left, const Float3& right);
        friend Float3& operator*=(Float3& left, const float multipler);
        friend Float3& operator/=(Float3& left, const float divider);

        friend bool operator<(const Float3& left, const Float3& right);
        friend bool operator<=(const Float3& left, const Float3& right);
        friend bool operator>(const Float3& left, const Float3& right);
        friend bool operator>=(const Float3& left, const Float3& right);


        Float3(float x = 0, float y = 0, float z = 0);

        float Magnitude();
        void Normalize();

        Float3 GetNormalized();

        bool operator==(Float3 other)
        {
            return (x <= other.x + FLOATCOMPACCURATE && x >= other.x - FLOATCOMPACCURATE) && (y <= other.y +
                FLOATCOMPACCURATE && y >= other.y - FLOATCOMPACCURATE) && (z <= other.z + FLOATCOMPACCURATE && z >=
                    other.z - FLOATCOMPACCURATE);
        }


        static Float3 GetSphericalCoords(float r, float theta, float phi);
        static Float3 Lerp(const Float3& a, const Float3& b, const float& time);
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

        float DotProduct(const Float4& other) const;

        Float4 operator+(const Float4& other);
        Float4 operator-(const Float4& other);
        Float4 operator-();
        Float4 operator*(const float& multiplicator);
        Float4 operator*(const int& multiplicator);
        friend Float4 operator+(const Float4& left, const Float4& right);
        friend Float4 operator-(const Float4& left, const Float4& right);
        friend Float4 operator*(const Float4& left, const float multiplier);
        friend Float4 operator/(const Float4& left, const float divider);

        friend Float4& operator+=(Float4& left, const Float4& right);
        friend Float4& operator-=(Float4& left, const Float4& right);
        friend Float4& operator*=(Float4& left, const float multipler);
        friend Float4& operator/=(Float4& left, const float divider);

        friend bool operator<(const Float4& left, const Float4& right);
        friend bool operator<=(const Float4& left, const Float4& right);
        friend bool operator>(const Float4& left, const Float4& right);
        friend bool operator>=(const Float4& left, const Float4& right);

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

        static Mat4 GetRotationX(const float& angle);
        static Mat4 GetRotationY(const float& angle);
        static Mat4 GetRotationZ(const float& angle);
        static Mat4 GetRotation(const float& yaw, const float& pitch, const float& roll);

        static Mat4 GetTranslation(const Float3& translation);
        static Mat4 GetScale(const Float3& scale);

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

        void operator=(const Mat4& other);
        void operator=(const float matrix[4][4]);

        static Mat4 CreateTransformMatrix(const Float3& position, const Float3& rotationDEG, const Float3& scale);
        static Mat4 CreateTransformMatrix(const Float3& position, const Quaternion& rotation, const Float3& scale);

        static Mat4 GetOrthographicMatrix(float right, float left, float top, float bottom, float far, float near);
        static Mat4 GetPerspectiveMatrix(float aspect, float FOV, float far, float near);

        Mat4 operator*(const Mat4& other) const;
        Float4 operator*(const Float4& ft4);
        Mat4 operator*(const float& mult);
        Mat4 operator*(const int& mult);

        const float* AsPtr() const;

        Mat4 GetTransposedMatrix();

        float GetDeterminent();
        Mat4 GetComplementaryMat();
        Mat4 GetCoMatrix();
        Mat4 GetInverseMatrix();
        static Mat4 GetIndentityMatrix();

        Float4 GetMatLine(int index);
    };

    Mat4 operator*(const float& mult, const Mat4& matrix);
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
        Quaternion(const float& roll, const float& pitch, const float& yaw);
        Quaternion(const Float3& eulerAngles);

        void Normalize();
        Quaternion GetNormalized() const;

        float Modulus() const;
        float SquaredModulus() const;

        //Return corresponding rotation matrix
        Mat4 GetRotationMatrix() const;

        static Quaternion Hamilton(const Quaternion& right, const Quaternion& left);

        //Return a Quaternion from corresponding Euler Angles
        static Quaternion FromEuler(const float& roll, const float& pitch, const float& yaw);
        //Return a Quaternion from corresponding Euler Angles
        static Quaternion FromEuler(const Float3& eulerAngles);
        //Return a Float3 as roll pitch yaw from corresponding quaternion
        static Float3 ToEuler(const Quaternion& quaternion);
        Float3 ToEuler();

        //Return a Quaternion from corresponding axis and radian angle
        static Quaternion AngleAxis(const Float3& axis, const float& angle);

        static Quaternion SLerp(const Quaternion& first, const Quaternion& second, float t);
        static Quaternion NLerp(const Quaternion& first, const Quaternion& second, float t);

        static float DotProduct(const Quaternion& first, const Quaternion& second);

        Quaternion operator*(const Quaternion& other) const;
        Quaternion operator*(const float& other) const;
        Quaternion operator+(const Quaternion& other) const;
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

        Float2 barrycentre(int nb_values, ...);

        Float3 barrycentreF3(Float3 point1, Float3 point2, Float3 point3);

        Float3 calcNormal(const Float3& p1, const Float3& p2, const Float3& p3);
    }
#pragma endregion

    // ----------------------------------------------------------- [Implementation] -----------------------------------------------------------

#ifdef REDFOXMATHS_IMPLEMENTATION

#pragma region Float2

    float Float2::CrossProduct(Float2 other) const
    {
        return this->x * other.y - this->y * other.x;
    }

    float Float2::DotProduct(const Float2* other) const
    {
        return (this->x * other->x) + (this->y * other->y);
    }

    void Float2::PlanRotation(float angle)
    {
        float tmpx = this->x;
        this->x = this->x * cosf(DEG2RAD * angle) - this->y * sinf(DEG2RAD * angle);
        this->y = tmpx * sinf(DEG2RAD * angle) + this->y * cosf(DEG2RAD * angle);
    }

    float Float2::Magnitude()
    {
        return sqrtf(x * x + y * y);
    }

    void Float2::Normalize()
    {
        float mag = Magnitude();

		if (mag == 0.0f)
			mag = 1.0;
        x = x / mag;
        y = y / mag;
    }

    Float2 Float2::GetNormalized()
    {
        float mag = Magnitude();

		if (mag == 0.0f)
			mag = 1.0;
        return { x / mag, y / mag };
    }

    Float2 Float2::Lerp(const Float2& a, const Float2& b, const float& time)
    {
        return { Misc::Lerp(time, a.x, b.x), Misc::Lerp(time, a.y, b.y) };
    }

#pragma region operators
    bool Float2::operator==(Float2& other)
    {
        return (x <= other.x + FLOATCOMPACCURATE && x >= other.x - FLOATCOMPACCURATE) &&
            (y <= other.y + FLOATCOMPACCURATE && y >= other.y - FLOATCOMPACCURATE);
    }

    Float2 Float2::operator+(const Float2& other)
    {
        Float2 result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    Float2 operator+(const Float2& left, const Float2& right)
    {
        return { left.x + right.x, left.y + right.y };
    }

    Float2 Float2::operator-(const Float2& other)
    {
        Float2 result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    Float2 Float2::operator-()
    {
        return { -x, -y };
    }

    Float2 operator-(const Float2& left, const Float2& right)
    {
        return { left.x - right.x, left.y - right.y };
    }

    Float2 Float2::operator*(const float& multiplicator)
    {
        return { x * multiplicator, y * multiplicator };
    }

    Float2 operator*(const Float2& left, const float multiplier)
    {
        return { left.x * multiplier, left.y * multiplier };
    }

    Float2 Float2::operator/(const float& divider)
    {
        return { x / divider, y / divider };
    }

    Float2 operator/(const Float2& left, const float divider)
    {
        return { left.x / divider, left.y / divider };
    }

    Float2& operator+=(Float2& left, const Float2& right)
    {
        left = left + right;
        return left;
    }

    Float2& operator-=(Float2& left, const Float2& right)
    {
        left = left - right;
        return left;
    }

    Float2& operator*=(Float2& left, const float multipler)
    {
        left = left * multipler;
        return left;
    }

    Float2& operator/=(Float2& left, const float divider)
    {
        left = left / divider;
        return left;
    }

    Float2 Float2::operator*(const int& multiplicator)
    {
        return { x * multiplicator, y * multiplicator };
    }

    bool operator<(const Float2& left, const Float2& right)
    {
        if (left.x < right.x && left.y < right.y)
            return true;
        else
            return false;
    }

    bool operator<=(const Float2& left, const Float2& right)
    {
        if (left.x <= right.x && left.y <= right.y)
            return true;
        else
            return false;
    }

    bool operator>(const Float2& left, const Float2& right)
    {
        if (left.x > right.x && left.y > right.y)
            return true;
        else
            return false;
    }

    bool operator>=(const Float2& left, const Float2& right)
    {
        if (left.x >= right.x && left.y >= right.y)
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

    Float3 Float3::CrossProduct(const Float3& other) const
    {
        Float3 result;

        result.x = y * other.z - z * other.y;
        result.y = x * other.z - z * other.x;
        result.z = y * other.x - x * other.y;

        return result;
    }

    float Float3::DotProduct(const Float3 vec3) const
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

		if (mag == 0.0f)
			mag = 1.0;
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }

    Float3 Float3::GetNormalized()
    {
        float mag = Magnitude();

		if (mag == 0.0f)
			mag = 1.0;
        return { x / mag, y / mag, z / mag };
    }

    Float3 Float3::GetSphericalCoords(float r, float theta, float phi)
    {
        return {
            r * sinf(theta) * cosf(phi),
            r * cosf(theta),
            r * sinf(theta) * sinf(phi)
        };
    }

    Float3 Float3::Lerp(const Float3& a, const Float3& b, const float& time)
    {
        return Float3(Misc::Lerp(time, a.x, b.x), Misc::Lerp(time, a.y, b.y), Misc::Lerp(time, a.z, b.z));
    }

#pragma region operators
    Float3 Float3::operator+(const Float3& other)
    {
        Float3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }

    Float3 operator+(const Float3& left, const Float3& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z };
    }

    Float3 Float3::operator-(const Float3& other)
    {
        Float3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }

    Float3 Float3::operator-()
    {
        return { -x, -y, -z };
    }

    Float3 operator-(const Float3& left, const Float3& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z };
    }

    Float3 Float3::operator*(const float& multiplicator)
    {
        return { x * multiplicator, y * multiplicator, z * multiplicator };
    }

    Float3 operator*(const Float3& left, const float multiplier)
    {
        return { left.x * multiplier, left.y * multiplier, left.z * multiplier };
    }

    Float3 operator/(const Float3& left, const float divider)
    {
        return { left.x / divider, left.y / divider, left.z / divider };
    }

    Float3& operator+=(Float3& left, const Float3& right)
    {
        left = left + right;
        return left;
    }

    Float3& operator-=(Float3& left, const Float3& right)
    {
        left = left - right;
        return left;
    }

    Float3& operator*=(Float3& left, const float multipler)
    {
        left = left * multipler;
        return left;
    }

    Float3& operator/=(Float3& left, const float divider)
    {
        left = left / divider;
        return left;
    }

    Float3 Float3::operator*(const int& multiplicator)
    {
        return { x * multiplicator, y * multiplicator, z * multiplicator };
    }

    bool operator<(const Float3& left, const Float3& right)
    {
        if (left.x < right.x && left.y < right.y && left.z < right.z)
            return true;
        else
            return false;
    }

    bool operator<=(const Float3& left, const Float3& right)
    {
        if (left.x <= right.x && left.y <= right.y && left.z <= right.z)
            return true;
        else
            return false;
    }

    bool operator>(const Float3& left, const Float3& right)
    {
        if (left.x > right.x && left.y > right.y && left.z > right.z)
            return true;
        else
            return false;
    }

    bool operator>=(const Float3& left, const Float3& right)
    {
        if (left.x >= right.x && left.y >= right.y && left.z >= right.z)
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

    float Float4::DotProduct(const Float4& other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    float Float4::Magnitude()
    {
        return sqrtf(x * x + y * y + z * z);
    }

    void Float4::Normalize()
    {
        float mag = Magnitude();

		if (mag == 0.0f)
			mag = 1.0;
        x = x / mag;
        y = y / mag;
        z = z / mag;
        w = w / mag;
    }

    Float4 Float4::GetNormalized()
    {
        float mag = Magnitude();

		if (mag == 0.0f)
			mag = 1.0;
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
    Float4 Float4::operator+(const Float4& other)
    {
        Float4 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        result.w = w + other.w;
        return result;
    }

    Float4 operator+(const Float4& left, const Float4& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w };
    }

    Float4 Float4::operator-(const Float4& other)
    {
        Float4 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        result.w = w - other.w;
        return result;
    }

    Float4 Float4::operator-()
    {
        return { -x, -y, -z, -w };
    }

    Float4 operator-(const Float4& left, const Float4& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w };
    }

    Float4 Float4::operator*(const float& multiplicator)
    {
        return { x * multiplicator, y * multiplicator, z * multiplicator, w * multiplicator };
    }

    Float4 operator*(const Float4& left, const float multiplier)
    {
        return { left.x * multiplier, left.y * multiplier, left.z * multiplier, left.w * multiplier };
    }

    Float4 operator/(const Float4& left, const float divider)
    {
        return { left.x / divider, left.y / divider, left.z / divider, left.w / divider };
    }

    Float4& operator+=(Float4& left, const Float4& right)
    {
        left = left + right;
        return left;
    }

    Float4& operator-=(Float4& left, const Float4& right)
    {
        left = left - right;
        return left;
    }

    Float4& operator*=(Float4& left, const float multipler)
    {
        left = left * multipler;
        return left;
    }

    Float4& operator/=(Float4& left, const float divider)
    {
        left = left / divider;
        return left;
    }

    Float4 Float4::operator*(const int& multiplicator)
    {
        return { x * multiplicator, y * multiplicator, z * multiplicator, w * multiplicator };
    }

    bool operator<(const Float4& left, const Float4& right)
    {
        if (left.x < right.x && left.y < right.y && left.z < right.z && left.w < right.w)
            return true;
        else
            return false;
    }

    bool operator<=(const Float4& left, const Float4& right)
    {
        if (left.x <= right.x && left.y <= right.y && left.z <= right.z && left.w <= right.w)
            return true;
        else
            return false;
    }

    bool operator>(const Float4& left, const Float4& right)
    {
        if (left.x > right.x && left.y > right.y && left.z > right.z && left.w > right.w)
            return true;
        else
            return false;
    }

    bool operator>=(const Float4& left, const Float4& right)
    {
        if (left.x >= right.x && left.y >= right.y && left.z >= right.z && left.w >= right.w)
            return true;
        else
            return false;
    }
#pragma endregion

#pragma endregion

    // ----------------------------[Matrix]----------------------------
#pragma region Matrix

    Mat4::Mat4()
    {}

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

    void Mat4::operator=(const Mat4& other)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat[i][j] = other.mat[i][j];
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

    Mat4 Mat4::operator*(const Mat4& other) const
    {
        Mat4 res;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                res.mat[i][j] = mat[i][0] * other.mat[0][j] + mat[i][1] * other.mat[1][j] + mat[i][2] * other.mat[2][j]
                    + mat[i][3] * other.mat[3][j];
            }
        }
        return res;
    }

    Mat4 Mat4::operator*(const float& mult)
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = mat[i][j] * mult;
            }
        }
        return result;
    }

    Mat4 operator*(const float& mult, const Mat4& matrix)
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = matrix.mat[i][j] * mult;
            }
        }
        return result;
    }

    Mat4 Mat4::operator*(const int& mult)
    {
        Mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = mat[i][j] * mult;
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

    Mat4 Mat4::GetRotationX(const float& angle)
    {
        Mat4 result;
        result.mat[0][0] = 1;

        result.mat[1][1] = cosf(angle);
        result.mat[1][2] = -sinf(angle);
        result.mat[2][1] = sinf(angle);
        result.mat[2][2] = cosf(angle);

        return result;
    }

    Mat4 Mat4::GetRotationY(const float& angle)
    {
        Mat4 result;
        result.mat[1][1] = 1;

        result.mat[0][0] = cosf(angle);
        result.mat[2][0] = -sinf(angle);
        result.mat[0][2] = sinf(angle);
        result.mat[2][2] = cosf(angle);

        return result;
    }

    Mat4 Mat4::GetRotationZ(const float& angle)
    {
        Mat4 result;
        result.mat[2][2] = 1;

        result.mat[0][0] = cosf(angle);
        result.mat[0][1] = -sinf(angle);
        result.mat[1][0] = sinf(angle);
        result.mat[1][1] = cosf(angle);

        return result;
    }

    Mat4 Mat4::GetRotation(const float& yaw, const float& pitch, const float& roll)
    {
        return GetRotationY(pitch) * GetRotationX(yaw) * GetRotationZ(roll);
    }

    Mat4 Mat4::GetTranslation(const Float3& translation)
    {
        Mat4 result;

        result.mat[0][0] = 1;
        result.mat[1][1] = 1;
        result.mat[2][2] = 1;
        result.mat[3][3] = 1;

        result.mat[0][3] = translation.x;
        result.mat[1][3] = translation.y;
        result.mat[2][3] = translation.z;

        return result;
    }

    Mat4 Mat4::GetScale(const Float3& scale)
    {
        Mat4 result;
        result.mat[0][0] = scale.x;
        result.mat[1][1] = scale.y;
        result.mat[2][2] = scale.z;
        result.mat[3][3] = 1;

        return result;
    }

    Mat4 Mat4::CreateTransformMatrix(const Float3& position, const Float3& rotationDEG, const Float3& scale)
    {
        return GetTranslation(position) * GetRotationY(DEG2RAD * rotationDEG.y) * GetRotationX(DEG2RAD * rotationDEG.x)
            * GetRotationZ(DEG2RAD * rotationDEG.z) * GetScale(scale);
    }

    Mat4 Mat4::CreateTransformMatrix(const Float3& position, const Quaternion& rotation, const Float3& scale)
    {
        return GetTranslation(position) * rotation.GetRotationMatrix() * GetScale(scale);
    }

    Mat4 Mat4::GetOrthographicMatrix(float right, float left, float top, float bottom, float far, float near)
    {
        float result[4][4] = {
            {2.f / (right - left), 0.f, 0.f, -(right + left) / (right - left)},
            {0.f, 2.f / (top - bottom), 0.f, -(top + bottom) / (top - bottom)},
            {0.f, 0.f, -2.f / (far - near), -(far + near) / (far - near)},
            {0.f, 0.f, 0.f, 1.f}
        };

        return result;
    }

    Mat4 Mat4::GetPerspectiveMatrix(float aspect, float FOV, float far, float near)
    {
        float a = 1.f / tanf(DEG2RAD * FOV / 2.f);

        float result[4][4] = {
            {a / aspect, 0.f, 0.f, 0.f},
            {0.f, a, 0.f, 0.f},
            {0.f, 0.f, -(far + near) / (far - near), -(2 * far * near) / (far - near)},
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

    Mat4 Mat4::GetIndentityMatrix()
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
            return GetIndentityMatrix();

        return GetComplementaryMat() * (1.f / det);
    }

    Float4 Mat4::GetMatLine(int index)
    {
        if (index < 0 && index > 3)
            return 0;

        return (Float4)(mat[index][0], mat[index][1], mat[index][2], mat[index][3]);
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

    Quaternion::Quaternion(const float& roll, const float& pitch, const float& yaw)
    {
        *this = FromEuler(roll, pitch, yaw);
    }

    Quaternion::Quaternion(const Float3& eulerAngles)
    {
        *this = FromEuler(eulerAngles);
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

		if (mod == 0.0f)
			mod = 1.0;
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
		if (mod == 0.0f)
			mod = 1.0;
        return { a / mod, b / mod, c / mod, d / mod };
    }

#pragma  region StaticsMethods
    Quaternion Quaternion::Hamilton(const Quaternion& right, const Quaternion& left)
    {
        Quaternion result;

        result.a = right.a * left.a + right.b * left.b + right.c * left.c + right.d * left.d;
        result.b = right.a * left.b + right.b * left.a + right.c * left.d + right.d * left.c;
        result.c = right.a * left.c + right.b * left.d + right.c * left.a + right.d * left.b;
        result.d = right.a * left.d + right.b * left.c + right.c * left.b + right.d * left.a;

        return result;
    }

    Quaternion Quaternion::FromEuler(const float& roll, const float& pitch, const float& yaw)
    {
        Quaternion result;

        const float cr = cosf(roll * 0.5f);
        const float sr = sinf(roll * 0.5f);
        const float cp = cosf(pitch * 0.5f);
        const float sp = sinf(pitch * 0.5f);
        const float cy = cosf(yaw * 0.5f);
        const float sy = sinf(yaw * 0.5f);

        result.a = cr * cp * cy + sr * sp * sy;
        result.b = sr * cp * cy - cr * sp * sy;
        result.c = cr * sp * cy + sr * cp * sy;
        result.d = cr * cp * sy - sr * sp * cy;

        return result;
    }

    Quaternion Quaternion::FromEuler(const Float3& EulerAngles)
    {
        return FromEuler(EulerAngles.x, EulerAngles.y, EulerAngles.z);
    }

    Float3 Quaternion::ToEuler(const Quaternion& quaternion)
    {
        return
        {
            atan2f(2 * (quaternion.a * quaternion.b + quaternion.c * quaternion.d), 1 - 2 * (quaternion.b * quaternion.b + quaternion.c * quaternion.c)),
            asinf(2 * (quaternion.a * quaternion.c - quaternion.d * quaternion.b)),
            atan2f(2 * (quaternion.a * quaternion.d + quaternion.b * quaternion.c), 1 - 2 * (quaternion.c * quaternion.c + quaternion.d * quaternion.d))
        };
    }

    Float3 Quaternion::ToEuler()
    {
        return
        {
            atan2f(2 * (a * b + c * d), 1 - 2 * (b * b + c * c)),
            asinf(2 * (a * c - d * b)),
            atan2f(2 * (a * d + b * c), 1 - 2 * (c * c + d * d))
        };
    }

    Quaternion Quaternion::AngleAxis(const Float3& axis, const float& angle)
    {
        Quaternion result;

        float sinTetha = sin(angle / 2);

        result.a = cosf(angle / 2);
        result.b = sinTetha * axis.x;
        result.c = sinTetha * axis.y;
        result.d = sinTetha * axis.z;

        return result;
    }

    float Quaternion::DotProduct(const Quaternion& first, const Quaternion& second)
    {
        return  first.a * second.a + first.b * second.b + first.c * second.c + first.d * second.d;
    }

    //TODO : fix quternion lerp
    Quaternion Quaternion::SLerp(const Quaternion& first, const Quaternion& second, float t)
    {
        float alpha = acosf(DotProduct(first, second));
        const float sinAlpha = sinf(alpha);

#if 0
        const float sinInvTAlpha = sinf((1 - t) * alpha);
        const float sinTAlpha = sinf(t * alpha);

        Quaternion result;
        result.a = sinInvTAlpha / sinAlpha * first.a + sinTAlpha / sinAlpha * second.a;
        result.b = sinInvTAlpha / sinAlpha * first.b + sinTAlpha / sinAlpha * second.b;
        result.c = sinInvTAlpha / sinAlpha * first.c + sinTAlpha / sinAlpha * second.c;
        result.d = sinInvTAlpha / sinAlpha * first.d + sinTAlpha / sinAlpha * second.d;

        result.Normalize();

        return result;
#endif

        return (first * (sinf((1 - t) * alpha) / sinAlpha) + second * (sinf(t * alpha) / sinAlpha)).GetNormalized();
    }

    Quaternion Quaternion::NLerp(const Quaternion& first, const Quaternion& second, float t)
    {
        return (first * (1 - t) + second * t).GetNormalized();
    }
#pragma endregion

#pragma region Operators
    Quaternion Quaternion::operator*(const Quaternion& other) const
    {
        return Hamilton(*this, other);
    }

    Quaternion Quaternion::operator+(const Quaternion& other) const
    {
        return { this->a + other.a, this->b + other.b, this->c + other.c, this->d + other.d };
    }

    Quaternion Quaternion::operator*(const float& other) const
    {
        return { a * other, b * other, c * other, d * other };
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

        Float2 barrycentre(int nb_values, ...)
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

        Float3 barrycentreF3(Float3 point1, Float3 point2, Float3 point3)
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

        Float3 calcNormal(const Float3& p1, const Float3& p2, const Float3& p3)
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

#endif
}
