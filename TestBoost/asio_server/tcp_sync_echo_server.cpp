#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#include <conio.h>
#endif
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

namespace tcp_sync{
io_service service;

// 完成返回0；未完成返回1
size_t read_complete(char * buff, const error_code & err, size_t bytes){
	if(err) return 0;
	// 判断buff中是否已包含\n结束符
	bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
	return found ? 0 : 1;
}

void handle_connections(){
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
	char buff[1024];
	std::cout<< "tcp sync server starting..."<<std::endl;
	while(true){
		ip::tcp::socket sock(service);
		acceptor.accept(sock);
		int bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
		std::string msg(buff,bytes);

		ip::tcp::endpoint ep_client = sock.remote_endpoint();
		std::cout<<ep_client.address().to_string() << ":" << ep_client.port() <<" says -> [" << msg <<"]" << std::endl;
		
		sock.write_some(buffer(msg));
		sock.close();
	}
}


// test methods
void test_find(){
	char data[] = "0123456789\n";
	int len = strlen(data);
	//The find() algorithm looks for an element matching val between start and end. 
	//If an element matching val is found, the return value is an iterator that points to that element. 
	//Otherwise, the return value is an iterator that points to end. 
	bool found = std::find(data,data +len, '\r') < data + len;
	std:: cout<< std::boolalpha << found << std::noboolalpha << std::endl;

	std::string copy(data,4);
	std::cout<< copy << std::endl;
	// std::cout << std::hex << (int)ch << std::dec << ":" << ch << std::endl;
}

}
int main_tcp_sync(){
	tcp_sync::handle_connections();

	//tcp_sync::test_find();
	//_getch();
	return 0;
}
