#pragma once
#include <SDL3/SDL.h>

#include "Input.hpp"

template <typename T>
class Widget 
{
public:
    SDL_FRect bounds{};

public:
    Widget(const SDL_FRect& bounds) : bounds(bounds) {};

    void render(SDL_Renderer* renderer) 
    {
        static_cast<T*>(this)->renderImpl(renderer);
    }

    void update(const Input& input) 
    {
        static_cast<T*>(this)->updateImpl(input);
    }
};
