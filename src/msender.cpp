#include "sender.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

//kira-kira program utamanya begini
int main(int argc, char ** argv){
	int portno;
	const char* rcv_ip;
	

	if (argc!=3){
		std::cout<<argc;
		std::cerr<<"USAGE: "<<argv[0]<<"<IP> <port>"<<std::endl;
		return 1;
	}
	
	
	rcv_ip = argv[1];
	portno = atoi(argv[2]);
	
  	sender s;
	s.connect(rcv_ip, portno);
	s<<std::cin;
	
	
}
