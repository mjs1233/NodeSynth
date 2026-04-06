#include "NodeContainer.hpp"


NodeContainer::NodeContainer() {

}

bool NodeContainer::connect(
	uint32_t start_id,
	uint32_t end_id,
	uint32_t port_id
) {
	std::optional<base_pointer> start_node = get_base(start_id);
	std::optional<base_pointer> end_node = get_base(end_id);

	if (!start_node.has_value() || !end_node.has_value()) {
		return false;
	}

	return connect(start_node.value(), end_node.value(), port_id);
}

bool NodeContainer::connect(
	const base_pointer& start_node, 
	const base_pointer& end_node, 
	uint32_t port_id) {


	// 확인 과정이 너무 복잡함.... 다른 함수로 분리가 필요함.

	// Validate the nodes and prevent circular connections.
	if (start_node != nullptr && end_node != nullptr && (start_node->id() != end_node->id())) {

		type_id_t output_type_id = start_node->output_router().get_output_type_id();
		std::optional<InputPort> input_port = end_node->input_router().get_port(port_id);
		
		// Verify that the port ID is valid.
		if (!input_port.has_value())
			return false;

		// Ensure that the port type matches.
		if (input_port.value().type_id != output_type_id)
			return false;

		// Check whether the input is already connected to the output.
		if (input_port.value().allocated && input_port.value().connected_node_id ==  start_node->id())
			return false;
		

		// Verify that there is no circular connection between nodes



		if (!end_node->input_router().add_prev(start_node->id(), port_id))
			return false;

		start_node->output_router().add_next(end_node, port_id);

		return true;
	}
	return false;
}


void NodeContainer::make_ref_count_list(std::vector<int32_t>& container) {

	container.resize(nodes.size());

	for (uint32_t idx = 0; idx < nodes.size(); idx++) {

		if (nodes[idx] == nullptr) {
			container[idx] = 0;
			continue;
		}

		//for advanced id system
		container[nodes[idx]->id()] = nodes[idx]->input_router().get_ref_count();
	}
}

void NodeContainer::find_start_nodes(std::queue<uint32_t>& container) {

	for (const auto& node : nodes) {

		if (node == nullptr) {
			continue;
		}

		if (node->input_router().get_ref_count() == 0) {

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