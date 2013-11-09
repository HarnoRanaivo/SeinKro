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

main : main.o commun.o societe.o tampon.o | bin
	$(CC) $(CFLAGS) -o $(BPATH)/$@ $(OPATH)/main.o $(OPATH)/commun.o $(OPATH)/societe.o $(OPATH)/tampon.o

%.o : %.c | obj
	gcc $(CFLAGS) -o $(OPATH)/$@ -c $< $(IFLAGS)

tampon.o : tampon.c tampon.h commun.h limite.h
societe.o : societe.c societe.h commun.h limite.h tampon.h
commun.o : commun.c commun.h limite.h
main.o : main.c commun.h limite.h societe.h tampon.h

obj :
	@mkdir -p $(OPATH)

bin :
	@mkdir -p $(BPATH)

clean :
	@rm -rf $(OPATH) $(BPATH)

archive :
	tar -cvzf $(shell basename `pwd`).tar.gz src include makefile
