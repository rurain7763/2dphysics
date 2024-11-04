#include "World.h"

#include "Constant.h"
#include "Contact.h"
#include "CollisionDetection.h"

World::World(float gravity) : _gravity(-gravity) {}

World::~World() {
    for(auto body : _bodies) {
        delete body;
    }

    for(auto constraint : _constraints) {
        delete constraint;
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

void World::AddConstraint(Constraint* constraint) {
    _constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints() {
    return _constraints;
}

void World::Update(float deltaTime) {
    std::vector<PenetrationConstraint> penetrations;

    for(auto& body : _bodies) {
        Vec2 weight(0.f, body->mass * _gravity * PIXELS_PER_METER);
        body->AddForce(weight);

        for(auto force : _forces) {
            body->AddForce(force);
        }

        for(auto torque : _torques) {
            body->AddTorque(torque);
        }
    }

    for(auto& body : _bodies) {
        body->IntergrateForces(deltaTime);
    }

    for(int i = 0; i < _bodies.size() - 1; i++) {
        for(int j = i + 1; j < _bodies.size(); j++) {
            Body* a = _bodies[i];
            Body* b = _bodies[j];

            Contact contact;
            if(CollisionDetection::IsCollision(a, b, contact)) {
                penetrations.push_back(PenetrationConstraint(a, b, contact.start, contact.end, contact.normal));
            }
        }
    }

    for(auto& contraint : _constraints) {
        contraint->PreSolve(deltaTime);
    }

    for(auto& penetration : penetrations) {
        penetration.PreSolve(deltaTime);
    }

    for(int i = 0; i < 5; i++) {
        for(auto& contraint : _constraints) {
            contraint->Solve();
        }

        for(auto& penetration : penetrations) {
            penetration.Solve();
        }
    }

    for(auto& contraint : _constraints) {
        contraint->PostSolve();
    }

    for(auto& penetration : penetrations) {
        penetration.PostSolve();
    }

    for(auto& body : _bodies) {
        body->IntergrateVelocities(deltaTime);
    }
}