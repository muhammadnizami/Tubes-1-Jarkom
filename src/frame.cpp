#ifndef FRAME_CPP
#define FRAME_CPP

#include "frame.h"
#include "dcomm.h"

#include <cassert>

//untuk membuat dari frameNumber dan data
//membuat frame dari frameNumber dan data sesuai format, termasuk menghitung checksum
frame::frame(unsigned int frameNumber, std::string data){
	bytes="";
	bytes+=SOH;	
	for(int i = 0; i < sizeof(frameNumber); i++) {
		bytes+=((unsigned char)(frameNumber >> (i * 8)));
	}
	bytes+=STX;
	bytes+=data;
	bytes+=ETX;
	bytes+=createChecksum(bytes);
}
const char* frame::getPointerToBytes() const{
	return bytes.c_str();
}
int frame::getBytesLength() const{
	return bytes.size();
}

//untuk mengoperasikan bytes
frame::frame(std::string _bytes){
	bytes=_bytes;
}
frame::frame(char * _bytes, size_t size):bytes(_bytes,size){
}
unsigned int frame::getFrameNumber() const{ //mengembalikan frame number. prekondisi: isValid();
	assert(isValid());
	return ((unsigned int)(unsigned char)bytes[4] << 24) | ((unsigned int)(unsigned char)bytes[3] << 16) | ((unsigned int)(unsigned char)bytes[2] << 8) | ((unsigned int)(unsigned char)bytes[1]);;
}
std::string frame::getData() const{ //mengembalikan data. prekondisi: isValid()
	assert(isValid());
	unsigned int STXpos=5;
	unsigned int ETXpos;
	for (ETXpos=bytes.length()-1;bytes[ETXpos]!=ETX;ETXpos--){}
	return bytes.substr(STXpos+1,ETXpos-STXpos-1);
}
std::string frame::getChecksum()const{ //mengembalikan checksum. prekondisi: formatIsValid()
	unsigned int ETXpos;
	for (ETXpos=bytes.length()-1;bytes[ETXpos]!=ETX;ETXpos--){}
	return bytes.substr(ETXpos+1,bytes.length()-ETXpos);
}
bool frame::formatIsValid() const{ //mengembalikan true bila format benar
	unsigned int SOHpos=0;
	unsigned int STXpos=5;
	unsigned int ETXpos;
	for (ETXpos=bytes.length()-1;bytes[ETXpos]!=ETX&&ETXpos!=STXpos;ETXpos--){}
	return bytes[SOHpos]==SOH && bytes[STXpos]==STX && bytes[ETXpos]==ETX;
}
bool frame::checksumIsValid() const{ //mengembalikan true bila checksum benar
	assert(formatIsValid());
	unsigned int ETXpos;
	for (ETXpos=bytes.length()-1;bytes[ETXpos]!=ETX;ETXpos--){}
	return checkChecksum(getChecksum(),bytes.substr(0,ETXpos+1));
}
bool frame::isValid() const{ //mengembalikan true bila format dan checksum benar
	if (formatIsValid()){
		return checksumIsValid();
	}else return false;
}

#endif

