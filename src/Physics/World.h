#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include "Vec2.h"

#include <vector>

class World {
public:
    World(float gravity);
    ~World();

    void AddBody(Body* body);
    std::vector<Body*>& GetBodies();

    void AddForce(const Vec2& force);
    void AddTorque(float torque);

    void Update(float deltaTime);

    void CheckCollisions();

private:
    std::vector<Body*> _bodies;
    std::vector<Vec2> _forces;
    std::vector<float> _torques;

    float _gravity = 9.8f;
};

#endif