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

    // 0 ~ 1
    // 충돌 처리 시 impulse 계산에 필요
    float restitution;

    // 0 ~ 1
    // 표면 마찰력 계산에 필요
    float friction;

    Shape* shape = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    Body(const Body& other);
    ~Body();

    void AddForce(const Vec2& force);
    void AddTorque(float torque);

    void IntergrateForces(float dt);
    void IntergrateVelocities(float dt);

    void ClearForces();
    void ClearTorque();

    void ApplyImpulseLinear(const Vec2& impulse);
    void ApplyImpulseAngular(const float impulse);
    void ApplyImpulseAtPoint(const Vec2& impulse, const Vec2& contactVector);

    bool IsStatic();

    Vec2 WorldToLocal(const Vec2& worldPoint) const;
    Vec2 LocalToWorld(const Vec2& localPoint) const;

    Body& operator=(const Body& other);
};

#endif