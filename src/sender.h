	#ifndef sender_h
#define sender_h

#include "dcomm.h"
#include "frame.h"
#include "ackframe.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

class sender{
	public:
	sender& operator<<(std::string msg);
	sender& operator<<(std::istream& str);
	bool connect(const char* ip_str, int port);
	bool startSending();
	bool stopSending();
	bool closefd();

	private:
	static const int bufsize_sender = bufsize;
	
	frame buffer[bufsize_sender];
	int windowposition=0;
	const int windowsize=WINDOW_SIZE;
	int bufferhead=0; //menunjukkan posisi "kepala" buffer: Posisi frame terakhir yang di-buffer
	bool sent [bufsize_sender] = { 0 };
	bool acked [bufsize_sender] = { 0 };
	bool buffered [bufsize_sender] = { 0 };
	clock_t timesent [bufsize_sender];

	bool isBufferFull=false;

	int s;
	struct sockaddr_in si_other;

	std::string text;

	const int charperframe = PREFERREDFRAMEDATASIZE;

	//tambah variabel socket

	//fungsi yang dipanggil thread dijadikan friend
	friend void* /*mohon disesuaikan*/ sending (void * callerobj);
	friend void* /*mohon disesuaikan*/ listenack (void * callerobj);
	friend void* /*mohon disesuaikan*/ consume (void * callerobj);
	//kontrol thread
	pthread_t sending_tr, listenack_tr, consume_tr;
	bool isSending=false;

	//fungsi helper
	void geserJendela(int n){windowposition=(windowposition+n)%bufsize_sender;isBufferFull=false;}
	void geserBufferHead(){bufferhead=(bufferhead+1)%bufsize_sender;if(bufferhead==windowposition){isBufferFull=true;}}
	bool bufferIsEmpty(){for (int i=0;i<bufsize_sender;i++) if (buffered[i])return false; return true;}

};

#endif
