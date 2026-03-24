#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <print>
#include <queue>
#include <deque>
#include <optional>
#include <atomic>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include "CircularQueue.hpp"
#include "Debug.hpp"
#include "ProcessorNode.hpp"
#include "NodeContainer.hpp"
#include "PlayContext.h"
#include "TMP_tuple_gen.hpp"

template <typename ...output_container>
int AudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *userData );


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
	size_t sample_per_update;
	CircularQueue<float> audio_queue;

	std::atomic<mode_type> mode;

	uint32_t connect_start_node_id;
	PlayContext play_context;


	NodeContainer node_container;
	using base_pointer = NodeContainer::base_pointer;


public:

	AudioManager_t() :
		connect_start_node_id(0),
		play_context(256,256,48000,AudioLIB::RTAUDIO),
		audio_queue(48000),
		sample_per_update() {
	
		mode.store(mode_type::idle);
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

		std::optional<base_pointer> node = node_container.get_base(idx);

		if (!node.has_value()) {
			return std::nullopt;
		}

		return std::static_pointer_cast<T>(node);
	}

	void update_node() {
		
		std::vector<uint32_t> update_seq;
		node_container.calc_node_update_seq(update_seq);
	}
	

	void play() {

		std::deque<uint32_t> update_seq;
		node_container.calc_node_update_seq(update_seq);

		LOG(std::print("SEQ ==========================\n"));
		for (const auto& node_id : update_seq) {
			LOG(std::print("{} ", node_id))
		}
		LOG(std::print("==============================\n"));

		play_context.update_seq = update_seq;
		play_context.transfer_pool = BufferPool<float>(512,256);

		if ( play_context.rtaudio.openStream( &play_context.rt_parameters, NULL, RTAUDIO_FLOAT32, play_context.sample_rate,
					   &play_context.buffer_frames, &AudioCallback<output_container...>, this )){
			return;
		}

		if ( play_context.rtaudio.startStream() == RTAUDIO_SYSTEM_ERROR) {
			LOG(std::cout << play_context.rtaudio.getErrorText() << std::endl)

			if ( play_context.rtaudio.isStreamOpen())
				play_context.rtaudio.closeStream();

			return;
		}

		LOG(std::print("START!\n"))
		mode.store(mode_type::play);
	}

	void stop() {

		mode.store(mode_type::idle);

		if ( play_context.rtaudio.isStreamRunning() )
			play_context.rtaudio.stopStream();


		if ( play_context.rtaudio.isStreamOpen())
			play_context.rtaudio.closeStream();

	}

	void ui_update() {

		for (uint32_t idx = 0; idx < node_container.size(); idx++) {

			std::optional<base_pointer> node =  node_container.get_base(idx);
			
			if (!node.has_value())
				continue;

			NodeUIUpdateResult result = node.value()->update_ui();


			if (mode.load() == mode_type::idle && result.output_pin_clicked) {

				LOG(std::println("start_conn {}", idx))
				connect_start_node_id = idx;
				mode.store(mode_type::connect);
			}
			else if (mode.load() == mode_type::connect && result.clicked_input_pin.has_value()) {

				LOG(std::println("end_conn {}", idx))
				uint32_t connect_end_node_id = idx;

				mode.store(mode_type::idle);

				bool connection_result = node_container.connect(connect_start_node_id, connect_end_node_id,result.clicked_input_pin.value());

				if (connection_result) {
					LOG(std::println("connect success {} -> {}", connect_start_node_id , connect_end_node_id))
				}
			}
			draw_connections(node.value());
		}

	}

	void audio_update(
		void* input, void* output,
		unsigned int nBufferFrames,
		double streamTime,
		RtAudioStreamStatus status ) {

		if (mode.load() == mode_type::play) {

			for (const auto& node_id : play_context.update_seq) {
				std::optional<base_pointer> node = node_container.get_base(node_id);
				if (!node.has_value())
					return;
				node.value()->process();
			}
			
			//LOG(std::print("add: {} tot: {}\n", additional_amount, total_amount))
		}
	}

	void seperate_audio_buffer() {


	}


private:
	void draw_connections(base_pointer node) {

		if (node == nullptr) {
			return;
		}

		std::deque<OutputRouter::Connection> connections = node->output_router().next();

		for (const auto& conn : connections) {
			//LOG(std::println("draw connection {} ---> {}:{}", node->id(),conn.next_ptr->id(), conn.id))
		}
	}


};


template <typename ...output_container>
int AudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *userData ) {

	LOG(std::println("AUDIO CALLBACK : {}",nBufferFrames))
	static_cast<AudioManager_t<output_container...>*>(userData)->audio_update(inputBuffer,outputBuffer,nBufferFrames,streamTime,status);
	return 0;

}


using AudioManager = AudioManager_t<RealtimeSample, FloatParam, Trigger>;