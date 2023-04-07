#include "Transform.hpp"

void Transform::assign(PxTransform other)
{
	position = RedFoxMaths::Float3(other.p.x, other.p.y, other.p.z);
	orientation = RedFoxMaths::Quaternion(other.q.x, other.q.y, other.q.z, other.q.w);
	scale = RedFoxMaths::Float3(1, 1, 1);
}

inline Transform Transform::operator=(const Transform& pOther)
{
	Transform tr;

	tr.position = pOther.position;
	tr.orientation = pOther.orientation;
	tr.scale = pOther.scale;

	return tr;
}


inline Transform Transform::operator=(const PxTransform& pOther)
{
	Transform tr;
	RedFoxMaths::Float3 pos = (pOther.p.x, pOther.p.y, pOther.p.z);

	RedFoxMaths::Quaternion quat = RedFoxMaths::Quaternion(pOther.q.x, pOther.q.y, pOther.q.z, pOther.q.w);


	tr.position = pos;
	tr.orientation = quat;
	tr.scale = RedFoxMaths::Float3(1, 1, 1);

	return tr;
}