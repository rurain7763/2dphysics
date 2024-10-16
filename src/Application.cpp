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

    Particle* bigParticlee = new Particle(Graphics::Width() / 2, Graphics::Height() / 2, 500);
    bigParticlee->radius = 50;
    _particles.push_back(bigParticlee);

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

        Vec2 friction = Force::GenerateFrictionForce(*particle, 20);
        particle->AddForce(friction);
    }

    for(int i = 0; i < _particles.size(); i++) {
        for(int j = i + 1; j < _particles.size(); j++) {
            Particle& a = *_particles[i];
            Particle& b = *_particles[j];

            Vec2 attraction = Force::GenerateGravitationalForce(a, b, 100.0, 5, 100);
            a.AddForce(attraction);
            b.AddForce(-attraction);
        }
    }

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

    for(auto particle : _particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
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