#include "Transform.hpp"


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


//Loses the scale information because physX does not handle it
PxTransform Transform::toPxTransform()
{
	PxTransform newTr;

	newTr.p.x = position.x;
	newTr.p.y = position.y;
	newTr.p.z = position.z;
	
	newTr.q.x = orientation.a;
	newTr.q.y = orientation.b;
	newTr.q.z = orientation.c;
	newTr.q.w = orientation.d;

	return newTr;
}
