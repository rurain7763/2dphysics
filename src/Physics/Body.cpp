#include "Body.h"
#include "iostream"

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone();
    position = Vec2(x, y);
    this->mass = mass;
    if(mass != 0.f) {
        invMass = 1.0f / mass;
    } else {
        invMass = 0.f;
    }
    velocity = Vec2(0.f, 0.f);
    acceleration = Vec2(0.f, 0.f);

    rotation = 0.f;
    angularVelocity = 0.f;
    angularAcceleration = 0.f;
    I = shape.GetMomentOfInertia() * mass;
    if(I != 0.f) {
        invI = 1.0f / I;
    } else {
        invI = 0.f;
    }

    restitution = 1.0;

    friction = 0.7f;

    sumForces = Vec2(0.f, 0.f);
    sumTorque = 0.f;

    std::cout << "Particle constructor called!\n";
}

Body::Body(const Body& other) {
    shape = other.shape->Clone();
    position = other.position;
    mass = other.mass;
    velocity = other.velocity;
    acceleration = other.acceleration;
    rotation = other.rotation;
    angularVelocity = other.angularVelocity;
    angularAcceleration = other.angularAcceleration;
}

Body::~Body() {
    std::cout << "Particle destructor called!\n";
    delete shape;
}

void Body::AddForce(const Vec2& force) {
    sumForces += force;
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}

void Body::IntergrateForces(float dt) {
    if(IsStatic()) {
        ClearForces();
        ClearTorque();
        return;
    }

    acceleration = sumForces * invMass;
    velocity += acceleration * dt;

    angularAcceleration = sumTorque * invI;
    angularVelocity += angularAcceleration * dt;

    ClearForces();
    ClearTorque();
}

void Body::IntergrateVelocities(float dt) {
    if(IsStatic()) return;

    position += velocity * dt;
    rotation += angularVelocity * dt;

    shape->UpdateVertices(position, rotation);
}

void Body::ClearForces() {
    sumForces = Vec2(0.f, 0.f);
}

void Body::ClearTorque() {
    sumTorque = 0.f;
}

void Body::ApplyImpulse(const Vec2& impulse) {
    if(IsStatic()) {
        return;
    }
    velocity += impulse * invMass;
}

void Body::ApplyImpulse(const Vec2& impulse, const Vec2& contactVector) {
    if(IsStatic()) {
        return;
    }
    velocity += impulse * invMass;
    angularVelocity += contactVector.Cross(impulse) * invI;
}

bool Body::IsStatic() { 
    const float epsilon = 0.005f; 
    return fabs(invMass - 0.0) < epsilon; 
}

Vec2 Body::WorldToLocal(const Vec2& worldPoint) const {
    Vec2 ret = worldPoint - position;
    ret = ret.Rotate(-rotation);
    return ret;
}

Vec2 Body::LocalToWorld(const Vec2& localPoint) const {
    Vec2 ret = localPoint.Rotate(rotation);
    ret += position;
    return ret;
}

Body& Body::operator=(const Body& other) {
    if(this != &other) {
        delete shape;

        shape = other.shape->Clone();
        position = other.position;
        mass = other.mass;
    }
    return *this;
}