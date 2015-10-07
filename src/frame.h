#ifndef FRAME_H
#define FRAME_H

#define MAX_FRAME_LENGTH 65507

#include <string>

class frame{
	public:
	//untuk buffer kosong
	frame():bytes(""){};

	//untuk membuat dari frameNumber dan data
	//membuat frame dari frameNumber dan data sesuai format, termasuk menghitung checksum
	frame(int frameNumber, std::string data);
	char* getPointerToBytes();
	int getBytesLength();

	//untuk mengoperasikan bytes
	frame(std::string bytes);
	frame(char * bytes, int size);
	int getFrameNumber(); //mengembalikan frame number. prekondisi: isValid();
	std::string getData(); //mengembalikan data. prekondisi: isValid();
	std::string getChecksum(); //mengembalikan checksum. prekondisi: formatIsValid();
	bool formatIsValid(); //mengembalikan true bila format benar
	bool checksumIsValid(); //mengembalikan true bila checksum benar
	bool isValid(); //mengembalikan true bila format dan checksum benar

	private:
	std::string bytes;
};

#endif
