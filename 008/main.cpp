/*
 * Copyright (C) 2019 francis_hao <francis_hao@126.com>
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
#include <iostream>
#include <string.h>

#include <stdlib.h>

#include <stdio.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

int filter(const struct dirent *filter_dirent)
{
	if ((0 == strcmp(filter_dirent->d_name, ".")) || (0 == strcmp(filter_dirent->d_name, ".."))) {
		return 0;
	}
	return 1;
}

/**
 * get_node_size
 * @param[in]  path node path
 * @param[out] size total size of path
 *
 */
int get_node_size(const char *path, size_t *size)
{
	size_t total_size = 0;
	struct stat node_stat;
	if (0 != stat(path, &node_stat)) {
		perror("stat");
		return -1;
	}

	if (S_ISREG(node_stat.st_mode)) { //a regular file
		*size = node_stat.st_size;
		printf("%s:  %zu\n", path, *size);
		return 0;
	} else if (S_ISDIR(node_stat.st_mode)) { //directory
		int file_num;
		struct dirent **file_list;
		file_num = scandir(path, &file_list, filter, alphasort);
		if (-1 == file_num) {
			perror("scandir");
			return -1;
		}
		int ret;
		size_t node_size = 0;
		string node_path = path;
		if ('/' != node_path.back()) {
			node_path.push_back('/');
		}
		while (file_num--) {
			ret = get_node_size((node_path+file_list[file_num]->d_name).c_str(), &node_size);
			if (0 == ret) {
				total_size += node_size;
			}
			free(file_list[file_num]);
		}
		free(file_list);
	} else { //others, now are not supported
		return -1;
	}

	*size = total_size;
	return 0;

}

int main(int argc, char *argv[])
{
	size_t sizee;
	get_node_size("../../auxiliary_tool/", &sizee);
	printf("total size is %zu KB\n", sizee/1024);
	printf("total size is %zu MB\n", sizee/1024/1024);
	return 0;
}

