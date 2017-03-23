all: test

test:
	$(CC) -O2 -Os -o $@ test.c speck32.c speck64.c speck128.c

clean:
	rm -f test *.obj *.o

.PHONY: all clean