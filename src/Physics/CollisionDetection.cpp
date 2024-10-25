#include "CollisionDetection.h"

#include <limits>
#include <iostream>

bool CollisionDetection::IsCollision(Body* a, Body* b, Contact& contact) {
    ShapeType aType = a->shape->GetType();
    ShapeType bType = b->shape->GetType();
    if(aType == CIRCLE && bType == CIRCLE) {
        return IsCollidingCircleCircle(a, b, contact);
    } else if((aType == BOX || aType == POLYGON) && (bType == BOX || bType == POLYGON)) {
        return IsCollidingPolygonPoloygon(a, b, contact);
    } else if(aType == CIRCLE && (bType == POLYGON || bType == BOX)) {
        return IsCollidingCirclePolygon(a, b, contact);
    } else if((aType == POLYGON || aType == BOX) && bType == CIRCLE) {
        return IsCollidingCirclePolygon(b, a, contact);
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

bool CollisionDetection::IsCollidingCirclePolygon(Body* a, Body* b, Contact& contact) {
    const CircleShape* circle = static_cast<CircleShape*>(a->shape);
    const PolygonShape* polygon = static_cast<PolygonShape*>(b->shape);

    float distToEdge = std::numeric_limits<float>::lowest();
    Vec2 closestEdgeStart;
    Vec2 closestEdgeEnd;

    for(int i = 0; i < polygon->worldVertices.size(); i++) {
        Vec2 edge = polygon->EdgeAt(i);
        Vec2 edgeNormal = edge.Normal();
        Vec2 toCircle = a->position - polygon->worldVertices[i];

        float projection = toCircle.Dot(edgeNormal);

        if(projection > distToEdge) {
            distToEdge = projection;
            closestEdgeStart = polygon->worldVertices[i];
            closestEdgeEnd = closestEdgeStart + edge;
        }
    }

    if(distToEdge > 0.f) {
        // circle centr is outside of the polygon
        Vec2 v1 = a->position - closestEdgeStart;
        Vec2 v2 = closestEdgeEnd - closestEdgeStart;

        if(v1.Dot(v2) < 0) {
            // region a
            float distToCircle = v1.Magnitude();
            if(distToCircle < circle->radius) {
                contact.a = b;
                contact.b = a;
                contact.depth = circle->radius - distToCircle;
                contact.normal = v1 / distToCircle;
                contact.start = a->position - contact.normal * circle->radius;
                contact.end = contact.start + contact.normal * contact.depth;
                return true;
            }
        } else {
            v1 = a->position - closestEdgeEnd;
            v2 = closestEdgeStart - closestEdgeEnd;

            if(v1.Dot(v2) < 0) {
                // region b
                float distToCircle = v1.Magnitude();
                if(distToCircle < circle->radius) {
                    contact.a = b;
                    contact.b = a;
                    contact.depth = circle->radius - distToCircle;
                    contact.normal = v1 / distToCircle;
                    contact.start = a->position - contact.normal * circle->radius;
                    contact.end = contact.start + contact.normal * contact.depth;
                    return true;
                }               
            } else {
                // region c
                if(distToEdge < circle->radius) {
                    contact.a = b;
                    contact.b = a;
                    contact.depth = circle->radius - distToEdge;
                    contact.normal = (closestEdgeEnd - closestEdgeStart).Normal();
                    contact.start = a->position - contact.normal * circle->radius;
                    contact.end = contact.start + contact.normal * contact.depth;
                    return true;
                }
            }
        }
    } else {
        // circle center is inside the polygon
        // therefore always colliding
        contact.a = b;
        contact.b = a;
        contact.depth = circle->radius - distToEdge;
        contact.normal = (closestEdgeEnd - closestEdgeStart).Normal();
        contact.start = a->position - contact.normal * circle->radius;
        contact.end = contact.start + contact.normal * circle->radius;

        return true;
    }

    return false;
}
