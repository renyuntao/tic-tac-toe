server:
	gcc server.c -o server
	gcc -c client.c 
	gcc -c game.c
	gcc client.o game.o -o client
clean:
	rm server client game.o client.o
