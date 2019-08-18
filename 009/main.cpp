#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/uv.h"

#define DEFAULT_PORT 24
#define DEFAULT_BACKLOG 16

const char *home_page = R"(
Welcome to interactable log system

 * Documentation:  https://github.com/yinghao-liu/auxiliary_tool/tree/master/009

)";

uv_loop_t *g_loop;
uv_buf_t write_buf;
void on_write(uv_write_t* req, int status)
{
	printf("on_write:%d\n", status);	
	free(req);
}

void process_request(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	printf("echo read, nread: %zd, buf->len:%zu\n", nread, buf->len);
	if (0 != nread && nullptr != buf->base)
	printf("[%s]\n", buf->base);
	uv_write_t *req = (uv_write_t *)malloc(sizeof (uv_write_t));
	write_buf = uv_buf_init((char *)malloc(nread), nread-1);
	strncpy(write_buf.base, "aa login:", write_buf.len);
	uv_write(req, stream, &write_buf, 1, on_write);
	free(buf->base);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	*buf = uv_buf_init((char *)malloc(suggested_size), suggested_size);
	if (nullptr != buf->base) {
		memset(buf->base, 0, buf->len);
	}
	printf("alloc_buffer:%zd\n", suggested_size);
}

void on_new_connection(uv_stream_t* server, int status)
{
	printf("on_new_connection: %d\n", status);	
	uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
	uv_tcp_init(g_loop, client);
	if (uv_accept(server, (uv_stream_t*) client) == 0) {
		write_buf = uv_buf_init((char *)malloc(10), 9);
		strncpy(write_buf.base, "aa login:", 9);
		uv_write_t *req = (uv_write_t *)malloc(sizeof (uv_write_t));
		int ret = uv_write(req, (uv_stream_t*)client, &write_buf, 1, on_write);
		printf("%s\n", uv_strerror(ret));
		uv_read_start((uv_stream_t*) client, alloc_buffer, process_request);
	}
}

int main(void ) 
{
	g_loop = uv_default_loop();

	uv_tcp_t server;
	uv_tcp_init(g_loop, &server);
	
	int ret;
	 struct sockaddr_in addr;
	ret = uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	ret = uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	if (0 != ret) {
		printf("%s\n", uv_strerror(ret));
	}
	ret = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
	if (0 != ret) {
		fprintf(stderr, "Listen error %s\n", uv_strerror(ret));
		return -1;
	}

	printf("run...\n");
	uv_run(g_loop, UV_RUN_DEFAULT);
	printf("after uv_run\n");
	uv_loop_close(uv_default_loop());
	return 0;
}
