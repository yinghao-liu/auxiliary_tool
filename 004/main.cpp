#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
/**
 * dirname and basename refer to ../005
 *
 *
 */
string dirname(string path)
{
	size_t location = path.find_last_of('/');
	if (path.size() == location + 1){
		return path.substr(0, path.find_last_of('/', location-1)+1);
	}
	if (string::npos == location){
		return ".";
	}
	return path.substr(0, path.find_last_of('/'));
}
string basename(string path)
{
	size_t location = path.find_last_of('/');
	if (0 == location && 1 == path.size()){
		return "/";
	}
	if (path.size() == location + 1){
		return path.substr(path.find_last_of('/', location-1)+1, location-1);
	}
	return path.substr(path.find_last_of('/')+1);
}
/**
 * path format\n
 * a/b  when b is a regular file or a directory\n
 * a/b/ when b is only a directory\n
 * a/\*  all regular file or directory in a
 *
 */
int rmrf(const string &path)
{
	DIR *dirp=nullptr;
	struct dirent *dp=nullptr;
	struct stat dir_stat;
	string path_name;
	string dir;
	string base;
	string path_c;
	dir = dirname(path);
	base = basename(path);
	if ("*" == base){
		path_c = dir;
	}else{
		path_c = path;
	}
	// 参数传递进来的目录不存在，直接返回
	if ( 0 != access(path_c.c_str(), F_OK) ) {
		printf("file %s : %s\n", path.c_str(), strerror(errno));
		return 0;
	}

	// 获取目录属性失败，返回错误
	if ( 0 > stat(path_c.c_str(), &dir_stat) ) {
		perror("get directory stat error");
		return -1;
	}

	if ( S_ISREG(dir_stat.st_mode) ) {  // 普通文件直接删除
		unlink(path_c.c_str());
	} else if ( S_ISDIR(dir_stat.st_mode) ) {   // 目录文件，递归删除目录中内容
		dirp = opendir(path_c.c_str());
		while ( (dp=readdir(dirp)) != NULL ) {
			// 忽略 .xx 隐藏文件
			if (0 == strncmp(".", dp->d_name, 1)){
				continue;
			}
			path_name = "";
			path_name = path_c + "/" + dp->d_name;
			rmrf(path_name.c_str());   // 递归调用
		}
		closedir(dirp);

		if ("*" != base){
			rmdir(path_c.c_str());     // 删除空目录
		}
	} else {
		perror("unknow file type!");    
	}

	return 0;
}
int main(void)
{
	//rmrf("kkk/");
	rmrf("kkk/*");
	return 0;
}
