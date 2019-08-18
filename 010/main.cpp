#include <stdio.h>
#include "debug/debug.h"
#include "net_interface/net_interface.h"

int main(void)
{
	debug_init();

	get_sockaddr("2001::1122");	

	g_debug_quit = true;
	while (!g_debug_quit) {
		pause();
	}
	printf("all clean up\n");
	return 0;
}

