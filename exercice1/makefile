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

all: consommateur producteur

%.o : %.c | obj
	gcc $(CFLAGS) -o $(OPATH)/$@ -c $< $(IFLAGS)

consommateur.o : consommateur.c
producteur.o : producteur.c

consommateur : consommateur.o | bin
	$(CC) $(CFLAGS) -o $(BPATH)/$@ $(OPATH)/consommateur.o

producteur : producteur.o | bin
	$(CC) $(CFLAGS) -o $(BPATH)/$@ $(OPATH)/producteur.o

obj :
	@mkdir -p $(OPATH)

bin :
	@mkdir -p $(BPATH)

clean :
	@rm -rf $(OPATH) $(BPATH)

archive :
	tar -cvzf $(shell basename `pwd`).tar.gz src include makefile
