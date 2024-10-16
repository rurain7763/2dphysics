#include "Application.h"
#include "Physics/Constant.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
    return _running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    _running = Graphics::OpenWindow();

    _liquid.x = 0;
    _liquid.y = Graphics::Height() / 2.0;
    _liquid.w = Graphics::Width();
    _liquid.h = Graphics::Height() / 2.0;

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
                Particle* particle = new Particle(msX, msY, 1.0);
                _particles.push_back(particle);
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

    for(auto particle : _particles) {
        Vec2 weight = Vec2(0, particle->mass * 9.8f * PIXELS_PER_METER);
        particle->AddForce(weight);

        particle->AddForce(_pushForce);

        if(particle->position.y > _liquid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.1);
            particle->AddForce(drag);
        }
    }

    for(auto particle : _particles) {
        particle->Integrate(deltaTime);
    }

    const int windowWidth = Graphics::Width();
    const int windowHeight = Graphics::Height();
    for(auto particle : _particles) {
        if(particle->position.x > windowWidth - PARTICLE_RADIUS) {
            particle->position.x = windowWidth - PARTICLE_RADIUS;
            particle->velocity.x *= -0.9;
        } else if(particle->position.x < PARTICLE_RADIUS) {
            particle->position.x = PARTICLE_RADIUS;
            particle->velocity.x *= -0.9;
        }

        if(particle->position.y > windowHeight - PARTICLE_RADIUS) {
            particle->position.y = windowHeight - PARTICLE_RADIUS;
            particle->velocity.y *= -0.9;
        } else if(particle->position.y < PARTICLE_RADIUS) {
            particle->position.y = PARTICLE_RADIUS;
            particle->velocity.y *= -0.9;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    Graphics::DrawFillRect(_liquid.x + _liquid.w / 2, _liquid.y + _liquid.h / 2, _liquid.w, _liquid.h, 0xFF6e3713);

    for(auto particle : _particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, PARTICLE_RADIUS, 0xFFFFFFFF);
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : _particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}