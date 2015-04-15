#include <iostream>
#include <boost/thread.hpp>


void workerFunc(){
	std::cout<<"thread:Heool world "<<std::endl;
}

// void workerFunc2(const char*msg,unsigned delaySecs){

// }

class Worker{
public:
	Worker(int num,const char* str):m_num(num),m_str(str){

	}
	void operator()(){
		std::cout<<this.m_str<<std::endl;
	}
private:
	int m_num;
	const char* m_str; 
};

int main(int argc,char* argv[]){
	std::cout<<"main startup"<<std::endl;
	boost::thread workerThread(workerFunc);
	//boost::thread workerThread(workerFunc2,"hello boost!",3)
	Worker w(2,"Hello world");
	boost::thread workerThread(w);
	
	workerThread.join();
	std::cout<<"main end"<<std::endl;


	return 0;
}