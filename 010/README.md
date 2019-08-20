# 010
net interface

## Description
`get_sockaddr`  
get socket address
`get_ifaddr`  
get interface addresses (from getifaddrs(3))

## Note
`int socket(int domain, int type, int protocol);`  
The `protocol` argument, there are two means to get it:
### netinet_in.h(7)
search in file `netinet/in.h`, they are all prefixed by 'IPPROTO_'

### getprotobyname(3)
all the protocols are listed in `/etc/protocols`, see also protocols(5).  
then you can get the number by `getprotobyname`, but be careful, it is not thread safe.

## reference
1. man 2 getsockname
2. man 2 getpeername
3. man 2 recvfrom
4. man 3 getifaddrs
