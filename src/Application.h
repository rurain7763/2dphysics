#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Body.h"

#include <vector>

class Application {
    private:
        bool _running = false;
        int _prevFrameTime;

        std::vector<Body*> _bodies;
        Vec2 _pushForce = Vec2(0, 0);

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