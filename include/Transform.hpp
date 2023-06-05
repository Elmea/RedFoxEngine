#pragma once

#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{

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

		Transform operator*(const Transform& t) const
		{
			return {
				position + orientation * t.position,
				scale,
				orientation * t.orientation
			};
		}

		Transform Inverse() const
		{
			return {
				Inversed(orientation) * Negate(position),
				{-scale.x, -scale.y, -scale.z},
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