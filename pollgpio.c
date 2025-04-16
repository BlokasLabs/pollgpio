/*
 * pisound-btn daemon for the Pisound button.
 * Copyright (C) 2023  Vilniaus Blokas UAB, https://blokas.io/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>

static void print_version()
{
	printf("pollgpio version 1.0.0 © Blokas https://blokas.io/\n");
}

static void print_usage()
{
	printf(
		"pollgpio usage:\n"
		"\n"
		"\tpollgpio <path>             Path to GPIO value file to monitor\n"
		"\n"
		"\t--help                      Print this help\n"
		"\t--version                   Print version\n"
		"\n"
		);
	print_version();
}

int main(int argc, char **argv)
{
	for (int i=1; i<argc; ++i)
	{
		if (strncmp(argv[i], "--help", 7) == 0)
		{
			print_usage();
			return 0;
		}
		else if (strncmp(argv[i], "--version", 10) == 0)
		{
			print_version();
			return 0;
		}
	}

	if (argc != 2)
	{
		print_usage();
		return 1;
	}

	int n, fd;
	char data[100];
	memset(data, 0, sizeof(data));
	struct pollfd pfd;

	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "Unable to open '%s' for reading. (%d)\n", argv[1], errno);
		return 1;
	}

	n = read(fd, data, sizeof(data)-1);
	fputs(data, stdout);

	pfd.fd = fd;
	pfd.events = POLLPRI;

	for (;;)
	{
		n = poll(&pfd, 1, -1);
		if (n <= 0)
			break;

		if (pfd.revents & POLLPRI)
		{
			memset(data, 0, sizeof(data));
			lseek(fd, 0, SEEK_SET);
			n = read(fd, data, sizeof(data)-1);
			if (n > 0)
				fputs(data, stdout);
			else break;
		}
	}

	close(fd);
	return 0;
}
