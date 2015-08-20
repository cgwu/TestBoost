#include "stdafx.h"
#include "pyinit.hpp"

using namespace std;
namespace bp = boost::python;

int _tmain_TestPython(int argc, _TCHAR* argv[])
{
	//cout<<"Hello"<<endl;
	Py_Initialize();

	PyRun_SimpleString("import hello");
	PyRun_SimpleString("import sys;reload(sys);sys.setdefaultencoding('gbk')");
	PyRun_SimpleString("hello.hello('zhangsan')");

	PyRun_SimpleString("result = 5**2");
	PyObject * module = PyImport_AddModule("__main__");
	assert(module);
	PyObject * dictionary = PyModule_GetDict(module);
	PyObject * result = PyDict_GetItemString(dictionary, "result");
	cout << extract<long>(result) << endl;

	PyRun_SimpleString("sum = hello.add(11,22)");
	PyObject * sum = PyDict_GetItemString(dictionary, "sum");
	cout << extract<long>(sum) << endl;


	cout<<"/***************方法 2 *****************/"<<endl;

	try {
		PyRun_SimpleString("result = 3 ** 2");
    
		object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
		object dictionary = module.attr("__dict__");
		object result = dictionary["result"];
		int result_value = extract<int>(result);
    
		std::cout << result_value << std::endl;
    
		dictionary["result"] = 20;

		PyRun_SimpleString("print(result)");

		bp::list l;
		l.append("zelda");
		//l.append(2.236);
		l.append("ella");
		l.append("sfff");
		cout<< extract<char*>(l[1]) << endl;
		l.sort();
		cout<<"**** after sort ****"<<endl;
		cout<< extract<char*>(l[0]) << endl;
		cout<< extract<char*>(l[1]) << endl;
		cout<< extract<char*>(l[2]) << endl;

		cout<<"/********************************/"<<endl;
		cout<< extract<int>(bp::eval("3*3")) <<endl;		//执行表达式
		bp::exec("1+2");

	} catch (error_already_set) {
		PyErr_Print();
	}

	Py_Finalize();
	_getch();
	return 0;
}