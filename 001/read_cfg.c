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
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN	1024
int read_cfg_file(const char *file)
{
	char line[MAX_LINE_LEN] = {0};
	char name[MAX_LINE_LEN] = {0};
	char value[MAX_LINE_LEN] = {0};
    FILE *cfg_fd = NULL;

	cfg_fd = fopen(file, "r");
	if (NULL == cfg_fd){
		perror("open cfg file");
		return -1;
	}
	/*There is no need to memset line, A terminating null byte ('\0') is stored to line by fgets*/
	while (NULL != fgets(line, sizeof (line), cfg_fd)){
		/*There is no need to memset name and value, it include the terminating null byte by sscanf*/
		if (2 != sscanf(line, "%[^ #=\t\n]%*[= \t]%[^ #=\t\n]", name, value)){
			continue;
		}
		/*here do what you want*/
		/*you can assign name and value through function argument, global variable. struct and json are all OK*/

		printf("[%s]\n", name);
		printf("[%s]\n", value);
		printf("-----------\n");
	}

	fclose(cfg_fd);	
	return 0;
}
