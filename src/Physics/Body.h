#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body {
    Vec2 position;
    Vec2 acceleration;
    Vec2 velocity;

    float mass;
    float invMass;

    Vec2 sumForces;

    Shape* shape = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    Body(const Body& other);
    ~Body();

    void AddForce(Vec2 force);
    void ClearForces();

    void Integrate(float dt);

    Body& operator=(const Body& other);
};

#endif