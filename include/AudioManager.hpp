#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <print>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include "ProcessorNode.hpp"

template <AudioProcessNode ...processnode>
static void SDLCALL AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount);


template <AudioProcessNode ...processnode>
class AudioManager
{
public:

private:
	std::vector<float> audio_buffer;
	SDL_AudioStream* stream;
	SDL_AudioSpec audio_spec;
	std::tuple<processnode...> nodes;

public:

	AudioManager() : stream(nullptr) {}

	AudioManager(uint32_t channel, uint32_t sample_rate) {
		if (!SDL_Init(SDL_INIT_AUDIO)) {
			std::print("error init sdl audio subsystem : {}", SDL_GetError());
			exit(1);
		}

		audio_spec.format = SDL_AUDIO_F32;
		audio_spec.channels = channel;
		audio_spec.freq = sample_rate;

		stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, AudioCallback<processnode...>, this);
		
		if (!stream) {
			std::print("error open audio device stream : {}", SDL_GetError());
		}
		SDL_ResumeAudioStreamDevice(stream);
	}

	void update(int additional_amount, int total_amount) {
		std::print("add: {} tot: {}\n", additional_amount, total_amount);
	}
};

template <AudioProcessNode ...processnode>
static void __cdecl AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount) {
	reinterpret_cast<AudioManager<processnode...>*>(user_data)->update(additional_amount,total_amount);
}