#include "stdafx.h"

using namespace std;

void sync_action_scoped(){
	boost::mutex mu;
	boost::mutex::scoped_lock sl(mu);
	cout<<"some action synchronized."<<endl;
}

void sync_action(){
	boost::mutex mu;
	try{
		mu.lock();
		cout<<"some action scoped synchronized."<<endl;
		mu.unlock();
	}
	catch(...){
		mu.unlock();
	}
}

template<typename T>
class basic_atom: boost::noncopyable{
private:
	T n;
	typedef boost::mutex mutex_t;
	mutex_t mu;
public:
	basic_atom(T x = T()):n(x){}
	T operator++ () {
		mutex_t::scoped_lock lock(mu);
		return ++n;
	}
	operator T(){ return n; }
};

typedef basic_atom<int> basic_cnt;
boost::mutex io_mu;

void printing(basic_atom<int> &cnt, const string& name){
//void printing(int &cnt, const string& name){
	boost::mutex::scoped_lock lock(io_mu);
	for(int i=0;i<5;++i){
		++cnt;
		boost::this_thread::sleep(boost::posix_time::microseconds(1000));
		cout<<name<<":"<< cnt <<endl;
	}
}

void simple_print(string & name){
	cout<<boost::this_thread::get_id()<< ":this is in my thread:"<< name <<endl;
}

int fab(int n){
	if(n==0 || n==1)return 1;
	else return fab(n-2) + fab(n-1); 
}

int _tmain_concurrent(int argc, _TCHAR* argv[])
{
	/*
	cout<<"before sleep."<<endl;
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	cout<<"end sleep."<<endl;
	*/
	sync_action();
	sync_action_scoped();

	basic_atom<int> cnt;
	++cnt;
	cout << ++ cnt <<endl;
	cout << ++ cnt <<endl;

	int icnt = int(cnt);
	cout << "icnt="<<icnt<<endl;
	cout<<"----------------"<<endl;

	string name("ÕÅÁ¼Èý");
	
	//boost::thread t1(boost::bind( simple_print,name) );
	//t1.join();

	
	basic_atom<int> p_cnt;
	int mycnt = 0;
	//printing(p_cnt,name);
	boost::thread p1(printing,boost::ref(p_cnt),"hello");
	boost::thread p2(printing,boost::ref(p_cnt),"boost");
	p1.join();
	p2.join();

	//boost::this_thread::sleep(boost::posix_time::seconds(2));
	cout<<"wake up."<<endl;
	cout<<"----------------"<<endl;

	boost::packaged_task<int> pt(boost::bind(fab,10));
	boost::unique_future<int> uf = pt.get_future();
	
	boost::thread(boost::move(pt));
	uf.wait();

	assert(uf.is_ready() && uf.has_value());
	cout<< uf.get()<<endl;

	_getch();
	return 0;
}