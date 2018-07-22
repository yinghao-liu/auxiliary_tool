#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>

#include "log.h"
#include "curl_builder.h"
#include "json/json.h"
#include "sha256/picosha2.h"
#include "encryption/md5.h"
using namespace std;
using namespace Json;
#define MACW_FILENAME	"macw-file.dat"

const string curl_t::content_type = "Content-Type: application/json";
const string curl_t::basic_url = "https://wifimacapi.getui.com/v1/";
//const string curl_t::basic_url = "http://www.asciima.com/v1/";
const string curl_t::account = "yuren2018";
const string curl_t::passwd  = "getui1234";

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




