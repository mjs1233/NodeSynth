#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <print>
#include <queue>
#include <deque>
#include <optional>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include "Debug.hpp"
#include "ProcessorNode.hpp"
#include "NodeContainer.hpp"
#include "TMP_tuple_gen.hpp"

template <typename ...output_container>
static void SDLCALL AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount);



template <typename ...output_container>
class AudioManager_t {
public:
private:
	enum class mode_type {
		idle,
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

	uint32_t connect_start_node_id;

	using node_list_t = std::vector<std::shared_ptr<ProcessNodeBase>>;
	node_list_t nodes;

	NodeContainer node_container;
	using base_pointer = NodeContainer::base_pointer;


public:

	AudioManager_t() : 
		stream(nullptr),
		mode(mode_type::idle),
		connect_start_node_id(0) {
	
	}


	AudioManager_t(uint32_t channel, uint32_t sample_rate) {

		if (!SDL_Init(SDL_INIT_AUDIO)) {
			LOG(std::print("error init sdl audio subsystem : {}", SDL_GetError()))
			exit(1);
		}

		audio_spec.format = SDL_AUDIO_F32;
		audio_spec.channels = channel;
		audio_spec.freq = sample_rate;

		stream = SDL_OpenAudioDeviceStream(
			SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, 
			&audio_spec, 
			AudioCallback<output_container...>, 
			this
		);
		
		if (!stream) {
			LOG(std::print("error open audio device stream : {}", SDL_GetError()))
		}
		SDL_ResumeAudioStreamDevice(stream);
	}


	template <ProcessNodeTrait T, typename ...Args>
	requires std::is_constructible<T,Args...>::value 
	&& (std::is_same<typename T::output_container,output_container>::value || ... )
	std::optional<size_t> add(Args&& ...args) {

		uint32_t id = node_container.add(std::make_shared<T>(std::forward<Args>(args)...));
		
		std::optional<base_pointer> new_node = node_container.get_base(id);

		if (!new_node.has_value()) {
			return std::nullopt;
		}

		new_node.value()->set_id(id);

		return id;
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
		
		std::vector<uint32_t> update_seq;
		node_container.calc_node_update_seq(update_seq);
	}
	

	void ui_update() {

		for (uint32_t idx = 0; idx < node_container.size(); idx++) {
			std::optional<base_pointer> node =  node_container.get_base(idx);
			
			if (!node.has_value())
				continue;

			bool connect_start = false;
			bool connect_end = false;
			ConnectionData conn_data = node.value()->update_ui(connect_start, connect_end);
			if (mode == mode_type::idle && connect_start) {

				LOG(std::println("start_conn {}", idx))
				connect_start_node_id = idx;
				mode = mode_type::connect;
			}
			else if (mode == mode_type::connect && connect_end) {

				LOG(std::println("end_conn {}", idx))
				uint32_t connect_end_node_id = idx;

				mode = mode_type::idle;

				bool result = node_container.connect(connect_start_node_id, connect_end_node_id, conn_data);

				if (result) {
					LOG(std::println("connect success {} -> {}", connect_start_node_id ,connect_end_node_id))
				}
			}
			draw_connections(node.value());
		}

	}

	void audio_update(int additional_amount, int total_amount) {
		LOG(std::print("add: {} tot: {}\n", additional_amount, total_amount))
	}

private:
	void draw_connections(base_pointer node) {

		if (node == nullptr) {
			return;
		}

		std::deque<ConnectionHandler::connection> connections = node->connection().get_connections();

		for (const auto& conn : connections) {
			LOG(std::println("draw connection {} ---> {}:{}", node->id(), conn.target_id, conn.connection_id))
		}
	}


};


template <typename ...output_container>
static void __cdecl AudioCallback(
	void* user_data, 
	SDL_AudioStream* audio_stream, 
	int additional_amount, 
	int total_amount) {

	reinterpret_cast<AudioManager_t<output_container...>*>(user_data)->audio_update(additional_amount,total_amount);
}


using AudioManager = AudioManager_t<realtime_sample_output, const_float_output, trigger_output>;