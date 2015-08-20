#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace udp_sync{
using namespace boost::asio;
using namespace boost::posix_time;

io_service service;
void handle_connections(){
	char buff[1024];
	ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 8001));
	while(true){
		ip::udp::endpoint sender_ep;
		int bytes = sock.receive_from(buffer(buff), sender_ep);
		std::string msg(buff, bytes);
		std::cout << "server got msg:" << msg<< std::endl;
		sock.send_to(buffer(msg), sender_ep);
	}
}
}

int main_udp_sync_echo_server(int argc, char *argv[]){
	std::cout <<"udp sync server starting..."<< std::endl;
	udp_sync::handle_connections();
	return 0;
}