#if __cplusplus <= 199711L
  #error Perlu support c++11. Coba tambahkan -std=c++11 sebagai tag kompilasi.
#else

#ifndef CHECKSUM_CPP
#define CHECKSUM_CPP

#include "checksum.h"
#include <sstream>
#include <functional>


//SALAH
//harus diganti jadi yang CRC
//lihat https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks

//bikin pake std hash function aja
//saat kompilasi tambahkan -std=c++11
//formatnya harus sesuai antara createChecksum dan checkChecksum

std::hash<std::string> hash_fn;

std::string createChecksum(const std::string& in){
	std::ostringstream ostr;
	ostr<<hash_fn(in);
	return ostr.str();
}

bool checkChecksum(const std::string& checksum, const std::string& checkedstr){
	return checksum==createChecksum(checkedstr);
}

#endif

#endif
