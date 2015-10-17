
#ifndef SENDER_CPP
#define SENDER_CPP

#include "frame.h"
#include "dcomm.h"
#include "sender.h"

#include <cassert>

//kata nizam ada tiga thread: yang ngirim, yang nerima ack, dan yang consum

void* sending (void * callerobj){
	sender* senderobj = reinterpret_cast<sender*>(callerobj);
	if ( senderobj != nullptr ) {
		while ( senderobj->isSending ){
			for(int i=0;i<senderobj->windowsize;i++){
				int j = (senderobj->windowposition + i)%(senderobj->bufsize_sender);
				if (!senderobj->sent[j]){
					senderobj->sent[j] = false;
					size_t sentBytes=sendto(senderobj->s, senderobj->buffer[j].getPointerToBytes(),senderobj->buffer[j].getBytesLength(),0,(struct sockaddr*) &senderobj->si_other ,sizeof(senderobj->si_other));
				} else if (!senderobj->acked[j]){//tidak menerima ack atau menerima nak
					size_t sentBytes=sendto(senderobj->s, senderobj->buffer[j].getPointerToBytes(),senderobj->buffer[j].getBytesLength(),0,(struct sockaddr*) &senderobj->si_other ,sizeof(senderobj->si_other));
				}
			}
			usleep(100000);//delay
		}
	}
}

void* listenack (void * callerobj){
	sender* senderobj = reinterpret_cast<sender*>(callerobj);
	if ( senderobj != nullptr )
	{
	   while ( senderobj->isSending )
	   {
		//menerima ack
//		int x=recvfrom(senderobj->s, [ga tau], [ga tau], 0,(struct sockaddr *) &caller->si_other, [ga tau]);
		//if ack set ack jadi true
	   }
	}
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
	std::cout<<"zx"<<msg<<std::endl;
	frame f(framenumber, msg);
	//letakkan frame di buffer yang ke bufferHead
	buffer[bufferhead] = f;
	sent[bufferhead] = false;
	framenumber++;
	geserBufferHead();	
}

sender& sender::operator<<(std::istream& str){
	
	
	char c;	
	std::cout<<"xxx: "<<str<<std::endl;
	//membaca seluruh str dan menyimpan ke string
	while(str.get(c)){text += c;}
	text+=Endfile;
	startSending();
	//stopSending(); //harusnya dipanggil waktu baca EOF
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
	//set semua sent[buffersize] menjadi true
	//waktu frame dimasukkan ke buffer ke i, set sent[i] jadi false
	//jadi yang dikirim pasti yang sent nya bernilai false
	//buat menghindari ngirim frame kosong yang masuk bagian windows
	for(int i=0;i<bufsize_sender;i++){

	}
}

bool sender::stopSending(){
	isSending = false;
}


#endif
