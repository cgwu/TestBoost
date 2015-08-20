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


#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
/*
#include <boost/log/core/core.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
*/
using namespace boost::asio;

namespace tcp_async{
io_service service;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
                  , boost::noncopyable {
    typedef talk_to_svr self_type;
    talk_to_svr(const std::string & message) 
      : sock_(service), started_(true), message_(message) {}
	// 启动连接远程端点
    void start(ip::tcp::endpoint ep) {
        sock_.async_connect(ep, MEM_FN1(on_connect,_1));	// 连接操作完成时调用 on_connect
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_svr> ptr;
	// 起始入口
    static ptr start(ip::tcp::endpoint ep, const std::string & message) {
        ptr new_(new talk_to_svr(message));
        new_->start(ep);
        return new_;
    }
    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
private:
	// 连接操作完成时调用 on_connect
    void on_connect(const error_code & err) {
        if ( !err)      do_write(message_ + "\n");		// 连接成功写入数据
        else            stop();
    }
	// on_read是一次读入完成时的回调
    void on_read_complete(const error_code & err, size_t bytes) {
		BOOST_LOG_TRIVIAL(trace) << "on_read_complete called:" << bytes;
        if ( !err) {
            std::string copy(read_buffer_, bytes - 1);
            std::cout << "server echoed our " << message_ << ": "
                      << (copy == message_ ? "OK" : "FAIL") << std::endl;
        }
        stop();
    }
    // 写入操作完成时调用on_write
    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }
    void do_read() {
        async_read(sock_, buffer(read_buffer_), 
                   MEM_FN2(is_read_complete,_1,_2), MEM_FN2(on_read_complete,_1,_2));	// read_complete判断是否读入完成；on_read是完成时的回调
    }
    void do_write(const std::string & msg) {
        if ( !started() ) return;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some( buffer(write_buffer_, msg.size()), 
                                MEM_FN2(on_write,_1,_2));		// 写入操作完成时调用on_write
    }
	// 判断读入是否完成,每读入一个字节都会调用一次： 0完成，1未完成.
    size_t is_read_complete(const boost::system::error_code & err, size_t bytes) {
		BOOST_LOG_TRIVIAL(trace) << "is_read_complete called:" << bytes;
        if ( err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string message_;
};

void init_log()
{
	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace sinks = boost::log::sinks;
	namespace keywords = boost::log::keywords;
	
	/*
	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace expr = boost::log::expressions;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;
	namespace keywords = boost::log::keywords;

	BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
	BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
	BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
	BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

	logging::add_common_attributes();

    logging::add_file_log
    (
        keywords::file_name = "asio_client_%N.log",                                        
        keywords::rotation_size = 10 * 1024 * 1024,                                   
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0) , 
        //keywords::format = "[%TimeStamp%]: %Message%"    
		keywords::format = (
			expr::stream << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " " <<
				line_id << " [" << severity << "] " << expr::smessage
		)
    );

	
	  // setup console log
	logging::add_console_log (
		std::clog,
		keywords::filter = severity >= logging::trivial::trace,
		keywords::format = (
			expr::stream << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " " <<
				line_id << " [" << severity << "] " << expr::smessage
		)
	);
	*/
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::trace
    );
/*
BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
BOOST_LOG_TRIVIAL(info) << "An informational severity message";
BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
BOOST_LOG_TRIVIAL(error) << "An error severity message";
BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
*/
}

}

int main_tcp_async_echo_client(int argc, char* argv[]) {
	using namespace tcp_async;
	init_log();
    // connect several clients
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
	//ip::tcp::endpoint ep( ip::address::from_string("192.168.10.8"), 8001);
    char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
    for ( char ** message = messages; *message; ++message) {
        talk_to_svr::start(ep, *message);
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    service.run();

	_getch();
	return 0;
}
