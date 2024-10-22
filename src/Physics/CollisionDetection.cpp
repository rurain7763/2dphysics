#include "CollisionDetection.h"

bool CollisionDetection::IsCollision(Body* a, Body* b, Contact& contact) {
    ShapeType aType = a->shape->GetType();
    ShapeType bType = a->shape->GetType();

    if(aType == CIRCLE && bType == CIRCLE) {
        return IsCollidingCircleCircle(a, b, contact);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact) {
    CircleShape* aCircle = static_cast<CircleShape*>(a->shape);
    CircleShape* bCircle = static_cast<CircleShape*>(b->shape);

    Vec2 ab = b->position - a->position;

    const float radiusSum = aCircle->radius + bCircle->radius;
    const float sqrtDist = ab.MagnitudeSquared();

    bool isColliding = sqrtDist <= (radiusSum * radiusSum);

    if(!isColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;

    contact.normal = ab.UnitVector();
    contact.start = b->position + (-contact.normal * bCircle->radius);
    contact.end = a->position + (contact.normal * aCircle->radius);
    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
}