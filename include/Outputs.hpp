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

//이건 너무 ㅂㅅ같은 방법임.. 복사가 일어나잖아...ㅅㅂ 아마도 buffer pool 같은것을 극한으로 최적화해서 쓰는게 좋을듯
struct RealtimeSample : public OutputHeader {
	std::vector<float> samples;

	RealtimeSample() : OutputHeader(0) {}
	RealtimeSample(type_id_t id) : OutputHeader(id) {}
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

