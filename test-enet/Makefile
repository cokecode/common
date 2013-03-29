CFLAGS = -Wall -DDEBUG -g -I$(HOME)/opt/include
LDFLAGS = -L$(HOME)/opt/lib -lenet

all: enet_server enet_client

enet_server: enet_server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

enet_client: enet_client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -f enet_server enet_client

z: clean rsync all

rsync:
	rsync -e ssh -r -v . cyb@192.168.10.4:src/test-enet/

.PHONY: all clean z rsync
