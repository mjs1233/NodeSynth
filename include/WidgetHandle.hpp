#pragma once
#include <SDL3/SDL.h>
#include <functional>

#include "Input.hpp"

struct WidgetHandle 
{
    std::function<void(SDL_Renderer*)> render;
    std::function<void(const Input&)> update;
};

template <typename W>
WidgetHandle makeHandle(std::shared_ptr<W> widget)
{
    return {
        [widget](SDL_Renderer* renderer) mutable { widget->render(renderer); },
        [widget](const Input& input) mutable { widget->update(input); }
    };
}


#include "WidgetHandle.inl"
