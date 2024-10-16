#include "Particle.h"
#include "iostream"

Particle::Particle(float x, float y, float mass) 
{
    position = Vec2(x, y);
    this->mass = mass;
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
    acceleration = sumForces / mass;

    // Euler integration
    velocity += acceleration * dt;
    position += velocity * dt;

    ClearForces();
}