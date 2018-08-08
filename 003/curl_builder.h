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
#ifndef _CURL_BUILDER_
#define _CURL_BUILDER_
//#include <mqueue.h>
//#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

class curl_t{
public:
	~curl_t();
	int init(void);	
	int url_get(const char *url);
	int url_post(const char *url, const std::vector<std::string> &head={}, const std::string &data="");

protected:
	static size_t receive_data(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int debug_info(CURL *handle, curl_infotype type, char *data, size_t size, void *userp);
	static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size);

private:
	CURL *curl_handle = NULL;
	std::string buff;
};

#endif
