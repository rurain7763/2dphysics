#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Body.h"
#include "Physics/Contact.h"
#include "Actor.h"

#include <vector>

class Application {
    private:
        bool _running = false;
        bool _isDebug = false;
        int _prevFrameTime;

        std::vector<Body*> _bodies;
        std::vector<Actor*> _actors;
        std::vector<Contact> _contacts;

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