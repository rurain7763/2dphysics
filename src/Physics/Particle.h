#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    float radius = 4;

    Vec2 position;
    Vec2 acceleration;
    Vec2 velocity;

    float mass;
    float invMass;

    Vec2 sumForces;

    Particle(float x, float y, float mass);
    ~Particle();

    void AddForce(Vec2 force);
    void ClearForces();

    void Integrate(float dt);
};

#endif