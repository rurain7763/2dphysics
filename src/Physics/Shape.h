#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"

#include <vector>

enum ShapeType {
    CIRCLE, POLYGON, BOX
};

struct Shape {
    virtual ~Shape() = default;
    virtual Shape* Clone() const = 0;
    virtual ShapeType GetType() const = 0;
};

struct CircleShape : public Shape {
    float radius;

    CircleShape(const float radius) : radius(radius) {}
    virtual Shape* Clone() const override;
    virtual ShapeType GetType() const override { return CIRCLE; }
};

struct PolygonShape : public Shape {
    std::vector<Vec2> vertices; 
    PolygonShape() = default;
    PolygonShape(const std::vector<Vec2>& vertices) : vertices(vertices) {}
    virtual Shape* Clone() const override;
    virtual ShapeType GetType() const override { return POLYGON; }
};

struct BoxShape : public PolygonShape {
    float width, height;

    BoxShape(float width, float height) : width(width), height(height) {}
    virtual Shape* Clone() const override;
    virtual ShapeType GetType() const override { return BOX; }
};

#endif