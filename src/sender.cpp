
#ifndef SENDER_CPP
#define SENDER_CPP

#include "frame.h"
#include "dcomm.h"
#include "sender.h"

#include <cassert>

//kata nizam ada tiga thread: yang ngirim, yang nerima ack, dan yang consum

void* sending (void * callerobj){
	std::cout<<"sending thread online"<<std::endl;
	static const float nonstricttimeoutseconds=0.3;
	sender* senderobj = reinterpret_cast<sender*>(callerobj);
	if ( senderobj != nullptr ) {
		while ( senderobj->isSending || !senderobj->bufferIsEmpty()){
			for(int i=0;i<senderobj->windowsize;i++){
				int j = (senderobj->windowposition + i)%(senderobj->bufsize_sender);
				if (senderobj->buffered[j])
				if (!senderobj->sent[j]){
					size_t sentBytes=sendto(senderobj->s, senderobj->buffer[j].getPointerToBytes(),senderobj->buffer[j].getBytesLength(),0,(struct sockaddr*) &senderobj->si_other ,sizeof(senderobj->si_other));
					senderobj->sent[j] = true;
					senderobj->timesent[j]=clock();
					std::cout<<"sent frame "<<senderobj->buffer[j].getFrameNumber()<<std::endl;
				} else if (senderobj->sent[j] && !senderobj->acked[j]
					&& (float(clock()-senderobj->timesent[j]))/CLOCKS_PER_SEC>nonstricttimeoutseconds){//tidak menerima ack atau menerima nak setelah timeout
					size_t sentBytes=sendto(senderobj->s, senderobj->buffer[j].getPointerToBytes(),senderobj->buffer[j].getBytesLength(),0,(struct sockaddr*) &senderobj->si_other ,sizeof(senderobj->si_other));
					senderobj->sent[j] = true;
					senderobj->timesent[j]=clock();
					std::cout<<"timeout, retry, sent frame "<<senderobj->buffer[j].getFrameNumber()<<std::endl;
				} else if (j==senderobj->windowposition && senderobj->sent[j] && senderobj->acked[j]){
					senderobj->geserJendela(1);
					senderobj->sent[j]=false;
					senderobj->acked[j]=false;
					senderobj->buffered[j]=false;
				}
			}
		}
	}
	std::cout<<"sending thread offline"<<std::endl;
}

void* listenack (void * callerobj){
	std::cout<<"listenack thread online"<<std::endl;
	sender* senderobj = reinterpret_cast<sender*>(callerobj);
	if ( senderobj != nullptr )
	{
		socklen_t slen=sizeof(senderobj->si_other);
		while ( senderobj->isSending || !senderobj->bufferIsEmpty())
		{
			//menerima ack
			char bufchar[128];
			int ackframelen=recvfrom(senderobj->s, bufchar, 128, 0,(struct sockaddr *) &senderobj->si_other, &slen);
			ackframe AF(bufchar,ackframelen);
			if (AF.isValid()){
				int framenum=AF.getFrameNumber();
				std::cout<<(AF.isACK()?"ACK ":"NAK ")<<framenum<<std::endl;
				if (senderobj->sent[framenum])
					if (AF.isACK())
						senderobj->acked[framenum]=true;
					else
						senderobj->sent[framenum]=false;
			}else{
				std::cout<<"invalid ack received"<<std::endl;
			}
		}
	}
	std::cout<<"listenack thread offline"<<std::endl;
}

void* consume (void * callerobj){
	std::string curStr;
	sender* senderobj = reinterpret_cast<sender*>(callerobj);
	if ( senderobj != nullptr ){
		int count=0;
		if(senderobj->text.length()>0){
			do{	
				if(!senderobj->isBufferFull){//kalau buffer tidak penuh				
					curStr+=senderobj->text[count];
					count++;
					if(((count % senderobj->charperframe) == 0) || (count==senderobj->text.length())){
						*senderobj<<curStr;
						curStr = "";
					}
				} else {
					//wait
				}
			} while (count<senderobj->text.length());
		}	
	}
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
	std::cout<<"buffering "<<msg<<std::endl;
	frame f(bufferhead, msg);
	//wait
	while(isBufferFull)usleep(100000);
	//letakkan frame di buffer yang ke bufferHead
	buffer[bufferhead] = f;
	sent[bufferhead] = false;
	acked[bufferhead] = false;
	buffered[bufferhead] = true;
	geserBufferHead();	
}

sender& sender::operator<<(std::istream& str){
	
	
	char c;	
//	std::cout<<"xxx: "<<str<<std::endl;
	//membaca seluruh str dan menyimpan ke string
	startSending();
	std::string msg="";
	while(str.get(c)){
		msg+=c;
		if (msg.length()>=charperframe){
			*this<<msg;
			msg="";
		}
	}
	msg+=Endfile;
	*this<<msg;
	
	stopSending(); //harusnya dipanggil waktu baca EOF
	return *this;
}


bool sender::connect(const char* ip_str, int port){
	bool connected=false;
	int i, slen=sizeof(si_other);

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
	isSending=true;
	if (pthread_create(&sending_tr,NULL,sending,this)) pthread_diep("error creating sending thread");
	if (pthread_create(&listenack_tr,NULL,listenack,this)) pthread_diep("error creating listenack thread");
	//if (pthread_create(&consume_tr,NULL,consume,this)) pthread_diep("error creating consume thread");
}

bool sender::stopSending(){
	isSending = false;
	pthread_join(listenack_tr,NULL);
	pthread_join(sending_tr,NULL);
}


#endif
