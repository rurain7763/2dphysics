#include "Shape.h"

Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(vertices);
}

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
}