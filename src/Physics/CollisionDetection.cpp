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
    Vec2 closestEdge;

    for(int i = 0; i < polygon->worldVertices.size(); i++) {
        Vec2 edge = polygon->EdgeAt(i);
        Vec2 toCircle = a->position - polygon->worldVertices[i];

        float projection = edge.Normal().Dot(toCircle);

        if(projection > distToEdge) {
            distToEdge = projection;
            closestEdgeStart = polygon->worldVertices[i];
            closestEdgeEnd = closestEdgeStart + edge;
            closestEdge = edge;
        }
    }

    bool isOutside = distToEdge > 0.f;

    if(isOutside) {
        Vec2 toCircle = a->position - closestEdgeStart;
        float projection = closestEdge.Dot(toCircle);

        if(projection < 0) {
            // region a
            float distToCircle = toCircle.Magnitude();
            if(distToCircle < circle->radius) {
                contact.a = b;
                contact.b = a;
                contact.normal = toCircle / distToCircle;
                contact.start = a->position - contact.normal * circle->radius;
                contact.end = closestEdgeStart;
                contact.depth = circle->radius - distToCircle;
                return true;
            }
        } else {
            float edgeLength = closestEdge.Magnitude();
            projection /= edgeLength;

            if(projection < edgeLength) {
                // region b
                Vec2 closestPoint = closestEdgeStart + (closestEdge / edgeLength * projection);
                toCircle = a->position - closestPoint;
                float distToCircle = toCircle.Magnitude();

                if(distToCircle < circle->radius) {
                    contact.a = b;
                    contact.b = a;
                    contact.normal = toCircle / distToCircle;
                    contact.start = a->position - contact.normal * circle->radius;
                    contact.end = closestPoint;
                    contact.depth = (contact.end - contact.start).Magnitude();
                    return true;
                }
            } else {
                // region c
                toCircle = a->position - closestEdgeEnd;
                float distToCircle = toCircle.Magnitude();

                if(distToCircle < circle->radius) {
                    contact.a = b;
                    contact.b = a;
                    contact.normal = toCircle / distToCircle;
                    contact.start = a->position - contact.normal * circle->radius;
                    contact.end = closestEdgeEnd;
                    contact.depth = circle->radius - distToCircle;
                    return true;
                }
            }
        }
    } else {
        // always colliding
        Vec2 toCircle = b->position - a->position;

        contact.a = b;
        contact.b = a;
        contact.normal = toCircle.UnitVector();
        contact.start = a->position - contact.normal * circle->radius;
        contact.end = b->position + contact.normal * (circle->radius - distToEdge);
        contact.depth = circle->radius - distToEdge;

        return true;
    }

    return false;
}
