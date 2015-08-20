#ifdef WIN32
#define _WIN32_WINNT 0x0505
#include <stdio.h>
#include <conio.h>
#endif

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace udp_sync{
using namespace boost::asio;
io_service service;

//ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
ip::udp::endpoint ep(ip::address::from_string("192.168.10.8"), 8001);

void sync_echo(std::string msg){
	ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 0));
	sock.send_to(buffer(msg), ep);
	char buff[1024];
	ip::udp::endpoint sender_ep;
	int bytes = sock.receive_from(buffer(buff), sender_ep);
	std::string copy(buff, bytes);
	std::cout << "server echoed our " << msg << ": "
				<< (copy == msg ? "OK" : "FAIL" ) << std::endl;
	sock.close();
}
}

int main_udp_sync_echo_client(int argc, char * argv[]){
	using namespace udp_sync;
	char * messages[] = {
		"John says hi", "so does James", "Lucy just got Home", 0
	};
	boost::thread_group threads;
	for( char **msg = messages; *msg; ++ msg){
		threads.create_thread( boost::bind(sync_echo, *msg));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

	_getch();
	return 0;
}