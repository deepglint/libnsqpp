#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
//#include <ctime>

using boost::asio::ip::tcp;

uint64_t ntoh64(uint8_t *data) {
    return (uint64_t)(data[7]) | (uint64_t)(data[6])<<8 |
        (uint64_t)(data[5])<<16 | (uint64_t)(data[4])<<24 |
        (uint64_t)(data[3])<<32 | (uint64_t)(data[2])<<40 |
        (uint64_t)(data[1])<<48 | (uint64_t)(data[0])<<56;
}

uint32_t read_size(tcp::socket * socket,boost::array<char,4> buf){
    boost::system::error_code error;
    //tcp::socket socket=*(psocket);

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

void readBufFromNet(tcp::socket * socket,std::vector<char>  &buf){
  boost::system::error_code error;
  size_t len=socket->read_some(boost::asio::buffer(buf),error);
  if(len<buf.size()){
    std::cout<<"The length error for reading\n";
  }
}

int main(int argc, char* argv[])
{
  try
  {
    // the user should specify the server - the 2nd argument
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    // Any program that uses asio need to have at least one io_service object
      boost::asio::io_service io_service;
      tcp::resolver resolver(io_service);
      tcp::resolver::query query(argv[1], "4150");
      tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
      tcp::socket socket(io_service);
      boost::asio::connect(socket, endpoint_iterator);
      std::string magic = "  V2";
      std::string sub="SUB testboost test\n";
      std::string pub="PUB testboost\n12";
      std::string rdy="RDY 50\n";
      //std::string fin;
      //char* data=new char[7];
      //sprintf(data,"000212");
      boost::system::error_code ignored_error;

      // writing the message for current time
      boost::asio::write(socket, boost::asio::buffer(magic), ignored_error);
      //boost::asio::write(socket, boost::asio::buffer(pub), ignored_error);

      boost::asio::write(socket, boost::asio::buffer(sub), ignored_error);
      boost::asio::write(socket, boost::asio::buffer(rdy), ignored_error);
      boost::array<char, 1024000> buf;
      boost::array<char,4> sizebuf;
      std::vector<char> 

    // The connection is open. All we need to do now is read the response from the daytime service.
    for (;;)
    {
      std::cout<<"Here is the receving loop"<<std::endl;
      // We use a boost::array to hold the received data. 
      uint32_t size=read_size(&socket,sizebuf);
      uint32_t frametype=read_size(&socket,sizebuf);

      boost::system::error_code error;
      std::cout<<size<<std::endl;
      std::cout<<frametype<<std::endl;

      //boost::array<char,6> databuf;
      size_t len = socket.read_some(boost::asio::buffer(buf), error);
      if (error == boost::asio::error::eof)
        std::cout<<"Connection closed\n";
      else if (error)
        throw boost::system::system_error(error);
      char* data=buf.data();
      if(len>=26)
        std::cout<<data+26<<std::endl;
      else std::cout<<data<<std::endl;
      if(frametype==2){
        char* idbuf[16];
        memcpy(idbuf,data+10,16);
        //std::string id(idbuf, std::find(idbuf, idbuf + 15, '\0'));
        //fin="FIN "+std::string((const char*)idbuf,16)+'\n';
        std::string fin=//"FIN "+std::string((const char*)idbuf,15)+'\n';
        "FIN 0825a3b59229b04f\n";
        //std::cout<<fin<<std::endl;
        char finchar[22];

        memcpy(finchar,"FIN ",4);
        memcpy(finchar+4,idbuf,16);
        finchar[20]='\n';
        finchar[21]='\0';
        //memcpy(finchar+20,&('\n'),1);
        //memcpy(finchar+21,&('\0'),1);
        std::cout<<finchar<<std::endl;

        boost::asio::write(socket, boost::asio::buffer(finchar,21), ignored_error);
      }
      // char timestamp[8];
      // char id[17];
      // char datasize[4];
      // char frametype[4];
      // id[16]=0;
      // char* data;
      //       // The boost::asio::buffer() function automatically determines 
      // // the size of the array to help prevent buffer overruns.
      // size_t len = socket.read_some(boost::asio::buffer(buf), error);

      // // When the server closes the connection, 
      // // the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof error, 
      // // which is how we know to exit the loop.
      // if (error == boost::asio::error::eof)
      //   break; // Connection closed cleanly by peer.
      // else if (error)
      //   throw boost::system::system_error(error); // Some other error.
      // //std::cout<<"----------------------------"<<std::endl;
      // //std::cout<< (int64_t)ntoh64((uint8_t *)buf.data())<<std::endl;
      // data=buf.data();
      // uint32_t* pdatasize=(uint32_t*)datasize;
      // uint32_t datasize2=ntohl(*pdatasize);
      // std::cout<<"----------------------------"<<std::endl;
      // memcpy(id, data+10, 16);
      // memcpy(timestamp,data,8);
      // memcpy(datasize,data,4);
      // memcpy(frametype,data+4,4);
      // std::cout<<(uint32_t)*((uint8_t*)id)<<std::endl;
      // std::cout<<"----------------------------"<<std::endl;
      // std::cout<<(int)*((int*)frametype)<<std::endl;
      // std::cout<<(uint32_t)*((uint32_t*)frametype)<<std::endl;
      // std::cout<<*(reinterpret_cast<unsigned long long*>(frametype))<<std::endl;
      // std::cout<<"----------------------------"<<std::endl;
      // std::cout<<(int)*((int*)datasize)<<std::endl;
      // std::cout<<datasize2<<std::endl;
      // std::cout<<*(reinterpret_cast<unsigned long long*>(frametype))<<std::endl;
      //std::cout.write(buf.data(), len);
    }
  }
  // handle any exceptions that may have been thrown.
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}