#pragma once
#include <memory>
#include <algorithm>

//non-resizable stack
template <typename element_type>
class StaticStack {

private:
	element_type* data;
	size_t reserved_size;
	size_t top_idx;

public:
	StaticStack(size_t reserved_size) {

		data = new element_type[reserved_size];
		this->reserved_size = reserved_size;
		this->top_idx = 0;
	}

	StaticStack(const StaticStack<element_type>& other) {

		data = new element_type[other.reserved_size];
		this->reserved_size = other.reserved_size;
		this->top_idx = other.top_idx;

		memcpy(this->data, other.data, other.reserved_size * sizeof(element_type));
	}

	StaticStack(StaticStack<element_type>&& other) noexcept {

		data = other.data;
		reserved_size = other.reserved_size;
		top_idx = other.top_idx;

		other.data = nullptr;
		other.reserved_size = 0;
		other.top_idx = 0;
	}

	StaticStack<element_type>& operator=(const StaticStack<element_type>& other) {

		if (this == &other)
			return *this;

		delete[] data;

		this->reserved_size = other.reserved_size;
		this->top_idx = other.top_idx;
		data = new element_type[reserved_size];

		std::copy(other.data, other.data + reserved_size, data);

		return *this;
	}


	StaticStack<element_type>& operator=(StaticStack<element_type>&& other) {

		if (this == &other)
			return *this;

		delete[] data;

		data = other.data;
		reserved_size = other.reserved_size;
		top_idx = other.top_idx;

		other.data = nullptr;
		other.reserved_size = 0;
		other.top_idx = 0;

		return *this;
	}

	~StaticStack() {

		delete[] data;
	}

	void push(element_type value) {
		data[top_idx] = value;
		top_idx++;
	}
	
	void pop() {
		top_idx--;
	}

	const element_type& top() {

		return data[top_idx - 1];
	}

	size_t size() {

		return top_idx;
	}

	size_t capacity() {

		return reserved_size;
	}
};