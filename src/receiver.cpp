#ifndef receiver_cpp
#define receiver_cpp

#include "receiver.h"
#include "ackframe.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#if __cplusplus <= 199711L
	#error Perlu support c++11. Coba tambahkan -std=c++11 sebagai tag kompilasi.
#else

std::string receiver::popStringFromOneFrame(){
	//std::cerr << "popping..." << std::endl;
	while (buffertail==windowposition){
		if (!isReceiving)
			return "";
		usleep(1000);
	}
	frame curFrame=buffer[buffertail];
	bufferIsFilled[buffertail]=false;
	buffertail++;
	if (buffertail>=bufsize) buffertail=0;
	//std::cerr << "popping done" << std::endl;
	if (curFrame.withEndFile()) EndFileReceived=true;
	return curFrame.getData();
}

#define WAITBEFORECLOSEDURATION 2000000
receiver& receiver::operator>>(std::ostream& str){
	EndFileReceived=false;
	startReceiving();
	std::string curString = "";
	do{
		str<<curString;
		str.flush();
		curString=popStringFromOneFrame();
	}while (!curString.empty()?!EndFileReceived:true);
	usleep(WAITBEFORECLOSEDURATION);
	stopReceiving();
	closefd();
	return *this;
}

bool receiver::listen(int port){


	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		return false;

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *) &si_me, sizeof(si_me))==-1)
		return false;
	return true;

}

bool receiver::startReceiving(){
	pthread_t t;
	pthread_create(&t, NULL, receiving, (void*) this);
	isReceiving=true;
	return false;
}

bool receiver::stopReceiving(){
	isReceiving=false;
	return false;
}

bool receiver::closefd(){
	return close(sockfd);
}

void * receiving (void * callerobj){
	//std::cerr<< "test"<<std::endl;
	receiver * caller = (receiver*) callerobj;
	socklen_t slen=sizeof(caller->si_other);
	struct sockaddr sourceaddr;
	socklen_t addrlen;
	while (caller->isReceiving){
		char bufchar[MAX_FRAME_LENGTH];
		size_t msglen;
		//std::cerr<< "receiving a frame"<<std::endl;
		msglen=recvfrom(caller->sockfd, bufchar,MAX_FRAME_LENGTH,0,(struct sockaddr *) &caller->si_other,&slen);
		//std::cerr<< "received a frame"<<std::endl;
		frame curframe(bufchar,msglen);
		if (curframe.formatIsValid()){
			if (curframe.checksumIsValid()){

				//putting to windowed buffer
				if (caller->isInWindow(curframe.getFrameNumber()))
					caller->putframetowindowedbuffer(curframe);

				//sending ACK
				ackframe curack(ACK,curframe.getFrameNumber());
				//std::cerr<<"sending ACK"<<std::endl;
				//std::cerr.flush();
				size_t sentBytes=sendto(caller->sockfd, curack.getPointerToBytes(),curack.getBytesLength(),0,(struct sockaddr*) &caller->si_other ,slen);
				//std::cerr<<(sentBytes<=0?"error sending ACK":"ACK sent")<<std::endl;
				//std::cerr.flush();

				
			}else{
				//sending NAK
				ackframe curack(NAK,curframe.getFrameNumber());
				//std::cerr<< "received invalid frame"<<std::endl;
				size_t sentBytes=sendto(caller->sockfd, curack.getPointerToBytes(),curack.getBytesLength(),0,(struct sockaddr*) &caller->si_other ,slen);
			}
		}else{
			//std::cerr<< "received invalid frame"<<std::endl;
			//send NAK
			//TODO gimana caranya tahu nomor frame untuk send NAK kalau formatnya tidak valid?
		}
	}
	return NULL;
}

bool receiver::isInWindow(int frameNum){
	if (windowEnd()<windowposition)
		return (frameNum>=0&&frameNum<=windowEnd()) || (frameNum>=windowposition&&frameNum<=bufsize);
	else	return frameNum>=windowposition && frameNum<=windowEnd();
}

//prekondisi: isInWindow(f.getFrameNumber())
void receiver::putframetowindowedbuffer(const frame& f){
	int fpos=f.getFrameNumber();
	buffer[fpos]=f;
	bufferIsFilled[fpos]=true;
	while(bufferIsFilled[windowposition]){
		if(windowPastEnd()==buffertail) usleep(1000);
		geserWindow();
	}
	//std::cerr<<" put frame to buffer "<<fpos<<std::endl;
	//std::cerr<<" windowposition: "<<windowposition<<std::endl;
	
}

#endif

#endif
