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
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <iostream>
#include "log.h"
using std::string;

static string g_log_file;
static log_level_t g_log_level=DEFAULT_LOG_LEVEL;
static FILE *g_log_fd=NULL;
static int g_max_mb=10;
static const char *g_map[]={"FATAL", "ERROR", "WARN", "INFO", "DEBUG"};
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//int log_init(level_t log_level=FATAL, const char *log_file=NULL, uint16_t max_mb=10);
int log_init(log_level_t log_level, const char *log_file, uint16_t max_mb)
{
	if (NULL != log_file) {
		if (0 != g_log_file.size() && NULL != g_log_fd) {
			fclose(g_log_fd);
		}
		g_log_file = log_file;
		g_log_fd = fopen(log_file, "a");
		/*setvbuf if need*/
		if (NULL == g_log_fd) {
			perror("open log file");
			return -1;
		}
	}
	g_log_level = log_level;// there is no need to check it
	g_max_mb = max_mb;
	return 0;
}

int check_file_size(void)
{
	struct stat file_stat;
	static size_t max_size = 1024 * 1024 * g_max_mb;
	if (-1 == stat(g_log_file.c_str(), &file_stat)) {
		perror("stat");
		return -1;
	}
	if (file_stat.st_size < max_size) {
		return 0;
	}

	fclose(g_log_fd);//The fclose() function flushes the stream pointed to by g_log_fd
	g_log_fd = NULL;
	if (-1 == stat(g_log_file.c_str(), &file_stat)) {
		perror("stat");
		return -1;
	}

	size_t half_point;
	size_t len;
	half_point = file_stat.st_size/2;
	len = file_stat.st_size - half_point;
	int fd_log = -1 ;
	int fd_tmp = -1;
	char tmp_file[]="tmpXXXXXX";
	int ret = -1;
	fd_log = open(g_log_file.c_str(), O_RDWR);
	if (-1 == fd_log) {
		perror("open file");
		goto ending;
	}
	fd_tmp = mkstemp(tmp_file);
	if (-1==fd_tmp) {
		perror("mkstemp");
		goto ending;
	}
	unlink(tmp_file);

	off_t offset;
	offset = half_point;
	if (-1 == sendfile(fd_tmp, fd_log, &offset, len)) {
		perror("sendfile");
		goto ending;
	}
	if (-1 == ftruncate(fd_log, 0)) {
		perror("ftruncate");
		goto ending;
	}
	offset = 0;
	if (-1 == sendfile(fd_log, fd_tmp, &offset, len)) {
		perror("sendfile");
		goto ending;
	}
	ret = 0;
ending:
	close(fd_log);
	close(fd_tmp);
	if (-1 == log_init(g_log_level, g_log_file.c_str(), g_max_mb)) {
		return -1;
	}
	return ret;
}

void __log(log_level_t level, const char *s, ...)
{
	if (level > g_log_level) {
		return;
	}
	time_t now_t;
	struct tm now_m;
	char str[32]= {0};// 32 is enough for string like "03-14 14:02:40 [FATAL] "
	now_t = time(NULL);
	localtime_r(&now_t, &now_m);
	strftime(str, sizeof (str) - 1, "%m-%d %H:%M:%S ", &now_m);
	sprintf(str+strlen(str), "[%s] ", g_map[(uint8_t)level]);

	va_list ap; 
	va_start(ap, s);
	pthread_mutex_lock(&mutex);
	if (NULL != g_log_fd) {
		fprintf(g_log_fd, "%s", str);
		vfprintf(g_log_fd, s, ap);
		check_file_size();
	} else {
		printf("%s", str);
		vprintf(s, ap);
	}
	pthread_mutex_unlock(&mutex);
	va_end(ap);
}


