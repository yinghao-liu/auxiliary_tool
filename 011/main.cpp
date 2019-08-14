#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include "debug/debug.h"

#include <stdlib.h>
int tail_f(const char *file_name)
{
	int ret = -1;

	ret = access(file_name, F_OK);
	if (0 != ret) {
		printf("access %s:%s\n", file_name, strerror(errno));
		return -1;
	}

	int nty_fd = -1;
	int watch_fd = -1;
	int epoll_fd = -1;
	struct epoll_event ep_evin;
	struct epoll_event ep_evout;
	int fds_num = 0;
	int fd = -1;
	ssize_t read_len;
	struct stat file_stat;
	size_t file_len = 0;
	char buff[1024] = {};

	nty_fd = inotify_init();
	if (-1 == nty_fd) {
		printf("inotify_init error :%s\n", strerror(errno));
		return -1;
	}
	watch_fd = inotify_add_watch(nty_fd, file_name, IN_MODIFY);
	if (-1 == watch_fd) {
		printf("inotify_add_watch error :%s\n", strerror(errno));
		ret = -1;
		goto ending;
	}
	printf("nty_fd:%d, watch_fd:%d\n", nty_fd, watch_fd);

	epoll_fd = epoll_create(1);
	if (-1 == epoll_fd) {
		printf("epoll_create :%s\n", strerror(errno));
		ret = -1;
		goto ending;
	}

	//ep_evin.events = EPOLLIN | EPOLLET;
	ep_evin.events = EPOLLIN;
	ep_evin.data.fd = nty_fd; //for ep_evout, the value is given to ep_evout.data.fd
	ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, nty_fd, &ep_evin);
	if (-1 == ret) {
		printf("epoll_ctl error :%s\n", strerror(errno));
		ret = -1;
		goto ending;
	}

    ret = stat(file_name, &file_stat);
	if (0 != ret) {
		printf("stat %s :%s\n", file_name, strerror(errno));
		ret = -1;
		goto ending;
	}
	file_len = file_stat.st_size;

	fd = open(file_name, O_RDONLY);
	if (-1 == fd) {
		printf("open file error :%s\n", strerror(errno));
		ret = -1;
		goto ending;
	}

	printf("%s size : %zu\n", file_name, file_len);
	while (true) {
		while (true) {
			read_len = read(fd, buff, sizeof (buff) -1);
			if (read_len > 0) {
				buff[read_len] = '\0';
				printf("%s", buff);
			} else {
				//error or end of file
				break;
			}
		}

		//start watch
		fds_num = epoll_wait(epoll_fd, &ep_evout, 5, -1);
		printf("events is %x\n", ep_evout.events);
		if (-1 == fds_num) {
			printf("epoll_wait : %s\n", strerror(errno));
			// interrupted by a signal handler
			if (EINTR == errno && g_debug_quit) {
				break;	
			}
		} else if (0 == fds_num) {
			printf("timeout\n");
		}
		if (nty_fd != ep_evout.data.fd) {
			printf("not the right fd : %d\n", ep_evout.data.fd);
			//return -1;
		}
		malloc(1024);
		printf("ep_evout %#x\n", ep_evout.events);
		struct inotify_event nty_ev;
		read_len = read(ep_evout.data.fd, &nty_ev, sizeof (nty_ev));
	}
	printf("end loop\n");
	malloc(1024);
	ret = 0;
ending:
	if (-1 != fd) {
		close(fd);
	}
	if (-1 != epoll_fd) {
		close(epoll_fd);
	}
	if (nty_fd != -1) {
		if (watch_fd != -1) {
			inotify_rm_watch(nty_fd, watch_fd);
		}
		close(nty_fd);
	}
	return ret;
}

int main(void)
{
	debug_init();
	tail_f("log.log");
	
	while (!g_debug_quit) {
		pause();
	}
	printf("all clean up\n");
	return 0;
}
