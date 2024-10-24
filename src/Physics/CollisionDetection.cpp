#include "CollisionDetection.h"

#include <limits>

bool CollisionDetection::IsCollision(Body* a, Body* b, Contact& contact) {
    ShapeType aType = a->shape->GetType();
    ShapeType bType = a->shape->GetType();

    if(aType == CIRCLE && bType == CIRCLE) {
        return IsCollidingCircleCircle(a, b, contact);
    } else if((aType == BOX || aType == POLYGON) && (bType == BOX || bType == POLYGON)) {
        return IsCollidingPolygonPoloygon(a, b, contact);
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

bool CollisionDetection::IsCollidingPolygonPoloygon(Body* a, Body* b, Contact& contact) {
    // SAT algorithm
    const PolygonShape* aPoly = static_cast<PolygonShape*>(a->shape);
    const PolygonShape* bPoly = static_cast<PolygonShape*>(b->shape);

    Vec2 aAxis, bAxis;
    Vec2 aVertex, bVertex;

    float abSeperation = aPoly->FindMinimumSeperation(bPoly, aAxis, aVertex);

    if(abSeperation >= 0) {
        return false;
    }

    float baSeperation = bPoly->FindMinimumSeperation(aPoly, bAxis, bVertex);

    if(baSeperation >= 0) {
        return false;
    }

    contact.a = a;
    contact.b = b;

    if(abSeperation > baSeperation) {
        contact.depth = -abSeperation;
        contact.normal = aAxis.Normal();
        contact.start = aVertex;
        contact.end = aVertex + contact.normal * contact.depth;
    } else {
        contact.depth = -baSeperation;
        contact.normal = -bAxis.Normal();
        contact.start = bVertex - contact.normal * contact.depth;
        contact.end = bVertex;
    }

    return true;
}