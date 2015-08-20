#include "stdafx.h"

using namespace std;

void f(int a,int b){
	cout<<"_1:"<<a<<",_2:"<<b<<endl;
}

void g(int a,int b, int c){
	cout<<"_1:"<<a<<",_2:"<<b<<",_3:"<<c<<endl;
}

typedef void (*f_type) (int a,int b);
typedef void (*g_type) (int a,int b, int c);

struct demo{
	void f(int a,int b){
		cout<<"demo::f() -> _1:"<<a<<",_2:"<<b<<endl;
	}
};

struct point{
	int x,y;
	point(int a=0, int b=0):x(a),y(b){}
	void print(){
		cout<<"("<<x<<","<<y<<")"<<endl;
	}
};

int mygreater(int a, int b){
	return a>b?a:b;
}
struct func_min{
	int operator() (int a, int b){
		return a<b?a:b;
	}
};

int add_1(int x){
	return x+1;
}

int add_2(int x){
	return x+2;
}

int fadd(int a,int b){
	return a+b;
}

struct demo_class{
	int add(int a,int b){ return a+b;}
	int operator() (int x){ return x*x;}
};

int _tmain_demo(int argc, _TCHAR* argv[])
{
	boost::bind(f,33,_1)(110);
	boost::bind(f,_1,_3)(110,333,4);
	boost::bind(f,11,22)();
	boost::bind(f,_2,_1)(11,22);

	boost::bind(g,_3,_2,_2)(11,22,33);
	// printf("Hello\n");
	boost::bind<int>(printf,"***hello,%d,%f\n",123, 3.14)();		//error
	cout<<"--------------"<<endl;
	f_type pf = f;
	boost::bind(pf,1,2)();
	cout<<"--------------"<<endl;

	demo d, &rd = d, *pd = &d;
	boost::bind(&demo::f, d,1,2)();
	boost::bind(&demo::f, rd,11,22)();
	boost::bind(&demo::f, pd,111,222)();
	cout<<"--------------"<<endl;

	vector<point> v;
	vector<int> v2;
	for(int i=0;i<10;i++){
		v.push_back(point(i,i*10));
	}
	std::for_each(v.begin(),v.end(),boost::bind(&point::print,_1));
	cout<<"--------------"<<endl;
	typedef pair<int,string> pair_t;
	pair_t p(123,"hello中国");
	cout<< boost::bind(&pair_t::first,p)()<<endl;
	cout<< boost::bind(&pair_t::second,p)()<<endl;

	cout<< boost::bind<int>(&mygreater,11,2)() <<endl;
	cout<< boost::bind<int>(func_min(),11,2)() <<endl;
	cout << boost::bind(plus<int>(),_1,3)(4) << endl;
	
	int a=10,b=20,c=30;
	boost::bind(g,boost::ref(a),boost::cref(b),boost::ref(c))();
	
	auto fobj = func_min();
	auto fbigger_std = boost::bind<int>(fobj,_1,_2);
	
	BOOST_AUTO(fbigger,boost::bind<int>(ref(fobj),_1,_2));		// BOOST_AUTO 模拟c++11 auto
	cout<< fbigger(11,22)<< endl;
	cout<< fbigger_std(111,222)<< endl;
	cout<<"--------------"<<endl;

	auto f = boost::bind(add_2,boost::bind(add_1,_1));
	cout<<f(3)<<endl;
	cout<<"--------------"<<endl;
	boost::function<int(int,int)> fo;
	assert(!fo);
	fo = fadd;
	if(fo){
		cout<<fo(1,2)<<endl;
	}
	assert(!fo.empty());
	fo.clear();
	assert(fo.empty());
	cout<<"--------------"<<endl;

	function<int(demo_class&, int,int)> func1;
	func1 = boost::bind(&demo_class::add, _1,_2,_3);
	demo_class sc;
	cout<<func1(sc,10,20)<<endl;

	function<int(int,int)> func2;
	func2 = boost::bind(&demo_class::add, &sc,_1,_2);
	cout<< func2(11,22) << endl;

	function<int(int)> func3;
	func3 = boost::bind<int>(sc,_1);	//绑定函数对象
	cout<< func3(3)<<endl;
	cout<< sc(4)<<endl;

	_getch();
	return 0;
}
