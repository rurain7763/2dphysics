#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

#define PARTICLE_RADIUS 4

struct Particle {
    Vec2 position;
    Vec2 acceleration;
    Vec2 velocity;
    float mass;
    Vec2 sumForces;

    Particle(float x, float y, float mass);
    ~Particle();

    void AddForce(Vec2 force);
    void ClearForces();

    void Integrate(float dt);
};

#endif