#ifndef UDP_HANDLER_H_
#define UDP_HANDLER_H_


#include <atomic>
#include <boost/asio.hpp>

static std::atomic<uint32_t>  _recv_packet_counter;
class udp_handler {
	using udp = boost::asio::ip::udp;
	
public:
	void  operator()(udp::endpoint peer_ep, char* data, uint32_t len)  {
		_recv_packet_counter.fetch_add(1, std::memory_order_relaxed);
	}
};

#endif // UDP_HANDLER_H_