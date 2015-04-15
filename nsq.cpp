// #include <iostream>
// #include <boost/array.hpp>
// #include <boost/asio.hpp>
// #include <boost/thread.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>
// #include <boost/circular_buffer.hpp>
#include "nsq.h"

using boost::asio::ip::tcp;

uint64_t ntoh64(uint8_t *data) {
    return (uint64_t)(data[7]) | (uint64_t)(data[6])<<8 |
        (uint64_t)(data[5])<<16 | (uint64_t)(data[4])<<24 |
        (uint64_t)(data[3])<<32 | (uint64_t)(data[2])<<40 |
        (uint64_t)(data[1])<<48 | (uint64_t)(data[0])<<56;
}

uint32_t readInt32FromNet(tcp::socket * socket)
{
    boost::system::error_code error;
    //tcp::socket socket=*(psocket);
    boost::array<char, 4> buf;
    size_t len = socket->read_some(boost::asio::buffer(buf), error);
    if(len<4){
      std::cout<<"Error for reading socket buffer\n";
    }
    if (error == boost::asio::error::eof)
      std::cout<<"Connection closed\n";
    else if (error)
      throw boost::system::system_error(error);
    char* data=buf.data();
    uint32_t datasize=ntohl(*(uint32_t*)data);
    return datasize;

}

uint32_t readInt32FromCB(boost::circular_buffer<char> &cb){
	char num[4];
	for(int i=0;i<4;i++){
		num[i]=cb[0];
		cb.pop_front();
	}
	uint32_t datasize=ntohl(*(uint32_t*)num);
	return datasize;
}


void readFromCB(boost::circular_buffer<char> &cb,std::vector<char> & data,int length){
	std::cout<<"reading from buffer:"<<cb.size()<<std::endl;
	data.resize(0);
	for(int i=0;i<length;i++){
		data.push_back(cb[0]);
		cb.pop_front();
	}
	std::cout<<"exiting from buffer:"<<cb.size()<<std::endl;
}

uint32_t readSizeFromBuffer(char* buf){
	uint32_t datasize=ntohl(*(uint32_t*)buf);
	return datasize;
}

size_t readBufFromNet(tcp::socket * socket,std::vector<char>  &buf,int length){
	boost::system::error_code error;
	buf.empty();
	buf.resize(length);
	std::cout<<"Now length is :"<<buf.size()<<std::endl;
	size_t len=socket->read_some(boost::asio::buffer(buf),error);
	return len;
}

void wirteBufToCB(boost::circular_buffer<char> &cb,char* buf,int length){
	if(cb.reserve()<length){
		cb.set_capacity(cb.size()+length+20);
	}
	for(int i=0;i<length;i++){
		cb.push_back(buf[i]);
	}
}

void sub(tcp::socket &socket,std::string topic,std::string channel){
	boost::system::error_code ignored_error;
	std::string subcmd="SUB "+topic+" "+channel+"\n";
	boost::asio::write(socket,boost::asio::buffer(subcmd),ignored_error);
}

void rdy(tcp::socket &socket,int count){
	std::string rdycmd="RDY "+std::to_string(count)+"\n";
	boost::system::error_code ignored_error;
	boost::asio::write(socket,boost::asio::buffer(rdycmd),ignored_error);
}

void nop(tcp::socket &socket){
	std::string nopcmd="NOP\n";
	boost::system::error_code ignored_error;
	boost::asio::write(socket,boost::asio::buffer(nopcmd),ignored_error);
}

// void hello(std::vector <char> vc){
// 	//std::cout<<"Hello world\n";
// 	std::cout<<vc.data()+30<<std::endl;
// }

Worker::Worker(void(* callback)(std::vector <char>),void(* errorcallback)(),std::string ip,std::string port,std::string topic,std::string channel){
	this->onmessage=callback;
	this->ip=ip;
	this->port=port;
	this->topic=topic;
	this->channel=channel;
	this->received=0;
	this->ondisconnected=errorcallback;
}

void Worker::Sub(std::string topic,std::string channel){
	sub(*(this->socket),topic,channel);
}

// class Worker{
// public:
// 	std::string ip;
// 	std::string port;
// 	std::string topic;
// 	std::string channel;
// 	int received;
// 	Worker(void (* callback)(std::vector <char> ),std::string ip,std::string port,std::string topic,std::string channel){
// 		//this->socket=socket;
		
