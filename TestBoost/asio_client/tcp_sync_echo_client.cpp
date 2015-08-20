#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#include <conio.h>
#endif

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using boost::system::error_code;

namespace tcp_sync{
io_service service;

// 完成返回0;否则返回1
size_t read_complete(char * buf, const error_code & err, size_t bytes){
	if(err) return 0;
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);

void sync_echo(std::string msg){
	msg += "\n";
	ip::tcp::socket sock(service);
	sock.connect(ep);
	sock.write_some(buffer(msg));
	char buf[1024];
	int bytes = read(sock, buffer(buf), boost::bind(read_complete, buf, _1, _2));
	std::string copy(buf, bytes - 1);
	msg = msg.substr(0, msg.size() - 1);
	std::cout << "server echoed our " << msg << ":"
		<< (copy == msg ? "OK" : "FAIL") << std::endl;
	sock.close();
}

}

int main_tcp_sync(int argc, char *argv[]){
	char * messages[] = { "John says hi", "so does James",
						"Lucy just got home", "Boost.Asio if fun!", "我爱中国！", 0 };
	boost::thread_group threads;
	for( char ** msg = messages; *msg; ++msg){
		threads.create_thread( boost::bind(tcp_sync::sync_echo, *msg));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
	threads.join_all();
	_getch();
	return 0;
}