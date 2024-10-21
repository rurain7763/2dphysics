#include "Shape.h"

float CircleShape::GetMomentOfInertia() const {
    return (radius * radius) * 0.5f;
}

Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
}

float PolygonShape::GetMomentOfInertia() const {
    return 0.f;
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(vertices);
}

float BoxShape::GetMomentOfInertia() const {
    return (width * width + height * height) * 0.083333f;
}

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
}