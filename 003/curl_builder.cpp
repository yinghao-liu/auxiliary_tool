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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include "curl_builder.h"

using namespace std;

curl_t::~curl_t()
{
	curl_easy_cleanup(curl_handle);	
}

int curl_t::init(string mac_w, string longitude, string latitude)
{
	curl_handle = curl_easy_init();
	if (NULL == curl_handle){
		return -1;
	}
	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
//	curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&buff);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, receive_data);
}

size_t curl_t::receive_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string *data = (string *)userdata;
	data->append(ptr, size*nmemb);
	return data->size();
}

int curl_t::url_post(const char *url, vector<string> &head, string &data)
{
	CURLcode code;

	curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	buff.clear();
	/*add head information*/
	struct curl_slist *slist=NULL;
	for (int i=0; i<head.size(); i++){
		slist = curl_slist_append(slist, head[i].c_str());
		curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);
	}
	/*post data*/
	if (0 == data.size()){
		struct curl_httppost* post = NULL;
		struct curl_httppost* last = NULL;
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "file",\
				CURLFORM_FILE, MACW_FILENAME, CURLFORM_END);
		curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
	}else{
		curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, (void*)data.c_str());
	}
	/*perform*/
	code = curl_easy_perform(curl_handle);
	if (0 != code){
		printf("curl_easy_perform error\n");
	}
	curl_slist_free_all(slist);
	return 0;
}

int curl_t::url_get(const char *url)
{
	CURLcode code;

	curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	buff.clear();
	/*add head information*/
	struct curl_slist *slist=NULL;
	slist = curl_slist_append(slist, "yinghao:francis");
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);
	/*perform*/
	code = curl_easy_perform(curl_handle);
	if (0 != code){
		printf("curl_easy_perform error\n");
	}
	curl_slist_free_all(slist);
	//cout<<buff<<endl;
	cout<<"buff len is "<<buff.size()<<endl;
	return 0;
}




