#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"

struct CollisionDetection {
    static bool IsCollision(Body* a, Body* b);
    static bool IsCollidingCircleCircle(Body* a, Body* b);
};

#endif