#ifndef FRAME_H
#define FRAME_H

#include "checksum.h"

#define MAX_FRAME_LENGTH 65507
#define MAX_FRAME_DATA_LENGTH (MAX_FRAME_LENGTH-CHECKSUM_MAX_LENGTH)
#include <string>

class frame{
	public:
	//untuk buffer kosong
	frame():bytes(""){};

	//untuk membuat dari frameNumber dan data
	//membuat frame dari frameNumber dan data sesuai format, termasuk menghitung checksum
	frame(unsigned int frameNumber, std::string data);
	const char* getPointerToBytes() const;
	int getBytesLength() const;

	//untuk mengoperasikan bytes
	frame(std::string bytes);
	frame(char * bytes, size_t size);
	unsigned int getFrameNumber() const; //mengembalikan frame number. prekondisi: isValid();
	std::string getData() const; //mengembalikan data. prekondisi: isValid();
	std::string getChecksum() const; //mengembalikan checksum. prekondisi: formatIsValid();
	bool formatIsValid() const; //mengembalikan true bila format benar
	bool checksumIsValid() const; //mengembalikan true bila checksum benar
	bool isValid() const; //mengembalikan true bila format dan checksum benar

	private:
	std::string bytes;
};

#endif
