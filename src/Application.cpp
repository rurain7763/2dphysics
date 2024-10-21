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
    
    _anchor = { Graphics::Width() / 2.0f, 30.0f };

    Particle* bob = new Particle( Graphics::Width() / 2, _anchor.y + _restLength, 10);
    bob->radius = 10;
    _particles.push_back(bob);

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
        particle->AddForce(_pushForce);

        Vec2 drag = Force::GenerateDragForce(*particle, 0.001);
        particle->AddForce(drag);

        Vec2 weight(0.f, particle->mass * 9.8f * PIXELS_PER_METER);
        particle->AddForce(weight);
    }

    Vec2 spring = Force::GenerateSpringForce(*_particles[0], _anchor, _restLength, _k);
    _particles[0]->AddForce(spring);

    for(auto particle : _particles) {
        particle->Integrate(deltaTime);
    }

    const int windowWidth = Graphics::Width();
    const int windowHeight = Graphics::Height();
    for(auto particle : _particles) {
        if(particle->position.x > windowWidth - particle->radius) {
            particle->position.x = windowWidth - particle->radius;
            particle->velocity.x *= -0.9;
        } else if(particle->position.x < particle->radius) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;
        }

        if(particle->position.y > windowHeight - particle->radius) {
            particle->position.y = windowHeight - particle->radius;
            particle->velocity.y *= -0.9;
        } else if(particle->position.y < particle->radius) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

#if false
    for(auto particle : _particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
#else
    Graphics::DrawFillCircle(_anchor.x, _anchor.y, 5, 0xff0000ff);
    Graphics::DrawLine(_anchor.x, _anchor.y, _particles[0]->position.x, _particles[0]->position.y, 0xff000000);
    Graphics::DrawFillCircle(_particles[0]->position.x, _particles[0]->position.y, _particles[0]->radius, 0xFFFFFFFF);
#endif

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