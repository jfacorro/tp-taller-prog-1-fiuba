#include "StringHelper.h"

#ifndef StringHelper_cpp

#define StringHelper_cpp

#define ISSPACE_TAB_NEWLINE(x) x == ' ' || x == '\t' || x == '\n'

int StringHelper::FindFirstOcurrence(char * line, int len, int offset, char character)
{
	int i;

	for ( i = offset ; i < len ; i++ )
		if ( line[i] == character )
		{
			return i;
		}

	return -1;
}

char * StringHelper::Substring(char * str, int begin, int length)
{
	char * subStr = new char[length + 1];

	for(int index = begin; index < begin + length; index++)
	{
		subStr[index - begin] = str[index];
	}

	subStr[length] = '\0';

	return subStr; 
}

void StringHelper::Copy(char * src, char * dst, int length)
{
	for(int index = 0; index < length; index++)
	{
		dst[index] = src[index];
	}

    dst[length] = '\0';
}

char * StringHelper::Trim(char * str)
{
	int length = strlen(str);
	int index = 0;
	int beginIndex = 0;

	while
	(
		ISSPACE_TAB_NEWLINE(str[index])
	) index++;

	beginIndex = index;

	if(length > 0 && beginIndex != length)
	{
		index = length - 1;

		while(ISSPACE_TAB_NEWLINE(str[index])) index--;

		return StringHelper::Substring(str, beginIndex, index - beginIndex + 1);
	}
	else
	{
		return StringHelper::GetEmptyString();
	}
}

char * StringHelper::GetEmptyString()
{
	char * emptyStr = new char[1];
	emptyStr[0] = '\0';
	return emptyStr;
}

char * StringHelper::AppendString(char * str1, char * str2)
{
  char * str3;
  int length = strlen(str1) + strlen(str2) + 1;

  str3 = new char[length];

  str3[0] = '\0';

  strcat(str3, str1);
  strcat(str3, str2);
  
  return str3;
}


#endif