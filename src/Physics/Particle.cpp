#include "Particle.h"
#include "iostream"

Particle::Particle(float x, float y, float mass) {
    position = Vec2(x, y);
    this->mass = mass;
    if(mass != 0) {
        invMass = 1.0 / mass;
    } else {
        invMass = 0;
    }
    std::cout << "Particle constructor called!\n";
}

Particle::~Particle() {
    std::cout << "Particle destructor called!\n";
}

void Particle::AddForce(Vec2 force) {
    sumForces += force;
}

void Particle::ClearForces() {
    sumForces = Vec2(0, 0);
}

void Particle::Integrate(float dt) {
    acceleration = sumForces * invMass;

    // Euler integration
    velocity += acceleration * dt;
    position += velocity * dt;

    ClearForces();
}