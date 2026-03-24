#pragma once
#include <deque>
#include <print>
#include <cinttypes>
#include <RtAudio.h>
#include "Debug.hpp"
#include "BufferPool.hpp"

struct PlayContext {

	std::deque<uint32_t> update_seq;
	BufferPool<float> sample_pool;
	RtAudio audio;

	RtAudio::StreamParameters parameters;
	uint32_t sample_rate;
	uint32_t buffer_frames;

	PlayContext(uint32_t buffer_frames,uint32_t sample_block_count,uint32_t sample_rate) :
	sample_pool(BufferPool<float>(buffer_frames, sample_block_count)) {
		init_RtAudio(sample_rate,buffer_frames);
	}

	void init_RtAudio(uint32_t sample_rate, uint32_t buffer_frames) {

		RtAudio::Api api = audio.getCurrentApi();

		parameters.deviceId = audio.getDefaultOutputDevice();
		parameters.nChannels = 2;
		parameters.firstChannel = 0;
		this->sample_rate = sample_rate;
		this->buffer_frames = buffer_frames;

		LOG(std::print("Audio API : {}" , std::to_underlying(api)))
	}

};