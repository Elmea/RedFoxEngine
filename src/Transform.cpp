#include "Transform.hpp"


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
