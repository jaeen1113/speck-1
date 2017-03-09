all: test

test: speck64.c speck128.c speck.h
	$(CC) -O2 -Os -o $@ test.c

clean:
	rm -f test

.PHONY: all clean