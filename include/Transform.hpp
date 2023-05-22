#pragma once

#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{

	//class Vector3
	//{
	//public:
	//	union
	//	{
	//		struct { float x; float y; float z; };
	//		float e[3];
	//	};

	//	Vector3() : x(0.f), y(0.f), z(0.f) { }
	//	Vector3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) { }

	//	float Lenght() const { return (sqrt(x * x + y * y + z * z)); }

	//	static float Dot(const Vector3& a, const Vector3& b)
	//	{
	//		return ((float)(a.x * b.x + a.y * b.y + a.z * b.z));
	//	}

	//	void Normalize()
	//	{
	//		float length = this->Lenght();
	//		if (length == 0.f) length = 1.f;
	//		x = x / length;
	//		y = y / length;
	//		z = z / length;
	//	}

	//	Vector3 operator+(const Vector3 a)  const { return { x + a.x, y + a.y, z + a.z }; }
	//	Vector3 operator-()				    const { return { -x, -y, -z }; }
	//	Vector3 operator*(const float scale)  const { return { x * scale, y * scale, z * scale }; }
	//	Vector3 operator*(const Vector3& v) const { return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; }

	//	friend Vector3 operator/(const float& f, const Vector3& v) { return { f / v.x, f / v.y, f / v.z }; }
	//	friend Vector3 operator*(float s, const Vector3& v) { return { v.x * s, v.y * s, v.z * s }; }
	//	friend Vector3 operator-(const Vector3& a, const Vector3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }

	//	void operator+=(const Vector3 a)
	//	{
	//		this->x = x + a.x;
	//		this->y = y + a.y;
	//		this->z = z + a.z;
	//	}

	//	void operator*=(const float scale)
	//	{
	//		this->x = x * scale;
	//		this->y = y * scale;
	//		this->z = z * scale;
	//	}

	//	static Vector3 Cross(Vector3& a, Vector3& b)
	//	{
	//		return(Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x));
	//	}
	//};



	//class Quaternion
	//{
	//public:
	//	float a = 1.f, i = 0.f, j = 0.f, k = 0.f;


	//	Quaternion()
	//	{
	//		a = 1;
	//	}
	//	Quaternion(const float _a, const float _i, const float _j, const float _k)
	//		: a(_a), i(_i), j(_j), k(_k) { }

	//	Quaternion(const float angle_rad, const Vector3& axis)
	//	{
	//		const float sina = sin(angle_rad / 2.f);
	//		a = cos(angle_rad / 2.f);
	//		i = sina * axis.x;
	//		j = sina * axis.y;
	//		k = sina * axis.z;
	//	}

	//	Quaternion operator+ (const Quaternion& q) const
	//	{
	//		return { a + q.a, i + q.i, j + q.j, k + q.k };
	//	}

	//	Quaternion operator* (const float f) const
	//	{
	//		return { a * f, i * f, j * f, k * f };
	//	}

	//	Quaternion operator* (const Quaternion& q) const
	//	{
	//		return {
	//			a * q.a - i * q.i - j * q.j - k * q.k,
	//			a * q.i + q.a * i + j * q.k - k * q.j,
	//			a * q.j + q.a * j + k * q.i - i * q.k,
	//			a * q.k + q.a * k + i * q.j - j * q.i
	//		};
	//	}

	//	Vector3 operator* (const Vector3& v) const
	//	{
	//		Vector3 res;

	//		res.x = (1.0f - 2.0f * j * j - 2.0f * k * k) * v.x + (2.0f * i * j - 2.0f * k * a) * v.y + (2.0f * i * k + 2.0f * j * a) * v.z;
	//		res.y = (2.0f * i * j + 2.0f * k * a) * v.x + (1.0f - 2.0f * i * i - 2.0f * k * k) * v.y + (2.0f * j * k - 2.0f * i * a) * v.z;
	//		res.z = (2.0f * i * k - 2.0f * j * a) * v.x + (2.0f * j * k + 2.0f * i * a) * v.y + (1.0f - 2.0f * i * i - 2.0f * j * j) * v.z;

	//		return res;
	//	}

	//	Quaternion operator*=(const Quaternion& q) const
	//	{
	//		return *this * q;
	//	}

	//	Vector3 Rotate(const Vector3& v) const
	//	{
	//		const Quaternion q{
	//			-i * v.x - j * v.y - k * v.z,
	//			 a * v.x + j * v.z - k * v.y,
	//			 a * v.y + k * v.x - i * v.z,
	//			 a * v.z + i * v.y - j * v.x
	//		};

	//		return {
	//			q.a * -i + a * q.i - q.j * k + q.k * j,
	//			q.a * -j + a * q.j - q.k * i + q.i * k,
	//			q.a * -k + a * q.k - q.i * j + q.j * i
	//		};
	//	}

	//	Quaternion Rotate(const Quaternion& q) const
	//	{
	//		return *this * q * (*this).Conjugated();
	//	}

	//	float Dot(Quaternion b) const { return a * b.a + i * b.i + j * b.j + k * b.k; }

	//	Quaternion Inversed()      const { return { -a, i, j, k }; }
	//	Quaternion Conjugated()    const { return { a, -i, -j, -k }; }
	//	Quaternion Negated()       const { return { -a, -i, -j, -k }; }
	//	float      LengthSquared() const { return this->Dot(*this); }
	//	float      Length()        const { return sqrt(LengthSquared()); }
	//	Quaternion Normal()        const { Quaternion r = *this; r.Normalize(); return r; }

	//	static Quaternion SLerp(Quaternion a, Quaternion b, float t)
	//	{
	//		Quaternion c = {};
	//		float theta, mult1, mult2;

	//		/*
	//		 * Dot product the quaternions, so we can do nearest neigboor slerping
	//		 * Explanation:
	//		 * Quaternions encode angles in 720 degrees (4PI), we want the 360 degrees interpolation (2PI)
	//		*/
	//		float dotProduct = a.Dot(b);
	//		if (dotProduct < 0)
	//		{
	//			b = b.Negated();
	//			dotProduct = a.Dot(b);
	//		}
	//		theta = acos(dotProduct);

	//		if (theta > 0.000001)
	//		{
	//			mult1 = sin((1 - t) * theta) / sin(theta);
	//			mult2 = sin(t * theta) / sin(theta);
	//		}
	//		else
	//		{
	//			mult1 = 1 - t;
	//			mult2 = t;
	//		}

	//		c = a * mult1;
	//		c = c + (b * mult2);
	//		return c;
	//	}

	//	void Normalize() { const float l = Length(); a /= l; i /= l; j /= l; k /= l; }
	//};

	static RedFoxMaths::Quaternion Inversed(RedFoxMaths::Quaternion q)
	{
		return { -q.a, q.b, q.c, q.d };
	}

	static RedFoxMaths::Float3 Negate(RedFoxMaths::Float3 v)
	{
		return { -v.x, -v.y, -v.z };
	}

	static RedFoxMaths::Float3 operator*(RedFoxMaths::Quaternion q, RedFoxMaths::Float3 v)
	{
		RedFoxMaths::Float3 res;
		res.x = (1.0f - 2.0f * q.c * q.c - 2.0f * q.c * q.c) * v.x + (2.0f * q.b * q.c - 2.0f * q.c * q.a) * v.y + (2.0f * q.b * q.c + 2.0f * q.c * q.a) * v.z;
		res.y = (2.0f * q.b * q.c + 2.0f * q.c * q.a) * v.x + (1.0f - 2.0f * q.b * q.b - 2.0f * q.c * q.c) * v.y + (2.0f * q.c * q.c - 2.0f * q.b * q.a) * v.z;
		res.z = (2.0f * q.b * q.c - 2.0f * q.c * q.a) * v.x + (2.0f * q.c * q.c + 2.0f * q.b * q.a) * v.y + (1.0f - 2.0f * q.b * q.b - 2.0f * q.c * q.c) * v.z;

		return res;
	}

	class Transform
	{
	public:
		RedFoxMaths::Float3 position;
		RedFoxMaths::Float3 scale;
		RedFoxMaths::Quaternion orientation;

		Transform operator+(const Transform& t) const
		{
			return { position + t.position, scale + t.scale, orientation + t.orientation };
		}

		Transform operator*(const float& f)
		{
			return { position * f, scale * f, orientation * f };
		}

		Transform operator*(const Transform& t) const
		{
			return {
				position + orientation * t.position,
				scale * t.scale,
				orientation * t.orientation
			};
		}

		Transform Inverse() const
		{
			return {
				Inversed(orientation) * Negate(position),
				scale,
				Inversed(orientation)
			};
		}

		void Normalize()
		{
			orientation.Normalize();
		}
	};

	class Matrix4
	{
	public:
		union
		{
			struct {
				float xx, xy, xz, xw;
				float yx, yy, yz, yw;
				float zx, zy, zz, zw;
				float wx, wy, wz, ww;
			};
			float v[16];
			float vm[4][4];
		};

		Matrix4() { xx = yy = zz = ww = 1; }

		void FromTransform(const Transform& transform)
		{
			RedFoxMaths::Float3 temp = { 0, 1, 0 };
			RedFoxMaths::Float3 Forward = transform.orientation * temp;
			temp = { 1, 0, 0 };
			RedFoxMaths::Float3 Right = transform.orientation * temp;
			temp = { 0, 0, 1 };
			RedFoxMaths::Float3 Up = transform.orientation * temp;

			xx = Right.x;   xy = Right.y;   xz = Right.z;
			yx = Forward.x; yy = Forward.y; yz = Forward.z;
			zx = Up.x;      zy = Up.y;      zz = Up.z;

			wx = transform.position.x;
			wy = transform.position.y;
			wz = transform.position.z;
			ww = 1;
		}
	};
}