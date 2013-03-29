CC = gcc
CFLAGS = -Wall -m32
LDFLAGS = -Wall -m32

all: hello

hello: hello.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) hello hello.o
