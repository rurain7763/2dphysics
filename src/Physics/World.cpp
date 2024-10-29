#include "World.h"

#include "Constant.h"
#include "Contact.h"
#include "CollisionDetection.h"

World::World(float gravity) : _gravity(-gravity) {}

World::~World() {
    for(auto body : _bodies) {
        delete body;
    }
}

void World::AddBody(Body* body) {
    _bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return _bodies;
}

void World::AddForce(const Vec2& force) {
    _forces.push_back(force);
}

void World::AddTorque(float torque) {
    _torques.push_back(torque);
}

void World::Update(float deltaTime) {
    for(auto body : _bodies) {
        Vec2 weight(0.f, body->mass * _gravity * PIXELS_PER_METER);
        body->AddForce(weight);

        for(auto force : _forces) {
            body->AddForce(force);
        }

        for(auto torque : _torques) {
            body->AddTorque(torque);
        }
    }

    for(auto body : _bodies) {
        body->UpdateBody(deltaTime);
    }

    for(int i = 0; i < 10; i++) {
        CheckCollisions();
    }
}

void World::CheckCollisions() {
    for(int i = 0; i < _bodies.size() - 1; i++) {
        for(int j = i + 1; j < _bodies.size(); j++) {
            Body* a = _bodies[i];
            Body* b = _bodies[j];

            Contact contact;
            if(CollisionDetection::IsCollision(a, b, contact)) {
                contact.ResolveCollision();
            }
        }
    }
}