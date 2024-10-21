#include "Application.h"
#include "Physics/Constant.h"
#include "Physics/Force.h"

#include <iostream>

bool Application::IsRunning() {
    return _running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    _running = Graphics::OpenWindow();

    Body* body = new Body(CircleShape(50), Graphics::Width() / 2, 50, 1.0);
    _bodies.push_back(body);
    
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

                if (event.key.keysym.sym == SDLK_RIGHT) {
                    _pushForce.x = 50.f * PIXELS_PER_METER; 
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    _pushForce.x = -50.f * PIXELS_PER_METER;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    _pushForce.y = -50.f * PIXELS_PER_METER;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    _pushForce.y = 50.f * PIXELS_PER_METER;
                } 
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    _pushForce.x = 0; 
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    _pushForce.x = 0;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    _pushForce.y = 0;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    _pushForce.y = 0;
                } 
                break;
            case SDL_MOUSEBUTTONDOWN:
                int msX, msY;
                SDL_GetMouseState(&msX, &msY);
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

        body->AddTorque(20);
    }

    for(auto body : _bodies) {
        body->IntegrateLinear(deltaTime);
        body->IntegrateAngular(deltaTime);
    }

    const int windowWidth = Graphics::Width();
    const int windowHeight = Graphics::Height();
    for(auto body : _bodies) {
        if(body->shape->GetType() == CIRCLE) {
            CircleShape* circle = static_cast<CircleShape*>(body->shape);
            
            if(body->position.x > windowWidth - circle->radius) {
                body->position.x = windowWidth - circle->radius;
                body->velocity.x *= -0.9;
            } else if(body->position.x < circle->radius) {
                body->position.x = circle->radius;
                body->velocity.x *= -0.9;
            }

            if(body->position.y > windowHeight - circle->radius) {
                body->position.y = windowHeight - circle->radius;
                body->velocity.y *= -0.9;
            } else if(body->position.y < circle->radius) {
                body->position.y = circle->radius;
                body->velocity.y *= -0.9;
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
        if(body->shape->GetType() == CIRCLE) {
            CircleShape* circle = static_cast<CircleShape*>(body->shape);
            Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation, 0xFFFFFFFF);
        }
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : _bodies) {
        delete particle;
    }
    Graphics::CloseWindow();
}