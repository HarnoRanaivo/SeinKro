CC = gcc
CFLAGS = -Wall -g -pedantic -std=gnu99 -pthread
IFLAGS = -Iinclude

SPATH = src
OPATH = obj
BPATH = bin
IPATH = include

vpath %.c $(SPATH)/
vpath %.h $(IPATH)/
vpath %.o $(OPATH)/

main : main.o common.o | bin
	$(CC) $(CFLAGS) -o $(BPATH)/$@ $(OPATH)/main.o $(OPATH)/common.o

%.o : %.c | obj
	gcc $(CFLAGS) -o $(OPATH)/$@ -c $< $(IFLAGS)

common.o : common.c common.h
main.o : main.c common.h temps.h

obj :
	@mkdir -p $(OPATH)

bin :
	@mkdir -p $(BPATH)

clean :
	@rm -rf $(OPATH) $(BPATH)

archive :
	tar -cvzf $(shell basename `pwd`).tar.gz src include makefile
