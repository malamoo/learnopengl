#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/endian.h"

unsigned char *loadimg(const char *filename)
{
	FILE *fp;
	unsigned char *chunkdata = NULL;
	unsigned int chunklen;
	unsigned int chunktype;
	unsigned int ihdr = 0x49484452;
	unsigned int idat = 0x49444154;
	unsigned int iend = 0x49454E44;
	unsigned int width;
	unsigned int height;
	unsigned char bitdepth;
	unsigned char colortype;
	unsigned long png = 0x0A1A0A0D474E5089;
	unsigned long filesig;

	fp = fopen(filename, "r");

	fread(&filesig, sizeof(unsigned long), 1, fp);
	if (filesig != png) {
		printf("PNG loader error: file format is not PNG.\n");
		exit(EXIT_FAILURE);
	}

	fread(&chunklen, sizeof(unsigned int), 1, fp);
	fread(&chunktype, sizeof(unsigned int), 1, fp);
	chunklen = reversebo(chunklen);
	chunktype = reversebo(chunktype);
	if (chunktype != ihdr) {
		printf("PNG loader error: IHDR chunk not present.\n");
		exit(EXIT_FAILURE);
	}
	fread(&width, sizeof(unsigned int), 1, fp);
	fread(&height, sizeof(unsigned int), 1, fp);
	fread(&bitdepth, sizeof(unsigned char), 1, fp);
	fread(&colortype, sizeof(unsigned char), 1, fp);
	width = reversebo(width);
	printf("width = %d\n", width);
	height = reversebo(height);
	printf("height = %d\n", height);
	printf("bitdepth = %d\n", bitdepth);
	printf("colortype = %d\n", colortype);
	if (colortype != 2 || bitdepth != 8) {
		printf
		    ("PNG loader error: disallowed color type and bit depth \
                     combination.\n");
		exit(EXIT_FAILURE);
	}
	fseek(fp, 3, SEEK_CUR);
	fseek(fp, 4, SEEK_CUR);
	unsigned int read = 0;
	unsigned int totchunklen = 0;

	do {
		fread(&chunklen, sizeof(unsigned int), 1, fp);
		fread(&chunktype, sizeof(unsigned int), 1, fp);
		chunklen = reversebo(chunklen);
		chunktype = reversebo(chunktype);
		if (chunktype == idat) {
			totchunklen += chunklen;
			chunkdata = realloc(chunkdata, read + chunklen);
			read +=
			    fread(chunkdata + read, sizeof(unsigned char),
				  chunklen, fp);
			fseek(fp, 4, SEEK_CUR);
		} else {
			fseek(fp, chunklen + 4, SEEK_CUR);
		}
	} while (chunktype != iend);

	printf("read = %d\n", read);
	printf("totalchunklen = %d\n", totchunklen);
	fclose(fp);

	return chunkdata;
}
