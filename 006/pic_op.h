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
#ifndef PARSE_PATHNAME
#define PARSE_PATHNAME
#include <stdint.h>
#include <stdio.h>
/**
 *	open a image file
 *	@return 0 on success, a negative AVERROR on failure.
 */
int pic_open_path(const char *path);
/**
 *	open a image file
 *	@return 0 on success, a negative AVERROR on failure.
 */
int pic_open_data(const uint8_t *data, size_t data_size);

#endif
