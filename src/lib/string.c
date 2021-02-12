#include <string.h>
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
int isdigit(char c) {
  return c >= '0' && c <= '9';
}
 
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

// Function to implement strcmp function
int strcmp(const char *X, const char *Y)
{
    while(*X)
    {
        // if characters differ or end of second string is reached
        if (*X != *Y)
            break;
 
        // move to next pair of characters
        X++;
        Y++;
    }
 
    // return the ASCII difference after converting char* to unsigned char*
    return *(const unsigned char*)X - *(const unsigned char*)Y;
}
