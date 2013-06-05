udp_socket: mainfun.o
	gcc mainfun.o -o udp_socket -O2 -lpthread
	
mainfun.o : mainfun.c
	gcc -Wall -c mainfun.c -o mainfun.o

clean:
	rm -f *.o
	rm -f udp_socket
