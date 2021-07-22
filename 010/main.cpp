#include <stdio.h>
#include "debug/debug.h"
#include "net_interface/net_interface.h"

int main(int argc, char *argv[])
{
	debug_init();

	//get_sockaddr("fe80::20c:29ff:fee7:2186", "ens37");
	//get_sockaddr("fe80::20c:29ff:ee7:2186");
	//get_sockaddr("2001::2321", "ens37");
	//get_ifaddr();
	addrinfo(argv[1]);
	//name_info();
	//addrinfo_obsolete("kdkdcslfvnlk.com");
	g_debug_quit = true;
	while (!g_debug_quit) {
		pause();
	}
	printf("all clean up\n");
	return 0;
}

