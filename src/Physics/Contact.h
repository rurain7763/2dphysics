#ifndef CONTACT_H
#define CONTACT_H

#include "Body.h"
#include "Vec2.h"

struct Contact {
    Body* a;
    Body* b;

    // a -> b
    Vec2 start, end;
    Vec2 normal;
    float depth;

    Contact() = default;
    ~Contact() = default;
};

#endif