#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"

#include <vector>

enum ShapeType {
    CIRCLE, POLYGON, BOX
};

struct Shape {
    virtual ~Shape() = default;

    virtual float GetMomentOfInertia() const = 0;
    virtual Shape* Clone() const = 0;
    virtual ShapeType GetType() const = 0;
};

struct CircleShape : public Shape {
    float radius;

    CircleShape(const float radius) : radius(radius) {}

    virtual float GetMomentOfInertia() const override;
    virtual Shape* Clone() const override;
    virtual ShapeType GetType() const override { return CIRCLE; }
};

struct PolygonShape : public Shape {
    std::vector<Vec2> localVertices; 
    std::vector<Vec2> worldVertices;
    
    PolygonShape() = default;
    PolygonShape(const std::vector<Vec2>& vertices);
    virtual ~PolygonShape() = default;

    void UpdateVertices(Vec2 position, float rotation);

    Vec2 EdgeAt(int idx) const;

    float FindMinimumSeperation(const PolygonShape* other, Vec2& axis, Vec2& vertex) const;

    virtual float GetMomentOfInertia() const override;
    virtual Shape* Clone() const override;
    virtual ShapeType GetType() const override { return POLYGON; }
};

struct BoxShape : public PolygonShape {
    float width, height;

    BoxShape(float width, float height);

    virtual float GetMomentOfInertia() const override;
    virtual Shape* Clone() const override;
    virtual ShapeType GetType() const override { return BOX; }
};

#endif