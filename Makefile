server:
	gcc -c chat_serv.c
	gcc chat_serv.o -lpthread -o chat_serv
	gcc -c server.c  
	gcc -c forserv.c
	gcc forserv.o server.o -o $@
client:
	gcc -c chat_clnt.c
	gcc chat_clnt.o -lpthread -o chat_clnt
	gcc -c client.c
	gcc -c game.c
	gcc client.o game.o -o $@
clean:
	rm server client game.o client.o forserv.o server.o chat_clnt chat_serv chat_clnt.o chat_serv.o
