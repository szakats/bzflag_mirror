#ifndef FTOA_H_
#define FTOA_H_

inline char* ftoa(float f) {
	char buff[30];
	sprintf(buff, "%f", f);
	return (char*)(string(buff).c_str());
}

#endif /*FTOA_H_*/
