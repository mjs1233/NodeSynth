#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <print>

#include "Button.hpp"
#include "NodeUI.hpp"
#include "Widget.hpp"
#include "WidgetHandle.hpp"
#include "UIManager.hpp"

#include "Appstate.hpp"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;


void test_callback(Button* self, Button::callback_type event_) {}





//SDL init callback
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("node-synth", "1.0","node-synth_10");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("node-synth", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    *appstate = new Appstate;
    Appstate& state = *static_cast<Appstate*>(*appstate);
    state.input_mode = InputMode::SDL_WINDOW;


    //button test
    Button button({ 0,0,100,30 }, test_callback, SDL_Color{ 0xFF,0x00,0x00,0xFF });
    state.ui.addWidget(std::make_shared<Button>(std::move(button)));

    //node test
    NodeUI nodeui;
    state.ui.addWidget(std::make_shared<NodeUI>(std::move(nodeui)));
    return SDL_APP_CONTINUE;
}

//SDL event callback
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    Appstate& state = *static_cast<Appstate*>(appstate);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    state.input.handle_input_event(*event);
    

    return SDL_APP_CONTINUE;
}


//SDL update callback
SDL_AppResult SDL_AppIterate(void* appstate)
{
    Appstate& state = *static_cast<Appstate*>(appstate);

    state.ui.update(state.input);


    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    state.ui.render(renderer);

    SDL_RenderPresent(renderer);
    //std::print("================\n");
    //std::print("{} {}\n", state.input.mouse_x, state.input.mouse_y);
    state.input.clear_update();
    return SDL_APP_CONTINUE;  
}

//SDL terminate callback
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}