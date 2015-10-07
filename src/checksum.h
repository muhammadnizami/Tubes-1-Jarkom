#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <string>

//bikin pake std hash function aja

std::string createChecksum(std::string);

bool checkChecksum(std::string checksum, std::string checkedstr);

#endif
