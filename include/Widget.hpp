#pragma once
#include <SDL3/SDL.h>

#include "Input.hpp"

template <typename T>
class Widget 
{
public:
    SDL_FRect bounds{};
    bool active;

public:
    Widget(const SDL_FRect& bounds, bool active) : bounds(bounds), active(active) {};

    void render(SDL_Renderer* renderer) const 
    {
        if(active)
            static_cast<const T*>(this)->renderImpl(renderer);
    }

    void update(const Input& input)
    {
        if (active)
            static_cast<T*>(this)->updateImpl(input);
    }
};
