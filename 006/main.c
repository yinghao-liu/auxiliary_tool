#include "pic_op.h"
#include <stdio.h>
#include <stdlib.h>
#define FFALIGN(x, a) (((x)+(a)-1)&~((a)-1))

int main(void)
{
	/*
	int x = 0;
	int a = 0;
	x=1;
	for (a=0; a<100; a++){
		printf("x is %d, a is %d: %d\n", x, a, FFALIGN(x, a)) ;
	}*/

	pic_open_path("realjpg.jpg");

	FILE* fd = fopen("realjpg.jpg", "r+");
	if (NULL == fd) {
		printf("fopen file error\n");
		return -1;
	}
	fseek(fd, 0, SEEK_END);
	int file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	char *data = NULL;
	data = (char *)malloc(file_size);
	if (file_size != fread(data, 1, file_size, fd)) {
		printf("fread file error:\n");
		free(data);
		fclose(fd);
		return -1;
	}
	pic_open_data(data, file_size);
	free(data);
	fclose(fd);
	return 0;
}
