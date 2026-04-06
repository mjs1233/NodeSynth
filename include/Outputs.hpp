#pragma once
#include <cinttypes>
#include <vector>
#include "BufferPool.hpp"

using type_id_t = uint32_t;
struct OutputHeader {

	type_id_t type_id;
	OutputHeader(type_id_t type_id) : type_id(type_id) {

	}

	virtual ~OutputHeader() = default;
};

struct RealtimeSample : public OutputHeader {


	BufferPool<float>::id_type transfer_buffer_id;
	RealtimeSample() : OutputHeader(0),transfer_buffer_id(0xFFFF) {}
	RealtimeSample(type_id_t id) : OutputHeader(id),transfer_buffer_id(0xFFFF) {}
};

struct FloatParam : public OutputHeader {
	float data;

	FloatParam() : OutputHeader(0), data(0) {}
	FloatParam(type_id_t id) : OutputHeader(id), data(0) {}
};

struct Trigger : public OutputHeader {
	uint32_t trigger_offset;
	
	Trigger() : OutputHeader(0), trigger_offset(0) {}
	Trigger(type_id_t id) : OutputHeader(id), trigger_offset(0) {}
};

struct VoidOutput : OutputHeader {

	VoidOutput() : OutputHeader(0) {}
	VoidOutput(type_id_t id) : OutputHeader(id) {}

};
