#include <boost/circular_buffer.hpp>
#include <numeric>
#include <assert.h>
#include <iostream>

int main(){

	boost::circular_buffer<char> cb(3);
	assert(cb.capacity()==3);

	assert(cb.size()==0);
	assert(cb.empty());

	cb.push_back('a');

	std::cout<<cb.size()<<std::endl;
}