#ifndef FORCE_H
#define FORCE_H

#include "Vec2.h"
#include "Body.h"

class Force {
public:
    static Vec2 GenerateDragForce(const Body& particle, float k);
    static Vec2 GenerateFrictionForce(const Body& particle, float k); 
    static Vec2 GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);
    static Vec2 GenerateSpringForce(const Body& particle, Vec2 anchor, float restLength, float k);
    static Vec2 GenerateSpringForce(const Body& a, const Body& b, float restLength, float k);
};

#endif