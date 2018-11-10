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
#include "parse_pathname.h"

string dirname(string path)
{
	size_t location = path.find_last_of('/');
	// eg. "/usr/"
	if (path.size() == location + 1){
		return path.substr(0, path.find_last_of('/', location-1)+1);
	}
	// there is no "/"
	if (string::npos == location){
		return ".";
	}
	// otherwise
	return path.substr(0, path.find_last_of('/'));
}

string basename(string path)
{
	size_t location = path.find_last_of('/');
	// just a "/"
	if (0 == location && 1 == path.size()){
		return "/";
	}
	// eg. "/usr/"
	if (path.size() == location + 1){
		return path.substr(path.find_last_of('/', location-1)+1, location-1);
	}
	// otherwise
	return path.substr(path.find_last_of('/')+1);
}
