#ifndef ACTOR_H
#define ACTOR_H

#include "Physics/Body.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

struct Actor {
    Body* body;
    SDL_Texture* texture;  

    Actor(Body* body, const std::string& texturePath);
    ~Actor();
};

#endif