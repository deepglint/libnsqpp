#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/circular_buffer.hpp>


using boost::asio::ip::tcp;

class Worker{
public:
	std::string ip;
	std::string port;
	std::string topic;
	std::string channel;
	int received;
	Worker(void(* callback)(std::vector<char>),void(* errorcallback)(),std::string ip, std::string port,std::string topic, std::string channel);
	void Sub(std::string topic,std::string channel);
	void Rdy(int count);
	void operator()();
private:
	tcp::socket * socket;
	void (* onmessage)(std::vector <char>);
	void (* ondisconnected)();	
};
uint64_t ntoh64(uint8_t *data);
uint32_t readInt32FromNet(tcp::socket * socket);
uint32_t readInt32FromCB(boost::circular_buffer<char> &cb);
void readFromCB(boost::circular_buffer<char> &cb,std::vector<char> & data,int length);
uint32_t readSizeFromBuffer(char* buf);
size_t readBufFromNet(tcp::socket * socket,std::vector<char>  &buf,int length);
void wirteBufToCB(boost::circular_buffer<char> &cb,char* buf,int length);
void sub(tcp::socket &socket,std::string topic,std::string channel);
void rdy(tcp::socket &socket,int count);
void nop(tcp::socket &socket);