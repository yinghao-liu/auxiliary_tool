#include <stdio.h>
#include <stdint.h>
#include <endian.h>

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

// part of rtp protocol for test
struct rtp_protocol {
# if __BYTE_ORDER == __LITTLE_ENDIAN
	uint32_t csrc_ct : 4;
	uint32_t extension : 1;
	uint32_t padding : 1;
	uint32_t version : 2;
# elif __BYTE_ORDER == __BIG_ENDIAN
	uint32_t version : 2;
	uint32_t padding : 1;
	uint32_t extension : 1;
	uint32_t csrc_ct : 4;
# else
#  error "Adjust your <bits/endian.h> defines"
# endif

# if __BYTE_ORDER == __LITTLE_ENDIAN
	uint32_t payload : 7;
	uint32_t marker : 1;
# else
	uint32_t marker : 1;
	uint32_t payload : 7;
# endif
	uint32_t sequence_num : 16;
};

int send_udp_data(int sockfd, const char *addr, uint16_t port, const char *data, size_t len)
{
	struct sockaddr_in peer_addr;
	memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addr.sin_family = AF_INET;
	peer_addr.sin_port = htons(port);
	inet_aton(addr, &peer_addr.sin_addr);
	ssize_t ret = 0;
	ret = sendto(sockfd, data, len, 0, (const struct sockaddr *)&peer_addr, sizeof (peer_addr));
	if (-1 == ret) {
		perror("sendto");
	}
	return ret;
}
int main(int argc, char *argv[])
{
	int client_sock;
	client_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == client_sock) {
		perror("socket");
		return -1;
	}

	struct rtp_protocol rtp = {};
	rtp.version = 2;
	rtp.padding = 0;
	rtp.extension = 0;
	rtp.csrc_ct = 0;
	rtp.marker = 0;
	rtp.payload = 96;
	rtp.sequence_num = htons(30241); // 0x7621

	send_udp_data(client_sock, "127.0.0.1", 980, (const char *)&rtp, sizeof (rtp));
	close(client_sock);
	return 0;
}

