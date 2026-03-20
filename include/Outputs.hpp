#pragma once
#include <cinttypes>
#include <vector>

using type_id_t = uint32_t;
struct OutputHeader {

	type_id_t type_id;
	OutputHeader(type_id_t type_id) : type_id(type_id) {

	}

	virtual ~OutputHeader() = default;
};


struct RealtimeSample : public OutputHeader {
	std::vector<float> samples;

	RealtimeSample(type_id_t id) : OutputHeader(id) {}
};

struct FloatParam : public OutputHeader {
	float data;

	FloatParam(type_id_t id) : OutputHeader(id), data(0) {}
};

struct Trigger : public OutputHeader {
	uint32_t trigger_offset;

	Trigger(type_id_t id) : OutputHeader(id), trigger_offset(0) {}
};

