#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <print>
#include <optional>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"


#include "Appstate.hpp"

#include "AudioManager.hpp"

#include "SceneHandler.hpp"

class App {
public:
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SceneHandler scene_handler;

public:
    App();
    void init();
    void run();
    void terminate();
    ~App();



};