#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Particle.h"

#include <vector>

class Application {
    private:
        bool _running = false;
        int _prevFrameTime;

        std::vector<Particle*> _particles;
        Vec2 _pushForce = Vec2(0, 0);

        Vec2 _anchor;
        float _k = 100;
        float _restLength = 400;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif