#
 # Copyright (c) 2019 Saulo Jorge Beltrao de Queiroz
 # Contact: Saulo Queiroz <ssaulojorge@gmail.com> <sauloqueiroz@utfpr.edu.br> <saulo@dei.uc.pt>
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation; either version 2 of the License, or
 # (at your option) any later version.
 # 
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 # 
 # You should have received a copy of the GNU General Public License
 # along with this program; if not, write to the Free Software 
 # Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 #
IDIR =./include
LIBDIR =./lib 
CC=g++
CFLAGS=-I$(IDIR) -L$(LIBDIR) -O2


all:
	$(CC) -o ./lib/mlut.o -c ./src/mlut.cc $(CFLAGS)
	$(CC) -o ./lib/ixs.o -c ./src/ixs.cc $(CFLAGS)
	$(CC) -o ./lib/immapper.o -c ./src/immapper.cc $(CFLAGS)
	$(CC) -o ./examples/ofdmim ./src/examples/ofdmim.cc lib/immapper.o lib/ixs.o lib/mlut.o $(CFLAGS)
	$(CC) -o ./examples/ieeeaccess ./src/examples/ieeeaccess.cc lib/immapper.o lib/ixs.o lib/mlut.o $(CFLAGS) -lboost_system -lboost_timer
	
clean:
	rm ./lib/* ; rm ./examples/ofdmim examples/ieeeaccess
