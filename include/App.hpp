#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <print>
#include <optional>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"


#include "UI.hpp"
#include "Appstate.hpp"

#include "AudioManager.hpp"
#include "AudioSystem.hpp"


class App {
public:
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
	App() {

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::print("Error: SDL_Init(): {}\n", SDL_GetError());
            exit(1);
        }

        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        window = SDL_CreateWindow("Node Synth", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);

        if (window == nullptr) {
            std::print("Error: SDL_CreateWindow(): {}\n", SDL_GetError());
            exit(1);
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        SDL_SetRenderVSync(renderer, 1);
        if (renderer == nullptr) {
            std::print("Error: SDL_CreateRenderer(): {}\n", SDL_GetError());
            exit(1);
        }
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);
	}

	void run() {


        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        bool done = false;
        while (!done) {

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                    done = true;
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                    done = true;
            }


            if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
                SDL_Delay(10);
                continue;
            }

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();



            ImGui::Render();

            ImGuiIO& io = ImGui::GetIO(); (void)io;

            SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
            SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            SDL_RenderClear(renderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
            SDL_RenderPresent(renderer);
        }
	}

	~App() {

        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

	}



};