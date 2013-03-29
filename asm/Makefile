CFLAGS = -DCONFIG_SMP

all: atomic

atomic: atomic.o
	gcc -o $@ $<

.c.o:
	gcc -Wall -g $(CFLAGS) -c -o $@ $<

clean:
	@rm atomic.o
	@rm atomic
