#Libnsqpp
##A cpp client library for NSQ

Libnsqpp is the client library for nsq

By now libnsqpp only support for subscribe

building nsq lib

```g++ -c nsq.cpp```

run test:

```g++ -c testnsq.cpp ```

```g++ -o testnsq testnsq.o -I /usr/local/include/ ./nsq.o -lboost_system -lboost_thread-mt```

###Usage for libnsqpp:

```

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
```
As you can see above, that when you use it, you should note the message receive and on disconnect handler.

When the message handler is called, you can get the Message body vector vc, vc.data()+30 is the begin of Message Data Body.

###TODO:

###Message Object

libnsqpp needs a message class and object 

###Pub/MPub function

Libnsqpp needs these 2 method to streaming posting data into nsqd

###Support for nsqlookupd 

By now, libnsqpp only support to subscribe the messages from nsqd with out nsqlookupd

###Identify/Auth

The Identify & Auth rule and method for client to do more security things
