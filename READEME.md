g++ -c tcptest.cpp
g++ -o tcptest tcptest.o -I /usr/local/include/ -lboost_filesystem -lboost_system

building nsq lib

g++ -c nsq.cpp

run test:

g++ -c testnsq.cpp

g++ -o testnsq testnsq.o -I /usr/local/include/ ./nsq.o -lboost_system -lboost_thread-mt
