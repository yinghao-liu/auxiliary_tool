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
			sockaddr->sin6_scope_id = if_nametoindex(interface);
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
	strncpy(ifr.ifr_name, interface, strlen(interface));
	setsockopt(sock_fd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));
	if (0 != ioctl(sock_fd, SIOCGIFINDEX, &ifr)) {
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
	int ret;
	struct ifaddrs *ifaddr, *ifa;
	int family;
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
/************************************clumsy way to get ip string*****************************************/
		if (AF_INET == family || AF_INET6 == family) {
			char ipaddr[64] = {};
			if (AF_INET == family) {
				struct sockaddr_in *sockaddr;
				sockaddr = (struct sockaddr_in *)ifa->ifa_addr;
				if (NULL == inet_ntop(family, &sockaddr->sin_addr, ipaddr, sizeof (ipaddr))) {
					fprintf(stderr, "inet_ntop: %s\n", strerror(errno));
				}
			} else {
				struct sockaddr_in6 *sockaddr;
				sockaddr = (struct sockaddr_in6 *)ifa->ifa_addr;
				if (NULL == inet_ntop(family, &sockaddr->sin6_addr, ipaddr, sizeof (ipaddr))) {
					fprintf(stderr, "inet_ntop: %s\n", strerror(errno));
				}
			}
			printf("ipaddr:%s\n", ipaddr);

/*************************************elegant way to get ip string****************************************/
			//if family is AF_INET6, and ip is link ip, this means can get a string that ip bind eth interface
			//eg fe80::20c:29ff:1122:2233%ens33
			ret = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
					host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (ret != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(ret));
				freeifaddrs(ifaddr);
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

/**
 * get index and name of all interface
 */
int if_nameindex_demo(void)
{
	struct if_nameindex *index = NULL;
	struct if_nameindex *index_tmp = NULL;
	index = if_nameindex();
	if (NULL == index) {
		fprintf(stderr, "if_nameindex err:%s\n", strerror(errno));
		return -1;
	}
	for (index_tmp = index; ((NULL != index_tmp->if_name) || (0 != index_tmp->if_index)); index_tmp++) {
		printf("if_index:%d, if_name:%s\n", index_tmp->if_index, index_tmp->if_name);
	}   
	if_freenameindex(index);
}


int name_info(struct sockaddr *addr)
{
	int ret = -1;
	socklen_t addrlen;
	if (AF_INET == addr->sa_family) {
		addrlen = sizeof (sockaddr_in);
	} else if (AF_INET6) {
		addrlen = sizeof (sockaddr_in6);
	} else {
		printf("ai_family not supported\n");
	}
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	ret = getnameinfo(addr, addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
	if (ret == 0) {
		printf("----host=%s, serv=%s\n", hbuf, sbuf);
	} else {
		printf("%s\n", gai_strerror(ret));
	}
	return 0;
}


int addrinfo(const char *domain)
{
	int ret = -1;
	struct addrinfo *result = nullptr;
	struct addrinfo *next = nullptr;
	ret = getaddrinfo("baidu.com", nullptr, nullptr, &result);
	if (0 != ret) {
		printf("%s\n", gai_strerror(ret));	
	}
	next = result;
	while (nullptr != next) {
		printf("ai_family: %d\n", next->ai_family);
		printf("ai_canonname: %s\n", next->ai_canonname);
		printf("ai_canonname: %s\n", inet_ntoa(((sockaddr_in *)next->ai_addr)->sin_addr));
		name_info(next->ai_addr);
		next = next->ai_next;
	}
	freeaddrinfo(result);

	return 0;
}


/*
 * POSIX.1-2008 removes the specifications of gethostbyname(), gethostbyaddr(), and h_errno, 
 * recommending the use of getaddrinfo(3) and getnameinfo(3) instead
 */
int addrinfo_obsolete(const char *domain)
{
	struct hostent *host = NULL;
	host = gethostbyname(domain);
	if (!host) {
		printf("Get IP address error! %s\n", hstrerror(h_errno));
		return -1;
	}
	printf("host name:%s\n", host->h_name);
	for (int i=0; host->h_aliases[i]; i++) {
		printf("Aliases %d: %s\n", i, host->h_aliases[i]);
	}
	printf("Address type: %s\n", (host->h_addrtype==AF_INET) ? "AF_INET": "AF_INET6");
	for (int i=0; host->h_addr_list[i]; i++) {
		printf("IP addr %d: %s\n", i, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	return 0;
}
