/*
 * Copyright (C) 2018 francis_hao <francis_hao@126.com>
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
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "log.h"
void *thread1(void *nul)
{
	for (int i=0; i<1000; i++) {
		log(log_level_t::ERROR, "--------------------%d\n", i);
		usleep(2);
	}
	return nullptr;
}
void *thread2(void *nul)
{
	for (int i=0; i<1000; i++) {
		log(log_level_t::ERROR, "++++++++++++++++++++%d\n", i);
		usleep(2);
	}
	return nullptr;
}
int main(void)
{
	/*
	const char *p = "--%s\n";
	printf(p, "printf message");
	//log(log_level_t::ERROR, p, "log message"); // compile error
	log(log_level_t::ERROR, "++++++++++++++++++++%s\n", "ddd");
	*/
	log_init(log_level_t::INFO, "main.log", 2);
	pthread_t p1;
	pthread_t p2;
	pthread_create(&p1, NULL, thread1, NULL);
	pthread_create(&p2, NULL, thread2, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	return 0;
}
