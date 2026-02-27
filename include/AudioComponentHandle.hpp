#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <vector>

#include "Input.hpp"

struct AudioComponentHandle
{
    std::function<void(const std::vector<float>& in, std::vector<float>& out)> process;
};

template <typename A>
AudioComponentHandle makeAudioComponentHandle(std::shared_ptr<A>& audio_component)
{
    return {
        [audio_component](const std::vector<float>& in, std::vector<float>& out) mutable {  audio_component->process(in,out); }
        //[audio_component]()mutable {  audio_component->process(in,out); }
    };
}
