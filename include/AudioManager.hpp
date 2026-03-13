#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <print>
#include <deque>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include "ProcessorNode.hpp"
#include "TMP_tuple_gen.hpp"

template<typename ...output_containers>
static void SDLCALL AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount);


template <typename ...output_containers> 
class AudioManager {
public:
private:
	enum class mode_type {
		edit,
		connect,
		play,
		probe
		
	};


public:

private:
	std::vector<float> audio_buffer;
	SDL_AudioStream* stream;
	SDL_AudioSpec audio_spec;
	mode_type mode;

	using node_list_t = std::vector<std::shared_ptr<ProcessNodeBase>>;
	node_list_t nodes;

public:

	AudioManager() : 
		stream(nullptr),
		mode(mode_type::play){
	
	}


	AudioManager(uint32_t channel, uint32_t sample_rate) {

		if (!SDL_Init(SDL_INIT_AUDIO)) {
			std::print("error init sdl audio subsystem : {}", SDL_GetError());
			exit(1);
		}

		audio_spec.format = SDL_AUDIO_F32;
		audio_spec.channels = channel;
		audio_spec.freq = sample_rate;

		stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, AudioCallback<output_containers...>, this);
		
		if (!stream) {
			std::print("error open audio device stream : {}", SDL_GetError());
		}
		SDL_ResumeAudioStreamDevice(stream);
	}


	template <ProcessNodeTrait T, typename ...Args>
	requires std::is_constructible<T,Args...>::value 
	&& (std::is_same<typename T::ouput_tag,output_containers>::value || ... )
	size_t add(Args&& ...args) {

		nodes.push_back(std::make_shared<T>(std::forward<Args>(args)...));
		//TODO) add advanced id management system
		return nodes.size() - 1;
	}

	template <ProcessNodeTrait T>
	std::optional<std::shared_ptr<T>> get(size_t idx) {

		if (idx >= nodes.size())
			return std::nullopt;

		if (nodes[idx] == nullptr)
			return std::nullopt;

		return std::static_pointer_cast<T>(nodes[idx]);
	}



	void update_node() {
		
	}
	

	void ui_update() {


	}

	void audio_update(int additional_amount, int total_amount) {
		std::print("add: {} tot: {}\n", additional_amount, total_amount);
	}

private:

	void calc_node_update_seq() {

		//find roots
		for (const auto& node : nodes) {

		}

	}

};

template <typename ...output_container>
static void __cdecl AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount) {
	reinterpret_cast<AudioManager<output_container...>*>(user_data)->audio_update(additional_amount,total_amount);
}