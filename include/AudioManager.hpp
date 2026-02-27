#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <print>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include "AudioComponent.hpp"
#include "AudioComponentHandle.hpp"
#include "NodeManager.hpp"

static void SDLCALL AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount);


// AudioManager manages the ordered execution of AudioComponents and routes audio output to AudioEndpoints.
class AudioManager
{
public:

private:
	std::vector<float> audio_buffer;
	SDL_AudioStream* stream;
	SDL_AudioSpec audio_spec;


	/*
		(init)
		make_shared<AudioComp.>(UI Manager,...) ==> AudioManager.addComp.(...), NodeManager.add() ==> 

	
	
	
	*/
	std::vector<AudioComponentHandle> audio_components;
public:

	AudioManager() : stream(nullptr) {}

	AudioManager(uint32_t channel, uint32_t sample_rate)
	{
		if (!SDL_Init(SDL_INIT_AUDIO)) {
			std::print("error init sdl audio subsystem : {}", SDL_GetError());
			exit(1);
		}

		audio_spec.format = SDL_AUDIO_F32;
		audio_spec.channels = channel;
		audio_spec.freq = sample_rate;

		stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, AudioCallback, this);
		if (!stream)
		{
			std::print("error open audio device stream : {}", SDL_GetError());
		}
		SDL_ResumeAudioStreamDevice(stream);
	}

	template <typename A>
	void addAudioComponent(std::shared_ptr<A>& w)
	{
		audio_components.push_back(makeAudioComponentHandle(w));
	}


	void update(int additional_amount, int total_amount)
	{
		std::print("add: {} tot: {}\n", additional_amount, total_amount);
	}
};

static void __cdecl AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount)
{
	reinterpret_cast<AudioManager*>(user_data)->update(additional_amount,total_amount);
}