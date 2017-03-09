all: speck-test

speck-test: speck.c speck.h
	$(CC) -O2 -DTEST -o $@ speck.c

clean:
	rm -f speck-test

.PHONY: all clean