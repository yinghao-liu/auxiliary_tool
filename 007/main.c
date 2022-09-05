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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int split(const char *str, const char *delim, const char *subdelim)
{
	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int j;

	if (NULL == delim || NULL == str) {
		return -1;
	}
	size_t str_len = strlen(str);
	char *str_shd = (char *)malloc(str_len);
	if (NULL == str_shd) {
		perror("malloc");
		return -1;
	}
	memcpy(str_shd, str, str_len);
	/*
	 * On the first call to strtok(), the string to be parsed should be specified in str.
	 * In each subsequent call that should parse the same string, str must be NULL.
	 */
	for (j = 1, str1 = str_shd; ; j++, str1 = NULL) {
		token = strtok_r(str1, delim, &saveptr1);
		if (token == NULL)
			break;
		printf("%d: %s\n", j, token);
		if (NULL == subdelim) {
			continue;
		}
		for (str2 = token; ; str2 = NULL) {
			subtoken = strtok_r(str2, subdelim, &saveptr2);
			if (subtoken == NULL)
				break;
			printf(" --> %s\n", subtoken);
		}
	}
	free(str_shd);
	return 0;
}

int split_with_empty(const char *str, const char *delim)
{
	size_t str_len = strlen(str);
	char *str_shd = (char *)malloc(str_len);
	if (NULL == str_shd) {
		perror("malloc");
		return -1;
	}
	memcpy(str_shd, str, str_len);

	char *token = NULL;
	while (NULL != str_shd){
		token = strsep(&str_shd, delim);
		printf("after token:%p, [%s]\n", token, token);
		printf("after str_shd:%p, [%s]\n", str_shd, str_shd);
		printf("----------------\n");
	}
}

int main(int argc, char *argv[])
{
	//char aa[] = "1-90,22-3,77";
	char aa[] = ";try;;;;0;0;0;0;";
	printf("before %s\n", aa);
	//split(aa, ";", NULL);
	//split(NULL, ",", NULL);
	split_with_empty(aa, ";");
	printf("after %s\n", aa);

	return 0;
}

