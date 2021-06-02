# 010
net interface

## Description
`get_sockaddr`  
get socket address, use `getsockname` mainly, It returns  the  current address to which the socket sockfd is bound.

`get_ifaddr`  
get interface addresses (from getifaddrs(3)), use `getnameinfo` for converts a socket 
address to a corresponding host and service.

`getaddrinfo`
do what likes DNS do, network address and service translation

`if_nameindex`
get index and name of all interface

## Note
`int socket(int domain, int type, int protocol);`  
The `protocol` argument, there are two means to get it:
### netinet_in.h(7)
search in file `netinet/in.h`, they are all prefixed by 'IPPROTO_'

### getprotobyname(3)
all the protocols are listed in `/etc/protocols`, see also protocols(5).  
then you can get the number by `getprotobyname`, but be careful, it is not thread safe.

others are examples in man

## reference
1. man 2 getsockname
2. man 2 getpeername
3. man 2 recvfrom
4. man 3 getifaddrs
5. /etc/iproute2/rt_scopes
6. man 3 getnameinfo
7. man 3 getaddrinfo
8. man 3 if_nameindex *get network interface names and indexes*
