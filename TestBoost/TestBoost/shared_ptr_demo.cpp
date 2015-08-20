#include "stdafx.h"
using namespace std;
/*
ʹ��enable_shared_from_this

˵��
The header <boost/enable_shared_from_this.hpp> defines the class template enable_shared_from_this. 
It is used as a base class that allows a shared_ptr to the current object to be obtained from within a member function.
�̳и���Ϳ��Խ��л��ڵ�ǰ������а�ȫ��weap_ptr��shared_ptr��ת��...

*/
struct Foo: boost::enable_shared_from_this<Foo>{
	int data;
	boost::shared_ptr<Foo> get_shared(){
		return shared_from_this();
	}
};

int func_foo(int val, boost::function<int(int)> func){
	return func(val);
}

int mul_1(int a){ return a*2; }

int mul_2(int a, int b){ return a * b; }

int _tmain(int argc, _TCHAR* argv[])
{
	int r;
	r = func_foo(3, mul_1);
	cout << r <<endl;

	r = func_foo(4, boost::bind( mul_1, _1 ));
	cout << r <<endl;

	r = func_foo(5, boost::bind( mul_2, 3, _1 ));
	cout << r <<endl;

	/*
	cout<<"Hello�й�"<<endl;

	boost::shared_ptr<Foo> spFoo(new Foo);
	boost::shared_ptr<Foo> spNew = spFoo->get_shared();
	spFoo->data = 110;
	cout<<spFoo->data<<endl;
	cout<<spNew->data<<endl;
	spFoo->data = 220;
	cout<<spFoo->data<<endl;
	cout<<spNew->data<<endl;
	*/
	_getch();
	return 0;
}