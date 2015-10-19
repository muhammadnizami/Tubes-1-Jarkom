#ifndef CHECKSUM_CPP
#define CHECKSUM_CPP

#include "checksum.h"
#include <string>
#include <cctype>
#include <stdint.h>

uint32_t generatorPolynomial (0x814141AB);//CRC-32Q
uint32_t checkZero (0x1);

//fungsi ini baru bisa dipakai jika MSB generatorPolynomial bernilai 1
uint32_t crccompute(const std::string& in){
	uint32_t tmpRemainingXOR (0);
	uint32_t curUint32(0);
	int i=0;
	while(i<in.length()%4){
		curUint32<<=8;
		curUint32+=(uint8_t)in[i];
		i++;
	}
	if (in.length()>=4)
	for (int j=0;j<32;j++){
		if (curUint32&(checkZero<<(32-j))){
			curUint32^=generatorPolynomial>>j;
			tmpRemainingXOR^=generatorPolynomial<<(32-j);
		}
	}
	while(i<(int)in.length()){
		curUint32 = 0;
		for (int ii=0;ii<4;ii++){
			curUint32<<=8;
			curUint32+=(uint8_t)in[i];
			i++;
		}
		curUint32^=tmpRemainingXOR;
		tmpRemainingXOR = 0;		
		for (int j=0;j<32;j++){
			if (curUint32&(checkZero<<(32-j))){
				curUint32^=generatorPolynomial>>j;
				tmpRemainingXOR^=generatorPolynomial<<(32-j);
			}
		}
	}
	curUint32 = 0;
	curUint32^=tmpRemainingXOR;
	return curUint32;
}


std::string createChecksum(const std::string& in){
	uint32_t rrr=crccompute(in);
	std::string retval="";
	for(int i = 0; i < sizeof(rrr); i++) {
		retval+=((unsigned char)(rrr >> (i * 8)));
	}
	return retval;
}

bool checkChecksum(const std::string& checksum, const std::string& checkedstr){
	return checksum==createChecksum(checkedstr);
}

#endif

