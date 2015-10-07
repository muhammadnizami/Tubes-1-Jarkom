#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <string>

#define CHECKSUM_MAX_LENGTH 20

//bikin pake std hash function aja
//untuk mempermudah nanti di frame, checksum ini string [0-9]+

std::string createChecksum(const std::string&);

bool checkChecksum(const std::string& checksum, const std::string& checkedstr);

#endif
