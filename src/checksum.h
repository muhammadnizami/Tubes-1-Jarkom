#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <string>

//bikin pake std hash function aja

std::string createChecksum(const std::string&);

bool checkChecksum(const std::string& checksum, const std::string& checkedstr);

#endif
