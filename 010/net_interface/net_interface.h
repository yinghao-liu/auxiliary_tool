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
/** @file */
#ifndef _NET_INTERFACE_
#define _NET_INTERFACE_

/**
 * get local ip which connected to peer through local route
 * @param[in] peer: peer ip address v4 or v6
 * @param[in] interface: local net interface, eg. "eth0", null is default
 * @note when ipv6 link address(fe80::/64) is used, interface must be specified
 */
int get_sockaddr(const char *peer, const char *interface = nullptr);
/**
 * get interface addr and print
 */
int get_ifaddr(void);
/**
 * get addr info through domain like baidu.com, using getaddrinfo inside
 * @param[in] domain: hostname
 */
int addrinfo(const char *domain);
/**
 * get addr info through domain like baidu.com, using getostbyname inside which is deprecated
 * @param[in] domain: hostname
 */
int addrinfo_obsolete(const char *domain);

#endif
