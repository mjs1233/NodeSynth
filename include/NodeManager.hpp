#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <print>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include "AudioComponent.hpp"
#include "AudioComponentHandle.hpp"

class NodeManager
{
public:
	std::vector<AudioComponentHandle> audio_components;


public:
	template <typename T>
	requires requires { std::declval<T>().process(std::vector<float>(),std::declval<std::vector<float>&>() ); }
	size_t addAudioComponent(std::shared_ptr<T>& w)
	{
		audio_components.push_back(makeAudioComponentHandle(w));
		w->id = audio_components.size() - 1;
		return w->id;
	}
	
};