#ifndef UTILITY_H
#define UTILITY_H

#include "Mtx44.h"
#include "vertex.h"

// pi values and convertion formulas
#define M_PI   3.14159265358979323846264338327950288
#define rad(d) (d * M_PI / 180.0)
#define deg(r) (r * 180.0 / M_PI)

Vector3 operator*(const Mtx44& lhs, const Vector3& rhs);
Position operator*(const Mtx44& lhs, const Position& rhs);

int randDir();

#endif 