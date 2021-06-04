#include <stdio.h>

unsigned int isbigend()
{
	int i = 1;

	return *((unsigned char *) &i) == 0;
}

unsigned int reversebo(unsigned int i)
{
	unsigned char c1;
	unsigned char c2;
	unsigned char c3;
	unsigned char c4;

	if (isbigend())
		return i;

	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;

	return ((unsigned int) c1 << 24) + ((unsigned int) c2 << 16) +
	    ((unsigned int) c3 << 8) + c4;
}
