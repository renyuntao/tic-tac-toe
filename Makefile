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
clean_serv:
	rm server forserv.o server.o  chat_serv  chat_serv.o
clean_clnt:
	rm chat_clnt.o chat_clnt client.o game.o client
clean:
	rm chat_clnt.o chat_clnt client.o game.o client server forserv.o server.o  chat_serv  chat_serv.o
