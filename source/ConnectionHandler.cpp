#pragma once
#include "ConnectionHandler.hpp"

ConnectionHandler::ConnectionHandler(const ConnectionData& output_data) :
	output_data(output_data),
	connection_data_count(0),
	reference_count(0) {

}

void ConnectionHandler::connect(uint32_t target_id, uint32_t connection_id) {

	connections.push_back(connection{ target_id,connection_id });
	inc_ref();
}

void ConnectionHandler::disconnect(uint32_t target_id, uint32_t connection_id) {

	disconnect(connection{ target_id,connection_id });
}


void ConnectionHandler::disconnect(connection conn) {

	for (uint32_t idx = 0; idx < connections.size(); idx++) {

		if (connections[idx] == conn) {

			connections.erase(connections.begin() + idx);
			dec_ref();
			return;
		}
	}
}

size_t ConnectionHandler::connection_size() const {

	return connections.size();
}

ConnectionHandler::connection ConnectionHandler::get_connection(uint32_t idx) {

	return connections[idx];
}


const std::deque<ConnectionHandler::connection>& ConnectionHandler::get_connections() {
	return connections;
}

void ConnectionHandler::add_input(uint32_t id, const ConnectionData& conn_data) {

	connection_data_count++;
	input_data[id] = conn_data;
}


void ConnectionHandler::id_list(std::vector<uint32_t>& container) {

	auto keys = std::views::keys(input_data);
	container = std::vector<uint32_t>(keys.begin(), keys.end());
}

std::optional<ConnectionData> ConnectionHandler::get_input(uint32_t id) {

	if (input_data.find(id) != input_data.end())
		return input_data[id];

	return std::nullopt;
}

const ConnectionData& ConnectionHandler::get_output() {

	return output_data;
}


void ConnectionHandler::inc_ref() {

	reference_count++;
}

void ConnectionHandler::dec_ref() {

	if (reference_count != 0)
		reference_count--;
	else
		std::runtime_error("reference count is already zero!");
}

uint32_t ConnectionHandler::ref_count() const {

	return reference_count;
}
