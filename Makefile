# pisound-btn daemon for the Pisound button.
# Copyright (C) 2023  Vilniaus Blokas UAB, https://blokas.io/
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2 of the
# License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

-include Makefile.local

VERSION=1.0.0

PREFIX?=/usr/local

BINDIR?=$(PREFIX)/bin

CC?=cc

INSTALL?=install
INSTALL_PROGRAM?=$(INSTALL)
INSTALL_DATA?=$(INSTALL) -m 644

ifeq ($(DEBUG),yes)
	CFLAGS += -DDEBUG -g -O0
endif

all: pollgpio

pollgpio: pollgpio.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f pollgpio

install: pollgpio
	mkdir -p $(DESTDIR)$(BINDIR)
	$(INSTALL_PROGRAM) $^ $(DESTDIR)$(BINDIR)/

dist:
	git archive --format=tar.gz --prefix=pollgpio-$(VERSION)/ HEAD -o pollgpio-$(VERSION).tar.gz

.PHONY: clean
