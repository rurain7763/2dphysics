#include "Force.h"

Vec2 Force::GenerateDragForce(Particle& particle, float k) {
    Vec2 ret = Vec2(0, 0);
    float sqrtMag = particle.velocity.MagnitudeSquared();
    if(sqrtMag > 0) {
        Vec2 dir = particle.velocity.UnitVector() * -1.0;
        float factor = k * sqrtMag;
        ret = dir * factor;
    }
    return ret;
}