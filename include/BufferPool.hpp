#pragma once
#include <concepts>
#include <vector>
#include <memory>
#include <print>
#include <stdexcept>
#include <expected>
#include <optional>
#include <StaticStack.hpp>

/* static size buffer pool*/
template <typename element_type>
class BufferPool {
public:
	
	using id_type = size_t;

	enum class error_code {

		buffer_full
	};

private:
	element_type* data = nullptr;
	//bit 연산으로 대체는 너무 오버 엔지니어링 같음.....
	bool* alloc_state = nullptr;
	size_t reserved_block = 0;
	size_t block_used = 0;
	size_t element_per_block = 0;

	StaticStack<size_t> free_list;

public:
	BufferPool(size_t element_per_block, size_t block_count) :
		element_per_block(element_per_block),
		block_used(0)
	{

		allocate(block_count);
	}

	BufferPool(const BufferPool<element_type>& other) {

		this->element_per_block = other.element_per_block;
		this->block_used = other.block_used;

		allocate(other.reserved_block);
		
		std::memcpy(data, other.data, sizeof(element_type) * reserved_block * element_per_block);
	}

	BufferPool(BufferPool<element_type>&& other) noexcept {

		this->data = other.data;
		this->reserved_block = other.reserved_block;
		this->element_per_block = other.element_per_block;
		this->block_used = other.block_used;

		other.data = nullptr;
		other.alloc_state = nullptr;
	}

	~BufferPool() {

		if(data != nullptr)
			delete[] data;
	}

	void init_free_list() {

		free_list = StaticStack<size_t>(reserved_block);

		for (size_t idx = 0; idx < reserved_block; idx++) {

			free_list.push(idx);
		}

	}

	inline element_type& get(id_type id, size_t idx) {
		
		element_type* block = data + (id * element_per_block);
		return block[idx];
	}

	inline element_type& safe_get(id_type id, size_t idx) {

		if (id >= reserved_block) {

			throw std::invalid_argument("invaild id");
		}

		if (alloc_state[id] == false) {

			throw std::invalid_argument("invaild id");
		}

		element_type* block = data + (id * element_per_block);
		return block[idx];
	}

	std::expected<id_type,error_code> alloc_block() {

		std::optional<size_t> opt_idx = find_free_space();

		if (!opt_idx.has_value())
			return std::unexpected(error_code::buffer_full);

		size_t idx = opt_idx.value();

		alloc_state[idx] = true;

		block_used++;

		return idx;
	}

	void return_block(id_type id) {

		if (id >= reserved_block)
			return;

		if (alloc_state[id] == false)
			return;

		block_used--;
	}

private:

	/* 
		This function reallocates every element without copying the original. 
		It is a destructive operation
	*/
	void allocate(size_t block_count) {
		
		if (data != nullptr) {

			delete[] data;
		}

		data = new element_type[element_per_block * block_count];
		if (data == nullptr)
			throw std::runtime_error("heap allocation fail");

		alloc_state = new bool[block_count];
		if (alloc_state == nullptr)
			throw std::runtime_error("heap allocation fail");

		std::memset(alloc_state, 0, block_count * sizeof(bool));

		reserved_block = block_count;
	}

	std::optional<size_t> find_free_space() {

		if (block_used >= reserved_block) {

			return std::nullopt;
		}

		for (size_t idx = block_search_start, search_count = 0; search_count < reserved_block; idx = (idx + 1) % reserved_block) {
			
			if (!alloc_state[idx])
				return idx;
		}

		return std::nullopt;
	}
};