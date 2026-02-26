#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "WidgetHandle.hpp"


class UIManager 
{
public:
    std::vector<WidgetHandle> widgets;

    UIManager() = default;

    template <typename W>
    void addWidget(const std::shared_ptr<W>& w)
    {
        widgets.push_back(makeHandle(w));
    }

    void render(SDL_Renderer* renderer) 
    {
        for (auto& w : widgets)
            w.render(renderer);
    }

    void update(const Input& input) 
    {
        for (auto& w : widgets)
            w.update(input);
    }
};
