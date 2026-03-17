#pragma once
#include <optional>
#include <print>
#include <queue>

#include "ProcessorNode.hpp"

#include "ConnectionData.hpp"


template <typename T,typename Arg>
//이름 tlqkf.....
concept has_push_or_push_back =
	requires {
	std::declval<T>().push(std::declval<Arg>());
}
|| requires {
	std::declval<T>().push_back(std::declval<Arg>());
};



class NodeContainer {


public:
	using base_pointer = std::shared_ptr<ProcessNodeBase>;

private:
	std::vector<base_pointer> nodes;

public:

	NodeContainer();
	~NodeContainer() = default;

	bool connect(
		uint32_t start_id,
		uint32_t end_id,
		const ConnectionData& end_connection_data
	);

	bool connect(
		const base_pointer& start_node,
		const base_pointer& end_node,
		const ConnectionData& end_connection_data);


	std::optional<base_pointer> get_base(size_t idx);

	size_t add(const base_pointer& node);

	size_t size();

	template<typename T>
	requires std::same_as<typename std::remove_cvref_t<T>::value_type, uint32_t>
	&& has_push_or_push_back<T,uint32_t>
	void calc_node_update_seq(T& container) {

		std::queue<uint32_t> start_nodes;
		find_start_nodes(start_nodes);

		std::queue<uint32_t> next_nodes;

		std::vector<int32_t> ref_counts;
		make_ref_count_list(ref_counts);

		while (!start_nodes.empty()) {

			std::optional<base_pointer> base_ptr = get_base(start_nodes.front());
			if (!base_ptr.has_value())
				continue;

			const std::vector<uint32_t>& connected_nodes = base_ptr.value()->next();

			for (const auto& conn_node : connected_nodes) {

				std::optional<base_pointer> conn_ptr = get_base(conn_node);
				if (!conn_ptr.has_value())
					continue;

				next_nodes.push(conn_ptr.value()->id());
			}
		}

		while (!next_nodes.empty()) {

			uint32_t front_id = next_nodes.front();

			ref_counts[front_id]--;
			if (ref_counts[front_id] == 0) {

				std::optional<base_pointer> base_ptr = get_base(front_id);
				if (!base_ptr.has_value())
					continue;

				for (const auto& next : base_ptr.value()->next()) {

					next_nodes.push(next);
				}

				if constexpr (requires{ container.push(0); }) {

					container.push(front_id);
				}
				else if constexpr (requires{ container.push_back(0); }) {

					container.push_back(front_id);
				}

			}
		}

	}

private:


	void make_ref_count_list(std::vector<int32_t>& container);
	void find_start_nodes(std::queue<uint32_t>& container);





};