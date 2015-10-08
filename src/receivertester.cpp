#include "frame.h"
#include "ackframe.h"
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

	struct sockaddr_in si_other;
	int s, i;
	socklen_t slen=sizeof(si_other);

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

	int choice;
	do{
		std::cout<<"menu:\n0. quit\n1. send normal frame\n2.send frame with invalid format\n3.send frame with invalid checksum\n";
		std::cin>>choice;
		while (choice<0 || choice>3) {std::cout<<"invalid input";std::cin>>choice;}
		if (choice==0) break;
		std::string data;
		int framenum;
		std::cout<<"frame number:";
		std::cin>>framenum;
		std::cout<<"data:";
		std::cin>>data;
		frame F ( framenum, data);
		std::string tempstr (F.getPointerToBytes(),F.getBytesLength());
		switch(choice){
			case 1:
				break;
			case 2:
				tempstr[0]='a';
				break;
			case 3:
				if (data.size()>0)
					tempstr[6]++;
				else
					tempstr[2]++;
				break;
		}
		F = frame(tempstr);

		//sending
		if (sendto(s,F.getPointerToBytes(), F.getBytesLength(),0, (struct sockaddr*) &si_other, slen)==-1)
		        diep("sendto()");

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
		}else{
			ackframe AF(bufchar,ackframelen);
			if (AF.isValid()){
				std::cout<<(AF.isACK()?"ACK ":"NAK ")<<AF.getFrameNumber()<<std::endl;
			}else{
				std::cout<<"invalid ack received"<<std::endl;
			}
		}
		

	}while (true);
}
