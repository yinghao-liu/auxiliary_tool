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

