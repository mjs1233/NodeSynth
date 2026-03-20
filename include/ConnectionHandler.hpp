#pragma once

#include <print>
#include <vector>
#include <functional>
#include <memory>
#include <tuple>
#include <deque>
#include <utility>
#include <optional>
#include <map>
#include <type_traits>

#include "ConnectionData.hpp"
#include "ProcessorNode.hpp"


struct connection {
	uint32_t target_id;
	uint32_t connection_id;
	bool operator==(const connection& V_right) {
		return (V_right.target_id == target_id && V_right.connection_id == connection_id);
	}

	bool operator!=(const connection& V_right) {
		return (V_right.target_id != target_id || V_right.connection_id != connection_id);
	}
};
class ConnectionHandler {

public:


private:
	std::deque<connection> connections;
	std::map<uint32_t, ConnectionData> input_data;
	ConnectionData output_data;
	size_t connection_data_count;//for vector::reserve() operation
	uint32_t reference_count;
public:
	explicit ConnectionHandler(const ConnectionData& output_data);


	void connect(uint32_t target_id, uint32_t connection_id);

	void disconnect(uint32_t target_id, uint32_t connection_id);
	void disconnect(connection conn);

	size_t connection_size() const;

	connection get_connection(uint32_t idx);
	const std::deque<connection>& get_connections();

	void add_input(uint32_t id, const ConnectionData& conn_data);
	std::optional<ConnectionData> get_input(uint32_t id);
	const ConnectionData& get_output();

	void id_list(std::vector<uint32_t>& list);

	uint32_t ref_count() const;

private:
	void inc_ref();
	void dec_ref();
};


