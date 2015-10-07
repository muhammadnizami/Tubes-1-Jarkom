#ifndef FRAME_H
#define FRAME_H

#define MAX_FRAME_LENGTH 65507

#include <string>
#include "dcomm.h"

class frame{
	public:
	//untuk buffer kosong
	frame():bytes(""){};

	//untuk membuat dari acktype dan frameNumber
	frame(char acktype, int frameNumber);
	char* getPointerToBytes();
	int getBytesLength();

	//untuk mengoperasikan bytes
	frame(std::string bytes);
	frame(char * bytes, int size);
	int getFrameNumber(); //mengembalikan frame number
	char getacktype(); //mengembalikan byte pertama bytes. prekondisi: isValid()
	bool isACK(); //getacktype==ACK
	bool isNAK(); //getacktype==NAK
	std::string getChecksum(); //mengembalikan checksum. prekondisi: formatIsValid();
	bool formatIsValid(); //mengembalikan true bila format benar
	bool checksumIsValid(); //mengembalikan true bila checksum benar
	bool isValid(); //mengembalikan true bila format dan checksum benar

	private:
	std::string bytes;
};

#endif
