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

class sender{
	public:
	sender& operator<<(std::string msg);
	sender& operator<<(std::istream& str);
	bool connect(const char* ip_str, int port);
	bool startSending();
	bool stopSending();
	bool isBufferFull();

	private:
	const int bufsize_sender = 10;
	
	frame buffer[bufsize];
	int windowposition=-1;
	const int windowsize=WINDOW_SIZE;
	int bufferhead=0; //menunjukkan posisi "kepala" buffer: Posisi frame terakhir yang di-buffer
	bool sent [bufsize];
	bool acked [bufsize];
		
	int framenumber = 0;
	const int charperframe = 5;

	//tambah variabel socket

	//fungsi yang dipanggil thread dijadikan friend
	friend void* /*mohon disesuaikan*/ sending (void * callerobj);
	friend void* /*mohon disesuaikan*/ listenack (void * callerobj);

	//kontrol thread
	int isSending=false;

	//fungsi helper
	void geserJendela(int n){windowposition=(windowposition+n)%bufsize_sender;}
	void geserBufferHead(){bufferhead=(bufferhead+1)%bufsize_sender;}
};

#endif
