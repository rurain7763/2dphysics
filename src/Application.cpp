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
    
    _anchor = { Graphics::Width() / 2.0f, 30.0f };
    for(int i = 0; i < 10; i++) {
        Particle* bob = new Particle( Graphics::Width() / 2, _anchor.y + _restLength * i, 2.0);
        bob->radius = 10;
        _chain.push_back(bob);
        _particles.push_back(bob);
    }

    Vec2 startDotPos = {100, 100};
    Vec2 dotPosition[] = {
        startDotPos, 
        {startDotPos.x + _softBodyRestLength, startDotPos.y}, 
        {startDotPos.x + _softBodyRestLength, startDotPos.y + _softBodyRestLength}, 
        {startDotPos.x, startDotPos.y + _softBodyRestLength}
    };
    for(int i = 0; i < 4; i++) {
        Particle* dot = new Particle(dotPosition[i].x, dotPosition[i].y, 1.0);
        dot->radius = 10;
        _softBody[i] = dot;
        _particles.push_back(dot);
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
        Vec2 drag = Force::GenerateDragForce(*particle, 0.002);
        particle->AddForce(drag);

        Vec2 weight(0.f, particle->mass * 9.8f * PIXELS_PER_METER);
        particle->AddForce(weight);
    }

    // chain 제어
    Vec2 spring = Force::GenerateSpringForce(*_chain[0], _anchor, _restLength, _k);
    _chain[0]->AddForce(spring);
    for(int i = 1; i < _chain.size(); i++) {
        Particle& a = *_chain[i]; 
        Particle& b = *_chain[i - 1];
        spring = Force::GenerateSpringForce(a, b, _restLength, _k);
        a.AddForce(spring);
        b.AddForce(-spring);
    }
    _chain.back()->AddForce(_pushForce);

    // softbody 제어
    for(int i = 0; i < 4; i++) {
        int prevIdx = i - 1 >= 0 ? i - 1 : 3;
        int nextIdx = i + 1 < 4 ? i + 1 : 0;
        int middleIdx = i < 2 ? i + 2 : i - 2;

        Particle& a = *_softBody[i];
        Particle& b = *_softBody[prevIdx];
        Particle& c = *_softBody[nextIdx];
        Particle& d = *_softBody[middleIdx];

        spring = Force::GenerateSpringForce(a, b, _softBodyRestLength, _softBodyK);
        a.AddForce(spring);
        b.AddForce(-spring);

        spring = Force::GenerateSpringForce(a, c, _softBodyRestLength, _softBodyK);
        a.AddForce(spring);
        c.AddForce(-spring);

        spring = Force::GenerateSpringForce(a, d, _softBodyRestLength, _softBodyK);
        a.AddForce(spring);
        d.AddForce(-spring);
    }
    _softBody[0]->AddForce(_pushForce);

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
    // 체인 그리기
    Graphics::DrawFillCircle(_anchor.x, _anchor.y, 5, 0xff0000ff);

    Graphics::DrawLine(_anchor.x, _anchor.y, _chain[0]->position.x, _chain[0]->position.y, 0xff000000);
    Graphics::DrawFillCircle(_chain[0]->position.x, _chain[0]->position.y, _chain[0]->radius, 0xFFFFFFFF);

    for(int i = _chain.size() - 1; i > 0; i--) {
        Particle& a = *_chain[i];
        Particle& b = *_chain[i - 1];

        Graphics::DrawLine(a.position.x, a.position.y, b.position.x, b.position.y, 0xff000000);
        Graphics::DrawFillCircle(a.position.x, a.position.y, a.radius, 0xFFFFFFFF);
    }

    // soft body 그리기
    for(int i = 0; i < 4; i++) {
        int prevIdx = i - 1 >= 0 ? i - 1 : 3;
        int nextIdx = i + 1 < 4 ? i + 1 : 0;

        Particle& a = *_softBody[i];
        Particle& b = *_softBody[prevIdx];
        Particle& c = *_softBody[nextIdx];

        Graphics::DrawLine(a.position.x, a.position.y, b.position.x, b.position.y, 0xff000000);
        Graphics::DrawLine(a.position.x, a.position.y, c.position.x, c.position.y, 0xff000000);

        Graphics::DrawFillCircle(a.position.x, a.position.y, a.radius, 0xFFFFFFFF);
    }

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