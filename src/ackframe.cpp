#ifndef FRAME_CPP
#define FRAME_CPP

#include "ackframe.h"
#include "checksum.h"
#include "dcomm.h"
#include <cassert>

//struktur ackframe: ACK  -  FrameNum(4-byte Little Endian)  -  Checksum

#include<iostream>
//untuk membuat dari acktype dan frameNumber
ackframe::ackframe(char acktype, unsigned int frameNumber){
	assert(acktype==ACK || acktype==NAK);
	bytes="";
	bytes+=acktype;
	for(int i = 0; i < sizeof(frameNumber); i++) {
		bytes+=((unsigned char)(frameNumber >> (i * 8)));
	}
	bytes+=createChecksum(bytes);
}
const char* ackframe::getPointerToBytes() const{
	return bytes.c_str();
}
int ackframe::getBytesLength() const{
	return bytes.size();
}

//untuk mengoperasikan bytes
ackframe::ackframe(const std::string& _bytes){
	bytes=_bytes;
}
ackframe::ackframe(const char * _bytes, size_t size):bytes(_bytes,size){
}
unsigned int ackframe::getFrameNumber() const{
	assert(isValid());
	return ((unsigned int)(unsigned char)bytes[4] << 24) | ((unsigned int)(unsigned char)bytes[3] << 16) | ((unsigned int)(unsigned char)bytes[2] << 8) | ((unsigned int)(unsigned char)bytes[1]);;
} //mengembalikan frame number. prekondisi: isValid()

char ackframe::getacktype() const{
	assert(isValid());
	return bytes[0];
} //mengembalikan byte pertama bytes. prekondisi: isValid()

bool ackframe::isACK() const{
	return getacktype()==ACK;
} //getacktype==ACK
bool ackframe::isNAK() const{
	return getacktype()==NAK;
} //getacktype==NAK
std::string ackframe::getChecksum() const{
	assert(formatIsValid());
	return bytes.substr(5,bytes.length()-4);
} //mengembalikan checksum. prekondisi: formatIsValid()
bool ackframe::formatIsValid() const{
	return (bytes[0]==NAK||bytes[0]==ACK) && bytes.length()>5;
} //mengembalikan true bila format benar
bool ackframe::checksumIsValid() const{
	return checkChecksum(getChecksum(),bytes.substr(0,5));
} //mengembalikan true bila checksum benar
bool ackframe::isValid() const{
	if (formatIsValid()){
		return checksumIsValid();
	}else return false;
} //mengembalikan true bila format dan checksum benar

#endif
