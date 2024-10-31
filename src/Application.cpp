#include "Application.h"
#include "Physics/Constant.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"
#include "Actor.h"

#include <iostream>

bool Application::IsRunning() {
    return _running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    _running = Graphics::OpenWindow();

    _world = new World(-9.8f);

    for(int i = 0; i < 10; i++) {
        float mass = i == 0 ? 0.0 : 1.0;
        Body* box = new Body(BoxShape(20, 20), Graphics::Width() / 2 + 20 * i, 100, mass);
        Actor* actor = new Actor(box, "assets/box.png");
        _actors.push_back(actor);
        _world->AddBody(box);
    }

    for(int i = 1; i < 10; i++) {
        Body* a = _world->GetBodies()[i - 1];
        Body* b = _world->GetBodies()[i];
        JointConstraint* joint = new JointConstraint(a, b, a->position);
        _world->AddConstraint(joint);
    }

    _prevFrameTime = SDL_GetTicks();
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    _running = false;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    _isDebug = !_isDebug;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    static int shapeFlag = 0;
                    int msX, msY;
                    SDL_GetMouseState(&msX, &msY);

                    Body* body;
                    if(shapeFlag == 0) {
                        body = new Body(CircleShape(25), msX, msY, 1.0);
                        Actor* actor = new Actor(body, "assets/basketball.png");
                        _actors.push_back(actor);
                    } else {
                        int edgeCount = 3 + rand() % 7;
                        float deg = 360.f / edgeCount;
                        float radius = 50;

                        std::vector<Vec2> vertices;

                        float currentDeg = 0;
                        for(int i = 0; i < edgeCount; i++) {
                            currentDeg += deg;
                            float x = cos(currentDeg * M_PI / 180.f) * radius;
                            float y = sin(currentDeg * M_PI / 180.f) * radius;
                            vertices.push_back(Vec2(x, y));
                        }

                        body = new Body(PolygonShape(vertices), msX, msY, 1.0);
                        body->restitution = 0.5f;

                        Actor* actor = new Actor(body, "");
                        _actors.push_back(actor);
                    }

                    shapeFlag = (shapeFlag + 1) % 2;
                    _world->AddBody(body);
                }
                break;
            case SDL_MOUSEMOTION:
                {
                    int msX2, msY2;
                    SDL_GetMouseState(&msX2, &msY2);
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    int timeToWait = (_prevFrameTime + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - _prevFrameTime) / 1000.f;
    if(deltaTime > 0.016f) {
        deltaTime = 0.016f;
    }
    _prevFrameTime = SDL_GetTicks();
    
    _world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF000030);

    if(_isDebug) {
        for(auto constraint : _world->GetConstraints()) {
            JointConstraint* joint = dynamic_cast<JointConstraint*>(constraint);
            if(joint) {
                Vec2 a = joint->GetBodyA()->position;
                Vec2 b = joint->GetBodyB()->position;
                Graphics::DrawLine(a.x, a.y, b.x, b.y, 0xFF0000FF);
            }
        }
    }

    for(auto actor : _actors) {
        if(actor->body->shape->GetType() == BOX) {
            BoxShape* box = static_cast<BoxShape*>(actor->body->shape);
            Graphics::DrawTexture(actor->body->position.x, actor->body->position.y, box->width, box->height, actor->body->rotation, actor->texture);
        } else if(actor->body->shape->GetType() == CIRCLE) {
            CircleShape* circle = static_cast<CircleShape*>(actor->body->shape);
            Graphics::DrawTexture(actor->body->position.x, actor->body->position.y, circle->radius * 2, circle->radius * 2, actor->body->rotation, actor->texture);
        } else {
            PolygonShape* polygon = static_cast<PolygonShape*>(actor->body->shape);
            Graphics::DrawFillPolygon(actor->body->position.x, actor->body->position.y, polygon->worldVertices, 0xFFFFFFFF);
        }
    }

    if(_isDebug) {
        for(auto body : _world->GetBodies()) {
            PolygonShape* shape = dynamic_cast<PolygonShape*>(body->shape);
            if(!shape && body->shape->GetType() == CIRCLE) {
                CircleShape* circle = static_cast<CircleShape*>(body->shape);
                Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation, 0xFFFFFF00);
                //Graphics::DrawFillCircle(body->position.x, body->position.y, circle->radius, color);
            } else if(shape) {
                Graphics::DrawPolygon(body->position.x, body->position.y, shape->worldVertices, 0xFFFFFF00);
            }
        }
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete _world;

    for(auto actor : _actors) {
        delete actor;
    }

    Graphics::CloseWindow();
}
