#include "Shape.h"

float CircleShape::GetMomentOfInertia() const {
    return (radius * radius) * 0.5f;
}

Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices) {
    localVertices = vertices;
    worldVertices = vertices;
}

void PolygonShape::UpdateVertices(Vec2 position, float rotation) {
    for(int i = 0; i < localVertices.size(); i++) {
        worldVertices[i] = localVertices[i].Rotate(rotation);
        worldVertices[i] += position;
    }
}

Vec2 PolygonShape::EdgeAt(int idx) const {
    return worldVertices[(idx + 1) % worldVertices.size()] - worldVertices[idx];
}

float PolygonShape::FindMinimumSeperation(const PolygonShape* other, Vec2& axis, Vec2& vertex) const {
    float seperation = std::numeric_limits<float>::lowest();

    for(int i = 0; i < worldVertices.size(); i++) {
        const Vec2& vert = worldVertices[i];
        Vec2 edge = EdgeAt(i);
        Vec2 normal = edge.Normal();

        float minSeperation = 0;
        Vec2 minSeperationVertex;
        for(auto& otherVert : other->worldVertices) {
            float projection = normal.Dot(otherVert - vert);
            if(projection < minSeperation) {
                minSeperation = projection;
                minSeperationVertex = otherVert;
            }
        }

        if(minSeperation > seperation) {
            seperation = minSeperation;
            axis = edge;
            vertex = minSeperationVertex;
        }
    }

    return seperation;
}

float PolygonShape::GetMomentOfInertia() const {
    return 0.f;
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices);
}

BoxShape::BoxShape(float width, float height) 
    : width(width), height(height)
{
    const float halfW = width * 0.5f;
    const float halfH = height * 0.5f;

    localVertices.push_back({-halfW, -halfH});
    localVertices.push_back({halfW, -halfH});
    localVertices.push_back({halfW, halfH});
    localVertices.push_back({-halfW, halfH});

    worldVertices = localVertices;
}

float BoxShape::GetMomentOfInertia() const {
    return (width * width + height * height) * 0.083333f;
}

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
}