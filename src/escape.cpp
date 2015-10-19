#ifndef escape_cpp
#define escape_cpp

#define MAXTOESCAPE 28 //eksklusif

#include "escape.h"

std::string replaceToEscape(const std::string& target){
	std::string retval="";
	int i=0;
	while (i<target.length()){
		if ((unsigned char)target[i]<MAXTOESCAPE){
			retval+=ESC;
			retval+=target[i]+MAXTOESCAPE;
		}else{
			retval+=target[i];
		}
		i++;
	}
	return retval;
}

std::string replaceNoEscape(const std::string& target){
	std::string retval="";
	int i=0;
	while (i<target.length()){
		if ((unsigned char)target[i]==ESC){
			i++;
			retval+=target[i]-MAXTOESCAPE;
		}else{
			retval+=target[i];
		}
		i++;
	}
	return retval;
}

bool isEscapeChar(unsigned char X){
	return X==ESC;
}

#endif
