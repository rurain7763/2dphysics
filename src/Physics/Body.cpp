#include "Body.h"
#include "iostream"

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone();
    position = Vec2(x, y);
    this->mass = mass;
    if(mass != 0) {
        invMass = 1.0 / mass;
    } else {
        invMass = 0;
    }
    std::cout << "Particle constructor called!\n";
}

Body::Body(const Body& other) {
    shape = other.shape->Clone();
    position = other.position;
    mass = other.mass;
    if(mass != 0) {
        invMass = 1.0 / mass;
    } else {
        invMass = 0;
    }
}

Body::~Body() {
    std::cout << "Particle destructor called!\n";
    delete shape;
}

void Body::AddForce(Vec2 force) {
    sumForces += force;
}

void Body::ClearForces() {
    sumForces = Vec2(0, 0);
}

void Body::Integrate(float dt) {
    acceleration = sumForces * invMass;

    // Euler integration
    velocity += acceleration * dt;
    position += velocity * dt;

    ClearForces();
}

Body& Body::operator=(const Body& other) {
    if(this != &other) {
        delete shape;
        
        shape = other.shape->Clone();
        position = other.position;
        mass = other.mass;
        if(mass != 0) {
            invMass = 1.0 / mass;
        } else {
            invMass = 0;
        }
    }
    return *this;
}