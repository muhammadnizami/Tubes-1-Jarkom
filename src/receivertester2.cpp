#if __cplusplus <= 199711L
  #error Perlu support c++11. Coba tambahkan -std=c++11 sebagai tag kompilasi.
#else


#include "frame.h"
#include "ackframe.h"
#include "dcomm.h"
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

#define TIMEOUT 1000

void diep(char *s)
{
	perror(s);
	exit(1);
}

struct sockaddr_in si_other;
int s;
socklen_t slen;

bool ackEd(){
	//waiting for ack
	struct timeval tv;

	tv.tv_sec = TIMEOUT/1000;  /* 30 Secs Timeout */
	tv.tv_usec = TIMEOUT%1000;  // Not init'ing this can cause strange errors
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

	char bufchar[128];
	std::cout<<"waiting for ack..."<<std::endl;
	int ackframelen=recvfrom(s, bufchar, 128, 0, (sockaddr*) &si_other, &slen);
	if (ackframelen<0){
		(std::cout<<"timeout waiting for ack")<<std::endl;
		return false;
	}else{
		ackframe AF(bufchar,ackframelen);
		if (AF.isValid()){
			std::cout<<(AF.isACK()?"ACK ":"NAK ")<<AF.getFrameNumber()<<std::endl;
			return AF.isACK();
		}else{
			std::cout<<"invalid ack received"<<std::endl;
			return false;
		}
	}
}

int main(int argc, char ** argv){
	int portno;
	const char* rcv_ip;
	const char* pathtofiletosend;

	if (argc != 3){
		std::cerr<<std::string("")+"usage: "+argv[0]+" <IP> <PORT NUMBER>\n";
		exit(1);
	}

	rcv_ip = argv[1];
	portno = atoi(argv[2]);


	slen=sizeof(si_other);

	printf("Membuat socket untuk koneksi(?) ke %s:%d ...\n",rcv_ip,portno);
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(portno);
	if (inet_aton(rcv_ip, &si_other.sin_addr)==0) {
		std::cerr<< "inet_aton() failed\n";
		exit(1);
	}

	int numchars;
	std::cout<<"num of chars:";
	std::cin>>numchars;
	int i=0;
	int frameNum=0;
	char c='a';
	while (i<=numchars){
		std::cout << "framenum: " <<frameNum <<std::endl;	
		std::string data;
		for (int j=0;j<3;j++){
			data+=c;
			c++;
			if (c>'z'){
				c='a';
				data+=std::to_string(i);
				data+='\n';
				i++;
			}
		}
		frame F ( frameNum, data);

		

		do{
			//sending
			if (sendto(s,F.getPointerToBytes(), F.getBytesLength(),0, (struct sockaddr*) &si_other, slen)==-1)
				diep("sendto()");
		} while (!ackEd());
		frameNum=(frameNum+1)%bufsize;

	};
}

#endif
