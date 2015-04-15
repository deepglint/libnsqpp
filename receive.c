#include <assert.h>
#include <libc.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

int main(const int argc, const char **argv){
  int sock = nn_socket (AF_SP_RAW, NN_PULL);
  assert (sock >= 0);
  assert (nn_connect (sock, argv[1]) >= 0);
  char* cmd="  V2";
  char *buf=NULL;
  while(1){
  	int result=nn_recv(sock,&buf,NN_MSG,0);
  	if(result>0){
  		printf (" RECEIVED \"%s\"\n",buf);
      	nn_freemsg (buf);
  	}
  }    
  return 0;
}