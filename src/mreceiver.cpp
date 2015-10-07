#include "receiver.cpp"
#include <iostream>

//kira-kira program utamanya begini
int main(int argc, char ** argv){
	if (argc!=2){
		std::cerr<<"USAGE: ./receiver PORT [ > OUTPUT ]";
		return 1;
	}
	
	receiver RCV;
	RCV.listen(argv[1]);
	RCV>>std::cout;
	return 0;
}
