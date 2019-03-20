#include "pic_op.h"
#include <stdio.h>
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
	pic_open_path("realbmp.bmp");
	return 0;
}
