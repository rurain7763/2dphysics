#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Body.h"
#include "Physics/Contact.h"
#include "Actor.h"
#include "Physics/World.h"

#include <vector>

class Application {
    private:
        bool _running = false;
        bool _isDebug = false;
        int _prevFrameTime;

        World* _world;        
        std::vector<Actor*> _actors;

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