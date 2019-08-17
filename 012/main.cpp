#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include "debug/debug.h"

int router_solicitation(const char *interface)
{
	int sock_fd;
	sock_fd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
	if (-1 == sock_fd) {
		perror("socket");	
		return -1;
	}
	uint8_t icmp[32] = {};
	uint8_t rt_sol[32] = {};
	int optval = 0;
	memset(icmp, 0xff, sizeof (icmp));
	memset(rt_sol, 0, sizeof (rt_sol));
	icmp[16] = 0xbf;
	setsockopt(sock_fd, SOL_ICMPV6, 1, icmp, 32);
	optval = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_DONTROUTE, &optval, 4);
	optval = 255;
	setsockopt(sock_fd, SOL_IPV6, IPV6_MULTICAST_HOPS, &optval, 4);
	optval = 255;
	setsockopt(sock_fd, SOL_IPV6, IPV6_UNICAST_HOPS, &optval, 4);
	optval = 1;
	setsockopt(sock_fd, SOL_IPV6, IPV6_RECVHOPLIMIT, &optval, 4);
	rt_sol[0] = 133; 
	struct sockaddr_in6 dest;
	dest.sin6_family = AF_INET6;
	dest.sin6_port = htons(0);
	inet_pton(AF_INET6, "ff02::2", &dest.sin6_addr);
	dest.sin6_flowinfo = htonl(0);
	dest.sin6_scope_id = if_nametoindex(interface);

	sendto(sock_fd, rt_sol, 8, 0, (const struct sockaddr *)&dest, 28);
	// sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
	return 0;
}

int main(void)
{
	debug_init();
	
	router_solicitation("ens37");
	g_debug_quit = true;
	while (!g_debug_quit) {
		pause();
	}
	printf("all clean up\n");
	return 0;
}