// 	}
	// void Connect(std::string ip,std::string port){
	// 	boost::asio::io_service io_service;
	//     tcp::resolver resolver(io_service);
	//     tcp::resolver::query query(ip, port);
	//     tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	//     tcp::socket socket(io_service);
	//     boost::asio::connect(socket, endpoint_iterator);
	//     std::string magic = "  V2";
	//     boost::system::error_code ignored_error;
	// 	boost::asio::write(socket, boost::asio::buffer(magic), ignored_error);
	// 	this->socket=&socket;
	// 	std::cout<<"Connecting OK"<<std::endl;
	// }
	// void Sub(std::string topic,std::string channel){
	// 	sub(*(this->socket),topic,channel);
	// }

void Worker::Rdy(int count){
	rdy(*(this->socket),count);
}
	// void Rdy(int count){
	// 	rdy(*(this->socket),count);
	// }
void Worker::operator()(){
		//this->Connect(this->ip,this->port);
		boost::asio::io_service io_service;
	    tcp::resolver resolver(io_service);
	    tcp::resolver::query query(this->ip, this->port);
	    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	    tcp::socket socket(io_service);
	    boost::asio::connect(socket, endpoint_iterator);
	    std::string magic = "  V2";
	    boost::system::error_code ignored_error;
		boost::asio::write(socket, boost::asio::buffer(magic), ignored_error);
		this->socket=&socket;
		std::cout<<"Connecting OK"<<std::endl;
		this->Sub(this->topic,this->channel);
		this->Rdy(50);
		std::vector <char> buf;
		std::vector <char> body;
		boost::posix_time::ptime boostTimestamp = boost::posix_time::second_clock::local_time();
		boost::circular_buffer<char> ringbuffer(100000);
		//boost::system::error_code ignored_error;
		for(;;){
			std::cout<<"Here is the receving loop"<<std::endl;
		    size_t len=readBufFromNet(this->socket,buf,128);
		    std::cout<<len<<std::endl;
		    if(len<4){

		    	std::cout<<"Error:May be network is weak"<<std::endl;
		    	this->ondisconnected();
		    	break;
		    }
		    wirteBufToCB(ringbuffer,buf.data(),len);
		    uint32_t size=readInt32FromCB(ringbuffer);
		    std::cout<<size<<std::endl; 
		    while(size>ringbuffer.size()) {
		        size_t len=readBufFromNet(this->socket,buf,128);
		        wirteBufToCB(ringbuffer,buf.data(),len);
		    }
		    readFromCB(ringbuffer,body,size);
		    uint32_t frametype=readSizeFromBuffer(body.data());
		    std::cout<<frametype<<std::endl;
		    if(frametype!=2){
		    	char* data=body.data();
		    	std::cout<<data+4<<std::endl<<body.size()<<std::endl;
		    	body.resize(0);
		    }else{
		    	char* data=body.data();
		    	// std::cout<<data+30<<std::endl<<"length of body:"<<body.size()<<std::endl;
		    	this->onmessage(body);
		    	this->received++;
		    	char* idbuf[16];
			    memcpy(idbuf,data+14,16);
		        char finchar[22];
		        memcpy(finchar,"FIN ",4);
		        memcpy(finchar+4,idbuf,16);
		        finchar[20]='\n';
		        finchar[21]='\0';
		        std::cout<<finchar<<std::endl;
		        boost::asio::write(*(this->socket), boost::asio::buffer(finchar,21), ignored_error);
		    	body.resize(0);
		    }
		    boost::posix_time::ptime nowBoost = boost::posix_time::second_clock::local_time();
		    long diffBoost=(nowBoost-boostTimestamp).total_seconds();
		    std::cout<<"BoostTime:"<<diffBoost<<std::endl;
		    if(diffBoost>=30){
		    	nop(*(this->socket));
		    	boostTimestamp=nowBoost;
		    }
		    if((this->received%500)==449){
		    	this->Rdy(500);
		    }
		}
	}
// private:
// 	tcp::socket * socket;
// 	void (* onmessage)(std::vector <char> );
// 	void (* ondisconnected)();
// };

// int  main(int argc, char const *argv[])
// {
// 	Worker w(hello,argv[1],"4150","testboost","test");

// 	boost::thread workerThread(w);
// 	workerThread.join();
// 	return 0;
// }