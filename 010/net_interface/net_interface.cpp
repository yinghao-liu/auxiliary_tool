/*
 * Copyright (C) 2019 francis_hao <francis_hao@126.com>
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
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "net_interface.h"


/**
 *	@brief v4&v6
 */
int get_sockaddr(const char *peer, const char *interface)
{
	int sock_fd = -1;
    socklen_t sockaddr_len = 0;
	void *ntop_src = NULL;
    struct sockaddr_storage local_addr = {};
    struct sockaddr_storage peer_addr = {};

	if (NULL != strstr(peer, ":")) { // v6
		struct sockaddr_in6 *sockaddr;
		sockaddr = (struct sockaddr_in6 *)&peer_addr;
		sockaddr->sin6_family = AF_INET6;
		sockaddr->sin6_port = htons(0);
		sockaddr->sin6_scope_id = 0;
		if (nullptr != interface) {
			sockaddr->sin6_scope_id = if_nametoindex("ens37");
			fprintf(stdout, "%u\n", sockaddr->sin6_scope_id);
		}
		if (1 != inet_pton(sockaddr->sin6_family, peer, &sockaddr->sin6_addr)) {
			fprintf(stderr, "inet_pton: %s\n", strerror(errno));
		}

		sockaddr = (struct sockaddr_in6 *)&local_addr;
		ntop_src = &sockaddr->sin6_addr;
	} else if (NULL != strstr(peer, ".")) { // v4
		struct sockaddr_in *sockaddr;
		sockaddr = (struct sockaddr_in *)&peer_addr;
		
		sockaddr->sin_family = AF_INET;
		sockaddr->sin_port = htons(0);
		if (1 != inet_pton(sockaddr->sin_family, peer, &sockaddr->sin_addr)) {
			fprintf(stderr, "inet_pton: %s\n", strerror(errno));
		}

		sockaddr = (struct sockaddr_in *)&local_addr;
		ntop_src = &sockaddr->sin_addr;
	} else {
		printf("not support address  %s\n", peer);
		return -1;
	}


	sock_fd = socket(peer_addr.ss_family, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == sock_fd) {
		perror("socket");
		return -1;
	}

	/*struct ifreq ifr;
	memset(&ifr, 0x00, sizeof(ifr));
	strncpy(ifr.ifr_name, "ens37", strlen("ens37"));*/
	//setsockopt(sock_fd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));

	/*if (0 != ioctl(sock_fd, SIOCGIFINDEX, &ifr)) {
		perror("ioctl:");	
	}*/

    sockaddr_len = sizeof (peer_addr);
	if (0 != connect(sock_fd, (const struct sockaddr*)&peer_addr, sockaddr_len)) {
        fprintf(stderr, "connect: %s\n", strerror(errno));
        return -1;
	}

    if(0 != getsockname(sock_fd, (struct sockaddr*)(&local_addr), &sockaddr_len)) {
        fprintf(stderr, "getsockname: %s\n", strerror(errno));
        return -1;
    }
	char local_ip[64] ={};

	printf("%s\n", inet_ntop(local_addr.ss_family, ntop_src, local_ip, sizeof (local_ip)));
	return 0;
}


int get_ifaddr(void)
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];

	if (-1 == getifaddrs(&ifaddr)) {
		perror("getifaddrs");
		return -1;
	}


	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (NULL == ifa->ifa_addr) {
			continue;
		}

		family = ifa->ifa_addr->sa_family;


		printf("%-8s %s (%d)\n",
				ifa->ifa_name,
				(family == AF_PACKET) ? "AF_PACKET" :
				(family == AF_INET) ? "AF_INET" :
				(family == AF_INET6) ? "AF_INET6" : "???",
				family);

		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
					sizeof(struct sockaddr_in6),
					host, NI_MAXHOST,
					NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return -1;
			}

			printf("\t\taddress: <%s>\n", host);

		} else if (family == AF_PACKET && ifa->ifa_data != NULL) {
			struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

			printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
					"\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
					stats->tx_packets, stats->rx_packets,
					stats->tx_bytes, stats->rx_bytes);
		}
	}
	freeifaddrs(ifaddr);
	return 0;
}

