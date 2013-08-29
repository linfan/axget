# Using unit test framework check (http://check.sourceforge.net)

OBJS = TestConn.o TestMain.o

all: $(OBJS)
	gcc -o axget_test -Llib -lcheck -laxget -Wl,-rpath,lib/ $^

%.o: %.c
	gcc -c -I.. -Iinclude $<

clean:
	rm -f axget_test *.o
