#ifndef BRO_COMM_MANAGER_H
#define BRO_COMM_MANAGER_H

#include <broker/endpoint.hh>
#include <broker/message_queue.hh>
#include <memory>
#include <string>
#include <map>
#include "Reporter.h"
#include "iosource/IOSource.h"

namespace comm {

// TODO: documentation

// Manages various forms of communication between peer Bro processes
// or possibly between different parts of a single Bro process.
class Manager : public iosource::IOSource {
public:

	bool InitPreScript();

	bool InitPostScript();

	bool Listen(uint16_t port, const char* addr = nullptr);

	bool Connect(std::string addr, uint16_t port,
	             std::chrono::duration<double> retry_interval);

	bool Disconnect(const std::string& addr, uint16_t port);

	bool Print(std::string topic, std::string msg, const Val* flags);

	bool Event(std::string topic, broker::message msg, int flags);
	bool Event(std::string topic, const RecordVal* args, const Val* flags);

	bool AutoEvent(std::string topic, const Val* event, const Val* flags);

	bool AutoEventStop(const std::string& topic, const Val* event);

	RecordVal* MakeEventArgs(const val_list* args);

	bool SubscribeToPrints(std::string topic_prefix);

	bool UnsubscribeToPrints(const std::string& topic_prefix);

	bool SubscribeToEvents(std::string topic_prefix);

	bool UnsubscribeToEvents(const std::string& topic_prefix);

private:

	int get_flags(const Val* flags);

	// IOSource interface overrides:
	void GetFds(iosource::FD_Set* read, iosource::FD_Set* write,
	            iosource::FD_Set* except) override;

	double NextTimestamp(double* local_network_time) override;

	void Process() override;

	const char* Tag() override
		{ return "Comm::Manager"; }

	std::unique_ptr<broker::endpoint> endpoint;
	std::map<std::pair<std::string, uint16_t>, broker::peering> peers;
	std::map<std::string, broker::message_queue> print_subscriptions;
	std::map<std::string, broker::message_queue> event_subscriptions;

	int send_flags_self_idx;
	int send_flags_peers_idx;
	int send_flags_unsolicited_idx;

	VectorType* vector_of_data_type;
};

} // namespace comm

extern comm::Manager* comm_mgr;

#endif // BRO_COMM_MANAGER_H
