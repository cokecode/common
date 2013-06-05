CFLAGS = -Wall -g

SRCS = $(wildcard *.c)
PROGRAMS = $(SRCS:.c=)
DSYMS = $(SRCS:.c=.dSYM)

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)
	rm -rf $(DSYMS)

