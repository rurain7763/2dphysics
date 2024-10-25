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

    Body* floor = new Body(BoxShape(2000, 100), Graphics::Width() / 2, Graphics::Height() - 150, 0.0);
    floor->restitution = 0.1f;
    Body* leftWall = new Body(BoxShape(100, 2000), 50, Graphics::Height() / 2, 0.0);
    leftWall->restitution = 0.1f;
    Body* rightWall = new Body(BoxShape(100, 2000), Graphics::Width() - 50, Graphics::Height() / 2, 0.0);
    rightWall->restitution = 0.1f;
    Body* b = new Body(BoxShape(200, 200), Graphics::Width() / 2, Graphics::Height() / 2, 0.0);
    b->restitution = 0.5f;
    b->friction = 0.1f;
    b->rotation = 0.5f;
    _bodies.push_back(floor);
    _bodies.push_back(leftWall);
    _bodies.push_back(rightWall);
    _bodies.push_back(b);

    Actor* actor = new Actor(b, "assets/crate.png");
    _actors.push_back(actor);
    
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
                    }

                    shapeFlag = (shapeFlag + 1) % 2;
                    _bodies.push_back(body);    
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

    
    for(auto body : _bodies) {
        Vec2 drag = Force::GenerateDragForce(*body, 0.002);
        body->AddForce(drag);

        Vec2 weight(0.f, body->mass * 9.8f * PIXELS_PER_METER);
        body->AddForce(weight);
    }

    for(auto body : _bodies) {
        body->isColliding = 0;
        body->UpdateBody(deltaTime);
    }

    for(int i = 0; i < _bodies.size() - 1; i++) {
        for(int j = i + 1; j < _bodies.size(); j++) {
            Body* a = _bodies[i];
            Body* b = _bodies[j];

            Contact contact;
            if(CollisionDetection::IsCollision(a, b, contact)) {
                contact.ResolveCollision();

                a->isColliding++;
                b->isColliding++;

                if(_isDebug) {
                    _contacts.push_back(contact);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF000030);

    for(auto body : _bodies) {
        uint32_t color;
        if(body->isColliding) {
            color = 0xFF0000FF;
        } else {
            color = 0xFFFFFFFF;
        }
        PolygonShape* shape = dynamic_cast<PolygonShape*>(body->shape);
        if(!shape && body->shape->GetType() == CIRCLE) {
            CircleShape* circle = static_cast<CircleShape*>(body->shape);
            Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation, color);
            //Graphics::DrawFillCircle(body->position.x, body->position.y, circle->radius, color);
        } else if(shape) {
            Graphics::DrawPolygon(body->position.x, body->position.y, shape->worldVertices, color);
        }
    }

    for(auto actor : _actors) {
        if(actor->body->shape->GetType() == BOX) {
            BoxShape* box = static_cast<BoxShape*>(actor->body->shape);
            Graphics::DrawTexture(actor->body->position.x, actor->body->position.y, box->width, box->height, actor->body->rotation, actor->texture);
        } else if(actor->body->shape->GetType() == CIRCLE) {
            CircleShape* circle = static_cast<CircleShape*>(actor->body->shape);
            Graphics::DrawTexture(actor->body->position.x, actor->body->position.y, circle->radius * 2, circle->radius * 2, actor->body->rotation, actor->texture);
        }
    }

    for(auto& contact : _contacts) {
        Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFFFF00);
        Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFFFF00);
        Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFFFF00);
    }
    _contacts.clear();

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : _bodies) {
        delete particle;
    }

    for(auto actor : _actors) {
        delete actor;
    }

    Graphics::CloseWindow();
}
