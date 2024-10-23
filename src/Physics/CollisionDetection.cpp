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

float FindMinimumSeperation(const PolygonShape& a, const PolygonShape& b) {
    float seperation = std::numeric_limits<float>::lowest();

    for(int i = 0; i < a.worldVertices.size(); i++) {
        const Vec2& aVert = a.worldVertices[i];
        Vec2 aNormal = a.EdgeAt(i).Normal();

        float minSeperation = 0;
        for(auto& bVert : b.worldVertices) {
            minSeperation = std::min(minSeperation, aNormal.Dot(bVert - aVert));
        }

        seperation = std::max(seperation, minSeperation);
    }

    return seperation;
}

bool CollisionDetection::IsCollidingPolygonPoloygon(Body* a, Body* b, Contact& contact) {
    // SAT algorithm
    const PolygonShape* aPoly = static_cast<PolygonShape*>(a->shape);
    const PolygonShape* bPoly = static_cast<PolygonShape*>(b->shape);

    // collision check
    if(FindMinimumSeperation(*aPoly, *bPoly) >= 0 || FindMinimumSeperation(*bPoly, *aPoly) >= 0) {
        return false;
    }

    // TODO: set contact

    return true;
}