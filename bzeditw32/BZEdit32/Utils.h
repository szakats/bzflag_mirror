// utils.h

#ifndef _UTILS_H_
#define _UTILS_H_

void LogLine ( char* szLine );
bool GetLine( char** data, char* line );
bool GetNextElement ( char** pData, char* pElemnt, char* pName );

void CallPlugin ( int iID );
#endif //_UTILS_H_