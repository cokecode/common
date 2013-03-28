all: inet-pton

inet-pton: inet-pton.o
	gcc -Wall -g -o $@ $< -Wl,--strip-all

.c.o:
	gcc -Wall -g -c -o $@ $<

clean:
	@rm -f inet-pton.o inet-pton
