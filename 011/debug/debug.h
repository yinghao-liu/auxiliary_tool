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
#ifndef _DEBUG_H_
#define _DEBUG_H_

// for pause()
#include <unistd.h>

#define DEBUG_QUIT_SIG SIGUSR1
/**
 * @brief A gloabal quit flag, if true, your thread should do clean-up
 *
 */
extern int g_debug_quit;

/**
 * @brief debug init, after that, if receive SIGUSR1, change g_debug_quit to true
 *
 * @return 0, success  1, error
 */
int debug_init(void);

#endif
