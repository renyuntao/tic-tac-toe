#Synopsis

This is a little game based on Tic-Tac-Toe,and it is disigned for Linuxer.

#How to use this program?

If you just want to play the game ,what you need to do is compile it with

$ make client

and run as

$ ./client

I have already run the server as daemon in the remote host.

If you want to know how the program is running,you'd better to run the server and client at one computer,so you should first compile server with

$ make server

and run as

$ ./server

then modify the ’#define SERV_IP "157.7.53.149"‘  to  '#define SERV_IP "127.0.0.1"'  in 'game.h' file,and compile it with

$ make client

then run as

$ ./client

#Note

To play the game better,your computer should connect to the Internet.

*********************************** Chinese ************************************

#简介

这是一个基于井字棋的游戏，它为Linux用户而设计。

#如何使用这个程序？

如果你只是想玩游戏，你只要按如下步骤做就可以了：

编译：

$ make client

运行：

$ ./client

我已经在远程主机上将server程序作为守护进程运行了。

如果你想了解程序是如何运行的，你最好将服务器程序和客户端程序运行在同一台电脑上，因此你需要编译服务器程序:

$ make server

然后运行它:

$ ./server

接下来你需要在'game.h'文件中将'#define SERV_IP "157.7.53.149"' 修改为'#define SERV_IP "127.0.0.1"',

然后编译:

$ make client

运行:

$ ./client


#注意

为了能够更好的玩游戏，你的电脑需要连接到因特网。
