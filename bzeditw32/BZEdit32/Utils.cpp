// utils.cpp

#include "Utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//loger
void LogLine ( char* szLine )
{
	char cr[3] = {10,13,0};

	if (!szLine)
		return;

	FILE	*fpLog = fopen ("log.txt","at");

	if(!fpLog)
		return;

	fprintf(fpLog,"%s\n",szLine);
	fclose(fpLog);
}

// util funcs
bool GetLine( char** data, char* line )
{
	line[0] = NULL;
	char	*pPtr = *data, *pOut = line;

	while (*pPtr != '\n')
	{
		*pOut= *pPtr;
		if (*pPtr == NULL)
			return false;

		pPtr++;
		pOut++;
	}
	pPtr++;
	pOut;

	*data = pPtr;
	*pOut = NULL;

	// trim any trailing spaces

	for ( int i = strlen(line)-1; i >= 0;i--)
	{
		if (isspace(line[i]))
			line[i] = NULL;
		else
			i = -1;
	}

	return true;
}

bool GetNextElement ( char** pData, char* pElemnt, char* pName )
{
	if (!pData || !pElemnt)
		return false;
	
	pElemnt[0] = NULL;

	char	*pIn = *pData,*pOut = pElemnt,line[255];

	bool bDone = false,bEOF = false;

	while (!bDone)
	{
		if (!GetLine(&pIn,line))
		{
			bDone = true;
			bEOF = true;
		}
		switch (line[0])
		{
			case '\n':
			case '\t':
			case ' ':
			case NULL:
				break;

			case '#':
				if (pElemnt[0]== NULL) 
					break;

				if (line[1]!= '!')
					break;
			default:
				strcat(pElemnt,line);
				
				if (stricmp(line,"end")==0)
					bDone = true;

				strcat(pElemnt,"\n");
				break;
		}
	}

	*pData = pIn;
	return bEOF;
}