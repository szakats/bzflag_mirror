#ifndef FTOA_H_
#define FTOA_H_

#include <string>
#include "TextUtils.h"

using namespace std;

inline string ftoa(float f) {
	return TextUtils::format("%f", f);
}

inline string itoa(int i) {
	return TextUtils::format("%i", i);	
}

#endif /*FTOA_H_*/
