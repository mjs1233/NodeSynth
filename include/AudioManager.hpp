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
class AudioManagerBase
{
public:

private:
	std::vector<float> audio_buffer;
	SDL_AudioStream* stream;
	SDL_AudioSpec audio_spec;
	std::tuple<std::deque<processnode...>> nodes;

public:

	AudioManagerBase() : stream(nullptr) {}

	AudioManagerBase(uint32_t channel, uint32_t sample_rate) {

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
	
	template<AudioProcessNode T, typename ...Args>
	requires (std::is_same<processnode, T>::value || ...) && std::constructible_from<T, Args...>
	void add_node(Args&& ...args) {
		std::get<T>(nodes).emplace_back(std::forward<Args>(args)...);
	}

	void audio_update(int additional_amount, int total_amount) {
		std::print("add: {} tot: {}\n", additional_amount, total_amount);
	}
};

template <AudioProcessNode ...processnode>
static void __cdecl AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount) {
	reinterpret_cast<AudioManagerBase<processnode...>*>(user_data)->audio_update(additional_amount,total_amount);
}