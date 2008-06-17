#include <string.h>

#ifndef StringHelper_h

#define StringHelper_h

class StringHelper
{
	public:
		static int FindFirstOcurrence(char * line, int len, int offset, char character);
		static char * Substring(char * str, int begin, int length);
		static char * Trim(char * str);
		static char * GetEmptyString();
		static char * AppendString(char * str1, char * str2);
};

#endif