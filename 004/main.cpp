#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

int rmrf(const char *path)
{
	DIR *dirp=nullptr;
	struct dirent *dp=nullptr;
	struct stat dir_stat;
	std::string path_name;

	// 参数传递进来的目录不存在，直接返回
	if ( 0 != access(path, F_OK) ) {
		printf("file %s : %s\n", path, strerror(errno));
		return 0;
	}

	// 获取目录属性失败，返回错误
	if ( 0 > stat(path, &dir_stat) ) {
		perror("get directory stat error");
		return -1;
	}

	if ( S_ISREG(dir_stat.st_mode) ) {  // 普通文件直接删除
		unlink(path);
	} else if ( S_ISDIR(dir_stat.st_mode) ) {   // 目录文件，递归删除目录中内容
		dirp = opendir(path);
		while ( (dp=readdir(dirp)) != NULL ) {
			// 忽略 . 和 ..
			if ( (0 == strcmp(".", dp->d_name)) || (0 == strcmp("..", dp->d_name)) ) {
				continue;
			}
			path_name = "";
			path_name = path_name + path + "/" + dp->d_name;
			rmrf(path_name.c_str());   // 递归调用
		}
		closedir(dirp);

		rmdir(path);     // 删除空目录
	} else {
		perror("unknow file type!");    
	}

	return 0;
}
int main(void)
{
	rmrf("kkk");
	return 0;
}
