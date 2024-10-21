#include "Force.h"

#include <algorithm>

Vec2 Force::GenerateDragForce(const Body& particle, float k) {
    Vec2 ret = Vec2(0, 0);
    float velocityMag = particle.velocity.MagnitudeSquared();
    if(velocityMag > 0) {
        Vec2 dir = particle.velocity.UnitVector() * -1.0;
        float dragMag = k * velocityMag;
        ret = dir * dragMag;
    }
    return ret;
}

Vec2 Force::GenerateFrictionForce(const Body& particle, float k) {
    Vec2 dir = particle.velocity.UnitVector() * -1.0;
    Vec2 ret = dir * k;
    return ret;
}

Vec2 Force::GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance) {
    Vec2 ab = b.position - a.position;
    float dist = ab.MagnitudeSquared();

    dist = std::clamp(dist, minDistance, maxDistance);

    Vec2 attractionDir = ab.UnitVector();
    float attractionMag = G * (a.mass * b.mass / dist);
    
    return attractionDir * attractionMag;
}

Vec2 Force::GenerateSpringForce(const Body& particle, Vec2 anchor, float restLength, float k) {
    Vec2 d = particle.position - anchor;

    float delta = d.Magnitude() - restLength;
    float springMag = -k * delta;

    return d.UnitVector() * springMag;
}

Vec2 Force::GenerateSpringForce(const Body& a, const Body& b, float restLength, float k) {
    Vec2 d = a.position - b.position;

    float delta = d.Magnitude() - restLength;
    float springMag = -k * delta;

    return d.UnitVector() * springMag;
}
