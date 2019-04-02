#include "pic_op.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#define FFALIGN(x, a) (((x)+(a)-1)&~((a)-1))

int filterpictype(const struct dirent *filterdirent)
{
	size_t len = strlen(filterdirent->d_name);
	if ((0 == strncmp(filterdirent->d_name+len-4, ".jpg", 4)) \
			|| (0 == strncmp(filterdirent->d_name+len-5, ".jpeg", 5))) {
		return 1;
	}
	return 0;
}
int pic_open_data_wrap(const char * file)
{
	struct stat file_stat;
	if (0 != stat(file, &file_stat)) {
		perror("stat");
		return -1;
	}
	size_t file_size = file_stat.st_size;
	char *buffer = NULL;
	buffer = (char *)malloc(file_size);
	if (NULL == buffer) {
		perror("malloc");
	}
	int fd = open(file, O_RDONLY);
	if (-1 == fd) {
		printf("open file %s error: %s\n", file, strerror(errno));
		free(buffer);
		return -1;
	}

	if (file_size != read(fd, buffer, file_size)) {
		printf("read file %s error: %s\n", file, strerror(errno));
		free(buffer);
		close(fd);
		return -1;
	}
	pic_open_data(buffer, file_size);
	free(buffer);
	close(fd);
}

int main(void)
{
	/*
	int x = 0;
	int a = 0;
	x=1;
	for (a=0; a<100; a++){
		printf("x is %d, a is %d: %d\n", x, a, FFALIGN(x, a)) ;
	}*/
	/*******************pic_open_path************************/
	pic_open_path("realjpg.jpg");

	/*******************pic_open_data************************/
	pic_open_data_wrap("realjpg.jpg");

	/*******************loop***********************/
	struct dirent **name_list;
	const char *database_dir = "database_raw";
	int ret = 0;
	ret = scandir(database_dir, &name_list, filterpictype, alphasort);
	if (-1 == ret) {
		printf("scandir:%s failed, err:%s\n", database_dir, strerror(errno));
		return -1;
	}
	int i=0;
	char file_path[512]={0};
	while (ret--) {
		snprintf(file_path, sizeof (file_path), "%s/%s", database_dir, name_list[ret]->d_name);
		pic_open_data_wrap("realjpg.jpg");
		//pic_open_path(file_path);
		//usleep(100 * 1000); // 100ms
		printf("-------------------%d:%s-----------------\n", i++, file_path);
		free(name_list[ret]);
	}
	free(name_list);
	return 0;
}
