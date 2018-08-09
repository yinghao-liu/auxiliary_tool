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
	if (NULL != curl_handle){
		curl_easy_cleanup(curl_handle);
	}
	curl_global_cleanup();
}

int curl_t::init()
{
	if (0 != curl_global_init(CURL_GLOBAL_ALL)){
		return -1;	
	}
	curl_handle = curl_easy_init();
	if (NULL == curl_handle){
		return -1;
	}
	//curl_easy_setopt(curl_handle, CURLOPT_DEBUGFUNCTION, debug_info);
	//curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
	//curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, &err_msg);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&buff);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, receive_data);
}


int curl_t::url_post(const char *url, const vector<string> &head, const string &data)
{
	curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	buff.clear();
	if (NULL == curl_handle){
		cout<<"error------"<<endl;
	}
	/*add head information*/
	struct curl_slist *slist=NULL;
	for (int i=0; i<head.size(); i++){
		slist = curl_slist_append(slist, head[i].c_str());
		curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);
	}

	/*post data*/
	curl_mime *mime=nullptr;
	curl_mimepart *part=nullptr;
	if (0 == data.size()){
		mime = curl_mime_init(curl_handle);
		part = curl_mime_addpart(mime);
		curl_mime_data(part, "This is the field data", CURL_ZERO_TERMINATED);
		curl_mime_name(part, "data");
		part = curl_mime_addpart(mime);
		curl_mime_data(part, "francis", CURL_ZERO_TERMINATED);
		curl_mime_name(part, "name");
		curl_easy_setopt(curl_handle, CURLOPT_MIMEPOST, mime);
	}else{
		curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, (void*)data.c_str());
	}

	/*perform*/
	CURLcode code;
	code = curl_easy_perform(curl_handle);
	if (0 != code){
		printf("curl_easy_perform error\n");
	}
	curl_slist_free_all(slist);
	curl_mime_free(mime);//mime is NULL is OK  
	cout<<"buff len is "<<buff.size()<<endl;
	return (0==code)?0:-1;
}

int curl_t::url_get(const char *url)
{
	curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	buff.clear();
	/*add head information*/
	struct curl_slist *slist=NULL;
	slist = curl_slist_append(slist, "name:francis");
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);

	/*perform*/
	CURLcode code;
	code = curl_easy_perform(curl_handle);
	if (0 != code){
		printf("curl_easy_perform error\n");
		printf("%s\n", err_msg);
	}
	curl_slist_free_all(slist);
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, NULL);
	//cout<<buff<<endl;
	cout<<"buff len is "<<buff.size()<<endl;
	return (0==code)?0:-1;
}

size_t curl_t::receive_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string *data = (string *)userdata;
	data->append(ptr, size*nmemb);
	return size*nmemb;
}

void curl_t::dump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
{
	size_t i;
	size_t c;
	unsigned int width=0x10;

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
			text, (long)size, (long)size);

	for(i=0; i<size; i+= width) {
		fprintf(stream, "%4.4lx: ", (long)i);

		/* show hex to the left */
		for(c = 0; c < width; c++) {
			if(i+c < size)
				fprintf(stream, "%02x ", ptr[i+c]);
			else
				fputs("   ", stream);
		}

		/* show data on the right */
		for(c = 0; (c < width) && (i+c < size); c++) {
			char x = (ptr[i+c] >= 0x20 && ptr[i+c] < 0x80) ? ptr[i+c] : '.';
			fputc(x, stream);
		}

		fputc('\n', stream); /* newline */
	}
}

int curl_t::debug_info(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	const char *text=NULL;
	switch (type) {
		case CURLINFO_TEXT:
			fprintf(stderr, "== Info: %s", data);
		case CURLINFO_HEADER_OUT:
			text = "=> Send header";
			break;
		case CURLINFO_DATA_OUT:
			text = "=> Send data";
			break;
		/*case CURLINFO_SSL_DATA_OUT:
			text = "=> Send SSL data";
			break;*/
		case CURLINFO_HEADER_IN:
			text = "<= Recv header";
			break;
		case CURLINFO_DATA_IN:
			text = "<= Recv data";
			break;
		/*case CURLINFO_SSL_DATA_IN:
			text = "<= Recv SSL data";
			break;*/
		default: /* in case a new one is introduced to shock us */
			return 0;

	}
	dump(text, stderr, (unsigned char *)data, size);
	return 0;
}
