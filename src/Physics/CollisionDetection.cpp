#include "CollisionDetection.h"

bool CollisionDetection::IsCollision(Body* a, Body* b) {
    ShapeType aType = a->shape->GetType();
    ShapeType bType = a->shape->GetType();

    if(aType == CIRCLE && bType == CIRCLE) {
        return IsCollidingCircleCircle(a, b);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b) {
    CircleShape* aCircle = static_cast<CircleShape*>(a->shape);
    CircleShape* bCircle = static_cast<CircleShape*>(b->shape);

    const float radiusSum = aCircle->radius + bCircle->radius;
    const float sqrtDist = (a->position - b->position).MagnitudeSquared();

    return sqrtDist <= (radiusSum * radiusSum);
}