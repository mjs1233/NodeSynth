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

#include "ProcessorNode.hpp"
#include "TMP_tuple_gen.hpp"

template <typename ...output_container>
static void SDLCALL AudioCallback(void* user_data, SDL_AudioStream* audio_stream, int additional_amount, int total_amount);


///  The class implementation has become excessively long...
///  Refactoring is needed.
///
///  It seems necessary to create a dedicated class for 
/// node connect, disconnect, and sequence analysis.
///  
///  -->  NodeContainer...?? ........tlqkf :(


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

public:

	AudioManager_t() : 
		stream(nullptr),
		mode(mode_type::idle),
		connect_start_node_id(0) {
	
	}


	AudioManager_t(uint32_t channel, uint32_t sample_rate) {

		if (!SDL_Init(SDL_INIT_AUDIO)) {
			std::print("error init sdl audio subsystem : {}", SDL_GetError());
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
			std::print("error open audio device stream : {}", SDL_GetError());
		}
		SDL_ResumeAudioStreamDevice(stream);
	}


	template <ProcessNodeTrait T, typename ...Args>
	requires std::is_constructible<T,Args...>::value 
	&& (std::is_same<typename T::output_container,output_container>::value || ... )
	size_t add(Args&& ...args) {

		nodes.push_back(std::make_shared<T>(std::forward<Args>(args)...));
		nodes[nodes.size() - 1]->set_id(nodes.size() - 1);
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

	std::optional<std::shared_ptr<ProcessNodeBase>> get_base(size_t idx) {

		if (idx >= nodes.size())
			return std::nullopt;

		if (nodes[idx] == nullptr)
			return std::nullopt;

		return nodes[idx];
	}

	void update_node() {
		
		std::vector<uint32_t> update_seq;
		calc_node_update_seq(update_seq);
	}
	

	void ui_update() {

		for (uint32_t idx = 0; idx < nodes.size(); idx++) {

			if (nodes[idx] == nullptr) {
				continue;
			}

			bool connect_start = false;
			bool connect_end = false;
			ConnectionData conn_data = nodes[idx]->update_ui(connect_start,connect_end);
			if (mode == mode_type::idle && connect_start) {

				std::print("start_conn {}", idx);
				connect_start_node_id = idx;
				mode = mode_type::connect;
			}
			else if (mode == mode_type::connect && connect_end) {

				std::print("end_conn {}", idx);
				uint32_t connect_end_node_id = idx;

				mode = mode_type::idle;

				connect(connect_start_node_id, connect_end_node_id, conn_data);
			}
			draw_connections(idx);
		}

	}

	void audio_update(int additional_amount, int total_amount) {
		std::print("add: {} tot: {}\n", additional_amount, total_amount);
	}

private:
	void draw_connections(uint32_t idx) {

		if (nodes[idx] == nullptr) {
			return;
		}

		std::vector<ProcessNodeBase::connection> connections = nodes[idx]->next();

		for (const auto& conn : connections) {
			std::println("CONNECTED {} ---> {}:{}", idx, conn.target_id, conn.connection_id);
		}

	}

	void connect(uint32_t start,  uint32_t end, const ConnectionData& end_data) {

		std::optional<std::shared_ptr<ProcessNodeBase>> start_node = get_base(start);
		std::optional<std::shared_ptr<ProcessNodeBase>> end_node = get_base(end);

		if ((start_node.has_value() && end_node.has_value()) && start != end) {

			ConnectionData start_data = start_node.value()->output_data();;
			if (start_data.type != end_data.type)
				return;

			
			for (const auto& conn : start_node.value()->next()) {

				if (conn.connection_id == end_data.id) {
					return;
				}
			}

			// << check another node is already connected to end connection >>
			// 
			// 
			// It seems like this should be implemented as a bidirectional graph structure
			// rather than using a ref_count approach.
			// The overhead during connection lookup is far too heavy.

			// Otherwise, I'll just implement it first and run performance tests afterward.

			start_node.value()->add_next(end_node.value()->id(),end_data.id);
			end_node.value()->inc_ref();
			
		}
	}


	//need T::push_back()-able container
	template<typename T>
	requires std::same_as<typename std::remove_cvref_t<T>::value_type, uint32_t>
	void calc_node_update_seq(T& container) {
		
		std::queue<uint32_t> start_nodes;
		find_start_nodes(start_nodes);

		std::queue<uint32_t> next_nodes;

		std::vector<int32_t> ref_counts;
		make_ref_count_list(ref_counts);

		while(!start_nodes.empty()) {
			
			std::optional<std::shared_ptr<ProcessNodeBase>> base_ptr = get_base(start_nodes.front());
			if (!base_ptr.has_value())
				continue;

			const std::vector<uint32_t>& connected_nodes = base_ptr.value()->next();
			
			for (const auto& conn_node : connected_nodes) {
				
				std::optional<std::shared_ptr<ProcessNodeBase>> conn_ptr = get_base(conn_node);
				if (!conn_ptr.has_value())
					continue;

				next_nodes.push(conn_ptr.value()->id());
			}
		}

		while (!next_nodes.empty()) {
			uint32_t front_id = next_nodes.front();

			ref_counts[front_id]--;
			if (ref_counts[front_id] == 0) {

				std::optional<std::shared_ptr<ProcessNodeBase>> base_ptr = get_base(front_id);
				if (!base_ptr.has_value())
					continue;

				for (const auto& next : base_ptr.value()->next()) {

					next_nodes.push(next);
				}

				container.push_back(front_id);
			}
		}
		
	}

	void make_ref_count_list(std::vector<int32_t>& container) {

		container.resize(nodes.size());

		for (uint32_t idx = 0; idx < nodes.size(); idx++) {

			if (nodes[idx] == nullptr) {
				container[idx] = 0;
				continue;
			}

			//for advanced id system
			container[nodes[idx]->id()] = nodes[idx]->ref_count();
		}
	}

	//T::push_back()-able container
	template <typename T>
	requires std::same_as<typename std::remove_cvref_t<T>::element_type, uint32_t>
	void find_start_nodes(T& container) {
		
		for (const auto& node : nodes) {

			if (node == nullptr) {
				continue;
			}

			if (node->ref_count() == 0) {
				container.push_back(node->id());
			}
		}
	}

};


template <typename ...output_container>
static void __cdecl AudioCallback(void* user_data, 
	SDL_AudioStream* audio_stream, 
	int additional_amount, 
	int total_amount) {

	reinterpret_cast<AudioManager_t<output_container...>*>(user_data)->audio_update(additional_amount,total_amount);
}


using AudioManager = AudioManager_t<realtime_sample_output, const_float_output, trigger_output>;