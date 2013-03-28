CFLAGS = -Wall -g -DDEBUG
LDFLAGS = -lm

all: udp-intr

udp-intr: udp-intr.o
	$(CC) -o $@ $< $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@$(RM) udp-intr.o
	@$(RM) udp-intr
