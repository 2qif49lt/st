#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>
#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace sys = boost::system; 
namespace ip = boost::asio::ip;

using udp = boost::asio::ip::udp;
using udp_msg_handler = std::function<void(udp::endpoint, char*, uint32_t len)>;

class udp_server {
	static constexpr uint32_t BUFF_SIZE = 1024;
	using context_guard = typename asio::executor_work_guard<asio::io_context::executor_type>;
	asio::io_context _ioc;
	std::unique_ptr<context_guard> _guard;
	udp::endpoint _ep;
	udp::socket _sock;
	bool _working = false;  
	udp_msg_handler _handler;
	std::mutex _mux;
public:
	udp_server() : _sock(_ioc) {};
	udp_server(uint16_t port, const std::string& addr = "") : _sock(_ioc){
		if (addr == "" || addr == "INADDR_ANY") {
	         		_ep = udp::endpoint(udp::v4(), port);
        		} else {
	        		_ep = udp::endpoint(ip::make_address(addr), port);
		}
	}
	~udp_server() {
		if (_working) {
			stop();
		}
	}

	udp_server(const udp_server&) = delete;
	udp_server& operator=(const udp_server&) = delete;

	int listen_serve(udp_msg_handler fn, uint16_t port, const std::string& addr = "", 
		uint32_t n = std::thread::hardware_concurrency()) {
		if (addr == "" || addr == "INADDR_ANY") {
	         		_ep = udp::endpoint(udp::v4(), port);
        		} else {
	        		_ep = udp::endpoint(ip::make_address(addr), port);
		}
		return  listen_serve(std::move(fn), n);
	}
	    // run 开启n个工作线程
	int listen_serve(udp_msg_handler fn, uint32_t n = std::thread::hardware_concurrency()) {
		_handler = std::move(fn);

		int ret = 0;

		ret = init();
		if (ret != 0) {
			return ret;
		}

		_working = true;

		std::mutex mux;
		std::condition_variable cv;
		uint32_t thread_num = 0;

		std::vector<std::shared_ptr<std::thread>> vec;
		for (uint32_t i = 0; i != n; ++i) {
			vec.push_back(std::make_shared<std::thread>([this, &mux, &cv, &thread_num](){
				{
					std::lock_guard<std::mutex> lk(mux);
					++thread_num;
				}
				
				constexpr uint32_t len = udp_server::BUFF_SIZE;
				char data_buff[len];
				udp::endpoint peer_ep;
				do_recv(peer_ep, data_buff, len);

				_ioc.run(); 
				
				{
					std::lock_guard<std::mutex> lk(mux);
					--thread_num;
				}
				cv.notify_all();
			}));
		}
		for (auto& pt : vec) {
			pt->detach();
		}

		std::this_thread::sleep_for(std::chrono::seconds(1)); // for ioc.run thread switch;

		while(1) {
			std::unique_lock<std::mutex> lk(mux);
			if (cv.wait_for(lk, std::chrono::milliseconds(500), [&thread_num]{return thread_num == 0;}) == true) {
				break;
			}
		}
		_working = false;
		stop();
		return 0;
	}

	void  do_recv(udp::endpoint& peer_ep, char* data, uint32_t len) {
		std::lock_guard<std::mutex> lk(_mux);
		_sock.async_receive_from(asio::buffer(data, len), peer_ep, [this, &peer_ep, data, len](sys::error_code ec, std::size_t  bytes_transferred) {
			if (!ec && bytes_transferred > 0) {
				_handler(peer_ep, data, bytes_transferred);
			}
			
			do_recv(peer_ep, data, len);
		});
	}

	void  send(udp::endpoint peer_ep, const char* data, uint32_t len) {
		std::lock_guard<std::mutex> lk(_mux);
		_sock.async_send_to(asio::buffer(data, len), peer_ep, [](sys::error_code , std::size_t  ){});
	}

	void  stop() {
		 _guard->reset();
	}

	void shutdown() {
		_ioc.stop();
 	}

 private:
 	int init() {
		sys::error_code ec;

		_ioc.restart();
		_guard = std::unique_ptr<context_guard>( new context_guard(asio::make_work_guard(_ioc)));
		_sock.open(_ep.protocol(), ec);
		if (ec) {
		    return 1;
		}
		asio::socket_base::reuse_address option(true);
		_sock.set_option(option);
		_sock.bind(_ep, ec);
		if (ec) {
		    return 2;
		}
		return 0;
	}
};

#endif // UDP_SERVER_H_