
#ifndef SENDER_CPP
#define SENDER_CPP

#include "frame.h"
#include "dcomm.h"
#include "sender.h"

#include <cassert>

//kata nizam ada tiga thread: yang ngirim, yang nerima ack, dan yang consumer

void* sending (void * callerobj){
	
}

void* listenack (void * callerobj){

}

void diep(const char *s)
{
	perror(s);
	exit(1);
}

void pthread_diep(const char *s)
{
	perror(s);
	pthread_exit(NULL);
}

sender& sender::operator<<(std::string msg){
/* Membuat frame dan memasukkannya ke buffer */
/* Prekondisi: Buffer tidak penuh */
	frame f(framenumber, msg);
	//letakkan frame di buffer yang ke bufferHead
	buffer[bufferhead] = f;
	geserBufferHead();	
}

sender& sender::operator<<(std::istream& str){
	
	startSending();
	std::string text, curStr;
	char c;	
	//membaca seluruh str dan menyimpan ke string
	while(str.get(c)){text += c;}
	std::cout<<text;
	int count=0;
	if(text.length()>0){
		do{	
			if(!(bufferhead==windowposition)){				
				curStr+=text[count];
				count++;
				if(((count % charperframe) == 0) || (count==text.length())){
					*this<<curStr;
					curStr = "";
				}
			} else {
				//wait
			}
		} while (count<text.length());
	}
	stopSending();
	return *this;
}


bool sender::connect(const char* ip_str, int port){
	bool connected=false;
	struct sockaddr_in si_other;
	int s, i, slen=sizeof(si_other);

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	if (inet_aton(ip_str, &si_other.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
	} else {
		connected = true;
	}
	
}


bool sender::startSending(){
	isSending = true;
	pthread_t tSending;
	pthread_t tLitsenAck;
	pthread_create(&tSending, NULL, &sending, this);
    	pthread_create(&tLitsenAck, NULL, &listenack, this);

        pthread_join(tSending, NULL);
	pthread_join(tLitsenAck, NULL);

}

bool sender::stopSending(){
	isSending = false;
}


#endif
