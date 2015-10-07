#ifndef receiver_h
#define receiver_h

#include "dcomm.h"
#include "frame.h"
#include "ackframe.h"

#include <iostream>
#include <string>

class receiver{
	public:
	std::string popStringFromOneFrame();
	receiver& operator>>(std::stream str);
	bool listen(char* port);
	bool startReceiving();
	bool stopReceiving();

	private:
	const int bufsize = 1024;
	frame buffer[bufsize];
	int windowposition;
	const int windowsize=WINDOW_SIZE;
	int buffertail; //menunjukkan posisi "ekor" buffer: yang belum dikonsumsi. apabila buffertail==windowposition maka buffer belum dapat digunakan

	//tambah variabel socket

	//fungsi yang dipanggil thread dijadikan friend
	friend void* /*mohon disesuaikan*/ receiving (void * callerobj);

	//kontrol thread
	int isReceiving;
};

#endif
