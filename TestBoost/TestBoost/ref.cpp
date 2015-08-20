#include "stdafx.h"
using namespace std;

template<typename T>
struct summary{
	typedef void result_type;
	T sum;
	summary(T v = T()): sum(v){}
	void operator() (const T &x){
		sum += x;
	}
};
namespace test_ref{
class demo_class{
private:
	typedef boost::function<void(int)> func_t;
	func_t func;
	int n;
public:
	demo_class(int i):n(i){}
	
	template<typename CallBack>
	void accept(CallBack f){ func = f; }
	void run(){ if(!func.empty()) func(n); }
};

void call_back_func(int i){
	std::cout << "call back func:"  << i*2 << std::endl;
}

class call_back_obj{
private:
	int x;
public:
	call_back_obj(int i=0): x(i){}
	void operator() (int i){
		std::cout << "call back func:"  << i * x ++ << std::endl;
	}
};

class call_back_factory{
public:
	void call_back_func1(int i){
		std::cout << "call back factory func1:"  << i*2 << std::endl;
	}
	void call_back_func2(int i, int j){
		std::cout << "call back factory func2:"  << i*j*2 << std::endl;
	}
};

void ref_test_case(){
	demo_class dc(10);
	//dc.accept(call_back_func);
	//dc.run();
	/*
	call_back_obj cbo(2);
	dc.accept(boost::ref(cbo));
	dc.run();
	dc.run();
	*/
	call_back_factory cbf;
	dc.accept(boost::bind(&call_back_factory::call_back_func1,cbf,_1));
	dc.run();
	dc.accept(boost::bind(&call_back_factory::call_back_func2,cbf,_1,5));
	dc.run();
}

}

void slot1(){
	std::cout << "slot1" << std::endl;
}
void slot2(){
	std::cout << "slot2" << std::endl;
}
struct HelloWorld
{
  void operator()() const
  {
    std::cout << "Hello, World!" << std::endl;
  }
};

class do_not_copy //: boost::noncopyable
{
};

boost::optional<double> dao_shu(int x){
	return boost::optional<double>(x!=0, 1.0/x);
}

void print_string(string s){
	cout<<s<<",";
}

class A{
public:
	A(){ cout<< "A ctor." <<endl;}
	~A(){ cout<< "A dtor." <<endl;}
	void dosth(){ cout<<this<< " A do something." <<endl;}
	void dosthconst() const{ cout<<this<< " A do something const." <<endl;}
};

int _tmain_ref(int argc, _TCHAR* argv[])
{
	using namespace boost::assign;
	std::vector<int> v = (list_of(1),3,5,7,9);
	summary<int> s;
	boost::function<void(const int &)> func(boost::ref(s));
	std::for_each(v.begin(),v.end(),func);
	std::cout<<s.sum<<std::endl;
	std::cout<<"---------------"<<std::endl;

	test_ref::ref_test_case();
	std::cout<<"---------------"<<std::endl;

	HelloWorld hello;
	boost::signals2::signal<void()> sig;
	sig.connect(hello);
	sig.connect(&slot1);
	sig.connect(&slot2, boost::signals2::at_front);
	sig();

	using namespace boost::uuids::detail;
	sha1 sha;
	char *szMsg ="a short message";
	sha.process_block(szMsg,szMsg + strlen(szMsg));

	unsigned int digest[5]={0,};
	sha.get_digest(digest);
	std::cout<<std::setfill('0');
	for(int i=0;i<5;++i){
		std::cout<<std::hex<<std::setw(8)<<digest[i];
	}
	std::cout<<std::endl;

	int idata = 0;
	for(int j=0;j<4;++j){
		for(int i=0;i<10;++i){
			std::cout<<i;
		}
	}
	
	std::cout<<std::endl << "test 0:" << std::hex /*<<std::setw(4)*/ << idata <<std::endl<<std::dec;
	assert(strlen("b5adf4f048f244c71ce10b2e4246f54e188008dd")==5*4*2);

	do_not_copy c1;
	do_not_copy c2(c1);
	do_not_copy c3;
	c3 = c1;
	std::cout<<"---------------"<<std::endl;

	boost::optional<int> op0;
	boost::optional<int> op1(boost::none);
	assert(op1 == op0);
	assert(op0.get_value_or(123) == 123);
	
	boost::optional<std::string> os("HelloÖÐ¹ú");
	std::cout << *os << std::endl;

	std::vector<int> vi(10);
	boost::optional<std::vector<int>&> opv(vi);
	opv->push_back(5);
	assert(opv->size()==11);

	opv = boost::none;
	assert(!opv);
	std::cout<< vi.size() << std::endl;
	std::cout<<"---------------"<<std::endl;

	boost::optional<double> result =  dao_shu(0);
	if(result) std::cout<< *result<<std::endl;
	else std::cout << "no value" <<std::endl;

	std::cout<<"---------------"<<std::endl;
	vector<int> vassign;
	vassign += 1,2,3,4,6*6;
	cout<< vassign.size()<<endl;
	std::for_each(vassign.begin(), vassign.end(), boost::bind<int>(printf,"%d,",_1));
	cout<<endl;

	set<string> ss;
	ss += "cpp","java","C#","python";
	std::for_each(ss.begin(), ss.end(), boost::bind(print_string,_1));
	std::cout<<"---------------"<<std::endl;

	/*
	using boost::serialization::singleton;
	cout<< "singlton start"<<endl;
	typedef singleton<A> origin;
	origin::get_const_instance().dosthconst();
	origin::get_mutable_instance().dosth();
	cout<<"singleton end"<<endl;
	*/


	_getch();
	return 0;
}