#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body {
    Vec2 position;
    Vec2 acceleration;
    Vec2 velocity;

    float rotation;
    float angularVelocity;
    float angularAcceleration;

    Vec2 sumForces;
    float sumTorque;

    float mass;
    float invMass;
    float I;
    float invI;

    Shape* shape = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    Body(const Body& other);
    ~Body();

    void AddForce(Vec2 force);
    void AddTorque(float torque);

    void ClearForces();
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    Body& operator=(const Body& other);
};

#endif