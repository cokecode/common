X5 = print-zip-version zip-pipe

all: $(X5)

%: %.c
	gcc -Wall -g -o $@ $< -lz

clean:
	rm -f $(X5)
