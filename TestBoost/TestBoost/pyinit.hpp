#include "stdafx.h"
#include <boost/python.hpp>
#include <boost/noncopyable.hpp>
#include <cstdlib>
using namespace boost::python;

#ifndef __PYINIT__
#define __PYINIT__

class pyinit: boost::noncopyable{
public:
    pyinit(int initsigs = 1){
        /*
void Py_Initialize()
Initialize the Python interpreter. In an application embedding Python, 
this should be called before using any other Python/C API functions; 
with the exception of Py_SetProgramName(), Py_SetPythonHome(), PyEval_InitThreads(), 
PyEval_ReleaseLock(), and PyEval_AcquireLock(). This initializes the table of loaded modules 
(sys.modules), and creates the fundamental modules __builtin__, __main__ and sys.
It also initializes the module search path (sys.path). It does not set sys.argv;
use PySys_SetArgvEx() for that. This is a no-op when called for a second time 
(without calling Py_Finalize() first). There is no return value; 
it is a fatal error if the initialization fails.

void Py_InitializeEx(int initsigs)
This function works like Py_Initialize() if initsigs is 1. If initsigs is 0, 
it skips initialization registration of signal handlers, 
which might be useful when Python is embedded.

New in version 2.4.
		*/
		assert(initsigs == 0 || initsigs == 1);
        Py_InitializeEx(initsigs);
        
    }
    ~pyinit(){
        Py_Finalize();
    }
 
    bool isInitialized(){
        return Py_IsInitialized();
    }
    const char* version(){
        return Py_GetVersion();
    }
};
#endif