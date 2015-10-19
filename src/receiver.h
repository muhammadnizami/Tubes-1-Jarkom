#ifndef receiver_h
#define receiver_h

#include "dcomm.h"
#include "frame.h"
#include "ackframe.h"

#include <iostream>
#include <string>
#include <arpa/inet.h>


class receiver{
	public:
	receiver():windowposition(0),buffertail(0),bufferIsFilled(){};
	std::string popStringFromOneFrame();
	receiver& operator>>(std::ostream& str);
	bool listen(int port);
	bool startReceiving();
	bool stopReceiving();
	bool closefd();

	private:
	frame buffer[bufsize];
	int windowposition;
	const int windowsize=WINDOW_SIZE;
	int buffertail; //menunjukkan posisi "ekor" buffer: yang belum dikonsumsi. apabila buffertail==windowposition maka buffer belum dapat digunakan
	bool bufferIsFilled[bufsize];

	//file control
	bool EndFileReceived;

	//tambah variabel socket
	int sockfd;
	struct sockaddr_in si_me, si_other;

	//fungsi yang dipanggil thread dijadikan friend
	friend void* /*mohon disesuaikan*/ receiving (void * callerobj);

	//kontrol thread
	int isReceiving=0;


	//fungsi helper
	void geserWindow(){windowposition=(windowposition+1)%bufsize;}
	int windowPastEnd(){return (windowposition+windowsize)%bufsize;}
	int windowEnd(){return (windowposition+windowsize-1)%bufsize;}
	bool isInWindow(int framenum);
	void putframetowindowedbuffer(const frame& f);
};

void * receiving (void * callerobj);

#endif
