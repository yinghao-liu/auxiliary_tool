#ifndef _CURL_BUILDER_
#define _CURL_BUILDER_
#include <mqueue.h>
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
using std::vector;
using std::string;

class curl_t{
public:
	~curl_t();
	int init(string mac_w, string longitude, string latitude);	
	int url_get(const char *url);
	int url_post(const char *url, vector<string> &head, string &data);
protected:
	static size_t receive_data(char *ptr, size_t size, size_t nmemb, void *userdata);

private:
	CURL *curl_handle = NULL;
	string buff;
	const static string content_type;
	const static string basic_url;

	
};

#endif
