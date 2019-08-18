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
#include <stdlib.h>
#include <string.h>


#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "net_interface.h"

#if 0
/**
 * function unwrap in_port from sockaddr structure
 * depending on address family AF_INET or AF_INET6
 */
in_port_t get_in_port(const struct sockaddr *sa)
{
    if( sa->sa_family == AF_INET ) // IPv4 address
        return (((struct sockaddr_in*)sa)->sin_port);
    // else IPv6 address
    return (((struct sockaddr_in6*)sa)->sin6_port);
}

/**
 * function unwrap in_addr or in6_addr structure from
 * sockaddr structure depending on address family
 * AF_INET or AF_INET6
 */
void *get_in_addr(const struct sockaddr *sa) {

    if( sa->sa_family == AF_INET) // IPv4 address
        return &(((struct sockaddr_in*)sa)->sin_addr);
    // else IPv6 address
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * function unwrap ip address and port from sockaddr structure
 */
int get_address_and_port_from_sockaddr(const struct sockaddr *sockaddr, char **ip_address, int *port) {

    *ip_address = (char *) malloc(INET6_ADDRSTRLEN * sizeof(char));

    // converting network address to presentation address
    if(inet_ntop(sockaddr->sa_family, get_in_addr(sockaddr), *ip_address, INET6_ADDRSTRLEN * sizeof(char)) == NULL) {
        fprintf(stderr, "inet_ntop: %s\n", strerror(errno));
        return -1;
    }
    // converting network port to host port
    *port = ntohs(get_in_port(sockaddr));

    return 0;
}

/**
 * function retrieves current ip address and port
 * socket is bound to for given socket file descriptor
 */
int get_current_address_and_port(int sockfd, char **ip_address, int *port)
{

    struct sockaddr_storage address = {0};
    socklen_t sockaddrlen = sizeof(address);

    if(0 != getsockname(sockfd, (struct sockaddr*)(&address), &sockaddrlen)) {
        fprintf(stderr, "getsockname: %s\n", strerror(errno));
        return -1;
    }

    return get_address_and_port_from_sockaddr((struct sockaddr*)(&address), ip_address, port);
}
/**
 * function retrieves peer ip address and port
 * socket is connected to for given socket file descriptor
 */
int get_peer_address_and_port(int sockfd, char **ip_address, int *port) {

    struct sockaddr sockaddr;
    socklen_t sockaddrlen = sizeof(sockaddr);

    if(0 != getpeername(sockfd, &sockaddr, &sockaddrlen)) {
        fprintf(stderr, "getpeername: %s\n", strerror(errno));
        return -1;
    }

    return get_address_and_port_from_sockaddr(&sockaddr, ip_address, port);
}

/**
 * function unwrap ip address and port from addrinfo structure
 */
int get_address_and_port_from_addrinfo(const struct addrinfo *addrinfo, char **ip_address, int *port) {

    return get_address_and_port_from_sockaddr((struct sockaddr *)addrinfo->ai_addr, ip_address, port);
}

int print_socket_address(int sockfd, socket_type_t socket_type)
{

    char *ip_address; 	// address (passive) socket was binded to
    int port; 			// port (passive) socket was binded to

    switch(socket_type)
    {
        case PASSIVE_SOCKET:
            if(0 != get_current_address_and_port(sockfd, &ip_address, &port)) {
                fprintf(stderr, "get_current_address_and_port: faild!\n");
                free(ip_address);
                return -1;
            }
            printf("Created passive socket %d binded to %s:%d\n", sockfd, ip_address, port);
            break;
        case CONNECTION_SOCKET:
            if(0 != get_peer_address_and_port(sockfd, &ip_address, &port)) {
                fprintf(stderr, "get_peer_address_and_port: faild!\n");
                free(ip_address);
                return -1;
            }
            printf("Socket %d connected to %s:%d\n", sockfd, ip_address, port);
            break;
        default:
            fprintf(stderr, "Incorrect socket type!\n");
            free(ip_address);
            return -1;
    }

    free(ip_address);
    return 0;
}
#endif

/**
 *	@brief v4&v6, negtive&postive
 */
int get_sockaddr(const char *peer)
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
		sockaddr->sin6_port = htons(53);
		sockaddr->sin6_scope_id = 0;
		if (1 != inet_pton(sockaddr->sin6_family, peer, &sockaddr->sin6_addr)) {
			fprintf(stderr, "inet_pton: %s\n", strerror(errno));
		}

		sockaddr = (struct sockaddr_in6 *)&local_addr;
		ntop_src = &sockaddr->sin6_addr;
	} else if (NULL != strstr(peer, ".")) { // v4
		struct sockaddr_in *sockaddr;
		sockaddr = (struct sockaddr_in *)&peer_addr;
		
		sockaddr->sin_family = AF_INET;
		sockaddr->sin_port = htons(53);
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
