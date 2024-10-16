#include "Force.h"

Vec2 Force::GenerateDragForce(Particle& particle, float k) {
    Vec2 ret = Vec2(0, 0);
    float velocityMag = particle.velocity.MagnitudeSquared();
    if(velocityMag > 0) {
        Vec2 dir = particle.velocity.UnitVector() * -1.0;
        float dragMag = k * velocityMag;
        ret = dir * dragMag;
    }
    return ret;
}

Vec2 Force::GenerateFrictionForce(Particle& particle, float k) {
    Vec2 ret = Vec2(0, 0);
    float velocityMag = particle.velocity.MagnitudeSquared();
    Vec2 dir = particle.velocity.UnitVector() * -1.0;
    float frictionMag = k * velocityMag;
    ret = dir * frictionMag;
    return ret;
}


