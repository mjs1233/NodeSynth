#include "NodeContainer.hpp"


NodeContainer::NodeContainer() {

}

bool NodeContainer::connect(
	uint32_t start_id,
	uint32_t end_id,
	const ConnectionData& end_connection_data
) {
	std::optional<base_pointer> start_node = get_base(start_id);
	std::optional<base_pointer> end_node = get_base(end_id);

	if (!start_node.has_value() || !end_node.has_value()) {
		return false;
	}

	return connect(start_node.value(), end_node.value(), end_connection_data);
}

bool NodeContainer::connect(
	const base_pointer& start_node, 
	const base_pointer& end_node, 
	const ConnectionData& end_connection_data) {

	if (start_node != nullptr && end_node != nullptr && start_node->id() != end_node->id()) {

		ConnectionData start_data = start_node->output_data();;
		if (start_data.type != end_connection_data.type)
			return false;


		for (const auto& conn : start_node->next()) {

			if (conn.connection_id == end_connection_data.id) {
				return false;
			}
		}

		// TODO: Add logic to verify whether a connection between nodes already exists

		start_node->add_next(end_node->id(), end_connection_data.id);
		end_node->inc_ref();
		return true;
	}
}


void NodeContainer::make_ref_count_list(std::vector<int32_t>& container) {

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

void NodeContainer::find_start_nodes(std::queue<uint32_t>& container) {

	for (const auto& node : nodes) {

		if (node == nullptr) {
			continue;
		}

		if (node->ref_count() == 0) {
			container.push(node->id());
		}
	}
}


std::optional<NodeContainer::base_pointer> NodeContainer::get_base(size_t idx) {

	if (idx >= nodes.size())
		return std::nullopt;

	if (nodes[idx] == nullptr)
		return std::nullopt;

	return nodes[idx];
}


size_t NodeContainer::add(const base_pointer& node) {

	nodes.push_back(node);
	nodes[nodes.size() - 1]->set_id(nodes.size() - 1);
	//TODO) add advanced id management system
	return nodes.size() - 1;
}


size_t NodeContainer::size() {

	return nodes.size();
}