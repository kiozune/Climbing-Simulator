#include "Utility.h"

Vector3 operator*(const Mtx44& lhs, const Vector3& rhs)
{
	float b[4];
	for (int i = 0; i < 4; ++i)
		b[i] = lhs.a[i] * rhs.x + lhs.a[4 + i] * rhs.y + lhs.a[8 + i] * rhs.z + lhs.a[12 + i];
	return Vector3(b[0], b[1], b[2]);
}

Position operator*(const Mtx44& lhs, const Position& rhs)
{
	float b[4];
	for (int i = 0; i < 4; ++i)
		b[i] = lhs.a[i] * rhs.x + lhs.a[4 + i] * rhs.y + lhs.a[8 + i] * rhs.z + lhs.a[12 + i];
	return Position(b[0], b[1], b[2]);
}

int randDir() { return rand() % 2 * 2 - 1; }