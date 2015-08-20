// TestBoost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "pyinit.hpp"

using namespace std;
using namespace boost;
//using namespace boost::gregorian;

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, 这是回调!" << std::endl;
}

void print1(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
  if (*count < 5)
  {
    std::cout << *count << std::endl;
    ++(*count);

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
    t->async_wait(boost::bind(print1,
          boost::asio::placeholders::error, t, count));
  }
}

/*
class printer
{
public:
  printer(boost::asio::io_service& io)
    : timer_(io, boost::posix_time::seconds(1)),
      count_(0)
  {
    timer_.async_wait(boost::bind(&printer::print, this));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print()
  {
    if (count_ < 5)
    {
      std::cout << count_ << std::endl;
      ++count_;

      timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
      timer_.async_wait(boost::bind(&printer::print, this));
    }
  }

private:
  boost::asio::deadline_timer timer_;
  int count_;
};
*/


class printer
{
public:
  printer(boost::asio::io_service& io)
    : strand_(io),
      timer1_(io, boost::posix_time::seconds(1)),
      timer2_(io, boost::posix_time::seconds(1)),
      count_(0)
  {
    timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
    timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print1()
  {
    if (count_ < 10)
    {
		//	Sleep(1000);  
      std::cout << "Timer 1: " << count_ << std::endl;
      ++count_;

      timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
      timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
    }
  }

  void print2()
  {
    if (count_ < 10)
    {
		//Sleep(1000);  
      std::cout << "Timer 2: " << count_ << std::endl;
	  
      ++count_;

      timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
      timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
    }
  }

private:
  boost::asio::io_service::strand strand_;
  boost::asio::deadline_timer timer1_;
  boost::asio::deadline_timer timer2_;
  int count_;
};

int _tmain_TestBoost(int argc, _TCHAR* argv[])
{
	/*
	using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
	*/

	/*
	std::string line;
    boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

    while (std::cin)
    {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
            std::cout << matches[2] << std::endl;
    }
	*/

	/*
	pyinit init;
 
    object i(10);
    i = 10 * i;
    cout<<extract<int>(i)<<endl;

	PyRun_SimpleString("for x in range(1,10): print(x);\n");
 
    PyRun_SimpleString("print(\"Hello china\")");
	*/
	//boost::asio::io_service io;

	// boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	/*
	t.wait();	//同步等待
	cout << "Message:"<<endl;
	std::cout << "Hello, 中国!" << std::endl;
	*/
	/*
	t.async_wait(&print);	//异步等待
	cout<< "before run." <<endl;
	io.run();
	cout << "after run." << endl;
	*/
	/*
	int count = 0;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
	t.async_wait(boost::bind(print1,
		boost::asio::placeholders::error, &t, &count));
	io.run();

	std::cout << "Final count is " << count << std::endl;
	*/
	/*
	printer p(io);
	io.run();
	*/

	boost::asio::io_service io;
	printer p(io);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
	io.run();
	t.join();

	_getch();
	return 0;
}

