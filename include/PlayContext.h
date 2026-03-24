#pragma once
#include <deque>
#include <print>
#include <cinttypes>
#include <RtAudio.h>
#include "Debug.hpp"
#include "BufferPool.hpp"


enum class AudioLIB {

	SDL_AUDIO,
	RTAUDIO
};

struct PlayContext {

	std::deque<uint32_t> update_seq;
	BufferPool<float> sample_pool;
	AudioLIB audio_lib;


	RtAudio rtaudio;

	RtAudio::StreamParameters rt_parameters;


	uint32_t sample_rate;
	uint32_t buffer_frames;

	PlayContext(uint32_t buffer_frames, uint32_t sample_block_count, uint32_t sample_rate, AudioLIB audio_lib) :
	sample_pool(BufferPool<float>(buffer_frames, sample_block_count)),
	audio_lib(audio_lib) {

		if (audio_lib == AudioLIB::RTAUDIO) {

			init_RtAudio_Param(sample_rate,buffer_frames);
		}
	}

	[[deprecated]]
	void init_SDL_Param(uint32_t sample_rate) {

	}


	void init_RtAudio_Param(uint32_t sample_rate, uint32_t buffer_frames) {

		RtAudio::Api api = rtaudio.getCurrentApi();

		rt_parameters.deviceId = rtaudio.getDefaultOutputDevice();
		rt_parameters.nChannels = 2;
		rt_parameters.firstChannel = 0;
		this->sample_rate = sample_rate;
		this->buffer_frames = buffer_frames;

		LOG(std::print("Audio API : {}" , std::to_underlying(api)))
	}

};