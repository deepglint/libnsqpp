#include "nsq.h"
#include <iostream>

void hello(std::vector <char> vc){
	//std::cout<<"Hello world\n";
	std::cout<<vc.data()+30<<std::endl;
}

void onDisconnected(){
	std::cout<<"The Nsq Client disconnected!!\n";
}

int  main(int argc, char const *argv[])
{
	Worker w(hello,onDisconnected,argv[1],"4150","testboost","test");

	boost::thread workerThread(w);
	workerThread.join();
	return 0;
}