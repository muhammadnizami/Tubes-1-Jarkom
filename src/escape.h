#ifndef escape_h
#define escape_h

//HARUS MEMASTIKAN seluruh karakter setelah di-replace tidak ada yang merupakan control character kecuali escape character

#include "dcomm.h"

#include <string>

std::string replaceToEscape(const std::string& target);

std::string replaceNoEscape(const std::string& target);

bool isEscapeChar(unsigned char X);

#endif
