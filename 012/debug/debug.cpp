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
#include <string.h>
#include <signal.h>
#include "debug.h"

int g_debug_quit = false;
/**
 * SIGUSR1 for normal return (not exit) process, that can do a memory detecte
 *
 */
void debug_sig_handle(int sig)
{
	printf("\n----%d----\n", sig);
	g_debug_quit = true;
}

int debug_init(void)
{
	int ret = -1;
	struct sigaction sig_act;
	memset(&sig_act, 0, sizeof (sig_act));
	sig_act.sa_handler = debug_sig_handle;
	ret = sigaction(DEBUG_QUIT_SIG, &sig_act, NULL);
	if (0 != ret) {
		perror("sigaction");
	}
	return ret;
}

