################################
##      Makefile for Axget    ##
##                            ##
## Copyright 2001-2012 Lintux ##
## Copyright 2013      Freyr  ##
################################

# Using unit test framework check (http://check.sourceforge.net)

OBJS = TestConn.o TestMain.o

all: $(OBJS)
	gcc -o axget_test -Llib -lcheck -laxget -Wl,-rpath,lib/ $^
	./axget_test

%.o: %.c
	gcc -c -I../src -Iinclude -fPIC $<

clean:
	rm -f axget_test *.o

