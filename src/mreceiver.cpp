#include "receiver.h"
#include <iostream>

//kira-kira program utamanya begini
int main(int argc, char ** argv){
	if (argc!=2){
		std::cerr<<"USAGE: "<<argv[0]<<" PORT [ > OUTPUT ]"<<std::endl;
		return 1;
	}
	
	receiver RCV;
	if(!RCV.listen(atoi(argv[1]))){std::cerr<<"error listening";return 1;}
	RCV>>std::cout;
	return 0;
}
