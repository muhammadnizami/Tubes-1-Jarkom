#ifndef sender_h
#define sender_h

#include "dcomm.h"
#include "frame.h"
#include "ackframe.h"

#include <iostream>
#include <string>

class sender{
	public:
	sender& operator<<(std::string msg);
	sender& operator<<(std::stream str);
	bool connect(char* ip_str, char* port);
	bool startSending();
	bool stopSending();

	private:
	const int bufsize = 1024;
	frame buffer[bufsize];
	int windowposition;
	const int windowsize=WINDOW_SIZE;
	int bufferhead; //menunjukkan posisi "kepala" buffer: Posisi frame terakhir yang di-buffer
	bool sent [bufsize];
	bool acked [bufsize];

	//tambah variabel socket

	//fungsi yang dipanggil thread dijadikan friend
	friend void* /*mohon disesuaikan*/ sending (void * callerobj);
	friend void* /*mohon disesuaikan*/ listenack (void * callerobj);

	//kontrol thread
	int isSending;

	//fungsi helper
	void geserJendela(int n);
};

#endif
