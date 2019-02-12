#include "Scene.h"

#include <cmath>

float Scene::toRad(const int x) { return (float)x * 3.1415926 / 180; }

float Scene::v(int i) { return i * (float(rand() % 200) / 100.0 - 1); }
float Scene:: c(int i) { return i * (float(rand() % 100) / 100.0); }