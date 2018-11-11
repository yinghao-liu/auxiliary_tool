/*
 * Copyright (C) 2018 francis_hao <francis_hao@126.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "../005/parse_pathname.h"
#include "rmrf.h"
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
			if ("*" == base){
				// 忽略 .xx 隐藏文件
				if (0 == strncmp(".", dp->d_name, 1)){
					continue;
				}
			}else{
				// 忽略 . 和 ..
				if (0 == strcmp(".", dp->d_name) || 0 == strcmp("..", dp->d_name)){
					continue;
				}
			}
			path_name = "";
			path_name = path_c + "/" + dp->d_name;
			rmrf(path_name);   // 递归调用
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
