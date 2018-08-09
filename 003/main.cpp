#include "curl_builder.h"

int main(void)
{
	curl_t ascii;
	ascii.init();
	ascii.url_post("http://www.asciima.com/");
	return 0;
}
