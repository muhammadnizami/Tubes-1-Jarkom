#include "ackframe.h"
#include <cstring>
#include <cassert>
#include <iostream>

int main(){
	ackframe A(ACK,10493);

	char Abytes[128];
	char Bbytes[128];
	int Alength=A.getBytesLength();
	int Blength=A.getBytesLength()-1;

	memcpy(Abytes,A.getPointerToBytes(),Alength);
	memcpy(Bbytes,A.getPointerToBytes(),Blength);

	ackframe A1(Abytes,Alength);
	assert(A1.formatIsValid());
	assert(A1.checksumIsValid());
	assert(A1.isValid());
	assert(A1.checksumIsValid());
	assert(A1.isValid()==true);
	assert(A1.isACK());
	assert(A.getFrameNumber()==10493);
	assert(A1.getFrameNumber()==10493);

	Abytes[2]='a';
	ackframe A2(Abytes,Alength);
	assert(A2.formatIsValid());
	assert(!A2.checksumIsValid());
	assert(!A2.isValid());

	Abytes[0]='a';
	ackframe A3(Abytes,Alength);
	assert(!A3.formatIsValid());

	ackframe B(Bbytes,Blength);
	assert(B.formatIsValid());
	assert(!B.checksumIsValid());
	
}
