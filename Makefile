server:
	gcc -c server.c  
	gcc -c client.c 
	gcc -c game.c
	gcc -c forserv.c
	gcc forserv.o server.o -o server
	gcc client.o game.o -o client
clean:
	rm server client game.o client.o forserv.o server.o
