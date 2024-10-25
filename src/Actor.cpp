#include "Actor.h"

#include "Graphics.h"

#include <assert.h>

Actor::Actor(Body* body, const std::string& texturePath) {
    this->body = body;

    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    assert(surface != nullptr);

    texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
    SDL_FreeSurface(surface);
}

Actor::~Actor() {
    SDL_DestroyTexture(texture);
}