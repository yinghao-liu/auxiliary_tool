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
#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
#include <stdint.h>

/*
 * the second argument must be the const string like "aabbcc%s", can't be a char* pointer
 * there is another way to resolve that, but you should malloc memory.
 */
#define log(level, s, ...)  \
	    __log(level, "%s:%u(%s): "s, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

enum level_t{
	FATAL = 0,
	ERROR,
	WARN,
	INFO,
	DEBUG
};

int log_init(level_t log_level=FATAL, const char *log_file=NULL, uint16_t max_mb=10);

/*don't use this function directly, use macro log instead*/
void __log(level_t level, const char *s, ...);
#endif
