# 003 
libcurl

## 主要函数说明
`#include <curl/curl.h>`  
`CURLcode curl_global_init(long flags);`  
Global libcurl initialisation, This  function  is  not thread safe. In normal operation, The `flags` option must be specified as CURL_GLOBAL_ALL.

`CURL *curl_easy_init();`  
Start a libcurl easy session, you'd better call `curl_global_init` before call this function.

`void curl_easy_cleanup(CURL *handle);`  
End a libcurl easy handle

`void curl_global_cleanup(void);`  
global libcurl cleanup, This function is not thread safe. You must not call it when any other thread in the program is running.

`CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);`  
set options for a curl easy handle, this is the most used function.

`void curl_easy_reset(CURL *handle);`  
reset all options of a libcurl session handle

`CURLcode curl_easy_perform(CURL *easy_handle);`  
perform a blocking file transfer

## 辅助函数说明
`curl_version_info_data *curl_version_info( CURLversion age);`  
gets detailed libcurl (and other used libraries) version info

`CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, ... );`  
get information about a performed transfer, such as CURLINFO_EFFECTIVE_URL(Last used URL), CURLINFO_RESPONSE_CODE(Last received response code),
CURLINFO_TOTAL_TIME(Total time of previous transfer), CURLINFO_NAMELOOKUP_TIME(Time from start until name resolving completed), CURLINFO_CONNECT_TIME
(Time from start until remote host or proxy completed)...

`CURLFORMcode curl_formadd(struct curl_httppost ** firstitem, struct curl_httppost ** lastitem, ...);`  
add a section to a multipart/formdata HTTP POST. about this form, refer to *libcurl_curl_formadd.pcap*

`void curl_formfree(struct curl_httppost *form);`  
free a previously build multipart/formdata HTTP POST chain, that is used to clean up data previously built/appended with curl_formadd(3). This must be called when the data has been used, which typically means after curl_easy_perform(3) has been called.

`struct curl_slist *curl_slist_append(struct curl_slist *list, const char * string);`  
add a string to an slist.

`void curl_slist_free_all(struct curl_slist *list);`  
free an entire curl_slist list.

## 弃用函数的替代
`curl_formadd` and `curl_formfree` are deprecated  in 7.56.0, should use curl_mime_init(3) instead.

`curl_mime * curl_mime_init(CURL * easy_handle);`  
create a mime handle, `curl_mime_free` must be called  when the data has been used, which typically means after curl_easy_perform(3) has been called.

`void curl_mime_free(curl_mime *mime);`  
free a previously built mime structure.

`curl_mimepart * curl_mime_addpart(curl_mime * mime);`  
append a new empty part to a mime structure

`CURLcode curl_mime_name(curl_mimepart * part, const char * name);`  
set a mime part's name

`CURLcode curl_mime_data(curl_mimepart * part, const char * data , size_t datasize);`  
set a mime part's body data from memory

## reference
- [The libcurl API](https://curl.se/libcurl/c/)
