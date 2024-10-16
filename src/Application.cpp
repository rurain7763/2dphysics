#include "Application.h"
#include "Physics/Constant.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    for(int i = 0; i < 10; i++) {
        Particle* particle = new Particle(50, i * 100, i + 1.0);
        particles.push_back(particle);
    }

    prevFrameTime = SDL_GetTicks();
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    int timeToWait = (prevFrameTime + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - prevFrameTime) / 1000.f;
    if(deltaTime > 0.016f) {
        deltaTime = 0.016f;
    }
    prevFrameTime = SDL_GetTicks();

    Vec2 wind = Vec2(0.2f * PIXELS_PER_METER, 0.f);
    for(auto particle : particles) {
        particle->AddForce(wind);
    }

    for(auto particle : particles) {
        Vec2 weight = Vec2(0, 9.8f * PIXELS_PER_METER);
        particle->AddForce(weight);
    }

    const int windowWidth = Graphics::Width();
    const int windowHeight = Graphics::Height();

    for(auto particle : particles) {
        particle->Integrate(deltaTime);
    }

    for(auto particle : particles) {
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

    for(auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, PARTICLE_RADIUS, 0xFFFFFFFF);
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}