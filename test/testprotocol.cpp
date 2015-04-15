#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

tcp::socket *connect(char* ip,char* port){
  
  boost::asio::io_service io_service;
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(ip, "4150");
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::socket socket=new tcp::socket(io_service);
  boost::asio::connect(socket, endpoint_iterator);
  std::string message = "  V2";
  boost::system::error_code ignored_error;
  boost::asio::write(socket, boost::asio::buffer(message), ignored_error);}
  return &socket;

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

    // Convert the server name that was specified as a parameter to the application, to a TCP endpoint. 
    // To do this, we use an ip::tcp::resolver object.
    tcp::resolver resolver(io_service);

    // A resolver takes a query object and turns it into a list of endpoints. 
    // We construct a query using the name of the server, specified in argv[1], 
    // and the name of the service, in this case "daytime".
    tcp::resolver::query query(argv[1], "4150");

    // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator. 
    // A default constructed ip::tcp::resolver::iterator object can be used as an end iterator.
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Now we create and connect the socket.
    // The list of endpoints obtained above may contain both IPv4 and IPv6 endpoints, 
    // so we need to try each of them until we find one that works. 
    // This keeps the client program independent of a specific IP version. 
    // The boost::asio::connect() function does this for us automatically.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

   
    std::string message = "  V2";

      boost::system::error_code ignored_error;

      // writing the message for current time
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);

    // The connection is open. All we need to do now is read the response from the daytime service.
    for (;;)
    {
      // We use a boost::array to hold the received data. 
      boost::array<char, 128> buf;
      boost::system::error_code error;

      // The boost::asio::buffer() function automatically determines 
      // the size of the array to help prevent buffer overruns.
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      // When the server closes the connection, 
      // the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof error, 
      // which is how we know to exit the loop.
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  // handle any exceptions that may have been thrown.
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}