#ifndef ACKFRAME_H
#define ACKFRAME_H

#include <string>
#include "dcomm.h"

class ackframe{
	public:
	//untuk buffer kosong
	ackframe():bytes(""){};

	//untuk membuat dari acktype dan frameNumber
	ackframe(char acktype, unsigned int frameNumber);
	const char* getPointerToBytes() const;
	int getBytesLength() const;

	//untuk mengoperasikan bytes
	ackframe(const std::string& bytes);
	ackframe(const char * bytes, size_t size);
	unsigned int getFrameNumber() const; //mengembalikan frame number. prekondisi: isValid()
	char getacktype() const; //mengembalikan byte pertama bytes. prekondisi: isValid()
	bool isACK() const; //getacktype==ACK
	bool isNAK() const; //getacktype==NAK
	std::string getChecksum() const; //mengembalikan checksum. prekondisi: formatIsValid()
	bool formatIsValid() const; //mengembalikan true bila format benar
	bool checksumIsValid() const; //mengembalikan true bila checksum benar
	bool isValid() const; //mengembalikan true bila format dan checksum benar

	private:
	std::string bytes;
};

#endif
